#include "StdAfx.h"
#include "BaseTool.h"
#include "CAutoLock.h"
#include <boost/tokenizer.hpp>
#include <boost/algorithm/string.hpp>
#include <boost/filesystem.hpp>
#include <boost/filesystem/path.hpp>
#include <boost/filesystem/operations.hpp>
#include <boost/date_time.hpp>


#define FORMAT_LEN  8*1024



CBaseTool::CBaseTool( void )
{
    clean();
}

CBaseTool::~CBaseTool( void )
{
    clean();
}

void CBaseTool::clean()
{
    m_hdRefreshCpuThd = NULL;
    m_llCpuUse = 0;
    m_llCpuIdle = 0;
    m_iRefreshCpuFlag = 0;
}

HANDLE CBaseTool::m_hdRefreshCpuThd;
long long CBaseTool::m_llCpuUse;
long long CBaseTool::m_llCpuIdle;
CRITICAL_SECTION CBaseTool::m_csCpu;
BOOL CBaseTool::m_iRefreshCpuFlag;

UINT WINAPI CBaseTool::RefreshCpuUseRateThreadFunc( void* pParam )
{
    BOOL iRet;
    FILETIME ftLastIdleTime;    //上一次闲置时间量
    FILETIME ftLastKernelTime;  //上一次内核时间量
    FILETIME ftLastUserTime;    //上一次用户时间量
    FILETIME ftIdleTime;        //闲置时间量
    FILETIME ftKernelTime;      //内核时间量
    FILETIME ftUserTime;        //用户时间量
    iRet = GetSystemTimes( &ftIdleTime, &ftKernelTime, &ftUserTime );
    ftLastIdleTime = ftIdleTime;
    ftLastKernelTime = ftKernelTime;
    ftLastUserTime = ftUserTime;
    while( m_iRefreshCpuFlag == TRUE )
    {
        Sleep( 1000 );
        iRet = GetSystemTimes( &ftIdleTime, &ftKernelTime, &ftUserTime );
        long long llIdleDiff = CBaseTool::CompareFileTime( ftLastIdleTime, ftIdleTime );
        long long llKernelDiff = CBaseTool::CompareFileTime( ftLastKernelTime, ftKernelTime );
        long long llUserDiff = CBaseTool::CompareFileTime( ftLastUserTime, ftUserTime );
        {
            CAutoLock lock( m_csCpu );
            m_llCpuUse = ( llKernelDiff + llUserDiff - llIdleDiff ) * 100 / ( llKernelDiff + llUserDiff );
            m_llCpuIdle = ( llIdleDiff ) * 100 / ( llKernelDiff + llUserDiff );
        }
        ftLastIdleTime = ftIdleTime;
        ftLastKernelTime = ftKernelTime;
        ftLastUserTime = ftUserTime;
    }
    return 0;
}

void CBaseTool::ShowWindow( const tstring& data )
{
    tstring timestamp = CBaseTool::GetPhysicsDateTime_AsStr();
#ifdef _CONSOLE
    printf( "[%s] %s\n", timestamp.c_str(), data.c_str() );
#endif
#ifdef _WINDOWS
#endif
}

void CBaseTool::ShowWindow( const char* format, ... )
{
    char szLogBuff[FORMAT_LEN] = { 0 };
    va_list arg_ptr;
    va_start( arg_ptr, format );
    vsnprintf_s( szLogBuff, sizeof( szLogBuff ) - 1, _TRUNCATE, format, arg_ptr );
    va_end( arg_ptr );
    ShowWindow( tstring( szLogBuff ) );
}

int CBaseTool::GetPhysicsDate_AsInt()
{
    return atoi( GetPhysicsDate_AsStr().c_str() );
}

tstring CBaseTool::GetPhysicsDate_AsStr()
{
    char szDate[16] = {0};
    SYSTEMTIME st = {0};
    GetLocalTime( &st );
    sprintf_s( szDate, "%04d%02d%02d", st.wYear, st.wMonth, st.wDay );
    return tstring( szDate );
}

int CBaseTool::GetPhysicsTime_AsInt()
{
    return atoi( GetPhysicsTime_AsStr().c_str() );
}

tstring CBaseTool::GetPhysicsTime_AsStr()
{
    char szTime[16] = {0};
    SYSTEMTIME st = {0};
    GetLocalTime( &st );
    sprintf_s( szTime, "%02d%02d%02d", st.wHour, st.wMinute, st.wSecond );
    return tstring( szTime );
}

//long long CBaseTool::GetPhysicsDateTime_Asll()
//{
//    return ( long long )( GetPhysicsDate_AsInt() * 1000000 + GetPhysicsTime_AsInt() );
//}

long long CBaseTool::GetPhysicsDateTime_Asll()
{
    tstring tmp = GetPhysicsDateTime_AsStr();
    tstringstream tss( tmp.c_str() );
    long long llRet;
    tss >> llRet;
    return llRet;
}

tstring CBaseTool::GetPhysicsDateTime_AsStr()
{
    return tstring( GetPhysicsDate_AsStr() + GetPhysicsTime_AsStr() );
}

long long CBaseTool::GetTimestamp_Milli()
{
    boost::posix_time::ptime epoch( boost::gregorian::date( 1970, boost::gregorian::Jan, 1 ) );
    boost::posix_time::time_duration time_from_epoch =  boost::posix_time::microsec_clock::universal_time() - epoch;
    return time_from_epoch.total_milliseconds();
}

long long CBaseTool::GetTimestamp_Micro()
{
    boost::posix_time::ptime epoch( boost::gregorian::date( 1970, boost::gregorian::Jan, 1 ) );
    boost::posix_time::time_duration time_from_epoch =  boost::posix_time::microsec_clock::universal_time() - epoch;
    return time_from_epoch.total_microseconds();
}

tstring CBaseTool::tformat( const char* format, ... )
{
    char szTmp[FORMAT_LEN] = {0};
    va_list arg_ptr;
    va_start( arg_ptr, format );
    vsnprintf_s( szTmp, sizeof( szTmp ) - 1, _TRUNCATE, format, arg_ptr );
    va_end( arg_ptr );
    return tstring( szTmp );
}

tstring CBaseTool::trim_l( tstring& s )
{
    s = boost::trim_left_copy( s );
    return s;
}

tstring CBaseTool::trim_r( tstring& s )
{
    s = boost::trim_right_copy( s );
    return s;
}

tstring CBaseTool::trim( tstring& s )
{
    s = trim_l( s );
    s = trim_r( s );
    return s;
}


tstring CBaseTool::trim_ex( tstring& s )
{
    if( s.empty() )
    {
        return s;
    }
    s.erase( 0, s.find_first_not_of( " " ) );
    s.erase( s.find_last_not_of( " " ) + 1 );
    return s;
}

int CBaseTool::split_k( const tstring& strSrc, const tstring& strSep, std::vector<tstring>& vctStringList, bool bTrim/*=true*/ )
{
    boost::char_separator<char> sep( strSep.c_str(), "", boost::keep_empty_tokens );
    typedef boost::tokenizer<boost::char_separator<char>>   CustonTokenizer;
    CustonTokenizer tok( strSrc, sep );
    // 输出分割结果
    for( CustonTokenizer::iterator beg = tok.begin(); beg != tok.end(); ++beg )
    {
        std::string strValue = *beg;
        if( bTrim )
            strValue = trim( strValue );
        vctStringList.push_back( strValue );
    }
    return vctStringList.size();
}


int CBaseTool::split_d( const tstring& strSrc, const tstring& strSep, std::vector<tstring>& vctStringList, bool bTrim /*= true */ )
{
    boost::char_separator<char> sep( strSep.c_str(), "", boost::drop_empty_tokens );
    typedef boost::tokenizer<boost::char_separator<char>>   CustonTokenizer;
    CustonTokenizer tok( strSrc, sep );
    // 输出分割结果
    for( CustonTokenizer::iterator beg = tok.begin(); beg != tok.end(); ++beg )
    {
        std::string strValue = *beg;
        if( bTrim )
            strValue = trim( strValue );
        vctStringList.push_back( strValue );
    }
    return vctStringList.size();
}

int CBaseTool::split_ex_k( const tstring& strSrc, const tstring& strSep, std::vector<tstring>& vctStringList, bool bTrim /*= true */ )
{
    //由于:和,也可能为变量值。这里加入对''的字符串化处理。
    int iNewPos = strSrc.find( strSep, 0 );
    int iLastPos = 0;
    std::string strTemp;
    while( iNewPos >= iLastPos )
    {
        strTemp = strSrc.substr( iLastPos, iNewPos - iLastPos );
        if( strTemp.length() )
        {
            int iPosDYH1  = strTemp.find( "'", 0 );
            if( iPosDYH1 >= 0 )
            {
                int iPosDYH2 = strSrc.find( "'", iLastPos + iPosDYH1 + 1 );
                if( iPosDYH2 < 0 )
                {
                    break;
                }
                iNewPos = iPosDYH2 + 1;
                strTemp = strSrc.substr( iLastPos, iNewPos - iLastPos );
            }
            if( bTrim )
            {
                strTemp = trim( strTemp );
            }
            if( strTemp.at( 0 ) == '\'' && strTemp.at( strTemp.length() - 1 ) == '\'' )
            {
                strTemp = strTemp.substr( 1, strTemp.length() - 2 );
            }
            vctStringList.push_back( strTemp );
        }
        iLastPos = iNewPos + strSep.length();
        iNewPos = strSrc.find( strSep, iLastPos );
    }
    if( ( int )strSrc.length() >= iLastPos )
    {
        strTemp = strSrc.substr( iLastPos, -1 );
        if( bTrim )
        {
            strTemp = trim( strTemp );
        }
        vctStringList.push_back( strTemp );
    }
    return vctStringList.size();
}

int CBaseTool::split_ex_d( const tstring& strSrc, const tstring& strSep, std::vector<tstring>& vctStringList, bool bTrim/*=true*/ )
{
    //由于:和,也可能为变量值。这里加入对''的字符串化处理。
    int iNewPos = strSrc.find( strSep, 0 );
    int iLastPos = 0;
    std::string strTemp;
    while( iNewPos >= iLastPos )
    {
        strTemp = strSrc.substr( iLastPos, iNewPos - iLastPos );
        if( strTemp.length() )
        {
            int iPosDYH1  = strTemp.find( "'", 0 );
            if( iPosDYH1 >= 0 )
            {
                int iPosDYH2 = strSrc.find( "'", iLastPos + iPosDYH1 + 1 );
                if( iPosDYH2 < 0 )
                {
                    break;
                }
                iNewPos = iPosDYH2 + 1;
                strTemp = strSrc.substr( iLastPos, iNewPos - iLastPos );
            }
            if( bTrim )
            {
                strTemp = trim( strTemp );
            }
            if( strTemp.at( 0 ) == '\'' && strTemp.at( strTemp.length() - 1 ) == '\'' )
            {
                strTemp = strTemp.substr( 1, strTemp.length() - 2 );
            }
            vctStringList.push_back( strTemp );
        }
        iLastPos = iNewPos + strSep.length();
        iNewPos = strSrc.find( strSep, iLastPos );
    }
    if( ( int )strSrc.length() > iLastPos )
    {
        strTemp = strSrc.substr( iLastPos, -1 );
        if( strTemp.length() )
        {
            if( bTrim )
            {
                strTemp = trim( strTemp );
            }
            vctStringList.push_back( strTemp );
        }
    }
    return vctStringList.size();
}

tstring CBaseTool::all_upper( tstring& s )
{
    s = boost::to_upper_copy( s );
    return s;
}

tstring CBaseTool::all_lower( tstring& s )
{
    s = boost::to_lower_copy( s );
    return s;
}

tstring CBaseTool::all_replace( tstring& s, tstring& key /*= tstring(" ")*/, tstring& rep /*= tstring("")*/ )
{
    s = boost::replace_all_copy( s, key, rep );
    return s;
}

tstring CBaseTool::json_to_str( Json::Value& json )
{
    Json::FastWriter fw;
    tstring strRet = fw.write( json );
    int iLen = strRet.length();
    if( strRet.find_last_of( 0x0a ) != tstring::npos )      //Json::FastWriter::write()函数生成的string末尾莫名其妙多了一个0x0a，导致日志出现多余的换行。
    {
        strRet = strRet.substr( 0, iLen - 1 );
    }
    return strRet;
}

int CBaseTool::str_to_json( tstring& str, Json::Value& json, tstring& err )
{
    Json::Reader reader;
    try
    {
        if( !reader.parse( str, json ) )
        {
            err = reader.getFormatedErrorMessages();
            return -1;
        }
    }
    catch( std::exception& e )
    {
        err = e.what();
        return -2;
    }
    catch( ... )
    {
        err = "unknow Exception";
        return -3;
    }
    return 0;
}

tstring CBaseTool::get_json_val( const Json::Value& json, const tstring& key )
{
    tstring strRet = "";
    if( json.type() != Json::objectValue )
    {
        return strRet;
    }
    if( !json.isMember( key ) )
    {
        return strRet;
    }
    Json::ValueType vt = json[key].type();
    switch( vt )
    {
        case Json::stringValue:
            {
                strRet = json[key].asString();
            }
            break;
        case Json::intValue:
            {
                int iRet = json[key].asInt();
                std::ostringstream ss;
                ss << iRet;
                strRet = ss.str();
            }
            break;
        case Json::uintValue:
            {
                unsigned int uiRet = json[key].asUInt();
                std::ostringstream ss;
                ss << uiRet;
                strRet = ss.str();
            }
            break;
        case Json::realValue:
            {
                double dRet = json[key].asDouble();
                std::ostringstream ss;
                ss << dRet;
                strRet = ss.str();
            }
            break;
        default:
            strRet = "unknow json value type";
            break;
    }
    strRet = trim( strRet );
    return strRet;
}

Json::Value CBaseTool::make_json( const tstring& name, const tstring& value )
{
    Json::Value jsTmp;
    jsTmp[name] = value;
    return jsTmp;
}
tstring CBaseTool::get_ptree_val_secure( const boost::property_tree::ptree& node, const tstring& path )
{
    std::string value;
    try
    {
        value = node.get<std::string>( path );
    }
    catch( boost::property_tree::ptree_error e )
    {
        value = "";
    }
    return value;
}

//15:00:01-15:30:00
int CBaseTool::AnalysisTimeZone( tstring timezone, int& iBeginTime, int& iEndTime )
{
    iBeginTime = 0;
    iEndTime = 0;
    trim( timezone );
    all_replace( timezone );
    if( timezone.length() == 0 )
    {
        return -1;
    }
    if( all_lower( timezone ) == "all" )
    {
        iBeginTime = 000000;
        iEndTime = 240000;
        return 0;
    }
    std::vector<tstring> vcTime;
    int iNum = split_k( timezone, "-", vcTime );
    std::vector<tstring>  vcStart;
    int iNum1 = split_k( vcTime[0], ":", vcStart );
    std::vector<tstring>  vcEnd;
    int iNum2 = split_k( vcTime[1], ":", vcEnd );
    if( iNum != 2 || iNum1 != 3 || iNum2 != 3 )
    {
        return -1;
    }
    iBeginTime = atoi( vcStart[0].c_str() ) * 10000 + atoi( vcStart[1].c_str() ) * 100 + atoi( vcStart[2].c_str() );
    iEndTime = atoi( vcEnd[0].c_str() ) * 10000 + atoi( vcEnd[1].c_str() ) * 100 + atoi( vcEnd[2].c_str() );
    return 0;
}


void CBaseTool::OutputDebugStringEx( const char* format, ... )
{
#ifdef _WIN32
    static DWORD dwCount = 0;
    va_list arg_ptr;
    char szMessageTemp[8192] = {0};
    char szMessage[8192] = {0};
    va_start( arg_ptr, format );
    _vsnprintf_s( szMessageTemp, sizeof( szMessageTemp ) - 1, format, arg_ptr );
    va_end( arg_ptr );
#ifdef _DEBUG //如果是调试版本，在最后加个回车换行，要不然在VC里显示成一堆难看
    _snprintf_s( szMessage, sizeof( szMessage ) - 1, "%03u [%04x][%s]\n", ( WORD )dwCount, GetCurrentThreadId(), szMessageTemp );
#else
    _snprintf_s( szMessage, sizeof( szMessage ) - 1, "%03u [%04x][%s]", ( WORD )dwCount, GetCurrentThreadId(), szMessageTemp );
#endif
#ifdef WIN32
    OutputDebugStringA( szMessage );
#endif
#endif
}

tstring CBaseTool::RelativePathToCompletePath( const tstring& path )
{
    return boost::filesystem::system_complete( path ).string();
}

tstring CBaseTool::CompletePathToRelativePath( const tstring& path )
{
    return boost::filesystem::relative( path ).string();
}

#pragma comment (lib,"ws2_32.lib")
tstring CBaseTool::GetLocalIpAddress()
{
    WSADATA wsaData;
    int ret = WSAStartup( MAKEWORD( 2, 2 ), &wsaData );
    if( ret != 0 )
    {
        return "get_ip_fail";
    }
    char hostname[256];
    ret = gethostname( hostname, sizeof( hostname ) );
    if( ret == SOCKET_ERROR )
    {
        return "get_ip_fail";
    }
    HOSTENT* host = gethostbyname( hostname );
    if( host == NULL )
    {
        return "get_ip_fail";
    }
    char ip[32] = {0};
    strcpy_s( ip, inet_ntoa( *( in_addr* )*host->h_addr_list ) );
    WSACleanup();
    return trim( std::string( ip ) );
}

tstring CBaseTool::GetProgramLaunchDir()
{
    tstring strPath = boost::filesystem::initial_path<boost::filesystem::path>().string();
    strPath = all_replace( strPath, tstring( "\\" ), tstring( "/" ) ); //这里需要杜绝转移符的歧义
    return strPath;
}

tstring CBaseTool::GetCurrentDir()
{
    tstring strPath = boost::filesystem::current_path().string();
    strPath = all_replace( strPath, tstring( "\\" ), tstring( "/" ) ); //这里需要杜绝转移符的歧义
    return strPath;
}

tstring CBaseTool::CutDirFromPath( const tstring& path )
{
    tstring strDir;
    tstring strFullPath = path;
    strFullPath = all_replace( strFullPath, tstring( "\\" ), tstring( "/" ) );
    if( boost::filesystem::is_directory( strFullPath ) )
    {
        return strFullPath;
    }
    int iPos = strFullPath.find_last_of( "/" );
    if( iPos > 0 )
    {
        strDir = strFullPath.substr( 0, iPos );
    }
    return strDir;
}

tstring CBaseTool::CutFileFromPath( const tstring& path )
{
    tstring strFile;
    tstring strFullPath = path;
    strFullPath = all_replace( strFullPath, tstring( "\\" ), tstring( "/" ) );
    if( boost::filesystem::is_directory( strFullPath ) )
    {
        return tstring( "" );
    }
    int iPos = strFullPath.find_last_of( "/" );
    if( iPos > 0 && iPos < ( int )strFullPath.length() )
    {
        strFile = strFullPath.substr( iPos + 1 );
    }
    return strFile;
}

int CBaseTool::SetCurrentDir( const tstring& path, tstring& err )
{
    boost::system::error_code errcode;
    boost::filesystem::current_path( boost::filesystem::path( path ), errcode );
    if( errcode.value() != 0 )
    {
        err = errcode.message();
        return -1;
    }
    return 0;
}

bool CBaseTool::IsPathExists( const tstring& path )
{
    return boost::filesystem::exists( path );
}

int CBaseTool::CreatePath( const tstring& path )
{
    bool bRet = boost::filesystem::create_directories( path );
    if( false == bRet )
    {
        return -1;
    }
    return 0;
}

void CBaseTool::ShowCpuPercent()
{
    HANDLE hEvent;
    BOOL iRet;
    FILETIME ftLastIdleTime;    //上一次闲置时间量
    FILETIME ftLastKernelTime;  //上一次内核时间量
    FILETIME ftLastUserTime;    //上一次用户时间量
    FILETIME ftIdleTime;        //闲置时间量
    FILETIME ftKernelTime;      //内核时间量
    FILETIME ftUserTime;        //用户时间量
    iRet = GetSystemTimes( &ftIdleTime, &ftKernelTime, &ftUserTime );
    ftLastIdleTime = ftIdleTime;
    ftLastKernelTime = ftKernelTime;
    ftLastUserTime = ftUserTime;
    hEvent = CreateEventA( NULL, FALSE, FALSE, NULL );
    while( true )
    {
        WaitForSingleObject( hEvent, 1000 );
        iRet = GetSystemTimes( &ftIdleTime, &ftKernelTime, &ftUserTime );
        long long llIdleDiff = CompareFileTime( ftLastIdleTime, ftIdleTime );
        long long llKernelDiff = CompareFileTime( ftLastKernelTime, ftKernelTime );
        long long llUserDiff = CompareFileTime( ftLastUserTime, ftUserTime );
        long long llCpuUse = ( llKernelDiff + llUserDiff - llIdleDiff ) * 100 / ( llKernelDiff + llUserDiff );
        long long llCpuIdle = ( llIdleDiff ) * 100 / ( llKernelDiff + llUserDiff );
        CBaseTool::ShowWindow( "CPU使用率=[%lld%%] CPU空闲率=[%lld%%]\n", llCpuUse, llCpuIdle );
        ftLastIdleTime = ftIdleTime;
        ftLastKernelTime = ftKernelTime;
        ftLastUserTime = ftUserTime;
    }
}

int CBaseTool::GetMemoryPercent()
{
    MEMORYSTATUS ms;
    ::GlobalMemoryStatus( &ms );
    return ( int )ms.dwMemoryLoad;
}

long long CBaseTool::CompareFileTime( FILETIME time1, FILETIME time2 )
{
    long long a = ( long long )time1.dwHighDateTime << 32 | time1.dwLowDateTime;
    long long b = ( long long )time2.dwHighDateTime << 32 | time2.dwLowDateTime;
    return ( b - a );
}

int CBaseTool::GetIOInfo( ULONGLONG& read_bytes, ULONGLONG& write_bytes, ULONGLONG& wct, ULONGLONG& rct )
{
    IO_COUNTERS io_counter;
    HANDLE hProcess = GetCurrentProcess(); //获取当前进程句柄
    if( GetProcessIoCounters( hProcess, &io_counter ) )
    {
        read_bytes = io_counter.ReadTransferCount;//字节数
        write_bytes = io_counter.WriteTransferCount;
        wct = io_counter.WriteOperationCount; //次数
        rct = io_counter.ReadOperationCount;
        return 0;
    }
    return -1;
}

void CBaseTool::StartRefreshCpu()
{
    m_iRefreshCpuFlag = TRUE;
    m_hdRefreshCpuThd = ( HANDLE )_beginthreadex( NULL, 0, RefreshCpuUseRateThreadFunc, NULL, 0, NULL );
}

void CBaseTool::StopRefreshCpu()
{
    m_iRefreshCpuFlag = FALSE;
    WaitForSingleObject( m_hdRefreshCpuThd, INFINITE );
}

void CBaseTool::GetCpuUseInfo( long long& llCpuUse, long long& llCpuIdle )
{
    CAutoLock lock( m_csCpu );
    llCpuUse = m_llCpuUse > 0 ? m_llCpuUse : 0;
    llCpuIdle = m_llCpuIdle > 0 ? m_llCpuIdle : 0;
}
