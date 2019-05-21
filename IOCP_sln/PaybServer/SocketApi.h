#ifndef __SocketApi_h__
#define __SocketApi_h__

#include <winsock2.h>
#include <MSWSock.h>

class CSocketApi
{
public:
  static int GetSocketApi(SOCKET Socket);

  // AcceptEx 和 GetAcceptExSockaddrs 的函数指针，用于调用这两个扩展函数	
  static LPFN_ACCEPTEX AcceptEx;
  static LPFN_DISCONNECTEX DisconnectEx;
  static LPFN_GETACCEPTEXSOCKADDRS GetAcceptExSockAddrs;
private:
  CSocketApi();
};
#endif