#include "SynQuery.h"

#pragma warning(push)
#pragma warning(disable:4819)
#include "WMIBase/EventSinkExtern.h"
#pragma warning(pop)

#include <atlbase.h>
#include <atlcom.h>

CSynQuery::CSynQuery(const wstring& wszNamespace,const wstring& wszWQLQuery, HANDLE hExitEvent)
{
    ATLASSERT( NULL != hExitEvent );
    SetNameSpace(wszNamespace);
    m_wszWQLQuery = wszWQLQuery;
}

CSynQuery::~CSynQuery(void)
{
}

HRESULT CSynQuery::Excute( CComPtr<IWbemServices> pSvc )
{
    HRESULT hr = WBEM_S_FALSE;

    do {
        CComPtr<IEnumWbemClassObject> pEnumerator = NULL;
        hr = pSvc->ExecQuery( 
            CComBSTR("WQL"),
            CComBSTR(m_wszWQLQuery.c_str()),
            WBEM_FLAG_FORWARD_ONLY | WBEM_FLAG_RETURN_IMMEDIATELY,
            NULL,
            &pEnumerator );

        CHECKHR(hr);

        CComPtr<IWbemClassObject> pclsObj = NULL;
        ULONG uReturn = 0;

        while (pEnumerator) {
            HRESULT hr = pEnumerator->Next(WBEM_INFINITE, 1, &pclsObj, &uReturn);

            if ( 0 == uReturn) {
                break;
            }
            DealWithIWbemClassObject(pclsObj);
        }

    } while (0);

    return hr;  
}

HRESULT CSynQuery::DealWithIWbemClassObject( CComPtr<IWbemClassObject> pclsObj )
{
    CComVariant vtProp;
    HRESULT hr = WBEM_S_NO_ERROR;
    return hr;
}