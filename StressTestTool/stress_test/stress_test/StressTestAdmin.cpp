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
    tstring strCmd1 = CBaseTool::tformat("\"%s\"",path.c_str());    
    strCmd1 = CBaseTool::all_replace(strCmd1,tstring("/"),tstring("\\\\"));
    int iRet = 0;
    if (path.length() > 0)
    {
        iRet = system(strCmd1.c_str());         //此为同步调用，阻塞式可以获得执行文件的返回值。默认0为成功，1为失败。
        //iRet = WinExec(strCmd2.c_str(), SW_SHOWNORMAL);     //经过测试，WinExec是异步执行bat，非阻塞不满足此处的需求。返回值大于31，则表示运行成功。
        if (iRet != 0)
        {
            global::WriteLog(ll_warning,"执行脚本文件=[%s]失败，返回码=[%d]", path.c_str(), iRet);
        }
    }
    return iRet;
}

int CStressTestAdmin::ProgressThreadFunc(const int &iRealExecuteSum, const CStressUnitPointVector &csupv)
{
    FILETIME ftLastIdleTime;    //上一次闲置时间量
    FILETIME ftLastKernelTime;  //上一次内核时间量
    FILETIME ftLastUserTime;    //上一次用户时间量
    FILETIME ftIdleTime;        //闲置时间量
    FILETIME ftKernelTime;      //内核时间量
    FILETIME ftUserTime;        //用户时间量
    GetSystemTimes(&ftIdleTime, &ftKernelTime, &ftUserTime);
    ftLastIdleTime = ftIdleTime;
    ftLastKernelTime = ftKernelTime;
    ftLastUserTime = ftUserTime;
    long long llStartTime = CBaseTool::GetTimestamp_Milli();
    while(m_iProgressFlag == THREAD_RUNNING)
    {
        boost::this_thread::sleep(boost::posix_time::seconds(1));
        //统计当前总执行次数
        int iThisCountSum = 0;
        for (CStressUnitPointVector::const_iterator supvci = csupv.begin() ; supvci != csupv.end() ; supvci ++)
        {
            CStressUnit *pclsUnit = (*supvci);
            iThisCountSum += pclsUnit->GetCurrentCount();
        }
        //计算CPU
        GetSystemTimes(&ftIdleTime, &ftKernelTime, &ftUserTime);
        long long llIdleDiff = CBaseTool::CompareFileTime(ftLastIdleTime, ftIdleTime);
        long long llKernelDiff = CBaseTool::CompareFileTime(ftLastKernelTime, ftKernelTime);
        long long llUserDiff = CBaseTool::CompareFileTime(ftLastUserTime, ftUserTime);
        long long llCpuUse = (llKernelDiff + llUserDiff - llIdleDiff) * 100 / (llKernelDiff + llUserDiff);
        long long llCpuIdle = (llIdleDiff) * 100 / (llKernelDiff + llUserDiff);
        ftLastIdleTime = ftIdleTime;
        ftLastKernelTime = ftKernelTime;
        ftLastUserTime = ftUserTime;
        //计算速度
        long long llThisTamsp = CBaseTool::GetTimestamp_Milli();
        double dWorkSpeed = (double)iThisCountSum / ((double)(llThisTamsp - llStartTime) / 1000);
        //打印日志
        global::WriteLog(ll_info, "执行进度=[%d/%d]，当前速度(次/秒)=[%.2lf]，内存使用率=[%d%%]，CPU使用率=[%lld%%]，CPU空闲率=[%lld%%]",
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
    global::WriteLog(ll_info, "[%s] 即将执行...", stStressData.strCaseName.c_str());
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
    //初始化每个单元并分发测试用例明细
    int iEachUnitAmount = (int)vcWholeUseCaseDetail.size() / iThreadSum;   //每个线程分配执行次数
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
    int iRealExecuteSum = iEachUnitAmount * iThreadSum;     //计算最终真实的总执行次数
    //启动线程池执行压力测试
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
    //反初始化每个单元    
    for (CStressUnitPointVector::iterator supvi = vcStressUnitSet.begin() ; supvi != vcStressUnitSet.end() ; supvi ++)
    {
        CStressUnit *pclsUnit = (*supvi);
        if (pclsUnit != NULL)
        {
            pclsUnit->UninitAll();
            delete pclsUnit;
        }
    }

    //写测试结果日志
    double dUseTime = double(llEndStamp - llStartStamp) / 1000;
    global::WriteLog(ll_info, "[%s] 执行结束，处理线程数=[%d]，总执行次数=[%d]，总用时(秒)=[%.2lf]，平均每秒执行次数=[%.2lf]",
                     stStressData.strCaseName.c_str(), iThreadSum, iRealExecuteSum, dUseTime, ((double)iRealExecuteSum) / dUseTime);

    return 0;
}

