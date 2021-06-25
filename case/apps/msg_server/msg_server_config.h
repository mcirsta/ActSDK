/********************************************************************************
 *                              USDK(1100)
 *                             Module: app 
 *                 Copyright(c) 2003-2008 Actions Semiconductor,
 *                            All Rights Reserved. 
 * file : msg_server_config.h
 * History:         
 *      <author>    <time>           <version >             <desc>
 *      Herb Hu    2009-07-08             v1.0             build this file 
 ********************************************************************************/

#ifndef __MSG_SERVER_CONFIG_H__
#define __MSG_SERVER_CONFIG_H__

#include <include_case.h>
#include <include_psp.h>



/* added by shaofeng 2009-07-11 */
/* 是否允许低电检测, 用于stub调试时, 避免一直提示低电 */
#define CFG_ENABLE_LOW_VOLTAGE_DETECT   1

/* added by shaofeng 2009-03-24 */
/* time of detect key driver's buffer. unit:ms */
#define TIME_DETECT_KEY         60
#define CNT_DEBOUNCE_KEYUP      (2)
#define CNT_DEBOUNCE_KEYVAL     (3)

/* added by shaofeng 2009-03-24 */
/* time of detect key lock status. unit:ms */
#define TIME_DETECT_KEYLOCK     500

/* added by shaofeng 2009-03-24 */
/* time of detect system status, such as: battery voltage. unit:ms 
* mustn't be 0
*/
#define TIME_DETECT_CHARGE     500

/* added by shaofeng 2009-03-24 */
/* time of detect system event such as hotplugin  events, etc. USB plugin, SD card plugin. unit:ms */ 
#define TIME_DETECT_SYS_EVT  100

/* added by shaofeng 2009-03-24 */
/* time interval of calculate average battery voltage. unit:ms
* but time of detect voltage is: TIME_DETECT_CHARGE.
* must more than TIME_DETECT_CHARGE
*/
#define TIME_DETECT_LOW_VOLTAGE 5000
#define CNT_DETECT_LOW_VOLTAGE (TIME_DETECT_LOW_VOLTAGE/TIME_DETECT_CHARGE)

/* time interval of send low_voltage msg. unit:ms
*/
#define TIME_BROADCAST_MSG_LOW_VOLTAGE 20000
#define CNT_BROADCAST_MSG_LOW_VOLTAGE (TIME_BROADCAST_MSG_LOW_VOLTAGE/TIME_DETECT_CHARGE)

/* added by shaofeng 2009-03-24 */
/* count of low voltage to poweroff. unit:times */
#define CNT_LOW_VOLTAGE_TO_POWER_OFF     3


/* added by shaofeng 2009-03-24 */
/* time of send key long status, when key is down. unit:30ms */
#define TIME_KEY_LONG   (12*3)

/* added by shaofeng 2009-03-24 */
/* time of send key hold status interval, when key is long. unit:30ms */
#define TIME_KEY_HOLD   (2)

/* added by shaofeng 2009-07-11 */
/* 1:当按键hold时有按键音; 0:只有down时有按键音 */
#define CFG_KEY_TONE_PLAY_WHEN_HOLD   0

/* 当按键hold时没有按键音的键值 */
#define CFG_POWER_KEYVAL_WHEN_HOLD_NOT_PLAY_KEY_TONE   KEY_OK

/* added by shaofeng 2009-03-24 */
/* hardware volume's max value */
#define MAX_HARDWARE_VOLUME     31


/* added by herb hu 2009-05-25 */
/* time of detect gsensor driver's buffer. unit:ms */
#define TIME_DETECT_GSENSOR        100 

/* 按键按下, 上升到的频率 */
#define RISE_CLK_DOMAIN_WHEN_KEY_DOWN         CLK_LOAD_HIGH_0
/* 按键按下上升频率后, 持续的时间, unit:ms */
#define TIME_CONTINUE_RISE_CLK_FROM_KEY_DOWN         (2000)

#define CFG_TEXT_LEN  512 





typedef enum
{
    /* config value type is string*/
    CFG_VAL_STRING,   

    /* config value type is integer*/
    CFG_VAL_INTEGER,   
}cfg_value_type_e;

typedef struct
{
    /* config key name */
    char key[32];

    /* value type */
    cfg_value_type_e value_type;

    /* to store string type value */
    char * text;

    /* to store integer type value */
    int value;

    /* need to write back to config file ?*/
    bool changed;
}cfg_t;

typedef enum
{
    CFG_TYPE_SCR_MODE = 0,   
    CFG_TYPE_BRIGHTNESS,   
    CFG_TYPE_BACKLIGHT,   
    CFG_TYPE_IDLE_TIME,   
    CFG_TYPE_SLEEP_TIME,   
    CFG_TYPE_KEY_TONE_ON,   
    CFG_TYPE_KEY_TONE_FULLPATH,   
    CFG_TYPE_GSENSOR_SHAKE_SUPPORT,
    CFG_TYPE_GSENSOR_SHAKE_THRESHOLD,
    CFG_TYPE_UHOST_SUPPORT,   
    CFG_TYPE_CARD_SUPPORT,   
    CFG_TYPE_MAX,   
}cfg_type_e;


extern cfg_t g_msg_server_cfgs[];

extern bool msg_server_cfg_init(cfg_t cfgs[], int count);
extern bool msg_server_cfg_deinit(cfg_t cfgs[], int count);

extern bool msg_server_cfg_read(cfg_t * cfg);
extern bool msg_server_cfg_read_all(cfg_t cfgs[], int count);

extern bool msg_server_cfg_write(cfg_t * cfg);
extern bool msg_server_cfg_write_all(cfg_t cfgs[], int count);


#endif /* #ifndef __MSG_SERVER_CONFIG_H__ */
