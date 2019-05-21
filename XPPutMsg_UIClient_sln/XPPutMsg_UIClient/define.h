#if !defined(__kbss_define_h__)
#define __kbss_define_h__

// _countof兼容语法 add by xuph 2012.1.9
#if defined(OS_IS_LINUX) || defined(OS_IS_AIX)
typedef char  _TCHAR;
typedef unsigned char  SQLCHAR;

#ifndef __int64
#define __int64 long long
#endif  // __int64

#define _countof(szSource) (((szSource) != NULL) ? (sizeof((szSource))) : (0))
#define _T(szSource) szSource
#define _TMULT_TO_WIDE(szSource) (szSource)
#define _TWIDE_TO_MULT(szSource) (szSource)
#define snwprintf snprintf
#define _snprintf snprintf
#define _sntprintf snprintf
  #define _vsntprintf vsnprintf
#define _tcslen strlen
#define _tcsncat strncat
#define _tcscmp strcmp
#define _tcsicmp stricmp
#define _tcsnicmp strnicmp
#define _tcsstr strstr
#endif  // defined(OS_IS_LINUX)

#if defined (UNICODE) || defined (_UNICODE)
#define _T_atoi              _wtoi
#define _TKBSS_STRNCPY       KBSS_WCSNCPY
#else
#define _T_atoi              atoi
#define _TKBSS_STRNCPY       KBSS_STRNCPY
#endif

//------------------------------------------------------------------------------
// 版本控制   --邓福军 2010-12-06
#define VER_BANK_SUB_ACCT_STD  0x00     //标准版
#define VER_BANK_SUB_ACCT_ABC  0x00     //开银行子账户使用ABC接口
#define VER_BANK_SUB_ACCT_GXYZ 0x01     //开银行子账户使用国信银证接口

#define VER_CUST_BANK_TRA_STD  0x00     //标准版, 银证转只在系统中保留信息，不和第三方对接
#define VER_CUST_BANK_TRA_GXYZ 0x01     //开通银证转账使用国信银行系统

#define VER_CHK_ID_CARD_SN_YES    0x01   //检查国内身份证号是否唯一, 检测, 国信版本
#define VER_CHK_ID_CARD_SN_NO     0x00   //检查国内身份证号是否唯一, 不检测, 越秀版本

#define VER_CUST_CODE_SKIP_4_YES  0x01   //生成客户号时跳过数字‘4’
#define VER_CUST_CODE_SKIP_4_NO   0x00   //生成客户号时不跳过数字‘4’

#define VER_SEND_ABC_BIG5_YES     0x01   //支持发送给ABC繁体
#define VER_SEND_ABC_BIG5_NO      0x00   //不支持发送给ABC繁体

//------------------------------------------------------------------------------
// 常量宏定义
//
#define RTMSG_OK                0
#define RTMSG_NO_DATA           100
#define RTMSG_KO                -1
#define RTMSG_ODBC_KO           -2
#define RTMSG_LBM_PAR_OUT_RANG  100

#define RTMSG_LEVEL_OK          '0'
#define RTMSG_LEVEL_BIZ         '1'
#define RTMSG_LEVEL_SYSTEM      '2'
#define RTMSG_LEVEL_DATABASE    '3'

#define VERSION_STAND

#if defined(DATABASE_IS_DB2) || defined(DATABASE_IS_MSSQL)
#define SQLCODE_OK            0
#define SQLCODE_NO_DATA       100
#endif  // defined(DATABASE_IS_DB2) || defined(DATABASE_IS_MSSQL)

#if defined(DATABASE_IS_ORACLE)
#define SQLCODE_OK            0
#define SQLCODE_NO_DATA       1403
#endif  // defined(DATABASE_IS_ORACLE)

#if defined(OS_IS_LINUX)
#ifndef LLONG_MAX
#define LLONG_MAX           9223372036854775807LL
#define LLONG_MIN           (-LLONG_MAX - 1LL)
#define ULLONG_MAX          18446744073709551615ULL
#endif  // LLONG_MAX
#endif  // defined(OS_IS_LINUX)

// 业务宏定义
#define USER_ROLE_CORPORATION   '0'
#define USER_ROLE_CUSTOMER      '1'
#define USER_ROLE_OPERATOR      '2'
#define USER_ROLE_BROKER        '3'
#define USER_ROLE_AGENT         '4'

//------------------------------------------------------------------------------
// 全局结构定义
struct ST_CALL_LBM_INFO
{
  char szLBM[32 + 1];                           // 业务模块，指的是：LBM编号
  char szFunction[32 + 1];                      // 功能编号，指的是：柜台功能编号、外围功能编号
  char szSession[128 + 1];                      // 会话凭证
  char szRuntime[32 + 1];                       // 调用时间，时间格式：'YYYY-MM-DD HH24:MI:SS.nnnnnn'

  __int64 llOpUserCode;                       // 操作用户代码
  char chOpUserRole;                            // 操作用户角色
  char chChannel;                               // 操作渠道
  char szSite[32 + 1];                          // 操作站点，格式：站点类型char(1):站点标识varchar(30)，如：0:020054554E01，1:192.168.10.55
  // 站点类型：'0': 网卡物理地址，'1': TCP/IP地址，'2': 电话号码，'3': 其它终端标识
};

//------------------------------------------------------------------------------
// 函数宏定义
//
#ifndef min
#define min(a, b)	(((a) < (b)) ? (a) : (b))
#endif  // min

#ifndef max
#define max(a, b)	(((a) > (b)) ? (a) : (b))
#endif  // max

#ifndef LONG2DATE
#define LONG2DATE(p_szDate, p_lLong) \
{\
  sprintf(p_szDate, "%04ld-%02ld-%02ld", long(p_lLong / 10000), long((p_lLong % 10000) / 100), long(p_lLong % 100));\
}
#endif  // LONG2DATE

#ifndef DATE2LONG
#define DATE2LONG(p_lLong, p_szDate) \
{\
  p_lLong = atol(p_szDate) * 10000 + atol(p_szDate + 5) * 100 \
  + (atol(p_szDate + strlen(p_szDate) - 2) <= 0 \
  ? atol(p_szDate + strlen(p_szDate) - 1) \
  : atol(p_szDate + strlen(p_szDate) - 2)); \
}
#endif  // DATE2LONG

#ifndef KBSS_STRNCPY
#define KBSS_STRNCPY(szDest, szSource, iCount) \
{\
  if ((szDest) != NULL && (szSource) != NULL) \
{ \
  memset((void *)(szDest), 0x00, (iCount)); \
  strncpy((szDest), (szSource), (iCount) - 1); \
} \
}
#endif  // KBSS_STRNCPY

#ifndef KBSS_WCSNCPY
#define KBSS_WCSNCPY(szDest, szSource, iCount) \
{\
  if ((szDest) != NULL && (szSource) != NULL) \
{ \
  memset((void *)(szDest), 0x00, (iCount * 2)); \
  wcsncpy((szDest), (szSource), (iCount) - 1); \
} \
}
#endif  // KBSS_WCSNCPY

#ifndef SEND_RTMSG
#define SEND_RTMSG(clLBMApi) \
{ \
  clLBMApi.BeginWrite(); \
  clLBMApi.RsCreate("ERRMSG", 4, "MSG_LEVEL, MSG_CODE, MSG_TEXT, DEBUG_MSG"); \
  clLBMApi.RsAddRow(); \
  clLBMApi.RsSetCol(1, g_clRuntimeMsg.GetMsgLevel()); \
  clLBMApi.RsSetCol(2, g_clRuntimeMsg.GetMsgCode()); \
  clLBMApi.RsSetCol(3, g_clRuntimeMsg.GetMsgText()); \
  clLBMApi.RsSetCol(4, g_clRuntimeMsg.GetDebugMsg()); \
  clLBMApi.RsSaveRow(); \
}
#endif  // SEND_RTMSG

#ifndef SEND_RTMSG_WW
#define SEND_RTMSG_WW(clLBMApi) \
{ \
  clLBMApi.BeginWrite(); \
  clLBMApi.RsCreate("ERRMSG", 7, "LEVEL, CODE, MSG, MSG_LEVEL, MSG_CODE, MSG_TEXT, DEBUG_MSG"); \
  clLBMApi.RsAddRow(); \
  clLBMApi.RsSetCol(1, g_clRuntimeMsg.GetMsgLevel()); \
  clLBMApi.RsSetCol(2, g_clRuntimeMsg.GetMsgCode()); \
  clLBMApi.RsSetCol(3, g_clRuntimeMsg.GetMsgText()); \
  clLBMApi.RsSetCol(4, g_clRuntimeMsg.GetMsgLevel()); \
  clLBMApi.RsSetCol(5, g_clRuntimeMsg.GetMsgCode()); \
  clLBMApi.RsSetCol(6, g_clRuntimeMsg.GetMsgText()); \
  clLBMApi.RsSetCol(7, g_clRuntimeMsg.GetDebugMsg()); \
  clLBMApi.RsSaveRow(); \
}
#endif  // SEND_RTMSG_WW

#ifndef SEND_RTMSG_STG
#define SEND_RTMSG_STG(clLBMApi) \
{ \
  clLBMApi.BeginWrite(); \
  clLbmApi.RsCreate("MESSAGE", 3, "LEVEL,CODE,MSG"); \
  clLBMApi.RsAddRow(); \
  clLBMApi.RsSetCol(1, g_clRuntimeMsg.GetMsgLevel()); \
  clLBMApi.RsSetCol(2, g_clRuntimeMsg.GetMsgCode()); \
  clLBMApi.RsSetCol(3, g_clRuntimeMsg.GetMsgText()); \
  clLBMApi.RsSaveRow(); \
}
#endif  // SEND_RTMSG_STG

#ifndef CURSOR_IS_OPENED
#define CURSOR_IS_OPENED(p_uCMLstxxx)         ((m_uCursorMask & (p_uCMLstxxx)) == (p_uCMLstxxx) ? true : false)
#endif  // CURSOR_IS_OPENED

#ifndef CURSOR_OPENED
#define CURSOR_OPENED(p_uCMLstxxx)            (m_uCursorMask |= (p_uCMLstxxx))
#endif  // CURSOR_OPENED

#ifndef CURSOR_CLOSED
#define CURSOR_CLOSED(p_uCMLstxxx)            (m_uCursorMask &= (~(p_uCMLstxxx)))
#endif  // CURSOR_CLOSED

//------------------------------------------------------------------------------
// 系统函数转化
#if defined(OS_IS_WINDOWS)
#define snprintf              _snprintf
#define snwprintf             _snwprintf
#define atoll                 _atoi64
#define llabs                 _abs64
#define memicmp               _memicmp
#define stricmp               _stricmp
#define strnicmp              _strnicmp
#endif  // defined(OS_IS_WINDOWS)


#if defined(OS_IS_WINDOWS)
#define KBSS_DLL_EXPORT       __declspec(dllexport)
#define KBSS_DLL_IMPORT       __declspec(dllimport)

#if defined(KBSS_DLL_CLASS)
#define KBSS_DLL_CLASS_DECLARE              __declspec(dllexport)
#else
#define KBSS_DLL_CLASS_DECLARE              __declspec(dllimport)
#endif  // defined(KBSS_DLL_CLASS)

#else
#define KBSS_DLL_EXPORT
#define KBSS_DLL_IMPORT
#define KBSS_DLL_CLASS_DECLARE
#endif  // defined(OS_IS_WINDOWS)

#endif  // __kbss_define_h__
