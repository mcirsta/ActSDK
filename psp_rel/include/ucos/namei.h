/********************************************************************************
 *                              USDK(1100)
 *                            Module: NAMEI
 *                 Copyright(c) 2003-2008 Actions Semiconductor,
 *                            All Rights Reserved.
 *
 * History:
 *      <author>    <time>           <version >             <desc>
 *       hmwei     2008-8-30 16:45     1.0             build this file
 ********************************************************************************/
/*!
 * \file     namei.h
 * \brief    目录项/索引节点定位头文件
 * \author   hmwei
 * \version 1.0
 * \date 2008/8/30
 *******************************************************************************/
#ifndef __NAMEI_H__
#define __NAMEI_H__

#include "ucos/fs.h"
#include "ucos/dcache.h"

/*
 * The bitmask for a lookup event:
 *  - follow links at the end
 *  - require a directory
 *  - ending slashes ok even for nonexistent files
 *  - internal "there are more path compnents" flag
 *  - locked when lookup done with dcache_lock held
 *  - dentry cache is untrusted; force a real lookup
 */
#define LOOKUP_FOLLOW		 1
#define LOOKUP_DIRECTORY	 2
#define LOOKUP_CONTINUE	 4
#define LOOKUP_PARENT		16
#define LOOKUP_NOALT		32
#define LOOKUP_REVAL		64

/*
 * Intent data
 */
#define LOOKUP_OPEN		(0x0100)
#define LOOKUP_CREATE		(0x0200)
#define LOOKUP_ACCESS		(0x0400)

#ifndef _ASSEMBLER_

struct nameidata
{
    struct dentry *dentry;
    struct vfsmount *mnt;
    struct qstr last;
    unsigned int flags;
};

extern struct dentry * root;
extern int path_lookup(const char *name, unsigned int flags,
        struct nameidata *nd);
extern int __user_walk(const char *name, unsigned flags, struct nameidata *nd);
extern void path_release_on_umount(struct nameidata *nd);
extern struct dentry *lookup_create(struct nameidata *nd, int is_dir);
extern void expire_mount(struct vfsmount *mnt, struct list_head *mounts);
#endif /*_ASSEMBLER_*/
#endif	/*__NAMEI_H__*/
