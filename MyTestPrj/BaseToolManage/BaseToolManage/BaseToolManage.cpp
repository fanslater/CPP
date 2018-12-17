// BaseToolManage.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"
//#include <iostream>
//#include "json/json.h"
//#include "BaseTool.h"

//void traversal( Json::Value jsObj )
//{
//    if( jsObj.type() == Json::arrayValue )
//    {
//        for( Json::Value::iterator jvit = jsObj.begin() ; jvit != jsObj.end() ; ++jvit )
//        {
//            traversal( *jvit );
//        }
//    }
//    else
//    {
//        Json::Value::Members jvmNames = jsObj.getMemberNames();
//        for( Json::Value::Members::iterator jvmit = jvmNames.begin() ; jvmit != jvmNames.end() ; ++jvmit )
//        {
//            tstring strName = *jvmit;
//            if( jsObj[strName].type() == Json::objectValue )
//            {
//                traversal( jsObj[strName] );
//            }
//            else if( jsObj[strName].type() == Json::arrayValue )
//            {
//                for( Json::Value::iterator jvit = jsObj[strName].begin() ; jvit != jsObj[strName].end() ; ++ jvit )
//                {
//                    traversal( *jvit );
//                }
//            }
//            else if( jsObj[strName].type() == Json::booleanValue )
//            {
//                printf( "[%s]=[%d]\n", strName.c_str(), jsObj[strName].asBool() );
//            }
//            else if( jsObj[strName].type() == Json::stringValue )
//            {
//                printf( "[%s]=[%s]\n", strName.c_str(), jsObj[strName].asString().c_str() );
//            }
//            else if( jsObj[strName].type() == Json::realValue )
//            {
//                printf( "[%s]=[%lf]\n", strName.c_str(), jsObj[strName].asDouble() );
//            }
//            else if( jsObj[strName].type() == Json::uintValue )
//            {
//                printf( "[%s]=[%u]\n", strName.c_str(), jsObj[strName].asUInt() );
//            }
//            else if( jsObj[strName].type() == Json::intValue )
//            {
//                printf( "[%s]=[%d]\n", strName.c_str(), jsObj[strName].asInt() );
//            }
//            else if( jsObj[strName].type() == Json::nullValue )
//            {
//                printf( "null\n" );
//            }
//        }
//    }
//}
//
//void ji_suan_bao_xian()
//{
//    //（7962.18*Y）*(（1+X）^Y)=360000
//    for( int y = 1 ; y <= 30 ; ++y )
//    {
//        int real = ( y > 20 ? 20 : y );
//        double jine = 7962.18 * real;
//        double bili = 360000.0 / jine;
//        double x = pow( bili, 1.0 / real );
//        double lilv = (x-1.0)*100;
//        printf( "第 %d 年出事了，收益率=%.04lf%%\n", y, lilv );
//    }
//}
//




int _tmain( int argc, _TCHAR* argv[] )
{
    

    return 0;
}

