/******************************************************************************

                  ��Ȩ���� (C), 2010-2020, �����н�֤�Ƽ��ɷ����޹�˾

 ******************************************************************************
  �� �� ��   : ClientContext.h
  �� �� ��   : ����
  ��    ��   : �ƹ�Ԫ
  ��������   : 2016��12��10��
  ����޸�   :
  ��������   : IO������
  �����б�   :
  �޸���ʷ   :
  1.��    ��   : 2016��12��10��
    ��    ��   : �ƹ�Ԫ
    �޸�����   : �����ļ�

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
  IOCP_RemoveClient = 2,   // ɾ��ClientContext
  IOCP_NewClient = 3,// ����ClientContext
};

#define IOCP_Verify(res) do {if ( (res) != IOCP_OK) return (res);}while(0)

// ��ʼ׼��Ͷ�ݵ�Accept���������
#define IOCP_Ready_Accept 10

// ͬʱͶ�ݵ�Accept���������(���Ҫ����ʵ�ʵ�����������)
#define IOCP_Max_Accept   (IOCP_Ready_Accept + 40)
#define IOCP_ExitCode -100

// ���������� (1024*8)
// ֮����Ϊʲô����8K��Ҳ��һ�������ϵľ���ֵ
// ���ȷʵ�ͻ��˷�����ÿ�����ݶ��Ƚ��٣���ô�����õ�СһЩ��ʡ�ڴ�
#define BUFFER_SIZE        8192

// Ĭ�϶˿�
#define DEFAULT_PORT          8888

// Ĭ��IP��ַ
#define DEFAULT_IP            "127.0.0.1"

// ����ɶ˿���Ͷ�ݵ�I/O����������
enum _OperationType
{  
  OP_ACCEPT, // ��־Ͷ�ݵ�Accept����
  OP_READ,   // ��־Ͷ�ݵ��Ƿ��Ͳ���
  OP_WRITE,  // ��־Ͷ�ݵ��ǽ��ղ���
  OP_DUMMY   // ���ڳ�ʼ��������
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

  // �����WSABUF�������ַ��Ļ�����
  char m_szBuffer[BUFFER_SIZE]; 

  // WSA���͵Ļ����������ڸ��ص�������������
  WSABUF m_WsaBuf;

  // ������������ʹ�õ�Socket
  SOCKET m_sktClient;

  // �ͻ��˵ĵ�ַ
  SOCKADDR_IN m_ClientAddr;

  // ��ʶ�������������(��Ӧ�����ö��)
  _OperationType m_OpType;

  // ÿһ���ص�����������ص��ṹ(���ÿһ��Socket��ÿһ����������Ҫ��һ��)
  OVERLAPPED m_Overlapped;

  long m_dwSendSequenceNumber;
};

//typedef SharedPtr<CClientContext> _ClientContextPtr;
typedef std::list<CClientContext*> _lstClientContext;
//typedef _lstClientContext::value_type _vtClientContextPtr;
typedef _lstClientContext::iterator _itClientContext;

#endif
