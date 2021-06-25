/********************************************************************************
 *                              USDK130
 *                            Module: CODECHECK
 *                 Copyright(c) 2001-2008 Actions Semiconductor,
 *                            All Rights Reserved.
 *
 * History:
 *      <author>    <time>           <version >             <desc>
 *       hmwei     2008-6-12 9:42     1.0             build this file
 ********************************************************************************/
/*!
 * \file     codecheck.h
 * \brief    代码校验相关数据结构和函数声明
 * \author   hmwei
 * \version 1.0
 * \date  2008/6/12
 *******************************************************************************/
#ifndef __CODECHECK_H__
#define __CODECHECK_H__

#ifndef _ASSEMBLER_

/*!
 *  totalCodeCheck参数 \n
 *  \li START_CHECK : 开始校验
 *  \li GET_TOAL_LEN: 获取代码区长度
 *  \li GET_CUR_LEN: 获取校验当前偏移
 *
 */
enum
{
    GET_TOAL_LEN,
    START_CHECK,
    GET_CUR_LEN,
    CHECK_MAX
};

/*!
 * \brief
 *      校验和数据结构
 */
struct checksum_info
{
    /*! maigc:'A''C''R''C'*/
    char magic[4];
    /*! 校验和*/
    unsigned int checksum;
};

extern int cal_checksum(unsigned char *buf, unsigned int length,
        unsigned char nbytes, unsigned int *result);
extern int check_checksum_magic(struct checksum_info *p_checksum_info);

extern int load_codecheck(char *name);
extern int system_codecheck(unsigned int cmd, void *arg);
extern unsigned int cal_crc(unsigned char *buf, unsigned int length,
        unsigned char nbytes);

#endif /*_ASSEMBLER_*/
#endif/*__CODECHECK_H__*/
