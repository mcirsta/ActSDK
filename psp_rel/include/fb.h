/********************************************************************************
 *                              NOYA
 *                            Module: soft image layer
 *                 Copyright(c) 2003-2008 Actions Semiconductor,
 *                            All Rights Reserved.
 *
 * History:
 *      <author>    <time>           <version >             <desc>
 *       xkping     2008-09-10 15:00     1.0             build this file
 ********************************************************************************/
/*!
 * \file     fb.h
 * \brief    软图层的头文件
 * \author   xkping
 * \version 1.0
 * \date  2008/9/10
 *******************************************************************************/

#ifndef __FB_H__
#define __FB_H__
#include "display.h"

/*! 定义系统最多可设置的blending区域*/
#define MAX_BLENDING_AREA       0x08

/*! blend 区域的空handle值*/
#define BLENDING_HANLE_NULL     -1

/*相关的数据结构*/
/*!
 * \brief
 *      color_para_t, 颜色属性 .
 */
typedef struct
{
    /*! 颜色值占用的位宽*/
    int length;

    /*! 颜色值在32位中的偏移*/
    int offset;
} fb_color_para_t;

/*FB的相关信息,通过FBIOGET_FB_PARAM获取,FB的起始地址*/
/*在linux中是通过mmap实现,在usdk中通过FBIOGET_FB_ADDR获取*/
/*!
 * \brief
 *      fb_info_t, framebuffer的相关信息 .
 */
typedef struct
{
    /*! framebuffer X方向分辨率*/
    int xres;

    /*! framebuffer Y方向分辨率*/
    int yres;

    /*! 每个象素点所占的bit数*/
    int bits_per_pixel;

    /*! 每行的长度*/
    int line_length;

    /*! RGB三原色中R的属性*/
    fb_color_para_t red;

    /*! RGB三原色中G的属性*/
    fb_color_para_t green;

    /*! RGB三原色中B的属性*/
    fb_color_para_t blue;
} fb_info_t;

/*!
 * \brief
 *      rect_area_t,视频显示区域设置,定义一个矩形区域 .
 */
typedef struct
{
    /*! 坐标之x分量*/
    int x;
    /*! 坐标之y分量*/
    int y;
    /*! 矩形区域宽度*/
    int w;
    /*! 矩形区域高度*/
    int h;
} fb_rect_area_t;

/*!
 * \brief
 *      blending_param_t, 数据混合参数.
 */
typedef struct
{
    /*! 菜单数据的关键color值*/
    unsigned int color_key;
    /*! 数据混合叠加的比例值, 取值范围为(0-255)*/
    unsigned int alpha;
    /*! 数据混合叠加的区域*/
    fb_rect_area_t area;
    /*! blending区域需要叠加的数据buffer地址*/
    unsigned char *blending_buf;
} fb_blending_param_t;

/*!
 * \brief
 *      FBIOSET_BLENDING_AREA_CREATE的参数类型，包括一个输入的fb_blending_param_t属性和一个返回的handle值。
 * */
typedef struct
{
    /*! 建立blend区域后返回的handle值，该值为FBIOSET_BLENDING_AREA_CREATE接口的输出值*/
    int handle;
    /*! 输入的需要创建的blend区域的属性*/
    fb_blending_param_t blend_param;
} fb_blending_create_t;

/*!
 *  \brief
 *      fb_blending_attr_t, blending区域的属性
 */
typedef struct
{
    /*! 菜单数据的关键color值*/
    unsigned int color_key;
    /*! 数据混合叠加的比例值, 取值范围为(0-255)*/
    unsigned int alpha;
} fb_blending_attr_t;

/*!
 *  \brief
 *    fb_blending_setattr_t, 设置blending区域的属性的参数，包括需要设置的handle值。
 *
 * */
typedef struct
{
    /*! 需要设置属性的blending区域的handle*/
    int handle;
    /*! blending区域的属性，包括colorkey and alpha*/
    fb_blending_attr_t blending_attr;
} fb_blending_setattr_t;

/*!
 *  \brief
 *    blending_area_manage_t, 获取blending区域状态的参数。
 * */
typedef struct
{
    /*! blending area 的id号， area编号，编号从[0--(MAX_BLENDING_AREA-1)*/
    int area_id;
    /*! blending area 的状态，显示或隐藏，0-hide, 1-show*/
    int is_show;
    /*! blending area 的区域和属性*/
    fb_blending_param_t blending_param;
} fb_blending_area_manage_t;

/*!
 *  \brief
 *  显示设备
 */
enum DISPLAYER_MODE
{
    /*!  CPU_LCD_DISPLAYER : CPU LCD*/
    CPU_LCD_DISPLAYER = 0x01,
    /*! RGB_LCD_DISPLAYER:RGB LCD*/
    RGB_LCD_DISPLAYER = 0x02,
    /*! TV_DISPLAYER : TVOUT in pal mode*/
    TV_PAL_DISPLAYER = 0x04,
    /*! TV_DISPLAYER : TVOUT in ntsc mode*/
    TV_NTSC_DISPLAYER = 0x08,
};

/*告诉frmebuffer驱动现在是否进入video输出状态*/
/*!进入视频输出状态 */
#define FBIOSET_START_VIDEO             0x4660
/*!停止视频输出状态 */
#define FBIOSET_END_VIDEO               0x4661

/*!获取FB相关参数：如FB的分辨率、支持的数据格式 */
#define FBIOGET_FB_PARAM                0x4662
/*!获取FB的起始地址 */
#define FBIOGET_FB_ADDR                 0x4663

/*!设置视频显示区域 */
#define FBIOSET_VIDEO_AREA              0x4664
/*!获取视频显示区域 */
#define FBIOGET_VIDEO_AREA              0x4665

/*!向输出设备上更新一次fb数据即菜单数据 */
#define FBIOSET_UPDATE_FB               0x4666
/*!向输出设备上输出一帧视频数据 */
#define FBIOSET_UPDATE_VIDEO            0x4667

/*!切换图像输出设备：LCD_DISPLAYER, TV_DISPLAYER */
#define FBIOSET_DISPLAYER               0x4668
/*!获取本case支持的所有图像输出设备：LCD_DISPLAYER, TV_DISPLAYER */
#define FBIOGET_DISPLAYER_LIST          0x4669
/*!获取显示设备的相关参数：如支持的数据格式、行刷或列刷，调用此命令时使用displayer_attr_t接口 */
#define FBIOGET_DISPLAYER_ATTR          0x4670

/*!对FB加锁，加锁后所有的数据不再在屏幕上更新 */
#define FBIOSET_LOCK_FB                 0x4671
/*!对FB解锁 */
#define FBIOSET_UNLOCK_FB               0x4672

/*!设置屏的方式 */
#define FBIOSET_SCREEN_ROTATE           0x4673
/*!设置mmm video的备份buffer地址 */
#define FBIOSET_VIDEO_BACKUP_BUF        0x4674

/*!建立blending area */
#define FBIOSET_BLENDING_AREA_CREATE    0x4675
/*!show blending area */
#define FBIOSET_BLENDING_AREA_SHOW      0x4676
/*!hide blending area */
#define FBIOSET_BLENDING_AREA_HIDE      0x4677
/*!set blending area 的属性 */
#define FBIOSET_BLENDING_AREA_SETATTR   0x4678
/*!销毁blending area */
#define FBIOSET_BLENDING_AREA_DESTROY   0x4679
/*!获取blending区域 */
#define FBIOGET_BLENDING_AREA           0x4680

/*!主动检查当前的显示设备*/
#define FBIOSET_DEV_DETECT              0x4681
/*!获取所支持显示设备的最大的xres*yres,供视频使用*/
#define FBIOGET_DEV_MAX_RES             0x4682
/*!获取屏幕的方向*/
#define FBIOGET_SCREEN_ROTATE           0x4683

/*!获取当前的显示设备*/
#define FBIOGET_DISPLAYER               0x4684

#if 1
/*!刷新Fb，异步的，fb驱动不会延时*/
#define FBIOSET_UPDATE_FB_ASYNC         0x4685
/*!获取lcd的刷屏状态，是否DMA正在搬数据*/
#define FBIOGET_STATUS			        0x4686
#endif
#endif

