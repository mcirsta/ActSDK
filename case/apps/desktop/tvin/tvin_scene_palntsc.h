/********************************************************************************
 *                              USDK(1100)
 *                             Module: app
 *                 Copyright(c) 2003-2009 Actions Semiconductor,
 *                            All Rights Reserved.
 * File : tvin_scene_palntsc.h
 * History:
 *      <author>    <time>           <version >             <desc>
 *      Jerry Fu    2009-2-25          v1.0               build this file
 ********************************************************************************/
/*!
* \file     tvin_scene_palntsc.h
* \brief    tvin  palntsc scene
* \author   Jerry Fu
*
* \version 1.0
* \date  2009/02/25
*******************************************************************************/
#ifndef     __SCENE_PALNTSC_H__
#define    __SCENE_PALNTSC_H__

#include "tvin_config.h"
#include "tvin_palntsc_widget_list.h"
#include "tvin_palntsc_list.h"


/*!
 *  \par 图像输入格式
 *  \li TV_SIGNAL_PAL : PAL: Phase Alternating Line (逐行倒相)的缩写
 *  \li TV_SIGNAL_NTSC : NTSC:National Television Standards Committee的缩写
 */
typedef  enum
{
    TV_SIGNAL_PAL,
    TV_SIGNAL_NTSC,
    TV_SIGNAL_INVALID,
}tvin_signal_mode_e;


typedef   struct
{
    /*! window resource */
    int win_id;
    int hdc;
    
    /*! commonui */
    menuview_t *menuview;
    menu_t *signal_menu;
    
    /*! font description */
    int fontdes;
    
    /*! resource */
    headbar_t *hbar;
    
    tvin_signal_mode_e  tvin_signal_palntsc;    
    string_resource_t  *str_tvin_pal;
    string_resource_t  *str_tvin_ntsc;
    string_resource_t  *str_cable_info;
    string_resource_t  *str_tvout_info;
    string_resource_t  *str_dev_installed;

    gui_bitmap_info_t  *bg_bitmap_info;
    /*! tvin headbar 上的图标 */
    picture_resource_t  *pic_hb_icon;
    picture_resource_t  *pic_bg;

    /*! tvin headbar 上的title */
    string_resource_t *str_hb_title;
    
    resgroup_resource_t *group;

    /*! scene */
    scene_t *scene;
    scene_t *common_scene;
    scene_tvin_ret_e  next_scene;
    msgbox_t *msgbox;
    dialog_t *dialog;
    
}tvin_scene_palntsc_t;

tvin_scene_palntsc_t *fetch_tvin_palntsc(void);

#endif

