/********************************************************************************
 *                              USDK(1100)
 *                             Module: app 
 *                 Copyright(c) 2003-2009 Actions Semiconductor,
 *                            All Rights Reserved. 
 * File : tvin_record_widget_option.h
 * History:         
 *      <author>    <time>           <version >             <desc>
 *     Jerry Fu    2009-2-25          v1.0               build this file 
 ********************************************************************************/
/*!
* \file     tvin_record_widget_option.h
* \brief    创建录像选择菜单的menuview; 退出并且销毁所创建的menuview
* \author   Jerry Fu
*               
* \version 1.0
* \date  2009/02/25
*******************************************************************************/

#ifndef __TVIN_WIDGET_OPTION_H__
#define __TVIN_WIDGET_OPTION_H__

#include "tvin_config.h"

menuview_t* tvin_widget_option_create(void);
bool tvin_widget_option_delete( menuview_t *menuview );
bool tvin_widget_option_proc_gui( menuview_t *menuview, gui_wm_message_t *pmsg );
bool tvin_widget_menu_load_style(menuview_t *mv);
#endif




