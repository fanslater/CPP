#ifndef __log4cplus_h__
#define __log4cplus_h__

#include <log4cplus/config.hxx>
#include <log4cplus/configurator.h>
#include <log4cplus/consoleappender.h>
#include <log4cplus/socketappender.h>
#include <log4cplus/helpers/loglog.h>
#include <log4cplus/helpers/socket.h>
#include <log4cplus/helpers/threads.h>
#include <log4cplus/spi/loggerimpl.h>
#include <log4cplus/spi/loggingevent.h>

#include "StringUtil.h"
#include "Queue.h"

enum em_Log_Level
{
  LL_trace = '0',
  LL_debug,
  LL_info, 
  LL_warn, 
  LL_error,
  LL_fatal
};

class CLog
{
public:
  CLog(void);
  ~CLog(void);

public:
  static void CreateInstance();
  static void DestroyInstance();
  static CLog& GetInstance();

  void WriteLog(int nType, const char *p_pszLog);
  void WriteLog(int nType, const std::string& strLog);
  void GetShowLogInfo(std::string &strLogInfo);

private:
  static CLog* m_pInstance;
public:
  CQueue<tstring> m_ShowQueue;   //日志显示队列
};

#define UPD_WriteLog(nType, strLog) CLog::GetInstance().WriteLog(nType, InternalType2String<long>(__LINE__) +":" + strLog)

#define UPD_TraceLog(strLog) CLog::GetInstance().WriteLog(LL_trace, InternalType2String<long>(__LINE__) +":" + strLog)
#define UPD_DebugLog(strLog) CLog::GetInstance().WriteLog(LL_debug, InternalType2String<long>(__LINE__) +":" + strLog)
#define UPD_InfoLog(strLog)  CLog::GetInstance().WriteLog(LL_info , InternalType2String<long>(__LINE__) +":" + strLog)
#define UPD_WarnLog(strLog)  CLog::GetInstance().WriteLog(LL_warn , InternalType2String<long>(__LINE__) +":" + strLog)
#define UPD_ErrorLog(strLog) CLog::GetInstance().WriteLog(LL_error, InternalType2String<long>(__LINE__) +":" + strLog)
#define UPD_FatalLog(strLog) CLog::GetInstance().WriteLog(LL_fatal, InternalType2String<long>(__LINE__) +":" + strLog)

#define UPD_UpdateLog(strLog) UPD_TraceLog(strLog)


#endif