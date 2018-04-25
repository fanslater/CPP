#include "stdafx.h"
#include "global.h"
#include "BaseTool.h"
#include "LogShell.h"
#include "KcbpVisitor.h"

CLogShell g_LogShell;

void test::test_kcbp()
{
    KcbpConfig c;
    c.strIp = "127.0.0.1";
    c.strName = "KCBPB01";
    c.strPort = "21000";
    c.strPwd = "888888";
    c.strReqQue = "req_otcts_test";
    c.strRespQue = "ans_otcts_test";
    c.strUser = "KCXP00";
    CKcbpVisitor kcbp;
    kcbp.SetConfig(c);
    tstring msg;
    int iRet = kcbp.Connect(msg);
    if (iRet != 0)
    {
        global::ShowWindow(msg);
    }
    kvMap mp;
    mp["IP"] = "192.168.205.214";
    mp["PATH"] = "stress_test.exe";
    mp["TIME_OUT_S"] = "5";
    iRet = kcbp.CallLbm_UseForTest("L2600246",mp,msg);
    if (iRet != 0)
    {
        global::ShowWindow(msg);
    }
    iRet = kcbp.Disconnect(msg);
    if (iRet != 0)
    {
        global::ShowWindow(msg);
    }
}

void test::test_log()
{
    global::WriteLog(ll_trace,"ll_trace");
    global::WriteLog(ll_debug,"ll_debug");
    global::WriteLog(ll_info,"ll_info");
    global::WriteLog(ll_warning,"ll_warning");
    global::WriteLog(ll_error,"ll_error");
    global::WriteLog(ll_fatal,"ll_fatal");
}

void global::ShowWindow(const tstring& data)
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

void global::ShowWindow(const char* format, ...)
{
    char szLogBuff[FORMAT_LEN] = { 0 };
    va_list arg_ptr;
    va_start(arg_ptr, format);
    vsprintf_s(szLogBuff, format, arg_ptr);
    va_end(arg_ptr);
    ShowWindow(tstring(szLogBuff));
}

void global::WriteLog(emLogLevel iLvl, const char *format, ...)
{
    char szLogBuff[FORMAT_LEN] = { 0 };
    va_list arg_ptr;
    va_start(arg_ptr, format);
    vsprintf_s(szLogBuff, format, arg_ptr);
    va_end(arg_ptr);
    WriteLog(iLvl, tstring(szLogBuff));
}

void global::WriteLog(emLogLevel iLvl, const tstring &data)
{
     g_LogShell.WriteLog(iLvl,data);
}
