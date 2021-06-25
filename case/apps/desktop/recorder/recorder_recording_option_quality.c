/********************************************************************************
 *                              USDK(1100)
 *                             Module: app
 *                 Copyright(c) 2003-2009 Actions Semiconductor,
 *                            All Rights Reserved.
 * File : recorder_option_quality.c
 * History:
 *      <author>    <time>           <version >             <desc>
 *    wangsheng    2009-2-25          v1.0               build this file
 ********************************************************************************/
/*!
 * \file     listmenu.c
 * \brief    recorder option
 * \author   wangsheng
 *
 * \version 1.0
 * \date  2009/02/25
 *******************************************************************************/

#include "recorder_recording_option_quality.h"
#include "recorder_ui_recording.h"
#include "recorder_recording_option.h"

static void _proc_option_quality(menuview_t *menuview, menuview_sig_e signal);

/* menu list data define */
menu_t *g_recorder_option_quality = NULL;

#define RECORDING_OPTION_QUALITY_MAX_ITEMS 3

menu_t* recorder_recording_option_quality_init(void)
{
    menuitem_t option_quality_item[RECORDING_OPTION_QUALITY_MAX_ITEMS];
    int i;
    scene_recording_t *data = g_scene_recording;

    /*! load resource */
    data->rec_opt_quality = (resgroup_resource_t *) get_scene_child(data->scene, OPTION_QUALITY_RESGROUP);
    if(NULL == data->rec_opt_quality)
    {
        return NULL;
    }
    data->rec_low = (string_resource_t *) get_resgroup_child(data->rec_opt_quality, STR_LOW);
    if(NULL == data->rec_low)
    {
        return NULL;
    }
    data->rec_medium = (string_resource_t *) get_resgroup_child(data->rec_opt_quality, STR_MEDIUM);
    if(NULL == data->rec_medium)
    {
        return NULL;
    }
    data->rec_high = (string_resource_t *) get_resgroup_child(data->rec_opt_quality, STR_HIGH);
    if(NULL == data->rec_high)
    {
        return NULL;
    }

    memset(&option_quality_item, 0, sizeof(option_quality_item));

    /*! initial menu item */
    option_quality_item[0].text_id = data->rec_low->str_id;
    option_quality_item[1].text_id = data->rec_medium->str_id;
    option_quality_item[2].text_id = data->rec_high->str_id;

    option_quality_item[data->quality].sel_icon_id = g_scene_recording->rec_select->pic_id;
    option_quality_item[data->quality].unsel_icon_id = g_scene_recording->rec_select->pic_id;

    for (i = 0; i < RECORDING_OPTION_QUALITY_MAX_ITEMS; i++)
    {
        option_quality_item[i].callback = (item_cb_t) _proc_option_quality;
    }

    /*initial menu*/
    if (g_recorder_option_quality == NULL)
    {
        /*create menu*/
        g_recorder_option_quality = menu_create();
        if (g_recorder_option_quality == NULL)
        {
            print_err("can not create option quality");
            return false;
        }
        else
        {
            /* append items */
            menu_append_items(g_recorder_option_quality, option_quality_item, RECORDING_OPTION_QUALITY_MAX_ITEMS);
            /* set current index */
            menu_set_cur_index(g_recorder_option_quality, 0);
        }
    }

    return g_recorder_option_quality;
}

int recorder_recording_option_quality_deinit(void)
{
    /*delete menu*/
    if (g_recorder_option_quality != NULL)
    {
        /*create menu*/
        menu_delete(g_recorder_option_quality);
        g_recorder_option_quality = NULL;
    }

    return true;
}

bool recorder_recording_option_quality_enter(menuview_t *menuview)
{
    menu_t *cur_menu;

    cur_menu = recorder_recording_option_quality_init();
    menuview_set_menu(menuview, cur_menu);
    menuview_paint(menuview);

    return true;
}

bool recorder_recording_option_quality_exit(menuview_t *menuview)
{
    fusion_type_e fenter;

    fenter = FUSION_BACKWARD;

    /*delete main menu*/
    recorder_recording_option_quality_deinit();

    /*enter parent menu*/
    recorder_recording_option_enter(menuview, fenter);

    return true;
}

static void _proc_option_quality(menuview_t *menuview, menuview_sig_e signal)
{
    menu_t *cur_menu = menuview->menu;
    menuitem_t *item;
    int local_index = cur_menu->current;

    switch (signal)
    {
        case WIDGET_SIG_MENUVIEW_OK:
        /*只有录音质量有切换时才执行操作*/
        if (g_scene_recording->quality != local_index)
        {
            item = menu_get_item(cur_menu, g_scene_recording->quality);
            if (item == NULL)
            {
                print_err("menu_get_item err");
                return;
            }
            item->sel_icon_id = 0;
            item->unsel_icon_id = 0;

            g_scene_recording->quality = local_index;

            item = menu_get_item(cur_menu, g_scene_recording->quality);
            if (item == NULL)
            {
                print_err("menu_get_item err");
                return;
            }
            item->sel_icon_id = g_scene_recording->rec_select->pic_id;
            item->unsel_icon_id = g_scene_recording->rec_select->pic_id;

            /*设置参数*/
            if (false == set_param())
            {
                print_warning("set rec param err");
            }
            /*录音参数有变化，可录时间需更新*/
            g_scene_recording->totaltime = NEED_PAINT;
            recorder_draw_totaltime(g_scene_recording);

            menuview_paint(menuview);
        }
        break;

        case WIDGET_SIG_MENUVIEW_QUIT:
        /* return to parent menu*/
        recorder_recording_option_quality_exit(menuview);
        break;

        default:
        break;
    }
}
