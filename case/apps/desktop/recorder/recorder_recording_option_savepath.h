/********************************************************************************
 *                              USDK(1100)
 *                             Module: app 
 *                 Copyright(c) 2003-2009 Actions Semiconductor,
 *                            All Rights Reserved. 
 * File : recorder_option_save_path.h
 * History:         
 *      <author>    <time>           <version >             <desc>
 *    wangsheng    2009-2-25          v1.0               build this file 
 ********************************************************************************/
/*!
* \file     recorder_option.h
* \author   wangsheng
*               
* \version 1.0
* \date  2009/02/25
*******************************************************************************/

#ifndef __RECORDER_RECORDING_OPTION_SAVEPATH_H__
#define __RECORDER_RECORDING_OPTION_SAVEPATH_H__

#include "include_case.h"


#include "recorder_sty.h"

/* 进入主菜单 */
bool recorder_recording_option_savepath_enter( menuview_t *menuview );

/* 退出主菜单 */
bool recorder_recording_option_savepath_exit( menuview_t *menuview );

/* 初始化菜单数据 */
menu_t* recorder_recording_option_savepath_init( void );

/* 删除菜单数据 */
int recorder_recording_option_savepath_deinit( void );

extern void update_savepath(void);


#endif
