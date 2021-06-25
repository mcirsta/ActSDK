/*******************************************************************************
 *                              USDK(1100)
 *                            Module: camera_dsc
 *                 Copyright(c) 2003-2008 Actions Semiconductor,
 *                            All Rights Reserved.
 *
 * History:
 *      <author>    <time>           <version >             <desc>
 *        2009-5-26 16:32   1.0             build this file
 *******************************************************************************/
/*!
 * \file     camera_dsc_option_resolution.c
 * \brief    概述
 * \author
 * \par      GENERAL DESCRIPTION:
 *               菜单项-分辨率切换
 * \par      EXTERNALIZED FUNCTIONS:
 *               用到外面的模块
 * \version 1.0
 * \date  2009-5-26
 *******************************************************************************/
#include "camera_dsc_option_resolution.h"
#include "camera_dsc_config.h"
#include "camera_sty.h"
#include "camera_dsc_option.h"
#include "camera_dsc_ui.h"
#include "camera_dsc_control.h"
#include "camera_scene_dsc.h"
#include "camera_define.h"

static int _proc_items_do_resolution(menuview_t *menuview, menuview_sig_e signal);

/* menu list data define */
menu_t *g_camera_dsc_option_resolution = NULL;

/* menu max items */

/* 进入主菜单 */
bool camera_dsc_option_resolution_enter(menuview_t *menuview)
{
    menu_t *cur_menu;

    cur_menu = camera_dsc_option_resolution_init();
    menuview_set_menu(menuview, cur_menu);
    menuview_paint(menuview);

    return true;
}

/* 退出主菜单 */
bool camera_dsc_option_resolution_exit(menuview_t *menuview)
{
    //delete resolution menu
    camera_dsc_option_resolution_deinit();

    //return to option
    camera_dsc_option_enter(menuview, FUSION_BACKWARD);

    return true;
}

menu_t* camera_dsc_option_resolution_init(void)
{
    scene_t* scene; //!< app scene
    char * str_res[] =
    {
        "4672X3504", "4416X3312", "3840X2880", "3264X2448", "2816X2112", "2560X1920", "2400X1808", "2048X1536",
        "1600X1200", /*"1280X960",*/"1280X1024", "1024X768", "640X480",
    };
#if 0
    // 3.1M
#if (CMOS_SENSOR == 310)
    {
        "4672X3504","4416X3312","3840X2880","3264X2448","2816X2112","2560X1920","2400X1808","2048X1536",
    };
    // 2M
#elif (CMOS_SENSOR == 200)
    {
        "3840X2880","3264X2448","2816X2112","2560X1920","2400X1808","2048X1536","1600X1200",
    };
    // 1.3M
#elif (CMOS_SENSOR == 130)
    {
        "2560X1920","2400X1808","2048X1536","1600X1200","1280X960",
    };
    // .3M
#elif (CMOS_SENSOR == 30)
    {
        "2048X1536","1600X1200","1280X960","1024X768","640X480",
    };
#endif
#endif
    //TODO 有待改进用法camera_dsc_ui_resolution_to_buf()
    //picture_resource_t *g_cam_dsc_opt_dot;

    menuitem_t item =
    { 0 };
    int num;
    int offset, max_scale;
    camera_dsc_get_cur_sensor_items(&offset, &max_scale);

    //initial menu
    if (g_camera_dsc_option_resolution == NULL)
    {
        //get scene
        scene = sty_get_app_scene();
        if (scene == NULL)
        {
            print_err("cann't find app scene!");
            return NULL;
        }

        memset(&item, 0, sizeof(item));

        //create menu
        g_camera_dsc_option_resolution = menu_create();
        if (g_camera_dsc_option_resolution == NULL)
        {
            print_err("cann't create menu!");
            return NULL;
        }
        else
        {
            // append items
            if (g_camera_dsc_cfgs[CAM_DSC_CFG_TYPE_RESOLUTION].value > max_scale)
            {
                g_camera_dsc_cfgs[CAM_DSC_CFG_TYPE_RESOLUTION].value = 0;
                g_camera_dsc_cfgs[CAM_DSC_CFG_TYPE_RESOLUTION].changed = true;
            }
            for (num = 0; num < (int) (max_scale + 1); num++)

            {
                item.has_child = false;
                item.callback = (item_cb_t) _proc_items_do_resolution;
                if (num == g_camera_dsc_cfgs[CAM_DSC_CFG_TYPE_RESOLUTION].value)
                {
                    item.sel_icon_id = (int) g_cam_dsc_opt_dot->pic_id;
                    item.unsel_icon_id = (int) g_cam_dsc_opt_dot->pic_id;
                }
                else
                {
                    item.sel_icon_id = 0;
                    item.unsel_icon_id = 0;
                }
                item.text = str_res[num + offset];
                menu_append_item(g_camera_dsc_option_resolution, &item);
            }
        }

        //init current index
        menu_set_cur_index(g_camera_dsc_option_resolution, 0);
    }

    return g_camera_dsc_option_resolution;
}

int camera_dsc_option_resolution_deinit(void)
{
    //delete menu
    if (g_camera_dsc_option_resolution != NULL)
    {
        //create menu
        menu_delete(g_camera_dsc_option_resolution);
        g_camera_dsc_option_resolution = NULL;
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
            menuitem_t *save_opt_item = menu_get_item(cur_menu, g_camera_dsc_cfgs[CAM_DSC_CFG_TYPE_RESOLUTION].value);

            if (save_opt_item != NULL)
            {
                save_opt_item->sel_icon_id = 0;
                save_opt_item->unsel_icon_id = 0;
            }
            if (cur_item != NULL)
            {
                cur_item->sel_icon_id = (int) g_cam_dsc_opt_dot->pic_id;
                cur_item->unsel_icon_id = (int) g_cam_dsc_opt_dot->pic_id;
                print_dbg(">>>> menu_list: %s clicked", cur_item->text);                
            }
            //g_camera_dsc_cfgs[CAM_DSC_CFG_TYPE_RESOLUTION].value = cur_item_pos;
            //exit and save entry
            temp_res = camera_dsc_ui_resolution_to_buf(cur_item_pos);
            
            camera_dsc_ctr_set_resolution( temp_res);                
            camera_dsc_ctr_get_disk_info(&(g_camera_scene_dsc->dsc_config.cfg_freeinfo));
            camera_dsc_change_icon(CAM_ICON_RESOLUTION, (unsigned int)cur_item_pos);
            camera_dsc_save_parameter(CAM_DSC_PARA_RESOLUTION, cur_item_pos);
           
#if (MENU_SELECT_OK_HIDE == 1)
            camera_dsc_widget_option_exit(menuview, camera_dsc_option_resolution_enter);
#else
            menuview_set_menu(menuview, cur_menu);
            menuview_paint(menuview);
#endif /* MENU_SELECT_OK_HIDE */
        }
        break;

        case WIDGET_SIG_MENUVIEW_QUIT:
        // return to parent menu
        camera_dsc_option_resolution_exit(menuview);
        break;

        default:
        break;
    }
    return 0;
}
