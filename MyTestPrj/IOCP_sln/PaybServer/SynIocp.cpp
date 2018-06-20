#include "stdafx.h"
#include "SynIocp.h"
#include <iostream>
#include "Public.h"

CClientContext::CClientContext()
{

}

CServerContext::CServerContext()
{
  ZeroStaticMemory(ol);
  ZeroStaticMemory(buf);
  nOperationType = -1;
  pClientContext = 0;
  nTotalLen = 0;
}

tstring CServerContext::GetFunctionID()
{
  tstring strPost("POST ");
  int nBeginPos = strRecv.find(strPost);

  if (std::string::npos == nBeginPos)
  {
    strPost = "GET ";
    nBeginPos = strRecv.find(strPost);
  }
  int nEndPos = strRecv.find(" HTTP");
  if(nBeginPos != std::string::npos && nEndPos != std::string::npos)
  {
    return strRecv.substr( nBeginPos + strPost.length(), nEndPos - nBeginPos - strPost.length() );
  }
  return strRecv;
}

void CServerContext::FillBuffer(const tstring& strData)
{
  const int nMaxLen = strData.length() < BUFFER_SIZE ? strData.length() : BUFFER_SIZE - 1;
  memcpy(buf, strData.c_str(), nMaxLen);
  buf[nMaxLen] = 0;
}

CSynIocp::CSynIocp()
{
  m_nRunFlag = 1;
  m_hIoCompletion = NULL;
  m_sktServer = INVALID_SOCKET;
  m_bInitialized = false;
  m_pBarrier = 0;
}

CSynIocp::~CSynIocp()
{
  m_nRunFlag = 1;
  m_hIoCompletion = NULL;
  m_bInitialized = false;
  delete m_pBarrier;
}

int CSynIocp::Initialize(const tchar *pszAddr, int nPort)
{
  if (m_bInitialized)
  {
    return 0;
  }

  m_sktServer = ::WSASocket(AF_INET, SOCK_STREAM, 0, NULL, 0, WSA_FLAG_OVERLAPPED);
  if (INVALID_SOCKET == m_sktServer)
  {
    ErrorLog( "WSASocket错误:" + CStringUtil::FormatLastError() );
    return -1;
  }

  // 创建监听套接字
  int nReuseAddr = 1;

  if(::setsockopt(m_sktServer,SOL_SOCKET,SO_REUSEADDR,(const tchar *)&nReuseAddr,sizeof(int)) != 0)
  {
    ErrorLog("setsockopt错误");
    return -1;
  }

  SOCKADDR_IN LocalAddr;
  LocalAddr.sin_family = AF_INET;
  LocalAddr.sin_port = ::htons( nPort );
  LocalAddr.sin_addr.s_addr = inet_addr(pszAddr);

  // 绑定本地端口
  if (SOCKET_ERROR == ::bind(m_sktServer, (PSOCKADDR) &LocalAddr, sizeof(LocalAddr)))
  {
    ErrorLog( "bind错误:" + CStringUtil::FormatLastError() );
    return -1;
  }

  // 开始监听
  if (SOCKET_ERROR == ::listen(m_sktServer, SOMAXCONN))
  {
    ErrorLog( "listen错误:" + CStringUtil::FormatLastError() );
    return -1;
  }

  //获取CPU数目
  SYSTEM_INFO SysInfo;
  GetSystemInfo(&SysInfo);

  // 创建完成端口对象
  m_hIoCompletion = ::CreateIoCompletionPort( INVALID_HANDLE_VALUE, NULL, 0, SysInfo.dwNumberOfProcessors );

  if (NULL == m_hIoCompletion)
  {
    ErrorLog( "CreateIoCompletionPort:" + CStringUtil::FormatLastError() );
    return -1;
  }

  // 创建监听线程
  m_pBarrier = new boost::barrier(SysInfo.dwNumberOfProcessors + 2);
  m_ThreadPool.create_thread(boost::bind(&CSynIocp::ThreadListenClient, this));

  // 创建工作线程处理完成端口对象的事件
  for (DWORD i = 0; i < SysInfo.dwNumberOfProcessors; i++)
  {
    m_ThreadPool.create_thread(boost::bind(&CSynIocp::ThreadWorker, this));
  }
  m_pBarrier->wait();
  m_bInitialized = true;
  return 0;
}

void CSynIocp::ShutDown()
{
  m_nRunFlag = 0;

  SYSTEM_INFO SysInfo;
  GetSystemInfo(&SysInfo);

  for (DWORD i = 0; i < SysInfo.dwNumberOfProcessors; ++i)
  {
    ::PostQueuedCompletionStatus(m_hIoCompletion, 0, ((LONG_PTR)INVALID_HANDLE_VALUE), 0);
    ::Sleep(1);
  }
  ::Sleep(50);
  ::CloseHandle(m_hIoCompletion);
  ::closesocket(m_sktServer);
  ::Sleep(500);
  m_ThreadPool.join_all();
  m_bInitialized = false;
}

int CSynIocp::ThreadListenClient()
{
  m_pBarrier->wait();

  // 循环处理到来的请求
  while ( m_nRunFlag )
  {
    // 等待接受连接请求
    SOCKADDR_IN saRemote;
    int nRemoteLen = sizeof( saRemote );
    SOCKET sktClient = ::accept( m_sktServer, (sockaddr *)&saRemote, &nRemoteLen );

    if(INVALID_SOCKET == sktClient)
    {
      ErrorLog( "accept错误:" + CStringUtil::FormatLastError() );
      continue;
    }
    // 接受到新连接之后，为它创建一个per_handle数据，并将他们关联到完成端口对象
    CClientContext* pClientContext = new CClientContext();

    if( !pClientContext )
    {
      ErrorLog("pClientContext new() failed!");
      break;
    }
    pClientContext->m_sktClient = sktClient;
    memcpy( &pClientContext->addr, &saRemote, nRemoteLen );
    ::CreateIoCompletionPort( (HANDLE)pClientContext->m_sktClient, m_hIoCompletion, (DWORD)pClientContext, 0 );

    // 创建一个pPerIo数据, 投递一个接收请求操作
    CServerContext* pServerContext = new CServerContext();

    if( !pServerContext )
    {
      delete pClientContext;
      ErrorLog( "pServerContext new() failed!");
      break;
    }
    pServerContext->pClientContext = pClientContext;

    // 投递接收请求操作
    DWORD dwRecv = 0;
    DWORD dwFlags = 0;
    WSABUF buf;
    buf.buf = pServerContext->buf;
    buf.len = BUFFER_SIZE;
    pServerContext->nOperationType = OP_READ;

    //自定义一个结构体，起始数据结构为OVERLAPPED，利用系统返回OVERLAPPED指针，从而得到自定义的数据
    ::WSARecv( pClientContext->m_sktClient, &buf, 1, &dwRecv, &dwFlags, (OVERLAPPED*)pServerContext, NULL);
  }
  return 0;
}

int CSynIocp::ThreadWorker()
{
  m_pBarrier->wait();

  if(NULL == m_hIoCompletion)
  {
    return -1;
  }
  CClientContext* pClientContext = 0;
  CServerContext* pServerContext = 0;

  DWORD dwTrans = 0;
  DWORD dwRecv  = 0;
  DWORD dwFlags = 0;
  DWORD dwSend  = 0;

  while( m_nRunFlag )
  {
    // 在关联到此完成端口的所有套接字上等待I/O完成
    BOOL bRet = ::GetQueuedCompletionStatus( m_hIoCompletion, &dwTrans, (LPDWORD)&pClientContext, (LPOVERLAPPED*)&pServerContext, WSA_INFINITE );

    if (INVALID_HANDLE_VALUE == (HANDLE)pClientContext)
    {
      break;
    }
    if( !bRet ) // 发生错误
    {
      if (pClientContext)
      {
        ::closesocket( pClientContext->m_sktClient );
        delete pClientContext;
      }
      if (pServerContext)
      {
        delete pServerContext;
      }
      continue;
    }

    // 套接字被对方关闭)
    if( dwTrans == 0 && ( pServerContext->nOperationType == OP_READ || pServerContext->nOperationType == OP_WRITE ) )
    {
      ::closesocket( pClientContext->m_sktClient );
      delete pClientContext;
      delete pServerContext;

      continue;
    }

    switch ( pServerContext->nOperationType )
    {
    case OP_READ:       // 接收完成
      {
        pServerContext->buf[dwTrans] = (tchar)0x0;
        pServerContext->strRecv += pServerContext->buf;

        const tstring strHttpLen(TEXT("Content-Length: "));
        tchar *pContentLen = strstr(pServerContext->buf, strHttpLen.c_str());
        tchar *pHeadLen = strstr(pServerContext->buf, TEXT("\r\n\r\n"));

        if(pContentLen && pHeadLen)
        {
          //取http报文长度
          pServerContext->nTotalLen = String2InternalType<int>(pContentLen + strHttpLen.length()) + (int)(pHeadLen-pServerContext->buf) + 4;
        }
        else if(0x01 == pServerContext->buf[64] && 0x01 == pServerContext->buf[69])
        {
          tchar szTemp[8] = {0};
          strncpy(szTemp, &pServerContext->buf[56], 4);
          pServerContext->nTotalLen = String2InternalType<int>(szTemp);
        }

        //如果是HTTP报文，取数据长度，长度没取完时，服务器继续取数据
        if((int)pServerContext->strRecv.length() < pServerContext->nTotalLen)
        {
          WSABUF buf;
          buf.buf = pServerContext->buf;
          buf.len = BUFFER_SIZE;
          pServerContext->nOperationType = OP_READ;

          //继续投递接收操作
          ::WSARecv( pClientContext->m_sktClient, &buf, 1, &dwRecv, &dwFlags, (OVERLAPPED*)pServerContext, NULL);
          continue;
        }

        // 数据处理
        if(pServerContext->strRecv.length())
        {
          m_queueRecv.push(pServerContext);
        }
      }
      break;
    case OP_WRITE:     //　发送完成
      {
        ::closesocket( pClientContext->m_sktClient );
        delete pClientContext;
        delete pServerContext;
      }
      break;
    case OP_ACCEPT:
      break;
    }
  }
  return 0;
}

CServerContext* CSynIocp::GetRequest()
{
  CServerContext *pServerContext = NULL;
  m_queueRecv.wait_and_pop(pServerContext, 1);
  return pServerContext;
}

int CSynIocp::SendResponse(CClientContext* pClientContext, CServerContext* pServerContext, tstring &strAnsData)
{
  WSABUF wsabuf;
  DWORD dwSend;

  if(pServerContext->pClientContext)
  {
    pServerContext->FillBuffer(strAnsData);
    pServerContext->nOperationType = OP_WRITE;
    wsabuf.buf = pServerContext->buf;
    wsabuf.len = strlen(pServerContext->buf);
    ::WSASend( pClientContext->m_sktClient, &wsabuf, 1, &dwSend, 0, &pServerContext->ol, NULL );
    //			debugLog("===============WSASend:%s", pServerContext->buf);
  }
  return 0;
}
