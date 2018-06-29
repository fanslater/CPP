#pragma once
#include "tstring.h"

#define FORMAT1             "%d,%d,%d,%d"
#define FORMAT1_REGEX_STR   "(\\d+,\\d+,\\d+,\\d+)"
#define FORMAT2             "%d, %d, %d, %d"
#define FORMAT2_REGEX_STR   "(\\d+, \\d+, \\d+, \\d+)"

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
    static void replase_line( const tstring& src, tstring& dst );
    static void replase_line_bytime( const tstring& src, tstring& dst );
private:
    static tstring VersionAdd( const tstring& strVersionNo );
    static tstring MakeVersionByTime( const tstring& strVersionNo );
};
