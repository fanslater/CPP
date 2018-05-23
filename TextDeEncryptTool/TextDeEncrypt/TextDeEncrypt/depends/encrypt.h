#include <stdlib.h>
#include <stdio.h>
#include <errno.h>

#if !defined(__kbss_encrypt_h__)
#define __kbss_encrypt_h__

#define KBSS_ENCRYPT
#define KBSS_DECRYPT
#define KBSS_COMENCRYPT
#define KBSS_COMDECRYPT

#if defined(OS_IS_WINDOWS)

#define snprintf  _snprintf

#ifdef  DLLEXPORTS
#define DLLEXPORT _declspec(dllexport)
#else
#define DLLEXPORT _declspec(dllimport)
#endif
#define CALLTYPE _cdecl
#else
#define DLLEXPORT
#define CALLTYPE
#endif

#ifdef __cplusplus
extern "C"
{
#endif

#if defined(KBSS_ENCRYPT)
void DLLEXPORT CALLTYPE kbss_encrypt(char *p_pszOutput, int p_nFixedSize, const char *p_pszInput, const char *p_pszKey);
#endif

#if defined(KBSS_DECRYPT)
void DLLEXPORT CALLTYPE kbss_decrypt(char *p_pszOutput, int p_nFixedSize, const char *p_pszInput, const char *p_pszKey);
#endif

#if defined(KBSS_COMENCRYPT)
void DLLEXPORT CALLTYPE kbss_comencrypt(char *p_pszOutput, const char *p_pszInput, const char *p_pszKey);
#endif

#if defined(KBSS_COMDECRYPT)
void DLLEXPORT CALLTYPE kbss_comdecrypt(char *p_pszOutput, const char *p_pszInput, const char *p_pszKey);
#endif

// AES
// 前后台加解密认证数据时用。p_llKey为用户代码
void DLLEXPORT CALLTYPE AES_Encrypt1(char *p_pszEncrResult, int p_iSize, __int64 p_llKey, const char * p_pszEncrInfo);
void DLLEXPORT CALLTYPE AES_Decrypt1(char *p_pszDecrResult, int p_iSize, __int64 p_llKey, const char * p_pszDecrInfo);

/*
// AES
void DLLEXPORT CALLTYPE AES_Encrypt(char *p_pszEncrResult, int p_iSize, const unsigned char *p_pszKey, int p_iKeySize, const char * p_pszEncrInfo);
void DLLEXPORT CALLTYPE AES_Decrypt(char * p_pszDecrResult, int p_iSize, const unsigned char * p_pszKey, int p_iKeySize, const char * p_pszDecrInfo);
*/

// MD5
void DLLEXPORT CALLTYPE MD5_Digist(char * p_pszDigResult, int p_iSize, unsigned char p_pszDigRetInt[16], unsigned char * p_pszDigInfo);

// Base64
void DLLEXPORT CALLTYPE Base64_Encode(char * p_pszEnResult, const unsigned char * p_pszEnInfo, int p_iSize);
void DLLEXPORT CALLTYPE Base64_Decode(unsigned char *p_pszResult, int & p_refiCount, const char * pszDeInfo);

// RC5
// 前后台加解密保密键(验证码)用,p_llKey为的用户代码
void DLLEXPORT CALLTYPE RC5_Encrypt1(char * p_pszEnResult, int p_iSize, __int64 p_llKey, const char * p_pszEnInfo);
void DLLEXPORT CALLTYPE RC5_Decrypt1(char * p_pszDeResult, int p_iSize, __int64 p_llKey, const char * p_pszDeInfo);

// iEncryptType = 1 代表服务端加解密Key2。这时传入的pszKey与iKeySize没有作用，内部写死有密钥。
// iEncryptType = 2 其它情况加解密
void DLLEXPORT CALLTYPE RC5_Encrypt(char * p_pszEnResult, int p_iSize, const unsigned char * p_pszKey, int p_iKeySize, int p_iEncryptType, const char * p_pszEnInfo);
void DLLEXPORT CALLTYPE RC5_Decrypt(char * p_pszDeResult, int p_iSize, const unsigned char * p_pszKey, int p_iKeySize, int p_iDecryptType, const char * p_pszDeInfo);

// RSA
int DLLEXPORT CALLTYPE RSA_Encrypt(char *p_pszSignResult, int p_iSize, char * p_pszKey, const unsigned char *p_pszSignInfo, int p_iLen);
int DLLEXPORT CALLTYPE RSA_Decrypt(char *p_pszDecrResult, int p_iSize, int &p_refiCount, const char *p_pszKey, const char *p_pszVerifyInfo);

// RANDOM
void DLLEXPORT CALLTYPE RandomNo(char * p_pszRandNoBuf, int p_iSize);

#ifdef __cplusplus
}
#endif

#endif  // __kbss_encrypt_h__
