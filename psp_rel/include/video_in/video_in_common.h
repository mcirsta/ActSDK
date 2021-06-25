/*
 ********************************************************************************
 *                   video in device's driver
 *                (c) Copyright 2007 - 2010, Actions Co,Ld.
 *                        All Right Reserved
 *
 * History:
 *      <author>      <time>       <version >    <desc>
 *       houjingkun      2007/08/14     1.0   build this file
 *       houjingkun      2008/06/18     1.1   check code for QAC and add commentary.
 *       houjingkun      2008/08/04     1.2   change name from actions213x_sensor.h to video_in.h.
 ********************************************************************************/
/*!
 * \file   video_in_common.h
 * \brief
 *      video_in 驱动的通用接口头文件，不论是sensor还是tvin等视频输入设备，都具有这些通用的视频输入接口。
 * \author houjingkun
 * \par GENERAL DESCRIPTION:
 *      video_in 驱动的通用接口头文件，不论是sensor还是tvin等视频输入设备，都具有这些通用的视频输入接口。
 * \par EXTERNALIZED FUNCTIONS:
 *       null
 *
 *     Copyright(c) 2008-2012 Actions Semiconductor, All Rights Reserved.
 *
 *  \version 1.1
 *  \date  2008/06/18
 *******************************************************************************/

#ifndef __VIDEO_IN_COMMON_H__
#define __VIDEO_IN_COMMON_H__

/********************************************************/
/* below is video in driver ioctl func's command. */
#define VI_IO_CMD_BASE   0x47300
/********************************************************/

/* related with video in basical infomation. */
#define VI_IOSET_RESOLUTION         (VI_IO_CMD_BASE + 1)
#define VI_IOGET_RESOLUTION         (VI_IO_CMD_BASE + 25)
#define VI_IOGET_PIXEL_BITMAP       (VI_IO_CMD_BASE + 33)

#define VI_IOGET_BUFF_LEN           (VI_IO_CMD_BASE + 21)
#define VI_IOGET_PHYS_ADDR          (VI_IO_CMD_BASE + 24)

/* related with capturing data. */
#define VI_IOSET_CYCLE_START        (VI_IO_CMD_BASE + 9)
#define VI_IOSET_CYCLE_STOP         (VI_IO_CMD_BASE + 10)
#define VI_IOGET_CYCLE_FRAME        (VI_IO_CMD_BASE + 22)
#define VI_IOGET_CYCLE_FRAME_TRY    (VI_IO_CMD_BASE + 34)
#define VI_IOGET_ONE_FRAME          (VI_IO_CMD_BASE + 23)
#define VI_IOFREE_CYCLE_FRAME       (VI_IO_CMD_BASE + 41)

#define VI_IOGET_PAD_FRAMES         (VI_IO_CMD_BASE + 35)
#define VI_IOGET_FRAME_CNT          (VI_IO_CMD_BASE + 36)
#define VI_IOGET_LOST_FRAMES        (VI_IO_CMD_BASE + 37)

#define VI_IOGET_RATE_BITMAP        (VI_IO_CMD_BASE + 39)
#define VI_IOSET_FRAME_RATE         (VI_IO_CMD_BASE + 40)
#define VI_IOGET_FRAME_RATE         (VI_IO_CMD_BASE + 44)

#define VI_IOSET_PIXEL_FORMAT       (VI_IO_CMD_BASE + 42)
#define VI_IOGET_PIXEL_FORMAT       (VI_IO_CMD_BASE + 43)

#define VI_IOGET_TVIN_SIGNAL        (VI_IO_CMD_BASE + 46)

/********* video in picture's resolution type.  *********/
/*!
 * 主流的各种不同分辨率类型：
 * \li  128 x 96 subQCIF 4:3
 * \li  176 X 144 QCIF 11:9
 * \li  176 X 220 QCIF Plus (176x220)
 * \li  320 x 200 CGA Color-Graphics-Adapter 8:5
 * \li  320 x 240 Quarter-VGA 4:3
 * \li  352 x 288 CIF 11:9
 * \li  640 x 350 EGA Extended-Graphics-Adapter 64:35
 * \li  640 x 480 VGA Video-Graphics-Array 4:3
 * \li  800 x 600 SVGA Super-Video-Graphics-Array 4:3
 * \li  1024 x 768 XGA eXtended-VGA 4:3
 * \li  1280 x 768 XGA-W 15:9
 * \li  1280 x 960 QVGA Quad-VGA 4:3
 * \li  1280 x 1024 SXGA Super-eXtended-VGA 5:4
 * \li  1400 x 1050 SXGA+ Super-eXtended-VGA-plu 4:3
 * \li  1600 x 1024 SXGA-W 25:16
 * \li  1600 x 1200 UGA Ultra-VGA 4:3
 * \li  1920 x 1080 HDTV High-Definition-TV 16:9
 * \li  1900 x 1200 UXGA Ultra-eXtended-VGA 19:12
 * \li  1920 x 1200 UXGA-W 8:5
 * \li  2048 x 1536 QXGA Quad-eXtended-VGA 4:3
 * \li  2560 x 2048 QSXGA Quad-Super-eXtended-VGA 5:4
 * \li  3200 x 2400 QUXGA Quad-Ultra-eXtended-VGA 4:3
 * \li  3840 x 2400 QUXGA-W 8:5
 */
typedef enum
{
    VI_RES_128X96 = 0,
    VI_RES_160X120,
    VI_RES_176X144,
    VI_RES_220X176,
    VI_RES_224X176,
    VI_RES_320X240,
    VI_RES_352X280,
    VI_RES_640X480,
    VI_RES_800X600,
    VI_RES_1024X768,
    VI_RES_1280X960,
    VI_RES_1280X1024,
    VI_RES_1600X1024,
    VI_RES_1600X1200,
    VI_RES_1920X1080,
    VI_RES_1900X1200,
    VI_RES_1920X1200,
    VI_RES_2048X1536,
    VI_RES_2560X2048,
    VI_RES_3200X2400,
    VI_RES_3840X2400,

    //VI_SENSOR_RES_EXTEND,   /* extend resolution for sensor. */
    //VI_TVIN_RES_EXTEND,     /* extend resolution for tvin. */
    //VI_OTHER_RES_EXTEND,     /* extend resolution for other video in device. */

    VI_TVIN_RES_PAL320X240,
    VI_TVIN_RES_PAL360X288, /* standard pal resolution */
    VI_TVIN_RES_PAL352X288,
    VI_TVIN_RES_PAL720X576,

    VI_TVIN_RES_NTSC320X240,
    VI_TVIN_RES_NTSC352X240,
    VI_TVIN_RES_NTSC360X240, /* standard ntsc resolution */
    VI_TVIN_RES_NTSC720X480
} vi_resolution_t; /* current supported resolution. */

/*! video in's input pixel data format.
 *  \li     VI_PIX_FMT_4Y4U4Y4V --    Y1Y2Y3Y4 U1U2U3U4 Y5Y6Y7Y8 V5V6V7V8.
 *  \li     VI_PIX_FMT_YUV420 --      Y1Y2...Yn; U1U2..Un/4; V1V2..Vn/4.
 *  \li     VI_PIX_FMT_YUV422 --      Y1 U1 Y2 V2 Y3 U3 Y4 V4...
 */
typedef enum
{
    VI_PIX_FMT_4Y4U4Y4V = 0, /* Y1Y2Y3Y4 U1U2U3U4 Y5Y6Y7Y8 V5V6V7V8. */
    VI_PIX_FMT_YUV420, /* Y1Y2...Yn; U1U2..Un/4; V1V2..Vn/4. */
    VI_PIX_FMT_YUV422,
/* Y1 U1 Y2 V2 Y3 U3 Y4 V4... */
} vi_pix_fmt_t;

/*!
 * used in install driver, provided for vi_dev_t's app_type.
 *  \li     VI_DC_MODE --           DC mode.
 *  \li     VI_DV_MODE --           DV mode.
 *  \li     VI_PC_CAMERA_MODE --    PC Camera mode.
 *  \li     VI_ANALOG_TVIN_MODE --  Analog Tvin mode, based on telegent tvin IC.
 *  \li     VI_CVBS_TVIN_MODE --    CVBS Tvin mode, based on IC of ADI 7180 or TI 5150 and so on.
 */
typedef enum
{
    VI_DC_MODE = 0,
    VI_DV_MODE,
    VI_PC_CAMERA_MODE,
    VI_ANALOG_TVIN_MODE,
    VI_CVBS_TVIN_MODE,
    VI_SMALL_MEM_MODE
} vi_work_mode_t;

#endif  /* __VIDEO_IN_COMMON_H__ */

