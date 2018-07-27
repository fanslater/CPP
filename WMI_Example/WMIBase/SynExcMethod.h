#pragma once
#include "WMIBase/WMIExtern.h"
#include <string>
using namespace std;

class CSynQuery : public CWMI
{
public:
    CSynQuery(const wstring& wszNamespace, const wstring& wszWQLQuery, HANDLE hExitEvent);
    ~CSynQuery(void);
private:
    HRESULT Excute(CComPtr<IWbemServices> pSvc);
    virtual HRESULT DealWithIWbemClassObject(CComPtr<IWbemClassObject> pclsObj);
private:
     wstring m_wszWQLQuery;
};


