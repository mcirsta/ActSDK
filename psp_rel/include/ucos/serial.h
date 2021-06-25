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
 * \brief    ���ڲ���ͷ�ļ�
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
 *  ��ӡ�������ͨ��
 *  \li UART : �������
 *  \li USB : USB���
 *  \li EXTEND : ��չ�����file��
 */
enum
{
    UART,
    USB,
    EXTEND
};

/*!
 *  \brief
 *  ��ǰ�Ĵ�ӡ�������ͨ��
 */
extern int print_channel;
/*!
 *  \brief
 *  ��ӡ�����־\n
 *  bit31��0---UART�����1---USB���\n
 *  bit30��0---������USB����ں�ģ�飻1---����usb����ں�ģ��
 */
extern int print_debug_flag;
/*!
 *  \brief
 *  USB��ӡ�������ָ��\n
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
