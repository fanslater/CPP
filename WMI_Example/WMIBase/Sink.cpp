#pragma warning(push)
#pragma warning(disable:4819)
#include "Sink.h"
#pragma warning(pop)

CSink::CSink(void)
{
    m_lRef = 0;
}

CSink::~CSink(void)
{
    m_bDone = true;
}

ULONG STDMETHODCALLTYPE CSink::AddRef()
{
    return InterlockedIncrement(&m_lRef);
}

ULONG STDMETHODCALLTYPE CSink::Release()
{
    LONG lRef = InterlockedDecrement(&m_lRef);
    if ( 0 == lRef ) {
        delete this;
    }
    return lRef;
}

HRESULT STDMETHODCALLTYPE CSink::QueryInterface( REFIID riid, void** ppv )
{
    if ( IID_IUnknown == riid || IID_IWbemObjectSink == riid ) {
        *ppv = (IWbemObjectSink*) this;
        AddRef();
        return WBEM_S_NO_ERROR;
    }
    return E_NOINTERFACE;
}

HRESULT STDMETHODCALLTYPE CSink::Indicate( LONG lObjectCount, IWbemClassObject __RPC_FAR* __RPC_FAR* apObjArray )
{
    for (long i = 0; i < lObjectCount; i++) { 
        CComPtr<IWbemClassObject> pObj = apObjArray[i]; 
        DealIWbemClassObject(pObj);
    } 

    return WBEM_NO_ERROR; 
}

HRESULT STDMETHODCALLTYPE CSink::SetStatus( LONG lFlags, HRESULT hResult, BSTR strParam, IWbemClassObject __RPC_FAR* pObjParam )
{
    switch (lFlags) {
        case WBEM_STATUS_COMPLETE:
        case WBEM_STATUS_REQUIREMENTS:
        case WBEM_STATUS_PROGRESS:
        default:
            break;
    }
    return WBEM_S_NO_ERROR;
}

HRESULT CSink::DealIWbemClassObject( CComPtr<IWbemClassObject> pclsObj )
{
    HRESULT hr = WBEM_S_NO_ERROR; 
    do  {
        CComVariant vtClass; 

        hr = pclsObj->Get(L"__CLASS", 0, &vtClass, NULL, NULL); 
        CHECKWMIHR(hr);
        if ( VT_BSTR == vtClass.vt ) {
            wprintf(L"\n%s\n", vtClass.bstrVal);
        }

        hr = pclsObj->BeginEnumeration(WBEM_FLAG_LOCAL_ONLY);

        do {
            CComBSTR bstrName;
            CComVariant Value;
            CIMTYPE type;
            LONG lFlavor = 0;
            hr = pclsObj->Next(0, &bstrName, &Value, &type, &lFlavor);
            CHECKWMIHR(hr);
            hr = DealWithSingleItem(bstrName, Value, type, lFlavor);            
            
            // 获取属性所属类的名字
//             CComBSTR strClassName;
//             hr = pObj->GetPropertyOrigin( LPCWSTR(bstrName.m_str), &strClassName);
        }while ( WBEM_S_NO_ERROR == hr );

        hr = pclsObj->EndEnumeration();
    } while (0);

    return WBEM_S_NO_ERROR;
}

HRESULT CSink::DealWithSingleItem( CComBSTR bstrName, CComVariant Value, CIMTYPE type, LONG lFlavor )
{   
    return WBEM_S_NO_ERROR;
}
