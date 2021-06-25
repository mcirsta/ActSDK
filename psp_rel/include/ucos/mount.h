/********************************************************************************
 *                              USDK1100
 *                            Module: MOUNT
 *                 Copyright(c) 2003-2009 Actions Semiconductor,
 *                            All Rights Reserved.
 *
 * History:
 *      <author>    <time>           <version >             <desc>
 *       hmwei     2008-8-30 16:45     1.0             build this file
 ********************************************************************************/
/*!
 * \file     mount.h
 * \brief    fs mount������ݽṹ����
 * \author   hmwei
 * \version 1.0
 * \date  2008/8/30
 *******************************************************************************/
#ifndef __MOUNT_H__
#define __MOUNT_H__

#include "asm-mips/atomic.h"
#include "ucos/list.h"
#include "ucos/fs.h"

#define MNT_NOSUID  1
#define MNT_NODEV   2
#define MNT_NOEXEC  4

#ifndef _ASSEMBLER_

/*!
 * \brief
 *      �ļ�ϵͳ��װ������ݽṹ
 */
struct vfsmount
{
    /*! ���ڵ��Ŀ¼��*/
    struct dentry *mnt_mountpoint;
    /*! ����super block*/
    struct super_block *mnt_sb;
    /*! Ŀ¼����װ����*/
    atomic_t mnt_count;
    /*! ��־*/
    int mnt_flags;
    /*! ����װ�ļ�ϵͳ���豸����*/
    char *mnt_devname; /* Name of device e.g. /dev/dsk/hda1 */
    /*! vfsmount����ͷ���*/
    struct list_head mnt_list;
};

extern void __mntput(struct vfsmount *mnt);

static inline void mntput_no_expire(struct vfsmount *mnt)
{
    if (mnt)
    {
        __mntput(mnt);
    }
}
#endif /*_ASSEMBLER_*/
#endif  /*__MOUNT_H__*/
