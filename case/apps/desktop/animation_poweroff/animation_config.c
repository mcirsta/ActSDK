/*******************************************************************************
 *                              NOYA1100
 *                            Module: video
 *                 Copyright(c) 2003-2008 Actions Semiconductor,
 *                            All Rights Reserved. 
 * History:         
 *      <author>    <time>           <version >             <desc>
 *      shaofeng    2009-07-28          1.0             build this file 
*******************************************************************************/
/*!
 * \file     animation_config.c
 * \brief    
 * \author   shaofeng
 * \par      GENERAL DESCRIPTION:
 *               
 * \par      EXTERNALIZED FUNCTIONS:
 *               
 * \version 1.0
 * \date  2009/07/28
*******************************************************************************/

#include "include_psp.h"
#include "include_case.h"

/* include file of this application, 本应用的头文件 */
#include "animation_poweroff.h"






#if 0
#endif
/******************************************************************************/
/*!                    
* \par  Description:
*       get start_mode param
*       从config中获取启动方式配置信息
* \param[in]    
* \retval       
*******************************************************************************/
bool animation_cfg_read_start_mode( void )
{
    int tmp = 0;
    int start_mode = 0;
    
    //读取进程启动方式
    tmp = get_config( CFG_ANIMATION_POWEROFF_START_MODE, (char *)&start_mode, sizeof(start_mode) );
    //获取失败, 使用res方式启动
    if ( -1 == tmp )
    {
        animation_vars.start_mode = ANIMATION_START_MODE_RES;
    }
    else
    {
        if ( 0 == start_mode )
        {
            animation_vars.start_mode = ANIMATION_START_MODE_RES;
        }
        else
        {
            animation_vars.start_mode = ANIMATION_START_MODE_DECODER;
        }
    }
    
    return true;
}


/******************************************************************************/
/*!                    
* \par  Description:
*       get filename param
*       从config中获取动画文件名配置信息
* \param[in]    rotation_e: 当前场景角度
* \param[out]   char *: 文件名
* \retval       
*******************************************************************************/
bool animation_cfg_read_filename( rotation_e angle, char *filename )
{
    int tmp = 0;
    
    if ( NULL == filename )
    {
        return false;
    }
    
    if ( (angle == ROTATION_0) || (angle == ROTATION_180) )
    {
        //读取当前解码文件全路径
        tmp = get_config( CFG_ANIMATION_POWEROFF_PIC_SIDELONG_NAME, (char *)filename, PATH_MAX );
        if ( -1 == tmp )
        {
            print_warning("get_config failed");
            return false;
        }
    }
    else// if ( (angle == ROTATION_90) || (angle == ROTATION_270) )
    {
        //读取当前解码文件全路径
        tmp = get_config( CFG_ANIMATION_POWEROFF_PIC_UPRIGHT_NAME, (char *)filename, PATH_MAX );
        if ( -1 == tmp )
        {
            print_warning("get_config failed");
            return false;
        }
    }
    
    return true;
}


/******************************************************************************/
/*!                    
* \par  Description:
*       get filename param
*       从config中获取 默认动画文件名配置信息
* \param[in]    rotation_e: 当前场景角度
* \param[out]   char *: 文件名
* \retval       
*******************************************************************************/
bool animation_cfg_read_def_filename( rotation_e angle, char *filename )
{
    int tmp = 0;
    
    if ( NULL == filename )
    {
        return false;
    }
    
    if ( (angle == ROTATION_0) || (angle == ROTATION_180) )
    {
        //读取默认解码文件全路径
        tmp = get_config( CFG_DEF_ANIMATION_POWEROFF_PIC_SIDELONG_NAME, (char *)filename, PATH_MAX );
        if ( -1 == tmp )
        {
            print_warning("get_config failed");
            return false;
        }
    }
    else// if ( (angle == ROTATION_90) || (angle == ROTATION_270) )
    {
        //读取默认解码文件全路径
        tmp = get_config( CFG_DEF_ANIMATION_POWEROFF_PIC_UPRIGHT_NAME, (char *)filename, PATH_MAX );
        if ( -1 == tmp )
        {
            print_warning("get_config failed");
            return false;
        }
    }
    
    return true;
}

