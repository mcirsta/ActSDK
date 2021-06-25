/********************************************************************************
 *                              USDK(1100)
 *                             Module: app 
 *                 Copyright(c) 2003-2009 Actions Semiconductor,
 *                            All Rights Reserved. 
 * File : test_menuview_menu_ui.h
 * History:         
 *      <author>    <time>           <version >             <desc>
 *    wangsheng    2009-2-25          v1.0               build this file 
 ********************************************************************************/
/*!
* \file     recorder_recording_widget_option.h
* \brief    recorder_recording widget
* \author   wangsheng
*               
* \version 1.0
* \date  2009/02/25
*******************************************************************************/

#ifndef __RECORDER_RECORDING_WIDGET_OPTION_H__
#define __RECORDER_RECORDING_WIDGET_OPTION_H__


#include "include_case.h"


#include "recorder_sty.h"

#include "recorder_recording_option.h"
#include "recorder_recording_option_fmt.h"
#include "recorder_recording_option_quality.h"
#include "recorder_recording_option_mode.h"
#include "recorder_recording_option_savepath.h"

typedef bool (*option_entry_t)( menuview_t * );

menuview_t* recorder_recording_widget_option_create( menuview_cb_t callback );
bool recorder_recording_widget_option_delete( menuview_t *menuview );
#if 0
bool recorder_recording_widget_option_load_style( menuview_t *menuview );
bool recorder_recording_widget_option_paint( menuview_t *menuview );
bool recorder_recording_widget_option_proc_gui( menuview_t *menuview, gui_wm_message_t *pmsg );
#endif
bool recorder_recording_widget_option_exit( menuview_t *menuview,  option_entry_t entry );

#endif

