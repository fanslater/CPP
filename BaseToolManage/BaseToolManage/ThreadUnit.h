#pragma once

#include <boost/thread.hpp>
#include <boost/thread/barrier.hpp>

class CThreadUnit
{
public:
    CThreadUnit( void );
    ~CThreadUnit( void );
private:
    void clean();
public:
    int ThreadFunc();
    void StartThread();
    void StopThread();
private:
    int m_iThreadNum;
    int m_iThreadNeedRun;
    boost::thread_group m_ThreadPool;
    boost::barrier* m_pBarrier;
};
