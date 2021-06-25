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
 * \file     camera_dsc_option_save_path.c
 * \brief    概述
 * \author
 * \par      GENERAL DESCRIPTION:
 *               菜单项-保存介质
 * \par      EXTERNALIZED FUNCTIONS:
 *               用到外面的模块
 * \version 1.0
 * \date  2009-5-26
 *******************************************************************************/
#include "camera_dsc_option_save_path.h"

#include "camera_dsc_config.h"
#include "camera_sty.h"
#include "camera_dsc_option.h"
#include "camera_dsc_ui.h"

#include "camera_scene_dsc.h"
#include "camera_dsc_control.h"
#include "camera_define.h"

/* menu max items */
#define CAMERA_DSC_OPTION_SAVE_PATH_MAX_ITEMS   3
//static menuitem_t *s_save_opt_item = NULL;
static int s_save_opt_item_num = -1;

static string_resource_t *s_str_res[CAMERA_DSC_OPTION_SAVE_PATH_MAX_ITEMS] =
{ 0 };
static int _proc_items_do_save_path(menuview_t *menuview, menuview_sig_e signal);

/* menu list data define */
menu_t *g_camera_dsc_option_save_path = NULL;

/* 进入主菜单 */
bool camera_dsc_option_save_path_enter(menuview_t *menuview)
{
    menu_t *cur_menu;

    cur_menu = camera_dsc_option_save_path_init();
    menuview_set_menu(menuview, cur_menu);
    menuview_paint(menuview);

    return true;
}

/* 退出主菜单 */
bool camera_dsc_option_save_path_exit(menuview_t *menuview)
{
    //delete save_path menu
    camera_dsc_option_save_path_deinit();

    //return to option
    camera_dsc_option_enter(menuview, FUSION_BACKWARD);

    return true;
}

menu_t*     camera_dsc_option_save_path_init(void)
{
    scene_t* scene; //!< app scene
    resgroup_resource_t *group;
    //string_resource_t *s_str_res[CAMERA_DSC_OPTION_SAVE_PATH_MAX_ITEMS] = {0};
    //picture_resource_t *g_cam_dsc_opt_dot;
    menuitem_t item =
    { 0 };
    //cam_dsc_save_path_e num;
    int num = -1;
    s_save_opt_item_num = -1;
    print_dbg("save path init fun\n");
    //initial menu
    if (g_camera_dsc_option_save_path == NULL)
    {
        //get scene
        scene = sty_get_app_scene();
        if (scene == NULL)
        {
            print_err("cann't find app scene!");
            return NULL;
        }

        //load resource group
        group = (resgroup_resource_t *) get_scene_child(scene, DSC_STRGROUP_SAVE_PATH);
        s_str_res[0] = (string_resource_t *) get_resgroup_child(group, STR_INTERNAL_MEMORY);
        s_str_res[1] = (string_resource_t *) get_resgroup_child(group, STR_CARD_MEMORY);
        s_str_res[2] = (string_resource_t *) get_resgroup_child(group, STR_EXTERNAL_MEMORY1);
        //init items
        memset(&item, 0, sizeof(item));

        //create menu
        g_camera_dsc_option_save_path = menu_create();
        if (g_camera_dsc_option_save_path == NULL)
        {
            print_err("cann't create menu!");
            return NULL;
        }
        else
        {
            menuitem_t* internal = NULL;
            hotplugin_dev_info_t internal_info;

            menuitem_t* card = NULL;
            hotplugin_dev_info_t card_info;

            menuitem_t* external = NULL;
            hotplugin_dev_info_t external_info;

            //menuitem_t* tempitem = NULL;
            //hotplugin_dev_type_e main_disk = sys_get_main_disk();

            g_camera_dsc_option_save_path->callback = (menu_cb_t) _proc_items_do_save_path;

            sys_get_hotplugin_dev_info(HOTPLUGIN_DEV_TYPE_LOCAL, &internal_info);
            if (internal_info.access == HOTPLUGIN_DEV_ACCESS_OK)
            {
                num ++;
                internal = menuitem_create();
                if (internal == NULL)
                {
                    print_err("create menuitem_t failed.\n");
                    goto item_err;
                }
                if(s_str_res[0] != NULL)
                {
                    internal->text_id = (int) s_str_res[0]->str_id;
                }
                internal->has_child = false;
                internal->callback = (item_cb_t) _proc_items_do_save_path;
                if (CAM_DSC_STORAGE_INTERNAL == g_camera_dsc_cfgs[CAM_DSC_CFG_TYPE_SAVE_PATH].value)
                {
                    internal->sel_icon_id = (int) g_cam_dsc_opt_dot->pic_id;
                    internal->unsel_icon_id = (int) g_cam_dsc_opt_dot->pic_id;
                    s_save_opt_item_num = num;
                }
                else
                {
                    internal->sel_icon_id = 0;
                    internal->unsel_icon_id = 0;
                }
                menu_append_item(g_camera_dsc_option_save_path, internal);
                menuitem_delete(internal);

                /*
                另外一种做法获取高亮item
                s_save_opt_item = menu_get_cur_item(g_camera_dsc_option_save_path);
                if(s_save_opt_item->sel_icon_id == 0)
                {
                    s_save_opt_item = NULL;
                }
                */
            }

            sys_get_hotplugin_dev_info(HOTPLUGIN_DEV_TYPE_CARD, &card_info);
            if (card_info.access == HOTPLUGIN_DEV_ACCESS_OK)
            {
                num ++;
                card = menuitem_create();
                if (card == NULL)
                {
                    print_err("create menuitem_t failed.\n");
                    goto item_err;
                }
                if(s_str_res[1] != NULL)
                {
                    card->text_id = (int) s_str_res[1]->str_id;
                }
                card->has_child = false;
                card->callback = (item_cb_t) _proc_items_do_save_path;
                if (CAM_DSC_STORAGE_CARD == g_camera_dsc_cfgs[CAM_DSC_CFG_TYPE_SAVE_PATH].value)
                {
                    card->sel_icon_id = (int) g_cam_dsc_opt_dot->pic_id;
                    card->unsel_icon_id = (int) g_cam_dsc_opt_dot->pic_id;
                    s_save_opt_item_num = num;
                }
                else
                {
                    card->sel_icon_id = 0;
                    card->unsel_icon_id = 0;
                }

                menu_append_item(g_camera_dsc_option_save_path, card);
                menuitem_delete(card);
            }

            sys_get_hotplugin_dev_info(HOTPLUGIN_DEV_TYPE_UHOST, &external_info);
            if (external_info.access == HOTPLUGIN_DEV_ACCESS_OK)
            {
                num ++;
                external = menuitem_create();
                if (external == NULL)
                {
                    print_err("create menuitem_t failed.\n");
                    goto item_err;
                }
                if(s_str_res[2] != NULL)
                {
                    external->text_id = (int) s_str_res[2]->str_id;
                }
                external->has_child = false;
                external->callback = (item_cb_t) _proc_items_do_save_path;
                if (CAM_DSC_STORAGE_EXTERNAL == g_camera_dsc_cfgs[CAM_DSC_CFG_TYPE_SAVE_PATH].value)
                {
                    external->sel_icon_id = (int) g_cam_dsc_opt_dot->pic_id;
                    external->unsel_icon_id = (int) g_cam_dsc_opt_dot->pic_id;
                    s_save_opt_item_num = num;
                }
                else
                {
                    external->sel_icon_id = 0;
                    external->unsel_icon_id = 0;
                }
                menu_append_item(g_camera_dsc_option_save_path, external);
                menuitem_delete(external);
            }

            //if (g_camera_dsc_cfgs[CAM_DSC_CFG_TYPE_SAVE_PATH].value == HOTPLUGIN_DEV_TYPE_NONE)
            if(num == -1)
            {
                s_save_opt_item_num = -1;
                g_camera_dsc_cfgs[CAM_DSC_CFG_TYPE_SAVE_PATH].value = CAM_DSC_STORAGE_NONE;
                camera_dsc_dialog_create(CAM_DIALOG_NODISK);
            }
            else
            {
                #if 0
                if (s_save_opt_item_num == -1)
                {
                    printf("select storage moved\n");
                    if (internal_info.access == HOTPLUGIN_DEV_ACCESS_OK)
                    {
                        g_camera_dsc_cfgs[CAM_DSC_CFG_TYPE_SAVE_PATH].value =  CAM_DSC_STORAGE_INTERNAL;
                    }
                    else if (card_info.access == HOTPLUGIN_DEV_ACCESS_OK)
                    {
                        g_camera_dsc_cfgs[CAM_DSC_CFG_TYPE_SAVE_PATH].value =  CAM_DSC_STORAGE_CARD;
                    }
                    else if (external_info.access == HOTPLUGIN_DEV_ACCESS_OK)
                    {
                        g_camera_dsc_cfgs[CAM_DSC_CFG_TYPE_SAVE_PATH].value =  CAM_DSC_STORAGE_EXTERNAL;
                    }
                    s_save_opt_item_num = 0;
                    //只要不是空的就选第一个盘
                }
                tempitem = menu_get_item(g_camera_dsc_option_save_path, s_save_opt_item_num);
                if (tempitem != NULL)
                {
                    tempitem->sel_icon_id = (int) g_cam_dsc_opt_dot->pic_id;
                    tempitem->unsel_icon_id = (int) g_cam_dsc_opt_dot->pic_id;
                }
                #endif
                if (s_save_opt_item_num == -1)
                {
                    s_save_opt_item_num = 0;
                }

                printf("choice new path = %d\n",g_camera_dsc_cfgs[CAM_DSC_CFG_TYPE_SAVE_PATH].value );
                //逐个动态增加最后得到高亮号
            }
            //init current index
        }
        //空条目也无所谓
        menu_set_cur_index(g_camera_dsc_option_save_path, 0);

//        s_save_opt_item = menu_get_cur_item(g_camera_dsc_option_save_path);

    }
    return g_camera_dsc_option_save_path;

    item_err: menu_delete(g_camera_dsc_option_save_path);
    print_err("option init err\n");
    return NULL;

}

int camera_dsc_option_save_path_deinit(void)
{
    //delete menu
    if (g_camera_dsc_option_save_path != NULL)
    {
        //create menu
        menu_delete(g_camera_dsc_option_save_path);
        g_camera_dsc_option_save_path = NULL;
    }

    return true;
}
static int _proc_items_do_save_path(menuview_t *menuview, menuview_sig_e signal)
{
    menu_t *cur_menu = menuview->menu;
    menuitem_t *cur_item = menu_get_cur_item(cur_menu);
    int temp_value = -1;
    char temp_filepath[PATH_MAX] ;
    bool result = false;
    //hotplugin_dev_type_e main_disk = sys_get_main_disk();

    memset((char *)temp_filepath,0,PATH_MAX);

    switch (signal)
    {
        case WIDGET_SIG_MENUVIEW_OK:
        {
            //int cur_item_pos = menu_get_cur_index(cur_menu);
            menuitem_t *save_opt_item = NULL;
            if(menu_get_total(cur_menu) < 1)
            {
                printf("no menu item!\n");
                break;
            }

            if (s_save_opt_item_num < 0)
            {
                printf("no storage in\n");
                break;
            }

            if(cur_item != NULL)
            {
                if (cur_item->text_id == ((int) s_str_res[CAM_DSC_STORAGE_EXTERNAL-1]->str_id))
                {
                    hotplugin_dev_info_t external_info;

                    sys_get_hotplugin_dev_info(HOTPLUGIN_DEV_TYPE_UHOST, &external_info);
                    if (external_info.access != HOTPLUGIN_DEV_ACCESS_OK)
                    {
                        //不存在警告
                        // TODO:这里应该改为主盘
                        camera_dsc_dialog_create(CAM_DIALOG_OPEN_FAIL);
                        snprintf(temp_filepath, sizeof(temp_filepath), \
                            sys_get_media_dir(HOTPLUGIN_DEV_TYPE_LOCAL,DIR_PHOTO_CAMERA));
                        temp_value = CAM_DSC_STORAGE_INTERNAL;
                    }
                    else
                    {
                        snprintf(temp_filepath, sizeof(temp_filepath), \
                            sys_get_media_dir(HOTPLUGIN_DEV_TYPE_UHOST,DIR_PHOTO_CAMERA));
                        temp_value = CAM_DSC_STORAGE_EXTERNAL;
                    }
                }
                else if (cur_item->text_id == ((int) s_str_res[CAM_DSC_STORAGE_CARD-1]->str_id))
                {
                    hotplugin_dev_info_t card_info;

                    sys_get_hotplugin_dev_info(HOTPLUGIN_DEV_TYPE_CARD, &card_info);
                    if (card_info.access != HOTPLUGIN_DEV_ACCESS_OK)
                    {//不存在警告
                        camera_dsc_dialog_create(CAM_DIALOG_OPEN_FAIL);
                        snprintf(temp_filepath, sizeof(temp_filepath), \
                            sys_get_media_dir(HOTPLUGIN_DEV_TYPE_LOCAL,DIR_PHOTO_CAMERA));
                        temp_value = CAM_DSC_STORAGE_INTERNAL;
                    }
                    else
                    {
                        snprintf(temp_filepath, sizeof(temp_filepath), \
                            sys_get_media_dir(HOTPLUGIN_DEV_TYPE_CARD,DIR_PHOTO_CAMERA));
                        temp_value = CAM_DSC_STORAGE_CARD;
                    }
                }
                else
                if (cur_item->text_id == ((int) s_str_res[CAM_DSC_STORAGE_INTERNAL-1]->str_id))
                {
                    snprintf(temp_filepath, sizeof(temp_filepath), \
                        sys_get_media_dir(HOTPLUGIN_DEV_TYPE_LOCAL,DIR_PHOTO_CAMERA));
                    temp_value = CAM_DSC_STORAGE_INTERNAL;
                }
                else
                {
                }

            }

            //print_err("dsc temp path=%s", temp_filepath);
            //print_err("temp path = %d\n",temp_value);

            result = camera_dsc_check_path((hotplugin_dev_type_e)temp_value);
            if (result == true)
            {
                //memset((char *)dsc_filepath,0,PATH_MAX);
                //memcpy(dsc_filepath,temp_filepath,PATH_MAX);
                //strncpy(dsc_filepath, temp_filepath, PATH_MAX);
                g_camera_dsc_cfgs[CAM_DSC_CFG_TYPE_SAVE_PATH].value  = temp_value;
                camera_dsc_ctr_set_save_path((char *) &dsc_filepath);
                camera_dsc_ctr_get_disk_info(&(g_camera_scene_dsc->dsc_config.cfg_freeinfo));
                assert(g_camera_dsc_cfgs[CAM_DSC_CFG_TYPE_SAVE_PATH].value != CAM_DSC_STORAGE_NONE);
                //print_err("go show info path=%d\n",g_camera_dsc_cfgs[CAM_DSC_CFG_TYPE_SAVE_PATH].value);

                camera_dsc_change_icon(CAM_ICON_STORE_TYPE, \
                (unsigned int)g_camera_dsc_cfgs[CAM_DSC_CFG_TYPE_SAVE_PATH].value);
                print_dbg();
            }
            else
            {
                //如果没有准备好正确的路径不变原来的路径名
                break;
            }

            save_opt_item = menu_get_item(cur_menu, s_save_opt_item_num);
            if (save_opt_item != NULL)
            {
                save_opt_item->sel_icon_id = 0;
                save_opt_item->unsel_icon_id = 0;
            }
            if (cur_item != NULL)
            {
                cur_item->sel_icon_id = (int) g_cam_dsc_opt_dot->pic_id;
                cur_item->unsel_icon_id = (int) g_cam_dsc_opt_dot->pic_id;
                s_save_opt_item_num = menu_get_cur_index(cur_menu);

                print_dbg(">>>> menu_list: %s clicked", cur_item->text);
                // TODO:recheck s_str_res == null
            }
            else
            {
                s_save_opt_item_num = -1;
                break;
            }

#if (MENU_SELECT_OK_HIDE == 1)
            camera_dsc_widget_option_exit(menuview, camera_dsc_option_save_path_enter);
#else
            menuview_set_menu(menuview, cur_menu);
            menuview_paint(menuview);
#endif /* MENU_SELECT_OK_HIDE */
        }
        break;

        case WIDGET_SIG_MENUVIEW_QUIT:
        // return to parent menu
        camera_dsc_option_save_path_exit(menuview);
        break;

        default:
        break;
    }
    return 0;
}

bool camera_dsc_option_savepath_refresh(void)
{
    //print_err("refresh fun\n");
    if ((g_camera_scene_dsc->opt != NULL) && (g_camera_dsc_option_save_path != NULL))
    {
        menuview_t *menuview = g_camera_scene_dsc->opt;
        menu_t *cur_menu;
        //print_err("refresh fun 2\n");
        camera_dsc_option_save_path_exit(menuview);
        cur_menu = (menu_t *) camera_dsc_option_save_path_init();
        menuview_set_menu(menuview, cur_menu);
        menuview_paint(menuview);
    }
    return true;
}

