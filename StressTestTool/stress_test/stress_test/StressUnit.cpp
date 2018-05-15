#include "StdAfx.h"
#include "StressUnit.h"

CStressUnit::CStressUnit(void)
{
    clean();
}

CStressUnit::~CStressUnit(void)
{
    clean();
}

void CStressUnit::clean()
{
    m_vcCaseDataSet.clear();
}

int CStressUnit::SetBpCfg(const KcbpConfig &bp)
{
    m_BpCfg = bp;
    return 0;
}

int CStressUnit::InitAll()
{
    int iRet = 0;
    tstring strErrMsg;
    m_clsKcbpVisitor.SetConfig(m_BpCfg);
    iRet = m_clsKcbpVisitor.Connect(strErrMsg);
    if (iRet != 0)
    {
        global::WriteLog(ll_error, strErrMsg);
        return -1;
    }
    return 0;
}

int CStressUnit::UninitAll()
{
    int iRet = 0;
    tstring strErrMsg;
    iRet = m_clsKcbpVisitor.Disconnect(strErrMsg);
    if (iRet != 0)
    {
        global::WriteLog(ll_error, strErrMsg);
        return -1;
    }
    return 0;
}

int CStressUnit::AddOneTestData(const CaseDetail *data)
{
    m_vcCaseDataSet.push_back(data);
    return 0;
}

int CStressUnit::BeginTest()
{
    m_pBarrier = new boost::barrier(2);
    m_ThreadPool.create_thread( boost::bind( &CStressUnit::WorkThreadFunc, boost::ref(this) ));
    m_pBarrier->wait();
    m_ThreadPool.join_all();
    if (m_pBarrier)
    {
        delete m_pBarrier;
    }
    return 0;
}

int CStressUnit::WorkThreadFunc()
{
    int iRet = 0;
    tstring strErrMsg;
    m_pBarrier->wait();
    for (CaseDataPointerVector::iterator cdpvi = m_vcCaseDataSet.begin() ; cdpvi != m_vcCaseDataSet.end(); cdpvi ++)
    {
        const CaseDetail *pCaseDetail = (*cdpvi);
        //global::ShowWindow("正在调用lbm=[%s]", pCaseDetail->strLbmNo.c_str());
        kvMap mpResult;
        iRet = m_clsKcbpVisitor.CallLbm_Result1(pCaseDetail->strLbmNo, pCaseDetail->mpFields, mpResult, strErrMsg);
        if (iRet != 0)
        {
            global::WriteLog(ll_warning, "call lbm=[%s] defeat, errmsg=[%s]", pCaseDetail->strLbmNo.c_str(), strErrMsg.c_str());
            continue;
        }
        tstring strMsgCode = mpResult["MSG_CODE"];
        tstring strMsgText = mpResult["MSG_TEXT"];
        if (strMsgCode.compare("0") != 0)
        {
            global::WriteLog(ll_warning, "call lbm=[%s] success, but MsgCode=[%s] MsgText=[%s]", pCaseDetail->strLbmNo.c_str(), strMsgCode.c_str(), strMsgText.c_str());
            continue;
        }
    }
    return 0;
}

