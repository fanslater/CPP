#pragma once
#include "WMIBase/SynNotifyQuery.h"

class CSynEventQuery : public CSynEventNotify
{
public:
    CSynEventQuery(const wstring& wszNamespace, const wstring& wszWQLQuery);
    ~CSynEventQuery(void);
private:
    HRESULT DealWithSingleItem( CComBSTR bstrName, CComVariant Value, CIMTYPE type, LONG lFlavor );
    HRESULT DealWithUnknownTypeItem( CComBSTR bstrName, CComVariant Value, CIMTYPE type, LONG lFlavor );
    VOID PrintfItem( CComBSTR bstrName, CComVariant Value, CIMTYPE type, LONG lFlavor );
};
