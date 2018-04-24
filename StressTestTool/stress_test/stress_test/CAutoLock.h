#pragma once
#include <stdio.h>  
#include <process.h>  
#include <windows.h>             

class CAutoLock
{
public:
    CAutoLock(CRITICAL_SECTION& session);
    CAutoLock(CRITICAL_SECTION* session);
    ~CAutoLock();
protected:
    CRITICAL_SECTION* m_pSession;
};



