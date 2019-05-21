/******************************************************************************

                  ��Ȩ���� (C), 2010-2020, �����н�֤�Ƽ��ɷ����޹�˾

 ******************************************************************************
  �� �� ��   : IocpServer.h
  �� �� ��   : ����
  ��    ��   : �ƹ�Ԫ
  ��������   : 2016��12��10��
  ����޸�   :
  ��������   : �������ɶ˿�ʵ��
  �����б�   :
  �޸���ʷ   :
  1.��    ��   : 2016��12��10��
    ��    ��   : �ƹ�Ԫ
    �޸�����   : �����ļ�

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

	// ����֪ͨ�߳�ϵͳ�˳����¼���Ϊ���ܹ����õ��˳��߳�
	HANDLE m_hShutdownEvent;
  
	HANDLE m_hIocp;
	SOCKET m_sktServer;

  // �ͻ����������������������
  _lstClientContext m_ClientContext;
  DeclearMutex(ClientContextMutex);

	boost::thread_group m_ThreadPool;
	boost::barrier* m_pBarrier;
};

#endif

