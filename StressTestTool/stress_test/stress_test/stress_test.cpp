// stress_test.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"
#include <string>
#include "global.h"
#include "UnitTest.h"
#include "StressTestAdmin.h"

#define SET_COLOR(L)	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE),L)
#define light_green		10
#define light_bluegreen 11
#define light_red		12
#define light_yellow	14

void CommandLine()
{        
    while(1)
    {
        Sleep(1);
        char szLine[256] = {0};        
        gets_s(szLine);
        tstring strLine = CBaseTool::trim(tstring(szLine));
        if (strLine.length() == 0)
        {
            continue;
        }
        if (strLine.compare("exit") == 0)
        {
            break;
        }
    }
}

int _tmain(int argc, _TCHAR* argv[])
{
    EnableMenuItem(GetSystemMenu(GetConsoleWindow(), FALSE), SC_CLOSE, MF_GRAYED);       
    SET_COLOR(light_bluegreen);
    SendMessage(GetConsoleWindow(), WM_SETICON, ICON_SMALL, (LPARAM)LoadIcon(NULL, _T("IDI_ICON1"))); 

    std::string strMainCfgPath(path_default_cfg);
    if (argc == 2)
    {
        strMainCfgPath = argv[1];
    }
    
    CStressTestAdmin clsAdmin;    
    clsAdmin.SetMainCfg(strMainCfgPath);   
    clsAdmin.InitAll();
    clsAdmin.RunStressTests();    
    CommandLine();     
    clsAdmin.UninitAll();
    
	return 0;
}

