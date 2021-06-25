/********************************************************************************
 *                              USDK1100
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
 * \brief    setjmp/longjmp与硬件体系架构相关函数声明和相关数据结构
 * \author   hmwei
 * \version 1.0
 * \date  2009/1/5
 *******************************************************************************/
#ifndef __ASM_SETJMP_H__
#define __ASM_SETJMP_H__

typedef struct
{
    /* Program counter.  */
    void * __pc;
    /* Stack pointer.  */
    void * __sp;
    /* Callee-saved registers s0 through s7.  */
    int __regs[8];
    /* The frame pointer.  */
    void * __fp;
    /* The global pointer.  */
    void * __gp;
} jmp_buf[1];

#endif /* __ASM_SETJMP_H__ */
