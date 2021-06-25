#ifndef __FM_SYS_H__
#define __FM_SYS_H__

#include "fm_config.h"

/*include head file for linux kernel 2.6.14*/
#ifdef  _ACT_LINUX_KERNEL_2_6_
#ifndef __KERNEL__
#define __KERNEL__
#endif
#ifndef MODULE
#define MODULE
#endif
#include <linux/config.h>
#include <linux/module.h>
#include <linux/kernel.h>   /* printk() */
#include <linux/slab.h>   /* kmalloc() */
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
#include <linux/dma-mapping.h>

#include <asm/mach-atj213x/atj213x.h>
#include <asm/mach-atj213x/atj213x_dma.h>
#include <asm/mach-atj213x/atj213x_multifunc.h>

#include <asm/mipsregs.h>
#include <asm/asm.h>

#define	OS_INT_SAVE_AREA	unsigned long ss_flags;
#define	AP_OS_ENTER_CRITICAL()	local_irq_save(ss_flags)
#define	AP_OS_EXIT_CRITICAL()	local_irq_restore(ss_flags)

//#include "Actions_reg_213x.h"


#endif //end of '#ifdef  _ACT_LINUX_KERNEL_2_6_'

///////////////////////////////////////////////////////
/********include head file for UCOS************************/
#ifdef _ACT_UC_OS_

#include <kmod_calls.h>
#define DEBUG_PRINT(param...)

#endif //endof '_ACT_UC_OS_'

#endif //endof file

