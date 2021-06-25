/************************************************************************
*                             USDK1100                                
*                            music engin                              
*                                                                      
*                (c) Copyright 2003-2015, Actions Co,Ld.              
*                          All Right Reserved                         
*                 
*
* File: mengin_main.c                                               
* By  : JerryFu                                                       
* Version: 1> v1.00     first version     2009-03-10                   
************************************************************************/

/************************************************************************
*                                                                  
*                           head    file                              
*                                                                     
************************************************************************/
#include "include_case.h"

/************************************************************************
*                                                                     
*                       const  defination                            
*                                                                     
************************************************************************/

/************************************************************************
*                                                                     
*                      micro    defination                           
*                                                                   
************************************************************************/
#define    MAX_PLAY_SPEED    (8)
#define    MIN_PLAY_SPEED    (-8)
#define    MAX_FFB_STEP      (4)
#define    MAX_EQ_NUM        (8)

#define    STATUS_INTERVAL     500
#define    INTRO_INTERVAL     (10000+800)

/************************************************************************
*                                                                   
*                      global  variable                             
*                                                                    
************************************************************************/

static int s_uni_usereq[MAX_EQ_NUM][10] = 
{
    {12, 12, 12, 12, 12, 12, 12, 12, 1, 0}, //normal:
    {17, 15, 12, 11, 12, 16, 17, 14, 1, 0}, //rock:
    {15, 12, 12, 12, 12, 11, 10,  8, 1, 0}, //pop:
    {12, 12, 12, 11, 11, 10,  8,  0, 1, 0}, //classic
    {12, 12, 13, 14, 12, 13, 13,  8, 1, 0}, //soft:
    {15, 12, 12, 11, 12, 14, 16, 10, 1, 0}, //jaZZ:
    {17, 14, 12, 12, 12, 12, 12, 13, 1, 0}, //DBB
    {12, 12, 12, 12, 12, 12, 12, 24, 1, 0}, //USER
};

static int s_eq_srswow[10] =
{ 8, 8, 4, 5, 0, 1, 0, 0, 0, 0 };

static int s_eq_srswow_hd[10] =
{ 8, 8, 5, 4, 6, 1, 0, 0, 0, 0 };

static void *s_ad_handle = NULL;
static int s_timer_id = -1;  //timer id for play timing
static mengine_t s_mengine = {0};
static char s_filepath[PATH_MAX] = {0};    //文件路径(保存配置)
static bool s_auto_stopped = false; //自动播放停止标志
static bool s_setfile = false; //设置文件标志
static int s_delay_timer_id = -1;  //timer id for play delay process
static loop_switch_e s_smode = LOOP_SWITCH_NORMAL; //default: auto
static loop_direct_e s_dir = LOOP_DIRECT_NEXT; //default: next
static bool s_has_played = false; //已播放文件标志
static int s_intro_start = 0;  //intro开始时间

/************************************************************************
*                                                                    
*                 add  function here                                           
*                                                                   
************************************************************************/

/*************************************************************************
* Function:            _reply_msg                                       
* Description:        发送回执                                       
* para1：            send      发送内容                                        
* para1：            size       发送大小                                    
* para1：            success  成功/失败                                        
* Return：           none                                              
*************************************************************************/
static bool _reply_msg( void *send, int size, bool success )
{
    bool bret;
    msg_reply_t t;

    /*  发送应答，回执为当前音乐的播放信息
    这个结构体中记录了当前断点的位置，
    供下次恢复播放时使用
    */
    if ( success == true )
    {
        t.type = MSG_REPLY_SUCCESS;
    }
    else
    {
        t.type = MSG_REPLY_FAILED;
    }
    
    if ( ( send != NULL ) && ( size > 0 ) )
    {
        if ( size > (int)sizeof(t.content) )
        {
            size = sizeof(t.content);
        }
    
        memcpy( t.content, send, (unsigned int)size );
    }    
    
    bret = reply_sync_msg(&t);
    
    return bret;
}

static bool _convert_eq_value( mengine_eq_t *eq_val, mmm_eq_set_t *eq_set )
{
    int idx;
    
    switch(eq_val->eq_mode)
    {
    case EQ_USR_MODE:
        eq_set->eq_mode = MMM_EQ_USR_MODE;
        memcpy(eq_set->param, s_uni_usereq[EQ_USR_MODE], sizeof(eq_set->param));
        //convert user eq
        eq_set->param[0] = ( eq_val->user_param[0] + 6 ) * 2; // -6 ~ +6 --> 0 ~ 24
        eq_set->param[1] = ( eq_val->user_param[1] + 6 ) * 2; // -6 ~ +6 --> 0 ~ 24
        eq_set->param[3] = ( eq_val->user_param[2] + 6 ) * 2; // -6 ~ +6 --> 0 ~ 24
        eq_set->param[4] = ( eq_val->user_param[3] + 6 ) * 2; // -6 ~ +6 --> 0 ~ 24
        eq_set->param[5] = ( eq_val->user_param[4] + 6 ) * 2; // -6 ~ +6 --> 0 ~ 24
        break;
        
    case EQ_SRSWOW:
        eq_set->eq_mode = MMM_SRS_USR_MODE;
        //memcpy(eq_val->srs_param, s_eq_srswow, sizeof(eq_val->srs_param));  
        memcpy(eq_set->param, s_eq_srswow, sizeof(eq_set->param));           
        break;
        
    case EQ_SRSWOW_HD:
        eq_set->eq_mode = MMM_SRS_USR_MODE;
        //memcpy(eq_val->srs_param, s_eq_srswow_hd, sizeof(eq_val->srs_param));  
        memcpy(eq_set->param, s_eq_srswow_hd, sizeof(eq_set->param));    
        break;
        
    case EQ_SRSUSR_MODE:
        eq_set->eq_mode = MMM_SRS_USR_MODE;
        memcpy(eq_set->param, eq_val->srs_param, sizeof(eq_set->param));    
        break;
        
    default:
        eq_set->eq_mode = MMM_EQ_USR_MODE;
        idx = ( eq_val->eq_mode % MAX_EQ_NUM );
        memcpy(eq_set->param, s_uni_usereq[idx], sizeof(eq_set->param));
        break;
    }

    return true;
}

static bool _set_playmode( play_mode_e playmode, int current, int total )
{
    loop_mode_e loopmode;
    
    switch( playmode )
    {
    case EG_INTRO:
        loopmode = LOOP_MODE_SEQUENCE;      
        break;
        
    default:
        loopmode = ( loop_mode_e )playmode;    
        break;
    }    
    
    return loop_set_mode( loopmode, current, total );
}

static bool _check_playing( void )
{
    bool is_playing;
    
    switch( s_mengine.eg_status.play_status )
    {
    case EG_PLAY:
    case EG_PLAY_AB:
    case EG_PLAY_FAST:  
        is_playing = true;   
        break;
        
    default: 
        is_playing = false;    
        break;
    }    
    
    return is_playing;
}

static bool _remove_playing_item( void )
{
    bool bret = true;
    bool result = true;
    mengine_config_t *cfg_ptr = &s_mengine.config;  
    int i;
    int index_cur;
    
    //clear tag
    memset( &cfg_ptr->audio_tag, 0, sizeof(eg_audio_tag_t) ); 

    //delete items
    for ( i=0; i<cfg_ptr->file_total; i++ )
    {
        index_cur = nplist_get_index_by_fullpath( s_filepath );
        
        //playlist delete file
        if ( index_cur >= 0 )
        {
            //delete nplist item
            bret = nplist_del_item_by_index( index_cur );
            if( bret == false )
            {
                //fsel mode: 文件不支持
                if ( i == 0 ) //无法删除就跳到下一首
                {
                    print_err("cannot delete item!");
                }
                break;
            }
            
            //更新循环模式
            cfg_ptr->file_index = loop_delete_index( index_cur, s_smode, s_dir );
            if( cfg_ptr->file_index < 0 )   //need to stop
            {
                cfg_ptr->file_index = loop_get_current();
                result = false;
            }
        }
        else //fsel delete file
        {  
            if ( i == 0 ) //无法删除就跳到下一首
            {
                //更新循环模式
                cfg_ptr->file_index = loop_delete_index( cfg_ptr->file_index, s_smode, s_dir );
                if( cfg_ptr->file_index < 0 )   //need to stop
                {
                    cfg_ptr->file_index = loop_get_current();
                    result = false;
                }
            }
            break;
        }
    }

    //更新文件总数
    cfg_ptr->file_total = nplist_get_item_num();
    if ( cfg_ptr->file_total <= 0 )
    {
        result = false;
    }

    //更新文件索引
    if( cfg_ptr->file_index > ( cfg_ptr->file_total - 1 ) )
    {
        cfg_ptr->file_index = cfg_ptr->file_total - 1; 
    }
    
    return result;
}

static bool _load_config( void )
{
    int result;
    mengine_config_t *cfg_ptr = &s_mengine.config;
    
    //load display mode
    result = get_config( CFG_MUSIC_DISPLAY_MODE, (char*)&cfg_ptr->display_mode, sizeof(display_mode_e) );
    if( result < 0 )
    {
        cfg_ptr->display_mode = EG_SPECTRUM;
    }
    
    //load display effect
    result = get_config( CFG_MUSIC_DISPLAY_EFFECT, (char*)&cfg_ptr->display_effect, sizeof(bool) );
    if( result < 0 )
    {
        cfg_ptr->display_effect = 0;
    }
    
    //load play mode
    result = get_config( CFG_MUSIC_PLAY_MODE, (char*)&cfg_ptr->play_mode, sizeof(play_mode_e) );
    if( result < 0 )
    {
        cfg_ptr->play_mode = EG_SEQUENCE;
    }

    //load eq mode
    result = get_config( CFG_MUSIC_EQ_MODE, (char*)&cfg_ptr->eq_val.eq_mode, sizeof(eq_mode_e) );
    if( result < 0 )
    {
        cfg_ptr->eq_val.eq_mode = EQ_NORMAL;
    }

    //load usereq param
    result = get_config( CFG_MUSIC_USEREQ_PARAM, (char*)cfg_ptr->eq_val.user_param, 
                            sizeof(cfg_ptr->eq_val.user_param) );
    if( result < 0 )
    {
        memset( cfg_ptr->eq_val.user_param, 0, sizeof(cfg_ptr->eq_val.user_param));
    }

    //load srseq param
    result = get_config( CFG_MUSIC_SRSEQ_PARAM, (char*)cfg_ptr->eq_val.srs_param, 
                            sizeof(cfg_ptr->eq_val.srs_param) );
    if( result < 0 )
    {
        memcpy( cfg_ptr->eq_val.srs_param, s_eq_srswow, sizeof(cfg_ptr->eq_val.srs_param));
    }

    //load fade mode
    result = get_config( CFG_MUSIC_FADE, (char*)&cfg_ptr->fade, sizeof(bool) );
    if( result < 0 )
    {
        cfg_ptr->fade = true;
    }

    //load play speed
    result = get_config( CFG_MUSIC_PLAY_SPEED, (char*)&cfg_ptr->play_speed, sizeof(int) );
    if( result < 0 )
    {
        cfg_ptr->play_speed = 0;
    }

    //load ffb speed
    result = get_config( CFG_MUSIC_FFB_SPEED, (char*)&cfg_ptr->ffb_speed, sizeof(int) );
    if( result < 0 )
    {
        cfg_ptr->ffb_speed = 0;
    }

    //load ffb time
    result = get_config( CFG_MUSIC_FFB_TIME, (char*)&cfg_ptr->ffb_time, sizeof(int) );
    if( result < 0 )
    {
        cfg_ptr->ffb_time = 0;
    }

    //load ab count
    result = get_config( CFG_MUSIC_AB_COUNT, (char*)&cfg_ptr->ab_count, sizeof(int) );
    if( result < 0 )
    {
        cfg_ptr->ab_count = 1;
    }

    //load ab time
    result = get_config( CFG_MUSIC_AB_TIME, (char*)&cfg_ptr->ab_time, sizeof(int) );
    if( result < 0 )
    {
        cfg_ptr->ab_time = 2;
    }

    //load ab autoplay
    result = get_config( CFG_MUSIC_AB_AUTOPLAY, (char*)&cfg_ptr->ab_autoplay, sizeof(bool) );
    if( result < 0 )
    {
        cfg_ptr->ab_autoplay = 1;
    }

    //load file total
    result = get_config( CFG_MUSIC_FILE_TOTAL, (char*)&cfg_ptr->file_total, sizeof(int) );
    if( result < 0 )
    {
        cfg_ptr->file_total = 0;
    }

    //load file index
    result = get_config( CFG_MUSIC_FILE_INDEX, (char*)&cfg_ptr->file_index, sizeof(int) );
    if( result < 0 )
    {
        cfg_ptr->file_index = 0;
    }

    //load file path
    result = get_config( CFG_MUSIC_FILE_PATH, (char*)s_filepath, sizeof(s_filepath) );
    if( result < 0 )
    {
        memset( s_filepath, 0, sizeof(s_filepath) );
    }

    //load tag info
    result = get_config( CFG_MUSIC_TAG_INFO, (char*)&cfg_ptr->audio_tag, sizeof(eg_audio_tag_t) );
    if( result < 0 )
    {
        cfg_ptr->audio_tag.time = 0;
        memset( cfg_ptr->audio_tag.buf, 0, sizeof(cfg_ptr->audio_tag.buf) );
    }
    
    //load fsel mode
    result = get_config( CFG_MUSIC_FSEL_MODE, (char*)&cfg_ptr->fsel_mode, sizeof(bool) );
    if( result < 0 )
    {
        cfg_ptr->fsel_mode = 0;
    }    

    return true;
}

static bool _save_config( void )
{
    int result;
    mengine_config_t *cfg_ptr = &s_mengine.config;

    //save display mode
    result = set_config( CFG_MUSIC_DISPLAY_MODE, (char*)&cfg_ptr->display_mode, sizeof(display_mode_e) );
    
    //save play mode
    result = set_config( CFG_MUSIC_PLAY_MODE, (char*)&cfg_ptr->play_mode, sizeof(play_mode_e) );

    //save eq mode
    result = set_config( CFG_MUSIC_EQ_MODE, (char*)&cfg_ptr->eq_val.eq_mode, sizeof(eq_mode_e) );

    //save usereq param
    result = set_config( CFG_MUSIC_USEREQ_PARAM, (char*)cfg_ptr->eq_val.user_param, 
                            sizeof(cfg_ptr->eq_val.user_param) );

    //save srseq param
    result = set_config( CFG_MUSIC_SRSEQ_PARAM, (char*)cfg_ptr->eq_val.srs_param, 
                            sizeof(cfg_ptr->eq_val.srs_param) );

    //save fade mode
    result = set_config( CFG_MUSIC_FADE, (char*)&cfg_ptr->fade, sizeof(bool) );

    //save play speed
    result = set_config( CFG_MUSIC_PLAY_SPEED, (char*)&cfg_ptr->play_speed, sizeof(int) );

    //save ffb speed
    result = set_config( CFG_MUSIC_FFB_SPEED, (char*)&cfg_ptr->ffb_speed, sizeof(int) );

    //save ffb time
    result = set_config( CFG_MUSIC_FFB_TIME, (char*)&cfg_ptr->ffb_time, sizeof(int) );

    //save ab count
    result = set_config( CFG_MUSIC_AB_COUNT, (char*)&cfg_ptr->ab_count, sizeof(int) );

    //save ab time
    result = set_config( CFG_MUSIC_AB_TIME, (char*)&cfg_ptr->ab_time, sizeof(int) );

    //save ab autoplay
    result = set_config( CFG_MUSIC_AB_AUTOPLAY, (char*)&cfg_ptr->ab_autoplay, sizeof(bool) );

    //save file total
    result = set_config( CFG_MUSIC_FILE_TOTAL, (char*)&cfg_ptr->file_total, sizeof(int) );

    //save file index
    result = set_config( CFG_MUSIC_FILE_INDEX, (char*)&cfg_ptr->file_index, sizeof(int) );

    //save file path
    result = set_config( CFG_MUSIC_FILE_PATH, (char*)s_filepath, sizeof(s_filepath) );

    //save tag info
    result = set_config( CFG_MUSIC_TAG_INFO, (char*)&cfg_ptr->audio_tag, sizeof(eg_audio_tag_t) );

    //save fsel mode
    result = set_config( CFG_MUSIC_FSEL_MODE, (char*)&cfg_ptr->fsel_mode, sizeof(bool) );

    return true;
}

void _status_timer_event( void *param )
{
    mengine_update();
}

static bool _update_file( bool tag_flag )
{
    bool bret = true;
    mengine_config_t *cfg_ptr = &s_mengine.config;
    plist_detail_info_t *detail_info;

    //更新文件总数
    cfg_ptr->file_total = nplist_get_item_num();
    if ( cfg_ptr->file_total <= 0 )
    {
        print_dbg("file total <= 0");
        return false;
    }
    
    //更新文件索引
    if ( cfg_ptr->file_index > ( cfg_ptr->file_total - 1 ) )
    {
        cfg_ptr->file_index = cfg_ptr->file_total - 1;
        
        //更新循环模式
        _set_playmode(cfg_ptr->play_mode, cfg_ptr->file_index, cfg_ptr->file_total);
    }    

    //更新文件路径
    detail_info = nplist_get_item_by_index( cfg_ptr->file_index );
    if ( detail_info->fullpath != NULL )
    {   
        strcpy( s_filepath, detail_info->fullpath );
    }
    else
    {   
        strcpy( s_filepath, "" );
        
    }
    
    //更新tag信息
    if ( ( tag_flag == true ) && ( detail_info->markinfo.mark_param != NULL ) )
    {
        cfg_ptr->audio_tag.time = detail_info->markinfo.time;
        memcpy( cfg_ptr->audio_tag.buf, detail_info->markinfo.mark_param, detail_info->markinfo.mark_param_len );
    }
    else
    {
        cfg_ptr->audio_tag.time = 0;
        memset( cfg_ptr->audio_tag.buf, 0, sizeof(cfg_ptr->audio_tag.buf) );
    }
    
    return bret;
}

static bool _set_file( char *fullpath )
{
    bool bret = true;
    int result = 0;

    //set file
    result = mmm_ad_cmd( s_ad_handle, MMM_SET_FILE, (unsigned int)fullpath );
    if( result != 0 )
    {
        print_err("can't set play file\n");
        bret =  false;
    }
    else
    {
        bret =  true;
        s_setfile = true;
        s_auto_stopped = false;
        s_mengine.eg_status.play_status = EG_STOP;
        s_intro_start = 0;

        //get file info
        result = mmm_ad_cmd( s_ad_handle, MMM_GET_MEDIA_INFO, (unsigned int)&s_mengine.file_info );
        if( result < 0 )
        {
            print_err("can't get file info\n");
            bret =  false;
        }
    }
    
    result = mmm_ad_cmd( s_ad_handle, MMM_GET_PLAYER_STATUS, (unsigned int)&s_mengine.music_status );   
    if( result < 0 )
    {
        print_err("can't get player status\n");
        bret =  false;
    } 
    else
    {
        if ( s_mengine.music_status.status == MMM_PLAYER_ERROR )
        {
            bret =  false;
        }   
    } 

    return bret;
}

static bool _set_playable_file( void )
{
    bool bret = true;
    mengine_config_t *cfg_ptr = &s_mengine.config;
    int total = cfg_ptr->file_total;
    int i;
    
    //set file
    for ( i=0; i<total; i++ )
    {
        bret = _set_file( s_filepath );                
        if( bret == false )
        {
            //error process
            bret = _remove_playing_item();

            //更新文件路径(无tag)
            _update_file( false );  
            
            //stop status
            if ( bret == false )    
            {
                break;
            }    
        }
        else    //set file success
        {
            break;
        }                
    }                

    return bret;
}

static bool _next( bool force_loop )
{
    bool bret = true;
    bool result = true;
    mengine_config_t *cfg_ptr = &s_mengine.config;
    
    //获取文件索引
    if ( force_loop == true )
    {
        cfg_ptr->file_index = loop_get_next( LOOP_SWITCH_FORCELOOP );
    }
    else
    {
        cfg_ptr->file_index = loop_get_next( s_smode );
    }
    
    if( cfg_ptr->file_index < 0 )   //need to stop
    {
        cfg_ptr->file_index = loop_get_current();
        result = false;
    }
    
    //更新文件路径(无tag)
    bret = _update_file( false );
    if( bret == false )   //need to stop
    {
        result = false;
    }

    //clear play speed
    cfg_ptr->play_speed = 0;
    
    //clear flag
    s_setfile = false;
     
    return result;
}

static bool _prev( bool force_loop )
{
    bool bret = true;
    bool result = true;
    mengine_config_t *cfg_ptr = &s_mengine.config;
    
    //获取文件索引
    if ( force_loop == true )
    {
        cfg_ptr->file_index = loop_get_prev( LOOP_SWITCH_FORCELOOP );
    }
    else
    {
        cfg_ptr->file_index = loop_get_prev( s_smode );
    }
    
    if( cfg_ptr->file_index < 0 )   //need to stop
    {
        cfg_ptr->file_index = loop_get_current();
        result = false;
    }
    
    //更新文件路径(无tag)
    bret = _update_file( false );
    if( bret == false )   //need to stop
    {
        result = false;
    }
    
    //clear play speed
    cfg_ptr->play_speed = 0;
    
    //clear flag
    s_setfile = false;

    return result;
}

static bool _play( bool tag_play )
{
    int result = 0;
    bool bret =  true;
    mmm_audio_tag_t  audio_tag;
    mengine_config_t *cfg_ptr = &s_mengine.config;
 
    if( ( s_mengine.eg_status.play_status == EG_PAUSE )
        || ( s_mengine.eg_status.play_status == EG_STOP ) )
    { 
        //set file or tag  
        if( tag_play == true )
        {
            //set tag
            audio_tag.time = cfg_ptr->audio_tag.time;
            audio_tag.buf = cfg_ptr->audio_tag.buf;
        }
        else
        {
            //set file
            if ( s_setfile == false )
            {
                //set file
                bret = _set_playable_file();  
                
                //error process
                if ( bret == false )
                {
                    s_setfile = false;
                    return false;
                }                        
            }
        }
        
        //set ffb play time
        result = mmm_ad_cmd(s_ad_handle, MMM_SET_FFB_PLAY_TIME, (unsigned int)cfg_ptr->ffb_time);
        if( result != 0 )
        {
            print_err("MMM_SET_FFB_PLAY_TIME error\n");
        }  
   
        //fade in process
        if( cfg_ptr->fade == true )
        {
            result = mmm_ad_cmd(s_ad_handle, MMM_FADE_IN, 0);
            if(result !=0 )
            {
                print_err("set music fade int failed\n");
                bret = false;
            }
        }
         
        //play process        
        if( tag_play == true )
        {
            result = mmm_ad_cmd(s_ad_handle, MMM_PLAY, (unsigned int)&audio_tag);

            //set intro start time
            if ( s_mengine.eg_status.play_status == EG_STOP )
            {
                s_intro_start = ((int)audio_tag.time/1000)*1000; //whole second
            }
        }
        else
        {
            result = mmm_ad_cmd(s_ad_handle, MMM_PLAY, (unsigned int)NULL);
        }
        
        if(result !=0 )
        {
            print_err("can't play\n");
            bret = false;
        }
        else
        {
            s_mengine.eg_status.play_status = EG_PLAY;
            bret = true;
            
            //clear flag
            s_setfile = false;  
            
            //clear tag
            memset( &cfg_ptr->audio_tag, 0, sizeof(eg_audio_tag_t) ); 
            
            //start timer to read status
            kill_timer( s_timer_id );
            s_timer_id = set_timer( STATUS_INTERVAL, (timer_proc)_status_timer_event, NULL );    
        }
    }

    //设置播放速度
    result = mmm_ad_cmd(s_ad_handle, MMM_ALTER_PLAY_SPEED, (unsigned int)&cfg_ptr->play_speed);
  
    //打开/关闭频谱
    if ( cfg_ptr->display_mode != EG_LYRIC )
    {
        mmm_ad_cmd(s_ad_handle, MMM_ENABLE_SPECTRUM_SHOW, 0);
    }
    else
    {
        mmm_ad_cmd(s_ad_handle, MMM_DISABLE_SPECTRUM_SHOW, 0);
    }

    //update play status
    result = mmm_ad_cmd( s_ad_handle, MMM_GET_PLAYER_STATUS, (unsigned int)&s_mengine.music_status );    
    if( result < 0 )
    {
        print_err("get player status fail!");
        bret = false;
    }
    
    return bret;
}

static bool _stop( void )
{
    bool bret = true;
    int result = 0;
    mmm_audio_tag_t  audio_tag;
    mengine_config_t *cfg_ptr = &s_mengine.config;

    //cancel ab & fast
    switch (s_mengine.eg_status.play_status)
    {        
    case EG_PLAY_AB:
        mmm_ad_cmd(s_ad_handle, MMM_CLEAR_AB, 0);
        s_mengine.eg_status.ab_status = EG_PLAY_AB_NULL;
        s_mengine.eg_status.play_status = EG_PLAY;
        break;
        
    case EG_PLAY_FAST:
        mmm_ad_cmd(s_ad_handle, MMM_CANCEL_FFB, 0);
        s_mengine.eg_status.play_status = EG_PLAY;
        break;
        
    default:
        break;
    }
        
    if( s_mengine.eg_status.play_status == EG_PLAY )
    {
        //fade out process
        if( cfg_ptr->fade == true )
        {
            result = mmm_ad_cmd(s_ad_handle, MMM_FADE_OUT, 0);
            if(result !=0 )
            {
                print_err("set music fade out failed\n");
                bret = false;
            }
        }
        
        audio_tag.time = cfg_ptr->audio_tag.time;
        audio_tag.buf = cfg_ptr->audio_tag.buf;
        
        result = mmm_ad_cmd(s_ad_handle, MMM_STOP, (unsigned int)&audio_tag);
        if(result !=0 )
        {
            print_err("stop music play failed\n");
            bret = false;
        }
        else
        {
            s_mengine.eg_status.play_status = EG_STOP;
            bret = true;
            
            cfg_ptr->audio_tag.time = audio_tag.time;
        }
    }

    //update play status
    result = mmm_ad_cmd( s_ad_handle, MMM_GET_PLAYER_STATUS, (unsigned int)&s_mengine.music_status );    
    if( result < 0 )
    {
        print_err("get player status fail!");
        bret = false;
    }
    
    return bret;
}

static bool _auto_play_next( void )
{
    bool bret = true;
    mengine_status_t *eg_status = &s_mengine.eg_status;
    mmm_music_status_t *mmm_status = &s_mengine.music_status;
    
    eg_status->play_status = EG_STOP;
    
    //获取文件索引
    bret = _next( false );
    if ( bret == true )
    {
        //play new file
        bret = _play( false );            
        if ( bret == false )
        {
            print_err("play failed!");
        }
    }
    else
    {
        s_auto_stopped = true;   
        mmm_status->cur_time = 0;
    }
    
    s_delay_timer_id = -1;
    
    return bret;
}

bool mengine_init( void )
{
    mengine_status_t *eg_ptr = &s_mengine.eg_status;
    mengine_config_t *cfg_ptr = &s_mengine.config;
    mmm_eq_set_t eq_set;    
    plist_detail_info_t *detail_info;

    //禁止按键音
    sys_set_key_tone_enable(0);
    
    //读取设置信息
    _load_config();

    //初始化播放列表
    nplist_init( cfg_ptr->fsel_mode, s_filepath );

    //更新文件总数
    cfg_ptr->file_total = nplist_get_item_num();
    
    //更新文件索引/文件路径
    if ( cfg_ptr->file_total > 0 )
    {
        if ( s_filepath[0] != '\0' )
        {
            cfg_ptr->file_index = nplist_get_index_by_fullpath( s_filepath );
            if ( cfg_ptr->file_index < 0 )
            {
                cfg_ptr->file_index = 0;
            }
        }
        
        detail_info = nplist_get_item_by_index( cfg_ptr->file_index );
        if ( detail_info->fullpath != NULL )
        {            
            strcpy( s_filepath, detail_info->fullpath );
        }
        else
        {
            memset( s_filepath, 0, sizeof(s_filepath) );
        }
    }
    else
    {
        cfg_ptr->file_index = 0;
        memset( s_filepath, 0, sizeof(s_filepath) );
    }

    //open mmm
    s_ad_handle = mmm_ad_open(NULL);
    if(s_ad_handle == NULL)
    {
        print_err("open mmm error\n");
        return false;        
    }
    
    //初始化引擎状态信息
    eg_ptr->play_status = EG_STOP;        
    eg_ptr->err_status = EG_ERR_NONE;
    
    //初始化中间件设置
    _convert_eq_value(&cfg_ptr->eq_val, &eq_set);
    mmm_ad_cmd(s_ad_handle, MMM_SET_EQ, (unsigned int)&eq_set);

    //loop init
    loop_init();
    _set_playmode(cfg_ptr->play_mode, cfg_ptr->file_index, cfg_ptr->file_total);

    return true;
}

bool mengine_deinit( void )
{
    //stop playing
    _stop();

    //update engine
    mengine_update();

    //if auto stop  
    if ( s_auto_stopped == true )
    {
        _next( true );
    }
    
    //kill timer for updating status
    kill_timer( s_timer_id );
    s_timer_id = -1;
    
    //close mmm
    mmm_ad_close(s_ad_handle,NULL);

    //退出播放列表
    nplist_quit();    

    //save config
    _save_config();

    //打开按键音
    sys_set_key_tone_enable(1);
    
    return true;
}

bool mengine_update( void )
{
    int result;
    bool bret = true;
    mengine_status_t *eg_status = &s_mengine.eg_status;
    mmm_music_status_t *mmm_status = &s_mengine.music_status;
    int cur_time = mmm_status->cur_time;
    mengine_config_t *cfg_ptr = &s_mengine.config;
    bool now_playing;
    static bool s_last_playing = false; //上次的播放状态

    result = mmm_ad_cmd( s_ad_handle, MMM_GET_PLAYER_STATUS, (unsigned int)mmm_status );    
    if( result < 0 )
    {
        print_err();
        return false;
    }

    //process ab flag
    if( eg_status->play_status == EG_PLAY_AB )
    {
        if( ( eg_status->ab_status == EG_PLAY_AB_DO )
            && ( mmm_status->flag.AB_play_flag == false ) )
        {
            eg_status->ab_status = EG_PLAY_AB_NULL;
            eg_status->play_status = EG_PLAY;
        }
    }
    
    //快进退时间纠正
    if( eg_status->play_status == EG_PLAY_FAST )
    {
        if( ((eg_status->fast_status == EG_FFPLAY) && (mmm_status->cur_time < cur_time)) 
            || ((eg_status->fast_status == EG_FBPLAY) && (mmm_status->cur_time > cur_time))  )
        {
            mmm_status->cur_time = cur_time;
        }
    }

    eg_status->err_status = EG_ERR_NONE;
            
    switch(mmm_status->status)
    {
    case MMM_PLAYER_PLAYING:
        //if ab, break
        if ( eg_status->play_status == EG_PLAY_AB )
        {
            break;
        }
        else
        {
            eg_status->play_status = EG_PLAY;
            s_auto_stopped = false;
            s_has_played = true;
        }            
        
        //intro mode
        if ( ( ( cfg_ptr->play_mode == EG_INTRO ) && ( mmm_status->cur_time >= (s_intro_start+INTRO_INTERVAL) ) )
                || ( eg_status->err_status != EG_ERR_NONE ) )
        {
            //stop play
            bret = _stop();
            
            //获取文件索引
            bret = _next( false );
            if ( bret == true )
            {
                //play new file
                bret = _play( false );            
                if ( bret == false )
                {
                    print_err("play failed!");
                }
            }
            else
            {
                cfg_ptr->file_index = cfg_ptr->file_total - 1;
                s_auto_stopped = true;
                mmm_status->cur_time = 0;
                cfg_ptr->audio_tag.time = 0;            
            }
        }
        break;
        
    case MMM_PLAYER_PAUSED:
        eg_status->play_status = EG_PAUSE;
        break;
        
    case MMM_PLAYER_STOPPED:        
        switch( eg_status->play_status )
        {
        case EG_PLAY_FAST:            
            //fb stop at start
            if ( mmm_status->cur_time == 0 )
            {
                eg_status->play_status = EG_STOP;  
                
                //play new file
                bret = _play( false );            
                if ( bret == false )
                {
                    print_err("play failed!");
                }
                break;
            }
            
        case EG_PLAY:
        case EG_PLAY_AB:            
            //delay proc
            if ( s_delay_timer_id == -1 )
            {
                s_delay_timer_id = set_single_shot_timer(STATUS_INTERVAL, (timer_proc)_auto_play_next, NULL);
            }
            break;
            
        case EG_PAUSE:
        case EG_STOP:
            //status unchanged
            if ( ( s_auto_stopped == true ) || ( cfg_ptr->audio_tag.time == 0 ) )
            {
                mmm_status->cur_time = 0;
            }
            break;
            
        default:
            break;
        }           
        
        //intro mode
        if ( ( cfg_ptr->play_mode == EG_INTRO ) && ( mmm_status->cur_time >= (s_intro_start+INTRO_INTERVAL) ) )
        {
            mmm_status->cur_time = s_intro_start + INTRO_INTERVAL;
        }
        break;
        
    case MMM_PLAYER_FFB:
        eg_status->play_status = EG_PLAY_FAST;
        //intro mode
        if ( ( cfg_ptr->play_mode == EG_INTRO ) && ( mmm_status->cur_time >= (s_intro_start+INTRO_INTERVAL) ) )
        {
            //stop play
            bret = _stop();            
            mmm_status->cur_time = s_intro_start + INTRO_INTERVAL;
            if ( cfg_ptr->file_index == ( cfg_ptr->file_total - 1 ) )
            {
                s_auto_stopped = true;
            }
        }
        break;
        
    case MMM_PLAYER_ERROR:
//        eg_status->play_status = EG_ERROR;
        cfg_ptr->audio_tag.time = 0;
        mmm_status->cur_time = 0;

        switch( mmm_status->err_no )
        {
        case MMM_ERR_OPEN_FILE:
            eg_status->err_status = EG_ERR_OPEN_FILE;
            break;
            
        case MMM_ERR_FILE_NOT_SUPPORT: 
        case MMM_ERR_FILE_DAMAGED:
            eg_status->err_status = EG_ERR_NOT_SUPPORT;
            break;
            
        case MMM_ERR_DECODER_ERROR:
            eg_status->err_status = EG_ERR_DECODER_ERROR;
            break;
            
        case MMM_ERR_NO_LICENSE:
            eg_status->err_status = EG_ERR_NO_LICENSE;
            break;
            
        case MMM_ERR_SECURE_CLOCK:
            eg_status->err_status = EG_ERR_SECURE_CLOCK;
            break;
            
        case MMM_ERR_LICENSE_INFO:
            eg_status->err_status = EG_ERR_DRM_INFO;
            break;
        
        default:
            eg_status->err_status = EG_ERR_NOT_SUPPORT;
            break;
        }    
        break;
        
    default:
        break;
    }

    //check playing
    now_playing = _check_playing();

    //error process
    if ( ( eg_status->err_status != EG_ERR_NONE )
        && ( now_playing == true ) )
    {
        bret = _stop();
        
        //error process
        _remove_playing_item();

        //更新文件路径(无tag)
        bret = _update_file( false );  
        
        //clear play speed
        cfg_ptr->play_speed = 0;        

        //play next
        if ( bret == true )
        {
            bret = _play( false );
        }
    }     

    //process standby    
    if ( s_last_playing != now_playing )   //changed
    {
        if ( now_playing == true )   //start playing
        {
            sys_forbid_auto_standby( true );
        }
        else    //stop playing
        {
            sys_forbid_auto_standby( false );
        }
        
        s_last_playing = now_playing;
    }    

    //process update timer
    if ( now_playing != true )   //if stop
    {
        //kill timer for updating status
        kill_timer( s_timer_id );
        s_timer_id = -1;
    }

    return bret;
}

/*************************************************************************
* Function:            mengine_get_engine                      
* Description:        获取引擎信息                          
* para1：           pmsg    指向接收到的消息                         
* Return：          bool   true:  成功   false: 失败                    
*************************************************************************/
bool mengine_get_engine_info(const msg_apps_t *pmsg)
{
    bool bret = true;
    
    _reply_msg( &s_mengine, sizeof(mengine_t), bret );
        
    return bret;
}

/*************************************************************************
* Function:            mengine_get_filepath                      
* Description:        获取引擎当前文件路径                        
* para1：           pmsg    指向接收到的消息                         
* Return：          bool   true:  成功   false: 失败                    
*************************************************************************/
bool mengine_get_filepath(const msg_apps_t *pmsg)
{
    bool bret = true;
    
    _reply_msg( s_filepath, sizeof(s_filepath), bret );
        
    return bret;
}

/*************************************************************************
* Function:            mengine_set_eq                                 
* Description:        设置EQ                                         
* para1：           pmsg    指向接收到的消息                        
* Return：          bool   true:  成功   false: 失败           
*************************************************************************/
bool  mengine_set_eq(const msg_apps_t *pmsg)
{
    mengine_eq_t *eq_val = &s_mengine.config.eq_val;
    mmm_eq_set_t eq_set;
    bool bret = true;
    int result = 0;

    //process msg
    memcpy( eq_val, pmsg->content, sizeof(mengine_eq_t) );      
    _convert_eq_value( eq_val, &eq_set );
    
    //set mmm    
    result = mmm_ad_cmd(s_ad_handle, MMM_SET_EQ, (unsigned int)&eq_set);
    if(result !=0 )
    {
        print_err("set music eq failed\n");
        bret = false;
    }
    else
    {
        bret = true;
    }
    
    _reply_msg( NULL, 0, bret );        

    return bret;
}

/*************************************************************************
* Function:            mengine_set_playmode                      
* Description:        设置循环模式                        
* para1：           pmsg    指向接收到的消息                         
* Return：          bool   true:  成功   false: 失败                    
*************************************************************************/
bool mengine_set_playmode(const msg_apps_t *pmsg)
{
    mengine_config_t *config = &s_mengine.config;
    bool bret = true;
    
    //process msg
    if ( config->play_mode != (play_mode_e)pmsg->content )
    {
        memcpy( &config->play_mode, pmsg->content, sizeof(play_mode_e) );   
        
        //change loop mode
        _set_playmode( config->play_mode, config->file_index, config->file_total );

        //set intro start time
        if ( config->play_mode == EG_INTRO )
        {
            s_intro_start = (s_mengine.music_status.cur_time/1000)*1000; //whole second
        }
    }
    
    _reply_msg( NULL, 0, bret );
        
    return bret;
}

/*************************************************************************
* Function:            mengine_set_displaymode                      
* Description:        设置显示模式                        
* para1：           pmsg    指向接收到的消息                         
* Return：          bool   true:  成功   false: 失败                    
*************************************************************************/
bool mengine_set_displaymode(const msg_apps_t *pmsg)
{
    bool bret = true;
    mengine_config_t *config = &s_mengine.config;
    
    //process msg
    memcpy( &config->display_mode, pmsg->content, sizeof(display_mode_e) );   

    if ( config->display_mode != EG_LYRIC )
    {
        mmm_ad_cmd(s_ad_handle, MMM_ENABLE_SPECTRUM_SHOW, 0);
    }
    else
    {
        mmm_ad_cmd(s_ad_handle, MMM_DISABLE_SPECTRUM_SHOW, 0);
    }

    _reply_msg( NULL, 0, bret );
        
    return bret;
}

/*************************************************************************
* Function:            mengine_set_fade                      
* Description:        设置淡入淡出                      
* para1：           pmsg    指向接收到的消息                         
* Return：          bool   true:  成功   false: 失败                    
*************************************************************************/
bool mengine_set_fade(const msg_apps_t *pmsg)
{
    bool bret = true;
    
    //process msg
    memcpy( &s_mengine.config.fade, pmsg->content, sizeof(bool) );   
    
    _reply_msg( NULL, 0, bret );
        
    return bret;
}

/************************************************************************
* Function:            mengine_set_playspeed                            
* Description:        调整播放速度，值范围[-8，8]                    
* para1：           pmsg    指向接收到的消息                         
* Return：          bool   true:  成功   false: 失败                    
*************************************************************************/
bool mengine_set_playspeed(const msg_apps_t *pmsg)
{
    bool bret = true;
    int result = 0;
    int  play_speed = 0;

    memcpy(&play_speed, pmsg->content, sizeof(int));

    if(play_speed > MAX_PLAY_SPEED)
    {
        play_speed = MAX_PLAY_SPEED;
    }

    if(play_speed < MIN_PLAY_SPEED)
    {
        play_speed = MIN_PLAY_SPEED;
    }

    print_dbg("play speed is %d", play_speed);
    
    result = mmm_ad_cmd(s_ad_handle, MMM_ALTER_PLAY_SPEED, (unsigned int)&play_speed);
    if(result !=0 )
    {
        print_err("can't alter speed\n");
        bret = false;
    }
    else
    {
        //save play speed
        s_mengine.config.play_speed = play_speed;
        bret = true;
    }
    
    _reply_msg( NULL, 0, bret );
    
    return bret;
}

/*************************************************************************
* Function:            mengine_set_ab_auto                      
* Description:        设置AB自动复读标志                  
* para1：           pmsg    指向接收到的消息                         
* Return：          bool   true:  成功   false: 失败                    
*************************************************************************/
bool mengine_set_ab_auto(const msg_apps_t *pmsg)
{
    bool bret = true;
    
    //process msg
    memcpy( &s_mengine.config.ab_autoplay, pmsg->content, sizeof(bool) );   
    
    _reply_msg( NULL, 0, bret );
        
    return bret;
}

/*************************************************************************
* Function:            mengine_set_ab_count                         
* Description:        设置AB循环次数，值范围[0，+∞]                  
* para1：           pmsg    指向接收到的消息                         
* Return：          bool   true:  成功   false: 失败                    
*************************************************************************/
bool mengine_set_ab_count(const msg_apps_t *pmsg)
{
    bool bret = true;
    int result = 0;
    unsigned  int  ab_count = 0;

    memcpy(&ab_count, pmsg->content, sizeof(int));
    
    result = mmm_ad_cmd(s_ad_handle, MMM_SET_AB_COUNT, ab_count);
    if(result !=0 )
    {
        print_err("can't set ab count\n");
        bret = false;
    }
    else
    {
        //save ab count
        s_mengine.config.ab_count = (int)ab_count;
        bret = true;
    }
    
    _reply_msg( NULL, 0, bret );
    
    return bret;
}


/***************************************************************************
* Function:            mengine_set_ab_time                          
* Description:        设置从B点回到A点的等待时间                         
* para1：           pmsg    指向接收到的消息                             
* Return：          bool   true:  成功   false: 失败                      
***************************************************************************/
bool mengine_set_ab_time(const msg_apps_t *pmsg)
{
    bool bret = true;
    int result = 0;
    unsigned  int  wait_time = 0;

    memcpy(&wait_time, pmsg->content, sizeof(int));
    
    result = mmm_ad_cmd(s_ad_handle, MMM_SET_AB_WAIT_TIME, wait_time);
    if(result !=0 )
    {
        print_err("mengine_set_ab_waittime error\n");
        bret = false;
    }
    else
    {
        //save ab count
        s_mengine.config.ab_time = (int)wait_time;
        bret = true;
    }
    
    _reply_msg( NULL, 0, bret );
    
    return bret;
}

/*************************************************************************
* Function:            mengine_start_ab                              
* Description:        开始AB播放设置                                
* para1：           pmsg    指向接收到的消息                            
* Return：          bool   true:  成功   false: 失败                    
*************************************************************************/
bool mengine_start_ab(const msg_apps_t *pmsg)
{
    bool bret = true;
    int result = 0;
    mmm_music_status_t music_status;
    
    if( s_mengine.eg_status.play_status == EG_PLAY )
    {
        //read music status
        result = mmm_ad_cmd(s_ad_handle, MMM_GET_PLAYER_STATUS, (unsigned int)&music_status);
        if(result !=0 )
        {
            print_err("get player status failed\n");
            bret = false;
        }
        else
        {
            //save music status
            s_mengine.music_status = music_status;
            result = mmm_ad_cmd(s_ad_handle, MMM_SET_AB_COUNT, (unsigned int)s_mengine.config.ab_count);
            result = mmm_ad_cmd(s_ad_handle, MMM_SET_AB_WAIT_TIME, (unsigned int)s_mengine.config.ab_time);
            s_mengine.eg_status.play_status = EG_PLAY_AB;
            s_mengine.eg_status.ab_status = EG_PLAY_AB_A;
            bret = true;
        }
    }
    else
    {
        bret = false;
    }
    
    _reply_msg( NULL, 0, bret );
    
    return bret;
}

/*************************************************************************
* Function:            mengine_clear_ab                                
* Description:        取消AB播放                                     
* para1：           pmsg    指向接收到的消息                           
* Return：          bool   true:  成功   false: 失败                    
*************************************************************************/
bool mengine_clear_ab(const msg_apps_t *pmsg)
{
    bool bret = true;
    int result = 0;

    result = mmm_ad_cmd(s_ad_handle, MMM_CLEAR_AB, 0);
    if(result !=0 )
    {
        print_err("mengine_clear_ab error\n");
        bret = false;
    }
    else
    {
        s_mengine.eg_status.play_status = EG_PLAY;
        s_mengine.eg_status.ab_status = EG_PLAY_AB_NULL;
        bret = true;
    }
    
    _reply_msg( NULL, 0, bret );
    
    return bret;
}

/*************************************************************************
* Function:            mengine_set_ab_next                              
* Description:        设置AB播放进入下一状态                                  
* para1：           pmsg    指向接收到的消息                            
* Return：          bool   true:  成功   false: 失败                    
*************************************************************************/
bool mengine_set_ab_next(const msg_apps_t *pmsg)
{
    bool bret = true;
    int result = 0;

    if( s_mengine.eg_status.play_status == EG_PLAY_AB )
    {
        switch( s_mengine.eg_status.ab_status )
        {
        case EG_PLAY_AB_NULL:
        case EG_PLAY_AB_A:
            result = mmm_ad_cmd(s_ad_handle, MMM_SET_A, 0);
            if(result !=0 )
            {
                print_err("set A point failed\n");
                bret = false;
            }
            else
            {
                s_mengine.eg_status.ab_status = EG_PLAY_AB_B;
                bret = true;
            }
            break;
            
        case EG_PLAY_AB_B:
            result = mmm_ad_cmd(s_ad_handle, MMM_SET_B, 0);
            if(result !=0 )
            {
                print_err("set B point failed\n");
                bret = false;
            }
            else
            {
                s_mengine.eg_status.ab_status = EG_PLAY_AB_DO;
                bret = true;
            }
            break;
            
        case EG_PLAY_AB_DO:
            result = mmm_ad_cmd(s_ad_handle, MMM_CLEAR_AB, 0);
            if(result !=0 )
            {
                print_err("Clear AB point failed\n");
                bret = false;
            }
            else
            {
                s_mengine.eg_status.play_status = EG_PLAY;
                s_mengine.eg_status.ab_status = EG_PLAY_AB_NULL;
                bret = true;
            }
            break;
            
        default:
            break;
        }
    }
    
    _reply_msg( NULL, 0, bret );
    
    return bret;
}

/*************************************************************************
* Function:            mengine_set_ab_prev                              
* Description:        设置AB播放进入上一状态                                  
* para1：           pmsg    指向接收到的消息                            
* Return：          bool   true:  成功   false: 失败                    
*************************************************************************/
bool mengine_set_ab_prev(const msg_apps_t *pmsg)
{
    bool bret = true;
    int result = 0;

    if( s_mengine.eg_status.play_status == EG_PLAY_AB )
    {
        switch( s_mengine.eg_status.ab_status )
        {
        case EG_PLAY_AB_NULL:
        case EG_PLAY_AB_A:
            s_mengine.eg_status.play_status = EG_PLAY;
            s_mengine.eg_status.ab_status = EG_PLAY_AB_NULL;
            break;
            
        case EG_PLAY_AB_B:
        case EG_PLAY_AB_DO:
            result = mmm_ad_cmd(s_ad_handle, MMM_CLEAR_AB, 0);
            if(result != 0 )
            {
                print_err("Clear AB point failed\n");
                bret = false;
            }
            else
            {
                s_mengine.eg_status.ab_status = EG_PLAY_AB_A;
                bret = true;
            }
            break;
            
        default:
            break;
        }
    }
    
    _reply_msg( NULL, 0, bret );
    
    return bret;
}

/*************************************************************************
* Function:            mengine_play                                    
* Description:        开始播放音乐,根据消息的内容                      
*                          分为从头开始播放和从断点处开始播放          
*                                                                      
* para1：            pmsg    指向接收到的消息                          
* Return：           bool   true: 成功   false:失败                    
**************************************************************************/
bool mengine_play(const msg_apps_t *pmsg)
{
    bool bret =  true;
    bool tag_play = true;

    //if auto stop  or check tag
    if ( ( s_auto_stopped == true ) || ( s_mengine.config.audio_tag.time == 0 ) )
    {
        tag_play = false;
    }    

    bret = _play( tag_play );
    if ( bret == false )
    {
        bret = _play( false );  //try again
    }
    
    _reply_msg( NULL, 0, bret );
    
    return bret;
}

/************************************************************************
* Function:            mengine_stop                                     
* Description:        停止播放音乐                                      
* para1：           pmsg    指向接收到的消息                            
* Return：          bool   true:  成功   false: 失败                    
*************************************************************************/
bool mengine_stop(const msg_apps_t *pmsg)
{
    bool bret = true;
    
    bret = _stop();
    
    _reply_msg( NULL, 0, bret );
    
    return bret;
}

/***********************************************************************
* Function:            mengine_pause                                   
* Description:       暂停播放音乐                                      
* para1：           pmsg    指向接收到的消息                           
* Return：          bool   true:  成功   false: 失败                   
************************************************************************/
bool mengine_pause(const msg_apps_t *pmsg)
{
    bool bret = true;

    bret = _stop();
    if ( bret == true )
    {
        s_mengine.eg_status.play_status = EG_PAUSE;
    }
    
    _reply_msg( NULL, 0, bret );

    return bret;
}

/************************************************************************
* Function:            mengine_resume                                  
* Description:        继续播放音乐                                     
* para1：           pmsg    指向接收到的消息                           
* Return：          bool   true:  成功   false: 失败                   
*************************************************************************/
bool mengine_resume(const msg_apps_t *pmsg)
{
    bool bret = true;

    bret = _play( true );
    if ( bret == false )
    {
        bret = _play( false );  //try again
    }
    
    _reply_msg( NULL, 0, bret );
    
    return bret;
}

/*************************************************************************
* Function:            mengine_seek                                
* Description:        seek播放,传递time以ms为单位                       
* para1：           pmsg    指向接收到的消息                        
* Return：          bool   true:  成功   false: 失败                    
*************************************************************************/
bool  mengine_seek(const msg_apps_t *pmsg)
{
    bool bret = true;
    int result = 0;
    unsigned  int  begin_pos = 0;

    memcpy(&begin_pos, pmsg->content, sizeof(int));
    
    result = mmm_ad_cmd(s_ad_handle, MMM_SEEK, begin_pos);
    if(result !=0 )
    {
        print_err("set seek play failed\n");
        bret = false;
    }
    else
    {
        s_mengine.eg_status.play_status = EG_PLAY;
        bret = true;
    }
    
    _reply_msg( NULL, 0, bret );
        
    return bret;
}

/************************************************************************
* Function:            mengine_fast_forward                           
* Description:        快进，传递参数为快进                            
*                         的速度等级，范围为[0，4]                    
*                                                                     
* para1：           pmsg    指向接收到的消息                          
* Return：          bool   true:  成功   false: 失败                  
*************************************************************************/
bool mengine_fast_forward(const msg_apps_t *pmsg)
{
    bool bret = true;
    int result = 0;
    
    //cancel ab
    switch (s_mengine.eg_status.play_status)
    {        
    case EG_PLAY_AB:
        mmm_ad_cmd(s_ad_handle, MMM_CLEAR_AB, 0);
        s_mengine.eg_status.ab_status = EG_PLAY_AB_NULL;
        s_mengine.eg_status.play_status = EG_PLAY;
        break;
        
    default:
        break;
    }
    
    if ( s_mengine.eg_status.play_status == EG_PLAY )
    {
        result = mmm_ad_cmd(s_ad_handle, MMM_FAST_FORWARD, (unsigned int)s_mengine.config.ffb_speed);
        if( result != 0 )
        {
            print_err("mengine_fast_forward error\n");
            bret = false;
        }
        else
        {
            s_mengine.eg_status.play_status = EG_PLAY_FAST;
            s_mengine.eg_status.fast_status = EG_FFPLAY;
            bret = true;
        }
    }
    else
    {
        bret = false;
    }
    
    _reply_msg( NULL, 0, bret );

    return bret;
}


/************************************************************************
* Function:            mengine_fast_backward                            
* Description:        快退，传递参数为快退的                            
*                         速度等级，范围为[0，4]                        
*                                                                       
* para1：           pmsg    指向接收到的消息                            
* Return：          bool   true:  成功   false: 失败                    
*************************************************************************/
bool mengine_fast_backward(const msg_apps_t *pmsg)
{
    bool bret = true;
    int result = 0;
    
    //cancel ab
    switch (s_mengine.eg_status.play_status)
    {        
    case EG_PLAY_AB:
        mmm_ad_cmd(s_ad_handle, MMM_CLEAR_AB, 0);
        s_mengine.eg_status.ab_status = EG_PLAY_AB_NULL;
        s_mengine.eg_status.play_status = EG_PLAY;
        break;
        
    default:
        break;
    }
    
    if ( s_mengine.eg_status.play_status == EG_PLAY )
    {
        result = mmm_ad_cmd(s_ad_handle, MMM_FAST_BACKWARD, (unsigned int)s_mengine.config.ffb_speed);
        if( result != 0 )
        {
            print_err("can't play\n");
            bret = false;
        }
        else
        {
            s_mengine.eg_status.play_status = EG_PLAY_FAST;
            s_mengine.eg_status.fast_status = EG_FBPLAY;
            bret = true;
        }
    }
    else
    {
        bret = false;
    }
    
    _reply_msg( NULL, 0, bret );

    return bret;
}


/************************************************************************
* Function:            mengine_cancel_ffb                               
* Description:        取消快进快退                                      
* para1：           pmsg    指向接收到的消息                            
* Return：          bool   true:  成功   false: 失败                    
*************************************************************************/
bool mengine_cancel_ffb(const msg_apps_t *pmsg)
{
    bool bret = true;
    int result = 0;
    mmm_music_status_t *mmm_status = &s_mengine.music_status;
    mengine_config_t *cfg_ptr = &s_mengine.config;
    
    result = mmm_ad_cmd(s_ad_handle, MMM_CANCEL_FFB, 0);
    if(result !=0 )
    {
        print_err("cancel fast forward and fast backward failed\n");
        bret = false;
    }
    else
    {
        bret = true;
    }
    
    //intro mode and stop
    if ( ( cfg_ptr->play_mode == EG_INTRO ) && ( mmm_status->cur_time >= (s_intro_start+INTRO_INTERVAL) ) )
    {
        //get next
        bret = _next( false );
        if ( bret == true )
        {
            bret = _play( false );
        }
    }
    
    _reply_msg( NULL, 0, bret );

    return bret;
}



/************************************************************************
* Function:            mengine_set_ffb_step                             
* Description:        设置快近快退速度等级，范围为[0，4]                
* para1：           pmsg    指向接收到的消息                            
* Return：          bool   true:  成功   false: 失败                   
*************************************************************************/
bool mengine_set_ffb_step(const msg_apps_t *pmsg)
{
    bool bret = true;
    int result = 0;
    unsigned  int  fast_ffb_step = 0;

    memcpy(&fast_ffb_step, pmsg->content, sizeof(int));
    
    if(fast_ffb_step > MAX_FFB_STEP)
    {
        fast_ffb_step = MAX_FFB_STEP;
    }

    result = mmm_ad_cmd(s_ad_handle, MMM_SET_FFB_STEP, fast_ffb_step);
    if(result !=0 )
    {
        print_err("can't play\n");
        bret = false;
    }
    else
    {
//        s_mengine.eg_status.play_status = EG_PLAY_FAST;
        s_mengine.config.ffb_speed = (int)fast_ffb_step;
        bret = true;
    }
    
    _reply_msg( NULL, 0, bret );

    return bret;
}

/*************************************************************************
* Function:           mengine_set_ffb_time                             
* Description:       设置在快近快退过程中，                           
*                         每一快进或快退之间音乐                       
*                         播放的时间长度                               
*                                                                       
* para1：           pmsg    指向接收到的消息                            
* Return：          bool   true:  成功   false: 失败                    
*************************************************************************/
bool  mengine_set_ffb_time(const msg_apps_t *pmsg)
{
    bool bret = true;
    int result = 0;
    unsigned  int  ffb_play_time = 0;

    memcpy(&ffb_play_time, pmsg->content, sizeof(int));
    
    result = mmm_ad_cmd(s_ad_handle, MMM_SET_FFB_PLAY_TIME, ffb_play_time);
    if(result !=0 )
    {
        print_err("set ffb time failed\n");
        bret = false;
    }
    else
    {
        s_mengine.config.ffb_time = (int)ffb_play_time;
        bret = true;
    }
    
    _reply_msg( NULL, 0, bret );

    return bret;
}

/************************************************************************
* Function:            mengine_set_filepath                              
* Description:        打开指定路径的播放文件                            
* para1：           pmsg    指向接收到的消息                            
* Return：          int                                                 
*************************************************************************/
bool mengine_set_filepath(const msg_apps_t* pmsg )
{
    bool bret;
    mengine_config_t *cfg_ptr = &s_mengine.config;

    //stop
    bret = _stop();
    
    //set file
    bret = _set_file( (char*)pmsg->content );

    //save path
    strncpy( s_filepath, pmsg->content, sizeof(s_filepath) );

    //reinit nplist
    cfg_ptr->fsel_mode = true;
    nplist_init( cfg_ptr->fsel_mode, s_filepath );
    
    //更新文件总数
    cfg_ptr->file_total = nplist_get_item_num();
    
    //更新文件索引/文件路径
    cfg_ptr->file_index = nplist_get_index_by_fullpath( s_filepath );
    
    //更新循环模式
    _set_playmode(cfg_ptr->play_mode, cfg_ptr->file_index, cfg_ptr->file_total);
    
    //clear tag        
    cfg_ptr->audio_tag.time = 0;

    //clear play speed
    if ( s_has_played == true ) //不是第一次播放重置播放速度
    {
        cfg_ptr->play_speed = 0;
    }
    
    _reply_msg( NULL, 0, bret );

    return bret;
}

/************************************************************************
* Function:            mengine_set_fileindex                                
* Description:        设置当前播放文件索引                                 
* para1：            pmsg    指向接收到的消息                           
* Return：          bool    true:成功   false:失败                      
*************************************************************************/
bool  mengine_set_fileindex(const  msg_apps_t *pmsg)
{
    bool bret = true;
    mengine_config_t *cfg_ptr = &s_mengine.config;
    
    //save file index
    memcpy(&cfg_ptr->file_index, pmsg->content, sizeof(int));

    print_dbg("file_index=%d", cfg_ptr->file_index);

    //stop
    bret = _stop();    
    
    //reinit nplist
    cfg_ptr->fsel_mode = false;
    nplist_init( cfg_ptr->fsel_mode, s_filepath );
        
    //更新文件路径(有tag)
    bret = _update_file( true );
    
    //更新循环模式
    _set_playmode(cfg_ptr->play_mode, cfg_ptr->file_index, cfg_ptr->file_total);
    
    //set file
    bret = _set_file( s_filepath );
    
    //clear play speed
    if ( s_has_played == true ) //不是第一次播放重置播放速度
    {
        cfg_ptr->play_speed = 0;
    }
    
    _reply_msg( NULL, 0, bret );
 
    return bret;
}

/************************************************************************
* Function:            mengine_play_next                                
* Description:        播放下一首                            
* para1：            pmsg    指向接收到的消息                           
* Return：          bool    true:成功   false:失败                      
*************************************************************************/
bool  mengine_play_next(const  msg_apps_t *pmsg)
{
    bool bret = true;
    bool is_playing;

    s_smode = LOOP_SWITCH_FORCELOOP;
    s_dir = LOOP_DIRECT_NEXT;
     
    //play status
    is_playing = _check_playing();

    //获取文件索引    
    bret = _next( true );    
    if ( bret == true )
    {
        //stop music
        _stop();
        
        //new file
        if ( is_playing == true )
        {
            bret = _play( false );        
        }
        else
        {
            bret = _set_playable_file();        
        }
    }
    else
    {       
        //stop music
        _stop();
        
        //clear tag
        memset( &s_mengine.config.audio_tag, 0, sizeof(eg_audio_tag_t) ); 
        
        s_auto_stopped = true;  
        bret = true;     
    }
    
    if ( pmsg != NULL )
    {
        _reply_msg( NULL, 0, bret );
    }
    
    s_smode = LOOP_SWITCH_NORMAL;
    s_dir = LOOP_DIRECT_NEXT;
    
    return bret;
}

/************************************************************************
* Function:            mengine_play_prev                                
* Description:        播放上一首                            
* para1：            pmsg    指向接收到的消息                           
* Return：          bool    true:成功   false:失败                      
*************************************************************************/
bool  mengine_play_prev(const  msg_apps_t *pmsg)
{
    bool bret = true;
    bool is_playing;

    s_smode = LOOP_SWITCH_FORCELOOP;
    s_dir = LOOP_DIRECT_PREV;
    
    //play status
    is_playing = _check_playing();

    //获取文件索引
    bret = _prev( true );
    if ( bret == true )
    {
        //stop music
        _stop();
        
        //new file
        if ( is_playing == true )
        {
            bret = _play( false );        
        }
        else
        {
            bret = _set_playable_file(); 
        }  
    }
    else
    {   
        //stop music
        _stop();
        
        //clear tag
        memset( &s_mengine.config.audio_tag, 0, sizeof(eg_audio_tag_t) );   
        
        bret = true;     
    }

    if ( pmsg != NULL )
    {
        _reply_msg( NULL, 0, bret );
    }

    s_smode = LOOP_SWITCH_NORMAL;
    s_dir = LOOP_DIRECT_NEXT;

    return bret;
}

bool mengine_delete_file(const msg_apps_t *pmsg)
{
    bool bret = true;
    bool is_playing;
    int result;
    mengine_config_t *cfg_ptr = &s_mengine.config;

    //play status
    is_playing = _check_playing();
    
    //stop play
    _stop();
    
    //set null to release file resource
    mmm_ad_cmd( s_ad_handle, MMM_SET_FILE, (unsigned int)"" );

    //delete file
    result = remove( s_filepath );
    if ( result == 0 )  //success
    {
        //delete item in nplist
        bret = _remove_playing_item();
        if ( bret == false )    //need to stop
        {
            is_playing = false;  
        }
        
        //更新文件路径(无tag)
        bret = _update_file( false );
        if ( bret == false )    //need to stop
        {
            is_playing = false;  
        }

        //clear play speed
        cfg_ptr->play_speed = 0;
    
        //restore engine play status
        if ( is_playing == true )
        {
            //play new file
            _play( false );  
        }
        else
        {
            _set_playable_file(); 
        }
    }
    else
    {
        print_err("remove fail: %s", s_filepath);
    }
    
    _reply_msg( NULL, 0, ( result == 0 ) );
    
    return bret;
}

bool  mengine_make_bookmark(const  msg_apps_t *pmsg)
{
    bool bret = true;
    int result = 0;
    mmm_audio_tag_t  audio_tag;
    eg_audio_tag_t tag;
    audio_tag.buf = tag.buf;    
    
    result = mmm_ad_cmd(s_ad_handle, MMM_MAKE_MARK, (unsigned int)&audio_tag);
    if(result !=0 )
    {
        print_err("make mark failed\n");
        bret = false;
    }
    else
    {
        tag.time = audio_tag.time;
        bret = true;
    }
    
    _reply_msg( &tag, sizeof(eg_audio_tag_t), bret );

    return bret;
}

bool  mengine_set_last_file(const  msg_apps_t *pmsg)
{
    bool bret = true;

    //check pause or stop
    switch ( s_mengine.eg_status.play_status )
    {
//    case EG_PAUSE:
    case EG_STOP:
        //if auto stop    
        if ( s_auto_stopped == true )
        {
            _next( true );
        }    

        //set file
        bret = _set_file( s_filepath );    
        break;
        
    default:
        bret = true;
        break;
    }

    _reply_msg( NULL, 0, bret );
 
    return bret;
}

bool mengine_gsensor_shaked(const msg_apps_t *pmsg)
{
    bool bret = true;

    //check play
    switch ( s_mengine.eg_status.play_status )
    {
    case EG_PLAY:
    case EG_PLAY_AB:
        bret = mengine_play_next(NULL);
        break;
        
    default:
        bret = false;
        break;
    }
    
    return bret;
}

