#include "stdafx.h"
#include "SocketApi.h"
#include "ClientContext.h"

LPFN_ACCEPTEX CSocketApi::AcceptEx = 0;
LPFN_GETACCEPTEXSOCKADDRS CSocketApi::GetAcceptExSockAddrs = 0;
LPFN_DISCONNECTEX CSocketApi::DisconnectEx = 0;

int CSocketApi::GetSocketApi(SOCKET Socket)
{
  // ʹ��AcceptEx��������Ϊ���������WinSock2�淶֮���΢�������ṩ����չ����
  // ������Ҫ�����ȡһ�º�����ָ�룬��ȡAcceptEx����ָ��
  DWORD dwBytes = 0;  

  // AcceptEx��GUID�����ڵ�������ָ��
  GUID GuidAcceptEx = WSAID_ACCEPTEX;  

  if(SOCKET_ERROR == ::WSAIoctl(
    Socket, 
    SIO_GET_EXTENSION_FUNCTION_POINTER, 
    &GuidAcceptEx, sizeof(GuidAcceptEx), 
    &AcceptEx, sizeof(AcceptEx), 
    &dwBytes, NULL, NULL))
  {
    IOCP_WriteLog( "WSAIoctlδ�ܻ�ȡAcceptEx����ָ��:" + CStringUtil::FormatLastError() );
    return IOCP_KO;
  }

  // GetAcceptExSockaddrs��GUID�����ڵ�������ָ��
  GUID GuidGetAcceptExSockAddrs = WSAID_GETACCEPTEXSOCKADDRS;

  // ��ȡGetAcceptExSockAddrs����ָ�룬Ҳ��ͬ��
  if(SOCKET_ERROR == ::WSAIoctl(
    Socket,
    SIO_GET_EXTENSION_FUNCTION_POINTER,
    &GuidGetAcceptExSockAddrs, sizeof(GuidGetAcceptExSockAddrs),
    &GetAcceptExSockAddrs, sizeof(GetAcceptExSockAddrs),
    &dwBytes, NULL, NULL))
  {
    IOCP_WriteLog( "WSAIoctlδ�ܻ�ȡGuidGetAcceptExSockAddrs����ָ��:" + CStringUtil::FormatLastError() );
    return IOCP_KO;
  }

  // GetAcceptExSockaddrs��GUID�����ڵ�������ָ��
  GUID GuidDisconnectEx = WSAID_DISCONNECTEX;

  if(SOCKET_ERROR == ::WSAIoctl(
    Socket,
    SIO_GET_EXTENSION_FUNCTION_POINTER,
    &GuidDisconnectEx, sizeof(GuidDisconnectEx),
    &DisconnectEx, sizeof(DisconnectEx),
    &dwBytes, NULL, NULL))
  {
    IOCP_WriteLog( "WSAIoctlδ�ܻ�ȡDisconnectEx����ָ��:" + CStringUtil::FormatLastError() );
    return IOCP_KO;
  }
  return IOCP_OK;
}
