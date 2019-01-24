#include "StdAfx.h"
#include "KcbpVisitor.h"
#include "BaseTool.h"

CKcbpVisitor::CKcbpVisitor( void )
{
    CleanSelf();
}

CKcbpVisitor::~CKcbpVisitor( void )
{
    CleanSelf();
}

void CKcbpVisitor::CleanSelf()
{
    m_kcbp_handle = NULL;
}

tstring CKcbpVisitor::GetErrorMsg()
{
    int iErr = 0;
    char szErr[1024] = {0};
    KCBPCLI_GetErr( m_kcbp_handle, &iErr, szErr );
    return CBaseTool::tformat( "errcode=[%d] errmsg=[%s]", iErr, szErr );
}

void CKcbpVisitor::SetConfig( const KcbpConfig& bp )
{
    m_stConfig = bp;
}

int CKcbpVisitor::Connect( tstring& errmsg )
{
    int iRet = 0;
    if( m_kcbp_handle != NULL )
    {
        errmsg = CBaseTool::tformat( "kcbp handle is not null!" );
        return -1;
    }
    //初始化
    iRet = KCBPCLI_Init( &m_kcbp_handle );
    if( iRet != 0 )
    {
        errmsg = GetErrorMsg();
        return -1;
    }
    //设置参数
    tagKCBPConnectOption stKCBPConnection = {0};
    strcpy_s( stKCBPConnection.szServerName, m_stConfig.strName.c_str() );
    stKCBPConnection.nProtocal = atoi( m_stConfig.strProtocal.c_str() );
    strcpy_s( stKCBPConnection.szAddress, m_stConfig.strIp.c_str() );
    stKCBPConnection.nPort = atoi( m_stConfig.strPort.c_str() );
    strcpy_s( stKCBPConnection.szSendQName, m_stConfig.strReqQue.c_str() );
    strcpy_s( stKCBPConnection.szReceiveQName, m_stConfig.strRespQue.c_str() );
    iRet = KCBPCLI_SetOptions( m_kcbp_handle, KCBP_OPTION_CONNECT, &stKCBPConnection, sizeof( stKCBPConnection ) );
    if( iRet != 0 )
    {
        errmsg = GetErrorMsg();
        m_kcbp_handle = NULL;
        return -1;
    }
    int iTimeOut = atoi( m_stConfig.strTimeOut.c_str() );
    iTimeOut = ( iTimeOut == 0 ? 60 : iTimeOut );
    iRet = KCBPCLI_SetCliTimeOut( m_kcbp_handle, iTimeOut );
    if( iRet != 0 )
    {
        errmsg = GetErrorMsg();
        m_kcbp_handle = NULL;
        return -1;
    }
    //链接服务
    iRet = KCBPCLI_ConnectServer( m_kcbp_handle, ( char* )m_stConfig.strName.c_str(), ( char* )m_stConfig.strUser.c_str(), ( char* )m_stConfig.strPwd.c_str() );
    if( iRet != 0 )
    {
        errmsg = CBaseTool::tformat( "链接KCBP服务失败，返回码=[%d]，错误信息=[%s]", iRet, GetErrorMsg().c_str() );
        KCBPCLI_Exit( m_kcbp_handle );
        m_kcbp_handle = NULL;
        return -1;
    }
    return 0;
}

int CKcbpVisitor::Disconnect( tstring& errmsg )
{
    int iRet = 0;
    if( m_kcbp_handle == NULL )
    {
        errmsg = CBaseTool::tformat( "kcbp handle is null!" );
        return -1;
    }
    //断开
    KCBPCLI_DisConnectForce( m_kcbp_handle );
    //反初始化
    KCBPCLI_Exit( m_kcbp_handle );
    m_kcbp_handle = NULL;
    return 0;
}

int CKcbpVisitor::CallLbm_Result1( const tstring& strLbmNo, const kvMap& mpInParam, kvMap& mpResult1, tstring& errmsg )
{
    int iRet = 0;
    if( m_kcbp_handle == NULL )
    {
        errmsg = CBaseTool::tformat( "kcbp handle is null!" );
        return -1;
    }
    iRet = KCBPCLI_BeginWrite( m_kcbp_handle );
    if( iRet != 0 )
    {
        errmsg = GetErrorMsg();
        return iRet;
    }
    for( kvMap::const_iterator it = mpInParam.begin(); it != mpInParam.end(); ++it )
    {
        KCBPCLI_SetValue( m_kcbp_handle, ( char* )it->first.c_str(), ( char* )it->second.c_str() );
    }
    iRet = KCBPCLI_CallProgramAndCommit( m_kcbp_handle, ( char* )strLbmNo.c_str() );
    if( iRet != 0 )
    {
        errmsg = GetErrorMsg();
        return iRet;
    }
    iRet = KCBPCLI_RsOpen( m_kcbp_handle );
    if( iRet != 0 )
    {
        errmsg = GetErrorMsg();
        return iRet;
    }
    do
    {
        iRet = KCBPCLI_RsFetchRow( m_kcbp_handle );
        if( iRet != 0 )
        {
            errmsg = GetErrorMsg();
            break;
        }
        char szMsgCode[16] = {0};
        KCBPCLI_RsGetColByName( m_kcbp_handle, "MSG_CODE", szMsgCode );
        char szMsgText[512] = {0};
        KCBPCLI_RsGetColByName( m_kcbp_handle, "MSG_TEXT", szMsgText );
        mpResult1["MSG_CODE"] = szMsgCode;
        mpResult1["MSG_TEXT"] = szMsgText;
    }
    while( 0 );
    iRet = KCBPCLI_RsClose( m_kcbp_handle );
    if( iRet != 0 )
    {
        errmsg = GetErrorMsg();
        return iRet;
    }
    return iRet;
}

int CKcbpVisitor::CallLbm_Result1_Ex( const tstring& strLbmNo, const kvMap& mpInParam, kvMap& mpResult1, tstring& errmsg )
{
    int iRet = 0;
    if( m_kcbp_handle == NULL )
    {
        errmsg = CBaseTool::tformat( "kcbp handle is null!" );
        return -1;
    }
    iRet = KCBPCLI_BeginWrite( m_kcbp_handle );
    if( iRet != 0 )
    {
        errmsg = GetErrorMsg();
        return iRet;
    }
    for( kvMap::const_iterator it = mpInParam.begin(); it != mpInParam.end(); ++it )
    {
        KCBPCLI_SetValue( m_kcbp_handle, ( char* )it->first.c_str(), ( char* )it->second.c_str() );
    }
    iRet = KCBPCLI_CallProgramAndCommit( m_kcbp_handle, ( char* )strLbmNo.c_str() );
    if( iRet != 0 )
    {
        errmsg = GetErrorMsg();
        return iRet;
    }
    iRet = KCBPCLI_RsOpen( m_kcbp_handle );
    if( iRet != 0 )
    {
        errmsg = GetErrorMsg();
        return iRet;
    }
    do
    {
        iRet = KCBPCLI_RsFetchRow( m_kcbp_handle );
        if( iRet != 0 )
        {
            errmsg = GetErrorMsg();
            break;
        }
        char szMsgCode[16] = {0};
        KCBPCLI_RsGetColByName( m_kcbp_handle, "MSG_CODE", szMsgCode );
        char szMsgText[512] = {0};
        KCBPCLI_RsGetColByName( m_kcbp_handle, "MSG_TEXT", szMsgText );
        char szCode[16] = {0};
        KCBPCLI_RsGetColByName( m_kcbp_handle, "CODE", szCode );
        char szMsg[512] = {0};
        KCBPCLI_RsGetColByName( m_kcbp_handle, "MSG", szMsg );
        mpResult1["MSG_CODE"] = szMsgCode;
        mpResult1["MSG_TEXT"] = szMsgText;
        mpResult1["CODE"] = szCode;
        mpResult1["MSG"] = szMsg;
    }
    while( 0 );
    while( 0 == KCBPCLI_RsMore( m_kcbp_handle ) )   //将剩余的结果集都提取一下。
    {
        //while( 0 == KCBPCLI_RsFetchRow( m_kcbp_handle ) )
        //{
        //    //do nothing.
        //}
    }
    iRet = KCBPCLI_RsClose( m_kcbp_handle );
    if( iRet != 0 )
    {
        errmsg = GetErrorMsg();
        return iRet;
    }
    return iRet;
}
