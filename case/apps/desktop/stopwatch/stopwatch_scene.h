#ifndef      __MYSCENE_H
#define      __MYSCENE_H

#include "include_case.h"
#include "watch_sty.h"

/*!
 * scene type define
 */

/* exit type */
typedef enum
{
    WATCH_SCENE_RETURN, WATCH_SCENE_NEXT,
} scene_return_e;

/* scene of apps common */
typedef struct
{
    /* style resource */
    scene_t* scene;
    scene_t* common_scene;

    /* scene initial flag */
    bool inited;
    /* TRUE: need to exit */
    bool need_exit;

    /* window resource */
    int win_id;
    int handle_gui_dc;

    /* return flag 0: return, 1: next scene */
    scene_return_e exit_type;
} watch_scene_common_t;

/* class for stopwatch lable */
typedef struct
{
    /* label's picture source */
    picture_resource_t *pic_res;
    /* label's string source,show time value */
    string_resource_t *str_res;
    /* label's string source,show the lable's idx */
    string_resource_t *str_id_res;
} watch_label_t;

typedef struct
{
    /* scene */
    watch_scene_common_t scene_common;

    /* font description*/
    int fontdes;

    /* headbar */
    headbar_t *headbar;
    /* 指向headbar资源组的指针 */
    resgroup_resource_t *headbar_resgroup;
    /* dialog */
    dialog_t *dialog;
    msgbox_t *msgbox;
    volumebar_t *volumebar;
    gui_bitmap_info_t *bg_bitmap_info;

    string_resource_t *restart_str;
    string_resource_t *nodisk_str;
    
    /* resource of show */
    picture_resource_t *bg_pic;
    picture_resource_t *show_area; //show area
    picture_resource_t *cover_pic;

    /* 6条记录的图片和字符串  */
    watch_label_t watch1;
    watch_label_t watch2;
    watch_label_t watch3;
    watch_label_t watch4;
    watch_label_t watch5;
    watch_label_t watch6;

    /* 8 图片 和 数字资源 */
    picture_resource_t *num_pic;
    picture_resource_t *position1_pic; //用于取得8个数字图片的坐标
    picture_resource_t *position2_pic;
    picture_resource_t *position3_pic;
    picture_resource_t *position4_pic;
    picture_resource_t *position5_pic;
    picture_resource_t *position6_pic;
    picture_resource_t *position7_pic;

    picture_resource_t *dot2_pic;
    picture_resource_t *dot2_2_pic;
    picture_resource_t *dot1_pic;
    /*timer id*/
    int timer_id;

} scene_stopwatch_t;

/* scene entry */
extern int stopwatch_scene(fusion_enter_e fenter);
extern void watch_scene_init_paint(scene_stopwatch_t *my_scene);
extern bool is_upright_disp(void);

#endif

