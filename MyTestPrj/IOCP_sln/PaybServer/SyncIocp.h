#ifndef __SyncIocp_h__
#define __SyncIocp_h__


#include <winsock2.h>
#include <iostream>
#include <sstream>

#include "StringUtil.h"
#include "Public.h"
//#include "log4cplus.h"
using namespace std;
#include "Queue.h"


/**  ���ݻ�������С */
#define IOCP_BUFFER_SIZE 1024*64

/** @name ����״̬����
*  @{ */
/**  ������ */
#define OP_READ    1
/**  д���� */
#define OP_WRITE   2
/**  ���ղ��� */
#define OP_ACCEPT  3
/** @}*/

/**  ÿһ�����Ӵ������� */
typedef struct _PER_HANDLE_DATA
{
  /**  ��Ӧ���׽��־�� */
  SOCKET      s;
  /**  �Է��ĵ�ַ */
  sockaddr_in addr;
}PER_HANDLE_DATA, *PPER_HANDLE_DATA;

/**  ÿһ��IO���ݽṹ */
typedef struct _PER_IO_DATA
{
  /**  �ص��ṹ */
  OVERLAPPED  ol;
  /**  ���ݻ����� */
  char        buf[IOCP_BUFFER_SIZE];
  /**  �������� */
  int         nOperationType;

  PPER_HANDLE_DATA pHandleData;
  std::string strRecv;
  int iTotalLen;
}PER_IO_DATA, *PPER_IO_DATA;


class CIocpServ
{
public:
  int m_RunFlag;
  CQueue<PPER_IO_DATA> m_queueRecv;

  HANDLE m_hIocp;
  SOCKET m_sListen;

  CIocpServ();

  void Stop();
  void DebugLog(char *szFormatter, ...);
  static DWORD WINAPI ListenThread( LPVOID lpParam );
  static DWORD WINAPI ServerThread( LPVOID lpParam );

  int IocpRun(const char *pszAddr, int nPort);
  int SendAnsData(PPER_HANDLE_DATA pPerHandle, PPER_IO_DATA pPerIo, const std::string &strAnsData);
};


#endif