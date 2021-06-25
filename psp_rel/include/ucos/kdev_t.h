/********************************************************************************
 *                              USDK(1100)
 *                            Module: KDEV_T
 *                 Copyright(c) 2003-2008 Actions Semiconductor,
 *                            All Rights Reserved.
 *
 * History:
 *      <author>    <time>           <version >             <desc>
 *       hmwei     2008-8-30 16:45     1.0             build this file
 ********************************************************************************/
/*!
 * \file     kdev_t.h
 * \brief    设备号操作相关头文件
 * \author   hmwei
 * \version 1.0
 * \date  2008/8/30
 *******************************************************************************/
#ifndef __KDEV_T_H__
#define __KDEV_T_H__

#include <ucos/types.h>

#ifndef _ASSEMBLER_

#define makedev(major, minor) (((major) << 8) | (minor)) //added by hmwei
#define MINORBITS	20
#define MINORMASK	((1U << MINORBITS) - 1)

#define MAJOR(dev)	((unsigned int) ((dev) >> MINORBITS))
#define MINOR(dev)	((unsigned int) ((dev) & MINORMASK))
#define MKDEV(ma,mi)	(((ma) << MINORBITS) | (mi))

#define print_dev_t(buffer, dev)					\
	sprintf((buffer), "%u:%u\n", MAJOR(dev), MINOR(dev))

#define format_dev_t(buffer, dev)					\
	({								\
		sprintf(buffer, "%u:%u", MAJOR(dev), MINOR(dev));	\
		buffer;							\
	})

/* acceptable for old filesystems */
static inline int old_valid_dev(dev_t dev)
{
    return MAJOR(dev) < 256 && MINOR(dev) < 256;
}

static inline u16 old_encode_dev(dev_t dev)
{
    return (MAJOR(dev) << 8) | MINOR(dev);
}

static inline dev_t old_decode_dev(u16 val)
{
    return MKDEV((val >> 8) & 255, val & 255);
}

static inline int new_valid_dev(dev_t dev)
{
    return 1;
}

static inline u32 new_encode_dev(dev_t dev)
{
    unsigned major = MAJOR(dev);
    unsigned minor = MINOR(dev);
    return (minor & 0xff) | (major << 8) | ((minor & ~0xff) << 12);
}

/*! \cond KERNEL_INNER*/
/******************************************************************************/
/*!
 * \par  Description:
 *	  旧设备号转换为新设备号
 * \param[in]    dev  旧设备号
 * \return       新设备号
 * \ingroup      module name
 * \par          exmaple code
 * \code
 * case S_IFCHR: case S_IFBLK:
 * 	error = ramfs_mknod(nd.dentry->d_inode,dentry,mode, new_decode_dev(dev));
 * \endcode
 * \note
 * 旧设备号：bit8~bit19:major;bit0~bit7&bit20~bit31:minor
 * 新设备号：bit20~bit31:major;bit0~bit19:minor
 *******************************************************************************/
static inline dev_t new_decode_dev(u32 dev)
{
    unsigned major = (dev & 0xfff00) >> 8;
    unsigned minor = (dev & 0xff) | ((dev >> 12) & 0xfff00);
    return MKDEV(major, minor);
}
/*! \endcond */

static inline int sysv_valid_dev(dev_t dev)
{
    return MAJOR(dev) < (1 << 14) && MINOR(dev) < (1 << 18);
}

static inline u32 sysv_encode_dev(dev_t dev)
{
    return MINOR(dev) | (MAJOR(dev) << 18);
}

static inline unsigned sysv_major(u32 dev)
{
    return (dev >> 18) & 0x3fff;
}

static inline unsigned sysv_minor(u32 dev)
{
    return dev & 0x3ffff;
}
#endif /*_ASSEMBLER_*/
#endif	/*__KDEV_T_H__*/
