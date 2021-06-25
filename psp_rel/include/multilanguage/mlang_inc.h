/*******************************************************************************
 *                              这里填写项目名
 *                            Module: 这里填写模块名
 *                 Copyright(c) 2003-2008 Actions Semiconductor,
 *                            All Rights Reserved.
 *
 * History:
 *      <author>    <time>           <version >             <desc>
 *       rcmai     2008-02-01 15:00     1.0             build this file
 *******************************************************************************/
/*!
 * \file     这里请真实填写文件名字
 * \brief    这里填写文件的概述
 * \author   这里填写文件的作者
 * \version 1.0
 * \date  2007/5/4
 *******************************************************************************/
#ifndef __MLANG_INC_H__
#define __MLANG_INC_H__
/*!\cond MLANG*/

int mlang_unicode_to_mbcs(unsigned short *src_ucs, unsigned char*dst_mbcs,
        int src_len, int *dst_len, unsigned int language_id);
int mlang_mbcs_to_unicode(unsigned char *str, unsigned short *uni, int src_len,
        int *dst_len, unsigned int language_id);
int mlang_unicode_to_utf8(unsigned short *unicode, int unicode_len, char *utf8,
        int *putf8_len);
int mlang_utf8_to_unicode(char *utf8, int utf8_len, unsigned short *unicode,
        int *punicode_len);
int mlang_mbcs_to_utf8(unsigned short *mbcs, int mbcs_len, char *utf8,
        int *putf8_len, int language);

/*!\endcond*/
#endif /*_MLANG_INC_H_*/
