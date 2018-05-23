
// TextDeEncryptDlg.h : ͷ�ļ�
//

#pragma once


#define BUFFER_SIZE        1024
#define SLICE_SIZE         127


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
    afx_msg void OnBnClickedButtoncopy1();
    afx_msg void OnBnClickedButtoncopy2();
    afx_msg void OnBnClickedButtonpaste1();
    afx_msg void OnBnClickedButtonpaste2();    
private:
    int Encrypt(CString ctrKey, CString ctrData, CString& ctrResult);
    int Decrypt(CString ctrKey, CString ctrData, CString& ctrResult);
    void Byte2Hex(const unsigned char *src, int len, char *dest);
    void Hex2Byte(const char *src, int len, unsigned char *dest);
    int  Char2Int(char c);
};
