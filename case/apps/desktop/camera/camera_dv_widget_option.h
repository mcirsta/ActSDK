/*******************************************************************************
 *                              USDK(1100)
 *                            Module: camera_dv
 *                 Copyright(c) 2003-2009 Actions Semiconductor,
 *                            All Rights Reserved.
 *
 * History:
 *      <author>    <time>           <version >             <desc>
 *    Albert_lyu 2009-5-26  1.0             build this file
 *******************************************************************************/
/*!
 * \file    camera_dv_widget_option.h
 * \brief    菜单物件头文件
 * \author   Albert_lyu
 * \version 1.0
 * \date  2009-5-26
 *******************************************************************************/
#ifndef __TEST_MENUVIEW_WIDGET_OPTION_H__
#define __TEST_MENUVIEW_WIDGET_OPTION_H__

#include "include_case.h"
#include "include_psp.h"

typedef bool (*option_entry_t)(menuview_t *);

extern menuview_t* camera_dv_widget_option_create(menuview_cb_t callback);
extern bool camera_dv_widget_option_delete(menuview_t *menuview);
extern bool camera_dv_widget_option_proc_gui(menuview_t *menuview, gui_wm_message_t *pmsg);
extern bool camera_dv_widget_option_exit(menuview_t *menuview, option_entry_t entry);
extern bool camera_dv_widget_option_init(void);
extern bool camera_dv_widget_option_deinit(void);
#endif
