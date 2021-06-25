/*******************************************************************************
 *                              USDK(1100)
 *                            Module: camera_dsc
 *                 Copyright(c) 2003-2008 Actions Semiconductor,
 *                            All Rights Reserved.
 *
 * History:
 *      <author>    <time>           <version >             <desc>
 *        2009-5-26 16:31   1.0             build this file
 *******************************************************************************/
/*!
 * \file     camera_dsc_option_flash_lamp.c
 * \brief    概述
 * \author
 * \par      GENERAL DESCRIPTION:
 *               菜单项-曝光模式
 * \par      EXTERNALIZED FUNCTIONS:
 *               用到外面的模块
 * \version 1.0
 * \date  2009-5-26
 *******************************************************************************/
#include "camera_dsc_option_flash_lamp.h"
#include "camera_dsc_config.h"
#include "camera_sty.h"
#include "camera_dsc_option.h"
#include "camera_dsc_ui.h"
#include "camera_dsc_control.h"
#include "camera_main.h"
#include "camera_define.h"

static int _proc_items_do_flash_lamp(menuview_t *menuview, menuview_sig_e signal);

/* menu list data define */
menu_t *g_camera_dsc_option_flash_lamp = NULL;

/* menu max items */
#define CAMERA_DSC_OPTION_FLASH_LAMP_MAX_ITEMS   3

/* 进入主菜单 */
bool camera_dsc_option_flash_lamp_enter(menuview_t *menuview)
{
    menu_t *cur_menu;

    cur_menu = camera_dsc_option_flash_lamp_init();
    menuview_set_menu(menuview, cur_menu);
    menuview_paint(menuview);

    return true;
}

/* 退出主菜单 */
bool camera_dsc_option_flash_lamp_exit(menuview_t *menuview)
{
    //delete flash_lamp menu
    camera_dsc_option_flash_lamp_deinit();

    //return to option
    camera_dsc_option_enter(menuview, FUSION_BACKWARD);

    return true;
}

menu_t* camera_dsc_option_flash_lamp_init(void)
{
    scene_t* scene; //!< app scene
    resgroup_resource_t *group;
    string_resource_t *str_res[CAMERA_DSC_OPTION_FLASH_LAMP_MAX_ITEMS] =
    { 0 };
    //picture_resource_t *g_cam_dsc_opt_dot;

    menuitem_t item =
    { 0 };
    int num;

    if (g_camera_dsc_option_visible_cfgs[CAM_DSC_CFG_VISIBLE_TYPE_FLASH_LAMP].value == 0xffff)
    {
        return NULL;
    }

    //initial menu
    if (g_camera_dsc_option_flash_lamp == NULL)
    {
        //get scene
        scene = sty_get_app_scene();
        if (scene == NULL)
        {
            print_err("cann't find app scene!");
            return NULL;
        }

        //load resource group
        group = (resgroup_resource_t *) get_scene_child(scene, DSC_STRGROUP_FLASH_LAMP);
        str_res[0] = (string_resource_t *) get_resgroup_child(group, STR_AUTO);
        str_res[1] = (string_resource_t *) get_resgroup_child(group, STR_CAM_ON);
        str_res[2] = (string_resource_t *) get_resgroup_child(group, STR_CAM_OFF);
        //init items
        memset(&item, 0, sizeof(item));
        //create menu
        g_camera_dsc_option_flash_lamp = menu_create();
        if (g_camera_dsc_option_flash_lamp == NULL)
        {
            print_err("cann't create menu!");
            return NULL;
        }
        else
        {
            // append items
            for (num = 0; num < CAMERA_DSC_OPTION_FLASH_LAMP_MAX_ITEMS; num++)
            {
                if(str_res[num] != NULL)
                {
                    item.text_id = (int) str_res[num]->str_id;
                }
                item.has_child = false;
                item.callback = (item_cb_t) _proc_items_do_flash_lamp;
                if (num == g_camera_dsc_cfgs[CAM_DSC_CFG_TYPE_FLASH_LAMP].value)
                {
                    item.sel_icon_id = (int) g_cam_dsc_opt_dot->pic_id;
                    item.unsel_icon_id = (int) g_cam_dsc_opt_dot->pic_id;
                }
                else
                {
                    item.sel_icon_id = 0;
                    item.unsel_icon_id = 0;
                }
                menu_append_item(g_camera_dsc_option_flash_lamp, &item);
            }
        }

        //init current index
        menu_set_cur_index(g_camera_dsc_option_flash_lamp, 0);
    }

    return g_camera_dsc_option_flash_lamp;
}

int camera_dsc_option_flash_lamp_deinit(void)
{
    if (g_camera_dsc_option_visible_cfgs[CAM_DSC_CFG_VISIBLE_TYPE_FLASH_LAMP].value == 0xffff)
    {
        return true;
    }

    //delete menu
    if (g_camera_dsc_option_flash_lamp != NULL)
    {
        //create menu
        menu_delete(g_camera_dsc_option_flash_lamp);
        g_camera_dsc_option_flash_lamp = NULL;
    }

    return true;
}
static int _proc_items_do_flash_lamp(menuview_t *menuview, menuview_sig_e signal)
{
    menu_t *cur_menu = menuview->menu;
    menuitem_t *cur_item = menu_get_cur_item(cur_menu);
    switch (signal)
    {
        case WIDGET_SIG_MENUVIEW_OK:
        {
            int cur_item_pos = menu_get_cur_index(cur_menu);//MMM_AUTO_BALANCE = 0?
            menuitem_t *save_opt_item = menu_get_item(cur_menu, g_camera_dsc_cfgs[CAM_DSC_CFG_TYPE_FLASH_LAMP].value);

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
            //g_camera_dsc_cfgs[CAM_DSC_CFG_TYPE_FLASH_LAMP].value = cur_item_pos;
            camera_dsc_ctr_set_flashlight((unsigned int) cur_item_pos);
            camera_dsc_save_parameter(CAM_DSC_PARA_FLASH_LAMP, cur_item_pos);
            camera_dsc_change_icon(CAM_ICON_FLASH_LAMP, (unsigned int)cur_item_pos);
#if (MENU_SELECT_OK_HIDE == 1)
            camera_dsc_widget_option_exit(menuview, camera_dsc_option_flash_lamp_enter);
#else
            menuview_set_menu(menuview, cur_menu);
            menuview_paint(menuview);
#endif /* MENU_SELECT_OK_HIDE */
        }
        break;

        case WIDGET_SIG_MENUVIEW_QUIT:
        // return to parent menu
        camera_dsc_option_flash_lamp_exit(menuview);
        break;

        default:
        break;
    }
    return 0;
}
