// BigInteger.cpp : 定义大整数运算
//

#ifndef BIGINTEGER_H
#define BIGINTEGER_H

#include "include_case.h"

#include "calculat_sty.h"

#define MAX_NUMBER_COUNT   80     //最大的整数位数(10进制表示)
typedef struct
{
    char bytesArray[MAX_NUMBER_COUNT]; //保存大整数的各个位数,例：对于123 则bytesArray[0]=1
    //最高位填0
    char sbit; //符号位, 1表示负数，0表示正数
} BigInteger;

int getBigIntegerFromStr(const char * str, BigInteger *result);
int getStrFromBigInteger(BigInteger *left, char *str);
//溢出返回1
int BigInteger_add(BigInteger *left, BigInteger *right, BigInteger *result);

//溢出返回1
int BigInteger_sub(BigInteger *left, BigInteger *right, BigInteger *result);

/**
 *   计算abcde... * xyzqws = 的结果
 *
 *   基本算法， 先计算abcde*x, 然后将结果左移，然后再计算 abcde*y, ...
 *   返回值：溢出返回1, 正常返回0
 */
int BigInteger_mul(BigInteger *left, BigInteger *right, BigInteger *result);
int BigInteger_div(BigInteger *left, BigInteger *right, BigInteger *result);
int BigInteger_shiftleft(BigInteger *integer, int n);
int BigInteger_shiftright(BigInteger *integer, int n);
int BigInteger_cmp_unsigned(const BigInteger *left, const BigInteger *right);
#endif

