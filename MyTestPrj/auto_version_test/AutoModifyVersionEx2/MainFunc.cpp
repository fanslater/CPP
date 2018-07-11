// AutoModifyVersionEx.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"
#include <iostream>
#include <fstream>
#include <vector>
#include "tstring.h"
#include "BaseTool.h"
#include "CppRegex.h"

#define VERSION_FILE    "./version.h"

typedef std::vector<tstring> stringvec;

int _tmain( int argc, _TCHAR* argv[] )
{
    tstring strRcFilePath = VERSION_FILE;
    if( argc >= 2 )
    {
        strRcFilePath = argv[1];
    }
    if( strRcFilePath.length() == 0 )
    {
        printf( "rc file path is null" );
        return -1;
    }
    //读取文件
    std::ifstream ifsFile;
    ifsFile.open( strRcFilePath.c_str() );
    if( ifsFile.is_open() == false )
    {
        printf( "%s file open fail", strRcFilePath.c_str() );
        return -1;
    }
    stringvec vcFile;
    tstring strLine;
    while( getline( ifsFile, strLine ) )
    {
        vcFile.push_back( strLine );
    }
    ifsFile.close();
    //重写文件
    std::ofstream ofsFile;
    ofsFile.open( strRcFilePath.c_str() );
    if( ofsFile.is_open() == false )
    {
        printf( "%s file open fail", strRcFilePath.c_str() );
        return -1;
    }
    //ofsFile.clear();
    for( stringvec::iterator svit = vcFile.begin() ; svit != vcFile.end() ; svit++ )
    {
        tstring strLine = ( *svit );
        tstring strNewLine;
        CppRegex::replase_line_both( strLine, strNewLine );
        ofsFile << strNewLine << "\n";
    }
    ofsFile.close();
    return 0;
}

