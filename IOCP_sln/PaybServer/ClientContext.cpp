#include "stdafx.h"
#include "ClientContext.h"
#include "SocketApi.h"

// 客户端超时时间
const static int SOCKET_ACCEPT_TIMEOUT = 300; //seconds
static volatile long g_nCountResponse = 0;

CClientContext* GetClientContext(OVERLAPPED* pOverlapped)
{
  CClientContext* pClientContext = 0;
  if (pOverlapped != 0)
  {
    pClientContext = CONTAINING_RECORD(pOverlapped, CClientContext, m_Overlapped);
  }
	return pClientContext;
}

CClientContext::CClientContext(SOCKET sktClient) : m_sktClient(sktClient)
{
  ZeroStaticMemory(m_Overlapped);
  ZeroStaticMemory(m_szBuffer);
  ZeroStaticMemory(m_ClientAddr);
  m_WsaBuf.buf = m_szBuffer;
  m_WsaBuf.len = BUFFER_SIZE;
  m_OpType     = OP_DUMMY;
  m_dwSendSequenceNumber = 0;
}

CClientContext::~CClientContext()
{
  if( m_sktClient != INVALID_SOCKET )
  {
    ::shutdown(m_sktClient, SD_BOTH);
    ::closesocket(m_sktClient);
    m_sktClient = INVALID_SOCKET;
  }
}

bool CClientContext::IsClientAlive()
{
  return ( SOCKET_ERROR != ::send(m_sktClient, "", 0, 0) );
}

int CClientContext::HandleIocpError()
{
  DWORD dwErr = ::GetLastError();

  // 如果是超时了，就再继续等吧
  if(WAIT_TIMEOUT == dwErr)
  {
    if ( !IsClientAlive() )
    {
      IOCP_ErrorLog("检测到客户端失联！");
//      return IOCP_RemoveClient;
    }
    else
    {
      IOCP_ErrorLog("网络操作超时！重试中...");
      return IOCP_OK;
    }
  }
  // 可能是客户端异常退出了
  else if( ERROR_NETNAME_DELETED == dwErr )
  {
    IOCP_ErrorLog("检测到客户端异常退出！");
//    CSocketApi::DisconnectEx(m_sktClient, &m_Overlapped, TF_REUSE_SOCKET, 0);
//    return IOCP_RemoveClient;
  }
  else
  {
    IOCP_ErrorLog( "完成端口操作出现错误:" + CStringUtil::FormatLastError() );
//    return IOCP_KO;
  }
  return IOCP_RemoveClient;
}

bool CClientContext::NeedRemove(DWORD dwBytesTransfered)
{
  //  不是第一次连,并且没有接到数据
	if(0 == dwBytesTransfered &&  OP_ACCEPT != m_OpType)
	{
	  tstringstream ss;
	  ss << "客户端:" << inet_ntoa(m_ClientAddr.sin_addr)
	     << ":" << ntohs(m_ClientAddr.sin_port) << " 断开连接.";
	  IOCP_ErrorLog( ss.str() );

//	  CSocketApi::DisconnectEx(m_sktClient, &m_Overlapped, TF_REUSE_SOCKET, 0);
	  return true;
	}
	return false;
}

int CClientContext::HandleOpertion(HANDLE hIocp)
{
  int nRes = IOCP_OK;
  
  switch( m_OpType )
	{  
	case OP_ACCEPT:
		{ 
			// 为了增加代码可读性，这里用专门的DoAccpet函数进行处理连入请求
			nRes = DoAccpetEx(hIocp);
		}
		break;

	case OP_WRITE:
		{
			// 为了增加代码可读性，这里用专门的DoRecv函数进行处理接收请求
			nRes = DoRecv();
		}
		break;

	// 这里略过不写了，要不代码太多了，不容易理解，Send操作相对来讲简单一些
	case OP_READ:
		{
		  //nRes = IOCP_RemoveClient;
    	// 然后开始投递下一个WSARecv请求
    	nRes = DoSend();
		}
		break;
	default:
		// 不应该执行到这里
		IOCP_ErrorLog("m_OpType 参数异常.");
		nRes = IOCP_RemoveClient;
		break;
	}
	return nRes;
}

// 投递Accept请求
int CClientContext::PostAcceptEx(SOCKET sktServer)
{
  ZeroStaticMemory(m_szBuffer);
  BOOST_ASSERT( INVALID_SOCKET != sktServer );
  m_OpType = OP_ACCEPT;  

  // 准备参数
  DWORD dwBytes = 0;  

  // 投递AcceptEx
  if(!CSocketApi::AcceptEx( sktServer
    , m_sktClient
    , m_WsaBuf.buf

    // 本参数不为0不能阻止DDoS攻击 
    // 参考 http://www.cnblogs.com/BeginGame/archive/2011/09/22/2185164.html
    , m_WsaBuf.len - ( (sizeof(SOCKADDR_IN) + 16) * 2 )
    , sizeof(SOCKADDR_IN) + 16
    , sizeof(SOCKADDR_IN) + 16
    , &dwBytes
    , &m_Overlapped))
  {  
    if(WSA_IO_PENDING != ::WSAGetLastError())  
    {
      IOCP_ErrorLog( "投递AcceptEx失败:" + CStringUtil::FormatLastError() );
      return IOCP_RemoveClient;  
    }  
  }

  // 允许客户端继续调用WSARecv和WSASend
  // 如果是调用AcceptEX接收的连接 不设置该选项的话，随后的shutdown调用将返回失败
  // http://www.cnblogs.com/wlmbz/p/4919246.html
  ::setsockopt(m_sktClient, SOL_SOCKET, SO_UPDATE_ACCEPT_CONTEXT
      , (char *)&sktServer, sizeof(SOCKET));
      
  return IOCP_OK;
}

// 在有客户端连入的时候，进行处理
// 流程有点复杂，你要是看不懂的话，就看配套的文档吧....
// 如果能理解这里的话，完成端口的机制你就消化了一大半了
// 总之你要知道，传入的是ListenSocket的Context，我们需要复制一份出来给新连入的Socket用
// 原来的Context还是要在上面继续投递下一个Accept请求
int CClientContext::DoAccpetEx(HANDLE hIocp)
{
  IOCP_WriteLog("------DoAccpetEx()------");
	SOCKADDR_IN* ClientAddr = 0;
	SOCKADDR_IN* LocalAddr  = 0;  
	int nRemoteLen = sizeof(SOCKADDR_IN);
	int nLocalLen  = sizeof(SOCKADDR_IN);  

	// 首先取得连入客户端的地址信息
	// 这个 GetAcceptExSockAddrs 不得了啊~~~~~~
	// 不但可以取得客户端和本地端的地址信息，还能顺便取出客户端发来的第一组数据，老强大了...
	CSocketApi::GetAcceptExSockAddrs(m_WsaBuf.buf
			, m_WsaBuf.len - ((sizeof(SOCKADDR_IN) + 16) * 2)
			, sizeof(SOCKADDR_IN) + 16
			, sizeof(SOCKADDR_IN) + 16
			, (LPSOCKADDR*)&LocalAddr
			, &nLocalLen
			, (LPSOCKADDR*)&ClientAddr
			, &nRemoteLen);

  tstringstream ss;
  ss << "客户端 " << inet_ntoa(ClientAddr->sin_addr) 
    << ":" << ntohs(ClientAddr->sin_port) << " 连入:0x" << (DWORD)this;
  IOCP_WriteLog( ss.str() );
  ss.str("");

  ss << "DoAccpetEx 收到信息：" << m_WsaBuf.buf;
  IOCP_WriteLog( ss.str() );
  
	memcpy(&m_ClientAddr, ClientAddr, sizeof(SOCKADDR_IN));

	// 将用于和客户端通信的SOCKET绑定到完成端口中
	HANDLE hTemp = CreateIoCompletionPort((HANDLE)m_sktClient, hIocp, (DWORD)this, 0);

	// 参数设置完毕，将这个Socket和完成端口绑定(这也是一个关键步骤)
	if(hTemp != hIocp )
	{
	  IOCP_ErrorLog( "SOCKET绑定到完成端口失败:" + CStringUtil::FormatSocketError() );
		return IOCP_RemoveClient;
	}  

	// 3. 继续，建立其下的IoContext，用于在这个Socket上投递第一个Recv数据请求
	// 如果Buffer需要保留，就自己拷贝一份出来
	//memcpy( pNewIoContext->m_szBuffer,pIoContext->m_szBuffer,MAX_BUFFER_LEN );

	// 绑定完毕之后，就可以开始在这个Socket上投递完成请求了
	IOCP_Verify( PostSend() );

	// 5. 使用完毕之后,然后准备投递新的AcceptEx
	return IOCP_NewClient;
}

// 投递接收数据请求 SO_RCVBUF
int CClientContext::PostRecv()
{
  IOCP_WriteLog("------PostRecv()------");
	// 初始化变量
	int nHead = (sizeof(SOCKADDR_IN) + 16) * 2;
//	ZeroStaticMemory(m_szBuffer);
//  ZeroDynamicMemory(m_szBuffer + nHead, sizeof(m_szBuffer) - nHead);
	m_OpType = OP_WRITE;
	
	DWORD dwFlags = 0;
	DWORD dwBytes = 0;

	// 初始化完成后，，投递WSARecv请求
	int nBytesRecv = ::WSARecv( m_sktClient, &m_WsaBuf, 1
			, &dwBytes, &dwFlags, &m_Overlapped, NULL );

	// 如果返回值错误，并且错误的代码并非是Pending的话，那就说明这个重叠请求失败了
	if ((SOCKET_ERROR == nBytesRecv) && (WSA_IO_PENDING != ::WSAGetLastError()))
	{
		IOCP_ErrorLog( "投递第一个WSARecv失败:" + CStringUtil::FormatLastError() );
		return IOCP_RemoveClient;
	}
	return IOCP_OK;
}

// 在有接收的数据到达的时候，进行处理
int CClientContext::DoRecv()
{
  IOCP_WriteLog("------DoRecv()------");
	// 先把上一次的数据显示出现，然后就重置状态，发出下一个Recv请求
	SOCKADDR_IN* ClientAddr = &m_ClientAddr;
	tstringstream ss;
	ss << "收到  " << inet_ntoa(ClientAddr->sin_addr)
	   << ":" << ntohs(ClientAddr->sin_port);
	IOCP_WriteLog( ss.str() );
	ss.str("");
	ss << "PostRecv 收到信息：" << m_WsaBuf.buf;
	IOCP_WriteLog( ss.str() );

  return PostRecv();
//  return PostSend();
}

// SO_SNDBUF
int CClientContext::PostSend()
{
  IOCP_WriteLog("------PostSend()------");
  m_OpType = OP_READ;
  
  ::InterlockedIncrement(&g_nCountResponse);
  tstringstream ss;
  ss << "0x" << (DWORD)this << " OK:" << g_nCountResponse;
  tstring strAck( ss.str() );
  IOCP_WriteLog("服务端应答:" + strAck);
  
  memcpy( m_szBuffer, (char*)strAck.c_str(), strAck.length() );
  m_szBuffer[ strAck.length() ] = 0;

  DWORD dwSendNumBytes = 0;
  ULONG ulFlags = MSG_PARTIAL;
  int nRetVal = ::WSASend(m_sktClient, 
				&m_WsaBuf, 1, &dwSendNumBytes, 
				ulFlags, &m_Overlapped, NULL);

  DWORD dwError = ::WSAGetLastError();
  if ( SOCKET_ERROR == nRetVal && dwError != WSA_IO_PENDING )
  {
    if(dwError != WSAENOTSOCK)
    {
      IOCP_ErrorLog( "Error in PostSend:" + CStringUtil::FormatLastError() );
    }
    return IOCP_RemoveClient;
  }
  else
  {
    ::InterlockedIncrement(&m_dwSendSequenceNumber);
  }
  return IOCP_OK;
}

int CClientContext::DoSend()
{
  IOCP_WriteLog("------DoSend()------");
  return IOCP_RemoveClient;
}

// 防止DDoS攻击
bool CClientContext::IsTimeOut()
{
  int nSeconds;
	int nBytes = sizeof(nSeconds);
	int nErr = ::getsockopt( m_sktClient, SOL_SOCKET, SO_CONNECT_TIME,
			(char*)&nSeconds, (int*)&nBytes );

	if (nErr != NO_ERROR)
	{
	  if (nSeconds >= SOCKET_ACCEPT_TIMEOUT)
	  {
  	  tstringstream ss;
  	  ss << "客户端:" << inet_ntoa(m_ClientAddr.sin_addr)
  	     << ":" << ntohs(m_ClientAddr.sin_port) << " 超时.";
  	  IOCP_WarnLog( ss.str() );
  	  return true;
	  }
	}
	return false;
}

