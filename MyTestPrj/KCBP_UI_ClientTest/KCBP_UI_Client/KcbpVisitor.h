#pragma once
#include "tstring.h"
#include "KCBPCli.h"
#include <map>
#include <vector>
#include "json/json.h"

struct KcbpConfig
{
    tstring strIp;
    tstring strPort;
    tstring strName;
    tstring strReqQue;
    tstring strRespQue;
    tstring strUser;
    tstring strPwd;
    tstring strProtocal;
    tstring strTimeOut;
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
        strProtocal.clear();
        strTimeOut.clear();
    }
};

typedef std::map<tstring, tstring> kvMap;
typedef std::vector<tstring> string_vector;

class CKcbpVisitor
{
public:
    CKcbpVisitor( void );
    ~CKcbpVisitor( void );
private:
    void CleanSelf();    
public:
    tstring GetErrorMsg();
    void SetConfig( const KcbpConfig& bp );
    int Connect( tstring& errmsg );
    int Disconnect( tstring& errmsg );
    int CallLbm_Result1( const tstring& strLbmNo, const kvMap& mpInParam, kvMap& mpResult1, tstring& errmsg );
    int CallLbm_AllResult( const tstring& strLbmNo, const Json::Value& jsInParam, Json::Value& jsResult, tstring& errmsg );
private:
    KcbpConfig m_stConfig;
    KCBPCLIHANDLE m_kcbp_handle;
};






