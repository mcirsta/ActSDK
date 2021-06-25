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
/*��Ӧ�õ�ͷ�ļ�*/
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
    video��tag�˵���Ҫ�ṩ����Ϣ, ��Ҫ������ʾ */
typedef struct
{
    char file_name[PATH_MAX];
    unsigned int time;
    char time_buffer[VIDEO_BUFFER_LENGTH_STR_TIME];
}video_tag_info_t;


/*!
*\brief
    video��ʾ�ľ�̬�˵���Ҫ�ṩ����Ϣ, ��ȫ������ʾ */
typedef struct
{
    unsigned int style_str_id;
    
    unsigned int style_unsel_pic_id;
    unsigned int style_sel_pic_id;
}video_menu_info_t;
 


/*!
*\brief
    video��ʾ�ľ�̬�˵���Ҫ�ṩ����Ϣ, ��ȫ������ʾ 
* ע��: blending������������궼��8�ı���!
*/
typedef struct
{
    int no_use_canvas_id;
    
    /*! ��blending������ */
    int blending_handle[MAX_BLENDING_AREA+1];
    
    /*! playing����blending�����Զ����ؼ��� */
    int auto_hide_time;
    
    /*! playing �����ui, ��Ҫ�Լ�����canvas */
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
    
    /*! commonui�ؼ�, �Ѿ��ڿؼ��д�����canvas, ���ﲻ��Ҫcanvas_id */
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
    video���ŵ�ѭ��ģʽö�� */
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
    video���ŵ���ʾ����ö�� */
typedef enum
{
    VIDEO_DISPLAYMOE_FULL = 0,//����ȫ����ʾ
    VIDEO_DISPLAYMOE_ORG,     //����ԭʼ�ߴ���ʾ
    VIDEO_DISPLAYMOE_EQU,     //����ȱ�����ʾ
}video_displaymode_e;

/*!
*\brief
    video���Ų���, ��Ҫ���浽config, ÿ���������ȡ���ڿ��Ʋ��� */
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
    video�Ķϵ���Ϣ, ÿ���˳�����videoӦ��ʱ����Ҫ��ȡ 
    ����Ҫnp_index, ��Ϊ�û�����flash��np_index��仯, ��fullpath����仯*/
typedef struct
{
    int np_index;
    char np_fullpath[PATH_MAX];
    
    mmm_video_tag_t np_tag;
    
    video_play_param_t play_param;
}video_save_t;


/*!
*\brief
    video�Ĳ����б�ṹ��, �����ݽṹ�ᴩvideo��scene��, ��video���벥�ŵ����в��� */
typedef struct
{
    int np_index;
    char np_fullpath[PATH_MAX];
    mmm_video_tag_t np_tag;
}video_plist_t;


/*!
*\brief
    video����ģʽö�� */
typedef enum
{
    /*! ����list scene��mainmenu */
    VIDEO_START_MODE_LIST = 0,
    
    /*! ����list scene��dirlist��dvĿ¼ */
    VIDEO_START_MODE_DV_DIRLIST,
    
    /*! ����playing scene��playing���� */
    VIDEO_START_MODE_PLAYING,
}video_start_mode;


typedef struct
{
    video_start_mode start_mode;
    /*! �����˳���������һ���� */
    scene_type_e next_scene;
    /*! ��־λ, �Ƿ���np��, ���Ƿ���ʾnp�˵���. ����:plist_generӦ�ø���list��clear����;
        ɾ����ȫ���Ĳ����ļ�ʱclear����; ��ʼ���ź�set���� */
    bool flag_has_np_file;
    /*��־λ���Ƿ�֧�ֶϵ㲥��*/
    bool flag_resume;
    /*���ŷ�ʽ:false:ʹ��playlist���� true:ʹ���ļ�ѡ��������*/
    bool flag_fsel_mode;

    video_blending_t      blending;
    video_play_param_t    play_param;
    
    /*! scene playing resource */
    video_scene_playing_t scene_playing;
    /*! ��־λ, �Ƿ񱣴浱ǰ����tag��recent_list. ��:���Ź��̵͵�,return,��usb�˳� */
    bool flag_save_tag;
    
    /*! scene swf resource */
    video_scene_swf_t scene_swf;
    
    /*! scene list resource */
    video_scene_list_t scene_list;
    char dv_top_path[PATH_MAX];
    char dv_init_path[PATH_MAX];
    
    /*! Ӧ�õ�common�ؼ���Դ, common widget resource. ��������һ�� */
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
    
    /*! Ӧ�õ�playlist��Դ, �ᴩ�ڸ���scene�� */
    video_plist_t plist;
    char buffer_support_ext_video[40];
    char buffer_support_ext_video_swf[4];
    void *hplist;
    void *nplist;
    void *taglist;
    void *recent_list;

    /*! Ӧ�õ�ϵͳ��Դ, system resource. �볡���޹�, ȫ����video_main.c��ʹ�� */
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
    //��ǰĿ¼�б����ݿ�·��
    const char* playlist_dir;
    //�Ƿ�֧��swf��ʽ
    int swf_support;
    
    style_t *app_style;
    style_t *commonui_style;
}video_vars_t;



/************************ extern variable, �������� ***************************/
extern video_vars_t video_vars;



/************************ extern function, �������� ***************************/
extern bool video_set_scene_direction( rotation_e rotation );
extern int video_proc_sys_msg(msg_apps_t *msg);



#endif  /* __VIDEO_H__ */

