#include "StdAfx.h"
#include "BaseTool.h"
#include <boost/tokenizer.hpp>
#include <boost/algorithm/string.hpp>
#include <boost/filesystem/path.hpp>
#include <boost/filesystem/operations.hpp>
//#include <boost/asio.hpp>
//#include <WinSock2.h>
//#include <sstream>
//#include <Windows.h>
#include "global.h"

CBaseTool::CBaseTool(void)
{
}

CBaseTool::~CBaseTool(void)
{
}

int CBaseTool::GetPhysicsDate_AsInt()
{
    return atoi(GetPhysicsDate_AsStr().c_str());
}

tstring CBaseTool::GetPhysicsDate_AsStr()
{
    char szDate[16] = {0};
    SYSTEMTIME st = {0};
    GetLocalTime(&st);
    sprintf_s(szDate, "%04d%02d%02d", st.wYear, st.wMonth, st.wDay);
    return tstring(szDate);
}

int CBaseTool::GetPhysicsTime_AsInt()
{
    return atoi(GetPhysicsTime_AsStr().c_str());
}

tstring CBaseTool::GetPhysicsTime_AsStr()
{
    char szTime[16] = {0};
    SYSTEMTIME st = {0};
    GetLocalTime(&st);
    sprintf_s(szTime, "%02d%02d%02d", st.wHour, st.wMinute, st.wSecond);
    return tstring(szTime);
}

long long CBaseTool::GetPhysicsDateTime_Asll()
{
    return (long long)(GetPhysicsDate_AsInt() * 1000000 + GetPhysicsTime_AsInt());
}

tstring CBaseTool::GetPhysicsDateTime_AsStr()
{
    return tstring(GetPhysicsDate_AsStr() + GetPhysicsTime_AsStr());
}

long long CBaseTool::GetTimestamp_Milli()
{
    boost::posix_time::ptime epoch(boost::gregorian::date(1970, boost::gregorian::Jan, 1));
    boost::posix_time::time_duration time_from_epoch =  boost::posix_time::microsec_clock::universal_time() - epoch;
    return time_from_epoch.total_milliseconds();
}

long long CBaseTool::GetTimestamp_Micro()
{
    boost::posix_time::ptime epoch(boost::gregorian::date(1970, boost::gregorian::Jan, 1));
    boost::posix_time::time_duration time_from_epoch =  boost::posix_time::microsec_clock::universal_time() - epoch;
    return time_from_epoch.total_microseconds();
}

tstring CBaseTool::tformat(const char *format, ...)
{
    char szTmp[FORMAT_LEN] = {0};
    va_list arg_ptr;
    va_start(arg_ptr, format);
    vsnprintf_s(szTmp, sizeof(szTmp) - 1, _TRUNCATE, format, arg_ptr);
    va_end(arg_ptr);
    return tstring(szTmp);
}

tstring CBaseTool::trim_l(tstring &s)
{
    s = boost::trim_left_copy(s);
    return s;
}

tstring CBaseTool::trim_r(tstring &s)
{
    s = boost::trim_right_copy(s);
    return s;
}

tstring CBaseTool::trim(tstring &s)
{
    s = trim_l(s);
    s = trim_r(s);
    return s;
}


int CBaseTool::split(const tstring &strSrc, const tstring &strSep, std::vector<tstring> &vctStringList, bool bTrim/*=true*/)
{
    boost::char_separator<char> sep(strSep.c_str(), "", boost::keep_empty_tokens);
    typedef boost::tokenizer<boost::char_separator<char>>   CustonTokenizer;
    CustonTokenizer tok(strSrc, sep);

    // 输出分割结果
    for(CustonTokenizer::iterator beg = tok.begin(); beg != tok.end(); ++beg)
    {
        std::string strValue = *beg;
        if (bTrim)
            strValue = trim(strValue);

        vctStringList.push_back(strValue);
    }

    return vctStringList.size();
}


int CBaseTool::split_ex(const tstring &strSrc, const tstring &strSep, std::vector<tstring> &vctStringList, bool bTrim/*=true*/)
{
    //由于:和,也可能为变量值。这里加入对''的字符串化处理。
    int nNewPos = strSrc.find (strSep, 0);
    int nPos = 0;
    std::string strTemp;
    while( nNewPos >= nPos )
    {
        strTemp = strSrc.substr(nPos, nNewPos - nPos);
        if(strTemp.length())
        {
            int nPos1  = strTemp.find("'", 0);
            if(nPos1 >= 0)
            {
                nNewPos = strSrc.find ("'", nPos + nPos1 + 1) + 1;
                strTemp = strSrc.substr(nPos, nNewPos - nPos);
                if(strSep == ":")
                {
                    strTemp = strTemp.substr(1, strTemp.length() - 2);
                }
            }
            if(bTrim)
            {
                strTemp = trim(strTemp);
            }
            vctStringList.push_back(strTemp);
        }
        nPos = nNewPos + strSep.length();
        nNewPos = strSrc.find (strSep, nPos);
    }
    if((int)strSrc.length() > nPos)
    {
        strTemp = strSrc.substr(nPos, -1);
        if(strTemp.length())
        {
            if(bTrim)
            {
                strTemp = trim(strTemp);
            }
            vctStringList.push_back(strTemp);
        }
    }

    return vctStringList.size();
}

tstring CBaseTool::all_upper(tstring &s)
{
    s = boost::to_upper_copy(s);
    return s;
}

tstring CBaseTool::all_lower(tstring &s)
{
    s = boost::to_lower_copy(s);
    return s;
}

tstring CBaseTool::all_replace(tstring &s, tstring &key /*= tstring(" ")*/, tstring &rep /*= tstring("")*/)
{
    s = boost::replace_all_copy(s, key, rep);
    return s;
}

tstring CBaseTool::json_to_str(Json::Value &json)
{
    Json::FastWriter fw;
    return tstring(fw.write(json));
}

int CBaseTool::str_to_json(tstring &str, Json::Value &json, tstring &err)
{
    Json::Reader reader;
    try
    {
        if (!reader.parse(str, json))
        {
            err = reader.getFormatedErrorMessages();
            return -1;
        }
    }
    catch (std::exception &e)
    {
        err = e.what();
        return -2;
    }
    catch (...)
    {
        err = "unknow Exception";
        return -3;
    }
    return 0;
}

tstring CBaseTool::get_json_val(Json::Value &json, const tstring &key)
{
    tstring strRet = "";
    if (json.type() != Json::objectValue)
    {
        return strRet;
    }
    if (!json.isMember(key))
    {
        return strRet;
    }
    Json::ValueType vt = json[key].type();
    switch(vt)
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

    strRet = trim(strRet);
    return strRet;
}

tstring CBaseTool::get_ptree_val_secure(const boost::property_tree::ptree &node, const tstring &path)
{
    std::string value;
    try
    {
        value = node.get<std::string>(path);
    }
    catch(boost::property_tree::ptree_error e)
    {
        value = "";
    }
    return value;
}

//15:00:01-15:30:00
int CBaseTool::AnalysisTimeZone(tstring timezone, int &iBeginTime, int &iEndTime)
{
    iBeginTime = 0;
    iEndTime = 0;

    trim(timezone);
    all_replace(timezone);
    if (timezone.length() == 0)
    {
        return -1;
    }
    if (all_lower(timezone) == "all")
    {
        iBeginTime = 000000;
        iEndTime = 240000;
        return 0;
    }
    std::vector<tstring> vcTime;
    int iNum = split(timezone, "-", vcTime);
    std::vector<tstring>  vcStart;
    int iNum1 = split(vcTime[0], ":", vcStart);
    std::vector<tstring>  vcEnd;
    int iNum2 = split(vcTime[1], ":", vcEnd);
    if (iNum != 2 || iNum1 != 3 || iNum2 != 3)
    {
        return -1;
    }
    iBeginTime = atoi(vcStart[0].c_str()) * 10000 + atoi(vcStart[1].c_str()) * 100 + atoi(vcStart[2].c_str());
    iEndTime = atoi(vcEnd[0].c_str()) * 10000 + atoi(vcEnd[1].c_str()) * 100 + atoi(vcEnd[2].c_str());

    return 0;
}


void CBaseTool::OutputDebugStringEx(const char *format, ...)
{
#ifdef _WIN32
    static DWORD dwCount = 0;
    va_list arg_ptr;
    char szMessageTemp[8192] = {0};
    char szMessage[8192] = {0};

    va_start(arg_ptr, format);
    _vsnprintf_s(szMessageTemp, sizeof(szMessageTemp) - 1, format, arg_ptr);
    va_end(arg_ptr);

#ifdef _DEBUG //如果是调试版本，在最后加个回车换行，要不然在VC里显示成一堆难看
    _snprintf_s(szMessage, sizeof(szMessage) - 1, "%03u [%04x][%s]\n", (WORD)dwCount, GetCurrentThreadId(), szMessageTemp);
#else
    _snprintf(szMessage, sizeof(szMessage) - 1, "%03u [%04x][%s]", (WORD)dwCount, GetCurrentThreadId(), szMessageTemp);
#endif

#ifdef WIN32
    OutputDebugStringA(szMessage);
#endif

#endif
}

#pragma comment (lib,"ws2_32.lib")
tstring CBaseTool::GetLocalIpAddress()
{
    WSADATA wsaData;
    int ret = WSAStartup(MAKEWORD(2, 2), &wsaData);
    if (ret != 0)
    {
        return "get_ip_fail";
    }
    char hostname[256];
    ret = gethostname(hostname, sizeof(hostname));
    if (ret == SOCKET_ERROR)
    {
        return "get_ip_fail";
    }
    HOSTENT *host = gethostbyname(hostname);
    if (host == NULL)
    {
        return "get_ip_fail";
    }
    char ip[32] = {0};
    strcpy_s(ip, inet_ntoa(*(in_addr *)*host->h_addr_list));
    WSACleanup();
    return trim(std::string(ip));
}

tstring CBaseTool::GetProcedurePath()
{
    tstring strPath = boost::filesystem::initial_path<boost::filesystem::path>().string();
    strPath = all_replace(strPath, tstring("\\"), tstring("/")); //这里需要杜绝转移符的歧义
    return strPath;
}

bool CBaseTool::IsPathExists(const tstring &path)
{
    return boost::filesystem::exists(path);
}

int CBaseTool::CreatePath(const tstring &path)
{
    bool bRet = boost::filesystem::create_directories(path);
    if (false == bRet)
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

    iRet = GetSystemTimes(&ftIdleTime, &ftKernelTime, &ftUserTime);
    ftLastIdleTime = ftIdleTime;
    ftLastKernelTime = ftKernelTime;
    ftLastUserTime = ftUserTime;

    hEvent = CreateEventA(NULL, FALSE, FALSE, NULL);

    while (true)
    {
        WaitForSingleObject(hEvent, 1000);
        iRet = GetSystemTimes(&ftIdleTime, &ftKernelTime, &ftUserTime);
        long long llIdleDiff = CompareFileTime(ftLastIdleTime, ftIdleTime);
        long long llKernelDiff = CompareFileTime(ftLastKernelTime, ftKernelTime);
        long long llUserDiff = CompareFileTime(ftLastUserTime, ftUserTime);
        long long llCpuUse = (llKernelDiff + llUserDiff - llIdleDiff) * 100 / (llKernelDiff + llUserDiff);
        long long llCpuIdle = (llIdleDiff) * 100 / (llKernelDiff + llUserDiff);
        global::ShowWindow("CPU使用率=[%lld%%] CPU空闲率=[%lld%%]\n", llCpuUse, llCpuIdle);
        ftLastIdleTime = ftIdleTime;
        ftLastKernelTime = ftKernelTime;
        ftLastUserTime = ftUserTime;
    }
}

int CBaseTool::GetMemoryPercent()
{
    MEMORYSTATUS ms;
    ::GlobalMemoryStatus(&ms);
    return (int)ms.dwMemoryLoad;
}

long long CBaseTool::CompareFileTime(FILETIME time1, FILETIME time2)
{
    long long a = (long long)time1.dwHighDateTime << 32 | time1.dwLowDateTime;
    long long b = (long long)time2.dwHighDateTime << 32 | time2.dwLowDateTime;
    return (b - a);
}
