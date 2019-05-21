#include "stdafx.h"
#include "IocpServer.h"
#include "SocketApi.h"
#include <boost/bind.hpp>
#include <boost/function.hpp>

CIocpServer::CIocpServer()
{
  m_nThreadRunning = true;
  m_nWorkThreads = 0;
  m_hIocp = NULL;
  m_bInitialized = false;
  m_pBarrier = 0;
  m_hShutdownEvent = NULL;
}

CIocpServer::~CIocpServer()
{
	m_hIocp = NULL;
	m_bInitialized = false;
  delete m_pBarrier;
}

int CIocpServer::Start(const tchar *pszAddr, int nPort)
{
	if (m_bInitialized)
	{
		return IOCP_OK;
	}
	
	// 建立系统退出的事件通知
	m_hShutdownEvent = CreateEvent(NULL, TRUE, FALSE, NULL);

	//获取CPU数目
	SYSTEM_INFO SysInfo;
	GetSystemInfo(&SysInfo);
  m_nWorkThreads = 2 * (int)SysInfo.dwNumberOfProcessors;
	
	// 创建完成端口对象
	m_hIocp = ::CreateIoCompletionPort( INVALID_HANDLE_VALUE, NULL, 0, m_nWorkThreads );

  if (NULL == m_hIocp)
  {
	  IOCP_ErrorLog( "创建完成端口失败:" + CStringUtil::FormatLastError() );
		return IOCP_KO;
  }
  IOCP_Verify( InitServerSocket(pszAddr, nPort) );

	// 创建监听线程
	m_pBarrier = new boost::barrier(m_nWorkThreads + 1);

	// 创建工作线程处理完成端口对象的事件
	for (int i = 0; i < m_nWorkThreads; i++)
	{
		m_ThreadPool.create_thread(boost::bind(&CIocpServer::ThreadIocpService, this));
	}
	m_pBarrier->wait();
	m_bInitialized = true;
	return IOCP_OK;
}

int CIocpServer::InitServerSocket(const tchar *pszAddr, int nPort)
{
	// 需要使用重叠IO，必须得使用WSASocket来建立Socket，才可以支持重叠IO操作
  m_sktServer = ::WSASocket(AF_INET, SOCK_STREAM, 0, NULL, 0, WSA_FLAG_OVERLAPPED);
  
	if (INVALID_SOCKET == m_sktServer)
	{
    IOCP_ErrorLog( "WSASocket错误:" + CStringUtil::FormatLastError() );
		return IOCP_KO;
	}
  IOCP_Verify( CSocketApi::GetSocketApi(m_sktServer) );

	// 将Listen Socket绑定至完成端口中
	if( m_hIocp != ::CreateIoCompletionPort( (HANDLE)m_sktServer, m_hIocp, (DWORD)0, 0) )
	{
	  IOCP_ErrorLog( "WSASocket错误:" + CStringUtil::FormatLastError() );
	  return IOCP_KO;
	}

	// 服务器地址信息，用于绑定Socket
	struct sockaddr_in ServerAddress;
	ZeroStaticMemory(ServerAddress);
	
	// 填充地址信息
	ServerAddress.sin_family = AF_INET;
	
	// 这里可以绑定任何可用的IP地址，或者绑定一个指定的IP地址 
	//ServerAddress.sin_addr.s_addr = htonl(INADDR_ANY);                      
	ServerAddress.sin_addr.s_addr = inet_addr( pszAddr );
	ServerAddress.sin_port = htons(nPort);

	// 绑定地址和端口
	if (SOCKET_ERROR == ::bind(m_sktServer, (struct sockaddr *) &ServerAddress, sizeof(ServerAddress)))
	{
	  IOCP_ErrorLog( "bind错误:" + CStringUtil::FormatLastError() );
//	  ::closesocket(m_sktServer);
//	  m_sktServer = INVALID_SOCKET;
		return IOCP_KO;
	}

	// 开始进行监听 SOMAXCONN:Maximum queue length specifiable by listen
	if (SOCKET_ERROR == ::listen(m_sktServer, SOMAXCONN))
	{
	  IOCP_ErrorLog( "listen错误:" + CStringUtil::FormatLastError() );
		return IOCP_KO;
	}
	tstringstream ss;
	ss << "开始监听:" << pszAddr << ":" << nPort;
	IOCP_WriteLog( ss.str() );
	  
	// 为AcceptEx 准备参数，然后投递AcceptEx I/O请求
	for( int i = 0; i < IOCP_Ready_Accept; i++ )
	{
	  IOCP_Verify( NewClientContext() );
	}
	return IOCP_OK;
}

int CIocpServer::Shutdown()
{
	if (m_bInitialized)
	{
  	for (int i = 0; i < m_nWorkThreads; ++i)
  	{
  		::PostQueuedCompletionStatus(m_hIocp, 0, ((LONG_PTR)IOCP_ExitCode), 0);
  	}
  	::CloseHandle(m_hIocp);
  	m_ThreadPool.join_all();
    m_bInitialized = false;
	}
  AutoLock(ClientContextMutex);
  _itClientContext it = m_ClientContext.begin();
  
  while( it != m_ClientContext.end() )
  {
	  tstringstream ss;
	  ss << "delete CClientContext() " << (DWORD)(*it);
    IOCP_WriteLog( ss.str() );
    delete (*it);
    it = m_ClientContext.erase(it);
  }
  return IOCP_OK;
}

int CIocpServer::ThreadIocpService()
{
	m_pBarrier->wait();

	OVERLAPPED* pOverlapped = 0;
	DWORD dwBytesTransfered = 0;
	int nRes = IOCP_OK;
  CClientContext* pClientContext = 0;

  // 循环处理请求，直到接收到Shutdown信息为止
	while ( WAIT_OBJECT_0 != ::WaitForSingleObject(m_hShutdownEvent, 0) )
	{
		BOOL bRes = ::GetQueuedCompletionStatus(
				m_hIocp,
				&dwBytesTransfered,
				(PULONG_PTR)&pClientContext,
				&pOverlapped,
				INFINITE);

    // 读取传入的参数
    if (0 == pClientContext)
    {
      pClientContext = ::GetClientContext(pOverlapped);
    }
    if (0 == pClientContext)
    {
      IOCP_ErrorLog("0 == pClientContext,异常退出");
      nRes = IOCP_KO;
      break;
    }

		if(!bRes)
		{
		  nRes = pClientContext->HandleIocpError();
		  
		  if ( IOCP_KO == nRes )
		  {
		    break;
		  }
		  if (IOCP_RemoveClient == nRes)
		  {
		    RemoveClientContext(pClientContext);
		    continue;
		  }
		  if ( pClientContext->PostAcceptEx(m_sktServer) != IOCP_OK )
		  {
		    RemoveClientContext(pClientContext);
		  }
		}
		else
		{
      // 如果收到的是退出标志，则直接退出
      if ( IOCP_ExitCode == (DWORD)pClientContext )
      {
        break;
      }
		  if ( pClientContext->NeedRemove(dwBytesTransfered) )
		  {
		    RemoveClientContext(pClientContext);
		  }
		  else
		  {
  		  nRes = pClientContext->HandleOpertion(m_hIocp);

  		  switch (nRes)
  		  {
  		  case IOCP_NewClient:
  		    NewClientContext();
  		    break;
  		    
  		  case IOCP_RemoveClient:
  		    RemoveClientContext(pClientContext);
  		    break;
  		    
  		  default:
  		    break;
  		  }
		  }
		}
	} //end while
	return nRes;
}

int CIocpServer::NewClientContext()
{
  if (m_lAcceptExCounter >= IOCP_Max_Accept)
  {
    AutoLock(ClientContextMutex);

    for(_itClientContext it = m_ClientContext.begin(); it != m_ClientContext.end(); ++it)
    {
      if ( (*it)->IsTimeOut() )
      {
        
      }
    }
  }

  // 为以后连入的客户端先准备好Socket( 这个是与传统accept最大的区别 ) 
  SOCKET sktClient = ::WSASocket(AF_INET, SOCK_STREAM, IPPROTO_TCP, NULL, 0, WSA_FLAG_OVERLAPPED);

  if( INVALID_SOCKET == sktClient )  
  {  
    IOCP_ErrorLog( "创建用于AcceptEx的Socket失败:" + CStringUtil::FormatLastError() );
    return IOCP_KO;  
  } 
	// 新建一个ClientContext
	CClientContext* pClientContext = new CClientContext(sktClient);

	int nRes = pClientContext->PostAcceptEx( m_sktServer );
	
	if( IOCP_OK == nRes )
	{
	  tstringstream ss;
    ss << "new CClientContext:0x" << (DWORD)pClientContext;
	  IOCP_WriteLog( ss.str() );

    ::InterlockedIncrement(&m_lAcceptExCounter);
	  AutoLock(ClientContextMutex);
		m_ClientContext.push_back(pClientContext);
	}
	else
	{
	  delete pClientContext;
	}
	return nRes;
}

void CIocpServer::RemoveClientContext(CClientContext* pClientContext)
{
  bool bRemoved = false;
  {
	  AutoLock(ClientContextMutex);
	  
	  for(_itClientContext it = m_ClientContext.begin(); it != m_ClientContext.end(); ++it)
	  {
	    if ( (*it) == pClientContext )
	    {
 //       delete (*it);
	      m_ClientContext.erase(it);
	      bRemoved = true;
	      break;
	    }
	  }
  }
  if (bRemoved)
  {
	  tstringstream ss;
    ss << "delete CClientContext:0x" << (DWORD)pClientContext;
    IOCP_WriteLog( ss.str() );
    delete pClientContext;
  }
  else
  {
    IOCP_ErrorLog("找不到客户端上下文.");
  }
}

void CIocpServer::CheckTimeOutClient()
{
  for(_itClientContext it = m_ClientContext.begin(); it != m_ClientContext.end(); ++it)
  {
  }
}

