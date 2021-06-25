/********************************************************************************
 *                              USDK(1100)
 *                             Module: photo app
 *                 Copyright(c) 2003-2009 Actions Semiconductor,
 *                            All Rights Reserved.
 * File : photo_option_setbg.h
 * History:
 *      <author>    <time>           <version >             <desc>
 *       cheason    2009-06-14         v1.0               build this file
 ********************************************************************************/
/*!
 * \file     photo_option_setbg.h
 * \brief    set background of photo playing option
 * \author   cheason
 *
 * \version 1.0
 * \date  2009/06/14
 *******************************************************************************/

#ifndef __PHOTO_OPTION_SETBG_H__
#define __PHOTO_OPTION_SETBG_H__

#include "photo.h"
#include "photo_scene_playing.h"

typedef enum
{
    OPT_ITEM_BACKGROUND = 0,
    OPT_ITEM_BOOTLOGO,
    OPT_ITEM_STANDBYLOGO,
    OPT_ITEM_DEFAULT,
} opt_setbg_item_e;

bool photo_option_setbg_enter(menuview_t *mv, fusion_enter_e fenter);
bool photo_option_setbg_exit(menuview_t *mv);

#endif

