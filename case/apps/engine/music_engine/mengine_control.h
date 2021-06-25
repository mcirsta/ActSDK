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
* \file     music_config.h
* \brief    music config
* \author   songzhining
*               
* \version 1.0
* \date  2009/02/25
*******************************************************************************/

#ifndef __MENGIN_CONFIG_H__
#define __MENGIN_CONFIG_H__

#include "include_case.h"


#include "mengine_playlist.h"

typedef enum
{
	EG_PLAY,    //normal play
	EG_PAUSE,
	EG_STOP,
	
	EG_PLAY_AB,     //null, a, b , do -->ab_status
	EG_PLAY_FAST,   //fb, ff -->fast_status_t	
	
    EG_ERROR,   //in error status
    
}play_status_e;

typedef enum
{
	EG_PLAY_AB_NULL,
	EG_PLAY_AB_A,
	EG_PLAY_AB_B,
	EG_PLAY_AB_DO,
	
}ab_status_e;

typedef enum
{
	EG_FFPLAY,
	EG_FBPLAY
	
}fast_status_e;

typedef enum
{
	EG_SPECTRUM,
	EG_LYRIC,
	EG_EFFECT1,
	EG_EFFECT2,
	EG_EFFECT3,
	
}display_mode_e;

typedef enum
{
	EG_SEQUENCE,
	EG_REPEAT_ONE,
	EG_REPEAT_ALL,
	EG_SHUFFLE,
	EG_SHUFFLE_REPEAT,
	EG_INTRO,
	
}play_mode_e;

typedef enum
{
    EG_ERR_NONE = 0,
        
    /*! 打开文件失败 */
    EG_ERR_OPEN_FILE,    
    /*! 文件格式不支持 */
    EG_ERR_NOT_SUPPORT,    
    /*! 解码出错 */
    EG_ERR_DECODER_ERROR,

    /*! 无license，播放次数已满或是播放时间已经到期 */
    EG_ERR_NO_LICENSE,
    /*! DRM时钟错误 */
    EG_ERR_SECURE_CLOCK,
    /*! DRM信息错误 */
    EG_ERR_DRM_INFO
	
}eg_err_e;

/*!
 *  \brief
 *     EQ模式
 */
typedef enum
{
    /*! 普通模式*/
    EQ_NORMAL=0,
    /*! 摇滚模式*/
    EQ_ROCK,
    /*! 流行模式*/
    EQ_POP,
    /*! 古典模式*/
    EQ_CLASSIC,
    /*! 柔和模式*/
    EQ_SOFT,
    /*! 爵士模式*/
    EQ_JAZZ,
    /*! 重低音模式*/
    EQ_DBB,
    /*! 用户模式*/
    EQ_USR_MODE,
    /*! SRS下的WOW模式*/
    EQ_SRSWOW,
    /*! SRS下的WOW HD模式*/
    EQ_SRSWOW_HD,
    /*! SRS用户模式*/
    EQ_SRSUSR_MODE
}eq_mode_e;

/* EQ类型 */
typedef struct
{
    //Eq类型：0-Norma, 1-Pop, 2-Classic, 3-Soft, 4-Jazz, 5-Rock, 6-DBB, 7-User EQ, 
    //                    8-SRSWOW, 9-SRSWOW HD, 10-SRS User Mode
    //设置EQ参数
    eq_mode_e eq_mode;
    
    /*! 在用户模式时用户需要设置的参数*/
    int user_param[10];
    
    /*! 在SRS用户模式时用户需要设置的参数*/
    int srs_param[10];
    
}mengine_eq_t;

/*!
 *  \brief
 *     断点、标签信息结构
 */
typedef struct
{
	/*! 断点时间 xxms*/
	unsigned int time;
	/*! buf信息*/
	char buf[AUDIO_TAG_SIZE];
}eg_audio_tag_t;

/******************************************************************
*                                            设置信息(保存配置)
*******************************************************************/
typedef struct
{
    //显示模式：0-Spectrum, 1-Lyric, 2-Effect1, 3-Effect2, 4-Effect3
    display_mode_e display_mode;

    //显示特效开关
    bool display_effect;
    
    //循环模式：0-Sequence, 1-Repeat One, 2-Repeat All, 3-Shuffle, 4-Shuffle + Repeat, 5-Intro
    play_mode_e play_mode;

    //Eq模式
    mengine_eq_t eq_val;
    
    //淡入淡出：0-disable, 1-enable
    bool fade;
    
    //变速播放的速度：-4~+4
    int play_speed;

    //快进速度：0-2X, 1-4X, 2-8X, 3-16X, 4-32X
    int ffb_speed;

    //快进过程播放音乐时间
    int ffb_time;
    
    //AB循环的次数：1～10
    int ab_count;

    //AB循环的间隔：1～6秒
    int ab_time;

    //自动AB播放的标志：0-手动, 1-自动
    bool ab_autoplay;

    //在播放列表的文件总数
    int file_total;
    
    //当前文件在播放列表中的索引
    int file_index;
    
    //文件标签信息
    eg_audio_tag_t audio_tag;

    //fsel mode
    bool fsel_mode;
    
}mengine_config_t;


/******************************************************************
 *                                            引擎状态信息
*******************************************************************/
typedef struct
{
    //播放状态
    play_status_e play_status;

    //AB复读状态
    ab_status_e ab_status;
    
    //快进/快退状态
    fast_status_e fast_status;
    
    //错误状态
    eg_err_e err_status;

}mengine_status_t;


/* music engine 配置信息 */
typedef struct
{
	int	magic;

    /******************************************************************
    *                                            引擎状态信息
    *******************************************************************/
    mengine_status_t eg_status;
    
    /******************************************************************
     *                                            中间件状态信息
    *******************************************************************/
    //播放状态信息
    mmm_music_status_t music_status;
    
    //文件信息
    mmm_music_file_info_t  file_info;
    
    //DRM信息
    mmm_license_info_t license_info;

    /******************************************************************
    *                                            设置信息(保存配置)
    *******************************************************************/    
    mengine_config_t config;

}mengine_t;



/*************************************************************************
*                                                                       
*                     全局函数原型                                      
*                                                                      
*************************************************************************/
bool mengine_init( void );
bool mengine_deinit( void );
bool mengine_update( void );

bool mengine_get_engine_info(const msg_apps_t *pmsg);
bool mengine_get_filepath(const msg_apps_t *pmsg);

bool mengine_set_eq(const msg_apps_t *pmsg);
bool mengine_set_playmode(const msg_apps_t *pmsg);
bool mengine_set_displaymode(const msg_apps_t *pmsg);
bool mengine_set_fade(const msg_apps_t *pmsg);
bool mengine_set_playspeed(const msg_apps_t *pmsg);
bool mengine_set_ab_auto(const msg_apps_t *pmsg);
bool mengine_set_ab_count(const msg_apps_t *pmsg);
bool mengine_set_ab_time(const msg_apps_t *pmsg);

bool mengine_start_ab(const msg_apps_t *pmsg);
bool mengine_clear_ab(const msg_apps_t *pmsg);
bool mengine_set_ab_next(const msg_apps_t *pmsg);
bool mengine_set_ab_prev(const msg_apps_t *pmsg);

bool mengine_play(const msg_apps_t *pmsg);
bool mengine_stop(const msg_apps_t *pmsg);
bool mengine_pause(const msg_apps_t *pmsg);
bool mengine_resume(const msg_apps_t *pmsg);
bool mengine_seek(const msg_apps_t *pmsg);

bool mengine_fast_forward(const msg_apps_t *pmsg);
bool mengine_fast_backward(const msg_apps_t *pmsg);
bool mengine_cancel_ffb(const msg_apps_t *pmsg);
bool mengine_set_ffb_step(const msg_apps_t *pmsg);
bool mengine_set_ffb_time(const msg_apps_t *pmsg);

bool mengine_set_filepath(const msg_apps_t *pmsg);
bool mengine_set_fileindex(const msg_apps_t *pmsg);
bool mengine_play_next(const msg_apps_t *pmsg);
bool mengine_play_prev(const msg_apps_t *pmsg);
bool mengine_delete_file(const msg_apps_t *pmsg);

bool  mengine_make_bookmark(const  msg_apps_t *pmsg);

bool  mengine_set_last_file(const  msg_apps_t *pmsg);

bool mengine_gsensor_shaked(const msg_apps_t *pmsg);


#endif

