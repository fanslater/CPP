#ifndef __IocpTeset_h__
#define __IocpTeset_h__

#include "StringUtil.h"
#include "SyncIocp.h"
#include <map>

class CIocpTest
{
public:
  CIocpTest();
  ~CIocpTest();
  int Initialize(const char* pszIp, int nPort);
  int ThreadResponse();
protected:
private:
  int ReadAllMessage();
  const tstring& GetTestMessage(const tstring& strMsgId) const;

  static tstring m_strEmpty;
  std::map<tstring, tstring> m_mpMsg;

  CIocpServ  m_clsIocpServ;
  int m_nRunningThread;
};
#endif