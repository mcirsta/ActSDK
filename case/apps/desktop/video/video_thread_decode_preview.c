/*******************************************************************************
 *                              NOYA1100
 *                            Module: video
 *                 Copyright(c) 2003-2008 Actions Semiconductor,
 *                            All Rights Reserved. 
 * History:         
 *      <author>    <time>           <version >             <desc>
 *      shaofeng    2009-06-19          1.0             build this file 
*******************************************************************************/
/*!
 * \file     video_thread_decode_preview.c
 * \brief    
 * \author   shaofeng
 * \par      GENERAL DESCRIPTION:
 *               
 * \par      EXTERNALIZED FUNCTIONS:
 *               
 * \version 1.0
 * \date  2009/06/19
*******************************************************************************/

#include "include_psp.h"
#include "include_case.h"

/* include file of this application, 本应用的头文件 */
#include "include_video.h"




/* define variable, 变量定义 */
static pthread_t s_pthread;

/* 需要解码线程退出时通过此变量退出 */
static bool s_pthread_break_flag = false;


#if 0
#endif
/******************************************************************************/
/*!                    
* \par  Description:
*      获取fullpath文件的time位置的 preview数据
*      此过程由mmm的几个命令组合实现. 在几个命令中间可以通过stop标志退出此函数, 进而退出线程
* \param[in]    char *: preview文件的全路径
* \param[in]    int: tag的time数据, 单位ms
* \param[out]   mmm_cur_video_t *: preview数据指针
* \retval       bool
*******************************************************************************/
bool video_thread_get_one_preview( char *fullpath, unsigned int time, mmm_cur_video_t *preview_info )
{
    bool result = true;
    
    if ( (NULL == fullpath) || (NULL == preview_info) || ( NULL == preview_info->buf ) )
    {
        return false;
    }
    
    //SET_FILE前防止处于err状态, 需要先stop
    video_control( V_CMD_STOP, NULL );
    result = video_control( V_CMD_SET_FILE, fullpath );
    if( (false == result) || (true == s_pthread_break_flag) )
    {
        goto VIDEO_THREAD_GET_ONE_PREVIEW_ERR;
    }
    
    //如果time不为0, 需要seek到需要解缩略图的播放点
    if ( time > 0)
    {
        result = video_control( V_CMD_SEEK, &time );
        if( (false == result) || (true == s_pthread_break_flag) )
        {
            goto VIDEO_THREAD_GET_ONE_PREVIEW_ERR;
        }
    }

    //获取一帧图片，注意在调用该接口时不要打开video区域，如果打开，
    //该帧图片会直接显示到屏幕上。
    result = video_control( V_CMD_SHOW_NEW_FRAME, 0 );
    if( (false == result) || (true == s_pthread_break_flag) )
    {
        goto VIDEO_THREAD_GET_ONE_PREVIEW_ERR;
    }
     
    //获取缩略图
    result = video_control( V_CMD_PRINT_SCREEN, preview_info );
    
    VIDEO_THREAD_GET_ONE_PREVIEW_ERR:
    video_control( V_CMD_STOP, NULL );
    if ( true == s_pthread_break_flag )
    {
        result = false;
    }
    
    return result;
}


/******************************************************************************/
/*!                    
* \par  Description:
*      设置线程结束标志. 通过设置结束标志变量, 使线程运行中间检测到此标志后立即退出
* \retval       bool
*******************************************************************************/
bool video_thread_decode_preview_stop( void )
{
    s_pthread_break_flag = true;
    
    return true;
}


/******************************************************************************/
/*!                    
* \par  Description:
*      创建join型线程, 用于缩略图解码
* \retval       bool
*   注意: 
*       创建的线程比主进程低一级的线程, 保证主进程先运行
*       调用此函数前先video_thread_decode_preview_delete, 保证线程退出再创建
*******************************************************************************/
bool video_thread_decode_preview_create( void *(*start_rtn)(void*) )
{
    int result = -1;
    pthread_attr_t pthread_attr;
    struct sched_param param;
//    struct sched_param self_param;
    int policy;
    
    s_pthread_break_flag = false;

    //创建比主进程低一级的线程, 保证主进程先运行
    pthread_attr_init(&pthread_attr);
    pthread_attr_setinheritsched(&pthread_attr, PTHREAD_EXPLICIT_SCHED);
    pthread_attr_setdetachstate(&pthread_attr, PTHREAD_CREATE_JOINABLE);
    pthread_attr_setscope(&pthread_attr, PTHREAD_SCOPE_PROCESS);
    
    //创建比主进程低一级的线程, 保证主进程先运行
    pthread_getschedparam( pthread_self(), &policy, &param );
    param.sched_priority = param.sched_priority - 2;
    pthread_attr_setschedparam(&pthread_attr, &param);
    print_dbg(" manager_msg_loop  thread's priority is: %d\n", param.sched_priority );

    result = pthread_create( &s_pthread, &pthread_attr, start_rtn, NULL );
    pthread_attr_destroy( &pthread_attr );
    if( result != 0 )
    {
        print_err("can’t create thread, result:%d\n", result);
        return false;
    }
    return true;
}

/******************************************************************************/
/*!                    
* \par  Description:
*      删除join型线程
* \retval       bool
*   注意: 
*       对创建的join型线程, 此函数会等待, 直到目标thread的终止
*******************************************************************************/
bool video_thread_decode_preview_delete( void )
{
    if ( (0 != s_pthread.struct_id.pid) && (0 != s_pthread.struct_id.tid) )
    {
        pthread_join( s_pthread, NULL );
    }
    memset( &s_pthread, 0x00, sizeof(s_pthread) );
    
    return true;
}

