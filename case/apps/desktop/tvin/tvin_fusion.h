/********************************************************************************
 *                              USDK(1100)
 *                             Module: tvin 
 *                 Copyright(c) 2003-2008 Actions Semiconductor,
 *                            All Rights Reserved. 
 * file : tvin_fusion.h
 * History:         
 *      <author>     <time>        <version >            <desc>
 *      Jerry Fu     2009-4-27          v1.0             build this file 
 ********************************************************************************/
/*!
 * \file     tvin_fusion.h
 * \brief    tvin fusion control
 * \author   Jerry Fu
 * \version 1.0
 * \date  2009/07/04
*******************************************************************************/

/*! 
 * \defgroup   case_tvin    tvin 模块的数据结构和接口函数
 */
 
#ifndef   __TVIN_FUSION_H__
#define   __TVIN_FUSION_H__

#include "tvin_config.h"

/*
 * 设置当前场景全部区域作为特效的第一帧
 */
void tvin_scene_fusion_set_first_frame(scene_t *scene);


/*
 * 设置当前场景全部区域作为特效的最后一帧
 */
void tvin_scene_fusion_set_last_frame( scene_t *scene );


/*
 * Option菜单特效第一帧
 */
void tvin_option_fusion_set_first_frame( menuview_t *mv );

/*
 * Option菜单特效最后一帧
 */
void tvin_option_fusion_set_last_frame( menuview_t *mv );

#endif



