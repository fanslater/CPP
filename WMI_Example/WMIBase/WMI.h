#pragma once
#include <windows.h>
#include <comdef.h>
#include <wbemidl.h>

#include <atlbase.h>
#include <atlcom.h>
#include <string>

#define CHECKHR(x) \
    if ( FAILED((x))) break; \

#define CHECKWMIHR(x) \
    if ( (x) != WBEM_S_NO_ERROR ) break; \

using namespace std;

class CWMI
{
public:
    CWMI();
    ~CWMI(void);
public:
    HRESULT ExcuteFun();
protected:
    VOID SetNamespace(wstring wstrNamespace);
private:
    HRESULT InitialCom();
    HRESULT SetComSecLevels();
    HRESULT ObtainLocator2WMI(CComPtr<IWbemLocator>& pLoc);
    HRESULT Connect2WMI(CComPtr<IWbemLocator> pLoc,CComPtr<IWbemServices>& pSvc);
    HRESULT SetProxySecLevels(CComPtr<IWbemServices> pSvc);
    virtual HRESULT Excute(CComPtr<IWbemServices> pSvc) = 0;
private:
    wstring m_wstrNamespace;
};