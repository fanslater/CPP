#include "stdafx.h"
#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/ini_parser.hpp>

#include "platform.h" //WIN32 �� linuxƽ̨�Ĳ���ƥ��

#ifndef _WIN32 //��linux
#include <sys/time.h>
#include <unistd.h>

void GetLocalTime(SYSTEMTIME *lpSystemTime)
{
    struct timeval tv;
    gettimeofday (&tv , NULL);
 
    tm tm_now ;
    localtime_r(&tv.tv_sec,&tm_now);
 
    lpSystemTime->wYear         = tm_now.tm_year+1900;     //���
    lpSystemTime->wMonth        = tm_now.tm_mon+1;         //�� tm[0-11] sys[1-12]
    lpSystemTime->wDay          = tm_now.tm_mday;          //��
    lpSystemTime->wDayOfWeek    = (tm_now.tm_wday+1)%7 ;   // tmһ���ڵ�������������һ���𣬷�ΧΪ0-6 sys������������
    lpSystemTime->wHour         = tm_now.tm_hour;          //Сʱ
    lpSystemTime->wMinute       = tm_now.tm_min;           //����
    lpSystemTime->wSecond       = tm_now.tm_sec;           //��
    lpSystemTime->wMilliseconds = tv.tv_usec/1000;         //����
}
/*
//http://www.cnblogs.com/Zscorpio/archive/2011/10/13/2209565.html
void SystemTime2Time_T(LPSYSTEMTIME pSysTime,time_t* pTime)
{
	tm tm_tmp = {pSysTime->wSecond, 
	pSysTime->wMinute, 
	pSysTime->wHour,
	pSysTime->wDay,
	pSysTime->wMonth-1,
	pSysTime->wYear-1900,
	pSysTime->wDayOfWeek, 0, 0};
	*pTime = mktime(&tm_tmp);
}
*/

DWORD GetTickCount(void)
{
	struct timespec now;
	if (clock_gettime(CLOCK_MONOTONIC, &now))
		return 0;
	
	return (DWORD)(now.tv_sec * 1000.0 + now.tv_nsec / 1000000.0);
}

	
DWORD GetPrivateProfileString(
	LPCTSTR lpAppName, //�����ļ���section��
	LPCTSTR lpKeyName, //�����ļ���key��
	LPCTSTR lpDefault,
	LPTSTR lpReturnedString,
	DWORD nSize,
	LPCTSTR lpFileName
)
{
	try
	{
		std::string strSectionName = std::string(lpAppName) + "." + std::string(lpKeyName); 
	
		boost::property_tree::ptree pt;
		boost::property_tree::ini_parser::read_ini(lpFileName, pt);
		
		std::string strValue = pt.get<std::string>(strSectionName);
		strncpy(lpReturnedString, strValue.c_str(), nSize);
	}
	catch(...)
	{
		return -1;
	}

	return 0;
}


DWORD GetEnvironmentVariableA(
	LPCTSTR lpName,
	LPTSTR lpBuffer,
	DWORD nSize
)
{
	return 0;
}

BOOL SetEnvironmentVariableA(
	LPCTSTR lpName,
	LPCTSTR lpValue
)
{
	return 0;
}

void Sleep(int nTimeMS)
{
	usleep(nTimeMS);
}

char* gets(char* pszBuffer)
{
	fgets(pszBuffer, 1024, stdin);//stdin ��˼�Ǽ�������
	return pszBuffer;
}


#include <stdio.h>
#include <termios.h>
#include <unistd.h>
#include <fcntl.h>
int kbhit(void)
{
	struct termios oldt, newt;
	int ch;
	int oldf;
	tcgetattr(STDIN_FILENO, &oldt);
	newt = oldt;
	newt.c_lflag &= ~(ICANON | ECHO);
	tcsetattr(STDIN_FILENO, TCSANOW, &newt);
	oldf = fcntl(STDIN_FILENO, F_GETFL, 0);
	fcntl(STDIN_FILENO, F_SETFL, oldf | O_NONBLOCK);
	ch = getchar();
	tcsetattr(STDIN_FILENO, TCSANOW, &oldt);
	fcntl(STDIN_FILENO, F_SETFL, oldf);
	if(ch != EOF)
	{
	ungetc(ch, stdin);
	return 1;
	}
	return 0;
}
#endif