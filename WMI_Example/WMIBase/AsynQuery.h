#pragma once
#include "WMIBase/WMIExtern.h"
#include <string>
using namespace std;

template<typename T>
class CAsynQuery : public CWMI
{
public:
    CAsynQuery(const wstring& wszNamespace, const wstring& wszWQLQuery, HANDLE hExitEvent);
    ~CAsynQuery(void);
private:
    HRESULT Excute(CComPtr<IWbemServices> pSvc);
private:
     wstring m_wszWQLQuery;
     HANDLE m_hExitHandle;
};
