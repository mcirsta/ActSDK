#ifndef      __EMULATOR_SCENE_H__
#define      __EMULATOR_SCENE_H__

#include "include_case.h"
#include "emulator_sty.h"


#define ROM_FILE_FULL_PATH  512  /* rom�ļ�ȫ·���������ֵ */
#define GAEM_MAX_ROM_FILE  10    /* rom�ļ������ָ */

/*!
 * scene type define
 */

/* exit type */
typedef enum
{
    EMULATOR_SCENE_RETURN, EMULATOR_SCENE_NEXT,
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
} emulator_scene_common_t;

typedef struct
{
    /* scene */
    emulator_scene_common_t scene_common;

    /* font description*/
    int fontdes;

    /* resource */
    picture_resource_t *bg_pic;
    picture_resource_t *icon_pic; //Ӧ�õ�ͼ��
    /* string */
    string_resource_t *title_str; //Ӧ�õ�����

    string_resource_t *restart_str;
    string_resource_t *load_str;
    string_resource_t *save_str;
    string_resource_t *set_str;
    string_resource_t *quit_str;

    string_resource_t *sound_str; //���������ַ���
    string_resource_t *key_str;   //���������ַ���
    string_resource_t *disp_str;  //��ʾ�����ַ���
    
    string_resource_t *saved_str;
    string_resource_t *save_err_str;
    string_resource_t *modify_str;
    string_resource_t *failopen_str;
    string_resource_t *nosupp_str;    
    string_resource_t *nodisk_str;
             

    /* ���ļ���ʾ�Ի�����ַ��� */
    string_resource_t *no_file_str;
    /* ȫ����ʾ�Ի�����ַ��� */
    string_resource_t *full_screen_str;
    /* widget resource */
    menuview_t *menuview;
    adjust_t *adjust;
    headbar_t *headbar;
    dialog_t *dlg;
    dirlist_t *dirlist;
    msgbox_t *msgbox;
    volumebar_t *volumebar;
    gui_bitmap_info_t *bg_bitmap_info;

    /* 3���˵����Եĵ�ǰ item */
    int cur_item01;
    int cur_item02;
    int cur_item03;

    /* ��ǰ���ļ��˵�:1|2|3 */
    int cur_menu;

    /* ģ���������Ƿ���:[0:û��][1:�Ѿ�����] */
    int thread_exist;

} scene_emulator_t;

/* scene entry */
int emulator_scene(void);
void emulator_scene_quit(void);
bool em_create_headbar(void);
bool em_delete_headbar(void);
bool em_create_menuview(void);
bool em_delete_menuview(void);
void scene_fusion_set_first_frame(scene_t *scene);
void scene_fusion_set_last_frame(scene_t *scene);

#endif

