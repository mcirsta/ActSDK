/********************************************************************************
 *                              USDK(1100)
 *                             Module: app
 *                 Copyright(c) 2003-2009 Actions Semiconductor,
 *                            All Rights Reserved.
 * File : radio_playing_option.h
 * History:
 *      <author>    <time>           <version >             <desc>
 *       cheason    2009-04-23          v1.0               build this file
 ********************************************************************************/
/*!
* \file     radio_playing_option.h
* \brief    radio application option definitions
* \author   cheason
*
* \version 1.0
* \date  2009/02/25
*******************************************************************************/


#ifndef __RADIO_PLAYING_OPTION_H__
#define __RADIO_PLAYING_OPTION_H__

#include "radio_app.h"

typedef enum
{
    ITEM_SAVE_DEL_CHANNEL = 0,
    ITEM_AUTO_SEARCH,
    ITEM_CHANNEL_LIST,
    ITEM_FM_REC,
    ITEM_REC_QUALITY,
    ITEM_REC_GAIN,
    ITEM_REC_SAVE_PATH,
    ITEM_SENSITIVITY,
    ITEM_USER_CHANNEL_LIST,
    ITEM_BAND_REGION,
    ITEM_MUTE_PLAY,
}option_item_e;

/* ���Ž���Option�˵�ui��Դ */
typedef struct
{
    resgroup_resource_t     *resgroup;          /* Option�˵���Դ�� */

    string_resource_t       *str_save_channel;  /* ���浱ǰƵ�� */
    string_resource_t       *str_del_channel;   /* ɾ����ǰƵ�� */
    string_resource_t       *str_auto_search;   /* �Զ����� */
    string_resource_t       *str_channel_list;  /* Ƶ���б� */
    string_resource_t       *str_fm_rec;        /* FM¼�� */
    string_resource_t       *str_rec_quality;   /* FM¼������ */
    string_resource_t       *str_rec_gain;      /* FM¼������ */
    string_resource_t       *str_save_path;     /* FM¼���ļ�����λ�� */
    string_resource_t       *str_sensitivity;   /* ������ */
    string_resource_t       *str_user_channel_list;   /* �û��Զ���Ƶ���б���PC���ص��б��� */
    string_resource_t       *str_tuner_region;  /* ����ѡ�� ���й����������ձ���ŷ�ޣ�*/
    string_resource_t       *str_mute;          /* �ر����� */
    string_resource_t       *str_play;          /* �������� */

}res_option_t;

/* menu data init */
void radio_option_load_style (resgroup_resource_t *resgroup);
menu_t* radio_playing_option_init (void);
int radio_playing_option_deinit (void);

/* �������˵� */
bool radio_playing_option_enter( menuview_t *menuview, fusion_type_e fenter);
bool radio_playing_option_exit(menuview_t *menuview);


#endif  /* #ifndef __RADIO_OPTION_H__ */

