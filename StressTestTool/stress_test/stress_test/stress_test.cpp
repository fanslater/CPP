// stress_test.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"
#include <string>
#include "global.h"
#include "UnitTest.h"
#include "StressTestAdmin.h"

void CommandLine()
{    
    std::cout << "press 'exit' to Exit ..." << std::endl;
    while(1)
    {
        Sleep(1);
        char szLine[8*1024] = {0};
        std::cout << "stress_test:>";
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

    std::string strMainCfgPath(path_default_cfg);
    if (argc == 2)
    {
        strMainCfgPath = argv[1];
    }
    
    CStressTestAdmin clsAdmin;    
    clsAdmin.SetMainCfg(strMainCfgPath);
    clsAdmin.InitAll();
    CommandLine();     
    clsAdmin.UninitAll();
    
	return 0;
}

