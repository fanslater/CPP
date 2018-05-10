#include "StdAfx.h"
#include "CaseCfgCtrl.h"
#include "XmlIniReader.h"

CCaseCfgCtrl::CCaseCfgCtrl(void)
{
    Clean();
}

CCaseCfgCtrl::~CCaseCfgCtrl(void)
{
    Clean();
}

void CCaseCfgCtrl::Clean()
{
    m_mpCaseTestData.clear();
}

int CCaseCfgCtrl::LoadCfg(const tstring &path, tstring &errmsg)
{
    CXmlIniReader reader;
    try
    {
        reader.loadfile(ini_file, path);
    }
    catch (std::exception e)
    {
        errmsg = e.what();
        return -1;
    }
    //测试数据
    try
    {
        boost::property_tree::ptree treeTestData = reader.getTree("测试数据");
        BOOST_FOREACH(boost::property_tree::ptree::value_type & vt1, treeTestData)
        {
            tstring strLbmNo = vt1.first;
            tstring strParams = vt1.second.data();

        }
    }
    catch (std::exception e)
    {
        errmsg = e.what();
        return -1;
    }

    return 0;
}
