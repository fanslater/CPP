
// IOCP_MFC_SERVER.h : PROJECT_NAME Ӧ�ó������ͷ�ļ�
//

#pragma once

#ifndef __AFXWIN_H__
	#error "�ڰ������ļ�֮ǰ������stdafx.h�������� PCH �ļ�"
#endif

#include "resource.h"		// ������


// CIOCP_MFC_SERVERApp:
// �йش����ʵ�֣������ IOCP_MFC_SERVER.cpp
//

class CIOCP_MFC_SERVERApp : public CWinAppEx
{
public:
	CIOCP_MFC_SERVERApp();

// ��д
	public:
	virtual BOOL InitInstance();

// ʵ��

	DECLARE_MESSAGE_MAP()
};

extern CIOCP_MFC_SERVERApp theApp;