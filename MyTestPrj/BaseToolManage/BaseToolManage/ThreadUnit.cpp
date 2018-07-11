#include "StdAfx.h"
#include "ThreadUnit.h"
#include <Windows.h>

CThreadUnit::CThreadUnit( void )
{
    clean();
}

CThreadUnit::~CThreadUnit( void )
{
    clean();
}

void CThreadUnit::clean()
{
    m_iThreadNum = 0;
    m_iThreadNeedRun = 0;
    m_pBarrier = NULL;
}

int CThreadUnit::ThreadFunc()
{
    m_pBarrier->wait();
    while( m_iThreadNeedRun )
    {
        //线程工作
        Sleep(1);
    }
    return 0;
}

void CThreadUnit::StartThread()
{
    m_iThreadNum = 10;
    m_iThreadNeedRun = TRUE;
    m_pBarrier = new boost::barrier( m_iThreadNum + 1 );
    for( int i = 0; i < m_iThreadNum; i++ )
    {
        m_ThreadPool.create_thread( boost::bind( &CThreadUnit::ThreadFunc, boost::ref( this ) ) );
    }
    m_pBarrier->wait();
}

void CThreadUnit::StopThread()
{
    m_iThreadNeedRun = FALSE;
    m_ThreadPool.join_all();
    if( m_pBarrier )
    {
        delete m_pBarrier;
    }
}
