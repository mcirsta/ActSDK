/********************************************************************************
 *                              USDK(1100)
 *                             Module: app 
 *                 Copyright(c) 2003-2009 Actions Semiconductor,
 *                            All Rights Reserved. 
 * File : music_playing_lyric.h
 * History:         
 *      <author>    <time>           <version >             <desc>
 *    songzhining    2009-2-25          v1.0               build this file 
 ********************************************************************************/
/*!
* \file     music_playing_lyric.h
* \brief    music_playing_lyric operation
* \author   songzhining
*               
* \version 1.0
* \date  2009/02/25
*******************************************************************************/

#ifndef __MUSIC_PLAYING_LYRIC_H__
#define __MUSIC_PLAYING_LYRIC_H__

#include "include_case.h"


typedef struct
{
    char signflag;//正:1,负:-1, 为0表示没有offset
    char offset_p1s;//0.1s值
    char offset_sec; //秒值
    char reserved;
} lyric_offset_t;

typedef struct
{
    int timestick;
    int addr;
} timeindex_t;

typedef struct
{
    int lyric_time_addr; //
    int lyric_total; //
    int lyric_cur;
    timeindex_t timeindex[3];
} lyric_index_t;

typedef struct
{
    int lyric_enable;
    lyric_offset_t lyric_offset;
    lyric_index_t lyric_index;
} lyric_info_t;

typedef struct
{
    int ms;
    char second;
    char minute;
    char hour;
} full_time_t;

bool lyric_init(char *dirent);
bool lyric_get_prev(char *lyric_buf, int len);
bool lyric_get_next(char *lyric_buf, int len);
bool lyric_get_current(char *lyric_buf, int len);
bool lyric_set_current_time(int time);

#endif

