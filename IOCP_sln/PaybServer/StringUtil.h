/******************************************************************************

版权所有 (C), 2015-2035, 深圳市金证科技股份有限公司

******************************************************************************
文 件 名   : StringUtil.h
版 本 号   : 初稿
作    者   : 黄共元
生成日期   : 2016年4月8日
最近修改   :
功能描述   : 字符/字符串/流定义
函数列表   :
修改历史   :
1.日    期   : 2016年4月8日
作    者   : 黄共元
修改内容   : 创建文件

******************************************************************************/
#ifndef __StringUtil_H__
#define __StringUtil_H__

#include <string>
#include <iostream>
#include <sstream>
#include <vector>

#include <boost/lexical_cast.hpp>


#ifdef _MSC_VER
#pragma warning(disable:4996) //  warning C4996: 'std::copy': Function call with parameters that may be unsafe
#endif

#define ZeroDynamicMemory( m, l ) memset( ( m ), 0, l )
#define ZeroStaticMemory( m )    memset( (void*)&( m ), 0, sizeof( m ) )
#define UPD_ArraySize(arr) ( sizeof (arr) / sizeof( arr[0] ) )

typedef std::string tstring;
typedef char tchar;
typedef unsigned char uchar;

typedef std::stringstream tstringstream;


//字符串操作函数

//以下两个模板函数,在遇到非法参数时不会抛异常
//如果需要可以使用ConvertType
template<typename Type>
Type String2InternalType(const tstring& strNumber)
{
  std::stringstream ss(strNumber);
  Type Number;
  ss >> Number;
  return Number;
}

template<typename Type>
tstring InternalType2String(Type Number)
{
  std::stringstream ss;
  ss << Number;
  return ss.str();
}
//类型无法转换时抛异常
template<typename In, typename Out>
Out ConvertType(const In& i)
{
  return boost::lexical_cast<Out>(i);
}

class CStringUtil
{
public:
  typedef std::stringstream StrStreamType;

  static tstring GetSpecValue(const tstring &strData);
  static int SplitString(const tstring& strSrc, const tstring& strSep
    , std::vector<tstring>& vctStringList, bool bIncludeEmpties = true, bool bTrim = true);

  static tstring& RemoveCharacter(tstring& strSrc, const tchar cRemove);
  static tstring RemoveSubstring(const tstring& strSrc, const tstring& strRemoveWhat);
  static tstring& RemoveSubstring(tstring& strSrc, const tstring& strRemoveWhat);

  static tstring& ToUpper(tstring& strSrc);
  static const tstring ToUpper(const tstring& strSrc);

  static tstring& ToLower(tstring& strSrc);
  static const tstring ToLower(const tstring& strSrc);

  static tstring& TrimLeft (tstring& strSrc, const tchar* pszTrim = NULL);
  static const tstring TrimLeft (const tstring& strSrc, const tchar* pszTrim = NULL);

  static tstring& TrimRight(tstring& strSrc, const tchar* pszTrim = NULL);
  static const tstring TrimRight(const tstring& strSrc, const tchar* pszTrim = NULL);

  static tstring& TrimAll  (tstring& strSrc, const tchar* pszTrim = NULL);
  static const tstring TrimAll  (const tstring& strSrc, const tchar* pszTrim = NULL);

  static tstring& ReplaceAll(tstring& strSrc, const tstring& strReplaceWhat, const tstring& strReplaceWithWhat);
  static const tstring ReplaceAll(const tstring& strSrc, const tstring& strReplaceWhat, const tstring& strReplaceWithWhat);

  static tstring GetFilePath(const tstring& strFullPath);
  static tstring GetFileName(const tstring& strFullPath);
  static tstring GetFileExtension(const tstring& strFullPath);

  static tstring UTF82GBK(const char* pszSrc, int nSrcLen);
  static tstring GBK2UTF8(const tstring& strGBK);
  
  static std::wstring Ansi2WChat(const char* pszSrc, int nLen);
  static std::wstring str2wstr(const tstring& str);

  static tstring FormatLastError();
  static tstring FormatSocketError();
};

#endif// #ifndef __StringUtil_H__

