
// XPPutMsg_UIClientDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "XPPutMsg_UIClient.h"
#include "XPPutMsg_UIClientDlg.h"

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
    virtual void DoDataExchange( CDataExchange* pDX );  // DDX/DDV ֧��

    // ʵ��
protected:
    DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialog( CAboutDlg::IDD )
{
}

void CAboutDlg::DoDataExchange( CDataExchange* pDX )
{
    CDialog::DoDataExchange( pDX );
}

BEGIN_MESSAGE_MAP( CAboutDlg, CDialog )
END_MESSAGE_MAP()


// CXPPutMsg_UIClientDlg �Ի���




CXPPutMsg_UIClientDlg::CXPPutMsg_UIClientDlg( CWnd* pParent /*=NULL*/ )
    : CDialog( CXPPutMsg_UIClientDlg::IDD, pParent )
{
    m_hIcon = AfxGetApp()->LoadIcon( IDR_MAINFRAME );
}

void CXPPutMsg_UIClientDlg::DoDataExchange( CDataExchange* pDX )
{
    CDialog::DoDataExchange( pDX );
    DDX_Control( pDX, IDC_COMBO_MSG_TYPE, m_cbMsgTpye );
    DDX_Control( pDX, IDC_COMBO_session_type, m_cbSessionType );
    DDX_Control( pDX, IDC_COMBO_channel, m_cbChannelName );
}

BEGIN_MESSAGE_MAP( CXPPutMsg_UIClientDlg, CDialog )
    ON_WM_SYSCOMMAND()
    ON_WM_PAINT()
    ON_WM_QUERYDRAGICON()
    //}}AFX_MSG_MAP
    ON_BN_CLICKED( IDC_BUTTON_SendData2KCXP, &CXPPutMsg_UIClientDlg::OnBnClickedButtonSenddata2kcxp )
    ON_BN_CLICKED( IDC_BUTTON_LoopSend2KCXP, &CXPPutMsg_UIClientDlg::OnBnClickedButtonLoopsend2kcxp )
    ON_WM_CLOSE()
    ON_WM_DESTROY()
    ON_CBN_SELCHANGE( IDC_COMBO_MSG_TYPE, &CXPPutMsg_UIClientDlg::OnCbnSelchangeComboMsgType )
    ON_CBN_SELCHANGE( IDC_COMBO_session_type, &CXPPutMsg_UIClientDlg::OnCbnSelchangeCombosessiontype )
    ON_BN_CLICKED( IDC_BUTTON_random_send, &CXPPutMsg_UIClientDlg::OnBnClickedButtonrandomsend )
    ON_BN_CLICKED( IDC_BUTTON_Open, &CXPPutMsg_UIClientDlg::OnBnClickedButtonOpen )
    ON_BN_CLICKED( IDC_BUTTON_Close, &CXPPutMsg_UIClientDlg::OnBnClickedButtonClose )
    ON_BN_CLICKED( IDC_BUTTON_ReloadConfigFile, &CXPPutMsg_UIClientDlg::OnBnClickedButtonReloadconfigfile )
END_MESSAGE_MAP()


// CXPPutMsg_UIClientDlg ��Ϣ�������

BOOL CXPPutMsg_UIClientDlg::OnInitDialog()
{
    CDialog::OnInitDialog();
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
    init();
    return TRUE;  // ���ǽ��������õ��ؼ������򷵻� TRUE
}

void CXPPutMsg_UIClientDlg::OnSysCommand( UINT nID, LPARAM lParam )
{
    if( ( nID & 0xFFF0 ) == IDM_ABOUTBOX )
    {
        CAboutDlg dlgAbout;
        dlgAbout.DoModal();
    }
    else
    {
        CDialog::OnSysCommand( nID, lParam );
    }
}

// �����Ի��������С����ť������Ҫ����Ĵ���
//  �����Ƹ�ͼ�ꡣ����ʹ���ĵ�/��ͼģ�͵� MFC Ӧ�ó���
//  �⽫�ɿ���Զ���ɡ�

void CXPPutMsg_UIClientDlg::OnPaint()
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
        CDialog::OnPaint();
    }
}

//���û��϶���С������ʱϵͳ���ô˺���ȡ�ù��
//��ʾ��
HCURSOR CXPPutMsg_UIClientDlg::OnQueryDragIcon()
{
    return static_cast<HCURSOR>( m_hIcon );
}


int CXPPutMsg_UIClientDlg::init()
{
#if CONSOLE
    open_console();
#endif
    InitializeCriticalSection( &m_cs );
    SetDlgItemText( IDC_EDIT_SendCount, "1" );
    GetDlgItem( IDC_BUTTON_Close )->EnableWindow( FALSE );
    InitAllOptions();
    StartWorkThread();
    return 0;
}

int CXPPutMsg_UIClientDlg::uninit()
{
#if CONSOLE
    close_console();
#endif
    StopWorkThread();
    DeleteCriticalSection( &m_cs );
    m_mc.clear();
    return 0;
}

int CXPPutMsg_UIClientDlg::InitAllOptions()
{
    m_clsConfig.loadfile( ini_file, INI_CFG_PATH );
    CString ctrKcxpIp = m_clsConfig.get( "UIClient.kcxp_ip" ).c_str();
    CString ctrChannelNameList = m_clsConfig.get( "UIClient.kcxp_channel_name" ).c_str();
    m_iThreadNum = boost::lexical_cast<int>( m_clsConfig.get( "UIClient.thread_num" ).c_str() );
    SetDlgItemText( IDC_EDIT_KCXP_IP, ctrKcxpIp );
    m_cbChannelName.ResetContent();
    int iFind = 0;
    int iId = 0;
    while( 1 )
    {
        int iPos = ctrChannelNameList.Find( "|", iFind );
        if( iPos == -1 )
        {
            break;
        }
        CString ctrName = ctrChannelNameList.Mid( iFind, iPos - iFind ).Trim();
        m_cbChannelName.InsertString( iId, ctrName.GetString() );
        iId ++ ;
        iFind = iPos + 1;
    }
    //m_cbChannelName.SetCurSel( 0 );
    m_mc.clear();
    boost::property_tree::ptree tree = m_clsConfig.getTree();
    BOOST_FOREACH( boost::property_tree::ptree::value_type & session, tree )
    {
        std::string strSessionName = session.first;
        if( strSessionName == "UIClient" )
        {
            continue;
        }
        kvStrStr kvMap;
        BOOST_FOREACH( boost::property_tree::ptree::value_type & kv, tree.get_child( strSessionName ) )
        {
            std::string key = kv.first;
            std::string value = kv.second.data();
            kvMap.insert( prStrStr( key, value ) );
        }
        //boost::algorithm::to_upper(strSessionName);
        boost::algorithm::to_lower( strSessionName );
        m_mc.insert( prStrMap( strSessionName, kvMap ) );
    }
    m_cbSessionType.ResetContent();
    int iIndex = 0;
    kvStrMap::iterator it;
    for( it = m_mc.begin() ; it != m_mc.end() ; it ++ )
    {
        m_cbSessionType.InsertString( iIndex, it->first.c_str() );
        iIndex ++;
    }
    return 0;
}

int CXPPutMsg_UIClientDlg::open_console()
{
    AllocConsole();
    freopen( "CONOUT$", "w+t", stdout );
    freopen( "CONIN$", "r+t", stdin );
    return 0;
}

int CXPPutMsg_UIClientDlg::close_console()
{
    fclose( stdout );
    fclose( stdin );
    FreeConsole();
    return 0;
}

void CXPPutMsg_UIClientDlg::StartWorkThread()
{
    m_iThreadRunFlag = THREAD_RUNNING;
    m_pBarrier = new boost::barrier( m_iThreadNum + 1 );
    for( int i = 0; i < m_iThreadNum; i++ )
    {
        m_ThreadPool.create_thread( boost::bind( &CXPPutMsg_UIClientDlg::ThreadFunc, boost::ref( this ) ) );
    }
    m_pBarrier->wait();
}

void CXPPutMsg_UIClientDlg::StopWorkThread()
{
    m_iThreadRunFlag = THREAD_STOPPED;
    m_ThreadPool.join_all();
}



int CXPPutMsg_UIClientDlg::ThreadFunc()
{
    m_pBarrier->wait();
    while( THREAD_RUNNING == m_iThreadRunFlag )
    {
        std::string strPopData;
        m_clsCPCQueue.wait_and_pop( strPopData, SLEEP_INTERVAL );
        if( strPopData.length() > 0 )
        {
            EnterCriticalSection( &m_cs );
            m_clsKCXP.Send( strPopData.c_str(), strPopData.length() );
            LeaveCriticalSection( &m_cs );
        }
    }
    return 0;
}

void CXPPutMsg_UIClientDlg::PushData( const CString& data, const int iCount )
{
    for( int i = 0 ; i < iCount ; i ++ )
    {
        m_clsCPCQueue.push( data.GetString() );
    }
}

void CXPPutMsg_UIClientDlg::OnBnClickedButtonSenddata2kcxp()
{
    CString ctrMsgData;
    GetDlgItemText( IDC_EDIT_MSG_DATA, ctrMsgData );
    PushData( ctrMsgData, 1 );
}

void CXPPutMsg_UIClientDlg::OnBnClickedButtonLoopsend2kcxp()
{
    CString ctrMsgData;
    GetDlgItemText( IDC_EDIT_MSG_DATA, ctrMsgData );
    CString ctrSendCount;
    GetDlgItemText( IDC_EDIT_SendCount, ctrSendCount );
    int iSendCount = boost::lexical_cast<int>( ctrSendCount.GetString() );
    PushData( ctrMsgData, iSendCount );
}

void CXPPutMsg_UIClientDlg::OnBnClickedButtonrandomsend()
{
    int iSession = m_cbSessionType.GetCurSel();
    if( iSession < 0 )
    {
        return;
    }
    CString ctrSession;
    m_cbSessionType.GetLBText( iSession, ctrSession );
    CString ctrSendCount;
    GetDlgItemText( IDC_EDIT_SendCount, ctrSendCount );
    int iSendCount = boost::lexical_cast<int>( ctrSendCount.GetString() );
    kvStrMap::iterator it_session = m_mc.find( ctrSession.GetString() );
    if( it_session == m_mc.end() )
    {
        return;
    }
    kvStrStr::iterator it_msg;
    for( it_msg = it_session->second.begin() ; it_msg != it_session->second.end() ; it_msg ++ )
    {
        PushData( it_msg->second.c_str(), iSendCount );
    }
}

void CXPPutMsg_UIClientDlg::OnClose()
{
    uninit();
    CDialog::OnClose();
}

void CXPPutMsg_UIClientDlg::OnDestroy()
{
    CDialog::OnDestroy();
}

void CXPPutMsg_UIClientDlg::OnCbnSelchangeComboMsgType()
{
    int iSessionChoose = m_cbSessionType.GetCurSel();
    CString ctrSessionChoose;
    m_cbSessionType.GetLBText( iSessionChoose, ctrSessionChoose );
    int iMsgTypeChoose = m_cbMsgTpye.GetCurSel();
    CString ctrMsgTypeChoose;
    m_cbMsgTpye.GetLBText( iMsgTypeChoose, ctrMsgTypeChoose );
    kvStrMap::iterator session = m_mc.find( ctrSessionChoose.GetString() );
    if( session != m_mc.end() )
    {
        kvStrStr::iterator msg = session->second.find( ctrMsgTypeChoose.GetString() );
        if( msg != session->second.end() )
        {
            SetDlgItemText( IDC_EDIT_MSG_DATA, msg->second.c_str() );
        }
    }
}

void CXPPutMsg_UIClientDlg::OnCbnSelchangeCombosessiontype()
{
    m_cbMsgTpye.ResetContent();
    int iSessionChoose = m_cbSessionType.GetCurSel();
    CString ctrSessionChoose;
    m_cbSessionType.GetLBText( iSessionChoose, ctrSessionChoose );
    kvStrMap::iterator session = m_mc.find( ctrSessionChoose.GetString() );
    if( session != m_mc.end() )
    {
        int iIndex = 0;
        kvStrStr::iterator msg;
        for( msg = session->second.begin() ; msg != session->second.end() ; msg ++ )
        {
            m_cbMsgTpye.InsertString( iIndex, msg->first.c_str() );
            iIndex ++;
        }
    }
    m_cbMsgTpye.SetCurSel( 0 );
    OnCbnSelchangeComboMsgType();
}



void CXPPutMsg_UIClientDlg::OnBnClickedButtonOpen()
{
    CString ctrChannelName;
    int iChannelName = m_cbChannelName.GetCurSel();
    m_cbChannelName.GetLBText( iChannelName, ctrChannelName );
    CString ctrKcxpIp;
    GetDlgItemText( IDC_EDIT_KCXP_IP, ctrKcxpIp );
    //��ͨ��
    if( 0L != m_clsKCXP.Open( ctrKcxpIp.GetString(), ctrChannelName.GetString() ) )
    {
        char szErr[1024] = {0};
        m_clsKCXP.GetLastError( szErr, sizeof( szErr ) - 1 );
        CString err;
        err.Format( "�쳣��������Ϣ=[%s]", szErr );
        AfxMessageBox( err );
        return;
    }
    GetDlgItem( IDC_BUTTON_Open )->EnableWindow( FALSE );
    GetDlgItem( IDC_BUTTON_Close )->EnableWindow( TRUE );
    GetDlgItem( IDC_EDIT_KCXP_IP )->EnableWindow( FALSE );
    GetDlgItem( IDC_COMBO_channel )->EnableWindow( FALSE );
}

void CXPPutMsg_UIClientDlg::OnBnClickedButtonClose()
{
    //�ر�ͨ��
    if( 0L != m_clsKCXP.Close() )
    {
        char szErr[1024] = {0};
        m_clsKCXP.GetLastError( szErr, sizeof( szErr ) - 1 );
        CString err;
        err.Format( "�쳣��������Ϣ=[%s]", szErr );
        AfxMessageBox( err );
        return;
    }
    GetDlgItem( IDC_BUTTON_Open )->EnableWindow( TRUE );
    GetDlgItem( IDC_BUTTON_Close )->EnableWindow( FALSE );
    GetDlgItem( IDC_EDIT_KCXP_IP )->EnableWindow( TRUE );
    GetDlgItem( IDC_COMBO_channel )->EnableWindow( TRUE );
}

void CXPPutMsg_UIClientDlg::OnBnClickedButtonReloadconfigfile()
{
    InitAllOptions();
}
