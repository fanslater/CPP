#pragma once

#include <string>
#include <sstream>
#include <tchar.h>

//
// #if defined(UNICODE) || defined(_UNICODE)
// typedef std::basic_string<TCHAR> tstring;
// #else
// typedef std::basic_string<char> tstring;
// #endif
#if defined(UNICODE) || defined(_UNICODE)
typedef std::wstring tstring;
typedef std::wstringstream tstringstream;
#else
typedef std::string tstring;
typedef std::stringstream tstringstream;
typedef std::ostringstream tostringstream;
#endif
