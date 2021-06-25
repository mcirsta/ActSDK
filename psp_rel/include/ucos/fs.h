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
 * \brief    �ļ�ϵͳ���ͷ�ļ�
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
 *      ���ļ�ͷ��ʼ�ƶ�
 */
#define SEEK_SET 0
/*!
 * \brief
 *      �ӵ�ǰ�ļ���дλ�ÿ�ʼ�ƶ�
 */
#define SEEK_CUR 1
/*!
 * \brief
 *      ���ļ�β��ʼ�����ƶ�
 */
#define SEEK_END 2

#ifndef _ASSEMBLER_

/*!
 * \brief
 *      �����ڵ����ݽṹ
 */
struct inode
{
    //	struct hlist_node	i_hash;
    /*! inode����*/
    struct list_head i_list;
    //	struct list_head	i_dentry;
    /*! �ڵ�����*/
    umode_t i_mode;
    /*! �ڵ��豸��*/
    dev_t i_rdev;
    /*! �豸��С*/
    loff_t i_size;
    /*! �ļ�������ָ��*/
    struct file_operations *i_fop;
    /*! ����������ģ��*/
    struct block_device *i_bdev;
    /*! ��������*/
    size_t i_count;
};

/*!
 * \brief
 *      �ļ����ݽṹ
 */
struct file
{
    /*! �ļ�����*/
    struct list_head f_list;
    struct list_head f_files; //add for fat
    /*! �ļ���Ӧ��dentryָ��*/
    struct dentry *f_dentry;
    /*! �ļ������ļ�ϵͳ��mount���ݽṹָ��*/
    struct vfsmount *f_vfsmnt;
    /*! �ļ�������ָ��*/
    struct file_operations *f_op;
    loff_t f_file_pos; //add for fat
    u32 f_status; //add for fat
    /*! �ļ���־*/
    unsigned int f_flags;

    void *f_inode; //add for fat
    /*! Ȩ������*/
    mode_t f_mode;
    /*! ˽������*/
    void * private_data;
    /*! �ļ���ƫ��*/
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
 *      ��ʱΪ��
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
 *     ���豸�ļ������ݽṹ
 */
struct block_device
{
    //	dev_t			bd_dev;  /* not a kdev_t - it's a search key */
    /*! ���豸�ļ������Ĵ��̵Ľṹָ��*/
    struct gendisk * bd_disk;
    /*! ˽������*/
    unsigned long bd_private;
};

/*!
 * \brief
 *      �ļ����������ݽṹ
 */
struct file_operations
{
    /*! ����������ģ��*/
    struct module *owner;
    /*! ��λ��������ָ��*/
    loff_t (*llseek)(struct file *, loff_t, int);
    /*! ����������ָ��*/
    ssize_t (*read)(struct file *, char *, size_t, loff_t *);
    /*! д��������ָ��*/
    ssize_t (*write)(struct file *, const char *, size_t, loff_t *);
    /*! ���Ʋ�������ָ��*/
    int (*ioctl)(struct inode *, struct file *, unsigned int, unsigned long);
    /*! �ڴ�ӳ���������ָ��*/
    int (*mmap)(struct file *, struct vm_area_struct *);
    /*! �򿪲�������ָ��*/
    int (*open)(struct inode *, struct file *);
    /*! �رղ�������ָ��*/
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
 *      �ļ�ϵͳ�������ݽṹ
 */
struct file_system_type
{
    /*! �ļ�ϵͳ��������*/
    const char *name;
    /*! �ļ�ϵͳ��־*/
    int fs_flags;
    struct super_block *(*get_sb)(struct file_system_type *, int, const char *,
            void *);
    void (*kill_sb)(struct super_block *);
    /*! �ļ�ϵͳ����*/
    struct file_system_type * next;
    /*! ���ļ�ϵͳ����ʵ��super block����*/
    struct list_head fs_supers;
    /*! �ļ�ϵͳ������ָ��*/
    struct file_system_operations *fs_ops;
};

/*!
 * \brief
 *      �ļ�ϵͳʵ�����ݽṹ
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
    /*! �ļ�ϵͳ����ָ��*/
    struct file_system_type *s_type;
    //		struct super_operations	*s_op;
    //		struct dquot_operations	*dq_op;
    //	 	struct quotactl_ops	*s_qcop;
    //		struct export_operations *s_export_op;
    //		unsigned long		s_flags;
    //		unsigned long		s_magic;
    /*! �ļ�ϵͳ��װ��Ŀ¼��Ӧ��dentryָ��*/
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
    /*! ���豸���ݽṹָ��*/
    struct block_device *s_bdev;
    /*!��ͬ�ļ�ϵͳ��ʵ������ͷ���*/
    struct list_head s_instances;
    //		struct quota_info	s_dquot;	/* Diskquota specific options */
    //
    //		int			s_frozen;
    //		wait_queue_head_t	s_wait_unfrozen;
    //
    //		char s_id[32];				/* Informational name */
    /*! �ļ�ϵͳ˽����Ϣ*/
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
