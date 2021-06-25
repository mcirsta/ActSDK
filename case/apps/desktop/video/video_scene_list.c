/*******************************************************************************
 *                              NOYA1100
 *                            Module: video
 *                 Copyright(c) 2003-2008 Actions Semiconductor,
 *                            All Rights Reserved. 
 * History:         
 *      <author>    <time>           <version >             <desc>
 *      shaofeng    2009-05-15          1.0             build this file 
*******************************************************************************/
/*!
 * \file     video_scene_list.c
 * \brief    
 * \author   shaofeng
 * \par      GENERAL DESCRIPTION:
 *               
 * \par      EXTERNALIZED FUNCTIONS:
 *               
 * \version 1.0
 * \date  2009/05/15
*******************************************************************************/

#include "include_psp.h"
#include "include_case.h"

/* include file of this application, ��Ӧ�õ�ͷ�ļ� */
#include "include_video.h"


/* extern function, �������� */
static bool _open_theme( video_scene_list_t *scene_list );
static bool _close_theme( video_scene_list_t *scene_list );
static bool _open_str_res( void );
static bool _close_str_res( void );
static bool _scene_list_init( video_scene_list_t *scene_list );
static bool _scene_list_deinit( video_scene_list_t *scene_list );
static void _scene_list_paint( video_scene_list_t *scene_list );
static bool _scene_list_proc_gui( video_scene_list_t *scene_list, gui_wm_message_t *pmsg );
static void _scene_list_cb(gui_wm_message_t *pmsg);


static const clk_domain_t s_clk_domain = {CLK_LOAD_MIN, CLK_LOAD_HIGH_0};
static bool s_rotate_process_enter = true;

#if 0
#endif
bool video_scene_list_load_style( video_scene_list_t *scene_list )
{
    scene_t *scene = NULL;
    
    /* load theme ��Դ�ļ� */
    if(_open_theme( scene_list ) == false)
    {
        return false;
    }

    /* load �ַ�����Դ�ļ� */
    if(_open_str_res() == false)
    {
        return false;
    }
    
    //load pic and pic region from scene or resource group
    scene_list->bg_bitmap_info = get_user_bg_pic(BG_VIDEO);
    if ( NULL == scene_list->bg_bitmap_info )
    {
        scene = scene_list->app_scene.scene;
        scene_list->pic_bg = ( picture_resource_t * )get_scene_child( scene, PIC_BG );
        if ( NULL == scene_list->pic_bg )
        {
            return false;
        }
        
        scene_list->bg_bitmap_info = sty_create_bitmap( (int)scene_list->pic_bg->pic_id );
        if ( NULL == scene_list->bg_bitmap_info )
        {
            return false;
        }
    }
    
    return true;
}


bool video_scene_list_rotate( video_scene_list_t *scene_list )
{
    fusion_effect_t fusion_effect = {0};
    /*list�����Ѿ�deinit�����¼�������л���ʶ������*/
    if (scene_list->app_scene.win_id < 0)
    {
        /*���������л�������û��load ��Դ�͸���dirlist�����Ϊfalse��
        ���ڴӲ��ų������غ�����������仯ʱˢ��dirlist*/
        s_rotate_process_enter = false;
        return false;
    }
    
    fusion_get_config(FUSION_TYPE_ROTATE, FUSION_FORWARD, &fusion_effect);
    fusion_set_effect(&fusion_effect);
    /* ���ô˽���֮ǰ������, ��Ϊ��Ч��һ֡ */
    fusion_set_first_frame( NULL );
    
    //unload������ʾ��Դ
    _close_theme( scene_list );
    _close_str_res();
    sty_delete_bitmap( scene_list->bg_bitmap_info );
    scene_list->bg_bitmap_info = NULL;
    //����load������ʾ��Դ
    video_scene_list_load_style( scene_list );

    gui_wm_set_size( scene_list->app_scene.win_id, \
        scene_list->app_scene.scene->width, scene_list->app_scene.scene->height );

    _scene_list_paint( scene_list );

    if ( video_vars.menuview != NULL )
    {
        video_list_widget_menu_load_style( video_vars.menuview );
        video_list_widget_menu_paint( video_vars.menuview );
    }

    if ( video_vars.dirlist != NULL )
    {
        dirlist_load_style( video_vars.dirlist );
        dirlist_paint( video_vars.dirlist );
    }

    if ( video_vars.headbar != NULL )
    {
        headbar_load_style( video_vars.headbar );
        headbar_paint( video_vars.headbar );
    }

    if( video_vars.adjust != NULL )
    {
        adjust_load_style( video_vars.adjust );
        adjust_paint( video_vars.adjust );
    }
    
    if( video_vars.loadingbox != NULL )
    {
        loadingbox_load_style( video_vars.loadingbox );
        loadingbox_paint( video_vars.loadingbox );
    }
    
    if( video_vars.softkeyboard != NULL )
    {
        softkeyboard_load_style( video_vars.softkeyboard );
        softkeyboard_paint( video_vars.softkeyboard );
    }
    
    if( video_vars.dialog != NULL )
    {
        dialog_load_style( video_vars.dialog );
        dialog_paint( video_vars.dialog );
    }
    dialog_modal_load_style();
    dialog_modal_paint();

    if( video_vars.vbar != NULL )
    {
        volumebar_load_style( video_vars.vbar );
        volumebar_paint( video_vars.vbar );
    }

    if( video_vars.msgbox != NULL )
    {
        msgbox_load_style( video_vars.msgbox );
        msgbox_paint( video_vars.msgbox );
    }

    /* ���ô˽��������, ��Ϊ��Ч���һ֡ */
    fusion_set_last_frame(NULL);
    /* ��ʾ��Ч */
    fusion_display( NULL );
    
    s_rotate_process_enter = true;
    return true;
}

//shaofeng ����??
static void _list_callback(menuview_t *menuview, int signal)
{
}



static bool _open_theme( video_scene_list_t *scene_list )
{
    scene_t *scene = NULL;
    rotation_e angle;
    char file[PATH_MAX];

    // ȡ��ͼ�Ƕ�
    angle = sty_get_view_rotation();
    // ������Ļ���� 
    sty_set_screen_rotation(angle);

    // ����commonui�� style ���� 
    if ( (angle == ROTATION_0) || (angle == ROTATION_180) )
    {
        video_vars.commonui_scene = load_scene( video_vars.commonui_style, COMMONUI_SIDELONG_SCENE );
    }
    else //if ( (angle == ROTATION_90) || (angle == ROTATION_270) )
    {
        video_vars.commonui_scene = load_scene( video_vars.commonui_style, COMMONUI_UPRIGHT_SCENE );
    }
    
    if ( NULL == video_vars.commonui_scene )
    {
        print_err("load commonui scene err");
        return false;
    }
    
    // ���� commonui �� style ����
    sty_set_common_scene( video_vars.commonui_scene );

    // ��ȡcommonui ��ͼƬ��Դ�ļ��� 
    sprintf(file, "%s/%s.res", sys_get_common_theme_res_dir(), "commonui");
    // �� commonui ͼƬ��Դ�ļ� 
    if( false == sty_open_common_pic_res(file) )
    {
        print_err("open %s failed!", file);
        return false;
    }
    
    
    // ����Ӧ�õ� style ����
    if ( (angle == ROTATION_0) || (angle == ROTATION_180) )
    {
        scene = load_scene( video_vars.app_style, VIDEO_LIST_SIDELONG_SCENE );
    }
    else //if ( (angle == ROTATION_90) || (angle == ROTATION_270) )
    {
        scene = load_scene( video_vars.app_style, VIDEO_LIST_UPRIGHT_SCENE );
    }
    
    if ( scene == NULL )
    {
        print_err("load_scene fail\n\n");
        return false;   
    }
    sty_set_app_scene( scene );
    scene_list->app_scene.scene = scene;
    
    // ��ȡͼƬ��Դ�ļ���
    sprintf(file, "%s/%s.res", sys_get_app_theme_res_dir(), "video");
    // ��Ӧ�õ�ͼƬ��Դ�ļ�
    if( false == sty_open_app_pic_res(file) )
    {
        print_err("open %s failed!", file);
        return false;
    }
    
    return true;
}

static bool _close_theme( video_scene_list_t *scene_list )
{

    //�˳�����
    free_scene( video_vars.commonui_scene );
    video_vars.commonui_scene = NULL;
    free_scene( scene_list->app_scene.scene );
    scene_list->app_scene.scene = NULL;

    if( sty_close_common_pic_res() == false )
    {
        print_err("close common picture resource error!");
        return false;
    }

    if( sty_close_app_pic_res() == false )
    {
        print_err("close app picture resource error!");
        return false;
    }
    
    return true;
}

static bool _open_str_res( void )
{
    char file[PATH_MAX];  

    // ��ȡcommonui �ַ�����Դ�ļ��� 
    sprintf(file, "%s/%s_%s.str", sys_get_common_string_res_dir(), "commonui", sys_get_cur_language_codes());
    // �� commonui �ַ�����Դ�ļ� 
    if( false == sty_open_common_str_res(file) )
    {
        print_err("open %s failed!", file);
        return false;
    }

    // ��ȡӦ���ַ�����Դ�ļ���
    sprintf(file, "%s/%s_%s.str", sys_get_app_string_res_dir(), "video", sys_get_cur_language_codes());
    // ��Ӧ�õ��ַ�����Դ�ļ� 
    if( false == sty_open_app_str_res(file) )
    {
        print_err("open %s failed!", file);
        return false;
    }

    // �������Ա���
    sty_set_text_encoding(GUI_UTF8_ENC);
    
    return true;
}

static bool _close_str_res( void )
{
    if( false == sty_close_common_str_res() )
    {
        print_err("close common string resource error!");
        return false;
    }

    if( false == sty_close_app_str_res() )
    {
        print_err("close app string resource error!");
        return false;
    } 

    return true;
}


/*
* scene initial function
* ������Դ. ��ʼ����Դ
*/
static bool _scene_list_init( video_scene_list_t *scene_list )
{
    bool result = true;
    scene_t *scene = NULL;
    hotplugin_dev_type_e main_disk; 

    print_dbg("enter scene list");
    memset( scene_list, 0x00, sizeof(video_scene_list_t) );
    //���볡��
    result = video_scene_list_load_style( scene_list );
    if (false == result)
    {
        print_err("load_style fail\n\n");
        return false;
    }
    //���̼��
    main_disk = sys_get_main_disk();
    if(HOTPLUGIN_DEV_TYPE_NONE == main_disk)
    {
        //��ʾ�޴���
        video_dialog_modal( VIDEO_DIALOG_NODISK);
        return false;
    }
    
    scene = scene_list->app_scene.scene;
    
    //��������
    print_dbg("playwin xywh:%d,%d,%d,%d", scene->x, scene->y, scene->width, scene->height);
    scene_list->app_scene.win_id = gui_wm_create_window(scene->x, scene->y, scene->width, scene->height, \
                                        WM_CREATE_SHOW, _scene_list_cb, 0);
    if( scene_list->app_scene.win_id <= 0 )
    {
        print_err("gui_wm_create_window failed\n\n");
        return false;
    }
    
    //��ȡgui dc���
    scene_list->app_scene.handle_gui_dc = gui_dc_get( (unsigned short)scene_list->app_scene.win_id );
    if (scene_list->app_scene.handle_gui_dc <= 0)
    {
        print_err("gui_dc_get fail\n\n");
        return false;
    }
    
    /*** ������Դ�ɹ�, ��ʼ��ʼ�� ***/
    //���ý���
    gui_wm_set_focus( scene_list->app_scene.win_id );
    
    video_headbar_create();
    headbar_paint( video_vars.headbar );
    
    video_msgbox_create();
    
    //shaofeng shaofengע��λ��, ����Ч���˻ص�list��Ӱ��
    _scene_list_paint( scene_list );
    //��һ�ν���dirlist, ����dirlist; ����dirlist�����ע�����menu
    if ( (VIDEO_START_MODE_DV_DIRLIST == video_vars.start_mode) && (NULL==video_vars.dirlist) )
    {
        //����dv dirlist
        video_list_dirlist_create( video_vars.menuview, FUSION_FORWARD );
    }
    else
    {
        //����mainmenu
        video_list_widget_menu_create( (menuview_cb_t)_list_callback );
    }
    
    /*������������ˢ�µ���Ļ, ֮���ٳ�ʼ��plist */
    gui_screen_update();
    
    /* Ϊ�Ż���ʾ�ٶ�, ��playlist��ʼ������scene_list�͸������� */
    result = video_playlist_init();

    if(false == s_rotate_process_enter)
    {
        /*���������л�������û�и���dirlist����ʱ����£���Ҫ���ڴ�playing��������*/
        if ( video_vars.dirlist != NULL )
        {
            dirlist_load_style( video_vars.dirlist );
            dirlist_paint( video_vars.dirlist );
        }
        s_rotate_process_enter = true;
    }
    
    return result;
}


/* scene exit function */
static bool _scene_list_deinit( video_scene_list_t *scene_list )
{
    bool result = true;

    print_dbg();
    
    dirlist_hide( video_vars.dirlist );
    //deinit commonui    
    video_headbar_delete();
    video_msgbox_delete();
    video_dialog_delete();
    video_vbar_delete();
    
    //shaofeng ��������
    video_vars.menu = menuview_get_menu( video_vars.menuview );
    video_list_widget_menu_delete( video_vars.menuview );
    video_vars.menuview = NULL;
    
    //ɾ������
    if ( scene_list->app_scene.win_id > 0 )
    {
        gui_wm_delete_window( scene_list->app_scene.win_id );
        scene_list->app_scene.win_id = -1;
    }
    
    result = _close_theme( scene_list );
    result = _close_str_res();
    
    sty_delete_bitmap( scene_list->bg_bitmap_info );
    scene_list->bg_bitmap_info = NULL;
    
    return result;
}


/* scene paint function */
static void _scene_list_paint( video_scene_list_t *scene_list )
{
    sty_draw_bitmap( scene_list->app_scene.handle_gui_dc, scene_list->bg_bitmap_info, 0, 0 );
}

/* scene msg process */
static bool _scene_list_proc_gui( video_scene_list_t *scene_list, gui_wm_message_t *pmsg )
{
    key_event_t *kmsg;
    bool result = true;
    
    switch (pmsg->msgid) 
    {
    case WM_MSG_KEY:
        kmsg = (key_event_t *) (pmsg->data.p);

        switch (kmsg->val | kmsg->type)
        {
        case KEY_VOL | KEY_TYPE_DOWN:
        case KEY_VSUB | KEY_TYPE_DOWN:
        case KEY_VADD | KEY_TYPE_DOWN:
            if( NULL == video_vars.vbar )
            {
                video_vbar_create();
                return false;
            }
            break;
        default:
            break;
        }
        break;
         
    default:
        gui_wm_default_callback(pmsg);
        break;
    }

    return result;
}


/******************************************************************************/
/*!                    
* \par  Description:
*       �����ص�����, Ҳ�����ڻص�����. һ������ֻ��һ��ȫ������
*       ���䴰��cb��gui��Ϣ: ��ȡtop���ͼ��, ����Ϣ���͸�top.
*       menuview��scene�����²�
* \param[in]    
* \retval       
*   ע��: ���ؼ��� ����playing scene�Ĺ�ϵ
*******************************************************************************/
static void _scene_list_cb(gui_wm_message_t *pmsg)
{ 
    int top;
    bool ret;
    
    /* �յ�down������ż���������, ����ֱ���˳� */
    if ( WM_MSG_KEY == pmsg->msgid )
    {
        if ( KEY_TYPE_DOWN == ((key_event_t *)(pmsg->data.p))->type )
        {
            video_vars.scene_list.is_key_valid = 1;
        }
    }
    if ( 1 != video_vars.scene_list.is_key_valid )
    {
        return;
    }
    
    ret = _scene_list_proc_gui( &video_vars.scene_list, pmsg );
    if( false == ret )
    {
        return;
    }

    top = canvas_get_top();
    print_dbg("top canvas:%d", top);
    
    if ( (NULL != video_vars.msgbox) && (top == video_vars.msgbox->canvas_id) )
    {
        msgbox_proc_gui( video_vars.msgbox, pmsg );
    }
    else if ( (NULL != video_vars.dialog) && (top == video_vars.dialog->canvas_id) )
    {
        dialog_proc_gui( video_vars.dialog, pmsg );
    }
    else if ( (NULL != video_vars.vbar) && (top == video_vars.vbar->canvas_id) )
    {
        volumebar_proc_gui( video_vars.vbar, pmsg );
    }
    else if ( (NULL != video_vars.softkeyboard) && (top == video_vars.softkeyboard->canvas_id) )
    {
        softkeyboard_proc_gui( video_vars.softkeyboard, pmsg );
    }
    else if ( (NULL != video_vars.dirlist) && (top == video_vars.dirlist->canvas_id) )
    {
        dirlist_proc_gui( video_vars.dirlist, pmsg );
    }
    else
    {
        menuview_proc_gui( video_vars.menuview, pmsg );
    }
}



/******************************************************************************/
/*!                    
* \par  Description:
*       ������ں���. scene entry function
* \param[in]    
* \retval       
*******************************************************************************/
scene_type_e video_scene_list( video_scene_list_t *scene_list )
{
    scene_type_e next_scene = SCENE_ERR;
    bool result;
    msg_apps_t  msg;

    print_dbg();
    sys_pm_set_clk_domain(&s_clk_domain);
    /* ������ʼ�� */
    result = _scene_list_init( scene_list );
    if(false == result)
    {
        print_err("scene init error!\n");
        goto VIDEO_SCENE_LIST_ERR_INIT;
    }
    //�������sceneǰ��������Ϣ
    clear_sys_msg_queue( -1 );
    clear_key_msg_queue( -1, -1 );
    
    register_sys_dispatcher( (sys_msg_proc)video_proc_sys_msg );

    while(true == get_msg(&msg))
    {
        dispatch_msg(&msg);
    }

    unregister_sys_dispatcher();

    next_scene = scene_list->app_scene.scene_type;
    /* �����˳� */
    VIDEO_SCENE_LIST_ERR_INIT:
    result = _scene_list_deinit( scene_list );
    if(false == result)
    {
        print_err("scene deinit error!\n");
        next_scene = SCENE_EXIT;
    }
    clear_sys_msg_queue( -1 );
    clear_key_msg_queue( -1, -1 );
    
    sys_pm_clear_clk_domain(&s_clk_domain);
    
    return next_scene;
}


