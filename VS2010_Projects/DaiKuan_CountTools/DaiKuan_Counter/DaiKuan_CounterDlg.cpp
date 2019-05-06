
// DaiKuan_CounterDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "DaiKuan_Counter.h"
#include "DaiKuan_CounterDlg.h"
#include "afxdialogex.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// ����Ӧ�ó��򡰹��ڡ��˵���� CAboutDlg �Ի���

class CAboutDlg : public CDialogEx
{
public:
    CAboutDlg();

    // �Ի�������
    enum { IDD = IDD_ABOUTBOX };

protected:
    virtual void DoDataExchange( CDataExchange* pDX );  // DDX/DDV ֧��

    // ʵ��
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


// CDaiKuan_CounterDlg �Ի���




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


// CDaiKuan_CounterDlg ��Ϣ�������

BOOL CDaiKuan_CounterDlg::OnInitDialog()
{
    CDialogEx::OnInitDialog();
    // ��������...���˵�����ӵ�ϵͳ�˵��С�
    // IDM_ABOUTBOX ������ϵͳ���Χ�ڡ�
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
    // ���ô˶Ի����ͼ�ꡣ��Ӧ�ó��������ڲ��ǶԻ���ʱ����ܽ��Զ�
    //  ִ�д˲���
    SetIcon( m_hIcon, TRUE );       // ���ô�ͼ��
    SetIcon( m_hIcon, FALSE );      // ����Сͼ��
    // TODO: �ڴ���Ӷ���ĳ�ʼ������
    initAll();
    return TRUE;  // ���ǽ��������õ��ؼ������򷵻� TRUE
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

// �����Ի��������С����ť������Ҫ����Ĵ���
//  �����Ƹ�ͼ�ꡣ����ʹ���ĵ�/��ͼģ�͵� MFC Ӧ�ó���
//  �⽫�ɿ���Զ���ɡ�

void CDaiKuan_CounterDlg::OnPaint()
{
    if( IsIconic() )
    {
        CPaintDC dc( this ); // ���ڻ��Ƶ��豸������
        SendMessage( WM_ICONERASEBKGND, reinterpret_cast<WPARAM>( dc.GetSafeHdc() ), 0 );
        // ʹͼ���ڹ����������о���
        int cxIcon = GetSystemMetrics( SM_CXICON );
        int cyIcon = GetSystemMetrics( SM_CYICON );
        CRect rect;
        GetClientRect( &rect );
        int x = ( rect.Width() - cxIcon + 1 ) / 2;
        int y = ( rect.Height() - cyIcon + 1 ) / 2;
        // ����ͼ��
        dc.DrawIcon( x, y, m_hIcon );
    }
    else
    {
        CDialogEx::OnPaint();
    }
}

//���û��϶���С������ʱϵͳ���ô˺���ȡ�ù��
//��ʾ��
HCURSOR CDaiKuan_CounterDlg::OnQueryDragIcon()
{
    return static_cast<HCURSOR>( m_hIcon );
}

void CDaiKuan_CounterDlg::initAll()
{
    for( int i = 5; i <= 30; i += 5 )
    {
        CString showStr;
        showStr.Format( "%d�꣨%d�ڣ�", i, i * 12 );
        ( ( CComboBox* )GetDlgItem( IDC_COMBO_DaiKuanShiChang ) )->AddString( showStr );
    }
    ( ( CComboBox* )GetDlgItem( IDC_COMBO_DaiKuanShiChang ) )->SetCurSel( 5 );
    ( ( CButton* )GetDlgItem( IDC_RADIO_BenXi ) )->SetCheck( 1 );
    m_YueGongLIst.InsertColumn( 0, "�º�", LVCFMT_LEFT, 100 );
    m_YueGongLIst.InsertColumn( 1, "�¹�", LVCFMT_LEFT, 200 );
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
    //�������������ʡ���1�������ʣ��޻����������¡���1�������ʣ��޻���������1��
    double dResult = ( dBenJin * dMonthRate * pow( dMonthRate + 1, iMonthNum ) ) / ( pow( dMonthRate + 1, iMonthNum ) - 1 ) ;
    CString showStr;
    showStr.Format( "%f��Ԫ��", dBenJin );
    SetDlgItemText( IDC_EDIT_DaiKuan, showStr );
    showStr.Format( "%d (��)", iMonthNum );
    SetDlgItemText( IDC_EDIT_DaiKuanYueShu, showStr );
    showStr.Format( "%f��Ԫ��", dResult * iMonthNum );
    SetDlgItemText( IDC_EDIT_HuanKuan, showStr );
    showStr.Format( "%f��Ԫ��", dResult * iMonthNum - dBenJin );
    SetDlgItemText( IDC_EDIT_LIXI, showStr );
    showStr.Format( "%f��Ԫ��", dResult );
    int iRow = m_YueGongLIst.InsertItem( 0, "ƽ��ÿ��" );
    m_YueGongLIst.SetItemText( iRow, 1, showStr );
}

void CDaiKuan_CounterDlg::BenJinAcountFunc( GJJ_INPUT_INFO Info )
{
    double dBenJin = Info.dLoanAmount * 10000;
    double dMonthRate = Info.dRate / 1200;
    int iMonthNum = Info.iYearNums * 12;
    //ÿ�»����� = ������� �� ����������+������ �� �ѹ黹�����ۼƶ��ÿ������
    double dAvgCost = dBenJin / iMonthNum;
    double dSurplus = dBenJin;
    double dRepayment = 0.0;
    for( int i = 1; i <= iMonthNum; i ++ )
    {
        double dCurCost = dAvgCost + dSurplus * dMonthRate;
        dSurplus -= dAvgCost;
        dRepayment += dCurCost;
        CString tmpStr;
        tmpStr.Format( "��%d��", i );
        int iRow = m_YueGongLIst.InsertItem( m_YueGongLIst.GetItemCount(), tmpStr );
        tmpStr.Format( "%f��Ԫ��", dCurCost );
        m_YueGongLIst.SetItemText( iRow, 1, tmpStr );
    }
    CString showStr;
    showStr.Format( "%f��Ԫ��", dBenJin );
    SetDlgItemText( IDC_EDIT_DaiKuan, showStr );
    showStr.Format( "%d (��)", iMonthNum );
    SetDlgItemText( IDC_EDIT_DaiKuanYueShu, showStr );
    showStr.Format( "%f��Ԫ��", dRepayment );
    SetDlgItemText( IDC_EDIT_HuanKuan, showStr );
    showStr.Format( "%f��Ԫ��", dRepayment - dBenJin );
    SetDlgItemText( IDC_EDIT_LIXI, showStr );
}



void CDaiKuan_CounterDlg::OnClose()
{
    // TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ
    unInitAll();
    CDialogEx::OnClose();
}


void CDaiKuan_CounterDlg::OnBnClickedButtonClean()
{
    // TODO: �ڴ���ӿؼ�֪ͨ����������
    cleanInput();
    cleanOutput();
}


void CDaiKuan_CounterDlg::OnBnClickedButtonAcount()
{
    // TODO: �ڴ���ӿؼ�֪ͨ����������
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
        AfxMessageBox( "���������쳣��" );
    }
}
