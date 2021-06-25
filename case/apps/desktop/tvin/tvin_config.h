/********************************************************************************
 *                              USDK(1100)
 *                             Module: app
 *                 Copyright(c) 2003-2009 Actions Semiconductor,
 *                            All Rights Reserved.
 * File : tvin_config.h
 * History:
 *      <author>    <time>           <version >             <desc>
 *      Jerry Fu    2009-4-29          v1.0               build this file
 ********************************************************************************/
/*!
* \file     tvin_config.h
* \brief    tvin  config write and read
* \author   Jerry Fu
*
* \version 1.0
* \date  2009/04/29
*******************************************************************************/
#ifndef   __TVIN_CONFIG_H__
#define   __TVIN_CONFIG_H__

#include "include_case.h"
#include "include_psp.h"

#include "tvin_sty.h"
#define  TEST_MMMM_DV   (1)
#define  TVIN_RECORD_SHOW_HEADBAR     (1)

/* 单次录制的时间段的个数 */
#define  SIGLE_REC_TIME_SEGMENT_NUM       (3)

/* 重复录制的时间段个数 */
#define  REPEAT_REC_TIME_SEGMENT_NUM      (3)
/* 一周内包含的天数 */
#define  DAYS_IN_ONEWEEK                  (7)

#define   CFG_TEXT_LEN                 (32)

#define   MAX_ALARM_NUMBER             (6)


typedef enum
{
    /* config value type is string*/
    CFG_VALUE_TYPE_STRING,   

    /* config value type is integer*/
    CFG_VALUE_TYPE_INTEGER,   
}cfg_value_type_e;



typedef struct
{
    /* config key name */
    char key[32];

    /* value type */
    cfg_value_type_e value_type;

    /* to store string type value */
    char text[CFG_TEXT_LEN];

    /* to store integer type value */
    int value;

    /* need to write back to config file ?*/
    bool changed;
}tvin_cfg_t;


/*!
 *  \par config配置项类型
 */
typedef enum
{
    TVIN_CFG_TVIN_SIGNAL,
    TVIN_CFG_SAVEPATH,
    TVIN_CFG_RECORD_QUALITY,
    TVIN_CFG_LCD_ONOFF,
    
    TVIN_CFG_SINGLE1_DATE,
    TVIN_CFG_SINGLE2_DATE,
    TVIN_CFG_SINGLE3_DATE,
    TVIN_CFG_SINGLE1_TIME,
    TVIN_CFG_SINGLE2_TIME,
    TVIN_CFG_SINGLE3_TIME,
    TVIN_CFG_SINGLE1_LENGTH,
    TVIN_CFG_SINGLE2_LENGTH,
    TVIN_CFG_SINGLE3_LENGTH,

    TVIN_CFG_SINGLE1_ONOFF,
    TVIN_CFG_SINGLE2_ONOFF,
    TVIN_CFG_SINGLE3_ONOFF,

    TVIN_CFG_REPEAT1_TIME,
    TVIN_CFG_REPEAT2_TIME,
    TVIN_CFG_REPEAT3_TIME,
    TVIN_CFG_REPEAT1_LENGTH,
    TVIN_CFG_REPEAT2_LENGTH,
    TVIN_CFG_REPEAT3_LENGTH,

    TVIN_CFG_REPEAT1_WEEKDAY,
    TVIN_CFG_REPEAT2_WEEKDAY,
    TVIN_CFG_REPEAT3_WEEKDAY,

    TVIN_CFG_REPEAT1_ONOFF,
    TVIN_CFG_REPEAT2_ONOFF,
    TVIN_CFG_REPEAT3_ONOFF,

    TVIN_CFG_SINGLE1_CLKNUM,
    TVIN_CFG_SINGLE2_CLKNUM,
    TVIN_CFG_SINGLE3_CLKNUM,
    TVIN_CFG_REPEAT1_CLKNUM,
    TVIN_CFG_REPEAT2_CLKNUM,
    TVIN_CFG_REPEAT3_CLKNUM,

    TVIN_CFG_INVALID,
    
}tvin_cfg_type_e;



/* exit type */
typedef enum
{
    SCENE_TVIN_PALNTSC,
    SCENE_TVIN_RECORD,
    SCENE_TVIN_EXIT,
}scene_tvin_ret_e;


typedef  enum
{
    REP_SUNDAY,
    REP_MONDAY,
    REP_TUESDAY,
    REP_WEDNESDAY,
    REP_THRUSDAY,
    REP_FRIDAY,
    REP_SATURDAY,
}repeat_week_e;


/*!
 * \par  录像的保存路径
 *  \li INTERNAL_MEMORY : 内部存取器
 *  \li CARD_MEMORY ：保存在SD卡上
 *  \li EXTERNAL_MEMORY : 外部存储器
 */
typedef  enum
{
    INTERNAL_MEMORY,
    CARD_MEMORY,
    EXTERNAL_MEMORY,
    INVALID_MEMORY,
}tvin_save_path_e;


/*!
 * \par  录像质量选择
 *  \li RECORD_QUALITY_LOW : 低码率(500K)
 *  \li RECORD_QUALITY_MEDIUM ：中码率(800k)
 *  \li RECORD_QUALITY_HIGH : 高码率(1000k)
 *  \li RECORD_QUALITY_AUTO : 自适应码率
 */
typedef  enum
{
    RECORD_QUALITY_LOW,
    RECORD_QUALITY_MEDIUM,
    RECORD_QUALITY_HIGH,
    RECORD_QUALITY_AUTO,
    RECORD_QUALITY_INVALID,
}tvin_record_quality_e;



/*!
 * \par  单次录像的的时间段
 *  \li start_date :  开始录制的起始日期
 *  \li start_time ：    开始录制的起始时间
 *  \li record_time : 需要录制的时间
 *  \li enable : 是否允许该时段进行录制
 */
typedef  struct
{
    rtc_date_t start_date;
    rtc_time_t start_time;
    rtc_time_t record_time;   
    bool onoff;       	
}single_record_t;

/*!
 * \par  重复录像的的时间段设置
 *  \li start_time :  开始录制的起始时间
 *  \li record_time : 需要录制的时间,以分为单位
 *  \li repeat_day_flag : 该字节中的前7位记录了周日到周六是否允许录制 1：允许 0：不允许
 *  \li enable : 是否允许该时段进行repeat录制
 */
typedef  struct
{
    rtc_time_t  start_time;
    rtc_time_t  record_time;
    unsigned char  repeat_day_flag;
    bool  onoff;   	
}repeat_record_t;


/*!
 * \par  TVIN应用中使用的数据
 *  \li tv_signal : 输入图像格式
 *  \li single_record :  单次录制的时间段设置，共三个
 *  \li repeat_record : 重复录制的时间段设置，共三个
 *  \li save_path : 录像文件的存放路径设置
 *  \li lcd_on_off : 录像的时候是否关掉lcd 1-录像的时候不要关掉lcd；0-录像的时候关掉lcd。
 *  \li record_quality: 录像质量
 *  \li alarm_clknum: 定时录像对应的定时器号
 */
typedef  struct
{
    atv_zone_t  tv_signal;
    single_record_t   single_record[SIGLE_REC_TIME_SEGMENT_NUM];
    repeat_record_t   repeat_record[REPEAT_REC_TIME_SEGMENT_NUM];	
    tvin_save_path_e  save_path;
    adj_switch_mode_e  lcd_on_off;
    tvin_record_quality_e  record_quality;
    int  alarm_clknum[MAX_ALARM_NUMBER];
}tvin_config_t;


extern tvin_cfg_t g_tvin_cfgs[];

extern bool tvin_cfg_read(tvin_cfg_t * cfg);
extern bool tvin_cfg_read_all(tvin_cfg_t cfgs[], int count);

extern bool tvin_cfg_write(tvin_cfg_t * cfg);
extern bool tvin_cfg_write_all(tvin_cfg_t cfgs[], int count);

#endif


