/*******************************************************************************
 *                              USDK(1100)
 *                            Module: camera
 *                 Copyright(c) 2003-2009 Actions Semiconductor,
 *                            All Rights Reserved.
 *
 * History:
 *      <author>    <time>           <version >             <desc>
 *    Albert_lyu 2009-5-26  1.0             build this file
 *******************************************************************************/
/*!
 * \file    camera_main.h
 * \brief    camera主程序头文件
 * \author   Albert_lyu
 * \version 1.0
 * \date  2009-5-26
 *******************************************************************************/
#ifndef __CAMERA_MAIN_H__
#define __CAMERA_MAIN_H__
#include "include_case.h"
#include "include_psp.h"

#define FILE_MAX 1000
#define DSC_EXT "jpg|avi"

//
//================================================================================
typedef enum
{
    CAM_DIALOG_NOSPACE = 0,
    CAM_DIALOG_NOPOWER,
    CAM_DIALOG_NOMEMORY,
    CAM_DIALOG_OPEN_FAIL,    
    CAM_DIALOG_SAVEFILE,
    CAM_DIALOG_SAVE_FAIL,   //保存失败
    CAM_DIALOG_SETPATH_FAIL,
    CAM_DIALOG_INIT_SENSOR_FAIL,    //初始化摄像头失败
    CAM_DIALOG_TVOUT_OPENED,	//tvout 已打开不能同时支持tvout
    CAM_DIALOG_PLUG_IN_USB,
    CAM_DIALOG_NODISK,
    CAM_DIALOG_DISKINSERT,    
    CAM_DIALOG_TAG_ERR = 0xFF,
} cam_dialog_e;

//include "camera_dsc_ui.h"
/*!
 *  \brief
 *  dsc界面icon项目
 */
typedef enum
{
    /*! dsc */
    CAM_ICON_DCSCENE = 0,
    /*! dv */
    CAM_ICON_DVSCENE,
    /*! 自拍 */
    CAM_ICON_SELFTIME,
    /*! 明亮度 */
    CAM_ICON_BRIGHTNESS,
    /*! 特效 */
    CAM_ICON_SPECIAL_EFFECT,
    /*! 白平衡 */
    CAM_ICON_WHITE_BALANCE,
    /*! 闪光灯 */
    CAM_ICON_FLASH_LAMP,
    /*! 拍照音 */
    CAM_ICON_SHUTTER_SOUND,
    /*! 存储设备 */
    CAM_ICON_STORE_TYPE,
    /*! 分辨率 */
    CAM_ICON_RESOLUTION,
    /*! 剩余空间(照片张数) */
    CAM_ICON_FREE_NUMBER, //TODO 注意dv是没有用的    
    /*! 数码变焦比例 */
    CAM_ICON_FOCI_SLIDER,
    //dv
    /*! 曝光模式*/
    CAM_ICON_EXPOSURE_MODE,
    /*! 录像质量*/
    CAM_ICON_RECORD_QUALITY,

    /*! 录制状态*/
    CAM_ICON_REC_STATUS,
    /*! 使用空间(录制时间长) */
    CAM_ICON_USE_FREE_NUMBER, 
    //TODO 注意dv是用来显示dv刻录时间的，看dsc如果没有用,可以改名

    /*! 枚举值错误 */
    CAM_ICON_ERR = 0xFF,
} cam_icon_e;

/*!
 * \brief
 *  ui_slider_s--滚动条
 */
typedef struct ui_slider_s
{
    /*! 显示与否保存值 */
    char saved_show;
    /*! 显示与否 */
    char show;
    /*! y坐标 */
    char y;
    /*! y坐标保存值 */
    char saved_y;
    /*! 背景图片 */
    picture_resource_t* pic_bg_res;
    /*! 高亮图片 */
    picture_resource_t* pic_h_res;
    string_resource_t* max_str_res;
    string_resource_t* min_str_res;
} ui_slider_t;

/*!
 * \brief
 *  ui_pic_s--显示物件（图片）属性
 */
typedef struct ui_pic_s
{
    /*! 显示与否保存值 */
    char saved_show;
    /*! 显示与否 */
    char show;
    /*! 图片 */
    picture_resource_t* pic_res;
} ui_pic_t;

/*!
 * \brief
 *  ui_picregion_s--显示物件（多帧图片）属性
 */
typedef struct ui_picregion_s
{
    /*! 显示与否保存值 */
    char saved_show;
    /*! 显示与否 */
    char saved_frame;
    /*! 图片 */
    char show;
    /*! 帧数 */
    char frame;
    /*! 图片资源组 */
    picregion_resource_t* picregion_res;
} ui_picregion_t;

/*!
 * \brief
 *  ui_string_s--显示物件（字符串）属性
 */
typedef struct ui_string_s
{
    /*! 显示与否保存值 */
    char saved_show;
    /*! 显示与否 */
    char show;
    /*! 保存的字符串 */
    char *saved_buf;
    /*! 新字符串 */
    char *buf;
    //    char saved_buf[20]; //注意不要越界
    //    char buf[20];
    /*! 字符串资源 */
    string_resource_t* str_res;
} ui_string_t;

typedef struct
{
    fb_blending_create_t creat;
    bool show;
    bool change_show_status;
    int canvas_id; 
} camera_blending_section_t;

typedef enum
{
    /*! 场景退出 */
    CAM_SCENE_EXIT = 0, 
    /*! DSC场景 */
    CAM_SCENE_DSC, 
    /*! DV场景 */
    CAM_SCENE_DV, 
    /*! pccamera场景 */
    CAM_SCENE_PC, 
    /*! 场景错误 */
    CAM_SCENE_ERR = 0xFF,
} app_cam_scene_type_e;

typedef struct app_camera_s
{
    /* system resource */
    void* applib_lib;
    void* commonui_lib;
    void* fusion_lib;
    void* gui_lib;
    void* sty_lib;
    void* config_lib;
    void *sqlite_lib;
    void *mnavi_lib;
    style_t* common_style; 
    style_t* app_style;    
    /*! 场景退出后进入的下一场景 */
    app_cam_scene_type_e next_scene; 
} app_camera_t;

extern app_camera_t* g_app_camera;
extern bool g_option_flag;
extern bool g_setpath;
extern int file_num;
extern void scene_fusion_set_first_frame(scene_t *scene);
extern void scene_fusion_set_last_frame(scene_t *scene);
extern void option_fusion_set_first_frame(menuview_t *mv);
extern void option_fusion_set_last_frame(menuview_t *mv);

extern bool camera_check_folder(char *path);

#endif   /* __CAMERA_MAIN_H__ */
