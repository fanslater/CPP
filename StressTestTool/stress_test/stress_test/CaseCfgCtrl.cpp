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
        errmsg = "�ļ�·��Ϊ��";
        return -1;
    }
    try
    {
        std::ifstream ifs;
        ifs.open(path.c_str());
        if (ifs.is_open() == false)
        {
            errmsg = CBaseTool::tformat("�ļ�[%s]��ʧ��", path.c_str());
            return -1;
        }
        //���ζ�ȡÿ������
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
            cdCaseDetail.strLbmNo = svLbmAndParams[0];  //lbm��
            tstring strLbmParams = svLbmAndParams[1];  //lbm��������
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
                //���ڲ���ֵ�е� :ת~ ;ת` ,ת%�Ļ�ԭ����
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


