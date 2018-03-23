#if !defined(__commtools_h__)
#define __commtools_h__

//#include <stdio.h>
//#include <stdlib.h>
#include <string.h>
#include "define.h"
////////////////////////////
//#ifdef _CONNECT_USE_MQ
//#include <cmqc.h>
//#include <cmqxc.h>
//typedef MQLONG  COMMLONG;
//typedef MQHCONN COMMHCONN;
//typedef MQHOBJ COMMHOBJ;
//#else
#include "kcxpapi.h"
typedef KCXP_LONG COMMLONG;
typedef KCXP_HCONN COMMHCONN;
typedef KCXP_HOBJ COMMHOBJ;
//#endif

#define _MSG_ID_LENGTH 24
///////////////////////////


class CCommTools
{
private:
  COMMLONG m_lLifeTime;
  COMMLONG m_lWaitTime;
  COMMLONG m_lOption;
  COMMLONG m_lTimeout;

  COMMHCONN m_hConn;
  COMMHOBJ m_hQObj;

  char m_szQMName[32 + 1];
  char m_szQName[32 + 1];
  char m_szAnsQueue[32 + 1];
  char m_szMsgID[_MSG_ID_LENGTH+1] ;
  char m_szCorID[_MSG_ID_LENGTH+1] ;

  long m_lOpenFlag;             // 0x01: 判断已打开连接是否和本次将连接的相同，若是则不重新打开连接
  char m_szErrorMsg[256 + 1];
  long m_lErrorCode;


public:
  CCommTools(void);
  ~CCommTools();

  long Open(const char * p_lpszQMName, const char * p_lpszQName = NULL,
    const char * p_lpszChannelName = NULL, const char * p_lpszConnectionName = NULL);
  long OpenQueueManager(const char * p_lpszQMName,
    const char * p_lpszChannelName = NULL, const char * p_lpszConnectionName = NULL);
  long OpenQueueManagerWithCheckShm(const char * p_lpszQMName,
    const char * p_lpszChannelName = NULL, const char * p_lpszConnectionName = NULL);
  long OpenQueueManagerx(const char *szQM, const char *szChannel = NULL, const char*szConnect = NULL) ;
  long OpenQueue(const char * p_lpszQName);
  long CloseQueue(void);
  long CloseQueueManager(void);
  long Close(void);

  int  SetLifeTime(int iSecond) ;
  int  GetLifeTime() ;
  int  SetWaitTime(int iMiliSecond) ;
  int  GetWaitTime() ;
  long GetMaxDepth() ;
  long GetCurrentDepth() ;
  int  SetDepth(long depth);	//该函数还有问题

  long GetLastError(char * p_lpszErrorMsg, long p_lBuffSize)
  {
    memset((void *)p_lpszErrorMsg, 0x00, sizeof(char) * p_lBuffSize);
    KBSS_STRNCPY(p_lpszErrorMsg, m_szErrorMsg, p_lBuffSize);
    return m_lErrorCode;
  }
public:
  long Send(const char * p_lpszMsg, long p_lMsgLen,
    const char * p_lpszCorrelID = NULL, const char * p_lpszGroupId = NULL, const char * p_lpszMsgID = NULL,
    short p_siPriority = 0);
  long Send(const char * p_lpszQName, const char * p_lpszMsg, long p_lMsgLen,
    const char * p_lpszCorrelID = NULL, const char * p_lpszMsgID = NULL,
    short p_siPriority = 0);

  long Recv(char * p_lpszMsgBuff, long p_lBuffSize, COMMLONG& p_reflMsgLen,
    const char * p_lpszCorrelID = NULL, const char * p_lpszMsgID = NULL,
    short p_siPriority = 0);
  long BrowseFirst(char * p_lpszMsgBuff, long p_lBuffSize, COMMLONG& p_reflMsgLen,
    const char * p_lpszCorrelID = NULL, const char * p_lpszMsgID = NULL,
    short p_siPriority = 0);
  long BrowseNext(char * p_lpszMsgBuff, long p_lBuffSize, COMMLONG & p_reflMsgLen,
    const char * p_lpszCorrelID = NULL, const char * p_lpszMsgID = NULL,
    short p_siPriority = 0);

private:
  long __Recv(char * p_lpszMsgBuff, long p_lBuffSize, COMMLONG& p_reflMsgLen,
    const char * p_lpszCorrelID, const char * p_lpszMsgID,
    short p_siPriority, COMMLONG p_lOption);


};

#endif  // __commtools_h__
