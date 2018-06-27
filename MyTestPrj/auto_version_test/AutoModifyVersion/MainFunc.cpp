// AutoModifyVersion.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"
#include "tstring.h"
#include <fstream>
#include "BaseTool.h"


int ModifyRcFileData( tstring src, tstring& dst )
{    
    int iPos1 = src.find( "FILEVERSION" );
    if( iPos1 < 0 )
    {
        return -1;
    }
    int iPos2 = src.find( "\n", iPos1 );
    if( iPos2 < 0 )
    {
        return -1;
    }
    tstring strLine_FILEVERSION = src.substr( iPos1, iPos2 - iPos1 );
    int iPos3 = strLine_FILEVERSION.find_last_of( "," );
    int iFourthVerNo = atoi( strLine_FILEVERSION.substr( iPos3 + 1 ).c_str() );
    printf( "FILEVERSION [%d]->[%d]\n", iFourthVerNo, iFourthVerNo + 1 );
    tstring strFourthVerNoNew = CBaseTool::tformat( "%d", iFourthVerNo + 1 );
    tstring strLine_FILEVERSION_new = strLine_FILEVERSION.substr( 0, iPos3 + 1 );
    strLine_FILEVERSION_new.append( strFourthVerNoNew );
    dst.append( src.substr( 0, iPos1 ) );
    dst.append( strLine_FILEVERSION_new );
    //
    int iPos4 = src.find( "PRODUCTVERSION", iPos2 );
    if( iPos4 < 0 )
    {
        return -1;
    }
    int iPos5 = src.find( "\n", iPos4 );
    if( iPos5 < 0 )
    {
        return -1;
    }
    tstring strLine_PRODUCTVERSION = src.substr( iPos4, iPos5 - iPos4 );
    int iPos6 = strLine_PRODUCTVERSION.find_last_of( "," );
    iFourthVerNo = atoi( strLine_PRODUCTVERSION.substr( iPos6 + 1 ).c_str() );
    printf( "PRODUCTVERSION [%d]->[%d]\n", iFourthVerNo, iFourthVerNo + 1 );
    strFourthVerNoNew = CBaseTool::tformat( "%d", iFourthVerNo + 1 );
    tstring strLine_PRODUCTVERSION_new = strLine_PRODUCTVERSION.substr( 0, iPos6 + 1 );
    strLine_PRODUCTVERSION_new.append( strFourthVerNoNew );
    dst.append( src.substr( iPos2, iPos4 - iPos2 ) );
    dst.append( strLine_PRODUCTVERSION_new );
    //
    int iPos7 = src.find( "FileVersion", iPos5 );
    if( iPos7 < 0 )
    {
        return -1;
    }
    int iPos8 = src.find( "\n", iPos7 );
    if( iPos8 < 0 )
    {
        return -1;
    }
    tstring strLine_FileVersion = src.substr( iPos7, iPos8 - iPos7 );
    int iPos9 = strLine_FileVersion.find_last_of( ", " );
    int iPos10 = strLine_FileVersion.find_last_of( "\"" );
    iFourthVerNo = atoi( strLine_FileVersion.substr( iPos9 + 1, iPos10 - iPos9 - 1 ).c_str() );
    printf( "FileVersion [%d]->[%d]\n", iFourthVerNo, iFourthVerNo + 1 );
    strFourthVerNoNew = CBaseTool::tformat( "%d", iFourthVerNo + 1 );
    tstring strLine_FileVersion_new = strLine_FileVersion.substr( 0, iPos9 + 1 );
    strLine_FileVersion_new.append( strFourthVerNoNew );
    strLine_FileVersion_new.append( strLine_FileVersion.substr( iPos10 ) );
    dst.append( src.substr( iPos5, iPos7 - iPos5 ) );
    dst.append( strLine_FileVersion_new );
    //
    int iPos11 = src.find( "ProductVersion", iPos8 );
    if( iPos11 < 0 )
    {
        return -1;
    }
    int iPos12 = src.find( "\n", iPos11 );
    if( iPos12 < 0 )
    {
        return -1;
    }
    tstring strLine_ProductVersion = src.substr( iPos11, iPos12 - iPos11 );
    int iPos13 = strLine_ProductVersion.find_last_of( ", " );
    int iPos14 = strLine_ProductVersion.find_last_of( "\"" );
    iFourthVerNo = atoi( strLine_ProductVersion.substr( iPos13 + 1, iPos14 - iPos13 - 1 ).c_str() );
    printf( "ProductVersion [%d]->[%d]\n", iFourthVerNo, iFourthVerNo + 1 );
    strFourthVerNoNew = CBaseTool::tformat( "%d", iFourthVerNo + 1 );
    tstring strLine_ProductVersion_new = strLine_ProductVersion.substr( 0, iPos13 + 1 );
    strLine_ProductVersion_new.append( strFourthVerNoNew );
    strLine_ProductVersion_new.append( strLine_ProductVersion.substr( iPos14 ) );
    dst.append( src.substr( iPos8, iPos11 - iPos8 ) );
    dst.append( strLine_ProductVersion_new );
    //
    dst.append( src.substr( iPos12 ) );
    return 0;
}

int _tmain( int argc, _TCHAR* argv[] )
{
    tstring strRcFilePath = "./AutoModifyVersion.rc";
    if( argc >= 2 )
    {
        strRcFilePath = argv[1];
    }
    if( strRcFilePath.length() == 0 )
    {
        printf( "rc file path is null" );
        return -1;
    }
    std::ifstream ifsFile;
    ifsFile.open( strRcFilePath.c_str() );
    if( ifsFile.is_open() == false )
    {
        printf( "%s file open fail", strRcFilePath.c_str() );
        return -1;
    }
    tstringstream ssBuf;
    ssBuf << ifsFile.rdbuf();
    ifsFile.close();
    tstring strRcFileData = ssBuf.str();
    tstring strNewRcFileData;
    int iRet = ModifyRcFileData( strRcFileData, strNewRcFileData );
    if( iRet != 0 )
    {
        return -1;
    }
    std::ofstream ofsFile;
    ofsFile.open( strRcFilePath.c_str() );
    ofsFile << strNewRcFileData;
    ofsFile.close();
    return 0;
}

