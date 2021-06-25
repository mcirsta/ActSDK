/********************************************************************************
 *                              USDK(1100)
 *                             Module: app 
 *                 Copyright(c) 2003-2009 Actions Semiconductor,
 *                            All Rights Reserved. 
 * File : music_scene_playing.h
 * History:         
 *      <author>    <time>           <version >             <desc>
 *    songzhining    2009-2-25          v1.0               build this file 
 ********************************************************************************/
/*!
* \file     music_scene_playing.h
* \brief    playing scene
* \author   songzhining
*               
* \version 1.0
* \date  2009/02/25
*******************************************************************************/

#ifndef __SCENE_PLAYING_H__
#define __SCENE_PLAYING_H__

#include "include_case.h"



/* define max eq type*/
#define EQ_MAX 11

/* define text scroll interval */
#define SCROLL_INTERVAL 100

/* exit type */
typedef enum
{
    SCENE_PLAY_RETURN,
    SCENE_PLAY_NEXT,
    SCENE_PLAY_QUIT,
    
}scene_play_ret_t;


/* define scene data */
typedef struct scene_play_s
{
    /* style resource */
    scene_t* scene;
    
    /* common scene */
    scene_t *common_scene;        

    /* window resource */
    int win_id;
    
    /*! »æÍ¼DC */
    int hdc;
    int fontdes;
    
    /* widget resource */
    volumebar_t* vbar;
    headbar_t* hbar;
    menuview_t* opt;
    adjust_t *adjust;
    dialog_t *dialog;
    msgbox_t* msgbox;

    usereq_widget_t * usereq;
    srsuser_widget_t * srsuser;
    
    /* other data */
    bool bg_inited; //true: bg painted
    scene_play_ret_t exit_type;    //0: return, 1: next scene

    
    /* playing data */
    mengine_t engine;   //engine info
    int timer_id;   //timer id for play timing
    plist_detail_info_t *id3_info;
    gui_bitmap_info_t *album_bmp;
    bool album_flag;    //
    int single_timer;
    bool press_prev;
    bool press_state;
    bool press_next;
    
    /* scroll text */
    int scroll_timer;
    gui_scroll_info_t *scroll_info;
    
    bool lyric_flag;
    char prev_lyric[PATH_MAX];
    char cur_lyric[PATH_MAX];
    char next_lyric[PATH_MAX]; 
    
    void* ssf_handle;
    void* wav_handle;
    int eff_frame;

    gui_bitmap_info_t *bmp_bg;  //bitmap cache
    gui_bitmap_info_t *bmp_pb_bg;  //bitmap cache
    gui_bitmap_info_t *bmp_pb_full;  //bitmap cache
    gui_bitmap_info_t *bmp_pb_dot;  //bitmap cache
    gui_bitmap_info_t *bmp_effect_bg;  //bitmap cache
    gui_bitmap_info_t *bmp_effect_dot;  //bitmap cache
    gui_bitmap_info_t *bmp_effect_top;  //bitmap cache

    /* resource for painting */
    resgroup_resource_t *resgroup;
    picture_resource_t *pic_bg;
    picture_resource_t *pic_title;
    string_resource_t *str_app_name;
    picture_resource_t *pic_disk;
    picture_resource_t *pic_album_bg;
    picture_resource_t *pic_def_album;
    string_resource_t *str_index;
    string_resource_t *str_title;
    
    picture_resource_t *pic_artist;
    string_resource_t *str_artist;
    picture_resource_t *pic_album;
    string_resource_t *str_album;
    picture_resource_t *pic_genre;
    string_resource_t *str_genre;
    
    picture_resource_t *pic_effect_bg;
    picture_resource_t *pic_effect_dot;
    picture_resource_t *pic_effect_top;
    string_resource_t *str_lyric1;
    string_resource_t *str_lyric2;
    string_resource_t *str_lyric3;
    
    string_resource_t *str_effect;
    picregion_resource_t *reg_ab;
    picregion_resource_t *reg_playmode;
    picregion_resource_t *reg_star;
    picregion_resource_t *reg_fast;
    picregion_resource_t *reg_lyric;
    
    picture_resource_t *pic_pb_bg;
    picture_resource_t *pic_pb_full;
    picture_resource_t *pic_pb_dot;

    string_resource_t *str_cur_time;
    string_resource_t *str_total_time;

    picregion_resource_t *reg_state;
    picregion_resource_t *reg_prev;
    picregion_resource_t *reg_next;
    
    string_resource_t *str_eq[EQ_MAX];

    string_resource_t *str_del_file;
    string_resource_t *str_del_tag;
    string_resource_t *str_deleting;
    string_resource_t *str_searching;
    string_resource_t *str_del_fail;
    string_resource_t *str_nospace;
    string_resource_t *str_add_fail;
    
    picture_resource_t *pic_tick;
    
    string_resource_t *str_err_openfile;
    string_resource_t *str_err_notsupport;
    string_resource_t *str_err_decode;
    string_resource_t *str_err_nolicense;
    string_resource_t *str_err_clock;
    string_resource_t *str_err_drminfo;
    
}scene_play_t;


extern scene_play_t* g_scene_playing;

/* scene quit */
bool scene_playing_quit( void );

/* scene entry */
int music_scene_playing( fusion_enter_e fenter );


/* scene update playing status */
int playing_update_status( void );

#endif

