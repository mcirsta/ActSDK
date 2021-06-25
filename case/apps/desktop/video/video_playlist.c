/*******************************************************************************
 *                              NOYA1100
 *                            Module: video
 *                 Copyright(c) 2003-2008 Actions Semiconductor,
 *                            All Rights Reserved. 
 * History:         
 *      <author>    <time>           <version >             <desc>
 *      shaofeng    2009-04-23          1.0             build this file
 *      dengjihai   2009-11-20          1.1             support fsel play
*******************************************************************************/
/*!
 * \file     video_playlist.c
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
static bool _nplist_init( void );
static bool _nplist_deinit( void );
static bool _taglist_init( void );
static bool _recent_list_init( void );
/* fsel */
static FSEL_HANDLE s_fsel = NULL;


#if 0
#endif
bool is_swf_ext( char *fullpath )
{
    char tmp_buffer[4];
    size_t tmp = 0;
    
    if ( NULL == fullpath )
    {
        return false;
    }
    
    //找到文件后缀, 并与swf比较, 选择进入相应的播放场景
    tmp = strlen( fullpath );
    tmp -= 3;
    memcpy( tmp_buffer, &fullpath[tmp], 3 );
    if( 0 == strncasecmp( tmp_buffer, video_vars.buffer_support_ext_video_swf, 3 ) )
    {
        return true;
    }
    else
    {
        return false;
    }
}

#if 0
#endif

bool _fsel_deinit( void )
{
    if ( s_fsel != NULL )
    {
        fsel_close(s_fsel);
        s_fsel = NULL;
    }
    
    return true;
}

bool _fsel_init( char *filepath )
{
    char path[PATH_MAX];
    int result;
    char ext_video[PATH_MAX];
    char ext_video_swf[PATH_MAX];

    if( NULL == s_fsel )
    {
        //open
        s_fsel = fsel_open();
        if ( s_fsel == NULL )
        {
            print_err();
            return false;
        }    
        
        result = get_config( CFG_APP_SUPPORT_EXT_VIDEO, (char*)ext_video, sizeof(ext_video) );
        if( result < 0 )
        {
            print_err("get video ext failed!");
        } 

        result = get_config( CFG_APP_SUPPORT_EXT_VIDEO_SWF, (char*)ext_video_swf, sizeof(ext_video_swf) );       
        if( result < 0 )
        {
            print_err("get video swf ext failed!");
        } 
        strcat( ext_video, "|" );
        strcat( ext_video, ext_video_swf );
        
        fsel_setfilter( s_fsel, ext_video );
       
        
        //set subdir
        fsel_subdir_setting(s_fsel, FSEL_NOSUB, FSEL_FILENAME);   

        //sort
        fsel_sortstyle(s_fsel, FSEL_PINYIN);
    }

    //set dir
    strcpy(path, filepath);
    fsel_setworkdir(s_fsel, dirname(path));  
    fsel_getfilenum(s_fsel, FSEL_FILENAME);
    return true;
}

#if 0
#endif
/***************************** loop mode ******************************/
/*********************************************************************
*初始化播放模式
*注意:初始化的init_index参数为初始化后播放的第一首文件的index，
**********************************************************************/
bool video_init_play_mode( video_playmode_e playmode, int init_index ,int total_num )
{
    bool result = true;
    loop_mode_e loop_mode;
    
    switch ( playmode )
    {
    case VIDEO_PLAYMOE_SEQUENCE:
        loop_mode = LOOP_MODE_SEQUENCE;
        break;
        
    case VIDEO_PLAYMOE_REPEAT_ONE:
        loop_mode = LOOP_MODE_REPEAT_ONE;
        break;
        
    case VIDEO_PLAYMOE_REPEAT_ALL:
        loop_mode = LOOP_MODE_REPEAT_ALL;
        break;
        
    case VIDEO_PLAYMOE_SHUFFLE:
        loop_mode = LOOP_MODE_SHUFFLE;
        break;
        
    case VIDEO_PLAYMOE_SHUFFLE_REPEAT:
        loop_mode = LOOP_MODE_SHUFFLE_REPEAT;
        break;
        
    default:
        loop_mode = LOOP_MODE_SEQUENCE;
        break;
    }
   
    result = loop_set_mode( loop_mode, init_index, total_num );
    print_dbg("init_index: %d, total_num: %d", init_index, total_num );
    return result;
}

bool video_set_play_mode( video_playmode_e playmode )
{
    bool result = true;
    loop_mode_e loop_mode;
    int np_num;
    
    switch ( playmode )
    {
    case VIDEO_PLAYMOE_SEQUENCE:
        loop_mode = LOOP_MODE_SEQUENCE;
        break;
        
    case VIDEO_PLAYMOE_REPEAT_ONE:
        loop_mode = LOOP_MODE_REPEAT_ONE;
        break;
        
    case VIDEO_PLAYMOE_REPEAT_ALL:
        loop_mode = LOOP_MODE_REPEAT_ALL;
        break;
        
    case VIDEO_PLAYMOE_SHUFFLE:
        loop_mode = LOOP_MODE_SHUFFLE;
        break;
        
    case VIDEO_PLAYMOE_SHUFFLE_REPEAT:
        loop_mode = LOOP_MODE_SHUFFLE_REPEAT;
        break;
        
    default:
        print_warning("playmode:%d, out of range", playmode);
        loop_mode = LOOP_MODE_SEQUENCE;
        break;
    }
    
    np_num = video_plist_get_item_num( video_vars.nplist );
    print_dbg("playmode:%d, loopmode:%d, np_num:%d", playmode, loop_mode,np_num);
    
    result = loop_set_mode( loop_mode, video_vars.plist.np_index, np_num );
    video_vars.play_param.playmode = playmode;
    
    return result;
}


/***************************** display mode ******************************/
bool video_set_display_mode( video_displaymode_e displaymode )
{
    bool result = true;
    mmm_video_ratio_t display_mode;
    
    switch ( displaymode )
    {
    case VIDEO_DISPLAYMOE_FULL:
        display_mode = MMM_RATIO_FULL ;
        break;
        
    case VIDEO_DISPLAYMOE_ORG:
        display_mode = MMM_RATIO_ORG;
        break;
        
    case VIDEO_DISPLAYMOE_EQU:
        display_mode = MMM_RATIO_EQU;
        break;
               
    default:
        print_warning("displaymode:%d, out of range", displaymode);
        display_mode = MMM_RATIO_FULL;
        break;
    }
    print_dbg("display_mode:%d, displaymode:%d", display_mode, displaymode+1);
    
    result = video_control(V_CMD_SET_DISPLAY_MODE, &display_mode);
    video_vars.play_param.displaymode = displaymode;

    /*需要调用此函数以实现暂停时改变屏幕大小时立即开始播放刷新屏幕*/
    proc_displaymode_update();
    return result;
}

/******************************************************************************/
/*!                    
* \par  Description:
*      获得上一个index, 用于播放
* \param[in]    
* \retval       int: -1: err; 其他:index
*******************************************************************************/
int video_get_prev_play_index( loop_switch_e smode )
{
    int index_cur;
    index_cur = loop_get_prev( smode );
    
    print_dbg("switch mode:%d, index:%d", smode,index_cur);
    return index_cur;
}

/******************************************************************************/
/*!                    
* \par  Description:
*      获得下一个index, 用于播放
* \param[in]    
* \retval       int: -1: err; 其他:index
*******************************************************************************/
int video_get_next_play_index( loop_switch_e smode )
{
    int index_cur;
    index_cur = loop_get_next( smode );
    
    print_dbg("switch mode:%d, index:%d", smode,index_cur);
    return index_cur;
}


#if 0
#endif
/***************************** playlist 通用部分 ******************************/
static bool _nplist_init( void )
{
    int tmp;
    plist_trivefield_t trive_field;
    char video_ext[44];
    
    strncpy( video_ext, video_vars.buffer_support_ext_video, sizeof(video_vars.buffer_support_ext_video) );
    strncat( video_ext, video_vars.buffer_support_ext_video_swf, sizeof(video_vars.buffer_support_ext_video_swf) );
    tmp = plist_set_filetype_ext( VIDEO , (const char *)video_ext );
    
    //创建nowplaying list, 记录fullpath
    memset( &trive_field, 0x00, sizeof(trive_field) );
    trive_field.num=1; 
    trive_field.nfield[0] = FULLPATH;
    video_vars.nplist= plist_open(TBLNAME_VIDEO_NOWPLAYING, VIDEORESERVE1, &trive_field);
    if(NULL == video_vars.nplist)
    {
        print_err("open np_list err");
        return false;
    }
    
    return true;
}   

static bool _nplist_deinit(void)
{
    if( NULL != video_vars.nplist )
    {
        plist_close( video_vars.nplist );   
        video_vars.nplist = NULL; 
    }
    return true;
}

static bool _taglist_init( void )
{
    plist_trivefield_t trive_field;
    
    //创建taglist, 记录fullpath and time
    memset( &trive_field, 0x00, sizeof(trive_field) );
    trive_field.num=2; 
    trive_field.nfield[0] = FULLPATH;
    trive_field.nfield[1] = MARK;
    video_vars.taglist= plist_open("tag_list", VIDEORESERVE2, &trive_field);
    if(NULL == video_vars.taglist)
    {
        print_err("open tag_list err");
        return false;
    }
    
    return true;
}


static bool _recent_list_init( void )
{
#if VIDEO_CFG_RECENT_LIST_ENABLE > 0

    plist_trivefield_t trive_field;
    
    //创建recent file list, 记录fullpath and time
    memset( &trive_field, 0x00, sizeof(trive_field) );
    trive_field.num=2; 
    trive_field.nfield[0] = FULLPATH;
    trive_field.nfield[1] = MARK;
    video_vars.recent_list= plist_open("recent_list", VIDEORESERVE3, &trive_field);
    if(NULL == video_vars.recent_list)
    {
        print_err("open recent_list err");
        return false;
    }

#endif
    return true;
}

bool video_nplist_init( char * filepath )
{
    if( false == video_vars.flag_fsel_mode )
    {
        _fsel_deinit();
        return _nplist_init();
    }
    else
    {
        _nplist_deinit();
        return _fsel_init( filepath );
    }
}


bool video_playlist_init( void )
{
    bool result = true;


    video_vars.playlist_dir = sys_get_main_disk_dir(DIR_SYSTEM_PLAYLIST);
    if( NULL == video_vars.playlist_dir )
    {
        /*获取主盘上播放列表失败*/
        print_err("sys_get_main_disk_dir error");
//        return false;
    }
    else
    {
        /* 已经初始化, 则无需再初始化 */
        if ( NULL != video_vars.hplist )
        {
            return true;
        }
        /*设置数据库路径*/
        plist_set_rootdir(video_vars.playlist_dir);
    }
    if( plist_init() < 0 )
    {
        video_dialog_modal( VIDEO_DIALOG_PL_NOSP );        
        print_err("plist_init error!!\n");
//        return false;
    }
    
    video_vars.hplist = plist_open( NULL, MEDIALIB, NULL );
    if( NULL == video_vars.hplist )
    {
        print_err("video_vars.hplist is NULL!!\n");
//        return false;
    }       
    /*播放列表初始化失败退出应用，避免风险*/
/*    
    result = _nplist_init();
    if (false == result )
    {
        video_dialog_modal( VIDEO_DIALOG_PL_NOSP );
        return false;
    }
*/   
    result = _taglist_init();
    if (false == result )
    {
//        video_dialog_modal( VIDEO_DIALOG_PL_NOSP );
//        return false;
    }
    
    result = _recent_list_init();
    if (false == result )
    {
//        video_dialog_modal( VIDEO_DIALOG_PL_NOSP );
//        return false;
    }    
    
    loop_init();
    
#if 0
    为速度优化, 去掉如下检查功能
    bool result = true;
    int tag_num = 0;
    char tmp_fullpath[PATH_MAX];
    int i;
    
    //通过检查 nplist与config中的np内容, 决策是否显示np项菜单
    //index和fullpath中index优先, 通过index查找fullpath
    result = video_playlist_get_fullpath_by_index( video_vars.nplist, video_vars.plist.np_index, tmp_fullpath );
    if ( true == result )
    {
        //如果index对应的fullpath与config中的fullpath不同, 则说明u盘有修改, 则清除np菜单项
        if ( 0 != strcmp(tmp_fullpath, video_vars.plist.np_fullpath) )
        {
            video_nplist_clear_npinfo();
        }
    }
    //如果np文件不存在, 清空nplist, 清空np信息
    else
    {
        video_nplist_clear_npinfo();
    }
    
    //检查 taglist, 如果个数溢出删除多余tag
    tag_num = plist_get_item_num( video_vars.taglist );
    if ( tag_num > VIDEO_TAGLIST_MAX_NUM )
    {
        print_warning("video taglist item:%d, out of range", tag_num);
        for ( i=0; i<(tag_num-VIDEO_TAGLIST_MAX_NUM); i++ )
        {
            video_taglist_del_item( i );
        }
    }
#endif
    
    return true;
}


bool video_playlist_deinit( void )
{
    plist_close( video_vars.recent_list );
    video_vars.recent_list = NULL;
    
    plist_close( video_vars.taglist );
    video_vars.taglist = NULL;
    
    plist_close( video_vars.nplist );
    video_vars.nplist = NULL;
    
    plist_close( video_vars.hplist );
    video_vars.hplist = NULL;
    
    plist_quit();
    _fsel_deinit();
    return true;
}


int video_plist_get_item_num( void* listhandle )
{
    if( (s_fsel != NULL) && (true == video_vars.flag_fsel_mode) )
    {
        return fsel_getfilenum( s_fsel, FSEL_FILENUM );
    }
    else
    {
        return plist_get_item_num( listhandle );
    }    

}

bool video_fsel_get_item_index( char *fullpath ,int *index_cur )
{
    int index_tmp;
    int dirnum;
    if (NULL == fullpath)
    {
        return false;
    }

    dirnum = fsel_getfilenum( s_fsel, FSEL_DIRNUM);

    index_tmp = fsel_getidbyname(s_fsel, fullpath);
    
    if( index_tmp <= 0)
    {
        return false;
    }
    else
    {
        *index_cur = index_tmp - dirnum -1;
    }
    
    return true;
}
/******************************************************************************/
/*!                    
* \par  Description:
*      从指定的plist, 获取第index个 的全路径
* \param[in]    void *: 从指定的plist
* \param[in]    int: index号
* \param[out]   char *: 
* \retval       bool
*******************************************************************************/
bool video_playlist_get_fullpath_by_index( void *plist, int index_cur, char *fullpath )
{
    bool result = true;
    int tmp;
    fsel_filename_t file_path;
    plist_detail_info_t detail_info;
    
    if ( NULL == fullpath )
    {
        return false;
    }
    if( (true == video_vars.flag_fsel_mode) && (NULL != s_fsel) )
    {
        tmp = fsel_getfullpath(s_fsel, fullpath, index_cur+1);
        if ( tmp != -1 )
        {
            tmp = fsel_listfile(s_fsel, &file_path, index_cur+1, 1, FSEL_POSITIVE);
            if( tmp != -1 )
            {
                strcat( fullpath, file_path.name );
                print_dbg("fullpath=%s",fullpath );
            }
            else
            {
                print_warning("fsel_listfile err!\n");
            }
        } 
        else
        {
            print_err("fsel_getfullpath failed. \n index:%d ", index_cur );
            return false;
        }
    }
    else
    {
        tmp = plist_get_items_to_mallocbuf( plist, &detail_info, index_cur, 1, false );
        if ( (-1 == tmp) || (NULL == detail_info.fullpath) )
        {
            print_warning("plist_get_items_to_mallocbuf failed. index:%d", index_cur);
            return false;
        }
        
        strcpy( fullpath, detail_info.fullpath );

        plist_free_buf( &detail_info, 1 );


    }
    
    return result;
}


/******************************************************************************/
/*!                    
* \par  Description:
*      从指定的plist, 获取第index个 的filename
* \param[in]    void *: 从指定的plist
* \param[in]    int: index号
* \param[out]   char *: 
* \retval       bool
*******************************************************************************/
bool video_playlist_get_filename_by_index( void *plist, int index_cur, char *filename )
{
    bool result = true;
    int tmp;
    plist_detail_info_t detail_info;
    fsel_filename_t file_path;
    
    if ( NULL == filename )
    {
        return false;
    }

    if( (true == video_vars.flag_fsel_mode) && (NULL != s_fsel) )
    {
        tmp = fsel_listfile(s_fsel, &file_path, index_cur+1, 1, FSEL_POSITIVE);
        if( tmp != -1 )
        {
            strcpy( filename, file_path.name );
            print_dbg("filename=%s",filename );
        }
        else
        {
            print_warning("fsel_listfile err!\n");
        }
    }
    else
    {
        tmp = plist_get_items_to_mallocbuf( plist, &detail_info, index_cur, 1, false );
        if ( (-1 == tmp) || (NULL == detail_info.filename) || (NULL == detail_info.fullpath) )
        {
            print_warning("plist_get_items_to_mallocbuf failed. index_cur:%d", index_cur);
            return false;
        }
        
        strcpy( filename, detail_info.filename );
        
        plist_free_buf( &detail_info, 1 );
    }
    
    return result;
}




#if 0
#endif
/****************************** now playing list ******************************/
/******************************************************************************/
/*!                    
* \par  Description:
*      将 nplist中的第一个文件 置为np信息
*      用于播放结束, 退出playing时设置.
*      如:sequence模式播放完最后一曲后退出, 再进入时从第一个开始播放
* \retval       bool
*******************************************************************************/
bool video_nplist_set_to_first( void )
{
    video_vars.plist.np_index = 0;
    video_playlist_get_fullpath_by_index( video_vars.nplist, video_vars.plist.np_index, video_vars.plist.np_fullpath);
    memset( &video_vars.plist.np_tag, 0x00, sizeof(video_vars.plist.np_tag) );
    
    return true;
}

/******************************************************************************/
/*!                    
* \par  Description:
*      清空nplist, 清空np信息
* \retval       bool
*******************************************************************************/
bool video_nplist_clear_npinfo( void )
{
    plist_clear( video_vars.nplist );
    
    memset( video_vars.plist.np_fullpath, 0x00, sizeof(video_vars.plist.np_fullpath) );
    memset( &video_vars.plist.np_tag, 0x00, sizeof(video_vars.plist.np_tag) );
    video_vars.plist.np_index = -1;
    
    video_vars.flag_has_np_file = 0;
    
    return true;
}

bool video_add_taglist_to_nplist( int total_num )
{
    int result = 0;
    if( ( NULL == video_vars.taglist ) || ( NULL == video_vars.nplist ) )
    {
        print_err("list is NULL!\n");
        return false;
    }
    
    video_nplist_clear_npinfo();
    
    result = plist_transfer_items( video_vars.taglist, video_vars.nplist, -1, 0, total_num );
    if( result < 0 )
    {
        return false;
    }
    
    return true;
}


bool video_nplist_add_dirlist( dirlist_t *dirlist )
{
    int file_num ;
    FSEL_HANDLE  hdir;  

    if( NULL == video_vars.nplist )
    {
        print_err("video_vars.nplist is NULL!!\n");
        return false;
    }
   
    hdir = dirlist_get_fsel_handle( dirlist );
    if ( NULL == hdir )
    {
        return false;
    }
   
    file_num = plist_add_dirlist( video_vars.nplist, hdir, 0 );
    if ( file_num < 0 )
    {
        video_dialog_modal( VIDEO_DIALOG_PL_NOSP );        
        return false;
    }
    
    return true;
}


bool video_nplist_add_file( char *fullpath )
{
    bool result = true;
    int tmp = -1;

    if( NULL == video_vars.nplist )
    {
        print_err("video_vars.nplist is NULL!!\n");        
        return false;
    }
    
    tmp = plist_add_file( video_vars.nplist, fullpath);
    if ( -1 == tmp )
    {
        print_warning("plist_add_file err");
        video_dialog_modal( VIDEO_DIALOG_PL_NOSP );        
        result = false;
    }
    
    //更新loop参数
    result = video_set_play_mode( video_vars.play_param.playmode );
    if ( false == result )
    {
        print_warning("video_set_play_mode err");
    }
    
    return result;
}



/******************************************************************************/
/*!                    
* \par  Description:
*      从np_list中删除指定index的文件记录
* \par[in]      np_index:需要删除的记录的index, video_switch_mode:当前播放切换模式
* \par[out]     next_index:下一个要播放的文件的index
* \retval       bool 删除记录结果
*******************************************************************************/
bool video_nplist_del_item( int np_index, video_play_switch_mode_e video_switch_mode, int *next_index )
{
    bool result = true;
    int tmp = -1;
    char fullpath[PATH_MAX];
    loop_switch_e loop_switch_mode;
    loop_direct_e loop_dir_mode;

    if( (true == video_vars.flag_fsel_mode) && (NULL != s_fsel) )
    {
        video_playlist_get_fullpath_by_index( video_vars.nplist, np_index, fullpath );
        tmp = fsel_update( s_fsel, fullpath, FSEL_DELDATA );
        if( -1 == tmp )
        {
            print_warning("fsel delete item err\n");
            result = false;
        }
    }
    else
    {
        tmp = plist_del_items( video_vars.nplist, np_index, 1 );
        if ( -1 == tmp )
        {
            print_warning("plist_del_items err\n");
            result = false;
        }
    }
    
    switch( video_switch_mode )
    {
    case VIDEO_AUTO_PLAY:
        loop_switch_mode = LOOP_SWITCH_NORMAL ;
        loop_dir_mode = LOOP_DIRECT_NEXT;
        break;

    case VIDEO_FORCE_PLAY_NEXT:
        loop_switch_mode = LOOP_SWITCH_FORCELOOP;
        loop_dir_mode = LOOP_DIRECT_NEXT;
        break;
    
    case VIDEO_FORCE_PLAY_PREV:
        loop_switch_mode = LOOP_SWITCH_FORCELOOP;
        loop_dir_mode = LOOP_DIRECT_PREV;
        break;

    default:
        print_warning("video_switch_mode: %d\n",video_switch_mode);
        loop_switch_mode = LOOP_SWITCH_NORMAL;
        loop_dir_mode = LOOP_DIRECT_NEXT;            
        break;
    }

    //只有删除记录成功才更新loop，否则会导致loop和播放列表记录不对应
    if( true == result )
    {
        //更新loop参数
        tmp = loop_delete_index( np_index, loop_switch_mode, loop_dir_mode );
        if( NULL != next_index )
        {
            *next_index = tmp;
        }
    }
    return result;
}

/******************************************************************************/
/*!                    
* \par  Description:
*      根据fullpath, 循环遍历nplist, 删除所有item
* \param[in]    char *: fullpath
* \param[out]    int *: next_index,下一个要播放的文件的index
* \retval       bool
*******************************************************************************/
bool _nplist_del_items_by_fullpath( char *fullpath ,int *next_index )
{
    int i;
    int index_cur = -1;
    int total_num = 0;
    bool result = true;
    
    total_num = video_plist_get_item_num( video_vars.nplist );
    for ( i=0; i<total_num; i++ )
    {
        index_cur = plist_name_to_index( video_vars.nplist, fullpath, 0 );
        if ( -1 != index_cur )
        {
            result = video_nplist_del_item( index_cur, VIDEO_AUTO_PLAY, next_index );
        }
        else
        {
            break;
        }
    }
    
    return result;
}

/******************************************************************************/
/*!                    
* \par  Description:
*      根据fullpath, 删除物理文件并更新播放列表
* \param[in]    char *: fullpath
* \param[out]    int *: next_index,下一个要播放的文件的index
* \retval       bool
*******************************************************************************/
bool video_delete_npfile( char *fullpath, int *next_index )
{
    bool result = true;
    int index_cur = 0;
    int tmp = 0;
    FSEL_HANDLE  hdir;  
    
    //停止当前播放
    video_control( V_CMD_STOP, NULL );

    result = video_nplist_get_index_by_fullpath( fullpath, &index_cur );      
    if( false == result )
    {
        print_warning("get index of %s failed\n",fullpath);
        return false;
    }

    
    //删除物理文件
    tmp = remove( fullpath );
    if ( -1 == tmp )
    {
        print_warning("remove:%s failed", fullpath);
        return false;
    }
    
    //更新fsel
    hdir = dirlist_get_fsel_handle( video_vars.dirlist );
    if ( NULL != hdir )
    {
        fsel_update( hdir, fullpath, FSEL_DELDATA );
    }
    
    if( false == video_vars.flag_fsel_mode )
    {
        //更新nplist
        result = _nplist_del_items_by_fullpath( fullpath, next_index );
        //删除记录失败，loop不会有更新，返回的next_index不是有效值，需要获取下一个要播放的index
        if( false == result )
        {
            *next_index = video_get_next_play_index(LOOP_SWITCH_NORMAL);
        }        
    }
    else
    {
        fsel_update( s_fsel, fullpath, FSEL_DELDATA );
        //更新loop参数,删除文件一律按自动播放处理
        tmp = loop_delete_index( index_cur, LOOP_SWITCH_NORMAL, LOOP_DIRECT_NEXT );
        if( NULL != next_index )
        {
            *next_index = tmp;
        }

    }
    
    //更新taglist
    video_taglist_del_items_by_fullpath( fullpath );
    
    //更新recent list
    index_cur = plist_name_to_index( video_vars.recent_list, fullpath, 0 );
    if ( -1 != index_cur )
    {
        tmp = plist_del_items( video_vars.recent_list, index_cur, 1 );
    }
    
    return result;
}


bool video_nplist_get_index_by_fullpath( char *fullpath, int *np_index )
{
    int tmp = 0;
    bool result;
    
    if( (s_fsel != NULL) && (true == video_vars.flag_fsel_mode) )
    {
        result = video_fsel_get_item_index( fullpath, &tmp );
        if( false == result )
        {
            return false;
        }
    }
    else
    {
        tmp = plist_name_to_index( video_vars.nplist, fullpath, 0 );
        if ( -1 == tmp)
        {
            return false;
        }       
    }    

    *np_index = tmp;

    return true;
}

/******************************************************************************/
/*!                    
* \par  Description:
*      从nplist获取第np_index个的 信息, 并把np_tag信息一起, 去播放video
*      同时先检查此文件是否在: recent_list中, 用以提取tag播放时间
* \param[in]    int: nplist的index号
* \param[in]    mmm_video_tag_t *: 开始播放的tag信息. 
*               NULL:从recent list中寻找tag时间; 非NULL: 从tag参数处播放
*
* \retval       bool
*******************************************************************************/
bool video_nplist_setplay_by_index( int np_index, mmm_video_tag_t *np_tag )
{
    bool result = true;
    unsigned int time_recent_list;
    mmm_video_tag_t tag_recent_list = {0};
    mmm_video_file_info_t file_info = {0};
    char fullpath[PATH_MAX] = {0};
    mmm_video_area_t video_area;

    if( np_index < 0 )
    {
        print_err("index error: %d\n",np_index );
        return false;
    }
    //通过index, 从plist获得fullpath
    result = video_playlist_get_fullpath_by_index( video_vars.nplist, np_index, fullpath );
    if ( false == result )
    {
        print_err("video_playlist_get_fullpath_by_index failed");
        return false;
    }
    
    //获得fullpath成功. 停止当前播放, 设置新fullpath
    video_control( V_CMD_STOP, NULL );
    result = video_control(V_CMD_SET_FILE, fullpath);
    if ( false == result )
    {
        print_err("SET_FILE:%s, failed", fullpath);
        return false;
    }
    //获取总时间, 必须在V_CMD_SET_FILE命令之后
    result = video_control(V_CMD_GET_MEDIA_INFO, &file_info);
    if ( result == true )
    {
        //shaofeng 中间件bug
        video_vars.scene_playing.total_time = file_info.total_time;
    }
    
    //设置video的yuv数据的显示区域, 否则只显示rgb数据, 没有播放界面数据
    //在play之前
    video_area.xpos = 0;
    video_area.ypos = 0;
    video_area.width = (unsigned int)video_vars.scene_playing.app_scene.scene->width;
    video_area.height = (unsigned int)video_vars.scene_playing.app_scene.scene->height;
    video_control(V_CMD_SET_VIDEO_AREA, &video_area);
    
    /*播放文件*/
    if ( NULL == np_tag )
    {
        /*tag为空, 说明不是从taglist进入, 需要查找最近是否播放过*/
        result = video_recent_list_get_tag_by_fullpath( fullpath, &time_recent_list );
        if ( true == result )
        {
            tag_recent_list.time = time_recent_list;
            result = video_control(V_CMD_PLAY, &tag_recent_list );
            video_vars.scene_playing.cur_time = time_recent_list;
        }
        else
        {
            result = video_control(V_CMD_PLAY, NULL );
            video_vars.scene_playing.cur_time = 0;
        }

    }
    else
    {
        result = video_control(V_CMD_PLAY, np_tag );
        video_vars.scene_playing.cur_time = np_tag->time;
    }
    if ( true == result )
    {
        memset( video_vars.plist.np_fullpath, 0x00, sizeof(video_vars.plist.np_fullpath) );
        strcpy( video_vars.plist.np_fullpath, fullpath );
        video_vars.plist.np_index = np_index;
    }
    print_dbg("index:%d, fullpath:%s", np_index, video_vars.plist.np_fullpath);
    
    return result;
}



#if 0
#endif
/*********************************** tag list *********************************/
/******************************************************************************/
/*!                    
* \par  Description:
*      从nplist获取第np_index个的 信息, 并把time信息一起加入tag
* \param[in]    int: nplist的index号
* \param[out]   int: 时间, 单位:ms
* \retval       bool
*******************************************************************************/
bool video_taglist_add_item( int np_index, unsigned int time )
{
    int tmp = -1;
    plist_detail_info_t detail_info;
    char fullpath[PATH_MAX];
    char file_name[PATH_MAX];

    if( NULL == video_vars.taglist )
    {
        video_dialog_create( VIDEO_DIALOG_ADD_FAIL, NULL );
        print_err("video_vars.taglist is NULL!!\n");       
        return false;
    }    

    //初始化
    memset( &detail_info, 0x00, sizeof(detail_info) );
    //初始化结构体中的指针
    plist_init_detail_info_struct( &detail_info,1 );

    if( false == video_vars.flag_fsel_mode )
    {
        //从nplist获取此tag除time外的信息
        tmp = plist_get_items_to_mallocbuf( video_vars.nplist, &detail_info, np_index, 1, false );
        if ( -1 != tmp )
        {
            //增加time信息
            detail_info.markinfo.time = time;
            //add TAG
            tmp = plist_add_item( video_vars.taglist, &detail_info );
        }
    }
    else
    {
        detail_info.markinfo.time = time;
        video_playlist_get_fullpath_by_index( video_vars.nplist, np_index, fullpath );
        video_playlist_get_filename_by_index( video_vars.nplist, np_index, file_name);

        detail_info.fullpath = fullpath;
        detail_info.filename = file_name;
        //add TAG
        tmp = plist_add_item( video_vars.taglist, &detail_info );      

    }
    
    plist_free_buf( &detail_info, 1 );
    
    if ( -1 == tmp )
    {
        video_dialog_create( VIDEO_DIALOG_ADD_FAIL, NULL );
        return false;
    }
    return true;
}


/******************************************************************************/
/*!                    
* \par  Description:
*      从taglist删除第index个
* \param[in]    int: index号
* \retval       bool
*******************************************************************************/
bool video_taglist_del_item( int tag_index )
{
    int tmp = -1;
    
    tmp = plist_del_items( video_vars.taglist, tag_index, 1 );
    if ( -1 == tmp )
    {
        return false;
    }
    return true;
}

/******************************************************************************/
/*!                    
* \par  Description:
*      根据fullpath, 循环遍历taglist, 删除所有item
* \param[in]    char *: fullpath
* \retval       bool
*******************************************************************************/
bool video_taglist_del_items_by_fullpath( char *fullpath )
{
    int i;
    int index_cur = -1;
    
    for ( i=0; i<VIDEO_TAGLIST_MAX_NUM; i++ )
    {
        index_cur = plist_name_to_index( video_vars.taglist, fullpath, 0 );
        if ( -1 != index_cur )
        {
            plist_del_items( video_vars.taglist, index_cur, 1 );
        }
        else
        {
            break;
        }
    }
    
    return true;
}

/******************************************************************************/
/*!                    
* \par  Description:
*      从taglist, 获取第index个的 tag数据信息
* \param[in]    int: 从taglist获取的index号
* \param[out]    video_tag_info_t *: tag数据 数组指针
* \retval       bool
*******************************************************************************/
bool video_taglist_get_tag_info( int tag_index, video_tag_info_t *tag_info )
{
    bool result = true;
    int tmp;
    plist_detail_info_t detail_info;
    
    if ( NULL == tag_info )
    {
        return false;
    }
    
    tmp = plist_get_items_to_mallocbuf( video_vars.taglist, &detail_info, tag_index, 1, false );
    if ( (-1 == tmp) || (NULL == detail_info.filename) || (NULL == detail_info.fullpath) )
    {
        print_warning("plist_get_items_to_mallocbuf failed. tag_index:%d", tag_index);
        return false;
    }
    
    strcpy( tag_info->file_name, detail_info.filename );
    tag_info->time = detail_info.markinfo.time;
    
    plist_free_buf( &detail_info, 1 );
    
    return result;
}




#if 0
#endif
/***************************** recent file list *******************************/
/******************************************************************************/
/*!                    
* \par  Description:
*      遍历recent file list, 查找是否有此fullpath文件, 有 则获取此文件的time信息
* \param[in]    int: 
* \param[out]   int: 时间, 单位:ms
* \retval       bool
*******************************************************************************/
bool video_recent_list_get_tag_by_fullpath( char *fullpath, unsigned int *time )
{
#if VIDEO_CFG_RECENT_LIST_ENABLE > 0
    int tmp = -1;
    int index_cur;
    plist_detail_info_t detail_info;

    /*如果断点播放关闭，直接返回*/
    if( false == video_vars.flag_resume )
    {
        return false;
    }

    if( NULL == video_vars.recent_list )
    {
        return false;
    }
    
    index_cur = plist_name_to_index( video_vars.recent_list, fullpath, 0 );
    if ( -1 == index_cur)
    {
        return false;
    }
    
    memset( &detail_info, 0x00, sizeof(detail_info) );
    tmp = plist_get_items_to_mallocbuf( video_vars.recent_list, &detail_info, index_cur, 1, false );
    if ( -1 != tmp )
    {
        *time = detail_info.markinfo.time;
    }
    plist_free_buf( &detail_info, 1 );
    
    if ( -1 == tmp )
    {
        return false;
    }
    else
    {
        return true;
    }
#else
    return false;
#endif
}

/******************************************************************************/
/*!                    
* \par  Description:
*      从nplist中, 将np项添加到
*      保存最近播放过而没播放完的10个文件的time信息, 文件只存在一份, 不会重复
*      list个数最大:RECENT_LIST_MAX_ITEM_NUM, 超过删除最前项, append新内容到末尾
* \param[in]    int: nplist的np index号
* \param[in]    int: np文件时间, 单位:ms
* \retval       bool
*******************************************************************************/
bool video_recent_list_add_item( int np_index, unsigned int time )
{
#if VIDEO_CFG_RECENT_LIST_ENABLE > 0
    bool result = true;
    int tmp = -1;
    int file_num = 0;
    int index_cur;
    plist_detail_info_t detail_info;
    char fullpath[PATH_MAX];
    char file_name[PATH_MAX];

    if( NULL == video_vars.recent_list )
    {
        print_err("video_vars.recent_list is NULL!!\n");       
        return false;
    }       
    /*如果断点播放使能关闭，不记录直接返回*/
    if( false == video_vars.flag_resume )
    {
        return false;
    }
    
    //初始化
    memset( &detail_info, 0x00, sizeof(detail_info) );
    //初始化结构体中的指针
    plist_init_detail_info_struct( &detail_info,1 );

    if( false == video_vars.flag_fsel_mode )
    {
        //从recent_list获取此tag除time外的信息
        tmp = plist_get_items_to_mallocbuf( video_vars.nplist, &detail_info, np_index, 1, false );
        if ( -1 == tmp )
        {
            goto VIDEO_RECENT_LIST_ADD_ITEM_ERR;
        }
    }
    else
    {
        video_playlist_get_fullpath_by_index(video_vars.nplist, np_index, fullpath);
        video_playlist_get_filename_by_index( video_vars.nplist, np_index, file_name);
        
        detail_info.fullpath = fullpath;
        detail_info.filename = file_name;
    }
       
    index_cur = plist_name_to_index( video_vars.recent_list, detail_info.fullpath, 0 );
    //此fullpath文件已经在recent list中, 则删除, 再添加
    if ( -1 != index_cur )
    {
        plist_del_items( video_vars.recent_list, index_cur, 1 );
    }
    
    //已经是最大个数, 删除第一个, 再添加
    file_num = plist_get_item_num( video_vars.recent_list );
    if ( file_num >= RECENT_LIST_MAX_ITEM_NUM )
    {
        tmp = plist_del_items( video_vars.recent_list, 0, 1 );
        if ( -1 == tmp )
        {
            print_warning("delete recent_list first item failed");
            goto VIDEO_RECENT_LIST_ADD_ITEM_ERR;
        }
    }
    
    //add TAG

    detail_info.markinfo.time = time;
    
    tmp = plist_add_item( video_vars.recent_list, &detail_info );
    
    VIDEO_RECENT_LIST_ADD_ITEM_ERR:
    plist_free_buf( &detail_info, 1 );
    
    if ( -1 == tmp )
    {
//        video_dialog_create( VIDEO_DIALOG_PL_NOSP, NULL );
        return false;
    }
    return result;
#else
    return false;
#endif
}

/******************************************************************************/
/*!                    
* \par  Description:
*      从nplist中, 将np项从最近播放而没有播放完的文件列表中删除
*      list个数最大:RECENT_LIST_MAX_ITEM_NUM, 超过删除最前项, append新内容到末尾
* \param[in]    int: nplist的np index号
* \retval       bool
*******************************************************************************/
bool video_recent_list_delete_item( int np_index )
{
#if VIDEO_CFG_RECENT_LIST_ENABLE > 0
    bool result = true;
    int tmp = -1;
    int index_cur;
    plist_detail_info_t detail_info;
    char fullpath[PATH_MAX];

    /*如果断点播放使能关闭，直接返回*/
    if( false == video_vars.flag_resume )
    {
        return false;
    }

    if( NULL == video_vars.recent_list )
    {
        print_err("video_vars.recent_list is NULL!!\n");       
        return false;
    }       

    
    memset( &detail_info, 0x00, sizeof(detail_info) );
    //从recent_list获取此tag除time外的信息
    if( false == video_vars.flag_fsel_mode )
    {
        tmp = plist_get_items_to_mallocbuf( video_vars.nplist, &detail_info, np_index, 1, false );
        if ( -1 == tmp )
        {
            goto VIDEO_RECENT_LIST_ADD_ITEM_ERR;
        }
    }
    else
    {
        video_playlist_get_fullpath_by_index(video_vars.nplist, np_index, fullpath);
        
        detail_info.fullpath = fullpath;
    }  

    index_cur = plist_name_to_index( video_vars.recent_list, detail_info.fullpath, 0 );
    if( -1 != index_cur )
    {
        //删除resent中的文件
        plist_del_items( video_vars.recent_list, index_cur, 1 );
    }
       
    VIDEO_RECENT_LIST_ADD_ITEM_ERR:
    plist_free_buf( &detail_info, 1 );
    
    if ( -1 == tmp )
    {
        return false;
    }
    return result;
#else
    return false;
#endif
}

