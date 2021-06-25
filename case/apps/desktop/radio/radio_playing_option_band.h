/********************************************************************************
 *                              USDK(1100)
 *                             Module: app
 *                 Copyright(c) 2003-2009 Actions Semiconductor,
 *                            All Rights Reserved.
 * File : radio_playing_option_band.h
 * History:
 *      <author>    <time>           <version >             <desc>
 *       cheason    2009-04-23          v1.0               build this file
 ********************************************************************************/
/*!
* \file     radio_playing_option_band.h
* \brief    FM���Ž��泡���µ�option�˵�����ѡ����ز���
* \author   cheason
*
* \version 1.0
* \date  2009/02/25
*******************************************************************************/


#ifndef __RADIO_OPTION_BAND_H__
#define __RADIO_OPTION_BAND_H__

#include "radio_app.h"
#include "radio_scene_playing.h"



/* ����ѡ��˵���Դ */
typedef struct
{
    resgroup_resource_t     *resgroup;          /* ����ѡ��˵���Դ�� */

    string_resource_t       *str_china;         /* �й� */
    string_resource_t       *str_usa;           /* ���� */
    string_resource_t       *str_japan;         /* �ձ� */
    string_resource_t       *str_europe;        /* ŷ�� */

}res_option_band_t;

/* menu data init */
void radio_option_band_load_style (resgroup_resource_t *resgroup);
menu_t* radio_playing_option_band_init (void);
int radio_playing_option_band_deinit (void);
/* �������˵� */
bool radio_playing_option_band_enter( menuview_t *menuview );

/* �˳����˵� */
bool radio_playing_option_band_exit( menuview_t *menuview );

#endif  /* #ifndef __RADIO_OPTION_BAND_H__ */

