
// KCBP_UI_ClientDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "KCBP_UI_Client.h"
#include "KCBP_UI_ClientDlg.h"
#include "BaseTool.h"
#include <regex>

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
    virtual void DoDataExchange( CDataExchange* pDX );  // DDX/DDV 支持

    // 实现
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


// CKCBP_UI_ClientDlg 对话框




CKCBP_UI_ClientDlg::CKCBP_UI_ClientDlg( CWnd* pParent /*=NULL*/ )
    : CDialog( CKCBP_UI_ClientDlg::IDD, pParent )
{
    m_hIcon = AfxGetApp()->LoadIcon( IDR_MAINFRAME );
}

void CKCBP_UI_ClientDlg::DoDataExchange( CDataExchange* pDX )
{
    CDialog::DoDataExchange( pDX );
    DDX_Control( pDX, IDC_LIST_RESULT_SET_2, m_etlcSecondResultSet );
    DDX_Control( pDX, IDC_COMBO_LBM_NO, m_cbxLbmNo );
    DDX_Control( pDX, IDC_LIST_RESULT_SET_1, m_etlcFirstResultSet );
}

BEGIN_MESSAGE_MAP( CKCBP_UI_ClientDlg, CDialog )
    ON_WM_SYSCOMMAND()
    ON_WM_PAINT()
    ON_WM_QUERYDRAGICON()
    //}}AFX_MSG_MAP
    ON_WM_CLOSE()
    ON_BN_CLICKED( IDC_BUTTON_save_connect_cfg, &CKCBP_UI_ClientDlg::OnBnClickedButtonsaveconnectcfg )
    ON_BN_CLICKED( IDC_BUTTON_connect_svr, &CKCBP_UI_ClientDlg::OnBnClickedButtonconnectsvr )
    ON_BN_CLICKED( IDC_BUTTON_disconnect_svr, &CKCBP_UI_ClientDlg::OnBnClickedButtondisconnectsvr )
    ON_CBN_SELCHANGE( IDC_COMBO_LBM_NO, &CKCBP_UI_ClientDlg::OnCbnSelchangeComboLbmNo )
    ON_BN_CLICKED( IDC_BUTTON_call_lbm, &CKCBP_UI_ClientDlg::OnBnClickedButtoncalllbm )
    ON_BN_CLICKED( IDC_BUTTON_add_this_lbm_info, &CKCBP_UI_ClientDlg::OnBnClickedButtonaddthislbminfo )
    ON_BN_CLICKED( IDC_BUTTON_save_this_libm_info, &CKCBP_UI_ClientDlg::OnBnClickedButtonsavethislibminfo )
    ON_BN_CLICKED( IDC_BUTTON_CLEAN_PARAMLIST, &CKCBP_UI_ClientDlg::OnBnClickedButtonCleanParamlist )
END_MESSAGE_MAP()


// CKCBP_UI_ClientDlg 消息处理程序

BOOL CKCBP_UI_ClientDlg::OnInitDialog()
{
    CDialog::OnInitDialog();
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
    Init();
    return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

void CKCBP_UI_ClientDlg::OnSysCommand( UINT nID, LPARAM lParam )
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

// 如果向对话框添加最小化按钮，则需要下面的代码
//  来绘制该图标。对于使用文档/视图模型的 MFC 应用程序，
//  这将由框架自动完成。

void CKCBP_UI_ClientDlg::OnPaint()
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
        CDialog::OnPaint();
    }
}

//当用户拖动最小化窗口时系统调用此函数取得光标
//显示。
HCURSOR CKCBP_UI_ClientDlg::OnQueryDragIcon()
{
    return static_cast<HCURSOR>( m_hIcon );
}


void CKCBP_UI_ClientDlg::Init()
{
    ModifyWindowsShow( TRUE );
    //open_console();
    m_clsCfg.loadfile( xml_file, XML_CFG_PATH );
    //在界面显示配置文件中的值
    CString ctrTmp = m_clsCfg.get( "root.config.kcbp_svr_name.<xmlattr>.value" ).c_str();
    SetDlgItemText( IDC_EDIT_kcbp_svr_name, ctrTmp );
    ctrTmp = m_clsCfg.get( "root.config.kcbp_svr_ip.<xmlattr>.value" ).c_str();
    SetDlgItemText( IDC_EDIT_kcbp_ip, ctrTmp );
    ctrTmp = m_clsCfg.get( "root.config.kcbp_svr_port.<xmlattr>.value" ).c_str();
    SetDlgItemText( IDC_EDIT_kcbp_svr_port, ctrTmp );
    ctrTmp = m_clsCfg.get( "root.config.send_queue_name.<xmlattr>.value" ).c_str();
    SetDlgItemText( IDC_EDIT_send_queue_name, ctrTmp );
    ctrTmp = m_clsCfg.get( "root.config.recv_queue_name.<xmlattr>.value" ).c_str();
    SetDlgItemText( IDC_EDIT_recv_queue_name, ctrTmp );
    ctrTmp = m_clsCfg.get( "root.config.user_name.<xmlattr>.value" ).c_str();
    SetDlgItemText( IDC_EDIT_user_name, ctrTmp );
    ctrTmp = m_clsCfg.get( "root.config.user_pwd.<xmlattr>.value" ).c_str();
    SetDlgItemText( IDC_EDIT_user_pwd, ctrTmp );
    //这里设置控件风格
    DWORD dwLong1 = ::GetWindowLong( m_etlcFirstResultSet.GetSafeHwnd(), GWL_STYLE );
    DWORD dwLong2 = ::GetWindowLong( m_etlcSecondResultSet.GetSafeHwnd(), GWL_STYLE );
    dwLong1 |= LVS_SINGLESEL;       //只可单行选中
    dwLong2 |= LVS_SINGLESEL;       //只可单行选中
    dwLong1 |= LVS_SHOWSELALWAYS;   //Always show selection
    dwLong2 |= LVS_SHOWSELALWAYS;   //Always show selection
    ::SetWindowLong( m_etlcFirstResultSet.GetSafeHwnd(), GWL_STYLE, dwLong1 );
    ::SetWindowLong( m_etlcSecondResultSet.GetSafeHwnd(), GWL_STYLE, dwLong2 );
    DWORD dwStyle1 = m_etlcFirstResultSet.GetExtendedStyle();
    DWORD dwStyle2 = m_etlcSecondResultSet.GetExtendedStyle();
    dwStyle1 |= LVS_EX_GRIDLINES;        //网格线
    dwStyle2 |= LVS_EX_GRIDLINES;        //网格线
    dwStyle1 |= LVS_EX_FULLROWSELECT;    //整行高亮
    dwStyle2 |= LVS_EX_FULLROWSELECT;    //整行高亮
    //dwStyleEx |= LVS_EX_CHECKBOXES;       //Item前生成check box
    m_etlcFirstResultSet.SetExtendedStyle( dwStyle1 );
    m_etlcSecondResultSet.SetExtendedStyle( dwStyle2 );
    //初始化lbmcbx
    tstring strDefParam = m_clsCfg.get( "root.lbms.<xmlattr>.DefParam" ).c_str();
    boost::property_tree::ptree xml = m_clsCfg.getTree();
    BOOST_FOREACH( boost::property_tree::ptree::value_type & kv, xml.get_child( "root.lbms" ) )
    {
        try
        {
            LbmInfo stLbm;
            stLbm.strLbmNo = kv.second.get<tstring>( "<xmlattr>.Id" );
            stLbm.strParams = kv.second.get_value<std::string>();
            m_mpLbmInfo.insert( LbmInfoPair( stLbm.strLbmNo, stLbm ) );
        }
        catch( boost::property_tree::ptree_error ex )
        {
            //AfxMessageBox(ex.what());
            OutputDebugStringA( ex.what() );
        }
        catch( std::exception& ex )
        {
            //AfxMessageBox(ex.what());
            OutputDebugStringA( ex.what() );
        }
    }
    //排序后插入combox
    for( LbmInfoMap::iterator it = m_mpLbmInfo.begin() ; it != m_mpLbmInfo.end() ; ++it )
    {
        m_cbxLbmNo.InsertString( m_cbxLbmNo.GetCount(), it->first.c_str() );
    }
}

void CKCBP_UI_ClientDlg::Uninit()
{
    //close_console();
    m_mpLbmInfo.clear();
    m_cbxLbmNo.Clear();
}

void CKCBP_UI_ClientDlg::OnClose()
{
    Uninit();
    CDialog::OnClose();
}

void CKCBP_UI_ClientDlg::OpenConsole()
{
    AllocConsole();
    freopen( "CONOUT$", "w+t", stdout );
    freopen( "CONIN$", "r+t", stdin );
}

void CKCBP_UI_ClientDlg::CloseConsole()
{
    fclose( stdout );
    fclose( stdin );
    FreeConsole();
}


void CKCBP_UI_ClientDlg::ShowError()
{
    AfxMessageBox( m_clsKcbp.GetErrorMsg().c_str() );
}

void CKCBP_UI_ClientDlg::PrintTree( boost::property_tree::ptree root )
{
    BOOST_FOREACH( boost::property_tree::ptree::value_type & v1, root )
    {
        //如果当前节点为属性，打印全部属性信息
        if( v1.first == "<xmlattr>" )
        {
            BOOST_FOREACH( boost::property_tree::ptree::value_type & vAttr, v1.second )
            {
                std::cout << vAttr.first << "=" << vAttr.second.data() << " ";
            }
            printf( "\n" );
        }
        else
        {
            //如果没有子节点，则打印键值对
            if( v1.second.empty() )
            {
                std::cout << v1.first << "=" << v1.second.data() << std::endl;
            }
            else
            {
                //打印当前节点的名称
                std::cout << v1.first << std::endl;
                PrintTree( v1.second );
                if( v1.second.size() == 0x01 )
                    std::cout << root.get<std::string>( v1.first ) << std::endl;
            }
        }
    }
}

void CKCBP_UI_ClientDlg::CallLbm( const LbmInfo& lbm )
{
    Json::Value jsParamList;
    AnalysisParams( lbm.strParams, jsParamList );
    int iRet = 0;
    tstring strRet;
    Json::Value jsResultSet;
    tstring strTmp = CBaseTool::tformat( "开始 %s\n", CBaseTool::GetTimestamp_AsStr().c_str() );
    SetDlgItemText( IDC_STATIC_CALL_LBM_LOG, strTmp.c_str() );
    iRet = m_clsKcbp.CallLbm_AllResult( lbm.strLbmNo, jsParamList, jsResultSet, strRet );
    strTmp.append( CBaseTool::tformat( "结束 %s", CBaseTool::GetTimestamp_AsStr().c_str() ) );
    SetDlgItemText( IDC_STATIC_CALL_LBM_LOG, strTmp.c_str() );
    if( iRet != 0 )
    {
        AfxMessageBox( CBaseTool::tformat( "[%d]=[%s]", iRet, strRet.c_str() ).c_str() );
        return;
    }
    tstring strResultSet = CBaseTool::json_to_str( jsResultSet );
    printf( strResultSet.c_str() );
    ShowResultToView( jsResultSet );
}

void CKCBP_UI_ClientDlg::AnalysisParams( const tstring& strInput, Json::Value& jsParams )
{
    std::string src( strInput );
    if( src.length() == 0 )
    {
        return;
    }
    int iFind = 0;
    while( 1 )
    {
        int iPos = src.find( ",", iFind );
        if( iPos == -1 )
        {
            break;
        }
        std::string one_param = CBaseTool::trim( src.substr( iFind, iPos - iFind ) );
        if( one_param.find( ":'" ) != std::string::npos )
        {
            int iDYHZ = src.find( ":'", iFind );
            int iDYHY = src.find( "'", iDYHZ + 2 );
            one_param = CBaseTool::trim( src.substr( iFind, iDYHY - iFind + 1 ) );
            iPos = iDYHY;
        }
        int iSeparate = one_param.find( ":" );
        if( iSeparate > 0 )
        {
            std::string key = CBaseTool::trim( one_param.substr( 0, iSeparate ) );
            std::string value = CBaseTool::trim( one_param.substr( iSeparate + 1 ) );
            if( value.find( "'" ) != std::string::npos )
            {
                value = value.substr( 1, value.length() - 2 );
            }
            jsParams[key] = value;
        }
        iFind = iPos + 1;
    }
}

//for (int i = 0 ; i < 10 ; i ++)
//{
//  m_list_ctrl_input_param.InsertColumn(i,"field",LVCFMT_LEFT,LISTCTRL_WIDTH);
//  m_list_ctrl_input_param.SetColumnCtrlType(i,CCT_EDITBOX);
//}

//int nItem = 0;
//nItem = m_list_ctrl_input_param.InsertItem(m_list_ctrl_input_param.GetItemCount(), TEXT("WangYao"), 0);
//m_list_ctrl_input_param.SetItemText(nItem, 1, TEXT("26"));   //子项从1开始,0代表主项
//m_list_ctrl_input_param.SetItemText(nItem, 2, TEXT("Male"));
//m_list_ctrl_input_param.SetItemText(nItem, 3, TEXT("12"));

//nItem = m_list_ctrl_input_param.InsertItem(m_list_ctrl_input_param.GetItemCount(), TEXT("WangYan"), 0);
//m_list_ctrl_input_param.SetItemText(nItem, 1, TEXT("24"));
//m_list_ctrl_input_param.SetItemText(nItem, 2, TEXT("Female"));
//m_list_ctrl_input_param.SetItemText(nItem, 3, TEXT("12"));

//nItem = m_list_ctrl_input_param.InsertItem(m_list_ctrl_input_param.GetItemCount(), TEXT("WangXuan"), 0);
//m_list_ctrl_input_param.SetItemText(nItem, 1, TEXT("24"));
//m_list_ctrl_input_param.SetItemText(nItem, 2, TEXT("Female"));
//m_list_ctrl_input_param.SetItemText(nItem, 3, TEXT("12"));

//nItem = m_list_ctrl_input_param.InsertItem(m_list_ctrl_input_param.GetItemCount(), TEXT("WangJiLiang"), 0);
//m_list_ctrl_input_param.SetItemText(nItem, 1, TEXT("15"));
//m_list_ctrl_input_param.SetItemText(nItem, 2, TEXT("Male"));
//m_list_ctrl_input_param.SetItemText(nItem, 3, TEXT("12"));

void CKCBP_UI_ClientDlg::ShowResultToView( const Json::Value& jsResultSet )
{
    //清理整个控件
    m_etlcFirstResultSet.DeleteAllItems();
    int iCols = m_etlcFirstResultSet.GetHeaderCtrl()->GetItemCount();
    for( int i = 0 ; i < iCols ; ++i )
    {
        m_etlcFirstResultSet.DeleteColumn( 0 );
    }
    m_etlcSecondResultSet.DeleteAllItems();   //删除所有行
    iCols = m_etlcSecondResultSet.GetHeaderCtrl()->GetItemCount();
    for( int i = 0 ; i < iCols ; i ++ )
    {
        m_etlcSecondResultSet.DeleteColumn( 0 ); //删除所有表格列
    }
    if( jsResultSet.isMember( "result1" ) == false )
    {
        AfxMessageBox( "没有获取到第一结果集" );
        return;
    }
    m_etlcFirstResultSet.InsertColumn( 0, "行号", LVCFMT_LEFT, LISTCTRL_WIDTH );
    m_etlcFirstResultSet.SetColumnCtrlType( 0, CCT_EDITBOX );
    Json::Value jsResult1 = jsResultSet["result1"];
    if( jsResult1.type() != Json::arrayValue )
    {
        AfxMessageBox( "没有获取到第一结果集" );
        return;
    }
    //显示第一结果集
    {
        Json::Value::Members jvmNames = jsResult1[0u].getMemberNames();
        for( Json::Value::Members::iterator jvmit = jvmNames.begin() ;  jvmit != jvmNames.end() ; ++jvmit )
        {
            int iIndex = m_etlcFirstResultSet.GetHeaderCtrl()->GetItemCount();
            m_etlcFirstResultSet.InsertColumn( iIndex, ( *jvmit ).c_str(), LVCFMT_LEFT, LISTCTRL_WIDTH );
            m_etlcFirstResultSet.SetColumnCtrlType( iIndex, CCT_EDITBOX );
        }
        for( Json::Value::iterator jvit = jsResult1.begin() ; jvit != jsResult1.end() ; ++ jvit )
        {
            int iCurCount = m_etlcFirstResultSet.GetItemCount();
            int iRowId = m_etlcFirstResultSet.InsertItem( iCurCount, CBaseTool::tformat( "%d", iCurCount + 1 ).c_str() );
            Json::Value jsRow = *jvit;
            int iColId = 1;
            for( Json::Value::Members::iterator jvmit = jvmNames.begin() ;  jvmit != jvmNames.end() ; ++jvmit )
            {
                tstring strValue = CBaseTool::get_json_val( jsRow, *jvmit, false );
                m_etlcFirstResultSet.SetItemText( iRowId, iColId, strValue.c_str() );
                iColId++;
            }
        }
    }
    //显示第二结果集
    if( jsResultSet.isMember( "result2" ) )
    {
        m_etlcSecondResultSet.InsertColumn( 0, "行号", LVCFMT_LEFT, LISTCTRL_WIDTH );
        m_etlcSecondResultSet.SetColumnCtrlType( 0, CCT_EDITBOX );
        Json::Value jsResult2 = jsResultSet["result2"];
        if( jsResult2.type() != Json::arrayValue )
        {
            AfxMessageBox( "没有获取到第二结果集" );
            return;
        }
        Json::Value::Members jvmNames = jsResult2[0u].getMemberNames();
        for( Json::Value::Members::iterator jvmit = jvmNames.begin() ;  jvmit != jvmNames.end() ; ++jvmit )
        {
            int iIndex = m_etlcSecondResultSet.GetHeaderCtrl()->GetItemCount();
            m_etlcSecondResultSet.InsertColumn( iIndex, ( *jvmit ).c_str(), LVCFMT_LEFT, LISTCTRL_WIDTH );
            m_etlcSecondResultSet.SetColumnCtrlType( iIndex, CCT_EDITBOX );
        }
        for( Json::Value::iterator jvit = jsResult2.begin() ; jvit != jsResult2.end() ; ++ jvit )
        {
            int iCurCount = m_etlcSecondResultSet.GetItemCount();
            int iRowId = m_etlcSecondResultSet.InsertItem( iCurCount, CBaseTool::tformat( "%d", iCurCount + 1 ).c_str() );
            Json::Value jsRow = *jvit;
            int iColId = 1;
            for( Json::Value::Members::iterator jvmit = jvmNames.begin() ;  jvmit != jvmNames.end() ; ++jvmit )
            {
                tstring strValue = CBaseTool::get_json_val( jsRow, *jvmit, false );
                m_etlcSecondResultSet.SetItemText( iRowId, iColId, strValue.c_str() );
                iColId++;
            }
        }
    }
}


void CKCBP_UI_ClientDlg::ModifyWindowsShow( BOOL bflag )
{
    GetDlgItem( IDC_BUTTON_connect_svr )->EnableWindow( bflag );
    GetDlgItem( IDC_BUTTON_disconnect_svr )->EnableWindow( !bflag );
    GetDlgItem( IDC_EDIT_kcbp_ip )->EnableWindow( bflag );
    GetDlgItem( IDC_EDIT_kcbp_svr_port )->EnableWindow( bflag );
    GetDlgItem( IDC_EDIT_kcbp_svr_name )->EnableWindow( bflag );
    GetDlgItem( IDC_EDIT_send_queue_name )->EnableWindow( bflag );
    GetDlgItem( IDC_EDIT_recv_queue_name )->EnableWindow( bflag );
    GetDlgItem( IDC_EDIT_user_name )->EnableWindow( bflag );
    GetDlgItem( IDC_EDIT_user_pwd )->EnableWindow( bflag );
}

void CKCBP_UI_ClientDlg::OnBnClickedButtonsaveconnectcfg()
{
    CString ctrKcbpSvrName, ctrKcbpSvrIp, ctrKcbpSvrPort, ctrSendQueueName, ctrRecvQueueName, ctrUserName, ctrUserPwd;
    GetDlgItemText( IDC_EDIT_kcbp_svr_name, ctrKcbpSvrName );
    GetDlgItemText( IDC_EDIT_kcbp_ip, ctrKcbpSvrIp );
    GetDlgItemText( IDC_EDIT_kcbp_svr_port, ctrKcbpSvrPort );
    GetDlgItemText( IDC_EDIT_send_queue_name, ctrSendQueueName );
    GetDlgItemText( IDC_EDIT_recv_queue_name, ctrRecvQueueName );
    GetDlgItemText( IDC_EDIT_user_name, ctrUserName );
    GetDlgItemText( IDC_EDIT_user_pwd, ctrUserPwd );
    std::ifstream ifsFile;
    ifsFile.open( XML_CFG_PATH );
    if( ifsFile.is_open() == false )
    {
        AfxMessageBox( CBaseTool::tformat( "%s file open fail", XML_CFG_PATH ).c_str() );
        return;
    }
    tstringstream stmXmlFileStream;
    stmXmlFileStream << ifsFile.rdbuf();
    tstring strFileData = stmXmlFileStream.str();
    strFileData = std::tr1::regex_replace( strFileData, std::tr1::regex( "<kcbp_svr_name\\s+value=\"\\S+\"\\s*/>" ), CBaseTool::tformat( "<kcbp_svr_name value=\"%s\"/>", ctrKcbpSvrName.GetString() ) );
    strFileData = std::tr1::regex_replace( strFileData, std::tr1::regex( "<kcbp_svr_ip\\s+value=\"\\S+\"\\s*/>" ), CBaseTool::tformat( "<kcbp_svr_ip value=\"%s\"/>", ctrKcbpSvrIp.GetString() ) );
    strFileData = std::tr1::regex_replace( strFileData, std::tr1::regex( "<kcbp_svr_port\\s+value=\"\\S+\"\\s*/>" ), CBaseTool::tformat( "<kcbp_svr_port value=\"%s\"/>", ctrKcbpSvrPort.GetString() ) );
    strFileData = std::tr1::regex_replace( strFileData, std::tr1::regex( "<send_queue_name\\s+value=\"\\S+\"\\s*/>" ), CBaseTool::tformat( "<send_queue_name value=\"%s\"/>", ctrSendQueueName.GetString() ) );
    strFileData = std::tr1::regex_replace( strFileData, std::tr1::regex( "<recv_queue_name\\s+value=\"\\S+\"\\s*/>" ), CBaseTool::tformat( "<recv_queue_name value=\"%s\"/>", ctrRecvQueueName.GetString() ) );
    strFileData = std::tr1::regex_replace( strFileData, std::tr1::regex( "<user_name\\s+value=\"\\S+\"\\s*/>" ), CBaseTool::tformat( "<user_name value=\"%s\"/>", ctrUserName.GetString() ) );
    strFileData = std::tr1::regex_replace( strFileData, std::tr1::regex( "<user_pwd\\s+value=\"\\S+\"\\s*/>" ), CBaseTool::tformat( "<user_pwd value=\"%s\"/>", ctrUserPwd.GetString() ) );
    std::ofstream ofsFile;
    ofsFile.open( XML_CFG_PATH );
    if( ofsFile.is_open() == false )
    {
        AfxMessageBox( CBaseTool::tformat( "%s file open fail", XML_CFG_PATH ).c_str() );
        return;
    }
    ofsFile << strFileData;
    ofsFile.close();
    AfxMessageBox( "链接配置保存成功" );
}

void CKCBP_UI_ClientDlg::OnBnClickedButtonconnectsvr()
{
    CString kcbp_svr_name, kcbp_svr_ip, kcbp_svr_port, send_queue_name, recv_queue_name, user_name, user_pwd;
    GetDlgItemText( IDC_EDIT_kcbp_svr_name, kcbp_svr_name );
    GetDlgItemText( IDC_EDIT_kcbp_ip, kcbp_svr_ip );
    GetDlgItemText( IDC_EDIT_kcbp_svr_port, kcbp_svr_port );
    GetDlgItemText( IDC_EDIT_send_queue_name, send_queue_name );
    GetDlgItemText( IDC_EDIT_recv_queue_name, recv_queue_name );
    GetDlgItemText( IDC_EDIT_user_name, user_name );
    GetDlgItemText( IDC_EDIT_user_pwd, user_pwd );
    KcbpConfig stKcbp;
    stKcbp.strIp = kcbp_svr_ip.GetString();
    stKcbp.strName = kcbp_svr_name.GetString();
    stKcbp.strPort = kcbp_svr_port.GetString();
    stKcbp.strProtocal = m_clsCfg.get( "root.config.kcbp_svr_protocal.<xmlattr>.value" );
    stKcbp.strPwd = user_pwd.GetString();
    stKcbp.strReqQue = send_queue_name.GetString();
    stKcbp.strRespQue = recv_queue_name.GetString();
    stKcbp.strTimeOut = "30";
    stKcbp.strUser = user_name.GetString();
    int iRet = 0;
    tstring strRet;
    m_clsKcbp.SetConfig( stKcbp );
    iRet = m_clsKcbp.Connect( strRet );
    if( iRet != 0 )
    {
        AfxMessageBox( strRet.c_str() );
        return;
    }
    //链接按钮置灰，断开按钮还原
    ModifyWindowsShow( FALSE );
}

void CKCBP_UI_ClientDlg::OnBnClickedButtondisconnectsvr()
{
    int iRet = 0;
    tstring strRet;
    m_clsKcbp.Disconnect( strRet );
    if( iRet != 0 )
    {
        AfxMessageBox( strRet.c_str() );
        return;
    }
    //链接按钮置灰，断开按钮还原
    ModifyWindowsShow( TRUE );
}

void CKCBP_UI_ClientDlg::OnCbnSelchangeComboLbmNo()
{
    CString lbm_no;
    m_cbxLbmNo.GetLBText( m_cbxLbmNo.GetCurSel(), lbm_no );
    LbmInfoMap::iterator it = m_mpLbmInfo.find( lbm_no.GetString() );
    if( it != m_mpLbmInfo.end() )
    {
        SetDlgItemText( IDC_EDIT_LBM_PARAMLIST, it->second.strParams.c_str() );
    }
}

void CKCBP_UI_ClientDlg::OnBnClickedButtoncalllbm()
{
    CString ctrLbmNo;
    GetDlgItemText( IDC_COMBO_LBM_NO, ctrLbmNo );
    CString ctrParams;
    GetDlgItemText( IDC_EDIT_LBM_PARAMLIST, ctrParams );
    if( ctrLbmNo.IsEmpty() )
    {
        AfxMessageBox( "没有选中LBM" );
        return;
    }
    LbmInfo lbm;
    lbm.strLbmNo = ctrLbmNo.GetString();
    lbm.strParams = ctrParams.GetString();
    CallLbm( lbm );
}



void CKCBP_UI_ClientDlg::OnBnClickedButtonaddthislbminfo()
{
    CString ctrLbmNo;
    GetDlgItemText( IDC_COMBO_LBM_NO, ctrLbmNo );
    CString ctrParams;
    GetDlgItemText( IDC_EDIT_LBM_PARAMLIST, ctrParams );
    LbmInfo lbm;
    lbm.strLbmNo = ctrLbmNo.GetString();
    lbm.strParams = ctrParams.GetString();
    //写入配置
    std::ifstream ifsFile;
    ifsFile.open( XML_CFG_PATH );
    if( ifsFile.is_open() == false )
    {
        AfxMessageBox( CBaseTool::tformat( "%s file open fail", XML_CFG_PATH ).c_str() );
        return;
    }
    tstringstream stmXmlFileStream;
    stmXmlFileStream << ifsFile.rdbuf();
    tstring strFileData = stmXmlFileStream.str();
    if( strFileData.find( CBaseTool::tformat( "\"%s\"", lbm.strLbmNo.c_str() ) ) != tstring::npos )
    {
        AfxMessageBox( CBaseTool::tformat( "%s 已经存在于配置中", lbm.strLbmNo.c_str() ).c_str() );
        return;
    }
    tstring strNewLbm = CBaseTool::tformat( "\n    <lbm Id=\"%s\">\n      <![CDATA[%s]]>\n    </lbm>\n", lbm.strLbmNo.c_str(), lbm.strParams.c_str() );
    strFileData = std::tr1::regex_replace( strFileData, std::tr1::regex( "</lbm>\\s*</lbms>" ), CBaseTool::tformat( "</lbm>%s  </lbms>", strNewLbm.c_str() ) );
    std::ofstream ofsFile;
    ofsFile.open( XML_CFG_PATH );
    if( ofsFile.is_open() == false )
    {
        AfxMessageBox( CBaseTool::tformat( "%s file open fail", XML_CFG_PATH ).c_str() );
        return;
    }
    ofsFile << strFileData;
    ofsFile.close();
    //写入控件和缓存
    m_mpLbmInfo.insert( LbmInfoPair( lbm.strLbmNo, lbm ) );
    m_cbxLbmNo.InsertString( m_cbxLbmNo.GetCount(), lbm.strLbmNo.c_str() );
    AfxMessageBox( "新增案例成功" );
}

void CKCBP_UI_ClientDlg::OnBnClickedButtonsavethislibminfo()
{
    CString ctrLbmNo;
    GetDlgItemText( IDC_COMBO_LBM_NO, ctrLbmNo );
    CString ctrParams;
    GetDlgItemText( IDC_EDIT_LBM_PARAMLIST, ctrParams );
    LbmInfo lbm;
    lbm.strLbmNo = ctrLbmNo.GetString();
    lbm.strParams = ctrParams.GetString();
    //写入配置
    std::ifstream ifsFile;
    ifsFile.open( XML_CFG_PATH );
    if( ifsFile.is_open() == false )
    {
        AfxMessageBox( CBaseTool::tformat( "%s file open fail", XML_CFG_PATH ).c_str() );
        return;
    }
    tstringstream stmXmlFileStream;
    stmXmlFileStream << ifsFile.rdbuf();
    tstring strFileData = stmXmlFileStream.str();
    if( strFileData.find( CBaseTool::tformat( "\"%s\"", lbm.strLbmNo.c_str() ) ) == tstring::npos )
    {
        AfxMessageBox( CBaseTool::tformat( "%s 不存在于配置中，请点击新增", lbm.strLbmNo.c_str() ).c_str() );
        return;
    }
    tstring strRegex = CBaseTool::tformat( "<lbm\\s+Id=\"%s\"\\s*>\\s*<!\\[CDATA\\[\\S*\\]\\]>\\s*</lbm>", lbm.strLbmNo.c_str() );
    std::tr1::regex rgxSearch( strRegex );
    std::tr1::smatch result;
    bool bFlag = std::tr1::regex_search( strFileData, result, rgxSearch );
    if( !bFlag )
    {
        AfxMessageBox( CBaseTool::tformat( "虽然[%s]存在，但正则表达式[%s]无法匹配成功", lbm.strLbmNo.c_str(), strRegex.c_str() ).c_str() );
        return;
    }
    tstring strNewData = CBaseTool::tformat( "<lbm Id=\"%s\">\n      <![CDATA[%s]]>\n    </lbm>", lbm.strLbmNo.c_str(), lbm.strParams.c_str() );
    strFileData = std::tr1::regex_replace( strFileData, rgxSearch, strNewData );
    std::ofstream ofsFile;
    ofsFile.open( XML_CFG_PATH );
    if( ofsFile.is_open() == false )
    {
        AfxMessageBox( CBaseTool::tformat( "%s file open fail", XML_CFG_PATH ).c_str() );
        return;
    }
    ofsFile << strFileData;
    ofsFile.close();
    //写入缓存
    m_mpLbmInfo[lbm.strLbmNo] = lbm;
    AfxMessageBox( "参数保存成功" );
}

void CKCBP_UI_ClientDlg::OnBnClickedButtonCleanParamlist()
{
    SetDlgItemText( IDC_EDIT_LBM_PARAMLIST, "" );
}
