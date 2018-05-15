#pragma once
#include "global.h"
#include "KcbpVisitor.h"
#include <boost/thread.hpp> 
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
    int AddOneTestData(const CaseDetail* data);
    int BeginTest();
private:
    KcbpConfig m_BpCfg;
    CKcbpVisitor m_clsKcbpVisitor;
    CaseDataPointerVector m_vcCaseDataSet;
private:
    int WorkThreadFunc();    
private:
    boost::thread_group m_ThreadPool;		
    boost::barrier* m_pBarrier;
};
