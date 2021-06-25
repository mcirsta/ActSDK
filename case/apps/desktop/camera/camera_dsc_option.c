/*******************************************************************************
 *                              USDK(1100)
 *                            Module: camera_dsc
 *                 Copyright(c) 2003-2008 Actions Semiconductor,
 *                            All Rights Reserved.
 *
 * History:
 *      <author>    <time>           <version >             <desc>
 *        2009-5-26 15:56   1.0             build this file
 *******************************************************************************/
/*!
 * \file     camera_dsc_option.c
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
#include "camera_dsc_option.h"

#include "camera_dsc_config.h"
#include "camera_sty.h"
#include "camera_scene_dsc.h"
#include "camera_dsc_control.h"
#include "camera_dsc_ui.h"

#include "camera_dsc_option_date_stamp.h"
#include "camera_dsc_option_flash_lamp.h"
#include "camera_dsc_option_exposure_mode.h"
#include "camera_dsc_option_free_space.h"
#include "camera_dsc_option_resolution.h"
#include "camera_dsc_option_save_path.h"
#include "camera_dsc_option_shutter_sound.h"
#include "camera_dsc_option_special_effect.h"
#include "camera_dsc_option_sticker_photo.h"
#include "camera_dsc_option_white_balance.h"

#include "camera_dsc_widget_option.h"

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


menu_t *g_cam_dsc_option = NULL;
picture_resource_t *g_cam_dsc_opt_dot = NULL;

static bool _check_dsc_option_visible( void );
static int _proc_item_levelone(menuview_t *menuview, menuview_sig_e signal);
static void _dsc_fusion_cb(unsigned char *framebuffer, int width, int height);
static int _dsc_load_item_resource(resgroup_resource_t* res_group, menuitem_style_t* menuitem_style, \
        menuitem_t* menuitem,  int index_array[], int count);

static void _dsc_fusion_cb(unsigned char *framebuffer, int width, int height)
{
    usleep(1000* 1 );
}


/*********************************************************************************
 * internal static struct and data definition
 *********************************************************************************/

#undef ARRAY_SIZE
#define ARRAY_SIZE(A)    (sizeof(A)/sizeof((A)[0]))

/* menu items style data define */
static menuitem_style_t s_dsc_option_style_items[] =
{
    { 0, 0, STR_DSC_PLAYBACK,           false, (item_cb_t)_proc_item_levelone,       NULL, true, 0 },
    { 0, 0, STR_WHITE_BALANCE,          true, (item_cb_t)_proc_item_levelone,      NULL, true, 0 },
    { 0, 0, STR_BRIGHTNESS,                true, (item_cb_t)_proc_item_levelone,       NULL, true, 0 },
    { 0, 0, STR_EXPOSURE_MODE,         true, (item_cb_t)_proc_item_levelone,      NULL, true, 0 },
    { 0, 0, STR_FLASH_LAMP,                 true, (item_cb_t)_proc_item_levelone,       NULL, true, 0 },
    { 0, 0, STR_FACE_IDENTIFY,             true, (item_cb_t)_proc_item_levelone,   NULL, true, 0 },
    { 0, 0, STR_STICKER_PHOTO,           true, (item_cb_t)_proc_item_levelone,  NULL, true, 0 },
    { 0, 0, STR_SPECIAL_EFFECT,           true, (item_cb_t)_proc_item_levelone,      NULL, true, 0 },
    { 0, 0, STR_SAVE_PATH,                   true, (item_cb_t)_proc_item_levelone,  NULL, true, 0 },
    { 0, 0, STR_RESOLUTION,                 true, (item_cb_t)_proc_item_levelone,     NULL, true, 0 },
    { 0, 0, STR_FREE_SPACE,                 false, (item_cb_t)_proc_item_levelone,        NULL, true, 0 },
    { 0, 0, STR_SELF_TIMER,                  true, (item_cb_t)_proc_item_levelone,     NULL, true, 0 },
    { 0, 0, STR_CONT_SHOOTING,          true, (item_cb_t)_proc_item_levelone,            NULL, true, 0 },
    { 0, 0, STR_SHUTTER_SOUND,          true, (item_cb_t)_proc_item_levelone,            NULL, true, 0 },
    { 0, 0, STR_DATE_STAMP,                true, (item_cb_t)_proc_item_levelone,            NULL, true, 0 },
        
};
static int s_visible_menu_real_index[ARRAY_SIZE(s_dsc_option_style_items)];

/*********************************************************************************
 * interface implementaiton
 *********************************************************************************/
bool camera_dsc_option_enter(menuview_t *menuview, fusion_enter_e fenter )
{
    menu_t *cur_menu;

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
        //camera_dsc_option_blending_create_show();
    }
    // TODO:recheck why mark
    //if(NULL == g_cam_dsc_option)
    {
        cur_menu = camera_dsc_option_init();
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
        camera_dsc_option_blending_create_show();
        fusion_display(_dsc_fusion_cb);
    }
    
    return true;
}


/*********************************************************************************
 * internal static  implementaiton
 *********************************************************************************/
bool camera_dsc_option_exit(menuview_t *menuview)
{
    camera_dsc_option_blending_delete();
    //delete main menu
    camera_dsc_option_deinit();

    //exit option_ui
    camera_dsc_widget_option_exit(menuview, NULL);

    return true;
}

int camera_dsc_option_deinit(void)
{
    //delete menu
    if (g_cam_dsc_option != NULL)
    {
        //create menu
        menu_delete(g_cam_dsc_option);
        g_cam_dsc_option = NULL;
    }

    return true;
}
static int _dsc_load_item_resource(resgroup_resource_t* res_group, menuitem_style_t* menuitem_style, \
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
menu_t* camera_dsc_option_init(void)
{
    scene_t * scene;
    resgroup_resource_t * res_group;

#pragma __PRQA_IGNORE_START__
    menuitem_t menu_items[ARRAY_SIZE(s_dsc_option_style_items)] = {{0}};
#pragma __PRQA_IGNORE_END__
    int real_items = 0;

    //initial menu
    if (g_cam_dsc_option == NULL)
    {
        scene = sty_get_app_scene();
        if ( NULL == scene )
        {
            print_err("get app scene failed!");
            return NULL;
        }

        //load advanced menu resource group
        res_group = (resgroup_resource_t *)get_scene_child(scene, DSC_STRGROUP_OPTION_MAIN);
        if(NULL == res_group)
        {
            print_err("get resource group REG_ADVANCED");
            return NULL;
        }
        g_cam_dsc_opt_dot = (picture_resource_t *) get_scene_child(scene, DSC_PIC_OPT_DOT);
        if(NULL == g_cam_dsc_opt_dot)
        {
            print_err("get resource DOT");
            return NULL;
        }
        if(_check_dsc_option_visible() == false)
        {
            return NULL;
        }

        /* load menu item string and pictrure resource */
        real_items = _dsc_load_item_resource(res_group, s_dsc_option_style_items, menu_items,\
        s_visible_menu_real_index, ARRAY_SIZE(s_dsc_option_style_items));

        /* load other string resourc */
        //_load_str_res(res_group, s_str_res_table, ARRAY_SIZE(s_str_res_table));

        //create menu
        g_cam_dsc_option = menu_create();
        if(NULL == g_cam_dsc_option)
        {
            print_err("create menu failed!");
            return NULL;
        }

        //init menu items
        menu_append_items( g_cam_dsc_option, menu_items, real_items);

        menu_set_cur_index( g_cam_dsc_option, 0 );
    }
    return g_cam_dsc_option;
}

static bool _check_dsc_option_visible( void )
{
    int i = 0;

    for (i = 0; i <CAM_DSC_CFG_VISIBLE_TYPE_TOTAL; i++)
    {
        if(g_camera_dsc_option_visible_cfgs[i].value == 0xffff)
        {
            s_dsc_option_style_items[i].visible = false;
        }
    }
    
    return true;
}
static bool _dsc_adjust_set_switch_info(adjust_switch_init_t *adjust_switch)
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
    group = (resgroup_resource_t *) get_scene_child(scene, DSC_STRGROUP_OPTION_MAIN);
    if (group == NULL)
    {
        return false;
    }    
    str_res = (string_resource_t *) get_resgroup_child(group, STR_FACE_IDENTIFY);
    if (str_res == NULL)
    {
        return false;
    }
    str_id = str_res->str_id;
    adjust_switch->str_title_id = (int) str_id;

    /*
    1，adjust_switch->on_off_status 0==open 1==close
    如果保存的是打开下次adjust需要显示关闭为高亮
    2，g_camera_dsc_cfgs[CAM_DSC_CFG_TYPE_FACE_IDENTIFY].value 0,close 1,open
    3，mmm cmd 0,close 1,open
    (进入dsc默认为0,如果config为1需要设置一次)
    注意mmm如果已经为0不能再设0，如果为1不能再设1，否则会死机
    4，回归测试修改了，显示为用户保存的同动静检测
    */
    if (g_camera_dsc_cfgs[CAM_DSC_CFG_TYPE_FACE_IDENTIFY].value == 0)
    {
        adjust_switch->on_off_status = ADJUST_OFF;
    }
    else
    {
        adjust_switch->on_off_status = ADJUST_ON;
    }
    print_dbg("set=%d\n",adjust_switch->on_off_status);
    return true;
}
static void _dsc_adjust_face_identify_callback(adjust_t *adjust, adjust_signal_e signal)
{
    adj_switch_mode_e on_off;
    unsigned int param;
    switch (signal)
    {
        case WIDGET_SIG_ADJUST_OK:
        print_dbg("face dect ok\n");
        adjust_get_switch_status(&on_off);
        print_dbg("get=%d\n",on_off);
        if(on_off == ADJUST_ON)
        {
            param = 1;
        }
        else
        {
            param = 0;
        }
        if(param != (unsigned int)g_camera_dsc_cfgs[CAM_DSC_CFG_TYPE_FACE_IDENTIFY].value)
        {
            camera_dsc_ctr_set_face_detect(param);
            camera_dsc_save_parameter(CAM_DSC_PARA_FACE_IDENTIFY, (int) param);
        }
        camera_dsc_adjust_blending_delete();
        adjust_delete(adjust);
        g_camera_scene_dsc->adjust = NULL;

        break;
        case WIDGET_SIG_ADJUST_CANCEL:
        print_dbg("face dect esc\n");
        camera_dsc_adjust_blending_delete();
        adjust_delete(adjust);
        g_camera_scene_dsc->adjust = NULL;

        break;

        default:
        break;
    }
}

static bool _dsc_adjust_brightness(adjust_textdigit_init_t *adjust_digit)
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
    group = (resgroup_resource_t *) get_scene_child(scene, DSC_STRGROUP_OPTION_MAIN);
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
    adjust_digit->cur_val = (g_camera_dsc_cfgs[CAM_DSC_CFG_TYPE_BRIGHTNESS].value);
    adjust_digit->step = 1;
    return true;
}
static void _dsc_adjust_brightness_callback(adjust_t *adjust, adjust_signal_e signal)
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
            if (g_camera_dsc_cfgs[CAM_DSC_CFG_TYPE_BRIGHTNESS].value != value)
            {
                camera_dsc_save_parameter(CAM_DSC_PARA_BRIGHTNESS, value);
                camera_dsc_change_icon(CAM_ICON_BRIGHTNESS, (unsigned int) value);
            }
        }
        print_dbg("getvalue%d\n", value);
        camera_dsc_adjust_blending_delete();
        adjust_delete(adjust);
        g_camera_scene_dsc->adjust = NULL;
        break;
        case WIDGET_SIG_ADJUST_CANCEL:
        print_dbg("WIDGET_SIG_ADJUST_CANCEL\n");
        ret = adjust_get_current_val(&value);

        if (ret == true)
        {
            if (g_camera_dsc_cfgs[CAM_DSC_CFG_TYPE_BRIGHTNESS].value != value)
            {
                camera_dsc_ctr_set_brightness((unsigned int) g_camera_dsc_cfgs[CAM_DSC_CFG_TYPE_BRIGHTNESS].value);
            }
        }
        camera_dsc_adjust_blending_delete();
        adjust_delete(adjust);
        g_camera_scene_dsc->adjust = NULL;
        break;

        case WIDGET_SIG_ADJUST_UP:
        print_dbg("WIDGET_SIG_ADJUST_UP\n");
        ret = adjust_get_current_val(&value);
        if (ret == true)
        {
            camera_dsc_ctr_set_brightness((unsigned int) value);
        }
        break;

        case WIDGET_SIG_ADJUST_DOWN:
        print_dbg("WIDGET_SIG_ADJUST_DOWN\n");
        ret = adjust_get_current_val(&value);
        if (ret == true)
        {
            camera_dsc_ctr_set_brightness((unsigned int) value);
        }
        break;

        default:
        break;
    }
}
static bool _dsc_adjust_sefl_time(char **strtempp, adjust_textdigit_init_t *adjust_digit)
{
    scene_t* scene; //!< app scene
    const char* str_idtemp[3];
    resgroup_resource_t *group;
    string_resource_t *str_res[3];
    static char s_strtemp[5][20];

    scene = sty_get_app_scene();
    if (scene == NULL)
    {
        print_err("cann't find app scene!");
        return false;
    }
    //load resource group
    group = (resgroup_resource_t *) get_scene_child(scene, DSC_STRGROUP_OPTION_MAIN);
    if (group == NULL)
    {
        return false;
    }    
    str_res[0] = (string_resource_t *) get_resgroup_child(group, STR_SELF_TIMER);
    if (str_res[0] == NULL)
    {
        return false;
    }    
    str_res[1] = (string_resource_t *) get_resgroup_child(group, STR_CAM_SEC);
    if (str_res[1] == NULL)
    {
        return false;
    }
    str_res[2] = (string_resource_t *) get_resgroup_child(group, STR_CAM_OFF);
    if (str_res[2] == NULL)
    {
        return false;
    }
    str_idtemp[2] = sty_get_string((int) str_res[2]->str_id);
    snprintf((s_strtemp[0]), sizeof(s_strtemp[0]), "%s", str_idtemp[2]);

    str_idtemp[1] = sty_get_string((int) str_res[1]->str_id);
    snprintf((s_strtemp[1]), sizeof(s_strtemp[1]), "5%s", str_idtemp[1]);
    snprintf((s_strtemp[2]), sizeof(s_strtemp[2]), "10%s", str_idtemp[1]);
    snprintf((s_strtemp[3]), sizeof(s_strtemp[3]), "15%s", str_idtemp[1]);
    snprintf((s_strtemp[4]), sizeof(s_strtemp[4]), "20%s", str_idtemp[1]);

    memset(adjust_digit, 0, sizeof(adjust_textdigit_init_t));
    adjust_digit->str_title_id = (int) str_res[0]->str_id;
    adjust_digit->max_val = 4;
    adjust_digit->min_val = 0;
    adjust_digit->cur_val = g_camera_scene_dsc->dsc_config.cfg_adj_self_time;
    adjust_digit->step = 1;
    adjust_digit->str_disp[0] = (char*) s_strtemp[0];
    adjust_digit->str_disp[1] = (char*) s_strtemp[1];
    adjust_digit->str_disp[2] = (char*) s_strtemp[2];
    adjust_digit->str_disp[3] = (char*) s_strtemp[3];
    adjust_digit->str_disp[4] = (char*) s_strtemp[4];
    return true;
}
static void _dsc_adjust_sefl_time_callback(adjust_t *adjust, adjust_signal_e signal)
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
            camera_dsc_save_parameter(CAM_DSC_PARA_SELF_TIMER, value);
            if (value != 0)
            {
                camera_dsc_save_parameter(CAM_DSC_PARA_CONT_SHOOTING, 0);
            }
            camera_dsc_change_icon(CAM_ICON_SELFTIME, (unsigned int)value);
        }
        print_dbg("getvalue%d\n", value);
        camera_dsc_adjust_blending_delete();
        adjust_delete(adjust);
        g_camera_scene_dsc->adjust = NULL;
        //TODO:check if no free g_camera_scene_dsc->adjust will "Accessed address overflow!, bad addr:0x520173e0"
        break;

        case WIDGET_SIG_ADJUST_CANCEL:
        print_dbg("WIDGET_SIG_ADJUST_CANCEL\n");
        camera_dsc_adjust_blending_delete();
        adjust_delete(adjust);
        g_camera_scene_dsc->adjust = NULL;
        break;

        default:
        break;
    }
}
static bool _dsc_adjust_cont_shooting(char **strtempp, adjust_textdigit_init_t *adjust_digit)
{
    scene_t* scene; //!< app scene
    const char* str_idtemp[3];
    resgroup_resource_t *group;
    string_resource_t *str_res[3];
    static char s_strtemp[5][20];

    scene = sty_get_app_scene();
    if (scene == NULL)
    {
        print_err("cann't find app scene!");
        return false;
    }
    //load resource group
    group = (resgroup_resource_t *) get_scene_child(scene, DSC_STRGROUP_OPTION_MAIN);
    if (group == NULL)
    {
        return false;
    }    
    str_res[0] = (string_resource_t *) get_resgroup_child(group, STR_CONT_SHOOTING);
    if (str_res[0] == NULL)
    {
        return false;
    }    
    str_res[1] = (string_resource_t *) get_resgroup_child(group, STR_CAM_SEC);
    if (str_res[1] == NULL)
    {
        return false;
    }        
    str_res[2] = (string_resource_t *) get_resgroup_child(group, STR_CAM_OFF);
    if (str_res[2] == NULL)
    {
        return false;
    }    
    str_idtemp[2] = sty_get_string((int) str_res[2]->str_id);
    snprintf((s_strtemp[0]), sizeof(s_strtemp[0]), "%s", str_idtemp[2]);

    str_idtemp[1] = sty_get_string((int) str_res[1]->str_id);
    snprintf((s_strtemp[1]), sizeof(s_strtemp[1]), "5%s", str_idtemp[1]);
    snprintf((s_strtemp[2]), sizeof(s_strtemp[2]), "10%s", str_idtemp[1]);
    snprintf((s_strtemp[3]), sizeof(s_strtemp[3]), "15%s", str_idtemp[1]);
    snprintf((s_strtemp[4]), sizeof(s_strtemp[4]), "30%s", str_idtemp[1]);

    memset(adjust_digit, 0, sizeof(adjust_textdigit_init_t));
    adjust_digit->str_title_id = (int) str_res[0]->str_id;
    adjust_digit->max_val = 4;
    adjust_digit->min_val = 0;
    if (g_camera_scene_dsc->dsc_config.cfg_adj_cont_shooting == 6)
    {
        adjust_digit->cur_val = 4;
    }
    else
    {
        adjust_digit->cur_val = g_camera_scene_dsc->dsc_config.cfg_adj_cont_shooting;
    }
    adjust_digit->step = 1;
    adjust_digit->str_disp[0] = (char*) s_strtemp[0];
    adjust_digit->str_disp[1] = (char*) s_strtemp[1];
    adjust_digit->str_disp[2] = (char*) s_strtemp[2];
    adjust_digit->str_disp[3] = (char*) s_strtemp[3];
    adjust_digit->str_disp[4] = (char*) s_strtemp[4];
    return true;
}
static void _dsc_adjust_cont_shooting_callback(adjust_t *adjust, adjust_signal_e signal)
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
            /*! 自拍连拍用同一图标 ，时间也是6*5000ms 图标5是空白帧用来闪烁用*/
            if (value == 4)
            {
                value = 6;
            }
            camera_dsc_save_parameter(CAM_DSC_PARA_CONT_SHOOTING, value);
            if (value != 0)
            {
                camera_dsc_save_parameter(CAM_DSC_PARA_SELF_TIMER, 0);
            }

            camera_dsc_change_icon(CAM_ICON_SELFTIME, (unsigned int)value);
        }
        //todo:check Accessed address overflow!, bad addr:0x6812da40 by key esc?ok

        print_dbg("getvalue%d\n", value);
        camera_dsc_adjust_blending_delete();
        adjust_delete(adjust);
        g_camera_scene_dsc->adjust = NULL;
        break;

        case WIDGET_SIG_ADJUST_CANCEL:
        print_dbg("WIDGET_SIG_ADJUST_CANCEL\n");
        camera_dsc_adjust_blending_delete();
        adjust_delete(adjust);
        g_camera_scene_dsc->adjust = NULL;
        break;

        default:
        break;
    }
}
bool camera_dsc_playback(void)
{
    bool ret = false;
    const char* app_name = APP_NAME_PHOTO;// "photo.app";
    int temp_scene = 1;    
    /*需要回放的时候，设配置值拍照=1，录制=2，
    在回放结束后argv启动camera的时候读配置来判断启动场景*/

    if (set_config(CFG_CAM_PLAYBACK_SCENE, (char*)&temp_scene, sizeof(temp_scene)) < 0)
    {
        print_err("set config fail, errno = 0x%x", errno);
        return false;
    }   
    
    print_info(" filepath=%s filename = %s", dsc_filepath, dsc_filename);

    if(g_camera_dsc_cfgs[CAM_DSC_CFG_TYPE_SAVE_PATH].value == CAM_DSC_STORAGE_NONE)
    {
        camera_dsc_dialog_create(CAM_DIALOG_NODISK);
        return false;
    }
    if(g_setpath == false)
    {
        camera_dsc_dialog_create(CAM_DIALOG_SETPATH_FAIL);
        return false;
    }
    camera_dsc_ctr_stop_preview();
    camera_dsc_blending_deinit();
    camera_dsc_msgbox_delete();
    camera_dsc_mmm_close();
    canvas_set_blending_mode(false);

    gui_dc_clear(g_camera_scene_dsc->hdc );                

    if (app_name != NULL)
    {
        msg_apps_t t;
        size_t n_app_name = strlen(app_name);
        size_t n_file = strlen(dsc_filename);
        size_t n_path = strlen(dsc_filepath);
        size_t n_arg = strlen(PHOTO_ARGV_PREVIEW);

        memset(&t, 0, sizeof(t));
        t.type = MSG_CREAT_APP_RESUME;//old use MSG_CREAT_APP;
        strncpy(t.content, app_name,n_app_name);
        strncat(t.content, " ",1);

        strncat(t.content, PHOTO_ARGV_PREVIEW,n_arg);

        //增加路径top限制，告诉photo顶层目录
        strncat(t.content, " ",1);
        strncat(t.content, "\"",1);
        strncat(t.content, dsc_filepath,n_path);
        strncat(t.content, "\"",1);

        strncat(t.content, " ",1);

        strncat(t.content, "\"",1);
        strncat(t.content, dsc_filepath,n_path);
        strncat(t.content, dsc_filename,n_file);
        strncat(t.content, "\"",1);
        printf("t.content=%s", t.content);
        //warning:check array max?
        send_async_msg("manager.app", &t);
        enter_background_mode();
    }
    ret = true;
    print_err("playback out...\n");
    return ret;
}

static inline void _dsc_sub_option_init(menu_t* cur_menu, menuview_t* menuview)
{
    if(cur_menu != NULL)
    {
        menuview_set_menu(menuview, cur_menu);
        menuview_paint(menuview);
    }
    else
    {
        print_err("_dsc_sub_option_init err\n");
    }
}
static int _proc_item_levelone(menuview_t *menuview, menuview_sig_e signal)
{
    menu_t *cur_menu = menuview->menu;
    int cur = menu_get_cur_index(cur_menu);

    print_dbg("cur=%d", cur);
    switch (signal)
    {
        case WIDGET_SIG_MENUVIEW_OK:
        print_dbg(">>>> menu_option: %s clicked", cur_item->text);
        printf("real index=%d\n",s_visible_menu_real_index[cur]);
        switch (s_visible_menu_real_index[cur])
        {
            case CAM_DSC_OPT_PLAYBACK:
            camera_dsc_option_exit(menuview);
            camera_dsc_playback();
            break;

            case CAM_DSC_OPT_BRIGHTNESS:
            {
                adjust_textdigit_init_t adjust_digit;
                if(_dsc_adjust_brightness(&adjust_digit) == false)
                {
                    print_err();
                    break;
                }
                g_camera_scene_dsc->adjust = adjust_create(&adjust_digit, ADJ_STYLE_TEXTDIGIT,
                        _dsc_adjust_brightness_callback);
                camera_dsc_adjust_blending_create_show();

            }
            break;

            case CAM_DSC_OPT_WHITE_BALANCE:
            _dsc_sub_option_init((menu_t *) camera_dsc_option_white_balance_init(), menuview);
            break;

            case CAM_DSC_OPT_EXPOSURE_MODE:
            _dsc_sub_option_init((menu_t *) camera_dsc_option_exposure_mode_init(), menuview);
            break;

            case CAM_DSC_OPT_FLASH_LAMP:
            _dsc_sub_option_init((menu_t *) camera_dsc_option_flash_lamp_init(), menuview);
            break;

            case CAM_DSC_OPT_FACE_IDENTIFY:
            {
                adjust_switch_init_t adjust_switch;
                if (_dsc_adjust_set_switch_info(&adjust_switch) == false)
                {
                    print_err();
                    break;
                }
                g_camera_scene_dsc->adjust = adjust_create(&adjust_switch, ADJ_STYLE_SWITCH,
                        _dsc_adjust_face_identify_callback);
                camera_dsc_adjust_blending_create_show();
                break;
            }

            case CAM_DSC_OPT_STICKER_PHOTO:
            _dsc_sub_option_init((menu_t *) camera_dsc_option_sticker_photo_init(), menuview);
            break;

            case CAM_DSC_OPT_SPECIAL_EFFECT:
            _dsc_sub_option_init((menu_t *) camera_dsc_option_special_effect_init(), menuview);
            break;

            case CAM_DSC_OPT_SAVE_PATH:
            _dsc_sub_option_init((menu_t *) camera_dsc_option_save_path_init(), menuview);
            break;

            case CAM_DSC_OPT_RESOLUTION:
            _dsc_sub_option_init((menu_t *) camera_dsc_option_resolution_init(), menuview);
            break;

            case CAM_DSC_OPT_FREE_SPACE:
            _dsc_sub_option_init((menu_t *) camera_dsc_option_free_space_init(), menuview);
            break;

            case CAM_DSC_OPT_SELF_TIMER:
            {
                adjust_textdigit_init_t adjust_digit;
                char strtempp[5][20];
                if(_dsc_adjust_sefl_time((char **) strtempp, &adjust_digit) == false)
                {
                    print_err();
                    break;
                }
                g_camera_scene_dsc->adjust = adjust_create(&adjust_digit, ADJ_STYLE_TEXTDIGIT,
                        _dsc_adjust_sefl_time_callback);
                camera_dsc_adjust_blending_create_show();
            }
            break;

            case CAM_DSC_OPT_CONT_SHOOTING:
            {
                adjust_textdigit_init_t adjust_digit;
                char strtempp[5][20];
                if(_dsc_adjust_cont_shooting((char **) strtempp, &adjust_digit) == false)
                {
                    print_err();
                    break;
                }
                g_camera_scene_dsc->adjust = adjust_create(&adjust_digit, ADJ_STYLE_TEXTDIGIT,
                        _dsc_adjust_cont_shooting_callback);
                camera_dsc_adjust_blending_create_show();
            }
            break;

            case CAM_DSC_OPT_SHUTTER_SOUND:
            _dsc_sub_option_init((menu_t *) camera_dsc_option_shutter_sound_init(), menuview);
            break;

            case CAM_DSC_OPT_DATE_STAMP:
            _dsc_sub_option_init((menu_t *) camera_dsc_option_date_stamp_init(), menuview);
            break;

            default:
            print_err();
            break;
        }
        break;
        case WIDGET_SIG_MENUVIEW_QUIT:
        // return to parent menu
        camera_dsc_option_exit(menuview);
        break;

        default:
        break;
    }
    return 0;
}

