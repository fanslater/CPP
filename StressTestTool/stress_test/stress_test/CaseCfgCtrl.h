#pragma once
#include "tstring.h"
#include "global.h"

class CCaseCfgCtrl
{
public:
    CCaseCfgCtrl(void);
    ~CCaseCfgCtrl(void);
private:
    void Clean();
public:
    int LoadCfg(const tstring& path ,tstring& errmsg);
    CaseDataVector GetCaseDetailSet();
private:
    CaseDataVector m_vcCaseDetails;
};
