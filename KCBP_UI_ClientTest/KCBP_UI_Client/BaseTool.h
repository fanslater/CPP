#pragma once
#include "tstring.h"
#include <vector>
#include "json/json.h"
#include <boost/property_tree/ptree.hpp>
#include <winsock2.h>       //因为<windows.h>中包含了<winsock.h>头文件。为了防止WINSOCK.H与winsock2.h中的重定义。
#include <Windows.h>


class CBaseTool
{
public:
    CBaseTool( void );
    ~CBaseTool( void );
private:
    void clean();
private:
    static HANDLE m_hdRefreshCpuThd;
    static long long m_llCpuUse;
    static long long m_llCpuIdle;
    static CRITICAL_SECTION m_csCpu;
    static BOOL m_iRefreshCpuFlag;
private:
    static UINT WINAPI RefreshCpuUseRateThreadFunc( void* pParam );

public:

    //打印显示
    static void ShowWindow( const tstring& data );
    static void ShowWindow( const char* format, ... );

    //日期时间相关
    static int GetPhysicsDate_AsInt();
    static tstring GetPhysicsDate_AsStr();
    static int GetPhysicsTime_AsInt();
    static tstring GetPhysicsTime_AsStr();
    static long long GetPhysicsDateTime_Asll();
    static tstring GetPhysicsDateTime_AsStr();
    static long long GetTimestamp_Milli();
    static long long GetTimestamp_Micro();
    static tstring GetTimestamp_AsStr();

    //字符串相关
    static tstring tformat( const char* format, ... );
    static tstring trim_l( tstring& s );
    static tstring trim_r( tstring& s );
    static tstring trim( tstring& s );
    static tstring trim_ex( tstring& s );
    static int split_k( const tstring& strSrc, const tstring& strSep, std::vector<tstring>& vctStringList, bool bTrim = true );
    static int split_d( const tstring& strSrc, const tstring& strSep, std::vector<tstring>& vctStringList, bool bTrim = true );
    static int split_ex_k( const tstring& strSrc, const tstring& strSep, std::vector<tstring>& vctStringList, bool bTrim = true );
    static int split_ex_d( const tstring& strSrc, const tstring& strSep, std::vector<tstring>& vctStringList, bool bTrim = true );
    static tstring all_upper( tstring& s );
    static tstring all_lower( tstring& s );
    static tstring all_replace( tstring& s, tstring& key = tstring( " " ), tstring& rep = tstring( "" ) );

    //json相关
    static tstring json_to_str( Json::Value& json );
    static int str_to_json( tstring& str, Json::Value& json, tstring& err );
    static tstring get_json_val( const Json::Value& json, const tstring& key, bool bNeedTrim = true );
    static Json::Value make_json( const tstring& name, const tstring& value );

    //xml相关
    static tstring get_ptree_val_secure( const boost::property_tree::ptree& node, const tstring& path );

    //其他
    static int AnalysisTimeZone( tstring timezone, int& iBeginTime, int& iEndTime );
    static void OutputDebugStringEx( const char* format, ... );

    //操作系统相关
    static tstring RelativePathToCompletePath( const tstring& path );
    static tstring CompletePathToRelativePath( const tstring& path );
    static tstring GetLocalIpAddress();
    static tstring GetProgramLaunchDir();
    static tstring GetCurrentDir();
    static tstring CutDirFromPath( const tstring& path );
    static tstring CutFileFromPath( const tstring& path );
    static int SetCurrentDir( const tstring& path, tstring& err );
    static bool IsPathExists( const tstring& path );
    static int CreatePath( const tstring& path );
    static void ShowCpuPercent();   //因为CPU需要时间差计算，这里只是给了显示例子。
    static int GetMemoryPercent();
    static long long CompareFileTime( FILETIME time1, FILETIME time2 );
    static int GetIOInfo( ULONGLONG& read_bytes, ULONGLONG& write_bytes, ULONGLONG& wct, ULONGLONG& rct );
    static void StartRefreshCpu();
    static void StopRefreshCpu();
    static void GetCpuUseInfo( long long& llCpuUse, long long& llCpuIdle );
};

