/******************************************************************************

                  ��Ȩ���� (C), 2010-2020, �����н�֤�Ƽ��ɷ����޹�˾

 ******************************************************************************
  �� �� ��   : ServerContext.h
  �� �� ��   : ����
  ��    ��   : �ƹ�Ԫ
  ��������   : 2016��12��10��
  ����޸�   :
  ��������   : Socket������
  �����б�   :
  �޸���ʷ   :
  1.��    ��   : 2016��12��10��
    ��    ��   : �ƹ�Ԫ
    �޸�����   : �����ļ�

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

  // ÿһ���ͻ������ӵ�Socket
//  SOCKET m_Socket;

  // �ͻ����������������������
  _lstClientContextPtr m_ClientContext;
  DeclearMutex(ClientContextMutex);
};

typedef SharedPtr<CServerContext> _SocketContextPtr;
typedef std::vector<_SocketContextPtr> _vcSocketContextPtr;
typedef _vcSocketContextPtr::value_type _vtSocketContextPtr;
#endif
#endif
