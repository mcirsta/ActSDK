/********************************************************************************
 *                              USDK(1100)
 *                             Module: tvin 
 *                 Copyright(c) 2003-2008 Actions Semiconductor,
 *                            All Rights Reserved. 
 * file : tvin_record_option.h
 * History:         
 *      <author>       <time>        <version >            <desc>
 *      Jerry Fu      2009-4-27         v1.0             build this file 
 ********************************************************************************/
/*!
 * \file       tvin_record_option.h
 * \brief      define tvin options
 * \author   Jerry Fu 
 *               
 * \version 1.0
 * \date  2009/04/28
 *******************************************************************************/

/*! 
 * \defgroup   case_tvin    tvin 模块的数据结构和接口函数
 */

#ifndef   __TVIN_RECORD_OPTION_H__
#define   __TVIN_RECORD_OPTION_H__

#include "include_case.h"

bool tvin_option_enter( menuview_t *menuview, fusion_enter_e fenter );
bool tvin_option_exit(menuview_t *menuview);
bool tvin_option_deinit(void);

#endif

