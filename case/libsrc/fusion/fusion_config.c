/********************************************************************************
 *                              USDK(1100)
 *                             Module: fusion lib
 *                 Copyright(c) 2003-2009 Actions Semiconductor,
 *                            All Rights Reserved.
 * File : fusion_config.c
 * History:
 *      <author>    <time>           <version >             <desc>
 *       cheason     2009-07-01        v1.0               build this file
 ********************************************************************************/
/*!
 * \file     fusion_config.c
 * \brief    ¶ÁÈ¡ÌØÐ§ÅäÖÃ
 * \author   cheason
 *
 * \version 1.0
 * \date  2009/07/01
 *******************************************************************************/

#include "include_case.h"
#include "fusion.h"

#define CFG_FUSION_FORWARD_SET "fusion_forward_set"

typedef struct
{
    char    *name;
    int     value;
}config_item_t;

typedef struct
{
    config_item_t   mode;
    config_item_t   dir[2];
}config_type_t;

config_type_t s_fusion_cfg[] =
{
    {{CFG_MODE_COMMON, -1}, {{CFG_FORWAED_COMMON, -1}, {CFG_BACKWARD_COMMON, -1}}},
    {{CFG_MODE_APP_CHANGE, -1}, {{CFG_FORWAED_APP_CHANGE, -1}, {CFG_BACKWARD_APP_CHANGE, -1}}},
    {{CFG_MODE_OPTION_RGB, -1}, {{CFG_FORWARD_OPTION, -1}, {CFG_BACKWARD_OPTION, -1}}},
    {{CFG_MODE_OPTION_YUV, -1}, {{CFG_FORWARD_OPTION, -1}, {CFG_BACKWARD_OPTION, -1}}},
    {{CFG_MODE_ROTATE, -1}, {{CFG_FORWARD_ROTATE, -1}, {CFG_BACKWARD_ROTATE, -1}}},
    {{CFG_MODE_DIALOG, -1}, {{CFG_FORWARD_DIALOG, -1}, {CFG_BACKWARD_DIALOG, -1}}},
};

#define FUSION_CFG_NUM  (sizeof(s_fusion_cfg)/sizeof(config_type_t))

static bool _get_configs(fusion_type_e type, fusion_enter_e enter)
{
    if((type >= FUSION_TYPE_MAX) || (enter >= FUSION_MAX))
    {
        print_err("null parameter: type=%d, enter=%d", type, enter);
        return false;
    }

    if(get_config(s_fusion_cfg[type].mode.name,
            (char *)&s_fusion_cfg[type].mode.value, sizeof(int)) < 0)
    {
        return false;
    }

    if(get_config(s_fusion_cfg[type].dir[enter].name,
            (char *)&s_fusion_cfg[type].dir[enter].value, sizeof(int)) < 0)
    {
        return false;
    }

    return true;
}

bool fusion_set_config(fusion_type_e type, fusion_enter_e enter, const fusion_effect_t *effect)
{
    if((NULL == effect) || (type >= FUSION_TYPE_MAX)
            || (enter >= FUSION_MAX))
    {
        print_err("null parameter: type=%d, enter=%d,effect=%d", type, enter, effect);
        return false;
    }

    if(s_fusion_cfg[type].mode.value != effect->mode)
    {
        s_fusion_cfg[type].mode.value = effect->mode;
        set_config(s_fusion_cfg[type].mode.name, (char *)&effect->mode, sizeof(int));
    }

    if(s_fusion_cfg[type].dir[enter].value != effect->direct)
    {
        s_fusion_cfg[type].dir[enter].value = effect->direct;
        set_config(s_fusion_cfg[type].dir[enter].name, (char *)&effect->direct, sizeof(int));
    }

    return true;
}


bool fusion_get_config(fusion_type_e type, fusion_enter_e enter, fusion_effect_t *effect)
{
	int temp;
	
    if((NULL == effect) || (type >= FUSION_TYPE_MAX)
            || (enter >= FUSION_MAX))
    {
        print_err("null parameter: type=%d, enter=%d,effect=%d", type, enter, effect);
        return false;
    }
    
    _get_configs(type, enter);
    
    if(FUSION_FORWARD == enter)
    {
        temp = 1;
        set_config(CFG_FUSION_FORWARD_SET, (char *)&temp, 4);
    }
    else
    {
        if((get_config(CFG_FUSION_FORWARD_SET, (char *)&temp, 4) == 0) && (temp == 1))
        {
            temp = 0;
            set_config(CFG_FUSION_FORWARD_SET, (char *)&temp, 4);
        }
        else
        {
            effect->mode = FUSION_NORMAL;
            effect->direct = TRANS_LEFT;
            return true;
        }
    }

    effect->mode = s_fusion_cfg[type].mode.value;
    effect->direct = s_fusion_cfg[type].dir[enter].value;

    return true;
}

bool fusion_update_config(void)
{
    return true;
}


