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
 * \brief    Ŀ¼��ͷ�ļ�
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
    /*! ��ϣ��index*/
    unsigned int hash;
    /*! �ַ�������*/
    unsigned int len;
    /*! �ַ�����ָ��*/
    const unsigned char *name;
};

/*!
 * \brief
 *      Ŀ¼�����ݽṹ�����Ƴ��ȵ����ֵ
 * \note
 * ���Ŀ¼�����Ƴ��������ֵ�������ͨ��malloc����ռ�
 */
#define DNAME_INLINE_LEN_MAX 16

/*!
 * \brief
 *      Ŀ¼��ṹ����.
 */
struct dentry
{
    /*! Ŀ¼��ָ��������ڵ�ָ��*/
    struct inode *d_inode;
    /*! ��Ŀ¼��ָ��*/
    struct dentry *d_parent;
    /*! Ŀ¼������quick string���ݽṹ*/
    struct qstr d_name;
    /*! ��Ŀ¼�����Ŀ¼������*/
    struct list_head d_child;
    /*! ��Ŀ¼������*/
    struct list_head d_subdirs;
    /*! ʱ����Ϣ*/
    unsigned long d_time;
    /*! super_block���ݽṹָ��*/
    struct super_block *d_sb;
    /*! �Ƿ�װ���ļ�ϵͳ*/
    int d_mounted;
    /*! �϶�����*/
    unsigned char d_iname[DNAME_INLINE_LEN_MAX];
    /*! ����*/
    unsigned short d_count;
    /*! ��*/
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
