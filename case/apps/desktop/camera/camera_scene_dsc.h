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
 * \file    camera_scene_dsc.h
 * \brief    dsc����ͷ�ļ�
 * \author   Albert_lyu
 * \version 1.0
 * \date  2009-5-26
 *******************************************************************************/
#ifndef __CAMERA_SCENE_DSC_H__
#define __CAMERA_SCENE_DSC_H__
#include "include_case.h"
#include "include_psp.h"

#include "camera_main.h"
#include "camera_define.h"

typedef struct
{
    // int blending_handle[MAX_BLENDING_AREA+1];
    camera_blending_section_t left_section;
    camera_blending_section_t slider_section;
    camera_blending_section_t bottom_section;
    camera_blending_section_t focus_section;

    camera_blending_section_t menu_section;
    camera_blending_section_t volbar_section;
    camera_blending_section_t dialog_section;
    camera_blending_section_t adjust_section;
    camera_blending_section_t msgbox_section;

} camera_dsc_blending_t;

typedef enum
{
    /*! ���Ĺر� */
    CAM_DSC_SELFTIME_OFF = 0, 
    /*! 5������ */
    CAM_DSC_SELFTIME_5S, 
    /*! 10������ */
    CAM_DSC_SELFTIME_10S, 
    /*! 15������ */
    CAM_DSC_SELFTIME_15S, 
    /*! 20������ */
    CAM_DSC_SELFTIME_20S,
    /*! ����ö��ֵ */
    CAM_DSC_SELFTIME_ERR = 0xFF
} cam_dsc_self_time_e;

typedef enum
{
    /*! ���Ĺر� */
    CAM_DSC_CONT_SHOOTING_OFF = 0,
    /*! 5������ */
    CAM_DSC_CONT_SHOOTING_5S,
    /*! 10������ */
    CAM_DSC_CONT_SHOOTING_10S,
    /*! 15������ */
    CAM_DSC_CONT_SHOOTING_15S,
    /*! 20������ */
    CAM_DSC_CONT_SHOOTING_20S,
    /*! 25������ */
    CAM_DSC_CONT_SHOOTING_25S,    
    /*! 30������ */
    CAM_DSC_CONT_SHOOTING_30S,
    /*! ����ö��ֵ */
    CAM_DSC_CONT_SHOOTING_ERR = 0xFF
} cam_dsc_cont_shooting_e;

typedef enum
{
    /*! �޴洢�� */
    CAM_DSC_STORAGE_NONE = 0,
    /*! �ڲ��洢�� */
    CAM_DSC_STORAGE_INTERNAL , 
    /*! ���洢�� */
    CAM_DSC_STORAGE_CARD, 
    /*! �ⲿ�洢�� */
    CAM_DSC_STORAGE_EXTERNAL, 
    /*! ����ö��ֵ */
    CAM_DSC_STORAGE_ERR = 0xFF
} cam_dsc_save_path_e;

typedef struct
{
    mmm_disk_info_t cfg_freeinfo;
    cam_dsc_self_time_e cfg_adj_self_time;
    cam_dsc_cont_shooting_e cfg_adj_cont_shooting;
    mmm_zoom_level_t cfg_zoom_level;
} dsc_config_t;

/*!
 * \brief
 *  camera_dsc_ui_s--dsc��ʾui
 */
typedef struct camera_dsc_ui_s
{
    /*! ����id */
    int canvas_id;
    /*! dsc-ui����ͼƬ */
    int bg_pic; //!< background picture
    //int type;			//!< display type
    /*! ui�ص� */
    void (*callback)(struct camera_dsc_ui_s *, int);

    /*! dsc-ui������Դ�� */
    resgroup_resource_t *cam_ui;
    /*! ��߻������ */
    ui_pic_t blending_area_left_bg;
    /*! �ұ߻������ */
    ui_pic_t blending_area_botton_bg;
    /*! dcͼ�� */
    ui_picregion_t dsc_icon;
    /*! ����ʱ��ͼ�� */
    ui_picregion_t self_time;
    /*! ������ͼ�� */
    ui_picregion_t brightness;
    /*! ��Чͼ�� */
    ui_picregion_t special_effect;
    /*! ��ƽ��ͼ�� */
    ui_picregion_t white_balance;
    /*! �����ͼ�� */
    ui_picregion_t flash_lamp;
    /*! ������ͼ�� */
    ui_picregion_t shutter_sound;
    /*! �洢��ͼ�� */
    ui_picregion_t store_type;
    /*! �ֱ�����ֵ */
    ui_string_t resolution;
    /*! ����ͼ�� */
    ui_pic_t free_space;
    /*! ʣ��ռ���ֵ */
    ui_string_t free_space_number;
    /*! ����ʮ��ͼ�� */
    ui_pic_t focus;
    /*! ����ʮ�ֻ���� */
    ui_pic_t blending_area_focus_bg;
    /*! ����佹ָʾ�� */
    ui_slider_t foci_slider;

    resgroup_resource_t *blending_area_left;
    resgroup_resource_t *blending_area_botton;
    resgroup_resource_t *blending_area_focus;
    resgroup_resource_t *blending_area_foci_slider;

} camera_dsc_ui_t;

typedef struct camera_scene_dsc_s
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
    picture_resource_t *dsc_bg;
    /* widget resource */
    volumebar_t* vbar;
    menuview_t* opt;
    adjust_t *adjust;
    dialog_t *dialog;
    msgbox_t *msgbox;
    camera_dsc_blending_t blending;
    /* other data */
    int need_exit; 
    app_cam_scene_type_e exit_type;
    camera_dsc_ui_t dsc_ui_icon;

    int vbar_timer_id;
    int dsc_icon_flicker_id;
    int dsc_selftimer_id;
    int dsc_conttimer_id;
    dsc_config_t dsc_config;
} camera_scene_dsc_t;

extern camera_scene_dsc_t* g_camera_scene_dsc;
extern bool g_do_snapshotting ;
extern bool g_dsc_quiting;
extern bool camera_dsc_quit_do(void);
extern bool camera_dsc_conttimer_restart(void);

void camera_dsc_warning_nospace(void);
int camera_scene_dsc(void);
void camera_dsc_kill_timer(camera_scene_dsc_t *data);

/******************************************************************************/
/*!
 * \par  Description:
 *	  camera_dsc_ui_init-- dsc-ui��ʼ��
 * \param[in]    uiָ��
 * \return       the result      true �ɹ���false ʧ��
 * \retval
 * \retval
 * \ingroup      camera_dsc
 * \par          exmaple code  none
 * \code
 *
 * \endcode
 * \note
 *******************************************************************************/
bool camera_dsc_ui_init(camera_dsc_ui_t *dsc_ui);
void camera_scene_dsc_paint(camera_scene_dsc_t *data);
bool camera_dsc_blending_deinit(void);
bool camera_dsc_dialog_create(cam_dialog_e type);
bool camera_dsc_dialog_delete(void);
#if (USE_MODAL_DIALOG == 1)
bool camera_dsc_dialog_modal( cam_dialog_e type );
#endif /* USE_MODAL_DIALOG */
bool camera_dsc_adjust_blending_create_show(void);
bool camera_dsc_adjust_blending_delete(void);
bool camera_dsc_option_blending_create_show(void);
bool camera_dsc_option_blending_delete(void);
bool camera_dsc_msgbox_create(void);
bool camera_dsc_msgbox_delete(void);

#endif   /* __CAMERA_SCENE_DSC_H__ */

