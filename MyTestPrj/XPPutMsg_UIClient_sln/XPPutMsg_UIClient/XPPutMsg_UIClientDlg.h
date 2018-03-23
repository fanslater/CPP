
// XPPutMsg_UIClientDlg.h : ͷ�ļ�
//

#pragma once
#include <iostream>
#include <boost/lexical_cast.hpp>
#include <boost/thread/barrier.hpp> 
#include <boost/thread.hpp> 
#include "comm_def.h"
#include "XmlIniReader.h"
#include "commtools.h"
#include "afxwin.h"
#include "CPCQueue.hpp"



// CXPPutMsg_UIClientDlg �Ի���
class CXPPutMsg_UIClientDlg : public CDialog
{
// ����
public:
	CXPPutMsg_UIClientDlg(CWnd* pParent = NULL);	// ��׼���캯��

// �Ի�������
	enum { IDD = IDD_XPPUTMSG_UICLIENT_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV ֧��


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
	int m_iThreadNum;
	CXmlIniReader m_clsConfig;
	CCommTools m_clsKCXP;
	CPCQueue<std::string> m_clsCPCQueue;
	int m_iThreadRunFlag;					
	boost::thread_group m_ThreadPool;		
	boost::barrier* m_pBarrier;	
	CRITICAL_SECTION m_cs;
	kvStrMap mc;

public:
	int init();
	int uninit();
	int open_console();
	int close_console();
	void StartWorkThread();
	void StopWorkThread();
	int ThreadFunc();
	void PushData(const CString& data,const int iCount);

public:
	afx_msg void OnBnClickedButtonSenddata2kcxp();
	afx_msg void OnBnClickedButtonLoopsend2kcxp();
	afx_msg void OnBnClickedButtonrandomsend();
	
	// ��Ϣ���������б�
	CComboBox m_cbMsgTpye;
	afx_msg void OnClose();
	afx_msg void OnDestroy();
	afx_msg void OnCbnSelchangeComboMsgType();
	CComboBox m_cbSessionType;
	afx_msg void OnCbnSelchangeCombosessiontype();
	CComboBox m_cbChannelName;
	afx_msg void OnBnClickedButtonOpen();
	afx_msg void OnBnClickedButtonClose();
};
