#include "stdafx.h"
#include <iostream>
#include "IOCPServer.h"
#include <json/json.h>

#pragma comment (lib,"User32.lib")


int main(int argc, char **argv)
{
	SET_COLOR(light_green);
	SendMessage(GetConsoleWindow(), WM_SETICON, ICON_SMALL, (LPARAM)LoadIcon(NULL, _T("IDI_ICON1"))); 

	stCfg cfg;
	cfg.strIniPath = "./IOCPServCfg.ini"; 

	CIOCPServer svr;
	svr.init(cfg);
	svr.run();
	while(1)
	{
		Sleep(100);
		std::string input;
		cin>>input;
		if (input == "q")
		{
			break;
		}
		if (input == "clear")
		{
			system("cls");
		}
		if (input == "c")
		{
			svr.resetCount();
			printf("����������...\n");
		}
		if (input == "n")
		{
			printf("�ۼ�=[%ld] ����=[%ld] �쳣=[%ld]\n",svr.getCount(0),svr.getCount(1),svr.getCount(2));
		}
		if (input == "on")
		{
			svr.setPrint(1);
			printf("print on...\n");
		}
		if (input == "off")
		{
			svr.setPrint(0);
			printf("print off...\n");
		}
	}
	svr.stop();
	svr.uninit();

	return 0;
}