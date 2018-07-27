#pragma once
#include <comdef.h>
#include <Wbemidl.h>
# pragma comment(lib, "wbemuuid.lib")

#include <atlbase.h>
#include <atlcom.h>

#define CHECKHR(x) \
    if ( FAILED((x))) break; \

#define CHECKWMIHR(x) \
    if ( (x) != WBEM_S_NO_ERROR ) break; \

class CSink : public IWbemObjectSink
{
public:
    CSink();
    ~CSink();

    virtual ULONG STDMETHODCALLTYPE AddRef();
    virtual ULONG STDMETHODCALLTYPE Release();
    virtual HRESULT STDMETHODCALLTYPE QueryInterface(REFIID riid, void** ppv);
    virtual HRESULT STDMETHODCALLTYPE Indicate(LONG lObjectCount, IWbemClassObject __RPC_FAR* __RPC_FAR* apObjArray);
    virtual HRESULT STDMETHODCALLTYPE SetStatus(LONG lFlags, HRESULT hResult, BSTR strParam, IWbemClassObject __RPC_FAR* pObjParam);
    HRESULT DealIWbemClassObject(CComPtr<IWbemClassObject> pObj);
private:
    // 返回值为WBEM_S_NO_ERROR则继续枚举，否则中断枚举
    virtual HRESULT DealWithSingleItem(CComBSTR bstrName, CComVariant Value, CIMTYPE type, LONG lFlavor);
private:
    LONG m_lRef;
    bool m_bDone;
};
