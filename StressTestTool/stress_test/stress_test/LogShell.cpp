#include "StdAfx.h"
#include "LogShell.h"
#include <log4cplus/config.hxx>
#include <log4cplus/configurator.h>
#include <log4cplus/consoleappender.h>
#include <log4cplus/socketappender.h>
#include <log4cplus/helpers/loglog.h>
#include <log4cplus/helpers/socket.h>
#include <log4cplus/thread/threads.h>
#include <log4cplus/spi/loggerimpl.h>
#include <log4cplus/spi/loggingevent.h>
#include <log4cplus/loggingmacros.h>

CLogShell::CLogShell(void)
{
    CleanSelf();   
}

CLogShell::~CLogShell(void)
{    
    CleanSelf();
}

void CLogShell::CleanSelf()
{
    m_iThreadNum = 1;
}

int CLogShell::WorkThreadFunc()
{    
    log4cplus::PropertyConfigurator::doConfigure(path_log4plus_cfg);
    log4cplus::Logger logger = log4cplus::Logger::getInstance("stress_test");

    m_pBarrier->wait();
    while ( THREAD_RUNNING == m_iThreadRunFlag )
    {
        LogData data;
        if (m_queCatch.wait_and_pop(data,LOGSHELL_POP_INTERVAL))
        {                
            switch(data.ilvl)
            {
            case ll_trace:
                LOG4CPLUS_TRACE(logger,  data.strText.c_str());
                break;
            case ll_debug:
                LOG4CPLUS_DEBUG(logger,  data.strText.c_str());
                break;
            case ll_info:
                LOG4CPLUS_INFO(logger,  data.strText.c_str());
                break;
            case ll_warning:
                LOG4CPLUS_WARN(logger,  data.strText.c_str());
                break;
            case ll_error:
                LOG4CPLUS_ERROR(logger,  data.strText.c_str());
                break;
            case ll_fatal:
                LOG4CPLUS_FATAL(logger,  data.strText.c_str());
                break;
            default:               
                break;
            }
        }     
        boost::this_thread::sleep(boost::posix_time::milliseconds(WRITE_LOG_INTERVAL));
    }
    return 0;
}

void CLogShell::StartWorkThread()
{
    m_iThreadRunFlag = THREAD_RUNNING;
    m_pBarrier = new boost::barrier(m_iThreadNum + 1);
    for (int i = 0; i < m_iThreadNum; i++)
    {
        m_ThreadPool.create_thread( boost::bind( &CLogShell::WorkThreadFunc, boost::ref(this) ));
    }
    m_pBarrier->wait();
}

void CLogShell::StopWorkThread()
{
    m_iThreadRunFlag = THREAD_STOPPED;
    m_ThreadPool.join_all();
    if (m_pBarrier)
    {
        delete m_pBarrier;
    }
}

void CLogShell::InitAll()
{
    if (!CBaseTool::IsPathExists(LOG_DIR))
    {
        CBaseTool::CreatePath(LOG_DIR);
    }
    StartWorkThread();
}

void CLogShell::UninitAll()
{
    StopWorkThread();
}

void CLogShell::WriteLog(emLogLevel iLvl,tstring strText)
{
    if (iLvl>= ll_trace && iLvl <= ll_fatal && strText.length() > 0)
    {
        LogData data;
        data.ilvl = iLvl;
        data.strText = strText;
        m_queCatch.push(data);
    }
}
