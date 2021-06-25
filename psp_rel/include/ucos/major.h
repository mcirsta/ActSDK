/********************************************************************************
 *                              USDK(1100)
 *                            Module: MAJOR
 *                 Copyright(c) 2003-2008 Actions Semiconductor,
 *                            All Rights Reserved.
 *
 * History:
 *      <author>    <time>           <version >             <desc>
 *       hmwei     2008-8-30 16:45     1.0             build this file
 ********************************************************************************/
/*!
 * \file     major.h
 * \brief    主设备号定义头文件
 * \author   hmwei
 * \version 1.0
 * \date  2008/8/30
 *******************************************************************************/
#ifndef __MAJOR_H__
#define __MAJOR_H__

/*char devices major num.*/
#define MAJOR_VRAM      1

#define MAJOR_PM        2

#define MAJOR_UMONITOR  10
#define MAJOR_UOC       11
#define MAJOR_UDC       12
#define MAJOR_UDC4UVC   12
#define MAJOR_MSC       13
#define MAJOR_UVC       13
#define MAJOR_UPGRADE   13

#define MAJOR_FFT       20
#define MAJOR_FB        29

#define MAJOR_TUNNEL    60
#define MAJOR_MTPDRM    61

#define MAJOR_USTOR     70
#define MAJOR_UCORE     71
#define MAJOR_UHC       72
#define MAJOR_HOTPLUG   73

#define MAJOR_KERNINFO  229
#define MAJOR_RTC       230
#define MAJOR_SOUND     231
#define MAJOR_KEY       232
#define MAJOR_GSENSOR   233
#define MAJOR_TP        235
#define MAJOR_CHARGER   236
#define MAJOR_TVOUT     237

#define MAJOR_FM        239
#define MAJOR_LCD       240

#define MAJOR_MHA       247
#define MAJOR_VIDEO_IN  248     /* sensor or tvin and so on. */
#define MAJOR_ADA       249
#define MAJOR_WD        250
#define MAJOR_DSP_AD    251
#define MAJOR_DSP_AE    252

#define MAJOR_ADRM      253
#define MAJOR_ALARM     254

//char device name
#define DEVICE_VRAM     "/dev/vram"
#define DEVICE_PM       "/dev/pm"

#define DEVICE_UMONITOR "/dev/umonitor"
#define DEVICE_UOC      "/dev/uoc"
#define DEVICE_UDC      "/dev/udc"
#define DEVICE_UDC4UVC  "/dev/udc4uvc"
#define DEVICE_MSC      "/dev/msc"
#define DEVICE_UVC      "/dev/uvc"
#define DEVICE_UPGRADE  "/dev/upgrade"

#define DEVICE_FFT      "/dev/fft"
#define DEVICE_FB       "/dev/fb"

#define DEVICE_USTOR    "/dev/ustor"
#define DEVICE_UCORE    "/dev/ucore"
#define DEVICE_UHC      "/dev/uhc"
#define DEVICE_HOTPLUG  "/dev/hotplug"

#define DEVICE_KERNINFO "/dev/kerninfo"
#define DEVICE_RTC      "/dev/rtc"

#define DEVICE_KEY      "/dev/key"
#define DEVICE_GSENSOR  "/dev/gsensor"

#define DEVICE_TP       "/dev/tp"
#define DEVICE_CHARGER  "/dev/charger"
#define DEVICE_TVOUT    "/dev/tvout"

#define DEVICE_FM       "/dev/fm"
#define DEVICE_LCD      "/dev/lcd"

#define DEVICE_MHA      "/dev/mha"
#define DEVICE_VIDEO_IN "/dev/video_in"    /* sensor or tvin and so on. */
#define DEVICE_ADA      "/dev/ada"
#define DEVICE_WD       "/dev/wd"
#define DEVICE_DSP_AD   "/dev/dsp_ad"
#define DEVICE_DSP_AE   "/dev/dsp_ae"
#define DEVICE_ADRM     "/dev/adrm"
#define DEVICE_MTPDRM   "/dev/mtpdrm"
#define DEVICE_TUNNEL   "/dev/tunnel"

#define DEVICE_ALARM   "/dev/alarm"

#define DEVICE_SOUND    "/dev/sound"
/*block devices major num.*/
#define MAJOR_FLASH     1
#define MAJOR_CARD      254

/*block devices name*/
#define DEVICE_FLASH    "/dev/flash"
#define DEVICE_CARD     "/dev/card"

/*mount root dir*/
#define ROOT_REMOTE "/mnt/remotedisk"

#endif/*__MAJOR_H__*/
