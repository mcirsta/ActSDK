/********************************************************************************
 *                              USDK(1100)
 *                             Module: app
 *                 Copyright(c) 2003-2009 Actions Semiconductor,
 *                            All Rights Reserved.
 * File : radio_option_channellist.c
 * History:
 *      <author>    <time>           <version >             <desc>
 *      cheason    2009-04-23          v1.0               build this file
 ********************************************************************************/
/*!
 * \file     radio_option_channellist.c
 * \brief    FM播放界面场景下的option菜单频道列表相关操作
 * \author   cheason
 *
 * \version 1.0
 * \date  2009/04/23
 *******************************************************************************/


#include "radio_playing_option_channellist.h"
#include "radio_ui_playing.h"

static menu_t *s_menu_option_chlist = NULL;
static bool s_is_save = false;
static char * s_chlist_str[MAX_CHANNEL_NUM];

static void _option_chlist_cb(menuview_t *menuview, menuview_sig_e signal);
static void _add_item(menu_t *menu_data, menuitem_t *item, char *str, bool sub_menu_flag);
static bool _fill_items(menu_t *menu_data, radio_scene_playing_t *playing);

/******************************************************************************/
/*!                    
 * \par  Description:     channel list callback
*******************************************************************************/
static void _option_chlist_cb(menuview_t *menuview, menuview_sig_e signal)
{
    switch (signal)
    {
    case WIDGET_SIG_MENUVIEW_OK:
        g_scene_playing->is_userchannel_mode = false;
        
        if(s_is_save == true)
        {
            g_scene_playing->channel_list_info.channel_freq[s_menu_option_chlist->current]
                                    = g_scene_playing->cur_frequency;
            sprintf(g_scene_playing->channel_list_info.channel_name[s_menu_option_chlist->current], 
                        "_CH%02d_", s_menu_option_chlist->current + 1);
            g_scene_playing->cur_preset_channel_id = s_menu_option_chlist->current;
            g_scene_playing->channel_list_info.channel_num++;

            show_channel_name(g_scene_playing);/*paint channel name*/

            radio_playing_option_exit(menuview);
            
        }
        else
        {
            if(0 != g_scene_playing->channel_list_info.channel_freq[s_menu_option_chlist->current])
            {
                if((g_scene_playing->cur_preset_channel_id != s_menu_option_chlist->current)
                    || ((g_scene_playing->cur_frequency) != 
                        g_scene_playing->channel_list_info.channel_freq[s_menu_option_chlist->current]))
                {
                    g_scene_playing->cur_preset_channel_id = s_menu_option_chlist->current;
                    g_scene_playing->cur_frequency
                        = g_scene_playing->channel_list_info.channel_freq[s_menu_option_chlist->current];
                    fm_engine_msg(MSG_FMENGINE_SET_FREQ, &g_scene_playing->cur_frequency, sizeof(int), NULL);
                    show_cur_freq(g_scene_playing, true);
                }
                
                radio_playing_option_exit(menuview);
            }
        }

        break;

    case WIDGET_SIG_MENUVIEW_QUIT:
        radio_playing_option_channellist_exit(menuview);
        break;

    default:
        break;
    }
}
/******************************************************************************/
/*!                    
 * \par  Description:     fill one item
*******************************************************************************/
static void _add_item(menu_t *menu_data, menuitem_t *item, char *str, bool sub_menu_flag)
{
    item->callback = (item_cb_t)_option_chlist_cb;
    item->text = str;
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
    int i, len;

    item = menuitem_create();
    if(item == NULL)
    {
        return false;
    }

    for(i = 0; i < MAX_CHANNEL_NUM; i++)
    {
        s_chlist_str[i] = (char *)malloc(20 + strlen(playing->channel_list_info.channel_name[i]));
        memset(s_chlist_str[i], 0, 20 + strlen(playing->channel_list_info.channel_name[i]));
        sprintf(s_chlist_str[i], "CH%02d  ", i+1);
        len = (int)strlen(s_chlist_str[i]);
        if((playing->channel_list_info.channel_freq[i] != 0) 
            && (check_freq(playing->cur_band, (int)(playing->channel_list_info.channel_freq[i])) == true)) 
        {
            freq_to_str(s_chlist_str[i] + len, (playing->channel_list_info.channel_freq[i]));
            len = (int)strlen(s_chlist_str[i]);
            sprintf(s_chlist_str[i] + len, " MHz");
            len = (int)strlen(s_chlist_str[i]);
            sprintf(s_chlist_str[i] + len, "  %s", playing->channel_list_info.channel_name[i]);
        }
        else
        {
            sprintf(s_chlist_str[i] + len, "---");
        }
        _add_item(menu_data, item, s_chlist_str[i], false);
    }

    menuitem_delete(item);

    return true;
}

/******************************************************************************/
/*!                    
 * \par  Description:     channellist init
*******************************************************************************/
menu_t* radio_playing_option_channellist_init(bool save)
{
    s_is_save = save;

    memset(s_chlist_str, 0, sizeof(s_chlist_str));

    if(NULL == s_menu_option_chlist)
    {
        s_menu_option_chlist = menu_create();
        if(NULL != s_menu_option_chlist)
        {
            s_menu_option_chlist->callback = (menu_cb_t)_option_chlist_cb;
        }
        _fill_items(s_menu_option_chlist, g_scene_playing);
        menu_set_cur_index(s_menu_option_chlist, 0);
    }

    return s_menu_option_chlist;
}


/******************************************************************************/
/*!                    
 * \par  Description:     channellist deinit
*******************************************************************************/
int radio_playing_option_channellist_deinit(void)
{
    int i;

    if (NULL != s_menu_option_chlist)
    {
        menu_delete(s_menu_option_chlist);
        s_menu_option_chlist = NULL;
    }

    for(i = 0; i < MAX_CHANNEL_NUM; i++)
    {
        if(s_chlist_str[i] != NULL)
        {
            free(s_chlist_str[i]);
            s_chlist_str[i] = NULL;
        }
    }

    return 0;
}

/******************************************************************************/
/*!                    
 * \par  Description:     channellist enter
*******************************************************************************/
bool radio_playing_option_channellist_enter( menuview_t *menuview, bool save)
{
    menu_t *cur_menu;

    cur_menu = radio_playing_option_channellist_init(save);

    menuview_set_menu(menuview, cur_menu);
    menuview_paint(menuview);

    return true;
}

/******************************************************************************/
/*!                    
 * \par  Description:     channellist exit
*******************************************************************************/
bool radio_playing_option_channellist_exit( menuview_t *menuview )
{
    fusion_type_e fenter;
    fenter = FUSION_BACKWARD;
    
    radio_playing_option_channellist_deinit();

    radio_playing_option_enter(menuview, fenter);
    
    return true;
}
