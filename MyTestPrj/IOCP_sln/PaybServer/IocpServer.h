/******************************************************************************

                  版权所有 (C), 2010-2020, 深圳市金证科技股份有限公司

 ******************************************************************************
  文 件 名   : IocpServer.h
  版 本 号   : 初稿
  作    者   : 黄共元
  生成日期   : 2016年12月10日
  最近修改   :
  功能描述   : 服务端完成端口实现
  函数列表   :
  修改历史   :
  1.日    期   : 2016年12月10日
    作    者   : 黄共元
    修改内容   : 创建文件

******************************************************************************/
#ifndef __IocpServer_h__
#define __IocpServer_h__

#include <boost/thread.hpp> 
#include <boost/thread/barrier.hpp> 
#include <boost/smart_ptr.hpp>
#include <boost/thread/recursive_mutex.hpp>

//using namespace std;
#include "Queue.h"
#include "ServerContext.h"

class CIocpServer
{
public:
  CIocpServer();
  ~CIocpServer();

  int Start(const tchar *pszAddr, int nPort);
  int Shutdown();
private:
  int ThreadIocpService();
  int InitServerSocket(const tchar *pszAddr, int nPort);
  void RemoveClientContext(CClientContext* pClientContext);
  int NewClientContext();
  void CheckTimeOutClient();
private:
  volatile long m_lAcceptExCounter;

  bool m_bInitialized;
  int m_nWorkThreads;
	bool m_nThreadRunning;

	// 用来通知线程系统退出的事件，为了能够更好的退出线程
	HANDLE m_hShutdownEvent;
  
	HANDLE m_hIocp;
	SOCKET m_sktServer;

  // 客户端网络操作的上下文数据
  _lstClientContext m_ClientContext;
  DeclearMutex(ClientContextMutex);

	boost::thread_group m_ThreadPool;
	boost::barrier* m_pBarrier;
};

#endif

