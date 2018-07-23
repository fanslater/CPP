
// KCBP_UI_ClientDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "KCBP_UI_Client.h"
#include "KCBP_UI_ClientDlg.h"
#include "BaseTool.h"

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
    DDX_Control( pDX, IDC_LIST_Input_param, m_etlcResultSet );
    DDX_Control( pDX, IDC_COMBO_LBM_NO, m_cbxLbmNo );
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
    DWORD dwStyle = ::GetWindowLong( m_etlcResultSet.GetSafeHwnd(), GWL_STYLE );
    dwStyle |= LVS_SINGLESEL;       //只可单行选中
    dwStyle |= LVS_SHOWSELALWAYS;   //Always show selection
    ::SetWindowLong( m_etlcResultSet.GetSafeHwnd(), GWL_STYLE, dwStyle );
    DWORD dwStyleEx = m_etlcResultSet.GetExtendedStyle();
    dwStyleEx |= LVS_EX_GRIDLINES;        //网格线
    dwStyleEx |= LVS_EX_FULLROWSELECT;    //整行高亮
    //dwStyleEx |= LVS_EX_CHECKBOXES;       //Item前生成check box
    m_etlcResultSet.SetExtendedStyle( dwStyleEx );
    //初始化lbmcbx
    m_ctrDefaultParam = m_clsCfg.get( "root.lbms.<xmlattr>.DefParam" ).c_str();
    boost::property_tree::ptree xml = m_clsCfg.getTree();
    m_liLBM.clear();
    BOOST_FOREACH( boost::property_tree::ptree::value_type & kv, xml.get_child( "root.lbms" ) )
    {
        try
        {
            lbm_call_cfg lcc;
            lcc.lbm_Id = kv.second.get<string>( "<xmlattr>.Id" );
            lcc.lbm_NeedDef = kv.second.get<string>( "<xmlattr>.NeedDef" );
            lcc.lbm_Param = kv.second.get_value<std::string>();
            m_cbxLbmNo.InsertString( m_cbxLbmNo.GetCount(), lcc.lbm_Id.c_str() );
            m_liLBM.insert( lbmPair( lcc.lbm_Id, lcc ) );
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
}

void CKCBP_UI_ClientDlg::Uninit()
{
    //close_console();
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

void CKCBP_UI_ClientDlg::CallLbm( lbm_call_cfg& lbm )
{
    lbmInfo::iterator itLBM = m_liLBM.find( lbm.lbm_Id );
    if( itLBM == m_liLBM.end() )
    {
        AfxMessageBox( "lbm no found" );
        return;
    }
    std::string strFullParam;
    if( itLBM->second.lbm_NeedDef == "1" )
    {
        strFullParam = m_ctrDefaultParam.GetString();
    }
    strFullParam.append( lbm.lbm_Param );
    Json::Value jsParamList;
    AnalysisParams( strFullParam, jsParamList );
    int iRet = 0;
    tstring strRet;
    Json::Value jsResultSet;
    iRet = m_clsKcbp.CallLbm_AllResult( lbm.lbm_Id, jsParamList, jsResultSet, strRet );
    if( iRet != 0 )
    {
        AfxMessageBox( strRet.c_str() );
        return;
    }
    tstring strResultSet = CBaseTool::json_to_str( jsResultSet );
    printf( strResultSet.c_str() );
    ShowResultToView( jsResultSet );
}

void CKCBP_UI_ClientDlg::AnalysisInput( const tstring input, kvmap& paramlist )
{
    std::string src( input );
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
            paramlist.insert( kvpair( key, value ) );
        }
        iFind = iPos + 1;
    }
}

void CKCBP_UI_ClientDlg::AnalysisParams( const tstring& strInput, Json::Value& jsParams )
{
    kvmap paramlist;
    AnalysisInput( strInput, paramlist );
    for( kvmap::iterator it = paramlist.begin() ; it != paramlist.end() ; ++it )
    {
        jsParams[it->first] = it->second;
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
    m_etlcResultSet.DeleteAllItems();   //删除所有行
    int nCols = m_etlcResultSet.GetHeaderCtrl()->GetItemCount();
    for( int i = 0 ; i < nCols ; i ++ )
    {
        m_etlcResultSet.DeleteColumn( 0 ); //删除所有表格列
    }
    m_etlcResultSet.InsertColumn( 0, "行号", LVCFMT_LEFT, LISTCTRL_WIDTH );
    m_etlcResultSet.SetColumnCtrlType( 0, CCT_EDITBOX );
    Json::Value jsShowResultSet;
    int iResultSetCount = jsResultSet.size();
    if( iResultSetCount == 1 )      //只有第一结果集
    {
        jsShowResultSet = jsResultSet.get( "result1", Json::nullValue );
    }
    else
    {
        jsShowResultSet =  jsResultSet.get( "result2", Json::nullValue );
    }
    if( jsShowResultSet.type() != Json::arrayValue )
    {
        AfxMessageBox( "无法获取结果集" );
        return;
    }
    Json::Value::Members jvmNames = jsShowResultSet[0u].getMemberNames();
    for( Json::Value::Members::iterator jvmit = jvmNames.begin() ;  jvmit != jvmNames.end() ; ++jvmit )
    {
        int iIndex = m_etlcResultSet.GetHeaderCtrl()->GetItemCount();
        m_etlcResultSet.InsertColumn( iIndex, ( *jvmit ).c_str(), LVCFMT_LEFT, LISTCTRL_WIDTH );
        m_etlcResultSet.SetColumnCtrlType( iIndex, CCT_EDITBOX );
    }
    for( Json::Value::iterator jvit = jsShowResultSet.begin() ; jvit != jsShowResultSet.end() ; ++ jvit )
    {
        int iCurCount = m_etlcResultSet.GetItemCount();
        int iRowId = m_etlcResultSet.InsertItem( iCurCount, CBaseTool::tformat( "%d", iCurCount + 1 ).c_str() );
        Json::Value jsRow = *jvit;
        int iColId = 1;
        for( Json::Value::Members::iterator jvmit = jvmNames.begin() ;  jvmit != jvmNames.end() ; ++jvmit )
        {
            tstring strValue = CBaseTool::get_json_val( jsRow, *jvmit );
            m_etlcResultSet.SetItemText( iRowId, iColId, strValue.c_str() );
            iColId++;
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
    //把界面上的配置写入文件
    CString ctrTmp;
    GetDlgItemText( IDC_EDIT_kcbp_svr_name, ctrTmp );
    m_clsCfg.set( "root.config.kcbp_svr_name.<xmlattr>.value", ctrTmp.GetString() );
    GetDlgItemText( IDC_EDIT_kcbp_ip, ctrTmp );
    m_clsCfg.set( "root.config.kcbp_svr_ip.<xmlattr>.value", ctrTmp.GetString() );
    GetDlgItemText( IDC_EDIT_kcbp_svr_port, ctrTmp );
    m_clsCfg.set( "root.config.kcbp_svr_port.<xmlattr>.value", ctrTmp.GetString() );
    GetDlgItemText( IDC_EDIT_send_queue_name, ctrTmp );
    m_clsCfg.set( "root.config.send_queue_name.<xmlattr>.value", ctrTmp.GetString() );
    GetDlgItemText( IDC_EDIT_recv_queue_name, ctrTmp );
    m_clsCfg.set( "root.config.recv_queue_name.<xmlattr>.value", ctrTmp.GetString() );
    GetDlgItemText( IDC_EDIT_user_name, ctrTmp );
    m_clsCfg.set( "root.config.user_name.<xmlattr>.value", ctrTmp.GetString() );
    GetDlgItemText( IDC_EDIT_user_pwd, ctrTmp );
    m_clsCfg.set( "root.config.user_pwd.<xmlattr>.value", ctrTmp.GetString() );
    m_clsCfg.savefile( XML_CFG_PATH );
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
    lbmInfo::iterator it = m_liLBM.find( lbm_no.GetString() );
    if( it != m_liLBM.end() )
    {
        SetDlgItemText( IDC_EDIT_LBM_PARAMLIST, it->second.lbm_Param.c_str() );
    }
}

void CKCBP_UI_ClientDlg::OnBnClickedButtoncalllbm()
{
    CString lbm_no;
    GetDlgItemText( IDC_COMBO_LBM_NO, lbm_no );
    CString lbm_param;
    GetDlgItemText( IDC_EDIT_LBM_PARAMLIST, lbm_param );
    if( lbm_no.IsEmpty() )
    {
        AfxMessageBox( "没有选中LBM" );
        return;
    }
    lbm_call_cfg lbm;
    lbm.lbm_Id = lbm_no.GetString();
    lbm.lbm_Param = lbm_param.GetString();
    CallLbm( lbm );
}

