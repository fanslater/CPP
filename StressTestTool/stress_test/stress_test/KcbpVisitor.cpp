#include "StdAfx.h"
#include "KcbpVisitor.h"
#include "BaseTool.h"

CKcbpVisitor::CKcbpVisitor(void)
{
    CleanSelf();
}

CKcbpVisitor::~CKcbpVisitor(void)
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
    KCBPCLI_GetErr(m_kcbp_handle, &iErr, szErr);
    return CBaseTool::tformat("errcode=[%d] errmsg=[%s]", iErr, szErr);
}

void CKcbpVisitor::SetConfig(const KcbpConfig &bp)
{
    m_stConfig = bp;
}

int CKcbpVisitor::Connect(tstring &errmsg)
{
    int iRet = 0;
    if (m_kcbp_handle != NULL)
    {
        errmsg = CBaseTool::tformat("kcbp handle is not null!");
        return -1;
    }
    //��ʼ��
    iRet = KCBPCLI_Init(&m_kcbp_handle);
    if(iRet != 0)
    {
        errmsg = GetErrorMsg();
        return -1;
    }
    //���ò���
    tagKCBPConnectOption stKCBPConnection = {0};
    strcpy_s(stKCBPConnection.szServerName, m_stConfig.strName.c_str());
    stKCBPConnection.nProtocal = atoi(m_stConfig.strProtocal.c_str());
    strcpy_s(stKCBPConnection.szAddress, m_stConfig.strIp.c_str());
    stKCBPConnection.nPort = atoi(m_stConfig.strPort.c_str());
    strcpy_s(stKCBPConnection.szSendQName, m_stConfig.strReqQue.c_str());
    strcpy_s(stKCBPConnection.szReceiveQName, m_stConfig.strRespQue.c_str());
    iRet = KCBPCLI_SetOptions(m_kcbp_handle, KCBP_OPTION_CONNECT, &stKCBPConnection, sizeof(stKCBPConnection));
    if(iRet != 0)
    {
        errmsg = GetErrorMsg();
        return -1;
    }
    //���ӷ���
    iRet = KCBPCLI_ConnectServer(m_kcbp_handle, (char *)m_stConfig.strName.c_str(), (char *)m_stConfig.strUser.c_str(), (char *)m_stConfig.strPwd.c_str());
    if(iRet != 0)
    {
        errmsg = GetErrorMsg();
        KCBPCLI_Exit(m_kcbp_handle);
        return -1;
    }

    return 0;
}

int CKcbpVisitor::Disconnect(tstring &errmsg)
{
    int iRet = 0;
    if (m_kcbp_handle == NULL)
    {
        errmsg = CBaseTool::tformat("kcbp handle is null!");
        return -1;
    }
    //�Ͽ�
    KCBPCLI_DisConnectForce(m_kcbp_handle);
    //����ʼ��
    KCBPCLI_Exit(m_kcbp_handle);
    m_kcbp_handle = NULL;

    return 0;
}

int CKcbpVisitor::CallLbm_UseForTest(const tstring &strLbmNo, const kvMap &mpInParam, tstring &errmsg)
{
    if (m_kcbp_handle == NULL)
    {
        errmsg = CBaseTool::tformat("kcbp handle is null!");
        return -1;
    }
    if (KCBPCLI_BeginWrite(m_kcbp_handle) != 0)
    {
        errmsg = GetErrorMsg();
        return -1;
    }
    KCBPCLI_SetValue(m_kcbp_handle, "F_FUNCTION", "");
    KCBPCLI_SetValue(m_kcbp_handle, "F_SUBSYS", "14");
    KCBPCLI_SetValue(m_kcbp_handle, "F_OP_USER", "8888");
    KCBPCLI_SetValue(m_kcbp_handle, "F_OP_ROLE", "2");
    KCBPCLI_SetValue(m_kcbp_handle, "F_OP_SITE", "127.0.0.1");
    KCBPCLI_SetValue(m_kcbp_handle, "F_CHANNEL", "0");
    KCBPCLI_SetValue(m_kcbp_handle, "F_SESSION", "aaaaaaaaaaaaaaaa");
    KCBPCLI_SetValue(m_kcbp_handle, "F_RUNTIME", "");
    KCBPCLI_SetValue(m_kcbp_handle, "F_REMOTE_OP_ORG", "");
    KCBPCLI_SetValue(m_kcbp_handle, "F_REMOTE_OP_USER", "");
    KCBPCLI_SetValue(m_kcbp_handle, "F_OP_ORG", "0");
    for (kvMap::const_iterator it = mpInParam.begin(); it != mpInParam.end(); it++)
    {
        KCBPCLI_SetValue(m_kcbp_handle, (char *)it->first.c_str(), (char *)it->second.c_str());
    }
    if(KCBPCLI_CallProgramAndCommit(m_kcbp_handle, (char *)strLbmNo.c_str()) != 0)
    {
        errmsg = GetErrorMsg();
        return -1;
    }
    if(KCBPCLI_RsOpen(m_kcbp_handle) != 0)
    {
        errmsg = GetErrorMsg();
        return -1;
    }
    do
    {
        if (KCBPCLI_RsFetchRow(m_kcbp_handle) != 0)
        {
            errmsg = GetErrorMsg();
            break;
        }
        char szMsgCode[16] = {0};
        KCBPCLI_RsGetColByName(m_kcbp_handle, "MSG_CODE", szMsgCode);
        char szMsgText[512] = {0};
        KCBPCLI_RsGetColByName(m_kcbp_handle, "MSG_TEXT", szMsgText);
        errmsg = CBaseTool::tformat("MSG_CODE=[%s] MSG_TEXT=[%s]", szMsgCode, szMsgText);
    }
    while(0);
    if(KCBPCLI_RsClose(m_kcbp_handle) != 0)
    {
        errmsg = GetErrorMsg();
        return -1;
    }

    return 0;
}
