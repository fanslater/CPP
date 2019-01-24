#ifndef __FTP_CURL_H__
#define __FTP_CURL_H__

#include <string>
#include "curl/curl.h"
#include <sys/stat.h>

#define IN
#define OUT
#define INOUT

#pragma comment (lib,"ws2_32.lib")

class CAgreementClient
{
public:
    CAgreementClient( void );
    ~CAgreementClient( void );

private:
    bool m_bDebug;

public:
    int http_sync_post( IN const std::string& strUrl, IN const std::string& strPost, IN int iConnectTimeOut, IN int iTransmitTimeOut, OUT std::string& strResponse, OUT std::string& strRetErr );
    int http_sync_get( IN const std::string& strUrl, IN int iConnectTimeOut, IN int iTransmitTimeOut, OUT std::string& strResponse, OUT std::string& strRetErr );

    int https_sync_post( IN const std::string& strUrl, IN const std::string& strPost, IN int iConnectTimeOut, IN int iTransmitTimeOut, OUT std::string& strResponse, OUT std::string& strRetErr, IN const char* pCaPath = NULL );
    int https_sync_get( IN const std::string& strUrl, IN int iConnectTimeOut, IN int iTransmitTimeOut, OUT std::string& strResponse, OUT std::string& strRetErr, IN const char* pCaPath = NULL );

    int ftp_upload( IN const std::string& strRemotePath, IN const std::string& strLocalPath, IN const int iConnectTimeOut, IN const std::string& strUserName = "", IN const std::string& strUserPwd = "" );
    int ftp_download( IN const std::string& strRemotePath, IN const std::string& strLocalPath, IN const int iConnectTimeOut, IN const std::string& strUserName = "", IN const std::string& strUserPwd = "" );

    int tcp_sync_socket( IN const std::string& strIp, IN const int iPort, IN const std::string& strSend, IN int iConnectTimeOut, IN int iTransmitTimeOut, OUT std::string& strResponse );
public:
    void SetDebug( bool bDebug );
    static std::string get_error_detail( CURLcode code );
};

#endif
