#pragma once
#include "WMIBase/WMIExtern.h"
#include <string>
using namespace std;

template<typename T>
class CAsynNotifyQuery : public CWMI
{
public:
    CAsynNotifyQuery(const wstring& wszNamespace, const wstring& wszWQLQuery, HANDLE hExitEvent);
    ~CAsynNotifyQuery(void);
private:
    HRESULT Excute(CComPtr<IWbemServices> pSvc);
private:
     wstring m_wszWQLQuery;
     HANDLE m_hExitHandle;
};
