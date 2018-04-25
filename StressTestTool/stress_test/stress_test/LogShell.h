#pragma once

#include <boost/thread.hpp> 
#include <boost/thread/barrier.hpp> 
#include "tstring.h"
#include "global.h"

#define LOG_DIR     "./log"

class CLogShell
{
public:
    CLogShell(void);
    ~CLogShell(void);
private:
    void CleanSelf();
    int WorkThreadFunc();    
    void StartWorkThread();
    void StopWorkThread();
public: 
    void InitAll();
    void UninitAll();    
    void WriteLog(emLogLevel iLvl,tstring strText);
private:
    int m_iThreadNum;
    int m_iThreadRunFlag;
    boost::thread_group m_ThreadPool;		
    boost::barrier* m_pBarrier;	
    LogDataCpcq m_queCatch;
};
