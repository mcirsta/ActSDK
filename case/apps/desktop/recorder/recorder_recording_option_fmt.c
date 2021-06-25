/********************************************************************************
 *                              USDK(1100)
 *                             Module: app
 *                 Copyright(c) 2003-2009 Actions Semiconductor,
 *                            All Rights Reserved.
 * File : recorder_option_fmt.c
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

#include "recorder_recording_option_fmt.h"
#include "recorder_ui_recording.h"
#include "recorder_recording_option.h"

/* listmenu entry */
static void proc_option_fmt(menuview_t *menuview, menuview_sig_e signal);
static void _index_map_to_format(int format_index);

/* menu list data define */
menu_t *g_recorder_option_fmt = NULL;

#define RECORDING_OPTION_FMT_MAX_ITEMS 3

static bool support_wav = false;
static bool support_mp3 = false;
static bool support_wma = false;

menu_t* recorder_recording_option_fmt_init(void)
{
    scene_recording_t *data = g_scene_recording;
    menuitem_t option_fmt_item[RECORDING_OPTION_FMT_MAX_ITEMS];
    int i;
    int i_recorder_ext = 0;

    /*! load  option format resource */
    data->rec_opt_fmt = (resgroup_resource_t *) get_scene_child(data->scene, OPTION_FORMAT_RESGROUP);
    data->rec_wav = (string_resource_t *) get_resgroup_child(data->rec_opt_fmt, STR_WAV);
    if(NULL == data->rec_wav)
    {
        return NULL;
    }
    data->rec_mp3 = (string_resource_t *) get_resgroup_child(data->rec_opt_fmt, STR_MP3);
    if(NULL == data->rec_mp3)
    {
        return NULL;
    }
    data->rec_wma = (string_resource_t *) get_resgroup_child(data->rec_opt_fmt, STR_WMA);
    if(NULL == data->rec_wma)
    {
        return NULL;
    }

    memset(&option_fmt_item, 0, sizeof(option_fmt_item));
    /*支持wav则添加进菜单选项*/
    if(NULL != strstr(data->support_format, "|wav|") )
    {
        support_wav = true;
        option_fmt_item[0].text_id = data->rec_wav->str_id;
        i_recorder_ext++;

        if (strcmp(data->format, "wav") == 0)
        {
            option_fmt_item[0].sel_icon_id = data->rec_select->pic_id;
            option_fmt_item[0].unsel_icon_id = data->rec_select->pic_id;
        }
    }
    /*录音格式菜单排列顺序:wav-mp3-wma,某一种格式不支持，后面格式向上靠*/
    if(NULL != strstr(data->support_format, "|mp3|"))
    {
        support_mp3 = true;
        if(true == support_wav)
        {
            option_fmt_item[1].text_id = data->rec_mp3->str_id;

            if (strcmp(data->format, "mp3") == 0)
            {
                option_fmt_item[1].sel_icon_id = data->rec_select->pic_id;
                option_fmt_item[1].unsel_icon_id = data->rec_select->pic_id;
            }
        }
        else
        {
            option_fmt_item[0].text_id = data->rec_mp3->str_id;
            if (strcmp(data->format, "mp3") == 0)
            {
                option_fmt_item[0].sel_icon_id = data->rec_select->pic_id;
                option_fmt_item[0].unsel_icon_id = data->rec_select->pic_id;
            }
        }
        i_recorder_ext++;
    }
    
    if(NULL != strstr(data->support_format, "|wma|"))
    {
        support_wma = true;
        if((true == support_mp3) && (true == support_wav))
        {
            option_fmt_item[2].text_id = data->rec_wma->str_id;

            if (strcmp(data->format, "wma") == 0)
            {
                option_fmt_item[2].sel_icon_id = data->rec_select->pic_id;
                option_fmt_item[2].unsel_icon_id = data->rec_select->pic_id;
            }
        }
        else if((true == support_mp3) || (true == support_wav))
        {
            option_fmt_item[1].text_id = data->rec_wma->str_id;
            if (strcmp(data->format, "wma") == 0)
            {
                option_fmt_item[1].sel_icon_id = data->rec_select->pic_id;
                option_fmt_item[1].unsel_icon_id = data->rec_select->pic_id;
            }
        }
        else 
        {
            option_fmt_item[0].text_id = data->rec_wma->str_id;
            if (strcmp(data->format, "wma") == 0)
            {
                option_fmt_item[0].sel_icon_id = data->rec_select->pic_id;
                option_fmt_item[0].unsel_icon_id = data->rec_select->pic_id;
            }
        }
        i_recorder_ext++;
    }
   
    for (i = 0; i < i_recorder_ext; i++)
    {
        option_fmt_item[i].callback = (item_cb_t) proc_option_fmt;
    }

    /*至少支持一种格式才继续*/
    if(0 == i_recorder_ext)
    {
        goto NO_RECORDER_EXT;
    }
    else
    {
        /*进入此项标示至少支持一种录音格式*/
        /*当前设置的录音格式不支持时，选择支持的录音格式*/
        if((false == support_wav)&&(strcmp(data->format, "wav") == 0))
        {
            if(true == support_mp3)
            {
                strcpy(data->format, "mp3");
            }
            else
            {
                strcpy(data->format, "wma");
            }
        }
        else if((false == support_mp3)&&(strcmp(data->format, "mp3") == 0))
        {
            if(true == support_wav)
            {
                strcpy(data->format, "wav");
            }
            else
            {
                strcpy(data->format, "wma");
            }
        }
        else if((false == support_wma)&&(strcmp(data->format, "wma") == 0))
        {
            if(true == support_wav)
            {
                strcpy(data->format, "wav");
            }
            else
            {
                strcpy(data->format, "mp3");
            }
        }
        else
        {
        }
    }
    
    NO_RECORDER_EXT:
    /*initial menu*/
    if (g_recorder_option_fmt == NULL)
    {
        /*create menu*/
        g_recorder_option_fmt = menu_create();
        if (g_recorder_option_fmt == NULL)
        {
            print_err("can not create option format menu");
            return false;
        }
        else
        {
            /*append items*/
            menu_append_items(g_recorder_option_fmt, option_fmt_item, i_recorder_ext);
            /*set current index*/
            menu_set_cur_index(g_recorder_option_fmt, 0);
        }
    }

    g_recorder_option_fmt->callback = (menu_cb_t)proc_option_fmt;
    return g_recorder_option_fmt;
}

int recorder_recording_option_fmt_deinit(void)
{
    /*delete menu*/
    if (g_recorder_option_fmt != NULL)
    {
        menu_delete(g_recorder_option_fmt);
        g_recorder_option_fmt = NULL;
    }

    return true;
}

bool recorder_recording_option_fmt_enter(menuview_t * menuview)
{
    menu_t *cur_menu;

    cur_menu = recorder_recording_option_fmt_init();
    if(false == cur_menu)
    {
        print_err("fmt menu init err\n");
        return false;
    }
    menuview_set_menu(menuview, cur_menu);
    menuview_paint(menuview);

    return true;
}

bool recorder_recording_option_fmt_exit(menuview_t * menuview)
{
    fusion_type_e fenter;

    fenter = FUSION_BACKWARD;

    /*delete main menu*/
    recorder_recording_option_fmt_deinit();

    /*enter parent menu*/
    recorder_recording_option_enter(menuview, fenter);

    return true;
}

/*根据菜单索引获取选择的录音格式*/
static void _index_map_to_format(int format_index)
{
    if (format_index == 0)
    {
        if(true == support_wav)
        {
            strcpy(g_scene_recording->format, "wav");
        }
        else if(true == support_mp3)
        {
            strcpy(g_scene_recording->format, "mp3");
        }
        else
        {
            strcpy(g_scene_recording->format, "wma");
        }
    }
    else if (format_index == 1)
    {
        if((true == support_wav) && (true == support_mp3))
        {
            strcpy(g_scene_recording->format, "mp3");
        }
        else if((true == support_wav) || (true == support_mp3))
        {
            strcpy(g_scene_recording->format, "wma");
        }
        else
        {
            print_dbg("index unexpected");
        }
    }
    else
    {
        strcpy(g_scene_recording->format, "wma");
    }
}

static void proc_option_fmt(menuview_t *menuview, menuview_sig_e signal)
{
    menu_t *cur_menu = menuview->menu;
    menuitem_t *item;
    int fmt_index = cur_menu->current;
    char temp_format[5]={0};/*记录上次录音格式*/

    switch (signal)
    {
        case WIDGET_SIG_MENUVIEW_OK:

        strcpy(temp_format,g_scene_recording->format);
        /*根据菜单的index获取用户选择的录音格式*/
        _index_map_to_format(fmt_index);
        
        if(strcmp(temp_format, "wav") == 0)
        {
            item = menu_get_item(cur_menu, 0);
            if (item == NULL)
            {
                print_err("menu_get_item err");
                return;
            }
            /*录音格式转换为:wav->wma*/
            if (strcmp(g_scene_recording->format, "wma") == 0)
            {
                /*切换过程需要更换中间件，先关闭监听*/
                stop_monitor(g_scene_recording->mode);
                mmm_ae_close(g_scene_recording->ae_handle, NULL);
                dlclose(g_scene_recording->fptr);
                rmmod("dspenc.ko", 0);

                mount_dsp_dec();
                g_scene_recording->fptr = dlopen("a_record.so", RTLD_LAZY);
                g_scene_recording->ae_handle = mmm_ae_open(NULL);
            }
        }
        else if (strcmp(temp_format, "mp3") == 0)
        {
            if(true == support_wav)
            {
                item = menu_get_item(cur_menu, 1);
            }
            else
            {
                item = menu_get_item(cur_menu, 0);
            }
            if (item == NULL)
            {
                print_err("menu_get_item err");
                return;
            }
            /*录音格式转换为:mp3->wma*/
            if (strcmp(g_scene_recording->format, "wma") == 0)
            {
                /*切换过程需要更换中间件，先关闭监听*/
                stop_monitor(g_scene_recording->mode);
                mmm_ae_close(g_scene_recording->ae_handle, NULL);
                dlclose(g_scene_recording->fptr);
                rmmod("dspenc.ko", 0);

                mount_dsp_dec();
                g_scene_recording->fptr = dlopen("a_record.so", RTLD_LAZY);
                g_scene_recording->ae_handle = mmm_ae_open(NULL);
            }
        }
        else
        {
            if((true == support_wav)&&(true == support_mp3))
            {
                item = menu_get_item(cur_menu, 2);
            }
            else if((true == support_wav)||(true == support_mp3))
            {
                item = menu_get_item(cur_menu, 1);
            }
            else
            {
                item = menu_get_item(cur_menu, 0);
            }
            
            if (item == NULL)
            {
                print_err("menu_get_item err");
                return;
            }
            /*录音格式转换为:wma->wav或wma->mp3*/
            if (strcmp(g_scene_recording->format, "wma") != 0)
            {
                /*切换过程需要更换中间件，先关闭监听*/
                stop_monitor(g_scene_recording->mode);
                mmm_ae_close(g_scene_recording->ae_handle, NULL);
                dlclose(g_scene_recording->fptr);
                rmmod("dspdec.ko", 0);

                mount_dsp_enc();
                g_scene_recording->fptr = dlopen("a_codec.so", RTLD_LAZY);
                g_scene_recording->ae_handle = mmm_ae_open(NULL);
            }
        }

        item->sel_icon_id = 0;
        item->unsel_icon_id = 0;

        item = menu_get_item(cur_menu, fmt_index);
        if (item == NULL)
        {
            print_err("menu_get_item err");
            return;
        }
        item->sel_icon_id = g_scene_recording->rec_select->pic_id;
        item->unsel_icon_id = g_scene_recording->rec_select->pic_id;

        /*设置录音相关参数*/
        if (1 != set_mmm())
        {
            print_warning("mmm set err");
        }
        /*根据录音模式重新打开监听*/
        start_monitor(g_scene_recording->mode);
        /*需要更新可录时间*/
        g_scene_recording->totaltime = NEED_PAINT;
        recorder_draw_totaltime(g_scene_recording);

        menuview_paint(menuview);

        break;

        case WIDGET_SIG_MENUVIEW_QUIT:
        /* return to parent menu*/
        recorder_recording_option_fmt_exit(menuview);
        break;

        default:
        break;
    }
}
