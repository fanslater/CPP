
// IOCP_MFC_SERVERDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "IOCP_MFC_SERVER.h"
#include "IOCP_MFC_SERVERDlg.h"

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
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

// 实现
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialog(CAboutDlg::IDD)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
END_MESSAGE_MAP()


// CIOCP_MFC_SERVERDlg 对话框




CIOCP_MFC_SERVERDlg::CIOCP_MFC_SERVERDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CIOCP_MFC_SERVERDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CIOCP_MFC_SERVERDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}

void CIOCP_MFC_SERVERDlg::init()
{
	//【第一步】创建一个完成端口
	//对于最后一个参数 0，我这里要简单的说两句，这个0可不是一个普通的0，它代表的是NumberOfConcurrentThreads，
	//也就是说，允许应用程序同时执行的线程数量。当然，我们这里为了避免上下文切换，最理想的状态就是每个处理器上只运行一个线程了，
	//所以我们设置为0，就是说有多少个处理器，就允许同时多少个线程运行。
	HANDLE m_hIOCompletionPort = CreateIoCompletionPort(INVALID_HANDLE_VALUE, NULL, 0, 0 );

	//【第二步】根据系统中CPU核心的数量建立对应的Worker线程
	SYSTEM_INFO si;  
	GetSystemInfo(&si);  
	int m_nProcessors = si.dwNumberOfProcessors;
	// 根据CPU数量，建立*2的线程  
	m_nThreads = 2 * m_nProcessors;  
	HANDLE* m_phWorkerThreads = new HANDLE[m_nThreads];  
	for (int i = 0; i < m_nThreads; i++)  
	{  
		m_phWorkerThreads[i] = ::CreateThread(0, 0, _WorkerThread, …);  
	}  

	//【第三步】创建一个用于监听的Socket，绑定到完成端口上，然后开始在指定的端口上监听连接请求
	// 初始化Socket库  
	WSADATA wsaData;  
	WSAStartup(MAKEWORD(2,2), &wsaData);  
	//初始化Socket  
	struct sockaddr_in ServerAddress;  
	// 这里需要特别注意，如果要使用重叠I/O的话，这里必须要使用WSASocket来初始化Socket  
	// 注意里面有个WSA_FLAG_OVERLAPPED参数  
	SOCKET m_sockListen = WSASocket(AF_INET, SOCK_STREAM, 0, NULL, 0, WSA_FLAG_OVERLAPPED);  
	// 填充地址结构信息  
	ZeroMemory((char *)&ServerAddress, sizeof(ServerAddress));  
	ServerAddress.sin_family = AF_INET;  
	// 这里可以选择绑定任何一个可用的地址，或者是自己指定的一个IP地址   
	//ServerAddress.sin_addr.s_addr = htonl(INADDR_ANY);                        
	ServerAddress.sin_addr.s_addr = inet_addr("192.168.205.214");           
	ServerAddress.sin_port = htons(11111);                            
	// 绑定端口  
	if (SOCKET_ERROR == bind(m_sockListen, (struct sockaddr *) &ServerAddress, sizeof(ServerAddress)))   
	{	// 开始监听  
		listen(m_sockListen,SOMAXCONN));
	}
	CreateIoCompletionPort(m_sockListen,m_hIOCompletionPort,NULL,0);

	//【第四步】在这个监听Socket上投递AcceptEx请求
	LPFN_ACCEPTEX     m_lpfnAcceptEx;         // AcceptEx函数指针  
	GUID GuidAcceptEx = WSAID_ACCEPTEX;        // GUID，这个是识别AcceptEx函数必须的  
	DWORD dwBytes = 0;    

	WSAIoctl(  
		m_pListenContext->m_Socket,   
		SIO_GET_EXTENSION_FUNCTION_POINTER,   
		&GuidAcceptEx,   
		sizeof(GuidAcceptEx),   
		&m_lpfnAcceptEx,   
		sizeof(m_lpfnAcceptEx),   
		&dwBytes,   
		NULL,   
		NULL);  
	BOOL AcceptEx (       
		SOCKET sListenSocket,   
		SOCKET sAcceptSocket,   
		PVOID lpOutputBuffer,   
		DWORD dwReceiveDataLength,   
		DWORD dwLocalAddressLength,   
		DWORD dwRemoteAddressLength,   
		LPDWORD lpdwBytesReceived,   
		LPOVERLAPPED lpOverlapped   
		);
}

void CIOCP_MFC_SERVERDlg::uninit()
{
	
}

BEGIN_MESSAGE_MAP(CIOCP_MFC_SERVERDlg, CDialog)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	//}}AFX_MSG_MAP
	ON_WM_CLOSE()
END_MESSAGE_MAP()


// CIOCP_MFC_SERVERDlg 消息处理程序

BOOL CIOCP_MFC_SERVERDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// 将“关于...”菜单项添加到系统菜单中。

	// IDM_ABOUTBOX 必须在系统命令范围内。
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
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
	init();

	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

void CIOCP_MFC_SERVERDlg::OnSysCommand(UINT nID, LPARAM lParam)
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

void CIOCP_MFC_SERVERDlg::OnPaint()
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
HCURSOR CIOCP_MFC_SERVERDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}


void CIOCP_MFC_SERVERDlg::OnClose()
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	uninit();

	CDialog::OnClose();
}
