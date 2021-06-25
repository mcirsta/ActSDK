/********************************************************************************
 *                              USDK(1100)
 *                             Module: app
 *                 Copyright(c) 2003-2009 Actions Semiconductor,
 *                            All Rights Reserved.
 * File : radio_playing_option_path.h
 * History:
 *      <author>    <time>           <version >             <desc>
 *       cheason    2009-04-23          v1.0               build this file
 ********************************************************************************/
/*!
* \file     radio_playing_option_path.h
* \brief    FM播放界面场景下的option菜单保存路径相关操作
* \author   cheason
*
* \version 1.0
* \date  2009/02/25
*******************************************************************************/


#ifndef __RADIO_OPTION_PATH_H__
#define __RADIO_OPTION_PATH_H__

#include "radio_app.h"

typedef enum
{
    PATH_INTERNAL = 0,
    PATH_CARD,
    PATH_EXTERNAL,

}option_path_item_e;

/* 保存目标存储器菜单资源 */
typedef struct
{
    resgroup_resource_t     *resgroup;          /* 波段选择菜单资源组 */

    string_resource_t       *str_internal;      /* 内部存储器 */
    string_resource_t       *str_card;          /* 卡存储器 */
    string_resource_t       *str_external;      /* 外部存储器 */

}res_option_path_t;

/* load sytyle */
void radio_option_path_load_style (resgroup_resource_t *resgroup);
/* menu data init */
menu_t* radio_playing_option_path_init (void);
/* menu data deinit */
int radio_playing_option_path_deinit (void);
/* menu option path enter */
bool radio_playing_option_path_enter( menuview_t *menuview );
/* menu option path exit */
bool radio_playing_option_path_exit( menuview_t *menuview );

void update_savepath(void);

#endif  /* #ifndef __RADIO_OPTION_PATH_H__ */

