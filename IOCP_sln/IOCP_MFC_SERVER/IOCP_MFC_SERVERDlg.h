
// IOCP_MFC_SERVERDlg.h : ͷ�ļ�
//

#pragma once

#include <iostream>
#include <WinSock2.h>


typedef struct _PER_IO_CONTEXT{  
	OVERLAPPED   m_Overlapped;          // ÿһ���ص�I/O���������Ҫ��һ��                
	SOCKET       m_sockAccept;          // ���I/O������ʹ�õ�Socket��ÿ�����ӵĶ���һ����  
	WSABUF       m_wsaBuf;              // �洢���ݵĻ��������������ص��������ݲ����ģ�����WSABUF���滹�ὲ  
	char         m_szBuffer[MAX_BUFFER_LEN]; // ��ӦWSABUF��Ļ�����  
	OPERATION_TYPE  m_OpType;               // ��־����ص�I/O��������ʲô�ģ�����Accept/Recv��  

} PER_IO_CONTEXT, *PPER_IO_CONTEXT;


// CIOCP_MFC_SERVERDlg �Ի���
class CIOCP_MFC_SERVERDlg : public CDialog
{
// ����
public:
	CIOCP_MFC_SERVERDlg(CWnd* pParent = NULL);	// ��׼���캯��

// �Ի�������
	enum { IDD = IDD_IOCP_MFC_SERVER_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV ֧��

public:
	void init();
	void uninit();


// ʵ��
protected:
	HICON m_hIcon;

	// ���ɵ���Ϣӳ�亯��
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnClose();
};
