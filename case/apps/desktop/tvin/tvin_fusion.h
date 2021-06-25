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
 * \defgroup   case_tvin    tvin ģ������ݽṹ�ͽӿں���
 */
 
#ifndef   __TVIN_FUSION_H__
#define   __TVIN_FUSION_H__

#include "tvin_config.h"

/*
 * ���õ�ǰ����ȫ��������Ϊ��Ч�ĵ�һ֡
 */
void tvin_scene_fusion_set_first_frame(scene_t *scene);


/*
 * ���õ�ǰ����ȫ��������Ϊ��Ч�����һ֡
 */
void tvin_scene_fusion_set_last_frame( scene_t *scene );


/*
 * Option�˵���Ч��һ֡
 */
void tvin_option_fusion_set_first_frame( menuview_t *mv );

/*
 * Option�˵���Ч���һ֡
 */
void tvin_option_fusion_set_last_frame( menuview_t *mv );

#endif



