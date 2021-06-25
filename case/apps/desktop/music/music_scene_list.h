/********************************************************************************
 *                              USDK(1100)
 *                             Module: app 
 *                 Copyright(c) 2003-2009 Actions Semiconductor,
 *                            All Rights Reserved. 
 * File : music_scene_list.h
 * History:         
 *      <author>    <time>           <version >             <desc>
 *    songzhining    2009-2-25          v1.0               build this file 
 ********************************************************************************/
/*!
* \file     music_scene_list.h
* \brief    list scene
* \author   songzhining
*               
* \version 1.0
* \date  2009/02/25
*******************************************************************************/


#ifndef __MUSIC_SCENE_LIST_H__
#define __MUSIC_SCENE_LIST_H__

#include "include_case.h"



/* exit type */
typedef enum
{
    SCENE_LIST_RETURN,
    SCENE_LIST_NEXT,
    SCENE_LIST_QUIT,
    
}scene_list_ret_t;

/* define scene data */
typedef struct scene_list_s
{    
    /* style resource */
    scene_t* scene;

    /* common scene */
    scene_t *common_scene;        

    /* window resource */
    int win_id;
    
    /* widget resource */
    volumebar_t* vbar;
    headbar_t* hbar;
    menuview_t* list;
//    dirlist_t* dirlist;
    dialog_t* dialog;
    softkeyboard_t* softkeyboard;
    loadingbox_t* loadingbox;
    msgbox_t* msgbox;
    
    scene_list_ret_t exit_type;
    
    /* single timer resource */
    int single_timer;
    
    /*! »æÍ¼DC */
    int hdc;
    int fontdes;
    
    /* resource for painting */
    picture_resource_t *pic_bg;
    picture_resource_t *pic_title;
    string_resource_t *str_app_name;
    string_resource_t *str_del_file;
    string_resource_t *str_del_tag;
    string_resource_t *str_deleting;
    string_resource_t *str_searching;
    string_resource_t *str_notfound;
    string_resource_t *str_nofile;
    string_resource_t *str_notag;
    string_resource_t *str_del_fail;
    string_resource_t *str_nodisk;
    string_resource_t *str_nospace;
    
    gui_bitmap_info_t *bmp_bg;
    
    string_resource_t *str_err_openfile;
    string_resource_t *str_err_notsupport;
    string_resource_t *str_err_decode;
    string_resource_t *str_err_nolicense;
    string_resource_t *str_err_clock;
    string_resource_t *str_err_drminfo;
    
}scene_list_t;


extern scene_list_t* g_scene_list;

/* scene quit */
bool scene_list_quit( void );    

/* scene paint */
bool scene_list_paint( void );

/* scene entry */
int music_scene_list( fusion_enter_e fenter );

/* scene rotation */
bool scene_list_rotation( void );

#endif
