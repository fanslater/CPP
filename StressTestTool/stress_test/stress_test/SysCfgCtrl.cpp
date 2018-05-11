#include "StdAfx.h"
#include "SysCfgCtrl.h"
#include "XmlIniReader.h"

CSysCfgCtrl::CSysCfgCtrl(void)
{
    CleanSelf();
}

CSysCfgCtrl::~CSysCfgCtrl(void)
{
    CleanSelf();
}

void CSysCfgCtrl::CleanSelf()
{
    m_stKcbpCfg.clear();
    m_mpCaseInfos.clear();
}

int CSysCfgCtrl::LoadCfg(const tstring& path ,tstring& errmsg)
{
    CXmlIniReader reader;
    try
    {
        reader.loadfile(ini_file,path);
    }
    catch (std::exception e)
    {
        errmsg = e.what();
        return -1;
    }
    
    //���Ӳ���
    m_stKcbpCfg.strIp = reader.get("���Ӳ���.kcbp_ip");
    m_stKcbpCfg.strName = reader.get("���Ӳ���.kcbp_name");
    m_stKcbpCfg.strPort = reader.get("���Ӳ���.kcbp_port");
    m_stKcbpCfg.strProtocal = reader.get("���Ӳ���.kcbp_protocal");
    m_stKcbpCfg.strPwd = reader.get("���Ӳ���.user_pwd");
    m_stKcbpCfg.strReqQue = reader.get("���Ӳ���.send_queue_name");
    m_stKcbpCfg.strRespQue = reader.get("���Ӳ���.recv_queue_name");
    m_stKcbpCfg.strUser = reader.get("���Ӳ���.user_name");

    //�����б�
    try
    {
        boost::property_tree::ptree treeUseCase = reader.getTree("�����б�");
        BOOST_FOREACH(boost::property_tree::ptree::value_type & vt1, treeUseCase)
        {
            tstring strCaseName = vt1.first;
            tstring strCaseInfo = vt1.second.data();
            std::vector<tstring> vcFilter;
            int iSize = CBaseTool::split(strCaseInfo,";",vcFilter);
            if (iSize != 4)
            {
                global::WriteLog(ll_warning,"�쳣�������ļ�=[%s] key=[%s] val=[%s]",path.c_str(),strCaseName.c_str(),strCaseInfo.c_str());
                continue;
            }
            UseCaseInfo uci;            
            uci.strBatFilePath = vcFilter[0];
            uci.strUseCaseFilePath = vcFilter[1];
            uci.iThreadSum = atoi(vcFilter[2].c_str());
            uci.iExecuteSum = atoi(vcFilter[3].c_str());
            m_mpCaseInfos.insert(UseCasePair(strCaseName,uci));
        }
    }
    catch (std::exception e)
    {
        errmsg = e.what();
        return -1;
    }

    return 0;
}

KcbpConfig CSysCfgCtrl::GetKcbpCfg()
{
    return m_stKcbpCfg;
}

UseCaseMap CSysCfgCtrl::GetCaseInfoSet()
{
    return m_mpCaseInfos;
}

UseCaseInfo CSysCfgCtrl::GetCaseInfo(const tstring& strCaseName)
{
    UseCaseInfo ret;
    UseCaseMap::iterator it = m_mpCaseInfos.find(strCaseName);
    if (it != m_mpCaseInfos.end())
    {
        ret = it->second;
    }
    return ret;
}

