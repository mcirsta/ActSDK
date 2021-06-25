/*******************************************************************************
 *                              NOYA1100
 *                            Module: video
 *                 Copyright(c) 2003-2008 Actions Semiconductor,
 *                            All Rights Reserved. 
 * History:         
 *      <author>    <time>           <version >             <desc>
 *      shaofeng    2009-06-16          1.0             build this file 
******************************************************************************/
/*!
 * \file     video_list_dirlist.c
 * \brief    
 * \author   shaofeng
 * \par      GENERAL DESCRIPTION:
 *               
 * \par      EXTERNALIZED FUNCTIONS:
 *               
 * \version 1.0
 * \date  2009/04/23
*******************************************************************************/

#include "include_psp.h"
#include "include_case.h"

/* include file of this application, ��Ӧ�õ�ͷ�ļ� */
#include "include_video.h"



/* extern function, �������� */
static void _proc_timer_searching_done( void );
static void *_pthread_deal_searching( void *nouse );

static void _softkeyboard_cb( softkeyboard_t *softkeyboard, softkeyboard_sig_e signal );

static int _dirlist_proc_singel_timer_paint_bitmap( void *param );
static int _dirlist_proc_singel_timer_decode_preview( void *param );

static void *_dirlist_preview_decode_thread_start( void * nouse );
static void _dirlist_cb(dirlist_t *dirlist, dirlist_sig_e sig);
static bool _backto_dirlist( menuview_t *menuview, fusion_enter_e fenter );




/* define variable, �������� */
/*! dirlist��ʾʱʹ�õĽṹ�� */
static gui_bitmap_info_t s_preview_bitmap_info;

/*! video mmm����ʱʹ�õĽṹ�� */
static mmm_cur_video_t s_preview_info;

/*! ����ʱ, ��Ҫ�˳�ʱͨ���˱����˳�, ��:����return����Ҫ�˳� */
static bool s_dirlist_decode_stop_flag = false;

/*! �������, ����Ҫˢ�µ�bitmap index */
static int s_bitmap_index_to_paint = -1;

static int s_paint_timer_id = -1;

static int s_search_num = -1;

/* �յ�dirlist�Ľ�����Ϣ��, �ӳٽ���Ķ�ʱ�� */
static int s_decode_preview_timer_id = -1;


#if 0
#endif
/******************************************************************************/
/*!                    
* \par  Description:
*      timer������
* \retval       void
*   ע��: 
*       
*******************************************************************************/
static void _proc_timer_searching_done( void )
{ 
    //���������, ɾ��loadingbox��ʾ
    video_loadingbox_delete();
    
    if ( s_search_num <= 0 )
    {
        dirlist_set_search_string( video_vars.dirlist, "" );

        //show cann't found dialog
        video_dialog_modal( VIDEO_DIALOG_NOT_FOUND );
    }
    else
    {     
        //delete softkeyboard
        video_softkeyboard_delete();

        //show dirlist
        dirlist_show(video_vars.dirlist);
        //update dirlist
        dirlist_paint( video_vars.dirlist );       
        
        //��ֹͣ�߳�, ������. 
        video_thread_decode_preview_stop();
        s_dirlist_decode_stop_flag = true;
        video_thread_decode_preview_delete();
        //���������߳�
        video_thread_decode_preview_create( _dirlist_preview_decode_thread_start );
    }
}

/******************************************************************************/
/*!                    
* \par  Description:
*       �����߳����庯��
*       ���ڻ�ȡsoftkeyboard��������, ���ø�dirlist����
* \retval       
*   ע��: 
*       ���߳���Ҫ��ʱ��dirlist_set_search_string����
*       ���߳����������ʱ��, �����������������ʾ������. ��Ϊ�̲߳�������ʾ����
*******************************************************************************/
static void *_pthread_deal_searching( void *nouse )
{
    char text[PATH_MAX];
     
    //get text
    softkeyboard_get_text( video_vars.softkeyboard, text, sizeof(text) );
    
    //set filter string
    s_search_num = dirlist_set_search_string( video_vars.dirlist, text );
    
    //process after searching
    set_single_shot_timer( 1, (timer_proc)_proc_timer_searching_done, NULL );
    
    return (void *)NULL;
}

/******************************************************************************/
/*!                    
* \par  Description:
*      ����detached���߳�, ��������
* \retval       bool
*   ע��: 
*       �������̱߳������̵�һ�����߳�, ��֤������������
*******************************************************************************/
bool video_search_thread_create( void *(*start_rtn)(void*) )
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
    print_dbg(" manager_msg_loop  thread's priority is: %d\n", param.sched_priority );

    result = pthread_create( &pthread, &pthread_attr, start_rtn, NULL );
    pthread_attr_destroy( &pthread_attr );
    if( result != 0 )
    {
        print_err("can��t create thread, result:%d\n", result);
        return false;
    }
    return true;
}



static void _softkeyboard_cb( softkeyboard_t *softkeyboard, softkeyboard_sig_e signal )
{
    switch ( signal ) 
    {
    case WIDGET_SIG_SKB_OK:
        video_loadingbox_create();
        
        video_search_thread_create( _pthread_deal_searching );
        break;
        
    case WIDGET_SIG_SKB_CANCEL:
        video_softkeyboard_delete();
        
        dirlist_show( video_vars.dirlist );
        dirlist_paint( video_vars.dirlist );
        break;
    default:
        break;
    }
}
/******************************************************************************/
/*!                    
* \par  Description:
*       �����߳�������ˢ�¶�ʱ��������
* \param[in]    
* \retval       
*   ע��: 
*       ����dirlist�����̲߳���ˢ��, �����߳����������ζ�ʱ�����ˢ��
*******************************************************************************/
static int _dirlist_proc_singel_timer_paint_bitmap( void *param )
{
#if 0
    //����dirlist��bitmap����, һ�θ���һ��
    dirlist_paint_bitmap( video_vars.dirlist, s_bitmap_index_to_paint );
#else
    //����dirlist��bitmap����, һ�θ������е�ȫ��
    int i = 0;
    int preview_num;
    bool is_dir;
    gui_bitmap_info_t *p_tmp_bitmap_info = NULL;
    
    preview_num = dirlist_get_total( video_vars.dirlist );
    for( i=0; i<preview_num; i++ )
    {
        is_dir = dirlist_is_item_dir( video_vars.dirlist, i );
        p_tmp_bitmap_info = dirlist_get_item_bitmap( video_vars.dirlist, i);
        if ( (false == is_dir) && (NULL != p_tmp_bitmap_info) )
        {
            dirlist_paint_bitmap( video_vars.dirlist, i );
        }
    }
#endif    
    return 1;
}


/******************************************************************************/
/*!                    
* \par  Description:
*       dirlist�Ľ������������Ľ����̶߳�ʱ��������, �����������������߳�
* \param[in]    
* \retval       
*******************************************************************************/
static int _dirlist_proc_singel_timer_decode_preview( void *param )
{
    //��ֹͣ�߳�, ������. ����:������ٷ�ҳ֮ǰû�н���, �ַ����˴�����
    video_thread_decode_preview_stop();
    s_dirlist_decode_stop_flag = true;
    video_thread_decode_preview_delete();
    
    //���������߳�
    video_thread_decode_preview_create( _dirlist_preview_decode_thread_start );
    
    return 1;
}

/******************************************************************************/
/*!                    
* \par  Description:
*       dirlist��preview�����߳�
* \param[in]    
* \retval       
*   ע��: 
*       �����̲߳���ˢ��, �����ܵ���menuview_paint_bitmap
*       �յ�dirlist��WIDGET_SIG_DIRLIST_DECODE�źź��������߳̽���
*       ����ֹͣ��־, ���ڽ�����δ����˳��̵߳����
*******************************************************************************/
static void *_dirlist_preview_decode_thread_start( void * nouse )
{
    bool result = true;
    bool is_dir;
    int i;
    unsigned int time_recent_list;
    int preview_num = -1;
    char *p_filename = NULL;
    char fullpath[PATH_MAX] = {0};
    gui_bitmap_info_t *p_tmp_bitmap_info = NULL;
    
    s_dirlist_decode_stop_flag = false;
    
    //��ʼ��preview_info, ����thread����
    s_preview_info.xpos     = 0;
    s_preview_info.ypos     = 0;
    s_preview_info.width    = (unsigned int)dirlist_get_bitmap_width( video_vars.dirlist );
    s_preview_info.height   = (unsigned int)dirlist_get_bitmap_height( video_vars.dirlist );
    s_preview_info.buf      = malloc( 2 * s_preview_info.width * s_preview_info.height );
    if ( NULL == s_preview_info.buf )
    {
        return false;
    }
    memset( s_preview_info.buf, 0x00, 2 * s_preview_info.width * s_preview_info.height );
    
    //��ʼ��preview_bitmap_info, ��thread��������dirlist��ʾ
    s_preview_bitmap_info.xsize = (unsigned short)s_preview_info.width;
    s_preview_bitmap_info.ysize = (unsigned short)s_preview_info.height;
    s_preview_bitmap_info.bytesperpixel= 2;
    s_preview_bitmap_info.pdata = s_preview_info.buf;
    
    //��ý��뿪ʼindex, total num
    preview_num = dirlist_get_total( video_vars.dirlist );
    //preview_num��Ļ�ϵľֲ�index, ���Դ�0��ʼ, ���dirlist�ӿ�
    for( i=0; i<preview_num; i++ )
    {
        is_dir = dirlist_is_item_dir( video_vars.dirlist, i );

        //�ж��Ƿ���Ŀ¼, �Ƿ��Ѿ�����bitmap, ��������ȥ����
        p_tmp_bitmap_info = dirlist_get_item_bitmap( video_vars.dirlist, i);
        if ( (false == is_dir) && (NULL == p_tmp_bitmap_info) )
        {
            memset( fullpath, 0x00, sizeof(fullpath) );
            p_filename = dirlist_get_item_name( video_vars.dirlist, i);
            sprintf(fullpath, "%s%s", video_vars.dirlist->path_name, p_filename);
            
            //�Ƿ���recent file, �Ի�ȡtagʱ��
            result = video_recent_list_get_tag_by_fullpath( fullpath, &time_recent_list );
            if ( true == result )
            {
                result = video_thread_get_one_preview( fullpath, time_recent_list, &s_preview_info );
            }
            else
            {
                result = video_thread_get_one_preview( fullpath, 0, &s_preview_info );
            }
            if ( true == result )
            {
                dirlist_set_item_bitmap( video_vars.dirlist, i, &s_preview_bitmap_info );
                
                //��������menu��bitmap����ĵ��ζ�ʱ��
                s_bitmap_index_to_paint = i;
                //û��timer, ����; �Ѵ�����, ��restart, Ч�����
                if ( s_paint_timer_id <= 0 )
                {
                    s_paint_timer_id = set_single_shot_timer( 1, \
                        (timer_proc)_dirlist_proc_singel_timer_paint_bitmap, NULL );
                }
                else
                {
                    restart_timer( s_paint_timer_id );
                }
            }
        }
        
        //�յ��˳��̱߳�־, �˳�. ��:����ʱ, ����return����Ҫ�˳������
        if ( true == s_dirlist_decode_stop_flag )
        {
            s_dirlist_decode_stop_flag = false;
            break;
        }
    }
    
    if ( NULL != s_preview_info.buf )
    {
        free( s_preview_info.buf );
    }
    
    return (void *)NULL;
}


static void _dirlist_cb(dirlist_t *dirlist, dirlist_sig_e sig)
{
    bool result = true;
    
    switch(sig)
    {
    case WIDGET_SIG_DIRLIST_OK:
        video_thread_decode_preview_stop();
        s_dirlist_decode_stop_flag = true;
        video_thread_decode_preview_delete();
        if( s_decode_preview_timer_id > 0 )
        {
            stop_timer( s_decode_preview_timer_id );
        }

        
        //�˳�list sceneǰ, �����˳�dir list�Ķϵ���ں���, Ŀ¼·����ѡ�еĸ�����
        video_list_widget_menu_exit( video_vars.menuview, (menu_entry_t)_backto_dirlist );
/*        
        //���֮ǰ��nplist, ������ǰĿ¼���ļ���ӵ�nplist
        plist_clear( video_vars.nplist );
       
        video_nplist_add_dirlist( dirlist );
*/         
        //���np_tag��Ϣ, ��ͷ����ѡ�еĵ�ǰ�ļ�
        memset( &video_vars.plist.np_tag, 0x00, sizeof(video_vars.plist.np_tag) );
        //��ȡѡ���ļ�ȫ·��, ��np_index���ڲ���
        memset( video_vars.plist.np_fullpath, 0x00, sizeof(video_vars.plist.np_fullpath) );
        strcpy( video_vars.plist.np_fullpath, dirlist->path_name );
        strcat( video_vars.plist.np_fullpath, dirlist->file_name );
        print_dbg("filepath:%s, filename:%s", dirlist->path_name, dirlist->file_name);

        //ʹ���ļ�ѡ��������        
        video_vars.flag_fsel_mode = true;
        video_nplist_init(video_vars.plist.np_fullpath );
        result = video_nplist_get_index_by_fullpath( video_vars.plist.np_fullpath, &video_vars.plist.np_index );
        if( false == result )
        {
            print_err("video_nplist_get_index_by_fullpath false\n");
            video_vars.plist.np_index = -1;
        }
        
        //����������ɾ��dirlist, �������˳���main menu, �ٽ���playing
        video_vars.scene_list.app_scene.scene_type = SCENE_PLAYING;
        exit_msg_loop();
        break;
        
    case WIDGET_SIG_DIRLIST_QUIT:
        video_thread_decode_preview_stop();
        s_dirlist_decode_stop_flag = true;
        video_thread_decode_preview_delete();
        if( s_decode_preview_timer_id > 0 )
        {
            kill_timer( s_decode_preview_timer_id );
            s_decode_preview_timer_id = -1;
        }
        
        if ( VIDEO_START_MODE_DV_DIRLIST == video_vars.start_mode )
        {
            video_vars.scene_list.app_scene.scene_type = SCENE_EXIT;
            exit_msg_loop();
        }
        else
        {
            video_list_dirlist_delete();
            
            //��ʾdirlist֮ǰ��menu, ��menuview_hide�ɶ�ʹ��
            menuview_show( video_vars.menuview );
        }
        
        //����mainmenu״̬, ���ﶼ�ٽ���mainmenu. 
        //��playing�ص�dirlist�ٻص�mainmenu, ��ʱmainmenu������, ��Ҫ����;
        video_list_mainmenu_enter( video_vars.menuview, FUSION_BACKWARD );
        break;
        
    case WIDGET_SIG_DIRLIST_DECODE:
        //����timer, ��timer����ʱ���������߳�. ����:���ٷ�ҳ��, ֮ǰ��û�н���, �ַ����˴�����
        //û��timer, ����; �Ѵ�����, ��restart, Ч�����
        if ( s_decode_preview_timer_id <= 0 )
        {
            //��timer��ʱ3���Ӻ�, ����. ����߲����ָ�
            s_decode_preview_timer_id = set_single_shot_timer( 3000, \
                (timer_proc)_dirlist_proc_singel_timer_decode_preview, NULL );
        }
        else
        {
            restart_timer( s_decode_preview_timer_id );
        }
        
        //����ֹͣ�̱߳�־
        video_thread_decode_preview_stop();
        s_dirlist_decode_stop_flag = true;
        break;
        
    case WIDGET_SIG_DIRLIST_SHORT_MENU:
        
        if( s_decode_preview_timer_id > 0 )
        {
            stop_timer( s_decode_preview_timer_id );
        }
        //����softkeyboard, ��ʼ��������
        result = video_softkeyboard_create( _softkeyboard_cb );
        if ( true == result )
        {
            //hide dirlist
            dirlist_hide( dirlist );
        }
        break;
        
    default:
        break;
    }
}


/******************************************************************************/
/*!                    
* \par  Description:
*       ����dirlistĿ¼�б�
* \param[in]    
* \retval       true:�ɹ�; false:ʧ��
*   ע��: 
*       �����ڴ�playing���淵�ص�dirlistʱ, ����֮ǰ��dirlist��hide, ������show����
*******************************************************************************/
static bool _backto_dirlist( menuview_t *menuview, fusion_enter_e fenter )
{
    //����dirlist֮ǰ��menu, ��menuview_show�ɶ�ʹ��
    menuview_hide( menuview );
    
    dirlist_show( video_vars.dirlist );
    dirlist_paint( video_vars.dirlist );
    
    return true;
}
    
/******************************************************************************/
/*!                    
* \par  Description:
*       ����dirlist, Ŀ¼�б�
* \param[in]    
* \retval       true:�ɹ�; false:ʧ��
*   ע��: 
*       ����dirlistʱ, paintǰ, ��Ҫ����ǰ���menu����
*       �˳�dirlistʱ, ��Ҫ����ǰ���menu��ʾ
* fusion_set_first_frame(); fusion_set_last_frame(); fusion_display()����һ��ʹ��, �����ڴ�й©
*******************************************************************************/
bool video_list_dirlist_create( menuview_t *menuview, fusion_enter_e fenter )
{
    bool result;
    fusion_effect_t eff;
    dirlist_init_t init;
    
    /* ���ô˽���֮ǰ������, ��Ϊ��Ч��һ֡ */
    if ( FUSION_FORWARD == fenter )
    {
        fusion_get_config(FUSION_TYPE_COMMON, fenter, &eff);
        fusion_set_effect(&eff);
        
        fusion_set_first_frame( NULL );
    }
    
    s_dirlist_decode_stop_flag = false;
    
    memset( &init, 0x00, sizeof(init) );
    init.type           = DIRLIST_VIDEO;
    init.filter_string  = video_vars.buffer_support_ext_video;
    init.top_path_name  = NULL;
    
    if ( VIDEO_START_MODE_DV_DIRLIST == video_vars.start_mode )
    {
        //����top����dv���ֻ��dvĿ¼
        init.top_path_name  = video_vars.dv_top_path;
        //ʹ��dv�����filepath
        init.init_path_name = video_vars.dv_init_path;
    }
    else
    {
        //video��������, ��playing����ʱ, ���õ�ǰ����ȫ·��, ���ڸ���
        if ( FUSION_BACKWARD == fenter )
        {
            init.init_path_name = video_vars.plist.np_fullpath;
        }
        else
        {
            init.init_path_name = NULL;
        }
    }
    
    /* init type and callback function */
    video_vars.dirlist = dirlist_create( &init, _dirlist_cb );
    if ( NULL == video_vars.dirlist )
    {
        print_err("dirlist_create failed\n\n");
//        video_dialog_modal( VIDEO_DIALOG_PL_NOSP );
        return false;
    }
    
    //����dirlist֮ǰ��menu, ��menuview_show�ɶ�ʹ��
    menuview_hide( menuview );
    
    dirlist_paint( video_vars.dirlist );
    
    /* ��ʾ��Ч */
    if ( FUSION_FORWARD == fenter )
    {
        /* ���ô˽��������, ��Ϊ��Ч���һ֡ */
        fusion_set_last_frame( NULL );
        fusion_display( NULL );
    }
    
    //mmm��swf����
    video_mmm_swf_deinit();
    //����dirlist���������ͼ, ��Ҫmmm
    result = video_mmm_init();
    if ( false == result )
    {
        print_err("video_mmm_init err");
    }
    video_control_set_mem_mode( MMM_MEM_THUMBNAIL_MODE );
    if ( false == result )
    {
        print_err("set_mem_mode err");
    }
    
    return true;
}

bool video_list_dirlist_delete( void )
{
    video_thread_decode_preview_stop();
    s_dirlist_decode_stop_flag = true;
    video_thread_decode_preview_delete();
    
    dirlist_delete( video_vars.dirlist );
    video_vars.dirlist = NULL;
    
    return true;
}

