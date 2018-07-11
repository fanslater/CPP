#pragma once
#include "CAutoLock.h"

class CMyBarrier
{
public:        
    CMyBarrier(int iCount);
    ~CMyBarrier(void);
private:
    void clean();
public:
    bool wait();
private:
    CRITICAL_SECTION m_csLock;
    int m_iSumCount;
    int m_iCurCount;
    HANDLE m_hdEvent;
};
