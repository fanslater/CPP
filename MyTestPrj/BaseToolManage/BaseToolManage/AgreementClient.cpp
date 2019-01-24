#include "stdafx.h"
#include "AgreementClient.h"

CAgreementClient::CAgreementClient( void )
{
    m_bDebug = false;
    CURLcode code = curl_global_init( CURL_GLOBAL_ALL );
    if( code != CURLE_OK )
    {
        printf( curl_easy_strerror( code ) );
    }
}


CAgreementClient::~CAgreementClient( void )
{
    curl_global_cleanup();
}


static int OnDebug( CURL*, curl_infotype itype, char* pData, size_t size, void* )
{
    if( itype == CURLINFO_TEXT )
    {
        //printf("[TEXT]%s\n", pData);
    }
    else if( itype == CURLINFO_HEADER_IN )
    {
        printf( "[HEADER_IN]%s\n", pData );
    }
    else if( itype == CURLINFO_HEADER_OUT )
    {
        printf( "[HEADER_OUT]%s\n", pData );
    }
    else if( itype == CURLINFO_DATA_IN )
    {
        printf( "[DATA_IN]%s\n", pData );
    }
    else if( itype == CURLINFO_DATA_OUT )
    {
        printf( "[DATA_OUT]%s\n", pData );
    }
    return 0;
}


/* Auxiliary function that waits on the socket. */
static int wait_on_socket( curl_socket_t sockfd, int for_recv, long timeout_ms )
{
    struct timeval tv;
    fd_set infd, outfd, errfd;
    int res;
    tv.tv_sec = timeout_ms / 1000;
    tv.tv_usec = ( timeout_ms % 1000 ) * 1000;
    FD_ZERO( &infd );
    FD_ZERO( &outfd );
    FD_ZERO( &errfd );
    FD_SET( sockfd, &errfd ); /* always check for error */
    if( for_recv )
    {
        FD_SET( sockfd, &infd );
    }
    else
    {
        FD_SET( sockfd, &outfd );
    }
    /* select() returns the number of signalled sockets or -1 */
    res = select( ( int )sockfd + 1, &infd, &outfd, &errfd, &tv );
    return res;
}


static size_t Http_OnWriteData( void* buffer, size_t size, size_t nmemb, void* lpVoid )
{
    std::string* str = dynamic_cast<std::string*>( ( std::string* )lpVoid );
    if( NULL == str || NULL == buffer )
    {
        return -1;
    }
    char* pData = ( char* )buffer;
    str->append( pData, size * nmemb );
    return nmemb;
}

/* parse headers for Content-Length */
static size_t Ftp_OnGetContentLength( void* ptr, size_t size, size_t nmemb, void* stream )
{
    int r;
    long len = 0;
    /* _snscanf() is Win32 specific */
    //r = _snscanf(ptr, size * nmemb, "Content-Length: %ld\n", &len);
    r = sscanf( ( const char* )ptr, "Content-Length: %ld\n", &len );
    if( r ) /* Microsoft: we don't read the specs */
        *( ( long* ) stream ) = len;
    return size * nmemb;
}


//write data to download
static size_t Ftp_OnWriteData( void* ptr, size_t size, size_t nmemb, void* stream )
{
    return fwrite( ptr, size, nmemb, ( FILE* )stream );
}

/* discard downloaded data */
static size_t Ftp_OnDiscard( void* ptr, size_t size, size_t nmemb, void* stream )
{
    return size * nmemb;
}

/* read data to upload */
static size_t Ftp_OnReadData( void* ptr, size_t size, size_t nmemb, void* stream )
{
    FILE* f = ( FILE* )stream;
    size_t n;
    if( ferror( f ) )
        return CURL_READFUNC_ABORT;
    n = fread( ptr, size, nmemb, f ) * size;
    return n;
}



int CAgreementClient::http_sync_post( IN const std::string& strUrl, IN const std::string& strPost, IN int iConnectTimeOut, IN int iTransmitTimeOut, OUT std::string& strResponse, OUT std::string& strRetErr )
{
    CURLcode res;
    CURL* curlHandle = curl_easy_init();
    if( NULL == curlHandle )
    {
        return CURLE_FAILED_INIT;
    }
    if( m_bDebug )
    {
        curl_easy_setopt( curlHandle, CURLOPT_VERBOSE, 1 );
        curl_easy_setopt( curlHandle, CURLOPT_DEBUGFUNCTION, OnDebug );
    }
    curl_easy_setopt( curlHandle, CURLOPT_URL, strUrl.c_str() );
    curl_easy_setopt( curlHandle, CURLOPT_POST, 1 );
    curl_easy_setopt( curlHandle, CURLOPT_POSTFIELDS, strPost.c_str() );
    curl_easy_setopt( curlHandle, CURLOPT_READFUNCTION, NULL );
    curl_easy_setopt( curlHandle, CURLOPT_WRITEFUNCTION, Http_OnWriteData );
    curl_easy_setopt( curlHandle, CURLOPT_WRITEDATA, ( void* )&strResponse );
    curl_easy_setopt( curlHandle, CURLOPT_NOSIGNAL, 1 );
    curl_easy_setopt( curlHandle, CURLOPT_CONNECTTIMEOUT, iConnectTimeOut );    //CURLOPT_CONNECTIONTIMEOUT 设置连接等待时间
    curl_easy_setopt( curlHandle, CURLOPT_TIMEOUT, iTransmitTimeOut );      //CURLOPT_TIMEOUT 由于设置传输时间
    //默认情况下libcurl完成一个任务以后，出于重用连接的考虑不会马上关闭
    //如果没有新的TCP请求来重用这个连接，那么只能等到CLOSE_WAIT超时，这个时间默认在7200秒甚至更高，太多的CLOSE_WAIT连接会导致性能问题
    //解决方法：        curl_easy_setopt(curl, CURLOPT_FORBID_REUSE, 1);
    //最好再修改一下TCP参数调低CLOSE_WAIT和TIME_WAIT的超时时间
    curl_easy_setopt( curlHandle, CURLOPT_FORBID_REUSE, 1 );
    res = curl_easy_perform( curlHandle );
    strRetErr = std::string( curl_easy_strerror( res ) );
    curl_easy_cleanup( curlHandle );
    return res;
}


int CAgreementClient::http_sync_get( IN const std::string& strUrl, IN int iConnectTimeOut, IN int iTransmitTimeOut, OUT std::string& strResponse, OUT std::string& strRetErr )
{
    CURLcode res;
    CURL* curlHandle = curl_easy_init();
    if( NULL == curlHandle )
    {
        return CURLE_FAILED_INIT;
    }
    if( m_bDebug )
    {
        curl_easy_setopt( curlHandle, CURLOPT_VERBOSE, 1 );
        curl_easy_setopt( curlHandle, CURLOPT_DEBUGFUNCTION, OnDebug );
    }
    curl_easy_setopt( curlHandle, CURLOPT_URL, strUrl.c_str() );
    curl_easy_setopt( curlHandle, CURLOPT_READFUNCTION, NULL );
    curl_easy_setopt( curlHandle, CURLOPT_WRITEFUNCTION, Http_OnWriteData );
    curl_easy_setopt( curlHandle, CURLOPT_WRITEDATA, ( void* )&strResponse );
    /**
    * 当多个线程都使用超时处理的时候，同时主线程中有sleep或是wait等操作。
    * 如果不设置这个选项，libcurl将会发信号打断这个wait从而导致程序退出。
    */
    curl_easy_setopt( curlHandle, CURLOPT_NOSIGNAL, 1 );
    curl_easy_setopt( curlHandle, CURLOPT_CONNECTTIMEOUT, iConnectTimeOut );
    curl_easy_setopt( curlHandle, CURLOPT_TIMEOUT, iTransmitTimeOut );
    curl_easy_setopt( curlHandle, CURLOPT_FORBID_REUSE, 1 );
    res = curl_easy_perform( curlHandle );
    strRetErr = std::string( curl_easy_strerror( res ) );
    curl_easy_cleanup( curlHandle );
    return res;
}


int CAgreementClient::https_sync_post( IN const std::string& strUrl, IN const std::string& strPost, IN int iConnectTimeOut, IN int iTransmitTimeOut, OUT std::string& strResponse, OUT std::string& strRetErr, IN const char* pCaPath /*= NULL*/ )
{
    CURLcode res;
    CURL* curlHandle = curl_easy_init();
    if( NULL == curlHandle )
    {
        return CURLE_FAILED_INIT;
    }
    if( m_bDebug )
    {
        curl_easy_setopt( curlHandle, CURLOPT_VERBOSE, 1 );
        curl_easy_setopt( curlHandle, CURLOPT_DEBUGFUNCTION, OnDebug );
    }
    curl_easy_setopt( curlHandle, CURLOPT_URL, strUrl.c_str() );
    curl_easy_setopt( curlHandle, CURLOPT_POST, 1 );
    curl_easy_setopt( curlHandle, CURLOPT_POSTFIELDS, strPost.c_str() );
    curl_easy_setopt( curlHandle, CURLOPT_READFUNCTION, NULL );
    curl_easy_setopt( curlHandle, CURLOPT_WRITEFUNCTION, Http_OnWriteData );
    curl_easy_setopt( curlHandle, CURLOPT_WRITEDATA, ( void* )&strResponse );
    curl_easy_setopt( curlHandle, CURLOPT_NOSIGNAL, 1 );
    if( NULL == pCaPath )
    {
        curl_easy_setopt( curlHandle, CURLOPT_SSL_VERIFYPEER, false );
        curl_easy_setopt( curlHandle, CURLOPT_SSL_VERIFYHOST, false );
    }
    else
    {
        //缺省情况就是PEM，所以无需设置，另外支持DER
        //curl_easy_setopt(curl,CURLOPT_SSLCERTTYPE,"PEM");
        curl_easy_setopt( curlHandle, CURLOPT_SSL_VERIFYPEER, true );
        curl_easy_setopt( curlHandle, CURLOPT_CAINFO, pCaPath );
        //curl_easy_setopt(curlHandle,CURLOPT_SSLCERT,"client-cert.pem");
        //curl_easy_setopt(curlHandle,CURLOPT_SSLCERTPASSWD,"password");
        //curl_easy_setopt(curlHandle,CURLOPT_SSLCERTTYPE,"PEM");
        //curl_easy_setopt(curlHandle,CURLOPT_SSLKEY,"client-key.pem");
        //curl_easy_setopt(curlHandle,CURLOPT_SSLKEYPASSWD,"password");
        //curl_easy_setopt(curlHandle,CURLOPT_SSLKEYTYPE,"PEM");
    }
    curl_easy_setopt( curlHandle, CURLOPT_CONNECTTIMEOUT, iConnectTimeOut );
    curl_easy_setopt( curlHandle, CURLOPT_TIMEOUT, iTransmitTimeOut );
    curl_easy_setopt( curlHandle, CURLOPT_FORBID_REUSE, 1 );
    res = curl_easy_perform( curlHandle );
    strRetErr = std::string( curl_easy_strerror( res ) );
    curl_easy_cleanup( curlHandle );
    return res;
}


int CAgreementClient::https_sync_get( IN const std::string& strUrl, IN int iConnectTimeOut, IN int iTransmitTimeOut, OUT std::string& strResponse, OUT std::string& strRetErr, IN const char* pCaPath /*= NULL*/ )
{
    CURLcode res;
    CURL* curlHandle = curl_easy_init();
    if( NULL == curlHandle )
    {
        return CURLE_FAILED_INIT;
    }
    if( m_bDebug )
    {
        curl_easy_setopt( curlHandle, CURLOPT_VERBOSE, 1 );
        curl_easy_setopt( curlHandle, CURLOPT_DEBUGFUNCTION, OnDebug );
    }
    curl_easy_setopt( curlHandle, CURLOPT_URL, strUrl.c_str() );
    curl_easy_setopt( curlHandle, CURLOPT_READFUNCTION, NULL );
    curl_easy_setopt( curlHandle, CURLOPT_WRITEFUNCTION, Http_OnWriteData );
    curl_easy_setopt( curlHandle, CURLOPT_WRITEDATA, ( void* )&strResponse );
    curl_easy_setopt( curlHandle, CURLOPT_NOSIGNAL, 1 );
    if( NULL == pCaPath )
    {
        curl_easy_setopt( curlHandle, CURLOPT_SSL_VERIFYPEER, false );
        curl_easy_setopt( curlHandle, CURLOPT_SSL_VERIFYHOST, false );
    }
    else
    {
        curl_easy_setopt( curlHandle, CURLOPT_SSL_VERIFYPEER, true );
        curl_easy_setopt( curlHandle, CURLOPT_CAINFO, pCaPath );
    }
    curl_easy_setopt( curlHandle, CURLOPT_CONNECTTIMEOUT, iConnectTimeOut );
    curl_easy_setopt( curlHandle, CURLOPT_TIMEOUT, iTransmitTimeOut );
    curl_easy_setopt( curlHandle, CURLOPT_FORBID_REUSE, 1 );
    res = curl_easy_perform( curlHandle );
    strRetErr = std::string( curl_easy_strerror( res ) );
    curl_easy_cleanup( curlHandle );
    return res;
}

int CAgreementClient::ftp_upload( IN const std::string& strRemotePath, IN const std::string& strLocalPath, IN const int iConnectTimeOut, IN const std::string& strUserName /*= ""*/, IN const std::string& strUserPwd /*= ""*/ )
{
    CURL* curlHandle = curl_easy_init();
    if( NULL == curlHandle )
    {
        return CURLE_FAILED_INIT;
    }
    if( m_bDebug )
    {
        curl_easy_setopt( curlHandle, CURLOPT_VERBOSE, 1 );
        curl_easy_setopt( curlHandle, CURLOPT_DEBUGFUNCTION, OnDebug );
    }
    FILE* pFile = NULL;
    long uploaded_len = 0;
    CURLcode code = CURLE_GOT_NOTHING;
    pFile = fopen( strLocalPath.c_str(), "rb" );
    if( pFile == NULL )
    {
        return -1;
    }
    curl_easy_setopt( curlHandle, CURLOPT_FORBID_REUSE, 1 );
    curl_easy_setopt( curlHandle, CURLOPT_UPLOAD, 1L );
    curl_easy_setopt( curlHandle, CURLOPT_URL, strRemotePath.c_str() );
    curl_easy_setopt( curlHandle, CURLOPT_USERPWD, std::string( strUserName + ":" + strUserPwd ).c_str() );
    if( iConnectTimeOut )
        curl_easy_setopt( curlHandle, CURLOPT_FTP_RESPONSE_TIMEOUT, iConnectTimeOut );
    curl_easy_setopt( curlHandle, CURLOPT_HEADERFUNCTION, Ftp_OnGetContentLength );
    curl_easy_setopt( curlHandle, CURLOPT_HEADERDATA, &uploaded_len );
    curl_easy_setopt( curlHandle, CURLOPT_WRITEFUNCTION, Ftp_OnDiscard );
    curl_easy_setopt( curlHandle, CURLOPT_READFUNCTION, Ftp_OnReadData );
    curl_easy_setopt( curlHandle, CURLOPT_READDATA, pFile );
    curl_easy_setopt( curlHandle, CURLOPT_FTPPORT, "-" ); /* disable passive mode */
    curl_easy_setopt( curlHandle, CURLOPT_FTP_CREATE_MISSING_DIRS, 1L );
    curl_easy_setopt( curlHandle, CURLOPT_VERBOSE, 1L );
    for( int i = 0; ( code != CURLE_OK ) && ( i < 3 ); i++ )
    {
        /* are we resuming? */
        if( i )
        {
            /* yes */
            /* determine the length of the file already written */
            /*
            * With NOBODY and NOHEADER, libcurl will issue a SIZE
            * command, but the only way to retrieve the result is
            * to parse the returned Content-Length header. Thus,
            * getcontentlengthfunc(). We need discardfunc() above
            * because HEADER will dump the headers to stdout
            * without it.
            */
            curl_easy_setopt( curlHandle, CURLOPT_NOBODY, 1L );
            curl_easy_setopt( curlHandle, CURLOPT_HEADER, 1L );
            code = curl_easy_perform( curlHandle );
            if( code != CURLE_OK )
                continue;
            curl_easy_setopt( curlHandle, CURLOPT_NOBODY, 0L );
            curl_easy_setopt( curlHandle, CURLOPT_HEADER, 0L );
            fseek( pFile, uploaded_len, SEEK_SET );
            curl_easy_setopt( curlHandle, CURLOPT_APPEND, 1L );
        }
        else   /* no */
        {
            curl_easy_setopt( curlHandle, CURLOPT_APPEND, 0L );
        }
        code = curl_easy_perform( curlHandle );
    }
    fclose( pFile );
    curl_easy_cleanup( curlHandle );
    return code;
}

int CAgreementClient::ftp_download( IN const std::string& strRemotePath, IN const std::string& strLocalPath, IN const int iConnectTimeOut, IN const std::string& strUserName /*= ""*/, IN const std::string& strUserPwd /*= ""*/ )
{
    CURL* curlHandle = curl_easy_init();
    if( NULL == curlHandle )
    {
        return CURLE_FAILED_INIT;
    }
    if( m_bDebug )
    {
        curl_easy_setopt( curlHandle, CURLOPT_VERBOSE, 1 );
        curl_easy_setopt( curlHandle, CURLOPT_DEBUGFUNCTION, OnDebug );
    }
    FILE* pFile = NULL;
    curl_off_t local_file_len = -1 ;
    long filesize = 0;
    CURLcode code = CURLE_GOT_NOTHING;
    struct stat file_info;
    int use_resume = 0;
    //获取本地文件大小信息
    if( stat( strLocalPath.c_str(), &file_info ) == 0 )
    {
        local_file_len = file_info.st_size;
        use_resume = 1;
    }
    //追加方式打开文件，实现断点续传
    pFile = fopen( strLocalPath.c_str(), "ab+" );
    if( pFile == NULL )
    {
        return -1;
    }
    curl_easy_setopt( curlHandle, CURLOPT_FORBID_REUSE, 1 );
    curl_easy_setopt( curlHandle, CURLOPT_URL, strRemotePath.c_str() );
    curl_easy_setopt( curlHandle, CURLOPT_USERPWD, std::string( strUserName + ":" + strUserPwd ).c_str() );
    //连接超时设置
    curl_easy_setopt( curlHandle, CURLOPT_CONNECTTIMEOUT, iConnectTimeOut );
    //设置头处理函数
    curl_easy_setopt( curlHandle, CURLOPT_HEADERFUNCTION, Ftp_OnGetContentLength );
    curl_easy_setopt( curlHandle, CURLOPT_HEADERDATA, &filesize );
    // 设置断点续传
    curl_easy_setopt( curlHandle, CURLOPT_RESUME_FROM_LARGE, use_resume ? local_file_len : 0 );
    curl_easy_setopt( curlHandle, CURLOPT_WRITEFUNCTION, Ftp_OnWriteData );
    curl_easy_setopt( curlHandle, CURLOPT_WRITEDATA, pFile );
    curl_easy_setopt( curlHandle, CURLOPT_NOPROGRESS, 1L );
    curl_easy_setopt( curlHandle, CURLOPT_VERBOSE, 1L );
    code = curl_easy_perform( curlHandle );
    fclose( pFile );
    curl_easy_cleanup( curlHandle );
    return code;
}

int CAgreementClient::tcp_sync_socket( IN const std::string& strIp, IN const int iPort, IN const std::string& strSend, IN int iConnectTimeOut, IN int iTransmitTimeOut, OUT std::string& strResponse )
{
    CURL* curlHandle;
    CURLcode code;
    curl_socket_t sockfd;
    size_t request_len = strSend.length();
    size_t nsent_total = 0;
    curlHandle = curl_easy_init();
    if( curlHandle )
    {
        curl_easy_setopt( curlHandle, CURLOPT_URL, strIp.c_str() );
        curl_easy_setopt( curlHandle, CURLOPT_PORT, iPort );
        curl_easy_setopt( curlHandle, CURLOPT_CONNECT_ONLY, 1L );
        curl_easy_setopt( curlHandle, CURLOPT_CONNECTTIMEOUT, iConnectTimeOut ); //不知道有没有效果
        curl_easy_setopt( curlHandle, CURLOPT_TIMEOUT, iTransmitTimeOut );  //不知道有没有效果
        if( m_bDebug )
        {
            curl_easy_setopt( curlHandle, CURLOPT_VERBOSE, 1 );
            curl_easy_setopt( curlHandle, CURLOPT_DEBUGFUNCTION, OnDebug );
        }
        code = curl_easy_perform( curlHandle );
        if( code != CURLE_OK )
        {
            return code;
        }
        //code = curl_easy_getinfo(curlHandle, CURLINFO_ACTIVESOCKET, &sockfd);
        code = curl_easy_getinfo( curlHandle, CURLINFO_LASTSOCKET, &sockfd );
        if( code != CURLE_OK )
        {
            return code;
        }
        //发送数据
        do
        {
            size_t nsent;
            do
            {
                nsent = 0;
                code = curl_easy_send( curlHandle, strSend.substr( nsent ).c_str(), request_len - nsent_total, &nsent );
                nsent_total += nsent;
                if( code == CURLE_AGAIN && !wait_on_socket( sockfd, 0, ( long )iConnectTimeOut ) )
                {
                    return -1;
                }
            }
            while( code == CURLE_AGAIN );
            if( code != CURLE_OK )
            {
                return code;
            }
        }
        while( nsent_total < request_len );
        //接收数据
        strResponse.clear();
        while( 1 )
        {
            char buf[1024 * 4] = {0};
            size_t nread;
            do
            {
                nread = 0;
                code = curl_easy_recv( curlHandle, buf, sizeof( buf ), &nread );
                strResponse.append( buf );
                if( code == CURLE_AGAIN && !wait_on_socket( sockfd, 1, ( long )iTransmitTimeOut ) )
                {
                    return -1;
                }
            }
            while( code == CURLE_AGAIN );
            if( code != CURLE_OK )
            {
                break;
            }
            if( nread == 0 )
            {
                break;
            }
        }
        curl_easy_cleanup( curlHandle );
    }
    return 0;
}

///////////////////////////////////////////////////////////////////////////////////////////////

void CAgreementClient::SetDebug( bool bDebug )
{
    m_bDebug = bDebug;
}

std::string CAgreementClient::get_error_detail( CURLcode code )
{
    return curl_easy_strerror( code );
}




