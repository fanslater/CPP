#include "stdafx.h"
#include "SocketApi.h"
#include "ClientContext.h"

LPFN_ACCEPTEX CSocketApi::AcceptEx = 0;
LPFN_GETACCEPTEXSOCKADDRS CSocketApi::GetAcceptExSockAddrs = 0;
LPFN_DISCONNECTEX CSocketApi::DisconnectEx = 0;

int CSocketApi::GetSocketApi(SOCKET Socket)
{
  // 使用AcceptEx函数，因为这个是属于WinSock2规范之外的微软另外提供的扩展函数
  // 所以需要额外获取一下函数的指针，获取AcceptEx函数指针
  DWORD dwBytes = 0;  

  // AcceptEx的GUID，用于导出函数指针
  GUID GuidAcceptEx = WSAID_ACCEPTEX;  

  if(SOCKET_ERROR == ::WSAIoctl(
    Socket, 
    SIO_GET_EXTENSION_FUNCTION_POINTER, 
    &GuidAcceptEx, sizeof(GuidAcceptEx), 
    &AcceptEx, sizeof(AcceptEx), 
    &dwBytes, NULL, NULL))
  {
    IOCP_WriteLog( "WSAIoctl未能获取AcceptEx函数指针:" + CStringUtil::FormatLastError() );
    return IOCP_KO;
  }

  // GetAcceptExSockaddrs的GUID，用于导出函数指针
  GUID GuidGetAcceptExSockAddrs = WSAID_GETACCEPTEXSOCKADDRS;

  // 获取GetAcceptExSockAddrs函数指针，也是同理
  if(SOCKET_ERROR == ::WSAIoctl(
    Socket,
    SIO_GET_EXTENSION_FUNCTION_POINTER,
    &GuidGetAcceptExSockAddrs, sizeof(GuidGetAcceptExSockAddrs),
    &GetAcceptExSockAddrs, sizeof(GetAcceptExSockAddrs),
    &dwBytes, NULL, NULL))
  {
    IOCP_WriteLog( "WSAIoctl未能获取GuidGetAcceptExSockAddrs函数指针:" + CStringUtil::FormatLastError() );
    return IOCP_KO;
  }

  // GetAcceptExSockaddrs的GUID，用于导出函数指针
  GUID GuidDisconnectEx = WSAID_DISCONNECTEX;

  if(SOCKET_ERROR == ::WSAIoctl(
    Socket,
    SIO_GET_EXTENSION_FUNCTION_POINTER,
    &GuidDisconnectEx, sizeof(GuidDisconnectEx),
    &DisconnectEx, sizeof(DisconnectEx),
    &dwBytes, NULL, NULL))
  {
    IOCP_WriteLog( "WSAIoctl未能获取DisconnectEx函数指针:" + CStringUtil::FormatLastError() );
    return IOCP_KO;
  }
  return IOCP_OK;
}
