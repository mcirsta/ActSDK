/*
 *******************************************************************************
 *                       NOYA1100
 *                (c) Copyright 2002-2008, Actions Co,Ld.
 *                        All Right Reserved
 * FileName: app_msg.h
 * Description:
 * History:
 *       <author>    <time>          <version >      <desc>
 *       shaofeng    2009/03/10      1.0             建立该文件。
 *******************************************************************************
 */

#ifndef __APP_MSG_H__
#define __APP_MSG_H__

#define FUCNTION_SYMBOL
#include <include_psp.h>

#define MSG_CONTENT_SIZE   (1024)

#define GLOBAL_SYS_MQ_FOR_FOREGROUND_APP "/global_mq/foreground_app" 

typedef enum
{
    MSG_NULL = 0,
    
    //message of shortcut key
    MSG_POWER_OFF = 100,
    MSG_SHORTCUT_TVOUT,
    MSG_SHORTCUT_CAMERA,
    MSG_SHORTCUT_PLAYBACK,
    MSG_SHORTCUT_DELETE,
    MSG_SHORTCUT_RETURN,

    /******************************system message*****************************/
    SYSTEM_BASE         = 1000,
    MSG_APP_QUIT,
    MSG_LOW_VOLTAGE,
    MSG_KEYLOCK,
    MSG_KEYUNLOCK,
    MSG_CARD_IN,
    MSG_CARD_INIT_FAILED,
    MSG_CARD_OUT,
    MSG_USB_HOST_DISK_IN,
    MSG_USB_HOST_DISK_INIT_FAILED,
    MSG_USB_HOST_DISK_OUT,
    MSG_USB_B_IN,
    MSG_USB_B_OUT,
    MSG_USB_ADAPTOR_IN,
    MSG_USB_ADAPTOR_OUT,
    MSG_TVIN_ALARM_IN,
    MSG_RESET_DEFAULT_CFG,
    
    MSG_LANGUAGE_CHANGED,
    MSG_FONT_CHANGED,
    MSG_THEME_CHANGED,
    MSG_SET_DEF_FONT_SYNC,
    MSG_UNSET_DEF_FONT_SYNC,

    MSG_MAIN_DISK_CHANGED,
    MSG_NEED_GEN_PLIST,

    MSG_CARD_MOUNT_FAILED,

    /******************************manager message****************************/
    PROCESSMANAGER_BASE = 2000,
    MSG_CREAT_APP,
    MSG_CREAT_APP_SYNC,
    MSG_CREAT_APP_RESUME,    
    MSG_KILL_APP,
    MSG_KILL_APP_SYNC,
    MSG_KILL_USER_APPS,
    MSG_KILL_USER_APPS_SYNC,
    MSG_KILL_USER_APPS_CREAT_APP,
    MSG_ACTIVE,
    MSG_ACTIVE_SYNC,
    MSG_APP_EXITED,
    MSG_SET_APP_FOREGROUND,
    MSG_SET_APP_FOREGROUND_SYNC,
    MSG_SET_APP_BACKGROUND,
    MSG_SET_APP_BACKGROUND_SYNC,
    MSG_BOOTUP_FINISH,
    MSG_BACKGROUND_PIC_CHANGE,      /* 背景图已经更改消息 */
    MSG_KILL_ALL_USER_APPS_SYNC,
    MSG_STOP_HARD_WATCHDG_SYNC,              /* 停止manager内部的看门狗 */
    MSG_RESTART_HARD_WATCHDG_SYNC,     /* 重启manager内部的看门狗 */

    /******************************config message*****************************/
    CONFIG_BASE         = 3000,
    

    /****************************msg_server message***************************/
    MSG_SERVER_BASE        = 4000,
    MSG_SET_KEY_TONE_ON,            /* 打开关闭按键音, 用于setting设置. 优先级高于ENABLE */
    MSG_SET_KEY_TONE_ENABLE,        /* 使能禁止按键音, 用于audio应用设置 */
    
    MSG_SET_SYS_EVT_DETECT_ON_SYNC, /* 是否打开系统事件的监听 */

    MSG_SET_SCREENSAVER_TIME_SYNC,
    MSG_SET_AUTO_STANDBY_TIME_SYNC,
    MSG_SET_TIMED_STUTDOWN_TIME_SYNC,
    MSG_SET_LCD_BACKLIGHT_SYNC,

    MSG_SET_CHARGE_PARAM_SYNC,
    MSG_SET_CHARGE_START_SYNC,     /* 1:开始充电; 0:停止充电 */
    MSG_GET_CHARGE_STATUS_SYNC,

    MSG_SET_KEY_MAPPING_SYNC,
    MSG_SET_VOLUME_SYNC,
    MSG_GET_VOLUME_SYNC,

    MSG_GET_HW_STATUS_SYNC,

    MSG_SET_GSENSOR_ON,
    MSG_RESET_GSENSOR_ROTATION,
    MSG_GSENSOR_ROTATION_CHANGED,
    MSG_GET_GSENSOR_ROTATION_SYNC,
    MSG_RESET_GSENSOR_SHAKE,
    MSG_GSENSOR_SHAKED,

    MSG_RESET_SYS_IDLE_TIMERS_SYNC,
    MSG_RESET_AUTO_STANDBY_TIMER_SYNC,
    MSG_RESET_SCREENSAVER_TIMER_SYNC,
    MSG_ENTER_SCREENSAVER_SYNC,
    MSG_EXIT_SCREENSAVER_SYNC,

    
    MSG_SET_KEY_SERVER_ENABLE_SYNC, /* 1:启动按键服务; 0:禁止按键服务, 用于模拟器游戏 */
    MSG_GET_APP_STATUS_SYNC,
    
    MSG_RESET_TIMED_SHUTDOWN_SYNC,

    /***************************Music engine message**************************/
    MENGINE_BASE        = 5000,
    MSG_MENGINE_GET_ENGINE_INFO_SYNC,
    MSG_MENGINE_GET_FILEPATH_SYNC,

    MSG_MENGINE_SET_EQ_SYNC,
    MSG_MENGINE_SET_PLAYMODE_SYNC,
    MSG_MENGINE_SET_DISPMODE_SYNC,
    MSG_MENGINE_SET_FADE_SYNC,
    MSG_MENGINE_SET_PLAYSPEED_SYNC,

    MSG_MENGINE_SET_AB_AUTO_SYNC,
    MSG_MENGINE_SET_AB_COUNT_SYNC,
    MSG_MENGINE_SET_AB_TIME_SYNC,

    MSG_MENGINE_START_AB_SYNC,
    MSG_MENGINE_CLEAR_AB_SYNC,
    MSG_MENGINE_SET_AB_NEXT_SYNC,
    MSG_MENGINE_SET_AB_PREV_SYNC,

    MSG_MENGINE_PLAY_SYNC,
    MSG_MENGINE_STOP_SYNC,
    MSG_MENGINE_PAUSE_SYNC,
    MSG_MENGINE_RESUME_SYNC,
    MSG_MENGINE_SEEK_SYNC,

    MSG_MENGINE_SET_FILEPATH_SYNC,
    MSG_MENGINE_SET_FILEINDEX_SYNC,
    MSG_MENGINE_PLAY_NEXT_SYNC,
    MSG_MENGINE_PLAY_PREV_SYNC,
    MSG_MENGINE_DELETE_FILE_SYNC,

    MSG_MENGINE_FFWD_SYNC,
    MSG_MENGINE_FBWD_SYNC,
    MSG_MENGINE_CANCEL_FFB_SYNC,
    MSG_MENGINE_SET_FFBSTEP_SYNC,
    MSG_MENGINE_SET_FFBTIME_SYNC,

    MSG_MENGINE_MAKE_BOOKMARK_SYNC,
    MSG_MENGINE_SET_LAST_FILE_SYNC,


    /****************************fm engine message****************************/
    FMENGINE_BASE = 7000,
    MSG_FMENGINE_GET_STEREO_MODE,
    MSG_FMENGINE_GET_INFO,
    MSG_FMENGINE_GET_INTENSITY,
    MSG_FMENGINE_GET_BAND_MODE,
    MSG_FMENGINE_GET_FREQ,
    MSG_FMENGINE_GET_ANTENNA,
    MSG_FMENGINE_SET_FREQ,
    MSG_FMENGINE_SET_THRESHOLD,
    MSG_FMENGINE_SET_BAND_MODE,
    MSG_FMENGINE_SET_PLAY,
    MSG_FMENGINE_SET_STOP,
    MSG_FMENGINE_SET_ENTER_STANDBY,
    MSG_FMENGINE_SET_AUTO_SEEK,
    MSG_FMENGINE_SET_AUTO_SEEK_BREAK,
    MSG_FMENGINE_EARPHONE_NOT_INSERT,
    MSG_FMENGINE_AUTO_SEEK_OVER,
    MSG_FMENGINE_GET_AUTO_SEEK,
    MSG_FMENGINE_LINEIN_INSERT,

    /***************************photo decoder message*************************/
    PHOTO_BASE = 8000,
    MSG_PHOTO_DECODE,
    MSG_GIF_DECODE,

    /*******************************ebook message*****************************/
    EBOOK_BASE = 9000,
    MSG_EBOOK_UPDATE_TOTAL_PAGE,

}msg_apps_type_e;


/*!
 * \brief
 * 应用消息结构定义
 */
typedef struct
{
    /*! 消息类型 */
    msg_apps_type_e  type;
    /*! 消息内容 */
    char content[MSG_CONTENT_SIZE];
    /*! 发送者进程pid */
    pid_t sender_pid;
}msg_apps_t;



typedef enum
{
    /* 回复消息 */
    MSG_REPLY_NO_REPLY,
    MSG_REPLY_SUCCESS,
    MSG_REPLY_FAILED,

    /* 回复消息MSG_MENGINE_GET_STATE_SYNC */
    MSG_REPLY_MENGINE_GET_STATE_PLAYING,
    MSG_REPLY_MENGINE_GET_STATE_PAUSED,
    MSG_REPLY_MENGINE_GET_STATE_STOPED,

}msg_reply_type_e;



/*!
 * \brief
 * 同步消息回执结构定义
 */
typedef struct
{
    /*! 回执类型 */
    msg_reply_type_e  type;
    /*! 回执内容 */
    char content[MSG_CONTENT_SIZE];
}msg_reply_t;

#endif  /*#ifndef __APP_MSG_H__*/

