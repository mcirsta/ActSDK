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
 * Purpose:   提供表达式计算功能
 *
 * Version:       first version     2006-12-5
 ********************************************************************************
 ********************************************************************************
 */

/*******************************************************************************
 *
 *    模块提供函数列表:
 *	int calc(char *expr, float_type *retVar); expr为正确的表达式!!!
 *
 ********************************************************************************/

#ifndef CALC_EXPR_H
#define CALC_EXPR_H

#include "float_simlib.h"
int calc(char *expr, float_type *retVar);

/*
 *      判断某个表达式是否只有一个操作数，不包含任何
 *     运算符,如果是单个操作数，返回1,否则返回0
 *
 */
int isStringSingleOperand(const char *src);

/*
 *     检查某个数是否包含小数点(小数)，如果有返回1,否则返回0
 *
 *   注意!!!:  src必须是合法的数据字符串
 */
int isStringContainDot(const char *src);

int exprValidate(char *expr);

#endif
