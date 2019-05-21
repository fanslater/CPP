
// IOCP_MFC_SERVERDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "IOCP_MFC_SERVER.h"
#include "IOCP_MFC_SERVERDlg.h"

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
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

// ʵ��
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


// CIOCP_MFC_SERVERDlg �Ի���




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
	//����һ��������һ����ɶ˿�
	//�������һ������ 0��������Ҫ�򵥵�˵���䣬���0�ɲ���һ����ͨ��0�����������NumberOfConcurrentThreads��
	//Ҳ����˵������Ӧ�ó���ͬʱִ�е��߳���������Ȼ����������Ϊ�˱����������л����������״̬����ÿ����������ֻ����һ���߳��ˣ�
	//������������Ϊ0������˵�ж��ٸ���������������ͬʱ���ٸ��߳����С�
	HANDLE m_hIOCompletionPort = CreateIoCompletionPort(INVALID_HANDLE_VALUE, NULL, 0, 0 );

	//���ڶ���������ϵͳ��CPU���ĵ�����������Ӧ��Worker�߳�
	SYSTEM_INFO si;  
	GetSystemInfo(&si);  
	int m_nProcessors = si.dwNumberOfProcessors;
	// ����CPU����������*2���߳�  
	m_nThreads = 2 * m_nProcessors;  
	HANDLE* m_phWorkerThreads = new HANDLE[m_nThreads];  
	for (int i = 0; i < m_nThreads; i++)  
	{  
		m_phWorkerThreads[i] = ::CreateThread(0, 0, _WorkerThread, ��);  
	}  

	//��������������һ�����ڼ�����Socket���󶨵���ɶ˿��ϣ�Ȼ��ʼ��ָ���Ķ˿��ϼ�����������
	// ��ʼ��Socket��  
	WSADATA wsaData;  
	WSAStartup(MAKEWORD(2,2), &wsaData);  
	//��ʼ��Socket  
	struct sockaddr_in ServerAddress;  
	// ������Ҫ�ر�ע�⣬���Ҫʹ���ص�I/O�Ļ����������Ҫʹ��WSASocket����ʼ��Socket  
	// ע�������и�WSA_FLAG_OVERLAPPED����  
	SOCKET m_sockListen = WSASocket(AF_INET, SOCK_STREAM, 0, NULL, 0, WSA_FLAG_OVERLAPPED);  
	// ����ַ�ṹ��Ϣ  
	ZeroMemory((char *)&ServerAddress, sizeof(ServerAddress));  
	ServerAddress.sin_family = AF_INET;  
	// �������ѡ����κ�һ�����õĵ�ַ���������Լ�ָ����һ��IP��ַ   
	//ServerAddress.sin_addr.s_addr = htonl(INADDR_ANY);                        
	ServerAddress.sin_addr.s_addr = inet_addr("192.168.205.214");           
	ServerAddress.sin_port = htons(11111);                            
	// �󶨶˿�  
	if (SOCKET_ERROR == bind(m_sockListen, (struct sockaddr *) &ServerAddress, sizeof(ServerAddress)))   
	{	// ��ʼ����  
		listen(m_sockListen,SOMAXCONN));
	}
	CreateIoCompletionPort(m_sockListen,m_hIOCompletionPort,NULL,0);

	//�����Ĳ������������Socket��Ͷ��AcceptEx����
	LPFN_ACCEPTEX     m_lpfnAcceptEx;         // AcceptEx����ָ��  
	GUID GuidAcceptEx = WSAID_ACCEPTEX;        // GUID�������ʶ��AcceptEx���������  
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


// CIOCP_MFC_SERVERDlg ��Ϣ�������

BOOL CIOCP_MFC_SERVERDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// ��������...���˵�����ӵ�ϵͳ�˵��С�

	// IDM_ABOUTBOX ������ϵͳ���Χ�ڡ�
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

	// ���ô˶Ի����ͼ�ꡣ��Ӧ�ó��������ڲ��ǶԻ���ʱ����ܽ��Զ�
	//  ִ�д˲���
	SetIcon(m_hIcon, TRUE);			// ���ô�ͼ��
	SetIcon(m_hIcon, FALSE);		// ����Сͼ��

	// TODO: �ڴ���Ӷ���ĳ�ʼ������
	init();

	return TRUE;  // ���ǽ��������õ��ؼ������򷵻� TRUE
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

// �����Ի��������С����ť������Ҫ����Ĵ���
//  �����Ƹ�ͼ�ꡣ����ʹ���ĵ�/��ͼģ�͵� MFC Ӧ�ó���
//  �⽫�ɿ���Զ���ɡ�

void CIOCP_MFC_SERVERDlg::OnPaint()
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
HCURSOR CIOCP_MFC_SERVERDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}


void CIOCP_MFC_SERVERDlg::OnClose()
{
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ
	uninit();

	CDialog::OnClose();
}
