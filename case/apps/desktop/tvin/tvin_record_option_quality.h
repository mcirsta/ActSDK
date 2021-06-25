/********************************************************************************
 *                              USDK(1100)
 *                             Module: tvin 
 *                 Copyright(c) 2003-2008 Actions Semiconductor,
 *                            All Rights Reserved. 
 * file : tvin_record_option_quality.h
 * History:         
 *      <author>       <time>        <version >            <desc>
 *      Jerry Fu      2009-4-27         v1.0             build this file 
 ********************************************************************************/
/*!
 * \file       tvin_record_option_quality.h
 * \brief      set record quality
 * \author   Jerry Fu 
 *               
 * \version 1.0
 * \date  2009/04/28
 *******************************************************************************/

/*! 
 * \defgroup   case_tvin    tvin ģ������ݽṹ�ͽӿں���
 */

#ifndef   __TVIN_OPTION_RECORD_QUALITY_H__
#define   __TVIN_OPTION_RECORD_QUALITY_H__

#include "tvin_config.h"

bool tvin_option_quality_enter( menuview_t *menuview );
bool tvin_option_quality_deinit(void);

#endif

