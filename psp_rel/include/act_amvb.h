/*
 ********************************************************************************
 *
 *			amvb structure
 *                (c) Copyright 2002-2003, Actions Co,Ld.
 *                        All Right Reserved
 *
 * File   : amvb_struct.h
 * by	 : AL/FW
 * Version: 1> v1.00     first version     01.09.2007
 ********************************************************************************
 */
#ifndef __ACT_AMVB_H__
#define	__ACT_AMVB_H__

#ifdef __cplusplus
extern "C"
{
#endif

/*!
 * \brief
 *      定义了与amvb 信息有关的数据结构
 */
typedef struct
{
    /*! 节目流总数 */
    int total_prog;
    /*! 节目流语言类型 */
    int prog_language;
    /*! logo标志 */
    int has_logo;
    /*! logo语言类型 */
    int logo_language;
    /*! 音轨总数 */
    int total_track;
    /*! 各音轨的语言类型 */
    int track_language;
    /*! 含有用户信息描述标志 */
    int has_exif;
    /*! 含有字幕信息标志 */
    int lyric;
} amvb_info_t;

/*!
 * \brief
 *      定义了与amvb 用户exif 基本信息有关的数据结构
 */
typedef struct
{
    /*! 出版时间 */
    unsigned short PublishTimes;
    /*! 制作者 */
    char Make[36];
    /*! 媒体类型 */
    char Medium[16];
    /*! 模式 */
    char Mode[24];
    /*! 发行商 */
    char Publish[32];
    /*! 创作时间 */
    char CreatData[32];
    /*! 发行时间 */
    char PublishData[32];
    /*! 首次发行地点 */
    char location[40];
    /*! 版权所有者 */
    char Copyright[24];
    /*! 技术支持 */
    char Technical[56];
    /*! 软件开发 */
    char Software[24];
    /*! 评价 */
    char Comments[256];
} amvb_base_info_t;

/*!
 * \brief
 *      定义了与amvb exif video格式信息有关的数据结构
 */
typedef struct
{
    /*! 视频帧宽度 */
    unsigned short dwWitdh;
    /*! 视频帧高度 */
    unsigned short dwHeight;
    /*! 颜色深度 */
    unsigned short dwBPP;
    /*! 视频播放速率 */
    unsigned int dwBitRate;
    /*! 压缩格式 */
    unsigned int dwCompression;
    /*! 持续时长 */
    unsigned short dwTime;
    /*! 影名 */
    char MovieName[56];
    /*! 主要演员 */
    char Actor[48];
    /*! 关键词 */
    char KeyWord[100];
} amvb_video_info_t;

/*!
 * \brief
 *      定义了与amvb exif audio格式信息有关的数据结构
 */
typedef struct
{
    /*! 音频编码信息 */
    unsigned short dwID;
    /*! 采样率 */
    unsigned int dwSamplerate;
    /*! 采样精度 */
    unsigned short dwSamplebits;
    /*! 比特率 */
    unsigned int dwBitRate;
    /*! 持续时长 */
    unsigned int dwTime;
    /*! 演唱者 */
    char Author[32];
    /*! 歌曲名 */
    char MusicName[64];
    /*! 关键词 */
    char KeyWord[80];
} amvb_audio_info_t;

/*!
 * \brief
 *      定义了与amvb exif信息有关的数据结构
 */
typedef struct
{
    /*! amvb exif 基本信息 */
    amvb_base_info_t base_info;
    /*! amvb exif 视频信息 */
    amvb_video_info_t video_info;
    /*! amvb exif 音频信息 */
    amvb_audio_info_t audio_info;
} amvb_exif_info;

/*!
 * \brief
 *      定义了与amvb 单语言节目信息有关的数据结构
 */
typedef struct
{
    /*! 节目描述长度 */
    unsigned int prog_len;
    /*! 节目描述内容 */
    char *prog_context;
} amvb_siglang_prog_t;

/*!
 * \brief
 *      定义了与amvb 单节目信息有关的数据结构
 */
typedef struct
{
    /*! 对应的节目时间戳 */
    unsigned int prog_time;
    /*! 多语言节目，语言数等于amvb_progtab_t中的total_lang */
    amvb_siglang_prog_t *mutilang_prog;
} amvb_sig_prog_t;

/*!
 * \brief
 *      定义了与amvb 节目信息有关的数据结构
 */
typedef struct
{
    /*! 语言标记 */
    unsigned int lang_classify;
    /*! 总语言数 */
    unsigned int total_lang;
    /*! 多节目信息，节目数getfileinfo得到 */
    amvb_sig_prog_t *muti_prog;
} amvb_progtab_t;

/*!
 * \brief
 *      定义了不同的logo格式类型常量，这些常量用于指明logo格式
 */
typedef enum
{
    /*! bmp24 */
    AMVB_LOGO_FMT_BMP24 = 0x101,
    /*! gif */
    AMVB_LOGO_FMT_GIF,
    /*! bmp565 */
    AMVB_LOGO_FMT_BMP565,
    /*! yuv420 */
    AMVB_LOGO_FMT_YUV420,
} amvb_logo_fmt_t;

/*!
 * \brief
 *      定义了logo信息有关的数据结构
 */
typedef struct
{
    /*! logo的章节 */
    unsigned int logo_chapter;
    /*! logo的顺序序列号 */
    unsigned int logo_serial_no;
    /*! 有效语言类别，只一种有效 [外部输入] */
    unsigned int logo_language;
    /*! 当前logo的具体格式 */
    unsigned int logo_format;
    /*! logo的宽 */
    unsigned int logo_width;
    /*! logo的高 */
    unsigned int logo_height;
    /*! 含格式信息的图像信息 [输出] */
    char *logo_buf;
} amvb_logo_info_t;

#ifdef __cplusplus
}
#endif

#endif

