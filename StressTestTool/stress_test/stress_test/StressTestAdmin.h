#pragma once
#include "tstring.h"
#include "global.h"

class CStressTestAdmin
{
public:
    CStressTestAdmin(void);
    ~CStressTestAdmin(void);
private:
    void clean();
public:
    void SetMainCfg(const tstring& path);
    int InitAll();      
    int UninitAll();
    int RunStressTests();    
private:
    int InitStressData();
    int UninitStressData();
    int RunStressTest(const StressData& stStressData);
    int ExecuteBatFile(const tstring& path);
private:
    tstring m_strMainCfgPath;  
    KcbpConfig m_stKcbpConfig;
    StressDataVector m_vcStressData;
};
