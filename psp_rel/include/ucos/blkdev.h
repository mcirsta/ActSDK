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
 * \brief    ���豸����ͷ�ļ�
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
 *      ����struct request_queueΪrequest_queue_t
 */
typedef struct request_queue request_queue_t;
/*!
 *  \brief
 *      request_fn_proc -����ָ�붨��
 */
typedef void ( request_fn_proc)(request_queue_t *q);

/*!
 *  \brief
 *     ���豸�������ݽṹ
 */
struct request
{
    /*! �������ͣ�����д*/
    unsigned long flags; /* see REQ_ bits below */
    /*! ������ʼ����*/
    sector_t sector; /* next sector to submit */
    /*! ������������*/
    unsigned int current_nr_sectors;
    /*! ���豸�ṹָ��*/
    struct gendisk *rq_disk;
    /*! ������Ϣ*/
    int errors;
    /*! �����ַ*/
    char *buffer;
};

/*!
 *  \brief
 *     ���豸����������ݽṹ
 */
struct request_queue
{
    /*! ���豸��������ָ��*/
    request_fn_proc *request_fn;
    /*! ���豸����*/
    struct request *last_merge;
    /*! �����������ź���ʵ�֣�*/
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
