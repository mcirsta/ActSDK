/********************************************************************************
 *                              USDK(1100)
 *                            Module: OS
 *                 Copyright(c) 2003-2008 Actions Semiconductor,
 *                            All Rights Reserved.
 *
 * History:
 *      <author>    <time>           <version >             <desc>
 *       hmwei     2009-1-10 16:45     1.0             build this file
 ********************************************************************************/
/*!
 * \file     os_cpu.h
 * \brief    ucos需要移植的文件之一，包括类型定义、栈增长方向和临界区保护宏定义
 * \author   hmwei
 * \version 1.0
 * \date  2009/1/10
 *******************************************************************************/

#ifndef __OS_CPU_H__
#define __OS_CPU_H__

#ifdef  OS_CPU_GLOBALS
#define OS_CPU_EXT
#else
#define OS_CPU_EXT  extern
#endif

/*
 *********************************************************************************************************
 *                                              DATA TYPES
 *                                         (Compiler Specific)
 *********************************************************************************************************
 */
#include <ucos/types.h>
/*
 *********************************************************************************************************
 *                              MIPS
 *
 * Method #1:  Disable/Enable interrupts using simple instructions.  After critical section, interrupts
 *             will be enabled even if they were disabled before entering the critical section.
 *
 * Method #2:  Disable/Enable interrupts by preserving the state of interrupts.  In other words, if
 *             interrupts were disabled before entering the critical section, they will be disabled when
 *             leaving the critical section.
 *
 * Method #3:  Disable/Enable interrupts by preserving the state of interrupts.  Generally speaking you
 *             would store the state of the interrupt disable flag in the local variable 'cpu_sr' and then
 *             disable interrupts.  'cpu_sr' is allocated in all of uC/OS-II's functions that need to
 *             disable interrupts.  You would restore the interrupt disable state by copying back 'cpu_sr'
 *             into the CPU's status register.
 *
 * Note     :  In this MIPS Port: Only Method #3 is implemeted.
 *
 *********************************************************************************************************
 */
/* Don't modify these lines. This port can only support OS_CRITICAL_METHOD 3. */
#define  OS_CRITICAL_METHOD    3

#if      OS_CRITICAL_METHOD == 1
#define  OS_ENTER_CRITICAL()  (cpu_sr = os_cpu_save_sr())    /* Disable interrupts                        */
#define  OS_EXIT_CRITICAL()   (os_cpu_store_sr(cpu_sr))    /* Restore  interrupts                       */
#endif

#if      OS_CRITICAL_METHOD == 3
#define  OS_INT_SAVE_AREA      unsigned int  cpu_sr;
#define  OS_ENTER_CRITICAL()  ( cpu_sr = os_cpu_save_sr() )    /* Disable interrupts                        */
#define  OS_EXIT_CRITICAL()   ( os_cpu_store_sr(cpu_sr) )    /* Restore  interrupts                       */
#endif

/*
 *********************************************************************************************************
 *                           ARM Miscellaneous
 *********************************************************************************************************
 */

#define  OS_STK_GROWTH        1                       /* Stack grows from HIGH to LOW memory on 80x86  */

#if OS_CRITICAL_METHOD == 3                      /* Allocate storage for CPU status register           */
unsigned int os_cpu_save_sr(void);
void os_cpu_store_sr(unsigned int cpu_sr);
#endif

#endif /*__OS_CPU_H__*/
