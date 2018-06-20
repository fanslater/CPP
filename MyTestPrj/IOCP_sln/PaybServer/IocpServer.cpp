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
	
	// ����ϵͳ�˳����¼�֪ͨ
	m_hShutdownEvent = CreateEvent(NULL, TRUE, FALSE, NULL);

	//��ȡCPU��Ŀ
	SYSTEM_INFO SysInfo;
	GetSystemInfo(&SysInfo);
  m_nWorkThreads = 2 * (int)SysInfo.dwNumberOfProcessors;
	
	// ������ɶ˿ڶ���
	m_hIocp = ::CreateIoCompletionPort( INVALID_HANDLE_VALUE, NULL, 0, m_nWorkThreads );

  if (NULL == m_hIocp)
  {
	  IOCP_ErrorLog( "������ɶ˿�ʧ��:" + CStringUtil::FormatLastError() );
		return IOCP_KO;
  }
  IOCP_Verify( InitServerSocket(pszAddr, nPort) );

	// ���������߳�
	m_pBarrier = new boost::barrier(m_nWorkThreads + 1);

	// ���������̴߳�����ɶ˿ڶ�����¼�
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
	// ��Ҫʹ���ص�IO�������ʹ��WSASocket������Socket���ſ���֧���ص�IO����
  m_sktServer = ::WSASocket(AF_INET, SOCK_STREAM, 0, NULL, 0, WSA_FLAG_OVERLAPPED);
  
	if (INVALID_SOCKET == m_sktServer)
	{
    IOCP_ErrorLog( "WSASocket����:" + CStringUtil::FormatLastError() );
		return IOCP_KO;
	}
  IOCP_Verify( CSocketApi::GetSocketApi(m_sktServer) );

	// ��Listen Socket������ɶ˿���
	if( m_hIocp != ::CreateIoCompletionPort( (HANDLE)m_sktServer, m_hIocp, (DWORD)0, 0) )
	{
	  IOCP_ErrorLog( "WSASocket����:" + CStringUtil::FormatLastError() );
	  return IOCP_KO;
	}

	// ��������ַ��Ϣ�����ڰ�Socket
	struct sockaddr_in ServerAddress;
	ZeroStaticMemory(ServerAddress);
	
	// ����ַ��Ϣ
	ServerAddress.sin_family = AF_INET;
	
	// ������԰��κο��õ�IP��ַ�����߰�һ��ָ����IP��ַ 
	//ServerAddress.sin_addr.s_addr = htonl(INADDR_ANY);                      
	ServerAddress.sin_addr.s_addr = inet_addr( pszAddr );
	ServerAddress.sin_port = htons(nPort);

	// �󶨵�ַ�Ͷ˿�
	if (SOCKET_ERROR == ::bind(m_sktServer, (struct sockaddr *) &ServerAddress, sizeof(ServerAddress)))
	{
	  IOCP_ErrorLog( "bind����:" + CStringUtil::FormatLastError() );
//	  ::closesocket(m_sktServer);
//	  m_sktServer = INVALID_SOCKET;
		return IOCP_KO;
	}

	// ��ʼ���м��� SOMAXCONN:Maximum queue length specifiable by listen
	if (SOCKET_ERROR == ::listen(m_sktServer, SOMAXCONN))
	{
	  IOCP_ErrorLog( "listen����:" + CStringUtil::FormatLastError() );
		return IOCP_KO;
	}
	tstringstream ss;
	ss << "��ʼ����:" << pszAddr << ":" << nPort;
	IOCP_WriteLog( ss.str() );
	  
	// ΪAcceptEx ׼��������Ȼ��Ͷ��AcceptEx I/O����
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

  // ѭ����������ֱ�����յ�Shutdown��ϢΪֹ
	while ( WAIT_OBJECT_0 != ::WaitForSingleObject(m_hShutdownEvent, 0) )
	{
		BOOL bRes = ::GetQueuedCompletionStatus(
				m_hIocp,
				&dwBytesTransfered,
				(PULONG_PTR)&pClientContext,
				&pOverlapped,
				INFINITE);

    // ��ȡ����Ĳ���
    if (0 == pClientContext)
    {
      pClientContext = ::GetClientContext(pOverlapped);
    }
    if (0 == pClientContext)
    {
      IOCP_ErrorLog("0 == pClientContext,�쳣�˳�");
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
      // ����յ������˳���־����ֱ���˳�
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

  // Ϊ�Ժ�����Ŀͻ�����׼����Socket( ������봫ͳaccept�������� ) 
  SOCKET sktClient = ::WSASocket(AF_INET, SOCK_STREAM, IPPROTO_TCP, NULL, 0, WSA_FLAG_OVERLAPPED);

  if( INVALID_SOCKET == sktClient )  
  {  
    IOCP_ErrorLog( "��������AcceptEx��Socketʧ��:" + CStringUtil::FormatLastError() );
    return IOCP_KO;  
  } 
	// �½�һ��ClientContext
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
    IOCP_ErrorLog("�Ҳ����ͻ���������.");
  }
}

void CIocpServer::CheckTimeOutClient()
{
  for(_itClientContext it = m_ClientContext.begin(); it != m_ClientContext.end(); ++it)
  {
  }
}

