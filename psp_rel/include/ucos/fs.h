/********************************************************************************
 *                              USDK(1100)
 *                            Module: FS
 *                 Copyright(c) 2003-2008 Actions Semiconductor,
 *                            All Rights Reserved.
 *
 * History:
 *      <author>    <time>           <version >             <desc>
 *       hmwei     2008-8-30 16:45     1.0             build this file
 ********************************************************************************/
/*!
 * \file     fs.h
 * \brief    文件系统相关头文件
 * \author   hmwei
 * \version 1.0
 * \date  2008/8/30
 *******************************************************************************/
#ifndef __FS_H__
#define __FS_H__

#include <ucos/types.h>
#include <ucos/module.h>
#include <ucos/mount.h>
#include <ucos/list.h>
#include <ucos/genhd.h>
#include <ucos/kdev_t.h>

#define READ 0
#define WRITE 1

#define FMODE_READ 1
#define FMODE_WRITE 2

/*!
 * \brief
 *      从文件头开始移动
 */
#define SEEK_SET 0
/*!
 * \brief
 *      从当前文件读写位置开始移动
 */
#define SEEK_CUR 1
/*!
 * \brief
 *      从文件尾开始反向移动
 */
#define SEEK_END 2

#ifndef _ASSEMBLER_

/*!
 * \brief
 *      索引节点数据结构
 */
struct inode
{
    //	struct hlist_node	i_hash;
    /*! inode链表*/
    struct list_head i_list;
    //	struct list_head	i_dentry;
    /*! 节点类型*/
    umode_t i_mode;
    /*! 节点设备号*/
    dev_t i_rdev;
    /*! 设备大小*/
    loff_t i_size;
    /*! 文件操作集指针*/
    struct file_operations *i_fop;
    /*! 操作集所属模块*/
    struct block_device *i_bdev;
    /*! 操作次数*/
    size_t i_count;
};

/*!
 * \brief
 *      文件数据结构
 */
struct file
{
    /*! 文件链表*/
    struct list_head f_list;
    struct list_head f_files; //add for fat
    /*! 文件对应的dentry指针*/
    struct dentry *f_dentry;
    /*! 文件所在文件系统的mount数据结构指针*/
    struct vfsmount *f_vfsmnt;
    /*! 文件操作集指针*/
    struct file_operations *f_op;
    loff_t f_file_pos; //add for fat
    u32 f_status; //add for fat
    /*! 文件标志*/
    unsigned int f_flags;

    void *f_inode; //add for fat
    /*! 权限设置*/
    mode_t f_mode;
    /*! 私有数据*/
    void * private_data;
    /*! 文件内偏移*/
    loff_t f_pos;
};

/*
 * Manipulate page protection bits
 */
//	typedef struct { unsigned long pgprot; } pgprot_t;
//	#define pgprot_val(x)	((x).pgprot)
//	#define __pgprot(x)	((pgprot_t) { (x) } )
/*!
 * \brief
 *      暂时为空
 */
struct vm_area_struct
{
    unsigned long vm_start; /* Our start address within vm_mm. */
    unsigned long vm_end; /* The first byte after our end address within vm_mm. */

    unsigned long vm_page_prot; /* Access permissions of this VMA. */
    unsigned long vm_flags; /* Flags, listed below. */

    /* Information about our backing store: */
    unsigned long vm_pgoff; /* Offset (within vm_file) in PAGE_SIZE
     units, *not* PAGE_CACHE_SIZE */
    struct file * vm_file; /* File we map to (can be NULL). */
    unsigned long vm_truncate_count;/* truncate_count or restart_addr */
};

/*!
 * \brief
 *     块设备文件打开数据结构
 */
struct block_device
{
    //	dev_t			bd_dev;  /* not a kdev_t - it's a search key */
    /*! 块设备文件所属的磁盘的结构指针*/
    struct gendisk * bd_disk;
    /*! 私有数据*/
    unsigned long bd_private;
};

/*!
 * \brief
 *      文件操作集数据结构
 */
struct file_operations
{
    /*! 操作集所属模块*/
    struct module *owner;
    /*! 定位操作函数指针*/
    loff_t (*llseek)(struct file *, loff_t, int);
    /*! 读操作函数指针*/
    ssize_t (*read)(struct file *, char *, size_t, loff_t *);
    /*! 写操作函数指针*/
    ssize_t (*write)(struct file *, const char *, size_t, loff_t *);
    /*! 控制操作函数指针*/
    int (*ioctl)(struct inode *, struct file *, unsigned int, unsigned long);
    /*! 内存映射操作函数指针*/
    int (*mmap)(struct file *, struct vm_area_struct *);
    /*! 打开操作函数指针*/
    int (*open)(struct inode *, struct file *);
    /*! 关闭操作函数指针*/
    int (*release)(struct inode *, struct file *);
};

struct block_device_operations
{
    int (*open)(struct inode *, struct file *);
    int (*release)(struct inode *, struct file *);
    int (*ioctl)(struct inode *, struct file *, unsigned, unsigned long);
    struct module *owner;
};

/*!
 * \brief
 *      文件系统类型数据结构
 */
struct file_system_type
{
    /*! 文件系统类型名称*/
    const char *name;
    /*! 文件系统标志*/
    int fs_flags;
    struct super_block *(*get_sb)(struct file_system_type *, int, const char *,
            void *);
    void (*kill_sb)(struct super_block *);
    /*! 文件系统链表*/
    struct file_system_type * next;
    /*! 该文件系统类型实体super block链表*/
    struct list_head fs_supers;
    /*! 文件系统操作集指针*/
    struct file_system_operations *fs_ops;
};

/*!
 * \brief
 *      文件系统实体数据结构
 */
struct super_block
{
    //		struct list_head	s_list;		/* Keep this first */
    //		dev_t			s_dev;		/* search index; _not_ kdev_t */
    //		unsigned long		s_blocksize;
    //		unsigned long		s_old_blocksize;
    //		unsigned char		s_blocksize_bits;
    //		unsigned char		s_dirt;
    //		unsigned long long	s_maxbytes;	/* Max file size */
    /*! 文件系统类型指针*/
    struct file_system_type *s_type;
    //		struct super_operations	*s_op;
    //		struct dquot_operations	*dq_op;
    //	 	struct quotactl_ops	*s_qcop;
    //		struct export_operations *s_export_op;
    //		unsigned long		s_flags;
    //		unsigned long		s_magic;
    /*! 文件系统安装的目录相应的dentry指针*/
    struct dentry *s_root;
    //		struct rw_semaphore	s_umount;
    //		struct semaphore	s_lock;
    //		int			s_count;
    //		int			s_syncing;
    //		int			s_need_sync_fs;
    //		atomic_t		s_active;
    //		void                    *s_security;
    //		struct xattr_handler	**s_xattr;
    //
    //		struct list_head	s_inodes;	/* all inodes */
    //		struct list_head	s_dirty;	/* dirty inodes */
    //		struct list_head	s_io;		/* parked for writeback */
    //		struct hlist_head	s_anon;		/* anonymous dentries for (nfs) exporting */
    //		struct list_head	s_files;
    /*! 块设备数据结构指针*/
    struct block_device *s_bdev;
    /*!相同文件系统的实体链表头结点*/
    struct list_head s_instances;
    //		struct quota_info	s_dquot;	/* Diskquota specific options */
    //
    //		int			s_frozen;
    //		wait_queue_head_t	s_wait_unfrozen;
    //
    //		char s_id[32];				/* Informational name */
    /*! 文件系统私有信息*/
    void *s_fs_info; /* Filesystem private info */
//
//		/*
//		 * The next field is for VFS *only*. No filesystems have any business
//		 * even looking at it. You had been warned.
//		 */
//		struct semaphore s_vfs_rename_sem;	/* Kludge */
//
//		/* Granuality of c/m/atime in ns.
//		   Cannot be worse than a second */
//		u32		   s_time_gran;
};
/* Alas, no aliases. Too much hassle with bringing module.h everywhere */
#define fops_get(fops) (fops ? (fops) : NULL )
#define get_file(x)	do{}while(0)

static inline unsigned iminor(struct inode *inode)
{
    return MINOR(inode->i_rdev);
}

static inline unsigned imajor(struct inode *inode)
{
    return MAJOR(inode->i_rdev);
}

static inline loff_t i_size_read(struct inode *inode)
{
    return inode->i_size;
}

extern struct dentry * root;

extern int
        register_chrdev(unsigned int, const char *, struct file_operations *);
extern int unregister_chrdev(unsigned int, const char *);

extern int register_blkdev(unsigned int, const char *);
extern int unregister_blkdev(unsigned int, const char *);

extern int register_filesystem(struct file_system_type *);
extern int unregister_filesystem(struct file_system_type *);

extern ssize_t blkdev_read(struct file *filp, char *buf, size_t count,
        loff_t *ppos);
extern ssize_t blkdev_write(struct file *filp, const char *buf, size_t count,
        loff_t *ppos);
extern int blkdev_ioctl(struct inode *inode, struct file *file,
        unsigned int cmd, unsigned long arg);

extern struct file_system_type *get_fs_type(const char *name);

extern void destroy_inode(struct inode *);
extern struct inode *new_inode(void *);

extern int ramfs_mkdir(struct inode * dir, struct dentry * dentry, int mode);
extern int ramfs_mknod(struct inode *dir, struct dentry *dentry, int mode,
        dev_t dev);

extern int buildroot(void);

extern struct file *fget(unsigned int fd);
extern int get_unused_fd(void);
extern void put_unused_fd(unsigned int fd);
extern void fd_install(unsigned int fd, struct file * file);
extern void put_filp(struct file *file);
extern void fput(struct file *file);
extern struct file *get_empty_filp(void);

extern struct file *filp_open(const char *, int, int);
extern int filp_close(struct file *, void *id);

extern int set_kpwd(void *pwd);
extern void *get_kpwd(void);

#endif /*_ASSEMBLER_*/
#endif /* __FS_H__ */
