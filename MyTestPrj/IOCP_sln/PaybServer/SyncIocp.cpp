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

  // ѭ��������������
  while ( pIOCPServ->m_RunFlag )
  {
    // �ȴ�������������
    SOCKADDR_IN saRemote;
    int nRemoteLen = sizeof( saRemote );
    SOCKET sRemote = ::accept( pIOCPServ->m_sListen, (sockaddr *)&saRemote, &nRemoteLen );

    if(sRemote == INVALID_SOCKET)
    {
      //cout<<"WSASocket���� " << "socketfailedwitherror:%ld\n" << WSAGetLastError() << endl;;
      continue;
    }
    // ���ܵ�������֮��Ϊ������һ��per_handle���ݣ��������ǹ�������ɶ˿ڶ���
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


    // ����һ��pPerIo����, Ͷ��һ�������������
    PPER_IO_DATA pPerIo = ( PPER_IO_DATA )::GlobalAlloc( GPTR, sizeof( PER_IO_DATA ) );
    if( pPerIo == NULL )
    {
      pIOCPServ->DebugLog( "pPerIo GlobalAlloc() failed!");
      break;
    }
    pIOCPServ->DebugLog( "GlobalAlloc pPerHandle:%08x, pPerIo:%08x", pPerHandle, pPerIo);
    memset(pPerIo, 0, sizeof(PER_IO_DATA));
    pPerIo->pHandleData = pPerHandle;

    // Ͷ�ݽ����������
    DWORD dwRecv = 0;
    DWORD dwFlags = 0;
    WSABUF buf;
    buf.buf = pPerIo->buf;
    buf.len = IOCP_BUFFER_SIZE;
    pPerIo->nOperationType = OP_READ;
    //�Զ���һ���ṹ�壬��ʼ���ݽṹΪOVERLAPPED������ϵͳ����OVERLAPPEDָ�룬�Ӷ��õ��Զ��������
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
    // �ڹ���������ɶ˿ڵ������׽����ϵȴ�I/O���
    BOOL bRet = ::GetQueuedCompletionStatus( pIOCPServ->m_hIocp, &dwTrans, (LPDWORD)&pPerHandle, (LPOVERLAPPED*)&pPerIo, WSA_INFINITE );

    if (INVALID_HANDLE_VALUE == (HANDLE)pPerHandle)
    {
      break;
    }
    if( !bRet ) // ��������
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

    // �׽��ֱ��Է��ر�)
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
    case OP_READ:       // �������
      {
        pPerIo->buf[dwTrans] = (char)0x0;
        pPerIo->strRecv += pPerIo->buf;

        char *pContentLen = strstr(pPerIo->buf, "Content-Length: ");
        char *pHeadLen = strstr(pPerIo->buf, "\r\n\r\n");
        if(pContentLen && pHeadLen)
        {
          //ȡhttp���ĳ���
          pPerIo->iTotalLen = atoi(pContentLen+strlen("Content-Length: ")) + (int)(pHeadLen-pPerIo->buf)+4;
        }
        else if(0x01 == pPerIo->buf[64] && 0x01 == pPerIo->buf[69])
        {//A101    SUCCESS             153132627                   01110000SFCG20140729153132153132627909630900000000102600BANKSUCCESS0000ͨѶ:�Զ����سɹ�!
          //ȡ�����������ر��ĳ���
          char szTemp[8];
          memset(szTemp, 0, sizeof(szTemp));
          strncpy(szTemp, &pPerIo->buf[56], 4);
          pPerIo->iTotalLen = atoi(szTemp);
        }
        if((int)pPerIo->strRecv.length() < pPerIo->iTotalLen)
        {//�����HTTP���ģ�ȡ���ݳ��ȣ�����ûȡ��ʱ������������ȡ����
          WSABUF buf;
          buf.buf = pPerIo->buf;
          buf.len = IOCP_BUFFER_SIZE;
          pPerIo->nOperationType = OP_READ;
          //����Ͷ�ݽ��ղ���
          ::WSARecv( pPerHandle->s, &buf, 1, &dwRecv, &dwFlags, (OVERLAPPED*)pPerIo, NULL);
          pIOCPServ->DebugLog( "Socket:%d,���ݽ���δ���", ntohs(pPerHandle->addr.sin_port));

          continue;
        }

        cout <<  "Socket:" << ntohs(pPerHandle->addr.sin_port) << ",���գ�" << pPerIo->strRecv << endl;
        // ���ݴ���
        //pIOCPServ->DebugLog( "Socket:%d,���գ�%s", ntohs(pPerHandle->addr.sin_port), pPerIo->strRecv.c_str());
        if(pPerIo->strRecv.length()) pIOCPServ->m_queueRecv.push(pPerIo);
      }
      break;
    case OP_WRITE:     //���������
      {
        pIOCPServ->DebugLog( "Socket:%d,Ӧ��%s", ntohs(pPerHandle->addr.sin_port),pPerIo->buf);

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
  // ���������׽���
  int nReuseAddr = 1;
  if ((m_sListen = WSASocket(AF_INET, SOCK_STREAM, 0, NULL, 0, WSA_FLAG_OVERLAPPED)) == INVALID_SOCKET)
  {
    cout<<"WSASocket����"<<endl;
    return -1;
  }
  if(setsockopt(m_sListen,SOL_SOCKET,SO_REUSEADDR,(const char *)&nReuseAddr,sizeof(int)) != 0)
  {
    cout<<"setsockopt����"<<endl;
    return -1;
  }
  SOCKADDR_IN LocalAddr;
  LocalAddr.sin_family = AF_INET;
  LocalAddr.sin_port = ::htons( nPort );
  LocalAddr.sin_addr.s_addr = inet_addr(pszAddr);
  // �󶨱��ض˿�
  if (bind(m_sListen, (PSOCKADDR) &LocalAddr, sizeof(LocalAddr)) == SOCKET_ERROR)
  {
    int nError = WSAGetLastError();
    cout<<"bind����"<<nError<<endl;
    return -1;
  }
  // ��ʼ����
  if (listen(m_sListen, SOMAXCONN) == SOCKET_ERROR)
  {
    int nError = WSAGetLastError();
    cout<<"listen����"<<nError<<endl;
    return -1;
  }

  //��ȡCPU��Ŀ siSys.dwNumberOfProcessors
  SYSTEM_INFO siSys;
  GetSystemInfo(&siSys);
  int nProcessors = siSys.dwNumberOfProcessors;
  // ������ɶ˿ڶ���
  m_hIocp = ::CreateIoCompletionPort( INVALID_HANDLE_VALUE, NULL, 0, nProcessors );

  // ���������߳�
  HANDLE hThread;
  hThread = ::CreateThread( NULL, 0, ListenThread, (LPVOID)this, 0, 0 );
  CloseHandle(hThread);

  // ���������̴߳�����ɶ˿ڶ�����¼�
  for (int i=0; i<nProcessors; i++)
  {
    HANDLE hThread;
    hThread = ::CreateThread( NULL, 0, ServerThread, (LPVOID)this, 0, 0 );
    CloseHandle(hThread);
    //cout<<"�����������߳�"<<i<<endl;
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

