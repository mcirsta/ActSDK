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
#include "anim_on_sty.h"
#include "animation_poweron.h"
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



/* define variable, �������� */
FILE* s_app_pic_res = NULL;


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
        scene = load_scene( animation_vars.app_style, ANIMATION_POWERON_SIDELONG_SCENE );
    }
    else// if ( (angle == ROTATION_90) || (angle == ROTATION_270) )
    {
        scene = load_scene( animation_vars.app_style, ANIMATION_POWERON_UPRIGHT_SCENE );
    }
    
    if ( scene == NULL )
    {
        print_err("load_scene fail\n\n");
        return false;   
    }
    sty_set_app_scene( scene );
    res_scene->app_scene.scene = scene;
    
    // ��ȡͼƬ��Դ�ļ���
    sprintf(file, "%s/%s.res", sys_get_app_theme_res_dir(), "anim_on");
    // ��Ӧ�õ�ͼƬ��Դ�ļ�
#if CFG_ANIMATION_FAST_DISPLAY == 0
    if(sty_open_app_pic_res(file) == false)
    {
        print_err("open %s failed!", file);
        return false;
    }
#else
    s_app_pic_res = gui_open_ui_res( file );
    if ( NULL == s_app_pic_res )
    {
        return false;
    }
#endif
    
    return true;

}

static bool _close_theme( res_scene_t *res_scene )
{
    //�˳�����
    free_scene( res_scene->app_scene.scene );
    res_scene->app_scene.scene = NULL;
    
#if CFG_ANIMATION_FAST_DISPLAY == 0
    if( sty_close_app_pic_res() == false )
    {
        print_err("close app picture resource error!");
        return false;
    }
#else
    gui_close_resource_file( s_app_pic_res );
    s_app_pic_res = NULL;
#endif
    
    return true;
}

static bool _open_str_res(void)
{
#if 0
    char file[PATH_MAX];  

    // ��ȡӦ���ַ�����Դ�ļ���
    sprintf(file, "%s/%s_%s.str", sys_get_app_string_res_dir(), "anim_on", sys_get_cur_language_codes());
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
    res_scene->app_scene.handle_gui_dc = gui_dc_get(res_scene->app_scene.win_id );
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
#if CFG_ANIMATION_FAST_DISPLAY == 0
static void _draw_reg(res_scene_t *res_scene, int frame)
{
    bool changed;
    static int saved_frame = -1;
    
    changed = ( saved_frame != frame )? true:false;
    
    if (false == changed)
    {
        return;
    }
    sty_draw_picregion_res( res_scene->app_scene.handle_gui_dc, res_scene->reg, frame );
    
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

#else

static void _draw_reg(res_scene_t *res_scene, int frame, int* fb_addr)
{
    picregion_frame_t pic;
    gui_bitmap_info_t bmpinfo;

    get_picregion_frame(res_scene->reg,(unsigned short)frame, &pic );

    gui_load_bitmap_into_buffer(s_app_pic_res, pic.pic_id, (char*)fb_addr, \
        res_scene->app_scene.scene->width*res_scene->app_scene.scene->height*2,  &bmpinfo);
}

/* scene paint function */
static void _paint( res_scene_t *res_scene )
{
    int frames = 0;
    int i = 0;
    int fd = 0;
    int fb_addr = 0;
    int ret;
    
    if ( NULL == res_scene->reg )
    {
        return;
    }
    
    fd = open(DEVICE_FB, O_RDWR, 0);
    if (fd < 0)
    {
        print_err("open device fb failed!\n");
        return;
    }

    ret = ioctl(fd, FBIOGET_FB_ADDR, (int) &fb_addr);
    if (ret < 0)
    {
        print_err("ioctl device fb failed! ret=%d\n", ret);
        close( fd );
        return;
    }
    
    frames = (int)res_scene->reg->frames;
    for ( i=0; i<frames; i++ )
    {
        _draw_reg( res_scene, i, (int*)fb_addr );
        if (ioctl(fd, FBIOSET_UPDATE_FB, 0) < 0)
        {
            print_warning("ioctl device fb failed!");
            break;
        }
        usleep( CFG_ANIMATION_RES_USLEEP_TIME );
    }
    
    close( fd );
}
#endif



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

