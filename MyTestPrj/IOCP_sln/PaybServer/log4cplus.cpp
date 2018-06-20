#include "stdafx.h"
#include "log4cplus.h"

using namespace log4cplus;
using namespace log4cplus::helpers;

CLog* CLog::m_pInstance = 0;

CLog& CLog::GetInstance()
{
  BOOST_ASSERT(m_pInstance != 0);
  return *m_pInstance;
}

CLog::CLog(void)
{
}

CLog::~CLog(void)
{
}

void CLog::CreateInstance()
{
  if (0 == m_pInstance)
  {
    m_pInstance = new CLog;
    log4cplus::PropertyConfigurator::doConfigure(".\\cfglog.dat");
  }
}

void CLog::DestroyInstance()
{
  if (m_pInstance)
  {
    delete m_pInstance;
    m_pInstance = NULL;
  }
}

void  CLog::GetShowLogInfo(std::string &strLogInfo)
{
  if(!m_ShowQueue.empty())
  {
    strLogInfo = m_ShowQueue.wait_and_pop(strLogInfo, 0);
  }
}

void CLog::WriteLog(int nType, const std::string& strLog)
{ 
  log4cplus::Logger logger = log4cplus::Logger::getInstance("");
  switch(nType)
  {
  case LL_trace:
    LOG4CPLUS_TRACE(logger,  strLog.c_str()); 
    break;
  case LL_debug:
    LOG4CPLUS_DEBUG(logger,  strLog.c_str()); 
    break;
  case LL_info:
    LOG4CPLUS_INFO(logger,  strLog.c_str()); 
    break;
  case LL_warn:
    LOG4CPLUS_WARN(logger,  strLog.c_str()); 
    break;
  case LL_error:
    LOG4CPLUS_ERROR(logger,  strLog.c_str()); 
    break;
  case LL_fatal:
    LOG4CPLUS_FATAL(logger,  strLog.c_str()); 
    break;
  default:
    {
      char szMessage[128]={0};
#ifdef _DEBUG //如果是调试版本，在最后加个回车换行，要不然在VC里显示成一堆难看
      _snprintf(szMessage,sizeof(szMessage)-1,"[%04x][unknow log level:%d]\n",GetCurrentThreadId(), nType);
#else
      _snprintf(szMessage,sizeof(szMessage)-1,"[%04x][unknow log level:%d]",GetCurrentThreadId(),  nType);
#endif
      OutputDebugString(szMessage);
    }
    break;
  }//end of switch(nLevel)

  //写日志，带级别
  char szType[8] = {0};
  szType[0] = (unsigned char)nType; //复用字段，表示日志级别
  std::string strData = "~LOG~";
  strData += szType;
  strData += strLog;

  //日志文件使用内部日志队列
  m_ShowQueue.push(strData);
}