#include "headfile.h"

BYTE* RC4(char* pDate, int pData_len, BYTE* rc4_key, int rc4_key_len)
{
    int array[256] = { 0 };
    int array2[256] = { 0 };

    BYTE* passwd = (BYTE*)malloc(pData_len);
    ZeroMemory(passwd, pData_len);

    int i;
    for (i = 0; i < 256; i++)
    {
        array[i] = rc4_key[i % rc4_key_len];
        array2[i] = i;
    }

    int num = i = 0;
    for (; i < 256; i++)
    {
        num = (num + array2[i] + array[i]) % 256;
        int num2 = array2[i];
        array2[i] = array2[num];
        array2[num] = num2;
    }
    int num3 = num = (i = 0);

    for (; i < pData_len; i++)
    {
        num3++;
        num3 %= 256;
        num += array2[num3];
        num %= 256;
        int num2 = array2[num3];
        array2[num3] = array2[num];
        array2[num] = num2;
        int num4 = array2[(array2[num3] + array2[num]) % 256];
        passwd[i] = (byte)(pDate[i] ^ num4);

    }
    passwd[pData_len] = 0;

    return passwd;

}
