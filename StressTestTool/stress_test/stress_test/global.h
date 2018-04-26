#pragma once

#include "CPCQueue.hpp"
#include "json/json.h"
#include "CAutoLock.h"

#define path_default_cfg        "./config.ini"
#define path_log4plus_cfg       "./cfglog.dat"

#define THREAD_STOPPED		    0
#define THREAD_RUNNING		    1

#define FORMAT_LEN              8*1024
#define WRITE_LOG_INTERVAL      1        
#define LOGSHELL_POP_INTERVAL   1

enum emLogLevel
{
    ll_trace = 0,
    ll_debug,
    ll_info, 
    ll_warning, 
    ll_error,
    ll_fatal
};

struct LogData
{
    emLogLevel ilvl;
    tstring strText;
};
typedef CPCQueue<LogData> LogDataCpcq;
typedef std::map<tstring,tstring> kvMap;
typedef std::pair<tstring,tstring> kvPair;

struct KcbpConfig
{
    tstring strIp;
    tstring strPort;
    tstring strName;
    tstring strReqQue;
    tstring strRespQue;
    tstring strUser;
    tstring strPwd;
    tstring strProtocal;
    KcbpConfig()
    {
        clear();
    };
    ~KcbpConfig()
    {
        clear();
    };
    void clear()
    {
        strIp.clear();
        strPort.clear();
        strName.clear();
        strReqQue.clear();
        strRespQue.clear();
        strUser.clear();
        strPwd.clear();
        strProtocal.clear();
    }
};

struct UseCaseInfo
{
    tstring strBatFile;
    tstring strUseCaseFile;
    int iThreadSum;
    int iExecuteSum;
};
typedef std::map<tstring,UseCaseInfo> UseCaseMap;
typedef std::pair<tstring,UseCaseInfo> UseCasePair;

/************************************************************************/
namespace global
{    
    void ShowWindow(const tstring& data);
    void ShowWindow(const char* format, ...);
    void InitLog();
    void UninitLog();
    void WriteLog(emLogLevel iLvl, const tstring& data);
    void WriteLog(emLogLevel iLvl, const char* format, ...);
}

namespace test
{
    void test_kcbp();
    void test_log();
    void test_cfgCtrl();
}

/************************************************************************/
/*


1. 这个方法只能在线程中用, 在主线程中用无效.
boost::this_thread::sleep(boost::posix_time::seconds(2));    // 这种更好用
boost::this_thread::sleep(boost::get_system_time() + boost::posix_time::seconds(2));

2.在主线程中使用.
boost::thread::sleep(boost::get_system_time() + boost::posix_time::seconds(5));



*/