// breakpad_test.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"

#include "client/windows/handler/exception_handler.h"

bool callback(const wchar_t *dump_path, const wchar_t *id, void *context, EXCEPTION_POINTERS *exinfo, MDRawAssertionInfo *assertion, bool succeeded);
int mydiv(int x, int y);


int _tmain(int argc, _TCHAR* argv[])
{
    google_breakpad::ExceptionHandler eh(L"./", NULL, callback, NULL, google_breakpad::ExceptionHandler::HANDLER_ALL);
    printf("9/3=%d\n", mydiv(9, 3));
    printf("9/0=%d\n", mydiv(9, 0));  //程序将在此崩溃
    printf("8/2=%d\n", mydiv(8, 2));
    system("pause");
    return 0;
}


bool callback(const wchar_t *dump_path, const wchar_t *id, void *context, EXCEPTION_POINTERS *exinfo, MDRawAssertionInfo *assertion, bool succeeded)
{
    if (succeeded)
    {
        printf("dump guid is %ws\n", id);
    }
    else
    {
        printf("dump failed\n");
    }
    system("pause");
    return succeeded;
}


int mydiv(int x, int y)
{
    int z;
    z = x / y;
    return z;
}
