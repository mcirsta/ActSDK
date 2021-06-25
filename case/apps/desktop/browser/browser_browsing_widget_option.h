/********************************************************************************
 *                              USDK(1100)
 *                             Module: app
 *                 Copyright(c) 2003-2009 Actions Semiconductor,
 *                            All Rights Reserved.
 * File : browser_browsing_widget_option.h
 * History:
 *      <author>    <time>           <version >             <desc>
 *    binannan    2009-05-25          v1.0               build this file
 ********************************************************************************/
#ifndef __BROWSER_BROWSING_WIDGET_OPTION_H__
#define __BROWSER_BROWSING_WIDGET_OPTION_H__
#include "include_case.h"
#include "include_psp.h"

typedef bool (*option_entry_t)( menuview_t * );

menuview_t* browser_browsing_widget_option_create( menuview_cb_t callback );
bool browser_browsing_widget_option_delete( menuview_t *menuview );
bool browser_browsing_widget_option_load_style( menuview_t *menuview );
bool browser_browsing_widget_option_paint( menuview_t *menuview );
bool browser_browsing_widget_option_proc_gui( menuview_t *menuview, gui_wm_message_t *pmsg );
bool browser_browsing_widget_option_exit( menuview_t *menuview,  option_entry_t entry );

extern bool g_low_power_dialog_visible;

#endif
