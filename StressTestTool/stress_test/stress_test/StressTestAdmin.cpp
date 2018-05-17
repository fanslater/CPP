#include "StdAfx.h"
#include "StressTestAdmin.h"
#include "SysCfgCtrl.h"
#include "CaseCfgCtrl.h"
#include <boost/thread.hpp>
#include <boost/thread/barrier.hpp>


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
    m_iProgressFlag = 0;
    m_strMainCfgPath.clear();
    m_vcStressData.clear();
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

int CStressTestAdmin::ExecuteBatFile(const tstring &path)
{
    tstring strCmd1 = CBaseTool::tformat("\"%s\"",path.c_str());    
    strCmd1 = CBaseTool::all_replace(strCmd1,tstring("/"),tstring("\\\\"));
    int iRet = 0;
    if (path.length() > 0)
    {
        iRet = system(strCmd1.c_str());         //��Ϊͬ�����ã�����ʽ���Ի��ִ���ļ��ķ���ֵ��Ĭ��0Ϊ�ɹ���1Ϊʧ�ܡ�
        //iRet = WinExec(strCmd2.c_str(), SW_SHOWNORMAL);     //�������ԣ�WinExec���첽ִ��bat��������������˴������󡣷���ֵ����31�����ʾ���гɹ���
        if (iRet != 0)
        {
            global::WriteLog(ll_warning,"ִ�нű��ļ�=[%s]ʧ�ܣ�������=[%d]", path.c_str(), iRet);
        }
    }
    return iRet;
}

int CStressTestAdmin::ProgressThreadFunc(const int &iRealExecuteSum, const CStressUnitPointVector &csupv)
{
    FILETIME ftLastIdleTime;    //��һ������ʱ����
    FILETIME ftLastKernelTime;  //��һ���ں�ʱ����
    FILETIME ftLastUserTime;    //��һ���û�ʱ����
    FILETIME ftIdleTime;        //����ʱ����
    FILETIME ftKernelTime;      //�ں�ʱ����
    FILETIME ftUserTime;        //�û�ʱ����
    GetSystemTimes(&ftIdleTime, &ftKernelTime, &ftUserTime);
    ftLastIdleTime = ftIdleTime;
    ftLastKernelTime = ftKernelTime;
    ftLastUserTime = ftUserTime;
    long long llStartTime = CBaseTool::GetTimestamp_Milli();
    while(m_iProgressFlag == THREAD_RUNNING)
    {
        boost::this_thread::sleep(boost::posix_time::seconds(1));
        //ͳ�Ƶ�ǰ��ִ�д���
        int iThisCountSum = 0;
        for (CStressUnitPointVector::const_iterator supvci = csupv.begin() ; supvci != csupv.end() ; supvci ++)
        {
            CStressUnit *pclsUnit = (*supvci);
            iThisCountSum += pclsUnit->GetCurrentCount();
        }
        //����CPU
        GetSystemTimes(&ftIdleTime, &ftKernelTime, &ftUserTime);
        long long llIdleDiff = CBaseTool::CompareFileTime(ftLastIdleTime, ftIdleTime);
        long long llKernelDiff = CBaseTool::CompareFileTime(ftLastKernelTime, ftKernelTime);
        long long llUserDiff = CBaseTool::CompareFileTime(ftLastUserTime, ftUserTime);
        long long llCpuUse = (llKernelDiff + llUserDiff - llIdleDiff) * 100 / (llKernelDiff + llUserDiff);
        long long llCpuIdle = (llIdleDiff) * 100 / (llKernelDiff + llUserDiff);
        ftLastIdleTime = ftIdleTime;
        ftLastKernelTime = ftKernelTime;
        ftLastUserTime = ftUserTime;
        //�����ٶ�
        long long llThisTamsp = CBaseTool::GetTimestamp_Milli();
        double dWorkSpeed = (double)iThisCountSum / ((double)(llThisTamsp - llStartTime) / 1000);
        //��ӡ��־
        global::WriteLog(ll_info, "ִ�н���=[%d/%d]����ǰ�ٶ�(��/��)=[%.2lf]���ڴ�ʹ����=[%d%%]��CPUʹ����=[%lld%%]��CPU������=[%lld%%]",
                         iThisCountSum, iRealExecuteSum, dWorkSpeed, CBaseTool::GetMemoryPercent(), llCpuUse, llCpuIdle);
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
        global::WriteLog(ll_info, tstring("****************************"));
        RunStressTest(*sdvi);
        global::WriteLog(ll_info, tstring("****************************"));
    }
    return 0;
}

int CStressTestAdmin::RunStressTest(const StressData &stStressData)
{
    global::WriteLog(ll_info, "[%s] ����ִ��...", stStressData.strCaseName.c_str());
    //ִ��bat
    ExecuteBatFile(stStressData.stUseCaseInfo.strBatFilePath);
    //����������Ҫ���õ�������ϸ
    int iExecuteSum = stStressData.stUseCaseInfo.iExecuteSum;
    int iThreadSum = stStressData.stUseCaseInfo.iThreadSum;
    int iUseCaseDetailNum = stStressData.vcUseCaseDetail.size();
    CaseDataPointerVector vcWholeUseCaseDetail;
    if (iExecuteSum > iUseCaseDetailNum)
    {
        for (int i = 0 ; i < iExecuteSum ; i++)
        {
            vcWholeUseCaseDetail.push_back(&stStressData.vcUseCaseDetail[i % iUseCaseDetailNum]);   //���������
        }
    }
    else
    {
        for (CaseDataVector::const_iterator cdvci = stStressData.vcUseCaseDetail.begin() ; cdvci != stStressData.vcUseCaseDetail.end() ; cdvci++)
        {
            vcWholeUseCaseDetail.push_back(&(*cdvci));  //�������
        }
    }
    //��ʼ��ÿ����Ԫ���ַ�����������ϸ
    int iEachUnitAmount = (int)vcWholeUseCaseDetail.size() / iThreadSum;   //ÿ���̷߳���ִ�д���
    CStressUnitPointVector vcStressUnitSet;
    int iDetailIndex = 0;
    for (int i = 0; i < iThreadSum; i++)
    {
        CaseDataPointerVector vcEachUseCaseDetail(vcWholeUseCaseDetail.begin() + iDetailIndex, vcWholeUseCaseDetail.begin() + iDetailIndex + iEachUnitAmount);                
        iDetailIndex += iEachUnitAmount;
        CStressUnit *pclsUnit = new CStressUnit;
        pclsUnit->SetBpCfg(m_stKcbpConfig);
        pclsUnit->InputTestData(vcEachUseCaseDetail);
        pclsUnit->InitAll();
        vcStressUnitSet.push_back(pclsUnit);
    }
    int iRealExecuteSum = iEachUnitAmount * iThreadSum;     //����������ʵ����ִ�д���
    //�����̳߳�ִ��ѹ������
    m_iProgressFlag = THREAD_RUNNING;
    boost::thread thdProgress(boost::bind(&CStressTestAdmin::ProgressThreadFunc, boost::ref(this), boost::ref(iRealExecuteSum), boost::ref(vcStressUnitSet)));
    long long llStartStamp = 0, llEndStamp = 0;
    {
        llStartStamp = CBaseTool::GetTimestamp_Milli();
        boost::thread_group ThreadPool;
        boost::barrier *pBarrier = new boost::barrier(iThreadSum + 1);
        for (int i = 0; i < iThreadSum; i++)
        {
            ThreadPool.create_thread( boost::bind( &CStressUnit::StressThreadFunc, boost::ref(vcStressUnitSet[i]), boost::ref(pBarrier)));
        }
        pBarrier->wait();
        ThreadPool.join_all();
        delete pBarrier;
        llEndStamp = CBaseTool::GetTimestamp_Milli();
    }
    m_iProgressFlag = THREAD_STOPPED;
    thdProgress.join();
    //����ʼ��ÿ����Ԫ    
    for (CStressUnitPointVector::iterator supvi = vcStressUnitSet.begin() ; supvi != vcStressUnitSet.end() ; supvi ++)
    {
        CStressUnit *pclsUnit = (*supvi);
        if (pclsUnit != NULL)
        {
            pclsUnit->UninitAll();
            delete pclsUnit;
        }
    }

    //д���Խ����־
    double dUseTime = double(llEndStamp - llStartStamp) / 1000;
    global::WriteLog(ll_info, "[%s] ִ�н����������߳���=[%d]����ִ�д���=[%d]������ʱ(��)=[%.2lf]��ƽ��ÿ��ִ�д���=[%.2lf]",
                     stStressData.strCaseName.c_str(), iThreadSum, iRealExecuteSum, dUseTime, ((double)iRealExecuteSum) / dUseTime);

    return 0;
}

