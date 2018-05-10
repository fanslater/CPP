#include "stdafx.h"
#include "global.h"
#include "BaseTool.h"
#include "LogShell.h"

CLogShell g_LogShell;

void global::ShowWindow(const tstring &data)
{
    tstring timestamp = CBaseTool::GetPhysicsDateTime_AsStr();
#ifdef _CONSOLE
    printf("[%s] %s\n", timestamp.c_str(), data.c_str());
#endif
#ifdef _WINDOWS

#endif
}

void global::InitLog()
{
    g_LogShell.InitAll();
}

void global::UninitLog()
{
    g_LogShell.UninitAll();
}

void global::ShowWindow(const char *format, ...)
{
    char szLogBuff[FORMAT_LEN] = { 0 };
    va_list arg_ptr;
    va_start(arg_ptr, format);
    vsnprintf_s(szLogBuff, sizeof(szLogBuff) - 1, _TRUNCATE, format, arg_ptr);
    va_end(arg_ptr);
    ShowWindow(tstring(szLogBuff));
}

void global::WriteLog(emLogLevel iLvl, const char *format, ...)
{
    char szLogBuff[FORMAT_LEN] = { 0 };
    va_list arg_ptr;
    va_start(arg_ptr, format);
    vsnprintf_s(szLogBuff, sizeof(szLogBuff) - 1, _TRUNCATE, format, arg_ptr);
    va_end(arg_ptr);
    WriteLog(iLvl, tstring(szLogBuff));
}

void global::WriteLog(emLogLevel iLvl, const tstring &data)
{
    g_LogShell.WriteLog(iLvl, data);
}
