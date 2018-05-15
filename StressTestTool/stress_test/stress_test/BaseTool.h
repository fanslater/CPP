#pragma once
#include "tstring.h"
#include <vector>
#include "json/json.h"
#include <boost/property_tree/ptree.hpp> 


class CBaseTool
{
public:
    CBaseTool(void);
    ~CBaseTool(void);

public:
    
    //日期时间相关
    static int GetPhysicsDate_AsInt();
    static tstring GetPhysicsDate_AsStr();
    static int GetPhysicsTime_AsInt();
    static tstring GetPhysicsTime_AsStr();
    static long long GetPhysicsDateTime_Asll();
    static tstring GetPhysicsDateTime_AsStr();
    static long long GetTimestamp_Milli();
    static long long GetTimestamp_Micro();        

    //字符串相关
    static tstring tformat(const char* format, ...);
    static tstring trim_l(tstring& s);
    static tstring trim_r(tstring& s);
    static tstring trim(tstring& s);
    static int split(const tstring& strSrc, const tstring& strSep, std::vector<tstring>& vctStringList, bool bTrim=true);
    static int split_ex(const tstring& strSrc, const tstring& strSep, std::vector<tstring>& vctStringList, bool bTrim=true);
    static tstring all_upper(tstring& s);
    static tstring all_lower(tstring& s);
    static tstring all_replace(tstring& s,tstring& key = tstring(" "),tstring& rep = tstring(""));
    
    //json相关
    static tstring json_to_str(Json::Value& json);
    static int str_to_json(tstring& str,Json::Value& json,tstring& err);
    static tstring get_json_val(Json::Value& json,const tstring& key);

    //xml相关
    static tstring get_ptree_val_secure(const boost::property_tree::ptree& node,const tstring& path);

    //其他
    static int AnalysisTimeZone(tstring timezone,int& iBeginTime,int& iEndTime);
    static void OutputDebugStringEx(const char* format, ...);

    //操作系统相关
    static tstring GetLocalIpAddress();
    static tstring GetProcedurePath();
    static bool IsPathExists(const tstring& path);
    static int CreatePath(const tstring& path);
};

