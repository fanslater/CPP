#pragma once
#include "tstring.h"
#include <vector>
//#include "json/json.h"
//#include <boost/property_tree/ptree.hpp>
#include <Windows.h>


class CBaseTool
{
public:
    CBaseTool( void );
    ~CBaseTool( void );

public:

    //����ʱ�����
    static int GetPhysicsDate_AsInt();
    static tstring GetPhysicsDate_AsStr();
    static int GetPhysicsTime_AsInt();
    static tstring GetPhysicsTime_AsStr();
    static long long GetPhysicsDateTime_Asll();
    static tstring GetPhysicsDateTime_AsStr();
    //static long long GetTimestamp_Milli();
    //static long long GetTimestamp_Micro();

    //�ַ������
    static tstring tformat( const char* format, ... );
    //static tstring trim_l( tstring& s );
    //static tstring trim_r( tstring& s );
    //static tstring trim( tstring& s );
    static tstring trim_ex( tstring& s );
    //static int split_k( const tstring& strSrc, const tstring& strSep, std::vector<tstring>& vctStringList, bool bTrim = true );
    //static int split_d( const tstring& strSrc, const tstring& strSep, std::vector<tstring>& vctStringList, bool bTrim = true );
    static int split_ex_k( const tstring& strSrc, const tstring& strSep, std::vector<tstring>& vctStringList, bool bTrim = true );
    static int split_ex_d( const tstring& strSrc, const tstring& strSep, std::vector<tstring>& vctStringList, bool bTrim = true );
    //static tstring all_upper( tstring& s );
    //static tstring all_lower( tstring& s );
    //static tstring all_replace( tstring& s, tstring& key = tstring( " " ), tstring& rep = tstring( "" ) );

    //json���
    //static tstring json_to_str( Json::Value& json );
    //static int str_to_json( tstring& str, Json::Value& json, tstring& err );
    //static tstring get_json_val( Json::Value& json, const tstring& key );

    //xml���
    //static tstring get_ptree_val_secure( const boost::property_tree::ptree& node, const tstring& path );

    //����
    //static int AnalysisTimeZone( tstring timezone, int& iBeginTime, int& iEndTime );
    static void ShowWindow( const tstring& data );
    static void ShowWindow( const char* format, ... );
    static void OutputDebugStringEx( const char* format, ... );

    //����ϵͳ���
    //static tstring RelativePathToCompletePath( const tstring& path );
    //static tstring CompletePathToRelativePath( const tstring& path );
    static tstring GetLocalIpAddress();
    //static tstring GetProcedurePath();
    //static bool IsPathExists( const tstring& path );
    //static int CreatePath( const tstring& path );
    static void ShowCpuPercent();   //��ΪCPU��Ҫʱ�����㣬����ֻ�Ǹ�����ʾ���ӡ�
    static int GetMemoryPercent();
    static long long CompareFileTime( FILETIME time1, FILETIME time2 );
    static int GetIOInfo( ULONGLONG& read_bytes, ULONGLONG& write_bytes, ULONGLONG& wct, ULONGLONG& rct );
};

