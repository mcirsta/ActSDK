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


/*��Ӧ�õ�ͷ�ļ�*/
#include "animation_res_scene.h"
#include "animation_decoder_scene.h"



/*!
*\brief
    video����ģʽö�� */
typedef enum
{
    /*! ����res scene, ͨ����ȡres��ͼƬ, ��������ʾ����ɶ���Ч�� */
    ANIMATION_START_MODE_RES = 0,
    
    /*! ����decoder scene, ͨ����ȡphoto�����õ�ͼƬ, ������ɶ���Ч�� */
    ANIMATION_START_MODE_DECODER,
    
}animation_start_mode_t;


typedef struct
{
    animation_start_mode_t start_mode;
    
    /* ����: ��ȡres��ͼƬ, ��������ʾ����ɶ���Ч�� */
    res_scene_t res_scene;
    
    /* ����: ��ȡphoto�����õ�ͼƬ, ������ɶ���Ч�� */
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



/************************ extern variable, �������� ***************************/
extern animation_vars_t animation_vars;



/************************ extern function, �������� ***************************/
extern int proc_sys_msg(msg_apps_t *msg);



#endif  /* __ANIMATION_POWERON_H__ */

