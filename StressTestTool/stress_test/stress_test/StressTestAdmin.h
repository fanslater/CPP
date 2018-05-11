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
    int RunStressTest(const StressData& stStressData);
private:
    int InitStressData();
    int UninitStressData();
    int ExecuteBatFile(const tstring& path);
private:
    tstring m_strMainCfgPath;      
    StressDataVector m_vcStressData;
};
