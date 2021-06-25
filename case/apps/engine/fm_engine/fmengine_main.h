/********************************************************************************
 *                              USDK(1100)
 *                             Module: app
 *                 Copyright(c) 2003-2009 Actions Semiconductor,
 *                            All Rights Reserved.
 * File : fmengine_main.h
 * History:
 *      <author>    <time>           <version >             <desc>
 *      binzhang    2009-09-03          v1.0               build this file
 ********************************************************************************/
/*!
 * \file     fmengine_main.h
 * \brief    header of fm radio engine
 * \author   binzhang
 *
 * \version 1.0
 * \date  2009/09/03
 *******************************************************************************/

#ifndef __FMENGINE_MAIN_H__
#define __FMENGINE_MAIN_H__
#include "include_case.h"
/*!
 * \brief  
 *      max channel number saved in engine
 */
#define MAX_CHANNEL_NUM 20

/*!
 * \brief  
 *      ������һ����̨�󲥷ŵ�ʱ�䣬��λms
 */
#define INTER_SEEK_PLAY 1000

/*! ģ��֧���Զ���̨ */
#define REAL_AUTO_SEEK

/*! result of engine auto seek */
typedef struct
{
    /*!  channel number*/
    unsigned int channel_num;
    /*!  channel frequency*/
    unsigned int channel_freq[MAX_CHANNEL_NUM];
} eg_seek_info_t;

/*!
 * \brief  
 *      �Զ���̨ģʽ
 */
typedef enum
{
    NORMAL_PLAY = 0, /* �������� */
    HALF_AUTO_SEARCH_ADD, /* ǰ����Զ�����(����̨��ͣ) */
    HALF_AUTO_SEARCH_MINUS, /* ������Զ�����(����̨��ͣ) */
    AUTO_SEARCH,
/* ȫ�Զ�������������ǰ�������е�̨����ౣ��20����*/

} search_state_e;


typedef enum
{
    FM_STOPPED = 0,
    FM_PLAYING, 
    FM_ERROR,
}engine_state_e;

/*!
 * \brief  
 *      �Զ���̨ʱӦ�ô���Ĳ����ṹ
 */
typedef struct
{
    unsigned int min_freq;
    unsigned int max_freq;
    unsigned int step;
    search_state_e mode;
} en_seek_info_t;/*engine neeed info for seeking���ýṹ��Ϊradio��������ṹ*/


/*!
 * \brief  
 *      use to control auot seek
 */
typedef struct
{
    unsigned int min_freq;
    unsigned int max_freq;
    unsigned int step;
    search_state_e mode;/*half inc 0 ,half dec 1, auto 2 */
    int timer_id_seek;/*to save auto seek timer id*/
    int timer_id_play;/*timer id to play */
    bool seek_over_flg;
    bool need_seek;/*�����ڶ�ʱ����ָʾ�Ƿ����Զ���̨: false ����ѯ��Ϣ��true ���Զ���̨�ٲ�ѯ��*/

    unsigned int cur_freq;
} engine_seek_t;
#endif
