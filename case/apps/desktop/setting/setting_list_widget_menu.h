/********************************************************************************
 *                              USDK(1100)
 *                             Module: app
 *                 Copyright(c) 2003-2008 Actions Semiconductor,
 *                            All Rights Reserved.
 * file : setting_list_widget_menu.h
 * History:
 *      <author>    <time>           <version >             <desc>
 *      Herb Hu    2009-5-5             v1.0             build this file
 ********************************************************************************/

#ifndef __SET_LIST_WIDGET_MENU_H__
#define __SET_LIST_WIDGET_MENU_H__

#include <include_case.h>
#include <include_psp.h>
#include "setting_sty.h"
#include "setting_config.h"
#include "setting.h"


/*********************************************************************************
 * multilevel menu widget interface
 *********************************************************************************/

typedef enum
{
    WIDGET_SIG_MENU_QUIT,
    WIDGET_SIG_MENU_LANGUAGE_CHANGED,
    WIDGET_SIG_MENU_THEME_CHANGED,
    WIDGET_SIG_MENU_SCREEN_MODE_CHANGED,
    WIDGET_SIG_MENU_FONT_CHANGED,
}menu_sig_e;

typedef bool (*menu_entry_t)( menuview_t * );
typedef bool (* menu_deinit_cb)(void);
typedef bool (* menu_update_minor_info_cb)(void);

typedef struct
{
    menu_deinit_cb deinit_cb;
    menu_update_minor_info_cb update_minor_info_cb;
}sub_menu_t;

menuview_t* setting_list_widget_menu_create( menu_cb_t callback );
extern bool setting_list_widget_menu_delete( menuview_t *menuview );
extern bool setting_list_widget_menu_load_style( menuview_t *menuview );
extern bool setting_list_widget_menu_paint( menuview_t *menuview );
extern bool setting_list_widget_menu_hide( menuview_t *menuview );
extern bool setting_list_widget_menu_show( menuview_t *menuview );
extern bool setting_list_widget_menu_proc_gui( menuview_t *menuview, gui_wm_message_t *pmsg );
extern bool setting_list_widget_menu_exit( menuview_t *menuview,  menu_entry_t entry );
extern inline bool setting_list_widget_menu_emit_signal(menuview_t *menuview, menu_sig_e sig);
extern int setting_list_widget_menu_register_sub_menu( menuview_t *menuview , sub_menu_t* sub_menu);
extern bool setting_list_widget_menu_unregister_sub_menu(int sub_menu_id);
extern void setting_list_widget_menu_show_fusion( menuview_t *menuview, fusion_enter_e fusion_enter_type );

extern bool setting_list_widget_menu_show( menuview_t *menuview );

extern bool setting_list_widget_menu_update_minor_info( menuview_t *menuview );

/*********************************************************************************
 * menu common functions
 *********************************************************************************/

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

extern int menu_load_item_resource(resgroup_resource_t * res_group, menuitem_style_t * menuitem_style,\
        menuitem_t * menuitem, int count);


typedef struct
{
    int value;
    char show_text[64];
}value_map_t;

extern int maps_find(value_map_t * maps, int count, int value);

/*********************************************************************************
 * main menu
 *********************************************************************************/
extern menu_t* setting_list_menu_main_enter( menuview_t *menuview , fusion_enter_e fusion_enter_type);

/*********************************************************************************
 * desktop theme select menu
 *********************************************************************************/
extern menu_t* setting_list_menu_theme_enter( menuview_t *menuview , fusion_enter_e fusion_enter_type);
extern const char * setting_list_menu_theme_get_cur_selected_text(void);

/*********************************************************************************
 * screen mode set menu
 *********************************************************************************/
extern menu_t* setting_list_menu_screen_mode_enter( menuview_t *menuview , fusion_enter_e fusion_enter_type);
extern const char * setting_list_menu_screen_mode_get_cur_selected_text(void);

/*********************************************************************************
 * display set menu
 *********************************************************************************/
extern menu_t* setting_list_menu_display_enter( menuview_t *menuview , fusion_enter_e fusion_enter_type);

/*********************************************************************************
 * power set menu
 *********************************************************************************/
extern menu_t* setting_list_menu_power_enter( menuview_t *menuview , fusion_enter_e fusion_enter_type);

/*********************************************************************************
 * date and time set menu
 *********************************************************************************/
extern menu_t* setting_list_menu_date_time_enter( menuview_t *menuview , fusion_enter_e fusion_enter_type);

/*********************************************************************************
 * language set menu
 *********************************************************************************/
extern menu_t* setting_list_menu_language_enter( menuview_t *menuview , fusion_enter_e fusion_enter_type);
extern const char * setting_list_menu_language_get_cur_selected_text(void);

/*********************************************************************************
 * tv out set menu
 *********************************************************************************/
extern menu_t* setting_list_menu_tvout_enter( menuview_t *menuview , fusion_enter_e fusion_enter_type);
extern const char * setting_list_menu_tvout_get_cur_selected_text(void);

/*********************************************************************************
 * desktop theme menu
 *********************************************************************************/
extern menu_t* setting_list_menu_advanced_enter( menuview_t *menuview , fusion_enter_e fusion_enter_type);

/*********************************************************************************
 * player information menu
 *********************************************************************************/
extern menu_t* setting_list_menu_info_enter( menuview_t *menuview , fusion_enter_e fusion_enter_type);

/*********************************************************************************
 * connect type set menu
 *********************************************************************************/
extern menu_t* setting_list_menu_connect_type_enter( menuview_t *menuview , fusion_enter_e fusion_enter_type);
extern const char * setting_list_menu_connect_type_get_cur_selected_text(void);

/*********************************************************************************
 * user font select menu
 *********************************************************************************/
extern menu_t* setting_list_menu_font_select_enter( menuview_t *menuview , fusion_enter_e fusion_enter_type);
extern const char * setting_list_menu_font_select_get_cur_selected_text(void);

/*********************************************************************************
 * key tone select menu
 *********************************************************************************/
extern menu_t* setting_list_menu_key_tone_enter( menuview_t *menuview , fusion_enter_e fusion_enter_type);
extern const char * setting_list_menu_key_tone_get_cur_selected_text(void);

/*********************************************************************************
 * show effect select menu
 *********************************************************************************/
extern menu_t* setting_list_menu_show_effect_enter( menuview_t *menuview , fusion_enter_e fusion_enter_type);
extern const char * setting_list_menu_show_effect_get_cur_selected_text(void);


#endif /* #ifndef __SET_LIST_WIDGET_MENU_H__ */

