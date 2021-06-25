/*******************************************************************************
 *                              NOYA1100
 *                            Module: video
 *                 Copyright(c) 2003-2008 Actions Semiconductor,
 *                            All Rights Reserved. 
 * History:         
 *      <author>    <time>           <version >             <desc>
 *      shaofeng    2009-04-01          1.0             build this file 
*******************************************************************************/
/*!
 * \file     video_control.c
 * \brief    
 * \author   shaofeng
 * \par      GENERAL DESCRIPTION:
 *               
 * \par      EXTERNALIZED FUNCTIONS:
 *               
 * \version 1.0
 * \date  2009/04/01
*******************************************************************************/

#include "include_psp.h"
#include "include_case.h"

/* include file of this application, 本应用的头文件 */
#include "include_video.h"




/* define variable, 变量定义 */
static void *s_vd_handle = NULL;
static void *s_v_decode_lib = NULL;





/******************************************************************************/
/*!                    
* \par  Description:
*       对video中间件控制, 采用集中式, 所有的对中间件操作都通过此函数完成
* \param[in]    v_cmd_type_e:
* \param[in]    void *: cmd参数, 需要都以指针的形式传入, 不要直接传入数值
* \retval       true:成功; false:失败
*******************************************************************************/
bool video_control( v_cmd_type_e v_cmd_type, void * param )
{
    bool result = true;
    int tmp = 1;
    
//    print_info("cmdtype:%d", v_cmd_type);
    if (NULL == s_vd_handle)
    {
        print_err("s_vd_handle is NULL");
        return false;
    }

    switch( v_cmd_type )
    {
    /* message of mmm_image_video_cmd_t */
    case V_CMD_SET_SCREEN_MODE:
        {
            rotation_e rotation = *(rotation_e *)param;
            tmp = mmm_vd_cmd(s_vd_handle, MMM_SET_SCREEN_ROTATE, (unsigned int)rotation);
            print_dbg("rotation:%d", rotation);
        }
        break;
        
    /* message of mmm_dec_cmd_t*/
    case V_CMD_PLAY:
        tmp = mmm_vd_cmd(s_vd_handle, MMM_PLAY, (unsigned int)param);
        break;
        
    case V_CMD_STOP:
        {
            mmm_video_tag_t tag_nouse;
            //不需要记录tag信息, 则使用tag_nouse
            if ( NULL == param )
            {
                tmp = mmm_vd_cmd(s_vd_handle, MMM_STOP, (unsigned int)&tag_nouse);
            }
            else
            {
                tmp = mmm_vd_cmd(s_vd_handle, MMM_STOP, (unsigned int)param);
            }
        }
        break;
        
    case V_CMD_PAUSE:
        tmp = mmm_vd_cmd(s_vd_handle, MMM_PAUSE, (unsigned int)NULL);
        break;
        
    case V_CMD_RESUME:
        tmp = mmm_vd_cmd(s_vd_handle, MMM_RESUME, (unsigned int)NULL);
        break;
        
    case V_CMD_SEEK:
        tmp = mmm_vd_cmd(s_vd_handle, MMM_SEEK, *(unsigned int *)param);
        break;
        
    case V_CMD_FAST_FORWARD:
        {
            unsigned  int  fast_forward_step = 0;
            memcpy(&fast_forward_step, param, sizeof(fast_forward_step));
            print_dbg("fast_forward_step:%d", fast_forward_step);
            if(fast_forward_step > MAX_FFB_STEP)
            {
                fast_forward_step = MAX_FFB_STEP;
            }
            
            tmp = mmm_vd_cmd(s_vd_handle, MMM_FAST_FORWARD, (unsigned int)fast_forward_step);
        }
        break;
        
    case V_CMD_FAST_BACKWARD:
        {
            unsigned  int  fast_backward_step = 0;
            memcpy(&fast_backward_step, param, sizeof(fast_backward_step));
            print_dbg("fast_backward_step:%d", fast_backward_step);
            if(fast_backward_step > MAX_FFB_STEP)
            {
                fast_backward_step = MAX_FFB_STEP;
            }
            
            tmp = mmm_vd_cmd(s_vd_handle, MMM_FAST_BACKWARD, (unsigned int)fast_backward_step);
        }
        break;
        
    case V_CMD_CANCEL_FFB:
        tmp = mmm_vd_cmd(s_vd_handle, MMM_CANCEL_FFB, (unsigned int)NULL);
        break;
        
#if 0
    //此接口目前保留, 设置step在 FAST_BACKWARD的参数中设置
    case V_CMD_SET_FFB_STEP:
        {
            unsigned  int  fast_ffb_step = 0;
            memcpy(&fast_ffb_step, param, sizeof(fast_ffb_step));
            print_dbg("ffb_step:%d", fast_ffb_step);
            if(fast_ffb_step > MAX_FFB_STEP)
            {
                print_warning("");
                fast_ffb_step = MAX_FFB_STEP;
            }
            
            tmp = mmm_vd_cmd(s_vd_handle, MMM_SET_FFB_STEP, (unsigned int)fast_ffb_step);
        }
        break;
#endif
        
    case  V_CMD_SET_FILE:
        if ( NULL == param )
        {
            print_err("param is NULL");
            result = false;
            break;
        }
        tmp = mmm_vd_cmd(s_vd_handle, MMM_SET_FILE, (unsigned int)param);
        break;
        
    case V_CMD_MAKE_MARK:
        if ( NULL == param )
        {
            print_err("param is NULL");
            result = false;
            break;
        }
        tmp = mmm_vd_cmd(s_vd_handle, MMM_MAKE_MARK, (unsigned int)param);
        break;
        
    case V_CMD_GET_MEDIA_INFO:
        if ( NULL == param )
        {
            print_err("param is NULL");
            result = false;
            break;
        }
        tmp = mmm_vd_cmd(s_vd_handle, MMM_GET_MEDIA_INFO, (unsigned int)param);
        break;
        
    case V_CMD_GET_PLAYER_STATUS:
        if ( NULL == param )
        {
            print_err("param is NULL");
            result = false;
            break;
        }
        tmp = mmm_vd_cmd(s_vd_handle, MMM_GET_PLAYER_STATUS, (unsigned int)param);
        break;
        
    case V_CMD_SET_A:
        tmp = mmm_vd_cmd(s_vd_handle, MMM_SET_A, (unsigned int)NULL);
        break;
        
    case V_CMD_SET_B:
        tmp = mmm_vd_cmd(s_vd_handle, MMM_SET_B, (unsigned int)NULL);
        break;
        
    case V_CMD_CLEAR_AB:
        tmp = mmm_vd_cmd(s_vd_handle, MMM_CLEAR_AB, (unsigned int)NULL);
        break;
        
    case V_CMD_SET_AB_COUNT:
        {
            unsigned int ab_count = *(unsigned int *)param;
            tmp = mmm_vd_cmd(s_vd_handle, MMM_SET_AB_COUNT, ab_count);
            print_dbg("set ab_count:%d", ab_count);
        }
        break;
        
    case V_CMD_SET_AB_WAIT_TIME:
        {
            unsigned int ab_wait_time = *(unsigned int *)param;
            tmp = mmm_vd_cmd(s_vd_handle, MMM_SET_AB_WAIT_TIME, ab_wait_time);
            print_dbg("set wait_time:%d", ab_wait_time);
        }
        break;
        
    case V_CMD_GET_DRM_INFO:
        break;
        
    case V_CMD_MEM_MODE:
        {
            mmm_video_mem_mode_t mem_mode = *(mmm_video_mem_mode_t *)param;
            tmp = mmm_vd_cmd(s_vd_handle, MMM_VIDEO_MEM_MODE, mem_mode);
            print_dbg("set mem_mode:%d", mem_mode);
        }
        break;




    /* message of mmm_video_cmd_t*/
    case V_CMD_SET_VIDEO_AREA:
        tmp = mmm_vd_cmd(s_vd_handle, MMM_SET_VIDEO_AREA, (unsigned int)param);
        break;

    case V_CMD_BLENDING_AREA_CREATE:
        {
            mmm_blending_create_t *blend = NULL;
            
            if ( NULL == param )
            {
                print_err("param is NULL");
                result = false;
                break;
            }
            blend = (mmm_blending_create_t *)param;
            tmp = mmm_vd_cmd(s_vd_handle, MMM_BLENDING_AREA_CREATE, (unsigned int)blend);
            if( (blend->handle < 0) || (tmp < 0) )
            {        
                print_err("blending area create err");   
                result = false;
            }
        }
        break;

    case V_CMD_BLENDING_AREA_SHOW:
        tmp = mmm_vd_cmd(s_vd_handle, MMM_BLENDING_AREA_SHOW, (unsigned int)param);
        break;
    case V_CMD_BLENDING_AREA_HIDE:
        tmp = mmm_vd_cmd(s_vd_handle, MMM_BLENDING_AREA_HIDE, (unsigned int)param);
        break;
    case V_CMD_BLENDING_AREA_SETATTR:
        tmp = mmm_vd_cmd(s_vd_handle, MMM_BLENDING_AREA_SETATTR, (unsigned int)param);
        break;
    case V_CMD_BLENDING_AREA_DESTROY:
        tmp = mmm_vd_cmd(s_vd_handle, MMM_BLENDING_AREA_DESTROY, *(unsigned int *)param);
        break;
        
    case V_CMD_PRINT_SCREEN:
        if ( NULL == param )
        {
            print_err("param is NULL");
            result = false;
            break;
        }
        tmp = mmm_vd_cmd(s_vd_handle, MMM_PRINT_SCREEN, (unsigned int)param);
        break;
        
    case V_CMD_SET_DISPLAY_DEV:
        tmp = mmm_vd_cmd(s_vd_handle, MMM_SET_DISPLAY_DEV, (unsigned int)*(mmm_video_dev_t *)param);
        break;

    case V_CMD_SET_DISPLAY_MODE:
        tmp = mmm_vd_cmd(s_vd_handle, MMM_SET_VIDEO_RATIO, (unsigned int)*(mmm_video_ratio_t *)param);
        break;
    
        
    case V_CMD_SHOW_NEW_FRAME:
        tmp = mmm_vd_cmd(s_vd_handle, MMM_SHOW_NEW_FRAME, (unsigned int)NULL);
        break;
        



    /* message of mmm_kshow_cmd_t */
    case V_CMD_KSHOW_SWITCH_CHANNEL:
        tmp = mmm_vd_cmd(s_vd_handle, MMM_KSHOW_SWITCH_CHANNEL, *(unsigned int *)param);
        break;
        
    case V_CMD_KSHOW_ENABLE_VOCALIZE:
        tmp = mmm_vd_cmd(s_vd_handle, MMM_KSHOW_ENABLE_VOCALIZE, (unsigned int)NULL);
        break;
        
    case V_CMD_KSHOW_SET_MIX_PROPORTION:
        tmp = mmm_vd_cmd(s_vd_handle, MMM_KSHOW_SET_MIX_PROPORTION, (unsigned int)*(int *)param);
        break;
        
    case V_CMD_KSHOW_DISABLE_VOCALIZE:
        tmp = mmm_vd_cmd(s_vd_handle, MMM_KSHOW_DISABLE_VOCALIZE, (unsigned int)NULL);
        break;
        
    case V_CMD_KSHOW_START_RECORDING:
        tmp = mmm_vd_cmd(s_vd_handle, MMM_KSHOW_DISABLE_VOCALIZE, (unsigned int)NULL);
        break;
        
    case V_CMD_KSHOW_STOP_RECORDING:
        tmp = mmm_vd_cmd(s_vd_handle, MMM_KSHOW_STOP_RECORDING, (unsigned int)NULL);
        break;
        
    case V_CMD_KSHOW_SET_TITLE_FILE:
        tmp = mmm_vd_cmd(s_vd_handle, MMM_KSHOW_SET_TITLE_FILE, (unsigned int)param);
        break;
        
    case V_CMD_KSHOW_ENABLE_TITLE_DISPLAY:
        tmp = mmm_vd_cmd(s_vd_handle, MMM_KSHOW_ENABLE_TITLE_DISPLAY, *(unsigned int *)param);
        break;
        
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
*       主要是装载 video解码库
* \param[in]    
* \retval       true:成功; false:失败
*******************************************************************************/
bool video_mmm_init( void )
{    
    if ( NULL != s_vd_handle )
    {
        return true;
    }
    
    s_v_decode_lib = dlopen("v_codec.so", RTLD_LAZY);
    s_vd_handle = mmm_vd_open(NULL);
    if(s_vd_handle == NULL)
    {
        print_err("mmm_vd_open error\n");
        return false;
    }
    
    return true;
}

bool video_mmm_deinit( void )
{
    if ( NULL != s_vd_handle )
    {
        video_control( V_CMD_STOP, NULL );
        mmm_vd_close( s_vd_handle, NULL );
        s_vd_handle = NULL;
    }
    
    if ( NULL != s_v_decode_lib )
    {
        dlclose( s_v_decode_lib );
        s_v_decode_lib = NULL;
    }
    
    return true;
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
bool video_control_get_total_time_by_fullpath( char *fullpath, unsigned int *total_time )
{
    bool result = true;
    mmm_video_file_info_t file_info;
    
    video_control( V_CMD_STOP, NULL );
    result = video_control(V_CMD_SET_FILE, fullpath);
    if ( true == result )
    {
        memset( &file_info, 0x00, sizeof(file_info) );
        result = video_control(V_CMD_GET_MEDIA_INFO, &file_info);
        if ( result == true )
        {
            *total_time = file_info.total_time;
        }
    }
    
    return result;
}


/******************************************************************************/
/*!                    
* \par  Description:
*      设置video解码缩略图和播放的内存模式
* \param[in]    void *: 从指定的plist
* \param[in]    int: index号
* \param[out]   char *: 
* \retval       bool
*******************************************************************************/
bool video_control_set_mem_mode( mmm_video_mem_mode_t mem_mode )
{
    bool result = true;
    
    result = video_control( V_CMD_MEM_MODE, &mem_mode );
    if ( false == result )
    {
        video_mmm_deinit();
    }
    
    return result;
}

