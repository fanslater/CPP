// IOCPServer.cpp : �������̨Ӧ�ó������ڵ㡣
//

#include "stdafx.h"


/** @defgroup IOCP
*  @author  ��Ծ
*  @version 1.0
*  @date    2013-08-05
*  @{
*/

#pragma comment (lib,"ws2_32.lib")

#include <winsock2.h>
#include <stdio.h>
#include <iostream>
using namespace std;


/**  ���ݻ�������С */
#define BUFFER_SIZE 1024*4

/** @name ����״̬����
*  @{ */
/**  ������ */
#define OP_READ    1
/**  д���� */
#define OP_WRITE   2
/**  ���ղ��� */ 
#define OP_ACCEPT  3
/** @}*/

/**  ÿһ�����Ӵ������� */
typedef struct _PER_HANDLE_DATA
{
	/**  ��Ӧ���׽��־�� */
	SOCKET      s;      
	/**  �Է��ĵ�ַ */
	sockaddr_in addr;  
}PER_HANDLE_DATA, *PPER_HANDLE_DATA;

/**  ÿһ��IO���ݽṹ */
typedef struct _PER_IO_DATA
{ 
	/**  �ص��ṹ */
	OVERLAPPED  ol;
	/**  ���ݻ����� */
	char        buf[BUFFER_SIZE]; 
	/**  �������� */
	int         nOperationType;   
	/**  HTTP��������ݳ��� */
	int         nHttpPackLen;   		
}PER_IO_DATA, *PPER_IO_DATA;
typedef struct
{ 
	char szAnsDataPath[256];
	HANDLE hIocp;  
}structMainData;


std::string GBKToUTF8(const std::string& strGBK)
{
	int nLen = MultiByteToWideChar(CP_ACP, 0, strGBK.c_str(), -1, NULL, 0);
	WCHAR * wszUTF8 = new WCHAR[nLen];
	MultiByteToWideChar(CP_ACP, 0, strGBK.c_str(), -1, wszUTF8, nLen);

	nLen = WideCharToMultiByte(CP_UTF8, 0, wszUTF8, -1, NULL, 0, NULL, NULL);
	char * szUTF8 = new char[nLen];
	WideCharToMultiByte(CP_UTF8, 0, wszUTF8, -1, szUTF8, nLen, NULL, NULL);

	std::string strTemp(szUTF8);
	delete[]wszUTF8;
	delete[]szUTF8;
	return strTemp;
}

std::string UTF8ToGBK(const std::string& strUTF8)
{
	int nLen = MultiByteToWideChar(CP_UTF8, 0, strUTF8.c_str(), -1, NULL, 0);
	unsigned short * wszGBK = new unsigned short[nLen + 1];
	memset(wszGBK, 0, nLen * 2 + 2);
	MultiByteToWideChar(CP_UTF8, 0, strUTF8.c_str(), -1, (LPWSTR)wszGBK, nLen);

	nLen = WideCharToMultiByte(CP_ACP, 0, (LPWSTR)wszGBK, -1, NULL, 0, NULL, NULL);
	char *szGBK = new char[nLen + 1];
	memset(szGBK, 0, nLen + 1);
	WideCharToMultiByte(CP_ACP,0, (LPWSTR)wszGBK, -1, szGBK, nLen, NULL, NULL);

	std::string strTemp(szGBK);
	delete[]szGBK;
	delete[]wszGBK;
	return strTemp;
}
void DebugLog(char *szFormatter, ...)
{
	va_list vp;
	char szMsg[1024];
	szMsg[sizeof(szMsg) - 1] = 0;

	SYSTEMTIME systime;		
	GetLocalTime(&systime);
	sprintf(szMsg, "[%02d:%02d:%02d:%03d] [0x%04x] ",
		systime.wHour, systime.wMinute, systime.wSecond, systime.wMilliseconds,GetCurrentThreadId());

	va_start(vp, szFormatter);
	_vsnprintf(&szMsg[strlen(szMsg)], sizeof(szMsg) - 3, szFormatter, vp);
	va_end(vp);
	strcat(szMsg,"\n");

	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_INTENSITY | FOREGROUND_RED);
	printf(szMsg);
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);	
}

/** �����̴߳�����ɶ˿ڶ�����¼�
*  @param[in] lpParam ��ɶ˿ڶ�����
*  @return �����߳̽���״̬ 
*/
DWORD WINAPI ServerThread( LPVOID lpParam )
{
	structMainData *pstMainData = (structMainData*)lpParam;
	if( pstMainData->hIocp == NULL )
	{
		return -1;
	}

	PPER_HANDLE_DATA pPerHandle;
	PPER_IO_DATA     pPerIo;

	WSABUF wsabuf;
	DWORD dwTrans = 0, dwRecv = 0, dwFlags = 0, dwSend = 0; 

	while( TRUE )
	{
		// �ڹ���������ɶ˿ڵ������׽����ϵȴ�I/O���
		BOOL bRet = ::GetQueuedCompletionStatus( pstMainData->hIocp, &dwTrans, (LPDWORD)&pPerHandle, (LPOVERLAPPED*)&pPerIo, WSA_INFINITE );
		if( !bRet ) // ��������
		{
			::closesocket( pPerHandle->s );
			memset(pPerIo, 0, sizeof(PER_IO_DATA));
			::GlobalFree( pPerHandle );
			::GlobalFree( pPerIo );

			DebugLog("GetQueuedCompletionStatus return:%d,pPerHandle:%08x,pPerIo:%08x", bRet,pPerHandle,pPerIo);
			continue;
		}

		// �׽��ֱ��Է��ر�)
		if( dwTrans == 0 && ( pPerIo->nOperationType == OP_READ || pPerIo->nOperationType == OP_WRITE ) )
		{
			::closesocket( pPerHandle->s );			
			memset(pPerIo, 0, sizeof(PER_IO_DATA));
			::GlobalFree( pPerHandle );
			::GlobalFree( pPerIo );

			DebugLog("client closed:%d,pPerHandle:%08x,pPerIo:%08x", bRet,pPerHandle,pPerIo);
			continue;
		}

		switch ( pPerIo->nOperationType )
		{
		case OP_READ:  			
			{//��HTTP�ı���������
				char *pContentLen = strstr(pPerIo->buf, "Content-Length: ");
				char *pHeadLen = strstr(pPerIo->buf, "\r\n\r\n");
				if(pContentLen && pHeadLen)
				{
					pPerIo->nHttpPackLen = atoi(pContentLen+strlen("Content-Length: ")) + (int)(pHeadLen-pPerIo->buf)+4;
				}
			}

			if(strlen(pPerIo->buf) < pPerIo->nHttpPackLen)
			{
				// Ͷ�ݽ����������
				DWORD dwRecv = 0;
				DWORD dwFlags = 0;
				WSABUF buf;
				buf.buf = &pPerIo->buf[strlen(pPerIo->buf)];
				buf.len = BUFFER_SIZE;
				pPerIo->nOperationType = OP_READ;
				//�Զ���һ���ṹ�壬��ʼ���ݽṹΪOVERLAPPED������ϵͳ����OVERLAPPEDָ�룬�Ӷ��õ��Զ��������
				::WSARecv( pPerHandle->s, &buf, 1, &dwRecv, &dwFlags, (OVERLAPPED*)pPerIo, NULL);
			}
			else
			{// �������
				DebugLog( "Socket:%d,���գ�", ntohs(pPerHandle->addr.sin_port));
				printf("%s\n", pPerIo->buf);

				//--------------��ȡӦ�������ļ�����---------------------------
				FILE *fpFile;
				if ((fpFile = fopen(pstMainData->szAnsDataPath, "r+b")) != NULL)
				{
					memset(pPerIo->buf, 0, BUFFER_SIZE);
					int nReadLen = fread(pPerIo->buf, 1, BUFFER_SIZE-1, fpFile);
					fclose(fpFile);

					std::string strGBK = pPerIo->buf;
					std::string strUTF8 = GBKToUTF8(strGBK);
					strncpy(pPerIo->buf, strUTF8.c_str(), BUFFER_SIZE);
				}
				//--------------------------------------------------------------��

				//Sleep(1000);
				// Ͷ�ݷ��Ͳ��� 
				if(pPerIo->nOperationType)
				{
					pPerIo->nOperationType = OP_WRITE;  
					wsabuf.buf = pPerIo->buf;
					wsabuf.len = strlen(pPerIo->buf);
					::WSASend( pPerHandle->s, &wsabuf, 1, &dwSend, 0, &pPerIo->ol, NULL );
				}
			}
			break;
		case OP_WRITE:     
			//���������
			{        
				pPerIo->buf[dwTrans] = (char)0x0;
				DebugLog( "Socket:%d,Ӧ��", ntohs(pPerHandle->addr.sin_port));
				printf("%s\n", pPerIo->buf);    

				//body�е������ѷ������
				::closesocket( pPerHandle->s );
				::GlobalFree( pPerHandle );
				::GlobalFree( pPerIo );		
			}
			break;
		case OP_ACCEPT:
			break;

		}

	}

	return 0;
}

/** main����
*  @param[in] argc     ��������
*  @param[in] *argv[]  ����ָ��
*/
int _tmain(int argc, char* argv[])
{
	if(argc<4)
	{
		printf("please set listen address, listen, ansfilepath port by argv[]!\n");
		system("pause");
		return 0;
	}


	//��ȡCPU��Ŀ siSys.dwNumberOfProcessors
	SYSTEM_INFO siSys;
	GetSystemInfo(&siSys);
	int nProcessors = siSys.dwNumberOfProcessors;

	//////////////////////////////////////////////////////////////////////////  
	WSADATA wsaData;
	if( 0 != WSAStartup( MAKEWORD( 2, 2 ), &wsaData ) )
	{
		printf( "Using %s (Status:%s)\n", wsaData.szDescription, wsaData.szSystemStatus );
		printf( "with API versions: %d.%d to %d.%d",
			LOBYTE( wsaData.wVersion), HIBYTE( wsaData.wVersion ),
			LOBYTE( wsaData.wHighVersion), HIBYTE( wsaData.wHighVersion) );
		return -1;
	}
	printf("Windows sockets 2.2 startup\n");
	//////////////////////////////////////////////////////////////////////////

	// ������ɶ˿ڶ���
	structMainData stMainData;
	stMainData.hIocp = ::CreateIoCompletionPort( INVALID_HANDLE_VALUE, NULL, 0, nProcessors );
	cout<<"����һ��IO��ɶ˿�"<<endl;
	// ���������̴߳�����ɶ˿ڶ�����¼�
	for (int i=0; i<nProcessors; i++)
	{
		HANDLE hThread;
		hThread = ::CreateThread( NULL, 0, ServerThread, (LPVOID)&stMainData, 0, 0 );  
		CloseHandle(hThread);
		cout<<"�����������߳�"<<i<<endl;
	}

	// ���������׽���
	SOCKET sListen;
	char szAddr[32]={0};
	int nPort;
	int nReuseAddr = 1;
	strncpy(szAddr, argv[1], sizeof(szAddr)-1);
	nPort = atoi(argv[2]);
	strncpy(stMainData.szAnsDataPath, argv[3], sizeof(stMainData.szAnsDataPath)-1);	
	cout<<"��������SOCKET"<<endl;
	if ((sListen = WSASocket(AF_INET, SOCK_STREAM, 0, NULL, 0, WSA_FLAG_OVERLAPPED)) == INVALID_SOCKET)
	{
		cout<<"WSASocket����"<<endl;
		goto ERROR_END;
	}
	if(setsockopt(sListen,SOL_SOCKET,SO_REUSEADDR,(const char *)&nReuseAddr,sizeof(int)) != 0)
	{
		cout<<"setsockopt����"<<endl;
		goto ERROR_END;
	}
	SOCKADDR_IN LocalAddr;
	LocalAddr.sin_family = AF_INET;
	LocalAddr.sin_port = ::htons( nPort );
	LocalAddr.sin_addr.s_addr = inet_addr(szAddr);
	// �󶨱��ض˿�
	if (bind(sListen, (PSOCKADDR) &LocalAddr, sizeof(LocalAddr)) == SOCKET_ERROR)
	{
		int nError = WSAGetLastError();
		cout<<"bind����"<<nError<<endl;
		goto ERROR_END;
	}  
	cout<<"��"<<szAddr<<":"<<nPort<<endl;
	// ��ʼ����  
	if (listen(sListen, SOMAXCONN) == SOCKET_ERROR)
	{
		int nError = WSAGetLastError();
		cout<<"listen����"<<nError<<endl;
		goto ERROR_END;
	}
	cout<<"��ʼ����......"<<endl;

	// ѭ��������������
	while ( TRUE )
	{
		// �ȴ�������������
		SOCKADDR_IN saRemote;
		int nRemoteLen = sizeof( saRemote );
		SOCKET sRemote = ::accept( sListen, (sockaddr *)&saRemote, &nRemoteLen );

		// ���ܵ�������֮��Ϊ������һ��per_handle���ݣ��������ǹ�������ɶ˿ڶ���
		PPER_HANDLE_DATA pPerHandle = ( PPER_HANDLE_DATA )::GlobalAlloc( GPTR, sizeof( PER_HANDLE_DATA ) );
		if( pPerHandle == NULL )
		{
			DebugLog( "pPerHandle GlobalAlloc() failed!");
			break;
		}
		DebugLog( "GlobalAlloc pPerHandle:%08x", pPerHandle);
		pPerHandle->s = sRemote;
		memcpy( &pPerHandle->addr, &saRemote, nRemoteLen );
		::CreateIoCompletionPort( (HANDLE)pPerHandle->s, stMainData.hIocp, (DWORD)pPerHandle, 0 );


		// ����һ��pPerIo����, Ͷ��һ�������������
		PPER_IO_DATA pPerIo = ( PPER_IO_DATA )::GlobalAlloc( GPTR, sizeof( PER_IO_DATA ) );
		if( pPerIo == NULL )
		{			
			DebugLog( "pPerIo GlobalAlloc() failed!");
			break;
		}
		DebugLog( "GlobalAlloc saRemote.sin_port:%d, pPerIo:%08x", saRemote.sin_port,pPerIo);
		memset(pPerIo, 0, sizeof(PER_IO_DATA));

		// Ͷ�ݽ����������
		DWORD dwRecv = 0;
		DWORD dwFlags = 0;
		WSABUF buf;
		buf.buf = pPerIo->buf;
		buf.len = BUFFER_SIZE;  
		pPerIo->nOperationType = OP_READ;
		//�Զ���һ���ṹ�壬��ʼ���ݽṹΪOVERLAPPED������ϵͳ����OVERLAPPEDָ�룬�Ӷ��õ��Զ��������
		::WSARecv( pPerHandle->s, &buf, 1, &dwRecv, &dwFlags, (OVERLAPPED*)pPerIo, NULL);
	}

	//////////////////////////////////////////////////////////////////////////
ERROR_END:
	WSACleanup();
	//////////////////////////////////////////////////////////////////////////

	system("pause");
	return 0;
}

/* ���͵�HTTP����
POST /otcgw/services/messageUpdateNotify HTTP/1.1
Accept-Encoding: gzip,deflate
Content-Type: application/json
Content-Length: 0
Host: 127.0.0.1:8001
Connection: Keep-Alive
User-Agent: Apache-HttpClient/4.1.1 (java 1.5)
*/

/* ���͵�HTTPӦ��
HTTP/1.1 200 0K
Connectlon:close
Date: Thu, 13 Oct 2014 12:17:33 GMT
Server: Apache-Coyote/1.1
Last-Nodified:Mon,22 Jun 1998 09;23;24 GMT

<?xml version="1.0" encoding="UTF-8"?>
<response>
<SYSTEM_ID>OTC123</SYSTEM_ID>
<NOTIFY_FLAG>1</NOTIFY_FLAG>
</response>
*/





