/********************************************************************************
 *                              USDK(1100)
 *                             Module: photo app
 *                 Copyright(c) 2003-2009 Actions Semiconductor,
 *                            All Rights Reserved.
 * File : photo_list_mem.h
 * History:
 *      <author>    <time>           <version >             <desc>
 *       cheason    2009-06-14          v1.0               build this file
 ********************************************************************************/
/*!
 * \file     photo_list_mem.h
 * \brief    photo list mem selection
 * \author   cheason
 *
 * \version 1.0
 * \date  2009/06/14
 *******************************************************************************/

#ifndef __PHOTO_LIST_MEM_H__
#define __PHOTO_LIST_MEM_H__

#include "photo.h"

bool photo_list_mem_enter(menuview_t *mv, fusion_enter_e fenter);
bool photo_list_mem_exit(menuview_t *mv);
bool photo_list_mem_refresh(menuview_t *mv);

#endif

