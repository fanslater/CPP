
// KCBP_UI_ClientDlg.h : ͷ�ļ�
//

#pragma once

#include "comm_def.h"
#include "XmlIniReader.h"
#include "EditableListCtrl.h"
#include "afxwin.h"
#include "afxcmn.h"
#include "KcbpVisitor.h"

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
