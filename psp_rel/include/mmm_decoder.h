/*! \cond MMM_DECODE_API*/
/********************************************************************************
 *                              usdk1100
 *                            Module: act_decoder.c
 *                 Copyright(c) 2003-2008 Actions Semiconductor,
 *                            All Rights Reserved.
 *
 * History:
 *      <author>    <time>           <version >             <desc>
 *       jszeng    2008-08-27 15:00     1.0             build this file
 ********************************************************************************/
/*!
 * \file     mmm_decoder.h
 * \brief    定义与应用层的接口及参数
 * \author   曾金生、徐会
 * \version 1.0
 * \date  2008/08/27
 *******************************************************************************/
/*! \endcond*/
#ifndef __MMM_DECODER_H__
#define __MMM_DECODER_H__

#include "act_amvb.h"
#include "fb.h"

#ifdef __cplusplus
extern "C"
{
#endif

/*! \cond MMM_AUDIO_VEDIO_API*/

#define VIDEO_TAG_SIZE
#define AUDIO_TAG_SIZE 64
/*!
 * \brief
 *      提供给应用层的公共播放命令
 */
typedef enum
{
    /*! 播放 */
    MMM_PLAY = 1,
    /*! 停止 */
    MMM_STOP,
    /*! 暂停 */
    MMM_PAUSE,
    /*! 继续 */
    MMM_RESUME,
    /*! 定位播放点 */
    MMM_SEEK,
    /*! 快进 */
    MMM_FAST_FORWARD,
    /*! 快退 */
    MMM_FAST_BACKWARD,
    /*! 取消快进快退 */
    MMM_CANCEL_FFB,
    /*! 设置快进快退步长 */
    MMM_SET_FFB_STEP,
    /*! 设置播放的文件名 */
    MMM_SET_FILE,
    /*! 将当前播放点设置为标签 */
    MMM_MAKE_MARK,
    /*! 获取多媒体文件的信息 */
    MMM_GET_MEDIA_INFO,
    /*! 获取播放器的当前状态 */
    MMM_GET_PLAYER_STATUS,
    /*! 设置A点 */
    MMM_SET_A,
    /*! 设置B点 */
    MMM_SET_B,
    /*! 取消AB播放 */
    MMM_CLEAR_AB,
    /*! 设置AB播放的次数 */
    MMM_SET_AB_COUNT,
    /*! 从B点返回到A点的等待时间，只对音频播放有效*/
    MMM_SET_AB_WAIT_TIME,
    /*! 获取DRM信息 */
    MMM_GET_DRM_INFO,
    /*! 设置video 的缓存模式*/
    MMM_VIDEO_MEM_MODE
} mmm_dec_cmd_t;

/*!
 * 设置video缓存的分配模式
 */
typedef enum
{
    /*! 正常解码模式*/
    MMM_MEM_NORMAL_MODE = 0x0,
    /*! 缩略图模式*/
    MMM_MEM_THUMBNAIL_MODE,
} mmm_video_mem_mode_t;
/*!
 *  \brief
 *      DRM类型
 */
typedef enum
{
    /*! 不是DRM文件 */
    MMM_NO_DRM = 0,
    /*! ACTIONS的DRM视频文件 */
    MMM_ACT_DRM,
    /*! 微软的DRM视频文件 */
    MMM_MS_DRM,
    /*! 其他类型的DRM文件 */
    MMM_DRM_OTHER
} mmm_drm_type_t;
/*!
 *  \brief
 *      播放引擎的播放状态
 */
typedef enum
{
    /*! 停止状态 */
    MMM_PLAYER_STOPPED,
    /*! 播放状态 */
    MMM_PLAYER_PLAYING,
    /*! 暂停状态 */
    MMM_PLAYER_PAUSED,
    /*! 快进快退状态 */
    MMM_PLAYER_FFB,
    /*! 出错状态 */
    MMM_PLAYER_ERROR
} mmm_player_status_t;

/*!
 *  \brief
 *      AB播放状态
 */
typedef enum
{
    /*! A点已经设置状态 */
    MMM_A_IS_SET,
    /*! 处于AB播放状态 */
    MMM_AB_PLAYING,
    /*! 非AB播放状态 */
    MMM_NORMAL_PLAYING
} mmm_ab_status_t;

/*! \endcond*/

/*! \cond MMM_IMAGE_VIDEO_API*/
/*!
 *  \brief
 *     图片解码与视频解码公用命令
 */
typedef enum
{
    /*! 设置刷屏模式，横屏或是竖屏 */
    MMM_SET_SCREEN_ROTATE = 0x21,
} mmm_image_video_cmd_t;

/*! \endcond*/

/*! \cond MMM_VIDEO_API*/

/*!
 *  \brief
 *      提供给应用层的视频播放命令
 */
typedef enum
{
    /*! 设置视频的显示区域 */
    MMM_SET_VIDEO_AREA = 0x31,
    /*! 建立blending区域*/
    MMM_BLENDING_AREA_CREATE,
    /*! 显示blending区域*/
    MMM_BLENDING_AREA_SHOW,
    /*! 隐藏blending区域*/
    MMM_BLENDING_AREA_HIDE,
    /*! 设置blending区域的属性*/
    MMM_BLENDING_AREA_SETATTR,
    /*! 销毁blending区域*/
    MMM_BLENDING_AREA_DESTROY,

    /*! 获取当前帧数据 */
    MMM_PRINT_SCREEN,
    /*! 设置显示设备，LCD, TV等*/
    MMM_SET_DISPLAY_DEV,
    /*! 获取一帧新的数据去显示 */
    MMM_SHOW_NEW_FRAME,
    /*! 设置视频画面比例*/
    MMM_SET_VIDEO_RATIO,
} mmm_video_cmd_t;

/*!
 *  \brief
 *      kshow功能命令
 */
typedef enum
{
    /*! 切换声道 */
    MMM_KSHOW_SWITCH_CHANNEL = 0x41,
    /*! 开始练唱 */
    MMM_KSHOW_ENABLE_VOCALIZE,
    /*! 设置练唱的混音比 */
    MMM_KSHOW_SET_MIX_PROPORTION,
    /*! 停止练唱 */
    MMM_KSHOW_DISABLE_VOCALIZE,
    /*! 开始录音 */
    MMM_KSHOW_START_RECORDING,
    /*! 停止录音 */
    MMM_KSHOW_STOP_RECORDING,
    /*! 设置歌词文件 */
    MMM_KSHOW_SET_TITLE_FILE,
    /*! 显示歌词 */
    MMM_KSHOW_ENABLE_TITLE_DISPLAY
} mmm_kshow_cmd_t;

/*!
 *  \brief
 *     录音状态
 */
typedef enum
{
    /*! Kshow录音处于关闭状态 */
    MMM_KSHOW_RECORDER_OFF,
    /*! Kshow录音处于录音状态 */
    MMM_KSHOW_RECORDER_ON,
    /*! Kshow录音处于暂停状态 */
    MMM_KSHOW_RECORDER_PAUSED,
    /*! Kshow录音处于错误状态 */
    MMM_KSHOW_RECORDER_ERROR
} mmm_k_record_status_t;

/*!
 *  \brief
 *     练唱状态
 */
typedef enum
{
    /*! Kshow练唱处于关闭状态 */
    MMM_KSHOW_VOCALIZER_OFF,
    /*! Kshow练唱处于练唱状态 */
    MMM_KSHOW_VOCALIZER_ON,
    /*! Kshow练唱处于暂停状态 */
    MMM_KSHOW_VOCALIZER_PAUSED,
    /*! Kshow练唱处于错误状态 */
    MMM_KSHOW_VOCALIZER_ERROR
} mmm_vocalize_status_t;
/*!
 *  \brief
 *      显示设备
 */
typedef enum
{
    /*! CPU屏显示设备*/
    MMM_CPU_LCD_DEV,
    /*! RGB屏显示设备*/
    MMM_RGB_LCD_DEV,
    /*! TV out显示设备, PAL mode*/
    MMM_TVOUT_PAL_DEV,
    /*! TV out显示设备, NTSC mode*/
    MMM_TVOUT_NTSC_DEV
} mmm_video_dev_t;

typedef enum
{
    /*! 画面全屏显示 */
    MMM_RATIO_FULL = 1,
    /*! 画面原始尺寸显示 */
    MMM_RATIO_ORG,
    /*! 画面等比例显示 */
    MMM_RATIO_EQU,
} mmm_video_ratio_t;

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
typedef fb_blending_param_t mmm_blending_area_t;

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
typedef fb_blending_create_t mmm_blending_create_t;
/*! \brief
 *     blending区域的属性，包括alpha值和colorkey值
 *
 *  typedef struct
 *  {
 *      unsigned int color_key;
 *      unsigned int alpha;
 *  }fb_blending_attr_t;
 * */

typedef fb_blending_attr_t mmm_blending_attr_t;
/*! \brief
 *      设置blendint区域属性是的参数，包括设置区域的handle，以及需要设置的blending区域的属性mmm_blending_attr_t值。
 *
 *  typedef struct
 *  {
 *      int handle;
 *      fb_blending_attr_t blending_attr;
 *  }fb_blending_setattr_t;
 * */
typedef fb_blending_setattr_t mmm_blending_setattr_t;

/*!
 *  \brief
 *     DRM时间信息
 */
typedef struct
{
    /*! 年 */
    short year;
    /*! 月 */
    char month;
    /*! 日 */
    char day;
    /*! 时 */
    char hour;
    /*! 分 */
    char minute;
    /*! 秒 */
    char second;
    /*! 微秒 */
    char minisecond;
} mmm_drm_date_t;
/*!
 *  \brief
 *     DRM信息选择标志
 */
typedef struct
{
    /*! 标识remain_play_cnt字段有效 */
    char remain_cnt :1;
    /*! 标识remain_play_hours字段有效 */
    char remain_play_hours :1;
    /*! 标识begin_time字段有效 */
    char begin_time :1;
    /*! 标识expire_time字段有效 */
    char expire_time :1;
    /*! 标识rollback字段有效 */
    char rollback :1;
    /*! 标识remain_store_hours字段有效 */
    char remain_store_hours :1;
    /*! 标识origin_play_cnt字段有效 */
    char origin_cnt :1;
} mmm_switch_flag_t;
/*!
 *  \brief
 *     DRM信息
 */
typedef struct
{
    /*! 是否有DRM信息，1为有DRM信息，0表示无DRM信息 */
    char license_flag;
    /*! DRM信息选择标志 */
    mmm_switch_flag_t switch_flag;
    /*!  */
    char rollback;
    /*!  */
    short remain_play_cnt;
    /*!  */
    short origin_play_cnt;
    /*!  */
    short remain_play_hours;
    /*!  */
    short remain_store_hours;
    /*!  */
    mmm_drm_date_t begin_time;
    /*!  */
    mmm_drm_date_t expire_time;
} mmm_license_info_t;
/*!
 *  \brief
 *     视频文件的信息
 */
typedef struct
{
    /*! 文件的总时间（ms计） */
    unsigned int total_time;
    /*! 多媒体文件的实际格式 */
    char format[8];
    /*! 文件中的音频流格式，如"ogg" */
    char audio[8];
    /*! 文件中的视频流格式，如 "wmv" */
    char video[8];
    /*! 视频文件的帧率（fps） */
    unsigned int frame_rate;
    /*! 视频的宽（pixels） */
    unsigned int width;
    /*! 视频的高（pixels） */
    unsigned int height;
    /*! 文件中的视频流比特率 */
    unsigned int video_bitrate;
    /*! 音频采样率 */
    unsigned int sample_rate;
    /*! 文件中的音频流比特率 */
    unsigned int audio_bitrate;
    /*! 音频流的声道数 */
    unsigned int channels;
    /*! 视频文件的drm信息 */
    mmm_drm_type_t drm_type;
    /*! amvb信息 */
    amvb_info_t amvb_info;
} mmm_video_file_info_t;
/*!
 *  \brief
 *     视频引擎的状态信息
 */
typedef struct
{
    /*! 当前的播放时间，单位为ms */
    unsigned int cur_time;
    /*! 视频播放引擎的状态 */
    mmm_player_status_t player_status;
    /*! 中间件KSHOW录音状态 */
    mmm_k_record_status_t record_status;
    /*! 中间件KSHOW练唱状态 */
    mmm_vocalize_status_t vocalize_status;
    /*! 中间件错误类型 */
    unsigned int err_no;
    /*! 当前的ab播放状态 */
    mmm_ab_status_t ab_flag;
    /*! 当前磁盘的剩余空间 */
    unsigned int space_left;
} mmm_video_status_t;
/*!
 *  \brief
 *     音频录音的音源
 */
typedef struct
{
    /*! 内部麦克风 */
    unsigned int mic :1;
    /*! FM */
    unsigned int fm :1;
    /*! 由line-in连接的声音输入 */
    unsigned int line_in :1;
    /*! 暂未提供 */
    unsigned int iis :1;
    /*! 暂未提供 */
    unsigned int spdif :1;
    /*! 暂未提供 */
    unsigned int tv :1;
    /*! 保留位 */
    unsigned int reserved :26;
} mmm_kaudio_source_t;
/*!
 *  \brief
 *     录音增益要求
 */
typedef struct
{
    /*! 声音的输入增益，以mv为单位*/
    unsigned int input_gain;
    /*! 声音的输出增益，以mv为单位*/
    unsigned int output_gain;
} mmm_kvoice_gain_t;
/*!
 *  \brief
 *     练唱的参数
 */
typedef struct
{
    /*! 音源 */
    mmm_kaudio_source_t ai_source;
    /*! 录音增益要求*/
    mmm_kvoice_gain_t voice_gain;
    /*! 音频采样精度，目前只支持16bits */
    char bpp;
    /*! 声道数，目前支持双声道 */
    char channel;
    /*! 采样率，目前只能设置成32K */
    unsigned int sample_rate;
    /*! 比特率*/
    unsigned int bitrate;
    /*! 音频编码的模式，主要为wav录音时需要区分编码模式为adpcm或者pcm，目前只支持adpcm，也即默认设0*/
    unsigned int encode_mode;
    /*! 练唱的混音比 */
    int mix_propotion;
} mmm_vocalize_param_t;

/*!
 *  \brief
 *     视频的标签数据
 */
typedef struct
{
    /*! 标签对应的视频时间点 XXms*/
    unsigned int time;
    /*! 其他标签信息，由解码器确定 */
    unsigned char* buf;
} mmm_video_tag_t;
/*!
 *  \brief
 *     视频显示的区域
 */
typedef struct
{
    /*! 视频区域在屏幕x方向上的偏移 */
    unsigned int xpos;
    /*! 视频区域在屏幕y方向上的偏移 */
    unsigned int ypos;
    /*! 视频区域的宽 */
    unsigned int width;
    /*! 视频区域的高 */
    unsigned int height;
} mmm_video_area_t;
/*!
 *  \brief
 *     当前显示的视频帧的数据
 */
typedef struct
{
    /*! 当前视频数据显示的x方向偏移 */
    unsigned int xpos;
    /*! 当前视频数据显示的y方向偏移 */
    unsigned int ypos;
    /*! 当前视频的宽 */
    unsigned int width;
    /*! 当前视频的高 */
    unsigned int height;
    /*! 转换后的视频帧数据 */
    char *buf;
} mmm_cur_video_t;

void * mmm_vd_open(void *param);
int mmm_vd_cmd(void *handle, unsigned int cmd, unsigned int param);
int mmm_vd_close(void *handle, void *param);
/*! \endcond*/

/*! \cond MMM_MUSIC_API*/
/*!
 *  \brief
 *     提供给应用层的音频播放命令
 */
typedef enum
{
    /*! 淡入*/
    MMM_FADE_IN = 0x61,
    /*! 淡出*/
    MMM_FADE_OUT,
    /*! 设置EQ*/
    MMM_SET_EQ,
    /*! 设置能量门限值*/
    MMM_SET_ENERGY_DOOR,
    /*! 设置能量步长*/
    MMM_SET_ENERGY_STEP,
    /*! 使能频谱显示*/
    MMM_ENABLE_SPECTRUM_SHOW,
    /*! 关闭频谱显示*/
    MMM_DISABLE_SPECTRUM_SHOW,
    /*! 调整播放的速度*/
    MMM_ALTER_PLAY_SPEED,
    /*! 设置快进快退之间的声音输出*/
    MMM_SET_FFB_PLAY_TIME,
    /*! 保留命令1*/
    MMM_RESERVED1,
    /*! 保留命令2*/
    MMM_RESERVED2
} mmm_music_decoder_cmd_t;
/*!
 *  \brief
 *     音乐文件信息
 */
typedef struct
{
    /*! 文件格式*/
    char file_format[8];
    /*! 音乐总时间,以秒（s）为单位*/
    unsigned int total_time;
    /*! 音乐比特率，以Kbps为单位*/
    unsigned int bitrate;
    /*! 音乐采样率，以KHz为单位*/
    unsigned int sample_rate;
    /*! 音乐声道数*/
    unsigned int channels;
    /*! 采样精度*/
    unsigned int bpp;
    /*! 是否是drm歌曲*/
    mmm_drm_type_t drm_type;
} mmm_music_file_info_t;
/*!
 *  \brief
 *     断点、标签信息结构
 */
typedef struct
{
    /*! 断点时间 xxms*/
    unsigned int time;
    /*! buf信息*/
    char *buf;
} mmm_audio_tag_t;
/*!
 *  \brief
 *     EQ模式
 */
typedef enum
{
    /*! 普通模式*/
    MMM_NORMAL = 0,
    /*! 摇滚模式*/
    MMM_ROCK,
    /*! 流行模式*/
    MMM_POP,
    /*! 古典模式*/
    MMM_CLASSIC,
    /*! 柔和模式*/
    MMM_SOFT,
    /*! 爵士模式*/
    MMM_JAZZ,
    /*! 重低音模式*/
    MMM_DBB,
    /*! 用户模式*/
    MMM_EQ_USR_MODE,
    /*! SRS模式*/
    MMM_SRS = 0x40,
    /*! SRS下的WOW模式*/
    MMM_WOW = 0x41,
    /*! TRUE_BASS模式*/
    MMM_TRUE_BASS = 0x42,
    /*! SRS用户模式*/
    MMM_SRS_USR_MODE = 0x4f
} mmm_eq_mode_t;
/*!
 *  \brief
 *     设置eq时的参数
 */
typedef struct
{
    /*!EQ模式*/
    mmm_eq_mode_t eq_mode;
    /*! 在用户模式时用户需要设置的参数*/
    int param[10];
} mmm_eq_set_t;
/*!
 *  \brief
 *     部分当前是否已设置状态标志
 */
typedef struct
{
    /*!当前是否在淡出状态*/
    unsigned fade_out_flag :1;
    /*!当前是否静音状态*/
    unsigned silence_flag :1;
    /*!当前是否处于ab播放状态*/
    unsigned AB_play_flag :1;
    /*!当前是否已到达B点*/
    unsigned reach_B_flag :1;
    /*!当前是否在B处的等待时间已到，要返回到A点*/
    unsigned return_A_flag :1;
} mmm_status_flag_t;
/*!
 *  \brief
 *     音乐状态
 */
typedef struct
{
    /*!歌曲当前的播放时间,xxms*/
    int cur_time;
    /*!中间件音频解码目前所处的状态*/
    mmm_player_status_t status;
    /*!错误号*/
    unsigned int err_no;
    /*!部分当前是否已设置状态标志*/
    mmm_status_flag_t flag;
    /*!当前歌曲的比特率信息*/
    unsigned int cur_bps;
    /*!当前输出的能量值*/
    unsigned int cur_power;
    /*!频谱,18个频率点.*/
    unsigned char spectrum[18];
} mmm_music_status_t;

void *mmm_ad_open(void *param);
int mmm_ad_cmd(void *handle, unsigned int cmd, unsigned int param);
int mmm_ad_close(void *handle, void *param);
/*! \endcond*/

/*! \cond MMM_IMAGE_API*/
/*!
 *  \brief
 *     提供给应用层的图片解码命令
 */
typedef enum
{
    /*! 设置专集图片信息*/
    MMM_SET_ALBUM_INFO = 0x91,
    /*! 设置BUF图片信息*/
    MMM_SET_MEDIA_BUFFER,
    /*! 获取解码信息*/
    MMM_GET_DECODER_STATUS,
    /*! 图片解码*/
    MMM_IMG_DECODE,
    /*! 图片缩略图解码*/
    MMM_IMG_BROWSE,
    /*! 取消图片解码*/
    MMM_ABORT_DECODE,
    /*! 重新定位解码图片大小*/
    MMM_RESIZE,
    /*! 图片移动*/
    MMM_MOVE,
    /*! 图片缩放*/
    MMM_ZOOM,
    /*! 图片旋转*/
    MMM_ROTATE,
    /*! 图片镜像对称变换*/
    MMM_MIRROR,
    /*! 设置解码的回调函数*/
    MMM_SET_CALLBACK,
    /*! 设置解码时需要获取alpha数据，
     后续MMM_IMG_DECODE传递的buf增加大小width*height，由于存储alpha数据*/
    MMM_SET_ALPHA_DEC
} mmm_image_decoder_cmd_t;
/*!
 *  \brief
 *     颜色格式
 */
typedef enum
{
    /*! YUV格式*/
    MMM_YUV,
    /*! RGB格式*/
    MMM_RGB
} mmm_color_format_t;
/*!
 *  \brief
 *     中间件图片解码状态
 */
typedef enum
{
    /*! 解码状态*/
    MMM_IMAGE_DECODER_BUSY,
    /*! 处于空闲状态*/
    MMM_IMAGE_DECODER_FREE,
    /*! 处于错误状态*/
    MMM_IMAGE_DECODER_ERROR
} mmm_imgdec_status_t;

/*!
 * \brief
 *      图片解码情况
 */
typedef struct
{
    /*! 解码状态*/
    mmm_imgdec_status_t status;
    /*! 错误号*/
    unsigned int err_no;
    /*! 文件是否达到尾部的标志*/
    unsigned int end_of_file;
} mmm_image_status_t;

/*!
 *  \brief
 *      图片缩放模式
 */
typedef enum
{
    /*! 放大*/
    MMM_IMG_ZOOMIN,
    /*! 缩小*/
    MMM_IMG_ZOOMOUT
} mmm_image_zoom_t;

/*!
 *  \brief
 *     旋转方向
 */
enum mmm_image_rotation_t
{
    /*! 顺时针旋转90度*/
    MMM_IMG_LEFT90,
    /*! 逆时针旋转90度*/
    MMM_IMG_RIGHT90,
    /*! 旋转180度*/
    MMM_IMG_R180
};

/*!
 *  \brief
 *    镜像对称变换
 */
enum mmm_image_mirror_t
{
    /*! 上下颠倒*/
    MMM_IMG_UP_DOWN,
    /*! 左右颠倒（镜像）*/
    MMM_IMG_LEFT_RIGHT
};

/*!
 *  \brief
 *     移动图片
 */
enum mmm_image_move_direction_t
{
    /*! 向上*/
    MMM_IMG_UP,
    /*! 向下*/
    MMM_IMG_DOWN,
    /*! 向左*/
    MMM_IMG_LEFT,
    /*! 向右*/
    MMM_IMG_RIGHT
};

/*!
 * \brief
 *      图片日期
 */
typedef struct
{
    /*! 年*/
    unsigned int year;
    /*! 月*/
    unsigned int month;
    /*! 日*/
    unsigned int day;
} mmm_img_date_t;

#define MAX_ASCII_LEN 120
/*!
 * \brief
 *      图片其他信息
 */
typedef struct
{
    /*! 制造商名字*/
    char make[MAX_ASCII_LEN];
    /*! 设备模式*/
    char model[MAX_ASCII_LEN];
    /*! 软件说明*/
    char software[MAX_ASCII_LEN];
    /*! 图片拍摄日期*/
    char datetime[20];
    /*! 图片拍摄者*/
    char artist[MAX_ASCII_LEN];
    /*! 制造版权说明*/
    char copyright[MAX_ASCII_LEN];
    /*! 图象描述*/
    char imageDescription[MAX_ASCII_LEN];
    /*! 制造商说明*/
    char makernote[MAX_ASCII_LEN];
    /*! 暴光时间*/
    char exposuretime[MAX_ASCII_LEN];
    /*! 照相机光圈*/
    char F_number[MAX_ASCII_LEN];
    /*! 暴光提要*/
    char exposureProg[MAX_ASCII_LEN];
    /*! 图片文件包含的总帧数（如gif的总帧数）*/
    short iso;
    /*! 元素配置*/
    char componentConfig[5];
    /*! 快门速度*/
    char shutterSpeed[MAX_ASCII_LEN];
    /*! 表型*/
    char meterModel[MAX_ASCII_LEN];
    /*! 光源*/
    char lightSource[MAX_ASCII_LEN];
    /*! 制作时的flash相关说明*/
    char flash[MAX_ASCII_LEN];
    /*! 焦长*/
    char focalLength[MAX_ASCII_LEN];
    /*! 图象宽度*/
    long width;
    /*! 图象高度*/
    long height;
    /*! Exif版本*/
    char exifVersion[4];
    /*! GPS 版本*/
    char GPSVersion[4];
    /*! GPS纬度参考*/
    char GPSLatitudeRef[4];
    /*! GPS纬度*/
    char GPSLatitude[MAX_ASCII_LEN];
    /*! GPS经度参考*/
    char GPSLongtitudeRef[4];
    /*! GPS经度*/
    char GPSLongtitude[MAX_ASCII_LEN];
} mmm_image_exif_t;

/*!
 * \brief
 *      图片信息
 */
typedef struct
{
    /*! 图片格式*/
    char file_format[8];
    /*! 图片宽度*/
    unsigned int width;
    /*! 图片高度*/
    unsigned int height;
    /*! 图片日期*/
    mmm_img_date_t date;
    /*! 图片其他信息*/
    mmm_image_exif_t *exif;
} mmm_img_file_info_t;

/*!
 * \brief
 *      图片解码参数
 */
typedef struct
{
    /*! 图片精度，即每一象素点对应的byte数，如2、3*/
    unsigned int bpp;
    /*! 图片解码格式（YUVorRGB）*/
    unsigned int formate;
    /*! 要求解码输出的图片的宽*/
    unsigned int rect_width;
    /*! 要求解码输出的图片的高*/
    unsigned int rect_height;
    /*! 实际解码输出的图片的宽*/
    unsigned int img_width;
    /*! 要求解码输出的图片的高*/
    unsigned int img_height;
    /*! 要求解码输出的图片数据存储的buf地址*/
    unsigned char *buf;
    /*! buffer地址的长度*/
    unsigned int len;
} mmm_img_info_t;
/*!
 * \brief
 *      存储与buf图片的信息结构
 */
typedef struct
{
    /*! buffer地址*/
    char *media_buf;
    /*! buffer长度*/
    int media_len;
} mmm_media_buf_t;
/*!
 * \brief
 *      专集图片信息结构
 */
typedef struct
{
    /*! album信息所在的文件*/
    char *file_name;
    /*! album信息在文件中的偏移*/
    int data_offset;
    /*! album信息的长度*/
    int data_len;
} mmm_album_info_t;

void *mmm_id_open(void *param);
int mmm_id_cmd(void *handle, unsigned int cmd, unsigned int param);
int mmm_id_close(void *handle, void *param);

/*! \endcond*/

#ifdef __cplusplus
}
#endif

#endif
