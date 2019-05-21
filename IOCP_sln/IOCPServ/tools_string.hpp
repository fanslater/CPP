/*! @tools_string.hpp
********************************************************************************
<PRE>
ģ����       : ��װ���ַ�������
�ļ���       : tools_string.hpp
����ļ�     :
ʵ�ֹ���     : ��װ���ַ�������
����         : szluosj@21cn.com
�汾         : v1.0
--------------------------------------------------------------------------------
���̰߳�ȫ�� : ��
�쳣ʱ��ȫ�� : ��
--------------------------------------------------------------------------------
��ע         :
--------------------------------------------------------------------------------
�޸ļ�¼ :
�� ��        �汾     �޸���              �޸�����
2012/04/24   v1.0     luosj               ����
</PRE>
*******************************************************************************/

#ifndef __TOOLS_STRING_H
#define __TOOLS_STRING_H

//#pragma once

#ifdef _WIN32
#include <windows.h>
#endif

#include "platform.h"

#include <string>
#include <map>
#include <iostream>
#include<sstream>

//#include <boost/format.hpp>
#include <boost/tokenizer.hpp>
#include <boost/algorithm/string.hpp>




#include <iostream>
#include <string>
#include <stdarg.h>
#include <fstream>

#include <boost/archive/text_oarchive.hpp>
#include <boost/archive/text_iarchive.hpp>
#include <boost/archive/iterators/binary_from_base64.hpp>

//base64
#include <boost/archive/iterators/base64_from_binary.hpp>
#include <boost/archive/iterators/transform_width.hpp>
#include <boost/archive/iterators/ostream_iterator.hpp>


#ifdef _MSC_VER
#pragma warning(disable:4996) //  warning C4996: 'std::copy': Function call with parameters that may be unsafe
#endif


//�ַ�����������
namespace MyTools
{
    //-----------------------------------------------------------------------------
    inline std::string &Serialize(std::string &strData)
    {
        //���л����Ѵ����16����
        return strData;
    }
    //-----------------------------------------------------------------------------
    inline std::string &Deserialize(std::string &strData)
    {
        //�����л�����16���Ʊ�ɴ�
        return strData;
    }
    //-----------------------------------------------------------------------------
    inline std::string &StringXOR(std::string &strData)
    {
        {
            std::ofstream ofs("./test.txt", std::ios::binary);
            boost::archive::text_oarchive oa(ofs /*,boost::archive::archive_flags::no_codecvt | boost::archive::archive_flags::no_header*/  );
            unsigned char szBuffer[32] = {0};
            for (size_t i = 0; i < sizeof(szBuffer) - 1; i++)
                szBuffer[i] = i;

            szBuffer[10] = 'A';
            szBuffer[11] = 'B';
            szBuffer[12] = 0xff;
            szBuffer[13] = 0xfe;

            oa << szBuffer;
        }
        {
            std::ifstream ifs("./test.txt", std::ios::binary);
            boost::archive::text_iarchive ia(ifs);
        }
        //        MyTools::OutputDebugStringEx("sn:%s",strTT.c_str());
        return strData;
    }

    //------------------------------------------------------------------------------
    inline unsigned char *XOR(
        unsigned char *p_pszString,
        const unsigned char *p_pszKey,
        int p_iLength
    )
    {
        int nLen = p_iLength;//strlen((char *)p_pszString);
        int j = 0;
        for (int i = 0; i < nLen; i++)
        {
            if (p_pszKey[j] == 0)
                j = 0;

            p_pszString[i] ^= p_pszKey[j++];
        }

        return p_pszString;
    }
    //------------------------------------------------------------------------------
    // ����������
    //     ��ʮ�����Ƶ���ֵת����ʮ�����Ƶ��ִ�
    //     �� :
    //         p_pszString = 010203040A0B0C0D
    //         p_pszHex = 0x01 0x02 0x03 0x04 0x0A 0x0B 0x0C 0x0D
    //         p_iLength = 8
    // ����˵����
    //     p_pszString[out]
    //         ���ڴ��ת�����ʮ�������ִ������С������p_iLength��2��
    //     p_pszHex[in]
    //         ��Ҫת����ʮ��������ֵ
    //     p_iLength[in]
    //         ��ʶp_lpszHex�ĳ���
    // ����˵����
    //     ����p_pszString
    inline unsigned char *H2A(
        unsigned char *p_pszString,
        const unsigned char *p_pszHex,
        int p_iLength
    )
    {
        int iIndex = 0, iByte = 0;

        for (iIndex = 0; iIndex < p_iLength; iIndex++)
        {
            iByte = (p_pszHex[iIndex] & 0xF0) >> 4;
            p_pszString[iIndex * 2] = iByte > 0x09 ? iByte + 0x41 - 0x0A : iByte + 0x30;
            iByte = (p_pszHex[iIndex] & 0x0F);
            p_pszString[iIndex * 2 + 1] = iByte > 0x09 ? iByte + 0x41 - 0x0A : iByte + 0x30;
        }

        return p_pszString;
    }

    //------------------------------------------------------------------------------
    // ����������
    //     ��ʮ�����Ƶ��ִ�ת����ʮ�����Ƶ���ֵ
    //     �� :
    //         p_pszHex = 0x01 0x02 0x03 0x04 0x0A 0x0B 0x0C 0x0D
    //         p_pszString = 010203040A0B0C0D
    //         p_iLength = 8
    // ����˵����
    //     p_pszHex[out]
    //         ���ڴ��ת�����ʮ��������ֵ�����С������p_iLength
    //     p_pszString[in]
    //         ��Ҫת����ʮ�����Ʊ�ʾ���ִ������С������p_iLength��2��
    //     p_iLength[in]
    //         ��ʶp_lpszHex�ĳ���
    // ����˵����
    //     ����p_pszHex
    inline unsigned char *A2H(
        unsigned char *p_pszHex,
        const unsigned char *p_pszString,
        int p_iLength
    )
    {
        int iIndex = 0;

        for (iIndex = 0; iIndex < p_iLength; iIndex++)
        {
            p_pszHex[iIndex] = p_pszString[iIndex * 2] <= 0x39 ? p_pszString[iIndex * 2] - 0x30
                               : p_pszString[iIndex * 2] - 0x41 + 0x0A;
            p_pszHex[iIndex] = p_pszHex[iIndex] << 4;
            p_pszHex[iIndex] += p_pszString[iIndex * 2 + 1] <= 0x39 ? p_pszString[iIndex * 2 + 1] - 0x30
                                : p_pszString[iIndex * 2 + 1] - 0x41 + 0x0A;
        }

        return p_pszHex;
    }

    //-----------------------------------------------------------------------------
    inline std::string Trim(std::string src)
    {
        return boost::trim_right_copy(boost::trim_left_copy(src));
    }
    //-----------------------------------------------------------------------------
    static std::string string_format( const char *pMsg, ... )
    {
        va_list ap;
        char text[32768] = {0};

        va_start(ap, pMsg);
        vsnprintf(text, sizeof(text) - 1, pMsg, ap);
        va_end(ap);

        return std::string( text );
    }
    //-----------------------------------------------------------------------------
    static void OutputDebugStringEx(const char *p_pszErrorMessageFMT, ...)
    {
#ifdef _WIN32
        static DWORD dwCount = 0;
        va_list arg_ptr;
        char szMessageTemp[8192] = {0};
        char szMessage[8192] = {0};

        va_start(arg_ptr, p_pszErrorMessageFMT);
        _vsnprintf(szMessageTemp, sizeof(szMessageTemp) - 1, p_pszErrorMessageFMT, arg_ptr);
        va_end(arg_ptr);

#ifdef _DEBUG //����ǵ��԰汾�������Ӹ��س����У�Ҫ��Ȼ��VC����ʾ��һ���ѿ�
        snprintf(szMessage, sizeof(szMessage) - 1, "%03u [%04x][%s]\n", (WORD)dwCount, GetCurrentThreadId(), szMessageTemp);
#else
        snprintf(szMessage, sizeof(szMessage) - 1, "%03u [%04x][%s]", (WORD)dwCount, GetCurrentThreadId(), szMessageTemp);
#endif

#ifdef WIN32
        OutputDebugStringA(szMessage);
#endif

#endif
    }
    //-----------------------------------------------------------------------------
    inline DWORD GetTodayNumber()
    {
        //����20120403
        SYSTEMTIME stSystemTime = {0};
        GetLocalTime(&stSystemTime);

        char szDate[32] = {0};
        snprintf(szDate, sizeof(szDate) - 1, "%04d%02d%02d", stSystemTime.wYear, stSystemTime.wMonth, stSystemTime.wDay);

        return atol(szDate);
    }
    //-----------------------------------------------------------------------------
    inline std::string GetTimestamp()
    {
        //����"2009-2-27 18:02:15.123"
        SYSTEMTIME stSystemTime = {0};
        GetLocalTime(&stSystemTime);

        char szDate[32] = {0};
        snprintf(szDate, sizeof(szDate) - 1, "%04d-%02d-%02d %02d:%02d:%02d.%03d",
                 stSystemTime.wYear, stSystemTime.wMonth, stSystemTime.wDay,
                 stSystemTime.wHour, stSystemTime.wMinute, stSystemTime.wSecond, stSystemTime.wMilliseconds);

        return szDate;
    }
    //-----------------------------------------------------------------------------
    inline std::string GetTimeStr(char *pszFormat)
    {
        //����"2009-2-27 18:02:15.123"
        SYSTEMTIME stSystemTime = {0};
        GetLocalTime(&stSystemTime);

        char szDate[32] = {0};
        snprintf(szDate, sizeof(szDate) - 1, pszFormat,
                 stSystemTime.wYear, stSystemTime.wMonth, stSystemTime.wDay,
                 stSystemTime.wHour, stSystemTime.wMinute, stSystemTime.wSecond, stSystemTime.wMilliseconds);

        return szDate;
    }
    //-----------------------------------------------------------------------------
    static int SplitString(const std::string &strSrc, const std::string strSep, std::vector<std::string> &vctStringList, bool bTrim = true)
    {
        //�ָ��ַ���
        // ���ָ���ַ���
        //std::string strTag = _T("I Come from China");
        // ����ָʽΪӢ�Ķ��ţ����Ķ��źͿո񣬹���һ���ִ�����
        boost::char_separator<char> sep(strSep.c_str(), "", boost::keep_empty_tokens);
        typedef boost::tokenizer<boost::char_separator<char> >
        CustonTokenizer;
        CustonTokenizer tok(strSrc, sep);

        // ����ָ���
        for(CustonTokenizer::iterator beg = tok.begin(); beg != tok.end(); ++beg)
        {
            std::string strValue = *beg;
            if (bTrim)
                strValue = Trim(strValue);

            vctStringList.push_back(strValue);
        }

        return vctStringList.size();
    }

    //-----------------------------------------------------------------------------
    //ȥ��ո�
    inline char *API_ltrim(char *str)
    {
        int i;

        for(i = 0; i < (int) strlen(str) && (str[i] == ' ' || str[i] == '\t'); i++)
            if(str[i] == ' ' || str[i] == '\t' || str[i] == 0x0a || str[i] == 0x0d) continue;
        if(i > 0) strcpy(str, str + i);
        return str;
    }

    //ȥ�ҿո�
    inline char *API_rtrim(char *str)
    {
        int i;

        for(i = strlen(str) - 1; i >= 0 && (str[i] == ' ' || str[i] == '\t' || str[i] == 0x0a || str[i] == 0x0d); i--) str[i] = 0;
        return str;
    }

    //ȥ���ҿո�
    inline char *API_alltrim(char *str)
    {
        API_ltrim(str);
        API_rtrim(str);
        return str;
    }

    /*
    nIndex����1��ʼ
    pszOutput: output
    nLen:size of pszOutput
    return:>=0 len of output
    <0 error
    */
    static int API_ReadField(char *pszInput, int nIndex, char *pszOutput, int nLen, char chTerm)
    {
        int i, n;
        char *p, *p0, *p1;
        if(!pszOutput) return -1;	//buffer��ָ��
        *pszOutput = 0;
        if(nIndex < 1) return -3;
        p0 = pszInput;
        p1 = pszInput + strlen(pszInput);
        p = p1;
        for(i = 1; i < nIndex; i++)
        {
            p = strchr(p0, chTerm);
            if(!p) return -2;		//�ֶ���Ŀ����
            p0 = p + 1;
        }

        p = strchr(p0, chTerm);
        if(p) p1 = p;
        i = p1 - p0;
        n = i < nLen ? i : nLen;
        strncpy(pszOutput, p0, n);
        if(n < nLen) pszOutput[n] = 0;

        //printf("in=%s i=%d ch=%c out=%s\n",pszInput, nIndex, chTerm, pszOutput);
        return n;
    }
    //-----------------------------------------------------------------------------
    inline std::string GetStringHeadTail(std::string &strData)
    {
        //�õ��ִ���ͷβ,����"{adsfadsfad}"�õ�"{}"
        std::string strTag;
        strTag = strData.substr(0, 1);
        strTag += strData.substr(strData.size() - 1, 1);

        return strTag;
    }
    //-----------------------------------------------------------------------------
    inline int GetSpecValueList(std::string &strData, std::string strSepartor, std::vector<std::string> &vctFields)
    {
        //�õ��ִ���ͷβ,����"{tag1:tag2}"�õ� "tag1","tag2"
        if ( (strData.size() < 2) || (GetStringHeadTail(strData) != "{}"))
        {
            return 0;
        }
        else
        {
            std::string strValues = strData.substr(1, strData.size() - 2);
            return SplitString(strValues, strSepartor, vctFields);
        }

        return vctFields.size();
    }
    //-----------------------------------------------------------------------------
    static std::string GetSpecValue(std::string &strData)
    {
        //�õ������ִ���ֵͷβ,����"{tag1:tag2}"�õ� "./dict.ini"��"[tag1]��tag2��ֵ"
        std::string strResult;
        std::vector<std::string> vctFields;

        if ( (strData.size() < 2) || (GetStringHeadTail(strData) != "{}"))
            return strData;

        std::string strValueTrue = strData.substr(1, strData.size() - 2);
        if (GetStringHeadTail(strValueTrue) == "{}") //����{{}} ��Ҫ����ʵʱ������
            return strValueTrue;

        SplitString(strValueTrue, ":", vctFields);

        //        GetSpecValueList(strData, ":", vctFields);

        std::string strFile = "./config_dict.ini";
        std::string strSection = "public";
        std::string strName;

        std::string strValue;

        int nFieldNum = vctFields.size();
        if (nFieldNum <= 0)
            return strData;
        else if (nFieldNum == 1)
            strName = vctFields[0];
        else if (nFieldNum == 2)
        {
            strSection = vctFields[0];
            strName    = vctFields[1];
        }
        else if (nFieldNum == 3)
        {
            strFile    = vctFields[0];
            strSection = vctFields[1];
            strName    = vctFields[2];
        }

        if (strSection == "[date]") //{[date]:%04d-%02d-%02d}
        {
            SYSTEMTIME stSystemTime = {0};
            GetLocalTime(&stSystemTime);
            return string_format(strName.c_str(), stSystemTime.wYear, stSystemTime.wMonth, stSystemTime.wDay);
        }
        else if (strSection == "[time]") //{[time]:%02d:%02d:%02d}
        {
            SYSTEMTIME stSystemTime = {0};
            GetLocalTime(&stSystemTime);
            return string_format(strName.c_str(), stSystemTime.wHour, stSystemTime.wMinute, stSystemTime.wSecond);
        }

        if (!strFile.empty() && !strSection.empty() && !strName.empty())
        {
            char szValue[8192] = {0};
            GetPrivateProfileString (strSection.c_str(), strName.c_str(), "", szValue, sizeof(szValue) - 1, strFile.c_str());
            strValue = szValue;
        }

        return strValue;
    }
    //-----------------------------------------------------------------------------
    //�������� name=luosj&age=35 �������ֶ�, dot�Ƿָ���s
    inline  bool GetFieldFromString(char *pszURL, char *pszKey, char *pszValue, int iSize, char dot = '&')
    {
        memset(pszValue, 0, iSize);

        char szURL[8192] = {0};
        snprintf(szURL, sizeof(szURL) - 1, "%c%s", dot, pszURL);

        char szTag[1024] = {0};
        snprintf(szTag, sizeof(szTag) - 1, "%c%s=", dot, pszKey);

        char *pBegin = strstr(szURL, szTag);
        if (pBegin == NULL)
            return false; //û�ҵ�

        pBegin += strlen(szTag); //ָ������

        char *pEnd = strchr(pBegin, dot);
        if (pEnd != NULL)
            *pEnd = 0;

        strncpy(pszValue, pBegin, iSize);

        return true;
    }
    //-----------------------------------------------------------------------------
    //-----------------------------------------------------------------------------
    //�������� http://localhost:1100/soap?name=luosj&age=35 �������ֶ�, dot�Ƿָ���s
    inline bool GetFieldFromURL(char *pszURL, char *pszKey, char *pszValue, int iSize, char dot)
    {
        char *pszBegin = strchr(pszURL, '?');
        if (pszBegin == NULL)
            return GetFieldFromString(pszURL, pszKey, pszValue, iSize, dot);

        return GetFieldFromString(pszBegin + 1, pszKey, pszValue, iSize, dot);
    }

    //-----------------------------------------------------------------------------
    inline std::string GetNow()
    {
        //���� 20111109
        SYSTEMTIME stSystemTime = {0};

        GetLocalTime(&stSystemTime);

        return string_format("%04d-%02d-%02d %02d:%02d:%02d", stSystemTime.wYear, stSystemTime.wMonth, stSystemTime.wDay,
                             stSystemTime.wHour,
                             stSystemTime.wMinute,
                             stSystemTime.wSecond);
    }
    //-----------------------------------------------------------------------------
    inline std::string GetDate()
    {
        //���� 20111109
        SYSTEMTIME stSystemTime = {0};

        GetLocalTime(&stSystemTime);

        return string_format("%04d%02d%02d", stSystemTime.wYear, stSystemTime.wMonth, stSystemTime.wDay);
    }
    inline std::string GetTime()
    {
        //���� 120101
        SYSTEMTIME stSystemTime = {0};

        GetLocalTime(&stSystemTime);

        return string_format("%02d%02d%02d", stSystemTime.wHour, stSystemTime.wMinute, stSystemTime.wSecond);
    }
    inline std::string GetTime1()
    {
        //����ExcuteTime:12:01:01:000
        SYSTEMTIME stSystemTime = {0};

        GetLocalTime(&stSystemTime);

        return string_format("ExcuteTime:%02d:%02d:%02d:%03d", stSystemTime.wHour, stSystemTime.wMinute, stSystemTime.wSecond, stSystemTime.wMilliseconds);
    }
    //-----------------------------------------------------------------------------
    inline void GetOverTime(std::string &strCurTime, std::string &strOverTime, int iOffSet = 3600 * 2)
    {
        time_t iTimeT = 0;
        time(&iTimeT);;
        struct tm *tmNowTime = localtime(&iTimeT);
        strCurTime = MyTools::string_format("%04d-%02d-%02d %02d:%02d:%02d", tmNowTime->tm_year + 1900, tmNowTime->tm_mon + 1, tmNowTime->tm_mday, tmNowTime->tm_hour, tmNowTime->tm_min, tmNowTime->tm_sec);
        iTimeT += iOffSet;
        struct tm *tmOverTime = localtime(&iTimeT);
        strOverTime = MyTools::string_format("%04d-%02d-%02d %02d:%02d:%02d", tmOverTime->tm_year + 1900, tmOverTime->tm_mon + 1, tmOverTime->tm_mday, tmOverTime->tm_hour, tmOverTime->tm_min, tmOverTime->tm_sec);
    }
    //-----------------------------------------------------------------------------
    static std::string &MapSerializer(std::map<std::string, std::string> &map, std::string &strMap)
    {
        //map������л�/�����л�
        strMap.clear();
        if (!map.empty())
        {
            std::map<std::string, std::string>::iterator it = map.begin();
            while(it != map.end())
            {
                if (!strMap.empty())
                    strMap += "&";

                strMap += (it->first + "=" + it->second);

                it++;
            }
        }

        return strMap;
    }
    //-----------------------------------------------------------------------------
    static int MapDeserializer(std::map<std::string, std::string> &map, char *pszBuffer, int nSize)
    {
        //map������л�/�����л�
        map.clear();

        char *pszBegin = pszBuffer;
        unsigned char temp = pszBuffer[nSize];
        pszBuffer[nSize] = 0;
        for (;;)
        {
            char *pEqu = strchr(pszBegin, '=');
            if (pEqu == NULL)
                break;

            *pEqu = 0;

            char *pEnd = strchr(pEqu + 1, '&');
            if (pEnd != NULL)
                *pEnd = 0;

            map.insert(std::make_pair(pszBegin, pEqu + 1));

            *pEqu = '=';
            if (pEnd != NULL)
                *pEnd = '&';

            if (pEnd == NULL)
                break;

            pszBegin = pEnd + 1;
            if (pszBegin - pszBuffer >= nSize)
                break;
        }
        pszBuffer[nSize] = temp;

        return map.size();
    }
    //-----------------------------------------------------------------------------
    static const std::string base64_padding[] = {"", "==", "="};
    static std::string base64_encode(const std::string &s)
    {
        namespace bai = boost::archive::iterators;

        std::stringstream os;

        // convert binary values to base64 characters
        typedef bai::base64_from_binary
        // retrieve 6 bit integers from a sequence of 8 bit bytes
        <bai::transform_width<const char *, 6, 8> > base64_enc; // compose all the above operations in to a new iterator

        std::copy(base64_enc(s.c_str()), base64_enc(s.c_str() + s.size()),
                  bai::ostream_iterator<char>(os));

        os << base64_padding[s.size() % 3];
        return os.str();
    }
    //-----------------------------------------------------------------------------
    static std::string base64_decode(const std::string &s)
    {
        namespace bai = boost::archive::iterators;

        std::stringstream os;

        // convert binary values to base64 characters
        typedef bai::transform_width
        // retrieve 6 bit integers from a sequence of 8 bit bytes
        <bai::binary_from_base64<const char *>, 8, 6> base64_dec; // compose all the above operations in to a new iterator

        unsigned int size = s.size();
        if(size <= 0) return "";

        for (int i = 1; i <= 2; i++)
        {
            if (s.c_str()[size - 1] == '=')
                size--;
        }

        std::copy(base64_dec(s.c_str()), base64_dec(s.c_str() + size),
                  bai::ostream_iterator<char>(os));

        return os.str();
    }
    //-----------------------------------------------------------------------------
}
#endif// #ifndef __TOOLS_STRING_H

