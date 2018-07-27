#pragma once
#include "SinkExtern.h"

class CInstanceEvent : public CSink
{
public:
    CInstanceEvent(void);
    ~CInstanceEvent(void);
private:
    HRESULT DealWithSingleItem( CComBSTR bstrName, CComVariant Value, CIMTYPE type, LONG lFlavor );
    HRESULT DealWithUnknownTypeItem( CComBSTR bstrName, CComVariant Value, CIMTYPE type, LONG lFlavor );
    VOID PrintfItem( CComBSTR bstrName, CComVariant Value, CIMTYPE type, LONG lFlavor );
};
