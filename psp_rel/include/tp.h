/*
 ********************************************************************************
 *                       usdk130
 *                    Module: TP Driver
 *                Copyright(c) 2003-2008 Actions Semiconductor,
 *                        All Right Reserved
 *
 * History:
 *      <author>             <time>           <version >             <desc>
 *        ycli,shaof       2007/07/20            1.0               build this file
 *         cccw                2008/09/28          2.0                1100 revised
 ********************************************************************************
 */

/*!
 * \file tp_drv.h
 * \brief general touchpanel Driver information
 * \author cccw
 * \par GENERAL DESCRIPTION:
 *       This file provide the basic type and macro. \n
 *
 * \par DECLARATIONS:
 *       (c) Copyright 2003-2008, Actions Co,Ld. All Right Reserved.
 *
 * \version 1.0
 * \date 2008/09/27
 */

#ifndef __TP_H__
#define __TP_H__
/*! \cond TP_API*/
#include "display.h"

/******************* TP related ioctl command request. ***********************/
#define TPIO_CMD_BASE          0x40056400

#define TPIO_SET_ROTATE                 (TPIO_CMD_BASE + 2)
//#define TPIO_SET_WORKMODE         (TPIO_CMD_BASE + 3)

#define TPIO_SET_REF_POINT           (TPIO_CMD_BASE + 4)
#define TPIO_GET_COORDINATE      (TPIO_CMD_BASE + 5)
#define TPIO_SET_LOCK                      (TPIO_CMD_BASE + 6)
#define TPIO_SET_UNLOCK                (TPIO_CMD_BASE + 7)
#define TPIO_SET_ADJUST    (TPIO_CMD_BASE + 8)
#define TPIO_SET_SCAN_INTERVAL         (TPIO_CMD_BASE + 9)
/*!
 *  \brief
 *  触摸事件类别
 *  \li TOUCH_NOTPRESSED : 无触摸动作
 *  \li TOUCH_DOWN: 首次触摸
 *  \li TOUCH_MOVE: 触摸移动
 *  \li TOUCH_UP: 触摸撤消
 *
 */
typedef enum
{
    TOUCH_NOTPRESSED,
    TOUCH_DOWN,
    TOUCH_UP,
} touch_state_t;

/*!
 *  \brief
 *  屏幕视角模式
 *  \li UPRIGHT_1 : 竖屏模式1，正视角时LCD短边水平放置,且屏幕左上角点的触摸坐标最小
 *  \li SIDELONG_2: 横屏模式2，正视角时LCD长边水平放置,UPRIGHT_1模式顺时针旋转90度后即为该状态
 *  \li UPRIGHT_2 : 竖屏模式2，正视角时LCD短边水平放置,SIDELONG_2模式顺时针旋转90度后即为该状态
 *  \li 0度: 横屏模式1，正视角时LCD长边水平放置,UPRIGHT_2模式顺时针旋转90度后即为该状态
 */

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
 *  驱动工作模式
 *  \li TP_NORMAL :             触摸工作模式
 *  \li HANDWRITTEN: 手写工作模式，查询频率比触摸模式高
 *
 */
typedef enum
{
    TP_NORMAL,
    HANDWRITTEN
} work_mode_t;

/*!
 *  \brief
 *    触摸事件消息结构体定义，包括事件类别、坐标等信息 \n
 *
 */
typedef struct
{
    /*! horizontal coordinate  */
    int x;
    /*! vertical coordinate  */
    int y;
    /*! msg type  */
    touch_state_t state;
} tp_msg_t;

typedef struct
{
    unsigned int x_leftup;
    unsigned int y_leftup;
    unsigned int x_abs_leftup;
    unsigned int y_abs_leftup;

    unsigned int x_rightdown;
    unsigned int y_rightdown;
    unsigned int x_abs_rightdown;
    unsigned int y_abs_rightdown;
} adjust_point_t;

typedef struct
{
    int x_abs_val;
    int y_abs_val;
} get_abs_t;
/*! \endcond*/
#endif  //#ifndef _TOUCH_DRIVER_H_
