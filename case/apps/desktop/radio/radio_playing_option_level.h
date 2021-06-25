/********************************************************************************
 *                              USDK(1100)
 *                             Module: app
 *                 Copyright(c) 2003-2009 Actions Semiconductor,
 *                            All Rights Reserved.
 * File : radio_playing_option_level.h
 * History:
 *      <author>    <time>           <version >             <desc>
 *       cheason    2009-04-23          v1.0               build this file
 ********************************************************************************/
/*!
* \file     radio_playing_option_level.h
* \brief    FM���Ž��泡���µ�option�˵�����¼�������ȼ��������ȵȼ���ز���
* \author   cheason
*
* \version 1.0
* \date  2009/02/25
*******************************************************************************/


#ifndef __RADIO_OPTION_LEVEL_H__
#define __RADIO_OPTION_LEVEL_H__

#include "radio_app.h"

typedef enum
{
    LEVEL_LOW = 0,
    LEVEL_MEDIUM,
    LEVEL_HIGH,

}option_level_item_e;

/* �˵����� */
typedef enum
{
    LTYPE_REC_QUALITY = 0,
    LTYPE_SENSITIVITY,
}option_level_type_e;

/* �����ȵȼ�ֵ(���õ�fm������threthod) */
#define SEN_LEVEL_LOW       5
#define SEN_LEVEL_MEDIUM   7
#define SEN_LEVEL_HIGH      10

/* ¼�������ȼ��������ȵȼ����ڲ˵���Դ */
typedef struct
{
    resgroup_resource_t     *resgroup;          /* �ȼ����ڲ˵���Դ�� */

    string_resource_t       *str_low;           /* �� */
    string_resource_t       *str_medium;        /* �� */
    string_resource_t       *str_high;          /* �� */

}res_option_level_t;

/* menu data init */
void radio_option_level_load_style (resgroup_resource_t *resgroup);
menu_t* radio_playing_option_level_init (option_level_type_e type);
int radio_playing_option_level_deinit (void);

bool radio_playing_option_level_enter( menuview_t *menuview, option_level_type_e type);
bool radio_playing_option_level_exit( menuview_t *menuview );

#endif  /* #ifndef __RADIO_OPTION_LEVEL_H__ */

