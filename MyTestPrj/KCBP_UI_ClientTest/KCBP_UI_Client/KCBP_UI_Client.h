
// KCBP_UI_Client.h : PROJECT_NAME Ӧ�ó������ͷ�ļ�
//

#pragma once

#ifndef __AFXWIN_H__
	#error "�ڰ������ļ�֮ǰ������stdafx.h�������� PCH �ļ�"
#endif

#include "resource.h"		// ������


// CKCBP_UI_ClientApp:
// �йش����ʵ�֣������ KCBP_UI_Client.cpp
//

class CKCBP_UI_ClientApp : public CWinAppEx
{
public:
	CKCBP_UI_ClientApp();

// ��д
	public:
	virtual BOOL InitInstance();

// ʵ��

	DECLARE_MESSAGE_MAP()
};

extern CKCBP_UI_ClientApp theApp;