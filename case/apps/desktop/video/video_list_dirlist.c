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

/* include file of this application, 本应用的头文件 */
#include "include_video.h"



/* extern function, 函数声明 */
static void _proc_timer_searching_done( void );
static void *_pthread_deal_searching( void *nouse );

static void _softkeyboard_cb( softkeyboard_t *softkeyboard, softkeyboard_sig_e signal );

static int _dirlist_proc_singel_timer_paint_bitmap( void *param );
static int _dirlist_proc_singel_timer_decode_preview( void *param );

static void *_dirlist_preview_decode_thread_start( void * nouse );
static void _dirlist_cb(dirlist_t *dirlist, dirlist_sig_e sig);
static bool _backto_dirlist( menuview_t *menuview, fusion_enter_e fenter );




/* define variable, 变量定义 */
/*! dirlist显示时使用的结构体 */
static gui_bitmap_info_t s_preview_bitmap_info;

/*! video mmm解码时使用的结构体 */
static mmm_cur_video_t s_preview_info;

/*! 解码时, 需要退出时通过此变量退出, 如:按了return键需要退出 */
static bool s_dirlist_decode_stop_flag = false;

/*! 解码完成, 并需要刷新的bitmap index */
static int s_bitmap_index_to_paint = -1;

static int s_paint_timer_id = -1;

static int s_search_num = -1;

/* 收到dirlist的解码消息后, 延迟解码的定时器 */
static int s_decode_preview_timer_id = -1;


#if 0
#endif
/******************************************************************************/
/*!                    
* \par  Description:
*      timer处理函数
* \retval       void
*   注意: 
*       
*******************************************************************************/
static void _proc_timer_searching_done( void )
{ 
    //完成搜索后, 删除loadingbox显示
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
        
        //先停止线程, 再启动. 
        video_thread_decode_preview_stop();
        s_dirlist_decode_stop_flag = true;
        video_thread_decode_preview_delete();
        //启动解码线程
        video_thread_decode_preview_create( _dirlist_preview_decode_thread_start );
    }
}

/******************************************************************************/
/*!                    
* \par  Description:
*       搜索线程主体函数
*       用于获取softkeyboard输入文字, 设置给dirlist搜索
* \retval       
*   注意: 
*       此线程主要耗时在dirlist_set_search_string函数
*       此线程最后启动定时器, 用于在主进程完成显示等任务. 因为线程不能做显示动作
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
*      创建detached型线程, 用于搜索
* \retval       bool
*   注意: 
*       创建的线程比主进程低一级的线程, 保证主进程先运行
*******************************************************************************/
bool video_search_thread_create( void *(*start_rtn)(void*) )
{
    int result = -1;
    pthread_t pthread;
    pthread_attr_t pthread_attr;
    struct sched_param param;
    struct sched_param self_param;
    int policy;
    
    //创建比主进程低一级的线程, 保证主进程先运行
    pthread_attr_init(&pthread_attr);
    pthread_attr_setinheritsched(&pthread_attr, PTHREAD_EXPLICIT_SCHED);
    pthread_attr_setdetachstate(&pthread_attr, PTHREAD_CREATE_DETACHED);
    pthread_attr_setscope(&pthread_attr, PTHREAD_SCOPE_PROCESS);
    
    //创建比主进程低一级的线程, 保证主进程先运行
    pthread_getschedparam( pthread_self(), &policy, &self_param );
    param.sched_priority = self_param.sched_priority - 2;
    pthread_attr_setschedparam(&pthread_attr, &param);
    print_dbg(" manager_msg_loop  thread's priority is: %d\n", param.sched_priority );

    result = pthread_create( &pthread, &pthread_attr, start_rtn, NULL );
    pthread_attr_destroy( &pthread_attr );
    if( result != 0 )
    {
        print_err("can’t create thread, result:%d\n", result);
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
*       解码线程启动的刷新定时器处理函数
* \param[in]    
* \retval       
*   注意: 
*       由于dirlist解码线程不能刷屏, 则在线程中启动单次定时器完成刷屏
*******************************************************************************/
static int _dirlist_proc_singel_timer_paint_bitmap( void *param )
{
#if 0
    //更新dirlist的bitmap区域, 一次更新一个
    dirlist_paint_bitmap( video_vars.dirlist, s_bitmap_index_to_paint );
#else
    //更新dirlist的bitmap区域, 一次更新已有的全部
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
*       dirlist的解码命令启动的解码线程定时器处理函数, 用于真正创建解码线程
* \param[in]    
* \retval       
*******************************************************************************/
static int _dirlist_proc_singel_timer_decode_preview( void *param )
{
    //先停止线程, 再启动. 用于:如果快速翻页之前没有解完, 又发送了此命令
    video_thread_decode_preview_stop();
    s_dirlist_decode_stop_flag = true;
    video_thread_decode_preview_delete();
    
    //启动解码线程
    video_thread_decode_preview_create( _dirlist_preview_decode_thread_start );
    
    return 1;
}

/******************************************************************************/
/*!                    
* \par  Description:
*       dirlist的preview解码线程
* \param[in]    
* \retval       
*   注意: 
*       解码线程不能刷屏, 即不能调用menuview_paint_bitmap
*       收到dirlist的WIDGET_SIG_DIRLIST_DECODE信号后启动此线程解码
*       设置停止标志, 用于解码尚未完毕退出线程的情况
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
    
    //初始化preview_info, 用于thread解码
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
    
    //初始化preview_bitmap_info, 将thread解码结果给dirlist显示
    s_preview_bitmap_info.xsize = (unsigned short)s_preview_info.width;
    s_preview_bitmap_info.ysize = (unsigned short)s_preview_info.height;
    s_preview_bitmap_info.bytesperpixel= 2;
    s_preview_bitmap_info.pdata = s_preview_info.buf;
    
    //获得解码开始index, total num
    preview_num = dirlist_get_total( video_vars.dirlist );
    //preview_num屏幕上的局部index, 所以从0开始, 详见dirlist接口
    for( i=0; i<preview_num; i++ )
    {
        is_dir = dirlist_is_item_dir( video_vars.dirlist, i );

        //判断是否是目录, 是否已经存在bitmap, 都不是则去解码
        p_tmp_bitmap_info = dirlist_get_item_bitmap( video_vars.dirlist, i);
        if ( (false == is_dir) && (NULL == p_tmp_bitmap_info) )
        {
            memset( fullpath, 0x00, sizeof(fullpath) );
            p_filename = dirlist_get_item_name( video_vars.dirlist, i);
            sprintf(fullpath, "%s%s", video_vars.dirlist->path_name, p_filename);
            
            //是否是recent file, 以获取tag时间
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
                
                //启动更新menu的bitmap区域的单次定时器
                s_bitmap_index_to_paint = i;
                //没有timer, 创建; 已创建过, 则restart, 效率最高
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
        
        //收到退出线程标志, 退出. 如:解码时, 按了return键需要退出的情况
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

        
        //退出list scene前, 保存退出dir list的断点入口函数, 目录路径和选中的高亮项
        video_list_widget_menu_exit( video_vars.menuview, (menu_entry_t)_backto_dirlist );
/*        
        //清除之前的nplist, 并将当前目录的文件添加到nplist
        plist_clear( video_vars.nplist );
       
        video_nplist_add_dirlist( dirlist );
*/         
        //清除np_tag信息, 从头播放选中的当前文件
        memset( &video_vars.plist.np_tag, 0x00, sizeof(video_vars.plist.np_tag) );
        //获取选中文件全路径, 及np_index用于播放
        memset( video_vars.plist.np_fullpath, 0x00, sizeof(video_vars.plist.np_fullpath) );
        strcpy( video_vars.plist.np_fullpath, dirlist->path_name );
        strcat( video_vars.plist.np_fullpath, dirlist->file_name );
        print_dbg("filepath:%s, filename:%s", dirlist->path_name, dirlist->file_name);

        //使用文件选择器播放        
        video_vars.flag_fsel_mode = true;
        video_nplist_init(video_vars.plist.np_fullpath );
        result = video_nplist_get_index_by_fullpath( video_vars.plist.np_fullpath, &video_vars.plist.np_index );
        if( false == result )
        {
            print_err("video_nplist_get_index_by_fullpath false\n");
            video_vars.plist.np_index = -1;
        }
        
        //不能在这里删除dirlist, 否则先退出到main menu, 再进入playing
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
            
            //显示dirlist之前的menu, 与menuview_hide成对使用
            menuview_show( video_vars.menuview );
        }
        
        //不管mainmenu状态, 这里都再进入mainmenu. 
        //从playing回到dirlist再回到mainmenu, 此时mainmenu不存在, 需要创建;
        video_list_mainmenu_enter( video_vars.menuview, FUSION_BACKWARD );
        break;
        
    case WIDGET_SIG_DIRLIST_DECODE:
        //创建timer, 在timer中延时启动解码线程. 用于:快速翻页是, 之前的没有解完, 又发送了此命令
        //没有timer, 创建; 已创建过, 则restart, 效率最高
        if ( s_decode_preview_timer_id <= 0 )
        {
            //此timer延时3秒钟后, 运行. 以提高操作手感
            s_decode_preview_timer_id = set_single_shot_timer( 3000, \
                (timer_proc)_dirlist_proc_singel_timer_decode_preview, NULL );
        }
        else
        {
            restart_timer( s_decode_preview_timer_id );
        }
        
        //设置停止线程标志
        video_thread_decode_preview_stop();
        s_dirlist_decode_stop_flag = true;
        break;
        
    case WIDGET_SIG_DIRLIST_SHORT_MENU:
        
        if( s_decode_preview_timer_id > 0 )
        {
            stop_timer( s_decode_preview_timer_id );
        }
        //启动softkeyboard, 开始搜索功能
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
*       返回dirlist目录列表
* \param[in]    
* \retval       true:成功; false:失败
*   注意: 
*       仅用于从playing界面返回到dirlist时, 由于之前的dirlist仅hide, 这里做show动作
*******************************************************************************/
static bool _backto_dirlist( menuview_t *menuview, fusion_enter_e fenter )
{
    //隐藏dirlist之前的menu, 与menuview_show成对使用
    menuview_hide( menuview );
    
    dirlist_show( video_vars.dirlist );
    dirlist_paint( video_vars.dirlist );
    
    return true;
}
    
/******************************************************************************/
/*!                    
* \par  Description:
*       创建dirlist, 目录列表
* \param[in]    
* \retval       true:成功; false:失败
*   注意: 
*       进入dirlist时, paint前, 需要将其前面的menu隐藏
*       退出dirlist时, 需要将其前面的menu显示
* fusion_set_first_frame(); fusion_set_last_frame(); fusion_display()必须一起使用, 否则内存泄漏
*******************************************************************************/
bool video_list_dirlist_create( menuview_t *menuview, fusion_enter_e fenter )
{
    bool result;
    fusion_effect_t eff;
    dirlist_init_t init;
    
    /* 设置此界面之前的数据, 作为特效第一帧 */
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
        //设置top限制dv浏览只在dv目录
        init.top_path_name  = video_vars.dv_top_path;
        //使用dv传入的filepath
        init.init_path_name = video_vars.dv_init_path;
    }
    else
    {
        //video正常播放, 从playing返回时, 设置当前播放全路径, 用于高亮
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
    
    //隐藏dirlist之前的menu, 与menuview_show成对使用
    menuview_hide( menuview );
    
    dirlist_paint( video_vars.dirlist );
    
    /* 显示特效 */
    if ( FUSION_FORWARD == fenter )
    {
        /* 设置此界面的数据, 作为特效最后一帧 */
        fusion_set_last_frame( NULL );
        fusion_display( NULL );
    }
    
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

