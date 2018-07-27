#pragma once
#include "WMIBase/SynQuery.h"

class CSynQueryData : public CSynQuery
{
public:
    CSynQueryData(const wstring& wszNamespace, const wstring& wszWQLQuery);
    ~CSynQueryData(void);
private:
    HRESULT DealWithSingleItem( CComBSTR bstrName, CComVariant Value, CIMTYPE type, LONG lFlavor );
    HRESULT DealWithUnknownTypeItem( CComBSTR bstrName, CComVariant Value, CIMTYPE type, LONG lFlavor );
    VOID PrintfItem( CComBSTR bstrName, CComVariant Value, CIMTYPE type, LONG lFlavor );
};
