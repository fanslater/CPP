// httpServer.cpp : �������̨Ӧ�ó������ڵ㡣
//

#include "stdafx.h"

#include "StringUtil.h"
#include "IocpServer.h"
//#include "httpServer.h"
//#include "log4cplus.h"
#include <boost/asio.hpp>
#include <boost/date_time/posix_time/posix_time.hpp>
#include "IocpTest.h"
#include <iostream>
using namespace std;

int _tmain(int argc, _TCHAR* argv[])
{
  WSADATA wsadata;
  WSAStartup(MAKEWORD(2,2),&wsadata);
//  CLog::CreateInstance();
//  CHttpServ httpServer;

//  httpServer.CreateHttpServer("218.17.116.130", "8888");
  //httpServer.CreateHttpServer("192.168.0.101", "8888");
//   CIocpServer IocpSvr;
//   IocpSvr.Start("127.0.0.1", 9001);

  CIocpTest IocpTest;

  const char* pszIp = "127.0.0.1";
  int nPort = 32003;
  if(0 == IocpTest.Initialize(pszIp, nPort))
  {
    cout << "����" << pszIp << ":" << nPort << "�ɹ�." << endl;
  }

  int chCode = 0;
  do 
  {
    chCode = getchar();
  } while (chCode != 'x');

  //  IocpSvr.Shutdown();
//  CLog::DestroyInstance();
  WSACleanup();
	return 0;
}

