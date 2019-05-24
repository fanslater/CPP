
// TiQianHuanKuan_CounterDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "TiQianHuanKuan_Counter.h"
#include "TiQianHuanKuan_CounterDlg.h"
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


// CTiQianHuanKuan_CounterDlg 对话框




CTiQianHuanKuan_CounterDlg::CTiQianHuanKuan_CounterDlg( CWnd* pParent /*=NULL*/ )
    : CDialogEx( CTiQianHuanKuan_CounterDlg::IDD, pParent )
{
    m_hIcon = AfxGetApp()->LoadIcon( IDR_MAINFRAME );
}

void CTiQianHuanKuan_CounterDlg::DoDataExchange( CDataExchange* pDX )
{
    CDialogEx::DoDataExchange( pDX );
    DDX_Control( pDX, IDC_LIST_YueGongJinELieBiao, m_clsListCtrl_YueGongJinE );
}

BEGIN_MESSAGE_MAP( CTiQianHuanKuan_CounterDlg, CDialogEx )
    ON_WM_SYSCOMMAND()
    ON_WM_PAINT()
    ON_WM_QUERYDRAGICON()
    ON_WM_CLOSE()
    ON_BN_CLICKED( IDC_BUTTON_StartWork, &CTiQianHuanKuan_CounterDlg::OnBnClickedButtonStartwork )
    ON_BN_CLICKED( IDC_BUTTON_Clean, &CTiQianHuanKuan_CounterDlg::OnBnClickedButtonClean )
END_MESSAGE_MAP()


// CTiQianHuanKuan_CounterDlg 消息处理程序

BOOL CTiQianHuanKuan_CounterDlg::OnInitDialog()
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
    InitAll();
    return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

void CTiQianHuanKuan_CounterDlg::OnSysCommand( UINT nID, LPARAM lParam )
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

void CTiQianHuanKuan_CounterDlg::OnPaint()
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
HCURSOR CTiQianHuanKuan_CounterDlg::OnQueryDragIcon()
{
    return static_cast<HCURSOR>( m_hIcon );
}



void CTiQianHuanKuan_CounterDlg::OnClose()
{
    // TODO: 在此添加消息处理程序代码和/或调用默认值
    UninitAll();
    CDialogEx::OnClose();
}

void CTiQianHuanKuan_CounterDlg::InitAll()
{
    ( ( CButton* )GetDlgItem( IDC_RADIO_DengEBenXi ) )->SetCheck( 1 );
    SetDlgItemText( IDC_EDIT_DaiKuanZongE, "80" );
    for( int i = 1; i <= 6; i ++ )
    {
        CString showStr;
        showStr.Format( "%d年（%d期）", i * 5, i * 60 );
        ( ( CComboBox* )GetDlgItem( IDC_COMBO_DaiKuanQiXian ) )->AddString( showStr );
    }
    ( ( CComboBox* )GetDlgItem( IDC_COMBO_DaiKuanQiXian ) )->SetCurSel( 5 );
    SetDlgItemText( IDC_EDIT_DaiKuanLiLv, "4.655" );
    SYSTEMTIME stFirstTime = {0};
    stFirstTime.wYear = 2016;
    stFirstTime.wMonth = 5;
    stFirstTime.wDay = 21;
    ( ( CDateTimeCtrl* )GetDlgItem( IDC_DATETIMEPICKER_ShouCiHuanKuanRiQi ) )->SetTime( &stFirstTime );
    SetDlgItemText( IDC_EDIT_TiQianHuanKuanJinE, "20" );
    ( ( CComboBox* )GetDlgItem( IDC_COMBO_ChuLiFangShi ) )->AddString( "月供不变，减少还款期限" );
    ( ( CComboBox* )GetDlgItem( IDC_COMBO_ChuLiFangShi ) )->AddString( "减少月供，还款期限不变" );
    ( ( CComboBox* )GetDlgItem( IDC_COMBO_ChuLiFangShi ) )->SetCurSel( 1 );
    m_clsListCtrl_YueGongJinE.InsertColumn( 0, "月号", LVCFMT_LEFT, 100 );
    m_clsListCtrl_YueGongJinE.InsertColumn( 1, "原月供", LVCFMT_LEFT, 150 );
    m_clsListCtrl_YueGongJinE.InsertColumn( 2, "新月供", LVCFMT_LEFT, 150 );
}

void CTiQianHuanKuan_CounterDlg::UninitAll()
{
}


void CTiQianHuanKuan_CounterDlg::CleanInput()
{
    ( ( CButton* )GetDlgItem( IDC_RADIO_DengEBenXi ) )->SetCheck( 1 );
    SetDlgItemText( IDC_EDIT_DaiKuanZongE, "" );
    ( ( CComboBox* )GetDlgItem( IDC_COMBO_DaiKuanQiXian ) )->SetCurSel( 5 );
    SetDlgItemText( IDC_EDIT_DaiKuanLiLv, "" );
    SetDlgItemText( IDC_EDIT_TiQianHuanKuanJinE, "" );
    ( ( CComboBox* )GetDlgItem( IDC_COMBO_ChuLiFangShi ) )->SetCurSel( 1 );
    SYSTEMTIME stSysTime;
    GetLocalTime( &stSysTime );
    ( ( CDateTimeCtrl* )GetDlgItem( IDC_DATETIMEPICKER_ShouCiHuanKuanRiQi ) )->SetTime( &stSysTime );
    ( ( CDateTimeCtrl* )GetDlgItem( IDC_DATETIMEPICKER_TiQianHuanKuanRiQi ) )->SetTime( &stSysTime );
}

void CTiQianHuanKuan_CounterDlg::CleanOutput()
{
    SetDlgItemText( IDC_EDIT_ShengYuHuanKuanYueShu, "" );
    SetDlgItemText( IDC_EDIT_YiHuanBenXiZongE, "" );
    SetDlgItemText( IDC_EDIT_YiHuanLiXiZongE, "" );
    SetDlgItemText( IDC_EDIT_YiHuanBenJinZongE, "" );
    SetDlgItemText( IDC_EDIT_ShengYuBenJinZongE, "" );
    SetDlgItemText( IDC_EDIT_YuanDaiKuanZongE, "" );
    SetDlgItemText( IDC_EDIT_YuanHuanKuanZongE, "" );
    SetDlgItemText( IDC_EDIT_YuanZhiFuLiXiZongE, "" );
    SetDlgItemText( IDC_EDIT_XinDaiKuanZongE, "" );
    SetDlgItemText( IDC_EDIT_XinHuanKuanZongE, "" );
    SetDlgItemText( IDC_EDIT_XinZhiFuLiXiZongE, "" );
    SetDlgItemText( IDC_EDIT_JieShengLiXiZhiChuZongE, "" );
    m_clsListCtrl_YueGongJinE.DeleteAllItems();
}

int CTiQianHuanKuan_CounterDlg::JiSuanDangQianQiShu( const SYSTEMTIME stFirst, const SYSTEMTIME stSecond )
{
    COleDateTime dTimeF( stFirst );
    COleDateTime dTimeS( stSecond );
    COleDateTimeSpan dTimeSpan = dTimeS - dTimeF;
    int iSpanDays = ( int )dTimeSpan.GetTotalDays();
    return ( iSpanDays / 30 ) + 1;
}

void CTiQianHuanKuan_CounterDlg::DaiKuanCounter_BenXi( const CountInfo stInfo, DaiKuanResult& stResult )
{
    if( stInfo.dDaiKuanZongE < 0.00 )
    {
        return;
    }
    double dMonthRate = stInfo.dDaiKuanLiLv / 12;
    //每月还款金额 =〔贷款本金×月利率×（1＋月利率）＾还款月数〕÷〔（1＋月利率）＾还款月数－1〕
    stResult.dYueGong = ( stInfo.dDaiKuanZongE * dMonthRate * pow( dMonthRate + 1, stInfo.iDaiKuanQiXian ) ) / ( pow( dMonthRate + 1, stInfo.iDaiKuanQiXian ) - 1 ) ;
    stResult.dDaiKuanZongE = stInfo.dDaiKuanZongE;
    stResult.dHuanKuanZongE = stResult.dYueGong * stInfo.iDaiKuanQiXian;
    stResult.dZhiFuLiXi = stResult.dHuanKuanZongE - stResult.dDaiKuanZongE;
    stResult.iDaiKuanYueShu = stInfo.iDaiKuanQiXian;
    stResult.dLiLv = stInfo.dDaiKuanLiLv;
}

void CTiQianHuanKuan_CounterDlg::DaiKuanCounter_BenJin( const CountInfo stInfo, DaiKuanResult& stResult )
{
    if( stInfo.dDaiKuanZongE < 0.00 )
    {
        return;
    }
    double dMonthRate = stInfo.dDaiKuanLiLv / 12;
    //每月还款金额 = （贷款本金 ÷ 还款月数）+（本金 — 已归还本金累计额）× 月利率
    //首次还款金额 = （贷款本金 ÷ 还款月数）+ 本金 × 月利率
    //每次还款递减金额 = （贷款本金 ÷ 还款月数）× 月利率
    stResult.dYueGong = ( stInfo.dDaiKuanZongE / stInfo.iDaiKuanQiXian ) + stInfo.dDaiKuanZongE * dMonthRate;
    stResult.dYueGondDiJianE = ( stInfo.dDaiKuanZongE / stInfo.iDaiKuanQiXian ) * dMonthRate;
    stResult.dDaiKuanZongE = stInfo.dDaiKuanZongE;
    stResult.iDaiKuanYueShu = stInfo.iDaiKuanQiXian;
    //还款总额 = (首次月供 x 还款月数) - ((月供递减金额 x 还款月数 x (还款月数-1)) ÷ 2)
    stResult.dHuanKuanZongE = ( stResult.dYueGong * stResult.iDaiKuanYueShu ) - ( ( stResult.dYueGondDiJianE * stResult.iDaiKuanYueShu * ( stResult.iDaiKuanYueShu - 1 ) ) / 2 );
    stResult.dZhiFuLiXi = stResult.dHuanKuanZongE - stResult.dDaiKuanZongE;
    stResult.dLiLv = stInfo.dDaiKuanLiLv;
}

void CTiQianHuanKuan_CounterDlg::TiQianHuanKuanCounter_BenXi_JianYueGong( const CountInfo stInfo, DaiKuanResult& stResult )
{
    DaiKuanResult stOldResult;
    DaiKuanCounter_BenXi( stInfo, stOldResult );
    double dMonthRate = stInfo.dDaiKuanLiLv / 12;
    stResult.ps_dYuanYueGong = stOldResult.dYueGong;
    //
    stResult.ps_dYiHuanBenJinLiXi = stOldResult.dYueGong * stInfo.iDangQianQiShu;
    //本息当前已还利息 = (本金*月利率-月供)*((月利率+1)^(当前期数)-1)/月利率 + 当前期数*月供
    stResult.ps_dYiHuanLiXi = ( stInfo.dDaiKuanZongE * dMonthRate - stOldResult.dYueGong ) * ( pow( dMonthRate + 1, stInfo.iDangQianQiShu ) - 1 ) / dMonthRate + stInfo.iDangQianQiShu * stOldResult.dYueGong;
    stResult.ps_dYiHuanBenJin = stResult.ps_dYiHuanBenJinLiXi - stResult.ps_dYiHuanLiXi;
    stResult.ps_dShengYuBenJin = stInfo.dDaiKuanZongE - stResult.ps_dYiHuanBenJin;
    //
    stResult.ps_dYuanDaiKuanZongE = stOldResult.dDaiKuanZongE;
    stResult.ps_dYuanHuanKuanZongE = stOldResult.dHuanKuanZongE;
    stResult.ps_dYuanZhiFuLiXi = stOldResult.dZhiFuLiXi;
    CountInfo stNewInfo = {0};
    stNewInfo.dDaiKuanZongE = stResult.ps_dShengYuBenJin - stInfo.dTiQianHuanKuanJinE;
    stNewInfo.dDaiKuanLiLv = stInfo.dDaiKuanLiLv;
    stNewInfo.iDaiKuanQiXian = stInfo.iDaiKuanQiXian - stInfo.iDangQianQiShu;
    DaiKuanCounter_BenXi( stNewInfo, stResult );
}

void CTiQianHuanKuan_CounterDlg::TiQianHuanKuanCounter_BenXi_JianQiXian( const CountInfo stInfo, DaiKuanResult& stResult )
{
    DaiKuanResult stOldResult;
    DaiKuanCounter_BenXi( stInfo, stOldResult );
    double dMonthRate = stInfo.dDaiKuanLiLv / 12;
    stResult.ps_dYuanYueGong = stOldResult.dYueGong;
    //
    stResult.ps_dYiHuanBenJinLiXi = stOldResult.dYueGong * stInfo.iDangQianQiShu;
    //本息当前已还利息 = (本金*月利率-月供)*((月利率+1)^(当前期数)-1)/月利率 + 当前期数*月供
    stResult.ps_dYiHuanLiXi = ( stInfo.dDaiKuanZongE * dMonthRate - stOldResult.dYueGong ) * ( pow( dMonthRate + 1, stInfo.iDangQianQiShu ) - 1 ) / dMonthRate + stInfo.iDangQianQiShu * stOldResult.dYueGong;
    stResult.ps_dYiHuanBenJin = stResult.ps_dYiHuanBenJinLiXi - stResult.ps_dYiHuanLiXi;
    stResult.ps_dShengYuBenJin = stInfo.dDaiKuanZongE - stResult.ps_dYiHuanBenJin;
    //
    stResult.ps_dYuanDaiKuanZongE = stOldResult.dDaiKuanZongE;
    stResult.ps_dYuanHuanKuanZongE = stOldResult.dHuanKuanZongE;
    stResult.ps_dYuanZhiFuLiXi = stOldResult.dZhiFuLiXi;
    CountInfo stNewInfo = {0};
    stNewInfo.dDaiKuanZongE = stResult.ps_dShengYuBenJin - stInfo.dTiQianHuanKuanJinE;
    stNewInfo.dDaiKuanLiLv = stInfo.dDaiKuanLiLv;
    stNewInfo.iDaiKuanQiXian = FindApproximateQiXian_BenXi( stOldResult.dYueGong, stNewInfo.dDaiKuanZongE, ( stNewInfo.dDaiKuanLiLv / 12 ) );
    DaiKuanCounter_BenXi( stNewInfo, stResult );
}

void CTiQianHuanKuan_CounterDlg::TiQianHuanKuanCounter_BenJin_JianYueGong( const CountInfo stInfo, DaiKuanResult& stResult )
{
    DaiKuanResult stOldResult = {0};
    DaiKuanCounter_BenJin( stInfo, stOldResult );
    double dMonthRate = stInfo.dDaiKuanLiLv / 12;
    stResult.ps_dYuanYueGong = stOldResult.dYueGong;
    stResult.ps_dYuanYueGondDiJianE = stOldResult.dYueGondDiJianE;
    //
    stResult.ps_dYiHuanBenJin = stOldResult.dDaiKuanZongE / stOldResult.iDaiKuanYueShu * stInfo.iDangQianQiShu;
    //本金当前已还总额 = (首次月供 x 当前期数) - ((月供递减金额 x 当前期数 x (当前期数-1) ÷ 2)
    stResult.ps_dYiHuanBenJinLiXi = ( stOldResult.dYueGong *  stInfo.iDangQianQiShu ) - ( ( stOldResult.dYueGondDiJianE * stInfo.iDangQianQiShu * ( stInfo.iDangQianQiShu - 1 ) ) / 2 );
    stResult.ps_dYiHuanLiXi = stResult.ps_dYiHuanBenJinLiXi - stResult.ps_dYiHuanBenJin;
    stResult.ps_dShengYuBenJin = stOldResult.dDaiKuanZongE - stResult.ps_dYiHuanBenJin;
    //
    stResult.ps_dYuanDaiKuanZongE = stOldResult.dDaiKuanZongE;
    stResult.ps_dYuanHuanKuanZongE = stOldResult.dHuanKuanZongE;
    stResult.ps_dYuanZhiFuLiXi = stOldResult.dZhiFuLiXi;
    CountInfo stNewInfo = {0};
    stNewInfo.dDaiKuanZongE = stResult.ps_dShengYuBenJin - stInfo.dTiQianHuanKuanJinE;
    stNewInfo.dDaiKuanLiLv = stInfo.dDaiKuanLiLv;
    stNewInfo.iDaiKuanQiXian = stInfo.iDaiKuanQiXian - stInfo.iDangQianQiShu;
    DaiKuanCounter_BenJin( stNewInfo, stResult );
}

void CTiQianHuanKuan_CounterDlg::TiQianHuanKuanCounter_BenJin_JianQiXian( const CountInfo stInfo, DaiKuanResult& stResult )
{
    DaiKuanResult stOldResult = {0};
    DaiKuanCounter_BenJin( stInfo, stOldResult );
    double dMonthRate = stInfo.dDaiKuanLiLv / 12;
    stResult.ps_dYuanYueGong = stOldResult.dYueGong;
    stResult.ps_dYuanYueGondDiJianE = stOldResult.dYueGondDiJianE;
    //
    stResult.ps_dYiHuanBenJin = stOldResult.dDaiKuanZongE / stOldResult.iDaiKuanYueShu * stInfo.iDangQianQiShu;
    //本金当前已还总额 = (首次月供 x 当前期数) - ((月供递减金额 x 当前期数 x (当前期数-1) ÷ 2)
    stResult.ps_dYiHuanBenJinLiXi = ( stOldResult.dYueGong *  stInfo.iDangQianQiShu ) - ( ( stOldResult.dYueGondDiJianE * stInfo.iDangQianQiShu * ( stInfo.iDangQianQiShu - 1 ) ) / 2 );
    stResult.ps_dYiHuanLiXi = stResult.ps_dYiHuanBenJinLiXi - stResult.ps_dYiHuanBenJin;
    stResult.ps_dShengYuBenJin = stOldResult.dDaiKuanZongE - stResult.ps_dYiHuanBenJin;
    //
    stResult.ps_dYuanDaiKuanZongE = stOldResult.dDaiKuanZongE;
    stResult.ps_dYuanHuanKuanZongE = stOldResult.dHuanKuanZongE;
    stResult.ps_dYuanZhiFuLiXi = stOldResult.dZhiFuLiXi;
    CountInfo stNewInfo = {0};
    stNewInfo.dDaiKuanZongE = stResult.ps_dShengYuBenJin - stInfo.dTiQianHuanKuanJinE;
    stNewInfo.dDaiKuanLiLv = stInfo.dDaiKuanLiLv;
    stNewInfo.iDaiKuanQiXian = FindApproximateQiXian_BenJin( stOldResult.dYueGong, stNewInfo.dDaiKuanZongE, stNewInfo.dDaiKuanLiLv / 12 );
    DaiKuanCounter_BenJin( stNewInfo, stResult );
}

int CTiQianHuanKuan_CounterDlg::FindApproximateQiXian_BenXi( double Y, double B, double A )
{
    // 还款月数 = log(月供 / (月供 - 贷款本金 x 月利率)) / log(1 + 月利率)
    double X = log( Y / ( Y - B * A ) ) / log( 1 + A );
    return ( int )X + 1;
}

int CTiQianHuanKuan_CounterDlg::FindApproximateQiXian_BenJin( double Y, double B, double A )
{
    // 还款月数 = 本金 ÷ (首次还款金额 - 本金 × 月利率)
    double X = B / ( Y - B * A );
    return ( int )X + 1;
}

void CTiQianHuanKuan_CounterDlg::ShowResults( const DaiKuanResult& stResult )
{
    CString ctrTmp;
    ctrTmp.Format( "%d(月)", stResult.iDaiKuanYueShu );
    SetDlgItemText( IDC_EDIT_ShengYuHuanKuanYueShu, ctrTmp );
    ctrTmp.Format( "%lf(元)", stResult.ps_dYiHuanBenJinLiXi );
    SetDlgItemText( IDC_EDIT_YiHuanBenXiZongE, ctrTmp );
    ctrTmp.Format( "%lf(元)", stResult.ps_dYiHuanLiXi );
    SetDlgItemText( IDC_EDIT_YiHuanLiXiZongE, ctrTmp );
    ctrTmp.Format( "%lf(元)", stResult.ps_dYiHuanBenJin );
    SetDlgItemText( IDC_EDIT_YiHuanBenJinZongE, ctrTmp );
    ctrTmp.Format( "%lf(元)", stResult.ps_dShengYuBenJin );
    SetDlgItemText( IDC_EDIT_ShengYuBenJinZongE, ctrTmp );
    ctrTmp.Format( "%lf(元)", stResult.ps_dYuanDaiKuanZongE );
    SetDlgItemText( IDC_EDIT_YuanDaiKuanZongE, ctrTmp );
    ctrTmp.Format( "%lf(元)", stResult.ps_dYuanHuanKuanZongE );
    SetDlgItemText( IDC_EDIT_YuanHuanKuanZongE, ctrTmp );
    ctrTmp.Format( "%lf(元)", stResult.ps_dYuanZhiFuLiXi );
    SetDlgItemText( IDC_EDIT_YuanZhiFuLiXiZongE, ctrTmp );
    ctrTmp.Format( "%lf(元)", stResult.dDaiKuanZongE );
    SetDlgItemText( IDC_EDIT_XinDaiKuanZongE, ctrTmp );
    ctrTmp.Format( "%lf(元)", stResult.dHuanKuanZongE );
    SetDlgItemText( IDC_EDIT_XinHuanKuanZongE, ctrTmp );
    ctrTmp.Format( "%lf(元)", stResult.dZhiFuLiXi );
    SetDlgItemText( IDC_EDIT_XinZhiFuLiXiZongE, ctrTmp );
    ctrTmp.Format( "%lf(元)", stResult.ps_dYuanZhiFuLiXi - stResult.ps_dYiHuanLiXi - stResult.dZhiFuLiXi );//节省利息=原支付利息-已还利息-新支付利息
    SetDlgItemText( IDC_EDIT_JieShengLiXiZhiChuZongE, ctrTmp );
    if( stResult.dYueGondDiJianE > 0.00 )   //等额本金
    {
        for( int i = 1 ; i <= stResult.iDaiKuanYueShu ; ++i )
        {
            ctrTmp.Format( "第%d月", i );
            int iRow = m_clsListCtrl_YueGongJinE.InsertItem( m_clsListCtrl_YueGongJinE.GetItemCount(), ctrTmp );
            ctrTmp.Format( "%lf(元)", stResult.ps_dYuanYueGong - stResult.ps_dYuanYueGondDiJianE * ( i - 1 ) );
            m_clsListCtrl_YueGongJinE.SetItemText( iRow, 1, ctrTmp );
            ctrTmp.Format( "%lf(元)", stResult.dYueGong - stResult.dYueGondDiJianE * ( i - 1 ) );
            m_clsListCtrl_YueGongJinE.SetItemText( iRow, 2, ctrTmp );
        }
    }
    else    //等额本息
    {
        int iRow = m_clsListCtrl_YueGongJinE.InsertItem( 0, "平均每月" );
        ctrTmp.Format( "%lf(元)", stResult.ps_dYuanYueGong );
        m_clsListCtrl_YueGongJinE.SetItemText( iRow, 1, ctrTmp );
        ctrTmp.Format( "%lf(元)", stResult.dYueGong );
        m_clsListCtrl_YueGongJinE.SetItemText( iRow, 2, ctrTmp );
    }
}

void CTiQianHuanKuan_CounterDlg::OnBnClickedButtonStartwork()
{
    CleanOutput();
    CountInfo stInfo = {0};
    CString ctrTmp;
    GetDlgItemText( IDC_EDIT_DaiKuanZongE, ctrTmp );
    stInfo.dDaiKuanZongE = atof( ctrTmp.GetString() ) * 10000;
    stInfo.iDaiKuanQiXian = ( ( ( ( CComboBox* )GetDlgItem( IDC_COMBO_DaiKuanQiXian ) )->GetCurSel() + 1 ) * 5 ) * 12;
    GetDlgItemText( IDC_EDIT_DaiKuanLiLv, ctrTmp );
    stInfo.dDaiKuanLiLv = atof( ctrTmp.GetString() ) / 100 ;
    SYSTEMTIME stShouCiHuanKuanRiQi, stTiQianHuanKuanRiQi;
    ( ( CDateTimeCtrl* )GetDlgItem( IDC_DATETIMEPICKER_ShouCiHuanKuanRiQi ) )->GetTime( &stShouCiHuanKuanRiQi );
    ( ( CDateTimeCtrl* )GetDlgItem( IDC_DATETIMEPICKER_TiQianHuanKuanRiQi ) )->GetTime( &stTiQianHuanKuanRiQi );
    stInfo.iDangQianQiShu = JiSuanDangQianQiShu( stShouCiHuanKuanRiQi, stTiQianHuanKuanRiQi );
    GetDlgItemText( IDC_EDIT_TiQianHuanKuanJinE, ctrTmp );
    stInfo.dTiQianHuanKuanJinE = atof( ctrTmp.GetString() ) * 10000;
    int iFlagBenXi = ( ( CButton* )GetDlgItem( IDC_RADIO_DengEBenXi ) )->GetCheck();
    int iFlagBenJin = ( ( CButton* )GetDlgItem( IDC_RADIO_DengEBenJin ) )->GetCheck();
    int iFLagChuLiFangShi = ( ( CComboBox* )GetDlgItem( IDC_COMBO_ChuLiFangShi ) )->GetCurSel();
    DaiKuanResult stResult = {0};
    if( iFlagBenXi == 1 )
    {
        if( iFLagChuLiFangShi == 1 )
        {
            TiQianHuanKuanCounter_BenXi_JianYueGong( stInfo, stResult );
        }
        else
        {
            TiQianHuanKuanCounter_BenXi_JianQiXian( stInfo, stResult );
        }
    }
    else if( iFlagBenJin == 1 )
    {
        if( iFLagChuLiFangShi == 1 )
        {
            TiQianHuanKuanCounter_BenJin_JianYueGong( stInfo, stResult );
        }
        else
        {
            TiQianHuanKuanCounter_BenJin_JianQiXian( stInfo, stResult );
        }
    }
    else
    {
        AfxMessageBox( "还款方式选项异常！" );
    }
    ShowResults( stResult );
}


void CTiQianHuanKuan_CounterDlg::OnBnClickedButtonClean()
{
    CleanInput();
    CleanOutput();
}

