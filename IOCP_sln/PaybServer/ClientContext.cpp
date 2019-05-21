#include "stdafx.h"
#include "ClientContext.h"
#include "SocketApi.h"

// �ͻ��˳�ʱʱ��
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

  // ����ǳ�ʱ�ˣ����ټ����Ȱ�
  if(WAIT_TIMEOUT == dwErr)
  {
    if ( !IsClientAlive() )
    {
      IOCP_ErrorLog("��⵽�ͻ���ʧ����");
//      return IOCP_RemoveClient;
    }
    else
    {
      IOCP_ErrorLog("���������ʱ��������...");
      return IOCP_OK;
    }
  }
  // �����ǿͻ����쳣�˳���
  else if( ERROR_NETNAME_DELETED == dwErr )
  {
    IOCP_ErrorLog("��⵽�ͻ����쳣�˳���");
//    CSocketApi::DisconnectEx(m_sktClient, &m_Overlapped, TF_REUSE_SOCKET, 0);
//    return IOCP_RemoveClient;
  }
  else
  {
    IOCP_ErrorLog( "��ɶ˿ڲ������ִ���:" + CStringUtil::FormatLastError() );
//    return IOCP_KO;
  }
  return IOCP_RemoveClient;
}

bool CClientContext::NeedRemove(DWORD dwBytesTransfered)
{
  //  ���ǵ�һ����,����û�нӵ�����
	if(0 == dwBytesTransfered &&  OP_ACCEPT != m_OpType)
	{
	  tstringstream ss;
	  ss << "�ͻ���:" << inet_ntoa(m_ClientAddr.sin_addr)
	     << ":" << ntohs(m_ClientAddr.sin_port) << " �Ͽ�����.";
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
			// Ϊ�����Ӵ���ɶ��ԣ�������ר�ŵ�DoAccpet�������д�����������
			nRes = DoAccpetEx(hIocp);
		}
		break;

	case OP_WRITE:
		{
			// Ϊ�����Ӵ���ɶ��ԣ�������ר�ŵ�DoRecv�������д����������
			nRes = DoRecv();
		}
		break;

	// �����Թ���д�ˣ�Ҫ������̫���ˣ���������⣬Send�������������һЩ
	case OP_READ:
		{
		  //nRes = IOCP_RemoveClient;
    	// Ȼ��ʼͶ����һ��WSARecv����
    	nRes = DoSend();
		}
		break;
	default:
		// ��Ӧ��ִ�е�����
		IOCP_ErrorLog("m_OpType �����쳣.");
		nRes = IOCP_RemoveClient;
		break;
	}
	return nRes;
}

// Ͷ��Accept����
int CClientContext::PostAcceptEx(SOCKET sktServer)
{
  ZeroStaticMemory(m_szBuffer);
  BOOST_ASSERT( INVALID_SOCKET != sktServer );
  m_OpType = OP_ACCEPT;  

  // ׼������
  DWORD dwBytes = 0;  

  // Ͷ��AcceptEx
  if(!CSocketApi::AcceptEx( sktServer
    , m_sktClient
    , m_WsaBuf.buf

    // ��������Ϊ0������ֹDDoS���� 
    // �ο� http://www.cnblogs.com/BeginGame/archive/2011/09/22/2185164.html
    , m_WsaBuf.len - ( (sizeof(SOCKADDR_IN) + 16) * 2 )
    , sizeof(SOCKADDR_IN) + 16
    , sizeof(SOCKADDR_IN) + 16
    , &dwBytes
    , &m_Overlapped))
  {  
    if(WSA_IO_PENDING != ::WSAGetLastError())  
    {
      IOCP_ErrorLog( "Ͷ��AcceptExʧ��:" + CStringUtil::FormatLastError() );
      return IOCP_RemoveClient;  
    }  
  }

  // ����ͻ��˼�������WSARecv��WSASend
  // ����ǵ���AcceptEX���յ����� �����ø�ѡ��Ļ�������shutdown���ý�����ʧ��
  // http://www.cnblogs.com/wlmbz/p/4919246.html
  ::setsockopt(m_sktClient, SOL_SOCKET, SO_UPDATE_ACCEPT_CONTEXT
      , (char *)&sktServer, sizeof(SOCKET));
      
  return IOCP_OK;
}

// ���пͻ��������ʱ�򣬽��д���
// �����е㸴�ӣ���Ҫ�ǿ������Ļ����Ϳ����׵��ĵ���....
// ������������Ļ�����ɶ˿ڵĻ������������һ�����
// ��֮��Ҫ֪�����������ListenSocket��Context��������Ҫ����һ�ݳ������������Socket��
// ԭ����Context����Ҫ���������Ͷ����һ��Accept����
int CClientContext::DoAccpetEx(HANDLE hIocp)
{
  IOCP_WriteLog("------DoAccpetEx()------");
	SOCKADDR_IN* ClientAddr = 0;
	SOCKADDR_IN* LocalAddr  = 0;  
	int nRemoteLen = sizeof(SOCKADDR_IN);
	int nLocalLen  = sizeof(SOCKADDR_IN);  

	// ����ȡ������ͻ��˵ĵ�ַ��Ϣ
	// ��� GetAcceptExSockAddrs �����˰�~~~~~~
	// ��������ȡ�ÿͻ��˺ͱ��ض˵ĵ�ַ��Ϣ������˳��ȡ���ͻ��˷����ĵ�һ�����ݣ���ǿ����...
	CSocketApi::GetAcceptExSockAddrs(m_WsaBuf.buf
			, m_WsaBuf.len - ((sizeof(SOCKADDR_IN) + 16) * 2)
			, sizeof(SOCKADDR_IN) + 16
			, sizeof(SOCKADDR_IN) + 16
			, (LPSOCKADDR*)&LocalAddr
			, &nLocalLen
			, (LPSOCKADDR*)&ClientAddr
			, &nRemoteLen);

  tstringstream ss;
  ss << "�ͻ��� " << inet_ntoa(ClientAddr->sin_addr) 
    << ":" << ntohs(ClientAddr->sin_port) << " ����:0x" << (DWORD)this;
  IOCP_WriteLog( ss.str() );
  ss.str("");

  ss << "DoAccpetEx �յ���Ϣ��" << m_WsaBuf.buf;
  IOCP_WriteLog( ss.str() );
  
	memcpy(&m_ClientAddr, ClientAddr, sizeof(SOCKADDR_IN));

	// �����ںͿͻ���ͨ�ŵ�SOCKET�󶨵���ɶ˿���
	HANDLE hTemp = CreateIoCompletionPort((HANDLE)m_sktClient, hIocp, (DWORD)this, 0);

	// ����������ϣ������Socket����ɶ˿ڰ�(��Ҳ��һ���ؼ�����)
	if(hTemp != hIocp )
	{
	  IOCP_ErrorLog( "SOCKET�󶨵���ɶ˿�ʧ��:" + CStringUtil::FormatSocketError() );
		return IOCP_RemoveClient;
	}  

	// 3. �������������µ�IoContext�����������Socket��Ͷ�ݵ�һ��Recv��������
	// ���Buffer��Ҫ���������Լ�����һ�ݳ���
	//memcpy( pNewIoContext->m_szBuffer,pIoContext->m_szBuffer,MAX_BUFFER_LEN );

	// �����֮�󣬾Ϳ��Կ�ʼ�����Socket��Ͷ�����������
	IOCP_Verify( PostSend() );

	// 5. ʹ�����֮��,Ȼ��׼��Ͷ���µ�AcceptEx
	return IOCP_NewClient;
}

// Ͷ�ݽ����������� SO_RCVBUF
int CClientContext::PostRecv()
{
  IOCP_WriteLog("------PostRecv()------");
	// ��ʼ������
	int nHead = (sizeof(SOCKADDR_IN) + 16) * 2;
//	ZeroStaticMemory(m_szBuffer);
//  ZeroDynamicMemory(m_szBuffer + nHead, sizeof(m_szBuffer) - nHead);
	m_OpType = OP_WRITE;
	
	DWORD dwFlags = 0;
	DWORD dwBytes = 0;

	// ��ʼ����ɺ󣬣�Ͷ��WSARecv����
	int nBytesRecv = ::WSARecv( m_sktClient, &m_WsaBuf, 1
			, &dwBytes, &dwFlags, &m_Overlapped, NULL );

	// �������ֵ���󣬲��Ҵ���Ĵ��벢����Pending�Ļ����Ǿ�˵������ص�����ʧ����
	if ((SOCKET_ERROR == nBytesRecv) && (WSA_IO_PENDING != ::WSAGetLastError()))
	{
		IOCP_ErrorLog( "Ͷ�ݵ�һ��WSARecvʧ��:" + CStringUtil::FormatLastError() );
		return IOCP_RemoveClient;
	}
	return IOCP_OK;
}

// ���н��յ����ݵ����ʱ�򣬽��д���
int CClientContext::DoRecv()
{
  IOCP_WriteLog("------DoRecv()------");
	// �Ȱ���һ�ε�������ʾ���֣�Ȼ�������״̬��������һ��Recv����
	SOCKADDR_IN* ClientAddr = &m_ClientAddr;
	tstringstream ss;
	ss << "�յ�  " << inet_ntoa(ClientAddr->sin_addr)
	   << ":" << ntohs(ClientAddr->sin_port);
	IOCP_WriteLog( ss.str() );
	ss.str("");
	ss << "PostRecv �յ���Ϣ��" << m_WsaBuf.buf;
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
  IOCP_WriteLog("�����Ӧ��:" + strAck);
  
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

// ��ֹDDoS����
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
  	  ss << "�ͻ���:" << inet_ntoa(m_ClientAddr.sin_addr)
  	     << ":" << ntohs(m_ClientAddr.sin_port) << " ��ʱ.";
  	  IOCP_WarnLog( ss.str() );
  	  return true;
	  }
	}
	return false;
}

