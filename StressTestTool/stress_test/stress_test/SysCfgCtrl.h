#pragma once
#include "global.h"

class CSysCfgCtrl
{
public:
    CSysCfgCtrl(void);
    ~CSysCfgCtrl(void);
private:
    void CleanSelf();
public:
    int LoadCfg(const tstring& path ,tstring& errmsg);
    KcbpConfig GetKcbpCfg();
    UseCaseMap GetCaseInfoSet();
    UseCaseInfo GetCaseInfo(const tstring& strCaseName);    
private:
    KcbpConfig m_stKcbpCfg;
    UseCaseMap m_mpCaseInfos;
};
