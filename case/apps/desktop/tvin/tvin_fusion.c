/*******************************************************************************
 *                              NOYA1100
 *                            Module: video
 *                 Copyright(c) 2003-2008 Actions Semiconductor,
 *                            All Rights Reserved. 
 * History:         
 *      <author>    <time>           <version >             <desc>
 *      Jerry Fu    2009-07-04          1.0             build this file 
*******************************************************************************/
/*!
 * \file     tvin_fusion.c
 * \brief    tvin fusion control
 * \author   Jerry Fu
 * \version 1.0
 * \date  2009/07/04
*******************************************************************************/

#include "tvin.h"
/*
 * 设置当前场景全部区域作为特效的第一帧
 */
void tvin_scene_fusion_set_first_frame(scene_t *scene)
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
 */
void tvin_scene_fusion_set_last_frame( scene_t *scene )
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

/*
 * Option菜单特效第一帧
 */
void tvin_option_fusion_set_first_frame( menuview_t *mv )
{
    fusion_frame_t frame;
    canvas_info_t info;

    canvas_get_info(mv->canvas_id, &info);

    frame.rect.x = info.rect.x;
    frame.rect.y = info.rect.y;
    frame.rect.width = info.rect.width;
    frame.rect.height = info.rect.height;
    frame.pdata = (unsigned char *)info.pdata;
    memset(frame.pdata, 0, (unsigned int)((frame.rect.width)*(frame.rect.height)*3));
    frame.alpha = true;
    fusion_set_first_frame(&frame);
}

/*
 * Option菜单特效最后一帧
 */
void tvin_option_fusion_set_last_frame( menuview_t *mv )
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



