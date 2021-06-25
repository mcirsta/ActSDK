/********************************************************************************
 *                              USDK(1100)
 *                             Module: photo app
 *                 Copyright(c) 2003-2009 Actions Semiconductor,
 *                            All Rights Reserved.
 * File : photo_scene_playing.h
 * History:
 *      <author>    <time>           <version >             <desc>
 *      carrie     2009-4-24          v1.0               build this file
 ********************************************************************************/
/*!
 * \file     photo_scene_playing.h
 * \brief    photo playing scene
 * \author   carrie
 *
 * \version 1.0
 * \date  2009/04/24
 *******************************************************************************/

#ifndef __PHOTO_SCENE_PLAYING_H__
#define __PHOTO_SCENE_PLAYING_H__

#include "photo.h"

/* exit type */
typedef enum
{
    SCENE_PLAYING_RETURN,
    SCENE_PLAYING_EXIT_APP,
    SCENE_PLAYING_NO_FILE,
    SCENE_PLAYING_STORAGE_OUT,

} scene_playing_ret_e;

typedef struct
{
    string_resource_t *str_rotate;
    string_resource_t *str_zoom;
    string_resource_t *str_slideinterval;
    string_resource_t *str_slideeffect;
    string_resource_t *str_setbg;
    string_resource_t *str_delete;
    string_resource_t *str_info;

} resg_option_t;

typedef struct
{
    string_resource_t *str_zoomin;
    string_resource_t *str_zoomout;

} resg_opt_zoom_t;

typedef struct
{
    string_resource_t *str_sec;

} resg_opt_slideinterval_t;

typedef struct
{
    picture_resource_t *pic_current;
    string_resource_t *str_random;
    string_resource_t *str_flyin;
    string_resource_t *str_check;
    string_resource_t *str_cut;
    string_resource_t *str_erase;
    string_resource_t *str_louver;
    string_resource_t *str_randomline;
    string_resource_t *str_waterwave;
    string_resource_t *str_cube;
    string_resource_t *str_3drotation;
    string_resource_t *str_off;

} resg_opt_slideeffect_t;

typedef struct
{
    string_resource_t *str_filename;
    string_resource_t *str_resolution;
    string_resource_t *str_filesize;
    string_resource_t *str_date;

    picture_resource_t *pic_filename;
    picture_resource_t *pic_resolution;
    picture_resource_t *pic_filesize;

} resg_opt_info_t;

typedef struct
{
    string_resource_t *str_background;
    string_resource_t *str_bootlogo;
    string_resource_t *str_standbylogo;
    string_resource_t *str_default;

} resg_opt_setbg_t;

typedef struct
{
    resgroup_resource_t *resg;
    picture_resource_t *pic_prev;
    picture_resource_t *pic_prevh;
    picture_resource_t *pic_next;
    picture_resource_t *pic_nexth;
    picture_resource_t *pic_r90;
    picture_resource_t *pic_r180;
    picture_resource_t *pic_r270;
    picture_resource_t *pic_r360;
} resg_directbar_t;

typedef struct
{
    resgroup_resource_t *resg;
    picture_resource_t *pic_tbar;
    picture_resource_t *pic_auto;
    string_resource_t *str_name;
    string_resource_t *str_num;
} resg_txtbar_t;

/* define scene data */
typedef struct scene_play_s
{
    int                     win_id;             /* window resource */
    int                     slideshow_timer_id;  /* timer id for slideshow */
    int                     opration_timer_id;  /* 用户按键之后，显示当前directbar, */
                                            /* txtbar, headbar等，3秒之后隐藏 */
    bool                    user_opration_status; /* 当前是否数据用户操作状态（持续3秒）  */
    bool                    slideshow;

    int                     dec_single_timer_id;    /* 静态图片解码完毕之后启动的singleshot timer */
    int                     dec_gif_single_timer_id;/* gif图片解码完毕之后启动的singleshot timer */
    int                     cur_gif_frames;         /* gif已播帧数（不是gif文件中的帧id） */

    /* 保存在config里面的值 */
    int                     slide_interval;
    int                     fusion_mode;
    int                     path_type;

    /* current information */
    dec_file_info_t         cur_file_info; /* 当前解码图片的信息 */
    int                     cur_file_index; /* 当前图片在playlist中的index */
    int                     cur_rotate;
    int                     cur_zoom;   /* 当前缩放情况：0-无缩放，<0--缩小，>0-放大 */
    int                     fontdes;

    /* style resource */
    scene_t                 *common_scene;
    scene_t                 *scene;
    picture_resource_t      *pic_bg;
    picture_resource_t      *pic_logo;
    string_resource_t       *str_title;
    resg_directbar_t        resg_directbar;
    resg_txtbar_t           resg_txtbar;

    /* option */
    resg_option_t           resg_option;
    resg_opt_zoom_t         resg_opt_zoom;
    resg_opt_slideinterval_t resg_opt_slideinterval;
    resg_opt_slideeffect_t  resg_opt_slideeffect;
    resg_opt_setbg_t        resg_opt_setbg;
    resg_opt_info_t         resg_opt_info;

    /* widget resource */
    volumebar_t             *vbar;
    headbar_t               *hbar;
    menuview_t              *option;
    adjust_t                *adjust;
    msgbox_t                *msgbox;
    dialog_t                *dialog;
    loadingbox_t            *loadingbox;

    /* 按键时，显示 的图标*/
    picture_resource_t      *show_pic_prev;
    picture_resource_t      *show_pic_next;
    picture_resource_t      *show_pic_rotate;
    int                     canvas_directbar_id;
    int                     canvas_txtbar_id;

    scene_playing_ret_e     exit_type;

} scene_playing_t;

/* scene entry */
int photo_scene_playing(fusion_enter_e fenter);
void scene_playing_paint(scene_playing_t *p, bool update_fb);

bool scene_playing_normal_display(scene_playing_t *p);
bool scene_playing_dialog(scene_playing_t *p, int type, unsigned int resid, dialog_cb_t callback);
void scene_playing_delete_file_dialog(void);
void set_operation_timer(scene_playing_t *p);

bool dec_gif_next_frame(scene_playing_t *p);
void dec_next_file(scene_playing_t *p);
void dec_prev_file(scene_playing_t *p);
bool dec_cur_file(scene_playing_t *p);


extern scene_playing_t* g_playing;

#endif

