/********************************************************************************
 *                              USDK(1100)
 *                             Module: app
 *                 Copyright(c) 2003-2009 Actions Semiconductor,
 *                            All Rights Reserved.
 * File : radio_app.h
 * History:
 *      <author>    <time>           <version >             <desc>
 *       cheason    2009-04-23          v1.0               build this file
 ********************************************************************************/
/*!
* \file     radio_app.h
* \brief    radioӦ��ȫ�ֶ���
* \author   cheason
*
* \version 1.0
* \date  2009/02/25
*******************************************************************************/


#ifndef __RADIO_APP_H__
#define __RADIO_APP_H__

#include "include_case.h"

#include "radio_sty.h"

#define RADIO_APP_NAME      "radio.app"
#define FM_EG_APP_NAME      "fmengine.app"
#define MANAGER_APP_NAME    "manager.app"


/* ��󲨶��� */
#define MAX_BAND        4

/* ÿ��������󱣴�Ƶ���� */
#define MAX_CHANNEL_NUM 20

/* scene type define */
typedef enum
{
    RADIO_SCENE_PLAYING_FORWARD = 0,
    RADIO_SCENE_PLAYING_BACKWARD,
    RADIO_SCENE_RECORDING_FORWARD,
    RADIO_SCENE_EXIT,

}app_scene_t;
typedef struct radio_app_s
{
    /* system resource */
    void *applib_lib;
    void *commonui_lib;
    void *fusion_lib;
    void *gui_lib;
    void *sty_lib;
    void *config_lib;
    void *sqlite_lib;
    void *mnavi_lib;

    /* other data */
    /* �����˳���������һ���� */
    app_scene_t next_scene; 
    /*style_tָ��*/
    style_t  * app_style;
    style_t * commonui_style;
    /*����*/
    hotplugin_dev_type_e main_disk;
}radio_app_t;
extern radio_app_t app_data;

#define RADIO_CFG_PLAYING  "radio_cfg_playing"

int fm_engine_msg(msg_apps_type_e msg, const void *send_buff, unsigned int send_length, void *ret_buff);


#endif

