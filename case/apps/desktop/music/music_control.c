/********************************************************************************
 *                              USDK(1100)
 *                             Module: app 
 *                 Copyright(c) 2003-2009 Actions Semiconductor,
 *                            All Rights Reserved. 
 * File : music_control.c
 * History:         
 *      <author>    <time>           <version >             <desc>
 *    songzhining    2009-2-25          v1.0               build this file 
 ********************************************************************************/
/*!
* \file     music_control.c
* \brief    music control operation
* \author   songzhining
*               
* \version 1.0
* \date  2009/02/25
*******************************************************************************/

/*******************************************************************************
*
*                               包含的头文件
*
*************************************************************************/
#include "include_case.h"


#include "music_control.h"
#include "plistclass.h"

/************************************************************************
* 
*                       定义常量
*           
*************************************************************************/


/************************************************************************
* 
*                      添加宏定义
*           
*************************************************************************/
#define     TIMEOUT_VAL       (10)

/*****************************************************************************
* 
*                      全局变量定义
*           
******************************************************************************/
static bool s_open_engine = false;
static void * s_mmm = NULL;
static mmm_img_info_t s_img_info;
static void * s_callback = NULL;
static int s_callback_timer = -1;
static bool s_nowplaying = false;

/************************************************************************
* 
*                      局部函数原型
*           
*************************************************************************/
static bool _send_msg( int type, void *send, int send_size, void *reply, int reply_size )
{
    bool bret;
    msg_apps_t  send_msg;
    msg_reply_t reply_msg;    
    struct timespec  timeout;    

    //get start time
    timeout.tv_sec = TIMEOUT_VAL;
    timeout.tv_nsec = 0;    

    //check size
    if ( (unsigned int)send_size > sizeof(send_msg.content) )
    {
        send_size = sizeof(send_msg.content);
    }
    
    if ( (unsigned int)reply_size > sizeof(reply_msg.content) )
    {
        reply_size = sizeof(reply_msg.content);
    }    
    
    //send msg
    memset(send_msg.content, 0, MSG_CONTENT_SIZE);    
    send_msg.type = type;    
    if ( send != NULL )
    {
        memcpy( send_msg.content, send, (unsigned int)send_size );
    }   

    bret =  send_sync_msg(APP_NAME_MUSIC_ENGINE, &send_msg, &reply_msg,  &timeout);    
    if ( bret == false )
    {
        return false;
    }
    else
    {
        //check reply status
        if ( reply_msg.type == MSG_REPLY_SUCCESS )
        {
            //save reply
            if ( reply != NULL )
            {
                memcpy( reply, reply_msg.content, (unsigned int)reply_size );
            }
            bret = true;
        }
        else
        {
            bret = false;
        }
    }

    return bret;
}

/************************************************************************
* 
*                      全局函数原型
*           
*************************************************************************/

bool music_open_engine( bool waiting )
{
    bool bret = true;
    msg_apps_t m;
    
    if ( s_open_engine == false )
    {
        strncpy(m.content,APP_NAME_MUSIC_ENGINE,MSG_CONTENT_SIZE);
        
        if ( waiting == true )
        {
            m.type = MSG_CREAT_APP_SYNC;
            bret = send_sync_msg(APP_NAME_MANAGER, &m, NULL, 0);    
        }
        else
        {
            m.type = MSG_CREAT_APP;
            bret = send_async_msg(APP_NAME_MANAGER, &m);
        }
        
        if ( bret == true )
        {
            s_open_engine = true;
        }
    }
    
    return bret;
}

bool music_close_engine( void )
{
    bool bret = true;
    msg_apps_t m;
    mengine_t mengine;

    //check engine open
    if ( s_open_engine == true )
    {
        //if is not play, then stop engine
        music_get_engine_info( &mengine );
        
        switch ( mengine.eg_status.play_status )
        {    
        case EG_PAUSE:
        case EG_STOP:
        case EG_ERROR:
            //exit music engine
            #if 1
            m.type = MSG_APP_QUIT;	
            bret = send_sync_msg(APP_NAME_MUSIC_ENGINE, &m, NULL, 0);  
            #else
            m.type = MSG_KILL_APP_SYNC;
            strncpy(m.content,APP_NAME_MUSIC_ENGINE,MSG_CONTENT_SIZE);
            bret = send_sync_msg(APP_NAME_MANAGER, &m, NULL, 0);     
            #endif
            break;
            
        default:
            break;
        }
    }  
    
    return bret;
}

bool music_get_engine_info(mengine_t *engine)
{
    bool bret;
    
    bret = _send_msg( MSG_MENGINE_GET_ENGINE_INFO_SYNC, NULL, 0, 
                                   engine, sizeof(mengine_t) );

    return bret;    
}

bool music_get_filepath(char *filepath, int max_len)
{
    bool bret;
    
    if ( filepath == NULL )
    {
        return false;
    }
    
    bret = _send_msg( MSG_MENGINE_GET_FILEPATH_SYNC, NULL, 0, 
                                   filepath, max_len );

    return bret;    
}

/************************************************************************
* Function:         music_set_eq
* Description:      设置EQ 
* para1：           eq 从该地址开始存放要设置EQ的信息
* Return：          bool   true:  成功   false: 失败
*************************************************************************/
bool  music_set_eq(mengine_eq_t *eq_val)
{
    bool bret;
    
    bret = _send_msg( MSG_MENGINE_SET_EQ_SYNC, eq_val, sizeof(mengine_eq_t), NULL, 0 );
    
    return bret;
}

bool music_set_playmode(play_mode_e playmode)
{
    bool bret;
    
    bret = _send_msg( MSG_MENGINE_SET_PLAYMODE_SYNC, &playmode, sizeof(play_mode_e), NULL, 0 );
    
    return bret;
}

bool music_set_displaymode(display_mode_e displaymode)
{
    bool bret;
    
    bret = _send_msg( MSG_MENGINE_SET_DISPMODE_SYNC, &displaymode, sizeof(display_mode_e), NULL, 0 );

    if ( displaymode == EG_LYRIC )
    {
        sys_forbid_screensaver( true );
    }
    else
    {
        sys_forbid_screensaver( false );
    }
    
    return bret;
}

bool music_set_fade(bool fade)
{
    bool bret;
    
    bret = _send_msg( MSG_MENGINE_SET_FADE_SYNC, &fade, sizeof(bool), NULL, 0 );
    
    return bret;
}

bool music_set_playspeed(int playspeed)
{
    bool bret;
    
    bret = _send_msg( MSG_MENGINE_SET_PLAYSPEED_SYNC, &playspeed, sizeof(int), NULL, 0 );
    
    return bret;
}

bool music_set_ab_auto(bool auto_play)
{
    bool bret;
    
    bret = _send_msg( MSG_MENGINE_SET_AB_AUTO_SYNC, &auto_play, sizeof(bool), NULL, 0 );
    
    return bret;
}

bool music_set_ab_count(int count)
{
    bool bret;
    
    bret = _send_msg( MSG_MENGINE_SET_AB_COUNT_SYNC, &count, sizeof(int), NULL, 0 );
    
    return bret;
}

bool music_set_ab_time(int time)
{
    bool bret;
    
    bret = _send_msg( MSG_MENGINE_SET_AB_TIME_SYNC, &time, sizeof(int), NULL, 0 );
    
    return bret;
}


bool music_start_ab(void)
{
    bool bret;
    
    bret = _send_msg( MSG_MENGINE_START_AB_SYNC, NULL, 0, NULL, 0 );
    
    return bret;
}

bool music_clear_ab(void)
{
    bool bret;
    
    bret = _send_msg( MSG_MENGINE_CLEAR_AB_SYNC, NULL, 0, NULL, 0 );
    
    return bret;
}

bool music_set_ab_next(void)
{
    bool bret;
    
    bret = _send_msg( MSG_MENGINE_SET_AB_NEXT_SYNC, NULL, 0, NULL, 0 );
    
    return bret;
}

bool music_set_ab_prev(void)
{
    bool bret;
    
    bret = _send_msg( MSG_MENGINE_SET_AB_PREV_SYNC, NULL, 0, NULL, 0 );
    
    return bret;
}


bool music_play(void)
{
    bool bret;
    
    bret = _send_msg( MSG_MENGINE_PLAY_SYNC, NULL, 0, NULL, 0 );
    
    return bret;
}

bool music_stop(void)
{
    bool bret;
    
    bret = _send_msg( MSG_MENGINE_STOP_SYNC, NULL, 0, NULL, 0 );
    
    return bret;
}

bool music_pause(void)
{
    bool bret;
    
    bret = _send_msg( MSG_MENGINE_PAUSE_SYNC, NULL, 0, NULL, 0 );
    
    return bret;
}

bool music_resume(void)
{
    bool bret;
    
    bret = _send_msg( MSG_MENGINE_RESUME_SYNC, NULL, 0, NULL, 0 );
    
    return bret;
}

bool music_seek(int ms)
{
    bool bret;
    
    bret = _send_msg( MSG_MENGINE_SEEK_SYNC, &ms, sizeof(int), NULL, 0 );
    
    return bret;
}

bool music_fast_forward(void)
{
    bool bret;
    
    bret = _send_msg( MSG_MENGINE_FFWD_SYNC, NULL, 0, NULL, 0 );
    
    return bret;
}

bool music_fast_backward(void)
{
    bool bret;
    
    bret = _send_msg( MSG_MENGINE_FBWD_SYNC, NULL, 0, NULL, 0 );
    
    return bret;
}

bool music_cancel_ffb(void)
{
    bool bret;
    
    bret = _send_msg( MSG_MENGINE_CANCEL_FFB_SYNC, NULL, 0, NULL, 0 );
    
    return bret;
}

bool music_set_ffb_step(int speed)
{
    bool bret;
    
    bret = _send_msg( MSG_MENGINE_SET_FFBSTEP_SYNC, &speed, sizeof(int), NULL, 0 );
    
    return bret;
}

bool music_set_ffb_time(int time)
{
    bool bret;
    
    bret = _send_msg( MSG_MENGINE_SET_FFBTIME_SYNC, &time, sizeof(int), NULL, 0 );
    
    return bret;
}

bool music_set_fileindex(int fileindex)
{
    bool bret;
    
    bret = _send_msg( MSG_MENGINE_SET_FILEINDEX_SYNC, &fileindex, sizeof(int), NULL, 0 );
    
    return bret;
}

bool music_set_filepath(char *filepath)
{
    bool bret;

    if ( filepath == NULL )
    {
        return false;
    }
    
    bret = _send_msg( MSG_MENGINE_SET_FILEPATH_SYNC, filepath, (int)(strlen(filepath)+1), NULL, 0 );
    
    return bret;
}

bool music_play_next(void)
{
    bool bret;
    
    bret = _send_msg( MSG_MENGINE_PLAY_NEXT_SYNC, NULL, 0, NULL, 0 );
    
    return bret;
}

bool music_play_prev(void)
{
    bool bret;
    
    bret = _send_msg( MSG_MENGINE_PLAY_PREV_SYNC, NULL, 0, NULL, 0 );
    
    return bret;
}


bool  music_make_bookmark(eg_audio_tag_t *audio_tag)
{
    bool bret;
    
    bret = _send_msg( MSG_MENGINE_MAKE_BOOKMARK_SYNC, NULL, 0, 
                                   audio_tag, sizeof(eg_audio_tag_t) );
    return bret;    
}

bool music_set_last_file( void )
{
    bool bret;
    
    bret = _send_msg( MSG_MENGINE_SET_LAST_FILE_SYNC, NULL, 0, NULL, 0 );
    
    return bret;    
}

bool music_delete_file(char * fullpath)
{
    bool bret;
    FSEL_HANDLE p_fsel;

    //delete file
    bret = _send_msg( MSG_MENGINE_DELETE_FILE_SYNC, NULL, 0, NULL, 0 );    

    //sync file
    if ( bret == true )
    {
        if ( g_dirlist != NULL )
        {
            p_fsel = dirlist_get_fsel_handle( g_dirlist );
            
            if ( p_fsel == NULL )
            {
                print_err("fsel_open failed.\n");
            }
            else
            {
                fsel_update(p_fsel, (char*) fullpath, FSEL_DELDATA);
            }        
        }
        
        //sync playlist
        plistclass_sync_to_fs(fullpath);
    }
    else
    {
        print_err("delete file fail!");
    }

    return bret;
}


/* album image decode */
bool album_open_engine( void )
{
    if(NULL == s_mmm)
    {
        s_mmm = mmm_id_open(NULL);
        if (s_mmm == NULL)
        {
            print_err("open mmm err!\n");
            return false;
        }
    }
    
    return true;
}

bool album_close_engine( void )
{
    if(NULL != s_mmm)
    {
        mmm_id_close(s_mmm, NULL);
        s_mmm = NULL;
    }
    
    return true;
}

bool album_get_status( mmm_image_status_t *status )
{
    int result;
    
    if( NULL != s_mmm )
    {
        result = mmm_id_cmd( s_mmm, MMM_GET_DECODER_STATUS, (unsigned int)status );        
        if( result != 0 )
        {
            print_err("album_get_status failed!");
            return false;
        }
        
        return true;
    }
    else
    {
        return false;
    }
    
}

bool album_get_image_info( mmm_img_info_t *img_info )
{
    (*img_info) = s_img_info;
    
    return true;
}

bool album_check_file( plist_detail_info_t *id3_info )
{
    bool bret;
    char folder[PATH_MAX];
    int result;
    struct stat buf;
    char *last_slash;
    
    if ( id3_info == NULL )
    {
        return false; 
    }

    /* 专辑图片类型判断*/
    if ( id3_info->imageinfo.len > 0 )
    {
        //内嵌专辑图片
        bret = true; 
    }
/*    
    else if ( id3_info->imageinfo.exist_folderjpg == true )
    {
        //目录专辑图片
        bret = true;          
    }
*/    
    else if ( id3_info->imageinfo.mtp_img_path != NULL )
    {
        //MTP专辑图片
        bret = true; 
    }
    else
    {     
        strncpy( folder, id3_info->fullpath, sizeof(folder) );  

        /* Find last '/'.  */
        last_slash = strrchr(folder, '/'); 
        if( last_slash != NULL )
        {
            last_slash ++;
            strcpy( last_slash, "folder.jpg" );
        }
        else
        {
            strncpy( folder, "folder.jpg", sizeof(folder) );
        }        

        //check if folder album exist
        result = stat( folder, &buf );   
        if( result != 0 )
        {
            bret = false; 
        }
        else
        {
            bret = true; 
        }
        
        id3_info->imageinfo.exist_folderjpg = (int)bret;  
    }
    
    return bret;
}

static void album_callback( void )
{
    mmm_image_status_t status;

    if ( true == album_get_status( &status ) )
    {
        if( MMM_IMAGE_DECODER_FREE == status.status ) /* 解码完成 */
        {
            s_callback_timer = set_single_shot_timer( 1, (timer_proc)s_callback, NULL );
        }
        else if( MMM_IMAGE_DECODER_ERROR == status.status )
        {
            print_err();
        }
        else
        {
        }

        if( MMM_IMAGE_DECODER_BUSY != status.status )
        {
            print_dbg();
        }
    }
}

bool album_start_decode( plist_detail_info_t *id3_info, gui_bitmap_info_t *bmp, void (*callback)() )
{
    int result;
    mmm_album_info_t album_info;
    char *filename;
    char folder[PATH_MAX];
    char *last_slash;

    //check engine open
    if( NULL == s_mmm )
    {
        album_open_engine();
    }
    
    if( ( NULL == s_mmm ) || ( NULL == id3_info ) || ( NULL == bmp ) )
    {
        return false;
    }
    else
    {
        //set callback
        s_callback = (void *)callback;
        result = mmm_id_cmd( s_mmm, MMM_SET_CALLBACK, (unsigned int)album_callback );        
        if( result != 0 )
        {
            print_err("set callback failed!");
            return false;
        }
        
        //init output format
        s_img_info.bpp = bmp->bytesperpixel;
        s_img_info.formate = MMM_RGB;
        s_img_info.rect_width = bmp->xsize;
        s_img_info.rect_height = bmp->ysize;
        s_img_info.img_width = 0;
        s_img_info.img_height = 0;
        s_img_info.buf = (unsigned char *)bmp->pdata;
        s_img_info.len = bmp->xsize * bmp->ysize * bmp->bytesperpixel;        
    }

    /* 专辑图片类型判断*/
    if ( id3_info->imageinfo.len > 0 )
    {
        //内嵌专辑图片
        album_info.file_name = id3_info->fullpath;
        album_info.data_offset = id3_info->imageinfo.offset;
        album_info.data_len = id3_info->imageinfo.len;
        
        //set album
        result = mmm_id_cmd( s_mmm, MMM_SET_ALBUM_INFO, (unsigned int)&album_info );        
        if( result != 0 )
        {
            print_err("set album failed!");
            return false;
        }   
    }
    else if ( id3_info->imageinfo.exist_folderjpg == true )
    {
        strncpy( folder, id3_info->fullpath, sizeof(folder) );  

        /* Find last '/'.  */
        last_slash = strrchr(folder, '/'); 
        if( last_slash != NULL )
        {
            last_slash ++;
            strcpy( last_slash, "folder.jpg" );
        }
        else
        {
            strncpy( folder, "folder.jpg", sizeof(folder) );
        }

        //set file
        result = mmm_id_cmd( s_mmm, MMM_SET_FILE, (unsigned int)folder );        
        if( result != 0 )
        {
            print_err("set file failed!");
            return false;
        }
    }
    else if ( id3_info->imageinfo.mtp_img_path != NULL )
    {
        //MTP专辑图片
        filename = id3_info->imageinfo.mtp_img_path;
        
        //set file
        result = mmm_id_cmd( s_mmm, MMM_SET_FILE, (unsigned int)filename );        
        if( result != 0 )
        {
            print_err("set file failed!");
            return false;
        }   
    }
    else
    {
        return false;
    }

    //decode album
    result = mmm_id_cmd( s_mmm, MMM_IMG_BROWSE, (unsigned int)&s_img_info );        
    if ( result != 0 )
    {
        print_err("decode album failed!");
        return false;
    }  
    else
    {
        //set output image size
        //bmp->xsize = img_info.img_width;
        //bmp->ysize = img_info.img_height;
    }

    return true;
}

bool album_abort_decode( void )
{
    int result;

    if( NULL != s_mmm )
    {
        result = mmm_id_cmd( s_mmm, MMM_ABORT_DECODE, 0 );
        if ( result != 0 )
        {
            print_err("abort decode err!\n");
            return false;
        }
        
        //stop timer
        kill_timer( s_callback_timer );
        s_callback_timer = -1;

        return true;
    }
    else
    {
        return false;
    }
    
}


bool music_create_thread( void *proc, void *param )
{
    pthread_t tid;
    pthread_attr_t pthread_attr;
    struct sched_param self_param;
    int policy;
    int result = -1;

    /*
     * process manger's msg loop should be activated as long as there is
     * a msg in the process manager's msg queue
     */
    pthread_attr_init(&pthread_attr);
    pthread_attr_setinheritsched(&pthread_attr, PTHREAD_EXPLICIT_SCHED);
    pthread_attr_setdetachstate(&pthread_attr, PTHREAD_CREATE_DETACHED);
    pthread_attr_setscope(&pthread_attr, PTHREAD_SCOPE_PROCESS);
    
    //创建比主进程低一级的线程, 保证主进程先运行
    pthread_getschedparam( pthread_self(), &policy, &self_param );
    self_param.sched_priority = self_param.sched_priority - 2;
    pthread_attr_setschedparam(&pthread_attr, &self_param);
    
    print_dbg(" manager_msg_loop  thread's priority is: %d\n", self_param.sched_priority );

    result = pthread_create( &tid, &pthread_attr, proc, param );
    pthread_attr_destroy( &pthread_attr );
    if( result != 0 )
    {
        print_err("can’t create thread, result:%d\n", result);
        return false;
    }
    
    return true;
}

bool music_keymap_control( key_val_e key_val, bool is_map )
{
    bool bret;
    msg_apps_t m;
    key_map_t key_map;

    key_map.enable = is_map;
    key_map.key_val_src = key_val;
    key_map.key_type_src = 0;
    key_map.key_val_dest = 0;
    key_map.key_type_dest = 0;

    m.type = MSG_SET_KEY_MAPPING_SYNC;    
    memcpy( m.content, &key_map, sizeof(key_map_t) );
    
    bret = send_sync_msg(APP_NAME_MSG_SERV, &m, NULL, 0);    

    return bret;
}


/*
 * 设置当前场景全部区域作为特效的第一帧 */
void scene_fusion_set_first_frame( scene_t *scene )
{
    fusion_frame_t frame;

    frame.rect.x = scene->x;
    frame.rect.y = scene->y;
    frame.rect.width = scene->width;
    frame.rect.height = scene->height;
    frame.pdata = NULL;
    frame.alpha = false;

    fusion_set_first_frame(&frame);
}

/*
 * 设置当前场景全部区域作为特效的最后一帧
 */
void scene_fusion_set_last_frame( scene_t *scene )
{
    fusion_frame_t frame;

    frame.rect.x = scene->x;
    frame.rect.y = scene->y;
    frame.rect.width = scene->width;
    frame.rect.height = scene->height;
    frame.pdata = NULL;
    frame.alpha = false;

    fusion_set_last_frame(&frame);
}

/*
 * 菜单特效第一帧 */
void option_fusion_set_first_frame( menuview_t *mv )
{
    fusion_frame_t frame;
    canvas_info_t info;

    canvas_get_info(mv->canvas_id, &info);

    frame.rect.x = info.rect.x;
    frame.rect.y = info.rect.y;
    frame.rect.width = info.rect.width;
    frame.rect.height = info.rect.height;
    frame.pdata = NULL;
    frame.alpha = false;
    
    fusion_set_first_frame(&frame);
}

/*
 * 菜单特效最后一帧
 */
void option_fusion_set_last_frame( menuview_t *mv )
{
    fusion_frame_t frame;
    canvas_info_t info;

    canvas_get_info(mv->canvas_id, &info);

    frame.rect.x = info.rect.x;
    frame.rect.y = info.rect.y;
    frame.rect.width = info.rect.width;
    frame.rect.height = info.rect.height;
    frame.pdata = (unsigned char *)info.pdata;
    
    if( info.mode == TRANSPARENT )
    {
        frame.alpha = true;
    }
    else
    {
        frame.alpha = false;
    }
    
    fusion_set_last_frame(&frame);
}

bool music_get_nowplaying( void )
{
    int result;
    bool nowplaying = false;
    
    //load nowplaying flag
    result = get_config( CFG_MUSIC_NOWPLAYING, (char*)&nowplaying, sizeof(bool) );
    if( result < 0 )
    {
        nowplaying = false;
    }
    else
    {
        s_nowplaying = nowplaying;
    }

    return s_nowplaying;
}

bool music_set_nowplaying( bool nowplaying )
{
    int result;
    bool bret = true;

    //load nowplaying flag    
    if( nowplaying != s_nowplaying )
    {
        result = set_config( CFG_MUSIC_NOWPLAYING, (char*)&nowplaying, sizeof(bool) );
        if( result < 0 )
        {
            bret = false;
        }
        else
        {
            s_nowplaying = nowplaying;
        }
    }

    return bret;
}

