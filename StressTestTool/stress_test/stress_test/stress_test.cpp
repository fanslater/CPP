// stress_test.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"
#include <string>
#include "global.h"

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

    std::string strCfgPath(path_default_cfg);
    if (argc == 2)
    {
        strCfgPath = argv[1];
    }

    global::InitLog();
    
    

    
    CommandLine();

    global::UninitLog();
    
	return 0;
}

