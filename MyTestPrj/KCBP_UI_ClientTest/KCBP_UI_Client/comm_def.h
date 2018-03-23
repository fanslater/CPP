#pragma once

//头文件
#include <iostream>
#include <map>
using namespace std;


//宏定义
#define INI_CFG_PATH		"./KCBP_UI_Client.ini"
#define XML_CFG_PATH		"./KCBP_UI_Client.xml"
#define CONSOLE				0

#define THREAD_STOPPED		0
#define THREAD_RUNNING		1
#define SLEEP_INTERVAL		1

#define LISTCTRL_WIDTH		100

struct lbm_call_cfg
{
    string lbm_Id;
    string lbm_NeedDef;
    string lbm_Param;
};

typedef map<string,lbm_call_cfg> lbmInfo;
typedef pair<string,lbm_call_cfg> lbmPair;

typedef map<string,string> kvmap;
typedef pair<string,string> kvpair;

