#pragma once

//头文件
#include <iostream>
#include <map>
#include "tstring.h"

//宏定义
#define XML_CFG_PATH        "./MainConfig.xml"
#define LISTCTRL_WIDTH      70

struct LbmInfo
{
    tstring strLbmNo;
    tstring strParams;
};
typedef std::map<tstring, LbmInfo> LbmInfoMap;
typedef std::pair<tstring, LbmInfo> LbmInfoPair;


