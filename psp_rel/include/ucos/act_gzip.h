/*******************************************************************************
 *                              USDK1100
 *                            Module: ACT_GZIP
 *                 Copyright(c) 2003-2009 Actions Semiconductor,
 *                            All Rights Reserved.
 *
 * History:
 *      <author>    <time>           <version >             <desc>
 *       hmwei     2009-9-21 11:25     1.0             build this file
 *******************************************************************************/
/*!
 * \file     hmwei
 * \brief    act_gzip文件格式相关定义
 * \author   hmwei
 * \version 1.0
 * \date  2009/9/21
 *******************************************************************************/
#ifndef __ACT_GZIP_H__
#define __ACT_GZIP_H__

#ifndef _ASSEMBLER_

extern int act_uncompress(int fd, unsigned int offset, unsigned int in_len,
        char *out_buf, unsigned int *out_len);

extern int act_uncompress2(int fd, unsigned int offset, unsigned int in_start,
        unsigned int in_len, char *out_buf, unsigned int *out_len);
#endif /*_ASSEMBLER_*/

#endif /*__ACT_GZIP_H__ */
