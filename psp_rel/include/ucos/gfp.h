/********************************************************************************
 *                               USDK(1100)
 *                            Module: SLAB
 *                 Copyright(c) 2003-2008 Actions Semiconductor,
 *                            All Rights Reserved.
 *
 * History:
 *      <author>    <time>           <version >             <desc>
 *       hmwei     2008-02-01 15:00     1.0             build this file
 ********************************************************************************/
/*!
 * \file     gfp.h
 * \brief    gfp类型及描述符定义
 * \author   open source
 * \version 1.0
 * \date  2008/2/1
 *******************************************************************************/
#ifndef __GFP_H__
#define __GFP_H__
/*
 * GFP bitmasks..
 */
/* Zone modifiers in GFP_ZONEMASK (see linux/mmzone.h - low two bits) */
#define __GFP_DMA	0x01u
#define __GFP_HIGHMEM	0x02u

/*
 * Action modifiers - doesn't change the zoning
 *
 * __GFP_REPEAT: Try hard to allocate the memory, but the allocation attempt
 * _might_ fail.  This depends upon the particular VM implementation.
 *
 * __GFP_NOFAIL: The VM implementation _must_ retry infinitely: the caller
 * cannot handle allocation failures.
 *
 * __GFP_NORETRY: The VM implementation must not retry indefinitely.
 */
#define __GFP_WAIT	0x10u	/* Can wait and reschedule? */
#define __GFP_HIGH	0x20u	/* Should access emergency pools? */
#define __GFP_IO	0x40u	/* Can start physical IO? */
#define __GFP_FS	0x80u	/* Can call down to low-level FS? */
#define __GFP_COLD	0x100u	/* Cache-cold page required */
#define __GFP_NOWARN	0x200u	/* Suppress page allocation failure warning */
#define __GFP_REPEAT	0x400u	/* Retry the allocation.  Might fail */
#define __GFP_NOFAIL	0x800u	/* Retry for ever.  Cannot fail */
#define __GFP_NORETRY	0x1000u	/* Do not retry.  Might fail */
#define __GFP_NO_GROW	0x2000u	/* Slab internal usage */
#define __GFP_COMP	0x4000u	/* Add compound page metadata */
#define __GFP_ZERO	0x8000u	/* Return zeroed page on success */
#define __GFP_NOMEMALLOC 0x10000u /* Don't use emergency reserves */
#define __GFP_NORECLAIM  0x20000u /* No realy zone reclaim during allocation */
#define __GFP_HARDWALL   0x40000u /* Enforce hardwall cpuset memory allocs */

#define __GFP_BITS_SHIFT 20	/* Room for 20 __GFP_FOO bits */
#define __GFP_BITS_MASK ((1 << __GFP_BITS_SHIFT) - 1)

/* if you forget to add the bitmask here kernel will crash, period */
#define GFP_LEVEL_MASK (__GFP_WAIT|__GFP_HIGH|__GFP_IO|__GFP_FS| \
			__GFP_COLD|__GFP_NOWARN|__GFP_REPEAT| \
			__GFP_NOFAIL|__GFP_NORETRY|__GFP_NO_GROW|__GFP_COMP| \
			__GFP_NOMEMALLOC|__GFP_NORECLAIM|__GFP_HARDWALL)

#define GFP_ATOMIC	(__GFP_HIGH)
#define GFP_NOIO	(__GFP_WAIT)
#define GFP_NOFS	(__GFP_WAIT | __GFP_IO)
#define GFP_KERNEL	(__GFP_WAIT | __GFP_IO | __GFP_FS)
#define GFP_USER	(__GFP_WAIT | __GFP_IO | __GFP_FS | __GFP_HARDWALL)
#define GFP_HIGHUSER	(__GFP_WAIT | __GFP_IO | __GFP_FS | __GFP_HARDWALL | \
			 __GFP_HIGHMEM)

/* Flag - indicates that the buffer will be suitable for DMA.  Ignored on some
 platforms, used as appropriate on others */

#define GFP_DMA		__GFP_DMA

#endif/*__GFP_H__*/
