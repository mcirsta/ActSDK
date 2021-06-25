#ifndef      __MYSCENE_H
#define      __MYSCENE_H

#include "include_case.h"

#include "calendar_sty.h"

/*!
 * scene type define
 */

/* exit type */
typedef enum
{
    CALENDAR_SCENE_RETURN, CALENDAR_SCENE_NEXT,
} scene_return_e;

/* scene of apps common */
typedef struct
{
    /* style resource */
    scene_t* scene;
    scene_t* common_scene;

    /* scene initial flag */
    bool inited;
    bool need_exit; //TRUE: need to exit

    /* window resource */
    int win_id;
    int handle_gui_dc;

    /* return flag */
    scene_return_e exit_type; //0: return, 1: next scene
} calendar_scene_common_t;

typedef struct
{
    /* scene */
    calendar_scene_common_t scene_common;

    /* font description*/
    int fontdes;

    /* widget resource */
    headbar_t *headbar;
    msgbox_t * msgbox;
    volumebar_t *volumebar;
    gui_bitmap_info_t *bg_bitmap_info;

    /* resource */
    picture_resource_t *bg_pic;
    picture_resource_t *icon_pic;
    picture_resource_t *week_pic; //星期1-7的图片
    picture_resource_t *area_pic; //消息栏
    picture_resource_t *bigarea_pic; //显示一个月所有天的表的背景
    picture_resource_t *left_pic; //左箭头
    picture_resource_t *right_pic; //右箭头
    picregion_resource_t *numArryY_pic; //黄色数字图片组
    picregion_resource_t *numArryW_pic; //白色数字图片组

    picture_resource_t *shield_pic; //当前日期中的天的黄色背景图片

    picture_resource_t *line1_pic; //分隔符'-'
    picture_resource_t *line2_pic; //
    /* string */
    string_resource_t *title_str;
    string_resource_t *info_str;
    string_resource_t *color_str; //用于规定周六(前景色)和周日的颜色(背景色)
    string_resource_t *nodisk_str;
    
    /* 用于取得8个数字图片的坐标 */
    picture_resource_t *position1_pic;
    picture_resource_t *position2_pic;
    picture_resource_t *position3_pic;
    picture_resource_t *position4_pic;
    picture_resource_t *position5_pic;
    picture_resource_t *position6_pic;
    picture_resource_t *position7_pic;
    picture_resource_t *position8_pic;

    picture_resource_t *block_pic; //date table的第一个坐标

} scene_calendar_t;

/* scene entry */
int calendar_scene(fusion_enter_e fenter);

#endif

