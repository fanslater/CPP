#pragma once
#include "global.h"
#include "KcbpVisitor.h"
#include <boost/thread/barrier.hpp>

class CStressUnit
{
public:
    CStressUnit(void);    
    ~CStressUnit(void);
private:
    void clean();
public:   
    int SetBpCfg(const KcbpConfig& bp);
    int InitAll();
    int UninitAll();
    int InputTestData(const CaseDataPointerVector& data);    
    int StressThreadFunc(boost::barrier* pBarrier);
    int GetCurrentCount();
private:
    KcbpConfig m_BpCfg;
    CKcbpVisitor m_clsKcbpVisitor;
    CaseDataPointerVector m_vcCaseDataSet;
    int m_iCount;
};
