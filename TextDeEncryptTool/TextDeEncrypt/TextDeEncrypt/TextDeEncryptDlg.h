
// TextDeEncryptDlg.h : ͷ�ļ�
//

#pragma once


#define BUFFER_SIZE        4*1024


// CTextDeEncryptDlg �Ի���
class CTextDeEncryptDlg : public CDialog
{
    // ����
public:
    CTextDeEncryptDlg(CWnd *pParent = NULL);	// ��׼���캯��

    // �Ի�������
    enum { IDD = IDD_TEXTDEENCRYPT_DIALOG };

protected:
    virtual void DoDataExchange(CDataExchange *pDX);	// DDX/DDV ֧��


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
    afx_msg void OnDestroy();
    afx_msg void OnBnClickedButtonencryptl2r();
    afx_msg void OnBnClickedButtondecryptr2l();
    afx_msg void OnBnClickedButtondecryptl2r();
    afx_msg void OnBnClickedButtonencryptr2l();
    afx_msg void OnBnClickedCheck1();
    afx_msg void OnBnClickedButtonclean1();
    afx_msg void OnBnClickedButtonclean2();
};
