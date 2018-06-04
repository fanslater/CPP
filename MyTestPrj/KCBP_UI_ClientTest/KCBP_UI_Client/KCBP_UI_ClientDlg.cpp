
// KCBP_UI_ClientDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "KCBP_UI_Client.h"
#include "KCBP_UI_ClientDlg.h"

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


// CKCBP_UI_ClientDlg �Ի���




CKCBP_UI_ClientDlg::CKCBP_UI_ClientDlg(CWnd *pParent /*=NULL*/)
    : CDialog(CKCBP_UI_ClientDlg::IDD, pParent)
{
    m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CKCBP_UI_ClientDlg::DoDataExchange(CDataExchange *pDX)
{
    CDialog::DoDataExchange(pDX);
    DDX_Control(pDX, IDC_LIST_Input_param, m_list_ctrl_result_set);
    DDX_Control(pDX, IDC_COMBO_LBM_NO, m_cbx_lbm_no);
}

BEGIN_MESSAGE_MAP(CKCBP_UI_ClientDlg, CDialog)
    ON_WM_SYSCOMMAND()
    ON_WM_PAINT()
    ON_WM_QUERYDRAGICON()
    //}}AFX_MSG_MAP
    ON_WM_CLOSE()
    ON_BN_CLICKED(IDC_BUTTON_save_connect_cfg, &CKCBP_UI_ClientDlg::OnBnClickedButtonsaveconnectcfg)
    ON_BN_CLICKED(IDC_BUTTON_connect_svr, &CKCBP_UI_ClientDlg::OnBnClickedButtonconnectsvr)
    ON_BN_CLICKED(IDC_BUTTON_disconnect_svr, &CKCBP_UI_ClientDlg::OnBnClickedButtondisconnectsvr)
    ON_CBN_SELCHANGE(IDC_COMBO_LBM_NO, &CKCBP_UI_ClientDlg::OnCbnSelchangeComboLbmNo)
    ON_BN_CLICKED(IDC_BUTTON_call_lbm, &CKCBP_UI_ClientDlg::OnBnClickedButtoncalllbm)
END_MESSAGE_MAP()


// CKCBP_UI_ClientDlg ��Ϣ�������

BOOL CKCBP_UI_ClientDlg::OnInitDialog()
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
    init();

    return TRUE;  // ���ǽ��������õ��ؼ������򷵻� TRUE
}

void CKCBP_UI_ClientDlg::OnSysCommand(UINT nID, LPARAM lParam)
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

void CKCBP_UI_ClientDlg::OnPaint()
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
HCURSOR CKCBP_UI_ClientDlg::OnQueryDragIcon()
{
    return static_cast<HCURSOR>(m_hIcon);
}


void CKCBP_UI_ClientDlg::init()
{
    m_kcbp_handle = NULL;

    modifyWindowsShow(TRUE);
    //open_console();
    m_clsCfg.loadfile(xml_file, XML_CFG_PATH);

    //�ڽ�����ʾ�����ļ��е�ֵ
    CString ctrTmp = m_clsCfg.get("root.config.kcbp_svr_name.<xmlattr>.value").c_str();
    SetDlgItemText(IDC_EDIT_kcbp_svr_name, ctrTmp);
    ctrTmp = m_clsCfg.get("root.config.kcbp_svr_ip.<xmlattr>.value").c_str();
    SetDlgItemText(IDC_IPADDRESS_kcbp_svr_ip, ctrTmp);
    ctrTmp = m_clsCfg.get("root.config.kcbp_svr_port.<xmlattr>.value").c_str();
    SetDlgItemText(IDC_EDIT_kcbp_svr_port, ctrTmp);
    ctrTmp = m_clsCfg.get("root.config.send_queue_name.<xmlattr>.value").c_str();
    SetDlgItemText(IDC_EDIT_send_queue_name, ctrTmp);
    ctrTmp = m_clsCfg.get("root.config.recv_queue_name.<xmlattr>.value").c_str();
    SetDlgItemText(IDC_EDIT_recv_queue_name, ctrTmp);
    ctrTmp = m_clsCfg.get("root.config.user_name.<xmlattr>.value").c_str();
    SetDlgItemText(IDC_EDIT_user_name, ctrTmp);
    ctrTmp = m_clsCfg.get("root.config.user_pwd.<xmlattr>.value").c_str();
    SetDlgItemText(IDC_EDIT_user_pwd, ctrTmp);


    //�������ÿؼ����
    DWORD dwStyle = ::GetWindowLong(m_list_ctrl_result_set.GetSafeHwnd(), GWL_STYLE);
    dwStyle |= LVS_SINGLESEL;       //ֻ�ɵ���ѡ��
    dwStyle |= LVS_SHOWSELALWAYS;   //Always show selection
    ::SetWindowLong(m_list_ctrl_result_set.GetSafeHwnd(), GWL_STYLE, dwStyle);
    DWORD dwStyleEx = m_list_ctrl_result_set.GetExtendedStyle();
    dwStyleEx |= LVS_EX_GRIDLINES;        //������
    dwStyleEx |= LVS_EX_FULLROWSELECT;    //���и���
    //dwStyleEx |= LVS_EX_CHECKBOXES;       //Itemǰ����check box
    m_list_ctrl_result_set.SetExtendedStyle(dwStyleEx);

    //��ʼ��lbmcbx
    m_ctrDefaultParam = m_clsCfg.get("root.lbms.<xmlattr>.DefParam").c_str();
    boost::property_tree::ptree xml = m_clsCfg.getTree();
    m_lbm_map.clear();
    BOOST_FOREACH(boost::property_tree::ptree::value_type & kv, xml.get_child("root.lbms"))
    {
        try
        {
            lbm_call_cfg lcc;
            lcc.lbm_Id = kv.second.get<string>("<xmlattr>.Id");
            lcc.lbm_NeedDef = kv.second.get<string>("<xmlattr>.NeedDef");
            lcc.lbm_Param = kv.second.get<string>("<xmlattr>.Param");
            m_cbx_lbm_no.InsertString(m_cbx_lbm_no.GetCount(), lcc.lbm_Id.c_str());
            m_lbm_map.insert(lbmPair(lcc.lbm_Id, lcc));
        }
        catch(boost::property_tree::ptree_error ex)
        {
            //AfxMessageBox(ex.what());
            OutputDebugStringA(ex.what());
        }
        catch (std::exception &ex)
        {
            //AfxMessageBox(ex.what());
            OutputDebugStringA(ex.what());
        }
    }
}

void CKCBP_UI_ClientDlg::uninit()
{
    m_kcbp_handle = NULL;
    //close_console();
}

void CKCBP_UI_ClientDlg::OnClose()
{
    uninit();

    CDialog::OnClose();
}

void CKCBP_UI_ClientDlg::open_console()
{
    AllocConsole();
    freopen("CONOUT$", "w+t", stdout);
    freopen("CONIN$", "r+t", stdin);
}

void CKCBP_UI_ClientDlg::close_console()
{
    fclose(stdout);
    fclose(stdin);
    FreeConsole();
}


void CKCBP_UI_ClientDlg::showerror()
{
    int iErr = 0;
    char szErr[1024] = {0};
    KCBPCLI_GetErr(m_kcbp_handle, &iErr, szErr);
    CString ctrErr;
    ctrErr.Format("errcode=[%d] errmsg=[%s]", iErr, szErr);
    AfxMessageBox(ctrErr);
}

void CKCBP_UI_ClientDlg::printTree(boost::property_tree::ptree root)
{
    BOOST_FOREACH(boost::property_tree::ptree::value_type & v1, root)
    {
        //�����ǰ�ڵ�Ϊ���ԣ���ӡȫ��������Ϣ
        if(v1.first == "<xmlattr>")
        {
            BOOST_FOREACH(boost::property_tree::ptree::value_type & vAttr, v1.second)
            {
                cout << vAttr.first << "=" << vAttr.second.data() << " ";
            }
            printf("\n");
        }
        else
        {
            //���û���ӽڵ㣬���ӡ��ֵ��
            if (v1.second.empty())
            {
                std::cout << v1.first << "=" << v1.second.data() << std::endl;
            }
            else
            {
                //��ӡ��ǰ�ڵ������
                std::cout << v1.first << std::endl;
                printTree(v1.second);
                if(v1.second.size() == 0x01)
                    std::cout << root.get<std::string>(v1.first) << std::endl;
            }
        }
    }
}

void CKCBP_UI_ClientDlg::call_lbm(lbm_call_cfg &lbm)
{
    lbmInfo::iterator itLBM = m_lbm_map.find(lbm.lbm_Id);
    if (itLBM == m_lbm_map.end())
    {
        AfxMessageBox("lbm no found");
        return;
    }
    std::string strFullParam;
    if (itLBM->second.lbm_NeedDef == "1")
    {
        strFullParam = m_ctrDefaultParam.GetString();
    }
    strFullParam.append(lbm.lbm_Param);
    kvmap paramlist;
    AnalysisInput(strFullParam, paramlist);

    if (m_kcbp_handle == NULL)
    {
        AfxMessageBox("KCBP���Ϊ��");
        return;
    }

    int iRet = 0;
    iRet = KCBPCLI_BeginWrite(m_kcbp_handle);
    if (iRet != 0)
    {
        showerror();
        return;
    }

    //д�����
    kvmap::iterator it;
    for (it = paramlist.begin(); it != paramlist.end(); it ++)
    {
        KCBPCLI_SetValue(m_kcbp_handle, (char *)it->first.c_str(), (char *)it->second.c_str());
    }

    //�������
    iRet = KCBPCLI_CallProgramAndCommit(m_kcbp_handle, (char *)lbm.lbm_Id.c_str());
    if(iRet != 0)
    {
        showerror();
        return;
    }

    //�򿪽��
    iRet = KCBPCLI_RsOpen(m_kcbp_handle);
    if(iRet != 0)
    {
        showerror();
        return;
    }

    //��ʾ���
    ShowLBMResult();

    //�رս��
    iRet = KCBPCLI_RsClose(m_kcbp_handle);
    if(iRet != 0)
    {
        showerror();
        return;
    }
}

void CKCBP_UI_ClientDlg::AnalysisInput(const string input, kvmap &paramlist)
{
    string src(input);
    if (src.length() == 0)
    {
        return;
    }

    int iFind = 0;
    while(1)
    {
        int iPos = src.find(",", iFind);
        if (iPos == -1)
        {
            break;
        }
        string one_param = trim(src.substr(iFind, iPos - iFind));
        int iSeparate = one_param.find(":");
        if (iSeparate > 0)
        {
            string key = trim(one_param.substr(0, iSeparate));
            string value = trim(one_param.substr(iSeparate + 1));
            paramlist.insert(kvpair(key, value));
        }
        iFind = iPos + 1;
    }
}

string &CKCBP_UI_ClientDlg::trim(string &s)
{
    if (s.empty())
    {
        return s;
    }

    s.erase(0, s.find_first_not_of(" "));
    s.erase(s.find_last_not_of(" ") + 1);
    return s;
}


//for (int i = 0 ; i < 10 ; i ++)
//{
//	m_list_ctrl_input_param.InsertColumn(i,"field",LVCFMT_LEFT,LISTCTRL_WIDTH);
//	m_list_ctrl_input_param.SetColumnCtrlType(i,CCT_EDITBOX);
//}

//int nItem = 0;
//nItem = m_list_ctrl_input_param.InsertItem(m_list_ctrl_input_param.GetItemCount(), TEXT("WangYao"), 0);
//m_list_ctrl_input_param.SetItemText(nItem, 1, TEXT("26"));   //�����1��ʼ,0��������
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

void CKCBP_UI_ClientDlg::ShowLBMResult()
{
    //���������ؼ�
    m_list_ctrl_result_set.DeleteAllItems();
    int nCols = m_list_ctrl_result_set.GetHeaderCtrl()->GetItemCount();
    for (int i = 0 ; i < nCols ; i ++)
    {
        m_list_ctrl_result_set.DeleteColumn(0);
    }

    m_list_ctrl_result_set.InsertColumn(0, "�к�", LVCFMT_LEFT, LISTCTRL_WIDTH);
    //m_list_ctrl_result_set.SetColumnWidth(0,LVSCW_AUTOSIZE);
    m_list_ctrl_result_set.SetColumnCtrlType(0, CCT_EDITBOX);
    int iPianYi = 0;
    do	//�����ѭ��
    {
        int iColNum = 0, iRowNum = 0;
        KCBPCLI_RsGetColNum(m_kcbp_handle, &iColNum);
        KCBPCLI_RsGetRowNum(m_kcbp_handle, &iRowNum);

        //��������
        for(int i = 1; i <= iColNum; i++)
        {
            char szFieldName[1024] = {0};
            KCBPCLI_RsGetColName(m_kcbp_handle, i, szFieldName, sizeof(szFieldName));
            int iIndex = m_list_ctrl_result_set.GetHeaderCtrl()->GetItemCount();
            m_list_ctrl_result_set.InsertColumn(iIndex, szFieldName, LVCFMT_LEFT, LISTCTRL_WIDTH);
            //m_list_ctrl_result_set.SetColumnWidth(iIndex,LVSCW_AUTOSIZE);
            m_list_ctrl_result_set.SetColumnCtrlType(iIndex, CCT_EDITBOX);
        }

        int iRowId = 0;
        while(1)	//�����������ѭ��
        {
            if (0 != KCBPCLI_RsFetchRow(m_kcbp_handle))
            {
                break;
            }
            char szRowId[16] = {0};
            sprintf(szRowId, "%d", iRowId + 1);
            iRowId = m_list_ctrl_result_set.InsertItem(m_list_ctrl_result_set.GetItemCount(), szRowId);
            char Result[1024] = {0};
            //��ӡ��ֵ
            for(int iColId = 1; iColId <= iColNum; iColId++)	//�����������ѭ��
            {
                KCBPCLI_RsGetCol(m_kcbp_handle, iColId, Result);
                m_list_ctrl_result_set.SetItemText(iRowId, iColId + iPianYi, Result);
            }
        }
        iPianYi += iColNum;
    }
    while(0 == KCBPCLI_RsMore(m_kcbp_handle));
}


void CKCBP_UI_ClientDlg::modifyWindowsShow(BOOL bflag)
{
    GetDlgItem(IDC_BUTTON_connect_svr)->EnableWindow(bflag);
    GetDlgItem(IDC_BUTTON_disconnect_svr)->EnableWindow(!bflag);

    GetDlgItem(IDC_IPADDRESS_kcbp_svr_ip)->EnableWindow(bflag);
    GetDlgItem(IDC_EDIT_kcbp_svr_port)->EnableWindow(bflag);
    GetDlgItem(IDC_EDIT_kcbp_svr_name)->EnableWindow(bflag);
    GetDlgItem(IDC_EDIT_send_queue_name)->EnableWindow(bflag);
    GetDlgItem(IDC_EDIT_recv_queue_name)->EnableWindow(bflag);
    GetDlgItem(IDC_EDIT_user_name)->EnableWindow(bflag);
    GetDlgItem(IDC_EDIT_user_pwd)->EnableWindow(bflag);
}

void CKCBP_UI_ClientDlg::OnBnClickedButtonsaveconnectcfg()
{
    //�ѽ����ϵ�����д���ļ�
    CString ctrTmp;
    GetDlgItemText(IDC_EDIT_kcbp_svr_name, ctrTmp);
    m_clsCfg.set("root.config.kcbp_svr_name.<xmlattr>.value", ctrTmp.GetString());
    GetDlgItemText(IDC_IPADDRESS_kcbp_svr_ip, ctrTmp);
    m_clsCfg.set("root.config.kcbp_svr_ip.<xmlattr>.value", ctrTmp.GetString());
    GetDlgItemText(IDC_EDIT_kcbp_svr_port, ctrTmp);
    m_clsCfg.set("root.config.kcbp_svr_port.<xmlattr>.value", ctrTmp.GetString());
    GetDlgItemText(IDC_EDIT_send_queue_name, ctrTmp);
    m_clsCfg.set("root.config.send_queue_name.<xmlattr>.value", ctrTmp.GetString());
    GetDlgItemText(IDC_EDIT_recv_queue_name, ctrTmp);
    m_clsCfg.set("root.config.recv_queue_name.<xmlattr>.value", ctrTmp.GetString());
    GetDlgItemText(IDC_EDIT_user_name, ctrTmp);
    m_clsCfg.set("root.config.user_name.<xmlattr>.value", ctrTmp.GetString());
    GetDlgItemText(IDC_EDIT_user_pwd, ctrTmp);
    m_clsCfg.set("root.config.user_pwd.<xmlattr>.value", ctrTmp.GetString());

    m_clsCfg.savefile(XML_CFG_PATH);
}

void CKCBP_UI_ClientDlg::OnBnClickedButtonconnectsvr()
{
    CString kcbp_svr_name, kcbp_svr_protocal, kcbp_svr_ip, kcbp_svr_port, send_queue_name, recv_queue_name, user_name, user_pwd;
    kcbp_svr_protocal = m_clsCfg.get("root.config.kcbp_svr_protocal.<xmlattr>.value").c_str();
    GetDlgItemText(IDC_EDIT_kcbp_svr_name, kcbp_svr_name);
    GetDlgItemText(IDC_IPADDRESS_kcbp_svr_ip, kcbp_svr_ip);
    GetDlgItemText(IDC_EDIT_kcbp_svr_port, kcbp_svr_port);
    GetDlgItemText(IDC_EDIT_send_queue_name, send_queue_name);
    GetDlgItemText(IDC_EDIT_recv_queue_name, recv_queue_name);
    GetDlgItemText(IDC_EDIT_user_name, user_name);
    GetDlgItemText(IDC_EDIT_user_pwd, user_pwd);

    int iRet = 0;
    if (m_kcbp_handle != NULL)
    {
        AfxMessageBox("KCBP����ǿգ�");
        return;
    }

    //��ʼ��
    iRet = KCBPCLI_Init(&m_kcbp_handle);
    if(iRet != 0)
    {
        showerror();
        return;
    }

    //���ò���
    tagKCBPConnectOption stKCBPConnection = {0};
    strcpy(stKCBPConnection.szServerName, kcbp_svr_name.GetString());
    stKCBPConnection.nProtocal = atoi(kcbp_svr_protocal.GetString());
    strcpy(stKCBPConnection.szAddress, kcbp_svr_ip.GetString());
    stKCBPConnection.nPort = atoi(kcbp_svr_port.GetString());
    strcpy(stKCBPConnection.szSendQName, send_queue_name.GetString());
    strcpy(stKCBPConnection.szReceiveQName, recv_queue_name.GetString());
    iRet = KCBPCLI_SetOptions(m_kcbp_handle, KCBP_OPTION_CONNECT, &stKCBPConnection, sizeof(stKCBPConnection));
    if(iRet != 0)
    {
        showerror();
        return;
    }

    //���ӷ���
    iRet = KCBPCLI_ConnectServer(m_kcbp_handle, (char *)kcbp_svr_name.GetString(), (char *)user_name.GetString(), (char *)user_pwd.GetString());
    if(iRet != 0)
    {
        showerror();
        KCBPCLI_Exit(m_kcbp_handle);
        return ;
    }

    //���Ӱ�ť�ûң��Ͽ���ť��ԭ
    modifyWindowsShow(FALSE);

}

void CKCBP_UI_ClientDlg::OnBnClickedButtondisconnectsvr()
{
    int iRet = 0;
    if (m_kcbp_handle == NULL)
    {
        AfxMessageBox("KCBP���Ϊ�գ�");
        return;
    }

    //�Ͽ�
    iRet = KCBPCLI_DisConnect(m_kcbp_handle);
    if(iRet != 0)
    {
        showerror();
        KCBPCLI_Exit(m_kcbp_handle);
        return;
    }

    //����ʼ��
    iRet = KCBPCLI_Exit(m_kcbp_handle);
    if(iRet != 0)
    {
        showerror();
        return;
    }
    m_kcbp_handle = NULL;

    //���Ӱ�ť�ûң��Ͽ���ť��ԭ
    modifyWindowsShow(TRUE);
}

void CKCBP_UI_ClientDlg::OnCbnSelchangeComboLbmNo()
{
    CString lbm_no;
    m_cbx_lbm_no.GetLBText(m_cbx_lbm_no.GetCurSel(), lbm_no);
    lbmInfo::iterator it = m_lbm_map.find(lbm_no.GetString());
    if (it != m_lbm_map.end())
    {
        SetDlgItemText(IDC_EDIT_LBM_PARAMLIST, it->second.lbm_Param.c_str());
    }
}

void CKCBP_UI_ClientDlg::OnBnClickedButtoncalllbm()
{
    CString lbm_no;
    GetDlgItemText(IDC_COMBO_LBM_NO, lbm_no);
    CString lbm_param;
    GetDlgItemText(IDC_EDIT_LBM_PARAMLIST, lbm_param);
    if (lbm_no.IsEmpty())
    {
        AfxMessageBox("û��ѡ��LBM");
        return;
    }

    lbm_call_cfg lbm;
    lbm.lbm_Id = lbm_no.GetString();
    lbm.lbm_Param = lbm_param.GetString();
    call_lbm(lbm);
}

