/********************************************************************************
 *                              USDK130
 *                            Module: CACHE
 *                 Copyright(c) 2001-2008 Actions Semiconductor,
 *                            All Rights Reserved.
 *
 * History:
 *      <author>    <time>           <version >             <desc>
 *       hmwei     2008-6-12 9:42     1.0             build this file
 ********************************************************************************/
/*!
 * \file     cache_opr.h
 * \brief    cache²Ù×÷º¯ÊýÉùÃ÷
 * \author   hmwei
 * \version 1.0
 * \date  2008/6/12
 *******************************************************************************/
#ifndef __CACHE_OPR_H__
#define __CACHE_OPR_H__

#ifndef _ASSEMBLER_

extern void r4k_flush_cache_all(void);
extern void r4k_cache_init(void);
extern void r4k_flush_icache_range(unsigned long addr, unsigned long size);
extern void r4k_flush_icache_all(void);
extern void r4k_dma_cache_inv(unsigned long addr, unsigned long size);
extern void r4k_dma_cache_wback_inv(unsigned long addr, unsigned long size);
extern void r4k_lock_icache_range(unsigned long addr, unsigned long size);

#endif /*_ASSEMBLER_*/

#endif/*__CACHE_OPR_H__*/
