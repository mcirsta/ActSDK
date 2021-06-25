/********************************************************************************
 *                              USDK(1100)
 *                             Module: app
 *                 Copyright(c) 2003-2009 Actions Semiconductor,
 *                            All Rights Reserved.
 * File : radio_option_band.c
 * History:
 *      <author>    <time>           <version >             <desc>
 *      cheason    2009-04-23          v1.0               build this file
 ********************************************************************************/
/*!
 * \file     radio_option_band.c
 * \brief    FM���Ž��泡���µ�option�˵�����¼�������ȼ��������ȵȼ���ز���
 * \author   cheason
 *
 * \version 1.0
 * \date  2009/04/23
 *******************************************************************************/

#include "radio_app.h"
#include "radio_playing_option.h"
#include "radio_playing_option_band.h"
#include "radio_ui_playing.h"

static res_option_band_t s_res_option_band =
{ NULL };
static menu_t *s_menu_option_band = NULL;

static void _option_band_cb(menuview_t *menuview, menuview_sig_e signal);
static void _add_item(menu_t *menu_data, menuitem_t *item, string_resource_t *str, bool sub_menu_flag);
static bool _fill_items(menu_t *menu_data, radio_scene_playing_t *playing);

/******************************************************************************/
/*!                    
 * \par  Description:     band option callback
*******************************************************************************/
static void _option_band_cb(menuview_t *menuview, menuview_sig_e signal)
{
    menu_t *cur_menu = menuview->menu;
    int band;
    unsigned int max, min;
    radio_scene_playing_t *playing = g_scene_playing;
    menuitem_t *item;

    switch (signal)
    {
        case WIDGET_SIG_MENUVIEW_OK:
        if (playing->cur_band != s_menu_option_band->current)
        {
            item = menu_get_item(cur_menu, playing->cur_band);
            if (NULL == item)
            {
                print_err();
                break;
            }
            item->sel_icon_id = 0;
            item->unsel_icon_id = 0;

            playing->cur_band = s_menu_option_band->current;

            item = menu_get_item(cur_menu, playing->cur_band);
            if (NULL == item)
            {
                print_err();
                break;
            }
            item->sel_icon_id = playing->pic_select->pic_id;
            item->unsel_icon_id = playing->pic_select->pic_id;

            band = get_radio_band(playing->cur_band);
            fm_engine_msg(MSG_FMENGINE_SET_BAND_MODE, &band, sizeof(int), NULL);

            memset(&playing->channel_list_info, 0, sizeof(channel_info_t));/*���Ƶ���б�*/

            playing->use_preset = false;/*�лص�normalģʽ*/
            playing->is_userchannel_mode = false;/*�лص���̨�б��µ�̨*/

            get_band_range(playing->cur_band, &max, &min);/*��ȡ�²���Ƶ�ʷ�Χ*/
            playing->cur_frequency = min;
           
            fm_engine_msg(MSG_FMENGINE_SET_FREQ, &playing->cur_frequency, sizeof(int), NULL);

            show_cur_freq(playing, true);

            menuview_paint(menuview);
        }
        //�����޸���use_preset���˵�ѡ���п��ܷ����仯���˴�ж�أ��Ա��ڷ��ز˵���ʱ�����³�ʼ���˵�����
        radio_playing_option_deinit();
        break;

        case WIDGET_SIG_MENUVIEW_QUIT:
        radio_playing_option_band_exit(menuview);
        break;

        default:
        break;
    }
}

/******************************************************************************/
/*!                    
 * \par  Description:    fill one item 
*******************************************************************************/
static void _add_item(menu_t *menu_data, menuitem_t *item, string_resource_t *str, bool sub_menu_flag)
{
    item->callback = (item_cb_t) _option_band_cb;
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
    menuitem_t * item;
    menuitem_t * cur_item;

    /* ����˳������ option_band_item_e �����һ�� */
    item = menuitem_create();
    if (item == NULL)
    {
        return false;
    }

    _add_item(menu_data, item, s_res_option_band.str_china, false);
    _add_item(menu_data, item, s_res_option_band.str_usa, false);
    _add_item(menu_data, item, s_res_option_band.str_japan, false);
    _add_item(menu_data, item, s_res_option_band.str_europe, false);

    menuitem_delete(item);
  
    cur_item = menu_get_item(menu_data, playing->cur_band);

    if ((cur_item != NULL) && (playing->pic_select != NULL))
    {
        cur_item->sel_icon_id = playing->pic_select->pic_id;
        cur_item->unsel_icon_id = playing->pic_select->pic_id;
    }

    return true;
}

/******************************************************************************/
/*!                    
 * \par  Description:     ������
*******************************************************************************/
void radio_option_band_load_style(resgroup_resource_t *resgroup)
{
    if (NULL == resgroup)
        return;

    /* resgroup option */
    s_res_option_band.resgroup = resgroup;
    s_res_option_band.str_china = (string_resource_t *) get_resgroup_child(resgroup, STR_CHINA);
    s_res_option_band.str_usa = (string_resource_t *) get_resgroup_child(resgroup, STR_USA);
    s_res_option_band.str_japan = (string_resource_t *) get_resgroup_child(resgroup, STR_JAPAN);
    s_res_option_band.str_europe = (string_resource_t *) get_resgroup_child(resgroup, STR_EUROPE);
}

/******************************************************************************/
/*!                    
 * \par  Description:     band init
*******************************************************************************/
menu_t* radio_playing_option_band_init(void)
{
    if ((NULL == g_scene_playing) || (NULL == s_res_option_band.resgroup))
    {
        return NULL;
    }

    if (NULL == s_menu_option_band)
    {
        s_menu_option_band = menu_create();
        _fill_items(s_menu_option_band, g_scene_playing);
        menu_set_cur_index(s_menu_option_band, 0);
    }

    return s_menu_option_band;
}

/******************************************************************************/
/*!                    
 * \par  Description:     band deinit
*******************************************************************************/
int radio_playing_option_band_deinit(void)
{
    if (NULL != s_menu_option_band)
    {
        menu_delete(s_menu_option_band);
        s_menu_option_band = NULL;
    }

    return 0;
}

/******************************************************************************/
/*!                    
 * \par  Description:     band enter
*******************************************************************************/
bool radio_playing_option_band_enter(menuview_t *menuview)
{
    menu_t *cur_menu;

    cur_menu = radio_playing_option_band_init();

    menuview_set_menu(menuview, cur_menu);
    menuview_paint(menuview);

    return true;
}

/******************************************************************************/
/*!                    
 * \par  Description:     band exit
*******************************************************************************/
bool radio_playing_option_band_exit(menuview_t *menuview)
{
    fusion_type_e fenter;
    fenter = FUSION_BACKWARD;

    radio_playing_option_band_deinit();

    radio_playing_option_enter(menuview, fenter);

    return true;
}
