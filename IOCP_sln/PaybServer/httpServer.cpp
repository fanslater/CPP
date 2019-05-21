#include "stdafx.h"

#include <sstream>
#include <fstream>
#include "httpServer.h"

CHttpServ::CHttpServ()
{
  m_pBarrier = 0;
  m_bThreadRunning = true;
}
CHttpServ::~CHttpServ()
{
  delete m_pBarrier;
  m_pBarrier = 0;
}

void CHttpServ::ShutDown()
{
  m_IocpManager.ShutDown();
  m_bThreadRunning = false;
  m_ThreadPool.join_all();
}

//-----------------------------------------------------------------------------
int CHttpServ::SendAnsToHttpClient(CSocketContext *pSocketContext)
{//取数据
  try
  {
    //组应答包发送到客户端
    tstring strAnsPack(AnsForOTC2PIF_Head);

    std::ifstream XmlFile;
    tstring strXmlName( "." + pSocketContext->GetFunctionID() );
    XmlFile.open(strXmlName.c_str(), std::ios::in);

		if (XmlFile.is_open())
		{
		  tstringstream ss;
      ss << XmlFile.rdbuf();
      strAnsPack += ss.str();
		}
		else
		{
		  strAnsPack += "error request.";
		}
		m_IocpManager.SendResponse(pSocketContext->pIoContext, pSocketContext, strAnsPack);
    WriteLog("服务端应答:\n" + strAnsPack);
  }
  catch(std::exception &e)
  {
    WriteLog( e.what() );
  }

  return 0;
}

int CHttpServ::CreateHttpServer(const tstring& strIp, const tstring &strListenPort)
{
  if(m_IocpManager.Initialize(strIp.c_str(), atoi(strListenPort.c_str())))
    return -1;
  m_pBarrier = new boost::barrier(Http_Threads + 1);

  for(int i = 0; i < Http_Threads; i++)
  {
    m_ThreadPool.create_thread( boost::bind(&CHttpServ::ThreadHttpServ, this) );
  }
  m_pBarrier->wait();

  WriteLog("........HttpServer listening at [" + strListenPort + "]");
  return 0;
}

//多线程测试运行
int CHttpServ::ThreadHttpServ()
{
  m_pBarrier->wait();

  while(m_bThreadRunning)
  {
    CSocketContext *pSocketContext = m_IocpManager.GetRequest();

    if ( NULL == pSocketContext)
    {
      continue;
    }
    WriteLog("----------客户端请求:\n" + pSocketContext->strRecv);
    SendAnsToHttpClient(pSocketContext);
  }
  return 0;
}
