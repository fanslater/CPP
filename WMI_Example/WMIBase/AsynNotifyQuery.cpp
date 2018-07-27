#include "AsynNotifyQuery.h"

#include <atlbase.h>
#include <atlcom.h>

template<typename T>
CAsynNotifyQuery<T>::CAsynNotifyQuery(const wstring& wszNamespace,const wstring& wszWQLQuery, HANDLE hExitEvent)
{
    ATLASSERT( NULL != hExitEvent );
    SetNamespace(wszNamespace);
    m_wszWQLQuery = wszWQLQuery;
    m_hExitHandle = hExitEvent;
}

template<typename T>
CAsynNotifyQuery<T>::~CAsynNotifyQuery(void)
{
}

template<typename T>
HRESULT CAsynNotifyQuery<T>::Excute( CComPtr<IWbemServices> pSvc )
{
    HRESULT hr = WBEM_S_FALSE;

    do {
        CComPtr<IUnsecuredApartment> pUnsecApp = NULL;
        hr = CoCreateInstance( CLSID_UnsecuredApartment, NULL,
            CLSCTX_LOCAL_SERVER, IID_IUnsecuredApartment, (void**) &pUnsecApp);
        CHECKWMIHR(hr);

        CComPtr<IWbemObjectSink> pSink = new T;

        CComPtr<IUnknown> pStubUnk = NULL;
        pUnsecApp->CreateObjectStub(pSink, &pStubUnk);

        CComPtr<IWbemObjectSink> pStubSink = NULL;
        pStubUnk->QueryInterface(IID_IWbemObjectSink, (void**)&pStubSink);

        hr = pSvc->ExecNotificationQueryAsync( 
            CComBSTR("WQL"),
            CComBSTR(m_wszWQLQuery.c_str()),
            WBEM_FLAG_SEND_STATUS,
            NULL,
            pStubSink );

        CHECKWMIHR(hr);

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
