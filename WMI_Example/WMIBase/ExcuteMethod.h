#pragma once
#include "WMIBase/WMIExtern.h"
#include <string>
#include <map>
using namespace std;

typedef std::map<std::wstring, CComVariant> ParamsMap;

class CExcuteMethod : public CWMI
{
public:
    CExcuteMethod(const wstring& wszNamespace, const std::wstring& wstrClass, 
        const std::wstring& wstrInstanceName, const std::wstring& wstrMethod, const std::wstring& wstrRet, const ParamsMap& params);
    ~CExcuteMethod(void);
private:
    HRESULT Excute(CComPtr<IWbemServices> pSvc);
private:
    std::wstring m_wstrInstanceName;
    std::wstring m_wstrClassName;
    std::wstring m_wstrMethod;
    std::wstring m_wstrRet;
    ParamsMap m_params;
};
