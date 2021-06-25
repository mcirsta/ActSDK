/********************************************************************************
 *                              USDK(1100)
 *                             Module: app
 *                 Copyright(c) 2003-2009 Actions Semiconductor,
 *                            All Rights Reserved.
 * File : radio_ui_playing.c
 * History:
 *      <author>    <time>           <version >             <desc>
 *      cheason    2009-04-23          v1.0               build this file
 ********************************************************************************/
/*!
 * \file     fmengine.c
 * \brief    define the function of communication between app & engine
 * \author   cheason
 *
 * \version 1.0
 * \date  2009/04/23
 *******************************************************************************/
#include "radio_app.h"
#include "radio_scene_playing.h"

/*!
 * 向引擎发送消息，设置或读取信息
 * 参数：
 *    msg: 发送的消息
 *    send_buff: 发送内容指针
 *    ret_buff: 读取到的内容保存指针
 * 返回值：0--成功，-1--失败
 */
int fm_engine_msg(msg_apps_type_e msg, const void *send_buff, unsigned int send_length, void *ret_buff)
{
    int ret = -1;
    msg_apps_t msg_apps;
    msg_reply_t msg_reply;

    msg_apps.type = msg;
    if(send_length > 0)
    {
        memcpy(msg_apps.content, send_buff, send_length);
    }
    switch (msg)
    {
    case MSG_FMENGINE_SET_BAND_MODE:
    case MSG_FMENGINE_SET_FREQ:
    case MSG_FMENGINE_SET_THRESHOLD:
    case MSG_FMENGINE_SET_PLAY:
        if(true == send_async_msg(FM_EG_APP_NAME, &msg_apps))
            ret = 0;
        return ret;
    default:
        break;
    }
    /*发送同步消息，需要取得返回值*/
    if((send_sync_msg(FM_EG_APP_NAME, &msg_apps, &msg_reply, NULL) == true)
            && (MSG_REPLY_SUCCESS == msg_reply.type))
    {
        switch (msg)
        {
        case MSG_FMENGINE_GET_STEREO_MODE:
        case MSG_FMENGINE_GET_INTENSITY:
        case MSG_FMENGINE_GET_FREQ:
        case MSG_FMENGINE_GET_BAND_MODE:
        case MSG_FMENGINE_GET_ANTENNA:
        
            memcpy(ret_buff, msg_reply.content, sizeof(unsigned long));
            break;
        case MSG_FMENGINE_AUTO_SEEK_OVER:
            memcpy(ret_buff, msg_reply.content, sizeof(bool));
            break;
        case MSG_FMENGINE_GET_INFO:
            memcpy(ret_buff, msg_reply.content, sizeof(struct radio_info_t));
            break;
        case MSG_FMENGINE_GET_AUTO_SEEK:
            memcpy(ret_buff, msg_reply.content, sizeof(eg_seek_info_t));
            break;

        default:
            break;
        }

        ret = 0;
    }

    return ret;
}
