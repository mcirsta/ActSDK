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
static int s_charge_device_fd = -1;



void charge_para_init( void )
{
}



void charge_device_open( void )
{
    s_charge_device_fd = open(DEVICE_CHARGER, O_RDWR, 0);
    if (s_charge_device_fd < 0)
    {
        print_err("\n\n\n");
    }
}



void charge_device_close( void )
{
    if (-1 != s_charge_device_fd)
    {
        close(s_charge_device_fd);
    }
}



/******************************************************************************/
/*!                    
* \par  Description:
*       获取电池电压, 
* \param[out]   charge_voltage_t *: 电池电压
* \retval       
*******************************************************************************/
bool charge_get_battery_voltage( charge_voltage_t *vol )
{
    bool result = true;
    int tmp;
    
    if (-1 == s_charge_device_fd)
    {
        print_warning("charge device isn't open\n");
        return false;
    }
    
    tmp = ioctl(s_charge_device_fd, CHARGE_IOGET_BATTERY, (int)vol);
    if ( tmp < 0 )
    {
        print_warning("\n\n");
        result = false;
    }
    
    return result;
}

/******************************************************************************/
/*!                    
* \par  Description:
*       获取电池显示电压, 已经经过驱动的平滑处理, 用于headbar显示
* \param[out]   charge_voltage_t *: 电池电压
* \retval       
*******************************************************************************/
bool charge_get_battery_voltage_show( charge_voltage_t *vol )
{
    bool result = true;
    int tmp;
    
    if (-1 == s_charge_device_fd)
    {
        print_warning("charge device isn't open\n");
        return false;
    }
    
    tmp = ioctl(s_charge_device_fd, CHARGE_IOGET_BATTERY_SHOW, (int)vol);
    if ( tmp < 0 )
    {
        print_warning("\n\n");
        result = false;
    }
    
    return result;
}

/******************************************************************************/
/*!                    
* \par  Description:
*       获取圆头adaptor是否插入, 通过检测DC5V实现, 同样适用于stub调试
* \param[out]   charge_voltage_t *: 电池电压
* \retval       
*******************************************************************************/
bool charge_get_adapter( bool *is_on )
{
    bool result = true;
    int tmp;
    
    if (-1 == s_charge_device_fd)
    {
        print_warning("charge device isn't open\n");
        return false;
    }
    
    tmp = ioctl(s_charge_device_fd, CHARGE_IOGET_DET_ADAPTER, (int)is_on);
    if ( tmp < 0 )
    {
        print_warning("\n\n");
        result = false;
    }
    
    return result;
}


/******************************************************************************/
/*!                    
* \par  Description:
*       设置充电参数
* \param[in]    charge_param_t *: 充电参数
* \retval       
*******************************************************************************/
bool charge_set_param( charge_param_t *param )
{
    bool result = true;
    int tmp;
    
    if (-1 == s_charge_device_fd)
    {
        print_warning("charge device isn't open\n");
        return false;
    }
    tmp = ioctl(s_charge_device_fd, CHARGE_IOSET_PARAM, (int)param);
    if ( tmp < 0 )
    {
        print_warning("\n\n");
        result = false;
    }
    
    return result;
}


/******************************************************************************/
/*!                    
* \par  Description:
*       设置充电状态
* \param[in]    int: 1:开始充电; 0:停止充电
* \retval       
*******************************************************************************/
bool charge_set_start( int start )
{
    bool result = true;
    int tmp = -1;
    
    if (-1 == s_charge_device_fd)
    {
        print_warning("charge device isn't open\n");
        return false;
    }
    if ( 1 == start )
    {
        tmp = ioctl(s_charge_device_fd, CHARGE_IOSET_START, (int)NULL);
    }
    else if ( 0 == start )
    {
        tmp = ioctl(s_charge_device_fd, CHARGE_IOSET_STOP, (int)NULL);
    }
    else
    {
    }
    
    if ( tmp < 0 )
    {
        print_warning("cmd:%d\n\n", start);
        result = false;
    }
    
    return result;
}


/******************************************************************************/
/*!                    
* \par  Description:
*       获得充电状态
* \param[out]   charge_status_t *: 充电状态
* \retval       
*******************************************************************************/
bool charge_get_status( charge_status_t *status )
{
    bool result = true;
    int tmp;
    
    if (-1 == s_charge_device_fd)
    {
        print_warning("charge device isn't open\n");
        return false;
    }
    
    tmp = ioctl(s_charge_device_fd, CHARGE_IOGET_STATE, (int)status);
    if ( tmp < 0 )
    {
        print_warning("\n\n");
        result = false;
    }
    
    return result;
}


/******************************************************************************/
/*!                    
* \par  Description:
*       设置充电过程, 充电驱动需要0.5秒循环调用一次, 不管充电与否
* \param[in]    
* \retval       
*******************************************************************************/
bool charge_set_process( void )
{
    bool result = true;
    int tmp;
    
    if (-1 == s_charge_device_fd)
    {
        print_warning("charge device isn't open\n");
        return false;
    }
    tmp = ioctl(s_charge_device_fd, CHARGE_IOSET_PROCESS, (int)NULL);
    if ( tmp < 0 )
    {
        print_warning("\n\n");
        result = false;
    }
    
    return result;
}

