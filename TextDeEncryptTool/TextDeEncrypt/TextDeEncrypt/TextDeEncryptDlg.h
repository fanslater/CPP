
// TextDeEncryptDlg.h : 头文件
//

#pragma once


#define BUFFER_SIZE        4*1024


// CTextDeEncryptDlg 对话框
class CTextDeEncryptDlg : public CDialog
{
    // 构造
public:
    CTextDeEncryptDlg(CWnd *pParent = NULL);	// 标准构造函数

    // 对话框数据
    enum { IDD = IDD_TEXTDEENCRYPT_DIALOG };

protected:
    virtual void DoDataExchange(CDataExchange *pDX);	// DDX/DDV 支持


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
    afx_msg void OnDestroy();
    afx_msg void OnBnClickedButtonencryptl2r();
    afx_msg void OnBnClickedButtondecryptr2l();
    afx_msg void OnBnClickedButtondecryptl2r();
    afx_msg void OnBnClickedButtonencryptr2l();
    afx_msg void OnBnClickedCheck1();
    afx_msg void OnBnClickedButtonclean1();
    afx_msg void OnBnClickedButtonclean2();
};
