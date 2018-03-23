// yu_e_bao_sum.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"
#include "client/windows/handler/exception_handler.h"

#define BASE_MONEY      800000.00
#define MAX_YEARS_NUM   30
#define AVERAGE_RATE    4.0849/100
#define YUE_GONG        4127.49219


bool callback(const wchar_t *dump_path, const wchar_t *id, void *context, EXCEPTION_POINTERS *exinfo, MDRawAssertionInfo *assertion, bool succeeded);

int _tmain(int argc, _TCHAR* argv[])
{
    google_breakpad::ExceptionHandler eh(L"./", NULL, callback, NULL, google_breakpad::ExceptionHandler::HANDLER_ALL);

    double dYueGong = YUE_GONG;
    double dBaseMoney = BASE_MONEY;
    int iCunFangDays = 365 * MAX_YEARS_NUM;
    double dRate = AVERAGE_RATE;
    double dCurMoneySum = dBaseMoney;
    for (int i = 0 ; i < iCunFangDays ; i++)
    {
        dCurMoneySum = (dCurMoneySum) * (dRate/365) + dCurMoneySum;
    }
    double dCurLiXiSum = 0;
    for (int i = 0 ; i < iCunFangDays/30 ; i++)
    {
       dCurLiXiSum += dYueGong;
       for (int j = 0 ;j<30;j++)
       {
           dCurLiXiSum = (dCurLiXiSum)*(dRate/365) + dCurLiXiSum;
       }       
    }
    printf("[%lf]\n[%d]\n[%lf]\n[%lf]\n",dBaseMoney,iCunFangDays,dCurMoneySum,dCurLiXiSum);
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