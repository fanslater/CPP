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
    tr1::regex pattern( "(.{5})_(\\d{4})" ); //ƥ��5�����ⵥ�ַ� + �»��� + 4������
    if( tr1::regex_match( str, result, pattern ) )
    {
        cout << result[0] << endl;      //����ƥ������Hello_2018
        cout << result[1] << endl;      //��һ��ƥ������ݣ�Hello
        cout << result[2] << endl;      //�ڶ���ƥ������ݣ�2018
        cout << "�����ʾ��ʽ2" << endl;
        cout << result.str() << endl;   //���������Hello_2018
        cout << result.str( 1 ) << endl; //��һ��ƥ������ݣ�Hello
        cout << result.str( 2 ) << endl; //�ڶ���ƥ������ݣ�2018
    }
    //�������
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
    tr1::regex pattern( "\\d{4}" );  //ƥ���ĸ�����
    //����������
    string::const_iterator iterStart = str.begin();
    string::const_iterator iterEnd = str.end();
    string temp;
    while( tr1::regex_search( iterStart, iterEnd, result, pattern ) )
    {
        temp = result[0];
        cout << temp << endl;
        iterStart = result[0].second;   //����������ʼλ��,����ʣ�µ��ַ���
    }
    cout << endl;
}

void CppRegex::regex_replace()
{
    string str = "Hello_2018!";
    tr1::regex pattern( "Hello" );
    cout << tr1::regex_replace( str, pattern, tstring( "" ) ) << endl; //�����_2018����Hello�滻Ϊ""
    cout << tr1::regex_replace( str, pattern, tstring( "233" ) ) << endl; //�����_2018����Hello�滻Ϊ2333
    tr1::regex pattern2( "(.{3})(.{2})_(\\d{4})" );  //ƥ��3�������ַ�+2�������ַ�+�»���+4������
    cout << tr1::regex_replace( str, pattern2, tstring( "$1$3" ) ) << endl; //�����Hel2018�����ַ����滻Ϊ��һ���͵��������ʽƥ�������
    cout << tr1::regex_replace( str, pattern2, tstring( "$1$3$2" ) ) << endl; //�����Hel2018lo������λ��˳��
    cout << endl;
}



void CppRegex::ignore_case()
{
    cout << tr1::regex_match( "aaaAAA", tr1::regex( "a*", tr1::regex::icase ) ) << endl; //���Ϊ1
    cout << tr1::regex_match( "aaaAAA", tr1::regex( "a*" ) ) << endl;             //���Ϊ0
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

