#include "ExcuteMethod.h"

#include <atlbase.h>
#include <atlcom.h>

CExcuteMethod::CExcuteMethod(const wstring& wszNamespace, const std::wstring& wstrClass, 
    const std::wstring& wstrInstanceName, const std::wstring& wstrMethod, const std::wstring& wstrRet, const ParamsMap& params)
{
    SetNamespace(wszNamespace);
    m_wstrInstanceName = wstrInstanceName;
    m_wstrClassName = wstrClass;
    m_wstrMethod = wstrMethod;
    m_wstrRet = wstrRet;
    m_params = params;
}

CExcuteMethod::~CExcuteMethod(void)
{
}

HRESULT CExcuteMethod::Excute( CComPtr<IWbemServices> pSvc )
{
    HRESULT hr = WBEM_S_FALSE;

    do {
        if (m_wstrClassName.empty()) {
            break;
        }
        CComBSTR bstrClassName = m_wstrClassName.c_str();
        CComPtr<IWbemClassObject> spClass = NULL;
        hr = pSvc->GetObject(bstrClassName, 0, NULL, &spClass, NULL);
        if (FAILED(hr) || !spClass) {
            break;
        }

        CComBSTR bstrMethodName = m_wstrMethod.c_str();
// 
//         CComPtr<IWbemClassObject> spClassObject;
//         hr = spClass->SpawnInstance(0, &spClassObject);
//         //hr = pSvc->ExecMethod(CComBSTR(L"notepad.exe"), bstrMethodName, 0, NULL, NULL, NULL, NULL);
// 
//         break;
        CComPtr<IWbemClassObject> spInParamsDefinition = NULL;
        hr = spClass->GetMethod(bstrMethodName, 0, &spInParamsDefinition, NULL);
        if (FAILED(hr)) {
            break;
        }

        CComPtr<IWbemClassObject> spParamsInstance = NULL;

        if (spInParamsDefinition) {
            hr = spInParamsDefinition->SpawnInstance(0, &spParamsInstance);
            if (FAILED(hr) || !spParamsInstance) {
                break;
            }

            for (ParamsMap::iterator it = m_params.begin(); it != m_params.end(); it++) {
                if (!it->first.empty()) {
                    CComVariant value = it->second;
                    hr = spParamsInstance->Put(it->first.c_str(), 0, &value, 0);
                }
            }
        }

        CComPtr<IWbemClassObject> spOutParams = NULL;
        if (!m_wstrInstanceName.empty()) {
            //CComBSTR(L"ClassInstance.name='notepad.exe'")
            CComBSTR bstrInstanceName = m_wstrInstanceName.c_str();
            hr = pSvc->ExecMethod(bstrInstanceName, bstrMethodName, 0, NULL, spParamsInstance, &spOutParams, NULL);
        }
        else if (!m_wstrClassName.empty()) {
            hr = pSvc->ExecMethod(bstrClassName, bstrMethodName, 0, NULL, spParamsInstance, &spOutParams, NULL);
        }

        if (SUCCEEDED(hr) && !m_wstrRet.empty()) {
            CComVariant varRet;
            hr = spOutParams->Get(CComBSTR(m_wstrRet.c_str()), 0, &varRet, NULL, 0);
        }

    } while (0);

    return hr;  
}
