/********************************************************************************
 *                              USDK(1100)
 *                             Module: app
 *                 Copyright(c) 2003-2009 Actions Semiconductor,
 *                            All Rights Reserved.
 * File : recorder_option_mode.c
 * History:
 *      <author>    <time>           <version >             <desc>
 *    wangsheng    2009-2-25          v1.0               build this file
 ********************************************************************************/
/*!
 * \file     recorder_option_mode.c
 * \brief    recorder option
 * \author   wangsheng
 *
 * \version 1.0
 * \date  2009/02/25
 *******************************************************************************/

#include "recorder_recording_option_mode.h"
#include "recorder_scene_recording.h"
#include "recorder_recording_option.h"
#include "recorder_ui_recording.h"

static void _proc_option_mode(menuview_t *menuview, menuview_sig_e signal);

/* menu list data define */
menu_t *g_recorder_option_mode = NULL;

#define RECORDING_OPTION_MODE_MAX_ITEMS 2

menu_t* recorder_recording_option_mode_init(void)
{
    menuitem_t option_mode_item[RECORDING_OPTION_MODE_MAX_ITEMS];
    int i;
    scene_recording_t *data = g_scene_recording;
    int recorder_mode_num = 1;

    /*! load option mode resource */
    data->rec_opt_mode = (resgroup_resource_t *) get_scene_child(data->scene, OPTION_MODE_RESGROUP);
    if(NULL == data->rec_opt_mode)
    {
        return NULL;
    }
    data->rec_mic = (string_resource_t *) get_resgroup_child(data->rec_opt_mode, STR_MIC);
    if(NULL == data->rec_mic)
    {
        return NULL;
    }
    data->rec_linein = (string_resource_t *) get_resgroup_child(data->rec_opt_mode, STR_LINEIN);
    if(NULL == data->rec_linein)
    {
        return NULL;
    }

    memset(&option_mode_item, 0, sizeof(option_mode_item));

    /*! initial menu item */
    option_mode_item[0].text_id = data->rec_mic->str_id;
    /*line_in support*/
    if(1 == g_scene_recording->line_in_support)
    {
        option_mode_item[1].text_id = data->rec_linein->str_id;
        recorder_mode_num++;
    }
    else
    {
        /*如果不支持持line_in模式，则录音模式只能为mic*/
        data->mode = MIC;
    }

    for (i = 0; i < recorder_mode_num; i++)
    {
        option_mode_item[i].callback = (item_cb_t) _proc_option_mode;
    }

    if (data->mode == MIC)
    {
        option_mode_item[0].sel_icon_id = data->rec_select->pic_id;
        option_mode_item[0].unsel_icon_id = data->rec_select->pic_id;
    }
    else
    {
        option_mode_item[1].sel_icon_id = data->rec_select->pic_id;
        option_mode_item[1].unsel_icon_id = data->rec_select->pic_id;
    }

    /*initial menu*/
    if (g_recorder_option_mode == NULL)
    {
        /*create menu*/
        g_recorder_option_mode = menu_create();
        if (g_recorder_option_mode == NULL)
        {
            print_err("can not create option mode");
            return false;
        }
        else
        {
            /* append items */
            menu_append_items(g_recorder_option_mode, option_mode_item, recorder_mode_num);
            /* set current index */
            menu_set_cur_index(g_recorder_option_mode, 0);
        }
    }

    return g_recorder_option_mode;
}

int recorder_recording_option_mode_deinit(void)
{
    /*delete menu*/
    if (g_recorder_option_mode != NULL)
    {
        /*create menu*/
        menu_delete(g_recorder_option_mode);
        g_recorder_option_mode = NULL;
    }

    return true;
}

bool recorder_recording_option_mode_enter(menuview_t *menuview)
{
    menu_t *cur_menu;

    cur_menu = recorder_recording_option_mode_init();
    menuview_set_menu(menuview, cur_menu);
    menuview_paint(menuview);

    return true;
}

bool recorder_recording_option_mode_exit(menuview_t *menuview)
{
    fusion_type_e fenter;

    fenter = FUSION_BACKWARD;

    /*delete main menu*/
    recorder_recording_option_mode_deinit();

    /*enter parent menu*/
    recorder_recording_option_enter(menuview, fenter);

    return true;
}

static void _proc_option_mode(menuview_t *menuview, menuview_sig_e signal)
{
    menu_t *cur_menu = menuview->menu;
    menuitem_t *item;
    int local_index = cur_menu->current;

    switch (signal)
    {
        case WIDGET_SIG_MENUVIEW_OK:
        /*只有录音模式有切换时才执行操作*/
        if (g_scene_recording->mode != local_index)
        {
            item = menu_get_item(cur_menu, g_scene_recording->mode);
            if (item == NULL)
            {
                print_err("menu_get_item err");
                return;
            }
            item->sel_icon_id = 0;
            item->unsel_icon_id = 0;

            g_scene_recording->mode = local_index;
            /*执行模式切换之前强制关闭监听*/
            stop_monitor(1);
            /*设置录音参数*/
            set_param();

            /*根据录音模式打开监听*/
            if (false == start_monitor(g_scene_recording->mode))
            {
                print_warning("start monitor err");
            }

            item = menu_get_item(cur_menu, local_index);
            if (item == NULL)
            {
                print_err("menu_get_item err");
                return;
            }
            item->sel_icon_id = g_scene_recording->rec_select->pic_id;
            item->unsel_icon_id = g_scene_recording->rec_select->pic_id;

            /*  line_in 时在standby状态需要显示能量条，其他时候在Recording时显示能量条 */
//            if (LINE_IN == g_scene_recording->mode)
//            {
//                g_scene_recording->vol_energe_timer_id = set_timer(500, (timer_proc) recorder_draw_vol,
//                        (void *) g_scene_recording);//set timer
//            }
//            else
//            {
//                /* kill timers */
//                if (g_scene_recording->vol_energe_timer_id != -1)
//                {
//                    kill_timer(g_scene_recording->vol_energe_timer_id);
//                    g_scene_recording->vol_energe_timer_id = -1;
//                }
//                recorder_draw_vol(g_scene_recording);
//            }

            menuview_paint(menuview);
        }

        break;

        case WIDGET_SIG_MENUVIEW_QUIT:
        /* return to parent menu*/
        recorder_recording_option_mode_exit(menuview);
        break;

        default:
        break;
    }
}

