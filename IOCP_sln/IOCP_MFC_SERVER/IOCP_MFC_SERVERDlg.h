
// IOCP_MFC_SERVERDlg.h : 头文件
//

#pragma once

#include <iostream>
#include <WinSock2.h>


typedef struct _PER_IO_CONTEXT{  
	OVERLAPPED   m_Overlapped;          // 每一个重叠I/O网络操作都要有一个                
	SOCKET       m_sockAccept;          // 这个I/O操作所使用的Socket，每个连接的都是一样的  
	WSABUF       m_wsaBuf;              // 存储数据的缓冲区，用来给重叠操作传递参数的，关于WSABUF后面还会讲  
	char         m_szBuffer[MAX_BUFFER_LEN]; // 对应WSABUF里的缓冲区  
	OPERATION_TYPE  m_OpType;               // 标志这个重叠I/O操作是做什么的，例如Accept/Recv等  

} PER_IO_CONTEXT, *PPER_IO_CONTEXT;


// CIOCP_MFC_SERVERDlg 对话框
class CIOCP_MFC_SERVERDlg : public CDialog
{
// 构造
public:
	CIOCP_MFC_SERVERDlg(CWnd* pParent = NULL);	// 标准构造函数

// 对话框数据
	enum { IDD = IDD_IOCP_MFC_SERVER_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 支持

public:
	void init();
	void uninit();


// 实现
protected:
	HICON m_hIcon;

	// 生成的消息映射函数
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnClose();
};
