// BaseToolManage.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"
#include <iostream>
#include "json/json.h"
#include "BaseTool.h"

int _tmain( int argc, _TCHAR* argv[] )
{
    Json::Value js1;
    js1["b"] = "b1";
    js1["a"] = "a1";
    Json::Value js2;
    js2.append(js1);
    js2.append(js1);    
    js2.type() == Json::arrayValue;
    
    tstring str = js2.toStyledString();
    printf( "%s\n", str.c_str() );
    return 0;
}

