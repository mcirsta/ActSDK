// BigInteger.cpp : �������������
//

#ifndef BIGINTEGER_H
#define BIGINTEGER_H

#include "include_case.h"

#include "calculat_sty.h"

#define MAX_NUMBER_COUNT   80     //��������λ��(10���Ʊ�ʾ)
typedef struct
{
    char bytesArray[MAX_NUMBER_COUNT]; //����������ĸ���λ��,��������123 ��bytesArray[0]=1
    //���λ��0
    char sbit; //����λ, 1��ʾ������0��ʾ����
} BigInteger;

int getBigIntegerFromStr(const char * str, BigInteger *result);
int getStrFromBigInteger(BigInteger *left, char *str);
//�������1
int BigInteger_add(BigInteger *left, BigInteger *right, BigInteger *result);

//�������1
int BigInteger_sub(BigInteger *left, BigInteger *right, BigInteger *result);

/**
 *   ����abcde... * xyzqws = �Ľ��
 *
 *   �����㷨�� �ȼ���abcde*x, Ȼ�󽫽�����ƣ�Ȼ���ټ��� abcde*y, ...
 *   ����ֵ���������1, ��������0
 */
int BigInteger_mul(BigInteger *left, BigInteger *right, BigInteger *result);
int BigInteger_div(BigInteger *left, BigInteger *right, BigInteger *result);
int BigInteger_shiftleft(BigInteger *integer, int n);
int BigInteger_shiftright(BigInteger *integer, int n);
int BigInteger_cmp_unsigned(const BigInteger *left, const BigInteger *right);
#endif

