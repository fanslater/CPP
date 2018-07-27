#include "SynEventQuery.h"

CSynEventQuery::CSynEventQuery(const wstring& wszNamespace, const wstring& wszWQLQuery):CSynEventNotify(wszNamespace,wszWQLQuery)
{
}

CSynEventQuery::~CSynEventQuery(void)
{
}

HRESULT CSynEventQuery::DealWithSingleItem( CComBSTR bstrName, CComVariant Value, CIMTYPE type, LONG lFlavor )
{
    HRESULT hr = WBEM_S_NO_ERROR; 
    switch ( Value.vt ) {
        case VT_UNKNOWN : {
            DealWithUnknownTypeItem(bstrName, Value, type, lFlavor );
            }break;
        default: {
            };
    }
    return hr;
}

HRESULT CSynEventQuery::DealWithUnknownTypeItem( CComBSTR bstrName, CComVariant Value, CIMTYPE type, LONG lFlavor )
{
    HRESULT hr = WBEM_S_NO_ERROR;
    if ( NULL == Value.punkVal ) {
        return hr;
    }
    // object类型转换成IWbemClassObject接口指针，通过该指针枚举其他属性
    CComPtr<IWbemClassObject> pObjInstance = (IWbemClassObject*)Value.punkVal;
    hr = pObjInstance->BeginEnumeration(WBEM_FLAG_LOCAL_ONLY);
    do {
        CHECKHR(hr);
        CComBSTR bstrNewName;
        CComVariant NewValue;
        CIMTYPE newtype;
        LONG lnewFlavor = 0;
        hr = pObjInstance->Next(0, &bstrNewName, &NewValue, &newtype, &lnewFlavor);
        CHECKHR(hr);
        PrintfItem(bstrNewName, NewValue, newtype, lnewFlavor);
    }while ( WBEM_S_NO_ERROR == hr );
    hr = pObjInstance->EndEnumeration();
    return WBEM_S_NO_ERROR;
}

VOID CSynEventQuery::PrintfItem( CComBSTR bstrName, CComVariant Value, CIMTYPE type, LONG lFlavor )
{
    wprintf(L"%s\t",bstrName.m_str);
    switch ( Value.vt ){
        case VT_BSTR: {
            wprintf(L"%s",Value.bstrVal);        
                      }break;
        case VT_I1:
        case VT_I2:
        case VT_I4:
        case VT_UI1:	
        case VT_UI2:
        case VT_UI4:	
        case VT_I8:	
        case VT_UI8:	
        case VT_INT:
        case VT_UINT:{
            wprintf(L"0x%08x",Value.intVal);     
                     }break;
        case VT_BOOL:{
            wprintf(L"%s", Value.boolVal ? L"TRUE" : L"FASLE" );
                     }break;
        default:{
                };
    }
    wprintf(L"\n");
}
