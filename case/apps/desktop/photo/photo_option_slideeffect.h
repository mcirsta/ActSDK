/********************************************************************************
 *                              USDK(1100)
 *                             Module: photo app
 *                 Copyright(c) 2003-2009 Actions Semiconductor,
 *                            All Rights Reserved.
 * File : photo_option_slideeffect.h
 * History:
 *      <author>    <time>           <version >             <desc>
 *       cheason    2009-06-14         v1.0               build this file
 ********************************************************************************/
/*!
 * \file     photo_option_slideeffect.h
 * \brief    slide effect of photo playing option
 * \author   cheason
 *
 * \version 1.0
 * \date  2009/06/14
 *******************************************************************************/

#ifndef __PHOTO_OPTION_SLIDEEFFECT_H__
#define __PHOTO_OPTION_SLIDEEFFECT_H__

#include "photo.h"
#include "photo_scene_playing.h"

bool photo_option_slideeffect_enter(menuview_t *mv, fusion_enter_e fenter);
bool photo_option_slideeffect_exit(menuview_t *mv);

#endif

