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
        global::WriteLog(ll_error, "����[%s]����[%s]���ִ���[%s]", __FUNCTION__, m_strMainCfgPath.c_str(), strErrMsg.c_str());
        return -1;
    }
    m_stKcbpConfig = clsSysCfgCtrl.GetKcbpCfg();
    UseCaseMap mpUseCase = clsSysCfgCtrl.GetCaseInfoSet();
    for (UseCaseMap::iterator ucmi = mpUseCase.begin() ; ucmi != mpUseCase.end() ; ucmi ++)
    {
        StressData stStressData;
        //��������
        stStressData.strCaseName = ucmi->first;
        stStressData.stUseCaseInfo = ucmi->second;
        //��������
        CCaseCfgCtrl clsCaseCfgCtrl;
        iRet = clsCaseCfgCtrl.LoadCfg(stStressData.stUseCaseInfo.strUseCaseFilePath, strErrMsg);
        if (iRet)
        {
            global::WriteLog(ll_error, "����[%s]����[%s]���ִ���[%s]", __FUNCTION__, stStressData.stUseCaseInfo.strUseCaseFilePath.c_str(), strErrMsg.c_str());
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

int CStressTestAdmin::ExecuteBatFile(const tstring& path)
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

int CStressTestAdmin::RunStressTest(const StressData& stStressData)
{
    global::ShowWindow("����ִ��ѹ��[%s]",stStressData.strCaseName.c_str());
    //ִ��bat
    ExecuteBatFile(stStressData.stUseCaseInfo.strBatFilePath);        
    //��ʼ�����Ե�Ԫ
    int iExecuteSum = stStressData.stUseCaseInfo.iExecuteSum;
    int iThreadSum = stStressData.stUseCaseInfo.iThreadSum;
    int iUseCaseDetailNum = stStressData.vcUseCaseDetail.size();
    int iCopyNum = iExecuteSum / iUseCaseDetailNum;
    int iModNum = iExecuteSum % iUseCaseDetailNum;
    CaseDataPointerVector vcWholeUseCaseDetail;    
    while(iCopyNum--)
    {        
        for (CaseDataVector::const_iterator cdvci = stStressData.vcUseCaseDetail.begin() ; cdvci != stStressData.vcUseCaseDetail.end() ; cdvci++)
        {            
            vcWholeUseCaseDetail.push_back(&(*cdvci));
        }
    }  
        
    std::vector<CStressUnit> vcStressUnitSet;
    for (int i = 0; i < iThreadSum; i++)
    {
        
    }
    //��������
    

    return 0;
}

