/********************************************************************************
 *                              USDK(1100)
 *                             Module: app 
 *                 Copyright(c) 2003-2009 Actions Semiconductor,
 *                            All Rights Reserved. 
 * File : usb_mtp_thread.h
 * History:         
 *      <author>    <time>           <version >             <desc>
 *    zhaowenjie    2009-2-25          v1.0               build this file 
 ********************************************************************************/
/*!
* \file     usb_mtp_thread.h
* \brief    mtp thread
* \author   zhaowenjie
*               
* \version 1.0
* \date  2009/06/01
*******************************************************************************/


#ifndef __USB_MTP_THREAD_H__
#define __USB_MTP_THREAD_H__

#include "include_case.h"


//	#define   MTP_DAEMON_RUNNING_STATUS_WAITING_COMMAND 100
//	#define   MTP_DAEMON_RUNNING_STATUS_DO_COMMMAND     101
//	#define   MTP_DAEMON_RUNNING_STATUS_SEND_OBJECT     102
//	#define   MTP_DAEMON_RUNNING_STATUS_GET_OBJECT      103


typedef struct
{
    bool (*const start_mtp_thread)(void);
    void (*const stop_mtp_thread)(void);

    bool (*const is_running)(void);
    int (*const get_mtp_status)();

    unsigned int (*const get_mtp_change_count)( void );
}mtp_thread_t;

mtp_thread_t* create_mtp_thread(void);

#endif /* __USB_MTP_THREAD_H__ */

