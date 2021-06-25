/********************************************************************************
 *                              USDK(1100)
 *                            Module: GENERAL_BLOCK
 *                 Copyright(c) 2003-2008 Actions Semiconductor,
 *                            All Rights Reserved.
 *
 * History:
 *      <author>    <time>           <version >             <desc>
 *       hmwei     2008-8-30 16:45     1.0             build this file
 ********************************************************************************/
/*!
 * \file     general_block.h
 * \brief    通用block层头文件
 * \author   hmwei
 * \version 1.0
 * \date  2008/8/30
 *******************************************************************************/
#ifndef __GENERAL_BLOCK_H__
#define __GENERAL_BLOCK_H__

#include "ucos/types.h"
#include "ucos/fs.h"

#ifndef _ASSEMBLER_

extern ssize_t blk_read(struct inode *inode, struct file *file, char *buf,
        size_t size, loff_t *ppos);
extern ssize_t blk_write(struct inode *inode, struct file *file,
        const char *buf, size_t count, loff_t *ppos);
extern int blk_ioctl(struct inode *inode, struct file *file, unsigned int cmd,
        unsigned long arg);

extern struct block_device *bd_acquire(struct inode *inode);
extern void bd_release(struct inode *inode);

#endif /*_ASSEMBLER_*/
#endif	/* __GENERAL_BLOCK_H__ */
