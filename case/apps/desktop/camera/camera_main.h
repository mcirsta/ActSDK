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
 * \brief    camera������ͷ�ļ�
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
    CAM_DIALOG_SAVE_FAIL,   //����ʧ��
    CAM_DIALOG_SETPATH_FAIL,
    CAM_DIALOG_INIT_SENSOR_FAIL,    //��ʼ������ͷʧ��
    CAM_DIALOG_TVOUT_OPENED,	//tvout �Ѵ򿪲���ͬʱ֧��tvout
    CAM_DIALOG_PLUG_IN_USB,
    CAM_DIALOG_NODISK,
    CAM_DIALOG_DISKINSERT,    
    CAM_DIALOG_TAG_ERR = 0xFF,
} cam_dialog_e;

//include "camera_dsc_ui.h"
/*!
 *  \brief
 *  dsc����icon��Ŀ
 */
typedef enum
{
    /*! dsc */
    CAM_ICON_DCSCENE = 0,
    /*! dv */
    CAM_ICON_DVSCENE,
    /*! ���� */
    CAM_ICON_SELFTIME,
    /*! ������ */
    CAM_ICON_BRIGHTNESS,
    /*! ��Ч */
    CAM_ICON_SPECIAL_EFFECT,
    /*! ��ƽ�� */
    CAM_ICON_WHITE_BALANCE,
    /*! ����� */
    CAM_ICON_FLASH_LAMP,
    /*! ������ */
    CAM_ICON_SHUTTER_SOUND,
    /*! �洢�豸 */
    CAM_ICON_STORE_TYPE,
    /*! �ֱ��� */
    CAM_ICON_RESOLUTION,
    /*! ʣ��ռ�(��Ƭ����) */
    CAM_ICON_FREE_NUMBER, //TODO ע��dv��û���õ�    
    /*! ����佹���� */
    CAM_ICON_FOCI_SLIDER,
    //dv
    /*! �ع�ģʽ*/
    CAM_ICON_EXPOSURE_MODE,
    /*! ¼������*/
    CAM_ICON_RECORD_QUALITY,

    /*! ¼��״̬*/
    CAM_ICON_REC_STATUS,
    /*! ʹ�ÿռ�(¼��ʱ�䳤) */
    CAM_ICON_USE_FREE_NUMBER, 
    //TODO ע��dv��������ʾdv��¼ʱ��ģ���dsc���û����,���Ը���

    /*! ö��ֵ���� */
    CAM_ICON_ERR = 0xFF,
} cam_icon_e;

/*!
 * \brief
 *  ui_slider_s--������
 */
typedef struct ui_slider_s
{
    /*! ��ʾ��񱣴�ֵ */
    char saved_show;
    /*! ��ʾ��� */
    char show;
    /*! y���� */
    char y;
    /*! y���걣��ֵ */
    char saved_y;
    /*! ����ͼƬ */
    picture_resource_t* pic_bg_res;
    /*! ����ͼƬ */
    picture_resource_t* pic_h_res;
    string_resource_t* max_str_res;
    string_resource_t* min_str_res;
} ui_slider_t;

/*!
 * \brief
 *  ui_pic_s--��ʾ�����ͼƬ������
 */
typedef struct ui_pic_s
{
    /*! ��ʾ��񱣴�ֵ */
    char saved_show;
    /*! ��ʾ��� */
    char show;
    /*! ͼƬ */
    picture_resource_t* pic_res;
} ui_pic_t;

/*!
 * \brief
 *  ui_picregion_s--��ʾ�������֡ͼƬ������
 */
typedef struct ui_picregion_s
{
    /*! ��ʾ��񱣴�ֵ */
    char saved_show;
    /*! ��ʾ��� */
    char saved_frame;
    /*! ͼƬ */
    char show;
    /*! ֡�� */
    char frame;
    /*! ͼƬ��Դ�� */
    picregion_resource_t* picregion_res;
} ui_picregion_t;

/*!
 * \brief
 *  ui_string_s--��ʾ������ַ���������
 */
typedef struct ui_string_s
{
    /*! ��ʾ��񱣴�ֵ */
    char saved_show;
    /*! ��ʾ��� */
    char show;
    /*! ������ַ��� */
    char *saved_buf;
    /*! ���ַ��� */
    char *buf;
    //    char saved_buf[20]; //ע�ⲻҪԽ��
    //    char buf[20];
    /*! �ַ�����Դ */
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
    /*! �����˳� */
    CAM_SCENE_EXIT = 0, 
    /*! DSC���� */
    CAM_SCENE_DSC, 
    /*! DV���� */
    CAM_SCENE_DV, 
    /*! pccamera���� */
    CAM_SCENE_PC, 
    /*! �������� */
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
    /*! �����˳���������һ���� */
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
