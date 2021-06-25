/*******************************************************************************
 *                              USDK(1100)
 *                            Module: camera
 *                 Copyright(c) 2003-2009 Actions Semiconductor,
 *                            All Rights Reserved.
 *
 * History:
 *      <author>    <time>           <version >             <desc>
 *    Albert_lyu 2009-5-26  1.0             build this file
 *******************************************************************************/
/*!
 * \file    camera_scene_pc.h
 * \brief    pc场景头文件
 * \author   Albert_lyu
 * \version 1.0
 * \date  2009-5-26
 *******************************************************************************/
#ifndef __CAMERA_SCENE_PC_H__
#define __CAMERA_SCENE_PC_H__
#include "include_case.h"
#include "include_psp.h"

#include "camera_main.h"
typedef struct camera_scene_pc_s
{
    /* style resource */
    scene_t* scene;
    scene_t* common_scene;

    /* font description*/
    int fontdes;

    /* window resource */
    int win_id;
    int hdc;
    /* resource for painting */
    picture_resource_t *pc_bg;
    picture_resource_t *icon_pccam;

    msgbox_t *msgbox;
    dialog_t *dialog;

    gui_bitmap_info_t *bg_bitmap_info; /* 背景图 */

    int need_exit; //TRUE: need to exit
    app_cam_scene_type_e exit_type;
    int pc_even_id;

} camera_scene_pc_t;

extern camera_scene_pc_t* g_camera_scene_pc;
int camera_scene_pc(void);
void camera_scene_pc_paint(camera_scene_pc_t *data);
#endif   /* __CAMERA_SCENE_PC_H__ */

