#pragma once

#include <iostream>
#include "XmlIniReader.h"
#include "iocpServ.hpp"
#include <boost/thread.hpp> 
#include <boost/thread/barrier.hpp> 
#include <boost/lexical_cast.hpp>
#include <json/json.h>

#define THREAD_STOPPED		0
#define THREAD_RUNNING		1
#define SLEEP_INTERVAL		1

#define SET_COLOR(L)	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE),L)
#define light_green		10
#define light_bluegreen 11
#define light_red		12
#define light_yellow	14

struct stCfg
{
	std::string strIniPath;
};

class CIOCPServer
{
public:
	CIOCPServer(void);
	~CIOCPServer(void);

private:
	CXmlIniReader m_clsIni;
	int m_iThreadRunFlag;					
	boost::thread_group m_ThreadPool;		
	boost::barrier* m_pBarrier;		
	CIocpServ m_clsIocp;
	std::string m_ip;
	int m_port;
	int m_iThreadNum;
	std::string m_MoNiType;
	volatile long m_lTotalNum;
	volatile long m_lNormalNum;
	volatile long m_lExceptionNum;
	int m_iPrintFlag;
	CRITICAL_SECTION m_cs;

public:
	int init(const stCfg& cfg);
	int uninit();
	int run();
	int stop();
	//0=全部 1=正常 2=异常
	long getCount(int type);
	void resetCount();
	//0=不显示 1=显示
	void setPrint(int flag);

private:
	void StartWorkThread();
	void StopWorkThread();
	int WorkThreadFunc();
	//int FreshFunc();
	
	int AnalysisData(const std::string str,Json::Value& js);
	unsigned int GenerateCheckSum(const char *buf, const long bufLen);
	std::string ExtractYwData(const std::string str);

	int DataDispose(const std::string& Type,const std::string& recvData,std::string& sendData);
    int normal(const std::string& recvData,std::string& sendData);
	int xhbs_and_xhzf(const std::string& recvData,std::string& sendData);
	int wjjj(const std::string& recvData,std::string& sendData);
    int EcifHttp(const std::string& recvData,std::string& sendData);
    int PayHttp(const std::string& recvData,std::string& sendData);
    int zrjj(const std::string& recvData,std::string& sendData);
    int XaccTcp(const std::string& recvData,std::string& sendData);
};
