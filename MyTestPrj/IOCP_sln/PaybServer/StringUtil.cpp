//------------------------------------------------------------------------------
// 版权声明：本程序模块属于金证券商综合业务系统(KBSS)的一部分
//           金证科技股份有限公司  版权所有
//
// 模块名称：StringUtil
// 模块描述：字符串处理
// 开发作者：黄共元
// 创建日期：2016年10月10日
// 模块版本：1.0.0.0
//------------------------------------------------------------------------------

#include "stdafx.h"
#include <Windows.h>
#include "StringUtil.h"
#include <tchar.h>
#include <map>

//----------------------------------------------------------------

static const tstring strDelims(" \t\r\n");
//----------------------------------------------------------------

//得到特殊字串的值头尾,例如"{tag1:tag2}"得到 "./dict.ini"中"[tag1]中tag2的值"
tstring CStringUtil::GetSpecValue(const tstring &strData)
{
  if (!(strData.length() > 0 && '{' == strData[0] && '}' == strData[strData.length() - 1]))
    return strData;

   tstring strValueTrue = strData.substr(1, strData.size() - 2);
//   if (GetStringHeadTail(strValueTrue)=="{}") //两层{{}} 主要用于实时解析，
//     return strValueTrue;

  std::vector<tstring> vctFields;
  SplitString(strValueTrue, ":", vctFields);

  tstring strName;
  tstring strValue;
  tstring strSection;
  tstring strFile;

  int nFieldNum = vctFields.size();
  if (nFieldNum<=0)
    return strData;
  else if (nFieldNum==1)
    strName = vctFields[0];
  else if (nFieldNum==2)
  {
    strSection = vctFields[0];
    strName    = vctFields[1];
  }
  else if (nFieldNum==3)
  {
    strFile    = vctFields[0];
    strSection = vctFields[1];
    strName    = vctFields[2];
  }

  if (!strFile.empty() && !strSection.empty() && !strName.empty())
  {
    char szValue[8192] = {0};
    GetPrivateProfileString (strSection.c_str(), strName.c_str(), "",szValue, sizeof(szValue)-1, strFile.c_str());
    strValue = szValue;
  }
  return strValue;
}

int CStringUtil::SplitString(const tstring& strInput, const tstring& strDelimiter, std::vector<tstring>& vcResults, bool bIncludeEmpties/* = true*/, bool bTrim/* = true*/)
{
  int nSizeS2 = (int)strDelimiter.size();
  int nSize = (int)strInput.size();

  if( !nSize || !nSizeS2 )
  {
    return 0;
  }
  int nNewPos = strInput.find (strDelimiter, 0);

  if( tstring::npos == nNewPos )
  { 
    vcResults.push_back(strInput);
    return 1; 
  }
  int nNumFound = 0;
  int nPos = 0;
  std::vector<int> vcPositions;

  while( nNewPos >= nPos )
  {
    nNumFound++;
    vcPositions.push_back(nNewPos);
    nPos = nNewPos;
    nNewPos = strInput.find (strDelimiter, nPos+nSizeS2);
  }
  if( !nNumFound )
  {
    return 0;
  }
  for( int i = 0; i <= (int)vcPositions.size(); ++i )
  {
    tstring s("");

    if( !i ) 
    { 
      s = strInput.substr( i, vcPositions[i] ); 
    }
    else
    {
      int nOffset = vcPositions[i - 1] + nSizeS2;

      if( nOffset < nSize )
      {
        if( vcPositions.size() == i )
        {
          s = strInput.substr(nOffset);
        }
        else if( i > 0 )
        {
          s = strInput.substr( vcPositions[i - 1] + nSizeS2, 
            vcPositions[i] - vcPositions[i - 1] - nSizeS2 );
        }
      }
    }
    if( bIncludeEmpties || ( s.size() > 0 ) )
    {
      if (bTrim)
        TrimAll(s);
      
      vcResults.push_back(s);
    }
  }
  return nNumFound;
}

tstring& CStringUtil::RemoveCharacter(tstring& strSrc, const tchar cRemove)
{
  tstring::iterator itEnd = std::remove_if(strSrc.begin(), strSrc.end(), std::bind2nd(std::equal_to<tchar>(), cRemove));
  strSrc.erase(itEnd, strSrc.end());
  return strSrc;
}

tstring CStringUtil::RemoveSubstring(const tstring& strSrc, const tstring& strRemoveWhat)
{
  tstring strTmp( CStringUtil::ReplaceAll(strSrc, strRemoveWhat, "\x2") );
  return CStringUtil::RemoveCharacter(strTmp, '\x2');
}

tstring& CStringUtil::RemoveSubstring(tstring& strSrc, const tstring& strRemoveWhat)
{
  CStringUtil::ReplaceAll(strSrc, strRemoveWhat, "\x2");
  return CStringUtil::RemoveCharacter(strSrc, '\x2');
}

tstring& CStringUtil::ToUpper(tstring& strSrc)
{
  std::transform(strSrc.begin(), strSrc.end(), strSrc.begin(), ::toupper);
  return strSrc;
}

const tstring CStringUtil::ToUpper(const tstring& strSrc)
{
  tstring strCopySrc(strSrc);
  std::transform(strCopySrc.begin(), strCopySrc.end(), strCopySrc.begin(), ::toupper);
  return strCopySrc;
}

tstring& CStringUtil::ToLower(tstring& strSrc)
{
  std::transform(strSrc.begin(), strSrc.end(), strSrc.begin(), ::tolower);
  return strSrc;
}

const tstring CStringUtil::ToLower(const tstring& strSrc)
{
  tstring strCopySrc(strSrc);
  std::transform(strCopySrc.begin(), strCopySrc.end(), strCopySrc.begin(), ::tolower);
  return strCopySrc;
}

tstring& CStringUtil::TrimLeft(tstring& strSrc, const tchar* pszTrim)
{
  strSrc.erase(0, strSrc.find_first_not_of(NULL == pszTrim ? strDelims : pszTrim));
  return strSrc;
}

const tstring CStringUtil::TrimLeft(const tstring& strSrc, const tchar* pszTrim)
{
  tstring strCopySrc(strSrc);
  strCopySrc.erase(0, strCopySrc.find_first_not_of(NULL == pszTrim ? strDelims : pszTrim));
  return strCopySrc;
}

tstring& CStringUtil::TrimRight(tstring& strSrc, const tchar* pszTrim)
{
  strSrc.erase( strSrc.find_last_not_of(NULL == pszTrim ? strDelims : pszTrim) + 1 );
  return strSrc;
}

const tstring CStringUtil::TrimRight(const tstring& strSrc, const tchar* pszTrim)
{
  tstring strCopySrc(strSrc);
  strCopySrc.erase( strCopySrc.find_last_not_of(NULL == pszTrim ? strDelims : pszTrim) + 1 );
  return strCopySrc;
}

tstring& CStringUtil::TrimAll(tstring& strSrc, const tchar* pszTrim)
{
  return CStringUtil::TrimRight( CStringUtil::TrimLeft(strSrc, pszTrim), pszTrim );
}

const tstring CStringUtil::TrimAll(const tstring& strSrc, const tchar* pszTrim)
{
  return CStringUtil::TrimRight( CStringUtil::TrimLeft(strSrc, pszTrim), pszTrim );
}

tstring& CStringUtil::ReplaceAll(tstring& strSrc
                                , const tstring& strReplaceWhat
                                , const tstring& strReplaceWithWhat)
{
  tstring::size_type nPos = 0;

  while(1)
  {
    nPos = strSrc.find(strReplaceWhat, nPos);

    if (tstring::npos == nPos) 
    {
      break;
    }
    strSrc.replace(nPos, strReplaceWhat.size(), strReplaceWithWhat);
    nPos += strReplaceWithWhat.size();
  }
  return strSrc;
}

const tstring CStringUtil::ReplaceAll(const tstring& strSrc
                                      , const tstring& strReplaceWhat
                                      , const tstring& strReplaceWithWhat)
{
  tstring strCopySrc(strSrc);
  return CStringUtil::ReplaceAll(strCopySrc, strReplaceWhat, strReplaceWithWhat);
}

tstring CStringUtil::GetFileName(const tstring& strFullPath)
{
  size_t i = strFullPath.find_last_of('\\');

  if (i == tstring::npos)
  {
    return strFullPath;
  }
  else
  {
    return strFullPath.substr(i+1, strFullPath.size() - i - 1);
  }
}

tstring CStringUtil::GetFileExtension(const tstring& strFullPath)
{
  size_t i = strFullPath.find_last_of('.');

  if (i == tstring::npos)
  {
    return strFullPath;
  }
  else
  {
    return strFullPath.substr(i, strFullPath.size() - i);
  }
}

tstring CStringUtil::GetFilePath(const tstring& strFullPath)
{
  size_t i = strFullPath.find_last_of('\\');

  if (i == tstring::npos)
  {
    return "";
  }
  else
  {
    return strFullPath.substr(0, i + 1);
  }
}

std::string CStringUtil::UTF82GBK(const char* pszSrc, int nSrcLen)
{
  int nLen = MultiByteToWideChar(CP_UTF8, 0, pszSrc, nSrcLen, NULL, 0);
  unsigned short * wszGBK = new unsigned short[nLen + 1];
  memset(wszGBK, 0, nLen * 2 + 2);
  MultiByteToWideChar(CP_UTF8, 0, pszSrc, nSrcLen, (LPWSTR)wszGBK, nLen);

  nLen = WideCharToMultiByte(CP_ACP, 0, (LPWSTR)wszGBK, -1, NULL, 0, NULL, NULL);
  tchar *szGBK = new tchar[nLen + 1];
  memset(szGBK, 0, nLen + 1);
  WideCharToMultiByte(CP_ACP,0, (LPWSTR)wszGBK, -1, szGBK, nLen, NULL, NULL);

  tstring strTemp(szGBK);
  delete[]szGBK;
  delete[]wszGBK;
  return strTemp;
}

tstring CStringUtil::GBK2UTF8(const tstring& strGBK)
{
  int nLen = MultiByteToWideChar(CP_ACP, 0, strGBK.c_str(), -1, NULL, 0);
  WCHAR * wszUTF8 = new WCHAR[nLen];
  MultiByteToWideChar(CP_ACP, 0, strGBK.c_str(), -1, wszUTF8, nLen);

  nLen = WideCharToMultiByte(CP_UTF8, 0, wszUTF8, -1, NULL, 0, NULL, NULL);
  tchar * szUTF8 = new tchar[nLen];
  WideCharToMultiByte(CP_UTF8, 0, wszUTF8, -1, szUTF8, nLen, NULL, NULL);

  tstring strTemp(szUTF8);
  delete[]wszUTF8;
  delete[]szUTF8;
  return strTemp;
}

std::wstring CStringUtil::Ansi2WChat(LPCSTR pszSrc, int nLen)
{
  int nSize = MultiByteToWideChar(CP_ACP, 0, (LPCSTR)pszSrc, nLen, 0, 0);  
  if(nSize <= 0) return NULL;  

  WCHAR *pwszDst = new WCHAR[nSize+1];  
  if( NULL == pwszDst) return NULL;  

  MultiByteToWideChar(CP_ACP, 0,(LPCSTR)pszSrc, nLen, pwszDst, nSize);  
  pwszDst[nSize] = 0;  

  if( pwszDst[0] == 0xFEFF) // skip Oxfeff  
    for(int i = 0; i < nSize; i ++)   
      pwszDst[i] = pwszDst[i+1];  

  std::wstring wcharString(pwszDst);  
  delete pwszDst;  

  return wcharString;  
}

std::wstring CStringUtil::str2wstr(const std::string& str)
{
  return Ansi2WChat(str.c_str(), str.size());
}

tstring CStringUtil::FormatLastError()
{
	LPVOID pszMsgBuf; 
	FormatMessage( 
		FORMAT_MESSAGE_ALLOCATE_BUFFER | 
		FORMAT_MESSAGE_FROM_SYSTEM | 
		FORMAT_MESSAGE_IGNORE_INSERTS, 
		NULL, 
		GetLastError(), 
		MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), 
		(tchar*) &pszMsgBuf, 
		0, 
		NULL 
		); 
	tstring strResult = (tchar*)pszMsgBuf;
	LocalFree( pszMsgBuf );
	return strResult;
}

tstring CStringUtil::FormatSocketError()
{
	LPVOID pszMsgBuf; 
	FormatMessage( 
		FORMAT_MESSAGE_ALLOCATE_BUFFER | 
		FORMAT_MESSAGE_FROM_SYSTEM | 
		FORMAT_MESSAGE_IGNORE_INSERTS, 
		NULL, 
		::WSAGetLastError(), 
		MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), 
		(tchar*) &pszMsgBuf, 
		0, 
		NULL 
		); 
	tstring strResult = (tchar*)pszMsgBuf;
	LocalFree( pszMsgBuf );
	return strResult;
}

