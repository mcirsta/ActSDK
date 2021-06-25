/********************************************************************************
 *                              USDK(1100)
 *                             Module: tvin
 *                 Copyright(c) 2003-2009 Actions Semiconductor,
 *                            All Rights Reserved.
 * File : tvin_record_alarm.c
 * History:
 *      <author>     <time>           <version >             <desc>
 *      Jerry Fu    2009-04-14          v1.0               build this file
 ********************************************************************************/
/*!
* \file     tvin_record_alarm.c
* \brief    controls of tvin record alarm
* \author   Jerry Fu
*
* \version 1.0
* \date  2009/04/14
*******************************************************************************/
#include "tvin_config.h"

#define TVIN_MODE_NO    0x01    //TVIN应用所占用的mode号
#define ALARM_MODE_NO   0x02    //闹钟应用所占用的mode号


/******************************************************************************/
/*!
* \par  Description:
*	  创建定时器
* \param[in]    alarm_new_param_t
* \return       成功返回true, 失败返回false
*******************************************************************************/
bool  tvin_create_alarm_clock(alarm_new_param_t *alarm_new_param)
{
    int fd = 0;
    int ret = 0;
    
    if(NULL == alarm_new_param)
    {
        print_err("error parameter input\n");
        return false;
    }
    
    fd = open(DEVICE_ALARM, O_RDWR);
    if(fd < 0)
    {
        print_err("open alarm driver error\n");
        return false;
    }    
    ret = ioctl(fd, ALARM_NEW_CLOCK, (int)alarm_new_param);
    if(ret < 0)
    {
        print_err("create alarm clock error\n");
        close(fd);
        return false;
    }
    close(fd);
    print_dbg("create clock num=%d\n",alarm_new_param->out.clock_no);
    return true;
}


/******************************************************************************/
/*!
* \par  Description:
*	  获取已生效定时器索引
* \param[in]    alarm_get_clock_param_t
* \return       成功返回true, 失败返回false
*******************************************************************************/
bool tvin_get_alarm_clock(alarm_get_clock_param_t *alarm_get_clock_param)
{
    int fd = 0;
    int ret = 0;
    
    if(NULL == alarm_get_clock_param)
    {
        print_err("error parameter input\n");
        return false;
    }
    
    fd = open(DEVICE_ALARM, O_RDWR);
    if(fd < 0)
    {
        print_err("open alarm driver error\n");
        return false;
    }    
    ret = ioctl(fd, ALARM_GET_CLOCK, (int)alarm_get_clock_param);
    if(ret < 0)
    {
        print_err("get alarm clock error\n"); 
        close(fd);
        return false;
    }
    close(fd);
    print_dbg("get clock num=%d\n",alarm_get_clock_param->clock_no);
    return true;
}


/******************************************************************************/
/*!
* \par  Description:
*	  查询定时器内容
* \param[in]    alarm_query_param_t
* \return       成功返回true, 失败返回false
*******************************************************************************/
bool tvin_query_alarm_clock(alarm_query_param_t *alarm_query_param)
{
    int fd = 0;
    int ret = 0;
    
    if(NULL == alarm_query_param)
    {
        print_err("error parameter input\n");
        return false;
    }
    
    fd = open(DEVICE_ALARM, O_RDWR);
    if(fd < 0)
    {
        print_err("tvin open alarm driver error\n");
        return false;
    }    
    ret = ioctl(fd, ALARM_QUERY_CLOCK, (int)alarm_query_param);
    if(ret < 0)
    {
        print_err("tvin query alarm clock error\n");
        close(fd);
        return false;
    }
    close(fd);
    return true;
}


/******************************************************************************/
/*!
* \par  Description:
*	  修改定时器内容
* \param[in]    alarm_modify_param_t
* \return       成功返回true, 失败返回false
*******************************************************************************/
bool tvin_modify_alarm_clock(alarm_modify_param_t *alarm_modify_param)
{
    int fd = 0;
    int ret = 0;
    
    if(NULL == alarm_modify_param)
    {
        print_err("error parameter input\n");
        return false;
    }
    
    fd = open(DEVICE_ALARM, O_RDWR);
    if(fd < 0)
    {
        print_err("tvin open alarm driver error\n");
        return false;
    }    
    ret = ioctl(fd, ALARM_MODIDY_CLOCK, (int)alarm_modify_param);
    if(ret < 0)
    {
        print_err("tvin modify alarm clock error\n");
        close(fd);
        return false;
    }
    close(fd);
    print_dbg("modify clock num=%d\n",alarm_modify_param->clock_no);
    return true;
}


/******************************************************************************/
/*!
* \par  Description:
*	  删除定时器
* \param[in]    alarm_delete_param_t
* \return       成功返回true, 失败返回false
*******************************************************************************/
bool tvin_delete_alarm_clock(alarm_delete_param_t *alarm_delete_param)
{
    int fd = 0;
    int ret = 0;
    
    if(NULL == alarm_delete_param)
    {
        print_err("error parameter input\n");
        return false;
    }
    
    fd = open(DEVICE_ALARM, O_RDWR);
    if(fd < 0)
    {
        print_err("tvin open alarm driver error\n");
        return false;
    }    
    ret = ioctl(fd, ALARM_DELETE_CLOCK, (int)alarm_delete_param);
    if(ret < 0)
    {
        print_err("tvin delete alarm clock error\n");
        close(fd);
        return false;
    }
    close(fd);
    print_dbg("delete clock num=%d\n",alarm_delete_param->clock_no);
    return true;
}



