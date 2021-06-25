/********************************************************************************
 *                              USDK(1100)
 *                             Module: tvin 
 *                 Copyright(c) 2003-2008 Actions Semiconductor,
 *                            All Rights Reserved. 
 * file : tvin_palntsc_list.h
 * History:         
 *      <author>       <time>        <version >            <desc>
 *      Jerry Fu      2009-4-27         v1.0             build this file 
 ********************************************************************************/
/*!
 * \file       tvin_palntsc_list.h
 * \brief      define data tvin options
 * \author   Jerry Fu 
 *               
 * \version 1.0
 * \date  2009/04/28
 *******************************************************************************/

/*! 
 * \defgroup   case_tvin    tvin 模块的数据结构和接口函数
 */

#ifndef   __TVIN_PALNTSC_LIST_H__
#define   __TVIN_PALNTSC_LIST_H__

#include "include_case.h"

bool tvin_palntsc_list_enter( menuview_t *menuview );
bool tvin_palntsc_list_exit(menuview_t *menuview);
bool tvin_palntsc_list_deinit(void);

#endif

