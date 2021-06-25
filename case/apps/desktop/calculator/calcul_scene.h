#ifndef      __MYSCENE_H
#define      __MYSCENE_H

#include "include_case.h"
#include "calculat_sty.h"

/*!
 * scene type define
 */

/* exit type */
typedef enum
{
    CALCUL_SCENE_RETURN, CALCUL_SCENE_NEXT,
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

    /* return flag 0: return, 1: next scene */
    scene_return_e exit_type;
} calcul_scene_common_t;

typedef struct
{
    /* scene */
    calcul_scene_common_t scene_common;

    /* font description*/
    int fontdes;

    /* headbar */
    headbar_t *headbar;
    msgbox_t * msgbox;
    volumebar_t *volumebar;
    gui_bitmap_info_t *bg_bitmap_info;

    /* 指向headbar资源组的指针 */
    resgroup_resource_t *headbar_resgroup;

    /*! resource of show */
    picture_resource_t *bg_pic;
    picture_resource_t *keyboard_pic;

    picture_resource_t *pic_0;
    picture_resource_t *pic_1;
    picture_resource_t *pic_2;
    picture_resource_t *pic_3;
    picture_resource_t *pic_4;
    picture_resource_t *pic_5;
    picture_resource_t *pic_6;
    picture_resource_t *pic_7;
    picture_resource_t *pic_8;
    picture_resource_t *pic_9;
    picture_resource_t *pic_dot;

    picture_resource_t *pic_mminus;
    picture_resource_t *pic_mplus;
    picture_resource_t *pic_mc;
    picture_resource_t *pic_mr;

    picture_resource_t *pic_add;
    picture_resource_t *pic_sub;
    picture_resource_t *pic_mul;
    picture_resource_t *pic_div;
    picture_resource_t *pic_cent; /* '%' */
    picture_resource_t *pic_sqrt; /* '根号' */
    picture_resource_t *pic_daoshu; /* '1/x' */

    picture_resource_t *pic_c;
    picture_resource_t *pic_ce;
    picture_resource_t *pic_sign; /* '+/-' */
    picture_resource_t *pic_equal;
    picture_resource_t *pic_backspace;

    picture_resource_t *pic_mem; /*用来遮蔽Memory标志的图片*/

    /* 字符串  */
    string_resource_t *str;
    string_resource_t *nodisk_str;
    
} scene_calcul_t;

/* scene entry */
int calcul_scene(fusion_enter_e fenter);



#endif

