/********************************************************************************
 *                              USDK(1100)
 *                             Module: app 
 *                 Copyright(c) 2003-2009 Actions Semiconductor,
 *                            All Rights Reserved. 
 * History:         
 *      <author>    <time>           <version >             <desc>
 *      shaofeng    2009-06-16          v1.0               build this file 
********************************************************************************/
/*!
 * \file     video_list_dirlist.h
 * \brief    
 * \author   shaofeng
 *               
 * \version 1.0
 * \date  2009/06/16
*******************************************************************************/

#ifndef __VIDEO_LIST_DIRLIST_H__
#define __VIDEO_LIST_DIRLIST_H__


#include "include_case.h"




/************************ extern function, ???????? ***************************/
extern bool video_search_thread_create( void *(*start_rtn)(void*) );

extern bool video_list_dirlist_create( menuview_t *menuview, fusion_enter_e fenter );
extern bool video_list_dirlist_delete( void );



#endif  /* __VIDEO_LIST_DIRLIST_H__ */

