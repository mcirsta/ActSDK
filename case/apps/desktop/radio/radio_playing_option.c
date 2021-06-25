/********************************************************************************
 *                              USDK(1100)
 *                             Module: app
 *                 Copyright(c) 2003-2009 Actions Semiconductor,
 *                            All Rights Reserved.
 * File : radio_option.c
 * History:
 *      <author>    <time>           <version >             <desc>
 *      cheason    2009-04-23          v1.0               build this file
 ********************************************************************************/
/*!
 * \file     radio_option.c
 * \brief    FM播放界面场景下的option菜单相关操作
 * \author   cheason
 *
 * \version 1.0
 * \date  2009/04/23
 *******************************************************************************/

#include "radio_app.h"
#include "radio_scene_playing.h"
#include "radio_ui_playing.h"
#include "radio_playing_option.h"
#include "radio_playing_option_level.h"
#include "radio_playing_option_band.h"
#include "radio_playing_option_path.h"
#include "radio_playing_option_channellist.h"

static res_option_t s_res_option =
{ NULL };
static menu_t *s_menu_option = NULL;
static int s_option_current = 0;
static int s_option_offset = 0;
static bool s_show_first_menu = true;

static void _adjust_callback(adjust_t *adjust, adjust_signal_e signal);
static menu_t *_get_sub_menu(menuview_t *menuview);
static bool _proc_item_click(menuview_t *menuview);
static void _option_cb(menuview_t *menuview, menuview_sig_e signal);
static void _add_item(menu_t *menu_data, menuitem_t *item, string_resource_t *str, bool sub_menu_flag);
static bool _fill_items(menu_t *menu_data, radio_scene_playing_t *playing);


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

/*
 * 增益调整
 */
static void _adjust_callback(adjust_t *adjust, adjust_signal_e signal)
{
    int lo_index;
    print_dbg("_adjust_callback signal=%d\n", signal);
    switch (signal)
    {
        case WIDGET_SIG_ADJUST_OK:
        print_dbg("WIDGET_SIG_ADJUST_OK\n");
        adjust_get_current_index(&lo_index);
        g_scene_playing->rec_gain = lo_index;

        if (g_scene_playing->adjust != NULL)
        {
            adjust_delete(g_scene_playing->adjust);
            g_scene_playing->adjust = NULL;
        }
        break;
        case WIDGET_SIG_ADJUST_CANCEL:
        print_dbg("WIDGET_SIG_ADJUST_CANCEL\n");
        if (g_scene_playing->adjust != NULL)
        {
            adjust_delete(g_scene_playing->adjust);
            g_scene_playing->adjust = NULL;
        }
        break;

        default:
        break;
    }
}
/*
 * 获取子菜单
 */
static menu_t *_get_sub_menu(menuview_t *menuview)
{
    menu_t *cur_menu = NULL;
    int option_menuitem; 

    if( NULL == menuview)
    {
        print_err("menuview is NULL!\n");
        return NULL;
    }
    cur_menu = menuview->menu;
    option_menuitem= cur_menu->current;

    if((g_scene_playing->is_userchannel_mode == true) && (g_scene_playing->use_preset == true))
    {
        option_menuitem++;
    }
    switch (option_menuitem)
    {
        case ITEM_SAVE_DEL_CHANNEL:
        radio_playing_option_channellist_enter(menuview, true);
        break;

        case ITEM_CHANNEL_LIST:
        radio_playing_option_channellist_enter(menuview, false);
        break;

        case ITEM_SENSITIVITY:
        radio_playing_option_level_enter(menuview, LTYPE_SENSITIVITY);
        break;

        case ITEM_REC_QUALITY:
        radio_playing_option_level_enter(menuview, LTYPE_REC_QUALITY);
        break;

        case ITEM_REC_SAVE_PATH:
        radio_playing_option_path_enter(menuview);
        break;

        case ITEM_USER_CHANNEL_LIST:
        radio_playing_option_userfilelist_enter(menuview);
        break;

        case ITEM_BAND_REGION:
        radio_playing_option_band_enter(menuview);
        break;

        default:
        break;
    }

    return cur_menu;
}
/*
 * Option菜单选项处理
 */
static bool _proc_item_click(menuview_t *menuview)
{
    menu_t *menu_option = menuview->menu;
    unsigned int max, min;
    bool ret = true;
    resgroup_resource_t *resgroup_adjust;
    adjust_textdigit_init_t adj_textdigit;
    bool last_channel_id_flag = false;
    int next_channel_id;
    int option_menuitem = menu_option->current;

    if((g_scene_playing->is_userchannel_mode == true) && (g_scene_playing->use_preset == true))
    {
        option_menuitem++;
    }
    switch (option_menuitem)
    {
        case ITEM_SAVE_DEL_CHANNEL:
        if(g_scene_playing->is_userchannel_mode == false)
        {
            g_scene_playing->parse_default_file_flag = true;

            /*判断当前是否在播放最后一个电台*/
            next_channel_id = get_next_channel_index(
                g_scene_playing->channel_list_info.channel_freq, g_scene_playing->cur_preset_channel_id);
            if( g_scene_playing->cur_preset_channel_id > next_channel_id )
            {
                last_channel_id_flag = true;
            }
            
            g_scene_playing->channel_list_info.channel_freq[g_scene_playing->cur_preset_channel_id] = 0;
            memset(g_scene_playing->channel_list_info.channel_name[g_scene_playing->cur_preset_channel_id], 
                0, PATH_MAX);
            
            g_scene_playing->channel_list_info.channel_num--;

            if( last_channel_id_flag == false )
            {
                g_scene_playing->cur_preset_channel_id = get_next_channel_index(
                    g_scene_playing->channel_list_info.channel_freq, g_scene_playing->cur_preset_channel_id);
            }
            else
            {
                /*删除的是最后一个电台，则播放前一个电台*/
                g_scene_playing->cur_preset_channel_id = get_prev_channel_index(
                    g_scene_playing->channel_list_info.channel_freq, g_scene_playing->cur_preset_channel_id);
            }

            /*如果channel list中不存在电台，回到normal模式，播当前波段最小频率*/
            if (g_scene_playing->cur_preset_channel_id == -1)
            {
                g_scene_playing->use_preset = false;/*切回normal模式*/
                get_band_range(g_scene_playing->cur_band, &max, &min);
                g_scene_playing->cur_frequency = min;
            }
            else
            {
                g_scene_playing->cur_frequency
                        = g_scene_playing->channel_list_info.channel_freq[g_scene_playing->cur_preset_channel_id];
            }

            fm_engine_msg(MSG_FMENGINE_SET_FREQ, &g_scene_playing->cur_frequency, sizeof(int), NULL);

            show_cur_freq(g_scene_playing, true);

        }
        
        radio_playing_option_exit(menuview);
        break;

        case ITEM_AUTO_SEARCH:
        g_scene_playing->is_userchannel_mode = false;
            
        memset(&g_scene_playing->channel_list_info, 0, sizeof(channel_info_t));
        get_band_range(g_scene_playing->cur_band, &max, &min);
        g_scene_playing->cur_frequency = min;

        g_scene_playing->searching_flag = AUTO_SEARCH;
        g_scene_playing->auto_search_start = true;

        radio_playing_option_exit(menuview);
        break;

        case ITEM_FM_REC:
        /*录音之前强制将静音关闭*/    
        g_scene_playing->mute = false;
        fm_engine_msg(MSG_FMENGINE_SET_PLAY, NULL, 0, NULL);

        g_scene_playing->next_scene = RADIO_SCENE_RECORDING_FORWARD;
        exit_msg_loop();

        radio_playing_option_exit(menuview);
        break;

        case ITEM_REC_GAIN:
        
        resgroup_adjust = (resgroup_resource_t *) get_scene_child(g_scene_playing->scene, WIDGET_ADJUST_RESGROUP);
        g_scene_playing->str_stoprec = (string_resource_t *) get_resgroup_child(resgroup_adjust, STR_ADJUST);
        if(NULL == g_scene_playing->str_stoprec)
        {
            return false;
        }

        /* initial adjust data */
        memset(&adj_textdigit, 0, sizeof(adjust_textdigit_init_t));
        adj_textdigit.cur_val = g_scene_playing->rec_gain;
        adj_textdigit.min_val = 1;
        adj_textdigit.max_val = 5;
        adj_textdigit.step = 1;
        adj_textdigit.str_title_id = g_scene_playing->str_stoprec->str_id;

        /* adjust init and callback function */
        g_scene_playing->adjust = adjust_create(&adj_textdigit, ADJ_STYLE_TEXTDIGIT, _adjust_callback);
        if (NULL == g_scene_playing->adjust)
        {
            print_err("adjust create failed");
            return false;
        }
        break;

        case ITEM_MUTE_PLAY:
        g_scene_playing->mute = !g_scene_playing->mute;
        if (g_scene_playing->mute == true)
        {
            fm_engine_msg(MSG_FMENGINE_SET_STOP, NULL, 0, NULL);
        }
        else
        {
            fm_engine_msg(MSG_FMENGINE_SET_PLAY, NULL, 0, NULL);
        }

        radio_playing_option_exit(menuview);
        break;
        default:
        break;
    }

    return ret;
}
#if 0
/* 退出菜单 */
void radio_option_menu_quit(void)
{
    radio_playing_option_deinit();
    radio_playing_option_level_deinit();
    radio_playing_option_band_deinit();
    radio_playing_option_path_deinit();
    radio_playing_option_channellist_deinit();
    if (NULL != g_scene_playing->option_menu)
    {
        menuview_delete(g_scene_playing->option_menu);
    }
    g_scene_playing->option_menu = NULL;
}
#endif
/*
 * Option菜单回调
 */
static void _option_cb(menuview_t *menuview, menuview_sig_e signal)
{
    switch (signal)
    {
        case WIDGET_SIG_MENUVIEW_QUIT:
        radio_playing_option_exit(menuview);
        break;

        case WIDGET_SIG_MENUVIEW_OK:
        {
            menuitem_t *item_data = menu_get_cur_item(menuview->menu);
            if (NULL != item_data)
            {
                if (item_data->has_child == true)
                {
                    _get_sub_menu(menuview);
                }
                else
                {
                    _proc_item_click(menuview);
                }
            }
        }
        break;

        case WIDGET_SIG_MENUVIEW_DOWN:
        break;

        case WIDGET_SIG_MENUVIEW_UP:
        break;

        case WIDGET_SIG_MENUVIEW_EXCEED_FIRST:
        {
            int last = menu_get_global_size(menuview->menu) - 1;
            if (last >= 0)
            {
                menu_set_cur_index(menuview->menu, last);
            }
        }
        break;

        case WIDGET_SIG_MENUVIEW_EXCEED_LAST:
        {
            int num = menu_get_global_size(menuview->menu);
            if (num > 0)
            {
                menu_set_cur_index(menuview->menu, 0);
            }
        }
        break;

        default:
        break;
    }
}

/*
 * 添加菜单项
 */
static void _add_item(menu_t *menu_data, menuitem_t *item, string_resource_t *str, bool sub_menu_flag)
{
    item->callback = (item_cb_t) _option_cb;
    item->text_id = str->str_id;
    item->has_child = sub_menu_flag;
    menu_append_item(menu_data, item);
}

/*
 * 全部添加菜单项
 */
static bool _fill_items(menu_t *menu_data, radio_scene_playing_t *playing)
{
    menuitem_t *item;

    /* 填充的顺序必须跟 option_item_e 定义的一样 */
    item = menuitem_create();
    if (NULL == item)
    {
        return false;
    }

    if (!playing->use_preset)
    {
        s_show_first_menu = true;
        _add_item(menu_data, item, s_res_option.str_save_channel, true);
    }
    else
    {
        if(playing->is_userchannel_mode == false)
        {
            s_show_first_menu = true;
            _add_item(menu_data, item, s_res_option.str_del_channel, false);
        }
        else
        {
            s_show_first_menu = false;
        }
    }
    _add_item(menu_data, item, s_res_option.str_auto_search, false);
    _add_item(menu_data, item, s_res_option.str_channel_list, true);
    _add_item(menu_data, item, s_res_option.str_fm_rec, false);
    _add_item(menu_data, item, s_res_option.str_rec_quality, true);
    _add_item(menu_data, item, s_res_option.str_rec_gain, false);
    _add_item(menu_data, item, s_res_option.str_save_path, true);
    _add_item(menu_data, item, s_res_option.str_sensitivity, true);
    _add_item(menu_data, item, s_res_option.str_user_channel_list, true);
    _add_item(menu_data, item, s_res_option.str_tuner_region, true);

    if (!playing->mute)
    {
        _add_item(menu_data, item, s_res_option.str_mute, false);
    }
    else
    {
        _add_item(menu_data, item, s_res_option.str_play, false);

    }

    menuitem_delete(item);

    return true;
}

/******************************************************************************/
/*!                    
* \par  Description:
*       场景界面加载风格
* \param[in]    resgroup_resource_t *: 资源指针
* \retval       无
*******************************************************************************/
void radio_option_load_style(resgroup_resource_t *resgroup)
{
    if (NULL == resgroup)
    {
        return;
    }

    /* resgroup option */
    s_res_option.resgroup = resgroup;
    s_res_option.str_save_channel = (string_resource_t *) get_resgroup_child(resgroup, STR_SAVE_CHANNEL);
    s_res_option.str_del_channel = (string_resource_t *) get_resgroup_child(resgroup, STR_DEL_CHANNEL);
    s_res_option.str_auto_search = (string_resource_t *) get_resgroup_child(resgroup, STR_AUTO_SEARCH);
    s_res_option.str_channel_list = (string_resource_t *) get_resgroup_child(resgroup, STR_CHANNEL_LIST);
    s_res_option.str_fm_rec = (string_resource_t *) get_resgroup_child(resgroup, STR_FM_REC);
    s_res_option.str_rec_quality = (string_resource_t *) get_resgroup_child(resgroup, STR_REC_QUALITY);
    s_res_option.str_rec_gain = (string_resource_t *) get_resgroup_child(resgroup, STR_REC_GAIN);
    s_res_option.str_save_path = (string_resource_t *) get_resgroup_child(resgroup, STR_SAVE_PATH);
    s_res_option.str_sensitivity = (string_resource_t *) get_resgroup_child(resgroup, STR_SENSITIVITY);
    s_res_option.str_user_channel_list = (string_resource_t *) get_resgroup_child(resgroup, STR_USER_CHANNEL_LIST);
    s_res_option.str_tuner_region = (string_resource_t *) get_resgroup_child(resgroup, STR_TUNER_REGION);
    s_res_option.str_mute = (string_resource_t *) get_resgroup_child(resgroup, STR_MUTE);
    s_res_option.str_play = (string_resource_t *) get_resgroup_child(resgroup, STR_PLAY);

}
/******************************************************************************/
/*!                    
 * \par  Description:
 *        菜单初始化  
 * \param  无      
 * \return   菜单指针       
*******************************************************************************/
menu_t* radio_playing_option_init(void)
{
    bool last_flag;
    if ((NULL == g_scene_playing) || (NULL == s_res_option.resgroup))
    {
        return NULL;
    }

    if (NULL == s_menu_option)
    {
        s_menu_option = menu_create();
        last_flag = s_show_first_menu;
        _fill_items(s_menu_option, g_scene_playing);

        if((s_option_current == 0)&&(s_option_offset == 0))
        {
            last_flag = s_show_first_menu;
        }
        
        //init current index    
        if( last_flag == s_show_first_menu ) 
        {
            menu_set_offset( s_menu_option, s_option_offset);        
            menu_set_cur_index(s_menu_option, s_option_current);        
        }
        else
        {
            if( (last_flag == true) && (s_show_first_menu == false) )
            {
                //上次显示了第一项菜单，这次不显示，偏移减1
                menu_set_offset( s_menu_option, s_option_offset - 1 );                    
                menu_set_cur_index(s_menu_option, s_option_current -1 );
            }
            else
            {
                //上次没有显示了第一项菜单，这次显示，偏移加1            
                menu_set_offset( s_menu_option, s_option_offset + 1 );                    
                menu_set_cur_index(s_menu_option, s_option_current + 1 );
            }
        }
    }

    return s_menu_option;
}
/******************************************************************************/
/*!                    
 * \par  Description:
 *       menu deinit     
 * \param  无      
 * \return   无       
*******************************************************************************/
int radio_playing_option_deinit(void)
{
    if (NULL != s_menu_option)
    {
        s_option_current = s_menu_option->current;
        s_option_offset = s_menu_option->offset;
        
        menu_delete(s_menu_option);
        s_menu_option = NULL;
    }

    return 0;
}

/******************************************************************************/
/*!                    
 * \par  Description:
 *       enter menu        
*******************************************************************************/
bool radio_playing_option_enter(menuview_t *menuview, fusion_type_e fenter)
{
    menu_t *cur_menu;
    fusion_effect_t eff;
    if (fenter == FUSION_FORWARD)
    {
        fusion_get_config(FUSION_TYPE_OPTION_RGB, fenter, &eff);
        fusion_set_effect(&eff);
    }

    cur_menu = radio_playing_option_init();

    menuview_set_menu(menuview, cur_menu);

    if (fenter == FUSION_FORWARD)
    {
        _option_fusion_set_first_frame(menuview);
    }

    menuview_paint(menuview);

    if (fenter == FUSION_FORWARD)
    {
        _option_fusion_set_last_frame(menuview);
        fusion_display( NULL);
    }

    return true;
}


bool radio_playing_option_exit(menuview_t *menuview)
{
    radio_playing_option_deinit();

    s_option_current = 0;
    s_option_offset = 0;
    radio_playing_widget_option_exit(menuview, NULL);

    return true;
}

