/********************************************************************************
 *                              USDK(1100)
 *                             Module: app 
 *                 Copyright(c) 2003-2009 Actions Semiconductor,
 *                            All Rights Reserved. 
 * History:         
 *      <author>    <time>           <version >             <desc>
 *      shaofeng    2009-04-01          v1.0               build this file 
********************************************************************************/
/*!
 * \file     video.h
 * \brief    
 * \author   shaofeng
 *               
 * \version 1.0
 * \date  2009/04/01
*******************************************************************************/

#ifndef __VIDEO_H__
#define __VIDEO_H__

#include "include_psp.h"
#include "include_case.h"

//#include "load_menuitem.h"
/*本应用的头文件*/
#include "video_utils.h"
#include "video_config.h"
#include "video_control.h"
#include "video_control_swf.h"
#include "video_commonui.h"

#include "video_scene_list.h"
#include "video_list_widget_menu.h"
#include "video_list_taglist.h"
#include "video_list_dirlist.h"

#include "video_scene_playing.h"
#include "video_playing_ui.h"
#include "video_playing_widget_option.h"
#include "video_playing_option.h"

#include "video_scene_swf.h"



/*!
*\brief
    video的tag菜单需要提供的信息, 主要用于显示 */
typedef struct
{
    char file_name[PATH_MAX];
    unsigned int time;
    char time_buffer[VIDEO_BUFFER_LENGTH_STR_TIME];
}video_tag_info_t;


/*!
*\brief
    video显示的静态菜单需要提供的信息, 完全用于显示 */
typedef struct
{
    unsigned int style_str_id;
    
    unsigned int style_unsel_pic_id;
    unsigned int style_sel_pic_id;
}video_menu_info_t;
 


/*!
*\brief
    video显示的静态菜单需要提供的信息, 完全用于显示 
* 注意: blending区域的所有坐标都是8的倍数!
*/
typedef struct
{
    int no_use_canvas_id;
    
    /*! 各blending区域句柄 */
    int blending_handle[MAX_BLENDING_AREA+1];
    
    /*! playing界面blending区域自动隐藏计数 */
    int auto_hide_time;
    
    /*! playing 界面的ui, 需要自己创建canvas */
    int pbar_canvas_id;
    mmm_blending_create_t pbar;
    bool pbar_is_show;
    
    int time_left_canvas_id;
    mmm_blending_create_t time_left;
    bool time_left_is_show;
    
    int time_right_canvas_id;
    mmm_blending_create_t time_right;
    bool time_right_is_show;
    
    int status_canvas_id;
    mmm_blending_create_t status;
    bool status_is_show;
    
    /*! commonui控件, 已经在控件中创建了canvas, 这里不需要canvas_id */
    mmm_blending_create_t headbar;
    bool headbar_is_show;
    
    mmm_blending_create_t msgbox;
    bool msgbox_is_show;
    
    mmm_blending_create_t dialog;
    bool dialog_is_show;
    
    mmm_blending_create_t volumebar;
    bool volumebar_is_show;
    
    mmm_blending_create_t adjust;
    bool adjust_is_show;
    
    mmm_blending_create_t option;
    bool option_is_show;
}video_blending_t;


/*!
*\brief
    video播放的循环模式枚举 */
typedef enum
{
    VIDEO_PLAYMOE_SEQUENCE = 0,
    VIDEO_PLAYMOE_REPEAT_ONE,
    VIDEO_PLAYMOE_REPEAT_ALL,
    VIDEO_PLAYMOE_SHUFFLE,
    VIDEO_PLAYMOE_SHUFFLE_REPEAT,
    VIDEO_PLAYMOE_MAX,
}video_playmode_e;


/*!
*\brief
    video播放的显示设置枚举 */
typedef enum
{
    VIDEO_DISPLAYMOE_FULL = 0,//画面全屏显示
    VIDEO_DISPLAYMOE_ORG,     //画面原始尺寸显示
    VIDEO_DISPLAYMOE_EQU,     //画面等比例显示
}video_displaymode_e;

/*!
*\brief
    video播放参数, 需要保存到config, 每次启动后读取用于控制播放 */
typedef struct
{
    video_playmode_e playmode;
    video_displaymode_e displaymode;
    int ffb_step;
//reverse    unsigned int ab_count;
//reverse    unsigned int ab_wait_time;
}video_play_param_t;



/*!
*\brief
    video的断点信息, 每次退出进入video应用时都需要读取 
    不需要np_index, 因为用户增减flash后np_index会变化, 而fullpath不会变化*/
typedef struct
{
    int np_index;
    char np_fullpath[PATH_MAX];
    
    mmm_video_tag_t np_tag;
    
    video_play_param_t play_param;
}video_save_t;


/*!
*\brief
    video的播放列表结构体, 此数据结构贯穿video各scene间, 是video进入播放的所有参数 */
typedef struct
{
    int np_index;
    char np_fullpath[PATH_MAX];
    mmm_video_tag_t np_tag;
}video_plist_t;


/*!
*\brief
    video启动模式枚举 */
typedef enum
{
    /*! 进入list scene的mainmenu */
    VIDEO_START_MODE_LIST = 0,
    
    /*! 进入list scene的dirlist的dv目录 */
    VIDEO_START_MODE_DV_DIRLIST,
    
    /*! 进入playing scene的playing界面 */
    VIDEO_START_MODE_PLAYING,
}video_start_mode;


typedef struct
{
    video_start_mode start_mode;
    /*! 场景退出后进入的下一场景 */
    scene_type_e next_scene;
    /*! 标志位, 是否有np项, 即是否显示np菜单项. 用于:plist_gener应用更新list后clear此项;
        删除完全部的播放文件时clear此项; 开始播放后set此项 */
    bool flag_has_np_file;
    /*标志位，是否支持断点播放*/
    bool flag_resume;
    /*播放方式:false:使用playlist播放 true:使用文件选择器播放*/
    bool flag_fsel_mode;

    video_blending_t      blending;
    video_play_param_t    play_param;
    
    /*! scene playing resource */
    video_scene_playing_t scene_playing;
    /*! 标志位, 是否保存当前播放tag到recent_list. 如:播放过程低电,return,插usb退出 */
    bool flag_save_tag;
    
    /*! scene swf resource */
    video_scene_swf_t scene_swf;
    
    /*! scene list resource */
    video_scene_list_t scene_list;
    char dv_top_path[PATH_MAX];
    char dv_init_path[PATH_MAX];
    
    /*! 应用的common控件资源, common widget resource. 场景共用一份 */
    scene_t     *commonui_scene;
    headbar_t   *headbar;
    menuview_t  *menuview;
    menu_t      *menu;
    dirlist_t   *dirlist;
    msgbox_t    *msgbox;
    dialog_t    *dialog;
    video_dialog_e dialog_type;
    volumebar_t *vbar;
    adjust_t    *adjust;
    loadingbox_t    *loadingbox;
    softkeyboard_t  *softkeyboard;
    
    /*! 应用的playlist资源, 贯穿于各个scene间 */
    video_plist_t plist;
    char buffer_support_ext_video[40];
    char buffer_support_ext_video_swf[4];
    void *hplist;
    void *nplist;
    void *taglist;
    void *recent_list;

    /*! 应用的系统资源, system resource. 与场景无关, 全部在video_main.c中使用 */
    void *applib_lib;
    void *commonui_lib;
    void *fusion_lib;
    void *apconfig_lib;
    void *sqlite_lib;
    void *mnavi_lib;
    void *playlist_lib;
    void *gui_lib;
    void *sty_lib;
    int   fontdesc;
    //当前目录列表数据库路径
    const char* playlist_dir;
    //是否支持swf格式
    int swf_support;
    
    style_t *app_style;
    style_t *commonui_style;
}video_vars_t;



/************************ extern variable, 变量声明 ***************************/
extern video_vars_t video_vars;



/************************ extern function, 函数声明 ***************************/
extern bool video_set_scene_direction( rotation_e rotation );
extern int video_proc_sys_msg(msg_apps_t *msg);



#endif  /* __VIDEO_H__ */

