/*******************************************************************************
 *                              USDK(1100)
 *                            Module: camera_dv
 *                 Copyright(c) 2003-2008 Actions Semiconductor,
 *                            All Rights Reserved.
 *
 * History:
 *      <author>    <time>           <version >             <desc>
 *        2009-5-26 17:8   1.0             build this file
 *******************************************************************************/
/*!
 * \file     camera_dv_widget_option.c
 * \brief    概述
 * \author
 * \par      GENERAL DESCRIPTION:
 *               dv菜单物件处理项
 * \par      EXTERNALIZED FUNCTIONS:
 *               用到外面的模块
 * \version 1.0
 * \date  2009-5-26
 *******************************************************************************/
#include "camera_dv_widget_option.h"
#include "camera_dv_option.h"
#include "camera_dv_option_exposure_mode.h"
#include "camera_dv_option_free_space.h"
#include "camera_dv_option_resolution.h"
#include "camera_dv_option_save_path.h"
#include "camera_dv_option_special_effect.h"
#include "camera_dv_option_white_balance.h"
#include "camera_dv_option_record_quality.h"
#include "camera_scene_dv.h"
#include "camera_main.h"
static option_entry_t s_option_entry = NULL;
static menuview_cb_t s_widget_option_cb = NULL;

static void _menuview_callback(menuview_t *menuview, menuview_sig_e signal)
{
    //here if photo.music....list quit 2 desktop ,must todo here
}

menuview_t* camera_dv_widget_option_create(menuview_cb_t callback)
{
    menuview_t *menuview;
    //create list
    menuview = menuview_create(MENUVIEW_OPTION, _menuview_callback);
    if (menuview == NULL)
    {
        print_err("menuview create fail!");
        return NULL;
    }
    else
    {
        g_camera_scene_dv->opt = menuview; 
        //restore last menu
        if (s_option_entry != NULL)
        {
            (s_option_entry)(menuview);
        }
        else
        {
            #if 0
            /*! 设置特效的第一帧 */
            option_fusion_set_first_frame(menuview);
            #endif
            camera_dv_option_enter(menuview, FUSION_FORWARD);

            #if 0
            /*! 设置特效的最后一帧 */
            option_fusion_set_last_frame(menuview);
            #endif
        }

        //save callback
        s_widget_option_cb = callback;
    }

    return menuview;
}

bool camera_dv_widget_option_delete(menuview_t *menuview)
{
    if (menuview == NULL)
    {
        print_err("menuview delete fail!");
        return false;
    }
    else
    {
        //delete menuview
        camera_dv_option_blending_delete();
        menuview_delete(menuview);

        //delete menu
        //		camera_dv_main_deinit();
        //		camera_dv_allmusic_deinit();
    }
    return true;
}

bool camera_dv_widget_option_proc_gui(menuview_t *menuview, gui_wm_message_t *pmsg)
{
    return menuview_proc_gui(menuview, pmsg);
}

bool camera_dv_widget_option_exit(menuview_t *menuview, option_entry_t entry)
{
    //save last menu entry
    s_option_entry = entry;

    //callback
    if (s_widget_option_cb != NULL)
    {
        (s_widget_option_cb)(menuview, WIDGET_SIG_MENUVIEW_QUIT);
        return true;
    }
    else
    {
        return false;
    }
}

bool camera_dv_widget_option_init(void)
{
    //不需要全初始化
    camera_dv_option_init();
    camera_dv_option_exposure_mode_init();
    camera_dv_option_free_space_init();
    camera_dv_option_resolution_init();
    camera_dv_option_save_path_init();
    camera_dv_option_special_effect_init();

    camera_dv_option_white_balance_init();
    camera_dv_option_record_quality_init();
    return true;
}

bool camera_dv_widget_option_deinit(void)
{
    //退出场景要全删除数据
    camera_dv_option_deinit();
    camera_dv_option_exposure_mode_deinit();
    camera_dv_option_free_space_deinit();
    camera_dv_option_resolution_deinit();
    camera_dv_option_save_path_deinit();
    camera_dv_option_special_effect_deinit();
    camera_dv_option_white_balance_deinit();
    camera_dv_option_record_quality_deinit();
    return true;
}

