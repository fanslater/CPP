
// DaiKuan_CounterDlg.h : ͷ�ļ�
//

#pragma once
#include <iostream>
#include <cmath>
#include "afxcmn.h"
using namespace std;

struct GJJ_INPUT_INFO
{
    double dLoanAmount;
    double dRate;
    int iYearNums;
};


// CDaiKuan_CounterDlg �Ի���
class CDaiKuan_CounterDlg : public CDialogEx
{
    // ����
public:
    CDaiKuan_CounterDlg( CWnd* pParent = NULL ); // ��׼���캯��

    // �Ի�������
    enum { IDD = IDD_DAIKUAN_COUNTER_DIALOG };

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
    afx_msg void OnClose();
    afx_msg void OnBnClickedButtonClean();
    afx_msg void OnBnClickedButtonAcount();
    void initAll();
    void unInitAll();
    void cleanInput();
    void cleanOutput();
    void BenXiAcountFunc( GJJ_INPUT_INFO Info );
    void BenJinAcountFunc( GJJ_INPUT_INFO Info );
public:
    CListCtrl m_YueGongLIst;
};
