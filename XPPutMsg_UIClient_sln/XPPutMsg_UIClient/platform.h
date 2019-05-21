#ifndef MYSTDINT_H_
#define MYSTDINT_H_

#include <stddef.h>
#include <stdio.h>

#if defined(_WIN32) && !defined(__MINGW32__)

typedef signed char int8_t;
typedef unsigned char uint8_t;
typedef short int16_t;  // NOLINT
typedef unsigned short uint16_t;  // NOLINT
typedef int int32_t;
typedef unsigned int uint32_t;
typedef __int64 int64_t;
typedef unsigned __int64 uint64_t;
// intptr_t and friends are defined in crtdefs.h through stdio.h.
#define snprintf _snprintf
#define stricmp _stricmp
#define vsnprintf _vsnprintf
#define getch _getch
#define kbhit _kbhit
#else //linux =================================================================

#include <stdint.h>	
#define stricmp strcasecmp
#define getch getchar
//#define _locking locking

#define DWORD   unsigned long
#define WORD    unsigned int
#define LPCTSTR const char *
#define LPTSTR  char *
#define BOOL    bool
#define LONG    unsigned long
#define TRUE    true
#define FALSE   false

//#include <algorithm>
//#define min std::min
//#define max std::max
//#define min(x,y) ({ typeof(x) _x = (x); typeof(y) _y = (y); (void) (&_x == &_y); _x < _y ? _x : _y; })
//#define max(x,y) ({ typeof(x) _x = (x); typeof(y) _y = (y); (void) (&_x == &_y); _x > _y ? _x : _y; })


typedef struct _SYSTEMTIME {
  WORD wYear;
  WORD wMonth;
  WORD wDayOfWeek;
  WORD wDay;
  WORD wHour;
  WORD wMinute;
  WORD wSecond;
  WORD wMilliseconds;
} SYSTEMTIME;

void GetLocalTime(SYSTEMTIME *pstSYSTEMTIME);
DWORD GetPrivateProfileString(
	LPCTSTR lpAppName, //配置文件的section名
	LPCTSTR lpKeyName, //配置文件的key名
	LPCTSTR lpDefault,
	LPTSTR lpReturnedString,
	DWORD nSize,
	LPCTSTR lpFileName
);


DWORD GetEnvironmentVariableA(
	LPCTSTR lpName,
	LPTSTR lpBuffer,
	DWORD nSize
);

BOOL SetEnvironmentVariableA(
	LPCTSTR lpName,
	LPCTSTR lpValue
);

DWORD GetTickCount(void);

void Sleep(int nTimeMS);
char* gets(char*);
int kbhit (void);
#endif
#endif