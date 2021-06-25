/********************************************************************************
 *                              USDK(1100)
 *                             Module: app 
 *                 Copyright(c) 2003-2009 Actions Semiconductor,
 *                            All Rights Reserved. 
 * History:         
 *      <author>    <time>           <version >             <desc>
 *      shaofeng    2009-07-02          v1.0               build this file 
********************************************************************************/
/*!
 * \file     video_config.h
 * \brief    
 * \author   shaofeng
 *               
 * \version 1.0
 * \date  2009/07/02
*******************************************************************************/

#ifndef __VIDEO_CONFIG_H__
#define __VIDEO_CONFIG_H__

#include "include_psp.h"
#include "include_case.h"



/***************************** video ���������� *******************************/
//1:ʹ�ܲ���video�����޵�ѹ��������; 0:��ֹ�˹���
#define VIDEO_CFG_DETECT_VOL_THRESHOLD_ENABLE    0
//����video�����޵�ѹ
#define VOL_THRESHOLD_PLAY_VIDEO    LI360
/* ����ص�ѹʱ����. ��λ: s */
#define VIDEO_DETECT_BAT_VOL_TIME       (5 * (1000/VIDEO_PLAYING_TIMER_INTERVAL))

//1:��prev����ͷ���ŵ�ǰ��Ŀ(ͬʱ�������µ�ʱ���ż�������), �����ǲ�����һ��; 0:������һ��
#define VIDEO_CFG_PLAYING_PREV_KEY_PLAY_CURRENT_AGAIN  1
//��prev����ͷ���ŵ�ǰ��Ŀ��ʱ���ż�. ������ʱ��ִ�д�ͷ���ŵ�ǰ��Ŀ����. 
//ͬʱҲ�Ǽ�¼��recent list��ʱ���ż�, ������ʱ��Ž��м�¼. ��λ: ms
#define VIDEO_CFG_PLAYING_PREV_KEY_PLAY_CURRENT_TIME    (10 * 1000)

//1:û��״̬��ʱ, ����, ֻ��ʾ״̬��, ��ִ�а�������, ֻ����״̬��ʱ������������Ч; 
//0:����һֱ��Ч, ��״̬������ʾ״̬�޹�
#define VIDEO_CFG_PLAYING_FIRST_KEY_IGNORE  0

//1:��ͣ״̬����������ʧ; 0:��ͣ״̬��������ʧ;
#define VIDEO_CFG_PLAYING_UI_SHOW_ALLTIME_WHEN_PAUSED  0

//1:�ս��벥�Ž���ʱ, playing uiһ����ʾ����; 0:�ս��벥�Ž���ʱ, playing ui����ʾ;
#define VIDEO_CFG_PLAYING_UI_SHOW_WHEN_START_PLAY  0

//1:��tag��ʱ, ����tag���ӵ����, ͬʱɾ����1��; 0:tag��ʱ, ��ʾ��, �������
#define VIDEO_CFG_ADD_NEWTAG_REPLACE_FIRST_WHEN_TAG_FULL  1


//1:��tag��ʱ, �Ƚ���taglist, ��ʾ��; 0:������taglist, ֱ����ʱ��
#define VIDEO_CFG_ENTER_WHEN_TAG_EMPTY  1
//1:��tag��ɾ��Ϊ�պ�, �˳���taglist��һ��; 0:ͣ���ڵ�ǰtaglist�ս���
//1:��tag��ʱ, ����taglist, ��ʾ��, �˳�; 0:���˳�
#define VIDEO_CFG_RETURN_WHEN_TAG_EMPTY  0


//1:������˵��ļ�ͷβ��, �������������һ��; 0:������˵��ļ�ͷβ��, ֹͣ�ڵ�ǰ�ļ�
#define VIDEO_CFG_FFB_END_THEN_START_NEXT  0




/*************************** video recent list������ **************************/
//1:ʹ�ܼ�¼��������ļ�����, ��¼����Ϊ:RECENT_LIST_MAX_ITEM_NUM
//ע:��������б��¼����������Ŷ�û�в�������ļ�
#define VIDEO_CFG_RECENT_LIST_ENABLE  1
#define RECENT_LIST_MAX_ITEM_NUM  (10)




/***************************** video taglist������ ****************************/
/*! video taglist�洢tag������, s_taglist_items[]������ݴ˺�ֵ, ��̬��ʼ�� */
#define VIDEO_TAGLIST_MAX_NUM   (32)




/***************************** video playing������ ****************************/
/* playing scene��ʱ������, ���ڲ�ѯplayer״̬. ��λ: ms */
#define VIDEO_PLAYING_TIMER_INTERVAL    (250)
/* playing scene ����ui�Զ���ʧʱ��. ��λ: s */
#define VIDEO_AUTO_HIDE_TIME       (3 * (1000/VIDEO_PLAYING_TIMER_INTERVAL))

/*�ֶ��л��ļ�ʱ����ʽ:
1 :�л����߽緵���б��� 
0:�л����߽����(����˳��ʱ���ŵ�һ��ʱ��prev�������һ��)*/
#define VIDEO_MANUAL_SWITCH_METHOD 0



/* ʱ��ת�����ַ�����ĳ��ȴ�С, ����playing������ʾ��ʱ��, taglist��ʱ����Ϣ��ʱ��string���� */
#define VIDEO_BUFFER_LENGTH_STR_TIME    (16)



//shaofeng
#define    MAX_FFB_STEP        (4)

/*********************************  swf playing ������*******************************/
/*swf���ų���ʱ����ģʽ:1:������һ���ļ� 0:�˳�����list*/
#define VIDEO_SWF_PLAYMODE  1



/************************ extern function, �������� ***************************/
extern bool video_cfg_read( void );
extern bool video_cfg_write( void );



#endif  /* __VIDEO_CONFIG_H__ */

