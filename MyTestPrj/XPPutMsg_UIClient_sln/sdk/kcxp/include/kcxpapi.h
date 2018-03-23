//# ifdef   __cplusplus
  extern  "C" {
//# endif

# ifndef  __KCXP_API_H
# define  __KCXP_API_H

# include <windows.h>
# include <stdio.h>
# include <stdlib.h>
# include <sys/types.h>

#define   NODECODE                 8
#define   DATALENGTH               32768       /*32K*/
#define   MSGDESC_BUFFER_LENGTH    0x19C 
#define   GMO_BUFFER_LENGTH        0x27
#define   PMO_BUFFER_LENGTH        0x1C
#define   PGO_BUFFER_LENGTH        0x28
#define   OBJDESC_BUFFER_LENGTH    0x54
#define   MAXOPENOBJECT             10   /*max object num*/
#define   KCXP_SOCKET_MAX_SIZE     8192

#define KCXP_VOID     void
#define KCXP_POINTER  *

typedef unsigned char KCXP_BYTE;
typedef char KCXP_CHAR;
typedef char *KCXP_LPSTR;
typedef long KCXP_LONG;
typedef int KCXP_INT;
typedef unsigned int KCXP_UINT;
typedef short KCXP_WORD;
typedef unsigned long KCXP_DWORD;
typedef KCXP_VOID *KCXP_HANDLE;

typedef KCXP_BYTE KCXP_BYTE8[8];
typedef KCXP_BYTE KCXP_BYTE16[16];
typedef KCXP_BYTE KCXP_BYTE24[24];
typedef KCXP_BYTE KCXP_BYTE32[32];
typedef KCXP_BYTE KCXP_BYTE40[40];
typedef KCXP_CHAR KCXP_CHAR4[4];
typedef KCXP_CHAR KCXP_CHAR8[8];
typedef KCXP_CHAR KCXP_CHAR9[9];
typedef KCXP_CHAR KCXP_CHAR12[12];
typedef KCXP_CHAR KCXP_CHAR16[16];
typedef KCXP_CHAR KCXP_CHAR20[20];
typedef KCXP_CHAR KCXP_CHAR24[24];
typedef KCXP_CHAR KCXP_CHAR28[28];
typedef KCXP_CHAR KCXP_CHAR32[32];
typedef KCXP_CHAR KCXP_CHAR48[48];
typedef KCXP_CHAR KCXP_CHAR64[64];
typedef KCXP_CHAR KCXP_CHAR128[128];
typedef KCXP_CHAR KCXP_CHAR256[256];
typedef KCXP_LONG KCXP_HCONN;
typedef KCXP_LONG KCXP_HOBJ;
typedef void KCXP_POINTER KCXP_PTR;

#define   KCXP_PERSISTENCE_MESSAGE     0x01
#define   KCXP_NOPERSISTENCE_MESSAGE   0x00

/* char set id */
#define   KCXP_CHARSET_ASCII      0
#define   KCXP_CHARSET_EBCDIC     1     
#define   KCXP_CHARSET_UNICODE    1200
#define   KCXP_CHARSET_UTF_8      65001
#define   KCXP_CAHRSET_GB2312     936

/* route type */
#define   KCXP_RT_Q             0x01
#define   KCXP_RT_QMGR          0x02
#define   KCXP_RT_CHANNEL       0x03
#define   KCXP_RT_ROUTE         0x04
#define   KCXP_RT_REMOTEDEF     0x05
#define   KCXP_RT_TRIGGER       0x06
#define   KCXP_RT_PLUGIN        0x07

/*queue type define*/
#define   KCXP_QT_PERSISTENCE    0x01
#define   KCXP_QT_TRANSMISSION   0x02
#define   KCXP_QT_APPLICATION    0x03
#define   KCXP_QT_INITIALIZE     0x04
#define   KCXP_QT_DEADLETTER     0x05
#define   KCXP_QT_COMMAND        0x06
#define   KCXP_QT_RET_COMMAND    0x07
#define   KCXP_QT_LOG            0x08
#define   KCXP_QT_MONITOR        0x09
#define   KCXP_QT_EVENT          0x10
#define   KCXP_QT_TIMER          0x11

/* message type define */
#define   KCXP_MT_DATAGRAM		0x01
#define   KCXP_MT_REQUEST		0x02
#define   KCXP_MT_RESPONSE		0x03
#define   KCXP_MT_REPORT		0x04
#define   KCXP_MT_IDENTIFY		0x05
#define   KCXP_MT_CHANNEL		0x06
#define   KCXP_MT_COMMAND		0x07
#define   KCXP_MT_DEADLETTER	0x08
#define   KCXP_MT_DETECT		0x09
#define   KCXP_MT_TRACE			0x13
#define   KCXP_MT_CANCEL		0x14
#define   KCXP_MT_TRIGGER       0x15
#define   KCXP_MT_EVENT         0x16
#define   KCXP_MT_FLOWCONTROL   0x17
#define   KCXP_MT_ROUTE_DETECT  0x18
#define   KCXP_MT_BULLETIN      0x19
#define   KCXP_MT_PLUGIN        0x20

#define   KCXP_MT_APICLOSE		0x22
#define   KCXP_MT_APICONN		0x23
#define   KCXP_MT_APIDISCONN	0x24
#define   KCXP_MT_APIGET		0x25
#define   KCXP_MT_APIINQ		0x26
#define   KCXP_MT_APIOPEN		0x27
#define   KCXP_MT_APIPUT		0x28
#define   KCXP_MT_APISET		0x29
#define   KCXP_MT_APIPUT1       0x30  
#define   KCXP_MT_APICONNEX     0x31
#define   KCXP_MT_APIPUTEX      0x32
#define   KCXP_MT_APIDISCONNEX  0x33
#define   KCXP_MT_APIIDENTIFY   0x34
#define   KCXP_MT_APIIDENTIFYEX 0x35
#define   KCXP_MT_APIPUTGET     0x36
#define   KCXP_MT_APIPUT2       0x37
#define   KCXP_MT_APIBATCH_DELETE   0x38

/* Message Flags */
# define  KCXP_MF_SEGMENTATION_INHIBITED 0x00
# define  KCXP_MF_NONE                   0x00
# define  KCXP_MF_SEGMENTATION_ALLOWED   0x01
# define  KCXP_MF_SEGMENT                0x02
# define  KCXP_MF_LAST_SEGMENT           0x04
# define  KCXP_MF_MSG_IN_GROUP           0x08
# define  KCXP_MF_LAST_MSG_IN_GROUP      0x10

/*open object type*/
#define   KCXP_OT_Q                 0x01L
#define   KCXP_OT_QMGR              0x02L
#define   KCXP_OT_CHANNEL           0x03L
#define   KCXP_OT_ROUTE             0x04L
#define   KCXP_OT_REMOTEDEF         0x05L
#define   KCXP_OT_TRIGGER           0x06L

/*object operation mode*/
#define   KCXP_OO_AS_Q_DEF			0x00000001L
#define   KCXP_OO_INPUT				0x00000002L
#define   KCXP_OO_OUTPUT			0x00000004L
#define   KCXP_OO_SHARED            0x00000008L
#define   KCXP_OO_EXCLUSIVE			0x00000020L
#define   KCXP_OO_BROWSE			0x00000040L
#define   KCXP_OO_INQUIRE			0x00000080L
#define   KCXP_OO_SET				0x00001000L
#define   KCXP_OO_FAIL_IF_QUIESCING 0x00002000L

/* message priority define */
#define   KCXP_PRIORITY_0      0x00
#define   KCXP_PRIORITY_1      0x01
#define   KCXP_PRIORITY_2      0x02
#define   KCXP_PRIORITY_3      0x03
#define   KCXP_PRIORITY_4      0x04
#define   KCXP_PRIORITY_5      0x05
#define   KCXP_PRIORITY_6      0x06
#define   KCXP_PRIORITY_7      0x07
#define   KCXP_PRIORITY_8      0x08
#define   KCXP_PRIORITY_9      0x09
#define   KCXP_PRIORITY_10     0x0A
#define   KCXP_PRIORITY_11     0x0B
#define   KCXP_PRIORITY_12     0x0C
#define   KCXP_PRIORITY_13     0x0D
#define   KCXP_PRIORITY_14     0x0E
#define   KCXP_PRIORITY_15     0x0F
#define   KCXP_PRIORITY_16     0x10
#define   KCXP_PRIORITY_17     0x11
#define   KCXP_PRIORITY_18     0x12
#define   KCXP_PRIORITY_19     0x13

#define   KCXP_SUCCESS      0
#define   KCXP_FAIL         -1

#define KCXP_QA_QNAME   0x01      	
#define KCXP_QA_QTYPE   0x02        
#define KCXP_QA_QDESC   0x04       
#define KCXP_QA_INHPUT  0x08      
#define KCXP_QA_INHGET  0x10      
#define KCXP_QA_DEFPRIORITY  0x20 	
#define KCXP_QA_DEFPERSISTENCE  0x30
#define KCXP_QA_MAXQDEPTH  0x40   
#define KCXP_QA_MAXMSGLENGTH  0x50
#define KCXP_QA_CURDEPTH  0x60   
#define KCXP_QA_VALDEPTH  0x70  
#define KCXP_QA_LINKNUM  0x80  
#define KCXP_QA_SHARE  0x90   
#define KCXP_QA_MSGDELSEQ 0xA0
#define KCXP_QA_TRIGGERCONTROL 0xB0 
#define KCXP_QA_TRIGGERTYPE  0xC0  
#define KCXP_QA_TRIGGERDEPTH 0xD0
#define KCXP_QA_TRIGGERMSGPRIORITY 0xE0
#define KCXP_QA_INITQNAME  0xF0       
#define KCXP_QA_PROCESSNAME  0x100  
#define KCXP_QA_MAXDEPTHEVT  0x200  
#define KCXP_QA_DEPTHHIGHEVT  0x300
#define KCXP_QA_DEPTHHIGHLIMIT  0x400
#define KCXP_QA_CREATEDATE  0x500 
#define KCXP_QA_CREATETIME 0x600
#define KCXP_QA_OPENINPUTCOUNT 0x700
#define KCXP_QA_OPENOUTPUTCOUNT 0x800
#define KCXP_QA_ALTERDATE  0x900     
#define KCXP_QA_ALTERTIME  0xA00    
#define KCXP_QA_TIMEWINDOWFROM 0xB00
#define KCXP_QA_ACTIVETIMEVAL   0xC00
#define KCXP_QA_TIMETHREADID   0xD00

#define   KCXP_QN_FTP_REQ        "FTPREQ"
#define   KCXP_QN_FTP_ANS        "FTPANS"

/* trigger type */
#define   KCXP_TT_NONE                   0x00
#define   KCXP_TT_FIRST                  0x01
#define   KCXP_TT_EVERY                  0x02
#define   KCXP_TT_DEPTH                  0x03

/* trigger control */
#define   KCXP_TT_ENABLE    0x01
#define   KCXP_TT_DISABLE   0x00

/* Completion Codes */
# define  KCXP_CC_OK       0L
# define  KCXP_CC_WARNING  1L
# define  KCXP_CC_FAILED   2L
# define  KCXP_CC_UNKNOWN  (-1)L

/*close option*/
#define   KCXP_CO_NONE              0x00L
#define   KCXP_CO_DELETE            0x01L
#define   KCXP_CO_DELETE_PURGE      0x02L

/* Reason Codes */
# define  KCXP_RC_NONE                       0L
# define  KCXP_RC_DEFAULT                    1L
# define  KCXP_RC_ALLOC_MEMORY_FAILED 	     10L
# define  KCXP_RC_INVALID_PARM		     	 100L
# define  KCXP_RC_MESSAGE_OVER_LIFETIME	     101L
# define  KCXP_RC_IDENTIFY_NAME              110L
# define  KCXP_RC_IDENTIFY_PASSWORD          111L
# define  KCXP_RC_INVALID_QMGRNAME           120L
# define  KCXP_RC_INVALID_NODECODE           121L
# define  KCXP_RC_ROUTE_NONODE		     	 200L
# define  KCXP_RC_NO_QUEUE		     		 201L
# define  KCXP_RC_NOT_FREE_COMM_SLOT         202L
# define  KCXP_RC_QUEUE_FILLED		         301L
# define  KCXP_RC_QUEUE_INSERT		         302L
# define  KCXP_RC_QUEUE_GET		             303L
# define  KCXP_RC_QUEUE_MAXMESSAGE_LENGTH    304L
# define  KCXP_RC_QUEUE_PROHIBIT_PUT	     305L
# define  KCXP_RC_QUEUE_MAX_DEPTH            306L
# define  KCXP_RC_RECEIVE_FAILED             400L
# define  KCXP_RC_SEND_FAILED                401L
# define  KCXP_RC_LIFETIME_ERROR             2002L
# define  KCXP_RC_HCONN_ERROR                2003L
# define  KCXP_RC_HOBJ_ERROR                 2004L
# define  KCXP_RC_MAX_CONNS_LIMIT_REACHED    2005L
# define  KCXP_RC_MD_ERROR                   2006L
# define  KCXP_RC_MISSING_RESPONSE_Q         2007L
# define  KCXP_RC_MSG_TYPE_ERROR             2008L
# define  KCXP_RC_MSG_TOO_BIG_FOR_Q          2009L
# define  KCXP_RC_MSG_TOO_BIG_FOR_QMGR       2010L
# define  KCXP_RC_NO_MSG_AVAILABLE           2011L
# define  KCXP_RC_NO_MSG_UNDER_CURSOR        2012L
# define  KCXP_RC_NOT_OPEN_FOR_BROWSE        2013L
# define  KCXP_RC_NOT_OPEN_FOR_GET           2014L
# define  KCXP_RC_NOT_OPEN_FOR_INQUIRE       2015L
# define  KCXP_RC_NOT_OPEN_FOR_PUT           2016L
# define  KCXP_RC_NOT_OPEN_FOR_SET           2017L
# define  KCXP_RC_OBJECT_CHANGED             2018L
# define  KCXP_RC_OBJECT_IN_USE              2019L
# define  KCXP_RC_OBJECT_TYPE_ERROR          2020L
# define  KCXP_RC_OD_ERROR                   2021L
# define  KCXP_RC_OPTION_NOT_VALID_FOR_TYPE  2022L
# define  KCXP_RC_OPTION_ERROR               2023L
# define  KCXP_RC_PERSISTENCE_ERROR          2024L
# define  KCXP_RC_PERSISTENT_NOT_ALLOWED     2025L
# define  KCXP_RC_PRIORITY_EXCEEDS_MAXIMUM   2026L
# define  KCXP_RC_PRIORITY_ERROR             2027L
# define  KCXP_RC_Q_DELETED                  2028L
# define  KCXP_RC_Q_FULL                     2029L
# define  KCXP_RC_Q_NOT_EMPTY                2030L
# define  KCXP_RC_Q_TYPE_ERROR               2031L
# define  KCXP_RC_Q_MGR_NAME_ERROR           2032L
# define  KCXP_RC_Q_MGR_NOT_AVAILABLE        2033L
# define  KCXP_RC_REPORT_OPTIONS_ERROR       2034L
# define  KCXP_RC_TRIGGER_CONTROL_ERROR      2035L
# define  KCXP_RC_TRIGGER_DEPTH_ERROR        2036L
# define  KCXP_RC_TRIGGER_MSG_PRIORITY_ERROR 2037L

# define  KCXP_RC_INTR_ERROR                 2038L
# define  KCXP_RC_AGAIN_ERROR                2039L
# define  KCXP_RC_IO_ERROR                   2040L
# define  KCXP_RC_INVAL_ERROR                2041L
# define  KCXP_RC_FAULT_ERROR                2042L
# define  KCXP_RC_SOCKET_ERROR               2043L
# define  KCXP_RC_CONNECT_ERROR              2044L

# define  KCXP_RC_NO_USER_ERROR              2045L
# define  KCXP_RC_PASSWORD_ERROR             2046L
# define  KCXP_RC_LB_ERROR                   2047L
# define  KCXP_RC_NODE_TOO_BUSY              2048L

# define  KCXP_RC_CHANNEL_NAME_ERROR         2049L
# define  KCXP_RC_NODE_CODE_ERROR            2050L
# define  KCXP_RC_NOT_ROUTE_ERROR            2051L

# define  KCXP_RC_QM_STOP                    2052L
# define  KCXP_RC_QM_PAUSE                   2063L
# define  KCXP_RC_QM_DISABLED                2064L
# define  KCXP_RC_Q_NAME_ERROR               2053L

# define  KCXP_RC_CONNECTION_BROKEN          2054L
# define  KCXP_RC_CONNECTION_STOPPING        2055L

# define  KCXP_RC_SELECTOR_COUNT_ERROR       2056L
# define  KCXP_RC_SELECTOR_ERROR             2057L
# define  KCXP_RC_SELECTOR_LIMIT_EXCEEDED    2058L

# define  KCXP_RC_CHAR_ATTRS_TOO_SHORT       2059L
# define  KCXP_RC_INT_ATTR_COUNT_TOO_SMALL   2060L
# define  KCXP_RC_SELECTOR_NOT_FOR_TYPE      2061L

# define  KCXP_RC_OPTIONS_ERROR              2062L

# define  KCXP_RC_OPER_OBJECT_ERROR          2065L

# define  KCXP_RC_GMO_ERROR                  2066L
# define  KCXP_RC_READ_CONF_Q_MGR_NAME_ERROR 2067L
# define  KCXP_RC_READ_CONF_Q_MGR_IP_ERROR   2068L
# define  KCXP_RC_READ_CONF_Q_MGR_USP_ERROR  2069L
# define  KCXP_RC_Q_MGR_NAME_DEFINE_ERROR    2070L
# define  KCXP_RC_Q_MGR_NAME_MATCH_ERROR     2071L
# define  KCXP_RC_READ_CONF_Q_MGR_USER_NAME_ERROR 2072L
# define  KCXP_RC_READ_SHADOW_ERROR          2073L

# define  KCXP_RC_Q_MGR_EXIST                2074L
# define  KCXP_RC_Q_MGR_UN_CREATE            2075L
# define  KCXP_RC_PARAMTER_ERROR             2076L

# define  KCXP_RC_OPEN_USER_INFO_FILE_ERROR  2078L
# define  KCXP_RC_READ_USER_INFO_FILE_ERROR  2079L
# define  KCXP_RC_OPEN_PROC_LOADAVG_ERROR    2080L
# define  KCXP_RC_OPEN_PROC_MEMINFO_ERROR    2081L
# define  KCXP_RC_DATA_ERROR                 2082L
# define  KCXP_RC_GET_FLOW_ERROR             2083L
# define  KCXP_RC_ARRIVE_VAL_DEPTH           2084L
# define  KCXP_RC_OPEN_FILE_ERROR            2085L
# define  KCXP_RC_EVENT_LIST_FULL            2086L
# define  KCXP_RC_NOT_FOUND_EVENT_DEFINE     2087L

# define  KCXP_RC_ENCRYPT_ERROR              2088L
# define  KCXP_RC_DECRYPT_ERROR              2089L
# define  KCXP_RC_COMPRESS_ERROR             2090L
# define  KCXP_RC_UNCOMPRESS_ERROR           2091L

# define  KCXP_RC_READ_DATA_TIMEOUT_ERROR    2092L

# define  KCXP_RC_NO_WAIT_TIME               2093L
# define  KCXP_RC_LOADBALANCE_FAIL           2094L
# define  KCXP_RC_MALLOC_ERROR               2095L

# define  KCXP_RC_DLOPEN_COMPRESS_SO_ERROR   2096L
# define  KCXP_RC_DLSYM_COMPRESS_SO_ERROR    2097L
# define  KCXP_RC_DLOPEN_CRYPT_SO_ERROR      2098L
# define  KCXP_RC_DLSYM_CRYPT_SO_ERROR       2099L

# define  KCXP_RC_OPEN_PLUGIN_FILE_ERROR     2100L
# define  KCXP_RC_READ_PLUGIN_FILE_ERROR     2101L
# define  KCXP_RC_INVALID_PARM_PLUGIN_FILE   2102L
# define  KCXP_RC_PLUGIN_NOT_FOUND           2103L
# define  KCXP_RC_PLUGIN_DLOPEN_ERROR        2104L
# define  KCXP_RC_PLUGIN_DLSYM_ERROR         2105L
# define  KCXP_RC_PLUGIN_CALL_ERROR          2106L

# define  KCXP_RC_MALLOC_HCONN_SLOT_ERROR    2107L

# define  KCXP_RC_APP_PROGRAM_ABEND          2108L

# define  KCXP_RC_OPEN_TOO_MANY_OBJECT_ERROR 2109L
# define  KCXP_RC_MT_NOT_TIME_INFINITE       2110L
# define  KCXP_RC_READ_CONF_Q_MGR_PROTOCOL_ERROR 2111L
# define  KCXP_RC_MSG_LIFETIME_OVER          2112L
# define  KCXP_RC_EXIT_HANDLE_FAIL           2200L
# define  KCXP_RC_ROUTE_NOT_EXIST            2290L

# define  KCXP_RC_SSL_NEGOCIATE_FAIL		 2300L
# define  KCXP_RC_QD_NOT_FOUND		     	 4000L
# define  KCXP_RC_QU_MALLOC_ERROR	     	 4001L
# define  KCXP_RC_NO_WRITE_PERM		         4002L
# define  KCXP_RC_REACH_MAX_DEPTH	         4003L
# define  KCXP_RC_REACH_VAL_DEPTH	         4004L
# define  KCXP_RC_MSG_TOO_LARGE		         4005L
# define  KCXP_RC_QUEUE_EMPTY		         4006L
# define  KCXP_RC_NO_READ_PERM		         4007L
# define  KCXP_RC_QUEUE_TYPE_ERROR 	         4008L
# define  KCXP_RC_QUEUE_SHARE_ERROR 	     4009L
# define  KCXP_RC_QUEUE_PERM_ERROR 	         4010L
# define  KCXP_RC_CREATE_QUEUE_ERROR 	     4011L
# define  KCXP_RC_SET_ATTR_NOT_PERM 	     4012L
# define  KCXP_RC_ATTR_NOT_FOUND 	         4013L
# define  KCXP_RC_INVALID_MODE 	     	     4014L
# define  KCXP_RC_REMOVE_QS_ERROR 	         4015L
# define  KCXP_RC_EMPTY_Q_ERROR 	         4016L
# define  KCXP_RC_MSG_NOT_FOUND 	         4017L
# define  KCXP_RC_TRIGGER_FIRST_MSG 	     4018L
# define  KCXP_RC_TRIGGER_DEPTH_MSG 	     4019L
# define  KCXP_RC_TRIGGER_EACH_MSG 	         4020L
 
# define  KCXP_RC_CHANNEL_OPERATION_TOBEGIN  5000L
# define  KCXP_RC_CHANNEL_OPERATION_NO_TOEND 5001L
# define  KCXP_RC_CHANNEL_OPERATION_TOEND    5002L
# define  KCXP_RC_CHANNEL_EXIST              5003L
# define  KCXP_RC_CHANNEL_PATH_EXIST         5004L
# define  KCXP_RC_CHANNEL_NOT_EXIST          5005L
# define  KCXP_RC_CHANNEL_INVALID_CHANNEL    5006L
# define  KCXP_RC_CHANNEL_INVALID_PATH       5007L
# define  KCXP_RC_CHANNEL_STATUS_STOP        5008L
# define  KCXP_RC_REMOTEDEF_EXIST            6000L
# define  KCXP_RC_REMOTEDEF_NO_EXIST         6001L
 
# define  KCXP_RC_PLUGIN_OPEN	             7000L
# define  KCXP_RC_PLUGIN_GET                 7001L
# define  KCXP_RC_PLUGIN_CALL                7002L
# define  KCXP_RC_PLUGIN_NOT_FIND            7003L
# define  KCXP_RC_MESSAGE_PLUGIN             7010L

# define  KCXP_RC_SYSTEM_ERROR               9999L

# define  KCXP_RC_FTP_LOGGED_IN              8000L
# define  KCXP_RC_FTP_LOGIN_INCORRECT        8001L
# define  KCXP_RC_FTP_TRANSFER_COMPLETE      8002L
# define  KCXP_RC_FTP_CURRENT_DIR            8003L
# define  KCXP_RC_FTP_NO_FILEDIR             8004L
# define  KCXP_RC_FTP_CWD_SUCCESS            8005L
# define  KCXP_RC_FTP_LOGIN_SERVICE          8006L
# define  KCXP_RC_FTP_TYPE_BINARY            8007L
# define  KCXP_RC_FTP_TYPE_ASCII             8008L
# define  KCXP_RC_FTP_USING_ASCII            8009L
# define  KCXP_RC_FTP_USING_BIARY            8010L
# define  KCXP_RC_FTP_PROMPT_OFF             8011L
# define  KCXP_RC_FTP_PROMPT_ON              8012L
# define  KCXP_RC_FTP_NO_REACH               8013L
# define  KCXP_RC_FTP_OVER_MAX               8014L
# define  KCXP_RC_FTP_UNKNOWN_COMMAND        8015L
# define  KCXP_RC_FTP_NO_RIGHT               8016L
# define  KCXP_RC_FTP_UNKNOWN_ERROR          8050L

/*============================================================================*/
/* Encryption Mode --                                                         */
/*============================================================================*/
# define   KCXP_EM_NONE                 	0x00
# define   KCXP_EM_ENCRYPT              	0x01
# define   KCXP_EM_DECRYPT              	0x02

/*============================================================================*/
/* Compress Mode --                                                           */
/*============================================================================*/
# define   KCXP_CM_NONE                 	0x00
# define   KCXP_CM_COMPRESS             	0x01
# define   KCXP_CM_UNCOMPRESS           	0x02

/*============================================================================*/
/* Put-Message Options                                                        */
/*============================================================================*/
# define  KCXP_PMO_NEW_MSG_ID               0x00000040L
# define  KCXP_PMO_NEW_CORREL_ID            0x00000080L
# define  KCXP_PMO_NEW_GROUP_ID             0x00000100L
# define  KCXP_PMO_LOGICAL_ORDER            0x00008000L
# define  KCXP_PMO_FAIL_IF_QUIESCING        0x00002000L
# define  KCXP_PMO_NONE                     0x00000000L
# define  KCXP_PMO_NO_RECV                  0x00000200L

/*============================================================================*/
/*  Values Related to KCXP_GMO Structure                                      */
/*============================================================================*/
/* Structure Identifier */
# define KCXP_GMO_STRUC_ID "GMO    "

/* Get-Message Options */
# define KCXP_GMO_WAIT                    0x00000001L
# define KCXP_GMO_NO_WAIT                 0x00000000L
# define KCXP_GMO_BROWSE_FIRST            0x00000010L
# define KCXP_GMO_BROWSE_NEXT             0x00000020L
# define KCXP_GMO_BROWSE_MSG_UNDER_CURSOR 0x00000800L
# define KCXP_GMO_GET                     0x00000100L
# define KCXP_GMO_CLEAR                   0x00000400L
# define KCXP_GMO_FAIL_IF_QUIESCING       0x00002000L
# define KCXP_GMO_CONVERT                 0x00004000L
# define KCXP_GMO_NONE                    0x00000000L
# define KCXP_GMO_NO_SEND                 0x00000200L
/* Wait Interval */
# define KCXP_WI_UNLIMITED (-1L)

/* Match Options */
# define KCXP_MO_MATCH_MSG_ID         0x00000001L
# define KCXP_MO_MATCH_CORREL_ID      0x00000002L
# define KCXP_MO_MATCH_GROUP_ID       0x00000004L
# define KCXP_MO_MATCH_MSG_SEQ_NUMBER 0x00000008L
# define KCXP_MO_MATCH_OFFSET         0x00000010L
# define KCXP_MO_MATCH_POSITION       0x00000020L
# define KCXP_MO_MATCH_NONE           0x00000000L

/* Group Status */
# define KCXP_GS_NOT_IN_GROUP      ' '
# define KCXP_GS_MSG_IN_GROUP      'G'
# define KCXP_GS_LAST_MSG_IN_GROUP 'L'

/* Segment Status */
# define KCXP_SS_NOT_A_SEGMENT ' '
# define KCXP_SS_SEGMENT       'S'
# define KCXP_SS_LAST_SEGMENT  'L'

/**************************** FTP define **********************/

#define FTPSTRUCID           "KCXPFTP"

#define KCXP_FTP_CMD_HELP     0x00
#define KCXP_FTP_CMD_LOGIN    0x01
#define KCXP_FTP_CMD_TYPE     0x02
#define KCXP_FTP_CMD_STATUS   0x03
#define KCXP_FTP_CMD_ASCII    0x04
#define KCXP_FTP_CMD_BINARY   0x05
#define KCXP_FTP_CMD_QUIT     0x06
#define KCXP_FTP_CMD_BYE      0x07
#define KCXP_FTP_CMD_DELETE   0x08

#define KCXP_FTP_CMD_PWD      0x20
#define KCXP_FTP_CMD_CD       0x21
#define KCXP_FTP_CMD_LS       0x22
#define KCXP_FTP_CMD_DIR      0x23
#define KCXP_FTP_CMD_MKDIR    0x24
#define KCXP_FTP_CMD_RMDIR    0x25

#define KCXP_FTP_CMD_PROMPT   0x30
#define KCXP_FTP_CMD_SEND     0x31
#define KCXP_FTP_CMD_RECV     0x32
#define KCXP_FTP_CMD_PUT      0x33
#define KCXP_FTP_CMD_GET      0x34
#define KCXP_FTP_CMD_GETEND   0x35
#define KCXP_FTP_CMD_MPUT     0x36
#define KCXP_FTP_CMD_MGET     0x37

/*ftp command type*/
#define KCXP_FTP_CMD_TYPE_REQUIRE 0x00
#define KCXP_FTP_CMD_TYPE_ANSWER  0x01

/*ftp type*/
#define KCXP_FTP_TYPE_ASCII       0x00
#define KCXP_FTP_TYPE_BINARY      0x01

/*ftp prompt*/
#define KCXP_FTP_PROMPT_ON        0x00
#define KCXP_FTP_PROMPT_OFF       0x01

/*ftp right*/
#define KCXP_FTP_RIGHT_READ        0x01
#define KCXP_FTP_RIGHT_LIST        0x02
#define KCXP_FTP_RIGHT_CREATE      0x04
#define KCXP_FTP_RIGHT_DELETE      0x08

#pragma pack(push, 1)
typedef struct tagKCXP_FTPCOMMAND
{
	KCXP_CHAR 	strStrucID[8];
	KCXP_CHAR 	strVersion[8];
	KCXP_CHAR   strUserID[32];    /*由FTP service 提供的对应每个客户的标识*/
	KCXP_INT   	iCommand;         /*FTP command*/
	KCXP_CHAR   cbType;           /*Req or Ans*/
	KCXP_INT    iAck;             /*确认序号*/
	KCXP_LONG  	iCompCode;        
}KCXP_FTPCOMMAND, *LPKCXP_FTPCOMMAND;
#pragma pack(pop)

#define KCXP_DEFAULT_FTPCOMMAND  "KCXPFTP",\
                                 "1.0.0.0",\
                                 0x0,\
                                 0x0,\
                                 0x0,\
                                 0x0,\
                                 0x0

/****************************FTP********************************/

#pragma pack(push, 1)
typedef struct tagKCXP_MD
{
  KCXP_CHAR8 strStrucId;		/*struct id */
  KCXP_CHAR8 strVersion;		/*version */
  KCXP_BYTE cbReport;			/*report flag */
  KCXP_BYTE cbMsgType;			/*message type */
  KCXP_LONG iLifeTime;			/*life time (ms) */
  KCXP_LONG iInitTime;			/*enter qm time, unit: ms */
  KCXP_LONG iFeedback;			/*feedback code */
  KCXP_LONG iEncoding;			/*data encode */
  KCXP_LONG iCodedCharSetId;	/*char set id */
  KCXP_LONG iPriority;			/*priority */
  KCXP_BYTE cbPersistence;		/*persistence */
  KCXP_BYTE cbFlow;			    /*flow of dest queue */
  KCXP_CHAR24 strMsgId;			/*message id */
  KCXP_CHAR24 strCorrelId;      /*Correlation identifier*/
  KCXP_CHAR32 strReplyToQ;		/*reply quene name */
  KCXP_CHAR32 strReplyToQMgr;	/*reply queue manager */
  KCXP_CHAR9 strSrcNodeCode;	/*source node code */
  KCXP_CHAR32 strChannelName;	/*channel name */
  KCXP_CHAR9 strDestNodeCode;	/*destination node code */
  KCXP_CHAR32 strDestQm;		/*destination queue manager */
  KCXP_CHAR32 strDestQ;			/*destination queue */
  KCXP_BYTE cbRouteType;		/*route type */
  KCXP_BYTE cbCompMode;			/*compress mode: 1-127: end to end, 1:default(lzh); 
                                 128-255: node to node, 125:default(lzh);                                                             */
  KCXP_BYTE cbEncryptMode;		/*encrypte mode: 1-127: end to end, 1:default(idea); 
                                 127-255: node to node, 125:idea;*/
  KCXP_LONG iPutAppType;		/*put application type */
  KCXP_CHAR32 strPutAppName;	/*put application name */
  KCXP_CHAR16 strPutDate;		/*put date */
  KCXP_CHAR16 strPutTime;		/*put time */
  KCXP_CHAR24 strGroupId;		/*group id */
  KCXP_LONG iMsgSeqNumber;		/*message sequence number */
  KCXP_LONG iOffset;			/*offset value */
  KCXP_BYTE cbMsgFlags;			/*message flag */
  KCXP_LONG iOriginalLength;	/*original message length */
  KCXP_LONG iDataLength;		/*data  length */
  KCXP_CHAR32 strReserved;		/*reserved */
}KCXP_MD, *PKCXP_MD;
#pragma pack(pop)

/* Trigger Message head*/
typedef struct tagKCXP_TM
{
	KCXP_CHAR8	strStrucID;
	KCXP_CHAR8	strVersion;
	KCXP_CHAR32	strQName;
	KCXP_CHAR32	strProcessName;
	KCXP_CHAR64	strTriggerData;
	KCXP_LONG	iAppType;
	KCXP_CHAR256	strAppID;
	KCXP_CHAR128    strEnvData;
	KCXP_CHAR128    strUserData;
}KCXP_TM;

typedef struct tagKCXP_NODEUSER
{
	KCXP_CHAR32 strUserName;	/*节点用户名 */
	KCXP_CHAR16 strPassword;	/*用户口令 */
	KCXP_LONG iRight;			/*用户权限 */
}KCXP_NODEUSER;

typedef struct tagKCXP_OD
{
	KCXP_CHAR8 strStrucId;
	KCXP_CHAR8 strVersion;
	KCXP_CHAR32 strObjectName;
	KCXP_LONG iObjectType;
	KCXP_CHAR32 strQmgrName;
}KCXP_OD, *PKCXP_OD;
 
/*============================================================================*/
/* Put Message Options structure.                                             */
/*============================================================================*/
typedef struct tagKCXP_PMO
{
	KCXP_CHAR8  strStrucId;
	KCXP_CHAR8  strVersion;
	KCXP_LONG   Options;
	KCXP_LONG   Timeout;
    KCXP_LONG   Hobj;
}KCXP_PMO;

/* for optimization ; by tianx in 20030706 at shanghai intel*/
typedef struct tagKCXP_PGO
{
	KCXP_CHAR8  strStrucId;
	KCXP_CHAR8  strVersion;
	KCXP_LONG putOptions;
	KCXP_LONG putHobj;
	KCXP_LONG getOptions;
	KCXP_LONG getMatchOptions;
	KCXP_LONG getHobj;
	KCXP_LONG	WaitInterval;
}KCXP_PGO, *LPKCXP_PGO;

#define KCXP_OD_STRUC_ID "OD     "

#define KCXP_OD_STRUC_ID_ARRAY 'O','D',' ',' ',' ',' ',' ','\0'

#define KCXP_OD_VERSION "1.0.0.0"

#define KCXP_OD_VERSION_ARRAY '1','.','0','.','0','.','0','\0'

#define KCXP_DEFAULT_OD  {KCXP_OD_STRUC_ID_ARRAY},\
        {KCXP_OD_VERSION_ARRAY},\
        "",\
        KCXP_OT_Q,\
        ""        

/* structure indentify */
# define  KCXP_PMO_STRUCT_ID  "PMO    "

# define  KCXP_PGO_STRUCT_ID  "PGO    "

/* structure indentify (array form) */
# define  KCXP_PMO_STRUCT_ID_ARRAY 'P','M','O',' ',' ',' ',' ','\0'

# define  KCXP_PGO_STRUCT_ID_ARRAY 'P','G','O',' ',' ',' ',' ','\0'

# define  KCXP_PMO_VERSION  "1.0.0.0"

# define  KCXP_PMO_VERSION_ARRAY  '1','.','0','.','0','.','0','\0'

# define  KCXP_DEFAULT_PMO  {KCXP_PMO_STRUCT_ID_ARRAY},\
			    {KCXP_PMO_VERSION_ARRAY},\
			    0L,\
			    0L,\
			    0L

# define  KCXP_DEFAULT_PGO  {KCXP_PGO_STRUCT_ID_ARRAY},\
							{KCXP_PMO_VERSION_ARRAY},\
							0L,\
							0L,\
							0L,\
							0L,\
							0L,\
							0L
							

/*============================================================================*/
/* Get Message Options structure.                                             */
/*============================================================================*/
typedef  struct  tagKCXP_GMO
{
	KCXP_CHAR8  strStrucId;
	KCXP_CHAR8  strVersion;
	KCXP_LONG   Options;
	KCXP_LONG   WaitInterval;   /* unit: second */
	KCXP_LONG   MatchOptions;
	KCXP_CHAR   GroupStatus;	/* Flag indicating whether message
								   retrieved is in a group */
	KCXP_CHAR   SegmentStatus;	/* Flag indicating whether message
                                   retrieved is a segment of a logical
                                   message */
	KCXP_CHAR   Segmentation;	/* Flag indicating whether further
                                   segmentation is allowed for the message
                                   retrieved */
	KCXP_LONG   MsgToken;       /* Message token */
	KCXP_LONG   ReturnLength;   /* Length of message data returned(bytes) */
}KCXP_GMO;

/* structure identify */
# define  KCXP_GMO_STRUCT_ID  "GMO    "

/* structure identify (array form) */
# define  KCXP_GMO_STRUCT_ID_ARRAY  'G','M','O',' ',' ',' ',' ','\0'

# define  KCXP_GMO_VERSION  "1.0.0.0"

# define  KCXP_GMO_VERSION_ARRAY  '1','.','0','.','0','.','0','\0'

# define  KCXP_DEFAULT_GMO  {KCXP_GMO_STRUCT_ID_ARRAY},\
			    {KCXP_GMO_VERSION_ARRAY},\
			    0L,\
			    0L,\
			    0L,\
			    '\0',\
			    '\0',\
			    '\0',\
			    0L,\
			    0L

/*============================================================================*/
/* Define Encryption file head structure.                                     */
/*============================================================================*/
typedef struct tagKCXP_FILEHEAD
{
	KCXP_LONG   iFileID;       /* file id*/
	KCXP_INT    iNum;          /* num of unit */
	KCXP_LONG   iSize;         /* size of unit*/
	KCXP_CHAR32 strReserve;
}KCXP_FILEHEAD;

/* structure identify */
# define  KCXP_MD_STRUC_ID  "KCXP   "
/* structure identify (array form) */
# define  KCXP_MD_STRUC_ID_ARRAY  'K','C','X','P',' ',' ',' ','\0'
# define  KCXP_MD_VERSION  "1.0.0.0"
# define  KCXP_MD_VERSION_ARRAY  '1','.','0','.','0','.','0','\0'

#define KCXP_DEFAULT_MD  {KCXP_MD_STRUC_ID_ARRAY},\
	{KCXP_MD_VERSION_ARRAY},\
	0,\
	KCXP_MT_REQUEST,\
	20000,\
	0,\
	0,\
	0,\
	0,\
	5,\
	KCXP_NOPERSISTENCE_MESSAGE,\
	0,\
	0,\
	0,\
	0,\
	0,\
	0,\
	0,\
	0,\
	0,\
	0,\
	0,\
	0,\
	0,\
	0,\
	0,\
	0,\
	0,\
	0,\
	0,\
	0,\
	0,\
	0,\
	0,\
	0

/*============================================================================*/
/* KCXP_Close Call -- Close Object.                                           */
/*============================================================================*/
KCXP_VOID	WINAPI KCXP_Close (
	KCXP_HCONN 	Hconn,		/* input */
	KCXP_HOBJ 	*Hobj,		/* input/output */
	KCXP_LONG 	Options,	/* input */
	KCXP_LONG	*CompCode,	/* output */
	KCXP_LONG	*Reason);	/* output */


/*============================================================================*/
/* KCXP_Conn Call -- Connect Queue Manager.                                   */
/*============================================================================*/
KCXP_VOID 	WINAPI KCXP_Conn (
	KCXP_CHAR 	*QMgrName,		/* input */
	KCXP_HCONN 	*Hconn,			/* output */
	KCXP_LONG 	*CompCode,		/* output */
	KCXP_LONG 	*Reason);		/* output */


/*============================================================================*/
/* KCXP_Connx Call -- Connect Queue Manager with ip+port+userInformation.     */
/*============================================================================*/
KCXP_VOID 	WINAPI KCXP_Connx(
	KCXP_CHAR	*Ip,		/* input */
	KCXP_INT	Port,		/* input */
	KCXP_CHAR	*uName,		/* input */
	KCXP_CHAR	*uPass,		/* input */
	KCXP_HCONN	*Hconn,		/* output */
	KCXP_LONG	*CompCode,	/* output */
	KCXP_LONG	*Reason);	/* output */

/*============================================================================*/
/* KCXP_ConnEx -- Connect queue manager and Open an object.                   */
/*============================================================================*/
KCXP_VOID WINAPI KCXP_ConnEx (KCXP_CHAR  *QMgrName,
                       KCXP_OD    ObjDesc,
                       KCXP_LONG  Options,
                       KCXP_HCONN *Hconn,
                       KCXP_HOBJ  *Hobj,
                       KCXP_LONG  *CompCode,
                       KCXP_LONG  *Reason);

/*============================================================================*/
/* KCXP_Connect Call -- Connect QM with ip+port+proxy+ssl+userInformation.    */
/*============================================================================*/
KCXP_VOID WINAPI KCXP_Connect(KCXP_INT	 iProtocol,
							  KCXP_CHAR	 *Ip,
							  KCXP_INT	 Port,
							  KCXP_CHAR	 *uName,
							  KCXP_CHAR	 *uPass,
							  KCXP_CHAR   *ProxyURL,
							  KCXP_CHAR   *SSL,
							  KCXP_HCONN *Hconn,
							  KCXP_LONG	 *CompCode,
							  KCXP_LONG	 *Reason);

/*============================================================================*/
/* KCXP_Disconn Call -- Disconnect Queue Manager.                             */
/*============================================================================*/
KCXP_VOID	WINAPI KCXP_Disconn (
	KCXP_HCONN	*Hconn,		/* input */
	KCXP_LONG	Options,	/* input */
	KCXP_LONG	*CompCode,	/* output */
	KCXP_LONG	*Reason);	/* output */

/*============================================================================*/
/* KCXP_Get Call -- Get Message.                                              */
/*============================================================================*/
KCXP_VOID	WINAPI KCXP_Get (
	KCXP_HCONN	Hconn,			/* input */
	KCXP_HOBJ	Hobj,			/* input */
	KCXP_VOID	*pMsgDesc,		/* input/output */
	KCXP_VOID	*pGetMsgOpts,		/* input */
	KCXP_LONG	BuffLen,		/* input  */
	KCXP_CHAR	*Buffer,		/* output */
	KCXP_LONG	*DataLen,		/* output */
	KCXP_LONG	*CompCode,		/* output */
	KCXP_LONG	*Reason);		/* output */

KCXP_VOID	WINAPI KCXP_Get2 (
	KCXP_HCONN	Hconn,			/* input */
	KCXP_MD		*pMsgDesc,		/* input/output */
	KCXP_VOID	*pGetMsgOpts,		/* input */
	KCXP_LONG	BuffLen,		/* input  */
	KCXP_CHAR	*Buffer,		/* output */
	KCXP_LONG	*DataLen,		/* output */
	KCXP_LONG	*CompCode,		/* output */
	KCXP_LONG	*Reason);		/* output */

/*============================================================================*/
/* KCXP_Get Call -- Get Message and malloc buffer.                                              */
/*============================================================================*/

KCXP_VOID	WINAPI KCXP_GetM (
	KCXP_HCONN	Hconn,			/* input */
	KCXP_HOBJ	Hobj,			/* input */
	KCXP_VOID	*pMsgDesc,		/* input/output */
	KCXP_VOID	*pGetMsgOpts,	/* input */
	KCXP_CHAR	**Buffer,		/* output */
	KCXP_LONG	*DataLen,		/* output */
	KCXP_LONG	*CompCode,		/* output */
	KCXP_LONG	*Reason);		/* output */

/*============================================================================*/
/* KCXP_Inq Call -- Inquire Object Attributes.                                */
/*============================================================================*/
KCXP_VOID	WINAPI KCXP_Inq (
	KCXP_HCONN	Hconn,			/* input */
	KCXP_HOBJ	Hobj,			/* input */
	KCXP_LONG	SelectorCount,		/* input */
	KCXP_LONG	*pSelectors,		/* input */
	KCXP_LONG	*pIntAttrCount,		/* input */
	KCXP_LONG	*pIntAttrs,		/* output */
	KCXP_LONG	*pCharAttrLen,		/* input */
    KCXP_CHAR	pCharAttrs[][64],
	KCXP_LONG	*CompCode,		/* output */
	KCXP_LONG	*Reason);		/* output */


/*============================================================================*/
/* KCXP_Open Call -- Open Object.                                             */
/*============================================================================*/
KCXP_VOID	WINAPI KCXP_Open (
	KCXP_HCONN	Hconn,				/* input */
	KCXP_VOID   	*ObjDesc,			/* input/output */
	KCXP_LONG	Options,			/* input */
	KCXP_LONG	*Hobj,				/* output */
	KCXP_LONG	*CompCode,			/* output */
	KCXP_LONG	*Reason);			/* output */


/*============================================================================*/
/* KCXP_Put Call -- Put Message.                                              */
/*============================================================================*/
KCXP_VOID	WINAPI KCXP_Put (
	KCXP_HCONN	Hconn,			/* input */
	KCXP_HOBJ	Hobj,			/* input */
	KCXP_VOID	*pMsgDesc,		/* input/output */
	KCXP_VOID	*PutMsgOpts,            /* input/output */
	KCXP_LONG	BuffLen,		/* input */
	KCXP_VOID	*Buffer,		/* input =(KCXP_BYTE*BuffLen) */
	KCXP_LONG	*CompCode,		/* output */
	KCXP_LONG	*Reason);		/* output */


/*============================================================================*/
/* KCXP_Put Call -- Put Message.                                              */
/*============================================================================*/
KCXP_VOID	WINAPI KCXP_Put2 (
	KCXP_HCONN	Hconn,			/* input */
	KCXP_HOBJ	Hobj,			/* input */
	KCXP_VOID	*pMsgDesc,		/* input/output */
	KCXP_VOID	*PutMsgOpts,            /* input/output */
	KCXP_LONG	BuffLen,		/* input */
	KCXP_VOID	*Buffer,		/* input =(KCXP_BYTE*BuffLen) */
	KCXP_LONG	*CompCode,		/* output */
	KCXP_LONG	*Reason);		/* output */

/*============================================================================*/
/* KCXP_Put1 Call -- Put one Message.                                         */
/*============================================================================*/
KCXP_VOID	WINAPI KCXP_Put1 (
	KCXP_HCONN	Hconn,			/* input */
	KCXP_VOID	*pObjDesc,		/* input/output */
	KCXP_VOID 	*pMsgDesc,		/* input/output */
	KCXP_VOID	*pPutMsgOpts,   /* input/output */
	KCXP_LONG	BuffLen,		/* input */
	KCXP_VOID	*Buffer,		/* input =(KCXP_BYTE*BuffLen) */
	KCXP_LONG	*CompCode,		/* output */
	KCXP_LONG	*Reason);		/* output */

KCXP_VOID WINAPI KCXP_PutEx (KCXP_HCONN  Hconn,
							  KCXP_PGO    *pPGOpts,
							  KCXP_MD 	  *pPutMsgDesc,
							  KCXP_LONG	  PutBuffLen,
							  KCXP_VOID	  *pPutBuffer,
							  KCXP_MD     *pGetMsgDesc,
							  KCXP_LONG   GetBuffLen,
							  KCXP_CHAR   *pGetBuffer,
							  KCXP_LONG   *pGetDataLen,
							  KCXP_LONG	  *CompCode,
							  KCXP_LONG	  *Reason);

/*============================================================================*/
/* KCXP_Set Call -- Set Object Attributes.                                    */
/*============================================================================*/
KCXP_VOID	WINAPI KCXP_Set (
	KCXP_HCONN	Hconn,			/* input */
	KCXP_HOBJ	Hobj,			/* input */
	KCXP_LONG	SelectorCount,		/* input */
	KCXP_LONG	*pSelectors,		/* input */
	KCXP_LONG	IntAttrCount,		/* input */
	KCXP_LONG	*pIntAttrs,		/* input */
	KCXP_LONG	CharAttrLen,		/* input */
	KCXP_CHAR	pCharAttrs[][64],		/* input */
	KCXP_LONG	*CompCode,		/* output */
	KCXP_LONG	*Reason);		/* output */

KCXP_VOID WINAPI KCXP_BatDeleteMsg(KCXP_HCONN	Hconn,          /* input  */
								   KCXP_HOBJ	Hobj,			/* input  */
								   KCXP_VOID	*pGetMsgOpts,   /* input */
								   KCXP_CHAR	*strId,			/* msg id array, can be null, format: msgId1, msgId2, ... */
								   KCXP_LONG	BuffLen,        /* buf len */
								   KCXP_LONG	*CompCode,
								   KCXP_LONG	*Reason);


/*============================================================================*/
/* KCXP_FTPControl Call -- FTP Attributes.                                    */
/*============================================================================*/
KCXP_VOID  WINAPI KCXP_FTPControl(KCXP_HCONN Hconn,
								  KCXP_HOBJ  Hobj,
								  KCXP_HOBJ  HGetObj,    /* FTP Ans queue obj  */
								  KCXP_CHAR  *strReplyQ, /* FTP Req queue name */
								  KCXP_CHAR  *strUserID, /* FTP server ID */
								  KCXP_INT   iType,      /* FTP type*/
								  KCXP_INT   iTimeOut,   /* put and get timeout  */
								  KCXP_INT   iCommand,   /* FTP command */
								  KCXP_CHAR  *argv,      /* FTP command parm */
								  KCXP_INT   iDataLen,   /* RetBuff of size*/
								  KCXP_CHAR  *RetBuff,   /* ret data  */
								  KCXP_INT   *pDataLen,  /* ret data len */
								  KCXP_LONG  *CompCode,
								  KCXP_LONG  *Reason);

/*============================================================================*/
/* KCXP_ErrInfo Call -- Get Error Information with Reason.                    */
/*============================================================================*/
KCXP_VOID	WINAPI KCXP_ErrInfo (KCXP_LONG 	Reason,
								 KCXP_CHAR 	*EngLishErrInfo,
								 KCXP_CHAR  *ChineseErrInfo);

KCXP_VOID WINAPI KCXP_FreeBuffer(KCXP_VOID *buf);

# endif

//# ifdef   __cplusplus
  }
//# endif
