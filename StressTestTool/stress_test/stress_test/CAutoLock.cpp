#include "stdafx.h"
#include "CAutoLock.h"




CAutoLock::CAutoLock(CRITICAL_SECTION& session)
{
    m_pSession = &session;
    InitializeCriticalSection(m_pSession);
    EnterCriticalSection(m_pSession);
}

CAutoLock::CAutoLock(CRITICAL_SECTION* session)
{
    m_pSession = session;
    InitializeCriticalSection(m_pSession);
    EnterCriticalSection(m_pSession);
}

CAutoLock::~CAutoLock()
{
    LeaveCriticalSection(m_pSession);
    DeleteCriticalSection(m_pSession);
    m_pSession = NULL;
}
