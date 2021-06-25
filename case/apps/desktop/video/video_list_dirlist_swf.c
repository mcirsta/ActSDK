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

/* include file of this application, 本应用的头文件 */
#include "include_video.h"



/* extern function, 函数声明 */
static void _proc_timer_searching_done( void );
static void *_pthread_deal_searching( void *nouse );

static void _softkeyboard_cb( softkeyboard_t *softkeyboard, softkeyboard_sig_e signal );

static void _dirlist_cb(dirlist_t *dirlist, dirlist_sig_e sig);
static bool _backto_dirlist( menuview_t *menuview, fusion_enter_e fenter );




/* define variable, 变量定义 */

static int s_search_num = -1;


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
        video_nplist_init( video_vars.plist.np_fullpath );
        
        result = video_nplist_get_index_by_fullpath( video_vars.plist.np_fullpath, &video_vars.plist.np_index );
        if( false == result )
        {
            print_err("video_nplist_get_index_by_fullpath false\n");
            video_vars.plist.np_index = -1;
        }
        
        //不能在这里删除dirlist, 否则先退出到main menu, 再进入playing
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
            
            //显示dirlist之前的menu, 与menuview_hide成对使用
            menuview_show( video_vars.menuview );
        }
        
        //不管mainmenu状态, 这里都再进入mainmenu. 
        //从playing回到dirlist再回到mainmenu, 此时mainmenu不存在, 需要创建;
        video_list_mainmenu_enter( video_vars.menuview, FUSION_BACKWARD );
        break;
        
    case WIDGET_SIG_DIRLIST_SHORT_MENU:
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
bool video_list_dirlist_swf_create( menuview_t *menuview, fusion_enter_e fenter )
{
    fusion_effect_t eff;
    dirlist_init_t init;
    
    /* 设置此界面之前的数据, 作为特效第一帧 */
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
    
    //video正常播放, 从playing返回时, 设置当前播放全路径, 用于高亮
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
    
    return true;
}

bool video_list_dirlist_swf_delete( void )
{
    dirlist_delete( video_vars.dirlist );
    video_vars.dirlist = NULL;
    
    return true;
}

