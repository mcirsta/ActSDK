/********************************************************************************
 *                               USDK(1100)
 *                            Module: MULTIFUNC
 *                 Copyright(c) 2001-2008 Actions Semiconductor,
 *                            All Rights Reserved.
 *
 * History:
 *      <author>    <time>           <version >             <desc>
 *       hmwei     2008-6-12 9:42     1.0             build this file
 ********************************************************************************/
/*!
 * \file     multifunc.h
 * \brief    �๦��pin����������ݽṹ����ͺ����ӿ�����
 * \author   hmwei
 * \version 1.0
 * \date  2008/2/1
 *******************************************************************************/
#ifndef __ACT213X_MULTIFUNC_H
#define __ACT213X_MULTIFUNC_H

#define MULTIFUNC_CTRL0 0xB01C0018
#define MULTIFUNC_CTRL1 0xB01C001C

#ifndef _ASSEMBLER_

#ifndef NULL
#define NULL 0
#endif

///*!
// *  �๦��pin��ģ�� \n
// *  \li 0~16 : ��17��ģ���漰pin�Ÿ���
// *
// */
//enum {
//        PIN_OWNER_NAND = 0,
//        PIN_OWNER_NOR,
//        PIN_OWNER_LCD,
//        PIN_OWNER_ATA,
//        PIN_OWNER_SD,
//        PIN_OWNER_YUV,
//        PIN_OWNER_ICE,
//        PIN_OWNER_USB,
//        PIN_OWNER_BT656,
//        PIN_OWNER_I2S,
//        PIN_OWNER_UART1,
//        PIN_OWNER_UART2,
//        PIN_OWNER_I2C,
//        PIN_OWNER_SPI,
//        PIN_OWNER_SPDIF,
//        PIN_OWNER_KEY,
//        PIN_OWNER_EJTAG
//};

/*!
 *  �๦��pin������ \n
 *  \li PIN_SHARE_LCM_CARD_FLASH : ����pin����0
 *  \li PIN_SHARE_I2C : ����pin����1
 */
enum
{
    PIN_SHARE_LCM_CARD_FLASH = 0,
    PIN_SHARE_I2C,
    PIN_SHARE_MAX
};

/*!
 *  �๦��pin�������� \n
 *  \li PIN_WAIT_CAN_SLEEP : ����pin��ʱ������˯��
 *  \li PIN_WAIT_NO_SLEEP : ����pin��ʱ����ֹ˯��
 */
enum
{
    PIN_WAIT_CAN_SLEEP = 0,
    PIN_WAIT_NO_SLEEP
};

typedef int (* WAIT_PIN_FUN_T)(void *);

extern int set_multip_reg(unsigned int multif_ctr0_val,
        unsigned int multif_ctr1_val, unsigned int optype);
extern int get_multip_reg(unsigned int* multif_ctr0_val,
        unsigned int* multif_ctr1_val, unsigned int optype);
extern int multip_get_pinowner(unsigned int pin_chan, int type,
        int(* wait_pin_func)(void *), void * dev_ptr);
extern int multip_release_pinowner(unsigned int pin_chan, int type);

extern void multip_init(void);

#endif /*_ASSEMBLER_*/
#endif /*__ACT213X_MULTIFUNC_H*/
