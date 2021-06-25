/*
 ********************************************************************************
 *                   video in device's driver
 *                (c) Copyright 2007 - 2010, Actions Co,Ld.
 *                        All Right Reserved
 *
 * History:
 *      <author>      <time>       <version >    <desc>
 *       houjingkun   2008/12/11     1.0       first provide for analog tvin driver.
 ********************************************************************************/
/*!
 * \file   analog_tv.h
 * \brief
 *      analog tvin 驱动独有的接口头文件，当视频输入设备为anlog tvin的时候，需要使用到该头文件里面的接口。
 * \author houjingkun
 * \par GENERAL DESCRIPTION:
 *      analog tvin 驱动独有的接口头文件，当视频输入设备为anlog tvin的时候，需要使用到该头文件里面的接口。
 * \par EXTERNALIZED FUNCTIONS:
 *       null
 *
 *     Copyright(c) 2008-2012 Actions Semiconductor, All Rights Reserved.
 *
 *  \version 1.1
 *  \date  2009/02/02
 *******************************************************************************/

#ifndef __ANALOG_TV_H__
#define __ANALOG_TV_H__

//#define SENSOR_IO_CMD_BASE   0x47300
#define ATV_IO_CMD_BASE   0x47500

/*
 * 设置当前模拟电视收看的国家或地区.
 * example code:  ioctl(fd, ATVIN_IOSET_ZONE, ZONE_PALD);
 */
#define ATVIN_IOSET_ZONE                (ATV_IO_CMD_BASE + 62)

/*
 * 获取当前模拟电视收看的国家或地区相关节目信息.
 * example code:
 *  atv_zone_info_t zone_info;
 *  ioctl(fd, ATVIN_IOGET_ZONE_INFO, &zone_info);
 */
#define ATVIN_IOGET_ZONE_INFO           (ATV_IO_CMD_BASE + 64)

/*
 * 设置当前所要收看的频道.
 * example code:  ioctl(fd, ATVIN_IOSET_CHANNEL, number);
 */
#define ATVIN_IOSET_CHANNEL             (ATV_IO_CMD_BASE + 66)

/*
 * 获取当前电视频道的相关信息.
 * example code:
 *  atv_channel_info_t channel_info;
 *  ioctl(fd, ATVIN_IOGET_CHANNEL_INFO, &channel_info);
 */
#define ATVIN_IOGET_CHANNEL_INFO        (ATV_IO_CMD_BASE + 68)

/*!
 * provided for ATVIN_IOSET_ZONE
 * \li ZONE_FM --             Default Channel map for fm.
 * \li ZONE_NTSC --           Default Channel Maps for USA, Canada, Korea, Taiwan, Mexico, Chile, Venezuela, the Philippines
 * \li ZONE_PALD --           Default Channel Map for China
 * \li ZONE_VIETNAM --        Default Channel Map for Vietnam
 * \li ZONE_W_EUROPE --       Default Channel Map for Western Europe, Turkey, UAE, Afghanistan, Singapore, Thailand, Cambodia, Indonesia, Malaysia, Myanmar, and Laos
 * \li ZONE_NTSC_JAPAN --     Default Channel Map for ntsc_japan
 * \li ZONE_A_UK --           Default Channel Maps for UK, Hong Kong, and South Africa
 * \li ZONE_BRAZIL --         Default Channel Map for Brazil
 * \li ZONE_ARGENTINA --      Default Channel Map for Argentina
 * \li ZONE_PALD_SHENZHEN --  Default Channel Map for shenzhen
 */
typedef enum
{
    ZONE_FM,
    ZONE_NTSC,
    ZONE_PALD,
    ZONE_VIETNAM,
    ZONE_W_EUROPE,
    ZONE_NTSC_JAPAN,
    ZONE_A_UK,
    ZONE_BRAZIL,
    ZONE_ARGENTINA,
    ZONE_PALD_SHENZHEN,
} atv_zone_t;

/*!
 * provided for ATVIN_IOGET_ZONE_INFO
 */
typedef struct atv_zone_info_s
{
    /*! current zone, refer to atv_zone_t. */
    unsigned int zone;
    /*! all area zone count, refer to atv_zone_t. */
    unsigned int zone_count;
    /*! the channel count that current zone support. */
    unsigned int chn_count;
} atv_zone_info_t;

/*!
 * provided for ATVIN_IOGET_CHANNEL_INFO
 */
typedef struct atv_channel_info_s
{
    /*! channel number, range: 0 -- atv_zone_info.chn_count. */
    unsigned int chn;
    /*! current channel's frequent. */
    unsigned int hz;
    /*! bandwidth, refer to atv_bw_t. */
    unsigned int bw;
    /*! channel standard, refer to atv_vidstd_t. */
    unsigned int chstd;
    /*! station available or not. refer to atv_station_av_t. */
    unsigned int available;
    /*! atv or fm, refer to atv_mode_t. if you set zone to ZONE_FM, mode is ATV_MODE_FM_RADIO. */
    unsigned int mode;
    /*! refer to atv_intensity_t. */
    unsigned int intensity;
} atv_channel_info_t;

/*!
 * Supported frequency bandwidth settings for ATV API.
 * \li ATV_BW_5 -- 5 MHz bandwidth
 * \li ATV_BW_6 -- 6 MHz bandwidth
 * \li ATV_BW_7 -- 7 MHz bandwidth
 * \li ATV_BW_8 -- 8 MHz bandwidth
 * \li ATV_BW_NONE -- no bandwidth.
 */
typedef enum
{
    ATV_BW_5 = 5,
    ATV_BW_6 = 6,
    ATV_BW_7 = 7,
    ATV_BW_8 = 8,
    ATV_BW_NONE
} atv_bw_t;

/*!
 * Modes for TLG3100.
 * \li ATV_MODE_NONE --          No Mode specified
 * \li ATV_MODE_ANALOG_TV --     Analog Television mode
 * \li ATV_MODE_FM_RADIO --      FM Radio mode
 */
typedef enum
{
    ATV_MODE_NONE,
    ATV_MODE_ANALOG_TV,
    ATV_MODE_FM_RADIO
} atv_mode_t;

/*!
 * Video Standards to configure TLG3100.
 */
typedef enum
{
    ATV_VSTD_NTSC_M = 0, /* NTSC-M                                 */
    ATV_VSTD_PAL_M = 1, /* PAL-M                                  */
    ATV_VSTD_PAL_B = 2, /* PAL-B                                  */
    ATV_VSTD_PAL_B1 = 3, /* PAL-B1                                 */
    ATV_VSTD_PAL_D = 4, /* PAL-D                                  */
    ATV_VSTD_PAL_D1 = 5, /* PAL-D1                                 */
    ATV_VSTD_PAL_G = 6, /* PAL-G                                  */
    ATV_VSTD_PAL_H = 7, /* PAL-H                                  */
    ATV_VSTD_PAL_K = 8, /* PAL-K                                  */
    ATV_VSTD_PAL_N = 9, /* PAL-N                                  */
    ATV_VSTD_PAL_I = 10, /* PAL-I                                  */
    ATV_VSTD_PAL_NC = 11, /* PAL-NC                                 */
    ATV_VSTD_NONE
/* No Video Standard Specified            */
} atv_vidstd_t;

/*!
 * signed station available or not.
 * \li ATV_UNAVAILABLE -- station unavailable.
 * \li ATV_AVAILABLE -- station available.
 * \li ATV_UNCERTAIN -- station uncertain.
 */
typedef enum
{
    ATV_UNAVAILABLE,
    ATV_AVAILABLE,
    ATV_UNCERTAIN,
} atv_station_av_t;

/*!
 * provided for atv_channel_info_t.
 */
typedef enum
{
    ATV_INTENSITY_TINY, /* almost no signal. */
    ATV_INTENSITY_LOW,
    ATV_INTENSITY_NORMAL,
    ATV_INTENSITY_HIGH,
} atv_intensity_t;

#endif  /* __ANALOG_TV_H__ */

