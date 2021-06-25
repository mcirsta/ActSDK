/*******************************************************************************
 *                              NOYA1100
 *                            Module: video
 *                 Copyright(c) 2003-2008 Actions Semiconductor,
 *                            All Rights Reserved. 
 * History:         
 *      <author>    <time>           <version >             <desc>
 *      shaofeng    2009-05-15          1.0             build this file 
*******************************************************************************/
/*!
 * \file     video_list_widget_menu.h
 * \brief    
 * \author   shaofeng
 * \par      GENERAL DESCRIPTION:
 *               
 * \par      EXTERNALIZED FUNCTIONS:
 *               
 * \version 1.0
 * \date  2009/05/15
*******************************************************************************/

#ifndef __VIDEO_MENUVIEW_WIDGET_MENU_H__
#define __VIDEO_MENUVIEW_WIDGET_MENU_H__


#include "include_case.h"


//shaofeng 需要统一放在commonui中
typedef bool (*menu_entry_t)( menuview_t *, fusion_enter_e );


/************************ extern function, 函数声明 ***************************/
extern bool video_list_widget_menu_create( menuview_cb_t callback );
extern bool video_list_widget_menu_delete( menuview_t *menuview );
extern bool video_list_widget_menu_load_style( menuview_t *menuview );
extern bool video_list_widget_menu_paint( menuview_t *menuview );
extern bool video_list_widget_menu_exit( menuview_t *menuview,  menu_entry_t entry );


#endif /* __VIDEO_MENUVIEW_WIDGET_MENU_H__ */

