/********************************************************************************
 *                              USDK(1100)
 *                             Module: app 
 *                 Copyright(c) 2003-2009 Actions Semiconductor,
 *                            All Rights Reserved. 
 * File : music_config.h
 * History:         
 *      <author>    <time>           <version >             <desc>
 *    songzhining    2009-2-25          v1.0               build this file 
 ********************************************************************************/
/*!
* \file     music_playlist.h
* \brief    music playlist
* \author   songzhining
*               
* \version 1.0
* \date  2009/02/25
*******************************************************************************/

#ifndef __MENGIN_PLAYLIST_H__
#define __MENGIN_PLAYLIST_H__

#include "include_case.h"




/*************************************************************************
*                                                                       
*                     全局函数原型                                      
*                                                                      
*************************************************************************/
bool nplist_init( bool fsel_mode, char *path );
bool nplist_quit( void );
int nplist_get_item_num( void );
int nplist_get_index_by_fullpath( char* fullpath );
plist_detail_info_t* nplist_get_item_by_index( int idx );
bool nplist_del_item_by_index( int idx );
bool nplist_del_item_by_fullpath( char* fullpath );

#endif

