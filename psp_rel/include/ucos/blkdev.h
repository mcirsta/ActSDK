/********************************************************************************
 *                              USDK213F
 *                            Module: BLKDEV
 *                 Copyright(c) 2003-2009 Actions Semiconductor,
 *                            All Rights Reserved.
 *
 * History:
 *      <author>    <time>           <version >             <desc>
 *       hmwei     2008-8-30 16:45     1.0             build this file
 ********************************************************************************/
/*!
 * \file     blkdev.h
 * \brief    块设备操作头文件
 * \author   hmwei
 * \version 1.0
 * \date  2008/8/30
 *******************************************************************************/
#ifndef __BLKDEV_H__
#define __BLKDEV_H__

#include "ucos/types.h"

#ifndef _ASSEMBLER_

struct request_queue;
/*!
 *  \brief
 *      定义struct request_queue为request_queue_t
 */
typedef struct request_queue request_queue_t;
/*!
 *  \brief
 *      request_fn_proc -函数指针定义
 */
typedef void ( request_fn_proc)(request_queue_t *q);

/*!
 *  \brief
 *     块设备请求数据结构
 */
struct request
{
    /*! 请求类型：读或写*/
    unsigned long flags; /* see REQ_ bits below */
    /*! 请求起始扇区*/
    sector_t sector; /* next sector to submit */
    /*! 请求中扇区数*/
    unsigned int current_nr_sectors;
    /*! 快设备结构指针*/
    struct gendisk *rq_disk;
    /*! 错误信息*/
    int errors;
    /*! 请求地址*/
    char *buffer;
};

/*!
 *  \brief
 *     块设备请求队列数据结构
 */
struct request_queue
{
    /*! 块设备请求处理函数指针*/
    request_fn_proc *request_fn;
    /*! 块设备请求*/
    struct request *last_merge;
    /*! 队列锁（用信号量实现）*/
    void *q_lock;
};

#define rq_data_dir(rq)     ((rq)->flags & 1)

extern struct file_operations def_blk_fops;
extern struct file_operations def_chr_fops;

extern request_queue_t *blk_init_queue(request_fn_proc *, void *);
extern void blk_cleanup_queue(request_queue_t *);

extern void end_request(struct request *req, int uptodate);
extern struct request *elv_next_request(struct request_queue *q);

#endif /*_ASSEMBLER_*/
#endif /* __BLKDEV_H__ */
