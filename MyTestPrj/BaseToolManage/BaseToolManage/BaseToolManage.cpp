// BaseToolManage.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"
#include <iostream>
#include "ThreadUnitEx.h"

int _tmain( int argc, _TCHAR* argv[] )
{
    CThreadUnitEx cls;
    cls.StartThread();
    getchar();
    cls.StopThread();
    return 0;
}

