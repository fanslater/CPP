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

}

tstring CKcbpVisitor::GetErrorMsg()
{
    int iErr = 0;
    char szErr[1024] = {0};
    KCBPCLI_GetErr(m_kcbp_handle, &iErr, szErr);
    return CBaseTool::tformat("errcode=[%d] errmsg=[%s]", iErr, szErr);
}

void CKcbpVisitor::SetConfig(const KcbpConfig& bp)
{
    m_stConfig = bp;
}

int CKcbpVisitor::Connect(tstring& errmsg)
{
    int iRet = 0;
    if (m_kcbp_handle != NULL)
    {
        errmsg = CBaseTool::tformat("kcbp handle is not null!");
        return -1;
    }
    iRet = KCBPCLI_Init(&m_kcbp_handle);
    if(iRet != 0)
    {
        errmsg = GetErrorMsg();
        return -1;
    }
    
    return 0;
}
