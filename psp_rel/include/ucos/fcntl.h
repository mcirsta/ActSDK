/********************************************************************************
 *                              USDK(1100)
 *                            Module: FCNTL
 *                 Copyright(c) 2003-2008 Actions Semiconductor,
 *                            All Rights Reserved.
 *
 * History:
 *      <author>    <time>           <version >             <desc>
 *       hmwei     2008-8-30 16:45     1.0             build this file
 ********************************************************************************/
/*!
 * \file     fcntl.h
 * \brief    文件控制操作头文件
 * \author   hmwei
 * \version 1.0
 * \date  2008/8/30
 *******************************************************************************/
#ifndef __OS_FCNTL_H__
#define __OS_FCNTL_H__

#include <asm-mips/config.h>
#include <ucos/types.h>

#define LINUX_FCNTL

#define O_APPEND    0x0008
#define O_SYNC      0x0010
#define O_NONBLOCK  0x0080
#define O_CREAT     0x0100  /* not fcntl */
#define O_EXCL      0x0400  /* not fcntl */
#define O_NOCTTY    0x0800  /* not fcntl */
#define FASYNC      0x1000  /* fcntl, for BSD compatibility */
#define O_LARGEFILE 0x2000  /* allow large file opens */
#define O_DIRECT    0x8000  /* direct disk access hint */

#define F_GETFL     3   /* get file->f_flags */
#define F_SETFL     4   /* set file->f_flags */
#define F_GETLK     14
#define F_SETLK     6
#define F_SETLKW    7

/* FAT volume */
#define F_SETVLM        32  /* set volume */
#define F_GETVLM        33  /* get volume */
#define F_RMVLM         34  /* remove volume */
#define F_FASTOPEN      35  /* building hash table for open files */
#define F_FASTCLOSE     36  /* free all hash table */
#define F_BUILDINDEX    37  /* building clust index for file */
#define F_REARRANGE     38  /* move file out of a given area */
#define F_CLUSTALIGN    39  /* start clust 16k align for MMM */
#define F_AVOIDMERGE    40  /* flash drv clean up log fragment in advance, avoid simple merge */

/* CODECHECK */
#define F_CODECHECK_FILE    91  /* check one file*/
#define F_CODECHECK_CUR     92  /* get current data during checking all files*/

#define F_SETOWN    24  /*  for sockets. */
#define F_GETOWN    23  /*  for sockets. */
#define F_SETSIG    10  /*  for sockets. */
#define F_GETSIG    11  /*  for sockets. */

//  #include <asm-generic/fcntl.h>
#define O_ACCMODE   00000003
#define O_RDONLY    00000000
#define O_WRONLY    00000001
#define O_RDWR      00000002

#ifndef O_CREAT
#define O_CREAT     00000100    /* not fcntl */
#endif
#ifndef O_EXCL
#define O_EXCL      00000200    /* not fcntl */
#endif
#ifndef O_NOCTTY
#define O_NOCTTY    00000400    /* not fcntl */
#endif
#ifndef O_TRUNC
#define O_TRUNC     00001000    /* not fcntl */
#endif
#ifndef O_APPEND
#define O_APPEND    00002000
#endif
#ifndef O_NONBLOCK
#define O_NONBLOCK  00004000
#endif
#ifndef O_SYNC
#define O_SYNC      00010000
#endif
#ifndef FASYNC
#define FASYNC      00020000    /* fcntl, for BSD compatibility */
#endif
#ifndef O_DIRECT
#define O_DIRECT    00040000    /* direct disk access hint */
#endif
#ifndef O_LARGEFILE
#define O_LARGEFILE 00100000
#endif
#ifndef O_DIRECTORY
#define O_DIRECTORY 00200000    /* must be a directory */
#endif
#ifndef O_NOFOLLOW
#define O_NOFOLLOW  00400000    /* don't follow links */
#endif
#ifndef O_NOATIME
#define O_NOATIME   01000000
#endif
#ifndef O_NDELAY
#define O_NDELAY    O_NONBLOCK
#endif

#endif /* __OS_FCNTL_H__ */
