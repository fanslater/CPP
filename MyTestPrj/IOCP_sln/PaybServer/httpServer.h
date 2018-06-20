#ifndef __httpServer_h__
#define __httpServer_h__

#include "IocpManager.h"
//#include "CKVContainer.hpp"
//#include "JsonTools.h"
//#include "..\webServ\ServiceMain.h"

#define DefHttpService "httpservice"

#define AnsForOTC2PIF_Head "HTTP/1.1 200 0K"\
                          "\nConnectlon:close"\
                          "\nDate: Thu, 13 Oct 2014 12:17:33 GMT"\
                          "\nServer: Apache-Coyote/1.1"\
                          "\nLast-Nodified:Mon,22 Jun 1998 09;23;24 GMT"\
                          "\n\n"


//CKVContainer<tstring, tstring> g_kvHttpRespPack;

#define Http_Threads 3

//-----------------------------------------------------------------------------
class CHttpServ
{
private:
  CIocpManager m_IocpManager;
  boost::thread_group m_ThreadPool;//工作线程池
  boost::barrier* m_pBarrier;
  bool m_bThreadRunning;
public:
  CHttpServ();
  ~CHttpServ();
  void ShutDown();
  
  //-----------------------------------------------------------------------------
  int SendAnsToHttpClient(CSocketContext *pSocketContext);  

  int CreateHttpServer(const tstring& strIp, const tstring &strListenPort);
private:
  int ThreadHttpServ();
};

#endif