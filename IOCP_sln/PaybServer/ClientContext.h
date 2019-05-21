/******************************************************************************

                  版权所有 (C), 2010-2020, 深圳市金证科技股份有限公司

 ******************************************************************************
  文 件 名   : ClientContext.h
  版 本 号   : 初稿
  作    者   : 黄共元
  生成日期   : 2016年12月10日
  最近修改   :
  功能描述   : IO上下文
  函数列表   :
  修改历史   :
  1.日    期   : 2016年12月10日
    作    者   : 黄共元
    修改内容   : 创建文件

******************************************************************************/
#ifndef __ClientContext_h__
#define __ClientContext_h__

#include "StringUtil.h"
#include <winsock2.h>
#include <MSWSock.h>
#include <list>
#include <boost/thread/recursive_mutex.hpp>
//#include "log4cplus.h"
#include "Public.h"

enum
{
  IOCP_KO = -1,
  IOCP_OK = 0,
  IOCP_RemoveClient = 2,   // 删除ClientContext
  IOCP_NewClient = 3,// 新增ClientContext
};

#define IOCP_Verify(res) do {if ( (res) != IOCP_OK) return (res);}while(0)

// 初始准备投递的Accept请求的数量
#define IOCP_Ready_Accept 10

// 同时投递的Accept请求的数量(这个要根据实际的情况灵活设置)
#define IOCP_Max_Accept   (IOCP_Ready_Accept + 40)
#define IOCP_ExitCode -100

// 缓冲区长度 (1024*8)
// 之所以为什么设置8K，也是一个江湖上的经验值
// 如果确实客户端发来的每组数据都比较少，那么就设置得小一些，省内存
#define BUFFER_SIZE        8192

// 默认端口
#define DEFAULT_PORT          8888

// 默认IP地址
#define DEFAULT_IP            "127.0.0.1"

// 在完成端口上投递的I/O操作的类型
enum _OperationType
{  
  OP_ACCEPT, // 标志投递的Accept操作
  OP_READ,   // 标志投递的是发送操作
  OP_WRITE,  // 标志投递的是接收操作
  OP_DUMMY   // 用于初始化无意义
};

class CClientContext
{
public:
  explicit CClientContext(SOCKET sktClient);
  ~CClientContext();
  friend CClientContext* GetClientContext(OVERLAPPED* pOverlapped);
  int PostAcceptEx(SOCKET hSocket);
  int HandleIocpError();
  bool NeedRemove(DWORD dwBytesTransfered);
  int HandleOpertion(HANDLE hIocp);
  bool IsTimeOut();
private:
  bool IsClientAlive();
  int DoAccpetEx(HANDLE hIocp);
  int PostRecv();
  int DoRecv();
  int PostSend();
  int DoSend();
private:

  // 这个是WSABUF里具体存字符的缓冲区
  char m_szBuffer[BUFFER_SIZE]; 

  // WSA类型的缓冲区，用于给重叠操作传参数的
  WSABUF m_WsaBuf;

  // 这个网络操作所使用的Socket
  SOCKET m_sktClient;

  // 客户端的地址
  SOCKADDR_IN m_ClientAddr;

  // 标识网络操作的类型(对应上面的枚举)
  _OperationType m_OpType;

  // 每一个重叠网络操作的重叠结构(针对每一个Socket的每一个操作，都要有一个)
  OVERLAPPED m_Overlapped;

  long m_dwSendSequenceNumber;
};

//typedef SharedPtr<CClientContext> _ClientContextPtr;
typedef std::list<CClientContext*> _lstClientContext;
//typedef _lstClientContext::value_type _vtClientContextPtr;
typedef _lstClientContext::iterator _itClientContext;

#endif
