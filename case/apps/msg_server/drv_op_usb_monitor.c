/*
*******************************************************************************
*                       NOYA1100
*                (c) Copyright 2002-2008, Actions Co,Ld. 
*                        All Right Reserved 
* FileName: get_drv_msg.c
* Description:
* History: 
*       <author>    <time>          <version >      <desc>
*       shaofeng    2009/03/11      1.0             建立该文件。
*******************************************************************************
*/ 

#include "include_case.h"
#include "include_msg_server.h"



/* define variable, 变量定义 */
static int s_usb_monitor_device_fd = -1;



void usb_monitor_para_init( void )
{
}



void usb_monitor_device_open( void )
{
    s_usb_monitor_device_fd = open(DEVICE_UMONITOR, O_RDWR, 0);
    if (s_usb_monitor_device_fd < 0)
    {
        print_err("open usb monitor device err\n\n");
    }
}



void usb_monitor_device_close( void )
{
    if (-1 != s_usb_monitor_device_fd)
    {
        close(s_usb_monitor_device_fd);
    }
}




/* reversed */
#if 0
bool usb_set_connect_status( USB_CONNECTION_T usb_connect_status )
{
    bool result = true;
    int tmp;
    
    if (-1 == s_usb_monitor_device_fd)
    {
        print_warning("usb monitor device isn't open\n");
        return false;
    }

    tmp = ioctl(s_usb_monitor_device_fd, UMONITOR_IOCTL_SET_USB_CON, usb_connect_status);
    if ( tmp < 0 )
    {
        print_warning("ioctl:%d\n\n", tmp);
        result = false;
    }
    else
    {
        result = true;
    }

    return result;
}

bool usb_set_detect_status( USB_DETECTION_T detect_enable )
{
    print_dbg("detect_enable:%d", detect_enable);
    bool result = true;
    int tmp;

    if (-1 == s_usb_monitor_device_fd)
    {
        print_warning("usb monitor device isn't open\n");
        return false;
    }

    tmp = ioctl(s_usb_monitor_device_fd, UMONITOR_IOCTL_ENABLE_DETECT, detect_enable);
    if ( tmp < 0 )
    {
        print_warning("ioctl:%d\n\n", tmp);
        result = false;
    }
    else
    {
        result = true;
    }

    return result;
}


/* enable or disable detect usb weather plug in */
bool usb_get_detect_status( USB_DETECTION_T *detect_enable )
{
    bool result = true;
    int tmp;

    if (-1 == s_usb_monitor_device_fd)
    {
        print_warning("usb monitor device isn't open\n");
        return false;
    }

    tmp = ioctl(s_usb_monitor_device_fd, UMONITOR_IOCTL_IS_WORKING, (int)detect_enable);
    print_dbg("detect_enable:%d", detect_enable);
    if ( tmp < 0 )
    {
        print_warning("ioctl:%d\n\n", tmp);
        result = false;
    }
    else
    {
        result = true;
    }

    return result;
}
#endif

#if 1
/******************************************************************************/
/*!                    
* \par  Description:
*       获取usb接口接入的是pc 还是 adaptor
*       get PC or Adaptor is pluged in
* \param[out]   USB_CONNECTOR_T *: 
* \retval       
* 注意:
*       在获取usb接入后调用此函数, 用以判断接入的是pc机还是adaptor充电器
*******************************************************************************/
bool usb_get_connect_type( USB_CONNECTOR_T *usb_connect_type )
{
    bool result = true;
    int tmp;

    if ( (-1 == s_usb_monitor_device_fd) || (NULL == usb_connect_type) )
    {
        print_warning("usb monitor device isn't open\n");
        return false;
    }

    tmp = ioctl(s_usb_monitor_device_fd, UMONITOR_IOCTL_IS_CHARGER, (int)usb_connect_type);
    if ( tmp < 0 )
    {
        print_warning("\n\n");
        result = false;
    }
    else
    {
        result = true;
    }
    return result;
}
#endif

