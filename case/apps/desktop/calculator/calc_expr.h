/*
 ********************************************************************************
 *                         uSDK130
 *
 *
 *
 *                     (c) Copyright 2002-2003, Actions Co,Ld.
 *                                       All Right Reserved
 *
 * File   :  calc_expr.h
 * By     :  ywwang
 * Purpose:   �ṩ���ʽ���㹦��
 *
 * Version:       first version     2006-12-5
 ********************************************************************************
 ********************************************************************************
 */

/*******************************************************************************
 *
 *    ģ���ṩ�����б�:
 *	int calc(char *expr, float_type *retVar); exprΪ��ȷ�ı��ʽ!!!
 *
 ********************************************************************************/

#ifndef CALC_EXPR_H
#define CALC_EXPR_H

#include "float_simlib.h"
int calc(char *expr, float_type *retVar);

/*
 *      �ж�ĳ�����ʽ�Ƿ�ֻ��һ�����������������κ�
 *     �����,����ǵ���������������1,���򷵻�0
 *
 */
int isStringSingleOperand(const char *src);

/*
 *     ���ĳ�����Ƿ����С����(С��)������з���1,���򷵻�0
 *
 *   ע��!!!:  src�����ǺϷ��������ַ���
 */
int isStringContainDot(const char *src);

int exprValidate(char *expr);

#endif
