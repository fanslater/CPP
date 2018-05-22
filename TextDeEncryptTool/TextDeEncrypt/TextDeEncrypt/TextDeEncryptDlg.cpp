
// TextDeEncryptDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "TextDeEncrypt.h"
#include "TextDeEncryptDlg.h"
#include "AesEncryptor.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// 用于应用程序“关于”菜单项的 CAboutDlg 对话框

class CAboutDlg : public CDialog
{
public:
    CAboutDlg();

    // 对话框数据
    enum { IDD = IDD_ABOUTBOX };

protected:
    virtual void DoDataExchange(CDataExchange *pDX);    // DDX/DDV 支持

    // 实现
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


// CTextDeEncryptDlg 对话框




CTextDeEncryptDlg::CTextDeEncryptDlg(CWnd *pParent /*=NULL*/)
    : CDialog(CTextDeEncryptDlg::IDD, pParent)
{
    //m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
    m_hIcon = AfxGetApp()->LoadIcon(IDI_ICON1);
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
    ON_BN_CLICKED(IDC_BUTTON_copy1, &CTextDeEncryptDlg::OnBnClickedButtoncopy1)
    ON_BN_CLICKED(IDC_BUTTON_copy2, &CTextDeEncryptDlg::OnBnClickedButtoncopy2)
    ON_BN_CLICKED(IDC_BUTTON_paste1, &CTextDeEncryptDlg::OnBnClickedButtonpaste1)
    ON_BN_CLICKED(IDC_BUTTON_paste2, &CTextDeEncryptDlg::OnBnClickedButtonpaste2)
END_MESSAGE_MAP()


// CTextDeEncryptDlg 消息处理程序

BOOL CTextDeEncryptDlg::OnInitDialog()
{
    CDialog::OnInitDialog();

    // 将“关于...”菜单项添加到系统菜单中。

    // IDM_ABOUTBOX 必须在系统命令范围内。
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

    // 设置此对话框的图标。当应用程序主窗口不是对话框时，框架将自动
    //  执行此操作
    SetIcon(m_hIcon, TRUE);			// 设置大图标
    SetIcon(m_hIcon, FALSE);		// 设置小图标

    // TODO: 在此添加额外的初始化代码

    return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
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

// 如果向对话框添加最小化按钮，则需要下面的代码
//  来绘制该图标。对于使用文档/视图模型的 MFC 应用程序，
//  这将由框架自动完成。

void CTextDeEncryptDlg::OnPaint()
{
    if (IsIconic())
    {
        CPaintDC dc(this); // 用于绘制的设备上下文

        SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

        // 使图标在工作区矩形中居中
        int cxIcon = GetSystemMetrics(SM_CXICON);
        int cyIcon = GetSystemMetrics(SM_CYICON);
        CRect rect;
        GetClientRect(&rect);
        int x = (rect.Width() - cxIcon + 1) / 2;
        int y = (rect.Height() - cyIcon + 1) / 2;

        // 绘制图标
        dc.DrawIcon(x, y, m_hIcon);
    }
    else
    {
        CDialog::OnPaint();
    }
}

//当用户拖动最小化窗口时系统调用此函数取得光标
//显示。
HCURSOR CTextDeEncryptDlg::OnQueryDragIcon()
{
    return static_cast<HCURSOR>(m_hIcon);
}


void CTextDeEncryptDlg::OnClose()
{
    // TODO: 在此添加消息处理程序代码和/或调用默认值

    CDialog::OnClose();
}

void CTextDeEncryptDlg::OnDestroy()
{
    CDialog::OnDestroy();

    // TODO: 在此处添加消息处理程序代码
}

void CTextDeEncryptDlg::OnBnClickedButtonencryptl2r()
{
    CString ctrKeyword;
    GetDlgItemText(IDC_EDIT_KEYWORD, ctrKeyword);
    CString ctrLeftText;
    GetDlgItemText(IDC_EDIT_TEXT1, ctrLeftText);
    AesEncryptor clsAes((unsigned char *)ctrKeyword.GetString());
    std::string strEncryptResult = clsAes.EncryptString(ctrLeftText.GetString());
    SetDlgItemText(IDC_EDIT_TEXT2, strEncryptResult.c_str());
}

void CTextDeEncryptDlg::OnBnClickedButtondecryptr2l()
{
    CString ctrKeyword;
    GetDlgItemText(IDC_EDIT_KEYWORD, ctrKeyword);
    CString ctrRigthText;
    GetDlgItemText(IDC_EDIT_TEXT2, ctrRigthText);
    AesEncryptor clsAes((unsigned char *)ctrKeyword.GetString());
    std::string strDecryptResult = clsAes.DecryptString(ctrRigthText.GetString());
    SetDlgItemText(IDC_EDIT_TEXT1, strDecryptResult.c_str());
}

void CTextDeEncryptDlg::OnBnClickedButtondecryptl2r()
{
    CString ctrKeyword;
    GetDlgItemText(IDC_EDIT_KEYWORD, ctrKeyword);
    CString ctrLeftText;
    GetDlgItemText(IDC_EDIT_TEXT1, ctrLeftText);
    AesEncryptor clsAes((unsigned char *)ctrKeyword.GetString());
    std::string strDecryptResult = clsAes.DecryptString(ctrLeftText.GetString());
    SetDlgItemText(IDC_EDIT_TEXT2, strDecryptResult.c_str());
}

void CTextDeEncryptDlg::OnBnClickedButtonencryptr2l()
{
    CString ctrKeyword;
    GetDlgItemText(IDC_EDIT_KEYWORD, ctrKeyword);
    CString ctrRightText;
    GetDlgItemText(IDC_EDIT_TEXT2, ctrRightText);
    AesEncryptor clsAes((unsigned char *)ctrKeyword.GetString());
    std::string strEncryptResult = clsAes.EncryptString(ctrRightText.GetString());
    SetDlgItemText(IDC_EDIT_TEXT1, strEncryptResult.c_str());
}

void CTextDeEncryptDlg::OnBnClickedCheck1()
{
    if (BST_CHECKED == ((CButton *)GetDlgItem(IDC_CHECK1))->GetCheck())
    {
        ((CEdit *)GetDlgItem(IDC_EDIT_KEYWORD))->SetPasswordChar(NULL);
    }
    else
    {
        ::SendMessageA(((CEdit *)GetDlgItem(IDC_EDIT_KEYWORD))->GetSafeHwnd(), EM_SETPASSWORDCHAR, '●', 0);
    }
    ((CEdit *)GetDlgItem(IDC_EDIT_KEYWORD))->RedrawWindow();
}

void CTextDeEncryptDlg::OnBnClickedButtonclean1()
{
    SetDlgItemText(IDC_EDIT_TEXT1, "");
}

void CTextDeEncryptDlg::OnBnClickedButtonclean2()
{
    // TODO: 在此添加控件通知处理程序代码
    SetDlgItemText(IDC_EDIT_TEXT2, "");
}

void CTextDeEncryptDlg::OnBnClickedButtoncopy1()
{
    CString ctrData;
    GetDlgItemText(IDC_EDIT_TEXT1, ctrData);
    if(OpenClipboard())
    {
        HGLOBAL clipbuffer;
        char *buffer;
        EmptyClipboard();
        clipbuffer = GlobalAlloc(GMEM_DDESHARE, ctrData.GetLength() + 1);
        buffer = (char *)GlobalLock(clipbuffer);
        strcpy_s(buffer, ctrData.GetLength() + 1, ctrData.GetString());
        GlobalUnlock(clipbuffer);
        SetClipboardData(CF_TEXT, clipbuffer);
        CloseClipboard();
    }
}

void CTextDeEncryptDlg::OnBnClickedButtoncopy2()
{
    CString ctrData;
    GetDlgItemText(IDC_EDIT_TEXT2, ctrData);
    if(OpenClipboard())
    {
        HGLOBAL clipbuffer;
        char *buffer;
        EmptyClipboard();
        clipbuffer = GlobalAlloc(GMEM_DDESHARE, ctrData.GetLength() + 1);
        buffer = (char *)GlobalLock(clipbuffer);
        strcpy_s(buffer, ctrData.GetLength() + 1, ctrData.GetString());
        GlobalUnlock(clipbuffer);
        SetClipboardData(CF_TEXT, clipbuffer);
        CloseClipboard();
    }
}

void CTextDeEncryptDlg::OnBnClickedButtonpaste1()
{
    CString ctrData;
    if ( OpenClipboard() )
    {
        HANDLE hData = GetClipboardData(CF_TEXT);
        char *buffer = (char *)GlobalLock(hData);
        ctrData = buffer;
        GlobalUnlock(hData);
        CloseClipboard();
    }
    SetDlgItemText(IDC_EDIT_TEXT1, ctrData);
}

void CTextDeEncryptDlg::OnBnClickedButtonpaste2()
{
    CString ctrData;
    if ( OpenClipboard() )
    {
        HANDLE hData = GetClipboardData(CF_TEXT);
        char *buffer = (char *)GlobalLock(hData);
        ctrData = buffer;
        GlobalUnlock(hData);
        CloseClipboard();
    }
    SetDlgItemText(IDC_EDIT_TEXT2, ctrData);
}
