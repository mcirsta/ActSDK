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

/* include file of this application, 本应用的头文件 */
#include "include_video.h"


/* extern function, 函数声明 */
static void *_taglist_preview_decode_thread_start( void * nouse );
static menu_t * _taglist_init( void );
static bool _taglist_deinit( void );
static bool _taglist_update( void );
static bool _taglist_delete_menuitem( int tag_index );
static int _proc_item_taglist( menuview_t *menuview, menuview_sig_e signal );


/* define variable, 变量定义 */
static menu_t *s_taglist = NULL;
static video_tag_info_t s_tag_info[VIDEO_TAGLIST_MAX_NUM];

/*! 大小由 VIDEO_TAGLIST_MAX_NUM指定, 对其初始化放在init函数中动态进行 */
static menuitem_t s_taglist_items[VIDEO_TAGLIST_MAX_NUM]
#pragma __PRQA_IGNORE_START__
=
{ { 0 } }
#pragma __PRQA_IGNORE_END__

;

/* taglist 的高亮index */
static int s_taglist_current = 0;


/*! 每个preview的信息, 用于解码 */
static mmm_cur_video_t s_preview_info[VIDEO_TAGLIST_MAX_NUM];
static unsigned int s_preview_buffer_len;

/*! 解码时, 需要退出时通过此变量退出, 如:按了return键需要退出 */
static bool s_taglist_decode_stop_flag = false;

/*! 解码完成, 并需要刷新的bitmap index */
static int s_bitmap_index_to_paint = -1;

/*! 画缩略图timer_id */
static int s_timer_id = -1;
/*记录tag_list中标签文件总数*/
static int s_tag_total_num = 0;
#if 0
#endif
/******************************************************************************/
/*!                    
* \par  Description:
*       解码线程启动的刷新定时器处理函数
* \param[in]    
* \retval       
*   注意: 
*       由于taglist解码线程不能刷屏, 则在线程中启动单次定时器完成刷屏
*******************************************************************************/
static int _taglist_proc_singel_timer_paint_bitmap( void *param )
{
    //专用于更新menu的bitmap区域
    menuview_paint_bitmap( video_vars.menuview, s_bitmap_index_to_paint );
    
    return 1;
}

/******************************************************************************/
/*!                    
* \par  Description:
*       taglist的preview解码线程
* \param[in]    
* \retval       
*   注意: 
*       解码线程不能刷屏, 即不能调用menuview_paint_bitmap
*       收到dirlist的WIDGET_SIG_DIRLIST_DECODE信号后启动此线程解码
*       设置停止标志, 用于解码尚未完毕退出线程的情况
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
                //启动更新menu的bitmap区域的单次定时器
                s_bitmap_index_to_paint = i;
                //没有timer, 创建; 已创建过, 则restart, 效率最高
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
    
        //解码时, 如果按了return键需要退出等情况
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
*       进入taglist
* \param[in]    menuview_t *: 控件指针
* \param[in]    fusion_enter_e: 进入特效
* \retval       true:成功; false:失败
*   注意: 
* fusion_set_first_frame(); fusion_set_last_frame(); fusion_display()必须一起使用, 否则内存泄漏
*******************************************************************************/
bool video_list_taglist_enter( menuview_t *menuview, fusion_enter_e fenter )
{
    bool result;
    fusion_effect_t eff;
    menu_t *cur_menu;

    s_tag_total_num = 0;
   
    /* 设置此界面之前的数据, 作为特效第一帧 */
    if ( FUSION_FORWARD == fenter )
    {
        fusion_get_config(FUSION_TYPE_COMMON, fenter, &eff);
        fusion_set_effect(&eff);
        
        fusion_set_first_frame( NULL );
    }
    
    s_taglist_decode_stop_flag = false;
    
    //更换为piclilst_video 风格
    menuview_load_style( menuview, MENUVIEW_PICLIST_VIDEO );

    s_tag_total_num = plist_get_item_num( video_vars.taglist );

    cur_menu = _taglist_init();
    
    //list个数为0, 提示退出
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
    
        /* 当不是从playing回退时才显示特效 */
        if ( FUSION_FORWARD == fenter )
        {
            /* 设置此界面的数据, 作为特效最后一帧 */
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
    
    /* 当不是从playing回退时才显示特效 */
    if ( FUSION_FORWARD == fenter )
    {
        /* 设置此界面的数据, 作为特效最后一帧 */
        fusion_set_last_frame( NULL );
        fusion_display( NULL );
    }
    /*由于下面的加载卸载中间件会耗时长，先将之前的数据刷新到屏幕*/
    gui_screen_update();
    //mmm与swf互斥
    video_mmm_swf_deinit();
    //进入dirlist会解码缩略图, 需要mmm
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
    
    //启动解码线程
    video_thread_decode_preview_create( _taglist_preview_decode_thread_start );

    return true;
}


/* 退出taglist */
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
        //set menu callback, 动态菜单数据指针为s_category, 处理函数为_proc_items
        s_taglist->callback = (menu_cb_t)_proc_item_taglist;
        
        //一定在menuview_load_style之后, 因为menuview_get_bitmap_width依赖于menuview
        /* 获得rect大小, 并计算相应rect的s_preview_buffer_len */
        srect.x = 0;
        srect.y = 0;
        srect.width = (short)menuview_get_bitmap_width( video_vars.menuview );
        srect.height= (short)menuview_get_bitmap_height( video_vars.menuview );
        //每个点占用2个byte
        s_preview_buffer_len = (unsigned int)(2 * srect.width * srect.height);
        
        //s_taglist_items个数随VIDEO_TAGLIST_MAX_NUM变化, 只能动态设置其成员, 不要静态定义
        memset( &s_taglist_items, 0x00, sizeof(s_taglist_items) );
        for ( i=0; i<VIDEO_TAGLIST_MAX_NUM; i++ )
        {
            s_taglist_items[i].bitmap = (gui_bitmap_info_t *)malloc( sizeof(gui_bitmap_info_t) );
            if ( NULL != s_taglist_items[i].bitmap )
            {
                //初始化preview
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
                
                //将preview信息设置到taglist item
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
        
        //init current index.. 前面有tag_num <= 0处理. 这里的tag_num一定是 >= 1
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

/* 先删除所有, 再append, 否则会append到当前列表后面 */
static bool _taglist_update( void )
{
    
//    tag_num = plist_get_item_num( video_vars.taglist );
    //tag_num减少到0, 需要调用此函数来画空taglist. 所以这里不能==0时返回
    if ( s_tag_total_num < 0 )
    {
        return false;
    }
    
    video_load_taglist_menuitem( s_tag_total_num, s_tag_info, s_taglist_items );
    menu_remove_all_items( s_taglist );
    menu_append_items( s_taglist, s_taglist_items, s_tag_total_num );

    //init current index.. 前面有tag_num <= 0处理. 这里的tag_num >= 1
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
        //将删除index之后的preview内容, 顺次向前覆盖, 更新preview
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
            //为空, 退出
            video_list_taglist_exit();
            video_list_mainmenu_enter( menuview, FUSION_BACKWARD );
#endif
            return 1;
        }   
        
        //获得tag信息, append到nplist最后, playing使用np_index进行播放
        index_cur = menuview->menu->global_offset + menuview->menu->current;
        print_dbg("index_cur = %d\n",index_cur);
        //查找tag文件, 
        result = video_taglist_get_tag_info( index_cur, &tag_info );

        video_vars.flag_fsel_mode = false;
        
        result = video_playlist_get_fullpath_by_index( video_vars.taglist, index_cur, fullpath );
        if ( true == result )
        {          
            video_nplist_init( fullpath );

            //将np_list清空并将整个tag_list添加到np_list
            result = video_add_taglist_to_nplist( s_tag_total_num );
            if( false == result )
            {
                return 0;
            }
            
            //获取tag文件全路径, 添加到nplist, 并设置np_index用于播放
            strcpy( video_vars.plist.np_fullpath, fullpath );

            video_vars.plist.np_tag.time = tag_info.time;
            video_vars.plist.np_index = index_cur;
            //退出list scene前, 保存退出tag list的断点入口函数和选中的高亮项
            video_list_widget_menu_exit( menuview, (menu_entry_t)video_list_taglist_enter );
            s_taglist_current = index_cur;
            
            video_vars.scene_list.app_scene.scene_type = SCENE_PLAYING;
            exit_msg_loop();
        }
        else
        {
            //tag不存在, 提示, 删除tag
            video_dialog_modal( VIDEO_DIALOG_OPEN_FAIL );
            result = _taglist_delete_menuitem( index_cur );
            if ( true == result )
            {
//                s_tag_total_num = plist_get_item_num( video_vars.taglist );
                if ( s_tag_total_num <= 0 )
                {
                    video_dialog_modal( VIDEO_DIALOG_NO_TAG );
#if VIDEO_CFG_RETURN_WHEN_TAG_EMPTY > 0
                    //为空, 退出
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
        /*只有VIDEO_CFG_ENTER_WHEN_TAG_EMPTY>0时，当标签列表为空才会进入目录列表，也才有必要在此检查*/
        /*如果没有标签，直接返回*/
//        tag_num = plist_get_item_num( video_vars.taglist );       
        if( s_tag_total_num <=0 )
        {
            return 1;
        }
#endif        
        //shaofeng tag正在decode??
        //弹出删除tag的dialog
        tmp = video_dialog_modal( VIDEO_DIALOG_DELETE_TAG );
        //删除当前tag, 更新taglist内容, 更新menu显示
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
                    //为空, 退出
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

