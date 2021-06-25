/********************************************************************************
 *                              USDK(1100)
 *                             Module: app
 *                 Copyright(c) 2003-2009 Actions Semiconductor,
 *                            All Rights Reserved.
 * File : fmengine_main.h
 * History:
 *      <author>    <time>           <version >             <desc>
 *      binzhang    2009-09-03          v1.0               build this file
 ********************************************************************************/
/*!
 * \file     fmengine_main.h
 * \brief    header of fm radio engine
 * \author   binzhang
 *
 * \version 1.0
 * \date  2009/09/03
 *******************************************************************************/

#ifndef __FMENGINE_MAIN_H__
#define __FMENGINE_MAIN_H__
#include "include_case.h"
/*!
 * \brief  
 *      max channel number saved in engine
 */
#define MAX_CHANNEL_NUM 20

/*!
 * \brief  
 *      搜索到一个电台后播放的时间，单位ms
 */
#define INTER_SEEK_PLAY 1000

/*! 模组支持自动搜台 */
#define REAL_AUTO_SEEK

/*! result of engine auto seek */
typedef struct
{
    /*!  channel number*/
    unsigned int channel_num;
    /*!  channel frequency*/
    unsigned int channel_freq[MAX_CHANNEL_NUM];
} eg_seek_info_t;

/*!
 * \brief  
 *      自动搜台模式
 */
typedef enum
{
    NORMAL_PLAY = 0, /* 正常播放 */
    HALF_AUTO_SEARCH_ADD, /* 前向半自动搜索(遇到台就停) */
    HALF_AUTO_SEARCH_MINUS, /* 后向半自动搜索(遇到台就停) */
    AUTO_SEARCH,
/* 全自动搜索，搜索当前波段所有电台（最多保存20个）*/

} search_state_e;


typedef enum
{
    FM_STOPPED = 0,
    FM_PLAYING, 
    FM_ERROR,
}engine_state_e;

/*!
 * \brief  
 *      自动搜台时应用传入的参数结构
 */
typedef struct
{
    unsigned int min_freq;
    unsigned int max_freq;
    unsigned int step;
    search_state_e mode;
} en_seek_info_t;/*engine neeed info for seeking，该结构体为radio传入参数结构*/


/*!
 * \brief  
 *      use to control auot seek
 */
typedef struct
{
    unsigned int min_freq;
    unsigned int max_freq;
    unsigned int step;
    search_state_e mode;/*half inc 0 ,half dec 1, auto 2 */
    int timer_id_seek;/*to save auto seek timer id*/
    int timer_id_play;/*timer id to play */
    bool seek_over_flg;
    bool need_seek;/*用于在定时器中指示是否发送自动搜台: false 仅查询信息，true 先自动搜台再查询。*/

    unsigned int cur_freq;
} engine_seek_t;
#endif
