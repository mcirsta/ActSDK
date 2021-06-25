/********************************************************************************
 *                              USDK(1100)
 *                             Module: app 
 *                 Copyright(c) 2003-2009 Actions Semiconductor,
 *                            All Rights Reserved. 
 * File : recorder_recording_option.h
 * History:         
 *      <author>    <time>           <version >             <desc>
 *    wangsheng    2009-2-25          v1.0               build this file 
 ********************************************************************************/
/*!
* \file     recorder_recording_option.h
* \author   wangsheng
*               
* \version 1.0
* \date  2009/02/25
*******************************************************************************/

#ifndef __RECORDER_RECORDING_OPTION_H__
#define __RECORDER_RECORDING_OPTION_H__

#include "include_case.h"


#include "recorder_sty.h"

/* �������˵� */
bool recorder_recording_option_enter( menuview_t *menuview, fusion_type_e fenter);

/* �˳����˵� */
bool recorder_recording_option_exit( menuview_t *menuview );

/* ��ʼ���˵����� */
menu_t* recorder_recording_option_init( void );

/* ɾ���˵����� */
int recorder_recording_option_deinit( void );


#endif
