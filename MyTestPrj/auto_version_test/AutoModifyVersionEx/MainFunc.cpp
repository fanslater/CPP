// AutoModifyVersionEx.cpp : �������̨Ӧ�ó������ڵ㡣
//

#include "stdafx.h"
#include <iostream>
#include <fstream>
#include <vector>
#include "tstring.h"
#include "BaseTool.h"
#include "CppRegex.h"

typedef std::vector<tstring> stringvec;

int _tmain( int argc, _TCHAR* argv[] )
{
    tstring strRcFilePath = "./AutoModifyVersion.rc";
    bool bIsTimeMode = false;
    if( argc >= 2 )
    {
        strRcFilePath = argv[1];
        if( argc == 3 )
        {
            if( 0 == strcmp( argv[2], "time" ) )
            {
                bIsTimeMode = true;
            }
        }
    }
    if( strRcFilePath.length() == 0 )
    {
        printf( "rc file path is null" );
        return -1;
    }
    //��ȡ�ļ�
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
    //��д�ļ�
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
        if( bIsTimeMode )
        {
            CppRegex::replase_line_bytime( strLine, strNewLine );
        }
        else
        {
            CppRegex::replase_line( strLine, strNewLine );
        }
        ofsFile << strNewLine << "\n";
    }
    ofsFile.close();
    return 0;
}
