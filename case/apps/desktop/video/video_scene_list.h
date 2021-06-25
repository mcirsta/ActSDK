/********************************************************************************
 *                              USDK(1100)
 *                             Module: app 
 *                 Copyright(c) 2003-2009 Actions Semiconductor,
 *                            All Rights Reserved. 
 * History:         
 *      <author>    <time>           <version >             <desc>
 *      shaofeng    2009-05-15          v1.0               build this file 
********************************************************************************/
/*!
 * \file     video_scene_list.h
 * \brief    
 * \author   shaofeng
 *               
 * \version 1.0
 * \date  2009/05/15
*******************************************************************************/

#ifndef __VIDEO_SCENE_LIST_H__
#define __VIDEO_SCENE_LIST_H__

#include "include_case.h"
#include "include_psp.h"

 


typedef struct
{
    app_scene_t app_scene;
    /* ����scene, ��������Ч״̬. ֻ���յ�down�����Ч */
    char is_key_valid;
    
    /*! resource of show */
    picture_resource_t      *pic_bg;
    gui_bitmap_info_t       *bg_bitmap_info;
}video_scene_list_t;



/************************ extern function, �������� ***************************/
extern bool video_scene_list_load_style( video_scene_list_t *scene_list );
extern bool video_scene_list_rotate( video_scene_list_t *scene_list );

/* scene entry */
extern scene_type_e video_scene_list( video_scene_list_t *scene_list );



#endif  /* __VIDEO_SCENE_LIST_H__ */

