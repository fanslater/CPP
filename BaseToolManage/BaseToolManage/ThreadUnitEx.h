#pragma once
#include <process.h>
#include <Windows.h>
#include "MyBarrier.h"

class CThreadUnitEx
{
public:
    CThreadUnitEx( void );
    ~CThreadUnitEx( void );
private:
    void clean();
public:
    void StartThread();
    void StopThread();
    friend UINT WINAPI ThreadFunc( LPVOID pvParam );
private:
    int m_iThreadNum;
    int m_iThreadNeedRun;
    HANDLE* m_phdThread;
    CMyBarrier* m_pBarrier;
};

