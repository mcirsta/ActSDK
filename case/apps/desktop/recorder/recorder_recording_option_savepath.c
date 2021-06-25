/********************************************************************************
 *                              USDK(1100)
 *                             Module: app
 *                 Copyright(c) 2003-2009 Actions Semiconductor,
 *                            All Rights Reserved.
 * File : recorder_option_save_path.c
 * History:
 *      <author>    <time>           <version >             <desc>
 *    wangsheng    2009-2-25          v1.0               build this file
 ********************************************************************************/
/*!
 * \file     recorder_option_save_path.c
 * \brief    recorder option
 * \author   wangsheng
 *
 * \version 1.0
 * \date  2009/02/25
 *******************************************************************************/

#include "recorder_recording_option_savepath.h"
#include "recorder_ui_recording.h"
#include "recorder_recording_option.h"

static void _proc_option_savepath(menuview_t *menuview, menuview_sig_e signal);

/* menu list data define */
menu_t *g_recorder_option_savepath = NULL;

#define RECORDING_OPTION_SAVEPATH_MAX_ITEMS 3

static bool card_in_status = false;

int path_num = 0;

menu_t* recorder_recording_option_savepath_init(void)
{
    menuitem_t option_savepath_item[RECORDING_OPTION_SAVEPATH_MAX_ITEMS];
    int i;

    scene_recording_t *data = g_scene_recording;
    hotplugin_dev_info_t card_info;
    hotplugin_dev_info_t external_info;

    dialog_init_t dialog_init;

    path_num = 0;

    /*! load resource */
    data->rec_opt_savepath = (resgroup_resource_t *) get_scene_child(data->scene, OPTION_SAVEPATH_RESGROUP);
    if(NULL == data->rec_opt_savepath)
    {
        return NULL;
    }
    data->rec_internal = (string_resource_t *) get_resgroup_child(data->rec_opt_savepath, STR_INTERNAL);
    if(NULL == data->rec_internal)
    {
        return NULL;
    }
    data->rec_card = (string_resource_t *) get_resgroup_child(data->rec_opt_savepath, STR_CARD);
    if(NULL == data->rec_card)
    {
        return NULL;
    }
    data->rec_external = (string_resource_t *) get_resgroup_child(data->rec_opt_savepath, STR_EXTERNAL);
    if(NULL == data->rec_external)
    {
        return NULL;
    }

    memset(&option_savepath_item, 0, sizeof(option_savepath_item));

#if 0
    /*! initial menu item */
    option_savepath_item[0].text_id = data->rec_internal->str_id;
    option_savepath_item[1].text_id = data->rec_card->str_id;
    option_savepath_item[2].text_id = data->rec_external->str_id;

    for(i = 0; i < RECORDING_OPTION_SAVEPATH_MAX_ITEMS; i++)
    {
        option_savepath_item[i].callback = (item_cb_t)_proc_option_savepath;
    }

    //initial menu
    if( g_recorder_option_savepath == NULL )
    {
        //create menu
        g_recorder_option_savepath = menu_create();
        if(g_recorder_option_savepath == NULL)
        {
            print_err("can not create option save path");
            return false;
        }
        else
        {
            /* append items */
            menu_append_items(g_recorder_option_savepath, option_savepath_item, RECORDING_OPTION_SAVEPATH_MAX_ITEMS);
            /* set current index */
            menu_set_cur_index( g_recorder_option_savepath, 0 );
        }
    }
#endif
    /*先更新卡和外部存储器状态*/
    sys_get_hotplugin_dev_info(HOTPLUGIN_DEV_TYPE_CARD, &card_info);
    sys_get_hotplugin_dev_info(HOTPLUGIN_DEV_TYPE_UHOST, &external_info);

    /*主盘为本地磁盘，则本地磁盘一定存在*/
    if(HOTPLUGIN_DEV_TYPE_LOCAL == g_scene_recording->main_disk)
    {
        option_savepath_item[0].text_id = data->rec_internal->str_id;
        path_num++;
    }

    if (card_info.access == HOTPLUGIN_DEV_ACCESS_OK)
    {
        option_savepath_item[path_num].text_id = data->rec_card->str_id;
        path_num++;
        card_in_status = true;
    }
    else
    {
        card_in_status = false;
    }

    if (external_info.access == HOTPLUGIN_DEV_ACCESS_OK)
    {
        option_savepath_item[path_num].text_id = data->rec_external->str_id;
        path_num++;
    }

    if(0 == path_num)
    {
        /*提示无磁盘*/
        if( NULL != data->rec_dialog_nodisk)
        {
            /* initial dialog data*/
            dialog_init.head = DIALOG_HEAD_WARN;
            dialog_init.str_id = data->rec_dialog_nodisk->str_id;

            dialog_modal(&dialog_init);
        }
    }

    if(HOTPLUGIN_DEV_TYPE_LOCAL == g_scene_recording->main_disk)/*本地存储器存在*/
    {
        if (data->path == INTERNAL_MEMORY)
        {
            option_savepath_item[0].sel_icon_id = data->rec_select->pic_id;
            option_savepath_item[0].unsel_icon_id = data->rec_select->pic_id;
        }
        else if (data->path == CARD_MEMORY)
        {
            if ((card_info.access == HOTPLUGIN_DEV_ACCESS_OK))
            {
                option_savepath_item[1].sel_icon_id = data->rec_select->pic_id;
                option_savepath_item[1].unsel_icon_id = data->rec_select->pic_id;
            }
            else
            {
                /*路径为卡，卡不在则设置到内部存储器*/
                option_savepath_item[0].sel_icon_id = data->rec_select->pic_id;
                option_savepath_item[0].unsel_icon_id = data->rec_select->pic_id;

                data->path = INTERNAL_MEMORY;
            }
        }
        else
        {
            if ((card_info.access == HOTPLUGIN_DEV_ACCESS_OK) && (external_info.access == HOTPLUGIN_DEV_ACCESS_OK))
            {
                option_savepath_item[2].sel_icon_id = data->rec_select->pic_id;
                option_savepath_item[2].unsel_icon_id = data->rec_select->pic_id;
            }
            else if((card_info.access != HOTPLUGIN_DEV_ACCESS_OK) && (external_info.access == HOTPLUGIN_DEV_ACCESS_OK))
            {
                option_savepath_item[1].sel_icon_id = data->rec_select->pic_id;
                option_savepath_item[1].unsel_icon_id = data->rec_select->pic_id;
            }
            else
            {
                /*路径为外部存储器，外部存储器不在则设置到内部存储器*/
                option_savepath_item[0].sel_icon_id = data->rec_select->pic_id;
                option_savepath_item[0].unsel_icon_id = data->rec_select->pic_id;

                data->path = INTERNAL_MEMORY;
            }
        }
    }
    else/*本地存储器不存在*/
    {
        if (data->path == INTERNAL_MEMORY)
        {
            data->path = CARD_MEMORY;
        }

        if (data->path == CARD_MEMORY)
        {
            if (card_info.access == HOTPLUGIN_DEV_ACCESS_OK)
            {
                option_savepath_item[0].sel_icon_id = data->rec_select->pic_id;
                option_savepath_item[0].unsel_icon_id = data->rec_select->pic_id;
            }
            else if( external_info.access == HOTPLUGIN_DEV_ACCESS_OK)
            {
                option_savepath_item[0].sel_icon_id = data->rec_select->pic_id;
                option_savepath_item[0].unsel_icon_id = data->rec_select->pic_id;
                /*路径为卡，卡不在则设置到外部存储器*/
                data->path = EXTERNAL_MEMORY;
            }
            else
            {
                print_info("no disk avriable");
            }
        }
        else if(data->path == EXTERNAL_MEMORY)
        {
            if ((card_info.access == HOTPLUGIN_DEV_ACCESS_OK) && (external_info.access == HOTPLUGIN_DEV_ACCESS_OK))
            {
                option_savepath_item[1].sel_icon_id = data->rec_select->pic_id;
                option_savepath_item[1].unsel_icon_id = data->rec_select->pic_id;
            }
            else
            {
                option_savepath_item[0].sel_icon_id = data->rec_select->pic_id;
                option_savepath_item[0].unsel_icon_id = data->rec_select->pic_id;

                if((external_info.access != HOTPLUGIN_DEV_ACCESS_OK) && (card_info.access == HOTPLUGIN_DEV_ACCESS_OK))
                {
                    /*路径为外部存储器，外部存储器不在则设置到卡*/
                    data->path = CARD_MEMORY;
                }
                else
                {
                    print_info("no disk avriable");
                }
            }
        }
        else
        {

        }
    }

    for (i = 0; i < path_num; i++)
    {
        option_savepath_item[i].callback = (item_cb_t) _proc_option_savepath;
    }

    /*initial menu*/
    if (g_recorder_option_savepath == NULL)
    {
        /*create menu*/
        g_recorder_option_savepath = menu_create();
        if (g_recorder_option_savepath == NULL)
        {
            print_err("can not create option save path");
            return false;
        }
        else
        {
            /* append items */
            menu_append_items(g_recorder_option_savepath, option_savepath_item, path_num);
            /* set current index */
            menu_set_cur_index(g_recorder_option_savepath, 0);
        }

        g_recorder_option_savepath->callback = (menu_cb_t)_proc_option_savepath;

    }

    return g_recorder_option_savepath;
}

int recorder_recording_option_savepath_deinit(void)
{
    /*delete menu*/
    if (g_recorder_option_savepath != NULL)
    {
        /*create menu*/
        menu_delete(g_recorder_option_savepath);
        g_recorder_option_savepath = NULL;
    }

    return true;
}

bool recorder_recording_option_savepath_enter(menuview_t *menuview)
{
    menu_t *cur_menu;

    cur_menu = recorder_recording_option_savepath_init();
    if(false == cur_menu)
    {
        return false;
    }
    menuview_set_menu(menuview, cur_menu);
    menuview_paint(menuview);

    return true;
}

bool recorder_recording_option_savepath_exit(menuview_t *menuview)
{
    fusion_type_e fenter;

    fenter = FUSION_BACKWARD;

    /*delete main menu*/
    recorder_recording_option_savepath_deinit();

    /*enter parent menu*/
    recorder_recording_option_enter(menuview, fenter);

    return true;
}

/*根据菜单索引获取对应的存储路径*/
/*外部存储器对应的存储路径为2，卡为1，内置存储器为0*/

recorder_save_path_e _index_to_path(int local_index)
{
    recorder_save_path_e recorder_path;
    /*没有卡插入时外部存储器对应的菜单选项为1，需要重新设置路径为2*/
    if( HOTPLUGIN_DEV_TYPE_LOCAL == g_scene_recording->main_disk )
    {
        recorder_path = local_index;
        if((card_in_status == false) && (1 == local_index))
        {
            recorder_path = EXTERNAL_MEMORY;
        }
    }
    else
    {
        /*本地磁盘不在，则local_index不再直接对应存储器宏定义*/
        recorder_path = local_index + 1;

        if(card_in_status == false)
        {
            recorder_path = EXTERNAL_MEMORY;
        }
    }
    return recorder_path;
}


/*根据存储路径获取对应于菜单上的索引*/
int _path_to_index(recorder_save_path_e recorder_path)
{
    int i_local_index;
    if( HOTPLUGIN_DEV_TYPE_LOCAL == g_scene_recording->main_disk )
    {
        if(INTERNAL_MEMORY == recorder_path)
        {
            i_local_index = 0;
        }
        else if((CARD_MEMORY == recorder_path))
        {
            i_local_index = 1;
        }
        else if(EXTERNAL_MEMORY == recorder_path)
        {

            if(card_in_status == false)
            {
                i_local_index = 1;
            }
            else
            {
                i_local_index = 2;
            }
        }
        else
        {
            i_local_index = 0;
        }
    }
    else
    {

        if((CARD_MEMORY == recorder_path))
        {
            i_local_index = 0;
        }
        else if(EXTERNAL_MEMORY == recorder_path)
        {
            if(card_in_status == false)
            {
                i_local_index = 0;
            }
            else
            {
                i_local_index = 1;
            }
        }
        else
        {
            i_local_index = 0;
        }
    }
    return i_local_index;
}

/*有移动存储器插入拔出，需要更新路径列表*/
void update_savepath(void)
{
    if(NULL != g_recorder_option_savepath)
    {
        recorder_recording_option_savepath_deinit();
        recorder_recording_option_savepath_enter(g_scene_recording->option);
        /*立刻设置参数，以便获取磁盘空间信息，刷新可录时间*/
        if (0 == set_path())
        {
            print_warning("set path err");
        }
        /*可录时间需要刷新*/
        g_scene_recording->totaltime = NEED_PAINT;
        recorder_draw_totaltime(g_scene_recording);
    }
}


static void _proc_option_savepath(menuview_t *menuview, menuview_sig_e signal)
{
    menu_t *cur_menu = menuview->menu;
    int local_index = cur_menu->current;
    menuitem_t *item;

    recorder_save_path_e tmp_path;

    switch (signal)
    {
        case WIDGET_SIG_MENUVIEW_OK:
        /*获取当前选中的菜单索引值对应的存储路径*/
        tmp_path = _index_to_path(local_index);
        
        /*当前选中的路径和以前路径不同时才执行操作*/
        if (g_scene_recording->path != tmp_path)
        {
            print_dbg("\n***g_scene_recording->path=%d,local_index=%d,tmp_path=%d\n",
                g_scene_recording->path,local_index,tmp_path);
            /*获取上次路径对应当前菜单的索引号,以便清除选中标志*/
            item = menu_get_item(cur_menu, _path_to_index(g_scene_recording->path));
            if (item == NULL)
            {
                print_err("menu_get_item err");
                return;
            }
            item->sel_icon_id = 0;
            item->unsel_icon_id = 0;

            item = menu_get_item(cur_menu, local_index);
            if (item == NULL)
            {
                print_err("menu_get_item err");
                return;
            }
            item->sel_icon_id = g_scene_recording->rec_select->pic_id;
            item->unsel_icon_id = g_scene_recording->rec_select->pic_id;
            /*设置当前选中的路径*/
            g_scene_recording->path = tmp_path;
            /*设置录音参数，以便获取磁盘空间，刷新可录时间*/
            if (0 == set_path())
            {
                print_warning("set path err");
            }
            /*可录时间需要刷新*/
            g_scene_recording->totaltime = NEED_PAINT;
            recorder_draw_totaltime(g_scene_recording);

            menuview_paint(menuview);
        }

        break;

        case WIDGET_SIG_MENUVIEW_QUIT:
        /* return to parent menu*/
        recorder_recording_option_savepath_exit(menuview);
        break;

        default:
        break;
    }
}

