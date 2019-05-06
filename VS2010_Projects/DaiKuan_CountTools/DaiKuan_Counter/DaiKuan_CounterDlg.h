
// DaiKuan_CounterDlg.h : 头文件
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


// CDaiKuan_CounterDlg 对话框
class CDaiKuan_CounterDlg : public CDialogEx
{
    // 构造
public:
    CDaiKuan_CounterDlg( CWnd* pParent = NULL ); // 标准构造函数

    // 对话框数据
    enum { IDD = IDD_DAIKUAN_COUNTER_DIALOG };

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
