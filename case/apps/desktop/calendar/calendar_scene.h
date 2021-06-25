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
    picture_resource_t *week_pic; //����1-7��ͼƬ
    picture_resource_t *area_pic; //��Ϣ��
    picture_resource_t *bigarea_pic; //��ʾһ����������ı�ı���
    picture_resource_t *left_pic; //���ͷ
    picture_resource_t *right_pic; //�Ҽ�ͷ
    picregion_resource_t *numArryY_pic; //��ɫ����ͼƬ��
    picregion_resource_t *numArryW_pic; //��ɫ����ͼƬ��

    picture_resource_t *shield_pic; //��ǰ�����е���Ļ�ɫ����ͼƬ

    picture_resource_t *line1_pic; //�ָ���'-'
    picture_resource_t *line2_pic; //
    /* string */
    string_resource_t *title_str;
    string_resource_t *info_str;
    string_resource_t *color_str; //���ڹ涨����(ǰ��ɫ)�����յ���ɫ(����ɫ)
    string_resource_t *nodisk_str;
    
    /* ����ȡ��8������ͼƬ������ */
    picture_resource_t *position1_pic;
    picture_resource_t *position2_pic;
    picture_resource_t *position3_pic;
    picture_resource_t *position4_pic;
    picture_resource_t *position5_pic;
    picture_resource_t *position6_pic;
    picture_resource_t *position7_pic;
    picture_resource_t *position8_pic;

    picture_resource_t *block_pic; //date table�ĵ�һ������

} scene_calendar_t;

/* scene entry */
int calendar_scene(fusion_enter_e fenter);

#endif

