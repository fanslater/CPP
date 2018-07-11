#pragma once
#include "tstring.h"

#define FORMAT1             "FILEVERSION %d,%d,%d,%d"
#define FORMAT1_REGEX_STR   "(FILEVERSION \\d+,\\d+,\\d+,\\d+)"
#define FORMAT2             "\"FileVersion\", \"%d, %d, %d, %d"
#define FORMAT2_REGEX_STR   "(\"FileVersion\", \"\\d+, \\d+, \\d+, \\d+)"

#define FORMAT3             "PRODUCTVERSION %d,%d,%d,%d"
#define FORMAT3_REGEX_STR   "(PRODUCTVERSION \\d+,\\d+,\\d+,\\d+)"
#define FORMAT4             "\"ProductVersion\", \"%d, %d, %d, %d"
#define FORMAT4_REGEX_STR   "(\"ProductVersion\", \"\\d+, \\d+, \\d+, \\d+)"

#define PRODUCT_BUILD_REGEX_STR   "(PRODUCT_BUILD \\d+)"
#define PRODUCT_BUILD_FORMAT    "PRODUCT_BUILD %lld"
#define FILE_BUILD_REGEX_STR    "(FILE_BUILD \\d+)"
#define FILE_BUILD_FORMAT       "FILE_BUILD %lld"


class CppRegex
{
public:
    CppRegex( void );
    ~CppRegex( void );
public:
    static void ignore_case();
    static void regex_match();
    static void regex_search();
    static void regex_replace();
    static void test_func();
public:
    static void replase_line( const tstring& src, tstring& dst );
    static void replase_line_bytime( const tstring& src, tstring& dst );
    static void replase_line_ex( const tstring& src, tstring& dst );
    static void replase_line_bytime_ex( const tstring& src, tstring& dst );
    static void replase_line_both( const tstring& src, tstring& dst );
private:
    static tstring VersionAdd( const tstring& strVersionNo );
    static tstring MakeVersionByTime( const tstring& strVersionNo );
};
