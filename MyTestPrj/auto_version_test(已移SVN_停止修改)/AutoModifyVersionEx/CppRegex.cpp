#include "StdAfx.h"
#include "CppRegex.h"
#include <regex>
#include <iostream>
#include "BaseTool.h"
using namespace std;

CppRegex::CppRegex( void )
{
}

CppRegex::~CppRegex( void )
{
}



void CppRegex::regex_match()
{
    tstring str = "Hello_2018";
    tr1::smatch result;
    tr1::regex pattern( "(.{5})_(\\d{4})" ); //匹配5个任意单字符 + 下划线 + 4个数字
    if( tr1::regex_match( str, result, pattern ) )
    {
        cout << result[0] << endl;      //完整匹配结果，Hello_2018
        cout << result[1] << endl;      //第一组匹配的数据，Hello
        cout << result[2] << endl;      //第二组匹配的数据，2018
        cout << "结果显示形式2" << endl;
        cout << result.str() << endl;   //完整结果，Hello_2018
        cout << result.str( 1 ) << endl; //第一组匹配的数据，Hello
        cout << result.str( 2 ) << endl; //第二组匹配的数据，2018
    }
    //遍历结果
    for( int i = 0; i < ( int )result.size(); ++i )
    {
        cout << result[i] << endl;
    }
    cout << endl;
}

void CppRegex::regex_search()
{
    string str = "Hello 2018, Bye 2017";
    tr1::smatch result;
    tr1::regex pattern( "\\d{4}" );  //匹配四个数字
    //迭代器声明
    string::const_iterator iterStart = str.begin();
    string::const_iterator iterEnd = str.end();
    string temp;
    while( tr1::regex_search( iterStart, iterEnd, result, pattern ) )
    {
        temp = result[0];
        cout << temp << endl;
        iterStart = result[0].second;   //更新搜索起始位置,搜索剩下的字符串
    }
    cout << endl;
}

void CppRegex::regex_replace()
{
    string str = "Hello_2018!";
    tr1::regex pattern( "Hello" );
    cout << tr1::regex_replace( str, pattern, tstring( "" ) ) << endl; //输出：_2018，将Hello替换为""
    cout << tr1::regex_replace( str, pattern, tstring( "233" ) ) << endl; //输出：_2018，将Hello替换为2333
    tr1::regex pattern2( "(.{3})(.{2})_(\\d{4})" );  //匹配3个任意字符+2个任意字符+下划线+4个数字
    cout << tr1::regex_replace( str, pattern2, tstring( "$1$3" ) ) << endl; //输出：Hel2018，将字符串替换为第一个和第三个表达式匹配的内容
    cout << tr1::regex_replace( str, pattern2, tstring( "$1$3$2" ) ) << endl; //输出：Hel2018lo，交换位置顺序
    cout << endl;
}



void CppRegex::ignore_case()
{
    cout << tr1::regex_match( "aaaAAA", tr1::regex( "a*", tr1::regex::icase ) ) << endl; //结果为1
    cout << tr1::regex_match( "aaaAAA", tr1::regex( "a*" ) ) << endl;             //结果为0
    cout << endl;
}

void CppRegex::replase_line( const tstring& src, tstring& dst )
{
    dst = src;
    std::tr1::regex fmtreg( FORMAT1_REGEX_STR  "|" FORMAT2_REGEX_STR "|" FORMAT3_REGEX_STR "|" FORMAT4_REGEX_STR );
    std::tr1::smatch result;
    if( std::tr1::regex_search( src, result, fmtreg ) )
    {
        tstring strVersionNo = result[0];
        dst = std::tr1::regex_replace( src, std::tr1::regex( strVersionNo ), VersionAdd( strVersionNo ) );
    }
}

void CppRegex::replase_line_bytime( const tstring& src, tstring& dst )
{
    dst = src;
    std::tr1::regex fmtreg( FORMAT1_REGEX_STR  "|" FORMAT2_REGEX_STR "|" FORMAT3_REGEX_STR "|" FORMAT4_REGEX_STR );
    std::tr1::smatch result;
    if( std::tr1::regex_search( src, result, fmtreg ) )
    {
        tstring strVersionNo = result[0];
        dst = std::tr1::regex_replace( src, std::tr1::regex( strVersionNo ), MakeVersionByTime( strVersionNo ) );
    }
}

void CppRegex::test_func()
{
    //tstring strText = " FILEVERSION 1,0,0,3";
    tstring strText = "            VALUE \"FileVersion\", \"1, 0, 0, 3\"";
    //
    std::tr1::regex fmtreg( FORMAT1_REGEX_STR  "|" FORMAT2_REGEX_STR );
    std::tr1::smatch result;
    if( std::tr1::regex_search( strText, result, fmtreg ) )
    {
        tstring strVersionNo = result[0];
        tstring strNewText = std::tr1::regex_replace( strText, std::tr1::regex( strVersionNo ), VersionAdd( strVersionNo ) );
    }
}

tstring CppRegex::VersionAdd( const tstring& strVersionNo )
{
    std::tr1::regex reg1( FORMAT1_REGEX_STR );
    std::tr1::regex reg2( FORMAT2_REGEX_STR );
    std::tr1::regex reg3( FORMAT3_REGEX_STR );
    std::tr1::regex reg4( FORMAT4_REGEX_STR );
    std::tr1::smatch result;
    bool bFlag1, bFlag2, bFlag3, bFlag4;
    bFlag1 = std::tr1::regex_search( strVersionNo, result, reg1 );
    bFlag2 = std::tr1::regex_search( strVersionNo, result, reg2 );
    bFlag3 = std::tr1::regex_search( strVersionNo, result, reg3 );
    bFlag4 = std::tr1::regex_search( strVersionNo, result, reg4 );
    if( bFlag1 || bFlag2 || bFlag3 || bFlag4 )
    {
        tstring strFormat = ( bFlag1 ? FORMAT1 : ( bFlag2 ? FORMAT2 : ( bFlag3 ? FORMAT3 : FORMAT4 ) ) );
        int Num[4] = {0};
        sscanf_s( strVersionNo.c_str(), strFormat.c_str(), &Num[0], &Num[1], &Num[2], &Num[3] );
        Num[3]++;
        return CBaseTool::tformat( strFormat.c_str(), Num[0], Num[1], Num[2], Num[3] );
    }
    return strVersionNo;
}

tstring CppRegex::MakeVersionByTime( const tstring& strVersionNo )
{
    std::tr1::regex reg1( FORMAT1_REGEX_STR );
    std::tr1::regex reg2( FORMAT2_REGEX_STR );
    std::tr1::regex reg3( FORMAT3_REGEX_STR );
    std::tr1::regex reg4( FORMAT4_REGEX_STR );
    std::tr1::smatch result;
    bool bFlag1, bFlag2, bFlag3, bFlag4;
    bFlag1 = std::tr1::regex_search( strVersionNo, result, reg1 );
    bFlag2 = std::tr1::regex_search( strVersionNo, result, reg2 );
    bFlag3 = std::tr1::regex_search( strVersionNo, result, reg3 );
    bFlag4 = std::tr1::regex_search( strVersionNo, result, reg4 );
    if( bFlag1 || bFlag2 || bFlag3 || bFlag4 )
    {
        tstring strFormat = ( bFlag1 ? FORMAT1 : ( bFlag2 ? FORMAT2 : ( bFlag3 ? FORMAT3 : FORMAT4 ) ) );
        int iDate = CBaseTool::GetPhysicsDate_AsInt();
        int iTime = CBaseTool::GetPhysicsTime_AsInt();
        int Num[4] = {0};
        sscanf_s( strVersionNo.c_str(), strFormat.c_str(), &Num[0], &Num[1], &Num[2], &Num[3] );
        return CBaseTool::tformat( strFormat.c_str(), Num[0], iDate / 10000, iDate % 10000, iTime );
    }
    return strVersionNo;
}

