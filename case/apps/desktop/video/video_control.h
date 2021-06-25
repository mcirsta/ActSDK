/********************************************************************************
 *                              USDK(1100)
 *                             Module: app 
 *                 Copyright(c) 2003-2009 Actions Semiconductor,
 *                            All Rights Reserved. 
 * History:         
 *      <author>    <time>           <version >             <desc>
 *      shaofeng    2009-04-01          v1.0               build this file 
 ********************************************************************************/
/*!
 * \file     video_control.h
 * \brief    
 * \author   shaofeng
 *               
 * \version 1.0
 * \date  2009/04/01
*******************************************************************************/

#ifndef   __VIDEO_CONTROL_H__
#define   __VIDEO_CONTROL_H__

#include "include_case.h"
#include "include_psp.h"


/*!
*\brief
    对video与mmm交互的所有命令进行了一次封装, 并汇总mmm的各命令 */
typedef enum
{
    V_CMD_BASE        = 0,
    
    /* this cmd from mmm_image_video_cmd_t of mmm_decoder.h */
    V_CMD_SET_SCREEN_MODE = 100,
    
    /* this cmd from mmm_dec_cmd_t of mmm_decoder.h */
    V_CMD_PLAY = 200,
    V_CMD_STOP,
    V_CMD_PAUSE,
    V_CMD_RESUME,
    V_CMD_SEEK,                 //204
    V_CMD_FAST_FORWARD,
    V_CMD_FAST_BACKWARD,
    V_CMD_CANCEL_FFB,
    V_CMD_SET_FFB_STEP,
    V_CMD_SET_FILE,             //209
    V_CMD_MAKE_MARK,            //210
    V_CMD_GET_MEDIA_INFO,
    V_CMD_GET_PLAYER_STATUS,
    V_CMD_SET_A,
    V_CMD_SET_B,
    V_CMD_CLEAR_AB,
    V_CMD_SET_AB_COUNT,
    V_CMD_SET_AB_WAIT_TIME,
    V_CMD_GET_DRM_INFO,
    V_CMD_MEM_MODE,
    
    /* this cmd from mmm_video_cmd_t of mmm_decoder.h */
    V_CMD_SET_VIDEO_AREA = 300,
    V_CMD_BLENDING_AREA_CREATE,
    V_CMD_BLENDING_AREA_SHOW,
    V_CMD_BLENDING_AREA_HIDE,
    V_CMD_BLENDING_AREA_SETATTR,
    V_CMD_BLENDING_AREA_DESTROY,
    V_CMD_PRINT_SCREEN,
    V_CMD_SET_DISPLAY_DEV,
    V_CMD_SET_DISPLAY_MODE,
    V_CMD_SHOW_NEW_FRAME,       //309
    
    /* this cmd from mmm_kshow_cmd_t of mmm_decoder.h */
    V_CMD_KSHOW_SWITCH_CHANNEL = 400,
    V_CMD_KSHOW_ENABLE_VOCALIZE,
    V_CMD_KSHOW_SET_MIX_PROPORTION,
    V_CMD_KSHOW_DISABLE_VOCALIZE,
    V_CMD_KSHOW_START_RECORDING,
    V_CMD_KSHOW_STOP_RECORDING,
    V_CMD_KSHOW_SET_TITLE_FILE,
    V_CMD_KSHOW_ENABLE_TITLE_DISPLAY,
}v_cmd_type_e;


/************************ extern function, 函数声明 ***************************/
extern bool video_control(v_cmd_type_e v_cmd_type, void * param);
extern bool video_mmm_init( void );
extern bool video_mmm_deinit( void );
extern bool video_control_get_total_time_by_fullpath( char *fullpath, unsigned int *total_time );
extern bool video_control_set_mem_mode( mmm_video_mem_mode_t mem_mode );

#endif  /* __VIDEO_CONTROL_H__ */

