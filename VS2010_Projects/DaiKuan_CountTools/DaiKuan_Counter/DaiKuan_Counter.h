
// DaiKuan_Counter.h : PROJECT_NAME Ӧ�ó������ͷ�ļ�
//

#pragma once

#ifndef __AFXWIN_H__
	#error "�ڰ������ļ�֮ǰ������stdafx.h�������� PCH �ļ�"
#endif

#include "resource.h"		// ������


// CDaiKuan_CounterApp:
// �йش����ʵ�֣������ DaiKuan_Counter.cpp
//

class CDaiKuan_CounterApp : public CWinApp
{
public:
	CDaiKuan_CounterApp();

// ��д
public:
	virtual BOOL InitInstance();

// ʵ��

	DECLARE_MESSAGE_MAP()
};

extern CDaiKuan_CounterApp theApp;