#include "stdafx.h"
#include "SyncIocp.h"
#include "IocpTest.h"
#include <boost/thread.hpp>
#include <fstream>
#include <iostream>
using namespace std;

tstring CIocpTest::m_strEmpty;

CIocpTest::CIocpTest()
{
    m_nRunningThread = 0;
}

CIocpTest::~CIocpTest()
{
    m_nRunningThread = 1;
    m_clsIocpServ.Stop();
}

int CIocpTest::Initialize(const char *pszIp, int nPort)
{
    if (0 == ReadAllMessage() && 0 == m_clsIocpServ.IocpRun(pszIp, nPort) )
    {
        boost::thread(boost::bind(&CIocpTest::ThreadResponse, this));
        return 0;
    }
    return -1;
}

int CIocpTest::ThreadResponse()
{
    while (0 == m_nRunningThread)
    {
        PPER_IO_DATA pPerIo = 0;
        m_clsIocpServ.m_queueRecv.wait_and_pop(pPerIo, 1);

        if(pPerIo != 0)
        {
            if(0 == pPerIo->pHandleData)
            {
                delete pPerIo;
            }
            else
            {
                const tstring &strResponse(GetTestMessage(pPerIo->strRecv.substr(0, 4)));
                int nTest = 10;

                if (!strResponse.empty())
                {
                    cout << "应答:" << strResponse << endl;
                    m_clsIocpServ.SendAnsData(pPerIo->pHandleData, pPerIo, strResponse);
                }
                else
                {
                    cout << "找不到消息类型[" << pPerIo->strRecv.substr(0, 4) << "]对应的应答配置." << endl;;
                }
            }
        }
    }
    return 0;
}

int CIocpTest::ReadAllMessage()
{
    tstring strPath("./test.txt");    
    ifstream infile;
    infile.open(strPath.c_str());
    assert(infile.is_open());
    string strLine;
    while(getline(infile,strLine))
    {
        if (strLine.length() > 0)
        {
            m_mpMsg.insert(std::make_pair(strLine.substr(0, 4), strLine));    
        }        
    }
    infile.close();  

    return 0;
}

const tstring &CIocpTest::GetTestMessage(const tstring &strMsgId) const
{
    std::map<tstring, tstring>::const_iterator it = m_mpMsg.find(strMsgId);

    if (it != m_mpMsg.end())
    {
        return it->second;
    }
    return m_strEmpty;
}
