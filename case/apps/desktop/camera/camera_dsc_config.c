/*******************************************************************************
 *                              USDK(1100)
 *                            Module: camera_dsc
 *                 Copyright(c) 2003-2008 Actions Semiconductor,
 *                            All Rights Reserved.
 *
 * History:
 *      <author>    <time>           <version >             <desc>
 *        2009-5-26 15:36   1.0             build this file
 *******************************************************************************/
/*!
 * \file     camera_dsc_config.c
 * \brief    概述
 * \author
 * \par      GENERAL DESCRIPTION:
 *               参数保存设置
 * \par      EXTERNALIZED FUNCTIONS:
 *               用到外面的模块
 * \version 1.0
 * \date  2009-5-26
 *******************************************************************************/
#include "camera_dsc_config.h"

#include "camera_dsc_control.h"
#include "camera_scene_dsc.h"
#include "camera_define.h"
#include "camera_main.h"

char user_name[MAXNAMLEN];
char machine_name[MAXNAMLEN]= "chip of atj213x";

static int s_savepath = 0;
camera_dsc_cfg_t g_camera_dsc_option_visible_cfgs[] =
{
    //camera_dsc_option_visible_cfg_type_e
    { CFG_DSC_VISIBLE_PLAYBACK, DSC_CFG_VALUE_TYPE_INTEGER, "", 0, false },
    { CFG_DSC_VISIBLE_WHITEBALANCE, DSC_CFG_VALUE_TYPE_INTEGER, "", 0, false },
    { CFG_DSC_VISIBLE_BRIGHTNESS, DSC_CFG_VALUE_TYPE_INTEGER, "", 0, false },
    { CFG_DSC_VISIBLE_EXPOSURE_MODE, DSC_CFG_VALUE_TYPE_INTEGER, "", 0, false },
    { CFG_DSC_VISIBLE_FLASHLAMP, DSC_CFG_VALUE_TYPE_INTEGER, "", 0, false },
    { CFG_DSC_VISIBLE_FACE_IDENTIFY, DSC_CFG_VALUE_TYPE_INTEGER, "", 0, false },
    { CFG_DSC_VISIBLE_STICKERPHOTO, DSC_CFG_VALUE_TYPE_INTEGER, "", 0, false },
    { CFG_DSC_VISIBLE_SPECIALEFFECT, DSC_CFG_VALUE_TYPE_INTEGER, "", 0, false },
    { CFG_DSC_VISIBLE_SAVEPATH, DSC_CFG_VALUE_TYPE_INTEGER, "", 0, false },
    { CFG_DSC_VISIBLE_RESOLUTION, DSC_CFG_VALUE_TYPE_INTEGER, "", 0, false },
    { CFG_DSC_VISIBLE_FREE_SPACE, DSC_CFG_VALUE_TYPE_INTEGER, "", 0, false },
    { CFG_DSC_VISIBLE_SELFTIMER,  DSC_CFG_VALUE_TYPE_INTEGER,  "",  0,  false },
    { CFG_DSC_VISIBLE_CONTSHOOTING,DSC_CFG_VALUE_TYPE_INTEGER, "",  0,  false },
    { CFG_DSC_VISIBLE_SHUTTER_SOUND, DSC_CFG_VALUE_TYPE_INTEGER, "", 0, false },
    { CFG_DSC_VISIBLE_DATER, DSC_CFG_VALUE_TYPE_INTEGER, "", 0, false },
};

camera_dsc_cfg_t g_camera_dsc_sticker_cfgs[] =
{
    //camera_dsc_sticker_cfg_type_e
    //{ CFG_DSC_HAVE_STICKER, DSC_CFG_VALUE_TYPE_INTEGER, "", 0, false },
    { CFG_DSC_STIKCER_FULLNAME_1, DSC_CFG_VALUE_TYPE_STRING, "dsc_stikcer_fullname_1", 0, false },
    { CFG_DSC_STIKCER_TMP_1, DSC_CFG_VALUE_TYPE_STRING, "dsc_stikcer_tmp_1", 0, false },
    { CFG_DSC_STIKCER_FULLNAME_2, DSC_CFG_VALUE_TYPE_STRING, "dsc_stikcer_fullname_2", 0, false },
    { CFG_DSC_STIKCER_TMP_2, DSC_CFG_VALUE_TYPE_STRING, "dsc_stikcer_tmp_2", 0, false },
    { CFG_DSC_STIKCER_FULLNAME_3, DSC_CFG_VALUE_TYPE_STRING, "dsc_stikcer_fullname_3", 0, false },
    { CFG_DSC_STIKCER_TMP_3, DSC_CFG_VALUE_TYPE_STRING, "dsc_stikcer_tmp_3", 0, false },
};

camera_dsc_cfg_t g_camera_dsc_shutter_sound_cfgs[] =
{
    //camera_dsc_shutter_sound_cfg_type_e
    //{ CFG_DSC_HAVE_SHUTTER_SOUND, DSC_CFG_VALUE_TYPE_INTEGER, "", 0, false },
    { CFG_DSC_SHUTTER_SOUND_FULLNAME_1, DSC_CFG_VALUE_TYPE_STRING, "dsc_shutter_sound_fullname_1", 0, false },
    { CFG_DSC_SHUTTER_SOUND_FULLNAME_2, DSC_CFG_VALUE_TYPE_STRING, "dsc_shutter_sound_fullname_2", 0, false },
    { CFG_DSC_SHUTTER_SOUND_FULLNAME_3, DSC_CFG_VALUE_TYPE_STRING, "dsc_shutter_sound_fullname_3", 0, false },
};

camera_dsc_cfg_t g_camera_dsc_cfgs[] =
{
    //序号注意这里与枚举值要对应
    //camera_dsc_cfg_type_e
    //{ CFG_DSC_HAVE_FLASHLAMP, DSC_CFG_VALUE_TYPE_INTEGER, "", 0, false },
    { CFG_DSC_PLAYBACK, DSC_CFG_VALUE_TYPE_INTEGER, "", 0, false },
    { CFG_DSC_WHITEBALANCE, DSC_CFG_VALUE_TYPE_INTEGER, "", 0, false },
    { CFG_DSC_BRIGHTNESS, DSC_CFG_VALUE_TYPE_INTEGER, "", 0, false },
    { CFG_DSC_EXPOSURE_MODE, DSC_CFG_VALUE_TYPE_INTEGER, "", 0, false },
    { CFG_DSC_FLASHLAMP, DSC_CFG_VALUE_TYPE_INTEGER, "", 0, false },
    { CFG_DSC_FACE_IDENTIFY, DSC_CFG_VALUE_TYPE_INTEGER, "", 0, false },
    { CFG_DSC_STICKERPHOTO, DSC_CFG_VALUE_TYPE_INTEGER, "", 0, false },
    { CFG_DSC_SPECIALEFFECT, DSC_CFG_VALUE_TYPE_INTEGER, "", 0, false },
    { CFG_DSC_SAVEPATH, DSC_CFG_VALUE_TYPE_INTEGER, "", 0, false },
    { CFG_DSC_RESOLUTION, DSC_CFG_VALUE_TYPE_INTEGER, "", 1, false }, //abt??resolution没有0
    //{ CFG_DSC_SELFTIMER,  DSC_CFG_VALUE_TYPE_INTEGER,  "",  0,  false },
    //{ CFG_DSC_CONTSHOOTING,DSC_CFG_VALUE_TYPE_INTEGER, "",  0,  false },
    { CFG_DSC_SHUTTER_SOUND, DSC_CFG_VALUE_TYPE_INTEGER, "", 0, false },
    { CFG_DSC_DATER, DSC_CFG_VALUE_TYPE_INTEGER, "", 0, false },
#if (SHORTCUT_PLAYBACK_DSC == 1)
    { CFG_DSC_PLAYBACK_ROOT, DSC_CFG_VALUE_TYPE_STRING, "/mnt/disk0/PICTURES/CAMERA/", 0, false },
    { CFG_DSC_PLAYBACK_FULLNAME, DSC_CFG_VALUE_TYPE_STRING, "/mnt/disk0/PICTURES/CAMERA/20090429231718.jpg", 0, false },
#endif /* SHORTCUT_PLAYBACK_DSC */
};

bool camera_dsc_cfg_init(camera_dsc_cfg_t cfgs[], int count)
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
        if(cfgs[i].value_type == DSC_CFG_VALUE_TYPE_INTEGER)
        {
            continue;
        }

        buf = (char *)malloc(CFG_TEXT_LEN);
        if(buf == NULL)
        {
            print_err("malloc failed!");
            goto cfg_init_out;
        }

        if(cfgs[i].text != NULL)
        {
            strncpy(buf, cfgs[i].text, CFG_TEXT_LEN);
        }

        cfgs[i].text = buf;
    }

    return true;
    cfg_init_out: camera_dsc_cfg_deinit(cfgs, count);

    return false;
}

bool camera_dsc_cfg_deinit(camera_dsc_cfg_t cfgs[], int count)
{
    int i;

    if(NULL == cfgs)
    {
        print_err("input parmeter(cfgs) is a NULL pointer!");
        return false;
    }

    for(i = 0; i < count; i++)
    {
        if(cfgs[i].value_type == DSC_CFG_VALUE_TYPE_INTEGER)
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

bool camera_dsc_cfg_read(camera_dsc_cfg_t * cfg)
{
    char text[CFG_TEXT_LEN];
    int value;

    if (NULL == cfg)
    {
        print_err("input parmeter(cfg) is a NULL pointer!");
        return false;
    }

    if (DSC_CFG_VALUE_TYPE_STRING == cfg->value_type)
    {
        if (get_config(cfg->key, text, CFG_TEXT_LEN) < 0)
        {
            print_info("get_config(%s) failed! write default value to it!", cfg->key);
            if (set_config(cfg->key, cfg->text, (int)(strlen(cfg->text)+1)) < 0)
            {
                print_err("set_config(%s) failed!", cfg->key);
                return false;
            }
        }
        else
        {
            strncpy(cfg->text, text, CFG_TEXT_LEN);
            print_dbg("**R*CFG******%s(%s)***********\n", cfg->key, cfg->text);
        }
    }
    else if (DSC_CFG_VALUE_TYPE_INTEGER == cfg->value_type)
    {
        if (get_config(cfg->key, (char *) &value, sizeof(value)) < 0)
        {
            print_info("get_config(%s) failed! write default value to it!", cfg->key);
            if (set_config(cfg->key, (char *) &cfg->value, sizeof(cfg->value)) < 0)
            {
                print_err("set_config(%s) failed!", cfg->key);
                return false;
            }
        }
        else
        {
            cfg->value = value;
            if (cfg->value != 0)
            {
                print_dbg("**R*CFG******%s(%d)***********\n", cfg->key, cfg->value);
            }
        }
    }
    else
    {
    }
    cfg->changed = false;

    return true;
}

bool camera_dsc_cfg_read_all(camera_dsc_cfg_t cfgs[], int count)
{
    int i;

    if (NULL == cfgs)
    {
        print_err("input parmeter(cfgs) is a NULL pointer!");
        goto cfg_out;
    }

    for (i = 0; i < count; i++)
    {
        if (camera_dsc_cfg_read(&cfgs[i]) == false)
        {
            print_err("cfg err!\n");
            goto cfg_out;
        }
        //success read
    }
    return true;
    cfg_out:

    return false;
}

bool camera_dsc_cfg_write(camera_dsc_cfg_t * cfg)
{
    if (NULL == cfg)
    {
        print_err("input parmeter(cfg) is a NULL pointer!");
        return false;
    }

    if (false == cfg->changed)
    {
        print_warning("config(%s) is not changed, no need to write!", cfg->key);
        return true;
    }

    if (DSC_CFG_VALUE_TYPE_STRING == cfg->value_type)
    {
        if (set_config(cfg->key, cfg->text, (int)(strlen(cfg->text)+1)) < 0)
        {
            print_err("set_config(%s) failed!", cfg->key);
            return false;
        }
        print_dbg("**W*CFG******%s(%s)***********\n", cfg->key, cfg->text);
    }
    else if (DSC_CFG_VALUE_TYPE_INTEGER == cfg->value_type)
    {
        if (set_config(cfg->key, (char *) &cfg->value, sizeof(cfg->value)) < 0)
        {
            print_err("set_config(%s) failed!", cfg->key);
            return false;
        }
        print_dbg("**W*CFG******%s(%d)***********\n", cfg->key, cfg->value);
    }
    else
    {
    }
    cfg->changed = false;

    return true;
}

bool camera_dsc_cfg_write_all(camera_dsc_cfg_t cfgs[], int count)
{
    int i;

    if (NULL == cfgs)
    {
        print_err("input parmeter(cfgs) is a NULL pointer!");
        goto cfg_out;
    }

    for (i = 0; i < count; i++)
    {
        if (camera_dsc_cfg_write(&cfgs[i]) == false)
        {
            print_err("cfg err!\n");
            goto cfg_out;
        }
    }

    return true;

    cfg_out:

    return false;
}

#define INIT_VISIBLE
bool camera_dsc_init_check_visible_cfg_read(void)
{
    bool ret = false;

    if(camera_dsc_cfg_init(g_camera_dsc_option_visible_cfgs, CAM_DSC_CFG_VISIBLE_TYPE_TOTAL) == false)
    {
        print_err("cfg err!\n");
        goto cfg_out;
    }
    if(camera_dsc_cfg_read_all(g_camera_dsc_option_visible_cfgs, CAM_DSC_CFG_VISIBLE_TYPE_TOTAL) == false)
    {
        print_err("cfg err!\n");
        goto cfg_out;
    }

    ret = true;
    return ret;

    cfg_out:
    camera_dsc_cfg_deinit(g_camera_dsc_option_visible_cfgs, CAM_DSC_CFG_VISIBLE_TYPE_TOTAL);

    return ret;

}
bool camera_dsc_deinit_check_visible_cfg_read(void)
{
    camera_dsc_cfg_deinit(g_camera_dsc_option_visible_cfgs, CAM_DSC_CFG_VISIBLE_TYPE_TOTAL);
    return true;
}
#undef INIT_VISIBLE

static int s_stikers = -1;
bool camera_dsc_init_sticker_cfg_read(void)
{
    s_stikers = g_camera_dsc_option_visible_cfgs[CAM_DSC_CFG_VISIBLE_TYPE_STICKERPHOTO].value ;
    if (s_stikers == 0xffff)
    {
        return true;
    }

    s_stikers *=2;

    if (s_stikers > 0)
    {
        if(camera_dsc_cfg_init(g_camera_dsc_sticker_cfgs, s_stikers) == false)
        {
            print_err("cfg err!\n");
            goto cfg_out;
        }
        if(camera_dsc_cfg_read_all(g_camera_dsc_sticker_cfgs, s_stikers) == false)
        {
            print_err("cfg err!\n");
            goto cfg_out;
        }
    }

    return true;

    cfg_out:
    camera_dsc_cfg_deinit(g_camera_dsc_sticker_cfgs, s_stikers);

    return false;
}

bool camera_dsc_deinit_sticker_cfg_read(void)
{
    if ((s_stikers > 0) && (s_stikers != 0xffff))
    {
        if(camera_dsc_cfg_deinit(g_camera_dsc_sticker_cfgs, s_stikers) == false)
        {
            print_err("cfg err!\n");
            return false;
        }
    }
    return true;
}
static int s_shutters = -1;
bool camera_dsc_init_shutter_cfg_read(void)
{
    s_shutters = g_camera_dsc_option_visible_cfgs[CAM_DSC_CFG_VISIBLE_TYPE_SHUTTER_SOUND].value ;
    if (s_shutters == 0xffff)
    {
        return true;
    }
    s_shutters *=2 ;

    if (s_shutters > 0)
    {
        if(camera_dsc_cfg_init(g_camera_dsc_shutter_sound_cfgs, s_shutters) == false)
        {
            print_err("cfg err!\n");
            goto cfg_out;
        }
        if(camera_dsc_cfg_read_all(g_camera_dsc_shutter_sound_cfgs, s_shutters) == false)
        {
            print_err("cfg err!\n");
            goto cfg_out;
        }
    }
    return true;

    cfg_out:
    camera_dsc_cfg_deinit(g_camera_dsc_shutter_sound_cfgs, s_shutters);

    return false;
}
bool camera_dsc_deinit_shutter_cfg_read(void)
{
    if ((s_shutters > 0) && (s_shutters != 0xffff))
    {
        if(camera_dsc_cfg_deinit(g_camera_dsc_shutter_sound_cfgs, s_shutters) == false)
        {
            print_err("cfg err!\n");
            return false;
        }
    }
    return true;
}
#define INIT_CFG_PARA
bool camera_dsc_init_parameter_read(void)
{

    #if 0
    /*! 如果要读取设置中的拥有者*/
    if (-1 == get_config(CFG_USERNAME, exifAttr.owner, sizeof(exifAttr.owner)))
    {
        print_err("get config fail, errno = 0x%x", errno);
        snprintf(exifAttr.owner,sizeof(exifAttr.owner), "action-semi");
        /*! 这里返回错误值就会逐级退出，最后退出dsc */
        return false;
    }

    print_dbg("**owner =%s\n",exifAttr.owner);
    #endif

    if (-1 == get_config(CFG_USERNAME, user_name, sizeof(user_name)))
    {
        print_err("get config fail, errno = 0x%x", errno);
        return false;
    }
    if (-1 == get_config(CFG_DSC_MACHINENAME, machine_name, sizeof(machine_name)))
    {
        print_err("get config fail, errno = 0x%x", errno);
        return false;
    }
    
    if(camera_dsc_cfg_init(g_camera_dsc_cfgs, CAM_DSC_CFG_TYPE_TOTAL) == false)
    {
        print_err("init cfg err!\n");
        goto err_out;
    }

    if(camera_dsc_cfg_read_all(g_camera_dsc_cfgs, CAM_DSC_CFG_TYPE_TOTAL) == false)
    {
        print_err("cfg err!\n");
        goto err_out;
    }
    camera_dsc_cfg_after_read_value_regulate();

    return true;
    err_out:
    camera_dsc_cfg_deinit(g_camera_dsc_cfgs, CAM_DSC_CFG_TYPE_TOTAL);
    return false;
}

bool camera_dsc_deinit_parameter_write(void)
{
    camera_dsc_cfg_before_write_value_regulate();
    if (camera_dsc_cfg_write_all(g_camera_dsc_cfgs, CAM_DSC_CFG_TYPE_TOTAL) == false)
    {
        print_err("cfg err!\n");
        return false;
    }

    if(camera_dsc_cfg_deinit(g_camera_dsc_cfgs, CAM_DSC_CFG_TYPE_TOTAL) == false)
    {
        print_err("cfg err!\n");
        return false;
    }

    return true;
}
#undef INIT_CFG_PARA

#define INIT_CFG
bool camera_dsc_init_all_cfg(void)
{
    if(camera_dsc_init_check_visible_cfg_read() == false)
    {
        goto visible_cfg_read_err;
    }
    if(camera_dsc_init_sticker_cfg_read() == false)
    {
        goto sticker_cfg_read_err;
    }
    if(camera_dsc_init_shutter_cfg_read() == false)
    {
        goto shutter_cfg_read_err;
    }
    if(camera_dsc_init_parameter_read() == false)
    {
        goto parameter_cfg_read_err;
    }

    return true;
    parameter_cfg_read_err:
    //camera_dsc_deinit_parameter_write();
    camera_dsc_cfg_deinit(g_camera_dsc_cfgs, CAM_DSC_CFG_TYPE_TOTAL);
    shutter_cfg_read_err:
    camera_dsc_deinit_shutter_cfg_read();
    sticker_cfg_read_err:
    camera_dsc_deinit_sticker_cfg_read();
    visible_cfg_read_err:
    camera_dsc_deinit_check_visible_cfg_read();

    return false;

}
bool camera_dsc_deinit_all_cfg(void)
{
    camera_dsc_deinit_parameter_write();
    camera_dsc_deinit_shutter_cfg_read();
    camera_dsc_deinit_sticker_cfg_read();
    camera_dsc_deinit_check_visible_cfg_read();
    return true;
}
#undef INIT_CFG
void camera_dsc_save_parameter(cam_dsc_parameter_e fun, int parameter)
{
    assert(parameter >= 0);
    switch (fun)
    {
        case CAM_DSC_PARA_WHITE_BALANCE:
        assert(parameter <5);
        g_camera_dsc_cfgs[CAM_DSC_CFG_TYPE_WHITE_BALANCE].value = parameter;
        g_camera_dsc_cfgs[CAM_DSC_CFG_TYPE_WHITE_BALANCE].changed = true;
        break;

        case CAM_DSC_PARA_EXPOSURE_MODE:
        assert(parameter <3);
        g_camera_dsc_cfgs[CAM_DSC_CFG_TYPE_EXPOSURE_MODE].value = parameter;
        g_camera_dsc_cfgs[CAM_DSC_CFG_TYPE_EXPOSURE_MODE].changed = true;
        break;

        case CAM_DSC_PARA_FLASH_LAMP:
        assert(parameter <3);
        g_camera_dsc_cfgs[CAM_DSC_CFG_TYPE_FLASH_LAMP].value = parameter;
        g_camera_dsc_cfgs[CAM_DSC_CFG_TYPE_FLASH_LAMP].changed = true;
        break;
        case CAM_DSC_PARA_STICKER_PHOTO:
        assert(parameter <4);
        g_camera_dsc_cfgs[CAM_DSC_CFG_TYPE_STICKERPHOTO].value = parameter;
        g_camera_dsc_cfgs[CAM_DSC_CFG_TYPE_STICKERPHOTO].changed = true;
        break;
        case CAM_DSC_PARA_SPECIAL_EFFECT:
        assert(parameter <4);
        g_camera_dsc_cfgs[CAM_DSC_CFG_TYPE_SPECIAL_EFFECT].value = parameter;
        g_camera_dsc_cfgs[CAM_DSC_CFG_TYPE_SPECIAL_EFFECT].changed = true;
        break;
        case CAM_DSC_PARA_SAVE_PATH:
        assert(parameter <3);
        g_camera_dsc_cfgs[CAM_DSC_CFG_TYPE_SAVE_PATH].value = parameter;
        g_camera_dsc_cfgs[CAM_DSC_CFG_TYPE_SAVE_PATH].changed = true;
        break;
        case CAM_DSC_PARA_RESOLUTION:
        {
            int offset, max_scale;
            camera_dsc_get_cur_sensor_items(&offset, &max_scale);
            assert(parameter <= max_scale);
        }
        g_camera_dsc_cfgs[CAM_DSC_CFG_TYPE_RESOLUTION].value = parameter;
        g_camera_dsc_cfgs[CAM_DSC_CFG_TYPE_RESOLUTION].changed = true;

        break;

        /*! ap进入的时候读取数值并设定播放路径，拍照时播放声音 */
        case CAM_DSC_PARA_SHUTTER_SOUND:
        assert(parameter <4);
        g_camera_dsc_cfgs[CAM_DSC_CFG_TYPE_SHUTTER_SOUND].value = parameter;
        g_camera_dsc_cfgs[CAM_DSC_CFG_TYPE_SHUTTER_SOUND].changed = true;
        break;

        case CAM_DSC_PARA_DATE_STAMP:
        assert(parameter <3);
        g_camera_dsc_cfgs[CAM_DSC_CFG_TYPE_DATE_STAMP].value = parameter;
        g_camera_dsc_cfgs[CAM_DSC_CFG_TYPE_DATE_STAMP].changed = true;
        break;
        //=============================================
        case CAM_DSC_PARA_PLAYBACK:
        g_camera_dsc_cfgs[CAM_DSC_CFG_TYPE_PLAYBACK].value = parameter;
        g_camera_dsc_cfgs[CAM_DSC_CFG_TYPE_PLAYBACK].changed = true;
        //保存菜单项目，路径表，跳转到photo,写pb
        break;
        case CAM_DSC_PARA_BRIGHTNESS:
        assert(parameter <7);
        g_camera_dsc_cfgs[CAM_DSC_CFG_TYPE_BRIGHTNESS].value = parameter;
        g_camera_dsc_cfgs[CAM_DSC_CFG_TYPE_BRIGHTNESS].changed = true;
        //保存当前亮度
        break;
        case CAM_DSC_PARA_FACE_IDENTIFY:
        assert(parameter <2);
        //是否人脸侦测
        g_camera_dsc_cfgs[CAM_DSC_CFG_TYPE_FACE_IDENTIFY].value = parameter;
        g_camera_dsc_cfgs[CAM_DSC_CFG_TYPE_FACE_IDENTIFY].changed = true;
        break;

        case CAM_DSC_PARA_SELF_TIMER:
        assert(parameter < 5);
        g_camera_scene_dsc->dsc_config.cfg_adj_self_time = parameter;
        //自拍时间
        break;
        case CAM_DSC_PARA_CONT_SHOOTING:
        assert((parameter < 7) && (parameter != 5));
        g_camera_scene_dsc->dsc_config.cfg_adj_cont_shooting = parameter;
        //连拍时间
        break;

        //================================================
#if 0
        case CAM_DSC_PARA_ZOOM_LEVEL:
        /*当前放大倍数*/
        g_camera_scene_dsc->dsc_config.cfg_zoom_level = parameter;
#endif
        default:
        print_err("save para err\n");
        break;
    }
}
/*! 主要config配置不支持负数 */
bool camera_dsc_cfg_after_read_value_regulate(void)
{
    /*! 保存的数字0--6这些修改为-3~~3 */
    assert((g_camera_dsc_cfgs[CAM_DSC_CFG_TYPE_BRIGHTNESS].value < 7) &&
            (g_camera_dsc_cfgs[CAM_DSC_CFG_TYPE_BRIGHTNESS].value >= 0));
    g_camera_dsc_cfgs[CAM_DSC_CFG_TYPE_BRIGHTNESS].value -= 3;

    /*! 记录刚进入时保存的path */
    s_savepath = g_camera_dsc_cfgs[CAM_DSC_CFG_TYPE_SAVE_PATH].value;

    return true;
}
bool camera_dsc_cfg_before_write_value_regulate(void)
{
    g_camera_dsc_cfgs[CAM_DSC_CFG_TYPE_BRIGHTNESS].value += 3;
    assert((g_camera_dsc_cfgs[CAM_DSC_CFG_TYPE_BRIGHTNESS].value < 7) &&
            (g_camera_dsc_cfgs[CAM_DSC_CFG_TYPE_BRIGHTNESS].value >= 0));

    if (s_savepath != g_camera_dsc_cfgs[CAM_DSC_CFG_TYPE_SAVE_PATH].value)
    {
        /* 因为插拔卡等动作，随时可能改变保存路径 */
        g_camera_dsc_cfgs[CAM_DSC_CFG_TYPE_SAVE_PATH].changed = true;
    }

    return true;
}

bool camera_dsc_check_path(hotplugin_dev_type_e path)
{
    bool result = false;
    int parameter = -1;
    hotplugin_dev_type_e main_disk;
    
    /*! 预览前(bf open mmm)和拍照前检查一次路径,外面卡等变化的时候 */
    /* 创建目录结构 */
    /*log 15/10/2009 10:49:10 所有的路径更新为同内部存储器结构*/
    //if (g_camera_dsc_cfgs[CAM_DSC_CFG_TYPE_SAVE_PATH].value == CAM_DSC_STORAGE_EXTERNAL)
    if(path == HOTPLUGIN_DEV_TYPE_UHOST)
    {
        hotplugin_dev_info_t external_info;
        //注意在硬件有时拔除检查不出来的时候
        sys_get_hotplugin_dev_info(HOTPLUGIN_DEV_TYPE_UHOST, &external_info);
        if (external_info.access == HOTPLUGIN_DEV_ACCESS_OK)
        {
            snprintf(dsc_filepath, sizeof(dsc_filepath), sys_get_media_dir(HOTPLUGIN_DEV_TYPE_UHOST,DIR_PHOTO_CAMERA));
            result = sys_check_media_dir(sys_get_media_dir(HOTPLUGIN_DEV_TYPE_UHOST,DIR_PHOTO_CAMERA), EXTERNAL_ROOT);
            if (result == true)
            {
                //=true只是说明存在了目录并可读，不代表可以写，磁盘满要靠别的检查
                parameter = 1;
            }
            else
            {
                //目录检测失败，不存在目录并且不能创建，如写保护,文件系统被破坏
                parameter = 2;
            }                        
        }
        else
        {
            //未插卡，或者mount失败(未格式化，msgbox提示:卡初始化失败)
            parameter = 0;
        }
    }
    //else if (g_camera_dsc_cfgs[CAM_DSC_CFG_TYPE_SAVE_PATH].value == CAM_DSC_STORAGE_CARD)
    else if(path == HOTPLUGIN_DEV_TYPE_CARD)
    {
        hotplugin_dev_info_t card_info;

        sys_get_hotplugin_dev_info(HOTPLUGIN_DEV_TYPE_CARD, &card_info);
        if (card_info.access == HOTPLUGIN_DEV_ACCESS_OK)
        {
            snprintf(dsc_filepath, sizeof(dsc_filepath), sys_get_media_dir(HOTPLUGIN_DEV_TYPE_CARD,DIR_PHOTO_CAMERA));
            result = sys_check_media_dir(sys_get_media_dir(HOTPLUGIN_DEV_TYPE_CARD,DIR_PHOTO_CAMERA), CARD_ROOT);
            if (result == true)
            {
                parameter = 1;
            }
            else
            {
                parameter = 2;
            }       
        }
        else
        {
            parameter = 0;
        }
    }
    //else if (g_camera_dsc_cfgs[CAM_DSC_CFG_TYPE_SAVE_PATH].value == CAM_DSC_STORAGE_INTERNAL)
    else if(path == HOTPLUGIN_DEV_TYPE_LOCAL)
    {
        hotplugin_dev_info_t internal_info;

        sys_get_hotplugin_dev_info(HOTPLUGIN_DEV_TYPE_LOCAL, &internal_info);
        if (internal_info.access == HOTPLUGIN_DEV_ACCESS_OK)
        {
            snprintf(dsc_filepath, sizeof(dsc_filepath), sys_get_media_dir(HOTPLUGIN_DEV_TYPE_LOCAL,DIR_PHOTO_CAMERA));
            result = sys_check_media_dir(sys_get_media_dir(HOTPLUGIN_DEV_TYPE_LOCAL,DIR_PHOTO_CAMERA), UDISK_ROOT);
            if (result == true)
            {
                parameter = 1;
            }
            else
            {
                parameter = 2;
            }       
        }
        else
        {
            parameter = 0;
        }
    }
    else
    {
        //g_camera_dsc_cfgs[CAM_DSC_CFG_TYPE_SAVE_PATH].value = CAM_DSC_STORAGE_NONE;
        //if(path == HOTPLUGIN_DEV_TYPE_NONE)

        parameter = 0;
    }

    main_disk = sys_get_main_disk();

    if ((parameter == 0) && (main_disk != path))
    {
        if(main_disk == HOTPLUGIN_DEV_TYPE_LOCAL)
        {
            /* 主盘是 本地磁盘*/
            snprintf(dsc_filepath, sizeof(dsc_filepath), sys_get_media_dir(HOTPLUGIN_DEV_TYPE_LOCAL,DIR_PHOTO_CAMERA));
            g_camera_dsc_cfgs[CAM_DSC_CFG_TYPE_SAVE_PATH].value = CAM_DSC_STORAGE_INTERNAL;
            result = sys_check_media_dir(sys_get_media_dir(HOTPLUGIN_DEV_TYPE_LOCAL,DIR_PHOTO_CAMERA), UDISK_ROOT);
            if (result == true)
            {
                //=true只是说明存在了目录并可读，不代表可以写，磁盘满要靠别的检查
                parameter = 3;
            }
            else
            {
                //目录检测失败，不存在目录并且不能创建，如写保护,文件系统被破坏
                parameter = 4;
            }                    
        }
        else if(main_disk == HOTPLUGIN_DEV_TYPE_CARD)
        {
             /*主盘是卡*/
            snprintf(dsc_filepath, sizeof(dsc_filepath), sys_get_media_dir(HOTPLUGIN_DEV_TYPE_CARD,DIR_PHOTO_CAMERA));
            g_camera_dsc_cfgs[CAM_DSC_CFG_TYPE_SAVE_PATH].value = CAM_DSC_STORAGE_CARD;
            result = sys_check_media_dir(sys_get_media_dir(HOTPLUGIN_DEV_TYPE_CARD,DIR_PHOTO_CAMERA), CARD_ROOT);
            if (result == true)
            {
                parameter = 3;
            }
            else
            {
                parameter = 4;
            }      
        }
        else if(main_disk == HOTPLUGIN_DEV_TYPE_UHOST)
        {
             /*主盘是uhost盘*/
            snprintf(dsc_filepath, sizeof(dsc_filepath), sys_get_media_dir(HOTPLUGIN_DEV_TYPE_UHOST,DIR_PHOTO_CAMERA));
            g_camera_dsc_cfgs[CAM_DSC_CFG_TYPE_SAVE_PATH].value = CAM_DSC_STORAGE_EXTERNAL;
            result = sys_check_media_dir(sys_get_media_dir(HOTPLUGIN_DEV_TYPE_UHOST,DIR_PHOTO_CAMERA), EXTERNAL_ROOT);
            if (result == true)
            {
                parameter = 3;
            }
            else
            {
                parameter = 4;
            }      
        }
        else
        {
             /*没有盘*/
            print_err("has no main disk");
            parameter =0; 
            //result = false;
        }
    }

    switch (parameter)
    {
        case 0:
        camera_dsc_dialog_create(CAM_DIALOG_NODISK);
        result = false;
        g_camera_dsc_cfgs[CAM_DSC_CFG_TYPE_SAVE_PATH].value = CAM_DSC_STORAGE_NONE;
        g_setpath = true;
        print_err("sys_check_media_dir fail\n");            
        break;
        
        case 2:
        case 4:
        camera_dsc_dialog_create(CAM_DIALOG_SETPATH_FAIL);
        //g_camera_dsc_cfgs[CAM_DSC_CFG_TYPE_SAVE_PATH].value = CAM_DSC_STORAGE_ERR;            
        g_setpath = false;
        result = false;
        break;
        
        case 1:
        case 3:
        /*为避免单个文件夹内文件过多时打开速度慢
        故在CAMERA文件夹下再创建以年月日命名的文件夹
        各文件夹最多放FILE_MAX个文件*/
        if(true == camera_check_folder(dsc_filepath))
        {
            g_setpath = true;
            result = true;
        }
        else
        {
            camera_dsc_dialog_create(CAM_DIALOG_SETPATH_FAIL);
            g_setpath = false;
            result = false;
        }
        break;
        
        default:
        //其他不提示
        result = false;
        g_setpath = true;
        break;
    }
    return result;
}

