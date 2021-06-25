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



/***************************** video 功能配置项 *******************************/
//1:使能播放video的门限电压保护功能; 0:禁止此功能
#define VIDEO_CFG_DETECT_VOL_THRESHOLD_ENABLE    0
//播放video的门限电压
#define VOL_THRESHOLD_PLAY_VIDEO    LI360
/* 检测电池电压时间间隔. 单位: s */
#define VIDEO_DETECT_BAT_VOL_TIME       (5 * (1000/VIDEO_PLAYING_TIMER_INTERVAL))

//1:按prev键从头播放当前曲目(同时还有如下的时间门槛的限制), 而不是播放上一个; 0:播放上一个
#define VIDEO_CFG_PLAYING_PREV_KEY_PLAY_CURRENT_AGAIN  1
//按prev键从头播放当前曲目的时间门槛. 超过此时间执行从头播放当前曲目操作. 
//同时也是记录到recent list的时间门槛, 超过此时间才进行记录. 单位: ms
#define VIDEO_CFG_PLAYING_PREV_KEY_PLAY_CURRENT_TIME    (10 * 1000)

//1:没有状态条时, 按键, 只显示状态条, 不执行按键动作, 只有有状态条时操作按键才有效; 
//0:按键一直有效, 与状态条的显示状态无关
#define VIDEO_CFG_PLAYING_FIRST_KEY_IGNORE  0

//1:暂停状态进度条不消失; 0:暂停状态进度条消失;
#define VIDEO_CFG_PLAYING_UI_SHOW_ALLTIME_WHEN_PAUSED  0

//1:刚进入播放界面时, playing ui一起显示出来; 0:刚进入播放界面时, playing ui不显示;
#define VIDEO_CFG_PLAYING_UI_SHOW_WHEN_START_PLAY  0

//1:当tag满时, 将新tag增加到最后, 同时删除第1个; 0:tag满时, 提示满, 不再添加
#define VIDEO_CFG_ADD_NEWTAG_REPLACE_FIRST_WHEN_TAG_FULL  1


//1:当tag空时, 先进入taglist, 提示空; 0:不进入taglist, 直接提时空
#define VIDEO_CFG_ENTER_WHEN_TAG_EMPTY  1
//1:当tag被删除为空后, 退出到taglist上一级; 0:停留在当前taglist空界面
//1:当tag空时, 进入taglist, 提示空, 退出; 0:不退出
#define VIDEO_CFG_RETURN_WHEN_TAG_EMPTY  0


//1:快进快退到文件头尾后, 继续快进快退下一个; 0:快进快退到文件头尾后, 停止在当前文件
#define VIDEO_CFG_FFB_END_THEN_START_NEXT  0




/*************************** video recent list配置项 **************************/
//1:使能记录最近播放文件功能, 记录个数为:RECENT_LIST_MAX_ITEM_NUM
//注:最近播放列表记录的是最近播放而没有播放完的文件
#define VIDEO_CFG_RECENT_LIST_ENABLE  1
#define RECENT_LIST_MAX_ITEM_NUM  (10)




/***************************** video taglist配置项 ****************************/
/*! video taglist存储tag最大个数, s_taglist_items[]数组根据此宏值, 动态初始化 */
#define VIDEO_TAGLIST_MAX_NUM   (32)




/***************************** video playing配置项 ****************************/
/* playing scene定时器周期, 用于查询player状态. 单位: ms */
#define VIDEO_PLAYING_TIMER_INTERVAL    (250)
/* playing scene 界面ui自动消失时间. 单位: s */
#define VIDEO_AUTO_HIDE_TIME       (3 * (1000/VIDEO_PLAYING_TIMER_INTERVAL))

/*手动切换文件时处理方式:
1 :切换到边界返回列表场景 
0:切换到边界继续(例如顺序时播放第一首时按prev播放最后一首)*/
#define VIDEO_MANUAL_SWITCH_METHOD 0



/* 时间转换成字符串后的长度大小, 用于playing界面显示的时间, taglist的时间信息等时间string长度 */
#define VIDEO_BUFFER_LENGTH_STR_TIME    (16)



//shaofeng
#define    MAX_FFB_STEP        (4)

/*********************************  swf playing 配置项*******************************/
/*swf播放出错时播放模式:1:播放下一个文件 0:退出进入list*/
#define VIDEO_SWF_PLAYMODE  1



/************************ extern function, 函数声明 ***************************/
extern bool video_cfg_read( void );
extern bool video_cfg_write( void );



#endif  /* __VIDEO_CONFIG_H__ */

