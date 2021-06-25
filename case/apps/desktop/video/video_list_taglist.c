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
 * \file     video_list_taglist.c
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
static void *_taglist_preview_decode_thread_start( void * nouse );
static menu_t * _taglist_init( void );
static bool _taglist_deinit( void );
static bool _taglist_update( void );
static bool _taglist_delete_menuitem( int tag_index );
static int _proc_item_taglist( menuview_t *menuview, menuview_sig_e signal );


/* define variable, �������� */
static menu_t *s_taglist = NULL;
static video_tag_info_t s_tag_info[VIDEO_TAGLIST_MAX_NUM];

/*! ��С�� VIDEO_TAGLIST_MAX_NUMָ��, �����ʼ������init�����ж�̬���� */
static menuitem_t s_taglist_items[VIDEO_TAGLIST_MAX_NUM]
#pragma __PRQA_IGNORE_START__
=
{ { 0 } }
#pragma __PRQA_IGNORE_END__

;

/* taglist �ĸ���index */
static int s_taglist_current = 0;


/*! ÿ��preview����Ϣ, ���ڽ��� */
static mmm_cur_video_t s_preview_info[VIDEO_TAGLIST_MAX_NUM];
static unsigned int s_preview_buffer_len;

/*! ����ʱ, ��Ҫ�˳�ʱͨ���˱����˳�, ��:����return����Ҫ�˳� */
static bool s_taglist_decode_stop_flag = false;

/*! �������, ����Ҫˢ�µ�bitmap index */
static int s_bitmap_index_to_paint = -1;

/*! ������ͼtimer_id */
static int s_timer_id = -1;
/*��¼tag_list�б�ǩ�ļ�����*/
static int s_tag_total_num = 0;
#if 0
#endif
/******************************************************************************/
/*!                    
* \par  Description:
*       �����߳�������ˢ�¶�ʱ��������
* \param[in]    
* \retval       
*   ע��: 
*       ����taglist�����̲߳���ˢ��, �����߳����������ζ�ʱ�����ˢ��
*******************************************************************************/
static int _taglist_proc_singel_timer_paint_bitmap( void *param )
{
    //ר���ڸ���menu��bitmap����
    menuview_paint_bitmap( video_vars.menuview, s_bitmap_index_to_paint );
    
    return 1;
}

/******************************************************************************/
/*!                    
* \par  Description:
*       taglist��preview�����߳�
* \param[in]    
* \retval       
*   ע��: 
*       �����̲߳���ˢ��, �����ܵ���menuview_paint_bitmap
*       �յ�dirlist��WIDGET_SIG_DIRLIST_DECODE�źź��������߳̽���
*       ����ֹͣ��־, ���ڽ�����δ����˳��̵߳����
*******************************************************************************/
static void *_taglist_preview_decode_thread_start( void * nouse )
{
    bool result = true;
    int i;
    int tmp = -1;
    int preview_num = VIDEO_TAGLIST_MAX_NUM;
    plist_detail_info_t detail_info;
    
    if ( s_tag_total_num < VIDEO_TAGLIST_MAX_NUM )
    {
        preview_num = s_tag_total_num;
    }
    for( i=0; i<preview_num; i++ )
    {
        tmp = plist_get_items_to_mallocbuf( video_vars.taglist, &detail_info, i, 1, false );
        if ( (-1 != tmp) && (NULL != detail_info.fullpath) )
        {
            result = video_thread_get_one_preview( detail_info.fullpath, detail_info.markinfo.time, \
                    &s_preview_info[i] );
            if ( true == result )
            {
                //��������menu��bitmap����ĵ��ζ�ʱ��
                s_bitmap_index_to_paint = i;
                //û��timer, ����; �Ѵ�����, ��restart, Ч�����
                if ( s_timer_id <= 0 )
                {
                    s_timer_id = set_single_shot_timer( 1, \
                        (timer_proc)_taglist_proc_singel_timer_paint_bitmap, NULL );
                }
                else
                {
                    restart_timer( s_timer_id );
                }
            }
        }
        plist_free_buf( &detail_info, 1 );
    
        //����ʱ, �������return����Ҫ�˳������
        if ( true == s_taglist_decode_stop_flag )
        {
            s_taglist_decode_stop_flag = false;
            break;
        }
    }
    return (void *)NULL;
}

/******************************************************************************/
/*!                    
* \par  Description:
*       ����taglist
* \param[in]    menuview_t *: �ؼ�ָ��
* \param[in]    fusion_enter_e: ������Ч
* \retval       true:�ɹ�; false:ʧ��
*   ע��: 
* fusion_set_first_frame(); fusion_set_last_frame(); fusion_display()����һ��ʹ��, �����ڴ�й©
*******************************************************************************/
bool video_list_taglist_enter( menuview_t *menuview, fusion_enter_e fenter )
{
    bool result;
    fusion_effect_t eff;
    menu_t *cur_menu;

    s_tag_total_num = 0;
   
    /* ���ô˽���֮ǰ������, ��Ϊ��Ч��һ֡ */
    if ( FUSION_FORWARD == fenter )
    {
        fusion_get_config(FUSION_TYPE_COMMON, fenter, &eff);
        fusion_set_effect(&eff);
        
        fusion_set_first_frame( NULL );
    }
    
    s_taglist_decode_stop_flag = false;
    
    //����Ϊpiclilst_video ���
    menuview_load_style( menuview, MENUVIEW_PICLIST_VIDEO );

    s_tag_total_num = plist_get_item_num( video_vars.taglist );

    cur_menu = _taglist_init();
    
    //list����Ϊ0, ��ʾ�˳�
    if ( s_tag_total_num <= 0 )
    {
#if VIDEO_CFG_ENTER_WHEN_TAG_EMPTY == 0
        g_enter_timer = 1;

        video_dialog_create( VIDEO_DIALOG_NO_TAG, NULL );
        video_list_taglist_exit();
        return false;
#else
        menuview_set_menu( menuview, cur_menu );
        menuview_paint( menuview );
    
        /* �����Ǵ�playing����ʱ����ʾ��Ч */
        if ( FUSION_FORWARD == fenter )
        {
            /* ���ô˽��������, ��Ϊ��Ч���һ֡ */
            fusion_set_last_frame( NULL );
            fusion_display( NULL );
        }
        
        video_dialog_modal( VIDEO_DIALOG_NO_TAG );
#if VIDEO_CFG_RETURN_WHEN_TAG_EMPTY > 0
        video_list_taglist_exit();
        return false;
#else
        return true;
#endif /* end #if VIDEO_CFG_RETURN_WHEN_TAG_EMPTY > 0 */

#endif /* end #if VIDEO_CFG_ENTER_WHEN_TAG_EMPTY == 0 */
    }
    
    menuview_set_menu( menuview, cur_menu );
    menuview_paint( menuview );
    
    /* �����Ǵ�playing����ʱ����ʾ��Ч */
    if ( FUSION_FORWARD == fenter )
    {
        /* ���ô˽��������, ��Ϊ��Ч���һ֡ */
        fusion_set_last_frame( NULL );
        fusion_display( NULL );
    }
    /*��������ļ���ж���м�����ʱ�����Ƚ�֮ǰ������ˢ�µ���Ļ*/
    gui_screen_update();
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
    
    //���������߳�
    video_thread_decode_preview_create( _taglist_preview_decode_thread_start );

    return true;
}


/* �˳�taglist */
bool video_list_taglist_exit( void )
{
    video_thread_decode_preview_stop();
    s_taglist_decode_stop_flag = true;
    video_thread_decode_preview_delete();
    
    _taglist_deinit();

    return true;
}

static menu_t * _taglist_init( void )
{
    int i;
    sty_rect_t srect;
    
    //initial menu
    if( s_taglist == NULL )
    {
        //create menu
        s_taglist = menu_create();
        if ( NULL == s_taglist )
        {
            print_err("menu_create failed");
            return NULL;
        }
        //set menu callback, ��̬�˵�����ָ��Ϊs_category, ������Ϊ_proc_items
        s_taglist->callback = (menu_cb_t)_proc_item_taglist;
        
        //һ����menuview_load_style֮��, ��Ϊmenuview_get_bitmap_width������menuview
        /* ���rect��С, ��������Ӧrect��s_preview_buffer_len */
        srect.x = 0;
        srect.y = 0;
        srect.width = (short)menuview_get_bitmap_width( video_vars.menuview );
        srect.height= (short)menuview_get_bitmap_height( video_vars.menuview );
        //ÿ����ռ��2��byte
        s_preview_buffer_len = (unsigned int)(2 * srect.width * srect.height);
        
        //s_taglist_items������VIDEO_TAGLIST_MAX_NUM�仯, ֻ�ܶ�̬�������Ա, ��Ҫ��̬����
        memset( &s_taglist_items, 0x00, sizeof(s_taglist_items) );
        for ( i=0; i<VIDEO_TAGLIST_MAX_NUM; i++ )
        {
            s_taglist_items[i].bitmap = (gui_bitmap_info_t *)malloc( sizeof(gui_bitmap_info_t) );
            if ( NULL != s_taglist_items[i].bitmap )
            {
                //��ʼ��preview
                s_preview_info[i].xpos = (unsigned int)srect.x;
                s_preview_info[i].ypos = (unsigned int)srect.y;
                s_preview_info[i].width = (unsigned int)srect.width;
                s_preview_info[i].height= (unsigned int)srect.height;
                s_preview_info[i].buf = malloc( s_preview_buffer_len );
                if ( NULL == s_preview_info[i].buf )
                {
                    //shaofeng
                    print_err("malloc%d preview failed", i);
                    return NULL;
                }
                memset( s_preview_info[i].buf, i, s_preview_buffer_len );
                
                //��preview��Ϣ���õ�taglist item
                memset( s_taglist_items[i].bitmap, 0x00, sizeof(gui_bitmap_info_t) );
                s_taglist_items[i].bitmap->xsize = (unsigned short)srect.width;
                s_taglist_items[i].bitmap->ysize = (unsigned short)srect.height;
                s_taglist_items[i].bitmap->bytesperpixel = 2;
                s_taglist_items[i].bitmap->pdata = s_preview_info[i].buf;
            }
            else
            {
                //shaofeng
                print_err("malloc%d bitmap failed", i);
                return NULL;
            }
        }

        video_load_taglist_menuitem( s_tag_total_num, s_tag_info, s_taglist_items );
        menu_remove_all_items( s_taglist );
        menu_append_items( s_taglist, s_taglist_items, s_tag_total_num );
        
        //init current index.. ǰ����tag_num <= 0����. �����tag_numһ���� >= 1
        if ( s_taglist_current > (s_tag_total_num-1) )
        {
            s_taglist_current = s_tag_total_num-1;
        }
        menu_set_cur_index( s_taglist, s_taglist_current );
    }

    return s_taglist;
}


static bool _taglist_deinit( void )
{
    int i;
    
    for ( i=0; i<VIDEO_TAGLIST_MAX_NUM; i++ )
    {
        free( s_preview_info[i].buf );
        s_preview_info[i].buf = NULL;
        
        free( s_taglist_items[i].bitmap );
        s_taglist_items[i].bitmap = NULL;
    }
    
    //delete menu   
    if( NULL == s_taglist )
    {
        return false;
    }
    
    menu_delete( s_taglist );
    s_taglist = NULL;

    return true;
}

/* ��ɾ������, ��append, �����append����ǰ�б���� */
static bool _taglist_update( void )
{
    
//    tag_num = plist_get_item_num( video_vars.taglist );
    //tag_num���ٵ�0, ��Ҫ���ô˺���������taglist. �������ﲻ��==0ʱ����
    if ( s_tag_total_num < 0 )
    {
        return false;
    }
    
    video_load_taglist_menuitem( s_tag_total_num, s_tag_info, s_taglist_items );
    menu_remove_all_items( s_taglist );
    menu_append_items( s_taglist, s_taglist_items, s_tag_total_num );

    //init current index.. ǰ����tag_num <= 0����. �����tag_num >= 1
    if ( (s_tag_total_num > 0) && (s_taglist_current > (s_tag_total_num-1)) )
    {
        s_taglist_current = s_tag_total_num-1;
    }
    menu_set_cur_index( s_taglist, s_taglist_current );
    
    menuview_paint( video_vars.menuview );
    
    return true;
}

static bool _taglist_delete_menuitem( int tag_index )
{
    bool result = true;
    int i;
    
    result = video_taglist_del_item( tag_index );
    s_tag_total_num = plist_get_item_num( video_vars.taglist );
    if ( true == result )
    {
        //��ɾ��index֮���preview����, ˳����ǰ����, ����preview
        for( i=tag_index; i<s_tag_total_num; i++ )
        {
            memcpy( s_preview_info[i].buf, s_preview_info[i+1].buf, s_preview_buffer_len );
            menuview_paint_bitmap( video_vars.menuview, i );
        }
        
        if ( tag_index >= s_tag_total_num )
        {
            tag_index = s_tag_total_num;
        }
        s_taglist_current = tag_index;
        
        _taglist_update();
    }
    
    return result;
}



static int _proc_item_taglist( menuview_t *menuview, menuview_sig_e signal )
{
    bool result = true;
    int tmp = 0;
    int index_cur = 0;
    video_tag_info_t tag_info;
    char fullpath[PATH_MAX];

    switch ( signal ) 
    {
    case WIDGET_SIG_MENUVIEW_OK:
        video_thread_decode_preview_stop();
        s_taglist_decode_stop_flag = true;
        video_thread_decode_preview_delete();

        if( 0 == s_tag_total_num )
        {
            video_dialog_modal( VIDEO_DIALOG_NO_TAG );
#if VIDEO_CFG_RETURN_WHEN_TAG_EMPTY > 0
            //Ϊ��, �˳�
            video_list_taglist_exit();
            video_list_mainmenu_enter( menuview, FUSION_BACKWARD );
#endif
            return 1;
        }   
        
        //���tag��Ϣ, append��nplist���, playingʹ��np_index���в���
        index_cur = menuview->menu->global_offset + menuview->menu->current;
        print_dbg("index_cur = %d\n",index_cur);
        //����tag�ļ�, 
        result = video_taglist_get_tag_info( index_cur, &tag_info );

        video_vars.flag_fsel_mode = false;
        
        result = video_playlist_get_fullpath_by_index( video_vars.taglist, index_cur, fullpath );
        if ( true == result )
        {          
            video_nplist_init( fullpath );

            //��np_list��ղ�������tag_list��ӵ�np_list
            result = video_add_taglist_to_nplist( s_tag_total_num );
            if( false == result )
            {
                return 0;
            }
            
            //��ȡtag�ļ�ȫ·��, ��ӵ�nplist, ������np_index���ڲ���
            strcpy( video_vars.plist.np_fullpath, fullpath );

            video_vars.plist.np_tag.time = tag_info.time;
            video_vars.plist.np_index = index_cur;
            //�˳�list sceneǰ, �����˳�tag list�Ķϵ���ں�����ѡ�еĸ�����
            video_list_widget_menu_exit( menuview, (menu_entry_t)video_list_taglist_enter );
            s_taglist_current = index_cur;
            
            video_vars.scene_list.app_scene.scene_type = SCENE_PLAYING;
            exit_msg_loop();
        }
        else
        {
            //tag������, ��ʾ, ɾ��tag
            video_dialog_modal( VIDEO_DIALOG_OPEN_FAIL );
            result = _taglist_delete_menuitem( index_cur );
            if ( true == result )
            {
//                s_tag_total_num = plist_get_item_num( video_vars.taglist );
                if ( s_tag_total_num <= 0 )
                {
                    video_dialog_modal( VIDEO_DIALOG_NO_TAG );
#if VIDEO_CFG_RETURN_WHEN_TAG_EMPTY > 0
                    //Ϊ��, �˳�
                    video_list_taglist_exit();
                    video_list_mainmenu_enter( menuview, FUSION_BACKWARD );
#endif
                }
            }
        }
        break;

    case WIDGET_SIG_MENUVIEW_QUIT:
        video_thread_decode_preview_stop();
        s_taglist_decode_stop_flag = true;
        video_thread_decode_preview_delete();
        
        video_list_taglist_exit();
        
        video_list_mainmenu_enter( menuview, FUSION_BACKWARD );
        break;
        
    case WIDGET_SIG_MENUVIEW_SHORT_MENU:
    case WIDGET_SIG_MENUVIEW_DELETE:
        
#if VIDEO_CFG_ENTER_WHEN_TAG_EMPTY > 0 
        /*ֻ��VIDEO_CFG_ENTER_WHEN_TAG_EMPTY>0ʱ������ǩ�б�Ϊ�ղŻ����Ŀ¼�б�Ҳ���б�Ҫ�ڴ˼��*/
        /*���û�б�ǩ��ֱ�ӷ���*/
//        tag_num = plist_get_item_num( video_vars.taglist );       
        if( s_tag_total_num <=0 )
        {
            return 1;
        }
#endif        
        //shaofeng tag����decode??
        //����ɾ��tag��dialog
        tmp = video_dialog_modal( VIDEO_DIALOG_DELETE_TAG );
        //ɾ����ǰtag, ����taglist����, ����menu��ʾ
        if ( (int)WIDGET_SIG_DIALOG_OK == tmp )
        {
            index_cur = menuview->menu->global_offset + menuview->menu->current;
            result = _taglist_delete_menuitem( index_cur );
            if ( true == result )
            {
//                s_tag_total_num = plist_get_item_num( video_vars.taglist );
                if ( s_tag_total_num <= 0 )
                {
                    video_dialog_modal( VIDEO_DIALOG_NO_TAG );
#if VIDEO_CFG_RETURN_WHEN_TAG_EMPTY > 0
                    //Ϊ��, �˳�
                    video_list_taglist_exit();
                    video_list_mainmenu_enter( menuview, FUSION_BACKWARD );
#endif
                }
            }
        }
        break;
        
    default:
        break;
    }
    
    return 1;
}

