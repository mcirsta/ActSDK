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

//app name & app no. string display positions
string_resource_t *str_app_name; //init step string display positions
string_resource_t *str_app_name_p1; //p1-->step1 string display positions
string_resource_t *str_app_name_p2;
string_resource_t *str_app_name_p3;

//app icon style name
unsigned int app_icon1[TYPE_COUNT] =
{
    PIC_MUSIC_1, PIC_VIDEO_1, PIC_TVIN_1, PIC_PHOTO_1, PIC_CAMERA_1, PIC_RECORDER_1, PIC_RADIO_1, PIC_EBOOK_1,
    PIC_GAMES_1, PIC_TOOLS_1, PIC_BROWSER_1, PIC_SETTING_1, PIC_SETTING_1, PIC_SETTING_1
};

pic_data_t app_icon1_data[TYPE_COUNT];

unsigned int app_icon2[TYPE_COUNT] =
{
    PIC_MUSIC_2, PIC_VIDEO_2, PIC_TVIN_2, PIC_PHOTO_2, PIC_CAMERA_2, PIC_RECORDER_2, PIC_RADIO_2, PIC_EBOOK_2,
    PIC_GAMES_2, PIC_TOOLS_2, PIC_BROWSER_2, PIC_SETTING_2, PIC_SETTING_2, PIC_SETTING_2
};

pic_data_t app_icon2_data[TYPE_COUNT];

unsigned int app_icon3[TYPE_COUNT] =
{
    PIC_MUSIC_3, PIC_VIDEO_3, PIC_TVIN_3, PIC_PHOTO_3, PIC_CAMERA_3, PIC_RECORDER_3, PIC_RADIO_3, PIC_EBOOK_3,
    PIC_GAMES_3, PIC_TOOLS_3, PIC_BROWSER_3, PIC_SETTING_3, PIC_SETTING_3, PIC_SETTING_3
};

pic_data_t app_icon3_data[TYPE_COUNT];

unsigned int app_icon4[TYPE_COUNT] =
{
    PIC_MUSIC_4, PIC_VIDEO_4, PIC_TVIN_4, PIC_PHOTO_4, PIC_CAMERA_4, PIC_RECORDER_4, PIC_RADIO_4, PIC_EBOOK_4,
    PIC_GAMES_4, PIC_TOOLS_4, PIC_BROWSER_4, PIC_SETTING_4, PIC_SETTING_4, PIC_SETTING_4
};

pic_data_t app_icon4_data[TYPE_COUNT];

//picture_resource_t * app_icon_pos[ROW_NUM][MAX_IN_ROW] =
//{ NULL };

//the headbar_appicon will be display on the NORMAL type headbar.
unsigned int headbar_appicon[TYPE_COUNT] =
{
    PIC_MUSIC_4, PIC_VIDEO_4, PIC_TVIN_4, PIC_PHOTO_4, PIC_CAMERA_4, PIC_RECORDER_4, PIC_RADIO_4, PIC_EBOOK_4,
    PIC_HBAR_GAME, PIC_HBAR_TOOL, PIC_BROWSER_4, PIC_SETTING_4, PIC_SETTING_4, PIC_SETTING_4
};
/*******************************************************************************/
static app_e _get_app_macro(char * app_name);
//static bool _erase_region(int hdc, gui_bitmap_info_t * bmp, picture_resource_t *pic);
static void _draw_pic(int hdc, pic_data_t page_data, int x, int y);
static void _draw_app_name(animation_step_e step, scene_desktop_t *data);
static void _draw_app_icon(animation_step_e step, scene_desktop_t *data);
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

    //load picture & string resource for app icon & app name display positions
    str_app_name = (string_resource_t *) get_scene_child(scene, STR_APP_NAME);
    str_app_name_p1 = (string_resource_t *) get_scene_child(scene, STR_APP_NAME_P1);
    str_app_name_p2 = (string_resource_t *) get_scene_child(scene, STR_APP_NAME_P2);
    str_app_name_p3 = (string_resource_t *) get_scene_child(scene, STR_APP_NAME_P3);

    group = (resgroup_resource_t *) get_scene_child(scene, APP_ICON_RESGROUP);
    desktop_bg.res = (picture_resource_t *) get_resgroup_child(group, PIC_DESKTOP_BG);
    for (i = 0; i < TYPE_COUNT; i++)
    {
        app_icon1_data[i].res = (picture_resource_t *) get_resgroup_child(group, app_icon1[i]);
        app_icon2_data[i].res = (picture_resource_t *) get_resgroup_child(group, app_icon2[i]);
        app_icon3_data[i].res = (picture_resource_t *) get_resgroup_child(group, app_icon3[i]);
        app_icon4_data[i].res = (picture_resource_t *) get_resgroup_child(group, app_icon4[i]);
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

    // === buffer app icon resource
    for (i = 0; i < TYPE_COUNT; i++)
    {
        app_icon1_data[i].buf = sty_create_bitmap((int)app_icon1_data[i].res->pic_id);
        app_icon2_data[i].buf = sty_create_bitmap((int)app_icon2_data[i].res->pic_id);
        app_icon3_data[i].buf = sty_create_bitmap((int)app_icon3_data[i].res->pic_id);
        app_icon4_data[i].buf = sty_create_bitmap((int)app_icon4_data[i].res->pic_id);
    }
    if ((app_icon1_data[i - 1].buf) && (app_icon2_data[i - 1].buf) && (app_icon3_data[i - 1].buf) && (app_icon4_data[i
            - 1].buf))
    {
        ret = true;
    }
    else
    {
        ret = false;
    }

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

    // release app icon resource
    for (i = 0; i < TYPE_COUNT; i++)
    {
        if (NULL != app_icon1_data[i].buf)
        {
            sty_delete_bitmap(app_icon1_data[i].buf);
            app_icon1_data[i].buf = NULL;
        }
        if (NULL != app_icon2_data[i].buf)
        {
            sty_delete_bitmap(app_icon2_data[i].buf);
            app_icon2_data[i].buf = NULL;
        }
        if (NULL != app_icon3_data[i].buf)
        {
            sty_delete_bitmap(app_icon3_data[i].buf);
            app_icon3_data[i].buf = NULL;
        }
        if (NULL != app_icon4_data[i].buf)
        {
            sty_delete_bitmap(app_icon4_data[i].buf);
            app_icon4_data[i].buf = NULL;
        }
    }

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

static void _draw_pic(int hdc, pic_data_t page_data, int x, int y)
{
    //first draw picture from SDRAM buffer
    if (NULL != page_data.buf)
    {
        sty_draw_bitmap(hdc, page_data.buf, x, y);
    }
    else
    {
        sty_draw_picture_id(hdc, page_data.res->pic_id, x, y);
    }
}

static void _draw_app_name(animation_step_e step, scene_desktop_t *data)
{
    appinfo_list_t *cur_app = NULL;
    char app_name[128] =
    { 0 };
    int alpha_bak;
    //获取dc
    int hdc = gui_dc_get(g_scene_desktop->win_id);

    switch (step)
    {
        case STEP_INIT:
        {
            /* make up the app name, for example: "1. music"*/
            snprintf(app_name, sizeof(app_name), "%d. %s", (data->cur_desktop->app_index + 1),
                    data->cur_desktop->value[DKEY_NAME_LANG]);
            app_name[sizeof(app_name) - 1] = '\0';
            /* draw cur app name */
            sty_draw_string_res_ext(hdc, str_app_name, app_name);
        }
        break;

        case STEP_1:
        {
            //根据动画显示方向（即按键），获取当前焦点应用的信息
            if (data->display_mode == PREV_ANIMATION)
            {
                cur_app = data->cur_desktop->next;
            }
            else if (data->display_mode == NEXT_ANIMATION)
            {
                cur_app = data->cur_desktop->prev;
            }
            else
            {
                print_err("the data->display_mode is error value.");
                return;
            }

            alpha_bak = gui_dc_get_alpha(hdc);
            gui_dc_set_alpha(hdc, ALPHA_70PAR);

            /* make up the app name, for example: "1. music"*/
            snprintf(app_name, sizeof(app_name), "%d. %s", (cur_app->app_index + 1), cur_app->value[DKEY_NAME_LANG]);
            app_name[sizeof(app_name) - 1] = '\0';
            /* draw cur app name */
            sty_draw_string_res_ext(hdc, str_app_name_p1, app_name);

            gui_dc_set_alpha(hdc, alpha_bak);

        }
        break;

        case STEP_2:
        {
            //根据动画显示方向（即按键），获取当前焦点应用的信息
            if (data->display_mode == PREV_ANIMATION)
            {
                cur_app = data->cur_desktop->next;
            }
            else if (data->display_mode == NEXT_ANIMATION)
            {
                cur_app = data->cur_desktop->prev;
            }
            else
            {
                print_err("the data->display_mode is error value.");
                return;
            }

            alpha_bak = gui_dc_get_alpha(hdc);
            gui_dc_set_alpha(hdc, ALPHA_30PAR);

            /* make up the app name, for example: "1. music"*/
            snprintf(app_name, sizeof(app_name), "%d. %s", (cur_app->app_index + 1), cur_app->value[DKEY_NAME_LANG]);
            app_name[sizeof(app_name) - 1] = '\0';
            /* draw cur app name */
            sty_draw_string_res_ext(hdc, str_app_name_p2, app_name);

            gui_dc_set_alpha(hdc, alpha_bak);
        }
        break;

        case STEP_3:
        {
            alpha_bak = gui_dc_get_alpha(hdc);
            gui_dc_set_alpha(hdc, ALPHA_30PAR);

            /* make up the app name, for example: "1. music"*/
            snprintf(app_name, sizeof(app_name), "%d. %s", (data->cur_desktop->app_index + 1),
                    data->cur_desktop->value[DKEY_NAME_LANG]);
            app_name[sizeof(app_name) - 1] = '\0';
            /* draw cur app name */
            sty_draw_string_res_ext(hdc, str_app_name_p3, app_name);

            gui_dc_set_alpha(hdc, alpha_bak);

        }
        break;

        default:

        break;
    }
}

//static bool _erase_region(int hdc, gui_bitmap_info_t * bmp, picture_resource_t *pic)
//{
//    sty_rect_t rect;
//
//    //draw bg
//    if ((bmp != NULL) && (pic != NULL))
//    {
//        rect.x = pic->x;
//        rect.y = pic->y;
//        rect.width = pic->width;
//        rect.height = pic->height;
//
//        sty_draw_sub_bitmap(hdc, bmp, pic->x, pic->y, &rect);
//    }
//
//    return true;
//}

/* draw un-selected app icon*/
static void _draw_unsel_app_icon(appinfo_list_t *current, scene_desktop_t *data)
{
    appinfo_list_t *cur_app, *next_app;
    picture_resource_t * app_icon_pos = NULL;
    app_e app_macro;
    int i, pos_offset, alpha_bak;

    //获取dc
    int hdc = gui_dc_get(data->win_id);

    pos_offset = current->app_index - (current->app_index / MAX_IN_ROW) * MAX_IN_ROW; //cur app to 1st_app
    next_app = data->cur_desktop;

    /* draw 1st row app icon */
    cur_app = current;
    for (i = 0; i < (pos_offset + MAX_IN_ROW); i++) //move to 1st_app of the 1st row
    {
        cur_app = cur_app->prev;
    }

    for (i = 0; i < MAX_IN_ROW; i++) // draw 1st app to last app in the 1st row.
    {
        if (cur_app != next_app) //don't draw the next app icon.
        {
            app_icon_pos = app_icon1_data[(cur_app->app_index % MAX_IN_ROW) + 2 * MAX_IN_ROW].res;

            alpha_bak = gui_dc_get_alpha(hdc);
            gui_dc_set_alpha(hdc, ALPHA_30PAR);
            app_macro = _get_app_macro(cur_app->value[DKEY_NAME]);
            _draw_pic(hdc, app_icon1_data[app_macro % TYPE_COUNT], app_icon_pos->x, app_icon_pos->y);
            gui_dc_set_alpha(hdc, alpha_bak);
        }
        cur_app = cur_app->next;
    }

    /* draw 2nd row app icon */
    cur_app = current;
    for (i = 0; i < pos_offset; i++) //move to 1st_app in 2nd row
    {
        cur_app = cur_app->prev;
    }

    for (i = 0; i < MAX_IN_ROW; i++) // draw 1st app to last app in the 2nd row.
    {
        app_macro = _get_app_macro(cur_app->value[DKEY_NAME]);
        app_icon_pos = app_icon1_data[(cur_app->app_index % MAX_IN_ROW)].res;
        if ((cur_app != next_app) && (cur_app != current))
        {
            _draw_pic(hdc, app_icon1_data[app_macro % TYPE_COUNT], app_icon_pos->x, app_icon_pos->y);
        }

        cur_app = cur_app->next;
    }

    /* draw 3th row app icon */
    cur_app = current;
    for (i = 0; i < (MAX_IN_ROW - pos_offset); i++) //move to 1st_app in 3th row
    {
        cur_app = cur_app->next;
    }
    for (i = 0; i < MAX_IN_ROW; i++) // draw 1st app to last app in the 3th row.
    {
        if (cur_app != next_app) //don't draw the next app icon.
        {
            app_macro = _get_app_macro(cur_app->value[DKEY_NAME]);
            app_icon_pos = app_icon1_data[(cur_app->app_index % MAX_IN_ROW) + MAX_IN_ROW].res;

            alpha_bak = gui_dc_get_alpha(hdc);
            gui_dc_set_alpha(hdc, ALPHA_30PAR);
            _draw_pic(hdc, app_icon1_data[app_macro % TYPE_COUNT], app_icon_pos->x, app_icon_pos->y);
            gui_dc_set_alpha(hdc, alpha_bak);
        }
        cur_app = cur_app->next;
    }
}

/*
 * for example: ROW_NUM = 3, MAX_IN_ROW = 4
 * 1st row:   1st_app  2nd_app  3th_app  4th_app
 * 2nd row:   1st_app  2nd_app  3th_app  4th_app
 * 3th row:   1st_app  2nd_app  3th_app  4th_app
 *
 */
static void _draw_app_icon(animation_step_e step, scene_desktop_t *data)
{
    appinfo_list_t *cur_app, *next_app;
    picture_resource_t * app_icon_pos = NULL;
    app_e app_macro;
    int alpha_bak;
    bool change_row = false;

    //获取dc
    int hdc = gui_dc_get(data->win_id);

    switch (step)
    {
        case STEP_INIT:
        case STEP_3: //same as STEP_INIT, so here don't do anything.
        {
            /* draw un-selected app icon*/
            _draw_unsel_app_icon(data->cur_desktop, data);

            /* draw selected app icon in 2nd row */
            cur_app = data->cur_desktop;
            app_macro = _get_app_macro(cur_app->value[DKEY_NAME]);
            app_icon_pos = app_icon1_data[(cur_app->app_index % MAX_IN_ROW)].res;
            _draw_pic(hdc, app_icon4_data[app_macro % TYPE_COUNT], app_icon_pos->x, app_icon_pos->y);
        }
        break;

        case STEP_1:
        {
            /* draw next app icon */
            next_app = data->cur_desktop;
            if (data->display_mode == PREV_ANIMATION)
            {
                cur_app = data->cur_desktop->next; //根据动画显示方向（即按键），获取当前焦点应用的信息

                if ((cur_app->app_index % MAX_IN_ROW) == 0) //next app icon should be display in 1st row
                {
                    change_row = true;
                    app_icon_pos = app_icon1_data[((next_app->app_index % MAX_IN_ROW) + (2 * MAX_IN_ROW))].res;
                }
                else
                {
                    change_row = false;
                    app_icon_pos = app_icon1_data[(next_app->app_index % MAX_IN_ROW)].res;
                }
            }
            else if (data->display_mode == NEXT_ANIMATION)
            {
                cur_app = data->cur_desktop->prev; //根据动画显示方向（即按键），获取当前焦点应用的信息

                if ((cur_app->app_index % MAX_IN_ROW) == (MAX_IN_ROW - 1)) //next app icon should be display in 3th row
                {
                    change_row = true;
                    app_icon_pos = app_icon1_data[((next_app->app_index % MAX_IN_ROW) + MAX_IN_ROW)].res;
                }
                else
                {
                    change_row = false;
                    app_icon_pos = app_icon1_data[(next_app->app_index % MAX_IN_ROW)].res;
                }
            }
            else
            {
                print_err();
                cur_app = data->cur_desktop;
                app_icon_pos = app_icon1_data[0].res; //have error,so display in first app positions.
            }
            //_erase_region(hdc, desktop_bg.buf, app_icon_pos);
            app_macro = _get_app_macro(next_app->value[DKEY_NAME]);
            if (change_row == true)
            {
                alpha_bak = gui_dc_get_alpha(hdc);
                gui_dc_set_alpha(hdc, ALPHA_30PAR);
                _draw_pic(hdc, app_icon2_data[app_macro % TYPE_COUNT], app_icon_pos->x, app_icon_pos->y);
                gui_dc_set_alpha(hdc, alpha_bak);
            }
            else
            {
                _draw_pic(hdc, app_icon2_data[app_macro % TYPE_COUNT], app_icon_pos->x, app_icon_pos->y);
            }

            /* draw current app icon */
            app_icon_pos = app_icon1_data[(cur_app->app_index % MAX_IN_ROW)].res;
            //_erase_region(hdc, desktop_bg.buf, app_icon_pos);
            app_macro = _get_app_macro(cur_app->value[DKEY_NAME]);
            _draw_pic(hdc, app_icon3_data[app_macro % TYPE_COUNT], app_icon_pos->x, app_icon_pos->y);

            /* draw un-selected app icon*/
            _draw_unsel_app_icon(cur_app, data);
        }
        break;

        case STEP_2:
        {
            /* draw next app icon */
            next_app = data->cur_desktop;
            if (data->display_mode == PREV_ANIMATION)
            {
                cur_app = data->cur_desktop->next; //根据动画显示方向（即按键），获取当前焦点应用的信息

                if ((cur_app->app_index % MAX_IN_ROW) == 0) //next app icon should be display in 1st row
                {
                    change_row = true;
                    app_icon_pos = app_icon1_data[((next_app->app_index % MAX_IN_ROW) + (2 * MAX_IN_ROW))].res;
                }
                else
                {
                    change_row = false;
                    app_icon_pos = app_icon1_data[(next_app->app_index % MAX_IN_ROW)].res;
                }
            }
            else if (data->display_mode == NEXT_ANIMATION)
            {
                cur_app = data->cur_desktop->prev; //根据动画显示方向（即按键），获取当前焦点应用的信息

                if ((cur_app->app_index % MAX_IN_ROW) == (MAX_IN_ROW - 1)) //next app icon should be display in 3th row
                {
                    change_row = true;
                    app_icon_pos = app_icon1_data[((next_app->app_index % MAX_IN_ROW) + MAX_IN_ROW)].res;
                }
                else
                {
                    change_row = false;
                    app_icon_pos = app_icon1_data[(next_app->app_index % MAX_IN_ROW)].res;
                }
            }
            else
            {
                print_err();
                cur_app = data->cur_desktop;
                app_icon_pos = app_icon1_data[0].res; //have error,so display in first app positions.
            }
            //_erase_region(hdc, desktop_bg.buf, app_icon_pos);
            app_macro = _get_app_macro(next_app->value[DKEY_NAME]);
            if (change_row == true)
            {
                alpha_bak = gui_dc_get_alpha(hdc);
                gui_dc_set_alpha(hdc, ALPHA_30PAR);
                _draw_pic(hdc, app_icon3_data[app_macro % TYPE_COUNT], app_icon_pos->x, app_icon_pos->y);
                gui_dc_set_alpha(hdc, alpha_bak);
            }
            else
            {
                _draw_pic(hdc, app_icon3_data[app_macro % TYPE_COUNT], app_icon_pos->x, app_icon_pos->y);
            }

            /* draw current app icon */
            app_icon_pos = app_icon1_data[(cur_app->app_index % MAX_IN_ROW)].res;
            //_erase_region(hdc, desktop_bg.buf, app_icon_pos);
            app_macro = _get_app_macro(cur_app->value[DKEY_NAME]);
            _draw_pic(hdc, app_icon2_data[app_macro % TYPE_COUNT], app_icon_pos->x, app_icon_pos->y);

            /* draw un-selected app icon*/
            _draw_unsel_app_icon(cur_app, data);

        }
        break;

        default:

        break;
    }
}

void show_next_desktop(scene_desktop_t *data)
{
    animation_step_e display_step = STEP_INIT;
    for (display_step = STEP_1; display_step < STEP_COUNT; display_step++)
    {
        //DUMP_TIME();
        show_desktop_bg();
        _draw_app_icon(display_step, data);
        _draw_app_name(display_step, data);
        gui_screen_update();
        //TODO: delay some ms for animation
    }
}

void show_child_desktop(scene_desktop_t *data)
{

}

void show_desktop_bg(void)
{
    int hdc = gui_dc_get(g_scene_desktop->win_id);
    _draw_pic(hdc, desktop_bg, desktop_bg.res->x, desktop_bg.res->y);
}

void show_current_desktop(scene_desktop_t *data)
{
    //DUMP_TIME();
    show_desktop_bg();
    _draw_app_icon(STEP_INIT, data);
    _draw_app_name(STEP_INIT, data);
    //DUMP_TIME();
}
