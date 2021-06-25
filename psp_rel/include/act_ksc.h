/*
 ********************************************************************************
 *                       linux213x
 *         actions decoder interface structure
 *                (c) Copyright 2002-2003, Actions Co,Ld.
 *                        All Right Reserved
 *
 * File   : act_decoder.h
 * by	 : cs/fw
 * Version: 1> v1.00     first version     02.24.2006
 ********************************************************************************
 */
#ifndef __ACT_KSC_H__
#define __ACT_KSC_H__

#ifdef __cplusplus
extern "C"
{
#endif

/*!
 * \brief
 *      定义了与显示区域信息有关的数据结构
 */
typedef struct
{
    /*! 左上角横坐标 */
    int tx_pos;
    /*! 左上角纵坐标 */
    int ty_pos;
    /*! 宽 */
    int width;
    /*! 高 */
    int height;
} ksc_display_area_t; //显示区域定义

/*!
 * \brief
 *      定义了与字体信息有关的数据结构
 */
typedef struct
{
    /*! 原来的字体颜色 */
    int OriginalFontColor;
    /*! 渐进的字体颜色 */
    int ProgressFontColor;
    /*! 字体类型：如黑体、宋体 */
    int FontType;
    /*! 字体大小 */
    int FontSize;
} ksc_font_property_t;

/*!
 * \brief
 *      定义了与ksc 初始化信息有关的数据结构
 */
typedef struct
{
    /*! 文件名为全路径名，在库实现时需要判断当前的ksc文件名与前一个使用的是否相同，若相同则不作任何操作，否则完成相关的操作； */
    char *FileName;
    /*! framebuffer的起始地址 */
    char *FbStartAddr;
    /*! 字幕显示 */
    ksc_font_property_t CurFont;
    /*! 字幕的显示区域 */
    ksc_display_area_t DisplayArea;
} ksc_init_t;

/*! 释放主函数 */
int Libkscdispose();

/*! 初始化函数 */
int LibkscInit(ksc_init_t *param);

/*!
 * \brief
 *      定义了与ksc 刷屏主函数
 * 变量说明：
 *        NowTime:  当前时间
 *        framebuf: 当前刷屏地址
 *        format: 横竖屏，竖屏为1，横屏为0
 *        cmd:    0,yuv显示单行，1，yuv数据显示双行，2，rgb显示单行，3，rgb显示双行
 *        DisplayArea: 横竖屏切换时，位置和宽高重设
 */
int LibkscLrcDisplay(int NowTime, char *framebuf, int format, int cmd,
        ksc_display_area_t *DisplayArea);

#ifdef __cplusplus
}
#endif

#endif
