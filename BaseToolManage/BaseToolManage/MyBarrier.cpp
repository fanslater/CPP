#include "StdAfx.h"
#include "MyBarrier.h"


CMyBarrier::CMyBarrier( int iCount )
{
    clean();
    m_iSumCount = iCount;
}

CMyBarrier::~CMyBarrier( void )
{
    if( m_hdEvent != NULL )
    {
        CloseHandle( m_hdEvent );
    }
    clean();
}

void CMyBarrier::clean()
{
    m_iSumCount = 0;
    m_iCurCount = 0;
    m_hdEvent = NULL;
}

bool CMyBarrier::wait()
{
    {
        CAutoLock lock( m_csLock );
        m_iCurCount++;
        if( m_iCurCount == m_iSumCount )
        {
            SetEvent( m_hdEvent );
            return true;
        }
        if( m_hdEvent == NULL )
        {
            m_hdEvent = CreateEvent( NULL, TRUE, FALSE, NULL );
        }
    }
    WaitForSingleObject( m_hdEvent, INFINITE );
    return false;
}
