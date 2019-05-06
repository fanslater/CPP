
// DaiKuan_CounterDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "DaiKuan_Counter.h"
#include "DaiKuan_CounterDlg.h"
#include "afxdialogex.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// 用于应用程序“关于”菜单项的 CAboutDlg 对话框

class CAboutDlg : public CDialogEx
{
public:
    CAboutDlg();

    // 对话框数据
    enum { IDD = IDD_ABOUTBOX };

protected:
    virtual void DoDataExchange( CDataExchange* pDX );  // DDX/DDV 支持

    // 实现
protected:
    DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialogEx( CAboutDlg::IDD )
{
}

void CAboutDlg::DoDataExchange( CDataExchange* pDX )
{
    CDialogEx::DoDataExchange( pDX );
}

BEGIN_MESSAGE_MAP( CAboutDlg, CDialogEx )
END_MESSAGE_MAP()


// CDaiKuan_CounterDlg 对话框




CDaiKuan_CounterDlg::CDaiKuan_CounterDlg( CWnd* pParent /*=NULL*/ )
    : CDialogEx( CDaiKuan_CounterDlg::IDD, pParent )
{
    m_hIcon = AfxGetApp()->LoadIcon( IDR_MAINFRAME );
}

void CDaiKuan_CounterDlg::DoDataExchange( CDataExchange* pDX )
{
    CDialogEx::DoDataExchange( pDX );
    DDX_Control( pDX, IDC_LIST_YueGongList, m_YueGongLIst );
}

BEGIN_MESSAGE_MAP( CDaiKuan_CounterDlg, CDialogEx )
    ON_WM_SYSCOMMAND()
    ON_WM_PAINT()
    ON_WM_QUERYDRAGICON()
    ON_WM_CLOSE()
    ON_BN_CLICKED( IDC_BUTTON_CLEAN, &CDaiKuan_CounterDlg::OnBnClickedButtonClean )
    ON_BN_CLICKED( IDC_BUTTON_ACOUNT, &CDaiKuan_CounterDlg::OnBnClickedButtonAcount )
END_MESSAGE_MAP()


// CDaiKuan_CounterDlg 消息处理程序

BOOL CDaiKuan_CounterDlg::OnInitDialog()
{
    CDialogEx::OnInitDialog();
    // 将“关于...”菜单项添加到系统菜单中。
    // IDM_ABOUTBOX 必须在系统命令范围内。
    ASSERT( ( IDM_ABOUTBOX & 0xFFF0 ) == IDM_ABOUTBOX );
    ASSERT( IDM_ABOUTBOX < 0xF000 );
    CMenu* pSysMenu = GetSystemMenu( FALSE );
    if( pSysMenu != NULL )
    {
        BOOL bNameValid;
        CString strAboutMenu;
        bNameValid = strAboutMenu.LoadString( IDS_ABOUTBOX );
        ASSERT( bNameValid );
        if( !strAboutMenu.IsEmpty() )
        {
            pSysMenu->AppendMenu( MF_SEPARATOR );
            pSysMenu->AppendMenu( MF_STRING, IDM_ABOUTBOX, strAboutMenu );
        }
    }
    // 设置此对话框的图标。当应用程序主窗口不是对话框时，框架将自动
    //  执行此操作
    SetIcon( m_hIcon, TRUE );       // 设置大图标
    SetIcon( m_hIcon, FALSE );      // 设置小图标
    // TODO: 在此添加额外的初始化代码
    initAll();
    return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

void CDaiKuan_CounterDlg::OnSysCommand( UINT nID, LPARAM lParam )
{
    if( ( nID & 0xFFF0 ) == IDM_ABOUTBOX )
    {
        CAboutDlg dlgAbout;
        dlgAbout.DoModal();
    }
    else
    {
        CDialogEx::OnSysCommand( nID, lParam );
    }
}

// 如果向对话框添加最小化按钮，则需要下面的代码
//  来绘制该图标。对于使用文档/视图模型的 MFC 应用程序，
//  这将由框架自动完成。

void CDaiKuan_CounterDlg::OnPaint()
{
    if( IsIconic() )
    {
        CPaintDC dc( this ); // 用于绘制的设备上下文
        SendMessage( WM_ICONERASEBKGND, reinterpret_cast<WPARAM>( dc.GetSafeHdc() ), 0 );
        // 使图标在工作区矩形中居中
        int cxIcon = GetSystemMetrics( SM_CXICON );
        int cyIcon = GetSystemMetrics( SM_CYICON );
        CRect rect;
        GetClientRect( &rect );
        int x = ( rect.Width() - cxIcon + 1 ) / 2;
        int y = ( rect.Height() - cyIcon + 1 ) / 2;
        // 绘制图标
        dc.DrawIcon( x, y, m_hIcon );
    }
    else
    {
        CDialogEx::OnPaint();
    }
}

//当用户拖动最小化窗口时系统调用此函数取得光标
//显示。
HCURSOR CDaiKuan_CounterDlg::OnQueryDragIcon()
{
    return static_cast<HCURSOR>( m_hIcon );
}

void CDaiKuan_CounterDlg::initAll()
{
    for( int i = 5; i <= 30; i += 5 )
    {
        CString showStr;
        showStr.Format( "%d年（%d期）", i, i * 12 );
        ( ( CComboBox* )GetDlgItem( IDC_COMBO_DaiKuanShiChang ) )->AddString( showStr );
    }
    ( ( CComboBox* )GetDlgItem( IDC_COMBO_DaiKuanShiChang ) )->SetCurSel( 5 );
    ( ( CButton* )GetDlgItem( IDC_RADIO_BenXi ) )->SetCheck( 1 );
    m_YueGongLIst.InsertColumn( 0, "月号", LVCFMT_LEFT, 100 );
    m_YueGongLIst.InsertColumn( 1, "月供", LVCFMT_LEFT, 200 );
}

void CDaiKuan_CounterDlg::unInitAll()
{
}

void CDaiKuan_CounterDlg::cleanInput()
{
    SetDlgItemText( IDC_EDIT_DaiKuanBenJin, "" );
    SetDlgItemText( IDC_EDIT_DaiKuanLiLv, "" );
    ( ( CComboBox* )GetDlgItem( IDC_COMBO_DaiKuanShiChang ) )->SetCurSel( 0 );
}

void CDaiKuan_CounterDlg::cleanOutput()
{
    SetDlgItemText( IDC_EDIT_DaiKuan, "" );
    SetDlgItemText( IDC_EDIT_DaiKuanYueShu, "" );
    SetDlgItemText( IDC_EDIT_HuanKuan, "" );
    SetDlgItemText( IDC_EDIT_LIXI, "" );
    m_YueGongLIst.DeleteAllItems();
}

void CDaiKuan_CounterDlg::BenXiAcountFunc( GJJ_INPUT_INFO Info )
{
    double dBenJin = Info.dLoanAmount * 10000;
    double dMonthRate = Info.dRate / 1200;
    int iMonthNum = Info.iYearNums * 12;
    //〔贷款本金×月利率×（1＋月利率）＾还款月数〕÷〔（1＋月利率）＾还款月数－1〕
    double dResult = ( dBenJin * dMonthRate * pow( dMonthRate + 1, iMonthNum ) ) / ( pow( dMonthRate + 1, iMonthNum ) - 1 ) ;
    CString showStr;
    showStr.Format( "%f（元）", dBenJin );
    SetDlgItemText( IDC_EDIT_DaiKuan, showStr );
    showStr.Format( "%d (月)", iMonthNum );
    SetDlgItemText( IDC_EDIT_DaiKuanYueShu, showStr );
    showStr.Format( "%f（元）", dResult * iMonthNum );
    SetDlgItemText( IDC_EDIT_HuanKuan, showStr );
    showStr.Format( "%f（元）", dResult * iMonthNum - dBenJin );
    SetDlgItemText( IDC_EDIT_LIXI, showStr );
    showStr.Format( "%f（元）", dResult );
    int iRow = m_YueGongLIst.InsertItem( 0, "平均每月" );
    m_YueGongLIst.SetItemText( iRow, 1, showStr );
}

void CDaiKuan_CounterDlg::BenJinAcountFunc( GJJ_INPUT_INFO Info )
{
    double dBenJin = Info.dLoanAmount * 10000;
    double dMonthRate = Info.dRate / 1200;
    int iMonthNum = Info.iYearNums * 12;
    //每月还款金额 = （贷款本金 ÷ 还款月数）+（本金 — 已归还本金累计额）×每月利率
    double dAvgCost = dBenJin / iMonthNum;
    double dSurplus = dBenJin;
    double dRepayment = 0.0;
    for( int i = 1; i <= iMonthNum; i ++ )
    {
        double dCurCost = dAvgCost + dSurplus * dMonthRate;
        dSurplus -= dAvgCost;
        dRepayment += dCurCost;
        CString tmpStr;
        tmpStr.Format( "第%d月", i );
        int iRow = m_YueGongLIst.InsertItem( m_YueGongLIst.GetItemCount(), tmpStr );
        tmpStr.Format( "%f（元）", dCurCost );
        m_YueGongLIst.SetItemText( iRow, 1, tmpStr );
    }
    CString showStr;
    showStr.Format( "%f（元）", dBenJin );
    SetDlgItemText( IDC_EDIT_DaiKuan, showStr );
    showStr.Format( "%d (月)", iMonthNum );
    SetDlgItemText( IDC_EDIT_DaiKuanYueShu, showStr );
    showStr.Format( "%f（元）", dRepayment );
    SetDlgItemText( IDC_EDIT_HuanKuan, showStr );
    showStr.Format( "%f（元）", dRepayment - dBenJin );
    SetDlgItemText( IDC_EDIT_LIXI, showStr );
}



void CDaiKuan_CounterDlg::OnClose()
{
    // TODO: 在此添加消息处理程序代码和/或调用默认值
    unInitAll();
    CDialogEx::OnClose();
}


void CDaiKuan_CounterDlg::OnBnClickedButtonClean()
{
    // TODO: 在此添加控件通知处理程序代码
    cleanInput();
    cleanOutput();
}


void CDaiKuan_CounterDlg::OnBnClickedButtonAcount()
{
    // TODO: 在此添加控件通知处理程序代码
    cleanOutput();
    CString csDaiKuanBenJin;
    GetDlgItemText( IDC_EDIT_DaiKuanBenJin, csDaiKuanBenJin );
    CString csDaiKuanLiLv;
    GetDlgItemText( IDC_EDIT_DaiKuanLiLv, csDaiKuanLiLv );
    int iCurSel = ( ( CComboBox* )GetDlgItem( IDC_COMBO_DaiKuanShiChang ) )->GetCurSel();
    GJJ_INPUT_INFO Info = { 0 };
    Info.dLoanAmount = atof( csDaiKuanBenJin.GetString() );
    Info.dRate = atof( csDaiKuanLiLv.GetString() );
    Info.iYearNums = iCurSel + 1;
    int iBenXi = ( ( CButton* )GetDlgItem( IDC_RADIO_BenXi ) )->GetCheck();
    int iBenJin = ( ( CButton* )GetDlgItem( IDC_RADIO_BenJin ) )->GetCheck();
    if( iBenXi == 1 && iBenJin == 0 )
    {
        BenXiAcountFunc( Info );
    }
    else if( iBenXi == 0 && iBenJin == 1 )
    {
        BenJinAcountFunc( Info );
    }
    else
    {
        AfxMessageBox( "计算类型异常！" );
    }
}
