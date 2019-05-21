#ifndef __SynIocp_h__
#define __SynIocp_h__
#include "StringUtil.h"

#include <boost/thread.hpp> 
#include <boost/thread/barrier.hpp> 
#include "Queue.h"

/**  ���ݻ�������С */
#define BUFFER_SIZE 1024

/**  ������ */
#define OP_READ    1

/**  д���� */
#define OP_WRITE   2

/**  ���ղ��� */
#define OP_ACCEPT  3

/**  ÿһ�����Ӵ������� */
class CClientContext
{
public:
  CClientContext();

  /**  ��Ӧ���׽��־�� */
  SOCKET      m_sktClient;

  /**  �Է��ĵ�ַ */
  sockaddr_in addr;
};

/***********************************************************************************
*                                                                                  *
*				��������ݽṹ�嶨��(����ÿһ����ɶ˿ڣ�Ҳ����ÿһ��Socket�Ĳ���)         *
*                                                                                  *
************************************************************************************/
class CServerContext
{
public:
  CServerContext();
  tstring GetFunctionID();
  void FillBuffer(const tstring& strData);

  /**  �ص��ṹ */
  OVERLAPPED  ol;

  /**  ���ݻ����� */
  char        buf[BUFFER_SIZE];

  /**  �������� */
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