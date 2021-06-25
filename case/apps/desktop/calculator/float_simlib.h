/*
 ********************************************************************************
 *                         uSDK130
 *
 *
 *
 *                     (c) Copyright 2002-2003, Actions Co,Ld.
 *                                       All Right Reserved
 *
 * File   :  float_simlib.h
 * By     :  ywwang
 * Purpose:   利用定点模拟浮点运算
 *
 * Version:       first version     2006-12-5
 ********************************************************************************
 ********************************************************************************
 */

/*******************************************************************************
 *
 *    模块提供函数列表:
 *
 ********************************************************************************/

#ifndef FLOAT_SIMLIB_H
#define FLOAT_SIMLIB_H

#ifndef LONG
#define LONG long
#endif
#ifndef ULONG
#define ULONG unsigned long
#endif
#ifndef UINT
#define UINT  unsigned int
#endif

#include "BigInteger.h"

#define MAX_DECIMAL_LEN   7          //小数部分的长度
#define INTEGER_LEN   (MAX_NUMBER_COUNT-MAX_DECIMAL_LEN)   //整数部分的长度
typedef struct
{
    BigInteger bInteger;
    int declen; //小数部分的长度
} float_type;

/* 从小数和整数部分构造内部格式的浮点数*/

int getFloatFromStr(char *str, float_type *value);
int getStrFromFloat(char *str, float_type *value);

/*浮点数相加*/
int float_add(float_type *x, float_type *y, float_type *result);

/** 浮点数相减
 *
 *    x-y  = x+ (-y)
 */
int float_dec(float_type *x, float_type *y, float_type *result);
/**
 *   浮点数相乘  x*y
 *
 */
int float_mul(float_type *x, float_type *y, float_type *result);
/**
 *
 *    浮点数相除x/y
 *
 */
int float_div(float_type *x, float_type *y, float_type *result);

int float_sqrt(const float_type *x_arg, float_type *result);

/**
 *
 *      求倒数
 */
int float_reverse(float_type *x, float_type *result);

/**
 *
 *      对整数求余
 */
int float_mod(float_type *x, float_type *y, float_type *result);

int float_striple_pend_zero(char * floatStr);

int float_isZero(const float_type *x);
#endif

