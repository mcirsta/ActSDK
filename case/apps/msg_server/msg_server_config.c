/********************************************************************************
 *                              USDK(1100)
 *                             Module: app 
 *                 Copyright(c) 2003-2008 Actions Semiconductor,
 *                            All Rights Reserved. 
 * file : msg_server_config.c
 * History:         
 *      <author>    <time>           <version >             <desc>
 *      Herb Hu    2009-07-08             v1.0             build this file 
 ********************************************************************************/

#include "msg_server_config.h"

cfg_t g_msg_server_cfgs[] =
{
    { CFG_SCREEN_MODE,           CFG_VAL_STRING,   "Vertical", 0,  false }, /* CFG_TYPE_SCR_MODE;    */
    { CFG_BACKLIGHT_BRIGHTNESS,  CFG_VAL_INTEGER,  NULL,       3,  false }, /* CFG_TYPE_BRIGHTNESS;  */
    { CFG_BACKLIGHT_DURATION,    CFG_VAL_INTEGER,  NULL,       0,  false }, /* CFG_TYPE_BACKLIGHT;   */
    { CFG_IDLE_TIME,             CFG_VAL_INTEGER,  NULL,       0,  false }, /* CFG_TYPE_IDLE_TIME;   */
    { CFG_SLEEP_TIME,            CFG_VAL_INTEGER,  NULL,       0,  false }, /* CFG_TYPE_SLEEP_TIME;  */
    { CFG_KEY_TONE_ON,           CFG_VAL_INTEGER,  NULL,       0,  false }, /* CFG_TYPE_KEY_TONE_ON; */
    { CFG_KEY_TONE_FULLPATH,     CFG_VAL_STRING,   NULL,       0,  false }, /* CFG_TYPE_KEY_TONE_FULLPATH;  */
    { CFG_GSENSOR_SHAKE_SUPPORT, CFG_VAL_STRING,   "on",       0,  false }, /* CFG_TYPE_GSENSOR_SHAKE_SUPPORT; */
    { CFG_GSENSOR_SHAKE_THRESHOLD, CFG_VAL_INTEGER,  NULL,       1500,  false }, /*  CFG_GSENSOR_SHAKE_THRESHOLD; */
    { CFG_UHOST_SUPPORT,         CFG_VAL_STRING,   "on",       0,  false }, /* CFG_TYPE_UHOST_SUPPORT;  */
    { CFG_CARD_SUPPORT,          CFG_VAL_STRING,   "on",       0,  false }, /* CFG_TYPE_CARD_SUPPORT;  */
};

bool msg_server_cfg_init(cfg_t cfgs[], int count)
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

bool msg_server_cfg_deinit(cfg_t cfgs[], int count)
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

bool msg_server_cfg_read(cfg_t * cfg)
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
            if(set_config(cfg->key, cfg->text, (int)(strlen(cfg->text) + 1)) < 0)
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

bool msg_server_cfg_read_all(cfg_t cfgs[], int count)
{
    int i;

    if(NULL == cfgs)
    {
        print_err("input parmeter(cfgs) is a NULL pointer!");
        return false;
    }

    for(i = 0; i < count; i++)
    {

        if(msg_server_cfg_read(&cfgs[i]) == false)
        {
            return false;
        }
    }

    return true;
}

bool msg_server_cfg_write(cfg_t * cfg)
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
        if(set_config(cfg->key, cfg->text, (int)(strlen(cfg->text) + 1)) < 0)
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

bool msg_server_cfg_write_all(cfg_t cfgs[], int count)
{   
    int i;

    if(NULL == cfgs)
    {
        print_err("input parmeter(cfgs) is a NULL pointer!");
        return false;
    }

    for(i = 0; i < count; i++)
    {

        if(msg_server_cfg_write(&cfgs[i]) == false)
        {
            return false;
        }
    }

    return true;
}

