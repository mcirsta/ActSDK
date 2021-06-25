/*******************************************************************************
 *                              USDK(1100)
 *                            Module: camera_dv
 *                 Copyright(c) 2003-2008 Actions Semiconductor,
 *                            All Rights Reserved.
 *
 * History:
 *      <author>    <time>           <version >             <desc>
 *        2009-5-26 15:56   1.0             build this file
 *******************************************************************************/
/*!
 * \file     camera_dv_option.c
 * \brief    概述
 * \author
 * \par      GENERAL DESCRIPTION:
 *               menu主菜单
 * \par      EXTERNALIZED FUNCTIONS:
 *               用到外面的模块
 * \version 1.0
 * \date  2009-5-26
 * \version 2.0
 * \date 2009-10-22
 *******************************************************************************/
#include "camera_dv_option.h"

#include "camera_dv_config.h"
#include "camera_sty.h"
#include "camera_scene_dv.h"
#include "camera_dv_control.h"
#include "camera_dv_ui.h"

#include "camera_dv_option_exposure_mode.h"
#include "camera_dv_option_free_space.h"
#include "camera_dv_option_resolution.h"
#include "camera_dv_option_save_path.h"
#include "camera_dv_option_special_effect.h"
#include "camera_dv_option_white_balance.h"
#include "camera_dv_option_record_quality.h"
#include "camera_dv_widget_option.h"
#include "camera_define.h"

typedef struct
{
    /*! unselect icon style id in *.sty file */
    unsigned int style_unsel_icon_id;

    /*! select icon style id in *.sty file */
    unsigned int style_sel_icon_id;

    /*! text id in *.sty file */
    unsigned int style_text_id;

    /*! has child menu ? */
    bool has_child;

    /*! menu item callback */
    item_cb_t callback;

    /*! dynamic text */
    char *text;

    /*! visible in menu ? */
    bool visible;

    /*! index in menu */
    int index;
}menuitem_style_t;

menu_t *g_cam_dv_option = NULL;
picture_resource_t *g_cam_dv_opt_dot = NULL;
/* menu max items */
#if (DV_ACTIVITY_DETECT == 1)
#define CAM_DV_OPTION_MAX_ITEMS   11
#else
#define CAM_DV_OPTION_MAX_ITEMS   10//dv menu have 10 items
#endif
static void _dv_fusion_cb(unsigned char *framebuffer, int width, int height);
static bool _check_dv_option_visible( void );
static int _proc_item_levelone(menuview_t *menuview, menuview_sig_e signal);
static int _dv_load_item_resource(resgroup_resource_t* res_group, menuitem_style_t* menuitem_style, \
        menuitem_t* menuitem,  int index_array[], int count);

static void _dv_fusion_cb(unsigned char *framebuffer, int width, int height)
{
    usleep(1000* 1 );
}
#undef ARRAY_SIZE
#define ARRAY_SIZE(A)    (sizeof(A)/sizeof((A)[0]))

/* menu items style data define */
static menuitem_style_t s_dv_option_style_items[] =
{
    { 0, 0, STR_DV_PLAYBACK,           false, (item_cb_t)_proc_item_levelone,       NULL, true, 0 },
    { 0, 0, STR_WHITE_BALANCE,          true, (item_cb_t)_proc_item_levelone,      NULL, true, 0 },
    { 0, 0, STR_BRIGHTNESS,                true, (item_cb_t)_proc_item_levelone,       NULL, true, 0 },
    { 0, 0, STR_EXPOSURE_MODE,         true, (item_cb_t)_proc_item_levelone,      NULL, true, 0 },
    { 0, 0, STR_SPECIAL_EFFECT,          true, (item_cb_t)_proc_item_levelone,       NULL, true, 0 },
    { 0, 0, STR_SAVE_PATH,                  true, (item_cb_t)_proc_item_levelone,  NULL, true, 0 },
    { 0, 0, STR_RESOLUTION,                true, (item_cb_t)_proc_item_levelone,     NULL, true, 0 },
    { 0, 0, STR_FREE_SPACE,                false, (item_cb_t)_proc_item_levelone,        NULL, true, 0 },
    { 0, 0, STR_RECORD_QUALITY,        true, (item_cb_t)_proc_item_levelone,     NULL, true, 0 },
    { 0, 0, STR_RECORD_GAIN,             true, (item_cb_t)_proc_item_levelone,            NULL, true, 0 },
    { 0, 0, STR_ACTIVITY_DETECT,       true, (item_cb_t)_proc_item_levelone,            NULL, true, 0 },        
};
//CAM_DV_OPTION_MAX_ITEMS
static int s_visible_menu_real_index[ARRAY_SIZE(s_dv_option_style_items)];

/*********************************************************************************
 * interface implementaiton
 *********************************************************************************/
bool camera_dv_option_enter(menuview_t *menuview, fusion_enter_e fenter )
{
    menu_t *cur_menu;

    #if 0
    cur_menu = camera_dv_option_init();
    menuview_set_menu(menuview, cur_menu);
    menuview_paint(menuview);
    #else
    fusion_effect_t eff;
    /*
    option特效第一帧需要清空,设置此界面之前的数据, 作为特效第一帧
    blending的创建在paint之前, 否则blending中会有paint的数据, 导致特效第一帧已经是最后一帧
    目前只考虑进入的时候有特效，第二层回第一层我特效
    */

    if ( FUSION_FORWARD == fenter )
    {
        //menuview_load_style( menuview, MENUVIEW_OPTION );
        //Optimize for Speed:显示类型不同时才重新加载资源
        if ( menuview->type != MENUVIEW_OPTION )
        {
            menuview_load_style( menuview, MENUVIEW_OPTION );
        }
        /*
        注意：动态旋转处理（如scene_list_rotation函数）中的menuview_load_style
        不用修改，因为横竖改变，即使显示类型不变也要重新加载资源。
        */        
        canvas_erase_all( menuview->canvas_id );
        fusion_get_config(FUSION_TYPE_OPTION_YUV, fenter, &eff);
        fusion_set_effect(&eff);        
        option_fusion_set_first_frame(menuview);
        //camera_dv_option_blending_create_show();
    }

    //if(NULL == g_cam_dv_option)
    {
        cur_menu = camera_dv_option_init();
    }
    menuview_set_menu(menuview, cur_menu);
    menuview_paint( menuview );
    if ( FUSION_FORWARD == fenter )
    {
        option_fusion_set_last_frame(menuview);
        /*注意:调用fusion_display显示前有可能其他高优先级的线程抢先刷屏，
        导致整个菜单数据（最后一帧）被抢先刷新到屏幕上造成闪屏，
        所以先清空，再创建bleeding区域
        */
        canvas_erase_all( menuview->canvas_id );
        camera_dv_option_blending_create_show();
        fusion_display(_dv_fusion_cb);
    }
    #endif
    return true;
}

bool camera_dv_option_exit(menuview_t *menuview)
{
    camera_dv_option_blending_delete();

    //delete main menu
    camera_dv_option_deinit();

    //exit option_ui
    camera_dv_widget_option_exit(menuview, NULL);

    return true;
}

int camera_dv_option_deinit(void)
{
    //delete menu
    if (g_cam_dv_option != NULL)
    {
        //create menu
        menu_delete(g_cam_dv_option);
        g_cam_dv_option = NULL;
    }

    return true;
}
static int _dv_load_item_resource(resgroup_resource_t* res_group, menuitem_style_t* menuitem_style, \
        menuitem_t* menuitem,  int index_array[], int count)
{
    int i = 0 ;
    int temp_index = 0;
    picture_resource_t * pic_res;
    string_resource_t  * str_res;

    printf("sizeof option=%d\n",count);
    for(i = 0; i < count; i++)
    {
        if(menuitem_style[i].visible == false)
        {
            //index_array[i] = -1; 
            continue; 
        }
        else
        {
            index_array[temp_index] = i;
        }

        if(menuitem_style[i].style_unsel_icon_id != 0)
        {
            pic_res = ( picture_resource_t * )get_resgroup_child(res_group, menuitem_style[i].style_unsel_icon_id);
            if(pic_res != NULL)
            {
                menuitem[temp_index].unsel_icon_id = pic_res->pic_id;
            }
        }
        else
        {
            menuitem[temp_index].unsel_icon_id = 0;
        }

        if(menuitem_style[i].style_sel_icon_id != 0)
        {
            pic_res = ( picture_resource_t * )get_resgroup_child(res_group, menuitem_style[i].style_sel_icon_id);
            if(pic_res != NULL)
            {
                menuitem[temp_index].sel_icon_id = pic_res->pic_id;
            }
        }
        else
        {
            menuitem[temp_index].sel_icon_id = 0;
        }

        if(menuitem_style[i].style_text_id != 0)
        {
            str_res = ( string_resource_t * )get_resgroup_child(res_group, menuitem_style[i].style_text_id);
            if(str_res != NULL)
            {
                menuitem[temp_index].text_id = str_res->str_id;
            }
        }
        else
        {
            menuitem[temp_index].text_id = 0;
        }

        menuitem[temp_index].has_child = menuitem_style[i].has_child;
        menuitem[temp_index].callback = menuitem_style[i].callback;
        menuitem[temp_index].text = menuitem_style[i].text;
        menuitem_style[i].index = temp_index;
        temp_index ++;
    }

    return temp_index;
}

menu_t* camera_dv_option_init(void)
{
    scene_t * scene;
    resgroup_resource_t * res_group;

#pragma __PRQA_IGNORE_START__
    menuitem_t menu_items[ARRAY_SIZE(s_dv_option_style_items)] = {{0}};
#pragma __PRQA_IGNORE_END__
    int real_items = 0;

    //initial menu
    if (g_cam_dv_option == NULL)
    {
        scene = sty_get_app_scene();
        if ( NULL == scene )
        {
            print_err("get app scene failed!");
            return NULL;
        }

        //load advanced menu resource group
        res_group = (resgroup_resource_t *)get_scene_child(scene, DV_STRGROUP_OPTION_MAIN);
        if(NULL == res_group)
        {
            print_err("get resource group REG_ADVANCED");
            return NULL;
        }
        g_cam_dv_opt_dot = (picture_resource_t *) get_scene_child(scene, DV_PIC_OPT_DOT);
        if(NULL == g_cam_dv_opt_dot)
        {
            print_err("get resource DOT");
            return NULL;
        }
        if(_check_dv_option_visible() == false)
        {
            return NULL;
            // TODO:返回false会出现问题
        }

        /* load menu item string and pictrure resource */
        real_items = _dv_load_item_resource(res_group, s_dv_option_style_items, menu_items,\
        s_visible_menu_real_index, ARRAY_SIZE(s_dv_option_style_items));

        /* load other string resourc */
        //_load_str_res(res_group, s_str_res_table, ARRAY_SIZE(s_str_res_table));

        //create menu
        g_cam_dv_option = menu_create();
        if(NULL == g_cam_dv_option)
        {
            print_err("create menu failed!");
            return NULL;
        }
        //init menu items
        menu_append_items( g_cam_dv_option, menu_items, real_items);
        menu_set_cur_index( g_cam_dv_option, 0 );
    }

    return g_cam_dv_option;
}

static bool _check_dv_option_visible( void )
{
    int i = 0;

    for (i = 0; i <CAM_DV_CFG_VISIBLE_TYPE_TOTAL; i++)
    {
        if(g_camera_dv_option_visible_cfgs[i].value == 0xffff)
        {
            s_dv_option_style_items[i].visible = false;
        }
    }
    
    return true;
}

#if (DV_ACTIVITY_DETECT == 1)
static bool _dv_adjust_set_switch_info(adjust_switch_init_t *adjust_switch)
{
    scene_t* scene; //!< app scene

    unsigned int str_id;
    resgroup_resource_t *group;
    string_resource_t *str_res;
    memset(adjust_switch, 0, sizeof(adjust_switch_init_t));

    scene = sty_get_app_scene();
    if (scene == NULL)
    {
        print_err("cann't find app scene!");
        return false;
    }

    //load resource group
    group = (resgroup_resource_t *) get_scene_child(scene, DV_STRGROUP_OPTION_MAIN );
    if (group == NULL)
    {
        return false;
    }    
    str_res = (string_resource_t *) get_resgroup_child(group, STR_ACTIVITY_DETECT);
    if (str_res == NULL)
    {
        return false;
    }
    str_id = str_res->str_id;
    adjust_switch->str_title_id = (int)str_id;

    /*
    1，adjust_switch->on_off_status 0==open 1==close
    如果保存的是打开下次adjust需要显示关闭为高亮
    2，g_camera_dv_cfgs[CAM_DV_CFG_TYPE_ACTIVITY_DETECT].value 0,close 1,open
    3，mmm cmd 0,close 1,open
    (进入dv默认为0,如果config为1需要设置一次)
    注意mmm如果已经为0不能再设0，如果为1不能再设1，否则会死机
    4，回归测试修改了，显示为用户保存的
    */
    if (g_camera_dv_cfgs[CAM_DV_CFG_TYPE_ACTIVITY_DETECT].value == 0)
    {
        adjust_switch->on_off_status = ADJUST_OFF;
    }
    else
    {
        adjust_switch->on_off_status = ADJUST_ON;
    }
    print_info("set=%d\n",adjust_switch->on_off_status);
    return true;
}
static void _dv_adjust_activity_detect_callback(adjust_t *adjust, adjust_signal_e signal)
{
    adj_switch_mode_e on_off;
    unsigned int param;
    switch (signal)
    {
        case WIDGET_SIG_ADJUST_OK:
        print_dbg("activity_detec ok\n");
        adjust_get_switch_status(&on_off);
        print_info("get=%d\n",on_off);
        if (on_off == ADJUST_ON)
        {
            param = 1;
        }
        else
        {
            param = 0;
        }
        if(param != (unsigned int)g_camera_dv_cfgs[CAM_DV_CFG_TYPE_ACTIVITY_DETECT].value)
        {
            camera_dv_ctr_set_activity_detect((unsigned int) (param));
            camera_dv_save_parameter(CAM_DV_PARA_ACTIVITY_DETECT, (int)param);
        }
        //have no icon camera_dv_change_icon(CAM_ICON_WHITE_BALANCE,cur_item_pos);
        camera_dv_adjust_blending_delete();
        adjust_delete(adjust);
        g_camera_scene_dv->adjust = NULL;

        break;
        case WIDGET_SIG_ADJUST_CANCEL:
        print_dbg("activity_detec esc\n");
        camera_dv_adjust_blending_delete();
        adjust_delete(adjust);
        g_camera_scene_dv->adjust = NULL;

        break;

        default:
        break;
    }
}
#endif

static bool _dv_adjust_brightness(adjust_textdigit_init_t *adjust_digit)
{
    scene_t* scene; //!< app scene
    resgroup_resource_t *group;
    string_resource_t *str_res;

    scene = sty_get_app_scene();
    if (scene == NULL)
    {
        print_err("cann't find app scene!");
        return false;
    }
    //load resource group
    group = (resgroup_resource_t *) get_scene_child(scene, DV_STRGROUP_OPTION_MAIN);
    if (group == NULL)
    {
        return false;
    }    
    str_res = (string_resource_t *) get_resgroup_child(group, STR_BRIGHTNESS);
    if (str_res == NULL)
    {
        return false;
    }
    memset(adjust_digit, 0, sizeof(adjust_textdigit_init_t));
    adjust_digit->str_title_id = (int) str_res->str_id;
    adjust_digit->max_val = 3;
    adjust_digit->min_val = -3;
    adjust_digit->cur_val = g_camera_dv_cfgs[CAM_DV_CFG_TYPE_BRIGHTNESS].value;
    adjust_digit->step = 1;
    return true;
}
static void _dv_adjust_brightness_callback(adjust_t *adjust, adjust_signal_e signal)
{
    int value;
    bool ret;
    print_dbg("_adjust_callback signal=%d\n", signal);
    switch (signal)
    {
        case WIDGET_SIG_ADJUST_OK:
        print_dbg("WIDGET_SIG_ADJUST_OK\n");
        ret = adjust_get_current_val(&value);
        if (ret == true)
        {
            if (g_camera_dv_cfgs[CAM_DV_CFG_TYPE_BRIGHTNESS].value != value)
            {
                camera_dv_save_parameter(CAM_DV_PARA_BRIGHTNESS, value);
                camera_dv_change_icon(CAM_ICON_BRIGHTNESS, (unsigned int)value);
            }
        }
        print_dbg("getvalue%d\n", value);
        camera_dv_adjust_blending_delete();
        adjust_delete(adjust);
        //adjust = NULL;
        g_camera_scene_dv->adjust = NULL;
        break;
        case WIDGET_SIG_ADJUST_CANCEL:
        print_dbg("WIDGET_SIG_ADJUST_CANCEL\n");
        ret = adjust_get_current_val(&value);

        if (ret == true)
        {
            if (g_camera_dv_cfgs[CAM_DV_CFG_TYPE_BRIGHTNESS].value != value)
            {
                camera_dv_ctr_set_brightness((unsigned int) g_camera_dv_cfgs[CAM_DV_CFG_TYPE_BRIGHTNESS].value);
            }
        }
        camera_dv_adjust_blending_delete();
        adjust_delete(adjust);
        //adjust = NULL;
        g_camera_scene_dv->adjust = NULL;
        break;

        case WIDGET_SIG_ADJUST_UP:
        print_dbg("WIDGET_SIG_ADJUST_UP\n");
        ret = adjust_get_current_val(&value);
        if (ret == true)
        {
            camera_dv_ctr_set_brightness((unsigned int) value);
        }
        break;

        case WIDGET_SIG_ADJUST_DOWN:
        print_dbg("WIDGET_SIG_ADJUST_DOWN\n");
        ret = adjust_get_current_val(&value);
        if (ret == true)
        {
            camera_dv_ctr_set_brightness((unsigned int) value);
        }
        break;

        default:
        break;
    }
}
//abt??重复的函数部分有待优化
static bool _dv_adjust_record_gain(adjust_textdigit_init_t *adjust_digit)
{
    scene_t* scene; //!< app scene
    resgroup_resource_t *group;
    string_resource_t *str_res;

    scene = sty_get_app_scene();
    if (scene == NULL)
    {
        print_err("cann't find app scene!");
        return false;
    }
    //load resource group
    group = (resgroup_resource_t *) get_scene_child(scene, DV_STRGROUP_OPTION_MAIN);
    if (group == NULL)
    {
        return false;
    }    
    str_res = (string_resource_t *) get_resgroup_child(group, STR_RECORD_GAIN);
    if (str_res == NULL)
    {
        return false;
    }
    memset(adjust_digit, 0, sizeof(adjust_textdigit_init_t));
    adjust_digit->str_title_id = (int) str_res->str_id;
    adjust_digit->max_val = 5;
    adjust_digit->min_val = 1;
    adjust_digit->cur_val = g_camera_dv_cfgs[CAM_DV_CFG_TYPE_RECORD_GAIN].value;
    adjust_digit->step = 1;
    return true;
}
static void _dv_adjust_record_gain_callback(adjust_t *adjust, adjust_signal_e signal)
{
    int value;
    bool ret;
    mmm_audio_param_t temp =
    {
        { 1, 0, 0, 0, 0, 0, 0 },
        { 1, 1 }, 8, 1, 8000, 0, 0
    };

    print_dbg("_adjust_callback signal=%d\n", signal);
    switch (signal)
    {
        case WIDGET_SIG_ADJUST_OK:
        print_dbg("WIDGET_SIG_ADJUST_OK\n");
        ret = adjust_get_current_val(&value);
        if (ret == true)
        {
            if (g_camera_dv_cfgs[CAM_DV_CFG_TYPE_RECORD_GAIN].value != value)
            {
                camera_dv_save_parameter(CAM_DV_PARA_RECORD_GAIN, value);
            }
        }
        print_dbg("getvalue%d\n", value);
        camera_dv_adjust_blending_delete();
        adjust_delete(adjust);
        //adjust = NULL;
        g_camera_scene_dv->adjust = NULL;
        break;
        case WIDGET_SIG_ADJUST_CANCEL:
        print_dbg("WIDGET_SIG_ADJUST_CANCEL\n");
        ret = adjust_get_current_val(&value);

        if (ret == true)
        {
            if (g_camera_dv_cfgs[CAM_DV_CFG_TYPE_RECORD_GAIN].value != value)
            {
                temp.voice_volt.input_volt = (unsigned int)g_camera_dv_cfgs[CAM_DV_CFG_TYPE_RECORD_GAIN].value;
                temp.voice_volt.output_volt = (unsigned int)g_camera_dv_cfgs[CAM_DV_CFG_TYPE_RECORD_GAIN].value;
                camera_dv_ctr_record_gain((mmm_audio_param_t *) (&temp));
            }
        }
        camera_dv_adjust_blending_delete();
        adjust_delete(adjust);
        //adjust = NULL;
        g_camera_scene_dv->adjust = NULL;
        break;

        case WIDGET_SIG_ADJUST_UP:
        print_dbg("WIDGET_SIG_ADJUST_UP\n");
        ret = adjust_get_current_val(&value);
        if (ret == true)
        {
            temp.voice_volt.input_volt = (unsigned int)value;
            temp.voice_volt.output_volt = (unsigned int)value;
            camera_dv_ctr_record_gain((mmm_audio_param_t *) (&temp));
        }
        break;

        case WIDGET_SIG_ADJUST_DOWN:
        print_dbg("WIDGET_SIG_ADJUST_DOWN\n");
        ret = adjust_get_current_val(&value);
        if (ret == true)
        {
            temp.voice_volt.input_volt = (unsigned int)value;
            temp.voice_volt.output_volt = (unsigned int)value;
            camera_dv_ctr_record_gain((mmm_audio_param_t *) (&temp));
        }
        break;

        default:
        break;
    }
}

bool camera_dv_playback(void)
{
    bool ret = false;
    const char* app_name = APP_NAME_VIDEO;//"video.app";
    int temp_scene = 2;
    /*需要回放的时候，设配置值拍照=1，录制=2，
    在回放结束后argv启动camera的时候读配置来判断启动场景*/
    print_info(" filepath=%s filename = %s", dv_filepath, dv_filename);

    if(g_camera_dv_cfgs[CAM_DV_CFG_TYPE_SAVE_PATH].value == CAM_DV_STORAGE_NONE)
    {
        camera_dv_dialog_create(CAM_DIALOG_NODISK);
        return false;
    }
    if(g_setpath == false)
    {
        camera_dv_dialog_create(CAM_DIALOG_SETPATH_FAIL);
        return false;
    }

    if (set_config(CFG_CAM_PLAYBACK_SCENE, (char*)&temp_scene, sizeof(temp_scene)) < 0)
    {
        print_err("set config fail, errno = 0x%x", errno);
        return false;
    }   
    
    camera_dv_ctr_stop_preview();
    camera_dv_blending_deinit();
    camera_dv_msgbox_delete();
    camera_dv_mmm_close();
    canvas_set_blending_mode(false);

    gui_dc_clear(g_camera_scene_dv->hdc );                

    if (app_name != NULL)
    {
        msg_apps_t t;
        size_t n_app_name = strlen(app_name);
        size_t n_file = strlen(dv_filename);
        size_t n_path = strlen(dv_filepath);
        size_t n_arg = strlen(VIDEO_ARGV_DV_DIRLIST);

        memset(&t, 0, sizeof(t));
        t.type = MSG_CREAT_APP_RESUME;//MSG_CREAT_APP;
        strncpy(t.content, app_name,n_app_name);
        strncat(t.content, " ",1);
        strncat(t.content, VIDEO_ARGV_DV_DIRLIST,n_arg);

        //增加路径top限制，告诉video顶层目录
        strncat(t.content, " ",1);
        strncat(t.content, "\"",1);
        strncat(t.content, dv_filepath,n_path);
        strncat(t.content, "\"",1);

        strncat(t.content, " ",1);
        strncat(t.content, "\"",1);
        strncat(t.content, dv_filepath,n_path);
        strncat(t.content, dv_filename,n_file);
        strncat(t.content, "\"",1);
        printf("t.content=%s", t.content);
        send_async_msg("manager.app", &t);
        enter_background_mode();
    }
    ret = true;
    print_err("playback out...\n");
    return ret;

}


static inline void _dv_sub_option_init(menu_t* cur_menu, menuview_t* menuview)
{
    if(cur_menu != NULL)
    {
        menuview_set_menu(menuview, cur_menu);
        menuview_paint(menuview);
    }
    else
    {
        print_err("_dv_sub_option_init err\n");
    }
}
static int _proc_item_levelone(menuview_t *menuview, menuview_sig_e signal)
{
    menu_t *cur_menu = menuview->menu;
    int cur = menu_get_cur_index(cur_menu);
    //--adjust


    print_dbg("cur=%d", cur);
    switch (signal)
    {
        case WIDGET_SIG_MENUVIEW_OK:
        print_dbg(">>>> menu_option: %s clicked", cur_item->text);
        printf("real index=%d\n",s_visible_menu_real_index[cur]);
        switch (s_visible_menu_real_index[cur])//获取索引
        {
            case CAM_DV_OPT_PLAYBACK:
            {
                camera_dv_option_exit(menuview);
                camera_dv_playback();
            }
            break;
            case CAM_DV_OPT_BRIGHTNESS:
            {
                adjust_textdigit_init_t adjust_digit;
                if (_dv_adjust_brightness(&adjust_digit) == false)
                {
                    print_err();
                    break;
                }
                g_camera_scene_dv->adjust = adjust_create(&adjust_digit, ADJ_STYLE_TEXTDIGIT,
                        _dv_adjust_brightness_callback);
                camera_dv_adjust_blending_create_show();
            }
            break;

            case CAM_DV_OPT_WHITE_BALANCE:
            _dv_sub_option_init((menu_t *) camera_dv_option_white_balance_init(), menuview);

            break;
            case CAM_DV_OPT_EXPOSURE_MODE:
            _dv_sub_option_init((menu_t *) camera_dv_option_exposure_mode_init(), menuview);

            break;
            case CAM_DV_OPT_SPECIAL_EFFECT:
            _dv_sub_option_init((menu_t *) camera_dv_option_special_effect_init(), menuview);

            break;
            case CAM_DV_OPT_SAVE_PATH:
            _dv_sub_option_init((menu_t *) camera_dv_option_save_path_init(), menuview);
            
            break;
            case CAM_DV_OPT_RESOLUTION:
            _dv_sub_option_init((menu_t *) camera_dv_option_resolution_init(), menuview);
            
            break;
            case CAM_DV_OPT_FREE_SPACE:
            _dv_sub_option_init((menu_t *) camera_dv_option_free_space_init(), menuview);
            
            break;
            case CAM_DV_OPT_RECORD_GAIN:
            {
                adjust_textdigit_init_t adjust_digit;
                if(_dv_adjust_record_gain(&adjust_digit) == false)
                {
                    print_err();
                    break;
                }
                g_camera_scene_dv->adjust = adjust_create(&adjust_digit, ADJ_STYLE_TEXTDIGIT,
                        _dv_adjust_record_gain_callback);
                camera_dv_adjust_blending_create_show();
            }
            break;
            case CAM_DV_OPT_RECORD_QUALITY:
            _dv_sub_option_init((menu_t *) camera_dv_option_record_quality_init(), menuview);
            
            break;
#if (DV_ACTIVITY_DETECT == 1)
            case CAM_DV_OPT_ACTIVITY_DETECT:
            {
                adjust_switch_init_t adjust_switch;
                if(_dv_adjust_set_switch_info(&adjust_switch) == false)
                {
                    print_err();
                    break;
                }
                g_camera_scene_dv->adjust = adjust_create(&adjust_switch, ADJ_STYLE_SWITCH,
                        _dv_adjust_activity_detect_callback);
                camera_dv_adjust_blending_create_show();
                break;
            }
#endif
            default:
            print_err();
            break;
        }

        break;

        case WIDGET_SIG_MENUVIEW_QUIT:
        // return to parent menu
        camera_dv_option_exit(menuview);
        break;

        default:
        break;
    }
    return 0;
}

