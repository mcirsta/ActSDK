/*******************************************************************************
 *                              NOYA1100
 *                            Module: video
 *                 Copyright(c) 2003-2008 Actions Semiconductor,
 *                            All Rights Reserved. 
 * History:         
 *      <author>    <time>           <version >             <desc>
 *      shaofeng    2009-05-15          1.0             build this file 
*******************************************************************************/
/*!
 * \file     video_playlist.h
 * \brief    
 * \author   shaofeng
 * \par      GENERAL DESCRIPTION:
 *               
 * \par      EXTERNALIZED FUNCTIONS:
 *               
 * \version 1.0
 * \date  2009/05/15
*******************************************************************************/

#ifndef __VIDEO_PLAYLIST_H__
#define __VIDEO_PLAYLIST_H__


#include "include_case.h"
#include "include_psp.h"

/*本应用的头文件*/
#include "video.h"


/************************ extern function, 函数声明 ***************************/
extern bool is_swf_ext( char *fullpath );

extern bool video_init_play_mode( video_playmode_e playmode, int init_index, int total_num );
extern bool video_set_play_mode( video_playmode_e playmode );
extern int  video_get_prev_play_index( loop_switch_e smode );
extern int  video_get_next_play_index( loop_switch_e smode );

extern bool video_playlist_init( void );
extern bool video_playlist_deinit( void );
extern bool video_nplist_init(char *filepath );

extern int  video_plist_get_item_num( void* listhandle );


extern bool video_playlist_get_fullpath_by_index( void *plist, int index_cur, char *fullpath );
extern bool video_playlist_get_filename_by_index( void *plist, int index_cur, char *filename );

extern bool video_nplist_set_to_first( void );
extern bool video_nplist_clear_npinfo( void );
extern bool video_add_taglist_to_nplist( int total_num );
extern bool video_nplist_add_dirlist( dirlist_t *dirlist );
extern bool video_nplist_add_file( char *fullpath );
extern bool video_nplist_del_item( int np_index, video_play_switch_mode_e video_switch_mode, int *next_index );
extern bool video_delete_npfile( char *fullpath, int *next_index );

extern bool video_set_play_mode( video_playmode_e playmode );
extern bool video_set_display_mode( video_displaymode_e displaymode );
extern bool video_nplist_get_index_by_fullpath( char *fullpath, int *np_index );
extern bool video_nplist_setplay_by_index( int np_index, mmm_video_tag_t *np_tag );

extern bool video_taglist_add_item( int np_index, unsigned int time );
extern bool video_taglist_del_item( int tag_index );
extern bool video_taglist_del_items_by_fullpath( char *fullpath );
extern bool video_taglist_get_tag_info( int tag_index, video_tag_info_t *tag_info );

extern bool video_recent_list_get_tag_by_fullpath( char *fullpath, unsigned int *time );
extern bool video_recent_list_add_item( int np_index, unsigned int time );
extern bool video_recent_list_delete_item( int np_index );

#endif /* __VIDEO_PLAYLIST_H__ */

