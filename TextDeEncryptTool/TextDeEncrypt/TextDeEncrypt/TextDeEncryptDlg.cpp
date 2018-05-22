
// TextDeEncryptDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "TextDeEncrypt.h"
#include "TextDeEncryptDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// ����Ӧ�ó��򡰹��ڡ��˵���� CAboutDlg �Ի���

class CAboutDlg : public CDialog
{
public:
    CAboutDlg();

    // �Ի�������
    enum { IDD = IDD_ABOUTBOX };

protected:
    virtual void DoDataExchange(CDataExchange *pDX);    // DDX/DDV ֧��

    // ʵ��
protected:
    DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialog(CAboutDlg::IDD)
{
}

void CAboutDlg::DoDataExchange(CDataExchange *pDX)
{
    CDialog::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
END_MESSAGE_MAP()


// CTextDeEncryptDlg �Ի���




CTextDeEncryptDlg::CTextDeEncryptDlg(CWnd *pParent /*=NULL*/)
    : CDialog(CTextDeEncryptDlg::IDD, pParent)
{
    m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CTextDeEncryptDlg::DoDataExchange(CDataExchange *pDX)
{
    CDialog::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CTextDeEncryptDlg, CDialog)
    ON_WM_SYSCOMMAND()
    ON_WM_PAINT()
    ON_WM_QUERYDRAGICON()
    //}}AFX_MSG_MAP
    ON_WM_CLOSE()
    ON_WM_DESTROY()
    ON_BN_CLICKED(IDC_BUTTON_encrypt_l2r, &CTextDeEncryptDlg::OnBnClickedButtonencryptl2r)
    ON_BN_CLICKED(IDC_BUTTON_decrypt_r2l, &CTextDeEncryptDlg::OnBnClickedButtondecryptr2l)
    ON_BN_CLICKED(IDC_BUTTON_decrypt_l2r, &CTextDeEncryptDlg::OnBnClickedButtondecryptl2r)
    ON_BN_CLICKED(IDC_BUTTON_encrypt_r2l, &CTextDeEncryptDlg::OnBnClickedButtonencryptr2l)
    ON_BN_CLICKED(IDC_CHECK1, &CTextDeEncryptDlg::OnBnClickedCheck1)
    ON_BN_CLICKED(IDC_BUTTON_clean1, &CTextDeEncryptDlg::OnBnClickedButtonclean1)
    ON_BN_CLICKED(IDC_BUTTON_clean2, &CTextDeEncryptDlg::OnBnClickedButtonclean2)
END_MESSAGE_MAP()


// CTextDeEncryptDlg ��Ϣ�������

BOOL CTextDeEncryptDlg::OnInitDialog()
{
    CDialog::OnInitDialog();

    // ��������...���˵�����ӵ�ϵͳ�˵��С�

    // IDM_ABOUTBOX ������ϵͳ���Χ�ڡ�
    ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
    ASSERT(IDM_ABOUTBOX < 0xF000);

    CMenu *pSysMenu = GetSystemMenu(FALSE);
    if (pSysMenu != NULL)
    {
        BOOL bNameValid;
        CString strAboutMenu;
        bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
        ASSERT(bNameValid);
        if (!strAboutMenu.IsEmpty())
        {
            pSysMenu->AppendMenu(MF_SEPARATOR);
            pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
        }
    }

    // ���ô˶Ի����ͼ�ꡣ��Ӧ�ó��������ڲ��ǶԻ���ʱ����ܽ��Զ�
    //  ִ�д˲���
    SetIcon(m_hIcon, TRUE);			// ���ô�ͼ��
    SetIcon(m_hIcon, FALSE);		// ����Сͼ��

    // TODO: �ڴ���Ӷ���ĳ�ʼ������

    return TRUE;  // ���ǽ��������õ��ؼ������򷵻� TRUE
}

void CTextDeEncryptDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
    if ((nID & 0xFFF0) == IDM_ABOUTBOX)
    {
        CAboutDlg dlgAbout;
        dlgAbout.DoModal();
    }
    else
    {
        CDialog::OnSysCommand(nID, lParam);
    }
}

// �����Ի��������С����ť������Ҫ����Ĵ���
//  �����Ƹ�ͼ�ꡣ����ʹ���ĵ�/��ͼģ�͵� MFC Ӧ�ó���
//  �⽫�ɿ���Զ���ɡ�

void CTextDeEncryptDlg::OnPaint()
{
    if (IsIconic())
    {
        CPaintDC dc(this); // ���ڻ��Ƶ��豸������

        SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

        // ʹͼ���ڹ����������о���
        int cxIcon = GetSystemMetrics(SM_CXICON);
        int cyIcon = GetSystemMetrics(SM_CYICON);
        CRect rect;
        GetClientRect(&rect);
        int x = (rect.Width() - cxIcon + 1) / 2;
        int y = (rect.Height() - cyIcon + 1) / 2;

        // ����ͼ��
        dc.DrawIcon(x, y, m_hIcon);
    }
    else
    {
        CDialog::OnPaint();
    }
}

//���û��϶���С������ʱϵͳ���ô˺���ȡ�ù��
//��ʾ��
HCURSOR CTextDeEncryptDlg::OnQueryDragIcon()
{
    return static_cast<HCURSOR>(m_hIcon);
}


void CTextDeEncryptDlg::OnClose()
{
    // TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ

    CDialog::OnClose();
}

void CTextDeEncryptDlg::OnDestroy()
{
    CDialog::OnDestroy();

    // TODO: �ڴ˴������Ϣ����������
}

void CTextDeEncryptDlg::OnBnClickedButtonencryptl2r()
{
    CString ctrKeyword;
    GetDlgItemText(IDC_EDIT_KEYWORD, ctrKeyword);
    __int64 i64Keyword = atol(ctrKeyword.GetString());
    CString ctrLeftText;
    GetDlgItemText(IDC_EDIT_TEXT1, ctrLeftText);
    char szEncryptResult[BUFFER_SIZE] = {0};
    AES_Encrypt1(szEncryptResult, sizeof(szEncryptResult), i64Keyword, ctrLeftText.GetString());
    SetDlgItemText(IDC_EDIT_TEXT2, szEncryptResult);
}

void CTextDeEncryptDlg::OnBnClickedButtondecryptr2l()
{
    CString ctrKeyword;
    GetDlgItemText(IDC_EDIT_KEYWORD, ctrKeyword);
    __int64 i64Keyword = atol(ctrKeyword.GetString());
    CString ctrRigthText;
    GetDlgItemText(IDC_EDIT_TEXT2, ctrRigthText);
    char szDecryptResult[BUFFER_SIZE] = {0};
    AES_Decrypt1(szDecryptResult, sizeof(szDecryptResult), i64Keyword, ctrRigthText.GetString());
    SetDlgItemText(IDC_EDIT_TEXT1, szDecryptResult);
}

void CTextDeEncryptDlg::OnBnClickedButtondecryptl2r()
{
    CString ctrKeyword;
    GetDlgItemText(IDC_EDIT_KEYWORD, ctrKeyword);
    __int64 i64Keyword = atol(ctrKeyword.GetString());
    CString ctrLeftText;
    GetDlgItemText(IDC_EDIT_TEXT1, ctrLeftText);
    char szDecryptResult[BUFFER_SIZE] = {0};
    AES_Decrypt1(szDecryptResult, sizeof(szDecryptResult), i64Keyword, ctrLeftText.GetString());
    SetDlgItemText(IDC_EDIT_TEXT2, szDecryptResult);
}

void CTextDeEncryptDlg::OnBnClickedButtonencryptr2l()
{
    CString ctrKeyword;
    GetDlgItemText(IDC_EDIT_KEYWORD, ctrKeyword);
    __int64 i64Keyword = atol(ctrKeyword.GetString());
    CString ctrRightText;
    GetDlgItemText(IDC_EDIT_TEXT2, ctrRightText);
    char szEncryptResult[BUFFER_SIZE] = {0};
    AES_Encrypt1(szEncryptResult, sizeof(szEncryptResult), i64Keyword, ctrRightText.GetString());
    SetDlgItemText(IDC_EDIT_TEXT1, szEncryptResult);
}

void CTextDeEncryptDlg::OnBnClickedCheck1()
{
    if (BST_CHECKED == ((CButton *)GetDlgItem(IDC_CHECK1))->GetCheck())
    {
        ((CEdit *)GetDlgItem(IDC_EDIT_KEYWORD))->SetPasswordChar(NULL);
    }
    else
    {
        ::SendMessageA(((CEdit *)GetDlgItem(IDC_EDIT_KEYWORD))->GetSafeHwnd(), EM_SETPASSWORDCHAR, '��', 0);
    }
    ((CEdit *)GetDlgItem(IDC_EDIT_KEYWORD))->RedrawWindow();
}

void CTextDeEncryptDlg::OnBnClickedButtonclean1()
{
    SetDlgItemText(IDC_EDIT_TEXT1,"");
}

void CTextDeEncryptDlg::OnBnClickedButtonclean2()
{
    // TODO: �ڴ���ӿؼ�֪ͨ����������
    SetDlgItemText(IDC_EDIT_TEXT2,"");
}
