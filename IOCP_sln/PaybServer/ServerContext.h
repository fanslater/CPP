/******************************************************************************

                  版权所有 (C), 2010-2020, 深圳市金证科技股份有限公司

 ******************************************************************************
  文 件 名   : ServerContext.h
  版 本 号   : 初稿
  作    者   : 黄共元
  生成日期   : 2016年12月10日
  最近修改   :
  功能描述   : Socket上下文
  函数列表   :
  修改历史   :
  1.日    期   : 2016年12月10日
    作    者   : 黄共元
    修改内容   : 创建文件

******************************************************************************/
#ifndef __ServerContext_h__
#define __ServerContext_h__

#include "ClientContext.h"
#include <vector>
#if 0
class CServerContext
{
public:
  static CServerContext* CreateInstance();
  static void DestroyInstance();
  ~CServerContext();

  int Initialize(HANDLE hIocp, const tchar *pszAddr, int nPort);
  void RemoveClientContext(CClientContext* pClientContext);
  int HandleOpertion(CClientContext* pClientContext);

private:
  CServerContext();

private:
  static CServerContext* m_pInstance;
	SOCKET m_sktServer;

  // 每一个客户端连接的Socket
//  SOCKET m_Socket;

  // 客户端网络操作的上下文数据
  _lstClientContextPtr m_ClientContext;
  DeclearMutex(ClientContextMutex);
};

typedef SharedPtr<CServerContext> _SocketContextPtr;
typedef std::vector<_SocketContextPtr> _vcSocketContextPtr;
typedef _vcSocketContextPtr::value_type _vtSocketContextPtr;
#endif
#endif
