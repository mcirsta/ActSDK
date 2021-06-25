/*******************************************************************************
 *                              NOYA1100
 *                            Module: video
 *                 Copyright(c) 2003-2008 Actions Semiconductor,
 *                            All Rights Reserved. 
 * History:         
 *      <author>    <time>           <version >             <desc>
 *      shaofeng    2009-07-04          1.0             build this file 
*******************************************************************************/
/*!
 * \file     video_utils.c
 * \brief    
 * \author   shaofeng
 * \par      GENERAL DESCRIPTION:
 *               
 * \par      EXTERNALIZED FUNCTIONS:
 *               
 * \version 1.0
 * \date  2009/07/04
*******************************************************************************/

#include "include_psp.h"
#include "include_case.h"

/* include file of this application, 本应用的头文件 */
#include "include_video.h"

#if 0
/*
 * 设置当前场景全部区域作为特效的第一帧
 * 注意:
 fusion_set_first_frame(); fusion_set_last_frame(); fusion_display()必须一起使用, 否则内存泄漏
 */
void scene_fusion_set_first_frame( scene_t *scene )
{
    fusion_frame_t frame;

    frame.rect.x = scene->x;
    frame.rect.y = scene->y;
    frame.rect.width = scene->width;
    frame.rect.height = scene->height;
    frame.pdata = NULL;
    frame.alpha = false;

    fusion_set_first_frame(&frame);
}

/*
 * 设置当前场景全部区域作为特效的最后一帧
 * 注意:
 fusion_set_first_frame(); fusion_set_last_frame(); fusion_display()必须一起使用, 否则内存泄漏
 */
void scene_fusion_set_last_frame( scene_t *scene )
{
    fusion_frame_t frame;

    frame.rect.x = scene->x;
    frame.rect.y = scene->y;
    frame.rect.width = scene->width;
    frame.rect.height = scene->height;
    frame.pdata = NULL;
    frame.alpha = false;

    fusion_set_last_frame(&frame);
}

#endif

/*
 * Option菜单特效第一帧
 * 注意:
 fusion_set_first_frame(); fusion_set_last_frame(); fusion_display()必须一起使用, 否则内存泄漏
 */
void option_fusion_set_first_frame( menuview_t *mv )
{
    fusion_frame_t frame;
    canvas_info_t info;

    canvas_get_info(mv->canvas_id, &info);

    frame.rect.x = info.rect.x;
    frame.rect.y = info.rect.y;
    frame.rect.width = info.rect.width;
    frame.rect.height = info.rect.height;
    frame.pdata = (unsigned char *)info.pdata;
    //yuv界面第一帧的alpha必须是true, RGB界面是false
    frame.alpha = true;
    fusion_set_first_frame(&frame);
}

/*
 * Option菜单特效最后一帧
 * 注意:
 fusion_set_first_frame(); fusion_set_last_frame(); fusion_display()必须一起使用, 否则内存泄漏
 */
void option_fusion_set_last_frame( menuview_t *mv )
{
    fusion_frame_t frame;
    canvas_info_t info;

    canvas_get_info(mv->canvas_id, &info);

    frame.rect.x = info.rect.x;
    frame.rect.y = info.rect.y;
    frame.rect.width = info.rect.width;
    frame.rect.height = info.rect.height;
    frame.pdata = (unsigned char *)info.pdata;
    frame.alpha = true;
    fusion_set_last_frame(&frame);
}

