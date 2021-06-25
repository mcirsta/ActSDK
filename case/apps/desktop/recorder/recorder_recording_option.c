/********************************************************************************
 *                              USDK(1100)
 *                             Module: app
 *                 Copyright(c) 2003-2009 Actions Semiconductor,
 *                            All Rights Reserved.
 * File : recorder_option.c
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

#include "recorder_recording_option.h"
#include "recorder_scene_recording.h"

#include "recorder_recording_option_fmt.h"
#include "recorder_recording_option_quality.h"
#include "recorder_recording_option_mode.h"
#include "recorder_recording_option_savepath.h"

/* option menu process */
static void _adjust_callback(adjust_t *adjust, adjust_signal_e signal);
static void _proc_item_fmt(menuview_t *data, menuview_sig_e signal);
static void _proc_item_quality(menuview_t *data, menuview_sig_e signal);
static void _proc_item_gain(menuview_t *data, menuview_sig_e signal);
static void _proc_item_mode(menuview_t *data, menuview_sig_e signal);
static void _proc_item_savepath(menuview_t *data, menuview_sig_e signal);

/* menu list data define */
menu_t *g_recorder_option = NULL;

/* menu max items */
#define RECORDING_OPTION_MAX_ITEMS   5

/*
 * Option菜单特效第一帧
 */
static void _option_fusion_set_first_frame(menuview_t *mv)
{
    fusion_frame_t frame;
    canvas_info_t info;

    canvas_get_info(mv->canvas_id, &info);

    frame.rect.x = info.rect.x;
    frame.rect.y = info.rect.y;
    frame.rect.width = info.rect.width;
    frame.rect.height = info.rect.height;
    frame.pdata = NULL;
    frame.alpha = false;
    fusion_set_first_frame(&frame);
}

/*
 * Option菜单特效最后一帧
 */
static void _option_fusion_set_last_frame(menuview_t *mv)
{
    fusion_frame_t frame;
    canvas_info_t info;

    canvas_get_info(mv->canvas_id, &info);

    frame.rect.x = info.rect.x;
    frame.rect.y = info.rect.y;
    frame.rect.width = info.rect.width;
    frame.rect.height = info.rect.height;
    frame.pdata = (unsigned char *) info.pdata;
    frame.alpha = true;
    fusion_set_last_frame(&frame);
}

static void _adjust_callback(adjust_t *adjust, adjust_signal_e signal)
{
    int locan_index;

    switch (signal)
    {
        case WIDGET_SIG_ADJUST_OK:
        adjust_get_current_index(&locan_index);
        g_scene_recording->gain = locan_index;

        if (g_scene_recording->adjust != NULL)
        {
            adjust_delete(g_scene_recording->adjust);
            g_scene_recording->adjust = NULL;
        }
        break;
        case WIDGET_SIG_ADJUST_CANCEL:
        if (g_scene_recording->adjust != NULL)
        {
            adjust_delete(g_scene_recording->adjust);
            g_scene_recording->adjust = NULL;
        }
        break;

        case WIDGET_SIG_ADJUST_SWITCHON:
        print_dbg("WIDGET_SIG_ADJUST_SWITCHON\n");
        break;

        case WIDGET_SIG_ADJUST_SWITCHOFF:
        print_dbg("WIDGET_SIG_ADJUST_SWITCHOFF\n");
        break;

        case WIDGET_SIG_ADJUST_UP:
        print_dbg("WIDGET_SIG_ADJUST_UP\n");
        break;

        case WIDGET_SIG_ADJUST_DOWN:
        print_dbg("WIDGET_SIG_ADJUST_DOWN\n");
        break;

        default:
        break;
    }
}

menu_t* recorder_recording_option_init(void)
{
    /* menu items data define */
    menuitem_t option_item[RECORDING_OPTION_MAX_ITEMS];
    scene_recording_t *data = g_scene_recording;

    /*! load option resource */
    data->rec_opt_main = (resgroup_resource_t *) get_scene_child(data->scene, OPTION_RESGROUP);
    data->rec_fmt = (string_resource_t *) get_resgroup_child(data->rec_opt_main, STR_FORMAT);
    if(NULL == data->rec_fmt)
    {
        return NULL;
    }
    data->rec_quality = (string_resource_t *) get_resgroup_child(data->rec_opt_main, STR_QUALITY);
    if(NULL == data->rec_quality)
    {
        return NULL;
    }
    data->rec_gain = (string_resource_t *) get_resgroup_child(data->rec_opt_main, STR_GAIN);
    if(NULL == data->rec_gain)
    {
        return NULL;
    }
    data->rec_mode = (string_resource_t *) get_resgroup_child(data->rec_opt_main, STR_MODE);
    if(NULL == data->rec_mode)
    {
        return NULL;
    }
    data->rec_savepath = (string_resource_t *) get_resgroup_child(data->rec_opt_main, STR_SAVEPATH);
    if(NULL == data->rec_savepath)
    {
        return NULL;
    }

    memset(&option_item, 0, sizeof(option_item));

    /*! initial menu item */
    option_item[0].text_id = data->rec_fmt->str_id;
    option_item[0].callback = (item_cb_t) _proc_item_fmt;
    option_item[0].has_child = true;

    option_item[1].text_id = data->rec_quality->str_id;
    option_item[1].callback = (item_cb_t) _proc_item_quality;
    option_item[1].has_child = true;

    option_item[2].text_id = data->rec_gain->str_id;
    option_item[2].callback = (item_cb_t) _proc_item_gain;
    option_item[2].has_child = true;

    option_item[3].text_id = data->rec_mode->str_id;
    option_item[3].callback = (item_cb_t) _proc_item_mode;
    option_item[3].has_child = true;

    option_item[4].text_id = data->rec_savepath->str_id;
    option_item[4].callback = (item_cb_t) _proc_item_savepath;
    option_item[4].has_child = true;

    /*initial menu*/
    if (g_recorder_option == NULL)
    {
        /*create menu*/
        g_recorder_option = menu_create();

        if (g_recorder_option == NULL)
        {
            print_err("can not create recorder option");
            return false;
        }
        else
        {
            /* append items */
            menu_append_items(g_recorder_option, option_item, RECORDING_OPTION_MAX_ITEMS);
            /* init curent index */
            menu_set_cur_index(g_recorder_option, 0);
        }
    }
    return g_recorder_option;
}

int recorder_recording_option_deinit(void)
{
    /*delete menu*/
    if (g_recorder_option != NULL)
    {
        /*create menu*/
        menu_delete(g_recorder_option);
        g_recorder_option = NULL;
    }

    return true;
}

/* 进入主菜单 */
bool recorder_recording_option_enter(menuview_t *menuview, fusion_type_e fenter)
{
    menu_t *cur_menu;
    fusion_effect_t eff;

    cur_menu = recorder_recording_option_init();
    menuview_set_menu(menuview, cur_menu);

    if (fenter == FUSION_FORWARD)
    {
        /*设置option特效*/
        fusion_get_config(FUSION_TYPE_OPTION_RGB, fenter, &eff); 

        fusion_set_effect(&eff);

        _option_fusion_set_first_frame(menuview);

    }

    menuview_paint(menuview);
    if (fenter == FUSION_FORWARD)

    {
        _option_fusion_set_last_frame(menuview);

        fusion_display(NULL);
    }

    return true;
}

/* 退出主菜单 */
bool recorder_recording_option_exit(menuview_t *menuview)
{
    /*delete main menu*/
    recorder_recording_option_deinit();

    /*exit option_ui*/
    recorder_recording_widget_option_exit(menuview, NULL);

    return true;
}

/* option menu process */
static void _proc_item_fmt(menuview_t *data, menuview_sig_e signal)
{
    switch (signal)
    {
        case WIDGET_SIG_MENUVIEW_OK:
        /*! enter format menu */
        recorder_recording_option_fmt_enter(data);
        break;

        case WIDGET_SIG_MENUVIEW_QUIT:
        /* return to parent menu*/
        recorder_recording_option_exit(data);
        break;

        default:
        break;
    }
}

static void _proc_item_quality(menuview_t *data, menuview_sig_e signal)
{
    switch (signal)
    {
        case WIDGET_SIG_MENUVIEW_OK:
        /*! enter format menu */
        recorder_recording_option_quality_enter(data);
        break;

        case WIDGET_SIG_MENUVIEW_QUIT:
        /* return to parent menu*/
        recorder_recording_option_exit(data);
        break;

        default:
        break;
    }
}

static void _proc_item_gain(menuview_t *data, menuview_sig_e signal)
{
    adjust_textdigit_init_t adj_textdigit;

    switch (signal)
    {
        case WIDGET_SIG_MENUVIEW_OK:
        /* load adjust resource */
        g_scene_recording->rec_adjust = (resgroup_resource_t *) get_scene_child(g_scene_recording->scene,
                WIDGET_ADJUST_RESGROUP);

        g_scene_recording->rec_adjust_str = (string_resource_t *) get_resgroup_child(g_scene_recording->rec_adjust,
                STR_ADJUST);

        if( (NULL == g_scene_recording->rec_adjust)
            ||(NULL == g_scene_recording->rec_adjust_str))
        {
            return;
        }
        /* initial adjust data */
        memset(&adj_textdigit, 0, sizeof(adjust_textdigit_init_t));
        adj_textdigit.cur_val = g_scene_recording->gain;
        adj_textdigit.min_val = 1;
        adj_textdigit.max_val = 5;
        adj_textdigit.step = 1;
        adj_textdigit.str_title_id = g_scene_recording->rec_adjust_str->str_id;

        //g_scene_recording->adjust->adjust_digit = adj_textdigit;
        /* adjust init and callback function */
        g_scene_recording->adjust = adjust_create(&adj_textdigit, ADJ_STYLE_TEXTDIGIT, _adjust_callback);
        if (NULL == g_scene_recording->adjust)
        {
            print_warning("adjust create failed");
        }

        break;

        case WIDGET_SIG_MENUVIEW_QUIT:
        /* return to parent menu*/
        recorder_recording_option_exit(data);
        break;

        default:
        break;
    }
}

static void _proc_item_mode(menuview_t *data, menuview_sig_e signal)
{
    switch (signal)
    {
        case WIDGET_SIG_MENUVIEW_OK:
        /*! enter format menu */
        recorder_recording_option_mode_enter(data);
        break;

        case WIDGET_SIG_MENUVIEW_QUIT:
        /* return to parent menu*/
        recorder_recording_option_exit(data);
        break;

        default:
        break;
    }
}

static void _proc_item_savepath(menuview_t *data, menuview_sig_e signal)
{
    switch (signal)
    {
        case WIDGET_SIG_MENUVIEW_OK:
        /*! enter format menu */
        recorder_recording_option_savepath_enter(data);
        break;

        case WIDGET_SIG_MENUVIEW_QUIT:
        /* return to parent menu*/
        recorder_recording_option_exit(data);
        break;

        default:
        break;
    }
}
