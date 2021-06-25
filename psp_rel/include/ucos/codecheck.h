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
 * \brief    ����У��������ݽṹ�ͺ�������
 * \author   hmwei
 * \version 1.0
 * \date  2008/6/12
 *******************************************************************************/
#ifndef __CODECHECK_H__
#define __CODECHECK_H__

#ifndef _ASSEMBLER_

/*!
 *  totalCodeCheck���� \n
 *  \li START_CHECK : ��ʼУ��
 *  \li GET_TOAL_LEN: ��ȡ����������
 *  \li GET_CUR_LEN: ��ȡУ�鵱ǰƫ��
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
 *      У������ݽṹ
 */
struct checksum_info
{
    /*! maigc:'A''C''R''C'*/
    char magic[4];
    /*! У���*/
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
