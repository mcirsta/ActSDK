/*******************************************************************************
 *                              5003
 *                            Module: sound
 *                 Copyright(c) 2003-2008 Actions Semiconductor,
 *                            All Rights Reserved. 
 *
 * History:         
 *      <author>    <time>           <version >             <desc>
 *       kkli     2008-09-05 15:00     1.0             build this file 
*******************************************************************************/
/*!
 * \file     sound.h
 * \brief    对音频输出设备的操作
 * \author   kkli
 * \version 1.0
 * \date  2008/09/05
*******************************************************************************/
#ifndef __SOUND_H__
#define __SOUND_H__
#ifdef __cplusplus
extern "C" {
#endif

/*!
 * \brief  
 *      定义声音输出的模式，和是否外接设备以及外接设备的类型有关
 *      播放过程中设置无效
 */
typedef enum
{
    /*! I2S双声道 */
    AO_MODE_I2S_2 = 0x1,
    /*! I2S 5.1声道，3wire */
    AO_MODE_I2S_51_3WDOUT,
    /*! I2S 5.1声道，TDM */
    AO_MODE_I2S_51_TDM,
    /*! SPDIF */
    AO_MODE_SPDIF,
    /*! HDMI */
    AO_MODE_HDMI,
    /*! On-chip Sigma-Delta */
    AO_MODE_CHIP
} ao_mode_t;
/*!
 * \brief  
 *      定义声音输出的调试方式
 *      播放过程中设置无效 
 */
typedef enum
{
    /*! 只是播放 */
    AO_DEBUG_PLAY = 0x1,
    /*! 写文件的同时播放（固定位置文件名） */
    AO_DEBUG_FILE_PLAY,
    /*! 只写文件（固定位置文件名） */
    AO_DEBUG_FILE,
    /*! 不输出 */    
    AO_DEBUG_NONE
} ao_debug_t;
/*!
 * \brief  
 *      定义PCM输出的方式
 */
typedef enum
{
    /*! 正常播放即停止*/
    AO_PCM_MODE_NORMAL = 0x1,
    /*! 循环播放 */
    AO_PCM_MODE_LOOP
} ao_pcm_mode_t;

/* magic word: 0x414f */
typedef enum
{
    AO_CMD_INIT = 0x414f00,
    AO_CMD_PLAY,
    AO_CMD_STOP,
    AO_CMD_PAUSE,
    AO_CMD_RESUME,
    /*! 输出模式 */
    AO_CMD_MODE,
	AO_CMD_SET_DEBUG,
    AO_CMD_SET_VOLUME,
    AO_CMD_GET_VOLUME,    
    /*! 设置PCM文件（全路径） */
    AO_CMD_PCM_FILE,
    /*! 设置PCM播放模式 */
    AO_CMD_PCM_MODE,
    /*! 开始播放PCM */
    AO_CMD_PCM_ON,
    /*! 停止播放PCM */
    AO_CMD_PCM_OFF,    
} ao_cmd_t;

typedef enum
{
    AO_TERMINATED,
    AO_PLAYING,     
    AO_STOPPED,     
    AO_PAUSED,      
} ao_status_t;

typedef struct
{
    int channels;		/* 输入的声道数 */
    int sample_rate;    /* 输入采样率 */
} audiout_init_t;

void *audiout_open(void);
int audiout_cmd(void *, ao_cmd_t, unsigned long);
void audiout_close(void *);

#define AI_SOURCE_MIC       0
#define AI_SOURCE_FM        1
#define AI_SOURCE_LINEIN    2

/*!
 * \brief  
 *      定义控制声音采集模块的命令字
 */
typedef enum
{
    /*! 输入参数类型为audioin_init_t */
    AI_CMD_INIT = 0x414900,
    AI_CMD_START,
    AI_CMD_STOP,
    AI_CMD_PAUSE,
    AI_CMD_RESUME,
    /*! 输入参数类型为audioin_source_t */
    AI_CMD_SET_SOURCE,
    AI_CMD_SET_DEBUG
} ai_cmd_t;
/*!
 * \brief  
 *      定义声音采集模块的初始化参数
 */
typedef struct
{
    /*! 一次采集的sample数 */
    int frame_samples; 
    /*! 采样率 */
    int sample_rate;
    /*! 输入的声道数 */
    int channels;
} audioin_init_t;
/*!
 * \brief  
 *      定义对录音源的设置
 */
typedef struct
{
    /*! 源类型 */
    int type;
    /*! 输入源的幅度，单位毫伏 */
    int source_amplitude;
    /* 期望输出的幅度，单位毫伏 */
    int target_amplitude;
} audioin_source_t;

void *audioin_open(void);
int audioin_cmd(void *, ai_cmd_t, unsigned long);
void audioin_close(void *);

#ifdef __cplusplus
}
#endif
#endif // __SOUND_H__
