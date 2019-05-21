#ifndef __SynIocp_h__
#define __SynIocp_h__
#include "StringUtil.h"

#include <boost/thread.hpp> 
#include <boost/thread/barrier.hpp> 
#include "Queue.h"

/**  数据缓存区大小 */
#define BUFFER_SIZE 1024

/**  读操作 */
#define OP_READ    1

/**  写操作 */
#define OP_WRITE   2

/**  接收操作 */
#define OP_ACCEPT  3

/**  每一个连接处理数据 */
class CClientContext
{
public:
  CClientContext();

  /**  对应的套接字句柄 */
  SOCKET      m_sktClient;

  /**  对方的地址 */
  sockaddr_in addr;
};

/***********************************************************************************
*                                                                                  *
*				单句柄数据结构体定义(用于每一个完成端口，也就是每一个Socket的参数)         *
*                                                                                  *
************************************************************************************/
class CServerContext
{
public:
  CServerContext();
  tstring GetFunctionID();
  void FillBuffer(const tstring& strData);

  /**  重叠结构 */
  OVERLAPPED  ol;

  /**  数据缓冲区 */
  char        buf[BUFFER_SIZE];

  /**  操作类型 */
  int         nOperationType;

  CClientContext* pClientContext;
  tstring strRecv;
  int nTotalLen;
};

class CSynIocp
{
public:

  CSynIocp();
  ~CSynIocp();
  CServerContext* GetRequest();
  void ShutDown();

  int Initialize(const char *pszAddr, int nPort);
  int SendResponse(CClientContext* pClientContext, CServerContext* pServerContext, tstring &strAnsData);
protected:
  CSynIocp& operator=(const CSynIocp&);
  CSynIocp(const CSynIocp&);

  int ThreadListenClient();
  int ThreadWorker();
private:
  CQueue<CServerContext*> m_queueRecv;

  HANDLE m_hIoCompletion;
  SOCKET m_sktServer;
  volatile int m_nRunFlag;
  boost::thread_group m_ThreadPool;
  boost::barrier* m_pBarrier;
  bool m_bInitialized;
};

#endif