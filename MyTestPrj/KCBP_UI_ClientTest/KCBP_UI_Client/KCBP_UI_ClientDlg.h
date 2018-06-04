
// KCBP_UI_ClientDlg.h : ͷ�ļ�
//

#pragma once

#include "comm_def.h"
#include "XmlIniReader.h"
#include "EditableListCtrl.h"
#include "KCBPCli.h"
#include "afxwin.h"
#include "afxcmn.h"

// CKCBP_UI_ClientDlg �Ի���
class CKCBP_UI_ClientDlg : public CDialog
{
    // ����
public:
    CKCBP_UI_ClientDlg( CWnd* pParent = NULL ); // ��׼���캯��

    // �Ի�������
    enum { IDD = IDD_KCBP_UI_CLIENT_DIALOG };

protected:
    virtual void DoDataExchange( CDataExchange* pDX );  // DDX/DDV ֧��


    // ʵ��
protected:
    HICON m_hIcon;

    // ���ɵ���Ϣӳ�亯��
    virtual BOOL OnInitDialog();
    afx_msg void OnSysCommand( UINT nID, LPARAM lParam );
    afx_msg void OnPaint();
    afx_msg HCURSOR OnQueryDragIcon();
    DECLARE_MESSAGE_MAP()

public:
    CXmlIniReader m_clsCfg;
    KCBPCLIHANDLE m_kcbp_handle;
    lbmInfo m_lbm_map;
    CString m_ctrDefaultParam;

public:
    void init();
    void uninit();
    void open_console();
    void close_console();
    void showerror();
    void printTree( boost::property_tree::ptree root );
    void call_lbm( lbm_call_cfg& lbm );
    void AnalysisInput( const string input, kvmap& paramlist );
    string& trim( string& s );
    void ShowLBMResult();
    void modifyWindowsShow( BOOL bflag );

public:
    afx_msg void OnClose();
    CEditableListCtrl m_list_ctrl_result_set;
    afx_msg void OnBnClickedButtonsaveconnectcfg();
    afx_msg void OnBnClickedButtonconnectsvr();
    afx_msg void OnBnClickedButtondisconnectsvr();
    CComboBox m_cbx_lbm_no;
    afx_msg void OnCbnSelchangeComboLbmNo();
    afx_msg void OnBnClickedButtoncalllbm();
    CTabCtrl m_tab;
};
