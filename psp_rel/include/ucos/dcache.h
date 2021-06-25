/********************************************************************************
 *                              USDK1100
 *                            Module: DCACHE
 *                 Copyright(c) 2003-2009 Actions Semiconductor,
 *                            All Rights Reserved.
 *
 * History:
 *      <author>    <time>           <version >             <desc>
 *       hmwei     2008-8-30 16:45     1.0             build this file
 ********************************************************************************/
/*!
 * \file     dcache.h
 * \brief    目录项头文件
 * \author   hmwei
 * \version 1.0
 * \date  2008/8/30
 *******************************************************************************/
#ifndef __DCACHE_H__
#define __DCACHE_H__

#include "ucos/list.h"

#ifndef _ASSEMBLER_

#define IS_ROOT(x) ((x) == (x)->d_parent)

/*!
 * \brief
 *      quick string
 */
struct qstr
{
    /*! 哈希表index*/
    unsigned int hash;
    /*! 字符串长度*/
    unsigned int len;
    /*! 字符数组指针*/
    const unsigned char *name;
};

/*!
 * \brief
 *      目录项数据结构内名称长度的最大值
 * \note
 * 如果目录项名称超过该最大值，会额外通过malloc申请空间
 */
#define DNAME_INLINE_LEN_MAX 16

/*!
 * \brief
 *      目录项结构定义.
 */
struct dentry
{
    /*! 目录项指向的索引节点指针*/
    struct inode *d_inode;
    /*! 父目录项指针*/
    struct dentry *d_parent;
    /*! 目录项名称quick string数据结构*/
    struct qstr d_name;
    /*! 父目录项的子目录项链表*/
    struct list_head d_child;
    /*! 子目录项链表*/
    struct list_head d_subdirs;
    /*! 时间信息*/
    unsigned long d_time;
    /*! super_block数据结构指针*/
    struct super_block *d_sb;
    /*! 是否安装了文件系统*/
    int d_mounted;
    /*! 较短名称*/
    unsigned char d_iname[DNAME_INLINE_LEN_MAX];
    /*! 计数*/
    unsigned short d_count;
    /*! 锁*/
    void *d_lock;
};

static inline struct dentry *dget(struct dentry *dentry)
{
    return dentry;
}
static inline struct dentry *dget_parent(struct dentry *dentry)
{
    struct dentry *ret;

    //      spin_lock(&dentry->d_lock);
    ret = dget(dentry->d_parent);
    //      spin_unlock(&dentry->d_lock);
    return ret;
}

static inline int d_mountpoint(struct dentry *dentry)
{
    return dentry->d_mounted;
}

extern struct list_head mntlist_head;

extern char * getname(const char *filename);
extern char * get_rel_name(const char *filename);
extern int get_base_name(const char *filename, char *buf);
extern void putname(const char *name);
extern struct dentry *lookup_dentry(const char *filename);
extern void vfs_caches_init(unsigned long mempages);

extern void dput(struct dentry *);
extern void d_instantiate(struct dentry *entry, struct inode * inode);
extern struct dentry *d_alloc(struct dentry * parent, const struct qstr *name);
extern struct dentry * d_alloc_root(struct inode * root_inode);

extern struct vfsmount *lookup_mnt(struct dentry *);

#endif /*_ASSEMBLER_*/
#endif  /* __DCACHE_H__ */
