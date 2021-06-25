/********************************************************************************
 *                              USDK(1100)
 *                             Module: app 
 *                 Copyright(c) 2003-2009 Actions Semiconductor,
 *                            All Rights Reserved. 
 * File : music_playing_option_advanced.c
 * History:         
 *      <author>    <time>           <version >             <desc>
 *    songzhining    2009-2-25          v1.0               build this file 
 ********************************************************************************/
/*!
* \file     music_playing_option_advanced.c
* \brief    music_playing_option_advanced widget
* \author   songzhining
*               
* \version 1.0
* \date  2009/02/25
*******************************************************************************/

#include "music_playing_option_advanced.h"

/* 退出主菜单 */
bool music_playing_option_advanced_exit( menuview_t *menuview );

/* 初始化菜单数据 */
menu_t* music_playing_option_advanced_init( void );

/* 删除菜单数据 */
int music_playing_option_advanced_deinit( void );

/* listmenu entry */
static int _proc_item_reptime( menuview_t *menuview, menuview_sig_e signal );
static int _proc_item_repinterval( menuview_t *menuview, menuview_sig_e signal );
static int _proc_item_scanspeed( menuview_t *menuview, menuview_sig_e signal );
static int _proc_item_fade( menuview_t *menuview, menuview_sig_e signal );

static bool _create_reptime_adjust_widget( void );
static bool _delete_reptime_adjust_widget( void );
static void _reptime_adjust_widget_cb(adjust_t *adjust, adjust_signal_e signal);

static bool _create_repinterval_adjust_widget( void );
static bool _delete_repinterval_adjust_widget( void );
static void _repinterval_adjust_widget_cb(adjust_t *adjust, adjust_signal_e signal);

static bool _create_scanspeed_adjust_widget( void );
static bool _delete_scanspeed_adjust_widget( void );
static void _scanspeed_adjust_widget_cb(adjust_t *adjust, adjust_signal_e signal);

static bool _create_fade_adjust_widget( void );
static bool _delete_fade_adjust_widget( void );
static void _fade_adjust_widget_cb(adjust_t *adjust, adjust_signal_e signal);

/* menu list data define */
static menu_t *s_option_advanced = NULL;
static int s_option_advanced_offset = 0;
static int s_option_advanced_current = 0;

/* menu items data define */
static style_menuitem_t s_option_advanced_items[] =
{    
    // name, has_child, callback
    { STR_REPTIMES,   false,  (item_cb_t)_proc_item_reptime, 0, 0, 0, 0, 0 },
    { STR_REPINTER,   false,   (item_cb_t)_proc_item_repinterval, 0, 0, 0, 0, 0 },
    { STR_SCAN,   false,   (item_cb_t)_proc_item_scanspeed, 0, 0, 0, 0, 0 },
    { STR_FADE,   false,   (item_cb_t)_proc_item_fade, 0, 0, 0, 0, 0 },
};

/* menu list max items */
#define MENU_OPTION_ADVANCED_MAX_ITEMS  ( sizeof(s_option_advanced_items)/sizeof(style_menuitem_t) )

/* scan speed strings */
static char *s_scan_speed[] = 
{
    "X2", "X4",
"X8", "X16", "X32",

};

/* scan speed max items */
#define SCANSPEED_MAX_ITEMS  ( sizeof(s_scan_speed)/sizeof(char *) )


/* 进入主菜单 */
bool music_playing_option_advanced_enter( menuview_t *menuview )
{
    menu_t *cur_menu;

    cur_menu = music_playing_option_advanced_init();
    menuview_set_menu( menuview, cur_menu );
    
    if ( menuview->type != MENUVIEW_OPTION )
    {
        menuview_load_style( menuview, MENUVIEW_OPTION );
    }
    menuview_paint( menuview );

    return true;
}

/* 退出主菜单 */
bool music_playing_option_advanced_exit( menuview_t *menuview )
{
    //delete main menu
    music_playing_option_advanced_deinit();

    //reset index
    s_option_advanced_offset = 0;
    s_option_advanced_current = 0;

    //return
    music_playing_option_enter( menuview );
    
    return true;
}

menu_t* music_playing_option_advanced_init( void )
{
    menuitem_t item;
    unsigned int num;
    
    //initial menu    
    if( s_option_advanced == NULL )
    {
        //create menu
        s_option_advanced = menu_create();

        //load style
        menu_load_style( ADVANCED_MENU_RESGROUP, s_option_advanced_items, MENU_OPTION_ADVANCED_MAX_ITEMS );
        
        //init items
        memset( &item, 0, sizeof(menuitem_t) );
        
        for ( num=0; num<MENU_OPTION_ADVANCED_MAX_ITEMS; num++ )
        {
            item.text_id = s_option_advanced_items[num].str_id;
            item.has_child = s_option_advanced_items[num].has_child;
            item.callback = s_option_advanced_items[num].callback;
            
            menu_append_item( s_option_advanced, &item );
        }

        //init index
        menu_set_offset( s_option_advanced, s_option_advanced_offset );
        menu_set_cur_index( s_option_advanced, s_option_advanced_current );
    }

    return s_option_advanced;
}

int music_playing_option_advanced_deinit( void )
{    
    //delete menu    
    if( s_option_advanced != NULL )
    {
        //save index
        s_option_advanced_offset = s_option_advanced->offset;
        s_option_advanced_current = s_option_advanced->current;
        
        //create menu
        menu_delete( s_option_advanced );
        s_option_advanced = NULL;
    }

    return true;
}

static int _proc_item_reptime( menuview_t *menuview, menuview_sig_e signal )
{
    switch ( signal ) 
    {
    case WIDGET_SIG_MENUVIEW_OK:
        _create_reptime_adjust_widget();
        break;

    case WIDGET_SIG_MENUVIEW_QUIT:
        // return to parent menu
        music_playing_option_advanced_exit( menuview );
        break;
         
    default:
        break;
    }    

    return true;
}

static int _proc_item_repinterval( menuview_t *menuview, menuview_sig_e signal )
{
    switch ( signal ) 
    {
    case WIDGET_SIG_MENUVIEW_OK:
        _create_repinterval_adjust_widget();
        break;

    case WIDGET_SIG_MENUVIEW_QUIT:
        // return to parent menu
        music_playing_option_advanced_exit( menuview );
        break;
         
    default:
        break;
    }    

    return true;
}

static int _proc_item_scanspeed( menuview_t *menuview, menuview_sig_e signal )
{
    switch ( signal ) 
    {
    case WIDGET_SIG_MENUVIEW_OK:
        _create_scanspeed_adjust_widget();
        break;

    case WIDGET_SIG_MENUVIEW_QUIT:
        // return to parent menu
        music_playing_option_advanced_exit( menuview );
        break;
         
    default:
        break;
    }    

    return true;
}

static int _proc_item_fade( menuview_t *menuview, menuview_sig_e signal )
{
    switch ( signal ) 
    {
    case WIDGET_SIG_MENUVIEW_OK:
        _create_fade_adjust_widget();
        break;

    case WIDGET_SIG_MENUVIEW_QUIT:
        // return to parent menu
        music_playing_option_advanced_exit( menuview );
        break;
         
    default:
        break;
    }    

    return true;
}


static bool _create_reptime_adjust_widget( void )
{
    adjust_textdigit_init_t adjust_data = {0};
    menuitem_t * menuitem;
    mengine_config_t *config = &g_scene_playing->engine.config;  

    menuitem = menu_get_cur_item( s_option_advanced );

    //check item
    if ( menuitem == NULL )
    {
        return false;
    }

    adjust_data.str_title_id = menuitem->text_id;

    adjust_data.max_val = 10;
    adjust_data.min_val = 1;
    adjust_data.cur_val = config->ab_count;
    adjust_data.step = 1;
    
    g_scene_playing->adjust = adjust_create( &adjust_data, ADJ_STYLE_TEXTDIGIT, _reptime_adjust_widget_cb);
    if ( NULL == g_scene_playing->adjust )
    {
        print_err("adjust_create failed\n\n");
        return false;
    }
    else
    {
        adjust_paint(g_scene_playing->adjust);
    }

    return true;
}

static bool _delete_reptime_adjust_widget( void )
{
    if(g_scene_playing->adjust != NULL)
    {
        adjust_delete(g_scene_playing->adjust);
        g_scene_playing->adjust = NULL;
    }

    return true;
}

static void _reptime_adjust_widget_cb(adjust_t *adjust, adjust_signal_e signal)
{
    mengine_config_t *config = &g_scene_playing->engine.config;   

    switch(signal)
    {
    case WIDGET_SIG_ADJUST_OK:
        //save value
        adjust_get_current_val( &config->ab_count );

        //set value
        music_set_ab_count( config->ab_count );
        
        //delete
        _delete_reptime_adjust_widget();
        break;
        
    case WIDGET_SIG_ADJUST_CANCEL:
        _delete_reptime_adjust_widget();
        break;

    case WIDGET_SIG_ADJUST_UP:
    case WIDGET_SIG_ADJUST_DOWN:
        //toDo:---
        break;

    default:
        break;
    }
}

static bool _create_repinterval_adjust_widget( void )
{
    adjust_textdigit_init_t adjust_data = {0};
    menuitem_t * menuitem;
    mengine_config_t *config = &g_scene_playing->engine.config;  

    menuitem = menu_get_cur_item( s_option_advanced );

    //check item
    if ( menuitem == NULL )
    {
        return false;
    }

    adjust_data.str_title_id = menuitem->text_id;

    adjust_data.max_val = 7;
    adjust_data.min_val = 1;
    adjust_data.cur_val = config->ab_time;
    adjust_data.step = 1;
    
    g_scene_playing->adjust = adjust_create( &adjust_data, ADJ_STYLE_TEXTDIGIT, _repinterval_adjust_widget_cb);
    if ( NULL == g_scene_playing->adjust )
    {
        print_err("adjust_create failed\n\n");
        return false;
    }
    else
    {
        adjust_paint(g_scene_playing->adjust);
    }

    return true;
}

static bool _delete_repinterval_adjust_widget( void )
{
    if(g_scene_playing->adjust != NULL)
    {
        adjust_delete(g_scene_playing->adjust);
        g_scene_playing->adjust = NULL;
    }

    return true;
}

static void _repinterval_adjust_widget_cb(adjust_t *adjust, adjust_signal_e signal)
{
    mengine_config_t *config = &g_scene_playing->engine.config;   

    switch(signal)
    {
    case WIDGET_SIG_ADJUST_OK:
        //save value
        adjust_get_current_val( &config->ab_time );

        //set value
        music_set_ab_time( config->ab_time );
        
        //delete
        _delete_repinterval_adjust_widget();
        break;
        
    case WIDGET_SIG_ADJUST_CANCEL:
        _delete_repinterval_adjust_widget();
        break;

    case WIDGET_SIG_ADJUST_UP:
    case WIDGET_SIG_ADJUST_DOWN:
        //toDo:---
        break;

    default:
        break;
    }
}

static bool _create_scanspeed_adjust_widget( void )
{
    adjust_textdigit_init_t adjust_data = {0};
    menuitem_t * menuitem;
    unsigned int num;
    mengine_config_t *config = &g_scene_playing->engine.config;  

    menuitem = menu_get_cur_item( s_option_advanced );

    //check item
    if ( menuitem == NULL )
    {
        return false;
    }

    adjust_data.str_title_id = menuitem->text_id;

    adjust_data.max_val = ( SCANSPEED_MAX_ITEMS - 1 );
    adjust_data.min_val = 0;
    adjust_data.cur_val = config->ffb_speed;
    adjust_data.step = 1;
    for( num = 0; num < SCANSPEED_MAX_ITEMS; num ++ )
    {
        adjust_data.str_disp[num] = s_scan_speed[num];
    }
    
    g_scene_playing->adjust = adjust_create( &adjust_data, ADJ_STYLE_TEXTDIGIT, _scanspeed_adjust_widget_cb);
    if ( NULL == g_scene_playing->adjust )
    {
        print_err("adjust_create failed\n\n");
        return false;
    }
    else
    {
        adjust_paint(g_scene_playing->adjust);
    }

    return true;
}

static bool _delete_scanspeed_adjust_widget( void )
{
    if(g_scene_playing->adjust != NULL)
    {
        adjust_delete(g_scene_playing->adjust);
        g_scene_playing->adjust = NULL;
    }

    return true;
}

static void _scanspeed_adjust_widget_cb(adjust_t *adjust, adjust_signal_e signal)
{
    mengine_config_t *config = &g_scene_playing->engine.config;   

    switch(signal)
    {
    case WIDGET_SIG_ADJUST_OK:
        //save value
        adjust_get_current_val( &config->ffb_speed );

        //set value
        music_set_ffb_step( config->ffb_speed );
        
        //delete
        _delete_scanspeed_adjust_widget();
        break;
        
    case WIDGET_SIG_ADJUST_CANCEL:
        _delete_scanspeed_adjust_widget();
        break;

    case WIDGET_SIG_ADJUST_UP:
    case WIDGET_SIG_ADJUST_DOWN:
        //toDo:---
        break;

    default:
        break;
    }
}

static bool _create_fade_adjust_widget( void )
{
    adjust_switch_init_t adjust_switch; 
    menuitem_t * menuitem;
    mengine_config_t *config = &g_scene_playing->engine.config;  

    menuitem = menu_get_cur_item( s_option_advanced );

    //check item
    if ( menuitem == NULL )
    {
        return false;
    }

    adjust_switch.str_title_id = menuitem->text_id;
    
    if( config->fade == true )
    {
        adjust_switch.on_off_status = ADJUST_ON;
    }
    else
    {
        adjust_switch.on_off_status = ADJUST_OFF;
    }
    
    g_scene_playing->adjust = adjust_create( &adjust_switch, ADJ_STYLE_SWITCH, _fade_adjust_widget_cb );
    if ( NULL == g_scene_playing->adjust )
    {
        print_err("adjust_create failed\n\n");
        return false;
    }
    else
    {
        adjust_paint(g_scene_playing->adjust);
    }

    return true;
}

static bool _delete_fade_adjust_widget( void )
{
    if(g_scene_playing->adjust != NULL)
    {
        adjust_delete(g_scene_playing->adjust);
        g_scene_playing->adjust = NULL;
    }

    return true;
}

static void _fade_adjust_widget_cb(adjust_t *adjust, adjust_signal_e signal)
{
    mengine_config_t *config = &g_scene_playing->engine.config;   
    adj_switch_mode_e status; 

    switch(signal)
    {
    case WIDGET_SIG_ADJUST_OK:
        //save value
        adjust_get_switch_status( &status );
        if( status == ADJUST_ON )
        {
            config->fade = true;
        }
        else
        {
            config->fade = false;
        }            

        //set value
        music_set_fade( config->fade );
        
        //delete
        _delete_fade_adjust_widget();
        break;
        
    case WIDGET_SIG_ADJUST_CANCEL:
        _delete_fade_adjust_widget();
        break;

    case WIDGET_SIG_ADJUST_UP:
    case WIDGET_SIG_ADJUST_DOWN:
        //toDo:---
        break;

    default:
        break;
    }
}

