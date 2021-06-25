/********************************************************************************
 *                              USDK(1100)
 *                             Module: app
 *                 Copyright(c) 2003-2009 Actions Semiconductor,
 *                            All Rights Reserved.
 * File : radio_option_level.c
 * History:
 *      <author>    <time>           <version >             <desc>
 *      cheason    2009-04-23          v1.0               build this file
 ********************************************************************************/
/*!
 * \file     radio_option_level.c
 * \brief    FM播放界面场景下的option菜单调节录音质量等级、灵敏度等级相关操作
 * \author   cheason
 *
 * \version 1.0
 * \date  2009/04/23
 *******************************************************************************/

#include "radio_playing_option.h"
#include "radio_playing_option_level.h"
#include "radio_scene_playing.h"

static res_option_level_t s_res_option_level =
{ NULL };
static menu_t *s_menu_option_level = NULL;
static option_level_type_e s_type;

static void _proc_item_click(menu_t *menu_option, radio_scene_playing_t *playing);
static void _option_level_cb(menuview_t *menuview, menuview_sig_e signal);
static void _add_item(menu_t *menu_data, menuitem_t *item, string_resource_t *str, bool sub_menu_flag);
static bool _fill_items(menu_t *menu_data, radio_scene_playing_t *playing);

/******************************************************************************/
/*!                    
 * \par  Description:     item click process
*******************************************************************************/
static void _proc_item_click(menu_t *menu_option, radio_scene_playing_t *playing)
{
    switch (s_type)
    {
        case LTYPE_SENSITIVITY:
        switch (menu_option->current)
        {
            case LEVEL_LOW:
            playing->cur_threthod = SEN_LEVEL_LOW;
            break;
            case LEVEL_MEDIUM:
            playing->cur_threthod = SEN_LEVEL_MEDIUM;
            break;
            case LEVEL_HIGH:
            playing->cur_threthod = SEN_LEVEL_HIGH;
            break;
            default:
            break;
        }
        fm_engine_msg(MSG_FMENGINE_SET_THRESHOLD, &playing->cur_threthod, sizeof(int), NULL);
        break;

        case LTYPE_REC_QUALITY:
        switch (menu_option->current)
        {
            case LEVEL_LOW:
            playing->rec_quality = 0;
            break;
            case LEVEL_MEDIUM:
            playing->rec_quality = 1;
            break;
            case LEVEL_HIGH:
            playing->rec_quality = 2;
            break;
            default:
            break;
        }
        break;

        default:
        break;
    }
}

/******************************************************************************/
/*!                    
 * \par  Description:     level callback
*******************************************************************************/
static void _option_level_cb(menuview_t *menuview, menuview_sig_e signal)
{
    menu_t *cur_menu = menuview->menu;
    menuitem_t *item;
    int lo_index;

    switch (signal)
    {
        case WIDGET_SIG_MENUVIEW_OK:
        if (s_type == LTYPE_SENSITIVITY)
        {
            if (g_scene_playing->cur_threthod == SEN_LEVEL_LOW)
            {
                lo_index = 0;
            }
            else if (g_scene_playing->cur_threthod == SEN_LEVEL_MEDIUM)
            {
                lo_index = 1;
            }
            else
            {
                lo_index = 2;
            }
        }
        else
        {
            lo_index = g_scene_playing->rec_quality;
        }

        item = menu_get_item(cur_menu, lo_index);
        if (NULL == item)
        {
            print_err();
            break;
        }
        item->sel_icon_id = 0;
        item->unsel_icon_id = 0;

        _proc_item_click(s_menu_option_level, g_scene_playing);

        if (s_type == LTYPE_SENSITIVITY)
        {
            if (g_scene_playing->cur_threthod == SEN_LEVEL_LOW)
            {
                lo_index = 0;
            }
            else if (g_scene_playing->cur_threthod == SEN_LEVEL_MEDIUM)
            {
                lo_index = 1;
            }
            else
            {
                lo_index = 2;
            }
        }
        else
        {
            lo_index = g_scene_playing->rec_quality;
        }

        item = menu_get_item(cur_menu, lo_index);
        if (NULL == item)
        {
            print_err();
            break;
        }
        item->sel_icon_id = g_scene_playing->pic_select->pic_id;
        item->unsel_icon_id = g_scene_playing->pic_select->pic_id;

        menuview_paint(menuview);
        break;

        case WIDGET_SIG_MENUVIEW_QUIT:
        radio_playing_option_level_exit(menuview);
        break;

        default:
        break;
    }
}

/******************************************************************************/
/*!                    
 * \par  Description:     add one item
*******************************************************************************/
static void _add_item(menu_t *menu_data, menuitem_t *item, string_resource_t *str, bool sub_menu_flag)
{
    item->callback = (item_cb_t) _option_level_cb;
    item->text_id = str->str_id;
    item->has_child = sub_menu_flag;
    menu_append_item(menu_data, item);
}

/******************************************************************************/
/*!                    
 * \par  Description:     fill all items
*******************************************************************************/
static bool _fill_items(menu_t *menu_data, radio_scene_playing_t *playing)
{
    menuitem_t *item;
    int lo_index;

    /* 填充的顺序必须跟 option_level_item_e 定义的一样 */
    item = menuitem_create();
    if (item == NULL)
    {
        return false;
    }

    _add_item(menu_data, item, s_res_option_level.str_low, false);
    _add_item(menu_data, item, s_res_option_level.str_medium, false);
    _add_item(menu_data, item, s_res_option_level.str_high, false);

    if (s_type == LTYPE_SENSITIVITY)
    {
        if (playing->cur_threthod == SEN_LEVEL_LOW)
        {
            lo_index = 0;
        }
        else if (playing->cur_threthod == SEN_LEVEL_MEDIUM)
        {
            lo_index = 1;
        }
        else
        {
            lo_index = 2;
        }
    }
    else
    {
        lo_index = playing->rec_quality;
    }

    menuitem_delete(item);


    item = menu_get_item(menu_data, lo_index);
    if ((NULL != item) && (NULL != playing) && (NULL != playing->pic_select))
    {
        item->sel_icon_id = playing->pic_select->pic_id;
        item->unsel_icon_id = playing->pic_select->pic_id;
    }

    return true;
}


/******************************************************************************/
/*!                    
 * \par  Description:     load style
*******************************************************************************/
void radio_option_level_load_style(resgroup_resource_t *resgroup)
{
    if (NULL == resgroup)
        return;

    /* resgroup option */
    s_res_option_level.resgroup = resgroup;
    s_res_option_level.str_low = (string_resource_t *) get_resgroup_child(resgroup, STR_LOW);
    s_res_option_level.str_medium = (string_resource_t *) get_resgroup_child(resgroup, STR_MEDIUM);
    s_res_option_level.str_high = (string_resource_t *) get_resgroup_child(resgroup, STR_HIGH);
}

/******************************************************************************/
/*!                    
 * \par  Description:     level init
*******************************************************************************/
menu_t* radio_playing_option_level_init(option_level_type_e type)
{
    if ((NULL == g_scene_playing) || (NULL == s_res_option_level.resgroup))
    {
        return NULL;
    }

    s_type = type;

    if (NULL == s_menu_option_level)
    {
        s_menu_option_level = menu_create();
        _fill_items(s_menu_option_level, g_scene_playing);
        if (type == LTYPE_SENSITIVITY)
        {
            if (g_scene_playing->cur_threthod < SEN_LEVEL_MEDIUM)
            {
                menu_set_cur_index(s_menu_option_level, LEVEL_LOW);
            }
            else if (g_scene_playing->cur_threthod < SEN_LEVEL_HIGH)
            {
                menu_set_cur_index(s_menu_option_level, LEVEL_MEDIUM);
            }
            else
            {
                menu_set_cur_index(s_menu_option_level, LEVEL_HIGH);
            }
        }
        
        else
        {
            menu_set_cur_index(s_menu_option_level, LEVEL_LOW);
        }
    }

    return s_menu_option_level;
}

/******************************************************************************/
/*!                    
 * \par  Description:     level deinit
*******************************************************************************/
int radio_playing_option_level_deinit(void)
{
    if (NULL != s_menu_option_level)
    {
        menu_delete(s_menu_option_level);
        s_menu_option_level = NULL;
    }

    return 0;
}

/******************************************************************************/
/*!                    
 * \par  Description:     level enter
*******************************************************************************/
bool radio_playing_option_level_enter(menuview_t *menuview, option_level_type_e type)
{
    menu_t *cur_menu;

    cur_menu = radio_playing_option_level_init(type);

    menuview_set_menu(menuview, cur_menu);
    menuview_paint(menuview);

    return true;
}

/******************************************************************************/
/*!                    
 * \par  Description:     level exit
*******************************************************************************/
bool radio_playing_option_level_exit(menuview_t *menuview)
{
    fusion_type_e fenter;
    fenter = FUSION_BACKWARD;

    radio_playing_option_level_deinit();

    radio_playing_option_enter(menuview, fenter);

    return true;
}
