/********************************************************************************
 *                              USDK(1100)
 *                            Module: DEVICE
 *                 Copyright(c) 2001-2008 Actions Semiconductor,
 *                            All Rights Reserved.
 *
 * History:
 *      <author>    <time>           <version >             <desc>
 *       hmwei     2008-6-12 9:42     1.0             build this file
 ********************************************************************************/
/*!
 * \file     device.h
 * \brief    soc���������
 * \author   hmwei
 * \version 1.0
 * \date  2008/6/12
 *******************************************************************************/
#ifndef __DEVICE_H__
#define __DEVICE_H__

#ifndef _ASSEMBLER_

/*!
 * \brief
 *      �弶��Ϣ�����ݽṹ
 */
typedef struct
{
    /*! ic�汾��'C'/'E'*/
    unsigned char ic_version;
    /*! SDRAM��������λΪMBytes*/
    unsigned char sdram_cap;
    /*! ic���ͣ�0x63/0x96*/
    unsigned char ic_type;
} board_info_t;

extern board_info_t board_info;
extern unsigned int IC_series;

extern unsigned char set_ic_info(void);
extern unsigned char get_ic_version(void);//����ֵ����C��---C��/O��IC����E��---E��IC����A��---����
extern unsigned char get_ic_type(void);//����ֵ��0x96/0x63
extern unsigned char get_sdram_cap(void);//����MBytes����NOYAW�淵��ֵΪ8Mbytes����ֻ��ʹ��ǰ4Mbytes��

extern unsigned int get_run_mode(void);
extern void set_run_mode(unsigned char mode);
extern void install_stub(unsigned char install);
extern unsigned char is_stub_installed(void);

#endif /*_ASSEMBLER_*/
#endif /*__DEVICE_H__*/
