
// IOCP_MFC_SERVER.h : PROJECT_NAME 应用程序的主头文件
//

#pragma once

#ifndef __AFXWIN_H__
	#error "在包含此文件之前包含“stdafx.h”以生成 PCH 文件"
#endif

#include "resource.h"		// 主符号


// CIOCP_MFC_SERVERApp:
// 有关此类的实现，请参阅 IOCP_MFC_SERVER.cpp
//

class CIOCP_MFC_SERVERApp : public CWinAppEx
{
public:
	CIOCP_MFC_SERVERApp();

// 重写
	public:
	virtual BOOL InitInstance();

// 实现

	DECLARE_MESSAGE_MAP()
};

extern CIOCP_MFC_SERVERApp theApp;