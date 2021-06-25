/*! \cond MMM_ENCODE_API*/
/********************************************************************************
 *                              usdk1100
 *                            Module: act_decoder.c
 *                 Copyright(c) 2003-2008 Actions Semiconductor,
 *                            All Rights Reserved.
 *
 * History:
 *      <author>    <time>           <version >             <desc>
 *       jszeng    2008-09-02 10:00     1.0             build this file
 ********************************************************************************/
/*!
 * \file     mmm_encoder.h
 * \brief    定义与应用层的接口及参数
 * \author   曾金生、易俊
 * \version 1.0
 * \date  2008/09/02
 *******************************************************************************/
/*! \endcond*/
#ifndef __MMM_ENCODER_H__
#define __MMM_ENCODER_H__

#include "fb.h"

#ifdef __cplusplus
extern "C"
{
#endif

/*! \cond MMM_ENC_COMMON_API*/
/*!
 *  \brief
 *     提供给应用的ENCODE公用命令
 */
typedef enum
{
    /*! 设置保存路径*/
    MMM_SET_SAVE_PATH = 0x01,
    /*! 设置文件格式*/
    MMM_SET_FILE_FMT,
    /*! 获取磁盘信息*/
    MMM_GET_DISK_INFO
} mmm_enc_cmd_t;
/*!
 *  \brief
 *     返回给应用层的磁盘信息
 */
typedef struct
{
    /*! 磁盘剩余空间*/
    unsigned int space_left;
    /*! 对于录像与录音此参数为剩余可录时间，对于dc此参数为可照得照片*/
    unsigned int info;
} mmm_disk_info_t;
/*! \endcond*/

/*! \cond MMM_DV_RECORD_API*/
/*!
 *  \brief
 *     提供给应用的DV和RECORD的公用命令
 */
typedef enum
{
    /*! 设置音频参数*/
    MMM_SET_AUDIO_PARAM = 0x21,
    /*! 获取编码器当前状态*/
    MMM_GET_RECORDER_STATUS
} mmm_dv_record_cmd_t;
/*!
 *  \brief
 *     中间件编码器的状态
 */
typedef enum
{
    /*! 处于编码状态*/
    MMM_RECORDER_RECORDING,
    /*! 处于停止状态*/
    MMM_RECORDER_STOPPED,
    /*! 处于暂停状态*/
    MMM_RECORDER_PAUSED,
    /*! 处于打开新文件状态 */
    MMM_NWE_FILE_RECORDING,
    /*! 处于出错状态状态*/
    MMM_RECORDER_ERROR
} mmm_recorder_status_t;

/*!
 *  \brief
 *     音源的选择，当有多个bit位为1时，便为多路输出的混合
 */
typedef struct
{
    /*! 内部麦克风*/
    unsigned mic :1;
    /*! 收音机*/
    unsigned fm :1;
    /*! line in线输入*/
    unsigned line_in :1;
    /*! 暂未使用，设为0*/
    unsigned iis :1;
    /*! 暂未使用，设为0*/
    unsigned spdif :1;
    /*! 暂未使用，设为0*/
    unsigned tv :1;
    /*! 保留位*/
    unsigned reserved :26;
} mmm_audio_source_t;
/*!
 *  \brief
 *     录音增益要求
 */
typedef struct
{
    /*! 声音的输入电压，以mv为单位*/
    unsigned int input_volt;
    /*! 声音的输出电压，以mv为单位*/
    unsigned int output_volt;
} mmm_voice_volt_t;
/*!
 *  \brief
 *     音频编码的参数设置
 */
typedef struct
{
    /*! 录音音源的选择*/
    mmm_audio_source_t source;
    /*! 录音增益要求*/
    mmm_voice_volt_t voice_volt;
    /*! 音频采样点的精度，如8bit,16bit,24bit*/
    unsigned int bpp;
    /*! 音频采样的通道数如:1 单声道；2 双声道*/
    unsigned int channel;
    /*! 采样频率如8000，16000，44100等*/
    unsigned int sample_rate;
    /*! 编码的比特率*/
    unsigned int bitrate;
    /*! 音频编码的模式，主要为wav录音时需要区分编码模式为adpcm或者pcm，目前只支持adpcm，也即默认设0*/
    unsigned int encode_mode;
} mmm_audio_param_t;

/*! \endcond*/

/*! \cond MMM_DV_DC_API*/
/*!
 *  \brief
 *     提供给应用的DV和DC的公用命令
 */
typedef enum
{
    /*! 设置视频显示区域 */
    MMM_SET_CAPTURE_AREA = 0x41,
    /*! 建立blending区域，返回建立的blending区域的一个int型的句柄 */
    MMM_CREATE_BLENDING_AREA,
    /*! 显示设置的blending区域，参数为之前create返回的句柄 */
    MMM_SHOW_BLENDING_AREA,
    /*! 隐藏设置的blending区域，参数为之前create返回的句柄 */
    MMM_HIDE_BLENDING_AREA,
    /*! 设置的blending区域的属性 */
    MMM_SETATTR_BLENDING_AREA,
    /*! 销毁之前建立的blending区域 */
    MMM_DESTROY_BLENDING_AREA,
    /*! 打印当前图片，将当前视频转化为RGB数据输出到屏幕上 */
    MMM_PRINT_CUR_SCREEN,
    /*! 设置对比度 */
    MMM_SET_CONTRAST,
    /*! 设置白平衡 */
    MMM_SET_WHITE_BALANCE,
    /*! 设置特殊效果 */
    MMM_SET_SPECIAL_EFFECT,
    /*! 设置曝光模式 */
    MMM_SET_EXPOSURE_MODE,
    /*! 设置分辨率 */
    MMM_SET_RESOLUTION,
    /*! 设置文件名 */
    MMM_SET_FILE_NAME,
    /*! 设置数码变焦级别 */
    MMM_SET_ZOOM_LEVEL,
    /*! 开始取景 */
    MMM_START_CAPTURE,
    /*! 停止取景 */
    MMM_STOP_CAPTURE,
    /*! 设置亮度 */
    MMM_SET_BRIGHTNESS,
    /*! 设置当前刷屏模式 */
    MMM_SET_REC_SCREEN_ROTATE
} mmm_dv_dc_cmd_t;

/*!
 *  \brief
 *    取景显示窗口参数
 */
typedef struct
{
    /*! 显示窗口左上角的x坐标*/
    int xpos;
    /*! 显示窗口左上角的y坐标*/
    int ypos;
    /*! 显示窗口宽*/
    unsigned int width;
    /*! 显示窗口高*/
    unsigned int height;
} mmm_capture_area_t;

/*!
 *  \brief
 *     blending区域的位置和相关属性
 *
 *  typedef struct
 *  {
 *      unsigned int color_key;
 *      unsigned int alpha;
 *      fb_rect_area_t area;
 *  }fb_blending_param_t;
 *
 * */
typedef fb_blending_param_t mmm_enc_blending_area_t;

/*!
 *  \brief
 *     blending区域, create blending区域的参数
 *
 *  typedef struct
 *  {
 *      int handle;
 *      fb_blending_param_t blend_param;
 *  }fb_blending_create_t;
 * */
typedef fb_blending_create_t mmm_enc_blending_create_t;

/*! \brief
 *     blending区域的属性，包括alpha值和colorkey值
 *
 *  typedef struct
 *  {
 *      unsigned int color_key;
 *      unsigned int alpha;
 *  }fb_blending_attr_t;
 * */

typedef fb_blending_attr_t mmm_enc_blending_attr_t;
/*! \brief
 *      设置blendint区域属性是的参数，包括设置区域的handle，以及需要设置的blending区域的属性mmm_blending_attr_t值。
 *
 *  typedef struct
 *  {
 *      int handle;
 *      fb_blending_attr_t blending_attr;
 *  }fb_blending_setattr_t;
 * */
typedef fb_blending_setattr_t mmm_enc_blending_setattr_t;

/*!
 *  \brief
 *   当前视频的rgb数据
 */
typedef struct
{
    /*! 当前视频在屏上的x坐标 */
    int xpos;
    /*! 当前视频在屏上的y坐标 */
    int ypos;
    /*! 当前视频的宽 */
    unsigned int width;
    /*! 当前视频的高 */
    unsigned int height;
    /*! 当前视频数据地址 */
    char *video_frame;
} mmm_cur_frame_t;

/*!
 *  \brief
 *   分辨率
 */
typedef enum
{
    /*! 分辨率为160*120 */
    MMM_RES160X120 = 0x0001,
    /*! 分辨率为176*144 */
    MMM_RES176X144,
    /*! 分辨率为224*176 */
    MMM_RES224X176 = 0x0004,
    /*! 分辨率为320*240 */
    MMM_RES320X240,
    /*! 分辨率为640*480 */
    MMM_RES640X480 = 0x0007,
    /*! 分辨率为1024*768 */
    MMM_RES1024X768 = 0x0009,
    /*! 分辨率为1280*960 */
    MMM_RES1280X960,
    /*! 分辨率为1280*1024 */
    MMM_RES1280X1024,
    /*! 分辨率为1600*1200 */
    MMM_RES1600X1200 = 0x000d,
    /*! 分辨率为2048*1536 */
    MMM_RES2048X1536 = 0x0011,
    /*! 分辨率为2400*1800 */
    MMM_RES2400X1800,
    /*! 分辨率为2560*1920 */
    MMM_RES2560X1920,
    /*! 分辨率为2816*2112 */
    MMM_RES2816X2112,
    /*! 分辨率为3264*2448 */
    MMM_RES3264X2448,
    /*! 分辨率为3840*2880 */
    MMM_RES3840X2880,
    /*! 分辨率为4416*3312 */
    MMM_RES4416X3312,
    /*! 分辨率为4672*3504 */
    MMM_RES4672X3504
} mmm_res_constant_t;

/*!
 *  \brief
 *   数码变焦参数，分母一般为10，分子大于等于分母，分数值即为变焦级别
 */
typedef struct
{
    /*! 分子 */
    int numerator;
    /*! 分母 */
    int denominator;
} mmm_zoom_level_t;

/*!
 *  \brief
 *  屏幕的旋转方向0度，90度，180度，270度
 */
/*屏幕的旋转方向*/
//typedef enum
//{
//    /*! 屏幕旋转0度，即正视角*/
//    ROTATION_0 = 0,
//    /*! 屏幕方向旋转90度*/
//    ROTATION_90 = 90,
//    /*! 屏幕方向旋转180度*/
//    ROTATION_180 = 180,
//    /*! 屏幕方向旋转270度*/
//    ROTATION_270 = 270
//}rotation_e;

/*!
 *  \brief
 *     白平衡设置参数
 */
typedef enum
{
    /*! 自动 */
    MMM_WB_AUTO,
    /*! 晴天 */
    MMM_WB_DAYLIGHT,
    /*! 阴天 */
    MMM_WB_CLOUD,
    /*! 白炽灯 */
    MMM_WB_INCANDESCENCE,
    /*! 荧光灯*/
    MMM_WB_FLUORESCENT
} mmm_white_balance_t;

/*!
 *  \brief
 *     特殊效果设置参数
 */
typedef enum
{
    /*! 正常效果*/
    MMM_NONE_SPECIAL,
    /*! 黑白效果*/
    MMM_WHITE_AND_BLACK,
    /*! 复古效果*/
    MMM_SEPIA,
    /*! 负片效果*/
    MMM_SOLARIZE
} mmm_special_effect_t;

/*!
 *  \brief
 *     曝光模式设置参数
 */
typedef enum
{
    /*! 自动曝光模式*/
    MMM_AUTO_EXPOSURE,
    /*! 室内曝光模式*/
    MMM_INDOOR_EXPOSURE,
    /*! 室外曝光模式*/
    MMM_OUTDOOR_EXPOSURE
} mmm_exposure_mode_t;

/*! \endcond*/
/*! \cond MMM_DV_API*/
/*!
 *  \brief
 *     提供给应用的DV命令
 */
typedef enum
{
    /*! 设置视频源 */
    MMM_SET_VIDEO_SOURCE = 0x61,
    /*! 设置音频编码格式 */
    MMM_SET_AUDIO_FMT,
    /*! 设置视频编码格式 */
    MMM_SET_VIDEO_FMT,
    /*! 设置视频编码参数 */
    MMM_SET_VIDEO_PARAM,
    /*! 是否使能动静监测 */
    MMM_ENABLE_VIDEO_MONITOR,
    /*! 开始录像 */
    MMM_START_VIDEO,
    /*! 停止录像 */
    MMM_STOP_VIDEO,
    /*! 暂停录像 */
    MMM_PAUSE_VIDEO,
    /*! 重新录像 */
    MMM_RESUME_VIDEO,
    /*! 是否需要打上录像时间 */
    MMM_VIDEO_TIME
} mmm_dv_cmd_t;

/*!
 *  \brief
 *     录像日期数据结构
 */
typedef struct
{
    /*! 是否打上录制日期, 0: 否; 1: 是 */
    int enc_date_flag;
    /*! 年*/
    unsigned int year;
    /*! 月*/
    unsigned int month;
    /*! 日*/
    unsigned int day;
} mmm_enc_date_t;

/*!
 *  \brief
 *     视频码率级别常量
 */
typedef enum
{
    /*! 视频码率级别0: 500kbps*/
    MMM_VIDDEO_CBR_LEVEL0,
    /*! 视频码率级别1: 800kbps*/
    MMM_VIDDEO_CBR_LEVEL1,
    /*! 视频码率级别2: 1000kbps*/
    MMM_VIDDEO_CBR_LEVEL2,
    /*! 视频码率级别3: 1200kbps*/
    MMM_VIDDEO_CBR_LEVEL3,
    /*! 视频码率级别4: 1500kbps*/
    MMM_VIDDEO_CBR_LEVEL4,
    /*! 码率可变*/
    MMM_VIDDEO_VBR
} mmm_video_bitrate_level_t;

/*!
 *  \brief
 *     视频参数数据结构
 */
typedef struct
{
    /*! 视频祯率*/
    unsigned int frame_rate;
    /*! 视频码率级别*/
    mmm_video_bitrate_level_t bitrate_level;
} mmm_video_args_t;

/*!
 *  \brief
 *    图像动静状态
 */
typedef enum
{
    /*! 图像静止 */
    MMM_STILL,
    /*! 图像运动 */
    MMM_MOVING
} mmm_monitor_status_t;

/*!
 *  \brief
 *     录像中间件状态
 */
typedef struct
{
    /*! 当前录像时间 */
    unsigned int time;
    /*! 当前中间件状态 */
    mmm_recorder_status_t status;
    /*! 当前图像动静状态 */
    mmm_monitor_status_t monitor_status;
    /*! 错误号(中间件状态为错误状态时有效)*/
    unsigned int err_no;
} mmm_dv_status_t;

void *mmm_ve_open(void *param);
int mmm_ve_cmd(void *handle, unsigned int cmd, unsigned int param);
int mmm_ve_close(void *handle, void *param);

/*! \endcond*/

/*! \cond MMM_DC_API*/
/*!
 *  \brief
 *     提供给应用的DC命令
 */
typedef enum
{
    /*! 获得最大分辨率 */
    MMM_GET_MAX_RESOLUTION = 0x81,
    /*! 设置闪光灯功能 */
    MMM_SET_FLASHLIGHT,
    /*! 是否启动人脸检测功能 */
    MMM_ENABLE_FACE_DETECT,
    /*! 是否启动大头贴功能 */
    MMM_PHOTO_STICKER,
    /*! 设置图片的exif信息 */
    MMM_SET_EXIF_INFO,
    /*! 是否给照片打上日期 */
    MMM_PHOTO_DATE,
    /*! 拍照 */
    MMM_SNAPSHOT
} mmm_dc_cmd_t;

/*!
 *  \brief
 *   设置闪光灯参数
 */
typedef enum
{
    /*! 关闭 */
    MMM_FLASHLIGHT_OFF,
    /*! 打开 */
    MMM_FLASHLIGHT_ON,
    /*! 自动 */
    MMM_FLASHLIGHT_AUTO
} mmm_flashlight_param;

/*!
 *  \brief
 *   是否启动大头贴功能
 */
typedef struct
{
    /*! 是否启动大头贴功能, 0: 否;1: 是 */
    int photo_sticker_flag;
    /*! 大头贴背景图片信息 */
    unsigned char *buf;
    /*! 大头贴背景图片宽 */
    unsigned int width;
    /*! 大头贴背景图片高 */
    unsigned int height;
} mmm_photo_sticker_t;

/*!
 *  \brief
 *   exif time信息
 */
typedef struct
{
    /*! 年 */
    int year;
    /*! 月 */
    int month;
    /*! 日 */
    int day;
    /*! 时 */
    int hour;
    /*! 分 */
    int minute;
    /*! 秒 */
    int second;
} mmm_camera_exif_time_t;

/*!
 *  \brief
 *   exif设置信息,写在文件头
 */
typedef struct
{
    /*! 照相机名称 */
    char *machine;
    /*! 照片所有者 */
    char *owner;
    /*! 拍摄时间信息 */
    mmm_camera_exif_time_t time;
} mmm_camera_exif_t;

/*!
 *  \brief
 *   拍摄时间信息(点阵)
 */
typedef struct
{
    /*! 是否打上拍照日期信息, 0: 否; 1: 是 */
    int photo_data_flag;
    /*! 时间信息点阵buf地址 */
    unsigned char *buf;
    /*! 时间信息点阵宽 */
    unsigned int width;
    /*! 时间信息点阵高 */
    unsigned int height;
} mmm_photo_data_t;

void *mmm_ie_open(void *param);
int mmm_ie_cmd(void *handle, unsigned int cmd, unsigned int param);
int mmm_ie_close(void *handle, void *param);

/*! \endcond*/

/*! \cond MMM_RECORD_API*/
/*!
 *  \brief
 *     提供给应用的录音命令
 */
typedef enum
{
    /*! 开始监听*/
    MMM_START_MONITOR = 0xa1,
    /*! 停止监听*/
    MMM_STOP_MONITOR,
    /*! 设置VOR参数，包括时间和增益大小*/
    MMM_SET_VOR_PARAM,
    /*! 使能静音检测*/
    MMM_SILENCE_DECTECT_ON,
    /*! 关闭静音检测*/
    MMM_SILENCE_DECTECT_OFF,
    /*! 开始录音*/
    MMM_START_RECORDING,
    /*! 停止录音*/
    MMM_STOP_RECORDING,
    /*! 暂停录音*/
    MMM_PAUSE_RECORDING,
    /*! 暂停后继续录音*/
    MMM_RESUME_RECORDING,
    /*! 设置能量*/
    MMM_SET_ENERGY_LEVEL
} mmm_record_cmd_t;
/*!
 *  \brief
 *     监听模式
 */
typedef enum
{
    /*! 模拟监听*/
    MMM_ANALOGE_MODE,
    /*! DSP监听*/
    MMM_DIGITAL_MODE
} mmm_monitor_mode_t;
/*!
 *  \brief
 *     vor参数
 */
typedef struct
{
    /*! 静音时间*/
    unsigned int vor_time;
    /*! 静音门限值*/
    unsigned int vor_gain;
} mmm_vor_t;
/*!
 *  \brief
 *     中间件编码器的实时信息
 */
typedef struct
{
    /*! 当前的录音时间*/
    unsigned int time;
    /*! 当前codec的状态*/
    mmm_recorder_status_t status;
    /*! 输出当前实时能量级别*/
    unsigned char energy_level;
    /*! 当前是否是静音，在静音检测功能开启时有效*/
    unsigned char silence_flag;
    /*! 在出错状态时返回的错误号*/
    unsigned int err_no;
} mmm_record_status_t;

void *mmm_ae_open(void *param);
int mmm_ae_cmd(void *handle, unsigned int cmd, unsigned int param);
int mmm_ae_close(void *handle, void *param);

#ifdef __cplusplus
}
#endif

#endif
