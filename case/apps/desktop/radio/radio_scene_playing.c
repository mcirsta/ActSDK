 /********************************************************************************
 *                              USDK(1100)
 *                             Module: app
 *                 Copyright(c) 2003-2009 Actions Semiconductor,
 *                            All Rights Reserved.
 * File : radio_scene_playing.c
 * History:
 *      <author>    <time>           <version >             <desc>
 *      cheason    2009-04-23          v1.0               build this file
 ********************************************************************************/
/*!
 * \file     radio_scene_playing.c
 * \brief    routine of radio scene playing
 * \author   cheason
 *
 * \version 1.0
 * \date  2009/04/23
 *******************************************************************************/

#include "radio_scene_playing.h"
#include "radio_ui_playing.h"

/* global variable define */
radio_scene_playing_t* g_scene_playing = NULL;
gui_text_encode_t default_encode;
/*需要强制显示电台名称，即使当前不是预设模式--用于自动搜台时强制显示电台名称*/
bool g_force_show_channel_name = false;

/* internal static scene function declaration */
static bool _scene_playing_get_bg_bitmap(void);
static bool _scene_playing_delete_bg_bitmap(void);
static bool _scene_playing_set_default_font(void);
static bool _scene_playing_unset_default_font(void);
static void _scene_playing_paint(radio_scene_playing_t *playing);
static void _scene_playing_style_init(radio_scene_playing_t *data);
static bool _scene_init(fusion_enter_e fenter);
static bool _scene_deinit(void);
static bool _scene_deal_msg(radio_scene_playing_t *playing, gui_wm_message_t *pmsg);
static void _scene_callback(gui_wm_message_t *pmsg);
static bool _check_default_file(radio_scene_playing_t *playing);
static bool _check_path(void);
static unsigned int _atoi(const char *src, int i);
static void _scene_playing_fusion_set_first_frame(scene_t *scene);
static void _scene_playing_fusion_set_last_frame(scene_t *scene);
static const clk_domain_t s_clk_domain =
{ CLK_LOAD_MIN, CLK_LOAD_MAX };
static bool clk_domain_flag = false;

static char channel_default_buf[] = "[FM]\nCH01=087500,_CH01_\nCH02=088800,_CH02_\nCH03=089000,_CH03_\n[END]";
static unsigned int freq_natation[181] =
{
    PIC_FREQ_NOTATION0,
    PIC_FREQ_NOTATION1,
    PIC_FREQ_NOTATION2,
    PIC_FREQ_NOTATION3,
    PIC_FREQ_NOTATION4,
    PIC_FREQ_NOTATION5,
    PIC_FREQ_NOTATION6,
    PIC_FREQ_NOTATION7,
    PIC_FREQ_NOTATION8,
    PIC_FREQ_NOTATION9,

    PIC_FREQ_NOTATION10,
    PIC_FREQ_NOTATION11,
    PIC_FREQ_NOTATION12,
    PIC_FREQ_NOTATION13,
    PIC_FREQ_NOTATION14,
    PIC_FREQ_NOTATION15,
    PIC_FREQ_NOTATION16,
    PIC_FREQ_NOTATION17,
    PIC_FREQ_NOTATION18,
    PIC_FREQ_NOTATION19,

    PIC_FREQ_NOTATION20,
    PIC_FREQ_NOTATION21,
    PIC_FREQ_NOTATION22,
    PIC_FREQ_NOTATION23,
    PIC_FREQ_NOTATION24,
    PIC_FREQ_NOTATION25,
    PIC_FREQ_NOTATION26,
    PIC_FREQ_NOTATION27,
    PIC_FREQ_NOTATION28,
    PIC_FREQ_NOTATION29,

    PIC_FREQ_NOTATION30,
    PIC_FREQ_NOTATION31,
    PIC_FREQ_NOTATION32,
    PIC_FREQ_NOTATION33,
    PIC_FREQ_NOTATION34,
    PIC_FREQ_NOTATION35,
    PIC_FREQ_NOTATION36,
    PIC_FREQ_NOTATION37,
    PIC_FREQ_NOTATION38,
    PIC_FREQ_NOTATION39,

    PIC_FREQ_NOTATION40,
    PIC_FREQ_NOTATION41,
    PIC_FREQ_NOTATION42,
    PIC_FREQ_NOTATION43,
    PIC_FREQ_NOTATION44,
    PIC_FREQ_NOTATION45,
    PIC_FREQ_NOTATION46,
    PIC_FREQ_NOTATION47,
    PIC_FREQ_NOTATION48,
    PIC_FREQ_NOTATION49,

    PIC_FREQ_NOTATION50,
    PIC_FREQ_NOTATION51,
    PIC_FREQ_NOTATION52,
    PIC_FREQ_NOTATION53,
    PIC_FREQ_NOTATION54,
    PIC_FREQ_NOTATION55,
    PIC_FREQ_NOTATION56,
    PIC_FREQ_NOTATION57,
    PIC_FREQ_NOTATION58,
    PIC_FREQ_NOTATION59,

    PIC_FREQ_NOTATION60,
    PIC_FREQ_NOTATION61,
    PIC_FREQ_NOTATION62,
    PIC_FREQ_NOTATION63,
    PIC_FREQ_NOTATION64,
    PIC_FREQ_NOTATION65,
    PIC_FREQ_NOTATION66,
    PIC_FREQ_NOTATION67,
    PIC_FREQ_NOTATION68,
    PIC_FREQ_NOTATION69,

    PIC_FREQ_NOTATION70,
    PIC_FREQ_NOTATION71,
    PIC_FREQ_NOTATION72,
    PIC_FREQ_NOTATION73,
    PIC_FREQ_NOTATION74,
    PIC_FREQ_NOTATION75,
    PIC_FREQ_NOTATION76,
    PIC_FREQ_NOTATION77,
    PIC_FREQ_NOTATION78,
    PIC_FREQ_NOTATION79,

    PIC_FREQ_NOTATION80,
    PIC_FREQ_NOTATION81,
    PIC_FREQ_NOTATION82,
    PIC_FREQ_NOTATION83,
    PIC_FREQ_NOTATION84,
    PIC_FREQ_NOTATION85,
    PIC_FREQ_NOTATION86,
    PIC_FREQ_NOTATION87,
    PIC_FREQ_NOTATION88,
    PIC_FREQ_NOTATION89,

    PIC_FREQ_NOTATION90,
    PIC_FREQ_NOTATION91,
    PIC_FREQ_NOTATION92,
    PIC_FREQ_NOTATION93,
    PIC_FREQ_NOTATION94,
    PIC_FREQ_NOTATION95,
    PIC_FREQ_NOTATION96,
    PIC_FREQ_NOTATION97,
    PIC_FREQ_NOTATION98,
    PIC_FREQ_NOTATION99,

    PIC_FREQ_NOTATION100,
    PIC_FREQ_NOTATION101,
    PIC_FREQ_NOTATION102,
    PIC_FREQ_NOTATION103,
    PIC_FREQ_NOTATION104,
    PIC_FREQ_NOTATION105,
    PIC_FREQ_NOTATION106,
    PIC_FREQ_NOTATION107,
    PIC_FREQ_NOTATION108,
    PIC_FREQ_NOTATION109,

    PIC_FREQ_NOTATION110,
    PIC_FREQ_NOTATION111,
    PIC_FREQ_NOTATION112,
    PIC_FREQ_NOTATION113,
    PIC_FREQ_NOTATION114,
    PIC_FREQ_NOTATION115,
    PIC_FREQ_NOTATION116,
    PIC_FREQ_NOTATION117,
    PIC_FREQ_NOTATION118,
    PIC_FREQ_NOTATION119,

    PIC_FREQ_NOTATION120,
    PIC_FREQ_NOTATION121,
    PIC_FREQ_NOTATION122,
    PIC_FREQ_NOTATION123,
    PIC_FREQ_NOTATION124,
    PIC_FREQ_NOTATION125,
    PIC_FREQ_NOTATION126,
    PIC_FREQ_NOTATION127,
    PIC_FREQ_NOTATION128,
    PIC_FREQ_NOTATION129,

    PIC_FREQ_NOTATION130,
    PIC_FREQ_NOTATION131,
    PIC_FREQ_NOTATION132,
    PIC_FREQ_NOTATION133,
    PIC_FREQ_NOTATION134,
    PIC_FREQ_NOTATION135,
    PIC_FREQ_NOTATION136,
    PIC_FREQ_NOTATION137,
    PIC_FREQ_NOTATION138,
    PIC_FREQ_NOTATION139,

    PIC_FREQ_NOTATION140,
    PIC_FREQ_NOTATION141,
    PIC_FREQ_NOTATION142,
    PIC_FREQ_NOTATION143,
    PIC_FREQ_NOTATION144,
    PIC_FREQ_NOTATION145,
    PIC_FREQ_NOTATION146,
    PIC_FREQ_NOTATION147,
    PIC_FREQ_NOTATION148,
    PIC_FREQ_NOTATION149,

    PIC_FREQ_NOTATION150,
    PIC_FREQ_NOTATION151,
    PIC_FREQ_NOTATION152,
    PIC_FREQ_NOTATION153,
    PIC_FREQ_NOTATION154,
    PIC_FREQ_NOTATION155,
    PIC_FREQ_NOTATION156,
    PIC_FREQ_NOTATION157,
    PIC_FREQ_NOTATION158,
    PIC_FREQ_NOTATION159,

    PIC_FREQ_NOTATION160,
    PIC_FREQ_NOTATION161,
    PIC_FREQ_NOTATION162,
    PIC_FREQ_NOTATION163,
    PIC_FREQ_NOTATION164,
    PIC_FREQ_NOTATION165,
    PIC_FREQ_NOTATION166,
    PIC_FREQ_NOTATION167,
    PIC_FREQ_NOTATION168,
    PIC_FREQ_NOTATION169,

    PIC_FREQ_NOTATION170,
    PIC_FREQ_NOTATION171,
    PIC_FREQ_NOTATION172,
    PIC_FREQ_NOTATION173,
    PIC_FREQ_NOTATION174,
    PIC_FREQ_NOTATION175,
    PIC_FREQ_NOTATION176,
    PIC_FREQ_NOTATION177,
    PIC_FREQ_NOTATION178,
    PIC_FREQ_NOTATION179,

    PIC_FREQ_NOTATION180

};
/*字符串转换为整形*/
static unsigned int _atoi(const char *src, int i)
{
    int total = 0;

    while (i != 0)
    {
        total = total * 10 + (int) (*src - '0');
        src++;
        i=i-1;
    }

    return total;
}

/******************************************************************************/
/*!                    
 * \par  Description:     get background bitmap
*******************************************************************************/
static bool _scene_playing_get_bg_bitmap(void)
{
    if (g_scene_playing->bg_bitmap_info != NULL)
    {
        return false;
    }

    g_scene_playing->bg_bitmap_info = get_user_bg_pic(BG_RECORDER);
    if (NULL == g_scene_playing->bg_bitmap_info)
    {
        /*获取用户自定义背景图失败，使用ui-editor上添加的默认图*/
        g_scene_playing->bg_bitmap_info = sty_create_bitmap((int) g_scene_playing->pic_bg->pic_id);
    }

    if (NULL == g_scene_playing->bg_bitmap_info)
    {
        print_err("get backgroud bitmap failed!");
        return false;
    }

    return true;
}

/******************************************************************************/
/*!                    
 * \par  Description:     delete background bitmap
*******************************************************************************/
static bool _scene_playing_delete_bg_bitmap(void)
{
    if (g_scene_playing->bg_bitmap_info == NULL)
    {
        return false;
    }

    sty_delete_bitmap(g_scene_playing->bg_bitmap_info);
    g_scene_playing->bg_bitmap_info = NULL;

    return true;
}

/******************************************************************************/
/*!                    
 * \par  Description:     set default font
*******************************************************************************/
static bool _scene_playing_set_default_font(void)
{
    /* crate font */
    if (g_scene_playing->fontdes > 0)
    {
        return false;
    }

    g_scene_playing->fontdes = gui_create_font((unsigned char *) sys_get_default_font_file(), 16);
    if (g_scene_playing->fontdes < 0)
    {
        print_err("call gui_create_font failed");
        return false;
    }

    if (gui_dc_set_default_fontface(g_scene_playing->fontdes) < 0)
    {
        print_err("gui_dc_set_default_fontface failed");
        return false;
    }

    return true;
}

/******************************************************************************/
/*!                    
 * \par  Description:     unset default font
*******************************************************************************/
static bool _scene_playing_unset_default_font(void)
{
    /* crate font */
    if (g_scene_playing->fontdes <= 0)
    {
        return false;
    }

    gui_destroy_font(g_scene_playing->fontdes);
    g_scene_playing->fontdes = 0;

    return true;
}

/*
 * 检查频率的有效性
 * return:
 * false:非法
 */
bool check_freq(option_band_item_e band, int freq)
{
    if (BAND_JAPAN == band)
    {
        if ((freq < FREQ_MIN_JP) || (freq > FREQ_MAX_JP) || ((freq % 100) != 0))
        {
            return false;
        }
    }
    else if (BAND_USA == band)
    {
        if ((freq < FREQ_MIN_US) || (freq > FREQ_MAX_US) || ((freq % 100) != 0))
        {
            return false;
        }
    }
    else
    {
        if ((freq < FREQ_MIN_US) || (freq > FREQ_MAX_US) || ((freq % 50) != 0))
        {
            return false;
        }
    }
    return true;
}

/******************************************************************************/
/*!                    
 * \par  Description:     check path
*******************************************************************************/
static bool _check_path(void)
{
    DIR *fp;
    DIR *fp1;
    int result;

    fp = opendir(sys_path);
    if (fp == NULL)
    {
        result = mkdir(sys_path, 0x777);
        if (result < 0)
        {
            return false;
        }
    }
    else
    {
        closedir(fp);
    }

    fp1 = opendir(sys_fm_path);
    if (fp1 == NULL)
    {
        result = mkdir(sys_fm_path, 0x777);
        if (result < 0)
        {
            return false;
        }
    }
    else
    {
        closedir(fp1);
    }

    return true;

}

/******************************************************************************/
/*!                    
 * \par  Description:     check userchannel file 
*******************************************************************************/
static bool _check_userchannel_file(void)
{
    int result;
    int i = 0,k = 0;
    int fd;
    struct stat file_stat;
    char *file_content;
    bool ret;
    
    fd = open(userchannel_path, O_RDWR);
    fcntl(fd, F_BUILDINDEX, 0);
    
    result = stat(userchannel_path, &file_stat);
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

    for (i = 0; i < MAX_CHANNEL_NUM; i++)
    {
        if ((check_freq(g_scene_playing->cur_band, (int)(channel_info.channel_freq[i])) == true)
                && (channel_info.channel_freq[i] != 0))
        {
            continue;
        }
        else
        {
            channel_info.channel_freq[i] = 0;
            sprintf(channel_info.channel_name[i], "---");
            k++;
        }
    }

    if (k > 20)
    {
        return false;
    }

    return true;
}

/******************************************************************************/
/*!                    
 * \par  Description:     check default file for auto seek
*******************************************************************************/
static bool _check_default_file(radio_scene_playing_t *playing)
{

    int result;
    bool path_ret;
    bool parse_ret;
    struct stat file_stat;
    char utf8_bom[3] = {0xEF,0xBB,0xBF};

    path_ret = _check_path();
    if (false == path_ret)
    {
        return false;
    }

    sprintf(channel_list_file, "%s%s.ini", sys_fm_path, "default");

    playing->fd = open(channel_list_file, O_RDWR | O_EXCL | O_CREAT);

    if (playing->fd < 0)
    {
        
        /*读文件状态*/
        result = stat(channel_list_file, &file_stat);
        if (result < 0)
        {
            print_err();
            return false;
        }

        playing->fd = open(channel_list_file, O_RDWR);
        if (playing->fd < 0)
        {
            print_err();
            return false;
        }

        fcntl(playing->fd, F_BUILDINDEX, 0);

        if (0 == file_stat.st_size)
        {
            goto write_label;
        }
        else
        {
            char *buf = malloc(file_stat.st_size);
            if (buf == NULL)
            {
                return false;
            }

            result = read(playing->fd, buf, file_stat.st_size);
            if (result < 0)
            {
                print_err();
                free(buf);
                return false;
            }

            /*call 文件解析函数*/
            parse_ret = parse_ini_file(&playing->channel_list_info, buf, file_stat.st_size);
            if (parse_ret == false)
            {
                close(playing->fd);
                playing->fd = -1;
            }
            free(buf);
        }
    }
    else
    {
        write_label:
            
        /*写utf8文件头*/
        write(playing->fd, utf8_bom, sizeof(utf8_bom));
        
        /*写文件*/
        result = write(playing->fd, channel_default_buf, sizeof(channel_default_buf));
        if (result < 0)
        {
            print_err();
            close(playing->fd);
            playing->fd = -1;
            return false;
        }

        /*call 文件解析函数*/
        parse_ret = parse_ini_file(&playing->channel_list_info, channel_default_buf, sizeof(channel_default_buf));
        if (parse_ret == false)
        {
            close(playing->fd);
            playing->fd = -1;
            return false;
        }
    }

    playing->parse_default_file_flag = true;
    
    return true;
}

/******************************************************************************/
/*!                    
 * \par  Description:     parse file for auto seek 
*******************************************************************************/
bool parse_ini_file(channel_info_t *radio_channel_info, char *file_buf, unsigned int file_buf_lenth)
{
    bool result = true;
    int i = 0;
    int k = 0;
    char tmp_head[FILE_FREQ_LENTH] = { 0 };
    unsigned int freq;
    int lo_index = 0;
    char utf8_bom[3] = {0xEF,0xBB,0xBF};
    char unicode_bom[2] = {0xFF,0xFE};
    unsigned int utf8_lenth;
    char *utf8_buf = NULL;
    
    memset(radio_channel_info, 0, sizeof(channel_info_t));
       
    print_dbg("\nfile_buf:\n %s",file_buf);
    if (memcmp(file_buf, unicode_bom, sizeof(unicode_bom)) == 0)
    {
        print_info("file is in unicode encoding");
        utf8_lenth = file_buf_lenth * 2;
        utf8_buf = (char *)malloc( utf8_lenth );
        if( NULL == utf8_buf )
        {
            print_err("malloc utf8 buffer error\n");
            return false;
        }
        if(mlang_unicode_to_utf8((unsigned short *)file_buf, (int)file_buf_lenth, utf8_buf,&utf8_lenth) < 0)
        {
            print_err("mlang_unicode_to_utf8 error\n");
            result = false;
            goto ERR_NEED_FREE_BUF;
        }

        /*skip unicode bom */
        utf8_buf += sizeof(utf8_bom);
    }   
    else if (memcmp(file_buf, utf8_bom, sizeof(utf8_bom)) == 0)
    {
        print_info("file is in utf8 encoding");
        utf8_buf = file_buf;
        utf8_lenth = file_buf_lenth;
        /*skip utf8 bom */
        utf8_buf += sizeof(utf8_bom);
    }
    else
    {
        print_info("file is in mbcs encoding");
        utf8_lenth = file_buf_lenth * 3 ;
        utf8_buf = (char *)malloc( utf8_lenth );
        if( NULL == utf8_buf )
        {
            print_err("malloc utf8 buffer error\n");
            return false;
        }
        if(mlang_mbcs_to_utf8((unsigned short *)file_buf, (int)file_buf_lenth, 
            utf8_buf, &utf8_lenth, sys_get_cur_language_id() ) < 0 )
        {
            print_err("mlang_mbcs_to_utf8 error\n");
            result = false;
            goto ERR_NEED_FREE_BUF;
        }

        /*mbcs 转 utf8后没有utf8文件头，故不用跳过utf8头*/
        /*skip unicode bom */
//        utf8_buf += sizeof(utf8_bom);
    }

    if (memcmp(utf8_buf, "[FM]", FILE_HEAD_LENTH) != 0)
    {
        print_err("file content format error");
        result = false;
        goto ERR_NEED_FREE_BUF;
    }

    while ((utf8_buf[i]) && (utf8_buf[i] != 'C'))
    {
        i++;
    }
    
    while ((i < (int)utf8_lenth) && (lo_index < MAX_CHANNEL_NUM))
    {
        if ((utf8_buf[i] == '[') && (utf8_buf[i + 1] == 'E') && (utf8_buf[i + 2] == 'N')
                && (utf8_buf[i + 3] == 'D') && (utf8_buf[i + 4] == ']'))
        {
            break;/*文件结尾退出*/
        }
        if ((utf8_buf[i] == ',') && ((i - k) > 10))
        {
            memcpy(tmp_head, utf8_buf + i - FILE_FREQ_LENTH, FILE_FREQ_LENTH);
            freq = _atoi(tmp_head, FILE_FREQ_LENTH);
            if ((freq >= FREQ_MIN_JP) && (freq <= FREQ_MAX_US) && ((freq % 50) == 0))
            {
                radio_channel_info->channel_freq[lo_index] = freq;
                radio_channel_info->channel_num++;
            }
            else
            {
                radio_channel_info->channel_freq[lo_index] = 0;
            }
            k = i + 1;
        }
        else if (((utf8_buf[i]) == '\n') && ((i - k) < PATH_MAX))
        {
            if (radio_channel_info->channel_freq[lo_index] != 0)
            {
                memcpy(radio_channel_info->channel_name[lo_index], utf8_buf + k, (unsigned int)(i - k));
                radio_channel_info->channel_name[lo_index][i - k] = '\0';
            }
            lo_index++;
        }
        else
        {
        }
        i++;
    }
    if (radio_channel_info->channel_num > 20)
    {
        result = false;
        goto ERR_NEED_FREE_BUF;
    }

    ERR_NEED_FREE_BUF:
    if( NULL != utf8_buf )
    {
        free( utf8_buf );
        utf8_buf = NULL;            
    }
    return result;    
}

/******************************************************************************/
/*!                    
 * \par  Description:     save default file
*******************************************************************************/
bool save_default_file(radio_scene_playing_t *playing)
{
    int i = 0;
    unsigned int len;
    int result;
    char *savefp;
    char line_info[PATH_MAX + 17];
    char utf8_bom[3] = {0xEF,0xBB,0xBF};
    unsigned int size = MAX_CHANNEL_NUM * (PATH_MAX + 17);

    if(playing == NULL)
    {
        return false; 
    }
    if (playing->fd > 0)
    {
        close(playing->fd);
        playing->fd = open(channel_list_file, O_RDWR | O_TRUNC);

        savefp = (char *) malloc(size);
        if (savefp == NULL)
        {
            return false;
        }

        memset(savefp, 0, size);

        sprintf(savefp, "[FM]\n");

        while (i < MAX_CHANNEL_NUM)
        {
            sprintf(line_info, "CH%.2d=%.6d,%s\n", i + 1, playing->channel_list_info.channel_freq[i],
                    playing->channel_list_info.channel_name[i]);
            i++;
            strcat(savefp, line_info);
        }

        strcat(savefp, "[END]");
       

        len = strlen(savefp);
        if( default_encode == GUI_UTF8_ENC)
        {
            /*写utf8文件头*/
            write(playing->fd, utf8_bom, sizeof(utf8_bom));
        }
        else
        {
//            lseek(playing->fd, 0, SEEK_SET);
        }
        
        result = write(playing->fd, savefp, len);
        if (result < 0)
        {
            free(savefp);
            return false;
        }

        free(savefp);

    }

    return true;
}


static bool _close_default_file(radio_scene_playing_t *playing)
{
    int result;

    if( playing->fd > 0 )
    {        
        result = close(playing->fd);
        if (result < 0)
        {
            return false;
        }
        playing->fd = -1; 
    }
    
    return true;
}

/* scene quit */
/*
 int radio_scene_playing_quit()
 {
 msg_apps_t m;
 m.type = MSG_QUIT;

 return send_async_msg(RADIO_APP_NAME, &m);
 }*/
 
/******************************************************************************/
/*!                    
 * \par  Description:     option callback
*******************************************************************************/
static void _option_cb(menuview_t *list_data, menuview_sig_e signal)
{
    switch (signal)
    {
        case WIDGET_SIG_MENUVIEW_QUIT:
            
        radio_playing_widget_option_delete(g_scene_playing->option_menu);
        g_scene_playing->option_menu = NULL;
        
        break;

        default:
        break;
    }
}

/******************************************************************************/
/*!                    
 * \par  Description:     headbar callback
*******************************************************************************/
static void _headbar_cb(headbar_t *headbar, widget_sig_headbar_e signal)
{

}

/******************************************************************************/
/*!                    
 * \par  Description:     volumebar callback
*******************************************************************************/
static void _volumebar_cb(volumebar_t *volbar, volumebar_sig_e signal)
{
    switch (signal)
    {
        case WIDGET_SIG_VOLUMEBAR_CHANGED:
        break;

        case WIDGET_SIG_VOLUMEBAR_QUIT:
        volumebar_delete(volbar);
        g_scene_playing->volumebar = NULL;
        break;

        default:
        break;
    }
}
#if 0
static void _dialog_cb(dialog_t* dialog, widget_sig_dialog_e signal)
{
    switch (signal)
    {
        case WIDGET_SIG_DIALOG_OK:
        break;

        case WIDGET_SIG_DIALOG_CANCAL:
        /*! delete dialog */
        if (g_scene_playing->dialog != NULL)
        {
            dialog_delete(g_scene_playing->dialog);
            g_scene_playing->dialog = NULL;
        }

        //scene quit
        g_scene_playing->next_scene = RADIO_SCENE_EXIT;
        exit_msg_loop();
        break;

        default:
        break;
    }
}
static void _dialog_cb_linein(dialog_t* dialog, widget_sig_dialog_e signal)
{
    switch (signal)
    {
        case WIDGET_SIG_DIALOG_OK:
        break;

        case WIDGET_SIG_DIALOG_CANCAL:
        /*! delete dialog */
        if (g_scene_playing->dialog_linein != NULL)
        {
            dialog_delete(g_scene_playing->dialog_linein);
            g_scene_playing->dialog_linein = NULL;
        }

        //scene quit
        g_scene_playing->next_scene = RADIO_SCENE_EXIT;
        exit_msg_loop();
        break;

        default:
        break;
    }
}
#endif
/******************************************************************************/
/*!                    
 * \par  Description:     msgbox callback
*******************************************************************************/
static void _msgbox_cb(msgbox_t * msgbox, msgbox_sig_e signal)
{
    switch (signal)
    {
        case WIDGET_SIG_MSGBOX_SHOW:
        print_dbg("show message box");
        break;
        case WIDGET_SIG_MSGBOX_QUIT:
        print_dbg("quit message box");
        break;
        default:
        break;
    }
}
#if 0
#ifndef REAL_AUTO_SEEK
static void _scene_timer(void * param)
{
    radio_scene_playing_t *playing = (radio_scene_playing_t *) param;
    int max, min;
    int first_preset, num;

    int old_intensity = playing->cur_intensity;
    bool old_stereo = playing->cur_stereo;
    int old_freq = playing->cur_frequency;

    /* 自动搜索 */
    if (NORMAL_PLAY != playing->searching_flag)
    {
        get_band_range(playing->cur_band, &max, &min);
        if (AUTO_SEARCH == playing->searching_flag)
        {
            playing->cur_frequency += get_band_step(playing->cur_band);
            num = playing->channel_list_info.channel_num;

            if ((playing->cur_frequency > max) || (num >= MAX_CHANNEL_NUM))
            {
                /* 已经搜索到最大频率值，或者保存了足够多的频道 */
                first_preset = playing->channel_list_info.channel_freq[0];
                playing->cur_frequency = (first_preset == 0) ? min : first_preset;
                playing->searching_flag = NORMAL_PLAY;
                fm_engine_msg(MSG_FMENGINE_SET_FREQ, &playing->cur_frequency, sizeof(int), NULL);
            }
            else
            {
                if (0 == fm_engine_msg(MSG_FMENGINE_SET_FREQ, &playing->cur_frequency, sizeof(int), NULL))
                {
                    /* 保存电台，注意，自动搜索时总是先将列表清空，然后从第一个开始填 */
                    playing->channel_list_info.channel_freq[num] = playing->cur_frequency;
                    sprintf(playing->channel_list_info.channel_name[num], "_CH%02d_", num + 1);
                    playing->channel_list_info.channel_num++;
                }
            }
        }
        else if (HALF_AUTO_SEARCH_ADD == playing->searching_flag) /* 半自动前向搜索 */
        {
            playing->cur_frequency += get_band_step(playing->cur_band);
            if (playing->cur_frequency > max)
            {
                /* 已经搜索到最大频率值 */
                playing->cur_frequency = max;
                playing->searching_flag = NORMAL_PLAY;
                fm_engine_msg(MSG_FMENGINE_SET_FREQ, &playing->cur_frequency, sizeof(int), NULL);
            }
            else
            {
                if (0 == fm_engine_msg(MSG_FMENGINE_SET_FREQ, &playing->cur_frequency, sizeof(int), NULL))
                {
                    playing->searching_flag = NORMAL_PLAY;
                }
            }
        }
        else
        //if(HALF_AUTO_SEARCH_MINUS == playing->searching_flag) /* 半自动后向搜索 */
        {
            playing->cur_frequency -= get_band_step(playing->cur_band);
            if (playing->cur_frequency < min)
            {
                /* 已经搜索到最大频率值 */
                playing->cur_frequency = min;
                playing->searching_flag = NORMAL_PLAY;
                fm_engine_msg(MSG_FMENGINE_SET_FREQ, &playing->cur_frequency, sizeof(int), NULL);
            }
            else
            {
                if (0 == fm_engine_msg(MSG_FMENGINE_SET_FREQ, &playing->cur_frequency, sizeof(int), NULL))
                {
                    playing->searching_flag = NORMAL_PLAY;
                }
            }
        }

        if (NORMAL_PLAY == playing->searching_flag)
        {
            modify_timer(playing->timer_id, 500);
        }
    }

    /* 读取当前状态并显示 */
    //fm_engine_msg(MSG_FMENGINE_GET_INFO, NULL, 0, &cur_info);
    //playing->cur_stereo = (cur_info.stereo_status == 1);
    //playing->cur_intensity = cur_info.intensity;
    //playing->cur_frequency = cur_info.current_freq;

    fm_engine_msg(MSG_FMENGINE_GET_STEREO_MODE, NULL, 0, &playing->cur_stereo);
    fm_engine_msg(MSG_FMENGINE_GET_INTENSITY, NULL, 0, &playing->cur_intensity);
    //fm_engine_msg(MSG_FMENGINE_GET_FREQ, NULL, 0, &playing->cur_frequency);
    //fm_engine_msg(MSG_FMENGINE_GET_BAND_MODE, NULL, 0, &playing->cur_band);

    if (old_stereo != playing->cur_stereo)
        show_stereo_pic(playing, true);

    if (old_intensity != playing->cur_intensity)
        show_intensity_pic(playing, true);

    if (old_freq != playing->cur_frequency)
        show_cur_freq(playing, true);
}
#else
static void _scene_tmp_timer(void * param)
{
    radio_scene_playing_t *playing = (radio_scene_playing_t *) param;
    //   stop_timer(playing->timer_id_tmp);
    restart_timer(playing->timer_id);
    //    printf("_scene_tmp_timer\n");

}

static void _scene_timer(void * param)
{
    radio_scene_playing_t *playing = (radio_scene_playing_t *) param;
    int max, min;
    int first_preset, num;

    int old_intensity = playing->cur_intensity;
    bool old_stereo = playing->cur_stereo;
    int old_freq = playing->cur_frequency;
    int ret_eg;
    struct radio_info_t *pfm_info;
    bool stop_tm = false;
    struct radio_seek_info_t seek_info;

    pfm_info = (struct radio_info_t *)malloc(sizeof(struct radio_info_t));
    if(NULL != pfm_info)
    {
        memset(pfm_info, 0, sizeof(struct radio_info_t));
    }
    else
    {
        return;
    }

    get_band_range(playing->cur_band, &max, &min);

    /* 自动搜索 */
    if(NORMAL_PLAY != playing->searching_flag)
    {

        memset(&seek_info, 0, sizeof(struct radio_seek_info_t));
        
        if(AUTO_SEARCH == playing->searching_flag)
        {
            // playing->cur_frequency += get_band_step(playing->cur_band);
            num = playing->channel_list_info.channel_num;
            //            printf("num is %d\n",num);

            if((playing->cur_frequency >= max) || (num >= MAX_CHANNEL_NUM))
            {
                /* 已经搜索到最大频率值，或者保存了足够多的频道 */
                first_preset = playing->channel_list_info.channel_freq[0];
                playing->cur_frequency = (first_preset == 0) ? min : first_preset;
                playing->searching_flag = NORMAL_PLAY;
                fm_engine_msg(MSG_FMENGINE_SET_FREQ, &playing->cur_frequency, sizeof(int), NULL);
                if(0 != playing->timer_id_tmp)
                {
                    kill_timer(playing->timer_id_tmp);
                }
            }
            else
            {
                if((playing->cur_frequency == min) && (0 == playing->timer_id_tmp))
                {
                    if(0 == fm_engine_msg(MSG_FMENGINE_SET_FREQ, &playing->cur_frequency, sizeof(int), NULL))
                    {

                        playing->channel_list_info.channel_freq[num] = playing->cur_frequency;
                        sprintf(playing->channel_list_info.channel_name[num], "_CH%02d_", num + 1);
                        playing->channel_list_info.channel_num++;
                        num = playing->channel_list_info.channel_num;
                        fm_engine_msg(MSG_FMENGINE_GET_STEREO_MODE, NULL, 0, &playing->cur_stereo);
                        fm_engine_msg(MSG_FMENGINE_GET_INTENSITY, NULL, 0, &playing->cur_intensity);
                        if(old_stereo != playing->cur_stereo)
                        {
                            show_stereo_pic(playing, true);
                        }

                        if (old_intensity != playing->cur_intensity)
                        {
                            show_intensity_pic(playing, true);
                        }
                        if(old_freq != playing->cur_frequency)
                        {
                            show_cur_freq(playing, true);
                        }
                        stop_timer (playing->timer_id);
                        playing->timer_id_tmp = set_single_shot_timer (2000, (timer_proc)_scene_tmp_timer, playing);
                        free(pfm_info);
                        return;
                    }

                }

                if(true == playing->auto_search_start)
                {
                    playing->auto_search_start = false;
                    seek_info.search_direction = AUTO_ADD;
                    seek_info.search_freq = playing->cur_frequency;
                    seek_info.search_step = 1;
                    ret_eg = fm_engine_msg(MSG_FMENGINE_SET_AUTO_SEEK, &seek_info,
                                            sizeof(struct radio_seek_info_t), NULL);
                    if(0 == ret_eg)
                    {
                        print_dbg("start auto search ok\n");
                    }
                    else
                    {
                        print_dbg("ret_eg is %d\n",ret_eg);
                    }

                }
                if(0!=fm_engine_msg( MSG_FMENGINE_GET_INFO,NULL,0,pfm_info))
                {
                    free(pfm_info);
                    return;
                }
                if(pfm_info->current_freq < max)
                {
                    playing->cur_frequency = pfm_info->current_freq;
                }
                else
                {
                    playing->cur_frequency = max;//exit auto seek
                    free(pfm_info);
                    return;
                }
                
                switch( pfm_info->seek_status)
                {
                    case INVALID_STATION://修改宏名EBADMSG://搜索到一个电台，但此电台不是有效电台

                    {
                        playing->auto_search_start = true;

                        break;
                    }
                    case FOUND_NOSTAION://EINPROGRESS，搜索过程正在进行

                    {
                        print_dbg("on_seeking show\n");
                        break;
                    }
                    case FOUND_STATION:
                    {
                        print_dbg("seeked,play,show\n");
                        //stop timer,set flag2 for another timer2s,
                        //...
                        playing->auto_search_start = true;
                        stop_tm = true;/*显示完频率后根据此值   定时器暂停2秒播放频道*/
                        /* 保存电台，注意，自动搜索时总是先将列表清空，然后从第一个开始填 */
                        //   playing->cur_frequency = pfm_info->current_freq;
                        playing->channel_list_info.channel_freq[num] = playing->cur_frequency;
                        sprintf(playing->channel_list_info.channel_name[num], "_CH%02d_", num + 1);
                        playing->channel_list_info.channel_num++;
                        break;
                    }
                    default:
                    break;

                }

                //      if(0 == fm_engine_msg(MSG_FMENGINE_SET_FREQ, &playing->cur_frequency, sizeof(int), NULL))
                //     {


                //   }

            }
        }
        else if(HALF_AUTO_SEARCH_ADD == playing->searching_flag) /* 半自动前向搜索 */
        {
            //     playing->cur_frequency += get_band_step(playing->cur_band);
            if(playing->cur_frequency >= max)
            {
                /* 已经搜索到最大频率值 */
                playing->cur_frequency = max;
                playing->searching_flag = NORMAL_PLAY;
                //fm_engine_msg(MSG_FMENGINE_SET_FREQ, &playing->cur_frequency, sizeof(int), NULL);
            }
            else
            {
                if(true == playing->auto_search_start)
                {
                    seek_info.search_direction = AUTO_ADD;
                    seek_info.search_freq = playing->cur_frequency;
                    seek_info.search_step = 1;
                    ret_eg = fm_engine_msg(MSG_FMENGINE_SET_AUTO_SEEK, &seek_info,
                                                sizeof(struct radio_seek_info_t), NULL);

                    if(0 == ret_eg)
                    {
                    }
                    playing->auto_search_start = false;
                }
                if(0!=fm_engine_msg( MSG_FMENGINE_GET_INFO,NULL,0,pfm_info))
                {
                    free(pfm_info);
                    return;
                }
                playing->cur_frequency = pfm_info->current_freq;
                if( FOUND_NOSTAION == pfm_info->seek_status)
                {
                    print_dbg("on_seeking show\n");
                }
                if(FOUND_STATION == pfm_info->seek_status)
                {
                    print_dbg("seeked,play,show\n");
                    playing->searching_flag = NORMAL_PLAY;
                }
                if( INVALID_STATION == pfm_info->seek_status)//搜索到一个电台，但此电台不是有效电台

                {
                    playing->auto_search_start = true;
                    //       playing->searching_flag = HALF_AUTO_SEARCH_ADD;


                }
            }
        }

        else
        {
            //     playing->cur_frequency += get_band_step(playing->cur_band);
            if(playing->cur_frequency <= min)
            {
                /* 已经搜索到最小频率值 */
                playing->cur_frequency = min;
                playing->searching_flag = NORMAL_PLAY;
                //fm_engine_msg(MSG_FMENGINE_SET_FREQ, &playing->cur_frequency, sizeof(int), NULL);
            }
            else
            {
                if(true == playing->auto_search_start)
                {
                    seek_info.search_direction = AUTO_MINUS;
                    seek_info.search_freq = playing->cur_frequency;
                    seek_info.search_step = 1;
                    ret_eg = fm_engine_msg(MSG_FMENGINE_SET_AUTO_SEEK, &seek_info, 
                                            sizeof(struct radio_seek_info_t), NULL);

                    if(0 != ret_eg)
                    {
                        free(pfm_info);
                        return;
                    }
                    playing->auto_search_start = false;
                }
                if(0!=fm_engine_msg( MSG_FMENGINE_GET_INFO,NULL,0,pfm_info))
                {
                    free(pfm_info);
                    return;
                }
                playing->cur_frequency = pfm_info->current_freq;
                if( FOUND_NOSTAION == pfm_info->seek_status)
                {
                    print_dbg("on_seeking show\n");
                }
                if(FOUND_STATION == pfm_info->seek_status)
                {
                    print_dbg("seeked,play,show\n");
                    playing->searching_flag = NORMAL_PLAY;
                }
                if( INVALID_STATION == pfm_info->seek_status)//搜索到一个电台，但此电台不是有效电台

                {
                    playing->auto_search_start = true;
                    //                        playing->searching_flag = HALF_AUTO_SEARCH_MINUS;

                }

            }
            //if(HALF_AUTO_SEARCH_MINUS == playing->searching_flag) /* 半自动后向搜索 */

        }

        if(NORMAL_PLAY == playing->searching_flag)
        {
            modify_timer(playing->timer_id, 500);
        }
    }

    /* 读取当前状态并显示 */
    //fm_engine_msg(MSG_FMENGINE_GET_INFO, NULL, 0, &cur_info);
    //playing->cur_stereo = (cur_info.stereo_status == 1);
    //playing->cur_intensity = cur_info.intensity;
    //playing->cur_frequency = cur_info.current_freq;
    //playing->cur_intensity = pfm_info->intensity;
    //printf("!!!!!!!!!!!!!!!!! pfm_info->intensity is %d,old is %d\n",pfm_info->intensity,old_intensity);
    //playing->cur_stereo = (pfm_info->stereo_status == 1);

    free(pfm_info);
    pfm_info = NULL;

    if(0 != fm_engine_msg(MSG_FMENGINE_GET_FREQ, NULL, 0, &playing->cur_frequency))
    {
        return;
    }

    if(0 != fm_engine_msg(MSG_FMENGINE_GET_STEREO_MODE, NULL, 0, &playing->cur_stereo))
    {
        return;
    }
    if(0 != fm_engine_msg(MSG_FMENGINE_GET_INTENSITY, NULL, 0, &playing->cur_intensity))
    {
        return;
    }

    //    fm_engine_msg(MSG_FMENGINE_GET_STEREO_MODE, NULL, 0, &playing->cur_stereo);
    //fm_engine_msg(MSG_FMENGINE_GET_INTENSITY, NULL, 0, &playing->cur_intensity);
    //fm_engine_msg(MSG_FMENGINE_GET_FREQ, NULL, 0, &playing->cur_frequency);
    //fm_engine_msg(MSG_FMENGINE_GET_BAND_MODE, NULL, 0, &playing->cur_band);

    if(old_stereo != playing->cur_stereo)
    {
        show_stereo_pic(playing, true);
    }

    if (old_intensity != playing->cur_intensity)
    {
        show_intensity_pic(playing, true);
    }

    if((old_freq != playing->cur_frequency)
        &&(playing->cur_frequency >= min)
        &&(playing->cur_frequency <= max))

    {
        show_cur_freq(playing, true);
    }
    if(true == stop_tm)
    {
        bool stret;
        stop_tm = false;
        //playing->timer_id = set_timer(500, (timer_proc)_scene_timer, playing);
        stret = stop_timer (playing->timer_id);
        if(true == stret)
        {
            if(0 == playing->timer_id_tmp)
            {
                playing->timer_id_tmp = set_single_shot_timer (2000, (timer_proc)_scene_tmp_timer, playing);
            }
            else
            {
                restart_timer(playing->timer_id_tmp);
            }
        }
    }
}
#endif
#endif

static void _set_clk_domain( void )
{
    if( false == clk_domain_flag )
    {
        sys_pm_set_clk_domain(&s_clk_domain);
        clk_domain_flag = true;        
    }
}

static void _clear_clk_domain( void )
{
    if( true == clk_domain_flag )
    {
        sys_pm_clear_clk_domain(&s_clk_domain);
        clk_domain_flag = false;
    }
}


/******************************************************************************/
/*!                    
 * \par  Description:     scene timer 
*******************************************************************************/
static void _scene_timer(void * param)
{
    radio_scene_playing_t *playing = (radio_scene_playing_t *) param;
    unsigned int max, min;
    unsigned int first_preset, num;
    int old_intensity = playing->cur_intensity;
    bool old_stereo = playing->cur_stereo;
    unsigned int old_freq = playing->cur_frequency;
    bool seek_over = false;
    en_seek_info_t seek_info;

    static unsigned int tmp_channel_num = 0;

//    printf("\ncur_channel_id:%d  channel_num: %d\n",
//        playing->cur_preset_channel_id, playing->channel_list_info.channel_num );
    
    get_band_range(playing->cur_band, &max, &min);
    if (NORMAL_PLAY != playing->searching_flag)
    {
        memset(&seek_info, 0, sizeof(en_seek_info_t));
        if (AUTO_SEARCH == playing->searching_flag)
        {
            /*升频以提高搜台速度*/
            //_set_clk_domain();
            
            print_dbg("auto_search begin...........................\n");
            if (true == playing->auto_search_start)
            {
                
                playing->auto_search_start = false;
                seek_info.max_freq = max;
                seek_info.min_freq = min;
                seek_info.step = get_band_step(playing->cur_band);
                seek_info.mode = playing->searching_flag;
                if(0 != fm_engine_msg(MSG_FMENGINE_SET_AUTO_SEEK, &seek_info, sizeof(en_seek_info_t), NULL))
                {
                    playing->auto_search_start = true;
                    return;
                }
            }
            
            if(0 != fm_engine_msg(MSG_FMENGINE_AUTO_SEEK_OVER, NULL, 0, &seek_over))
            {
                return;
            }
            
            if (true == seek_over)
            { 
                /*降频，恢复到以前频率*/
                _clear_clk_domain();
                
                first_preset = playing->channel_list_info.channel_freq[0];
                playing->cur_frequency = (first_preset == 0) ? min : first_preset;
                playing->searching_flag = NORMAL_PLAY;
                playing->cur_preset_channel_id = 0;
               
                g_force_show_channel_name = false;
                tmp_channel_num = 0;

                print_dbg("auto seek over!#$$$$$$$\n");
            }
            else
            {
                if(0 != fm_engine_msg(MSG_FMENGINE_GET_AUTO_SEEK, NULL, 0, &playing->channel_list_info))
                {
                    return;
                }
                num = playing->channel_list_info.channel_num;
//                print_dbg("num = %d, tmp_channel_num = %d\n",num,tmp_channel_num);                
                if( num != tmp_channel_num)
                {
                    g_force_show_channel_name = true;
                    tmp_channel_num = num;
                    if(num > 0)
                    {
                        playing->cur_preset_channel_id = (int) (num - 1);
                        
                        sprintf(playing->channel_list_info.channel_name[playing->cur_preset_channel_id], 
                            "_CH%02d_", num);
                        
                        playing->cur_frequency = 
                            playing->channel_list_info.channel_freq[playing->cur_preset_channel_id];                
                    }

                }
                else
                {
                    g_force_show_channel_name = false;
                } 
            }
        }
        else if (HALF_AUTO_SEARCH_ADD == playing->searching_flag) /* 半自动前向搜索 */
        {
            seek_info.max_freq = max;
            seek_info.min_freq = playing->cur_frequency;
            seek_info.step = get_band_step(playing->cur_band);
            seek_info.mode = playing->searching_flag;
            if(0 != fm_engine_msg(MSG_FMENGINE_SET_AUTO_SEEK, &seek_info, sizeof(en_seek_info_t), NULL))
            {
                return;
            } 
            playing->searching_flag = NORMAL_PLAY;   
        }
        else
        {
            seek_info.max_freq = playing->cur_frequency;
            seek_info.min_freq = min;
            seek_info.step = get_band_step(playing->cur_band);
            seek_info.mode = playing->searching_flag;
            if(0 != fm_engine_msg(MSG_FMENGINE_SET_AUTO_SEEK, &seek_info, sizeof(en_seek_info_t), NULL))
            {
                return;
            } 
            playing->searching_flag = NORMAL_PLAY;  
        }
    }
    
    if(NORMAL_PLAY == playing->searching_flag)
    {
        /*降频，恢复到以前频率*/
//        _clear_clk_domain();
        g_force_show_channel_name = false;
        
        if(0 != fm_engine_msg(MSG_FMENGINE_GET_FREQ, NULL, 0, &playing->cur_frequency))
        {
            print_dbg("!!!!!!!!!get freq err %d\n",playing->cur_frequency);
            return;
        }
    }
    
    if(0 != fm_engine_msg(MSG_FMENGINE_GET_STEREO_MODE, NULL, 0, &playing->cur_stereo))
    {
        return;
    }
    
    if(0 != fm_engine_msg(MSG_FMENGINE_GET_INTENSITY, NULL, 0, &playing->cur_intensity))
    {
        return;
    }
    
    if(old_stereo != playing->cur_stereo)
    {
        show_stereo_pic(playing, true);
    }
    
    if (old_intensity != playing->cur_intensity)
    {
        show_intensity_pic(playing, true);
    }

    if( (g_force_show_channel_name == true) || (old_freq != playing->cur_frequency) )
    {
        show_cur_freq(playing, true);
    }
}

/******************************************************************************/
/*!                    
 * \par  Description:    读取配置项 
*******************************************************************************/
static bool _load_config(radio_scene_playing_t *playing)
{
    int value;

    /*波段信息*/
    if (get_config(CFG_FM_BAND, (char *) &value, sizeof(value)) == -1)
    {
        return false;
    }
    else
    {
        playing->cur_band = value;
    }

    /*频率信息*/
    if (get_config(CFG_FM_FREQUENCY, (char *) &value, sizeof(value)) == -1)
    {
        return false;
    }
    else
    {
        playing->cur_frequency = (unsigned int)value;
    }

    /*灵敏度*/
    if (get_config(CFG_FM_INTENSITY, (char *) &value, sizeof(value)) == -1)
    {
        return false;
    }
    else
    {
        playing->cur_threthod = value;
    }

    /*录音质量*/
    if (get_config(CFG_FM_RECORD_QUALITY, (char *) &value, sizeof(value)) == -1)
    {
        return false;
    }
    else
    {
        playing->rec_quality = value;
    }

    /*录音增益*/
    if (get_config(CFG_FM_RECORD_GAIN, (char *) &value, sizeof(value)) == -1)
    {
        return false;
    }
    else
    {
        playing->rec_gain = value;
    }

    /*录音文件保存路径*/
    if (get_config(CFG_FM_RECORD_PATH, (char *) &value, sizeof(value)) == -1)
    {
        return false;
    }
    else
    {
        playing->rec_path = value;
    }
    /*是否需要查询line_in*/
    if (get_config(CFG_FM_LINEIN_CHECK, (char *) &value, sizeof(value)) == -1)
    {
        return false;
    }
    else
    {
        playing->line_in_check = value;
    }
    /*预制模式*/
    if (get_config(CFG_FM_USE_PRESET, (char *) &value, sizeof(value)) == -1)
    {
        return false;
    }
    else
    {
        playing->use_preset= value;
    }
    /*当前预制频段ID*/
    if (get_config(CFG_FM_CUR_PRESET_ID, (char *) &value, sizeof(value)) == -1)
    {
        return false;
    }
    else
    {
        playing->cur_preset_channel_id= value;
    }
    /*是否用户列表模式*/
    if (get_config(CFG_FM_USERCHANNEL_MODE, (char *) &value, sizeof(value)) == -1)
    {
        return false;
    }
    else
    {
        playing->is_userchannel_mode= value;
    }
    playing->load_config_flag = true;

    return true;
}

/******************************************************************************/
/*!                    
 * \par  Description:     save configure
*******************************************************************************/
static bool _save_config(radio_scene_playing_t *playing)
{
    if (set_config(CFG_FM_BAND, (const char *) &playing->cur_band, sizeof(playing->cur_band)) == -1)
    {
        return false;
    }
    if (set_config(CFG_FM_INTENSITY, (const char *) &playing->cur_threthod, sizeof(playing->cur_threthod)) == -1)
    {
        return false;
    }
    if (set_config(CFG_FM_FREQUENCY, (const char *) &playing->cur_frequency, sizeof(playing->cur_frequency)) == -1)
    {
        return false;
    }
    if (set_config(CFG_FM_RECORD_QUALITY, (const char *) &playing->rec_quality, sizeof(playing->rec_quality)) == -1)
    {
        return false;
    }
    if (set_config(CFG_FM_RECORD_GAIN, (const char *) &playing->rec_gain, sizeof(playing->rec_gain)) == -1)
    {
        return false;
    }
    if (set_config(CFG_FM_RECORD_PATH, (const char *) &playing->rec_path, sizeof(playing->rec_path)) == -1)
    {
        return false;
    }
    if (set_config(CFG_FM_USE_PRESET, (const char *) &playing->use_preset, sizeof(playing->use_preset)) == -1)
    {
        return false;
    }
    if (set_config(CFG_FM_CUR_PRESET_ID, (const char *) &playing->cur_preset_channel_id, 
        sizeof(playing->cur_preset_channel_id)) == -1)
    {
        return false;
    }
    if (set_config(CFG_FM_USERCHANNEL_MODE, (const char *) &playing->is_userchannel_mode, 
        sizeof(playing->is_userchannel_mode)) == -1)
    {
        return false;
    }
    if (set_config(CFG_FM_USERCHANNEL_PATH, userchannel_path, (int)strlen(userchannel_path) + 1) == -1)
    {
        return false;
    }
    return true;
}

/******************************************************************************/
/*!                    
 * \par  Description:     open theme
*******************************************************************************/
static bool _playing_open_theme(void)
{
    rotation_e angle;
    char file[512];
    const char * theme_dir = sys_get_common_theme_res_dir();
    const char * app_theme_dir = sys_get_app_theme_res_dir();
    if ((NULL == theme_dir) || (NULL == app_theme_dir))
    {
        return false;
    }
    /* 取视图角度*/
    angle = sty_get_view_rotation();

    /* 设置屏幕方向*/
    sty_set_screen_rotation(angle);

    /* 获取commonui 的 style 资源文件名*/
    sprintf(file, "%s/%s.sty", theme_dir, "commonui");

    /* 载入commonui的 style 场景*/
    if ((angle == ROTATION_0) || (angle == ROTATION_180))
    {
        /* 获取commonui 的 style 资源文件名*/
        g_scene_playing->common_scene = load_scene(app_data.commonui_style, COMMONUI_SIDELONG_SCENE);

        /* 获取应用的 style 资源文件名*/
        /* 载入应用的 style 场景*/
        g_scene_playing->scene = load_scene(app_data.app_style, PLAYING_SIDELONG_SCENE);
    }
    else
    {
        /* 获取commonui 的 style 资源文件名*/
        g_scene_playing->common_scene = load_scene(app_data.commonui_style, COMMONUI_UPRIGHT_SCENE);

        /* 获取应用的 style 资源文件名*/
        /* 载入应用的 style 场景*/
        g_scene_playing->scene = load_scene(app_data.app_style, PLAYING_UPRIGHT_SCENE);
    }

    if ((g_scene_playing->common_scene == NULL) && (g_scene_playing->scene == NULL))
    {
        print_err("load commonui and app scene failed");
        return false;
    }
    else
    {
        /* 设置 commonui 的 style 场景*/
        sty_set_common_scene(g_scene_playing->common_scene);

        /* 设置应用的 style 场景*/
        sty_set_app_scene(g_scene_playing->scene);
    }

    /* 获取commonui 的图片资源文件名*/
    sprintf(file, "%s/%s.res", theme_dir, "commonui");

    /* 打开 commonui 图片资源文件*/
    if (sty_open_common_pic_res(file) == false)
    {
        print_err("open %s failed!", file);
        return false;
    }

    /* 获取应用的图片资源文件名*/
    sprintf(file, "%s/%s.res", app_theme_dir, "radio");

    /* 打开应用的图片资源文件*/
    if (sty_open_app_pic_res(file) == false)
    {
        print_err("open %s failed!", file);
        return false;
    }

    return true;

}

/******************************************************************************/
/*!                    
 * \par  Description:     close theme
*******************************************************************************/
static bool _playing_close_theme(void)
{

    free_scene(g_scene_playing->common_scene);
    g_scene_playing->common_scene = NULL;

    free_scene(g_scene_playing->scene);
    g_scene_playing->scene = NULL;

    if (sty_close_common_pic_res() == false)
    {
        print_err("close common picture resource error!");
        return false;
    }

    if (sty_close_app_pic_res() == false)
    {
        print_err("close app picture resource error!");
        return false;
    }

    return true;
}

/******************************************************************************/
/*!                    
 * \par  Description:     open string resource
*******************************************************************************/
static bool _playing_open_str_res(void)
{
    char file[512];

    /* 获取commonui 字符串资源文件名*/
    sprintf(file, "%s/%s_%s.str", sys_get_common_string_res_dir(), "commonui", sys_get_cur_language_codes());
    print_dbg("load %s", file);
    /* 打开 commonui 字符串资源文件*/
    if (sty_open_common_str_res(file) == false)
    {
        print_err("open %s failed!", file);
        return false;
    }

    /* 获取setting应用字符串资源文件名*/
    sprintf(file, "%s/%s_%s.str", sys_get_app_string_res_dir(), "radio", sys_get_cur_language_codes());
    print_dbg("load %s", file);
    /* 打开应用的字符串资源文件*/
    if (sty_open_app_str_res(file) == false)
    {
        print_err("open %s failed!", file);
        return false;
    }

    /* 设置语言编码*/
    sty_set_text_encoding(GUI_UTF8_ENC);

    /* 设置语言模式*/
    gui_set_language_mode(sys_get_cur_language_id());

    return true;
}

/******************************************************************************/
/*!                    
 * \par  Description:     close string resource
*******************************************************************************/
static bool _playing_close_str_res(void)
{
    if (sty_close_common_str_res() == false)
    {
        print_err("close common string resource error!");
        return false;
    }

    if (sty_close_app_str_res() == false)
    {
        print_err("close app string resource error!");
        return false;
    }

    return true;
}

/******************************************************************************/
/*!                    
 * \par  Description:     create headbar
*******************************************************************************/
static bool _create_headbar(void)
{
    headbar_init_t headbar_init;

    /* 创建headbar控件 */
    headbar_init.type = 0;
    headbar_init.pic_id_head = g_scene_playing->pic_logo->pic_id;
    headbar_init.str_id_title = g_scene_playing->str_logo->str_id;

    g_scene_playing->headbar = headbar_create(&headbar_init, (headbar_cb_t) _headbar_cb);
    if (NULL == g_scene_playing->headbar)
    {
        print_err("headbar_create failed\n\n");
        return false;
    }
    headbar_paint(g_scene_playing->headbar);

    return true;
}
#if 0
static bool _create_dialog(void)
{
    dialog_init_t dialog_init;

    //create dialog
    dialog_init.head = DIALOG_HEAD_WARN;
    dialog_init.str_id = g_scene_playing->str_earinsert->str_id;

    g_scene_playing->dialog = dialog_create(&dialog_init, _dialog_cb);
    if (NULL == g_scene_playing->dialog)
    {
        print_err("dialog_create failed\n\n");
        return false;
    }

    /* dialog paint */
    dialog_paint(g_scene_playing->dialog);

    return true;
}
static bool _create_dialog_linein(void)
{
    dialog_init_t dialog_init;

    //create dialog
    dialog_init.head = DIALOG_HEAD_WARN;
    dialog_init.str_id = g_scene_playing->str_linein->str_id;

    g_scene_playing->dialog_linein = dialog_create(&dialog_init, _dialog_cb_linein);
    if (NULL == g_scene_playing->dialog_linein)
    {
        print_err("dialog line in_create failed\n\n");
        return false;
    }

    /* dialog paint */
    dialog_paint(g_scene_playing->dialog_linein);

    return true;
}
#endif

/******************************************************************************/
/*!                    
 * \par  Description:     create msgbox
*******************************************************************************/
static bool _create_msgbox(void)
{
    g_scene_playing->msgbox = msgbox_create(_msgbox_cb);
    if (NULL == g_scene_playing->msgbox)
    {
        print_err("create msgbox failed!");
        return false;
    }

    return true;
}

/*
 * 初始化style
 */
static void _scene_playing_style_init(radio_scene_playing_t *playing)
{
    int i;
    resgroup_resource_t *resg_tmp;

    playing->pic_bg = (picture_resource_t *) get_scene_child(playing->scene, PIC_BG);
    playing->pic_select = (picture_resource_t *) get_scene_child(playing->scene, PIC_SELECT);
    playing->picreg_num = (picregion_resource_t *) get_scene_child(playing->scene, PICREG_NUM);
  

    /* resgroup playing */
    resg_tmp = (resgroup_resource_t *) get_scene_child(playing->scene, RESGROUP_PLAYING);
    playing->res_playing.resgroup_playing = resg_tmp;
    playing->res_playing.pic_foreground = (picture_resource_t *) get_resgroup_child(resg_tmp, PIC_FOREGROUND);
    playing->res_playing.pic_stereo = (picture_resource_t *) get_resgroup_child(resg_tmp, PIC_STEREO);
    playing->res_playing.pic_num_hundreds = (picture_resource_t *) get_resgroup_child(resg_tmp, PIC_NUM_HUNDREDS);
    playing->res_playing.pic_num_tens = (picture_resource_t *) get_resgroup_child(resg_tmp, PIC_NUM_TENS);
    playing->res_playing.pic_num_units = (picture_resource_t *) get_resgroup_child(resg_tmp, PIC_NUM_UNITS);
    playing->res_playing.pic_num_dot = (picture_resource_t *) get_resgroup_child(resg_tmp, PIC_NUM_DOT);
    playing->res_playing.pic_num_tenth = (picture_resource_t *) get_resgroup_child(resg_tmp, PIC_NUM_TENTH);
    playing->res_playing.str_num_centi = (string_resource_t *) get_resgroup_child(resg_tmp, STR_NUM_CENTI);
    playing->res_playing.str_mhz = (string_resource_t *) get_resgroup_child(resg_tmp, STR_MHZ);
    playing->res_playing.picreg_signal = (picregion_resource_t *) get_resgroup_child(resg_tmp, PICREG_SIGNAL);
    playing->res_playing.str_country = (string_resource_t *) get_resgroup_child(resg_tmp, STR_COUNTRY);
    playing->res_playing.str_channel = (string_resource_t *) get_resgroup_child(resg_tmp, STR_CHANNEL);

    playing->res_playing.pic_freq_bg = (picture_resource_t *) get_resgroup_child(resg_tmp, PIC_FREQ_BG);
    playing->res_playing.str_freq_start = (string_resource_t *) get_resgroup_child(resg_tmp, STR_FREQ_START);
    playing->res_playing.str_freq_end = (string_resource_t *) get_resgroup_child(resg_tmp, STR_FREQ_END);

    for (i = 0; i < 181; i++)
    {
        playing->res_playing.pic_freq_notation[i] = (picture_resource_t *) get_resgroup_child(resg_tmp,
                freq_natation[i]);
    }

    /* option */
    resg_tmp = (resgroup_resource_t *) get_scene_child(playing->scene, RESGROUP_OPTION);
    radio_option_load_style(resg_tmp);
    resg_tmp = (resgroup_resource_t *) get_scene_child(playing->scene, RESGROUP_OPTION_LEVEL);
    radio_option_level_load_style(resg_tmp);
    resg_tmp = (resgroup_resource_t *) get_scene_child(playing->scene, RESGROUP_OPTION_BAND);
    radio_option_band_load_style(resg_tmp);
    resg_tmp = (resgroup_resource_t *) get_scene_child(playing->scene, RESGROUP_OPTION_PATH);
    radio_option_path_load_style(resg_tmp);

    /* headbar */
    resg_tmp = (resgroup_resource_t *) get_scene_child(playing->scene, WIDGET_HEADBAR_RESGROUP);
    playing->pic_logo = (picture_resource_t *) get_resgroup_child(resg_tmp, PIC_LOGO);
    playing->str_logo = (string_resource_t *) get_resgroup_child(resg_tmp, STR_LOGO);

    /* dialog */
    resg_tmp = (resgroup_resource_t *) get_scene_child(playing->scene, WIDGET_DIALOG_RESGROUP);
    playing->str_fmterr = (string_resource_t *) get_resgroup_child(resg_tmp, STR_DIALOG_FMTERR);
    playing->str_earinsert = (string_resource_t *) get_resgroup_child(resg_tmp, STR_DIALOG_EARINSERT);
    playing->str_linein = (string_resource_t *) get_resgroup_child(resg_tmp, STR_DIALOG_LINEIN);
    playing->str_nodisk = (string_resource_t *) get_resgroup_child(resg_tmp, STR_DIALOG_NODISK);
    playing->str_diskins = (string_resource_t *) get_resgroup_child(resg_tmp, STR_DIALOG_DISKINS);

    if (playing->bg_bitmap_info != NULL)
    {
        _scene_playing_delete_bg_bitmap();
    }

    _scene_playing_get_bg_bitmap();
}

/* scene paint function */
static void _scene_playing_paint(radio_scene_playing_t *playing)
{
    /*获取dc*/
    int hdc = gui_dc_get((unsigned short) playing->win_id);

    /* 显示应用背景*/
    sty_draw_bitmap(hdc, playing->bg_bitmap_info, 0, 0);

    /*显示场景背景*/
    sty_draw_picture_res(hdc, playing->res_playing.pic_foreground);

    /*显示信号强度*/
    show_intensity_pic(playing, false);

    /* 显示立体声状态*/
    show_stereo_pic(playing, false);

    /* 显示当前频率相关图片和字符串*/
    show_cur_freq(playing, false);

    print_dbg("radio scene playing paint finished!");
}

static bool _fm_engine_startup(void)
{
    msg_apps_t msg_app;
    msg_reply_t t;

    memset( msg_app.content, 0, sizeof(msg_app.content) );
    msg_app.type = MSG_CREAT_APP_SYNC;
    t.type = MSG_REPLY_FAILED;
    strncpy(msg_app.content, FM_EG_APP_NAME, 512);
    send_sync_msg(MANAGER_APP_NAME, &msg_app, &t, NULL);

    if (t.type != MSG_REPLY_SUCCESS)
    {
        return false;
    }

    return true;
}

/******************************************************************************/
/*!                    
 * \par  Description:     engine init
*******************************************************************************/
static void _fm_engine_init(void)
{
    int value;

    value = get_radio_band(g_scene_playing->cur_band);

    fm_engine_msg(MSG_FMENGINE_SET_BAND_MODE, &value, sizeof(int), NULL);
    fm_engine_msg(MSG_FMENGINE_SET_FREQ, &g_scene_playing->cur_frequency, sizeof(int), NULL);
    fm_engine_msg(MSG_FMENGINE_SET_THRESHOLD, &g_scene_playing->cur_threthod, sizeof(int), NULL);
    fm_engine_msg(MSG_FMENGINE_SET_PLAY, NULL, 0, NULL);

    g_scene_playing->searching_flag = NORMAL_PLAY;
}

/*
 * 设置当前场景全部区域作为特效的第一帧
 */
static void _scene_playing_fusion_set_first_frame(scene_t *scene)
{
    fusion_frame_t frame;

    frame.rect.x = scene->x;
    frame.rect.y = scene->y;
    frame.rect.width = scene->width;
    frame.rect.height = scene->height;
    frame.pdata = NULL;
    frame.alpha = false;

    fusion_set_first_frame(&frame);
}

/*
 * 设置当前场景全部区域作为特效的最后一帧
 */
static void _scene_playing_fusion_set_last_frame(scene_t *scene)
{
    fusion_frame_t frame;

    frame.rect.x = scene->x;
    frame.rect.y = scene->y;
    frame.rect.width = scene->width;
    frame.rect.height = scene->height;
    frame.pdata = NULL;
    frame.alpha = false;

    fusion_set_last_frame(&frame);
}

/******************************************************************************/
/*!                    
 * \par  Description:     get sys path
*******************************************************************************/
static bool _get_sys_path(void)
{
    sys_path = sys_get_main_disk_dir(DIR_SYSTEM);
    if( NULL == sys_path )
    {
        /*没有盘*/
        print_err("get DIR_SYSTEM err!!!\n");
    }

    sys_fm_path = sys_get_main_disk_dir(DIR_SYSTEM_FM);
    if( NULL == sys_fm_path )
    {
        print_err("get DIR_SYSTEM_FM err!!!\n");
    }
    
    return true;
}

/* scene initial function */
static bool _scene_init(fusion_enter_e fenter)
{
    int value, fm_fd;
    int value2;
    bool result;

    radio_scene_playing_t *playing;
    fusion_effect_t eff;
    dialog_init_t dialog_init;

    /*分配场景数据空间*/
    if (g_scene_playing == NULL)
    {
        g_scene_playing = malloc(sizeof(radio_scene_playing_t));
        if (g_scene_playing == NULL)
        {
            print_err("malloc failure\n");
            return false;
        }
        else
        {
            memset(g_scene_playing, 0, sizeof(radio_scene_playing_t)); /*clear*/
            playing = g_scene_playing;
        }
    }
    else
    {
        playing = g_scene_playing;
        /*由于要实现在没有磁盘的时候进入录音场景之后回来还能显示之前搜索到的电台，因此在退出播放场景时仅卸载资源，
        不会将g_scene_playing置为NULL,因此next_scene重新赋值*/
        playing->next_scene = RADIO_SCENE_PLAYING_FORWARD;
    }

    /* load theme 资源文件 */
    if (_playing_open_theme() == false)
    {
        return false;
    }

    /* load 字符串资源文件 */
    if (_playing_open_str_res() == false)
    {
        return false;
    }

    /*创建字体*/
    if (_scene_playing_set_default_font() == false)
    {
        return false;
    }

    _scene_playing_style_init(playing);

    _load_config(playing);
    /*检测天线*/
    fm_fd = open(DEVICE_FM, O_RDWR);
    ioctl(fm_fd, RADIO_GET_ANTENNA, (int) &value);
    if(playing->line_in_check == 1)
    {
        ioctl(fm_fd, RADIO_CHECK_LINEIN,(int)&value2);
    }
    close(fm_fd);

    if (value == 0)
    {
        dialog_init.head = DIALOG_HEAD_WARN;
        dialog_init.str_id = g_scene_playing->str_earinsert->str_id;
        dialog_modal(&dialog_init);
        print_err("ear no insert!\n");
        
        return false;
    }
    if(playing->line_in_check == 1)
    {
        if (value2 == LINEIN_IN)
        {
         
            dialog_init.head = DIALOG_HEAD_WARN;
            dialog_init.str_id = g_scene_playing->str_linein->str_id;
            dialog_modal(&dialog_init);

            print_err("line_in insert\n");
            return false;
        }
    }

    /*创建窗口*/
    playing->win_id = gui_wm_create_window(playing->scene->x, playing->scene->y, playing->scene->width,
            playing->scene->height, WM_CREATE_SHOW, _scene_callback, 0);
    if (playing->win_id <= 0)
    {
        print_err("--: %s---%d---\n", __FILE__, __LINE__);
        return false;
    }

    /*设置焦点*/
    gui_wm_set_focus(playing->win_id);
    /*存储器检查*/
    if(false == _get_sys_path())
    {
        return false;
    }
    

    /*若fm引擎进程不存在，则启动引擎*/
    if (is_app_exist(FM_EG_APP_NAME) == false)
    {
        /* 启动引擎 */
        if (false == _fm_engine_startup())
        {
            print_err("launch fm engine app failure!\n");
            applib_quit();
            return false;
        }

        _fm_engine_init();
    }
    _check_default_file(playing);
    
    /*判断如果上次退出是在用户列表状态，则进入原列表*/
    if( playing->is_userchannel_mode==true )
    {
        if(get_config(CFG_FM_USERCHANNEL_PATH, userchannel_path, PATH_MAX) < 0)
        {
            print_err("get config path failure\n");
            //normal mode
            playing->use_preset = false;
            playing->is_userchannel_mode = false;
        }
        else
        {
            if (strcmp(userchannel_path, channel_list_file) != 0)
            { 
                result = _check_userchannel_file();
                if(result == false)
                {
                    print_err("_check_userchannel_file failure\n");
                    //normal mode
                    playing->use_preset = false;
                    playing->is_userchannel_mode = false;
                }
            }
            else
            {
                channel_info = playing->channel_list_info;
            }
            
            if( channel_info.channel_num == 0 )
            {
                playing->use_preset = false;
                playing->is_userchannel_mode = false;                
            }
        }
    }
    else
    {
        if( playing->channel_list_info.channel_num == 0 )
        {
            playing->use_preset = false;
        }
    }
    
    //以下几个参数写进config，进入场景时从config读取。
    /*  false--normal mode, true-- preset mode */
    //playing->use_preset = false;
    //playing->is_userchannel_mode = false;
    //playing->cur_preset_channel_id = 0; 
    fusion_get_config(FUSION_TYPE_COMMON, fenter, &eff);
    fusion_set_effect(&eff);

    _scene_playing_fusion_set_first_frame(playing->scene);

    if (false == _create_headbar())
    {
        return false;
    }

    if (false == _create_msgbox())
    {
        return false;
    }

    /*paint all*/
    _scene_playing_paint(playing); /*draw static window bg*/

    _scene_playing_fusion_set_last_frame(playing->scene);

    fusion_display(NULL);

    /* 定时查询信息 */
    playing->timer_id = set_inner_timer(100, (timer_proc) _scene_timer, playing);
    if (playing->timer_id < 0)
    {
        print_err("set timer error!");
        return false;
    }

    clear_key_msg_queue(-1,-1);
    return true;
}

/* scene exit function */
static bool _scene_deinit(void)
{
    radio_scene_playing_t *playing = g_scene_playing;
    if(NULL == playing)
    {
        return false;

    }

    /*
     * 场景退出到主界面时，如果当前处于静音状态，则通知
     * FM引擎退出。
     */
    if(playing->next_scene == RADIO_SCENE_EXIT)
    {
        if (true == playing->mute)
        {
            fm_engine_msg(MSG_APP_QUIT, NULL, 0, NULL);
        }
    }
    
    /*正在自动搜台，退出之前停止自动搜台*/
    if (playing->searching_flag == AUTO_SEARCH)
    {
        print_dbg("stop auto search\n");
        playing->auto_search_start = false;
        playing->searching_flag = NORMAL_PLAY;
        fm_engine_msg(MSG_FMENGINE_SET_AUTO_SEEK_BREAK, NULL, 0, NULL);
    }
   
    /*只有当解析了defalut.ini文件，退出时才需要保存文件*/
    if (playing->parse_default_file_flag == true)
    {
        save_default_file(playing);
    }
    
    _close_default_file(playing);

    /*只有当正确读取配置项时，退出才需要保存配置项*/
    if (playing->load_config_flag == true)
    {
        _save_config(playing);
    }

    

    if (NULL != playing->option_menu)
    {
        radio_playing_option_exit(playing->option_menu);
    }

    if (NULL != playing->volumebar)
    {
        volumebar_delete(playing->volumebar);
        playing->volumebar = NULL;
    }

    if (NULL != playing->headbar)
    {
        headbar_delete(playing->headbar);
        playing->headbar = NULL;
    }

    if (NULL != playing->dialog)
    {
        dialog_delete(playing->dialog);
        playing->dialog = NULL;
    }
    if (playing->line_in_check == 1)
    {
        if (NULL != playing->dialog_linein)
        {
            dialog_delete(playing->dialog_linein);
            playing->dialog_linein = NULL;
        }
        
    }

    if (NULL != playing->msgbox)
    {
        msgbox_delete(playing->msgbox);
        playing->msgbox = NULL;
    }

    if (playing->timer_id != -1)
    {
        kill_inner_timer(playing->timer_id);
        playing->timer_id = -1;
    }

    /*删除窗体*/
    if(playing->win_id > 0)
    {
        gui_wm_delete_window(playing->win_id);
        playing->win_id = 0;
    }

    /* destroy font */
    _scene_playing_unset_default_font();

    if (_playing_close_theme() == false)
    {
        return false;
    }

    if (_playing_close_str_res() == false)
    {
        return false;
    }

    _scene_playing_delete_bg_bitmap();

//    free(g_scene_playing);
//    g_scene_playing = NULL;

    /*降频，恢复到以前频率*/
    _clear_clk_domain();


    return true;
}


static bool _scene_proc_key(radio_scene_playing_t *playing, key_event_t * kmsg)
{
    int lo_index;
    unsigned int temp_freq;
    unsigned int maxf, minf;
    bool seek_over;

    switch (kmsg->val | kmsg->type)
    {
        case (KEY_OK | KEY_TYPE_SHORT_UP):
        /*正在自动搜台，跳出*/
        if(AUTO_SEARCH == playing->searching_flag)
        {
            break;
        }
            
        playing->use_preset = !playing->use_preset;

        if( playing->is_userchannel_mode == false )
        {
            if (playing->channel_list_info.channel_num == 0)
            {
                playing->use_preset = false;
            }
        }
        else
        {
            if (channel_info.channel_num == 0)
            {
                playing->use_preset = false;
            }            
        }
        
        if (playing->use_preset == true)
        {
            if( playing->is_userchannel_mode == false )
            {
                playing->cur_preset_channel_id = 0;
                if (0 == playing->channel_list_info.channel_freq[0])
                {
                    lo_index = get_next_channel_index(playing->channel_list_info.channel_freq,
                            playing->cur_preset_channel_id);
                    if (-1 != lo_index)
                    {
                        playing->cur_preset_channel_id = lo_index;
                    }
                    else
                    {
                        /*电台列表下仅有一个频率为0的电台，设置到普通模式*/
                        playing->use_preset = false;
                    }
                }
                temp_freq = playing->channel_list_info.channel_freq[playing->cur_preset_channel_id];
                if ((temp_freq != playing->cur_frequency) && (0 != temp_freq) )
                {
                    playing->cur_frequency = temp_freq;
                    fm_engine_msg(MSG_FMENGINE_SET_FREQ, &playing->cur_frequency, sizeof(int), NULL);
                }
            }
            else
            {
                playing->cur_preset_channel_id = 0;
                if (0 == channel_info.channel_freq[0])
                {
                    lo_index = get_next_channel_index(channel_info.channel_freq,
                            playing->cur_preset_channel_id);
                    if (-1 != lo_index)
                    {
                        playing->cur_preset_channel_id = lo_index;
                    }
                    else
                    {
                        /*电台列表下仅有一个频率为0的电台，设置到普通模式*/
                        playing->use_preset = false;
                    }
                }
                temp_freq = channel_info.channel_freq[playing->cur_preset_channel_id];
                if ((temp_freq != playing->cur_frequency) && (0 != temp_freq) )
                {
                    playing->cur_frequency = temp_freq;
                    fm_engine_msg(MSG_FMENGINE_SET_FREQ, &playing->cur_frequency, sizeof(int), NULL);
                }
            }
        }
        show_cur_freq(playing, true);
        clear_key_msg_queue( -1, -1 );/*清除多余按键；*/            
        break;

        case (KEY_RETURN | KEY_TYPE_DOWN):
        if (playing->searching_flag == AUTO_SEARCH)
        {
           
            playing->auto_search_start = false;
            g_force_show_channel_name = false;
            playing->searching_flag = NORMAL_PLAY;
            fm_engine_msg(MSG_FMENGINE_SET_AUTO_SEEK_BREAK, NULL, 0, NULL);
    
        }
        else
        {
            playing->next_scene = RADIO_SCENE_EXIT;
            exit_msg_loop();
            print_dbg("quit scene!\n");
        }
        show_cur_freq(playing, true);        
        break;

        case (KEY_MENU | KEY_TYPE_DOWN):
        /*创建option菜单*/
        if (playing->option_menu == NULL)
        {
            playing->option_menu = radio_playing_widget_option_create(_option_cb);

            if (playing->option_menu == NULL)
            {
                print_err();
                return false;
            }
        }
        break;

        case (KEY_VOL | KEY_TYPE_DOWN):
        case (KEY_VSUB | KEY_TYPE_DOWN):
        case (KEY_VADD | KEY_TYPE_DOWN):
        if (NULL == playing->volumebar)
        {
            playing->volumebar = volumebar_create(0, _volumebar_cb);
            if (playing->volumebar == NULL)
            {
                print_err("volumebar create error");
                return false;
            }
            volumebar_paint(playing->volumebar);
        }
        break;

        case (KEY_NEXT | KEY_TYPE_DOWN):
        if(0 != fm_engine_msg(MSG_FMENGINE_AUTO_SEEK_OVER, NULL, 0, &seek_over))
        {
            break;
        }
        
        if (true != seek_over)
        {
            break;
        }
        if (playing->use_preset == true)
        {
            if(playing->is_userchannel_mode == false)
            {   
                playing->cur_preset_channel_id = get_next_channel_index(playing->channel_list_info.channel_freq,
                        playing->cur_preset_channel_id);
                if (playing->cur_preset_channel_id == -1)
                {
                    playing->use_preset = false;
                }
                else
                {
                    playing->cur_frequency = playing->channel_list_info.channel_freq[playing->cur_preset_channel_id];
                }
            }
            else
            {
                playing->cur_preset_channel_id = get_next_channel_index(channel_info.channel_freq,
                        playing->cur_preset_channel_id);
                if (playing->cur_preset_channel_id == -1)
                {
                    playing->use_preset = false;
                }
                else
                {
                    playing->cur_frequency = channel_info.channel_freq[playing->cur_preset_channel_id];
                }
            }
        }
        else
        {
            
            get_band_range(playing->cur_band, &maxf, &minf);

            playing->cur_frequency += get_band_step(playing->cur_band);
            if (playing->cur_frequency > maxf)
            {
                playing->cur_frequency = minf;
            }
        }
        fm_engine_msg(MSG_FMENGINE_SET_FREQ, &playing->cur_frequency, sizeof(int), NULL);
        show_cur_freq(playing, true);
        clear_key_msg_queue( -1, -1 );/*清除多余按键；*/
        break;

        case (KEY_PREV | KEY_TYPE_DOWN):
        if(0 != fm_engine_msg(MSG_FMENGINE_AUTO_SEEK_OVER, NULL, 0, &seek_over))
        {
            break;
        }
        
        if (true != seek_over)
        {
            break;
        }
        if (playing->use_preset == true)
        {
            if( playing->is_userchannel_mode == false )
            {
                playing->cur_preset_channel_id = get_prev_channel_index(playing->channel_list_info.channel_freq,
                        playing->cur_preset_channel_id);
                if (playing->cur_preset_channel_id == -1)
                {
                    playing->use_preset = false;
                }
                else
                {
                    playing->cur_frequency = playing->channel_list_info.channel_freq[playing->cur_preset_channel_id];
                }
            }
            else
            {
                playing->cur_preset_channel_id = get_prev_channel_index(channel_info.channel_freq,
                        playing->cur_preset_channel_id);
                if (playing->cur_preset_channel_id == -1)
                {
                    playing->use_preset = false;
                }
                else
                {
                    playing->cur_frequency = channel_info.channel_freq[playing->cur_preset_channel_id];
                }                    
            }
            
        }
        else
        {
     
            get_band_range(playing->cur_band, &maxf, &minf);
            playing->cur_frequency -= get_band_step(playing->cur_band);
            if (playing->cur_frequency < minf)
            {
                playing->cur_frequency = maxf;
            }
        }
        fm_engine_msg(MSG_FMENGINE_SET_FREQ, &playing->cur_frequency, sizeof(int), NULL);
        show_cur_freq(playing, true);
        clear_key_msg_queue( -1, -1 );/*清除多余按键；*/
        break;

        case (KEY_NEXT | KEY_TYPE_LONG):
        if(0 != fm_engine_msg(MSG_FMENGINE_AUTO_SEEK_OVER, NULL, 0, &seek_over))
        {
            break;
        }
        
        if ((true != seek_over) || (playing->use_preset == true))
        {
            break;
        }
        
        playing->searching_flag = HALF_AUTO_SEARCH_ADD;
        clear_key_msg_queue( -1, -1 );/*清除多余按键；*/
       
        break;

        case (KEY_PREV | KEY_TYPE_LONG):
        if(0 != fm_engine_msg(MSG_FMENGINE_AUTO_SEEK_OVER, NULL, 0, &seek_over))
        {
            break;
        }
        
        if ((true != seek_over) || (playing->use_preset == true))
        {
            break;
        }
        
        playing->searching_flag = HALF_AUTO_SEARCH_MINUS;
        clear_key_msg_queue( -1, -1 );/*清除多余按键；*/
       
        break;

        case (KEY_NEXT | KEY_TYPE_HOLD):

        if(playing->use_preset == true)
        {
            if(playing->is_userchannel_mode == false)
            {   
                playing->cur_preset_channel_id = get_next_channel_index(playing->channel_list_info.channel_freq,
                        playing->cur_preset_channel_id);
                if (playing->cur_preset_channel_id == -1)
                {
                    playing->use_preset = false;
                }
                else
                {
                    playing->cur_frequency = playing->channel_list_info.channel_freq[playing->cur_preset_channel_id];
                }
            }
            else
            {
                playing->cur_preset_channel_id = get_next_channel_index(channel_info.channel_freq,
                        playing->cur_preset_channel_id);
                if (playing->cur_preset_channel_id == -1)
                {
                    playing->use_preset = false;
                }
                else
                {
                    playing->cur_frequency = channel_info.channel_freq[playing->cur_preset_channel_id];
                }                   
            }
            fm_engine_msg(MSG_FMENGINE_SET_FREQ, &playing->cur_frequency, sizeof(int), NULL);
            show_cur_freq(playing, true);
        }
        clear_key_msg_queue( -1, -1 );/*清除多余按键；*/                        
        break;
        
        case (KEY_PREV | KEY_TYPE_HOLD):

        if(playing->use_preset == true)
        {
            if( playing->is_userchannel_mode == false )
            {
                playing->cur_preset_channel_id = get_prev_channel_index(playing->channel_list_info.channel_freq,
                        playing->cur_preset_channel_id);
                if (playing->cur_preset_channel_id == -1)
                {
                    playing->use_preset = false;
                }
                else
                {
                    playing->cur_frequency = playing->channel_list_info.channel_freq[playing->cur_preset_channel_id];
                }
            }
            else
            {
                playing->cur_preset_channel_id = get_prev_channel_index(channel_info.channel_freq,
                        playing->cur_preset_channel_id);
                if (playing->cur_preset_channel_id == -1)
                {
                    playing->use_preset = false;
                }
                else
                {
                    playing->cur_frequency = channel_info.channel_freq[playing->cur_preset_channel_id];
                }                   
            }
            fm_engine_msg(MSG_FMENGINE_SET_FREQ, &playing->cur_frequency, sizeof(int), NULL);
            show_cur_freq(playing, true);
        }
        clear_key_msg_queue( -1, -1 );/*清除多余按键；*/
        break;

        default:
        break;
    }
    return true;
}



/* scene msg process */
static bool _scene_deal_msg(radio_scene_playing_t *playing, gui_wm_message_t *pmsg)
{
    key_event_t *kmsg;
    /*
     int i;
     bool flag;
     */

    switch (pmsg->msgid)
    {
        case WM_MSG_CREATE:
        break;

        case WM_MSG_PAINT:
        print_dbg("WM_MSG_PAINT: %s---%d---\n", __FILE__, __LINE__);
        break;

        case WM_MSG_KEY:
        kmsg = (key_event_t *) (pmsg->data.p); /*get key msg*/
        _scene_proc_key(playing,kmsg);
        break;
        default:
        gui_wm_default_callback(pmsg);
        break;
    }

    return true;
}
/******************************************************************************/
/*!                    
 * \par  Description:     scene rotate
*******************************************************************************/
static bool _scene_playing_rotate(void)
{
    fusion_effect_t eff;
    fusion_get_config(FUSION_TYPE_ROTATE,  FUSION_FORWARD, &eff);

    fusion_set_effect(&eff);


    fusion_set_first_frame(NULL);

    _playing_close_theme();
    _playing_open_theme();

    _scene_playing_style_init(g_scene_playing);

    gui_wm_set_size(g_scene_playing->win_id, g_scene_playing->scene->width, g_scene_playing->scene->height);

    _scene_playing_paint(g_scene_playing);

    if (g_scene_playing->headbar != NULL)
    {
        headbar_load_style(g_scene_playing->headbar);
        headbar_paint(g_scene_playing->headbar);
    }

    if (g_scene_playing->adjust != NULL)
    {
        adjust_load_style(g_scene_playing->adjust);
        adjust_paint(g_scene_playing->adjust);
    }

    if (g_scene_playing->dialog != NULL)
    {
        dialog_load_style(g_scene_playing->dialog);
        dialog_paint(g_scene_playing->dialog);
    }

    if (g_scene_playing->volumebar != NULL)
    {
        volumebar_load_style(g_scene_playing->volumebar);
        volumebar_paint(g_scene_playing->volumebar);
    }

    if (g_scene_playing->msgbox != NULL)
    {
        msgbox_load_style(g_scene_playing->msgbox);
        msgbox_paint(g_scene_playing->msgbox);
    }

    if (g_scene_playing->option_menu != NULL)
    {
        menuview_load_style(g_scene_playing->option_menu, MENUVIEW_OPTION);
        menuview_paint(g_scene_playing->option_menu);
    }

    

    fusion_set_last_frame(NULL);
 
    fusion_display(NULL);

    return true;

}
/******************************************************************************/
/*!                    
 * \par  Description:   process sys message  
*******************************************************************************/
static int _playing_proc_sys(msg_apps_t *msg)
{
    msg_reply_t t;

    switch (msg->type)
    {
        case MSG_ACTIVE_SYNC:
        t.type = MSG_REPLY_SUCCESS;
        reply_sync_msg(&t);

        break;

        case MSG_GSENSOR_ROTATION_CHANGED:
        _scene_playing_rotate();
        break;

        case MSG_APP_QUIT:
        print_dbg("recieve MSG_APP_QUIT\n");
        g_scene_playing->next_scene = RADIO_SCENE_EXIT;
        exit_msg_loop();
        break;

        case MSG_USB_HOST_DISK_OUT:
        case MSG_CARD_OUT:
        case MSG_USB_HOST_DISK_IN:
        case MSG_CARD_IN:
        /*先弹提示框*/
        if (g_scene_playing->msgbox != NULL)
        {
            msgbox_proc_sys(g_scene_playing->msgbox, msg);
        }
        /*更新菜单项*/
        update_savepath();
        
        break;
        case MSG_MAIN_DISK_CHANGED:
/*        存储器检查
        if(false == _get_sys_path())
        {
            return false;
        }
        _check_default_file(g_scene_playing);
*/      
        g_scene_playing->parse_default_file_flag = false;
        /*主盘变化，用户电台列表文件需更新*/
        update_userfilelist();

        break;
        default:
        if (g_scene_playing->msgbox != NULL)
        {
            msgbox_proc_sys(g_scene_playing->msgbox, msg);
        }
        break;
    }
    return 0;
}

/* scene callback function */
static void _scene_callback(gui_wm_message_t *pmsg)
{
    int top;

    /*top canvas process*/
    top = canvas_get_focus(pmsg);

    /*dispatch msg*/
    if ((g_scene_playing->option_menu != NULL) && (top == g_scene_playing->option_menu->canvas_id))
    {
        menuview_proc_gui(g_scene_playing->option_menu, pmsg);
    }
    else if ((g_scene_playing->volumebar != NULL) && (top == g_scene_playing->volumebar->canvas_id))
    {
        volumebar_proc_gui(g_scene_playing->volumebar, pmsg);
    }
    else if ((g_scene_playing->adjust != NULL) && (top == g_scene_playing->adjust->canvas_id))
    {
        adjust_proc_gui(g_scene_playing->adjust, pmsg);
    }
    else if ((g_scene_playing->dialog != NULL) && (top == g_scene_playing->dialog->canvas_id))
    {
        dialog_proc_gui(g_scene_playing->dialog, pmsg);
    }
    else if ((g_scene_playing->msgbox != NULL) && (top == g_scene_playing->msgbox->canvas_id))
    {
        msgbox_proc_gui(g_scene_playing->msgbox, pmsg);
    }
    else
    {
        _scene_deal_msg(g_scene_playing, pmsg);
    }
}

/* scene entry function */
int radio_scene_playing(fusion_enter_e fenter)
{
    bool ret;
    msg_apps_t msg;
    int next_scene = RADIO_SCENE_EXIT;

    /* 场景初始化 */
    ret = _scene_init(fenter);
    if (ret == false)
    {
        print_err("scene init error! \n");
        goto radio_err_exit;
    }
    else
    {
        print_dbg("--: %s---%d---\n", __FILE__, __LINE__);
    }

    register_sys_dispatcher(_playing_proc_sys);

    /* 场景消息循环 */
    while (get_msg(&msg) == true)
    {
        dispatch_msg(&msg);
    }
    
    next_scene = g_scene_playing->next_scene;  
    
    radio_err_exit:
    /* 场景退出 */
    ret = _scene_deinit();
    if (ret == false)
    {
        print_err("scene exit error! \n");
        return -1;
    }
    else
    {
        print_dbg("--: %s---%d---\n", __FILE__, __LINE__);
    }

    unregister_sys_dispatcher();

    return next_scene;
}

