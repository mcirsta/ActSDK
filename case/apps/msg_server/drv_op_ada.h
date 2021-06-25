/*
*******************************************************************************
*                       NOYA1100
*                (c) Copyright 2002-2008, Actions Co,Ld. 
*                        All Right Reserved 
* FileName: .h
* Description:
* History: 
*       <author>    <time>          <version >      <desc>
*       shaofeng    2009/03/11      1.0             建立该文件。
*******************************************************************************
*/ 

#ifndef __DRV_OP_ADA_H__
#define __DRV_OP_ADA_H__


#include "include_case.h"
#include "include_msg_server.h"


/* wav按键音文件的 文件头长度宏定义. 有几种形式, 44为最常见的一种 */
#define WAV_FILE_HEAD_LENGTH    (44)



/*!
*\brief
    解析的按键音文件信息结构体, 用于设置给ADA驱动 */
typedef struct
{
    /*! key tone 声道参数, 分单声道和立体声. 一般按键音用单声道数据, 数据量可以减少一半 */
    enum ADA_VOICE_CHANNEL channle;
    /*! key tone 采样率参数. 建议在满足音质的情况下尽量低, 以减少数据量 */
    int sample_rate;
}key_tone_head_t;



/*!
*\brief
    按键音信息结构体, 所有的按键音参数及状态都包含在此结构体中 */
typedef struct
{
    /*! key tone 是否有效, 用于setting打开关闭按键音, 也根据内部初始化是否成功 而设置 */
    int on;
    
    /*! key tone是否是能, 防止与audio应用冲突, 当进入audio时设置0, 退出后设置1 */
    int enable;
    
    /*! key tone file handle */
    int fd;
    
    /*! key tone file fullpath */
    char fullpath[PATH_MAX];
    
    /*! key tone file data len, unit is byte */
    unsigned int len;
    
    /*! key tone data pointer */
    char *data;
    
    key_tone_head_t head;
}key_tone_param_t;



/************************ extern variable, 变量声明 ***************************/
extern key_tone_param_t key_tone_param;



/************************ extern function, 函数声明 ***************************/
extern void ada_para_init( void );
extern void ada_device_open( void );
extern void ada_device_close( void );

extern bool ada_set_key_tone_param( void );
extern bool ada_set_key_tone_on( int flag_on );
extern bool ada_set_key_tone_enable( int flag_enable );
extern bool ada_set_key_tone_play( void );
extern bool ada_set_volume( int volume );
extern bool ada_get_volume( int *volume );


#endif /* __DRV_OP_ADA_H__ */

