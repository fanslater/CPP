#include "StdAfx.h"
#include "ThreadUnitEx.h"



CThreadUnitEx::CThreadUnitEx( void )
{
    clean();
}

CThreadUnitEx::~CThreadUnitEx( void )
{
    clean();
}

void CThreadUnitEx::clean()
{
    m_iThreadNum = 0;
    m_iThreadNeedRun = 0;
    m_phdThread = NULL;
}

void CThreadUnitEx::StartThread()
{
    m_iThreadNum = 10;
    m_phdThread = new HANDLE[m_iThreadNum];
    m_pBarrier = new CMyBarrier( m_iThreadNum + 1 );
    m_iThreadNeedRun = TRUE;
    for( int i = 0 ; i < m_iThreadNum ; i++ )
    {
        m_phdThread[i] = ( HANDLE )_beginthreadex( NULL, 0, ThreadFunc, this, 0, NULL );
        printf( "begin[%d]\n", i );
    }
    m_pBarrier->wait();
}

void CThreadUnitEx::StopThread()
{
    m_iThreadNeedRun = FALSE;
    WaitForMultipleObjects( m_iThreadNum, m_phdThread, TRUE, INFINITE );
    if( m_phdThread )
    {
        delete[] m_phdThread;
    }
    if( m_pBarrier )
    {
        delete m_pBarrier;
    }
}

UINT WINAPI ThreadFunc( LPVOID pvParam )
{
    CThreadUnitEx* pCls = ( CThreadUnitEx* )pvParam;
    pCls->m_pBarrier->wait();
    printf( "wait over [%d]\n", GetCurrentThreadId() );
    while( pCls->m_iThreadNeedRun )
    {
        //线程处理
        Sleep( 1000 );
        printf( "this is thread [%d] msg\n", GetCurrentThreadId() );
    }
    return 0;
}
