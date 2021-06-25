/********************************************************************************
 *                              USDK(1100)
 *                             Module: app
 *                 Copyright(c) 2003-2009 Actions Semiconductor,
 *                            All Rights Reserved.
 * File : radio_playing_option_userfilelist.c
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

#include "radio_playing_option_userfilelist.h"

static void _proc_option_userfilelist(menuview_t *menuview, menuview_sig_e signal);
static int _get_file_num(char *path, int start_num, fsel_filename_t *file_name, int num);

/* menu list data define */
menu_t *g_recorder_option_userfile = NULL;

fsel_filename_t file_info[WANNA_GET_NUM];

static int s_option_userfile_cur_index = 0;
static int s_option_userfile_offset = 0;
static int s_option_userfile_global_offset = 0;

/******************************************************************************/
/*!                    
 * \par  Description:     user file list process
*******************************************************************************/
static void _proc_option_userfilelist(menuview_t *menuview, menuview_sig_e signal)
{
    menu_t *cur_menu = menuview->menu;
    menuitem_t *cur_item = menu_get_cur_item(cur_menu);
    char buf[PATH_MAX];
    int lo_index = cur_menu->current;
    int num, i;
    /*
     menuitem_t item;
     fsel_filename_t file_name;
     */

    switch (signal)
    {
        case WIDGET_SIG_MENUVIEW_OK:
            
        if( (strlen(file_info[lo_index].name) < 1) ) 
        {
            print_err("\nfilepath = %s, name = %s \n",
                file_info[lo_index].filepath,file_info[lo_index].name);            
            break;
        }       
        
        sprintf(buf, "%s%s", file_info[lo_index].filepath, file_info[lo_index].name);
        if( strcmp(buf, "<NULL>") == 0 )
        {
            print_err("buf:\n %s is error!\n",buf);
            break;
        }
        sprintf(userchannel_path, "%s%s", file_info[lo_index].filepath, file_info[lo_index].name);
        radio_playing_option_userfilelist_deinit();        
        radio_playing_option_userchannellist_enter(menuview, buf);
        break;

        case WIDGET_SIG_MENUVIEW_QUIT:
        radio_playing_option_userfilelist_exit(menuview);
        break;

        case WIDGET_SIG_MENUVIEW_EXCEED_FIRST:
        num = _get_file_num((char *)sys_fm_path, cur_menu->global_offset + 1, file_info, WANNA_GET_NUM);

        /*fill items*/
        for (i = 0; i < num; i++)
        {
            cur_item = menu_get_item(cur_menu, i);
            if (cur_item == NULL)
            {
                break;
            }
            cur_item->callback = (item_cb_t) _proc_option_userfilelist;
            cur_item->text = file_info[i].name;
        }

        cur_menu->current++;
        break;

        case WIDGET_SIG_MENUVIEW_EXCEED_LAST:
        num = _get_file_num((char *)sys_fm_path, cur_menu->global_offset + 1, file_info, WANNA_GET_NUM);

        /*fill items*/
        for (i = 0; i < num; i++)
        {
            cur_item = menu_get_item(cur_menu, i);
            if (cur_item == NULL)
            {
                break;
            }
            cur_item->callback = (item_cb_t) _proc_option_userfilelist;
            cur_item->text = file_info[i].name;
        }

        cur_menu->current--;
        break;

        case WIDGET_SIG_MENUVIEW_UPDATE_ALL:
        num = _get_file_num((char *)sys_fm_path, cur_menu->global_offset + 1, file_info, WANNA_GET_NUM);

        /*fill items*/
        for (i = 0; i < num; i++)
        {
            cur_item = menu_get_item(cur_menu, i);
            if (cur_item == NULL)
            {
                break;
            }
            cur_item->callback = (item_cb_t) _proc_option_userfilelist;
            cur_item->text = file_info[i].name;
        }

        break;

        default:
        break;
    }
}

/******************************************************************************/
/*!                    
 * \par  Description:     get file num
*******************************************************************************/
static int _get_file_num(char *path, int start_num, fsel_filename_t *file_name, int num)
{
    int ret;

    g_scene_playing->fp = fsel_open();
    if (g_scene_playing->fp == NULL)
    {
        print_err();
        return -1;
    }

    ret = fsel_setworkdir(g_scene_playing->fp, path);
    if (ret != 0)
    {
        print_err();
        return -1;
    }

    ret = fsel_setfilter(g_scene_playing->fp, FILE_MODE);
    if (ret != 0)
    {
        print_err();
        return -1;
    }

    ret = fsel_listfile(g_scene_playing->fp, file_name, start_num, num, FSEL_POSITIVE);

    return ret;
}

/******************************************************************************/
/*!                    
 * \par  Description:     user filelist init
*******************************************************************************/
menu_t* radio_playing_option_userfilelist_init(void)
{
    menuitem_t option_userfilelist_item[WANNA_GET_NUM];
    int i, num, total_num;

    memset(&option_userfilelist_item, 0, sizeof(option_userfilelist_item));

    num = _get_file_num((char *)sys_fm_path, 1, file_info, WANNA_GET_NUM);

    total_num = fsel_getfilenum(g_scene_playing->fp, FSEL_FILENUM);

    /*fill items*/
    for (i = 0; i < num; i++)
    {
        option_userfilelist_item[i].callback = (item_cb_t) _proc_option_userfilelist;
        option_userfilelist_item[i].text = file_info[i].name;
    }

    /*initial menu*/
    if (g_recorder_option_userfile == NULL)
    {
        /*create menu*/
        g_recorder_option_userfile = menu_create();
        if (g_recorder_option_userfile == NULL)
        {
            print_err("can not create option mode");
            return false;
        }
        else
        {
            /* append items */
            menu_append_items(g_recorder_option_userfile, option_userfilelist_item, num);
        }
    }
    
    /* set current index */
    menu_set_global_offset(g_recorder_option_userfile, s_option_userfile_global_offset);
    menu_set_global_size(g_recorder_option_userfile, total_num);

    menu_set_offset(g_recorder_option_userfile, s_option_userfile_offset);   
    menu_set_cur_index(g_recorder_option_userfile, s_option_userfile_cur_index);
    

    g_recorder_option_userfile->callback = (menu_cb_t)_proc_option_userfilelist;
    
    return g_recorder_option_userfile;
}

/******************************************************************************/
/*!                    
 * \par  Description:     user filelist deinit
*******************************************************************************/
bool radio_playing_option_userfilelist_deinit(void)
{
    int ret;
        
    /*delete menu*/
    if (g_recorder_option_userfile != NULL)
    {  
        s_option_userfile_cur_index = g_recorder_option_userfile->current;
        s_option_userfile_offset = g_recorder_option_userfile->offset;
        s_option_userfile_global_offset = g_recorder_option_userfile->global_offset;

        ret = fsel_close(g_scene_playing->fp);
        if (ret != 0)
        {
            return false;
        }
        /*create menu*/
        menu_delete(g_recorder_option_userfile);
        /*clear file_info*/
        memset(file_info,0,sizeof(file_info));
        g_recorder_option_userfile = NULL;
    }
    
    return true;
}

/******************************************************************************/
/*!                    
 * \par  Description:     user filelist enter
*******************************************************************************/
bool radio_playing_option_userfilelist_enter(menuview_t *menuview)
{
    menu_t *cur_menu;
    int i,num;
    menuitem_t *cur_item;

    cur_menu = radio_playing_option_userfilelist_init();
    if( NULL == cur_menu )
    {
        return false;
    }
    
    cur_item = menu_get_cur_item(cur_menu);

    num = _get_file_num((char *)sys_fm_path, cur_menu->global_offset + 1, file_info, WANNA_GET_NUM);
    /*update menu text data*/
    for (i = 0; i < num; i++)
    {
        cur_item = menu_get_item(cur_menu, i);
        if (cur_item == NULL)
        {
            break;
        }
        cur_item->text = file_info[i].name;
    }
    menuview_set_menu(menuview, cur_menu);
    menuview_paint(menuview);

    return true;
}

/******************************************************************************/
/*!                    
 * \par  Description:     user filelist exit
*******************************************************************************/
bool radio_playing_option_userfilelist_exit(menuview_t *menuview)
{
    fusion_type_e fenter;
    fenter = FUSION_BACKWARD;

    /*delete main menu*/
    radio_playing_option_userfilelist_deinit();
    
    s_option_userfile_cur_index = 0;
    s_option_userfile_offset = 0;
    s_option_userfile_global_offset = 0;

    /*enter parent menu*/
    radio_playing_option_enter(menuview, fenter);

    return true;
}

bool update_userfilelist(void)
{
    if( NULL != g_recorder_option_userfile )
    {
        radio_playing_option_userfilelist_deinit();
        radio_playing_option_userfilelist_enter(g_scene_playing->option_menu);        
    }
    
    return true;
}


