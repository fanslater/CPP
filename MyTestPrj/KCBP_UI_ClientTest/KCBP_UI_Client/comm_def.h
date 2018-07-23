#pragma once

//头文件
#include <iostream>
#include <map>
#include "tstring.h"

//宏定义
#define INI_CFG_PATH		"./KCBP_UI_Client.ini"
#define XML_CFG_PATH		"./KCBP_UI_Client.xml"
#define CONSOLE				0

#define THREAD_STOPPED		0
#define THREAD_RUNNING		1
#define SLEEP_INTERVAL		1

#define LISTCTRL_WIDTH		70

struct lbm_call_cfg
{
    tstring lbm_Id;
    tstring lbm_NeedDef;
    tstring lbm_Param;
};

typedef std::map<tstring,lbm_call_cfg> lbmInfo;
typedef std::pair<tstring,lbm_call_cfg> lbmPair;

typedef std::map<tstring,tstring> kvmap;
typedef std::pair<tstring,tstring> kvpair;

