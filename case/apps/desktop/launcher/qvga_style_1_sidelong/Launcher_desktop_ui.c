/********************************************************************************
 *                              USDK(1100)
 *                             Module: app
 *                 Copyright(c) 2003-2009 Actions Semiconductor,
 *                            All Rights Reserved.
 * File : launcher_desktop_ui.c
 * History:
 *      <author>    <time>           <version >             <desc>
 *      chencm    2009-04-22          v1.0               build this file
 ********************************************************************************/
/*!
 * \file     launcher_desktop_ui.c
 * \brief    desktop scene
 * \author   chencm
 *
 * \version 1.0
 * \date  2009/06/09
 *******************************************************************************/

#include <include_case.h>
#include <include_psp.h>
#include "./../launcher.h"
#include "launcher_desktop_ui.h"
#include "launcher_sty.h"

#define PAGE_NUM 0x05

typedef struct
{
    picture_resource_t *res;
    gui_bitmap_info_t *buf;
} pic_data_t;

static app_map_t app_map[TYPE_COUNT] =
{

    { "music", TYPE_MUSIC },
    { "video", TYPE_VIDEO },
    { "tvin", TYPE_TVIN },
    { "photo", TYPE_PHOTO },
    { "camera", TYPE_CAMERA },
    { "recorder", TYPE_RECORDER },
    { "radio", TYPE_RADIO },
    { "ebook", TYPE_EBOOK },
    { "games", TYPE_GAMES },
    { "tools", TYPE_TOOLS },
    { "browser", TYPE_BROWSER },
    { "setting", TYPE_SETTING },
    { "Test Commont UI", TYPE_TEST_COM },
    { "usb", TYPE_USB },

};

int debug_time;

/* resource for painting */
//desktop bg
static pic_data_t desktop_bg;

//downbar icon display positions
pic_data_t downbar_bg;
pic_data_t downbar_icon_sel_bg;

picture_resource_t *downbar_icon_p1;
picture_resource_t *downbar_icon_p2;
picture_resource_t *downbar_icon_p3;
picture_resource_t *downbar_icon_p4;
picture_resource_t *downbar_icon_p5;

//app name & app no. string display positions
string_resource_t *str_app_name; //init step string display positions
string_resource_t *str_app_no; //init step string display positions
string_resource_t *str_p1_cur_app; //p1-->step1 string display positions
string_resource_t *str_p1_cur_appno;
string_resource_t *str_p1_next_app;
string_resource_t *str_p1_next_appno;
string_resource_t *str_p2_cur_app; //p2-->step2 string display positions
string_resource_t *str_p2_cur_appno;
string_resource_t *str_p2_next_app;
string_resource_t *str_p2_next_appno;

//app icon style name
unsigned int app_icon_l2[TYPE_COUNT] =
{
    PIC_MUSIC_L2, PIC_VIDEO_L2, PIC_TVIN_L2, PIC_PHOTO_L2, PIC_CAMERA_L2, PIC_RECORDER_L2, PIC_RADIO_L2, PIC_EBOOK_L2,
    PIC_GAMES_L2, PIC_TOOLS_L2, PIC_BROWSER_L2, PIC_SETTING_L2, PIC_FLASH_L2, PIC_FLASH_L2
};

pic_data_t app_icon_l2_data[TYPE_COUNT];

unsigned int app_icon_l1[TYPE_COUNT] =
{
    PIC_MUSIC_L1, PIC_VIDEO_L1, PIC_TVIN_L1, PIC_PHOTO_L1, PIC_CAMERA_L1, PIC_RECORDER_L1, PIC_RADIO_L1, PIC_EBOOK_L1,
    PIC_GAMES_L1, PIC_TOOLS_L1, PIC_BROWSER_L1, PIC_SETTING_L1, PIC_FLASH_L1, PIC_FLASH_L1
};

pic_data_t app_icon_l1_data[TYPE_COUNT];

unsigned int app_icon_m[TYPE_COUNT] =
{
    PIC_MUSIC_M, PIC_VIDEO_M, PIC_TVIN_M, PIC_PHOTO_M, PIC_CAMERA_M, PIC_RECORDER_M, PIC_RADIO_M, PIC_EBOOK_M,
    PIC_GAMES_M, PIC_TOOLS_M, PIC_BROWSER_M, PIC_SETTING_M, PIC_FLASH_M, PIC_FLASH_M
};

pic_data_t app_icon_m_data[TYPE_COUNT];

unsigned int app_icon_r1[TYPE_COUNT] =
{
    PIC_MUSIC_R1, PIC_VIDEO_R1, PIC_TVIN_R1, PIC_PHOTO_R1, PIC_CAMERA_R1, PIC_RECORDER_R1, PIC_RADIO_R1, PIC_EBOOK_R1,
    PIC_GAMES_R1, PIC_TOOLS_R1, PIC_BROWSER_R1, PIC_SETTING_R1, PIC_FLASH_R1, PIC_FLASH_R1
};

pic_data_t app_icon_r1_data[TYPE_COUNT];

unsigned int app_icon_r2[TYPE_COUNT] =
{
    PIC_MUSIC_R2, PIC_VIDEO_R2, PIC_TVIN_R2, PIC_PHOTO_R2, PIC_CAMERA_R2, PIC_RECORDER_R2, PIC_RADIO_R2, PIC_EBOOK_R2,
    PIC_GAMES_R2, PIC_TOOLS_R2, PIC_BROWSER_R2, PIC_SETTING_R2, PIC_FLASH_R2, PIC_FLASH_R2
};

pic_data_t app_icon_r2_data[TYPE_COUNT];

//downbar app icon style name
unsigned int downbar_icon[TYPE_COUNT] =
{
    PIC_DBAR_MUSIC, PIC_DBAR_VIDEO, PIC_DBAR_TVIN, PIC_DBAR_PHOTO, PIC_DBAR_CAMERA, PIC_DBAR_RECORDER,
    PIC_DBAR_RADIO, PIC_DBAR_EBOOK, PIC_DBAR_GAMES, PIC_DBAR_TOOLS, PIC_DBAR_BROWSER, PIC_DBAR_SETTING,
    PIC_DBAR_FLASH, PIC_DBAR_FLASH
};

//the headbar_appicon will be display on the NORMAL type headbar.
unsigned int headbar_appicon[TYPE_COUNT] =
{
    PIC_DBAR_MUSIC, PIC_DBAR_VIDEO, PIC_DBAR_TVIN, PIC_DBAR_PHOTO, PIC_DBAR_CAMERA, PIC_DBAR_RECORDER,
    PIC_DBAR_RADIO, PIC_DBAR_EBOOK, PIC_HBAR_GAME, PIC_HBAR_TOOL, PIC_DBAR_BROWSER, PIC_DBAR_SETTING,
    PIC_DBAR_FLASH, PIC_DBAR_FLASH
};

picture_resource_t *downbar_icon_res[TYPE_COUNT];

unsigned int downbar_icon_sel[TYPE_COUNT] =
{
    PIC_DBAR_MUSICH, PIC_DBAR_VIDEOH, PIC_DBAR_TVINH, PIC_DBAR_PHOTOH, PIC_DBAR_CAMERAH, PIC_DBAR_RECORDERH,
    PIC_DBAR_RADIOH, PIC_DBAR_EBOOKH, PIC_DBAR_GAMESH, PIC_DBAR_TOOLSH, PIC_DBAR_BROWSERH, PIC_DBAR_SETTINGH,
    PIC_DBAR_FLASHH, PIC_DBAR_FLASHH
};

picture_resource_t *downbar_icon_sel_res[TYPE_COUNT];

//page_resgroup
//pig_page1, pig_page2, pig_page3, pig_page4, pig_page5
unsigned int page_l2[PAGE_NUM] =
{ PIC_PAGE1_L2, PIC_PAGE2_L2, PIC_PAGE3_L2, PIC_PAGE4_L2, PIC_PAGE5_L2 };

pic_data_t page_l2_data[PAGE_NUM];

unsigned int page_l1[PAGE_NUM] =
{ PIC_PAGE1_L1, PIC_PAGE2_L1, PIC_PAGE3_L1, PIC_PAGE4_L1, PIC_PAGE5_L1 };

pic_data_t page_l1_data[PAGE_NUM];

unsigned int page_m[PAGE_NUM] =
{ PIC_PAGE1_M, PIC_PAGE2_M, PIC_PAGE3_M, PIC_PAGE4_M, PIC_PAGE5_M };

pic_data_t page_m_data[PAGE_NUM];

unsigned int page_r1[PAGE_NUM] =
{ PIC_PAGE1_R1, PIC_PAGE2_R1, PIC_PAGE3_R1, PIC_PAGE4_R1, PIC_PAGE5_R1 };

pic_data_t page_r1_data[PAGE_NUM];

unsigned int page_r2[PAGE_NUM] =
{ PIC_PAGE1_R2, PIC_PAGE2_R2, PIC_PAGE3_R2, PIC_PAGE4_R2, PIC_PAGE5_R2 };

pic_data_t page_r2_data[PAGE_NUM];

/*******************************************************************************/
static app_e _get_app_macro(char * app_name);
static void _draw_app_name(animation_step_e step, scene_desktop_t *data);
static void _draw_pic(int hdc, pic_data_t page_data);
static void _draw_app_page(animation_step_e step, scene_desktop_t *data);
static void _draw_downbar(animation_step_e step, scene_desktop_t *data);
/*******************************************************************************/

/*get app icon, it will be displayed on the hearbar*/
picture_resource_t * desktop_get_headbar_appicon(appinfo_list_t *data)
{
    scene_t* scene;
    app_e app_id;
    picture_resource_t * on_headbar_icon;
    
    //get root resource
    scene = sty_get_app_scene();
    
    app_id = _get_app_macro(data->value[DKEY_NAME]);
    on_headbar_icon = (picture_resource_t *) get_scene_child(scene, headbar_appicon[app_id % TYPE_COUNT]);
    return on_headbar_icon;
}


/*load desktop style resource from the desktop scene*/
bool desktop_load_style(void)
{
    scene_t* scene;
    resgroup_resource_t *group;
    int i = 0;

    //get root resource
    scene = sty_get_app_scene();
    if (scene == NULL)
    {
        print_err("cann't find desktop scene!");
        return false;
    }

    //load string resource for app name & app number, string display positions
    desktop_bg.res = (picture_resource_t *) get_scene_child(scene, PIC_DESKTOP_BG);
    str_app_name = (string_resource_t *) get_scene_child(scene, STR_APP_NAME);
    str_app_no = (string_resource_t *) get_scene_child(scene, STR_APP_NO);
    str_p1_cur_app = (string_resource_t *) get_scene_child(scene, STR_P1_CUR_APP);
    str_p1_cur_appno = (string_resource_t *) get_scene_child(scene, STR_P1_CUR_APPNO);
    str_p1_next_app = (string_resource_t *) get_scene_child(scene, STR_P1_NEXT_APP);
    str_p1_next_appno = (string_resource_t *) get_scene_child(scene, STR_P1_NEXT_APPNO);
    str_p2_cur_app = (string_resource_t *) get_scene_child(scene, STR_P2_CUR_APP);
    str_p2_cur_appno = (string_resource_t *) get_scene_child(scene, STR_P2_CUR_APPNO);
    str_p2_next_app = (string_resource_t *) get_scene_child(scene, STR_P2_NEXT_APP);
    str_p2_next_appno = (string_resource_t *) get_scene_child(scene, STR_P2_NEXT_APPNO);

    //load APP_ICON_RESGROUP group
    group = (resgroup_resource_t *) get_scene_child(scene, APP_ICON_RESGROUP);
    for (i = 0; i < TYPE_COUNT; i++)
    {
        app_icon_l2_data[i].res = (picture_resource_t *) get_resgroup_child(group, app_icon_l2[i]);
        app_icon_l1_data[i].res = (picture_resource_t *) get_resgroup_child(group, app_icon_l1[i]);
        app_icon_m_data[i].res = (picture_resource_t *) get_resgroup_child(group, app_icon_m[i]);
        app_icon_r1_data[i].res = (picture_resource_t *) get_resgroup_child(group, app_icon_r1[i]);
        app_icon_r2_data[i].res = (picture_resource_t *) get_resgroup_child(group, app_icon_r2[i]);
    }

    //load DOWNBAR_RESGROUP group
    group = (resgroup_resource_t *) get_scene_child(scene, DOWNBAR_RESGROUP);
    downbar_bg.res = (picture_resource_t *) get_resgroup_child(group, PIC_DBAR_BG);
    downbar_icon_sel_bg.res = (picture_resource_t *) get_resgroup_child(group, PIC_DBAR_SEL_BG);
    downbar_icon_p1 = (picture_resource_t *) get_resgroup_child(group, PIC_DBAR_ICON_P1);
    downbar_icon_p2 = (picture_resource_t *) get_resgroup_child(group, PIC_DBAR_ICON_P2);
    downbar_icon_p3 = (picture_resource_t *) get_resgroup_child(group, PIC_DBAR_ICON_P3);
    downbar_icon_p4 = (picture_resource_t *) get_resgroup_child(group, PIC_DBAR_ICON_P4);
    downbar_icon_p5 = (picture_resource_t *) get_resgroup_child(group, PIC_DBAR_ICON_P5);
    for (i = 0; i < TYPE_COUNT; i++)
    {
        downbar_icon_res[i] = (picture_resource_t *) get_resgroup_child(group, downbar_icon[i]);
        downbar_icon_sel_res[i] = (picture_resource_t *) get_resgroup_child(group, downbar_icon_sel[i]);
    }

    //load PAGE_RESGROUP group
    group = (resgroup_resource_t *) get_scene_child(scene, PAGE_RESGROUP);
    for (i = 0; i < PAGE_NUM; i++)
    {
        page_l2_data[i].res = (picture_resource_t *) get_resgroup_child(group, page_l2[i]);
        page_l1_data[i].res = (picture_resource_t *) get_resgroup_child(group, page_l1[i]);
        page_m_data[i].res = (picture_resource_t *) get_resgroup_child(group, page_m[i]);
        page_r1_data[i].res = (picture_resource_t *) get_resgroup_child(group, page_r1[i]);
        page_r2_data[i].res = (picture_resource_t *) get_resgroup_child(group, page_r2[i]);
    }

    //get current background picture info & buffer.
    desktop_bg.buf = get_user_bg_pic(BG_LAUNCHER);
    if ((NULL == desktop_bg.buf) && (NULL != desktop_bg.res))
    {
        /*获取用户自定义背景图失败，使用ui-editor上添加的默认图*/
        desktop_bg.buf = sty_create_bitmap((int)desktop_bg.res->pic_id);//buffer desktop bg to SDRAM
    }

    if (NULL == desktop_bg.buf)
    {
        print_err("scene background picture load false!");
        return false;
    }

    return true;
}

//save some desktop resource data to SDRAM, for improve the speed of change application.
bool desktop_buffer_resource(void)
{
    int i;
    bool ret = true;

    downbar_bg.buf = sty_create_bitmap((int)downbar_bg.res->pic_id);//buffer desktop bg to SDRAM
    downbar_icon_sel_bg.buf = sty_create_bitmap((int)downbar_icon_sel_bg.res->pic_id);//buffer desktop bg to SDRAM
    if ((NULL == downbar_bg.buf) || (NULL == downbar_icon_sel_bg.buf))
    {
        ret = false;
    }

    // ===buffer page resource to SDRAM
    for (i = 0; i < PAGE_NUM; i++)
    {
        page_l2_data[i].buf = sty_create_bitmap((int)page_l2_data[i].res->pic_id);
        page_l1_data[i].buf = sty_create_bitmap((int)page_l1_data[i].res->pic_id);
        page_m_data[i].buf = sty_create_bitmap((int)page_m_data[i].res->pic_id);
        page_r1_data[i].buf = sty_create_bitmap((int)page_r1_data[i].res->pic_id);
        page_r2_data[i].buf = sty_create_bitmap((int)page_r2_data[i].res->pic_id);

    }
    if ((page_l2_data[i - 1].buf) && (page_l1_data[i - 1].buf) && (page_m_data[i - 1].buf) && (page_r1_data[i - 1].buf)
            && (page_r2_data[i - 1].buf))
    {
        ret = true;
    }
    else
    {
        ret = false;
    }

    // ===have no enough buffer to malloc app icon resource 090617!
    //    for( i = 0; i < TYPE_COUNT; i++ )
    //    {
    //        print_err("---------------------------%d\n",i);
    //        app_icon_l2_data[i].buf = sty_create_bitmap((int)app_icon_l2_data[i].res->pic_id);
    //        app_icon_l1_data[i].buf = sty_create_bitmap((int)app_icon_l1_data[i].res->pic_id);
    //        app_icon_m_data[i].buf = sty_create_bitmap((int)app_icon_m_data[i].res->pic_id);
    //        app_icon_r1_data[i].buf = sty_create_bitmap((int)app_icon_r1_data[i].res->pic_id);
    //        app_icon_r2_data[i].buf = sty_create_bitmap((int)app_icon_r2_data[i].res->pic_id);
    //    }
    //    if( (app_icon_l2_data[i - 1].buf) && (app_icon_l1_data[i - 1].buf) && (app_icon_m_data[i - 1].buf)
    //            && (app_icon_r1_data[i - 1].buf) && (app_icon_r2_data[i - 1].buf) )
    //        return true;
    //    else
    //        return false;

    return ret;
}

//release the created buffer by "desktop_buffer_resource()" from SDRAM.
bool desktop_release_resource(void)
{
    int i;
    if (NULL != desktop_bg.buf)
    {
        sty_delete_bitmap(desktop_bg.buf);
        desktop_bg.buf = NULL;
    }

    if (NULL != downbar_bg.buf)
    {
        sty_delete_bitmap(downbar_bg.buf);
        downbar_bg.buf = NULL;
    }

    if (NULL != downbar_icon_sel_bg.buf)
    {
        sty_delete_bitmap(downbar_icon_sel_bg.buf);
        downbar_icon_sel_bg.buf = NULL;
    }

    // release page resource
    for (i = 0; i < PAGE_NUM; i++)
    {
        if (NULL != page_l2_data[i].buf)
        {
            sty_delete_bitmap(page_l2_data[i].buf);
            page_l2_data[i].buf = NULL;
        }
        if (NULL != page_l1_data[i].buf)
        {
            sty_delete_bitmap(page_l1_data[i].buf);
            page_l1_data[i].buf = NULL;
        }
        if (NULL != page_m_data[i].buf)
        {
            sty_delete_bitmap(page_m_data[i].buf);
            page_m_data[i].buf = NULL;
        }
        if (NULL != page_r1_data[i].buf)
        {
            sty_delete_bitmap(page_r1_data[i].buf);
            page_r1_data[i].buf = NULL;
        }
        if (NULL != page_r2_data[i].buf)
        {
            sty_delete_bitmap(page_r2_data[i].buf);
            page_r2_data[i].buf = NULL;
        }
    }

    // release app icon resource
    //    for( i = 0; i < TYPE_COUNT; i++ )
    //    {
    //        if( app_icon_l2_data[i].buf )
    //        {
    //            sty_delete_bitmap(app_icon_l2_data[i].buf);
    //            app_icon_l2_data[i].buf = NULL;
    //        }
    //        if( app_icon_l1_data[i].buf )
    //        {
    //            sty_delete_bitmap(app_icon_l1_data[i].buf);
    //            app_icon_l1_data[i].buf = NULL;
    //        }
    //        if( app_icon_m_data[i].buf )
    //        {
    //            sty_delete_bitmap(app_icon_m_data[i].buf);
    //            app_icon_m_data[i].buf = NULL;
    //        }
    //        if( app_icon_r1_data[i].buf )
    //        {
    //            sty_delete_bitmap(app_icon_r1_data[i].buf);
    //            app_icon_r1_data[i].buf = NULL;
    //        }
    //        if( app_icon_r2_data[i].buf )
    //        {
    //            sty_delete_bitmap(app_icon_r2_data[i].buf);
    //            app_icon_r2_data[i].buf = NULL;
    //        }
    //    }
    return true;
}

static app_e _get_app_macro(char * app_name)
{
    int i;

    for (i = 0; i < TYPE_COUNT; i++)
    {
        if (0 == strcasecmp(app_map[i].app_name, app_name))
        {
            return app_map[i].app_type;
        }
    }
    //can not find the app offset,print error and return the first app offset(TYPE_MUSIC).
    print_err();
    return TYPE_COUNT;
}

static void _draw_app_name(animation_step_e step, scene_desktop_t *data)
{
    appinfo_list_t *cur_app = NULL;
    char app_no[11] =
    { 0 };
    int alpha_bak;
    //获取dc
    int hdc = gui_dc_get(g_scene_desktop->win_id);

    switch (step)
    {
        case STEP_INIT:
        case STEP_3:
        {
            sprintf(app_no, "%d", (data->cur_desktop->app_index + 1));

            sty_draw_string_res_ext(hdc, str_app_no, (char *) &app_no); //draw cur app no
            sty_draw_string_res_ext(hdc, str_app_name, data->cur_desktop->value[DKEY_NAME_LANG]); //draw cur app name

        }
        break;

        case STEP_1:
        {
            //根据动画显示方向（即按键），获取当前焦点应用的信息
            if (data->display_mode == PREV_ANIMATION)
            {
                cur_app = data->cur_desktop->next;

                //draw cur & next app number,name
                sprintf(app_no, "%d", (data->cur_desktop->app_index + 1));
                alpha_bak = gui_dc_get_alpha(hdc);
                gui_dc_set_alpha(hdc, (255 * 1) / 3);
                sty_draw_string_res_ext(hdc, str_p2_cur_appno, (char *) &app_no); //draw next app no
                //draw next app name
                sty_draw_string_res_ext(hdc, str_p2_cur_app, data->cur_desktop->value[DKEY_NAME_LANG]);
                gui_dc_set_alpha(hdc, (255 * 2) / 3);

                sprintf(app_no, "%d", (cur_app->app_index + 1));
                sty_draw_string_res_ext(hdc, str_p2_next_appno, (char *) &app_no); //draw cur app no
                sty_draw_string_res_ext(hdc, str_p2_next_app, cur_app->value[DKEY_NAME_LANG]);//draw cur app name
                gui_dc_set_alpha(hdc, alpha_bak);
            }
            else if (data->display_mode == NEXT_ANIMATION)
            {
                cur_app = data->cur_desktop->prev;

                //draw cur & next app number,name
                sprintf(app_no, "%d", (cur_app->app_index + 1));
                alpha_bak = gui_dc_get_alpha(hdc);
                gui_dc_set_alpha(hdc, (255 * 2) / 3);
                sty_draw_string_res_ext(hdc, str_p1_cur_appno, (char *) &app_no); //draw next app no
                sty_draw_string_res_ext(hdc, str_p1_cur_app, cur_app->value[DKEY_NAME_LANG]); //draw next app name
                gui_dc_set_alpha(hdc, (255 * 1) / 3);
                sprintf(app_no, "%d", (data->cur_desktop->app_index + 1));
                sty_draw_string_res_ext(hdc, str_p1_next_appno, (char *) &app_no); //draw cur app no
                //draw cur app name
                sty_draw_string_res_ext(hdc, str_p1_next_app, data->cur_desktop->value[DKEY_NAME_LANG]);
                gui_dc_set_alpha(hdc, alpha_bak);
            }
            else
            {
                print_err("the data->display_mode is error value.");
                return;
            }

        }
        break;

        case STEP_2:
        {
            //根据动画显示方向（即按键），获取当前焦点应用的信息
            if (data->display_mode == PREV_ANIMATION)
            {
                cur_app = data->cur_desktop->next;

                //draw cur & next app number,name
                sprintf(app_no, "%d", (data->cur_desktop->app_index + 1));
                alpha_bak = gui_dc_get_alpha(hdc);
                gui_dc_set_alpha(hdc, (255 * 2) / 3);
                sty_draw_string_res_ext(hdc, str_p1_cur_appno, (char *) &app_no); //draw next app no
                //draw next app name
                sty_draw_string_res_ext(hdc, str_p1_cur_app, data->cur_desktop->value[DKEY_NAME_LANG]);
                gui_dc_set_alpha(hdc, (255 * 1) / 3);

                sprintf(app_no, "%d", (cur_app->app_index + 1));
                sty_draw_string_res_ext(hdc, str_p1_next_appno, (char *) &app_no); //draw cur app no
                sty_draw_string_res_ext(hdc, str_p1_next_app, cur_app->value[DKEY_NAME_LANG]);//draw cur app name
                gui_dc_set_alpha(hdc, alpha_bak);
            }
            else if (data->display_mode == NEXT_ANIMATION)
            {
                cur_app = data->cur_desktop->prev;

                //draw cur & next app number,name
                sprintf(app_no, "%d", (cur_app->app_index + 1));
                alpha_bak = gui_dc_get_alpha(hdc);
                gui_dc_set_alpha(hdc, (255 * 1) / 3);
                sty_draw_string_res_ext(hdc, str_p2_cur_appno, (char *) &app_no); //draw next app no
                sty_draw_string_res_ext(hdc, str_p2_cur_app, cur_app->value[DKEY_NAME_LANG]); //draw next app name
                gui_dc_set_alpha(hdc, (255 * 2) / 3);
                sprintf(app_no, "%d", (data->cur_desktop->app_index + 1));
                sty_draw_string_res_ext(hdc, str_p2_next_appno, (char *) &app_no); //draw cur app no
                //draw cur app name
                sty_draw_string_res_ext(hdc, str_p2_next_app, data->cur_desktop->value[DKEY_NAME_LANG]);
                gui_dc_set_alpha(hdc, alpha_bak);
            }
            else
            {
                print_err("the data->display_mode is error value.");
                return;
            }
        }
        break;

        default:

        break;
    }
}

static void _draw_pic(int hdc, pic_data_t page_data)
{
    //first draw picture from SDRAM buffer
    if (NULL != page_data.buf)
    {
        sty_draw_bitmap(hdc, page_data.buf, page_data.res->x, page_data.res->y);
    }
    else
    {
        sty_draw_picture_res(hdc, page_data.res); //draw cur app middle page
    }
}

static void _draw_app_page(animation_step_e step, scene_desktop_t *data)
{
    appinfo_list_t *cur_app;
    app_e app_macro;

    //获取dc
    int hdc = gui_dc_get(data->win_id);

    switch (step)
    {
        case STEP_INIT:
        case STEP_3:
        {
            _draw_pic(hdc, page_m_data[data->cur_desktop->app_index % PAGE_NUM]);

            app_macro = _get_app_macro(data->cur_desktop->value[DKEY_NAME]); //draw cur app middle icon
            _draw_pic(hdc, app_icon_m_data[app_macro % TYPE_COUNT]);
        }
        break;

        case STEP_1:
        {
            if (data->display_mode == PREV_ANIMATION)
            {
                cur_app = data->cur_desktop->next; //根据动画显示方向（即按键），获取当前焦点应用的信息

                //turn page from right to left ----step1-draw left1&right2 page,icon
                _draw_pic(hdc, page_l1_data[cur_app->app_index % PAGE_NUM]); //draw left1 page

                app_macro = _get_app_macro(cur_app->value[DKEY_NAME]); //draw left1 app icon
                _draw_pic(hdc, app_icon_l1_data[app_macro % TYPE_COUNT]);

                _draw_pic(hdc, page_r2_data[data->cur_desktop->app_index % PAGE_NUM]); //draw next app right2 page

                app_macro = _get_app_macro(data->cur_desktop->value[DKEY_NAME]); //draw next app right2 icon
                _draw_pic(hdc, app_icon_r2_data[app_macro % TYPE_COUNT]);
            }
            else if (data->display_mode == NEXT_ANIMATION)
            {
                cur_app = data->cur_desktop->prev; //根据动画显示方向（即按键），获取当前焦点应用的信息

                //turn page from left to right ----step1-draw left2&right1 page,icon
                _draw_pic(hdc, page_l2_data[data->cur_desktop->app_index % PAGE_NUM]); //draw next app left2 page

                app_macro = _get_app_macro(data->cur_desktop->value[DKEY_NAME]); //draw next app left2 icon
                _draw_pic(hdc, app_icon_l2_data[app_macro % TYPE_COUNT]);

                _draw_pic(hdc, page_r1_data[cur_app->app_index % PAGE_NUM]); //draw right1 page

                app_macro = _get_app_macro(cur_app->value[DKEY_NAME]); //draw right1 app icon
                _draw_pic(hdc, app_icon_r1_data[app_macro % TYPE_COUNT]);
            }
            else
            {
                print_err();
            }
        }
        break;

        case STEP_2:
        {
            if (data->display_mode == PREV_ANIMATION)
            {
                cur_app = data->cur_desktop->next; //根据动画显示方向（即按键），获取当前焦点应用的信息

                //turn page from right to left ----step2-draw left2&right1 page,icon
                _draw_pic(hdc, page_l2_data[cur_app->app_index % PAGE_NUM]); //draw left2 page

                app_macro = _get_app_macro(cur_app->value[DKEY_NAME]); //draw left2 app icon
                _draw_pic(hdc, app_icon_l2_data[app_macro % TYPE_COUNT]);

                _draw_pic(hdc, page_r1_data[data->cur_desktop->app_index % PAGE_NUM]); //draw next app right1 page

                app_macro = _get_app_macro(data->cur_desktop->value[DKEY_NAME]); //draw next app right1 icon
                _draw_pic(hdc, app_icon_r1_data[app_macro % TYPE_COUNT]);
            }
            else if (data->display_mode == NEXT_ANIMATION)
            {
                cur_app = data->cur_desktop->prev; //根据动画显示方向（即按键），获取当前焦点应用的信息

                //turn page from left to right ----step2-draw left1&right2 page,icon

                _draw_pic(hdc, page_l1_data[data->cur_desktop->app_index % PAGE_NUM]); //draw next app left1 page

                app_macro = _get_app_macro(data->cur_desktop->value[DKEY_NAME]); //draw next app left1 icon
                _draw_pic(hdc, app_icon_l1_data[app_macro % TYPE_COUNT]);

                _draw_pic(hdc, page_r2_data[cur_app->app_index % PAGE_NUM]); //draw right2 page

                app_macro = _get_app_macro(cur_app->value[DKEY_NAME]); //draw right2 app icon
                _draw_pic(hdc, app_icon_r2_data[app_macro % TYPE_COUNT]);
            }
            else
            {
                print_err();
            }
        }
        break;

        default:

        break;
    }
}

static void _draw_dbar_icon(appinfo_list_t *appinfo, int x, int y)
{
    app_e app_type;
    //获取dc
    int hdc = gui_dc_get(g_scene_desktop->win_id);

    app_type = _get_app_macro(appinfo->value[DKEY_NAME]); //draw cur app downbar highlight icon
    sty_draw_picture_id(hdc, (int)downbar_icon_res[app_type % TYPE_COUNT]->pic_id, x, y);
    //_draw_pic(hdc, downbar_icon_sel_bg); //draw downbar highlight background picture
}

static void _draw_dbar_sel_icon(appinfo_list_t *appinfo)
{
    app_e app_type;
    //获取dc
    int hdc = gui_dc_get(g_scene_desktop->win_id);

    app_type = _get_app_macro(appinfo->value[DKEY_NAME]); //draw cur app downbar highlight icon
    sty_draw_picture_id(hdc, (int)downbar_icon_sel_res[app_type % TYPE_COUNT]->pic_id, downbar_icon_sel_bg.res->x,
            downbar_icon_sel_bg.res->y);
}

static void _draw_downbar(animation_step_e step, scene_desktop_t *data)
{

    appinfo_list_t *temp_app, *cur_app;
    int step_width;
    //int alpha_bak;

    //获取dc
    int hdc = gui_dc_get(g_scene_desktop->win_id);

    step_width = (downbar_icon_p2->x - downbar_icon_p1->x) / STEP_COUNT; //计算 每个downbar icon的动画步进宽度
    // draw downbar bg
    _draw_pic(hdc, downbar_bg);

    //准备当前应用以及前后1个应用的icon和前后3个应用的downbar icon
    switch (step)
    {
        case STEP_INIT:
        case STEP_3:
        {
            //draw left2 app downbar icon
            temp_app = data->cur_desktop->prev->prev;
            _draw_dbar_icon(temp_app, downbar_icon_p1->x, downbar_icon_p1->y);
            //draw left1 app downbar icon
            temp_app = data->cur_desktop->prev;
            _draw_dbar_icon(temp_app, downbar_icon_p2->x, downbar_icon_p2->y);

            //draw right1 app downbar icon
            temp_app = data->cur_desktop->next;
            _draw_dbar_icon(temp_app, downbar_icon_p4->x, downbar_icon_p4->y);
            //draw right2 app downbar icon
            temp_app = data->cur_desktop->next->next;
            _draw_dbar_icon(temp_app, downbar_icon_p5->x, downbar_icon_p5->y);

            //draw middle highlight app downbar icon
            _draw_pic(hdc, downbar_icon_sel_bg); //draw downbar highlight background picture
            _draw_dbar_sel_icon(data->cur_desktop);
        }
        break;

        case STEP_1:
        {
            if (data->display_mode == PREV_ANIMATION)
            {
                cur_app = data->cur_desktop->next; //根据动画显示方向（即按键），获取当前焦点应用的信息

                //draw left3 app downbar icon
                temp_app = cur_app->prev->prev->prev;
                _draw_dbar_icon(temp_app, downbar_icon_p1->x - (2 * step_width), downbar_icon_p1->y);

                //draw left2 app downbar icon
                temp_app = cur_app->prev->prev;
                _draw_dbar_icon(temp_app, downbar_icon_p1->x + step_width, downbar_icon_p1->y);
                //draw left1 app downbar icon
                temp_app = cur_app->prev;
                _draw_dbar_icon(temp_app, downbar_icon_p2->x + step_width, downbar_icon_p2->y);

                //draw right1 app downbar icon
                temp_app = cur_app->next;
                _draw_dbar_icon(temp_app, downbar_icon_p4->x + step_width, downbar_icon_p4->y);
                //draw right2 app downbar icon (gui can support: x>screen width)
                temp_app = cur_app->next->next;
                _draw_dbar_icon(temp_app, downbar_icon_p5->x + step_width, downbar_icon_p5->y);

                //draw middle highlight app downbar icon
                //                alpha_bak = gui_dc_get_alpha(hdc);
                //                gui_dc_set_alpha(hdc, 255 * 1 / 3);
                _draw_dbar_icon(cur_app, downbar_icon_p3->x + step_width, downbar_icon_p3->y);
                //                _draw_dbar_sel_icon(data->cur_desktop);
                //                gui_dc_set_alpha(hdc, 255 * 2 / 3);
                //                _draw_dbar_sel_icon(cur_app);
                //                gui_dc_set_alpha(hdc, alpha_bak);

            }
            else if (data->display_mode == NEXT_ANIMATION)
            {
                cur_app = data->cur_desktop->prev; //根据动画显示方向（即按键），获取当前焦点应用的信息

                //draw left2 app downbar icon
                temp_app = cur_app->prev->prev;
                _draw_dbar_icon(temp_app, downbar_icon_p1->x - step_width, downbar_icon_p1->y);
                //draw left1 app downbar icon
                temp_app = cur_app->prev;
                _draw_dbar_icon(temp_app, downbar_icon_p2->x - step_width, downbar_icon_p2->y);

                //draw right1 app downbar icon
                temp_app = cur_app->next;
                _draw_dbar_icon(temp_app, downbar_icon_p4->x - step_width, downbar_icon_p4->y);
                //draw right2 app downbar icon
                temp_app = cur_app->next->next;
                _draw_dbar_icon(temp_app, downbar_icon_p5->x - step_width, downbar_icon_p5->y);

                //draw right3 app downbar icon
                temp_app = cur_app->next->next->next;
                _draw_dbar_icon(temp_app, downbar_icon_p5->x + (2 * step_width), downbar_icon_p5->y);
                _draw_dbar_icon(cur_app, downbar_icon_p3->x - step_width, downbar_icon_p3->y);
            }
            else
            {
                print_err();
            }
            _draw_pic(hdc, downbar_icon_sel_bg); //draw downbar highlight background picture
        }
        break;

        case STEP_2:
        {
            if (data->display_mode == PREV_ANIMATION)
            {
                cur_app = data->cur_desktop->next; //根据动画显示方向（即按键），获取当前焦点应用的信息

                //draw left3 app downbar icon
                temp_app = cur_app->prev->prev->prev;
                _draw_dbar_icon(temp_app, downbar_icon_p1->x - step_width, downbar_icon_p1->y);
                //draw left2 app downbar icon
                temp_app = cur_app->prev->prev;
                _draw_dbar_icon(temp_app, downbar_icon_p1->x + (2 * step_width), downbar_icon_p1->y);
                //draw left1 app downbar icon
                temp_app = cur_app->prev;
                _draw_dbar_icon(temp_app, downbar_icon_p2->x + (2 * step_width), downbar_icon_p2->y);

                //draw right1 app downbar icon
                temp_app = cur_app->next;
                _draw_dbar_icon(temp_app, downbar_icon_p4->x + (2 * step_width), downbar_icon_p4->y);
                //draw right2 app downbar icon
                temp_app = cur_app->next->next;
                _draw_dbar_icon(temp_app, downbar_icon_p5->x + (2 * step_width), downbar_icon_p5->y);
                _draw_dbar_icon(cur_app, downbar_icon_p3->x + (2 * step_width), downbar_icon_p3->y);
            }
            else if (data->display_mode == NEXT_ANIMATION)
            {
                cur_app = data->cur_desktop->prev; //根据动画显示方向（即按键），获取当前焦点应用的信息

                //draw left2 app downbar icon(gui can support: x<0)
                temp_app = cur_app->prev->prev;
                _draw_dbar_icon(temp_app, downbar_icon_p1->x - (2 * step_width), downbar_icon_p1->y);
                //draw left1 app downbar icon
                temp_app = cur_app->prev;
                _draw_dbar_icon(temp_app, downbar_icon_p2->x - (2 * step_width), downbar_icon_p2->y);

                //draw right1 app downbar icon
                temp_app = cur_app->next;
                _draw_dbar_icon(temp_app, downbar_icon_p4->x - (2 * step_width), downbar_icon_p4->y);
                //draw right2 app downbar icon
                temp_app = cur_app->next->next;
                _draw_dbar_icon(temp_app, downbar_icon_p5->x - (2 * step_width), downbar_icon_p5->y);

                //draw right3 app downbar icon
                temp_app = cur_app->next->next->next;
                _draw_dbar_icon(temp_app, downbar_icon_p5->x + step_width, downbar_icon_p5->y);
                _draw_dbar_icon(cur_app, downbar_icon_p3->x - (2 * step_width), downbar_icon_p3->y);
            }
            else
            {
                print_err();
            }
            _draw_pic(hdc, downbar_icon_sel_bg); //draw downbar highlight background picture
        }
        break;

        default:

        break;
    }
}

void show_next_desktop(scene_desktop_t *data)
{
    animation_step_e display_step = STEP_INIT;
    //DUMP_TIME();
    for (display_step = STEP_1; display_step < STEP_COUNT; display_step++)
    {
        show_desktop_bg();

        _draw_app_name(display_step, data);
        _draw_app_page(display_step, data);
        _draw_downbar(display_step, data);
        gui_screen_update();
        //TODO: delay some ms for animation
    }
    //DUMP_TIME();
}

void show_child_desktop(scene_desktop_t *data)
{

}

void show_desktop_bg(void)
{

    //获取dc
    int hdc = gui_dc_get(g_scene_desktop->win_id);
    if (NULL != desktop_bg.buf)
    {
        sty_draw_bitmap(hdc, desktop_bg.buf, desktop_bg.res->x, desktop_bg.res->y);
    }
    else
    {
        sty_draw_picture_res(hdc, desktop_bg.res);
    }
}

void show_current_desktop(scene_desktop_t *data)
{
    show_desktop_bg();

    _draw_app_name(STEP_INIT, data);
    _draw_app_page(STEP_INIT, data);
    _draw_downbar(STEP_INIT, data);
}
