/********************************************************************************
 *                              USDK(1100)
 *                             Module: app
 *                 Copyright(c) 2003-2009 Actions Semiconductor,
 *                            All Rights Reserved.
 * File : browser_scene_browsing.h
 * History:
 *      <author>    <time>           <version >             <desc>
 *    binannan    2009-05-25          v1.0               build this file
 ********************************************************************************/
#ifndef __SCENE_BROWSING_H__
#define __SCENE_BROWSING_H__

#include "include_case.h"
#include "include_psp.h"
#include "browser_sty.h"
#include "browser.h"
#include "browser_browsing_widget_filelist.h"

typedef struct scene_browser_s
{
    scene_t* scene;
    scene_t* common_scene;

    int win_id;

    int fontdes;

    /* resource for painting */
    picture_resource_t *pic_bg;
    picture_resource_t* pic_title;
    string_resource_t* str_app_name;

    gui_bitmap_info_t       *bg_bitmap_info;

    string_resource_t* str_move_confirm;
    string_resource_t* str_moving;
    string_resource_t* str_move_succeed;
    string_resource_t* str_move_failed;

    string_resource_t* str_copy_confirm;
    string_resource_t* str_coping;
    string_resource_t* str_copy_succeed;
    string_resource_t* str_copy_failed;

    string_resource_t* str_delete_confirm;
    string_resource_t* str_deleting;
    string_resource_t* str_delete_succeed;
    string_resource_t* str_delete_failed;

    string_resource_t* str_replace_comfirm;
    string_resource_t* str_subdir_err;

    string_resource_t* str_file_opening;

    string_resource_t* str_file_notsup;

    string_resource_t* str_low_power;

    string_resource_t* str_no_space;

    headbar_t *headbar;

    msgbox_t * msgbox;

    filelist_t* filelist;

    menuview_t* opt;

    dialog_t *dialog;

    volumebar_t* vbar;

    softkeyboard_t* softkey;

    loadingbox_t* loadingbox;

    scene_status_t exit_type;
} scene_browsing_t;

typedef struct
{
    sem_t  sem;
    dialog_init_t modal;
    widget_sig_dialog_e  ret;
} sem_modal_t;

scene_status_t browser_scene_browsing(fusion_enter_e enter_type);

bool create_thread( void *proc, void *param );

void file_op_done(void);

bool com_create_loadingbox_widget(void);

bool  com_delete_loadingbox_widget( void );

bool com_create_noop_dialog_widget(dialog_init_t * dialog_init);

void  com_create_modal_dialog_widget(void);

extern scene_browsing_t* g_scene_browsing;

extern bool g_msg_lowpower_received;

extern sem_modal_t g_sem_modal;

extern bool g_thread_active;

#endif
