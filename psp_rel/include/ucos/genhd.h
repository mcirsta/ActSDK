/*******************************************************************************
 *                              USDK213F
 *                            Module: GENHD
 *                 Copyright(c) 2003-2009 Actions Semiconductor,
 *                            All Rights Reserved.
 *
 * History:
 *      <author>    <time>           <version >             <desc>
 *       hmwei     2009-07-10 10:56     1.0             build this file
 *******************************************************************************/
/*!
 * \file     genhd.h
 * \brief    磁盘管理头文件
 * \author   hmwei
 * \version 1.0
 * \date  2007/5/4
 *******************************************************************************/
#ifndef __GENHD_H__
#define __GENHD_H__

#include "ucos/types.h"
#include "ucos/fs.h"
#include "ucos/blkdev.h"

#define GENHD_FL_REMOVABLE      1
#define GENHD_FL_DRIVERFS       2
#define GENHD_FL_CD             8
#define GENHD_FL_UP             16
#define GENHD_FL_SUPPRESS_PARTITION_INFO    32

#ifndef _ASSEMBLER_

struct partition
{
    unsigned char boot_ind; /* 0x80 - active */
    unsigned char head; /* starting head */
    unsigned char sector; /* starting sector */
    unsigned char cyl; /* starting cylinder */
    unsigned char sys_ind; /* What partition type */
    unsigned char end_head; /* end head */
    unsigned char end_sector; /* end sector */
    unsigned char end_cyl; /* end cylinder */
    __le32 start_sect; /* starting sector counting from 0 */
    __le32 nr_sects; /* nr of sectors in partition */
};//__attribute__((packed));

struct hd_struct
{
    sector_t start_sect;
    sector_t nr_sects;
    //  struct kobject kobj;
    unsigned reads, read_sectors, writes, write_sectors;
    int policy, partno;
};

/*!
 * \brief
 *      磁盘数据结构
 */
struct gendisk
{
    /*! 磁盘主设备号*/
    int major;
    /*! 磁盘的首次设备号*/
    int first_minor;
    /*! 磁盘最大次设备号，如果=1，则该磁盘不能被分区*/
    int minors;
    /*! 磁盘名称*/
    char disk_name[32];
    /*! 磁盘分区数组指针(按此设备号索引)*/
    struct hd_struct **part;
    /*! 磁盘链表next*/
    struct gendisk *next;
    /*! 块设备操作集*/
    struct block_device_operations *fops;
    /*! 块设备请求队列指针*/
    struct request_queue *queue;
    /*! 私有数据*/
    void *private_data;
    /*! 容量*/
    sector_t capacity;
    /*! 标志*/
    int flags;
};


/*!
 * \brief
 *      磁盘私有数据结构
 */
typedef struct gendisk_info
{
    /*! 分区设备*/
    void    *dev;
    /*! 扇区大小*/
    unsigned int    sector_size;
}gendisk_info_t;


static inline sector_t get_capacity(struct gendisk *disk)
{
    return disk->capacity;
}

static inline void set_capacity(struct gendisk *disk, sector_t size)
{
    disk->capacity = size;
}

extern struct gendisk *alloc_disk(int minors);
extern void put_disk(struct gendisk *disk);
extern void add_disk(struct gendisk *disk);
extern void del_gendisk(struct gendisk *gp);
extern struct gendisk *get_gendisk(dev_t dev);
extern struct gendisk *get_gendisk_by_rq(request_queue_t *q);

#endif /*_ASSEMBLER_*/
#endif /* __GENHD_H__ */
