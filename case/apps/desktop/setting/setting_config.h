/********************************************************************************
 *                              USDK(1100)
 *                             Module: app 
 *                 Copyright(c) 2003-2008 Actions Semiconductor,
 *                            All Rights Reserved. 
 * file : setting_config.h
 * History:         
 *      <author>    <time>           <version >             <desc>
 *      Herb Hu    2009-5-9             v1.0             build this file 
 ********************************************************************************/

#ifndef __SETTING_CONFIG_H__
#define __SETTING_CONFIG_H__

#include <include_case.h>


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
    char key[64];

    /* value type */
    cfg_value_type_e value_type;

    /* to store string type value */
    char *text;

    /* to store integer type value */
    int value;

    /* need to write back to config file ?*/
    bool changed;
}cfg_t;

typedef enum
{
    SET_CFG_TYPE_THEME = 0,   
    SET_CFG_TYPE_SCR_MODE,   
    SET_CFG_TYPE_BRIGHTNESS,   
    SET_CFG_TYPE_BACKLIGHT,   
    SET_CFG_TYPE_IDLE_TIME,   
    SET_CFG_TYPE_SLEEP_TIME,   
    SET_CFG_TYPE_LANG_CODES,   
    SET_CFG_TYPE_TVOUT,   
    SET_CFG_TYPE_CONNECT,   
    SET_CFG_TYPE_DEFAULT_FONT,   
    SET_CFG_TYPE_USER_FONT_VALID,   
    SET_CFG_TYPE_USER_FONT,   
    SET_CFG_TYPE_KEY_TONE_ON,   
    SET_CFG_TYPE_KEY_TONE_FULLPATH,   
    SET_CFG_TYPE_KEY_TONES,   
    SET_CFG_TYPE_KEY_TONE_CUR,   
    SET_CFG_TYPE_USERNAME,   
    SET_CFG_TYPE_VERSION,   
    SET_CFG_TYPE_SEARCH_FOLDER,   
    SET_CFG_TYPE_GSENSOR_SHAKE_SUPPORT,   
    SET_CFG_TYPE_MAX,   
}cfg_type_e;

typedef enum
{
    CFG_MENU_MAIN_THEME = 0,    
    CFG_MENU_MAIN_SCR_MODE, 
    CFG_MENU_MAIN_DISPLAY,  
    CFG_MENU_MAIN_POWER,    
    CFG_MENU_MAIN_DATE_TIME,
    CFG_MENU_MAIN_LANGUAGE, 
    CFG_MENU_MAIN_TVOUT,    
    CFG_MENU_MAIN_SYNC_PLIST,
    CFG_MENU_MAIN_ADVANCED,
    CFG_MENU_MAIN_MAX,      
}cfg_menu_main_e;

typedef enum
{
    CFG_MENU_SCR_MODE_HORIZONTAL = 0,  
    CFG_MENU_SCR_MODE_VERITCAL, 
    CFG_MENU_SCR_MODE_AUTO,      
    CFG_MENU_SCR_MODE_MAX,      
}cfg_menu_scr_mode_e;

typedef enum
{
    CFG_MENU_DISPLAY_BRIGHTNESS = 0,  
    CFG_MENU_DISPLAY_BACKLIGHT,  
    CFG_MENU_DISPLAY_MAX,  
}cfg_menu_display_e;

typedef enum
{
    CFG_MENU_POWER_IDLE_TIME = 0, 
    CFG_MENU_POWER_SLEEP_TIME,    
    CFG_MENU_POWER_MAX,    
}cfg_menu_power_e;

typedef enum
{
    CFG_MENU_DATE_TIME_ADJUST_DATE = 0,
    CFG_MENU_DATE_TIME_ADJUST_TIME,
    CFG_MENU_DATE_TIME_DRM_TIME,   
    CFG_MENU_DATE_TIME_MAX,   
}cfg_menu_date_time_e;

typedef enum
{
    CFG_MENU_ADVANCED_INFO = 0,         
    CFG_MENU_ADVANCED_CONNECT_TYPE,  
    CFG_MENU_ADVANCED_FONT,          
    CFG_MENU_ADVANCED_KEY_TONE,      
    CFG_MENU_ADVANCED_SHOW_EFFECT,   
    CFG_MENU_ADVANCED_SEARCH_FOLDER, 
    CFG_MENU_ADVANCED_GSENSOR_SHAKE_SUPPORT, 
    CFG_MENU_ADVANCED_FORMAT_INTERNAL,
    CFG_MENU_ADVANCED_FORMAT_CARD,   
    CFG_MENU_ADVANCED_FORMAT_EXTERNAL,
    CFG_MENU_ADVANCED_SET_USERNAME,  
    CFG_MENU_ADVANCED_CLEAR_DRM,     
    CFG_MENU_ADVANCED_CHECK_SYS,     
    CFG_MENU_ADVANCED_RESET,         
    CFG_MENU_ADVANCED_MAX,         
}cfg_menu_advanced_e;

typedef enum
{
    CFG_MENU_INFO_USERNAME = 0, 
    CFG_MENU_INFO_VERSION,  
    CFG_MENU_INFO_INTERNAL_SPACE,
    CFG_MENU_INFO_INTERNAL_FREE,
    CFG_MENU_INFO_CARD_SPACE,
    CFG_MENU_INFO_CARD_FREE,
    CFG_MENU_INFO_EXTERNAL_SPACE,
    CFG_MENU_INFO_EXTERNAL_FREE,
    CFG_MENU_INFO_MAX,
}cfg_menu_info_e;

extern cfg_t g_setting_cfgs[];
extern cfg_t g_menu_main_cfgs[];
extern cfg_t g_menu_scr_mode_cfgs[]; 
extern cfg_t g_menu_display_cfgs[];
extern cfg_t g_menu_power_cfgs[];
extern cfg_t g_menu_date_time_cfgs[];
extern cfg_t g_menu_advanced_cfgs[];
extern cfg_t g_menu_info_cfgs[];

extern bool setting_cfg_init(cfg_t cfgs[], int count);
extern bool setting_cfg_deinit(cfg_t cfgs[], int count);

extern bool setting_cfg_read(cfg_t * cfg);
extern bool setting_cfg_read_all(cfg_t cfgs[], int count);

extern bool setting_cfg_write(cfg_t * cfg);
extern bool setting_cfg_write_all(cfg_t cfgs[], int count);


#endif /* #ifndef __SETTING_CONFIG_H__ */
