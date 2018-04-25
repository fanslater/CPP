#pragma once
#include "tstring.h"
#include "KCBPCli.h"

struct KcbpConfig
{
    tstring strIp;
    tstring strPort;
    tstring strName;
    tstring strReqQue;
    tstring strRespQue;
    tstring strUser;
    tstring strPwd;
    KcbpConfig()
    {
        clear();
    };
    ~KcbpConfig()
    {
        clear();
    };
    void clear()
    {
        strIp.clear();
        strPort.clear();
        strName.clear();
        strReqQue.clear();
        strRespQue.clear();
        strUser.clear();
        strPwd.clear();
    }
};

class CKcbpVisitor
{
public:
    CKcbpVisitor(void);
    ~CKcbpVisitor(void);
private:
    void CleanSelf();
    tstring GetErrorMsg();
public:
    int InitAll();
    int UninitAll();
    void SetConfig(const KcbpConfig& bp);
    int Connect(tstring& errmsg);
    int Disconnect();
    int CallLbm();    
private:
    KcbpConfig m_stConfig;
    KCBPCLIHANDLE m_kcbp_handle;
};






