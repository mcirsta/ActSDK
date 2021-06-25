/*******************************************************************************
 *                              NOYA1100
 *                            Module: video
 *                 Copyright(c) 2003-2008 Actions Semiconductor,
 *                            All Rights Reserved.
 * History:
 *      <author>    <time>           <version >             <desc>
 *      shaofeng    2009-07-02          1.0             build this file
*******************************************************************************/
/*!
 * \file     video_config.c
 * \brief
 * \author   shaofeng
 * \par      GENERAL DESCRIPTION:
 *
 * \par      EXTERNALIZED FUNCTIONS:
 *
 * \version 1.0
 * \date  2009/07/02
*******************************************************************************/

#include "include_psp.h"
#include "include_case.h"

/* include file of this application, 本应用的头文件 */
#include "include_video.h"



#define VIDEO_CFG_NP_INDEX      "video_np_index"
#define VIDEO_CFG_NP_FULLPATH   "video_np_fullpath"
#define VIDEO_CFG_NP_TAG        "video_np_tag"
#define VIDEO_CFG_PLAY_PARAM    "video_play_param"



/* define variable, 变量定义 */
/* 保存到config中的video播放参数 */
static video_save_t video_save;

#if 0
#endif
/******************************************************************************/
/*!
* \par  Description:
*       get saved param
*       从config中获取video的配置信息, 用于初始化video的播放参数
* \param[in]
* \retval
*******************************************************************************/
bool video_cfg_read( void )
{
    int tmp = 0;
    /*是否支持swf文件格式*/
    tmp = get_config( CFG_VIDEO_SWF_SUPPORT, \
        (char *)&video_vars.swf_support, sizeof(video_vars.swf_support) );
    if ( -1 == tmp )
    {
        print_warning("get_config failed");
    }

    tmp = get_config( CFG_VIDEO_FSEL_MODE, \
        (char *)&video_vars.flag_fsel_mode, sizeof(video_vars.flag_fsel_mode) );
    if ( -1 == tmp )
    {
        print_warning("get_config failed");
    }

    /* 支持的后缀 */
    tmp = get_config( CFG_APP_SUPPORT_EXT_VIDEO, \
        (char *)&video_vars.buffer_support_ext_video, sizeof(video_vars.buffer_support_ext_video) );
    if ( -1 == tmp )
    {
        print_warning("get_config failed");
    }
    tmp = get_config( CFG_APP_SUPPORT_EXT_VIDEO_SWF, \
        (char *)&video_vars.buffer_support_ext_video_swf, sizeof(video_vars.buffer_support_ext_video_swf) );
    if ( -1 == tmp )
    {
        print_warning("get_config failed");
    }

    /* 临时播放状态和参数 */
    memset( &video_save, 0x00, sizeof(video_save) );
    //断点使能
    tmp = get_config( CFG_VIDEO_RESUME_VISIBLE, (char *)&video_vars.flag_resume, sizeof(video_vars.flag_resume) );
    if ( -1 == tmp )
    {
        print_warning("get_config failed");
    }
    //get param from config
    tmp = get_config( CFG_VIDEO_NOWPLAYING, (char *)&video_vars.flag_has_np_file, sizeof(video_vars.flag_has_np_file) );
    if ( -1 == tmp )
    {
        print_warning("get_config failed");
    }
    tmp = get_config( VIDEO_CFG_NP_INDEX, (char *)&video_save.np_index, sizeof(video_save.np_index) );
    if ( -1 == tmp )
    {
        print_warning("get_config failed");
    }
    tmp = get_config( VIDEO_CFG_NP_FULLPATH, (char *)video_save.np_fullpath, sizeof(video_save.np_fullpath) );
    if ( -1 == tmp )
    {
        print_warning("get_config failed");
    }
    tmp = get_config( VIDEO_CFG_NP_TAG, (char *)&video_save.np_tag, sizeof(video_save.np_tag) );
    if ( -1 == tmp )
    {
        print_warning("get_config failed");
    }
    tmp = get_config( VIDEO_CFG_PLAY_PARAM, (char *)&video_save.play_param, sizeof(video_save.play_param) );
    if ( -1 == tmp )
    {
        print_warning("get_config failed");
    }

    video_vars.plist.np_index = video_save.np_index;
    strcpy( video_vars.plist.np_fullpath, video_save.np_fullpath );
    video_vars.plist.np_tag = video_save.np_tag;
    video_vars.play_param = video_save.play_param;

    print_dbg("index:%d, fullpath:%s, time:%d, playmode:%d, ffbstep:%d", \
        video_save.np_index, video_save.np_fullpath, video_save.np_tag.time, \
        video_save.play_param.playmode, video_save.play_param.ffb_step );

    return true;
}


/******************************************************************************/
/*!
* \par  Description:
*       save param
*       将video的播放参数保存到config中
* \param[in]
* \retval
*   注意:
*       需要保存到config中的np信息有: 正在播放文件index, fullpath 和 tag.
*       index 与 fullpath的关系是: index优先, 通过index获取的fullpath与config中的fullpath比较
*       如果一致, 则使用index播放; 如果不一致则说明文件有改动, 删除np菜单项
*******************************************************************************/
bool video_cfg_write( void )
{
    int tmp = 0;

    video_save.np_index = video_vars.plist.np_index;
    strcpy( video_save.np_fullpath, video_vars.plist.np_fullpath );
    video_save.np_tag = video_vars.plist.np_tag;
    video_save.play_param = video_vars.play_param;

    tmp = set_config( CFG_VIDEO_RESUME_VISIBLE, (const char *)&video_vars.flag_resume, sizeof(video_vars.flag_resume) );
    if ( -1 == tmp )
    {
        print_warning("set_config failed");
    }
    
    tmp = set_config( CFG_VIDEO_FSEL_MODE, \
            (const char *)&video_vars.flag_fsel_mode, sizeof(video_vars.flag_fsel_mode) );
    if ( -1 == tmp )
    {
        print_warning("set_config failed");
    }
    //save param to config
    tmp = set_config( CFG_VIDEO_NOWPLAYING, \
            (const char *)&video_vars.flag_has_np_file, sizeof(video_vars.flag_has_np_file) );
    if ( -1 == tmp )
    {
        print_warning("set_config failed");
    }
    tmp = set_config( VIDEO_CFG_NP_INDEX, (const char *)&video_save.np_index, sizeof(video_save.np_index) );
    if ( -1 == tmp )
    {
        print_warning("set_config failed");
    }
    tmp = set_config( VIDEO_CFG_NP_FULLPATH, (const char *)video_save.np_fullpath, sizeof(video_save.np_fullpath) );
    if ( -1 == tmp )
    {
        print_warning("set_config failed");
    }
    tmp = set_config( VIDEO_CFG_NP_TAG, (const char *)&video_save.np_tag, sizeof(video_save.np_tag) );
    if ( -1 == tmp )
    {
        print_warning("set_config failed");
    }
    tmp = set_config( VIDEO_CFG_PLAY_PARAM, (const char *)&video_save.play_param, sizeof(video_save.play_param) );
    if ( -1 == tmp )
    {
        print_warning("set_config failed");
    }

    print_dbg("index:%d, fullpath:%s, time:%d, playmode:%d, ffbstep:%d", \
        video_save.np_index, video_save.np_fullpath, video_save.np_tag.time, \
        video_save.play_param.playmode, video_save.play_param.ffb_step );
    return true;
}

