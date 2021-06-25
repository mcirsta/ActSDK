/********************************************************************************
 *                              USDK(1100)
 *                             Module: photo app
 *                 Copyright(c) 2003-2009 Actions Semiconductor,
 *                            All Rights Reserved.
 * File : photo_list_previewmode.h
 * History:
 *      <author>    <time>           <version >             <desc>
 *       cheason    2009-06-11         v1.0               build this file
 ********************************************************************************/
/*!
 * \file     photo_list_previewmode.h
 * \brief    previewmode photo list
 * \author   cheason
 *
 * \version 1.0
 * \date  2009/06/14
 *******************************************************************************/

#ifndef __PHOTO_LIST_PREVIEWMODE_H__
#define __PHOTO_LIST_PREVIEWMODE_H__

#include "photo.h"

bool photo_list_previewmode_enter(menuview_t *mv, fusion_enter_e fenter);
bool photo_list_previewmode_exit(menuview_t *mv);

#endif

