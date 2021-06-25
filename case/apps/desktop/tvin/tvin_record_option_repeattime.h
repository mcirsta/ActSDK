/********************************************************************************
 *                              USDK(1100)
 *                             Module: tvin 
 *                 Copyright(c) 2003-2008 Actions Semiconductor,
 *                            All Rights Reserved. 
 * file : tvin_record_option_repeattime.h
 * History:         
 *      <author>       <time>        <version >            <desc>
 *      Jerry Fu      2009-4-27         v1.0             build this file 
 ********************************************************************************/
/*!
 * \file       tvin_record_option_repeattime.h
 * \brief      设置重复录制功能菜单
 * \author   Jerry Fu 
 *               
 * \version 1.0
 * \date  2009/04/28
 *******************************************************************************/

/*! 
 * \defgroup   case_tvin    tvin 模块的数据结构和接口函数
 */
#ifndef   __TVIN_OPTION_REPEATTIME_H__
#define   __TVIN_OPTION_REPEATTIME_H__

#include "tvin_config.h"

bool tvin_option_repeatrec_enter( menuview_t *menuview );
bool tvin_option_repeatrec_deinit( void );
void set_weekday_status(bool  w_ch);

menu_t *fetch_menu_repeattime(void);
void exit_option_repeatrec(void);

#endif

