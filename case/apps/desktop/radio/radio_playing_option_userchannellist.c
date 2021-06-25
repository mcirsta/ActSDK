/********************************************************************************
 *                              USDK(1100)
 *                             Module: app
 *                 Copyright(c) 2003-2009 Actions Semiconductor,
 *                            All Rights Reserved.
 * File : radio_playing_option_userchannellist.c
 * History:
 *      <author>    <time>           <version >             <desc>
 *    wangsheng    2009-2-25          v1.0               build this file
 ********************************************************************************/
/*!
 * \file     radio_playing_option_userfilelist.c
 * \brief    radio option
 * \author   wangsheng
 *
 * \version 1.0
 * \date  2009/02/25
 *******************************************************************************/

#include "radio_playing_option_userchannellist.h"
#include "radio_scene_playing.h"
#include "radio_playing_option.h"
#include "radio_ui_playing.h"


static void _proc_option_userchannellist(menuview_t *menuview, menuview_sig_e signal);

/* menu list data define */
menu_t *g_recorder_option_userchannellist = NULL;

static menuitem_t option_userchannellist_item[MAX_CHANNEL_NUM];
//channel_info_t channel_info;
static char s_default_name[10];


/******************************************************************************/
/*!                    
 * \par  Description:     user channel list process
*******************************************************************************/
static void _proc_option_userchannellist(menuview_t *menuview, menuview_sig_e signal)
{
    menu_t *cur_menu = menuview->menu;
    int lo_index = cur_menu->current;

    switch (signal)
    {
        case WIDGET_SIG_MENUVIEW_OK:
        if (channel_info.channel_freq[lo_index] != 0)
        {
            g_scene_playing->cur_preset_channel_id = lo_index;
            g_scene_playing->is_userchannel_mode = true;

            g_scene_playing->cur_frequency = channel_info.channel_freq[lo_index];
            fm_engine_msg(MSG_FMENGINE_SET_FREQ, &g_scene_playing->cur_frequency, sizeof(int), NULL);
            show_cur_freq(g_scene_playing, true);

            radio_playing_option_exit(menuview);
        }
        break;

        case WIDGET_SIG_MENUVIEW_QUIT:
        radio_playing_option_userchannellist_exit(menuview);
        break;

        default:
        break;
    }
}

/******************************************************************************/
/*!                    
 * \par  Description:     userchannel list init
*******************************************************************************/
menu_t* radio_playing_option_userchannellist_init(menuview_t *menuview, char *path)
{
    int i, fd, result;
    int k = 0;
    bool ret;
    struct stat file_stat;
    char *file_content;

    sprintf(s_default_name, "---");

    memset(&option_userchannellist_item, 0, sizeof(option_userchannellist_item));

    if (strcmp(path, channel_list_file) != 0)
    {
        fd = open(path, O_RDWR);

        fcntl(fd, F_BUILDINDEX, 0);

        result = stat(path, &file_stat);
        if ((result < 0) || (file_stat.st_size == 0))
        {
            print_err();
            close(fd);
            return false;
        }

        file_content = malloc(file_stat.st_size);
        if (file_content == NULL)
        {
            print_err();
            close(fd);
            return false;
        }

        result = read(fd, file_content, file_stat.st_size);
        if (result < 0)
        {
            print_err();
            free(file_content);
            close(fd);
            return false;
        }

        close(fd);

        ret = parse_ini_file(&channel_info, file_content, file_stat.st_size);
        if (ret == false)
        {
            free(file_content);
            return false;
        }

        free(file_content);
    }
    else
    {
        channel_info = g_scene_playing->channel_list_info;
    }
    /*
     for(i = 0; i < MAX_CHANNEL_NUM; i++)
     {
     if(check_freq(g_scene_playing->cur_band, channel_info.channel_freq[i]) != false)
     {
     if(channel_info.channel_freq[i] != 0)
     {
     option_userchannellist_item[i].text = channel_info.channel_name[i];
     option_userchannellist_item[i].callback = (item_cb_t)_proc_option_userchannellist;
     }
     else
     {
     return false;
     }
     }
     else
     {
     if(channel_info.channel_freq[i] == 0)
     {
     break;
     }
     else
     {
     return false;
     }
     }
     }*/
    for (i = 0; i < MAX_CHANNEL_NUM; i++)
    {
        if ((check_freq(g_scene_playing->cur_band, (int)(channel_info.channel_freq[i])) == true)
                && (channel_info.channel_freq[i] != 0))
        {
            option_userchannellist_item[i].text = channel_info.channel_name[i];
            option_userchannellist_item[i].callback = (item_cb_t) _proc_option_userchannellist;
        }
        else
        {
            channel_info.channel_freq[i] = 0;
            option_userchannellist_item[i].text = s_default_name;
            option_userchannellist_item[i].callback = (item_cb_t) _proc_option_userchannellist;
            k++;
        }
    }

    if (k > 20)
    {
        return false;
    }

    /*initial menu*/
    if (g_recorder_option_userchannellist == NULL)
    {
        /*create menu*/
        g_recorder_option_userchannellist = menu_create();
        if (g_recorder_option_userchannellist == NULL)
        {
            print_err("can not create option mode");
            return false;
        }
        else
        {
            /* append items */
            menu_append_items(g_recorder_option_userchannellist, option_userchannellist_item, i);
            /* set current index */
            menu_set_cur_index(g_recorder_option_userchannellist, 0);
        }
    }

    return g_recorder_option_userchannellist;
}

/******************************************************************************/
/*!                    
 * \par  Description:     user channel list deinit
*******************************************************************************/
bool radio_playing_option_userchannellist_deinit(void)
{
    /*delete menu*/
    if (g_recorder_option_userchannellist != NULL)
    {
        /*create menu*/
        menu_delete(g_recorder_option_userchannellist);
        g_recorder_option_userchannellist = NULL;
    }

    return true;
}

/******************************************************************************/
/*!                    
 * \par  Description:     user channel lisst enter
*******************************************************************************/
bool radio_playing_option_userchannellist_enter(menuview_t *menuview, char *path)
{
    menu_t *cur_menu;
    dialog_init_t dialog_init;
    /*只有当解析了defalut.ini文件，需要保存文件*/
    if (g_scene_playing->parse_default_file_flag == true)
    {
        save_default_file(g_scene_playing);
    }

    cur_menu = radio_playing_option_userchannellist_init(menuview, path);
    if (cur_menu == NULL)
    {
        /*解析文件时会清空channel_info中的数据，因此解析失败需要将is_userchannel_mode置为false*/
        g_scene_playing->is_userchannel_mode = false;
        /*初始化失败，直接设置为普通模式，避免以前的电台名称还显示在屏幕上*/
        g_scene_playing->use_preset = false;
        dialog_init.head = DIALOG_HEAD_WARN;
        dialog_init.str_id = g_scene_playing->str_fmterr->str_id;

        dialog_modal(&dialog_init);
        show_cur_freq(g_scene_playing, true);
        /*返回文件列表菜单*/
        radio_playing_option_userfilelist_enter(menuview);    
        return false;
    }
    else
    {
        menuview_set_menu(menuview, cur_menu);
        
        menuview_paint(menuview);
        
        return true;
    }
}

/******************************************************************************/
/*!                    
 * \par  Description:     user channel list exit
*******************************************************************************/
bool radio_playing_option_userchannellist_exit(menuview_t *menuview)
{
    /*delete main menu*/
    radio_playing_option_userchannellist_deinit();
    
    /*enter parent menu*/
    radio_playing_option_userfilelist_enter(menuview);

    return true;
}

