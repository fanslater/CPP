#include "stdafx.h"
#include <map>
#include "SyncIocp.h"


CIocpServ::CIocpServ()
{
  m_RunFlag = 1;
  m_hIocp = NULL;
}

void CIocpServ::Stop()
{
  m_RunFlag = 0;

  SYSTEM_INFO siSys;
  GetSystemInfo(&siSys);
  int nProcessors = siSys.dwNumberOfProcessors;

  for (int i = 0; i < nProcessors; ++i)
  {
    ::PostQueuedCompletionStatus(m_hIocp, 0, ((LONG_PTR)-1), 0);
    ::Sleep(1);
  }
  ::Sleep(50);
  ::CloseHandle(m_hIocp);
  ::closesocket(m_sListen);
  ::Sleep(500);
}
void CIocpServ::DebugLog(char *szFormatter, ...)
{
#if 0
  va_list vp;
  char szMsg[1024*4];
  szMsg[sizeof(szMsg) - 1] = 0;

  SYSTEMTIME systime;
  GetLocalTime(&systime);
  sprintf(szMsg, "[%02d:%02d:%02d:%03d] [0x%04x] ",
    systime.wHour, systime.wMinute, systime.wSecond, systime.wMilliseconds,GetCurrentThreadId());

  va_start(vp, szFormatter);
  _vsnprintf(&szMsg[strlen(szMsg)], sizeof(szMsg) - 3, szFormatter, vp);
  va_end(vp);
  strcat(szMsg,"\n");

  //SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_INTENSITY | FOREGROUND_RED);
  cout<<szMsg<<endl;
  //SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);
#endif
}
DWORD WINAPI CIocpServ::ListenThread( LPVOID lpParam )
{
  CIocpServ*  pIOCPServ= (CIocpServ*)lpParam;

  // 循环处理到来的请求
  while ( pIOCPServ->m_RunFlag )
  {
    // 等待接受连接请求
    SOCKADDR_IN saRemote;
    int nRemoteLen = sizeof( saRemote );
    SOCKET sRemote = ::accept( pIOCPServ->m_sListen, (sockaddr *)&saRemote, &nRemoteLen );

    if(sRemote == INVALID_SOCKET)
    {
      //cout<<"WSASocket错误 " << "socketfailedwitherror:%ld\n" << WSAGetLastError() << endl;;
      continue;
    }
    // 接受到新连接之后，为它创建一个per_handle数据，并将他们关联到完成端口对象
    PPER_HANDLE_DATA pPerHandle = ( PPER_HANDLE_DATA )::GlobalAlloc( GPTR, sizeof( PER_HANDLE_DATA ) );
    if( pPerHandle == NULL )
    {
      pIOCPServ->DebugLog( "pPerHandle GlobalAlloc() failed!");
      break;
    }
    //DebugLog( "GlobalAlloc pPerHandle:%08x", pPerHandle);
    pPerHandle->s = sRemote;
    memcpy( &pPerHandle->addr, &saRemote, nRemoteLen );
    ::CreateIoCompletionPort( (HANDLE)pPerHandle->s, pIOCPServ->m_hIocp, (DWORD)pPerHandle, 0 );


    // 创建一个pPerIo数据, 投递一个接收请求操作
    PPER_IO_DATA pPerIo = ( PPER_IO_DATA )::GlobalAlloc( GPTR, sizeof( PER_IO_DATA ) );
    if( pPerIo == NULL )
    {
      pIOCPServ->DebugLog( "pPerIo GlobalAlloc() failed!");
      break;
    }
    pIOCPServ->DebugLog( "GlobalAlloc pPerHandle:%08x, pPerIo:%08x", pPerHandle, pPerIo);
    memset(pPerIo, 0, sizeof(PER_IO_DATA));
    pPerIo->pHandleData = pPerHandle;

    // 投递接收请求操作
    DWORD dwRecv = 0;
    DWORD dwFlags = 0;
    WSABUF buf;
    buf.buf = pPerIo->buf;
    buf.len = IOCP_BUFFER_SIZE;
    pPerIo->nOperationType = OP_READ;
    //自定义一个结构体，起始数据结构为OVERLAPPED，利用系统返回OVERLAPPED指针，从而得到自定义的数据
    ::WSARecv( pPerHandle->s, &buf, 1, &dwRecv, &dwFlags, (OVERLAPPED*)pPerIo, NULL);
  }
  return 0;
}
DWORD WINAPI CIocpServ::ServerThread( LPVOID lpParam )
{
  CIocpServ*  pIOCPServ= (CIocpServ*)lpParam;
  if(pIOCPServ == NULL || pIOCPServ->m_hIocp == NULL )
  {
    return -1;
  }

  PPER_HANDLE_DATA pPerHandle;
  PPER_IO_DATA     pPerIo;

  //WSABUF wsabuf;
  DWORD dwTrans = 0, dwRecv = 0, dwFlags = 0, dwSend = 0;

  while( pIOCPServ->m_RunFlag )
  {
    // 在关联到此完成端口的所有套接字上等待I/O完成
    BOOL bRet = ::GetQueuedCompletionStatus( pIOCPServ->m_hIocp, &dwTrans, (LPDWORD)&pPerHandle, (LPOVERLAPPED*)&pPerIo, WSA_INFINITE );

    if (INVALID_HANDLE_VALUE == (HANDLE)pPerHandle)
    {
      break;
    }
    if( !bRet ) // 发生错误
    {
      if (pPerHandle)
      {
        ::closesocket( pPerHandle->s );
        ::GlobalFree( pPerHandle );
      }
      if (pPerIo)
      {
        ::GlobalFree( pPerIo );
      }
      pIOCPServ->DebugLog("GetQueuedCompletionStatus return:%d, GlobalFree:pPerHandle:%08x,pPerIo:%08x", bRet,pPerHandle,pPerIo);
      continue;
    }

    // 套接字被对方关闭)
    if( dwTrans == 0 && ( pPerIo->nOperationType == OP_READ || pPerIo->nOperationType == OP_WRITE ) )
    {
      ::closesocket( pPerHandle->s );
      ::GlobalFree( pPerHandle );
      ::GlobalFree( pPerIo );

      pIOCPServ->DebugLog("client closed:%d, GlobalFree:pPerHandle:%08x,pPerIo:%08x", bRet,pPerHandle,pPerIo);
      continue;
    }

    switch ( pPerIo->nOperationType )
    {
    case OP_READ:       // 接收完成
      {
        pPerIo->buf[dwTrans] = (char)0x0;
        pPerIo->strRecv += pPerIo->buf;

        char *pContentLen = strstr(pPerIo->buf, "Content-Length: ");
        char *pHeadLen = strstr(pPerIo->buf, "\r\n\r\n");
        if(pContentLen && pHeadLen)
        {
          //取http报文长度
          pPerIo->iTotalLen = atoi(pContentLen+strlen("Content-Length: ")) + (int)(pHeadLen-pPerIo->buf)+4;
        }
        else if(0x01 == pPerIo->buf[64] && 0x01 == pPerIo->buf[69])
        {//A101    SUCCESS             153132627                   01110000SFCG20140729153132153132627909630900000000102600BANKSUCCESS0000通讯:自动返回成功!
          //取三方交易网关报文长度
          char szTemp[8];
          memset(szTemp, 0, sizeof(szTemp));
          strncpy(szTemp, &pPerIo->buf[56], 4);
          pPerIo->iTotalLen = atoi(szTemp);
        }
        if((int)pPerIo->strRecv.length() < pPerIo->iTotalLen)
        {//如果是HTTP报文，取数据长度，长度没取完时，服务器继续取数据
          WSABUF buf;
          buf.buf = pPerIo->buf;
          buf.len = IOCP_BUFFER_SIZE;
          pPerIo->nOperationType = OP_READ;
          //继续投递接收操作
          ::WSARecv( pPerHandle->s, &buf, 1, &dwRecv, &dwFlags, (OVERLAPPED*)pPerIo, NULL);
          pIOCPServ->DebugLog( "Socket:%d,数据接收未完成", ntohs(pPerHandle->addr.sin_port));

          continue;
        }

        cout <<  "Socket:" << ntohs(pPerHandle->addr.sin_port) << ",接收：" << pPerIo->strRecv << endl;
        // 数据处理
        //pIOCPServ->DebugLog( "Socket:%d,接收：%s", ntohs(pPerHandle->addr.sin_port), pPerIo->strRecv.c_str());
        if(pPerIo->strRecv.length()) pIOCPServ->m_queueRecv.push(pPerIo);
      }
      break;
    case OP_WRITE:     //　发送完成
      {
        pIOCPServ->DebugLog( "Socket:%d,应答：%s", ntohs(pPerHandle->addr.sin_port),pPerIo->buf);

        memset(pPerIo, 0, sizeof(PER_IO_DATA));
        ::closesocket( pPerHandle->s );
        ::GlobalFree( pPerHandle );
        ::GlobalFree( pPerIo );
        pIOCPServ->DebugLog("client closed:%d, GlobalFree:pPerHandle:%08x,pPerIo:%08x", bRet,pPerHandle,pPerIo);
      }
      break;
    case OP_ACCEPT:
      break;
    }
  }

  return 0;
}

int CIocpServ::IocpRun(const char *pszAddr, int nPort)
{
  // 创建监听套接字
  int nReuseAddr = 1;
  if ((m_sListen = WSASocket(AF_INET, SOCK_STREAM, 0, NULL, 0, WSA_FLAG_OVERLAPPED)) == INVALID_SOCKET)
  {
    cout<<"WSASocket错误"<<endl;
    return -1;
  }
  if(setsockopt(m_sListen,SOL_SOCKET,SO_REUSEADDR,(const char *)&nReuseAddr,sizeof(int)) != 0)
  {
    cout<<"setsockopt错误"<<endl;
    return -1;
  }
  SOCKADDR_IN LocalAddr;
  LocalAddr.sin_family = AF_INET;
  LocalAddr.sin_port = ::htons( nPort );
  LocalAddr.sin_addr.s_addr = inet_addr(pszAddr);
  // 绑定本地端口
  if (bind(m_sListen, (PSOCKADDR) &LocalAddr, sizeof(LocalAddr)) == SOCKET_ERROR)
  {
    int nError = WSAGetLastError();
    cout<<"bind错误"<<nError<<endl;
    return -1;
  }
  // 开始监听
  if (listen(m_sListen, SOMAXCONN) == SOCKET_ERROR)
  {
    int nError = WSAGetLastError();
    cout<<"listen错误"<<nError<<endl;
    return -1;
  }

  //获取CPU数目 siSys.dwNumberOfProcessors
  SYSTEM_INFO siSys;
  GetSystemInfo(&siSys);
  int nProcessors = siSys.dwNumberOfProcessors;
  // 创建完成端口对象
  m_hIocp = ::CreateIoCompletionPort( INVALID_HANDLE_VALUE, NULL, 0, nProcessors );

  // 创建监听线程
  HANDLE hThread;
  hThread = ::CreateThread( NULL, 0, ListenThread, (LPVOID)this, 0, 0 );
  CloseHandle(hThread);

  // 创建工作线程处理完成端口对象的事件
  for (int i=0; i<nProcessors; i++)
  {
    HANDLE hThread;
    hThread = ::CreateThread( NULL, 0, ServerThread, (LPVOID)this, 0, 0 );
    CloseHandle(hThread);
    //cout<<"创建工作者线程"<<i<<endl;
  }

  return 0;
}
int CIocpServ::SendAnsData(PPER_HANDLE_DATA pPerHandle, PPER_IO_DATA pPerIo, const std::string &strAnsData)
{
  WSABUF wsabuf;
  DWORD dwSend;

  if(pPerIo->pHandleData)
  {
    strcpy(pPerIo->buf, strAnsData.c_str());
    pPerIo->nOperationType = OP_WRITE;
    wsabuf.buf = pPerIo->buf;
    wsabuf.len = strlen(pPerIo->buf);
    ::WSASend( pPerHandle->s, &wsabuf, 1, &dwSend, 0, &pPerIo->ol, NULL );
    DebugLog("===============WSASend:%s", pPerIo->buf);
  }

  return 0;
}

