#include "StdAfx.h"
#include "StressTestAdmin.h"
#include "SysCfgCtrl.h"
#include "CaseCfgCtrl.h"
#include "StressUnit.h"

CStressTestAdmin::CStressTestAdmin(void)
{
    clean();
}

CStressTestAdmin::~CStressTestAdmin(void)
{
    clean();
}

void CStressTestAdmin::clean()
{

}

void CStressTestAdmin::SetMainCfg(const tstring &path)
{
    m_strMainCfgPath = path;
}

int CStressTestAdmin::InitAll()
{
    global::InitLog();

    int iRet = 0;
    iRet = InitStressData();
    if (iRet)
    {
        return -1;
    }
    return 0;
}

int CStressTestAdmin::InitStressData()
{
    int iRet = 0;
    tstring strErrMsg;
    CSysCfgCtrl clsSysCfgCtrl;
    iRet = clsSysCfgCtrl.LoadCfg(m_strMainCfgPath, strErrMsg);
    if (iRet)
    {
        global::WriteLog(ll_error, "函数[%s]加载[%s]出现错误[%s]", __FUNCTION__, m_strMainCfgPath.c_str(), strErrMsg.c_str());
        return -1;
    }
    m_stKcbpConfig = clsSysCfgCtrl.GetKcbpCfg();
    UseCaseMap mpUseCase = clsSysCfgCtrl.GetCaseInfoSet();
    for (UseCaseMap::iterator ucmi = mpUseCase.begin() ; ucmi != mpUseCase.end() ; ucmi ++)
    {
        StressData stStressData;
        //用例总配
        stStressData.strCaseName = ucmi->first;
        stStressData.stUseCaseInfo = ucmi->second;
        //用例详配
        CCaseCfgCtrl clsCaseCfgCtrl;
        iRet = clsCaseCfgCtrl.LoadCfg(stStressData.stUseCaseInfo.strUseCaseFilePath, strErrMsg);
        if (iRet)
        {
            global::WriteLog(ll_error, "函数[%s]加载[%s]出现错误[%s]", __FUNCTION__, stStressData.stUseCaseInfo.strUseCaseFilePath.c_str(), strErrMsg.c_str());
            return -1;
        }
        stStressData.vcUseCaseDetail = clsCaseCfgCtrl.GetCaseDetailSet();
        m_vcStressData.push_back(stStressData);
    }
    return 0;
}

int CStressTestAdmin::UninitStressData()
{
    m_vcStressData.clear();
    return 0;
}

int CStressTestAdmin::ExecuteBatFile(const tstring &path)
{
    if (path.length() > 0)
    {
        WinExec(path.c_str(), SW_SHOWNORMAL);
    }
    return 0;
}

int CStressTestAdmin::UninitAll()
{
    UninitStressData();
    global::UninitLog();
    return 0;
}

int CStressTestAdmin::RunStressTests()
{
    for (StressDataVector::iterator sdvi = m_vcStressData.begin() ; sdvi != m_vcStressData.end() ; sdvi ++)
    {
        RunStressTest(*sdvi);
    }    
    return 0;
}

int CStressTestAdmin::RunStressTest(const StressData &stStressData)
{
    global::WriteLog(ll_info, "即将执行压测 [ %s ]", stStressData.strCaseName.c_str());
    //执行bat
    ExecuteBatFile(stStressData.stUseCaseInfo.strBatFilePath);
    //整理所有需要调用的用例明细
    int iExecuteSum = stStressData.stUseCaseInfo.iExecuteSum;
    int iThreadSum = stStressData.stUseCaseInfo.iThreadSum;
    int iUseCaseDetailNum = stStressData.vcUseCaseDetail.size();
    CaseDataPointerVector vcWholeUseCaseDetail;
    if (iExecuteSum > iUseCaseDetailNum)
    {
        for (int i = 0 ; i < iExecuteSum ; i++)
        {
            vcWholeUseCaseDetail.push_back(&stStressData.vcUseCaseDetail[i % iUseCaseDetailNum]);   //少则复制填充
        }
    }
    else
    {
        for (CaseDataVector::const_iterator cdvci = stStressData.vcUseCaseDetail.begin() ; cdvci != stStressData.vcUseCaseDetail.end() ; cdvci++)
        {
            vcWholeUseCaseDetail.push_back(&(*cdvci));  //多则继续
        }
    }
    //初始化每个线程对象
    std::vector<CStressUnit*> vcStressUnitSet;
    for (int i = 0; i < iThreadSum; i++)
    {
        CStressUnit* pclsUnit = new CStressUnit;
        pclsUnit->SetBpCfg(m_stKcbpConfig);
        pclsUnit->InitAll();
        vcStressUnitSet.push_back(pclsUnit);
    }
    //分发测试用例明细
    for (CaseDataPointerVector::iterator cdpvi = vcWholeUseCaseDetail.begin() ; cdpvi != vcWholeUseCaseDetail.end() ; cdpvi++)
    {
        int iCurIndex = cdpvi - vcWholeUseCaseDetail.begin();
        vcStressUnitSet[iCurIndex % iThreadSum]->AddOneTestData(*cdpvi);
    }
    //开启调用
    for (std::vector<CStressUnit*>::iterator suvi = vcStressUnitSet.begin() ; suvi != vcStressUnitSet.end() ; suvi ++)
    {
        CStressUnit *pclsUnit = (*suvi);
        pclsUnit->BeginTest();
    }
    //清理
    for (std::vector<CStressUnit*>::iterator suvi = vcStressUnitSet.begin() ; suvi != vcStressUnitSet.end() ; suvi ++)
    {
        CStressUnit *pclsUnit = (*suvi);
        if (pclsUnit != NULL)
        {
            delete pclsUnit;
        }        
    }
    vcStressUnitSet.clear();

    return 0;
}

