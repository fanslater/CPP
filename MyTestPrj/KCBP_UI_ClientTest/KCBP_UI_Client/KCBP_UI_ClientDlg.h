
// KCBP_UI_ClientDlg.h : 头文件
//

#pragma once

#include "comm_def.h"
#include "XmlIniReader.h"
#include "EditableListCtrl.h"
#include "afxwin.h"
#include "afxcmn.h"
#include "KcbpVisitor.h"

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

private:
    CXmlIniReader m_clsCfg;
    CKcbpVisitor m_clsKcbp;
    lbmInfo m_liLBM;
    CString m_ctrDefaultParam;

public:
    void Init();
    void Uninit();
    void OpenConsole();
    void CloseConsole();
    void ShowError();
    void PrintTree( boost::property_tree::ptree root );
    void CallLbm( lbm_call_cfg& lbm );
    void AnalysisInput( const tstring input, kvmap& paramlist );
    void AnalysisParams( const tstring& strInput, Json::Value& jsParams );
    void ShowResultToView( const Json::Value& jsResultSet );
    void ModifyWindowsShow( BOOL bflag );

public:
    afx_msg void OnClose();
    CEditableListCtrl m_etlcResultSet;
    afx_msg void OnBnClickedButtonsaveconnectcfg();
    afx_msg void OnBnClickedButtonconnectsvr();
    afx_msg void OnBnClickedButtondisconnectsvr();
    CComboBox m_cbxLbmNo;
    afx_msg void OnCbnSelchangeComboLbmNo();
    afx_msg void OnBnClickedButtoncalllbm();
};
