/********************************************************************************
 *                              USDK(1100)
 *                             Module: app
 *                 Copyright(c) 2003-2009 Actions Semiconductor,
 *                            All Rights Reserved.
 * File : radio_ui_playing.c
 * History:
 *      <author>    <time>           <version >             <desc>
 *      cheason    2009-04-23          v1.0               build this file
 ********************************************************************************/
/*!
 * \file     radio_ui_playing.c
 * \brief    routine of radio scene playing ui
 * \author   cheason
 *
 * \version 1.0
 * \date  2009/04/23
 *******************************************************************************/

#include "radio_ui_playing.h"
#include "radio_scene_playing.h"


/*
 * ��ʾ�ź�ǿ��ͼ��
 * ������
 *   data: ���Ž��泡���ṹ��ָ��
 *   show_bg: �Ƿ��ػ�����
 */
void show_intensity_pic(radio_scene_playing_t *playing, bool show_bg)
{
    picregion_frame_t pic_frame;
    int hdc = gui_dc_get((unsigned short)playing->win_id);
    int sh_index;
    short x, y;
    sty_rect_t rect, rect1;

    print_dbg("intensity: %d", playing->cur_intensity);

    if(show_bg == true)
    {
        x = playing->res_playing.picreg_signal->x;
        y = playing->res_playing.picreg_signal->y;
        rect.x = x;
        rect.y = y;
        rect.width = playing->res_playing.picreg_signal->width;
        rect.height = playing->res_playing.picreg_signal->height;
        sty_draw_sub_bitmap(hdc, playing->bg_bitmap_info, x, y, &rect);

        rect1.x = x - playing->res_playing.pic_foreground->x;
        rect1.y = y - playing->res_playing.pic_foreground->y;
        rect1.width = playing->res_playing.picreg_signal->width;
        rect1.height = playing->res_playing.picreg_signal->height;
        sty_draw_sub_picture_id(hdc, playing->res_playing.pic_foreground->pic_id, x, y, &rect1);
    }

    if(playing->cur_intensity >= playing->res_playing.picreg_signal->frames)
    {
        sh_index = playing->res_playing.picreg_signal->frames - 1;
    }
    else
    {
        sh_index = playing->cur_intensity;
    }
    get_picregion_frame(playing->res_playing.picreg_signal, (unsigned short)sh_index, &pic_frame);
    sty_draw_picture_id(hdc, pic_frame.pic_id, pic_frame.x, pic_frame.y);
}

/*
 * ��ʾ��ǰƵ��������״̬ͼ��
 * ������
 *   data: ���Ž��泡���ṹ��ָ��
 *   show_bg: �Ƿ��ػ�����
 *   show: true--��ʾ��false--����
 */
void show_stereo_pic(radio_scene_playing_t *playing, bool show_bg)
{
    short x, y;
    sty_rect_t rect, rect1;
    int hdc = gui_dc_get((unsigned short)playing->win_id);

    if(show_bg == true)
    {
        x = playing->res_playing.pic_stereo->x;
        y = playing->res_playing.pic_stereo->y;
        rect.x = x;
        rect.y = y;
        rect.width = playing->res_playing.pic_stereo->width;
        rect.height = playing->res_playing.pic_stereo->height;
        sty_draw_sub_bitmap(hdc, playing->bg_bitmap_info, x, y, &rect);
        
        rect1.x = x - playing->res_playing.pic_foreground->x;
        rect1.y = y - playing->res_playing.pic_foreground->y;
        rect1.width = playing->res_playing.pic_stereo->width;
        rect1.height = playing->res_playing.pic_stereo->height;
        sty_draw_sub_picture_id(hdc, playing->res_playing.pic_foreground->pic_id, x, y, &rect1);
    }

    if(playing->cur_stereo == true)
    {
        sty_draw_picture_res(hdc, playing->res_playing.pic_stereo);
    }
}

/*
 * ��ʾ��ǰƵ��ָʾͼ��
 * ������
 *   data: ���Ž��泡���ṹ��ָ��
 *   show_bg: �Ƿ��ػ�����
 */
static void show_freq_denotion_pic(radio_scene_playing_t *playing, bool show_bg)
{
    int hdc = gui_dc_get((unsigned short)playing->win_id);
    unsigned int sh_index;
    unsigned int startf, endf,tempv;
    short x, y;
    sty_rect_t rect, rect1;

    if(show_bg == true)
    {
        x = playing->res_playing.pic_freq_bg->x;
        y = playing->res_playing.pic_freq_bg->y;
        rect.x = x;
        rect.y = y;
        rect.width = playing->res_playing.pic_freq_bg->width;
        rect.height = playing->res_playing.pic_freq_bg->height;
        sty_draw_sub_bitmap(hdc, playing->bg_bitmap_info, x, y, &rect);

        rect1.x = x - playing->res_playing.pic_foreground->x;
        rect1.y = y - playing->res_playing.pic_foreground->y;
        rect1.width = playing->res_playing.pic_freq_bg->width;
        rect1.height = playing->res_playing.pic_freq_bg->height;
        sty_draw_sub_picture_id(hdc, playing->res_playing.pic_foreground->pic_id, x, y, &rect1);
    }

    get_band_range(playing->cur_band, &endf, &startf);
    tempv = endf - startf;
    if(tempv != 0)
    {
        sh_index = 180 * ((playing->cur_frequency) - startf) / tempv;
    }
    else
    {
        print_err("endf == startf err! \n");
        return;
    }
    sty_draw_picture_res(hdc, playing->res_playing.pic_freq_bg);
    
    sty_draw_picture_id(hdc, playing->res_playing.pic_freq_notation[sh_index]->pic_id,
                                        playing->res_playing.pic_freq_notation[sh_index]->x,
                                        playing->res_playing.pic_freq_notation[sh_index]->y);
}

/*
 * ��ʾ��ǰƵ���ַ���
 * ������
 *   data: ���Ž��泡���ṹ��ָ��
 *   show_bg: �Ƿ��ػ�����
 */
static void show_cur_freq_str(radio_scene_playing_t *playing, bool show_bg)
{
    int hdc = gui_dc_get((unsigned short)playing->win_id);
    int number;
    char centi[2] = {0, 0};
    picregion_frame_t pic_frame;
    short x, y, width, height;
    sty_rect_t rect, rect1;

    if(show_bg == true)
    {
        x = playing->res_playing.pic_num_hundreds->x;
        y = playing->res_playing.pic_num_hundreds->y;
        width = playing->res_playing.pic_num_tenth->x
                    + playing->res_playing.pic_num_tenth->width
                    - playing->res_playing.pic_num_hundreds->x;
        height = playing->res_playing.pic_num_tenth->height;
        rect.x = x;
        rect.y = y;
        rect.width = width;
        rect.height = height;
        sty_draw_sub_bitmap(hdc, playing->bg_bitmap_info, x, y, &rect);

        rect1.x = x - playing->res_playing.pic_foreground->x;
        rect1.y = y - playing->res_playing.pic_foreground->y;
        rect1.width = width;
        rect1.height = height;
        sty_draw_sub_picture_id(hdc, playing->res_playing.pic_foreground->pic_id, x, y, &rect1);

        /*centi show  playing->res_playing.str_num_centi*/
        rect.x = playing->res_playing.str_num_centi->x;
        rect.y = playing->res_playing.str_num_centi->y;
        rect.width = playing->res_playing.str_num_centi->width;
        rect.height = playing->res_playing.str_num_centi->height;
        sty_draw_sub_bitmap(hdc, playing->bg_bitmap_info, rect.x, rect.y, &rect);

        rect1.x = playing->res_playing.str_num_centi->x - playing->res_playing.pic_foreground->x;
        rect1.y = playing->res_playing.str_num_centi->y - playing->res_playing.pic_foreground->y;
        rect1.width = playing->res_playing.str_num_centi->width;
        rect1.height = playing->res_playing.str_num_centi->height;
        sty_draw_sub_picture_id(hdc, playing->res_playing.pic_foreground->pic_id, rect.x, rect.y, &rect1);

        
    }

    /* ��λ�� */
    number = ((int)(playing->cur_frequency)/100000)%10;
    if(number > 0)  /* ��λ��ֻ�в�������ʱ����ʾ */
    {
        get_picregion_frame(playing->picreg_num, (unsigned short)number, &pic_frame);
        sty_draw_picture_id(hdc, pic_frame.pic_id, playing->res_playing.pic_num_hundreds->x,
                playing->res_playing.pic_num_hundreds->y);
    }

    /* ʮλ��  */
    number = ((int)(playing->cur_frequency)/10000)%10;
    get_picregion_frame(playing->picreg_num, (unsigned short)number, &pic_frame);
    sty_draw_picture_id(hdc, pic_frame.pic_id, playing->res_playing.pic_num_tens->x,
            playing->res_playing.pic_num_tens->y);

    /* ��λ�� */
    number = ((int)(playing->cur_frequency)/1000)%10;
    get_picregion_frame(playing->picreg_num, (unsigned short)number, &pic_frame);
    sty_draw_picture_id(hdc, pic_frame.pic_id, playing->res_playing.pic_num_units->x,
            playing->res_playing.pic_num_units->y);

    /* С���� */
    sty_draw_picture_res(hdc, playing->res_playing.pic_num_dot);

    /* ʮ��λ */
    number = ((int)(playing->cur_frequency)/100)%10;
    get_picregion_frame(playing->picreg_num, (unsigned short)number, &pic_frame);
    sty_draw_picture_id(hdc, pic_frame.pic_id, playing->res_playing.pic_num_tenth->x,
            playing->res_playing.pic_num_tenth->y);

    /* �ٷ�λ */
    number = ((int)(playing->cur_frequency)/10)%10;
    centi[0] = (char)(number + '0');
    print_dbg("%s", centi);
    sty_draw_string_res_ext(hdc, playing->res_playing.str_num_centi, centi);

    /* MHZ */
    print_dbg("mhz id : %d", playing->res_playing.str_mhz->str_id);
    sty_draw_string_res(hdc, playing->res_playing.str_mhz);
}

/*
 * ��ʾ����
 * ������
 *   playing: �����ṹ��ָ��
 */
void show_country(radio_scene_playing_t *playing)
{
    sty_rect_t rect;
    sty_rect_t rect1;
    resgroup_resource_t *resg_tmp;
    string_resource_t *str;
    
    int hdc = gui_dc_get((unsigned short)playing->win_id);

    rect.x = playing->res_playing.str_country->x;
    rect.y = playing->res_playing.str_country->y;
    rect.width = playing->res_playing.str_country->width;
    rect.height = playing->res_playing.str_country->height;
    sty_draw_sub_bitmap(hdc, playing->bg_bitmap_info, rect.x, rect.y, &rect);

    rect1.x = playing->res_playing.str_country->x - playing->res_playing.pic_foreground->x;
    rect1.y = playing->res_playing.str_country->y - playing->res_playing.pic_foreground->y;
    rect1.width = playing->res_playing.str_country->width;
    rect1.height = playing->res_playing.str_country->height;
    sty_draw_sub_picture_id(hdc, playing->res_playing.pic_foreground->pic_id, rect.x, rect.y, &rect1);

    resg_tmp = (resgroup_resource_t *) get_scene_child(playing->scene, RESGROUP_OPTION_BAND);
    if(playing->cur_band == BAND_CHINA)
    {
        str = (string_resource_t *)get_resgroup_child(resg_tmp, STR_CHINA);
    }
    else if(playing->cur_band == BAND_USA)
    {
        str = (string_resource_t *)get_resgroup_child(resg_tmp, STR_USA);
    }
    else if(playing->cur_band == BAND_JAPAN)
    {
        str = (string_resource_t *)get_resgroup_child(resg_tmp, STR_JAPAN);
    }
    else
    {
        str = (string_resource_t *)get_resgroup_child(resg_tmp, STR_EUROPE);
    }

    if(NULL == str)
    {
        return;
    }
    
    playing->res_playing.str_country->str_id = str->str_id;
    sty_draw_string_res(hdc, playing->res_playing.str_country);
}
/*
 * ��ʾƵ����
 * ������
 *   playing: �����ṹ��ָ��
 */
void show_channel_name(radio_scene_playing_t *playing)
{
    sty_rect_t rect;
    sty_rect_t rect1;

    int hdc = gui_dc_get((unsigned short)playing->win_id);

    rect.x = playing->res_playing.str_channel->x;
    rect.y = playing->res_playing.str_channel->y;
    rect.width = playing->res_playing.str_channel->width;
    rect.height = playing->res_playing.str_channel->height;

    sty_draw_sub_bitmap(hdc, playing->bg_bitmap_info, rect.x, rect.y, &rect);

    rect1.x = playing->res_playing.str_channel->x - playing->res_playing.pic_foreground->x;
    rect1.y = playing->res_playing.str_channel->y - playing->res_playing.pic_foreground->y;
    rect1.width = playing->res_playing.str_channel->width;
    rect1.height = playing->res_playing.str_channel->height;
    sty_draw_sub_picture_id(hdc, playing->res_playing.pic_foreground->pic_id, rect.x, rect.y, &rect1);

    if( (playing->use_preset == true) || (g_force_show_channel_name == true) )
    {  
        if((false == g_force_show_channel_name) && (playing->searching_flag == AUTO_SEARCH))
        {
            print_dbg();
        }
        else
        {
            if(playing->is_userchannel_mode == false)
            {
                sty_draw_string_res_ext(hdc, playing->res_playing.str_channel, 
                        playing->channel_list_info.channel_name[playing->cur_preset_channel_id]);
            }
            else
            {
                sty_draw_string_res_ext(hdc, playing->res_playing.str_channel, 
                        channel_info.channel_name[playing->cur_preset_channel_id]);            
            }
        }
    }
}

/*
 * ��ʾ��ǰƵ��
 */
void show_cur_freq(radio_scene_playing_t *playing, bool show_bg)
{
    unsigned int max,min;
    
    print_dbg("=========cur freq : %d", playing->cur_frequency);

    get_band_range(playing->cur_band, &max, &min); 
    if( (playing->cur_frequency < min) || (playing->cur_frequency > max) )
    {
        playing->cur_frequency = min;
    }
    
    /*������*/
    show_country(playing);
    
    /* ��ʾ��ǰƵ��ֵ*/
    show_cur_freq_str(playing, show_bg); 
   
    /*��Ƶ������*/
    show_channel_name(playing);   

    show_freq_denotion_pic(playing, show_bg);
    /* ��ʾ��ǰ���ε���ʼ��ĩβƵ��*/
    show_start_end_freq_str(playing, show_bg);

}

/*
 * ��ʾʼĩƵ���ַ���
 * ����:
 *   playing: ���Ž��泡���ṹ��ָ��
 *   show_bg: �Ƿ��ػ�����
 */
void show_start_end_freq_str(radio_scene_playing_t *playing, bool show_bg)
{
    static char strs[16], stre[16];
    unsigned int startf, endf;
    sty_rect_t rect, rect1;
    short x, y;

    int hdc = gui_dc_get((unsigned short)playing->win_id);

    if(show_bg == true)
    {
        /*draw start freq*/
        x = playing->res_playing.str_freq_start->x;
        y = playing->res_playing.str_freq_start->y;
        rect.x = x;
        rect.y = y;
        rect.width = playing->res_playing.str_freq_start->width;
        rect.height = playing->res_playing.str_freq_start->height;
        sty_draw_sub_bitmap(hdc, playing->bg_bitmap_info, x, y, &rect);
        
        rect1.x = x - playing->res_playing.pic_foreground->x;
        rect1.y = y - playing->res_playing.pic_foreground->y;
        rect1.width = playing->res_playing.str_freq_start->width;
        rect1.height = playing->res_playing.str_freq_start->height;
        sty_draw_sub_picture_id(hdc, playing->res_playing.pic_foreground->pic_id, x, y, &rect1);
        
        
        /*draw end freq*/
        x = playing->res_playing.str_freq_end->x;
        y = playing->res_playing.str_freq_end->y;
        rect.x = x;
        rect.y = y;
        rect.width = playing->res_playing.str_freq_end->width;
        rect.height = playing->res_playing.str_freq_end->height;
        sty_draw_sub_bitmap(hdc, playing->bg_bitmap_info, x, y, &rect);

        rect1.x = x - playing->res_playing.pic_foreground->x;
        rect1.y = y - playing->res_playing.pic_foreground->y;
        rect1.width = playing->res_playing.str_freq_end->width;
        rect1.height = playing->res_playing.str_freq_end->height;
        sty_draw_sub_picture_id(hdc, playing->res_playing.pic_foreground->pic_id, x, y, &rect1);
    }

    get_band_range(playing->cur_band, &endf, &startf);
    freq_to_str(strs, startf);
    sty_draw_string_res_ext(hdc, playing->res_playing.str_freq_start, strs);
    freq_to_str(stre, endf);
    sty_draw_string_res_ext(hdc, playing->res_playing.str_freq_end, stre);
}

