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


/**  数据缓存区大小 */
#define IOCP_BUFFER_SIZE 1024*64

/** @name 操作状态常量
*  @{ */
/**  读操作 */
#define OP_READ    1
/**  写操作 */
#define OP_WRITE   2
/**  接收操作 */
#define OP_ACCEPT  3
/** @}*/

/**  每一个连接处理数据 */
typedef struct _PER_HANDLE_DATA
{
  /**  对应的套接字句柄 */
  SOCKET      s;
  /**  对方的地址 */
  sockaddr_in addr;
}PER_HANDLE_DATA, *PPER_HANDLE_DATA;

/**  每一个IO数据结构 */
typedef struct _PER_IO_DATA
{
  /**  重叠结构 */
  OVERLAPPED  ol;
  /**  数据缓冲区 */
  char        buf[IOCP_BUFFER_SIZE];
  /**  操作类型 */
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