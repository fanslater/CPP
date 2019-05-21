#ifndef __SocketApi_h__
#define __SocketApi_h__

#include <winsock2.h>
#include <MSWSock.h>

class CSocketApi
{
public:
  static int GetSocketApi(SOCKET Socket);

  // AcceptEx �� GetAcceptExSockaddrs �ĺ���ָ�룬���ڵ�����������չ����	
  static LPFN_ACCEPTEX AcceptEx;
  static LPFN_DISCONNECTEX DisconnectEx;
  static LPFN_GETACCEPTEXSOCKADDRS GetAcceptExSockAddrs;
private:
  CSocketApi();
};
#endif