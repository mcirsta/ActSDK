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
 * Purpose:   ���ö���ģ�⸡������
 *
 * Version:       first version     2006-12-5
 ********************************************************************************
 ********************************************************************************
 */

/*******************************************************************************
 *
 *    ģ���ṩ�����б�:
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

#define MAX_DECIMAL_LEN   7          //С�����ֵĳ���
#define INTEGER_LEN   (MAX_NUMBER_COUNT-MAX_DECIMAL_LEN)   //�������ֵĳ���
typedef struct
{
    BigInteger bInteger;
    int declen; //С�����ֵĳ���
} float_type;

/* ��С�����������ֹ����ڲ���ʽ�ĸ�����*/

int getFloatFromStr(char *str, float_type *value);
int getStrFromFloat(char *str, float_type *value);

/*���������*/
int float_add(float_type *x, float_type *y, float_type *result);

/** ���������
 *
 *    x-y  = x+ (-y)
 */
int float_dec(float_type *x, float_type *y, float_type *result);
/**
 *   ���������  x*y
 *
 */
int float_mul(float_type *x, float_type *y, float_type *result);
/**
 *
 *    ���������x/y
 *
 */
int float_div(float_type *x, float_type *y, float_type *result);

int float_sqrt(const float_type *x_arg, float_type *result);

/**
 *
 *      ����
 */
int float_reverse(float_type *x, float_type *result);

/**
 *
 *      ����������
 */
int float_mod(float_type *x, float_type *y, float_type *result);

int float_striple_pend_zero(char * floatStr);

int float_isZero(const float_type *x);
#endif

