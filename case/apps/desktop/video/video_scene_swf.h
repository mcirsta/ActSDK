/********************************************************************************
 *                              USDK(1100)
 *                             Module: app 
 *                 Copyright(c) 2003-2009 Actions Semiconductor,
 *                            All Rights Reserved. 
 * History:         
 *      <author>    <time>           <version >             <desc>
 *      shaofeng    2009-09-02          v1.0               build this file 
********************************************************************************/
/*!
 * \file     video_scene__swf.h
 * \brief    
 * \author   shaofeng
 *               
 * \version 1.0
 * \date  2009/09/02
*******************************************************************************/

#ifndef __VIDEO_SCENE_SWF_H__
#define __VIDEO_SCENE_SWF_H__

#include "include_case.h"
#include "include_psp.h"

 

typedef struct
{
    app_scene_t app_scene;
    /* ����scene, ��������Ч״̬. ֻ���յ�down�����Ч */
    char is_key_valid;
    
    /* ���м����ȡ��status */
    mmm_player_status_t player_status;
    
    int timer_id;   //timer id for play timing
    int cur_time;   //current play time
    int total_time; //total time
    
}video_scene_swf_t;



/************************ extern function, �������� ***************************/
extern void video_scene_swf_shaked( video_scene_swf_t *scene_swf );

extern bool video_scene_swf_load_style( video_scene_swf_t *scene_swf );

/* scene entry */
extern scene_type_e video_scene_swf( video_scene_swf_t *scene_swf );



#endif  /* __VIDEO_SCENE_SWF_H__ */

