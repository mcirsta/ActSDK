/********************************************************************************
 *                              USDK(1100)
 *                             Module: photo app
 *                 Copyright(c) 2003-2009 Actions Semiconductor,
 *                            All Rights Reserved.
 * File : photo_dec_ctrl.h
 * History:
 *      <author>    <time>           <version >             <desc>
 *       carrie     2009-4-24          v1.0               build this file
 ********************************************************************************/
/*!
 * \file     photo_dec_ctrl.h
 * \brief    photo decoding control
 * \author   carrie
 *
 * \version 1.0
 * \date  2009/04/24
 *******************************************************************************/

#ifndef __PHOTO_DEC_CTRL_H__
#define __PHOTO_DEC_CTRL_H__

#include "include_case.h"
#include "mmm_decoder.h"

/*
 * 图片中间件接口命令列表
 */
typedef enum
{
    SET_FILE,
    SET_ALBUM_INFO,
    SET_MEDIA_BUFFER,
    GET_MEDIA_INFO,
    IMG_BROWSE,
    IMG_DECODE,
    SET_CALLBACK,
    GET_DECODER_STATUS,
    ABORT_DECODE,
    RESIZE,
    ZOOMIN,
    ZOOMOUT,
    ROTATELEFT90,
    ROTATERIGHT90,
    ROTATE180,
    MOVEUP,
    MOVEDOWN,
    MOVELEFT,
    MOVERIGHT,
    MIRRORUPDOWN,
    MIRRORLEFTRIGHT,

} dec_type_e;

bool photo_dec_open(void);
void photo_dec_close(void);
bool photo_dec_control(dec_type_e type, unsigned int param);

#endif

