/*******************************************************************************
 *                              USDK(1100)
 *                             Module: app 
 *                 Copyright(c) 2003-2009 Actions Semiconductor,
 *                            All Rights Reserved. 
 * History:         
 *      <author>    <time>           <version >             <desc>
 *      shaofeng    2009-05-09          v1.0               build this file 
*******************************************************************************/
/*!
 * \file     animation_poweron.h
 * \brief    
 * \author   shaofeng
 *               
 * \version 1.0
 * \date  2009/05/09
*******************************************************************************/

#ifndef __ANIMATION_POWERON_H__
#define __ANIMATION_POWERON_H__

#include "include_psp.h"
#include "include_case.h"


/*本应用的头文件*/
#include "animation_res_scene.h"
#include "animation_decoder_scene.h"



/*!
*\brief
    video启动模式枚举 */
typedef enum
{
    /*! 进入res scene, 通过读取res中图片, 并逐张显示以完成动画效果 */
    ANIMATION_START_MODE_RES = 0,
    
    /*! 进入decoder scene, 通过读取photo中设置的图片, 解码完成动画效果 */
    ANIMATION_START_MODE_DECODER,
    
}animation_start_mode_t;


typedef struct
{
    animation_start_mode_t start_mode;
    
    /* 场景: 读取res中图片, 并逐张显示以完成动画效果 */
    res_scene_t res_scene;
    
    /* 场景: 读取photo中设置的图片, 解码完成动画效果 */
    decoder_scene_t decoder_scene;
    char file_fullpath[PATH_MAX];
    
    /* system resource */
    void *gui_lib;
    void *sty_lib;
    void *cfg_lib;
    void *applib_lib;
    void *commonui_lib;
    int   fontdesc;
    
    style_t *app_style;
}animation_vars_t;



/************************ extern variable, 变量声明 ***************************/
extern animation_vars_t animation_vars;



/************************ extern function, 函数声明 ***************************/
extern int proc_sys_msg(msg_apps_t *msg);



#endif  /* __ANIMATION_POWERON_H__ */

