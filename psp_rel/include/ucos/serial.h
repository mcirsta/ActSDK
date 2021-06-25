/********************************************************************************
 *                               USDK(1100)
 *                            Module: SERIAL
 *                 Copyright(c) 2001-2008 Actions Semiconductor,
 *                            All Rights Reserved.
 *
 * History:
 *      <author>    <time>           <version >             <desc>
 *       hmwei     2008-6-12 9:42     1.0             build this file
 ********************************************************************************/
/*!
 * \file     serial.h
 * \brief    串口操作头文件
 * \author   hmwei
 * \version 1.0
 * \date  2008/6/12
 *******************************************************************************/
#ifndef __OS_SERIAL_H__
#define __OS_SERIAL_H__

#ifndef _ASSEMBLER_

#include "pm.h"
/*!
 *  \brief
 *  打印输出物理通道
 *  \li UART : 串口输出
 *  \li USB : USB输出
 *  \li EXTEND : 扩展输出（file）
 */
enum
{
    UART,
    USB,
    EXTEND
};

/*!
 *  \brief
 *  当前的打印输出物理通道
 */
extern int print_channel;
/*!
 *  \brief
 *  打印输出标志\n
 *  bit31：0---UART输出；1---USB输出\n
 *  bit30：0---不调试USB相关内核模块；1---调试usb相关内核模块
 */
extern int print_debug_flag;
/*!
 *  \brief
 *  USB打印输出函数指针\n
 */
extern int (*usb_print)(int, void*, unsigned int);//the type of the first argument is trans_mode_t

extern int serial_init(void);
extern int serial_getc(void);
extern void serial_setbrg(unsigned int baudrate);
extern char reset_baudrate(unsigned int baudrate);

extern int set_extend_print(void *ptr);
extern int get_debug_flag(void);
extern void set_debug_flag(int flag);

extern int printf(const char *fmt, ...);
extern int printk(const char *fmt, ...);

extern int serial_pm_cb(unsigned int clk, curclk_t *curclk, void *dev_id,
        unsigned int cmd, unsigned int direction);
#endif /*_ASSEMBLER_*/
#endif /*__OS_SERIAL_H__*/
