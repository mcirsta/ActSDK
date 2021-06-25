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

/* include file of this application, ��Ӧ�õ�ͷ�ļ� */
#include "include_video.h"

#if 0
/*
 * ���õ�ǰ����ȫ��������Ϊ��Ч�ĵ�һ֡
 * ע��:
 fusion_set_first_frame(); fusion_set_last_frame(); fusion_display()����һ��ʹ��, �����ڴ�й©
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
 * ���õ�ǰ����ȫ��������Ϊ��Ч�����һ֡
 * ע��:
 fusion_set_first_frame(); fusion_set_last_frame(); fusion_display()����һ��ʹ��, �����ڴ�й©
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
 * Option�˵���Ч��һ֡
 * ע��:
 fusion_set_first_frame(); fusion_set_last_frame(); fusion_display()����һ��ʹ��, �����ڴ�й©
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
    //yuv�����һ֡��alpha������true, RGB������false
    frame.alpha = true;
    fusion_set_first_frame(&frame);
}

/*
 * Option�˵���Ч���һ֡
 * ע��:
 fusion_set_first_frame(); fusion_set_last_frame(); fusion_display()����һ��ʹ��, �����ڴ�й©
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

