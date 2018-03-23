#ifndef _KCBPCLI_H
#define _KCBPCLI_H

#include <time.h>

#ifdef WIN32
#ifdef KCBPCLI_EXPORTS
#define KCBPCLI_API __declspec(dllexport)
#else
#define KCBPCLI_API __declspec(dllimport)
#endif
#define KCBPCLISTDCALL	__stdcall	/* ensure stcall calling convention on NT */
#else
#define KCBPCLI_API
#define KCBPCLISTDCALL				/* leave blank for other systems */
#endif

/* Transact option values  */
#define KCBP_COMMIT					9
#define KCBP_ROLLBACK				10

#define UNKNOWN_ROWS				0xffffffff

#define KCBP_SERVERNAME_MAX			32
#define KCBP_DESCRIPTION_MAX		32

#define KCBP_MSGID_LEN				32
#define KCBP_CORRID_LEN				32
#define KCBP_USERID_LEN				12
#define KCBP_SERIAL_LEN				26

#define KCBP_PSFLAG_PERSISTENT		0x00000001
#define KCBP_PSFLAG_NOPERSISTENT	0x00000002
#define KCBP_PSFLAG_REDELIVERED		0x00000004
#define KCBP_PSFLAG_NOREDELIVERED	0x00000008
#define KCBP_PSFLAG_SYSTEM			0x00000010
#define KCBP_PSFLAG_USER			0x00000020

#define KCBP_OPTION_CONNECT			0
#define KCBP_OPTION_TIMEOUT			1
#define KCBP_OPTION_TRACE			2
#define KCBP_OPTION_CRYPT			3
#define KCBP_OPTION_COMPRESS		4
#define KCBP_OPTION_PARITY			5
#define KCBP_OPTION_SEQUENCE		6
#define KCBP_OPTION_CURRENT_CONNECT 7
#define KCBP_OPTION_CONNECT_HANDLE	8
#define KCBP_OPTION_CONFIRM			9
#define KCBP_OPTION_NULL_PASS		10
#define KCBP_OPTION_TIME_COST		11
#define KCBP_OPTION_CONNECT_EX		12
#define KCBP_OPTION_CURRENT_CONNECT_EX 13
#define KCBP_OPTION_AUTHENTICATION	14
#define KCBP_OPTION_GROUPID			15
#define KCBP_OPTION_USE_GROUPID		16
#define KCBP_OPTION_RECEIVE_PACKET_BY_GROUP 17
#define KCBP_OPTION_XA				18
#define KCBP_OPTION_CURRENT_XA		19
#define KCBP_OPTION_CONNECTION_POOL 20
#define KCBP_OPTION_TRACE_DIR 		21

#define KCBP_PARAM_NODE				0
#define KCBP_PARAM_CLIENT_MAC		1
#define KCBP_PARAM_CONNECTION_ID	2
#define KCBP_PARAM_SERIAL			3
#define KCBP_PARAM_USERNAME			4
#define KCBP_PARAM_PACKETTYPE		5
#define KCBP_PARAM_PACKAGETYPE		KCBP_PARAM_PACKETTYPE
#define KCBP_PARAM_SERVICENAME		6
#define KCBP_PARAM_RESERVED			7
#define KCBP_PARAM_DESTNODE			8

#define KCBP_FETCH_NEXT			1
#define KCBP_FETCH_FIRST		2
#define KCBP_FETCH_LAST			3
#define KCBP_FETCH_PRIOR		4
#define KCBP_FETCH_ABSOLUTE		5
#define KCBP_FETCH_RELATIVE		6

#if (defined(KCBP_AIX) && defined(__xlC__))
#pragma options align = packed
#else
#pragma pack(1)
#endif
typedef struct
{
	char szServerName[KCBP_SERVERNAME_MAX + 1];
	int nProtocal;
	char szAddress[KCBP_DESCRIPTION_MAX + 1];
	int nPort;
	char szSendQName[KCBP_DESCRIPTION_MAX + 1];
	char szReceiveQName[KCBP_DESCRIPTION_MAX + 1];
	char szReserved[KCBP_DESCRIPTION_MAX + 1];
}
tagKCBPConnectOption;

#define KCBP_PROXY_MAX				128
#define KCBP_SSL_MAX				256
typedef struct
{
	char szServerName[KCBP_SERVERNAME_MAX + 1];
	int nProtocal;
	char szAddress[KCBP_DESCRIPTION_MAX + 1];
	int nPort;
	char szSendQName[KCBP_DESCRIPTION_MAX + 1];
	char szReceiveQName[KCBP_DESCRIPTION_MAX + 1];
	char szReserved[KCBP_DESCRIPTION_MAX + 1];
	char szProxy[KCBP_PROXY_MAX + 1];
	char szSSL[KCBP_SSL_MAX + 1];
}
tagKCBPConnectOptionEx;

/* callback notification function definition. */
typedef void (KCBPCLI_Callback_t) (void *);
typedef KCBPCLI_Callback_t *KCBPCLI_Notify_t;

/* control parameters to publish/subscribe queue primitives */
typedef struct
{
	int nFlags;						/* indicates which of the values are set , inlcude type, mode, redelivered flag*/
	char szId[KCBP_SERIAL_LEN + 1]; /* pub/sub serial identifer */
	char szMsgId[KCBP_MSGID_LEN + 1];	/* id of message before which to queue */
	char szCorrId[KCBP_CORRID_LEN + 1]; /* correlation id used to identify message */
	int nExpiry;						/* subscribe message duration time, unit with second */
	int nPriority;						/* publish priority */
	time_t tTimeStamp;					/* pub/sub timestamp*/
	KCBPCLI_Notify_t lpfnCallback;		/* callback function pointer*/
} tagCallCtrl;

typedef tagCallCtrl tagKCBPPSControl;

#if (defined(KCBP_AIX) && defined(__xlC__))
#pragma options align = reset
#elif defined(KCBP_SOL)
#pragma pack(4)
#else
#pragma pack()
#endif
typedef void *KCBPCLIHANDLE;

#ifdef __cplusplus
extern "C"
{
#endif
KCBPCLI_API int KCBPCLISTDCALL KCBPCLI_Init(KCBPCLIHANDLE *hHandle);
KCBPCLI_API int KCBPCLISTDCALL KCBPCLI_Exit(KCBPCLIHANDLE hHandle);

KCBPCLI_API int KCBPCLISTDCALL KCBPCLI_GetVersion(KCBPCLIHANDLE hHandle, int *pnVersion);

KCBPCLI_API int KCBPCLISTDCALL KCBPCLI_SetConnectOption(KCBPCLIHANDLE hHandle, tagKCBPConnectOption stKCBPConnection);
KCBPCLI_API int KCBPCLISTDCALL KCBPCLI_GetConnectOption(KCBPCLIHANDLE hHandle, tagKCBPConnectOption *pstKCBPConnection);

KCBPCLI_API int KCBPCLISTDCALL KCBPCLI_ConnectServer(KCBPCLIHANDLE hHandle, char *ServerName, char *UserName, char *Password);
KCBPCLI_API int KCBPCLISTDCALL KCBPCLI_DisConnect(KCBPCLIHANDLE hHandle);
KCBPCLI_API int KCBPCLISTDCALL KCBPCLI_DisConnectForce(KCBPCLIHANDLE hHandle);

KCBPCLI_API int KCBPCLISTDCALL KCBPCLI_BeginWrite(KCBPCLIHANDLE hHandle);

/*synchronize call*/
KCBPCLI_API int KCBPCLISTDCALL KCBPCLI_CallProgramAndCommit(KCBPCLIHANDLE hHandle, char *ProgramName);

KCBPCLI_API int KCBPCLISTDCALL KCBPCLI_CallProgram(KCBPCLIHANDLE hHandle, char *ProgramName);
KCBPCLI_API int KCBPCLISTDCALL KCBPCLI_Commit(KCBPCLIHANDLE hHandle);
KCBPCLI_API int KCBPCLISTDCALL KCBPCLI_RollBack(KCBPCLIHANDLE hHandle);

/*asynchronize call*/
KCBPCLI_API int KCBPCLISTDCALL KCBPCLI_ACallProgramAndCommit(KCBPCLIHANDLE hHandle, char *ProgramName, tagCallCtrl *ptagCallCtrl);
KCBPCLI_API int KCBPCLISTDCALL KCBPCLI_ACallProgram(KCBPCLIHANDLE hHandle, char *ProgramName, tagCallCtrl *ptagCallCtrl);
KCBPCLI_API int KCBPCLISTDCALL KCBPCLI_GetReply(KCBPCLIHANDLE hHandle, tagCallCtrl *ptagCallCtrl);
KCBPCLI_API int KCBPCLISTDCALL KCBPCLI_Cancel(KCBPCLIHANDLE hHandle, tagCallCtrl *ptagCallCtrl);

KCBPCLI_API int KCBPCLISTDCALL KCBPCLI_GetValue(KCBPCLIHANDLE hHandle, char *KeyName, char *Vlu, int Len);
KCBPCLI_API int KCBPCLISTDCALL KCBPCLI_SetValue(KCBPCLIHANDLE hHandle, char *KeyName, char *Vlu);

KCBPCLI_API int KCBPCLISTDCALL KCBPCLI_GetVal(KCBPCLIHANDLE hHandle, char *szKeyName, unsigned char **pValue, long *pSize);
KCBPCLI_API int KCBPCLISTDCALL KCBPCLI_SetVal(KCBPCLIHANDLE hHandle, char *szKeyName, unsigned char *pValue, long nSize);

KCBPCLI_API int KCBPCLISTDCALL KCBPCLI_Convert(KCBPCLIHANDLE hHandle, char *szType, int nSrcByteOrder, unsigned char *pSrcBuf, long nSrcBufSize, int nDstByteOrder, unsigned char *pDstBuf, long nDstBufSize, int nFlag);

/*rs*/
KCBPCLI_API int KCBPCLISTDCALL KCBPCLI_RsCreate(KCBPCLIHANDLE hHandle, char *Name, int ColNum, char *pColInfo);
KCBPCLI_API int KCBPCLISTDCALL KCBPCLI_RsNewTable(KCBPCLIHANDLE hHandle, char *Name, int ColNum, char *pColInfo);

KCBPCLI_API int KCBPCLISTDCALL KCBPCLI_RsAddRow(KCBPCLIHANDLE hHandle);
KCBPCLI_API int KCBPCLISTDCALL KCBPCLI_RsSaveRow(KCBPCLIHANDLE hHandle);
KCBPCLI_API int KCBPCLISTDCALL KCBPCLI_RsSetCol(KCBPCLIHANDLE hHandle, int Col, char *Vlu);
KCBPCLI_API int KCBPCLISTDCALL KCBPCLI_RsSetColByName(KCBPCLIHANDLE hHandle, char *Name, char *Vlu);

KCBPCLI_API int KCBPCLISTDCALL KCBPCLI_RsSetVal(KCBPCLIHANDLE hHandle, int nColumnIndex, unsigned char *pValue, long nSize);
KCBPCLI_API int KCBPCLISTDCALL KCBPCLI_RsSetValByName(KCBPCLIHANDLE hHandle, char *szColumnName, unsigned char *pValue, long nSize);

KCBPCLI_API int KCBPCLISTDCALL KCBPCLI_RsOpen(KCBPCLIHANDLE hHandle);
KCBPCLI_API int KCBPCLISTDCALL KCBPCLI_RsMore(KCBPCLIHANDLE hHandle);
KCBPCLI_API int KCBPCLISTDCALL KCBPCLI_RsClose(KCBPCLIHANDLE hHandle);
KCBPCLI_API int KCBPCLISTDCALL KCBPCLI_RsGetCursorName(KCBPCLIHANDLE hHandle, char *pszCursorName, int nLen);
KCBPCLI_API int KCBPCLISTDCALL KCBPCLI_RsGetColNames(KCBPCLIHANDLE hHandle, char *pszInfo, int nLen);
KCBPCLI_API int KCBPCLISTDCALL KCBPCLI_RsGetColName(KCBPCLIHANDLE hHandle, int nCol, char *pszName, int nLen);
KCBPCLI_API int KCBPCLISTDCALL KCBPCLI_RsFetchRow(KCBPCLIHANDLE hHandle);
KCBPCLI_API int KCBPCLISTDCALL KCBPCLI_RsFetchRowScroll(KCBPCLIHANDLE hHandle, int nOrientation, int nOffset);

KCBPCLI_API int KCBPCLISTDCALL KCBPCLI_RsGetCol(KCBPCLIHANDLE hHandle, int Col, char *Vlu);
KCBPCLI_API int KCBPCLISTDCALL KCBPCLI_RsGetColByName(KCBPCLIHANDLE hHandle, char *KeyName, char *Vlu);

KCBPCLI_API int KCBPCLISTDCALL KCBPCLI_RsGetVal(KCBPCLIHANDLE hHandle, int nColumnIndex, unsigned char **pValue, long *pSize);
KCBPCLI_API int KCBPCLISTDCALL KCBPCLI_RsGetValByName(KCBPCLIHANDLE hHandle, char *szColumnName, unsigned char **pValue, long *pSize);

KCBPCLI_API int KCBPCLISTDCALL KCBPCLI_RsGetRowNum(KCBPCLIHANDLE hHandle, int *pnRows);
KCBPCLI_API int KCBPCLISTDCALL KCBPCLI_RsGetColNum(KCBPCLIHANDLE hHandle, int *pnCols);
KCBPCLI_API int KCBPCLISTDCALL KCBPCLI_RsGetTableRowNum(KCBPCLIHANDLE hHandle, int nt, int *pnRows);
KCBPCLI_API int KCBPCLISTDCALL KCBPCLI_RsGetTableColNum(KCBPCLIHANDLE hHandle, int nt, int *pnCols);

/*misc*/
KCBPCLI_API int KCBPCLISTDCALL KCBPCLI_GetErr(KCBPCLIHANDLE hHandle, int *pErrCode, char *ErrMsg);
KCBPCLI_API int KCBPCLISTDCALL KCBPCLI_GetErrorCode(KCBPCLIHANDLE hHandle, int *pnErrno);
KCBPCLI_API int KCBPCLISTDCALL KCBPCLI_GetErrorMsg(KCBPCLIHANDLE hHandle, char *szError);

KCBPCLI_API int KCBPCLISTDCALL KCBPCLI_GetCommLen(KCBPCLIHANDLE hHandle, int *pnLen);
KCBPCLI_API int KCBPCLISTDCALL KCBPCLI_SetCliTimeOut(KCBPCLIHANDLE hHandle, int TimeOut);
KCBPCLI_API int KCBPCLISTDCALL KCBPCLI_SetOption(KCBPCLIHANDLE hHandle, int nIndex, void *pValue);
KCBPCLI_API int KCBPCLISTDCALL KCBPCLI_GetOption(KCBPCLIHANDLE hHandle, int nIndex, void *pValue);
KCBPCLI_API int KCBPCLISTDCALL KCBPCLI_SetOptions(KCBPCLIHANDLE hHandle, int nIndex, void *pValue, int nLen);
KCBPCLI_API int KCBPCLISTDCALL KCBPCLI_GetOptions(KCBPCLIHANDLE hHandle, int nIndex, void *pValue, int *nLen);

KCBPCLI_API int KCBPCLISTDCALL KCBPCLI_SetSystemParam(KCBPCLIHANDLE hHandle, int nIndex, char *szValue);
KCBPCLI_API int KCBPCLISTDCALL KCBPCLI_GetSystemParam(KCBPCLIHANDLE hHandle, int nIndex, char *szValue, int nLen);

/*SQL-Liked*/
KCBPCLI_API int KCBPCLISTDCALL KCBPCLI_SQLConnect(KCBPCLIHANDLE hHandle, char *ServerName, char *UserName, char *Password);
KCBPCLI_API int KCBPCLISTDCALL KCBPCLI_SQLDisconnect(KCBPCLIHANDLE hHandle);
KCBPCLI_API int KCBPCLISTDCALL KCBPCLI_SQLExecute(KCBPCLIHANDLE hHandle, char *szProgramName);
KCBPCLI_API int KCBPCLISTDCALL KCBPCLI_SQLNumResultCols(KCBPCLIHANDLE hHandle, int *pnresultcols);
KCBPCLI_API int KCBPCLISTDCALL KCBPCLI_SQLGetCursorName(KCBPCLIHANDLE hHandle, char *pszCursorName, int nLen);
KCBPCLI_API int KCBPCLISTDCALL KCBPCLI_SQLGetColNames(KCBPCLIHANDLE hHandle, char *szTableInfo, int nLen);
KCBPCLI_API int KCBPCLISTDCALL KCBPCLI_SQLGetColName(KCBPCLIHANDLE hHandle, int nCol, char *szTableInfo, int nLen);
KCBPCLI_API int KCBPCLISTDCALL KCBPCLI_SQLFetch(KCBPCLIHANDLE hHandle);
KCBPCLI_API int KCBPCLISTDCALL KCBPCLI_SQLFetchScroll(KCBPCLIHANDLE hHandle, int nOrientation, int nOffset);
KCBPCLI_API int KCBPCLISTDCALL KCBPCLI_SQLMoreResults(KCBPCLIHANDLE hHandle);
KCBPCLI_API int KCBPCLISTDCALL KCBPCLI_SQLCloseCursor(KCBPCLIHANDLE hHandle);
KCBPCLI_API int KCBPCLISTDCALL KCBPCLI_SQLEndTran(KCBPCLIHANDLE hHandle, int nType);

/*pub/sub*/
KCBPCLI_API int KCBPCLISTDCALL KCBPCLI_Subscribe(KCBPCLIHANDLE hHandle, tagKCBPPSControl *pstPSCtl, char *pszTopicExpr, char *pszFilterExpr);
KCBPCLI_API int KCBPCLISTDCALL KCBPCLI_Unsubscribe(KCBPCLIHANDLE hHandle, tagKCBPPSControl *pstPSCtl);
KCBPCLI_API int KCBPCLISTDCALL KCBPCLI_ReceivePublication(KCBPCLIHANDLE hHandle, tagKCBPPSControl *pstPSCtl, char *pszData, int nDataLen);

KCBPCLI_API int KCBPCLISTDCALL KCBPCLI_RegisterPublisher(KCBPCLIHANDLE hHandle, tagKCBPPSControl *pstPSCtl, char *pszTopicExpr);
KCBPCLI_API int KCBPCLISTDCALL KCBPCLI_DeregisterPublisher(KCBPCLIHANDLE hHandle, tagKCBPPSControl *pstPSCtl);
KCBPCLI_API int KCBPCLISTDCALL KCBPCLI_Publish(KCBPCLIHANDLE hHandle, tagKCBPPSControl *pstPSCtl, char *pszTopicExpr, char *pszData, int nDataLen);

/*broadcast/notify*/
KCBPCLI_API int KCBPCLISTDCALL KCBPCLI_Notify(KCBPCLIHANDLE hHandle, char *szConnectionId, char *szData, int nDataLen, int nFlags);
KCBPCLI_API int KCBPCLISTDCALL KCBPCLI_Broadcast(KCBPCLIHANDLE hHandle, char *szMachineId, char *szUserName, char *szConnectionId, char *szData, int nDataLen, int nFlags);
KCBPCLI_API int KCBPCLISTDCALL KCBPCLI_CheckUnsoliciety(KCBPCLIHANDLE hHandle);
KCBPCLI_API int KCBPCLISTDCALL KCBPCLI_SetUnsoliciety(KCBPCLIHANDLE hHandle);

KCBPCLI_API int KCBPCLISTDCALL KCBPCLI_SSLVerifyCertPasswd(KCBPCLIHANDLE hHandle, char *szKeyFileName, char *szPassword);
KCBPCLI_API int KCBPCLISTDCALL KCBPCLI_SSLModifyCertPasswd(KCBPCLIHANDLE hHandle, char *szKeyFileName, char *szOldPassword,char *szNewPassword);
KCBPCLI_API int KCBPCLISTDCALL KCBPCLI_SSLEncrypt(KCBPCLIHANDLE hHandle, char *szKeyFileName, char *szPassword, unsigned char *pInput, int nInLen, unsigned char **pOutput, int *pnOutLen);
KCBPCLI_API int KCBPCLISTDCALL KCBPCLI_SSLDecrypt(KCBPCLIHANDLE hHandle, char *szKeyFileName, char *szPassword, unsigned char *pInput, int nInLen, unsigned char **pOutput, int *pnOutLen);
KCBPCLI_API int KCBPCLISTDCALL KCBPCLI_FreeMemory(KCBPCLIHANDLE hHandle, void *memblock);
KCBPCLI_API int KCBPCLISTDCALL KCBPCLI_SSLGetCertInfo(KCBPCLIHANDLE hHandle, char *szCertFileName, int nInfoType, int nInfoSubType, char *szBuf, int nLen);

#ifdef __cplusplus
}
#endif
#endif
