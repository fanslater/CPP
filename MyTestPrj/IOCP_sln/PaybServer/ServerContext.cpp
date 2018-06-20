#include "stdafx.h"
#include "ServerContext.h"
#include "SysInterface.h"

#if 0
CServerContext* CServerContext::m_pInstance = 0;

CServerContext* CServerContext::CreateInstance()
{
  if (0 == m_pInstance)
  {
    m_pInstance = new CServerContext();
  }
  return m_pInstance;
}

void CServerContext::DestroyInstance()
{
  delete m_pInstance;
  m_pInstance = 0;
}

CServerContext::CServerContext()
{
  m_sktServer = INVALID_SOCKET;
  m_hIocp = INVALID_HANDLE;
}

CServerContext::~CServerContext()
{
  if( m_sktServer != INVALID_SOCKET )
  {
    ::closesocket( m_sktServer );
    m_sktServer = INVALID_SOCKET;
  }
  {
	  AutoLock(ClientContextMutex);
	  
	  for(_itClientContext it = m_ClientContext.begin(); it != m_ClientContext.end(); ++it)
	  {
      delete (*it);
	  }
	  m_ClientContext.clear();
  }
}

int CServerContext::Initialize(HANDLE hIocp, const tchar *pszAddr, int nPort)
{
	// 需要使用重叠IO，必须得使用WSASocket来建立Socket，才可以支持重叠IO操作
  m_sktServer = ::WSASocket(AF_INET, SOCK_STREAM, 0, NULL, 0, WSA_FLAG_OVERLAPPED);
  
	if (INVALID_SOCKET == m_sktServer)
	{
    WriteLog( "WSASocket错误:" + CStringUtil::FormatLastError() );
		return IOCP_KO;
	}

	// 将Listen Socket绑定至完成端口中
	if( NULL == ::CreateIoCompletionPort( (HANDLE)m_sktServer, hIocp, (DWORD)0, 0) )
	{
	  WriteLog( "WSASocket错误:" + CStringUtil::FormatLastError() );
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
	  WriteLog( "bind错误:" + CStringUtil::FormatLastError() );
//	  ::closesocket(m_sktServer);
//	  m_sktServer = INVALID_SOCKET;
		return IOCP_KO;
	}

	// 开始进行监听 SOMAXCONN:Maximum queue length specifiable by listen
	if (SOCKET_ERROR == ::listen(m_sktServer, SOMAXCONN))
	{
	  WriteLog( "listen错误:" + CStringUtil::FormatLastError() );
		return IOCP_KO;
	}
	
  if ( IOCP_OK != CSysInterface::GetSocketApi(m_sktServer) )
  {
		return IOCP_KO;
  }
  
	// 为AcceptEx 准备参数，然后投递AcceptEx I/O请求
	for( int i = 0; i < MAX_POST_ACCEPT; i++ )
	{
		// 新建一个IO_CONTEXT
		_ClientContextPtr ClientContextPtr( new CClientContext(hIocp) );

		if( IOCP_OK != ClientContextPtr->PostAccept( m_sktServer ) )
		{
			return IOCP_KO;
		}
		m_ClientContext.push_back(ClientContextPtr);
	}
	return IOCP_OK;
}

void CServerContext::RemoveClientContext(CClientContext* pClientContext)
{
  bool bRemoved = false;
  {
	  AutoLock(ClientContextMutex);
	  
	  for(_itClientContext it = m_ClientContext.begin(); it != m_ClientContext.end(); ++it)
	  {
	    if (*it == pClientContext)
	    {
	      delete pClientContext;
	      m_ClientContext.erase(it);
	      bRemoved = true;
	      break;
	    }
	  }
  }
  if (!bRemoved)
  {
    ErrorLog("找不到客户端上下文.");
    delete pClientContext;
  }
}

int CServerContext::HandleOpertion(CClientContext* pClientContext)
{
  int nRes = pClientContext->HandleOpertion();

  if (IOCP_Remove == nRes)
  {
    RemoveClientContext(pClientContext);
  }
}
#endif
