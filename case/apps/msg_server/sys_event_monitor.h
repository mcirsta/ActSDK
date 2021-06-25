/********************************************************************************
 *                              USDK(1100)
 *                             Module: app 
 *                 Copyright(c) 2003-2008 Actions Semiconductor,
 *                            All Rights Reserved. 
 * file : drv_op_sys_event_monitor.h
 * History:         
 *      <author>    <time>           <version >             <desc>
 *      Herb Hu    2009-6-18             v1.0             build this file 
 ********************************************************************************/

#ifndef __SYS_EVENT_MONITOR_H__
#define __SYS_EVENT_MONITOR_H__

#include <include_psp.h>
#include <include_case.h>

typedef enum
{
    SYS_EVT_ERR = 0 ,   
    SYS_EVT_NO_EVT ,    
    SYS_EVT_USB_A_IN ,  
    SYS_EVT_USB_HOST_DISK_IN ,  
    SYS_EVT_USB_HOST_DISK_READY ,   
    SYS_EVT_USB_A_OUT , 
    SYS_EVT_USB_HOST_DISK_OUT , 
    SYS_EVT_USB_B_IN ,  
    SYS_EVT_USB_B_OUT , 
    SYS_EVT_CARD_IN ,   
    SYS_EVT_CARD_READY,
    SYS_EVT_CARD_ERR,
    SYS_EVT_CARD_OUT ,  
    SYS_EVT_ALARM_IN,
    SYS_EVT_ROUND_POWER_ADAPTER_IN,
    SYS_EVT_POWER_ADAPTER_OUT,
}sys_evt_e;

extern bool sys_evt_monitor_open_dev(void);
extern bool sys_evt_monitor_close_dev(void);
extern sys_evt_e sys_evt_monitor_get_evt(void);

#endif /* __SYS_EVENT_MONITOR_H__*/

