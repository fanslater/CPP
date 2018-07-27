#pragma once
#include "WMIBase/WMIExtern.h"
#include <string>
using namespace std;

class CSynEventNotify : public CWMI
{
public:
    CSynEventNotify(const wstring& wszNamespace, const wstring& wszWQLQuery);
    ~CSynEventNotify(void);
private:
    HRESULT Excute(CComPtr<IWbemServices> pSvc);
    HRESULT DealWithIWbemClassObject(CComPtr<IWbemClassObject> pclsObj);
    virtual HRESULT DealWithSingleItem( CComBSTR bstrName, CComVariant Value, CIMTYPE type, LONG lFlavor );
private:
     wstring m_wszWQLQuery;
};


