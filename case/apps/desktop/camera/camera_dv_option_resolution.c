/*******************************************************************************
 *                              USDK(1100)
 *                            Module: camera_dv
 *                 Copyright(c) 2003-2008 Actions Semiconductor,
 *                            All Rights Reserved.
 *
 * History:
 *      <author>    <time>           <version >             <desc>
 *        2009-5-26 16:32   1.0             build this file
 *******************************************************************************/
/*!
 * \file     camera_dv_option_resolution.c
 * \brief    概述
 * \author
 * \par      GENERAL DESCRIPTION:
 *               菜单项-分辨率切换
 * \par      EXTERNALIZED FUNCTIONS:
 *               用到外面的模块
 * \version 1.0
 * \date  2009-5-26
 *******************************************************************************/
#include "camera_dv_option_resolution.h"
#include "camera_dv_config.h"
#include "camera_sty.h"
#include "camera_dv_option.h"
#include "camera_dv_ui.h"
#include "camera_scene_dv.h"
#include "camera_dv_control.h"
#include "camera_define.h"

static int _proc_items_do_resolution(menuview_t *menuview, menuview_sig_e signal);

/* menu list data define */
menu_t *g_camera_dv_option_resolution = NULL;

/* menu max items */

/* 进入主菜单 */
bool camera_dv_option_resolution_enter(menuview_t *menuview)
{
    menu_t *cur_menu;

    cur_menu = camera_dv_option_resolution_init();
    menuview_set_menu(menuview, cur_menu);
    menuview_paint(menuview);

    return true;
}

/* 退出主菜单 */
bool camera_dv_option_resolution_exit(menuview_t *menuview)
{
    //delete resolution menu
    camera_dv_option_resolution_deinit();

    //return to option
    camera_dv_option_enter(menuview, FUSION_BACKWARD);

    return true;
}

menu_t* camera_dv_option_resolution_init(void)
{
    scene_t* scene; 
    char * str_res[] = {"640X480", "320X240", "224X176", "176X144"};
    menuitem_t item = {0};
    int num;
    int offset, max_scale;
    
    camera_dv_get_cur_sensor_items(&offset, &max_scale);
    //initial menu
    if (g_camera_dv_option_resolution == NULL)
    {
        //get scene
        scene = sty_get_app_scene();
        if (scene == NULL)
        {
            print_err("cann't find app scene!");
            return false;
        }

        memset(&item, 0, sizeof(item));

        //create menu
        g_camera_dv_option_resolution = menu_create();
        if (g_camera_dv_option_resolution == NULL)
        {
            print_err("cann't create menu!");
            return false;
        }
        else
        {
            // append items
            if (g_camera_dv_cfgs[CAM_DV_CFG_TYPE_RESOLUTION].value > max_scale)
            {
                g_camera_dv_cfgs[CAM_DV_CFG_TYPE_RESOLUTION].value = 0;
                g_camera_dv_cfgs[CAM_DV_CFG_TYPE_RESOLUTION].changed = true;
            }
            for (num = 0; num < (int) (max_scale + 1); num++)
            {
                item.has_child = false;
                item.callback = (item_cb_t) _proc_items_do_resolution;
                if (num == g_camera_dv_cfgs[CAM_DV_CFG_TYPE_RESOLUTION].value)
                {
                    item.sel_icon_id = (int) g_cam_dv_opt_dot->pic_id;
                    item.unsel_icon_id = (int) g_cam_dv_opt_dot->pic_id;
                }
                else
                {
                    item.sel_icon_id = 0;
                    item.unsel_icon_id = 0;
                }
                item.text = str_res[num + offset];
                menu_append_item(g_camera_dv_option_resolution, &item);
            }
        }

        //init current index
        menu_set_cur_index(g_camera_dv_option_resolution, 0);
    }

    return g_camera_dv_option_resolution;
}

int camera_dv_option_resolution_deinit(void)
{
    //delete menu
    if (g_camera_dv_option_resolution != NULL)
    {
        //create menu
        menu_delete(g_camera_dv_option_resolution);
        g_camera_dv_option_resolution = NULL;
    }

    return true;
}
static int _proc_items_do_resolution(menuview_t *menuview, menuview_sig_e signal)
{
    menu_t *cur_menu = menuview->menu;
    menuitem_t *cur_item = menu_get_cur_item(cur_menu);
    mmm_res_constant_t temp_res;
    switch (signal)
    {
        case WIDGET_SIG_MENUVIEW_OK:
        {
            int cur_item_pos = menu_get_cur_index(cur_menu);
            menuitem_t *save_opt_item = menu_get_item(cur_menu, g_camera_dv_cfgs[CAM_DV_CFG_TYPE_RESOLUTION].value);

            if (save_opt_item != NULL)
            {
                save_opt_item->sel_icon_id = 0;
                save_opt_item->unsel_icon_id = 0;
            }
            if (cur_item != NULL)
            {
                cur_item->sel_icon_id = (int) g_cam_dv_opt_dot->pic_id;
                cur_item->unsel_icon_id = (int) g_cam_dv_opt_dot->pic_id;
                print_dbg(">>>> menu_list: %s clicked", cur_item->text);
            }

            //exit and save entry
            temp_res = camera_dv_ui_resolution_to_buf(cur_item_pos);
            camera_dv_ctr_set_resolution( temp_res);
            //camera_dv_ctr_get_disk_info(&(g_camera_scene_dv->dv_config.cfg_freeinfo));
            camera_dv_save_parameter(CAM_DV_PARA_RESOLUTION, cur_item_pos);

            /*spec 要求qvga以下分辨率要22帧*/
            if (temp_res > MMM_RES320X240)
            {
                dv_video_args.frame_rate = (unsigned int)g_camera_dv_cfgs[CAM_DV_CFG_TYPE_REC_FRAME_RATE_MIN].value;
            }
            else
            {
                dv_video_args.frame_rate = (unsigned int)g_camera_dv_cfgs[CAM_DV_CFG_TYPE_REC_FRAME_RATE_MAX].value;
            }
            
            dv_video_args.bitrate_level = camera_dv_ui_quality_to_bitrate(
            g_camera_dv_cfgs[CAM_DV_CFG_TYPE_RECORD_QUALITY].value);


            camera_dv_ctr_set_record_quality((mmm_video_args_t *) (&dv_video_args));
            camera_dv_ctr_get_disk_info(&(g_camera_scene_dv->dv_config.cfg_freeinfo));          


            
#if (MENU_SELECT_OK_HIDE == 1)
            camera_dv_widget_option_exit(menuview, camera_dv_option_resolution_enter);
#else
            menuview_set_menu(menuview, cur_menu);
            menuview_paint(menuview);
#endif /* MENU_SELECT_OK_HIDE */

            camera_dv_change_icon(CAM_ICON_RESOLUTION, 0);
        }
        break;

        case WIDGET_SIG_MENUVIEW_QUIT:
        // return to parent menu
        camera_dv_option_resolution_exit(menuview);
        break;

        default:
        break;
    }
    return 0;
}
