/*
********************************************************************************
*                   213x --- video in device driver
*                (c) Copyright 2007 - 2010, Actions Co,Ld. 
*                        All Right Reserved 
*
* History:        
*      <author>      <time>       <version >    <desc>
*       houjingkun      2008/06/18     1.0         build this file 
*       new added vi_sys.h and vi_sys.c for system seperating layer.
********************************************************************************/ 
/*!
 * \file   vi_sys.h
 * \brief  
 *      video in device driver's system seperating layer API.
 * \author houjingkun
 * \par GENERAL DESCRIPTION:
 *       video_in driver's system seperating layer.
 *       it is added to separate os's function, 
 *       in order to make the driver code more transplantable.
 * \par EXTERNALIZED FUNCTIONS:
 *       null
 *
 *     Copyright(c) 2008-2012 Actions Semiconductor, All Rights Reserved.
 *
 * \version 1.0
 * \date  2008/06/18
 *******************************************************************************/

#ifndef _VI_SYS_H_
#define _VI_SYS_H_

#include "video_in.h"
#include "vi_config.h"

/**************************************************************************/
/********************** head file in ucos system. *************************/
/**************************************************************************/
#ifdef _ACT_UC_OS_

#include "kmod_calls.h"
#include "syscalls.h"

#ifdef	_USE_OLD_SENSOR_API_

#define __put_user_check(x,ptr,size)    ({ \
switch(size){ \
case 1: *(unsigned char *)ptr = (unsigned char)x; break; \
case 2: *(unsigned short *)ptr = (unsigned short)x; break; \
case 4: *(unsigned int *)ptr = (unsigned int)x; break; \
}})
/* if need return value: }} 0; ) */

#define put_user(x, ptr)    __put_user_check((x),(ptr),sizeof(*(ptr)))

#else

#define MODULE_LICENSE(x)


#endif	/* posix version driver or not. */

//#define IRQ_HANDLED
//#define IRQ_NONE

#ifdef  printf
#undef  printf
#endif
#define SS_PRINTF(x...)   printf(x)


extern int wait_event_interruptible(int wait, int flag);

//extern unsigned int vi_app_type;

#endif  /******************* end of head file in ucos system. **********************/

/**************************************************************************/
/****************** head file in linux 2.6.14 system. *********************/
/**************************************************************************/


#endif  /* _VI_SYS_H_. */

