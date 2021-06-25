/********************************************************************************
 *                               USDK(1100)
 *                            Module: SYMBOL
 *                 Copyright(c) 2003-2008 Actions Semiconductor,
 *                            All Rights Reserved.
 *
 * History:
 *      <author>    <time>           <version >             <desc>
 *       hmwei     2008-6-12 9:42     1.0             build this file
 ********************************************************************************/
/*!
 * \file     symbol.h
 * \brief    symbol相关接口声明
 * \author   hmwei
 * \version 1.0
 * \date  2008/6/12
 *******************************************************************************/

#ifndef __SYMBOL_H__
#define __SYMBOL_H__

#ifndef _ASSEMBLER_

extern int Hash(char * key);
extern int insert_symbol(char * key, int j);
extern int del_symbol(char * key);
extern void *kernel_sym(const char *name);

#endif /*_ASSEMBLER_*/
#endif /*__SYMBOL_H__*/
