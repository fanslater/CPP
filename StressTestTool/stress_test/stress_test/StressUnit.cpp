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
    m_iCount = 0;
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

int CStressUnit::InputTestData(const CaseDataPointerVector& data)
{
    m_vcCaseDataSet.assign(data.begin(),data.end());
    return 0;
}

int CStressUnit::StressThreadFunc(boost::barrier *pBarrier)
{
    int iRet = 0;
    tstring strErrMsg;
    pBarrier->wait();
    for (CaseDataPointerVector::iterator cdpvi = m_vcCaseDataSet.begin() ; cdpvi != m_vcCaseDataSet.end(); cdpvi ++)
    {
        const CaseDetail *pCaseDetail = (*cdpvi);
        //global::ShowWindow("线程[%d]正在调用lbm=[%s]", boost::this_thread::get_id(), pCaseDetail->strLbmNo.c_str());
        kvMap mpResult;
        iRet = m_clsKcbpVisitor.CallLbm_Result1(pCaseDetail->strLbmNo, pCaseDetail->mpFields, mpResult, strErrMsg);
        m_iCount++;
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

int CStressUnit::GetCurrentCount()
{
    return m_iCount;
}
