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

/* 播放界面Option菜单ui资源 */
typedef struct
{
    resgroup_resource_t     *resgroup;          /* Option菜单资源组 */

    string_resource_t       *str_save_channel;  /* 保存当前频道 */
    string_resource_t       *str_del_channel;   /* 删除当前频道 */
    string_resource_t       *str_auto_search;   /* 自动搜索 */
    string_resource_t       *str_channel_list;  /* 频道列表 */
    string_resource_t       *str_fm_rec;        /* FM录音 */
    string_resource_t       *str_rec_quality;   /* FM录音质量 */
    string_resource_t       *str_rec_gain;      /* FM录音增益 */
    string_resource_t       *str_save_path;     /* FM录音文件保存位置 */
    string_resource_t       *str_sensitivity;   /* 灵敏度 */
    string_resource_t       *str_user_channel_list;   /* 用户自定义频道列表（从PC下载的列表？） */
    string_resource_t       *str_tuner_region;  /* 波段选择 （中国、美国、日本、欧洲）*/
    string_resource_t       *str_mute;          /* 关闭声音 */
    string_resource_t       *str_play;          /* 开启声音 */

}res_option_t;

/* menu data init */
void radio_option_load_style (resgroup_resource_t *resgroup);
menu_t* radio_playing_option_init (void);
int radio_playing_option_deinit (void);

/* 进入主菜单 */
bool radio_playing_option_enter( menuview_t *menuview, fusion_type_e fenter);
bool radio_playing_option_exit(menuview_t *menuview);


#endif  /* #ifndef __RADIO_OPTION_H__ */

