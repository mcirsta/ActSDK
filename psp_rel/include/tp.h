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
 *  �����¼����
 *  \li TOUCH_NOTPRESSED : �޴�������
 *  \li TOUCH_DOWN: �״δ���
 *  \li TOUCH_MOVE: �����ƶ�
 *  \li TOUCH_UP: ��������
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
 *  ��Ļ�ӽ�ģʽ
 *  \li UPRIGHT_1 : ����ģʽ1�����ӽ�ʱLCD�̱�ˮƽ����,����Ļ���Ͻǵ�Ĵ���������С
 *  \li SIDELONG_2: ����ģʽ2�����ӽ�ʱLCD����ˮƽ����,UPRIGHT_1ģʽ˳ʱ����ת90�Ⱥ�Ϊ��״̬
 *  \li UPRIGHT_2 : ����ģʽ2�����ӽ�ʱLCD�̱�ˮƽ����,SIDELONG_2ģʽ˳ʱ����ת90�Ⱥ�Ϊ��״̬
 *  \li 0��: ����ģʽ1�����ӽ�ʱLCD����ˮƽ����,UPRIGHT_2ģʽ˳ʱ����ת90�Ⱥ�Ϊ��״̬
 */

/*!
 *  \brief
 *  ��Ļ����ת����0�ȣ�90�ȣ�180�ȣ�270��
 */
/*��Ļ����ת����*/
//typedef enum
//{
//    /*! ��Ļ��ת0�ȣ������ӽ�*/
//    ROTATION_0 = 0,
//    /*! ��Ļ������ת90��*/
//    ROTATION_90 = 90,
//    /*! ��Ļ������ת180��*/
//    ROTATION_180 = 180,
//    /*! ��Ļ������ת270��*/
//    ROTATION_270 = 270
//}rotation_e;

/*!
 *  \brief
 *  ��������ģʽ
 *  \li TP_NORMAL :             ��������ģʽ
 *  \li HANDWRITTEN: ��д����ģʽ����ѯƵ�ʱȴ���ģʽ��
 *
 */
typedef enum
{
    TP_NORMAL,
    HANDWRITTEN
} work_mode_t;

/*!
 *  \brief
 *    �����¼���Ϣ�ṹ�嶨�壬�����¼�����������Ϣ \n
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
