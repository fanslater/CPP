#include "StdAfx.h"
#include "AesEncryptor.h"

#include "aes.h"

#include <fstream>
using namespace std;

AesEncryptor::AesEncryptor(unsigned char *key)
{
    m_pEncryptor = new AES(key);
}


AesEncryptor::~AesEncryptor(void)
{
    delete m_pEncryptor;
}

void AesEncryptor::Byte2Hex(const unsigned char *src, int len, char *dest)
{
    for (int i = 0; i < len; ++i)
    {
        sprintf_s(dest + i * 2, 3, "%02X", src[i]);
    }
}

void AesEncryptor::Hex2Byte(const char *src, int len, unsigned char *dest)
{
    int length = len / 2;
    for (int i = 0; i < length; ++i)
    {
        dest[i] = Char2Int(src[i * 2]) * 16 + Char2Int(src[i * 2 + 1]);
    }
}

int AesEncryptor::Char2Int(char c)
{
    if ('0' <= c && c <= '9')
    {
        return (c - '0');
    }
    else if ('a' <= c && c <= 'f')
    {
        return (c - 'a' + 10);
    }
    else if ('A' <= c && c <= 'F')
    {
        return (c - 'A' + 10);
    }
    return -1;
}

string AesEncryptor::EncryptString(string strInfor)
{
    int iSrcDataLen = strInfor.length();
    int iNeedAddSpaceLen = 16 - (iSrcDataLen % 16);
    int iRealSrcDataLen = iSrcDataLen + iNeedAddSpaceLen;
    unsigned char *pTransBuff = new unsigned char[iRealSrcDataLen + 1];
    memset(pTransBuff, 0, iRealSrcDataLen + 1);
    memcpy_s(pTransBuff, iRealSrcDataLen, strInfor.c_str(), iSrcDataLen);
    m_pEncryptor->Cipher(pTransBuff, iRealSrcDataLen);
    char *pOut = new char[2 * iRealSrcDataLen + 1];
    memset(pOut, 0, 2 * iRealSrcDataLen + 1);
    Byte2Hex(pTransBuff, iRealSrcDataLen, pOut);
    string retValue(pOut);
    delete[] pTransBuff;
    delete[] pOut;
    return retValue;
}

string AesEncryptor::DecryptString(string strMessage)
{
    int iBuffLen = strMessage.length() / 2;
    unsigned char *pTransBuff = new unsigned char[iBuffLen + 1];
    memset(pTransBuff, 0, iBuffLen + 1);
    Hex2Byte(strMessage.c_str(), strMessage.length(), pTransBuff);
    m_pEncryptor->InvCipher(pTransBuff, iBuffLen);
    string retValue((char *)pTransBuff);
    delete[] pTransBuff;
    return retValue;
}

void AesEncryptor::EncryptTxtFile(const char *inputFileName, const char *outputFileName)
{
    ifstream ifs;

    // Open file:
    ifs.open(inputFileName);
    if (!ifs)
    {
        TRACE("AesEncryptor::EncryptTxtFile() - Open input file failed!");
        return ;
    }

    // Read config data:
    string strInfor;
    string strLine;
    while (!ifs.eof())
    {
        char temp[1024];
        memset(temp, '\0', 1024);
        ifs.read(temp, 1000);
        strInfor += temp;
    }
    ifs.close();

    // Encrypt
    strLine = EncryptString(strInfor);

    // Writefile
    ofstream ofs;
    ofs.open(outputFileName);
    if (!ofs)
    {
        TRACE("AesEncryptor::EncryptTxtFile() - Open output file failed!");
        return ;
    }
    ofs << strLine;
    ofs.close();
}

void AesEncryptor::DecryptTxtFile(const char *inputFile, const char *outputFile)
{
    ifstream ifs;

    // Open file:
    ifs.open(inputFile);
    if (!ifs)
    {
        TRACE("AesEncryptor::DecryptTxtFile() - Open input file failed!");
        return ;
    }

    // Read config data:
    string strInfor;
    string strLine;
    while (!ifs.eof())
    {
        char temp[1024];
        memset(temp, '\0', 1024);
        ifs.read(temp, 1000);
        strInfor += temp;
    }
    ifs.close();

    // Encrypt
    strLine = DecryptString(strInfor);

    // Writefile
    ofstream ofs;
    ofs.open(outputFile);
    if (!ofs)
    {
        TRACE("AesEncryptor::DecryptTxtFile() - Open output file failed!");
        return ;
    }
    ofs << strLine;
    ofs.close();
}