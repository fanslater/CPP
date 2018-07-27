#include "AsynQuery.h"

#pragma warning(push)
#pragma warning(disable:4819)
#include "WMIBase/EventSinkExtern.h"
#pragma warning(pop)

#include <atlbase.h>
#include <atlcom.h>

template<typename T>
CAsynQuery<T>::CAsynQuery(const wstring& wszNamespace,const wstring& wszWQLQuery, HANDLE hExitEvent)
{
    ATLASSERT( NULL != hExitEvent );
    SetNameSpace(wszNamespace);
    m_wszWQLQuery = wszWQLQuery;
    m_hExitHandle = hExitEvent;
}

template<typename T>
CAsynQuery<T>::~CAsynQuery(void)
{
}

template<typename T>
HRESULT CAsynQuery<T>::Excute( CComPtr<IWbemServices> pSvc )
{
    HRESULT hr = WBEM_S_FALSE;

    do {
        CComPtr<IUnsecuredApartment> pUnsecApp = NULL;
        hr = CoCreateInstance( CLSID_UnsecuredApartment, NULL,
            CLSCTX_LOCAL_SERVER, IID_IUnsecuredApartment, (void**) &pUnsecApp);
        CComPtr<IWbemObjectSink> pSink = new T;

        CComPtr<IUnknown> pStubUnk = NULL;
        pUnsecApp->CreateObjectStub(pSink, &pStubUnk);

        CComPtr<IWbemObjectSink> pStubSink = NULL;
        pStubUnk->QueryInterface(IID_IWbemObjectSink, (void**)&pStubSink);

        hr = pSvc->ExecQueryAsync( 
            CComBSTR("WQL"),
            CComBSTR(m_wszWQLQuery.c_str()),
            WBEM_FLAG_SEND_STATUS,
            NULL,
            pStubSink );

        CHECKHR(hr);

        if ( NULL != m_hExitHandle ) {
            WaitForSingleObject(m_hExitHandle, INFINITE );
        }

        hr = pSvc->CancelAsyncCall(pStubSink);

        if ( NULL != pSink ) {
            delete pSink;
            pSink = NULL;
        }

    } while (0);

    return hr;  
}
