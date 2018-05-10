#include "StdAfx.h"
#include "UnitTest.h"
#include "global.h"
#include "KcbpVisitor.h"
#include "SysCfgCtrl.h"
#include "CaseCfgCtrl.h"

CUnitTest::CUnitTest(void)
{
}

CUnitTest::~CUnitTest(void)
{
}

void CUnitTest::test_kcbp()
{
    KcbpConfig c;
    c.strIp = "127.0.0.1";
    c.strName = "KCBPB01";
    c.strPort = "21000";
    c.strPwd = "888888";
    c.strReqQue = "req_otcts_test";
    c.strRespQue = "ans_otcts_test";
    c.strUser = "KCXP00";
    c.strProtocal = "0";
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
    kvMap ret;
    iRet = kcbp.CallLbm_Result1("L2600246", mp, ret, msg);
    if (iRet != 0)
    {
        global::ShowWindow(msg);
    }
    global::ShowWindow("CODE=[%s] TEXT=[%s]",ret["MSG_CODE"].c_str(),ret["MSG_TEXT"].c_str());
    iRet = kcbp.Disconnect(msg);
    if (iRet != 0)
    {
        global::ShowWindow(msg);
    }
}

void CUnitTest::test_log()
{
    global::WriteLog(ll_trace, "ll_trace");
    global::WriteLog(ll_debug, "ll_debug");
    global::WriteLog(ll_info, "ll_info");
    global::WriteLog(ll_warning, "ll_warning");
    global::WriteLog(ll_error, "ll_error");
    global::WriteLog(ll_fatal, "ll_fatal");
}

void CUnitTest::test_cfgCtrl()
{
    CSysCfgCtrl cls;
    tstring msg;
    cls.LoadCfg(path_default_cfg, msg);
    KcbpConfig st = cls.GetKcbpCfg();
    UseCaseMap ucm = cls.GetCaseInfoSet();
    UseCaseInfo info = cls.GetCaseInfo("case3");
    CCaseCfgCtrl Case;
    Case.LoadCfg(info.strUseCaseFile,msg);
    CaseDataVector cdm = Case.GetCaseDetailSet();    
}

