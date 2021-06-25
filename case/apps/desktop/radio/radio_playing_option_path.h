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
* \brief    FM���Ž��泡���µ�option�˵�����·����ز���
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

/* ����Ŀ��洢���˵���Դ */
typedef struct
{
    resgroup_resource_t     *resgroup;          /* ����ѡ��˵���Դ�� */

    string_resource_t       *str_internal;      /* �ڲ��洢�� */
    string_resource_t       *str_card;          /* ���洢�� */
    string_resource_t       *str_external;      /* �ⲿ�洢�� */

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

