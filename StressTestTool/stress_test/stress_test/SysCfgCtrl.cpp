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
    
    //连接参数
    m_stKcbpCfg.strIp = reader.get("连接参数.kcbp_ip");
    m_stKcbpCfg.strName = reader.get("连接参数.kcbp_name");
    m_stKcbpCfg.strPort = reader.get("连接参数.kcbp_port");
    m_stKcbpCfg.strProtocal = reader.get("连接参数.kcbp_protocal");
    m_stKcbpCfg.strPwd = reader.get("连接参数.user_pwd");
    m_stKcbpCfg.strReqQue = reader.get("连接参数.send_queue_name");
    m_stKcbpCfg.strRespQue = reader.get("连接参数.recv_queue_name");
    m_stKcbpCfg.strUser = reader.get("连接参数.user_name");

    //测试列表
    try
    {
        boost::property_tree::ptree treeUseCase = reader.getTree("测试列表");
        BOOST_FOREACH(boost::property_tree::ptree::value_type & vt1, treeUseCase)
        {
            tstring strCaseName = vt1.first;
            tstring strCaseInfo = vt1.second.data();
            std::vector<tstring> vcFilter;
            int iSize = CBaseTool::split(strCaseInfo,";",vcFilter);
            if (iSize != 4)
            {
                global::WriteLog(ll_warning,"异常的配置文件=[%s] key=[%s] val=[%s]",path.c_str(),strCaseName.c_str(),strCaseInfo.c_str());
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

