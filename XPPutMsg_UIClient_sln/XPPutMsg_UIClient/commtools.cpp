/*******************************************************************************
版权所有：深圳市金证科技股份有限公司 2003
　　　　　中信证券股份有限公司

功    能：封装KCXP的调用，提高易用性及移植性。
说    明：
作    者：钟桂全
创建日期：2004.03.03
版    本：1.0.0.0

修改日志：
    ----------------------------------------------------------------------------
    作者：Mr. Yuwei Du
    日期：2004.09.11
    内容：为提高系统性能，增加连接保存和恢复功能
    特征：
    ----------------------------------------------------------------------------
    ----------------------------------------------------------------------------
    作者：杨利强
    日期：2006.06.27
    内容：兼容IBM MQSeries(通过宏_CONNECT_USE_MQ区分)，
          兼容64位系统(通过OBJECT_MODE环境变量区分32或64, IBM MQSeries因为库的原因暂不支持64位)，
          兼容IBM MQSeries的客户端和服务端连接方式(通过编译时连接libmqm.a和libmqic来区分),
          兼容三方业务程序里的函数(类名不同)
    特征：1、使用长连接的连接方式：
          Open(),OpenQueueManagerWithCheckShm(),OpenQueueManagerx()
          2、程序不需要attach和detach共享内存，需要更改以前shm_templ.h里不要有attach和detach，
          以确保共用一个libsharedmem.so
    ----------------------------------------------------------------------------

*******************************************************************************/
#include "stdafx.h"
#include "commtools.h"

//#include <signal.h>
#if !defined(OS_IS_WINDOWS)
  #include <errno.h>
#endif  // !defined(OS_IS_WINDOWS)

#define SHM_KEY_LONG_CONNECT    666
#define MAX_Q_INFO              2000

////////////////////////////////////////////////////////////////////////////////////////////////////////
char * GetBuff(char * p_lpszDestBuff, const long p_lDestBuffSize,
                   const char * p_lpszString, const int p_iN, const char p_chSeparator /*= ','*/)
{
  int iN = p_iN, iIndex, nDestBuffIndex;
  int iStrLen = strlen(p_lpszString);

  memset((void *)p_lpszDestBuff, 0x00, sizeof(char) * p_lDestBuffSize);

  for (iIndex = 0; iIndex < iStrLen && iN > 0; iIndex++)
  {
    if (p_lpszString[iIndex] == p_chSeparator)
    {
      iN--;
    }
  }
  for (nDestBuffIndex = 0;
       iN == 0 && iIndex < iStrLen && p_lpszString[iIndex] != p_chSeparator && nDestBuffIndex < (p_lDestBuffSize - 1);
       iIndex++, nDestBuffIndex++)
  {
    p_lpszDestBuff[nDestBuffIndex] = p_lpszString[iIndex];
  }

  return p_lpszDestBuff;
}
///////////////////////////////////////////////////////////


CCommTools::CCommTools(void)
{
  m_lErrorCode = 0L;

  m_hConn = -1;
  m_hQObj = -1;
  m_lLifeTime = -1; //无限超时,单位KCXP为毫秒，MQ为10秒
  m_lWaitTime = -1;    //无限等待
  m_lTimeout = 20L ;   //发送超时，只在XP中用

  #ifndef _CONNECT_USE_MQ
  m_lOption = KCXP_OO_AS_Q_DEF | KCXP_OO_INPUT | KCXP_OO_OUTPUT | KCXP_OO_INQUIRE;
  #else
  m_lOption = MQOO_FAIL_IF_QUIESCING | MQOO_INQUIRE | MQOO_INPUT_AS_Q_DEF | MQOO_OUTPUT | MQOO_BROWSE;
  #endif

  m_lOpenFlag = 0x00;

  memset((void *)m_szQMName, 0x00, sizeof(m_szQMName));
  memset((void *)m_szQName, 0x00, sizeof(m_szQName));
  memset((void *)m_szAnsQueue, 0x00, sizeof(m_szAnsQueue));
  memset((void *)m_szErrorMsg, 0x00, sizeof(m_szErrorMsg));
}


CCommTools::~CCommTools()
{
//  Close();
}


long CCommTools::Open(const char * p_lpszQMName, const char * p_lpszQName /*= NULL*/,
                      const char * p_lpszChannelName /*= NULL*/, const char * p_lpszConnectionName /*= NULL*/)
{
  long lRetCode = -1L;

  if (lRetCode != 0)
  {
    if ((lRetCode = OpenQueueManager(p_lpszQMName, p_lpszChannelName, p_lpszConnectionName)) != 0L
      || (p_lpszQName != NULL && (lRetCode = OpenQueue(p_lpszQName)) != 0L))
    {
      goto __end;
    }
  }
  else
  {
    if (p_lpszQName != NULL && (lRetCode = OpenQueue(p_lpszQName)) != 0L)
    {
#ifndef _CONNECT_USE_MQ
      if ((lRetCode == KCXP_RC_HCONN_ERROR) || (lRetCode == KCXP_RC_CONNECTION_BROKEN) || (lRetCode == KCXP_RC_CONNECTION_STOPPING))
#else
      if (lRetCode == MQRC_HCONN_ERROR)
#endif
      {
        m_hConn = -1;
        if ((lRetCode = OpenQueueManager(p_lpszQMName, p_lpszChannelName, p_lpszConnectionName)) != 0L)
        {
          goto __end;
        }

        if (p_lpszQName != NULL && (lRetCode = OpenQueue(p_lpszQName)) != 0L)
        {
          goto __end;
        }
      }
      else
      {
        goto __end;
      }
    }
    m_lOpenFlag |= 0x10;
  }

__end:
  m_lErrorCode = lRetCode;

  if(lRetCode)
  {
    FILE *fp = NULL;
    fp = fopen("/tmp/commtools.txt", "a+");
    if (fp)
    {
#if defined(OS_IS_WINDOWS)
        fprintf(fp, "lRetCode[%d]errno[%d]\n", lRetCode, errno);
#else
        fprintf(fp, "lRetCode[%d]errno[%m]\n", lRetCode, errno);
#endif  // defined(OS_IS_WINDOWS)

        fclose(fp);
    }
  }

  return lRetCode;
}

long CCommTools::OpenQueueManagerWithCheckShm(const char * p_lpszQMName,
                                  const char * p_lpszChannelName /*= NULL*/,
                                  const char * p_lpszConnectionName /*= NULL*/)
{
  long lRetCode = 0L;
  FILE *fp = stdout;

  m_hConn = -1;
  if(lRetCode != 0)
  {
     lRetCode = OpenQueueManager(p_lpszQMName, p_lpszChannelName, p_lpszConnectionName);
  }
  else
  {
    //printf("..load sucess...conn[%d]..ppid[%d]...pid[%d]..tid[%d]\n", m_hConn, getppid(),getpid(),pthread_self());
  }

  if(lRetCode)
  {
    FILE *fp = NULL;
    fp = fopen("/tmp/commtools.txt", "a+");
    if (fp)
    {
        fprintf(fp, "lRetCode[%d]errno[%d]\n", lRetCode, errno);
        fclose(fp);
    }
  }

  m_lErrorCode = lRetCode;
  return lRetCode;
}

long CCommTools::OpenQueueManager(const char * p_lpszQMName,
                                  const char * p_lpszChannelName /*= NULL*/,
                                  const char * p_lpszConnectionName /*= NULL*/)
{
  long lRetCode = 0L;
  COMMLONG lCompCode=0, lReason=0;
  FILE *fp = NULL;

  #ifdef _CONNECT_USE_MQ
  MQCNO stConnectOpts = {MQCNO_DEFAULT};
  MQCD stClientConn = {MQCD_CLIENT_CONN_DEFAULT};

  stConnectOpts.Version = MQCNO_VERSION_2;
  stConnectOpts.Options = MQCNO_STANDARD_BINDING;

  stConnectOpts.ClientConnPtr = &stClientConn;
  if (p_lpszChannelName != NULL && p_lpszChannelName[0] != 0x00)
  {
    KBSS_STRNCPY(stClientConn.ChannelName, p_lpszChannelName, MQ_CHANNEL_NAME_LENGTH);
  }
  else
  {
    KBSS_STRNCPY(stClientConn.ChannelName, "SYSTEM.DEF.SVRCONN", MQ_CHANNEL_NAME_LENGTH);
  }
  if (p_lpszConnectionName != NULL && p_lpszConnectionName[0] != 0x00)
  {
    KBSS_STRNCPY(stClientConn.ConnectionName, p_lpszConnectionName, MQ_CONN_NAME_LENGTH);
  }
  #endif


  // 先关闭队列和队列管理器
//  CloseQueue();
//  CloseQueueManager();

  //连接队列管理器
  #ifndef _CONNECT_USE_MQ
  if(p_lpszChannelName!=NULL && p_lpszChannelName[0]!=0x00 && p_lpszConnectionName!=NULL && p_lpszConnectionName[0]!=0x00)
  {
      KCXP_Connx((char*)p_lpszConnectionName, atoi(p_lpszChannelName), "KCXP00", "888888", &m_hConn, &lCompCode, &lReason );
      if ( lCompCode != KCXP_CC_OK)
      {
        lRetCode = lReason;
        sprintf(m_szErrorMsg, "Open queue manager %s[%s] error.(reason=%ld)", p_lpszConnectionName, p_lpszChannelName, lReason);

        fp = fopen("/tmp/commtools.txt", "a+");
        if (fp)
        {
            fprintf(fp, "%s\n", m_szErrorMsg);
            fclose(fp);
        }

        goto __end;
      }
  }
  else
  {
	  KCXP_Connx(
		  (char *)p_lpszQMName,
		  21000,
		  (KCXP_CHAR*)"KCXP00",
		  (KCXP_CHAR*)"888888",
		  &m_hConn,&lCompCode,&lReason);

      //KCXP_Conn((char *)p_lpszQMName, &m_hConn, &m_hConn, &lReason );
      if ( lCompCode != KCXP_CC_OK)
      {
        lRetCode = lReason;
        sprintf(m_szErrorMsg, "Open queue manager %s error.(reason=%ld)", p_lpszQMName, lReason);

        fp = fopen("/tmp/commtools.txt", "a+");
        if (fp)
        {
            fprintf(fp, "%s\n", m_szErrorMsg);
            fclose(fp);
        }

        goto __end;
      }
  }
  #else
  if (p_lpszChannelName != NULL && p_lpszChannelName[0] != 0x00
    && p_lpszConnectionName != NULL && p_lpszConnectionName[0] != 0x00)
  {
      MQCONNX((MQCHAR *)p_lpszQMName, &stConnectOpts, &m_hConn, &lCompCode, &lReason);
  }
  else
  {
      MQCONN((MQCHAR *)p_lpszQMName, &m_hConn, &lCompCode, &lReason);
  }
  if (lCompCode == MQCC_FAILED)
  {
    lRetCode = lReason;
    sprintf(m_szErrorMsg, "Open queue manager %s error.(reason=%ld)", p_lpszQMName, lReason);
    fp = fopen("/tmp/commtools.txt", "a+");
    if (fp)
    {
       fprintf(fp, "%s\n", m_szErrorMsg);
       fclose(fp);
    }

    goto __end;
  }
  #endif

  if (strlen(p_lpszQMName) < sizeof(m_szQMName))
      strcpy(m_szQMName, p_lpszQMName) ;
  else
      m_szQMName[0] = 0 ;

__end:
  m_lErrorCode = lRetCode;
  return lRetCode;
}


long CCommTools::OpenQueue(const char * p_lpszQName)
{
  long lRetCode = 0L;

  COMMLONG lCompCode, lReason=0;
  #ifndef _CONNECT_USE_MQ
  KCXP_OD stObjDesc = {KCXP_DEFAULT_OD};
  #else
  MQOD stObjDesc = {MQOD_DEFAULT};
  #endif

  if (m_hConn == -1)
  {
    lRetCode = 101L;
    sprintf(m_szErrorMsg, "Didn't open queue manager.");
    goto __end;
  }
  // 先关闭队列和队列管理器
  CloseQueue();

  #ifndef _CONNECT_USE_MQ
  stObjDesc.iObjectType = KCXP_OT_Q ;
  KBSS_STRNCPY(stObjDesc.strObjectName, p_lpszQName, sizeof(stObjDesc.strObjectName) ) ;
  KCXP_Open(m_hConn, &stObjDesc, m_lOption, &m_hQObj, &lCompCode, &lReason);
  if (lCompCode != KCXP_CC_OK)
  {
    lRetCode = lReason;
    sprintf(m_szErrorMsg, "Open queue %s error.(reason=%ld)", p_lpszQName, lReason) ;

    FILE *fp = NULL;
    fp = fopen("/tmp/commtools.txt", "a+");
    if (fp)
    {
    	fprintf(fp, "%s\n", m_szErrorMsg);
    	fclose(fp);
    }

    goto __end;
  }
  #else
  stObjDesc.Version = MQOD_VERSION_2;
  KBSS_STRNCPY(stObjDesc.ObjectName, p_lpszQName, sizeof(stObjDesc.ObjectName) ) ;
  MQOPEN(m_hConn, &stObjDesc, m_lOption, &m_hQObj, &lCompCode, &lReason);
  if (lCompCode != MQCC_OK)
  {
    lRetCode = lReason;
    sprintf(m_szErrorMsg, "Open queue %s error.(reason=%ld)", p_lpszQName, lReason) ;

    FILE *fp = NULL;
    fp = fopen("/tmp/commtools.txt", "a+");
    if (fp)
    {
    	fprintf(fp, "%s\n", m_szErrorMsg);
    	fclose(fp);
    }
    goto __end;
  }
  #endif
  KBSS_STRNCPY(m_szQName, p_lpszQName, sizeof(m_szQName));

__end:
  m_lErrorCode = lRetCode;
  return lRetCode;
}


long CCommTools::CloseQueue(void)
{
  long lRetCode = 0L;

  COMMLONG lCompCode, lReason;

  if (m_hConn != -1 && m_hQObj != -1)
  {
    #ifndef _CONNECT_USE_MQ
    KCXP_Close(m_hConn, &m_hQObj, 0, &lCompCode, &lReason);
    #else
    MQCLOSE(m_hConn, &m_hQObj, MQCO_NONE, &lCompCode, &lReason);
    #endif
    m_hQObj = -1;
    memset((void *)m_szQName, 0x00, sizeof(m_szQName));
    if (lCompCode != 0)
    {
      lRetCode = lReason;
      sprintf(m_szErrorMsg, "Close queue error.(reason=%ld)", lReason);

      FILE *fp = NULL;
      fp = fopen("/tmp/commtools.txt", "a+");
      if (fp)
      {
    	fprintf(fp, "%s\n", m_szErrorMsg);
    	fclose(fp);
      }

      goto __end;
    }
  }

__end:
  m_lErrorCode = lRetCode;
  return lRetCode;
}


long CCommTools::CloseQueueManager(void)
{
  long lRetCode = 0L;

  COMMLONG lCompCode, lReason;

  if (m_lOpenFlag & 0x10)
  {
    // has been saved into shared memory
    goto __end;
  }

  if (m_hConn != -1)
  {
    #ifndef _CONNECT_USE_MQ
    KCXP_Disconn(&m_hConn, 0, &lCompCode, &lReason );
    #else
    MQDISC(&m_hConn, &lCompCode, &lReason);
    #endif
    m_hConn = -1;
    memset((void *)m_szQMName, 0x00, sizeof(m_szQMName));
    if (lCompCode != 0)
    {
      lRetCode = lReason;
      sprintf(m_szErrorMsg, "Disconnect queue manager error.(reason=%ld)", lReason) ;

      FILE *fp = NULL;
      fp = fopen("/tmp/commtools.txt", "a+");
      if (fp)
      {
    	fprintf(fp, "%s\n", m_szErrorMsg);
    	fclose(fp);
      }

      goto __end;
    }

  }

__end:
  m_lErrorCode = lRetCode;
  return lRetCode;
}


long CCommTools::Close(void)
{
  long lRetCode = 0L;

  if ((lRetCode = CloseQueue()) != 0L
    || (lRetCode = CloseQueueManager()) != 0L)
  {
    goto __end;
  }

__end:
  m_lErrorCode = lRetCode;
  return lRetCode;
}


long CCommTools::Send(const char * p_lpszMsg, long p_lMsgLen,
                      const char * p_lpszCorrelID /*= NULL*/, 
											const char * p_lpszGroupId, 
											const char * p_lpszMsgID /*= NULL*/,
                      short p_siPriority /*= 0*/)
{
  long lRetCode = 0L;

  COMMLONG lCompCode, lReason;
  #ifndef _CONNECT_USE_MQ
  KCXP_PMO stPutMsgOpts = {KCXP_DEFAULT_PMO};
  KCXP_MD stMsgDesc = {KCXP_DEFAULT_MD};
  #else
  MQPMO stPutMsgOpts = {MQPMO_DEFAULT};
  MQMD stMsgDesc = {MQMD_DEFAULT};
  #endif

  if (m_hConn == -1 || m_hQObj == -1)
  {
    lRetCode = 108L;
    sprintf(m_szErrorMsg, "Didn't open queue manager or queue");
    goto __end;
  }

  #ifndef _CONNECT_USE_MQ
  stMsgDesc.iPriority = (p_siPriority > 0 ? p_siPriority : KCXP_PRIORITY_0);
  stPutMsgOpts.Timeout = m_lTimeout;
  stMsgDesc.iLifeTime = m_lLifeTime;
  stMsgDesc.cbMsgType = KCXP_MT_DATAGRAM ;
  stMsgDesc.iDataLength = p_lMsgLen ;
  #else
  stMsgDesc.Priority = (p_siPriority > 0 ? p_siPriority : MQPRI_PRIORITY_AS_Q_DEF);
  stMsgDesc.Expiry = m_lLifeTime;
  #endif

  if (p_lpszCorrelID != NULL && p_lpszCorrelID[0] != 0x00)
  {
    #ifndef _CONNECT_USE_MQ
    KBSS_STRNCPY((char *)(stMsgDesc.strCorrelId), p_lpszCorrelID, sizeof(stMsgDesc.strCorrelId));
    #else
    memcpy((void *)(stMsgDesc.CorrelId), (void *)p_lpszCorrelID, sizeof(stMsgDesc.CorrelId));
    #endif
  }
  if (p_lpszGroupId != NULL && p_lpszGroupId[0] != 0x00)
  {
#ifndef _CONNECT_USE_MQ
    KBSS_STRNCPY((char *)(stMsgDesc.strGroupId), p_lpszGroupId, sizeof(stMsgDesc.strGroupId));
#else
    memcpy((void *)(stMsgDesc.GroupId), (void *)p_lpszGroupId, sizeof(stMsgDesc.GroupId));
#endif
  }

  if (p_lpszMsgID != NULL && p_lpszMsgID[0] != 0x00)
  {
    #ifndef _CONNECT_USE_MQ
    KBSS_STRNCPY((char *)(stMsgDesc.strMsgId), p_lpszMsgID, sizeof(stMsgDesc.strMsgId));
    #else
    memcpy((void *)(stMsgDesc.MsgId), (void *)p_lpszMsgID, sizeof(stMsgDesc.MsgId));
    #endif
  }
  else
  {
    #ifndef _CONNECT_USE_MQ
    stPutMsgOpts.Options = KCXP_PMO_NEW_MSG_ID;
    #else
    stPutMsgOpts.Options = MQPMO_NEW_MSG_ID;
    #endif
  }

  #ifndef _CONNECT_USE_MQ
  KCXP_Put(m_hConn, m_hQObj, &stMsgDesc, &stPutMsgOpts, p_lMsgLen, (void *)p_lpszMsg, &lCompCode, &lReason) ;
  #else
  MQPUT(m_hConn, m_hQObj, &stMsgDesc, &stPutMsgOpts, p_lMsgLen, (void *)p_lpszMsg, &lCompCode, &lReason);
  #endif
  if (lCompCode != 0)
  {
    lRetCode = lReason;
    sprintf(m_szErrorMsg, "Send message error.(reason=%ld)", lReason) ;

      FILE *fp = NULL;
      fp = fopen("/tmp/commtools.txt", "a+");
      if (fp)
      {
    	fprintf(fp, "%s\n", m_szErrorMsg);
    	fclose(fp);
      }

    goto __end;
  }

__end:
  m_lErrorCode = lRetCode;
  return lRetCode;
}
//采用打开队列方式的一步到位的PUT1，不需要执行Close函数
long CCommTools::Send(const char * p_lpszQName, const char * p_lpszMsg, long p_lMsgLen,
                      const char * p_lpszCorrelID /*= NULL*/, const char * p_lpszMsgID /*= NULL*/,
                      short p_siPriority /*= 0*/)
{
  long lRetCode = 0L;

  COMMLONG lCompCode, lReason;
  #ifndef _CONNECT_USE_MQ
  KCXP_PMO stPutMsgOpts = {KCXP_DEFAULT_PMO};
  KCXP_MD stMsgDesc = {KCXP_DEFAULT_MD};
  KCXP_OD stObjDesc = {KCXP_DEFAULT_OD};
  #else
  MQPMO stPutMsgOpts = {MQPMO_DEFAULT};
  MQMD stMsgDesc = {MQMD_DEFAULT};
  MQOD stObjDesc = {MQOD_DEFAULT};
  #endif

  if (m_hConn == -1)
  {
    lRetCode = 108L;
    sprintf(m_szErrorMsg, "Didn't open queue manager or queue");
    goto __end;
  }
  #ifndef _CONNECT_USE_MQ
  stObjDesc.iObjectType = KCXP_OT_Q ;
  KBSS_STRNCPY(stObjDesc.strObjectName, p_lpszQName, sizeof(stObjDesc.strObjectName) ) ;
  KBSS_STRNCPY(stObjDesc.strQmgrName, m_szQMName, sizeof(stObjDesc.strQmgrName)) ;
  stPutMsgOpts.Timeout = m_lTimeout;
  stMsgDesc.iPriority = (p_siPriority > 0 ? p_siPriority : KCXP_PRIORITY_0);
  stMsgDesc.cbMsgType = KCXP_MT_REQUEST;
  stMsgDesc.iLifeTime = m_lLifeTime;
  stMsgDesc.iDataLength = p_lMsgLen ;
  #else
  KBSS_STRNCPY(stObjDesc.ObjectName, p_lpszQName, sizeof(stObjDesc.ObjectName) ) ;
  KBSS_STRNCPY(stObjDesc.ObjectQMgrName, m_szQMName, sizeof(stObjDesc.ObjectQMgrName)) ;
  stMsgDesc.Priority = (p_siPriority > 0 ? p_siPriority : MQPRI_PRIORITY_AS_Q_DEF);
  stMsgDesc.Expiry = m_lLifeTime;
  #endif

  if (p_lpszCorrelID != NULL && p_lpszCorrelID[0] != 0x00)
  {
    #ifndef _CONNECT_USE_MQ
    KBSS_STRNCPY((char *)(stMsgDesc.strCorrelId), p_lpszCorrelID, sizeof(stMsgDesc.strCorrelId));
    #else
    memcpy((void *)(stMsgDesc.CorrelId), (void *)p_lpszCorrelID, sizeof(stMsgDesc.CorrelId));
    #endif
  }

  if (p_lpszMsgID != NULL && p_lpszMsgID[0] != 0x00)
  {
    #ifndef _CONNECT_USE_MQ
    KBSS_STRNCPY((char *)(stMsgDesc.strMsgId), p_lpszMsgID, sizeof(stMsgDesc.strMsgId));
    #else
    memcpy((void *)(stMsgDesc.MsgId), (void *)p_lpszMsgID, sizeof(stMsgDesc.MsgId));
    #endif
  }
  else
  {
    #ifndef _CONNECT_USE_MQ
    stPutMsgOpts.Options = KCXP_PMO_NEW_MSG_ID;
    #else
    stPutMsgOpts.Options = MQPMO_NEW_MSG_ID;
    #endif
  }

  #ifndef _CONNECT_USE_MQ
  KCXP_Put1(m_hConn, &stObjDesc, &stMsgDesc, &stPutMsgOpts, p_lMsgLen, (void *)p_lpszMsg, &lCompCode, &lReason) ;
  #else
  MQPUT1(m_hConn, &stObjDesc, &stMsgDesc, &stPutMsgOpts, p_lMsgLen, (void *)p_lpszMsg, &lCompCode, &lReason);
  #endif
  if (lCompCode != 0L)
  {
    lRetCode = lReason;
    //printf("..load sucess...conn[%d].....lReason[%d]..ppid[%d]...pid[%d]..tid[%d]\n", m_hConn, lReason, getppid(),getpid(),pthread_self());
    sprintf(m_szErrorMsg, "Send message error.(reason=%ld)", lReason) ;

    FILE *fp = NULL;
    fp = fopen("/tmp/commtools.txt", "a+");
    if (fp)
    {
        fprintf(fp, "%s\n", m_szErrorMsg);
        fclose(fp);
    }

    goto __end;
  }

__end:
  m_lErrorCode = lRetCode;
  return lRetCode;
}


long CCommTools::__Recv(char * p_lpszMsgBuff, long p_lBuffSize, COMMLONG & p_reflMsgLen,
                        const char * p_lpszCorrelID, const char * p_lpszMsgID,
                        short p_siPriority, COMMLONG p_lOption)
{
  long lRetCode = 0L;

  COMMLONG lCompCode, lReason;
  #ifndef _CONNECT_USE_MQ
  KCXP_GMO stGetMsgOpts = {KCXP_DEFAULT_GMO};
  KCXP_MD stMsgDesc = {KCXP_DEFAULT_MD};
  #else
  MQGMO stGetMsgOpts = {MQGMO_DEFAULT};
  MQMD stMsgDesc = {MQMD_DEFAULT};
  #endif

  if (m_hConn == -1 || m_hQObj == -1)
  {
    lRetCode = 108L;
    sprintf(m_szErrorMsg, "Didn't open queue manager or queue");
    goto __end;
  }

  stGetMsgOpts.Options = p_lOption;
  stGetMsgOpts.WaitInterval = (m_lWaitTime > 0L ? m_lWaitTime : 0L);
  #ifndef _CONNECT_USE_MQ
  stGetMsgOpts.Options |= (m_lWaitTime > 0L ? KCXP_GMO_WAIT : KCXP_GMO_NO_WAIT);
  #else
  stGetMsgOpts.Options |= (m_lWaitTime > 0L ? MQGMO_WAIT : MQGMO_NO_WAIT);
  #endif


  if (p_lpszCorrelID != NULL && p_lpszCorrelID[0] != 0x00)
  {
    #ifndef _CONNECT_USE_MQ
    stGetMsgOpts.MatchOptions |= KCXP_MO_MATCH_CORREL_ID;
    KBSS_STRNCPY((char *)(stMsgDesc.strCorrelId), p_lpszCorrelID, sizeof(stMsgDesc.strCorrelId));
    #else
    stGetMsgOpts.MatchOptions |= MQMO_MATCH_CORREL_ID;
    memcpy((void *)(stMsgDesc.CorrelId), (void *)p_lpszCorrelID, sizeof(stMsgDesc.CorrelId));
    #endif
  }

  if (p_lpszMsgID != NULL && p_lpszMsgID[0] != 0x00)
  {
    #ifndef _CONNECT_USE_MQ
    stGetMsgOpts.MatchOptions |= KCXP_MO_MATCH_MSG_ID;
    KBSS_STRNCPY((char *)(stMsgDesc.strMsgId), p_lpszMsgID, sizeof(stMsgDesc.strMsgId));
    #else
    stGetMsgOpts.MatchOptions |= MQMO_MATCH_MSG_ID;
    memcpy((void *)(stMsgDesc.MsgId), (void *)p_lpszMsgID, sizeof(stMsgDesc.MsgId));
    #endif
  }

  #ifndef _CONNECT_USE_MQ
  KCXP_Get(m_hConn, m_hQObj, &stMsgDesc, &stGetMsgOpts, p_lBuffSize, p_lpszMsgBuff, &p_reflMsgLen, &lCompCode, &lReason);
  #else
  MQGET(m_hConn, m_hQObj, &stMsgDesc, &stGetMsgOpts, p_lBuffSize, p_lpszMsgBuff, &p_reflMsgLen, &lCompCode, &lReason);
  #endif
  if (lCompCode != 0L)
  {
    #ifndef _CONNECT_USE_MQ
    if (lReason == KCXP_RC_NO_MSG_AVAILABLE)
    #else
    if (lReason == MQRC_NO_MSG_AVAILABLE)
    #endif
    {
      p_reflMsgLen = 0L;
      memset((void *)p_lpszMsgBuff, 0x00, sizeof(char) * p_lBuffSize);
      lRetCode = 106L;
      goto __end;
    }
    else
    {
      lRetCode = lReason;
      sprintf(m_szErrorMsg, "Get message error.(reason=%ld)", lReason) ;

    FILE *fp = NULL;
    fp = fopen("/tmp/commtools.txt", "a+");
    if (fp)
    {
        fprintf(fp, "%s\n", m_szErrorMsg);
        fclose(fp);
    }

      goto __end;
    }
  }


__end:
  m_lErrorCode = lRetCode;
  return lRetCode;
}


long CCommTools::Recv(char * p_lpszMsgBuff, long p_lBuffSize, COMMLONG& p_reflMsgLen,
                      const char * p_lpszCorrelID /*= NULL*/, const char * p_lpszMsgID /*= NULL*/,
                      short p_siPriority /*= 0*/)
{
  #ifndef _CONNECT_USE_MQ
  return __Recv(p_lpszMsgBuff, p_lBuffSize, p_reflMsgLen, p_lpszCorrelID, p_lpszMsgID, p_siPriority,
    KCXP_GMO_GET );
  #else
  return __Recv(p_lpszMsgBuff, p_lBuffSize, p_reflMsgLen, p_lpszCorrelID, p_lpszMsgID, p_siPriority,
    MQGMO_FAIL_IF_QUIESCING);
  #endif
}


long CCommTools::BrowseFirst(char * p_lpszMsgBuff, long p_lBuffSize, COMMLONG& p_reflMsgLen,
                             const char * p_lpszCorrelID /*= NULL*/, const char * p_lpszMsgID /*= NULL*/,
                             short p_siPriority /*= 0*/)
{
  #ifndef _CONNECT_USE_MQ
  return __Recv(p_lpszMsgBuff, p_lBuffSize, p_reflMsgLen, p_lpszCorrelID, p_lpszMsgID, p_siPriority,
    KCXP_GMO_BROWSE_FIRST);
  #else
  return __Recv(p_lpszMsgBuff, p_lBuffSize, p_reflMsgLen, p_lpszCorrelID, p_lpszMsgID, p_siPriority,
    MQGMO_FAIL_IF_QUIESCING | MQGMO_NO_SYNCPOINT | MQGMO_BROWSE_FIRST);
  #endif
}


long CCommTools::BrowseNext(char * p_lpszMsgBuff, long p_lBuffSize, COMMLONG& p_reflMsgLen,
                            const char * p_lpszCorrelID /*= NULL*/, const char * p_lpszMsgID /*= NULL*/,
                            short p_siPriority /*= 0*/)
{
  #ifndef _CONNECT_USE_MQ
  return __Recv(p_lpszMsgBuff, p_lBuffSize, p_reflMsgLen, p_lpszCorrelID, p_lpszMsgID, p_siPriority,
    KCXP_GMO_BROWSE_NEXT);
  #else
  return __Recv(p_lpszMsgBuff, p_lBuffSize, p_reflMsgLen, p_lpszCorrelID, p_lpszMsgID, p_siPriority,
    MQGMO_FAIL_IF_QUIESCING | MQGMO_NO_SYNCPOINT | MQGMO_BROWSE_NEXT);
  #endif
}
//取当前队列的最大深度
//输入：
//返回：队列最大深度，-1表示错误
long CCommTools::GetMaxDepth()
{
	COMMLONG   Select[1] ;
	COMMLONG   IAV[1] ;
	COMMLONG   CompCode ;
	COMMLONG   Reason ;
	COMMLONG   count = 1 ;
	long       lDepth = -1 ;

	#ifndef _CONNECT_USE_MQ
	Select[0] = KCXP_QA_MAXQDEPTH ;
	KCXP_Inq(m_hConn, m_hQObj, 1L, Select, &count, IAV, NULL, NULL, &CompCode, &Reason) ;
	#else
	Select[0] = MQIA_MAX_Q_DEPTH ;
	MQINQ(m_hConn, m_hQObj, 1L, Select, 1L, IAV, 0L, NULL, &CompCode, &Reason) ;
	#endif
	if (CompCode == 0L)
	{
		lDepth = IAV[0] ;
	}
	else
	{
		lDepth = -1;
	}

	return lDepth ;
}
//取当前队列的当前深度
//输入：
//返回：队列深度，-1表示错误
long CCommTools::GetCurrentDepth()
{
	COMMLONG   Select[1] ;
	COMMLONG   IAV[1] ;
	COMMLONG   CompCode ;
	COMMLONG   Reason ;
	COMMLONG   count = 1 ;
	long lDepth = -1 ;

	#ifndef _CONNECT_USE_MQ
	Select[0] = KCXP_QA_CURDEPTH ;
	KCXP_Inq(m_hConn, m_hQObj, 1L, Select, &count, IAV, NULL, NULL, &CompCode, &Reason) ;
	#else
	Select[0] = MQIA_CURRENT_Q_DEPTH ;
	MQINQ(m_hConn, m_hQObj, 1L, Select, 1L, IAV, 0L, NULL, &CompCode, &Reason) ;
	#endif
	if (CompCode == 0L)
	{
		lDepth = IAV[0] ;
	}
	else
	{
		lDepth = -1;
	}

	return lDepth ;
}

int CCommTools::SetDepth(long depth)
{
	KCXP_LONG CompCode, Reason;
	KCXP_LONG SelectorCount, Selectors[1];
	KCXP_LONG IntAttrCount, IntAttrs[1];
	SelectorCount =1L;
	Selectors[0] = KCXP_QA_VALDEPTH;
	IntAttrCount = 1L;
	IntAttrs[0] = depth;
	KCXP_Set( m_hConn, m_hQObj, SelectorCount, Selectors, IntAttrCount, IntAttrs, NULL, NULL, &CompCode, &Reason );
	if ( CompCode == KCXP_CC_FAILED )
	{
		return -1;
	}

	return 0;
}

//打开队列管理器
//输入：队列管理器名，[通道名，连接名]
//队列管理器名：
//  KCXP[ip,port,KCXP00,888888,qmname]
//  MQ[qmname]
//    NULL,此时通道名为服务端连接通道名，连接名为IP地址
//返回：0:成功 其它:失败
//  modi by  zhousm 20040518
long CCommTools::OpenQueueManagerx(const char *szQM, const char *szChannel, const char*szConnect)
{
	char szTemp[65];
	char szQueueManger[40];
	char Ip[40];
	char uName[40];
	char uPass[40];
	int Port;

	long lRetCode = 0L;

	#ifndef _CONNECT_USE_MQ
	GetBuff(szTemp, sizeof(szTemp), szQM, 0, ',');
	strcpy(Ip, szTemp);
	GetBuff(szTemp, sizeof(szTemp), szQM, 1, ',');
	Port = atol(szTemp);
	GetBuff(szTemp, sizeof(szTemp), szQM, 2, ',');
	strcpy(uName, szTemp);
	GetBuff(szTemp, sizeof(szTemp), szQM, 3, ',');
	strcpy(uPass, szTemp);
	GetBuff(szTemp, sizeof(szTemp), szQM, 4, ',');
	strcpy(szQueueManger, szTemp);

	sprintf(szTemp, "%d", Port);
	lRetCode = OpenQueueManagerWithCheckShm(szQueueManger, szTemp, Ip);
	#else
	lRetCode = OpenQueueManagerWithCheckShm(szQM, szChannel, szConnect);
	#endif

	return lRetCode;
}
//设置发送消息的生命周期
//输入：单位,KCXP为毫秒，MQ为10秒
//返回：老的生命周期(秒)
int CCommTools::SetLifeTime(int iSecond)
{
	int iOldLifeTime = m_lLifeTime ;

	m_lLifeTime = iSecond ;
	return iOldLifeTime ;
}

//取发送消息的生命周期
//输入：
//返回：生命周期
int CCommTools::GetLifeTime()
{
	return m_lLifeTime ;
}

//设置接收消息的等待时间
//输入：秒
//返回：老的等待时间(秒)
int CCommTools::SetWaitTime(int iMiliSecond)
{
	int iOldWaitTime = m_lWaitTime ;

	m_lWaitTime = iMiliSecond ;
	return iOldWaitTime ;
}

//取接收消息的等待时间
//输入：
//返回：等待时间(毫秒)
int CCommTools::GetWaitTime()
{
	return m_lWaitTime ;
}
