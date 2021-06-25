/*******************************************************************************
 *                              NOYA1100
 *                            Module: app
 *                 Copyright(c) 2003-2008 Actions Semiconductor,
 *                            All Rights Reserved. 
 * History:         
 *      <author>    <time>           <version >             <desc>
 *      shaofeng    2009-05-25          1.0             build this file 
*******************************************************************************/
/*!
 * \file     plist_generate_scene.c
 * \brief    
 * \author   shaofeng
 * \par      GENERAL DESCRIPTION:
 *               
 * \par      EXTERNALIZED FUNCTIONS:
 *               
 * \version 1.0
 * \date  2009/05/25
*******************************************************************************/

#include "include_case.h"


/*��Ӧ�õ�ͷ�ļ�*/
#include "pl_gener_sty.h"
#include "plist_generate.h"
#include "plist_generate_scene.h"
#include "plist_generate_ui.h"




/* extern function, �������� */
static bool _create_bitmap( plist_generate_scene_t *plist_generate_scene );
static bool _delete_bitmap( plist_generate_scene_t *plist_generate_scene );

static bool _open_theme( plist_generate_scene_t *plist_generate_scene );
static bool _close_theme( plist_generate_scene_t *plist_generate_scene );
static bool _open_str_res( void );
static bool _close_str_res( void );

static int _get_process_percent( int process_percent );
static bool _plist_generate_music( void *hmedia );
static bool _plist_delete_nplist( void );
static void *_plist_generate_process( void *nouse );

bool _thread_plist_generate_process_create( void *(*start_rtn)(void*) );
static int _proc_timer_paint( void *param );

static bool _init( plist_generate_scene_t *scene );
static bool _deinit( plist_generate_scene_t *scene );
static void _paint( plist_generate_scene_t *scene );
static void _callback(gui_wm_message_t *pmsg);


#if 0
#endif
bool plist_generate_load_style( plist_generate_scene_t *plist_generate_scene )
{
    scene_t *scene;
    
    /* load theme ��Դ�ļ� */
    print_dbg();
    if(_open_theme( plist_generate_scene ) == false)
    {
        return false;
    }

    /* load �ַ�����Դ�ļ� */
    if(_open_str_res() == false)
    {
        return false;
    }
    
    scene = plist_generate_scene->app_scene.scene;
    //load pic region from resource group
    plist_generate_scene->pic_bg        = ( picture_resource_t * )get_scene_child( scene, PIC_BG );
    plist_generate_scene->pic_pbar_bg  = ( picture_resource_t * )get_scene_child( scene, PIC_PBAR_BG );
    plist_generate_scene->pic_pbar_hl  = ( picture_resource_t * )get_scene_child( scene, PIC_PBAR_HL );
    plist_generate_scene->resgroup_pbar_highlight_rect      = ( resgroup_resource_t * ) \
        get_scene_child( scene, RESGROUP_PBAR_HIGHLIGHT_RECT );
    if ( NULL != plist_generate_scene->resgroup_pbar_highlight_rect )
    {
        plist_generate_scene->pbar_length = plist_generate_scene->resgroup_pbar_highlight_rect->width;
        plist_generate_scene->pbar_xstart = plist_generate_scene->resgroup_pbar_highlight_rect->absolute_x;
    }
    else
    {
        plist_generate_scene->pbar_length = 0;
        plist_generate_scene->pbar_xstart = 0;
    }
    
    plist_generate_scene->reg_wait  = ( picregion_resource_t * )get_scene_child( scene, REG_WAIT );
    plist_generate_scene->str_wait  = ( string_resource_t * )get_scene_child( scene, STR_WAIT );
    plist_generate_scene->str_nospace = ( string_resource_t * )get_scene_child( scene, STR_NOSPACE );
    
    return true;
}

static bool _create_bitmap( plist_generate_scene_t *plist_generate_scene )
{
    unsigned short i = 0;
    picregion_frame_t picregion_frame;
    
    plist_generate_scene->bg_bitmap_info = sty_create_bitmap( plist_generate_scene->pic_bg->pic_id );
    
    if ( NULL == plist_generate_scene->reg_wait )
    {
        return false;
    }
    plist_generate_scene->wait_bitmap_info = (gui_bitmap_info_t **)malloc( sizeof(gui_bitmap_info_t) \
        * plist_generate_scene->reg_wait->frames );
    if ( NULL == plist_generate_scene->wait_bitmap_info )
    {
        return false;
    }
    for ( i=0; i<plist_generate_scene->reg_wait->frames; i++ )
    {
        get_picregion_frame(plist_generate_scene->reg_wait, i, &picregion_frame );
        plist_generate_scene->wait_bitmap_info[i] = sty_create_bitmap( picregion_frame.pic_id );
    }
    
    return true;
}

static bool _delete_bitmap( plist_generate_scene_t *plist_generate_scene )
{
    int i = 0;
    
    sty_delete_bitmap( plist_generate_scene->bg_bitmap_info );
    plist_generate_scene->bg_bitmap_info = NULL;

    for ( i=0; i<plist_generate_scene->reg_wait->frames; i++ )
    {
        sty_delete_bitmap( plist_generate_scene->wait_bitmap_info[i] );
    }
    
    free( plist_generate_scene->wait_bitmap_info );
    plist_generate_scene->wait_bitmap_info = NULL;
    
    return true;
}

static bool _open_theme( plist_generate_scene_t *plist_generate_scene )
{
    scene_t *scene = NULL;
    rotation_e angle;
    char file[512];

    // ȡ��ͼ�Ƕ�
    angle = sty_get_view_rotation();
    // ������Ļ���� 
    sty_set_screen_rotation(angle);
    
    // ����Ӧ�õ� style ����
    if ( (angle == ROTATION_0) || (angle == ROTATION_180) )
    {
        scene = load_scene( plist_generate_vars.app_style, PLIST_GENERATE_SIDELONG_SCENE );
    }
    else //if ( (angle == ROTATION_90) || (angle == ROTATION_270) )
    {
        scene = load_scene( plist_generate_vars.app_style, PLIST_GENERATE_UPRIGHT_SCENE );
    }
    
    if ( scene == NULL )
    {
        print_err("load_scene fail\n\n");
        return false;   
    }
    sty_set_app_scene( scene );
    plist_generate_scene->app_scene.scene = scene;
    
    // ����commonui�� style ����
    if ( (angle == ROTATION_0) || (angle == ROTATION_180) )
    {
        scene = load_scene( plist_generate_vars.com_style, COMMONUI_SIDELONG_SCENE );
    }
    else //if ( (angle == ROTATION_90) || (angle == ROTATION_270) )
    {
        scene = load_scene( plist_generate_vars.com_style, COMMONUI_UPRIGHT_SCENE );
    }
    
    if ( scene == NULL )
    {
        print_err("load_scene fail\n\n");
        return false;   
    }
    sty_set_common_scene( scene );
    plist_generate_scene->app_scene.com_scene = scene;
    
    // ��ȡͼƬ��Դ�ļ���
    sprintf(file, "%s/%s.res", sys_get_app_theme_res_dir(), "pl_gener");
    // ��Ӧ�õ�ͼƬ��Դ�ļ�
    if(sty_open_app_pic_res(file) == false)
    {
        print_err("open %s failed!", file);
        return false;
    }    

    // ��ȡcommonui ��ͼƬ��Դ�ļ��� 
    sprintf(file, "%s/%s.res", sys_get_common_theme_res_dir(), "commonui");

    // �� commonui ͼƬ��Դ�ļ� 
    if( sty_open_common_pic_res(file) == false )
    {
        print_err("open %s failed!", file);
        return false;
    }
    
    return true;
}

static bool _close_theme( plist_generate_scene_t *plist_generate_scene )
{
    //�˳�����
    free_scene( plist_generate_scene->app_scene.com_scene );
    plist_generate_scene->app_scene.com_scene = NULL;

    free_scene( plist_generate_scene->app_scene.scene );
    plist_generate_scene->app_scene.scene = NULL;

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

static bool _open_str_res(void)
{
    char file[512];      

    // ��ȡcommonui �ַ�����Դ�ļ��� 
    sprintf(file, "%s/%s_%s.str", sys_get_common_string_res_dir(), "commonui", sys_get_cur_language_codes());

    // �� commonui �ַ�����Դ�ļ� 
    if( sty_open_common_str_res(file) == false )
    {
        print_err("open %s failed!", file);
        return false;
    }

    print_dbg();
    // ��ȡӦ���ַ�����Դ�ļ���
    sprintf(file, "%s/%s_%s.str", sys_get_app_string_res_dir(), "pl_gener", sys_get_cur_language_codes());
    print_dbg();
    // ��Ӧ�õ��ַ�����Դ�ļ� 
    if(sty_open_app_str_res(file) == false)
    {
        print_err("open %s failed!", file);
        return false;
    }

    // �������Ա���
    sty_set_text_encoding(GUI_UTF8_ENC);

    // ��������ģʽ
    gui_set_language_mode( sys_get_cur_language_id() );
    
    return true;
}

static bool _close_str_res(void)
{
    if( sty_close_common_str_res() == false )
    {
        print_err("close common string resource error!");
        return false;
    }
    
    if(sty_close_app_str_res() == false)
    {
        print_err("close app string resource error!");
        return false;
    } 

    return true;
}

static int _get_process_percent( int process_percent )
{
    print_dbg("prcess:%d\n" , process_percent);
    plist_generate_vars.scene.cur_percent = process_percent;
    
    return 1;
}

static bool _plist_generate_music( void *hmedia )
{
    void* hsubmusiclib;
    plist_trivefield_t musiclib_field;
    plist_sortfield_t tmplistsort;
    plist_filterfield_t filterfield;
    char path[PATH_MAX];
    int result;
    char ext_music[PATH_MAX];
    const char *dir = NULL;
    
    /* read ext config */
    result = get_config( CFG_APP_SUPPORT_EXT_MUSIC, (char*)ext_music, sizeof(ext_music) );
    if( result < 0 )
    {
        print_err("get music ext failed!");
        return false;
    }

    /*����������ý���Ľṹ*/
    musiclib_field.num = 10;
    musiclib_field.nfield[0] = FULLPATH;
    musiclib_field.nfield[1] = MARK;
    musiclib_field.nfield[2] = TITLE;
    musiclib_field.nfield[3] = ARTIST;
    musiclib_field.nfield[4] = ALBUM;
    musiclib_field.nfield[5] = GENRE;
    musiclib_field.nfield[6] = IMAGEINFO;
    musiclib_field.nfield[7] = RATING;
    musiclib_field.nfield[8] = AGE;
    musiclib_field.nfield[9] = TRACK;

    plist_set_filetype_ext(MUSIC, ext_music);

    /*������ȡ������ý���*/
    plist_delete_by_tblname(TBLNAME_MUSIC_SUBLIB); //del music sub
    hsubmusiclib=plist_open(TBLNAME_MUSIC_SUBLIB,MUSICRESERVE1,&musiclib_field);   
    
    /*����״̬���������ֶ�*/
    tmplistsort.num=5;
    tmplistsort.nfield[0]=TITLE;
    tmplistsort.nstyle[0]=PINYINSORT;
    tmplistsort.nsortdirection[0] = 0;
    tmplistsort.nfield[1]=ARTIST;
    tmplistsort.nstyle[1]=PINYINSORT;
    tmplistsort.nsortdirection[1] = 0;
    tmplistsort.nfield[2]=ALBUM;
    tmplistsort.nstyle[2]=PINYINSORT;
    tmplistsort.nsortdirection[2] = 0;
    tmplistsort.nfield[3]=GENRE;
    tmplistsort.nstyle[3]=PINYINSORT;
    tmplistsort.nsortdirection[3] = 0;
    tmplistsort.nfield[4]=TRACK;
    tmplistsort.nstyle[4]=PINYINSORT;
    tmplistsort.nsortdirection[4] = 0;
     
    /*����״̬���ù����ֶ�*/
    filterfield.field_num = 1;
    filterfield.is_pattern_search = 1;
    filterfield.filter_field[0].fieldno = FULLPATH;

    dir = sys_get_main_disk_dir(DIR_MUSIC);

    if ( dir == NULL )
    {
        return false;
    }

    strcpy( path, dir ); //MUSICĿ¼
    strcat( path, "*" ); //MUSICĿ¼�µ��ļ�
    filterfield.filter_field[0].content.pval = path;     
     
    result = plist_add_query(hmedia,hsubmusiclib,&filterfield,&tmplistsort,NULL);
    if ( result < 0 )
    {
        print_err("generate music sublib fail!");
        plist_close(hsubmusiclib);
        return false;
    }

    plist_close(hsubmusiclib);

    //ɾ��submusiclib��RECORDERĿ¼�µļ�¼
    dir = sys_get_main_disk_dir(DIR_RECORDER);
    if ( dir == NULL )
    {
        return false;
    }
    
    result = plist_erase_invalid_data(TBLNAME_MUSIC_SUBLIB,dir);
    if ( result < 0 )
    {
        print_err("generate music sublib fail!");
    }
    
    return ( result == 0 );
}

static bool _plist_delete_nplist( void )
{
    int result = -1;
    bool nowplaying = false;
    
    /*ɾ�����ڲ����б�*/
    plist_delete_by_tblname(TBLNAME_MUSIC_NOWPLAYING); //del music nplist
    plist_delete_by_tblname(TBLNAME_VIDEO_NOWPLAYING); //del video nplist
    plist_delete_by_tblname(TBLNAME_PHOTO_DEFAULT); //del photo nplist
    plist_delete_by_tblname(TBLNAME_PHOTO_SPECIAL); //del photo nplist
    
    /*���nowplaying�������*/
    nowplaying = false;
    
    result = set_config( CFG_MUSIC_NOWPLAYING, (char*)&nowplaying, sizeof(nowplaying) );
    if( result < 0 )
    {
        print_err("");
    }
    
    result = set_config( CFG_VIDEO_NOWPLAYING, (char*)&nowplaying, sizeof(nowplaying) );
    if( result < 0 )
    {
        print_err("");
    }
    
    result = set_config( CFG_PHOTO_NOWPLAYING, (char*)&nowplaying, sizeof(nowplaying) );
    if( result < 0 )
    {
        print_err("");
    }

    if ( result < 0 )
    {
        return false;
    }
    else
    {
        return false;
    }
}

//shaofeng ��Ҫ��ӿ�
static void *_plist_generate_process( void *nouse )
{
    int result = -1;
    void *hmedia = NULL;
    char ext_all[PATH_MAX];
    const char *dir = NULL;
    
    print_dbg();
    
    //init fail flag
    plist_generate_vars.scene.gen_fail = true;
    
    /* read ext config */
    result = get_config( CFG_APP_SUPPORT_EXT_ALL, (char*)ext_all, sizeof(ext_all) );
    if( result < 0 )
    {
        print_err("get all ext failed!");
        return NULL;
    }
    
    /*�������ݿ�·��*/
    dir = sys_get_main_disk_dir(DIR_SYSTEM_PLAYLIST);
    if(dir == NULL)
    {
        print_err("has no main disk");
        exit_msg_loop();
        return NULL;
    }
    else
    {
        plist_set_rootdir(dir);
    }

    result = plist_init();
    if(result < 0)
    {
        print_err("plist init failed!");
        exit_msg_loop();
        return NULL;
    }
    
    hmedia = plist_open( NULL, MEDIALIB, NULL );
    if(NULL != hmedia)
    {
        result = plist_set_filetype_ext( MUSIC , ext_all );
        print_dbg("result:%d", result);
        dir = sys_get_main_disk_dir(DIR_ROOT);
        result = plist_generate_medialib( dir, SYSTEMDISK, MUSIC, _get_process_percent );
        print_dbg("result:%d", result);
        if( result == 0 )
        {        
            //generate music sublib
            if( _plist_generate_music( hmedia ) == true )
            {
                plist_generate_vars.scene.gen_fail = false;
            }
        }
    }
    plist_close( hmedia );
    
    /*ɾ�����ڲ����б�*/
    _plist_delete_nplist();
    
    result = plist_quit();
    
    //scene quit
    exit_msg_loop();
    
    return NULL;
}

static bool _ui_init( plist_generate_scene_t *plist_generate_scene )
{
    bool result = true;
    
    plist_generate_scene->show.bg = 1;
    plist_generate_scene->show.wait = 1;
    plist_generate_scene->show.wait_frame = 0;
    
    plist_generate_scene->show.pbar_bg = 1;
    plist_generate_scene->show.pbar = 1;
    
    plist_generate_scene->show.str_wait = 1;
    
    /* ��ʼ��saved_show, ��֤��һ��show��saved_show��ͬ, ��һ��ִ��draw���� */
    memset(&plist_generate_scene->saved_show, -1, sizeof(plist_generate_scene->saved_show));
    
    return result;
}


/******************************************************************************/
/*!                    
* \par  Description:
*      ����join���߳�, ��������ͼ����
* \retval       bool
*   ע��: 
*       �������̱߳������̵�һ�����߳�, ��֤������������
*       ���ô˺���ǰ��video_thread_decode_preview_delete, ��֤�߳��˳��ٴ���
*******************************************************************************/
bool _thread_plist_generate_process_create( void *(*start_rtn)(void*) )
{
    int result = -1;
    pthread_t pthread;
    pthread_attr_t pthread_attr;
    struct sched_param param;
    struct sched_param self_param;
    int policy;
    
    //�����������̵�һ�����߳�, ��֤������������
    pthread_attr_init(&pthread_attr);
    pthread_attr_setinheritsched(&pthread_attr, PTHREAD_EXPLICIT_SCHED);
    pthread_attr_setdetachstate(&pthread_attr, PTHREAD_CREATE_DETACHED);
    pthread_attr_setscope(&pthread_attr, PTHREAD_SCOPE_PROCESS);
    
    //�����������̵�һ�����߳�, ��֤������������
    pthread_getschedparam( pthread_self(), &policy, &self_param );
    param.sched_priority = self_param.sched_priority - 2;
    pthread_attr_setschedparam(&pthread_attr, &param);

    result = pthread_create( &pthread, &pthread_attr, start_rtn, NULL );
    pthread_attr_destroy( &pthread_attr );
    if( result != 0 )
    {
        print_err("can��t create thread, result:%d\n", result);
        return false;
    }
    return true;
}

static int _proc_timer_paint( void *param )
{
    _paint( &plist_generate_vars.scene );
    
    return 0;
}
    

/*
* scene initial function
* ������Դ. ��ʼ����Դ
*/
static bool _init( plist_generate_scene_t *plist_generate_scene )
{
    bool result = true;
    scene_t *scene = NULL;

    memset( plist_generate_scene, 0x00, sizeof(plist_generate_scene_t) );
    
    /*** ������Դ, �����κ�ʧ��ֱ�ӷ����˳� ***/
    //���볡��
    result = plist_generate_load_style( plist_generate_scene );
    if (false == result)
    {
        print_err("load_style fail\n\n");
        return false;
    }
    scene = plist_generate_scene->app_scene.scene;
    
    result = _create_bitmap( plist_generate_scene );
    if (false == result)
    {
        print_err("_create_bitmap fail\n\n");
        return false;
    }
    
    //��������
    print_dbg("playwin xywh:%d,%d,%d,%d", scene->x, scene->y, scene->width, scene->height);
    plist_generate_scene->app_scene.win_id = gui_wm_create_window(scene->x, scene->y, scene->width, scene->height, \
                                        WM_CREATE_SHOW, _callback, 0);
    if( (int)NULL == plist_generate_scene->app_scene.win_id )
    {
        print_err("gui_wm_create_window failed\n\n");
        return false;
    }
    
    //��ȡgui dc���
    plist_generate_scene->app_scene.handle_gui_dc = gui_dc_get( plist_generate_scene->app_scene.win_id );
    if (plist_generate_scene->app_scene.handle_gui_dc <= 0)
    {
        print_err("gui_dc_get fail\n\n");
        return false;
    }
    
    /*** ������Դ�ɹ�, ��ʼ��ʼ�� ***/
    //���ý���
    gui_wm_set_focus( plist_generate_scene->app_scene.win_id );
    
    //draw the logo pic
    _ui_init( plist_generate_scene );
    _paint( plist_generate_scene );
    
    _thread_plist_generate_process_create( _plist_generate_process );
    
    //ȫ����ʼ���ɹ�, �������timer, ��ʱˢ�¶�������
    plist_generate_scene->timer_id = set_timer(PLIST_GENERATE_TIMER_INTERVAL, \
        _proc_timer_paint, (void *)plist_generate_scene);
    if (plist_generate_scene->timer_id < 0)
    {
        print_err("set_timer failed");
        return false;
    }
    
    return result;
}


/* scene exit function */
static bool _deinit( plist_generate_scene_t *plist_generate_scene )
{
    int value = 0;
    dialog_init_t dialog_init;

    print_dbg();
    
    //ģ̬��ʾ��
    if (plist_generate_scene->gen_fail == true)
    {
        dialog_init.head = DIALOG_HEAD_WARN;
        dialog_init.str_id = plist_generate_scene->str_nospace->str_id;
        dialog_modal( &dialog_init );
    }
        
    //kill timer
    if ( -1 != plist_generate_scene->timer_id )
    {
        kill_timer( plist_generate_scene->timer_id );
        plist_generate_scene->timer_id = -1;
    }
    
    //�Ƴ�ǰ��ˢ��һ��, ��֤��������
    _paint( plist_generate_scene );
    _delete_bitmap( plist_generate_scene );
    
    //���ɲ����б��, �����־
    set_config( CFG_NEED_GEN_PLIST, (char*)&value , 4 );
    
    //ɾ������
    if ( plist_generate_scene->app_scene.win_id > 0 )
    {
        gui_wm_delete_window( plist_generate_scene->app_scene.win_id );
        plist_generate_scene->app_scene.win_id = -1;
    }
    
    if( _close_theme( plist_generate_scene ) == false )
    {
        return false;
    }
    if( _close_str_res() == false )
    {
        return false;
    }
    
    return true;
}

/* scene paint function */
static void _paint( plist_generate_scene_t *plist_generate_scene )
{
    bool result = true;
    int x_coordinate = 0;
    
    if ( NULL == plist_generate_scene )
    {
        return;
    }
    
    plist_generate_draw_bg( plist_generate_scene, plist_generate_scene->show.bg );
    plist_generate_draw_wait_animate( plist_generate_scene, \
            plist_generate_scene->show.wait, plist_generate_scene->show.wait_frame );
    plist_generate_scene->show.wait_frame++;
    if( plist_generate_scene->show.wait_frame >= plist_generate_scene->reg_wait->frames )
    {
        plist_generate_scene->show.wait_frame = 0;
    }
    
    plist_generate_draw_pbar_bg( plist_generate_scene, plist_generate_scene->show.pbar_bg );
    result = plist_generate_progress_to_pbar_coordinate( plist_generate_scene->cur_percent, \
            PLIST_GENERATE_TOTAL_PERCENT, &x_coordinate );
    if ( true == result )
    {
        plist_generate_draw_pbar( plist_generate_scene, plist_generate_scene->show.pbar, x_coordinate );
    }
    
    plist_generate_draw_str( plist_generate_scene, plist_generate_scene->show.str_wait );
    
    gui_screen_update();
}


/*
* scene callback function
* ����cb��gui��Ϣ. ��AP������gui��Ϣ
*/
static void _callback(gui_wm_message_t *pmsg)
{ 
    gui_wm_default_callback(pmsg);
}



/* scene entry function */
void plist_gene_scene( plist_generate_scene_t *scene )
{
    bool result;
    msg_apps_t  msg;

    print_dbg();
    //��ֹ����, ʡ��
    sys_forbid_screensaver( true );
    sys_forbid_auto_standby( true);
    sys_forbid_timed_shutdown(true);
    
    /* ������ʼ�� */
    result = _init( scene );
    if(false == result)
    {
        print_err("scene init error!\n");
        goto PLIST_GENERATE_SCENE_ERR_INIT;
    }
    
    register_sys_dispatcher( (sys_msg_proc)plist_generate_proc_sys_msg );

    while( true == get_msg(&msg) )
    {
        dispatch_msg(&msg);
    }

    unregister_sys_dispatcher();

    /* �����˳� */
    PLIST_GENERATE_SCENE_ERR_INIT:
    //�ָ�����, ʡ��
    sys_forbid_screensaver( false );
    sys_forbid_auto_standby( false );
    sys_forbid_timed_shutdown(false);
    
    result = _deinit( scene );
    
    return;
}

