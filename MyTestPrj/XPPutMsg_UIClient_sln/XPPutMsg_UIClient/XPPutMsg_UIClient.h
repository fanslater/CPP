
// XPPutMsg_UIClient.h : PROJECT_NAME Ӧ�ó������ͷ�ļ�
//

#pragma once

#ifndef __AFXWIN_H__
	#error "�ڰ������ļ�֮ǰ������stdafx.h�������� PCH �ļ�"
#endif

#include "resource.h"		// ������


// CXPPutMsg_UIClientApp:
// �йش����ʵ�֣������ XPPutMsg_UIClient.cpp
//

class CXPPutMsg_UIClientApp : public CWinAppEx
{
public:
	CXPPutMsg_UIClientApp();

// ��д
	public:
	virtual BOOL InitInstance();

// ʵ��

	DECLARE_MESSAGE_MAP()
};

extern CXPPutMsg_UIClientApp theApp;