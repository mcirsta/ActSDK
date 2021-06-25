/********************************************************************************
 *                              USDK(1100)
 *                             Module: app 
 *                 Copyright(c) 2003-2008 Actions Semiconductor,
 *                            All Rights Reserved. 
 * file : setting_config.c
 * History:         
 *      <author>    <time>           <version >             <desc>
 *      Herb Hu    2009-5-9             v1.0             build this file 
 ********************************************************************************/

#include "setting_config.h"

cfg_t g_setting_cfgs[] =
{
    { CFG_CUR_THEME,             CFG_VAL_STRING,  NULL,   0,  false }, /*SET_CFG_TYPE_THEME*/
    { CFG_SCREEN_MODE,           CFG_VAL_STRING,  NULL,   0,  false }, /*SET_CFG_TYPE_SCR_MODE*/
    { CFG_BACKLIGHT_BRIGHTNESS,  CFG_VAL_INTEGER, NULL,   3,  false }, /*SET_CFG_TYPE_BRIGHTNESS*/
    { CFG_BACKLIGHT_DURATION,    CFG_VAL_INTEGER, NULL,   0,  false }, /*SET_CFG_TYPE_BACKLIGHT*/
    { CFG_IDLE_TIME,             CFG_VAL_INTEGER, NULL,   0,  false }, /*SET_CFG_TYPE_IDLE_TIME*/
    { CFG_SLEEP_TIME,            CFG_VAL_INTEGER, NULL,   0,  false }, /*SET_CFG_TYPE_SLEEP_TIME*/
    { CFG_LANG_CODES,            CFG_VAL_STRING,  NULL,   0,  false }, /*SET_CFG_TYPE_LANG_CODES*/
    { CFG_TVOUT,                 CFG_VAL_STRING,  NULL,   0,  false }, /*SET_CFG_TYPE_TVOUT*/
    { CFG_CONNECT_TYPE,          CFG_VAL_STRING,  NULL,   0,  false }, /*SET_CFG_TYPE_CONNECT*/
    { CFG_DEFAULT_FONT,          CFG_VAL_STRING,  NULL,   0,  false }, /*SET_CFG_TYPE_DEFAULT_FONT*/
    { CFG_USER_FONT_VALID,       CFG_VAL_INTEGER, NULL,   0,  false }, /*SET_CFG_TYPE_USER_FONT_VALID*/
    { CFG_USER_FONT,             CFG_VAL_STRING,  NULL,   0,  false }, /*SET_CFG_TYPE_USER_FONT*/
    { CFG_KEY_TONE_ON,           CFG_VAL_INTEGER, NULL,   0,  false }, /*SET_CFG_TYPE_KEY_TONE_ON*/
    { CFG_KEY_TONE_FULLPATH,     CFG_VAL_STRING,  NULL,   0,  false }, /*SET_CFG_TYPE_KEY_TONE_FULLPATH*/
    { CFG_KEY_TONES,             CFG_VAL_STRING,  NULL,   0,  false }, /*SET_CFG_TYPE_KEY_TONES*/
    { CFG_KEY_TONE_CUR,          CFG_VAL_STRING,  NULL,   0,  false }, /*SET_CFG_TYPE_KEY_TONE_CUR*/
    { CFG_USERNAME,              CFG_VAL_STRING,  NULL,   0,  false }, /*SET_CFG_TYPE_USERNAME*/
    { CFG_VER,                   CFG_VAL_STRING,  NULL,   0,  false }, /*SET_CFG_TYPE_VERSION*/
    { CFG_SEARCH_FOLDER,         CFG_VAL_STRING,  NULL,   0,  false }, /*SET_CFG_TYPE_SEARCH_FOLDER*/
    { CFG_GSENSOR_SHAKE_SUPPORT,       CFG_VAL_STRING,  NULL,   0,  false }, /*SET_CFG_TYPE_GSENSOR_SHAKE_SUPPORT*/
};

cfg_t g_menu_main_cfgs[] =
{
    { CFG_MENU_THEME_VISIBLE,      CFG_VAL_INTEGER, NULL, 1, false },  /*CFG_MENU_MAIN_THEME*/
    { CFG_MENU_SCR_MODE_VISIBLE,   CFG_VAL_INTEGER, NULL, 1, false },  /*CFG_MENU_MAIN_SCR_MODE*/
    { CFG_MENU_DISPLAY_VISIBLE,    CFG_VAL_INTEGER, NULL, 1, false },  /*CFG_MENU_MAIN_DISPLAY*/
    { CFG_MENU_POWER_VISIBLE,      CFG_VAL_INTEGER, NULL, 1, false },  /*CFG_MENU_MAIN_POWER*/
    { CFG_MENU_DATE_TIME_VISIBLE,  CFG_VAL_INTEGER, NULL, 1, false },  /*CFG_MENU_MAIN_DATE_TIME*/
    { CFG_MENU_LANGUAGE_VISIBLE,   CFG_VAL_INTEGER, NULL, 1, false },  /*CFG_MENU_MAIN_LANGUAGE*/
    { CFG_MENU_TVOUT_VISIBLE,      CFG_VAL_INTEGER, NULL, 1, false },  /*CFG_MENU_MAIN_TVOUT*/
    { CFG_MENU_SYNC_PLIST_VISIBLE, CFG_VAL_INTEGER, NULL, 1, false },  /*CFG_MENU_MAIN_SYNC_PLIST*/
    { CFG_MENU_ADVANCED_VISIBLE,   CFG_VAL_INTEGER, NULL, 1, false },  /*CFG_MENU_MAIN_ADVANCED*/
};

cfg_t g_menu_scr_mode_cfgs[] =
{
    { CFG_MENU_SCR_MODE_HORIZONTAL_VISIBLE, CFG_VAL_INTEGER, NULL, 1, false },  /*CFG_MENU_SCR_MODE_HORIZONTAL*/
    { CFG_MENU_SCR_MODE_VERITCAL_VISIBLE,   CFG_VAL_INTEGER, NULL, 1, false },  /*CFG_MENU_SCR_MODE_VERITCAL*/
    { CFG_MENU_SCR_MODE_AUTO_VISIBLE,       CFG_VAL_INTEGER, NULL, 1, false },  /*CFG_MENU_SCR_MODE_AUTO*/
};

cfg_t g_menu_display_cfgs[] =
{
    { CFG_MENU_DISPLAY_BRIGHTNESS_VISIBLE, CFG_VAL_INTEGER, NULL, 1, false },  /*CFG_MENU_DISPLAY_BRIGHTNESS*/
    { CFG_MENU_DISPLAY_BACKLIGHT_VISIBLE, CFG_VAL_INTEGER, NULL, 1, false },   /*CFG_MENU_DISPLAY_BACKLIGHT*/
};

cfg_t g_menu_power_cfgs[] =
{
    { CFG_MENU_POWER_IDLE_TIME_VISIBLE, CFG_VAL_INTEGER, NULL, 1, false },   /*CFG_MENU_POWER_IDLE_TIME*/
    { CFG_MENU_POWER_SLEEP_TIME_VISIBLE, CFG_VAL_INTEGER, NULL, 1, false },  /*CFG_MENU_POWER_SLEEP_TIME*/
};

cfg_t g_menu_date_time_cfgs[] =
{
    { CFG_MENU_DATE_TIME_ADJUST_DATE_VISIBLE, CFG_VAL_INTEGER, NULL, 1, false },   /*CFG_MENU_DATE_TIME_ADJUST_DATE*/
    { CFG_MENU_DATE_TIME_ADJUST_TIME_VISIBLE, CFG_VAL_INTEGER, NULL, 1, false },   /*CFG_MENU_DATE_TIME_ADJUST_TIME*/
    { CFG_MENU_DATE_TIME_DRM_TIME_VISIBLE,    CFG_VAL_INTEGER, NULL, 1, false },   /*CFG_MENU_DATE_TIME_DRM_TIME*/
};

cfg_t g_menu_advanced_cfgs[] =
{
    { CFG_MENU_ADVANCED_INFO_VISIBLE,          CFG_VAL_INTEGER, NULL, 1, false }, /*CFG_MENU_ADVANCED_INFO*/
    { CFG_MENU_ADVANCED_CONNECT_TYPE_VISIBLE,  CFG_VAL_INTEGER, NULL, 1, false }, /*CFG_MENU_ADVANCED_CONNECT_TYPE*/
    { CFG_MENU_ADVANCED_FONT_VISIBLE,          CFG_VAL_INTEGER, NULL, 1, false }, /*CFG_MENU_ADVANCED_FONT*/
    { CFG_MENU_ADVANCED_KEY_TONE_VISIBLE,      CFG_VAL_INTEGER, NULL, 1, false }, /*CFG_MENU_ADVANCED_KEY_TONE*/
    { CFG_MENU_ADVANCED_SHOW_EFFECT_VISIBLE,   CFG_VAL_INTEGER, NULL, 1, false }, /*CFG_MENU_ADVANCED_SHOW_EFFECT*/
    { CFG_MENU_ADVANCED_SEARCH_FOLDER_VISIBLE, CFG_VAL_INTEGER, NULL, 1, false }, /*CFG_MENU_ADVANCED_SEARCH_FOLDER*/
    { CFG_MENU_ADVANCED_GSENSOR_SHAKE_SUPPORT_VISIBLE, CFG_VAL_INTEGER, NULL, 1, false},/*CFG_MENU_ADVANCED_GSENSOR..*/
    { CFG_MENU_ADVANCED_FORMAT_INTERNAL_VISIBLE, CFG_VAL_INTEGER, NULL, 1, false},/*CFG_MENU_ADVANCED_FORMAT_INTERNAL*/
    { CFG_MENU_ADVANCED_FORMAT_CARD_VISIBLE,   CFG_VAL_INTEGER, NULL, 1, false }, /*CFG_MENU_ADVANCED_FORMAT_CARD*/
    { CFG_MENU_ADVANCED_FORMAT_EXTERNAL_VISIBLE, CFG_VAL_INTEGER, NULL, 1, false},/*CFG_MENU_ADVANCED_FORMAT_EXTERNAL*/
    { CFG_MENU_ADVANCED_SET_USERNAME_VISIBLE,  CFG_VAL_INTEGER, NULL, 1, false },/*CFG_MENU_ADVANCED_SET_USERNAME*/
    { CFG_MENU_ADVANCED_CLEAR_DRM_VISIBLE,     CFG_VAL_INTEGER, NULL, 1, false },/*CFG_MENU_ADVANCED_CLEAR_DRM*/
    { CFG_MENU_ADVANCED_CHECK_SYS_VISIBLE,     CFG_VAL_INTEGER, NULL, 1, false },/*CFG_MENU_ADVANCED_CHECK_SYS*/
    { CFG_MENU_ADVANCED_RESET_VISIBLE,         CFG_VAL_INTEGER, NULL, 1, false },/*CFG_MENU_ADVANCED_RESET*/
};

cfg_t g_menu_info_cfgs[] =
{
    { CFG_MENU_INFO_USERNAME_VISIBLE, CFG_VAL_INTEGER, NULL, 1, false }, /*CFG_MENU_INFO_USERNAME*/
    { CFG_MENU_INFO_VERSION_VISIBLE,  CFG_VAL_INTEGER, NULL, 1, false }, /*CFG_MENU_INFO_VERSION*/
    { CFG_MENU_INFO_INTERNAL_SPACE_VISIBLE,  CFG_VAL_INTEGER, NULL, 1, false }, /*CFG_MENU_INFO_INTERNAL_SPACE*/
    { CFG_MENU_INFO_INTERNAL_FREE_VISIBLE,  CFG_VAL_INTEGER, NULL, 1, false }, /*CFG_MENU_INFO_INTERNAL_FREE*/
    { CFG_MENU_INFO_CARD_SPACE_VISIBLE,  CFG_VAL_INTEGER, NULL, 1, false }, /*CFG_MENU_INFO_CARD_SPACE*/
    { CFG_MENU_INFO_CARD_FREE_VISIBLE,  CFG_VAL_INTEGER, NULL, 1, false }, /*CFG_MENU_INFO_CARD_FREE*/
    { CFG_MENU_INFO_EXTERNAL_SPACE_VISIBLE,  CFG_VAL_INTEGER, NULL, 1, false }, /*CFG_MENU_INFO_EXTERNAL_SPACE*/
    { CFG_MENU_INFO_EXTERNAL_FREE_VISIBLE,  CFG_VAL_INTEGER, NULL, 1, false }, /*CFG_MENU_INFO_EXTERNAL_FREE*/
};

bool setting_cfg_init(cfg_t cfgs[], int count)
{
    int i;
    char * buf = NULL;

    if(NULL == cfgs)
    {
        print_err("input parmeter(cfgs) is a NULL pointer!");
        return false;
    }

    for(i = 0; i < count; i++)
    {
        if(cfgs[i].value_type == CFG_VAL_INTEGER)
        {
            continue;
        }

        buf = (char *)malloc(CFG_TEXT_LEN);
        if(buf == NULL)
        {
            print_err("malloc failed!");
            return false;
        }

        if(cfgs[i].text != NULL)
        {
            strncpy(buf, cfgs[i].text, CFG_TEXT_LEN);
        }

        cfgs[i].text = buf;
    }

    return true;
}

bool setting_cfg_deinit(cfg_t cfgs[], int count)
{
    int i;

    if(NULL == cfgs)
    {
        print_err("input parmeter(cfgs) is a NULL pointer!");
        return false;
    }

    for(i = 0; i < count; i++)
    {
        if(cfgs[i].value_type == CFG_VAL_INTEGER)
        {
            continue;
        }

        if(cfgs[i].text != NULL)
        {
            free(cfgs[i].text);
            cfgs[i].text = NULL;
        }
    }

    return true;
}

bool setting_cfg_read(cfg_t * cfg)
{
    char text[CFG_TEXT_LEN];
    int value;

    if(NULL == cfg)
    {
        print_err("input parmeter(cfg) is a NULL pointer!");
        return false;
    }

    if(CFG_VAL_STRING == cfg->value_type)
    {
        if(get_config(cfg->key, text, CFG_TEXT_LEN) < 0)
        {
            print_info("get_config(%s) failed! write default value to it!", cfg->key);
            if(set_config(cfg->key, cfg->text, (int)(strlen(cfg->text)+1)) < 0)
            {
                print_err("set_config(%s) failed!", cfg->key);
                return false;
            }
        }
        else
        {
            strncpy(cfg->text, text, CFG_TEXT_LEN);
        }
    }
    else if(CFG_VAL_INTEGER == cfg->value_type)
    {
        if(get_config(cfg->key, (char *)&value, sizeof(value)) < 0)
        {
            print_info("get_config(%s) failed! write default value to it!", cfg->key);
            if(set_config(cfg->key, (char *)&cfg->value, sizeof(cfg->value)) < 0)
            {
                print_err("set_config(%s) failed!", cfg->key);
                return false;
            }
        }
        else
        {
            cfg->value = value;
        }
    }
    else
    {
    }

    cfg->changed = false;

    return true;
}

bool setting_cfg_read_all(cfg_t cfgs[], int count)
{
    int i;

    if(NULL == cfgs)
    {
        print_err("input parmeter(cfgs) is a NULL pointer!");
        return false;
    }

    for(i = 0; i < count; i++)
    {

        if(setting_cfg_read(&cfgs[i]) == false)
        {
            return false;
        }
    }

    return true;
}

bool setting_cfg_write(cfg_t * cfg)
{
    if(NULL == cfg)
    {
        print_err("input parmeter(cfg) is a NULL pointer!");
        return false;
    }

    if(false == cfg->changed)
    {
        print_warning("config(%s) is not changed, no need to write!", cfg->key);
        return true;
    }

    if(CFG_VAL_STRING == cfg->value_type)
    {
        if(set_config(cfg->key, cfg->text, (int)(strlen(cfg->text)+1)) < 0)
        {
            print_err("set_config(%s) failed!", cfg->key);
            return false;
        }
    }
    else if(CFG_VAL_INTEGER == cfg->value_type)
    {
        if(set_config(cfg->key, (char *)&cfg->value, sizeof(cfg->value)) < 0)
        {
            print_err("set_config(%s) failed!", cfg->key);
            return false;
        }
    }
    else
    {
    }

    cfg->changed = false;

    return true;
}

bool setting_cfg_write_all(cfg_t cfgs[], int count)
{   
    int i;

    if(NULL == cfgs)
    {
        print_err("input parmeter(cfgs) is a NULL pointer!");
        return false;
    }

    for(i = 0; i < count; i++)
    {

        if(setting_cfg_write(&cfgs[i]) == false)
        {
            return false;
        }
    }

    return true;
}

