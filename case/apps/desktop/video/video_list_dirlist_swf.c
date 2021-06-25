/*******************************************************************************
 *                              NOYA1100
 *                            Module: video
 *                 Copyright(c) 2003-2008 Actions Semiconductor,
 *                            All Rights Reserved. 
 * History:         
 *      <author>    <time>           <version >             <desc>
 *      shaofeng    2009-09-02          1.0             build this file 
******************************************************************************/
/*!
 * \file     video_list_dirlist_swf.c
 * \brief    
 * \author   shaofeng
 * \par      GENERAL DESCRIPTION:
 *               
 * \par      EXTERNALIZED FUNCTIONS:
 *               
 * \version 1.0
 * \date  2009/09/02
*******************************************************************************/

#include "include_psp.h"
#include "include_case.h"

/* include file of this application, ��Ӧ�õ�ͷ�ļ� */
#include "include_video.h"



/* extern function, �������� */
static void _proc_timer_searching_done( void );
static void *_pthread_deal_searching( void *nouse );

static void _softkeyboard_cb( softkeyboard_t *softkeyboard, softkeyboard_sig_e signal );

static void _dirlist_cb(dirlist_t *dirlist, dirlist_sig_e sig);
static bool _backto_dirlist( menuview_t *menuview, fusion_enter_e fenter );




/* define variable, �������� */

static int s_search_num = -1;


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


static void _dirlist_cb(dirlist_t *dirlist, dirlist_sig_e sig)
{
    bool result = true;
    
    switch(sig)
    {
    case WIDGET_SIG_DIRLIST_OK:
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
        video_nplist_init( video_vars.plist.np_fullpath );
        
        result = video_nplist_get_index_by_fullpath( video_vars.plist.np_fullpath, &video_vars.plist.np_index );
        if( false == result )
        {
            print_err("video_nplist_get_index_by_fullpath false\n");
            video_vars.plist.np_index = -1;
        }
        
        //����������ɾ��dirlist, �������˳���main menu, �ٽ���playing
        video_vars.scene_list.app_scene.scene_type = SCENE_SWF;
        exit_msg_loop();
        break;
        
    case WIDGET_SIG_DIRLIST_QUIT:
        if ( VIDEO_START_MODE_DV_DIRLIST == video_vars.start_mode )
        {
            video_vars.scene_list.app_scene.scene_type = SCENE_EXIT;
            exit_msg_loop();
        }
        else
        {
            video_list_dirlist_swf_delete();
            
            //��ʾdirlist֮ǰ��menu, ��menuview_hide�ɶ�ʹ��
            menuview_show( video_vars.menuview );
        }
        
        //����mainmenu״̬, ���ﶼ�ٽ���mainmenu. 
        //��playing�ص�dirlist�ٻص�mainmenu, ��ʱmainmenu������, ��Ҫ����;
        video_list_mainmenu_enter( video_vars.menuview, FUSION_BACKWARD );
        break;
        
    case WIDGET_SIG_DIRLIST_SHORT_MENU:
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
bool video_list_dirlist_swf_create( menuview_t *menuview, fusion_enter_e fenter )
{
    fusion_effect_t eff;
    dirlist_init_t init;
    
    /* ���ô˽���֮ǰ������, ��Ϊ��Ч��һ֡ */
    if ( FUSION_FORWARD == fenter )
    {
        fusion_get_config(FUSION_TYPE_COMMON, fenter, &eff);
        fusion_set_effect(&eff);
        
        fusion_set_first_frame( NULL );
    }
    
    memset( &init, 0x00, sizeof(init) );
    init.type           = DIRLIST_VIDEO;
    init.filter_string  = video_vars.buffer_support_ext_video_swf;
    init.top_path_name  = NULL;
    
    //video��������, ��playing����ʱ, ���õ�ǰ����ȫ·��, ���ڸ���
    if ( FUSION_BACKWARD == fenter )
    {
        init.init_path_name = video_vars.plist.np_fullpath;
    }
    else
    {
        init.init_path_name = NULL;
    }
    
    /* init type and callback function */
    video_vars.dirlist = dirlist_create( &init, _dirlist_cb );
    if ( NULL == video_vars.dirlist )
    {
//        video_dialog_modal( VIDEO_DIALOG_PL_NOSP );
        print_err("dirlist_create failed\n\n");
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
    
    return true;
}

bool video_list_dirlist_swf_delete( void )
{
    dirlist_delete( video_vars.dirlist );
    video_vars.dirlist = NULL;
    
    return true;
}

