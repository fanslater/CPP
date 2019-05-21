#pragma once

//头文件
#include <iostream>
#include <map>
using namespace std;

#include <boost/property_tree/ptree.hpp>  
#include <boost/property_tree/ini_parser.hpp>  
#include <boost/foreach.hpp>


//宏定义
#define INI_CFG_PATH		"./UIClientConfig.ini"
#define CONSOLE				0

#define THREAD_STOPPED		0
#define THREAD_RUNNING		1
#define SLEEP_INTERVAL		1


typedef map<string,string>  kvStrStr;
typedef pair<string,string> prStrStr;
typedef map<string,kvStrStr> kvStrMap;
typedef pair<string,kvStrStr> prStrMap;
