/********************************************************************************
 *                              USDK(1100)
 *                             Module: tvin 
 *                 Copyright(c) 2003-2009 Actions Semiconductor,
 *                            All Rights Reserved. 
 * File : tvin_palntsc_widget_list.h
 * History:         
 *      <author>    <time>           <version >             <desc>
 *      Jerry Fu    2009-2-25          v1.0               build this file 
 ********************************************************************************/
/*!
* \file     tvin_palntsc_widget_list.h
* \brief    create menuview of list
* \author   Jerry Fu
*               
* \version 1.0
* \date  2009/02/25
*******************************************************************************/

#ifndef __TVIN_WIDGET_PALNTSC_LIST_H__
#define __TVIN_WIDGET_PALNTSC_LIST_H__

#include "include_case.h"


//typedef bool (*menu_entry_t)( menuview_t * );

menuview_t* tvin_widget_palntsc_list_create(void);
bool tvin_widget_palntsc_list_delete( menuview_t *menuview );
bool tvin_widget_palntsc_list_proc_gui( menuview_t *menuview, gui_wm_message_t *pmsg );

#endif

