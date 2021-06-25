// BigInteger.cpp : �������������
//

#ifdef WIN32
#define LONG long
#define ULONG unsigned long
#define UINT  unsigned int

#include "stdafx.h"
#include "stdlib.h"
#include "string.h"
#endif

#include "BigInteger.h"

static int BigInteger_sub_unsigned(const BigInteger *left, const BigInteger *right, BigInteger *result);
static int BigInteger_add_unsigned(const BigInteger *left, const BigInteger *right, BigInteger *result);

//���left/right����ĵ�һλ
static int BigInteger_div_helpfunc(const BigInteger *left, const BigInteger *right, int *k);
static int BigInteger_mul_single(const BigInteger *left, char right, BigInteger *result);
static int BigInteger_getStartNBits(BigInteger *left, char *result, int startPos, int len);
static int BigInteger_addNBits(BigInteger *left, BigInteger * tmpInteger, int startPos, int len);

/*****************************************************************
 *
 *    ���ַ���ת���ɴ����ڲ���ʾ������ "12345"  ==> 12345
 *
 *    ����ֵ:  ��ȷ����0�����򷵻�1
 *****************************************************************
 */

int getBigIntegerFromStr(const char * str, BigInteger *result)
{
    int len, j;
    int i = 0;
    
    memset(result, 0, sizeof(BigInteger));
    
    if (!str)
    {
        return 1;
    }
    if (strlen(str) > MAX_NUMBER_COUNT)
    {
        return 1;
    }
    
    if (str[i] == '-')
    {
        result->sbit = 1;
        i++;
    }
    else if (str[i] == '+')
    {
        result->sbit = 0;
        i++;
    }
    else
    {
        result->sbit = 0;
    }
    
    len = (int)strlen(str) - 1;
    j = MAX_NUMBER_COUNT - 1;
    while (len >= i)
    {
        result->bytesArray[j] = str[len] - '0';
        j--;
        len--;
    }
    return 0;
}

/************************************************************
 *
 *    ��������ʾת�����ַ�����ʽ������ 12345  ==> "12345"
 *
 *    ����ֵ:  ��ȷ����0�����򷵻�1
 *************************************************************
 */
int getStrFromBigInteger(BigInteger *left, char *str)
{
    int i = 0;
    int j = 0;
    
    while (i < MAX_NUMBER_COUNT)
    {
        if (left->bytesArray[i] != 0)
        {
            break;
        }
        i++;
    }
    
    while (i < MAX_NUMBER_COUNT)
    {
        str[j] = left->bytesArray[i] + '0';
        j++;
        i++;
    }

    //����Ϊ0���������, ������000000,����ֵӦ��Ϊ"0"�������ǿմ�
    if (j == 0)
    {
        str[j] = '0';
        j++;
    }
    
    str[j] = '\0';
    return 0;
}

/****************************************************************
 *      ����left+right, left,right�����з���
 *
 *       ����ֵ���������1,���򷵻�0
 *****************************************************************
 */
int BigInteger_add(BigInteger *left, BigInteger *right, BigInteger *result)
{
    int flag = 0;
    
    if (left->sbit == right->sbit)
    {
        //����|left|+|right|
        flag = BigInteger_add_unsigned(left, right, result);
        //ͬ�ţ����,
        result->sbit = left->sbit;
    }
    else
    {
        //��ţ����
        if (BigInteger_cmp_unsigned(left, right) == 1)
        {
            //����|left|-|right|
            flag = BigInteger_sub_unsigned(left, right, result);
            result->sbit = left->sbit;
        }
        else if (BigInteger_cmp_unsigned(left, right) == -1)
        {
            //����|right|-|left|
            flag = BigInteger_sub_unsigned(right, left, result);
            result->sbit = right->sbit;
        }
        else
        {
            //|right|==|right|
            result->sbit = 0;
            memset(result, 0, sizeof(BigInteger));
        }

    }

    return 0;
}

/****************************************************************
 *      ����left-right, left,right�����з���
 *
 *       ����ֵ���������1,���򷵻�0
 *****************************************************************
 */
int BigInteger_sub(BigInteger *left, BigInteger *right, BigInteger *result)
{
    int sbit;
    int retVal;
    
    sbit = (int)right->sbit;
    if (right->sbit == 1)
    {
        right->sbit = 0;
    }
    else
    {
        right->sbit = 1;
    }
    retVal = BigInteger_add(left, right, result);
    right->sbit = (char)sbit;
    
    return retVal;
}

/***********************************************************************
 *   ����left* right = �Ľ��
 *
 *   �����㷨�� �ȼ���abcde*x, Ȼ�󽫽�����ƣ�Ȼ���ټ��� abcde*y, ...
 *   ����ֵ���������1, ��������0
 ************************************************************************
 */
int BigInteger_mul(BigInteger *left, BigInteger *right, BigInteger *result)
{
    //���� |left|*|right|��ֵ
    int j;
    
    BigInteger tmpInteger;
    BigInteger accInteger; //�ۼ���
    memset(&accInteger, 0, sizeof(BigInteger));
    memset(result, 0, sizeof(BigInteger));
    memset(&tmpInteger, 0, sizeof(tmpInteger));
    //���ҵ�һ����0λ
    j = 0;
    while ((j < MAX_NUMBER_COUNT) && (right->bytesArray[j] == 0))
    {
        j++;
    }    
    if (j >= MAX_NUMBER_COUNT)
    {
        return 0;
    }

    for (; j < MAX_NUMBER_COUNT; j++)
    {
        BigInteger_mul_single(left, right->bytesArray[j], &tmpInteger);
        BigInteger_shiftleft(&accInteger, 1);
        BigInteger_add_unsigned(&accInteger, &tmpInteger, &accInteger);
    }

    memcpy(result, &accInteger, sizeof(BigInteger));
    if (left->sbit == right->sbit)
    {
        result->sbit = 0;
    }
    else
    {
        result->sbit = 1;
    }
    
    return 0;
}

/**
 *   ����left/right�Ľ��
 *
 *   �����㷨: �ȶ��룬Ȼ������
 *
 *   ����ֵ���������1, ��������0
 */
int BigInteger_div(BigInteger *left, BigInteger *right, BigInteger *result)
{
    BigInteger tmpInteger;
    BigInteger mulInteger;
    BigInteger left_tmp;
    char str[MAX_NUMBER_COUNT];
    char divresult[MAX_NUMBER_COUNT + 2];  //QAM:char divresult[MAX_NUMBER_COUNT + 1];
    //int index = 0;
    int loops; //�����̵�λ��
    int len; //������λ��
    int k;
    int i = 0, j = 0;

    
    memcpy(&left_tmp, left, sizeof(BigInteger));
    while ((i < MAX_NUMBER_COUNT) && (left_tmp.bytesArray[i] == 0))
    {
        i++;
    }
    while ((j < MAX_NUMBER_COUNT) && (right->bytesArray[j] == 0))
    {
        j++;
    }
    loops = (j - i) + 1;
    len = MAX_NUMBER_COUNT - j;
    j = 0;
    if (loops < 0)
    {
        divresult[j] = '0';
        j++;
    }
    else
    {
        while (loops > 0)
        {
            memset(&tmpInteger, 0, sizeof(tmpInteger));
            BigInteger_getStartNBits(&left_tmp, str, i, len); //ȡ����λ
            getBigIntegerFromStr(str, &tmpInteger);
            BigInteger_div_helpfunc(&tmpInteger, right, &k); //��������һλ����
            BigInteger_mul_single(right, (char)k, &mulInteger);
            BigInteger_sub(&tmpInteger, &mulInteger, &tmpInteger);
            BigInteger_addNBits(&left_tmp, &tmpInteger, i + len - 1, len); //��������������һ�γ����ı�����
            loops--;
            divresult[j] = (char)(k + (int)'0');  
            j++;
            len++;
        }

    }
    divresult[j] = '\0';

    //printf("����Ľ��: %s \n", divresult);
    getBigIntegerFromStr(divresult, result);
    
    return 0;
}

static int BigInteger_addNBits(BigInteger *left, BigInteger * tmpInteger, int startPos, int len)
{
    int i = MAX_NUMBER_COUNT - 1;
    while ((len > 0) && (i >= 0))
    {
        left->bytesArray[startPos] = tmpInteger->bytesArray[i];
        startPos--;
        len--;
        i--;
    }
    return 0;
}

/**
 *
 *	����left��ȡ��startPos��ʼ��lenλ��left����Ӧλ����Ϊ0�������������
 *  ����ֵ:�ɹ�����0
 *
 */
static int BigInteger_getStartNBits(BigInteger *left, char *result, int startPos, int len)
{
    int i = 0;
    while ((i < len) && (startPos < MAX_NUMBER_COUNT))
    {
        result[i] = left->bytesArray[startPos] + '0';
        left->bytesArray[startPos] = 0;
        i++;
        startPos++;
    }
    result[i] = '\0';
    
    return 0;
}

/**************************************************
 * �� *k=left/right��ֵ
 *  ע�����������������㣺left<10*right
 *  ����ֵ:�ɹ�����0
 **************************************************
 */
static int BigInteger_div_helpfunc(const BigInteger *left, const BigInteger *right, int *k)
{

    int i = 0;
    int j = 0;
    int m = 0;
    BigInteger tmpInteger;
    
    while ((i < MAX_NUMBER_COUNT) && (left->bytesArray[i] == 0))
    {
        i++;
    }
    while ((j < MAX_NUMBER_COUNT) && (right->bytesArray[j] == 0))
    {
        j++;
    }
    if ((i < MAX_NUMBER_COUNT) && (j < MAX_NUMBER_COUNT))
    {
        if (i > j) //left<right
        {
            *k = 0;

        }
        else
        {
            //����
            for (m = 0; m < 10; m++)
            {
                BigInteger_mul_single(right, (char)m, &tmpInteger);
                if (BigInteger_cmp_unsigned(&tmpInteger, left) > 0)
                {
                    break;
                }
            }
            *k = m - 1;

        }
        /*
         else
         {
         if(left->bytesArray[i]>=right->bytesArray[j])
         *k= left->bytesArray[i]/right->bytesArray[j];
         else
         *k= 0;
         }
         */
    }
    else
    {
        if (i >= MAX_NUMBER_COUNT) //������Ϊ0����0/x
        {
            *k = 0;
        }
        if (j >= MAX_NUMBER_COUNT) //����Ϊ0����x/0
        {
            //			printf("����Ϊ0\n");

            return 1;
        }
    }
    return 0;
}

/**
 * ���㵥�����ֺ�����������ĳ˻�  :  left * right
 *
 *  ����ֵ:��result���ؼ���Ľ��
 *
 */
static int BigInteger_mul_single(const BigInteger *left, char right, BigInteger *result)
{
    BigInteger tmpInteger; 
    int i;
    int k = 0;
    
    memset(&tmpInteger, 0, sizeof(tmpInteger));
    
    for (i = MAX_NUMBER_COUNT - 1; i >= 0; i--)
    {
        k = left->bytesArray[i] * right;
        k += tmpInteger.bytesArray[i];
        tmpInteger.bytesArray[i] = (char)(k % 10);
        if (((k / 10) > 0) && ((i - 1) < 0))
        {
            return 1;
        }
        else
        {
            tmpInteger.bytesArray[i - 1] = (char)(k / 10);
        }
    }
    memcpy(result, &tmpInteger, sizeof(tmpInteger));
    
    return 0;
}

//����nλ, ������������1�����򷵻�0
int BigInteger_shiftleft(BigInteger *integer, int n)
{
    int i = 0;
    if (n > (MAX_NUMBER_COUNT - 1))
    {
        return 1;
    }
    if (n <= 0)
    {
        return 0;
    }
    
    while ((i + n) < MAX_NUMBER_COUNT)
    {
        integer->bytesArray[i] = integer->bytesArray[i + n];
        i++;
    }
    while (i < MAX_NUMBER_COUNT)
    {
        integer->bytesArray[i] = 0;
        i++;
    }
    
    return 0;
}

//����nλ, �ɹ�����0
int BigInteger_shiftright(BigInteger *integer, int n)
{
    int i = MAX_NUMBER_COUNT - 1;
    
    if (n > (MAX_NUMBER_COUNT - 1))
    {
        return 1;
    }
    if (n <= 0)
    {
        return 0;
    }
    while ((i - n) >= 0)
    {
        integer->bytesArray[i] = integer->bytesArray[i - n];
        i--;
    }
    while (i >= 0)
    {
        integer->bytesArray[i] = 0;
        i--;
    }
    return 0;
}

//�Ƚ��������ľ���ֵ��С, ���left>right,����1�����left<right,����-1�����򷵻�0
int BigInteger_cmp_unsigned(const BigInteger *left, const BigInteger *right)
{
    int i;
    
    for (i = 0; i < MAX_NUMBER_COUNT; i++)
    {
        if (left->bytesArray[i] > right->bytesArray[i])
        {
            return 1;
        }
        
        if (left->bytesArray[i] < right->bytesArray[i])
        {
            return -1;
        }
    }
    return 0;
}

//������������ֵ���, ����0����������1���
static int BigInteger_add_unsigned(const BigInteger *left, const BigInteger *right, BigInteger *result)
{
    BigInteger tmpInteger;
    int i = MAX_NUMBER_COUNT - 1;
    char cChar = 0;
    tmpInteger.sbit = result->sbit;
    
    memset(&tmpInteger, 0, sizeof(tmpInteger));
    
    while (i >= 0)
    {
        cChar = (left->bytesArray[i] + right->bytesArray[i]) + tmpInteger.bytesArray[i];
        if (cChar > 9)
        {
            cChar = cChar - 10;
            if ((i - 1) < 0)
            {
                //printf("�����!!!\n");
                return 1;
            }
            tmpInteger.bytesArray[i - 1] = 1;
        }
        tmpInteger.bytesArray[i] = cChar;
        i--;
    }
    memcpy(result, &tmpInteger, sizeof(tmpInteger));
    
    return 0;
}

//������������ֵ�������������left>=right!!!
//����0����������1���
static int BigInteger_sub_unsigned(const BigInteger *left, const BigInteger *right, BigInteger *result)
{
    BigInteger tmpInteger;
    int i = MAX_NUMBER_COUNT - 1;
    char cChar = 0;
    
    memset(&tmpInteger, 0, sizeof(tmpInteger));
    tmpInteger.sbit = result->sbit;
    
    while (i >= 0)
    {
        cChar = (left->bytesArray[i] - right->bytesArray[i]) - tmpInteger.bytesArray[i];
        if (cChar < 0)
        {
            if ((i - 1) <= 0)
            {
                //printf("������� \n");
                return 1;
            }
            tmpInteger.bytesArray[i - 1] = 1;
            cChar = (char)(10 + (int)cChar);
        }

        tmpInteger.bytesArray[i] = cChar;
        i--;
    }

    memcpy(result, &tmpInteger, sizeof(tmpInteger));
    
    return 0;
}

