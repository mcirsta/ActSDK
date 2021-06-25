/********************************************************************************
 *                               USDK(1100)
 *                            Module: SETJMP
 *                 Copyright(c) 2003-2008 Actions Semiconductor,
 *                            All Rights Reserved.
 *
 * History:
 *      <author>    <time>           <version >             <desc>
 *       hmwei     2009-1-5 19:56     1.0             build this file
 ********************************************************************************/
/*!
 * \file     setjmp.h
 * \brief    setjmp/longjmpº¯ÊýÉùÃ÷
 * \author   hmwei
 * \version 1.0
 * \date  2009/1/5
 *******************************************************************************/
#ifndef __OS_SETJMP_H__
#define __OS_SETJMP_H__

#include <asm-mips/setjmp.h>

#ifndef _ASSEMBLER_

extern int setjmp(jmp_buf __env);
extern void longjmp(jmp_buf __env, int __val);

#endif /*_ASSEMBLER_*/
#endif /* __OS_SETJMP_H__ */
