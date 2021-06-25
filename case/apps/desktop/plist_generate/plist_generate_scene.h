/*******************************************************************************
 *                              USDK(1100)
 *                             Module: app 
 *                 Copyright(c) 2003-2009 Actions Semiconductor,
 *                            All Rights Reserved. 
 * History:         
 *      <author>    <time>           <version >             <desc>
 *      shaofeng    2009-05-25          v1.0               build this file 
*******************************************************************************/
/*!
 * \file     plist_generate_scene.h
 * \brief    
 * \author   shaofeng
 *               
 * \version 1.0
 * \date  2009/05/25
*******************************************************************************/

#ifndef __PLIST_GENERATE_SCENE_H__
#define __PLIST_GENERATE_SCENE_H__

#include "include_case.h"




/*!
*\brief
    �������ʾ״̬ �ṹ
*/
typedef struct
{
    char bg;
    char wait;
    
    char pbar_bg;
    char pbar;
    
    char str_wait;
}plist_generate_saved_show_t;


/*!
*\brief
    ��ʾ״̬ �ṹ
*/
typedef struct
{
    char bg;
    char wait;
    char wait_frame;
    
    char pbar_bg;
    char pbar;
    
    char str_wait;
}plist_generate_show_t;

/*!
* scene type define
*/
typedef enum
{
    SCENE_ERR = 0,
    SCENE_EXIT,
    SCENE_RETURN,
    
    SCENE_LIST,
    SCENE_PLAYING,
}scene_type_e;

/* scene of all apps */
typedef struct
{
    scene_type_e scene_type;
    
    /* style resource */
    scene_t* scene;
    scene_t* com_scene;
    
    /* window resource */
    int win_id;
    int handle_gui_dc;
}app_scene_t;


typedef struct
{
    app_scene_t app_scene;
    
    int cur_percent;
    int timer_id;
    
    /*! ���ڿ�����������ʾ״̬, ��Ҫ�û��ı�, �������ʾ */
    plist_generate_show_t show;
    /*! ���ڱ��浱ǰ��ʾ״̬, �û�����Ҫ���� */
    plist_generate_saved_show_t saved_show;
    
    picture_resource_t *pic_bg;
    gui_bitmap_info_t  *bg_bitmap_info;
    
    picture_resource_t *pic_pbar_bg;
    picture_resource_t *pic_pbar_hl;
    resgroup_resource_t *resgroup_pbar_highlight_rect;
    int pbar_length;
    int pbar_xstart;
    
    picregion_resource_t *reg_wait;
    gui_bitmap_info_t    **wait_bitmap_info;
    
    string_resource_t *str_wait;
    string_resource_t *str_nospace;

    /*! ���ɱ�־: false-�ɹ�; true-ʧ�� */
    bool gen_fail;
}plist_generate_scene_t;



/************************ extern function, �������� ***************************/
extern bool plist_generate_load_style( plist_generate_scene_t *scene );
extern void plist_gene_scene( plist_generate_scene_t *scene );



#endif  /* __PLIST_GENERATE_SCENE_H__ */

