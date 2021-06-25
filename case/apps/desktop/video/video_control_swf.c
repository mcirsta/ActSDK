/*******************************************************************************
 *                              NOYA1100
 *                            Module: video
 *                 Copyright(c) 2003-2008 Actions Semiconductor,
 *                            All Rights Reserved. 
 * History:         
 *      <author>    <time>           <version >             <desc>
 *      shaofeng    2009-09-02          1.0             build this file 
*******************************************************************************/
/*!
 * \file     video_control_swf.c
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


static void * (*swfdec_open) (void *param);
static int    (*swfdec_close) (void *player);
static int    (*swfdec_cmd) (void * player, int cmd, void * param);


/* define variable, 变量定义 */
static void *s_swf_handle = NULL;
static void *s_swf_lib = NULL;





/******************************************************************************/
/*!                    
* \par  Description:
*       对video中间件控制, 采用集中式, 所有的对中间件操作都通过此函数完成
* \param[in]    v_cmd_type_e:
* \param[in]    void *: cmd参数, 需要都以指针的形式传入, 不要直接传入数值
* \retval       true:成功; false:失败
*******************************************************************************/
bool swf_control( v_cmd_type_e v_cmd_type, void * param )
{
    bool result = true;
    int tmp = 1;
    
    if (NULL == s_swf_handle)
    {
        print_err("s_swf_handle is NULL");
        return false;
    }

    switch( v_cmd_type )
    {
    case  V_CMD_SET_FILE:
        if ( NULL == param )
        {
            print_err("param is NULL");
            result = false;
            break;
        }
        tmp = swfdec_cmd(s_swf_handle, MMM_SET_FILE, param);
        break;
        
    case V_CMD_PLAY:
        tmp = swfdec_cmd(s_swf_handle, MMM_PLAY, param);
        break;
        
    case V_CMD_STOP:
        tmp = swfdec_cmd(s_swf_handle, MMM_STOP, param);
        break;
    case V_CMD_GET_PLAYER_STATUS:
        if ( NULL == param )
        {
            print_err("param is NULL");
            result = false;
            break;
        }
        tmp = swfdec_cmd(s_swf_handle, MMM_GET_PLAYER_STATUS, param );
        break;
#if 0        
    case V_CMD_PAUSE:
        tmp = swfdec_cmd(s_swf_handle, MMM_PAUSE, (unsigned int)NULL);
        break;
        
    case V_CMD_RESUME:
        tmp = swfdec_cmd(s_swf_handle, MMM_RESUME, (unsigned int)NULL);
        break;
        
    case V_CMD_GET_MEDIA_INFO:
        if ( NULL == param )
        {
            print_err("param is NULL");
            result = false;
            break;
        }
        tmp = swfdec_cmd(s_swf_handle, MMM_GET_MEDIA_INFO, (unsigned int)param);
        break;
        
#endif        
    default:
        break;
    }/* end switch( v_cmd_type ) */

    if(tmp < 0 )
    {        
        print_warning("cmd:%d, failed", v_cmd_type);   
        result = false;
    }
    return result;
}


/******************************************************************************/
/*!                    
* \par  Description:
*       video中间件初始化
*       主要是装载 swf解码库
* \param[in]    
* \retval       true:成功; false:失败
*******************************************************************************/
bool video_mmm_swf_init( void )
{    
    if( NULL != s_swf_handle )
    {
        return true;
    }
    
    s_swf_lib = dlopen("swfdec.so", RTLD_LAZY);
    if(NULL == s_swf_lib)
    {
        print_err("dlopen swfdec error\n");
        return false;
    }
    swfdec_open     = dlsym(s_swf_lib, "swfdec_open");
    swfdec_close    = dlsym(s_swf_lib, "swfdec_close");
    swfdec_cmd      = dlsym(s_swf_lib, "swfdec_cmd");
    if ( (NULL == swfdec_open) || (NULL == swfdec_close) || (NULL == swfdec_cmd) )
    {
        return false;
    }
    
    s_swf_handle = swfdec_open(NULL);
    if(NULL == s_swf_handle)
    {
        print_err("mmm_swf_open error\n");
        return false;
    }
    
    return true;
}

bool video_mmm_swf_deinit( void )
{
    if ( NULL != s_swf_handle )
    {
        swfdec_close( s_swf_handle );
        s_swf_handle = NULL;
    }
    
    if ( NULL != s_swf_lib )
    {
        dlclose( s_swf_lib );
        s_swf_lib = NULL;
    }

    return true;
}

/******************************************************************************/
/*!                    
* \par  Description:
*      从nplist获取第np_index个的 信息, 去播放swf
* \param[in]    int: nplist的index号
* \retval       bool
*******************************************************************************/
bool video_nplist_setplay_swf_by_index( int np_index )
{
    bool result = true;
    char fullpath[PATH_MAX] = {0};
    
    //通过index, 从plist获得fullpath
    result = video_playlist_get_fullpath_by_index( video_vars.nplist, np_index, fullpath );
    if ( false == result )
    {
        print_err("video_playlist_get_fullpath_by_index failed");
        return false;
    }
    
    //获得fullpath成功. stop, 然后设置新fullpath
    swf_control( V_CMD_STOP, NULL );
    
    result = swf_control(V_CMD_SET_FILE, fullpath);
    if ( false == result )
    {
        print_err("SET_FILE:%s, failed", fullpath);
        return false;
    }
    
    result = swf_control(V_CMD_PLAY, NULL );
    if ( true == result )
    {
        memset( video_vars.plist.np_fullpath, 0x00, sizeof(video_vars.plist.np_fullpath) );
        strcpy( video_vars.plist.np_fullpath, fullpath );
        video_vars.plist.np_index = np_index;
    }
    print_dbg("index:%d, fullpath:%s", np_index, video_vars.plist.np_fullpath);
    
    return result;
}

