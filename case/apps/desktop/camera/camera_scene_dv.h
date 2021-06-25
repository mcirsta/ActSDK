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
 * \file    camera_scene_dv.h
 * \brief    dv场景头文件
 * \author   Albert_lyu
 * \version 1.0
 * \date  2009-5-26
 *******************************************************************************/
#ifndef  __CAMERA_SCENE_DV_H__
#define  __CAMERA_SCENE_DV_H__
#include "include_case.h"
#include "include_psp.h"

#include "camera_main.h"
#include "camera_define.h"

typedef struct
{
    camera_blending_section_t left_section;
    camera_blending_section_t bottom_section;

    camera_blending_section_t focus_section;
    camera_blending_section_t menu_section;
    camera_blending_section_t volbar_section;
    camera_blending_section_t dialog_section;
    camera_blending_section_t adjust_section;
    camera_blending_section_t msgbox_section;
} camera_dv_blending_t;

typedef enum
{
    /*! 无存储器 */
    CAM_DV_STORAGE_NONE = 0, 
    /*! 内部存储器 */
    CAM_DV_STORAGE_INTERNAL, 
    /*! 卡存储器 */
    CAM_DV_STORAGE_CARD, 
    /*! 外部存储器 */
    CAM_DV_STORAGE_EXTERNAL, 
    /*! 错误枚举值 */
    CAM_DV_STORAGE_ERR = 0xFF
} cam_dv_save_path_e;

typedef enum //暂时不列入动静检测
{
    CHANGE_REC_EVEN_KEY = 0, CHANGE_REC_EVEN_TIMER, CHANGE_REC_EVEN_ERR
} change_rec_even_e;

typedef enum //暂时不列入动静检测
{
    CAM_DV_ACT_DETECT_OFF = 0, CAM_DV_ACT_DETECT_ON, CAM_DV_ACT_DETECT_ERR
} cam_dv_active_detect_e;

typedef enum
{
    CAM_DV_STATUS_RECING = 0, CAM_DV_STATUS_PAUSE, CAM_DV_STATUS_STOP, CAM_DV_STATUS_RESUME,
    /*! 错误枚举值 */
    CAM_DV_STATUS_ERR = 0xFF
} cam_dv_rec_status_e;

typedef struct
{
    mmm_disk_info_t cfg_freeinfo;
    mmm_dv_status_t dv_status;
    mmm_zoom_level_t cfg_zoom_level;
} dv_config_t;

/*!
 * \brief
 *  camera_dv_ui_s--dv显示ui
 */
typedef struct camera_dv_ui_s
{
    /*! 画布id */
    int canvas_id;
    /*! dv-ui背景图片 */
    int bg_pic; //!< background picture
    //int type;			//!< display type
    /*! ui回调 */
    void (*callback)(struct camera_dv_ui_s *, int);

    /*! dv-ui界面资源组 */
    resgroup_resource_t *cam_ui;
    /*! 左边混合区域 */
    ui_pic_t blending_area_left_bg;
    /*! 右边混合区域 */
    ui_pic_t blending_area_botton_bg;
    /*! dc图标 */
    ui_picregion_t dv_icon;
    /*! 明亮度图标 */
    ui_picregion_t brightness;
    /*! 特效图标 */
    ui_picregion_t special_effect;
    /*! 白平衡图标 */
    ui_picregion_t white_balance;
    /*! 录像质量图标 */
    ui_picregion_t record_quality;
    /*! 曝光模式图标 */
    ui_picregion_t exposure_mode;
    /*! 存储体图标 */
    ui_picregion_t store_type;
    /*!录制状态*/
    ui_picregion_t rec_status;
    /*! 剩余空间数值 */
    ui_string_t use_free_number;
    /*! 焦点十字混合区 */
    ui_pic_t blending_area_focus_bg;
    /*! 焦点十字图标 */
    ui_pic_t focus;
    resgroup_resource_t *blending_area_left;
    resgroup_resource_t *blending_area_botton;
    resgroup_resource_t *blending_area_focus;
} camera_dv_ui_t;

typedef struct camera_scene_dv_s
{
    /* style resource */
    scene_t* scene;
    scene_t* common_scene;

    /* font description*/
    int fontdes;

    /* window resource */
    int win_id;
    int hdc;
    /* resource for painting */
    picture_resource_t *dv_bg;
    /* widget resource */
    volumebar_t* vbar;
    menuview_t* opt;
    adjust_t *adjust;
    dialog_t *dialog;
    msgbox_t *msgbox;

    camera_dv_blending_t blending;
    /* other data */
    int need_exit;
    app_cam_scene_type_e exit_type;
    camera_dv_ui_t dv_ui_icon;

    int vbar_timer_id;
    int dv_recordtimer_id;
    dv_config_t dv_config;

} camera_scene_dv_t;

extern camera_scene_dv_t* g_camera_scene_dv;
#if (DV_ICON_STATUS2SDRAM == 1)
extern gui_bitmap_info_t *sdram_buf_icon_info[];
extern picregion_frame_t sdram_buf_icon_frame[];
#endif
int camera_scene_dv(void);

/******************************************************************************/
/*!
 * \par  Description:
 *	  camera_dv_ui_init-- dv-ui初始化
 * \param[in]    ui指针
 * \return       the result      true 成功，false 失败
 * \retval
 * \retval
 * \ingroup      camera_dv
 * \par          exmaple code  none
 * \code
 *
 * \endcode
 * \note
 *******************************************************************************/
bool camera_dv_ui_init(camera_dv_ui_t *dv_ui);
void camera_scene_dv_paint(camera_scene_dv_t *data);
bool camera_dv_blending_deinit(void);
bool camera_dv_dialog_create(cam_dialog_e type);
bool camera_dv_dialog_delete(void);
bool camera_dv_adjust_blending_create_show(void);
bool camera_dv_adjust_blending_delete(void);
bool camera_dv_option_blending_create_show(void);
bool camera_dv_option_blending_delete(void);
bool camera_dv_msgbox_create(void);
bool camera_dv_msgbox_delete(void);
#endif   /* __CAMERA_SCENE_DV_H__ */

