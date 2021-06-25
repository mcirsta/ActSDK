/*******************************************************************************
 *                              NOYA1100
 *                            Module: app
 *                 Copyright(c) 2003-2008 Actions Semiconductor,
 *                            All Rights Reserved. 
 * History:         
 *      <author>    <time>           <version >             <desc>
 *      shaofeng    2009-05-09          1.0             build this file 
*******************************************************************************/
/*!
 * \file     animation_res_scene.c
 * \brief    
 * \author   shaofeng
 * \par      GENERAL DESCRIPTION:
 *               
 * \par      EXTERNALIZED FUNCTIONS:
 *               
 * \version 1.0
 * \date  2009/05/09
*******************************************************************************/

#include "include_psp.h"
#include "include_case.h"


/*��Ӧ�õ�ͷ�ļ�*/
#include "anim_off_sty.h"
#include "animation_poweroff.h"
#include "animation_res_scene.h"




/* extern function, �������� */
static bool _open_theme( res_scene_t *res_scene );
static bool _close_theme( res_scene_t *res_scene );
static bool _open_str_res( void );
static bool _close_str_res( void );
static bool _init( res_scene_t *res_scene );
static bool _deinit( res_scene_t *res_scene );
static void _paint( res_scene_t *res_scene );
static void _callback(gui_wm_message_t *pmsg);


#if 0
#endif
/******************************************************************************/
/*!                    
* \par  Description:
*       res����������ط��, ʹ�ó����ĺ�����С, ����������res��Դ
* \param[in]    res_scene_t *: ����ָ��
* \retval       true:�ɹ�; false:ʧ��
*******************************************************************************/
bool res_load_style( res_scene_t *res_scene )
{
    scene_t *scene = NULL;
    
    /* load theme ��Դ�ļ� */
    if(_open_theme( res_scene ) == false)
    {
        return false;
    }

    /* load �ַ�����Դ�ļ� */
    if(_open_str_res() == false)
    {
        return false;
    }

    scene = res_scene->app_scene.scene;
    //load pic region from resource group
    res_scene->reg  = ( picregion_resource_t * )get_scene_child( scene, REG );
    
    return true;
}


static bool _open_theme( res_scene_t *res_scene )
{
    scene_t *scene = NULL;
    rotation_e angle;
    char file[PATH_MAX];

    // ȡ��ͼ�Ƕ�
    angle = sty_get_view_rotation();
    // ������Ļ���� 
    sty_set_screen_rotation(angle);
    
    // ����Ӧ�õ� style ����
    if ( (angle == ROTATION_0) || (angle == ROTATION_180) )
    {
        scene = load_scene( animation_vars.app_style, ANIMATION_POWEROFF_SIDELONG_SCENE );
    }
    else// if ( (angle == ROTATION_90) || (angle == ROTATION_270) )
    {
        scene = load_scene( animation_vars.app_style, ANIMATION_POWEROFF_UPRIGHT_SCENE );
    }

    
    if ( scene == NULL )
    {
        print_err("load_scene fail\n\n");
        return false;   
    }
    sty_set_app_scene( scene );
    res_scene->app_scene.scene = scene;
    
    // ��ȡͼƬ��Դ�ļ���
    sprintf(file, "%s/%s.res", sys_get_app_theme_res_dir(), "anim_off");
    // ��Ӧ�õ�ͼƬ��Դ�ļ�
    if(sty_open_app_pic_res(file) == false)
    {
        print_err("open %s failed!", file);
        return false;
    }
    
    return true;

}

static bool _close_theme( res_scene_t *res_scene )
{
    //�˳�����
    free_scene( res_scene->app_scene.scene );
    res_scene->app_scene.scene = NULL;

    if( sty_close_app_pic_res() == false )
    {
        print_err("close app picture resource error!");
        return false;
    }
    
    return true;
}

static bool _open_str_res(void)
{
#if 0
    char file[PATH_MAX];  

    // ��ȡӦ���ַ�����Դ�ļ���
    sprintf(file, "%s/%s_%s.str", sys_get_app_string_res_dir(), "anim_off", sys_get_cur_language_codes());
    // ��Ӧ�õ��ַ�����Դ�ļ� 
    if(sty_open_app_str_res(file) == false)
    {
        print_err("open %s failed!", file);
        return false;
    }

    // �������Ա���
    sty_set_text_encoding(GUI_UTF8_ENC);
#endif
    return true;
}

static bool _close_str_res(void)
{
#if 0
    if(sty_close_app_str_res() == false)
    {
        print_err("close app string resource error!");
        return false;
    } 

#endif
    return true;
}


/******************************************************************************/
/*!                    
* \par  Description:
*       scene callback function
*       ����cb��gui��Ϣ. ��AP������gui��Ϣ
*******************************************************************************/
static void _callback(gui_wm_message_t *pmsg)
{ 
    gui_wm_default_callback(pmsg);
}


/******************************************************************************/
/*!                    
* \par  Description:
*       scene initial function
*       ������Դ. ��ʼ����Դ
*******************************************************************************/
static bool _init( res_scene_t *res_scene )
{
    bool result = true;
    scene_t *scene = NULL;

    memset( res_scene, 0x00, sizeof(res_scene_t) );
    
    /*** ������Դ, �����κ�ʧ��ֱ�ӷ����˳� ***/
    //���볡��
    result = res_load_style( res_scene );
    if (false == result)
    {
        print_err("res_load_style fail\n\n");
        return false;
    }
    scene = res_scene->app_scene.scene;
    
    //��������
    print_dbg("playwin xywh:%d,%d,%d,%d", scene->x, scene->y, scene->width, scene->height);
    res_scene->app_scene.win_id = gui_wm_create_window(scene->x, scene->y, scene->width, scene->height, \
                                        WM_CREATE_SHOW, _callback, 0);
    if( res_scene->app_scene.win_id < 0 )
    {
        print_err("gui_wm_create_window failed\n\n");
        return false;
    }
    
    //��ȡgui dc���
    res_scene->app_scene.handle_gui_dc = gui_dc_get( res_scene->app_scene.win_id );
    if (res_scene->app_scene.handle_gui_dc <= 0)
    {
        print_err("gui_dc_get fail\n\n");
        return false;
    }
    
    /*** ������Դ�ɹ�, ��ʼ��ʼ�� ***/
    //���ý���
    gui_wm_set_focus( res_scene->app_scene.win_id );
    
    //draw the animation pic
    _paint( res_scene );
    
    //quit
    exit_msg_loop();
    
    return result;
}


/* scene exit function */
static bool _deinit( res_scene_t *res_scene )
{
    bool result = true;

    print_dbg();
    
    //ɾ������
    if ( res_scene->app_scene.win_id > 0 )
    {
        gui_wm_delete_window( res_scene->app_scene.win_id );
        res_scene->app_scene.win_id = -1;
    }
    
    if( _close_theme( res_scene ) == false )
    {
        return false;
    }
    if( _close_str_res() == false )
    {
        return false;
    }
    
    return result;
}


/******************************************************************************/
/*!                    
* \par  Description:
*      draw region of play_pause
* \param[in]    res_scene_t *: ָ��
* \param[in]    int: ��ʾ״̬. 1:show, 0:hide
* \param[in]    int: ��ʾ��֡��.
* \retval       void
* \ingroup      
*******************************************************************************/
static void _draw_reg(res_scene_t *res_scene, int frame)
{
    bool changed;
    static int saved_frame = -1;
    
    changed = ( saved_frame != frame )? true:false;
    
    if (false == changed)
    {
        return;
    }
    sty_draw_picregion_res( res_scene->app_scene.handle_gui_dc, res_scene->reg,(unsigned short) frame );
    
    saved_frame = frame;
}

/* scene paint function */
static void _paint( res_scene_t *res_scene )
{
    int frames = 0;
    int i = 0;
    
    if ( NULL == res_scene->reg )
    {
        return;
    }
    
    frames = (int)res_scene->reg->frames;
    for ( i=0; i<frames; i++ )
    {
        _draw_reg( res_scene, i );
        gui_screen_update();
    }
}



/* scene entry function */
void animation_res_scene( res_scene_t *res_scene )
{
    bool result;
    msg_apps_t  msg;

    print_dbg();
    /* ������ʼ�� */
    result = _init( res_scene );
    if(false == result)
    {
        print_err("scene init error!\n");
        goto ANIMATION_RES_SCENE_ERR_INIT;
    }
    
    register_sys_dispatcher( (sys_msg_proc)proc_sys_msg );

    while( true == get_msg(&msg) )
    {
        dispatch_msg(&msg);
    }

    unregister_sys_dispatcher();

    /* �����˳� */
    ANIMATION_RES_SCENE_ERR_INIT:
    result = _deinit( res_scene );
    
    return;
}

