
// TiQianHuanKuan_Counter.h : PROJECT_NAME Ӧ�ó������ͷ�ļ�
//

#pragma once

#ifndef __AFXWIN_H__
	#error "�ڰ������ļ�֮ǰ������stdafx.h�������� PCH �ļ�"
#endif

#include "resource.h"		// ������


// CTiQianHuanKuan_CounterApp:
// �йش����ʵ�֣������ TiQianHuanKuan_Counter.cpp
//

class CTiQianHuanKuan_CounterApp : public CWinApp
{
public:
	CTiQianHuanKuan_CounterApp();

// ��д
public:
	virtual BOOL InitInstance();

// ʵ��

	DECLARE_MESSAGE_MAP()
};

extern CTiQianHuanKuan_CounterApp theApp;