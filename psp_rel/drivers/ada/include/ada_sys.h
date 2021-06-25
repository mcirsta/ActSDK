/*
********************************************************************************
*                   usdk130 --- adc/dac driver
*                (c) Copyright 2003-2008, Actions Co,Ld. 
*                        All Right Reserved 
*
* History:        
*      <author>      <time>       <version >    <desc>
*       houjingkun      2008/07/30     1.0         build this file 
*       new char device driver for adc/dac, 1017 branch version.
********************************************************************************/ 
/*!
 * \file   ada_sys.h
 * \brief  
 *      adc/dac driver's system separating layer headfile.
 * \author houjingkun
 * \par GENERAL DESCRIPTION:
 *       system's separating layer. it is added to separate os's API. 
 *       in order to make the driver code more transplantable to linux or other os.
 * \par EXTERNALIZED FUNCTIONS:
 *       null
 *
 *  Copyright(c) 2003-2008 Actions Semiconductor, All Rights Reserved.
 *
 * \version 1.0
 * \date  2008/07/30
 *******************************************************************************/

#ifndef __ADA_SYS_H__
#define __ADA_SYS_H__

/************* select a os platform. **************/
#define _UC_OS_
//#define _LINUX_OS_


/*******************************************************************************/
#ifdef _UC_OS_

#include "kmod_calls.h"
#include "syscalls.h"


#endif /* _UC_OS_ */

/*******************************************************************************/
#ifdef _LINUX_OS_

#ifndef __KERNEL__
#define __KERNEL__
#endif
#ifndef MODULE
#define MODULE
#endif

#include <linux/config.h>
#include <linux/module.h>

#include <linux/kernel.h>   /* printk() */
#include <linux/slab.h>   /* malloc_dma() */
#include <linux/fs.h>       /* everything... */
#include <linux/errno.h>    /* error codes */
#include <linux/types.h>    /* size_t */
#include <linux/proc_fs.h>
#include <linux/fcntl.h>    /* O_ACCMODE */

#include <asm/system.h>     /* cli(), *_flags */
#include <asm/uaccess.h>     

#include <linux/init.h>
#include <linux/miscdevice.h>
#include <linux/sched.h>
#include <linux/delay.h>
#include <linux/poll.h>
#include <linux/spinlock.h>
#include <linux/irq.h>
#include <asm/addrspace.h>
#include <linux/dma-mapping.h>

#include <asm/mach-atj213x/atj213x.h>
#include <asm/mach-atj213x/atj213x_dma.h>
#include <asm/mach-atj213x/atj213x_multifunc.h>

#define ADA_MAJOR   250

#define os_mdelay(c)

#endif /* _LINUX_OS_ */

#endif  /* __ADA_SYS_H__ */

