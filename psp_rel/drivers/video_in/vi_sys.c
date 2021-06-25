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
 * \file   vi_sys.c
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

#include "vi_config.h"
#include "vi_sys.h"

#ifdef _ACT_UC_OS_

#ifdef  _USE_OLD_SENSOR_API_

#include "video_in.h"

/** elf relevent*/
#include "elf.h"
#include "drv_manager.h"
#include "includes.h"


static void * null_func(void);

unsigned int sensor_apitbl[] =  
{
    (unsigned int)sensor_open,
    (unsigned int)sensor_close,
    (unsigned int)sensor_ioctl,
};

#ifndef _UCOS_NEW_DMA_MEM_MALLOC_
extern INT32U _dstart,_dend;

addrInfo addr_info __addrdata = 
{
    startAddr: (INT32U)&_dstart,
    endAddr: (INT32U)&_dend
};  /*start/end addr of the drv*/
#endif  /* _UCOS_NEW_DMA_MEM_MALLOC_ */

static drv_Info drvinfo =  
{
    "drv",
    (INT32S (*)(void *))sensor_init,
    (INT32S (*)(void ))sensor_exit,
    (INT32U)sensor_apitbl
};  /* drvinfo required for installing drv */                       

drv_Info *GetDrvInfo(void)
{
    //printf("\n card GetDrvInfo is called !\n");
    return &drvinfo;
}

/**
 * no use, just to disable the link error 
 */
static void * null_func(void)
{
    return;
}

#else

#endif  /* posix version driver or not. */

/* reserved, maybe should be realized someday .... */
int wait_event_interruptible(int wait, int flag)
{
    return 0;
}


#endif /* _ACT_UC_OS_ */

#ifdef _ACT_LINUX_KERNEL_2_6_

unsigned int getICType(void)
{
    return 0x63;
}

#endif  /* _ACT_LINUX_KERNEL_2_6_ */

