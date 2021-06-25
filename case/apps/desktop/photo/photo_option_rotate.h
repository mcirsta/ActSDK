/********************************************************************************
 *                              USDK(1100)
 *                             Module: photo app
 *                 Copyright(c) 2003-2009 Actions Semiconductor,
 *                            All Rights Reserved.
 * File : photo_option_rotate.h
 * History:
 *      <author>    <time>           <version >             <desc>
 *       cheason    2009-06-14         v1.0               build this file
 ********************************************************************************/
/*!
 * \file     photo_option_rotate.h
 * \brief    rotation of photo playing option
 * \author   cheason
 *
 * \version 1.0
 * \date  2009/06/14
 *******************************************************************************/

#ifndef __PHOTO_OPTION_ROTATE_H__
#define __PHOTO_OPTION_ROTATE_H__

#include "photo.h"
#include "photo_scene_playing.h"

typedef enum
{
    OPT_ITEM_ROTATE90 = 0,
    OPT_ITEM_ROTATE180,
    OPT_ITEM_ROTATE270,
    OPT_ITEM_ROTATE_MAX,
} opt_rotate_item_e;

bool photo_option_rotate_enter(menuview_t *mv, fusion_enter_e fenter);
bool photo_option_rotate_exit(menuview_t *mv);

#endif

