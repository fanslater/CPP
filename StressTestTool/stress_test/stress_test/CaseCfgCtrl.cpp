#include "StdAfx.h"
#include "CaseCfgCtrl.h"
#include <fstream>
//#include <iostream>
//#include "XmlIniReader.h"

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
    m_vcCaseDetails.clear();
}

int CCaseCfgCtrl::LoadCfg(const tstring &path, tstring &errmsg)
{
    if (path.length() <= 0)
    {
        errmsg = "文件路径为空";
        return -1;
    }
    try
    {
        std::ifstream ifs;
        ifs.open(path.c_str());
        if (ifs.is_open() == false)
        {
            errmsg = CBaseTool::tformat("文件[%s]打开失败", path.c_str());
            return -1;
        }
        //依次读取每行数据
        tstring strLine;
        while(getline(ifs, strLine))
        {
            if (strLine.find("[") != tstring::npos && strLine.find("]") != tstring::npos)
            {
                continue;
            }
            stringVector svLbmAndParams;
            if (2 != CBaseTool::split(strLine, ";", svLbmAndParams))
            {
                continue;
            }
            CaseDetail cdCaseDetail;
            cdCaseDetail.strLbmNo = svLbmAndParams[0];  //lbm号
            tstring strLbmParams = svLbmAndParams[1];  //lbm参数序列
            stringVector svFieldsSet;
            CBaseTool::split(strLbmParams, ",", svFieldsSet);            
            for (stringVector::iterator it = svFieldsSet.begin() ; it != svFieldsSet.end() ; it ++)
            {
                tstring strField = (*it);
                stringVector svNameAndValue;
                if (2 != CBaseTool::split(strField, ":", svNameAndValue))
                {
                    continue;
                }
                tstring strFieldName = svNameAndValue[0];
                tstring strFieldValue = svNameAndValue[1];
                //对于参数值中的 :转~ ;转` ,转%的还原如下
                strFieldValue = CBaseTool::all_replace(strFieldValue,tstring("~"),tstring(":"));
                strFieldValue = CBaseTool::all_replace(strFieldValue,tstring("`"),tstring(";"));
                strFieldValue = CBaseTool::all_replace(strFieldValue,tstring("%"),tstring(","));
                cdCaseDetail.mpFields.insert(kvPair(strFieldName, strFieldValue));
            }
            m_vcCaseDetails.push_back(cdCaseDetail);
        }
    }
    catch (std::exception e)
    {
        errmsg = e.what();
        return -1;
    }

    return 0;
}

CaseDataVector CCaseCfgCtrl::GetCaseDetailSet()
{
    return m_vcCaseDetails;
}


