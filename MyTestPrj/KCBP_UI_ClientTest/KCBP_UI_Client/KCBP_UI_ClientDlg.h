
// KCBP_UI_ClientDlg.h : 头文件
//

#pragma once

#include "comm_def.h"
#include "XmlIniReader.h"
#include "EditableListCtrl.h"
#include "KCBPCli.h"
#include "afxwin.h"
#include "afxcmn.h"

// CKCBP_UI_ClientDlg 对话框
class CKCBP_UI_ClientDlg : public CDialog
{
    // 构造
public:
    CKCBP_UI_ClientDlg( CWnd* pParent = NULL ); // 标准构造函数

    // 对话框数据
    enum { IDD = IDD_KCBP_UI_CLIENT_DIALOG };

protected:
    virtual void DoDataExchange( CDataExchange* pDX );  // DDX/DDV 支持


    // 实现
protected:
    HICON m_hIcon;

    // 生成的消息映射函数
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
