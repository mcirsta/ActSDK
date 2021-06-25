/********************************************************************************
 *                              USDK(1100)
 *                             Module: app 
 *                 Copyright(c) 2003-2008 Actions Semiconductor,
 *                            All Rights Reserved. 
 * file : tvin_config.c
 * History:         
 *      <author>    <time>           <version >             <desc>
 *      Jerry Fu    2009-5-9             v1.0             build this file 
 ********************************************************************************/
/*!
* \file     tvin_config.c
* \brief    tvin  config write and read
* \author   Jerry Fu
*
* \version 1.0
* \date  2009/04/28
*******************************************************************************/
#include "tvin.h"

tvin_cfg_t g_tvin_cfgs[] =
{
    { CFG_TVIN_SIGNAL,              CFG_VALUE_TYPE_STRING,    "PAL",  0,  false },
    { CFG_TVIN_SAVEPATH,            CFG_VALUE_TYPE_INTEGER,   "",  0,  false },
    { CFG_TVIN_RECORD_QUALITY,      CFG_VALUE_TYPE_INTEGER,   "", 0,  false },
    { CFG_TVIN_LCD_ONOFF,           CFG_VALUE_TYPE_STRING,    "LCD_ON",  0,  false },
   
    { CFG_TVIN_SINGLE1_DATE,        CFG_VALUE_TYPE_STRING,    "2009-07-10",  0,  false },
    { CFG_TVIN_SINGLE2_DATE,        CFG_VALUE_TYPE_STRING,    "2009-07-10",  0,  false },
    { CFG_TVIN_SINGLE3_DATE,        CFG_VALUE_TYPE_STRING,    "2009-07-10",  0,  false },
    { CFG_TVIN_SINGLE1_TIME,        CFG_VALUE_TYPE_STRING,    "08:08",  0,  false },
    { CFG_TVIN_SINGLE2_TIME,        CFG_VALUE_TYPE_STRING,    "08:08",  0,  false },
    { CFG_TVIN_SINGLE3_TIME,        CFG_VALUE_TYPE_STRING,    "08:08",  0,  false },
    { CFG_TVIN_SINGLE1_LENGTH,      CFG_VALUE_TYPE_STRING,    "00:01",  0,  false },
    { CFG_TVIN_SINGLE2_LENGTH,      CFG_VALUE_TYPE_STRING,    "00:02",  0,  false },
    { CFG_TVIN_SINGLE3_LENGTH,      CFG_VALUE_TYPE_STRING,    "00:03",  0,  false },
    { CFG_TVIN_SINGLE1_ONOFF,       CFG_VALUE_TYPE_STRING,    "OFF",    0,  false },
    { CFG_TVIN_SINGLE2_ONOFF,       CFG_VALUE_TYPE_STRING,    "OFF",    0,  false },
    { CFG_TVIN_SINGLE3_ONOFF,       CFG_VALUE_TYPE_STRING,    "OFF",    0,  false },

    { CFG_TVIN_REPEAT1_TIME,        CFG_VALUE_TYPE_STRING,    "08:08",  0,  false },
    { CFG_TVIN_REPEAT2_TIME,        CFG_VALUE_TYPE_STRING,    "08:08",  0,  false },
    { CFG_TVIN_REPEAT3_TIME,        CFG_VALUE_TYPE_STRING,    "08:08",  0,  false },
    { CFG_TVIN_REPEAT1_LENGTH,      CFG_VALUE_TYPE_STRING,    "00:04",  0,  false },
    { CFG_TVIN_REPEAT2_LENGTH,      CFG_VALUE_TYPE_STRING,    "00:05",  0,  false },
    { CFG_TVIN_REPEAT3_LENGTH,      CFG_VALUE_TYPE_STRING,    "00:06",  0,  false },
    { CFG_TVIN_REPEAT1_WEEKDAY,     CFG_VALUE_TYPE_INTEGER,   "",  0x10,  false },
    { CFG_TVIN_REPEAT2_WEEKDAY,     CFG_VALUE_TYPE_INTEGER,   "",  0x10,  false },
    { CFG_TVIN_REPEAT3_WEEKDAY,     CFG_VALUE_TYPE_INTEGER,   "",  0x10,  false },
    { CFG_TVIN_REPEAT1_ONOFF,       CFG_VALUE_TYPE_STRING,    "OFF",    0,  false },
    { CFG_TVIN_REPEAT2_ONOFF,       CFG_VALUE_TYPE_STRING,    "OFF",    0,  false },
    { CFG_TVIN_REPEAT3_ONOFF,       CFG_VALUE_TYPE_STRING,    "OFF",    0,  false },            

    { CFG_TVIN_SINGLE1_CLOCKNUM,    CFG_VALUE_TYPE_INTEGER,    "",    0,  false },
    { CFG_TVIN_SINGLE2_CLOCKNUM,    CFG_VALUE_TYPE_INTEGER,    "",    0,  false },
    { CFG_TVIN_SINGLE3_CLOCKNUM,    CFG_VALUE_TYPE_INTEGER,    "",    0,  false },
    { CFG_TVIN_REPEAT1_CLOCKNUM,    CFG_VALUE_TYPE_INTEGER,    "",    0,  false },
    { CFG_TVIN_REPEAT2_CLOCKNUM,    CFG_VALUE_TYPE_INTEGER,    "",    0,  false },
    { CFG_TVIN_REPEAT3_CLOCKNUM,    CFG_VALUE_TYPE_INTEGER,    "",    0,  false },
};


bool tvin_cfg_read(tvin_cfg_t * cfg)
{
    char text[CFG_TEXT_LEN];
    int value;

    if(NULL == cfg)
    {
        print_err("input parmeter(cfg) is a NULL pointer!");
        return false;
    }

    if(CFG_VALUE_TYPE_STRING == cfg->value_type)
    {
        if(get_config(cfg->key, text, CFG_TEXT_LEN) < 0)
        {
            print_info("get_config(%s) failed! write default value to it!", cfg->key);
            if(set_config(cfg->key, cfg->text, CFG_TEXT_LEN) < 0)
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
    else
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

    cfg->changed = false;

    return true;
}

bool tvin_cfg_read_all(tvin_cfg_t cfgs[], int count)
{
    int i;

    if(NULL == cfgs)
    {
        print_err("input parmeter(cfgs) is a NULL pointer!");
        return false;
    }

    for(i = 0; i < count; i++)
    {

        if(tvin_cfg_read(&cfgs[i]) == false)
        {
            return false;
        }
    }

    return true;
}

bool tvin_cfg_write(tvin_cfg_t * cfg)
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

    if(CFG_VALUE_TYPE_STRING == cfg->value_type)
    {
        if(set_config(cfg->key, cfg->text, CFG_TEXT_LEN) < 0)
        {
            print_err("set_config(%s) failed!", cfg->key);
            return false;
        }
    }
    else
    {
        if(set_config(cfg->key, (char *)&cfg->value, sizeof(cfg->value)) < 0)
        {
            print_err("set_config(%s) failed!", cfg->key);
            return false;
        }
    }

    cfg->changed = false;

    return true;
}

bool tvin_cfg_write_all(tvin_cfg_t cfgs[], int count)
{   
    int i;

    if(NULL == cfgs)
    {
        print_err("input parmeter(cfgs) is a NULL pointer!");
        return false;
    }

    for(i = 0; i < count; i++)
    {

        if(tvin_cfg_write(&cfgs[i]) == false)
        {
            return false;
        }
    }

    return true;
}



