#pragma once

class AES
{
public:
    AES(unsigned char *key);
    virtual ~AES();
    unsigned char *Cipher(unsigned char *input);    // ���ܣ�����������С������16�ֽ�
    unsigned char *InvCipher(unsigned char *input); // ���ܣ����������Ҳ������16�ֽ�
    void *Cipher(void *input, int length = 0);  // ���Դ������飬��С������16����������������ǽ���Խ��������������length��Ĭ��Ϊ0����ô�������ַ���������'\0'����
    void *InvCipher(void *input, int length);   // ���봫������ʹ�С��������16��������

private:
    unsigned char Sbox[256];
    unsigned char InvSbox[256];
    unsigned char w[11][4][4];

    void KeyExpansion(unsigned char *key, unsigned char w[][4][4]);
    unsigned char FFmul(unsigned char a, unsigned char b);

    void SubBytes(unsigned char state[][4]);
    void ShiftRows(unsigned char state[][4]);
    void MixColumns(unsigned char state[][4]);
    void AddRoundKey(unsigned char state[][4], unsigned char k[][4]);

    void InvSubBytes(unsigned char state[][4]);
    void InvShiftRows(unsigned char state[][4]);
    void InvMixColumns(unsigned char state[][4]);
};
