
// TextDeEncrypt.h : PROJECT_NAME Ӧ�ó������ͷ�ļ�
//

#pragma once

#ifndef __AFXWIN_H__
	#error "�ڰ������ļ�֮ǰ������stdafx.h�������� PCH �ļ�"
#endif

#include "resource.h"		// ������


// CTextDeEncryptApp:
// �йش����ʵ�֣������ TextDeEncrypt.cpp
//

class CTextDeEncryptApp : public CWinAppEx
{
public:
	CTextDeEncryptApp();

// ��д
	public:
	virtual BOOL InitInstance();

// ʵ��

	DECLARE_MESSAGE_MAP()
};

extern CTextDeEncryptApp theApp;