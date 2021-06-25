/********************************************************************************
 *                              USDK(1100)
 *                             Module: app 
 *                 Copyright(c) 2003-2009 Actions Semiconductor,
 *                            All Rights Reserved. 
 * File : music_playing_option.c
 * History:         
 *      <author>    <time>           <version >             <desc>
 *    songzhining    2009-2-25          v1.0               build this file 
 ********************************************************************************/
/*!
* \file     music_playing_option.c
* \brief    music_playing_option widget
* \author   songzhining
*               
* \version 1.0
* \date  2009/02/25
*******************************************************************************/

#include "music_playing_option.h"

/* 退出主菜单 */
bool music_playing_option_exit( menuview_t *menuview );

/* 初始化菜单数据 */
menu_t* music_playing_option_init( void );

/* 删除菜单数据 */
int music_playing_option_deinit( void );

/* listmenu entry */
static int _proc_item_seleq( menuview_t *menuview, menuview_sig_e signal );
static int _proc_item_playmode( menuview_t *menuview, menuview_sig_e signal );
static int _proc_item_displaymode( menuview_t *menuview, menuview_sig_e signal );
static int _proc_item_addlist( menuview_t *menuview, menuview_sig_e signal );
static int _proc_item_setrating( menuview_t *menuview, menuview_sig_e signal );
static int _proc_item_addtag( menuview_t *menuview, menuview_sig_e signal );
static int _proc_item_abreq( menuview_t *menuview, menuview_sig_e signal );
static int _proc_item_playspeed( menuview_t *menuview, menuview_sig_e signal );
static int _proc_item_delete( menuview_t *menuview, menuview_sig_e signal );
static int _proc_item_advanced( menuview_t *menuview, menuview_sig_e signal );
static int _proc_item_setupeq( menuview_t *menuview, menuview_sig_e signal );
static int _proc_item_info( menuview_t *menuview, menuview_sig_e signal );

static bool _create_playspeed_adjust_widget( void );
static bool _delete_playspeed_adjust_widget( void );
static void _playspeed_adjust_widget_cb(adjust_t *adjust, adjust_signal_e signal);

static bool _create_del_dialog_widget( void );
static bool _delete_del_dialog_widget( void );
static void _del_dialog_widget_cb(dialog_t *dialog, widget_sig_dialog_e signal);

static bool _create_deleting_dialog_widget( void );
static bool _delete_deleting_dialog_widget( void );
static void _deleting_dialog_widget_cb(dialog_t *dialog, widget_sig_dialog_e signal);

static bool _load_config( void );

/* menu option data define */
static menu_t *s_playing_option = NULL;
static int s_playing_option_offset = 0;
static int s_playing_option_current = 0;

/* menu items data define */
static style_menuitem_t s_playing_option_items[] =
{    
    // name, has_child, callback
    { STR_SELEQ,   true,  (item_cb_t)_proc_item_seleq, 0, 0, 0, 0, 0 },
    { STR_PLAYMODE,     true,   (item_cb_t)_proc_item_playmode, 0, 0, 0, 0, 0 },
    { STR_DISPLAYMODE,       true,   (item_cb_t)_proc_item_displaymode, 0, 0, 0, 0, 0 },
    { STR_ADDLIST,        true,   (item_cb_t)_proc_item_addlist, 0, 0, 0, 0, 0 },
    { STR_SETRATING,        true,   (item_cb_t)_proc_item_setrating, 0, 0, 0, 0, 0 },
    { STR_ADDTAG,   false,   (item_cb_t)_proc_item_addtag, 0, 0, 0, 0, 0 },  
    { STR_ABREP,      false,   (item_cb_t)_proc_item_abreq, 0, 0, 0, 0, 0 },   
    { STR_PLAYSPEED,      false,   (item_cb_t)_proc_item_playspeed, 0, 0, 0, 0, 0 },
    { STR_DELETE,     false,   (item_cb_t)_proc_item_delete, 0, 0, 0, 0, 0 },   
    { STR_ADVANCED,      true,   (item_cb_t)_proc_item_advanced, 0, 0, 0, 0, 0 },
    { STR_SETUPEQ,      true,   (item_cb_t)_proc_item_setupeq, 0, 0, 0, 0, 0 },
    { STR_INFO,      true,   (item_cb_t)_proc_item_info, 0, 0, 0, 0, 0 },
};

/* menu option max items */
#define MENU_LIST_MAX_ITEMS  ( sizeof(s_playing_option_items)/sizeof(style_menuitem_t) )

/* menu items visible config */
static char *s_playing_option_config[MENU_LIST_MAX_ITEMS] =
{
    CFG_MUSIC_SELEQ_VISIBLE,   
    CFG_MUSIC_PLAYMODE_VISIBLE,
    CFG_MUSIC_DISPLAYMODE_VISIBLE,
    CFG_MUSIC_ADDLIST_VISIBLE,
    CFG_MUSIC_SETRATING_VISIBLE,
    CFG_MUSIC_ADDTAG_VISIBLE,
    CFG_MUSIC_ABREP_VISIBLE,
    CFG_MUSIC_PLAYSPEED_VISIBLE,
    CFG_MUSIC_DELETE_VISIBLE,
    CFG_MUSIC_ADVANCED_VISIBLE,
    CFG_MUSIC_SETUPEQ_VISIBLE,
    CFG_MUSIC_INFO_VISIBLE,
};

/* menu items visible flag */
static bool s_playing_option_visible[MENU_LIST_MAX_ITEMS];


static bool _load_config( void )
{
    static bool s_loaded = false;
    int result;
    bool value = false;
    int num;
    
    if ( s_loaded == true )
    {
        return true;
    }
    else
    {
        s_loaded = true;
    }
    
    //load visible flag
    for ( num=0; num<(int)MENU_LIST_MAX_ITEMS; num++ )
    {
        result = get_config( s_playing_option_config[num], (char*)&value, sizeof(bool) );
        if( result < 0 )
        {
            value = true;
        }
        
        s_playing_option_visible[num] = value;
    }

    return true;
}


/* 进入主菜单 */
bool music_playing_option_enter( menuview_t *menuview )
{
    menu_t *cur_menu;

    cur_menu = music_playing_option_init();
    menuview_set_menu( menuview, cur_menu );
    
    if ( menuview->type != MENUVIEW_OPTION )
    {
        menuview_load_style( menuview, MENUVIEW_OPTION );
    }
    menuview_paint( menuview );

    return true;
}

/* 退出主菜单 */
bool music_playing_option_exit( menuview_t *menuview )
{
    //delete main menu
    music_playing_option_deinit();

    //reset index
    s_playing_option_offset = 0;
    s_playing_option_current = 0;

    //exit menu_ui
    music_playing_widget_option_exit( menuview, NULL );
    
    return true;
}

menu_t* music_playing_option_init( void )
{
    menuitem_t item;
    unsigned int num;
    
    //initial menu    
    if( s_playing_option == NULL )
    {
        //load visible config
        _load_config();
        
        //create menu
        s_playing_option = menu_create();

        //load style
        menu_load_style( OPTION_MENU_RESGROUP, s_playing_option_items, MENU_LIST_MAX_ITEMS );
        
        //init items
        memset( &item, 0, sizeof(menuitem_t) );
        
        for ( num=0; num<MENU_LIST_MAX_ITEMS; num++ )
        {
            if ( s_playing_option_visible[num] == true )
            {
                item.text_id = s_playing_option_items[num].str_id;
                item.has_child = s_playing_option_items[num].has_child;
                item.callback = s_playing_option_items[num].callback;
                
                menu_append_item( s_playing_option, &item );
            }
        }

        //init current index
        menu_set_offset( s_playing_option, s_playing_option_offset );
        menu_set_cur_index( s_playing_option, s_playing_option_current );
    }

    return s_playing_option;
}

int music_playing_option_deinit( void )
{    
    //delete menu    
    if( s_playing_option != NULL )
    {
        //save index
        s_playing_option_offset = s_playing_option->offset;
        s_playing_option_current = s_playing_option->current;
        
        //create menu
        menu_delete( s_playing_option );
        s_playing_option = NULL;
    }

    return true;
}

static int _proc_item_seleq( menuview_t *menuview, menuview_sig_e signal )
{
    switch ( signal ) 
    {
    case WIDGET_SIG_MENUVIEW_OK:
        // enter menu
        music_playing_option_deinit();
        music_playing_option_seleq_enter( menuview );
        break;

    case WIDGET_SIG_MENUVIEW_QUIT:
        // return to parent menu
        music_playing_option_exit( menuview );
        break;
         
    default:
        break;
    }    

    return true;
}

static int _proc_item_playmode( menuview_t *menuview, menuview_sig_e signal )
{
    switch ( signal ) 
    {
    case WIDGET_SIG_MENUVIEW_OK:
        // enter menu
        music_playing_option_deinit();
        music_playing_option_playmode_enter( menuview );
        break;

    case WIDGET_SIG_MENUVIEW_QUIT:
        // return to parent menu
        music_playing_option_exit( menuview );
        break;
         
    default:
        break;
    }    

    return true;
}

static int _proc_item_displaymode( menuview_t *menuview, menuview_sig_e signal )
{
    switch ( signal ) 
    {
    case WIDGET_SIG_MENUVIEW_OK:
        // enter menu
        music_playing_option_deinit();
        music_playing_option_displaymode_enter( menuview );
        break;

    case WIDGET_SIG_MENUVIEW_QUIT:
        // return to parent menu
        music_playing_option_exit( menuview );
        break;
         
    default:
        break;
    }    

    return true;
}

static int _proc_item_addlist( menuview_t *menuview, menuview_sig_e signal )
{
    switch ( signal ) 
    {
    case WIDGET_SIG_MENUVIEW_OK:
        //enter
        music_playing_option_deinit();
        music_playing_option_mylist_enter( menuview );
        break;

    case WIDGET_SIG_MENUVIEW_QUIT:
        // return to parent menu
        music_playing_option_exit( menuview );
        break;
         
    default:
        break;
    }    

    return true;
}

static int _proc_item_setrating( menuview_t *menuview, menuview_sig_e signal )
{
    switch ( signal ) 
    {
    case WIDGET_SIG_MENUVIEW_OK:
        // enter menu
        music_playing_option_deinit();
        music_playing_option_level_enter( menuview );
        break;

    case WIDGET_SIG_MENUVIEW_QUIT:
        // return to parent menu
        music_playing_option_exit( menuview );
        break;
         
    default:
        break;
    }    

    return true;
}

static int _proc_item_addtag( menuview_t *menuview, menuview_sig_e signal )
{
    eg_audio_tag_t audio_tag;
    plist_detail_info_t *id3_info;
    bool bret;
    dialog_init_t dialog_init;
    
    switch ( signal ) 
    {
    case WIDGET_SIG_MENUVIEW_OK:
        //get mark
        bret = music_make_bookmark( &audio_tag );
        if( bret == true )
        {
            //add bookmark      
            id3_info = g_scene_playing->id3_info;
            id3_info->markinfo.time = audio_tag.time;
            id3_info->markinfo.mark_param_len = sizeof(audio_tag.buf);
            id3_info->markinfo.mark_param = audio_tag.buf;
          
            bret = plistclass_add_item_to_bmlist( id3_info );
        }
        
        if ( bret == false )
        {
            //模态提示框
            if (g_scene_playing->str_add_fail != NULL)
            {
                dialog_init.head = DIALOG_HEAD_WARN;
                dialog_init.str_id = g_scene_playing->str_add_fail->str_id;
                dialog_modal( &dialog_init );
            }
        }
        else
        {
            // return to parent menu
            music_playing_option_exit( menuview );
        }
        break;

    case WIDGET_SIG_MENUVIEW_QUIT:
        // return to parent menu
        music_playing_option_exit( menuview );
        break;
         
    default:
        break;
    }    

    return true;
}

static int _proc_item_abreq( menuview_t *menuview, menuview_sig_e signal )
{
    switch ( signal ) 
    {
    case WIDGET_SIG_MENUVIEW_OK:
        if(g_scene_playing->engine.eg_status.play_status != EG_PLAY_AB )
        {
            music_start_ab();
            music_get_engine_info( &g_scene_playing->engine );
        }
        else
        {
        }
        // return to parent menu
        music_playing_option_exit( menuview );
        
        //paint config info
        playing_paint_config( g_scene_playing );
        break;

    case WIDGET_SIG_MENUVIEW_QUIT:
        // return to parent menu
        music_playing_option_exit( menuview );
        break;
         
    default:
        break;
    }    

    return true;
}

static bool _create_playspeed_adjust_widget( void )
{
    adjust_textdigit_init_t adjust_data = {0};
    menuitem_t * menuitem;
    mengine_config_t *config = &g_scene_playing->engine.config;    
    bool bret = true;

    menuitem = menu_get_cur_item( s_playing_option );

    //check item
    if ( menuitem == NULL )
    {
        return false;
    }

    adjust_data.str_title_id = menuitem->text_id;

    //check play speed support
    bret = music_set_playspeed( config->play_speed );
    if ( bret == false )
    {
        adjust_data.max_val = 0;
        adjust_data.min_val = 0;
        adjust_data.cur_val = 0;
        adjust_data.step = 0;
    }
    else
    {
        adjust_data.max_val = 4;
        adjust_data.min_val = -4;
        adjust_data.cur_val = config->play_speed;
        adjust_data.step = 1;
    }    
    
    g_scene_playing->adjust = adjust_create( &adjust_data, ADJ_STYLE_TEXTDIGIT, _playspeed_adjust_widget_cb);
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

static bool _delete_playspeed_adjust_widget( void )
{
    if(g_scene_playing->adjust != NULL)
    {
        adjust_delete(g_scene_playing->adjust);
        g_scene_playing->adjust = NULL;
    }

    return true;
}

static void _playspeed_adjust_widget_cb(adjust_t *adjust, adjust_signal_e signal)
{
    mengine_config_t *config = &g_scene_playing->engine.config;   

    switch(signal)
    {
    case WIDGET_SIG_ADJUST_OK:
        //save value
        adjust_get_current_val( &config->play_speed );

        //set value
        music_set_playspeed( config->play_speed );
        
        //delete
        _delete_playspeed_adjust_widget();
        
        //paint config info
        playing_paint_config( g_scene_playing );
        break;
        
    case WIDGET_SIG_ADJUST_CANCEL:
        _delete_playspeed_adjust_widget();
        break;

    case WIDGET_SIG_ADJUST_UP:
    case WIDGET_SIG_ADJUST_DOWN:
        //toDo:---
        break;

    default:
        break;
    }
}

static int _proc_item_playspeed( menuview_t *menuview, menuview_sig_e signal )
{
    switch ( signal ) 
    {
    case WIDGET_SIG_MENUVIEW_OK:
        _create_playspeed_adjust_widget();
        break;

    case WIDGET_SIG_MENUVIEW_QUIT:
        // return to parent menu
        music_playing_option_exit( menuview );
        break;
         
    default:
        break;
    }    

    return true;
}

static bool _create_del_dialog_widget( void )
{
    dialog_init_t dialog_init;        
    dialog_init.head = DIALOG_HEAD_QUERY;
    dialog_init.str_id = g_scene_playing->str_del_file->str_id;
    dialog_init.def_sel = WIDGET_SIG_DIALOG_CANCAL;
    
    /* init and callback function */
    g_scene_playing->dialog = dialog_create( &dialog_init, _del_dialog_widget_cb);
    if ( NULL == g_scene_playing->dialog )
    {
        print_err("dialog_create failed\n\n");
        return false;
    }
    else
    {
        dialog_paint( g_scene_playing->dialog );
    }

    return true;
}

static bool _delete_del_dialog_widget( void )
{
    if(g_scene_playing->dialog != NULL)
    {
        dialog_delete( g_scene_playing->dialog );
        g_scene_playing->dialog = NULL;
    }

    return true;
}

static void _del_dialog_widget_cb(dialog_t *dialog, widget_sig_dialog_e signal)
{
    bool bret;
    dialog_init_t dialog_init;
    
    switch ( signal )
    {
    case WIDGET_SIG_DIALOG_OK:
        _delete_del_dialog_widget();

        //display deleting dialog
        _create_deleting_dialog_widget();
        gui_screen_update();
        
        //delete file
        if ( g_scene_playing->id3_info != NULL )
        {
            bret = music_delete_file( g_scene_playing->id3_info->fullpath );
            if ( bret == false )
            {
                //模态提示框
                if (g_scene_playing->str_del_fail != NULL)
                {
                    dialog_init.head = DIALOG_HEAD_WARN;
                    dialog_init.str_id = g_scene_playing->str_del_fail->str_id;
                    dialog_modal( &dialog_init );
                }
            }
        }
        
        //update
        playing_update_status();
        
        //paint file info
        playing_paint_file_info( g_scene_playing );
        
        //paint config info
        playing_paint_config( g_scene_playing );  
        
        //paint effect
        playing_paint_effect( g_scene_playing );
        
        //paint progress
        playing_paint_progress( g_scene_playing );        
        
        //paint button
        playing_paint_button( g_scene_playing ); 

        //deleting dialog hide
        _delete_deleting_dialog_widget();
        break;

    case WIDGET_SIG_DIALOG_CANCAL:     
        _delete_del_dialog_widget();
        break;

    default:
        break;
    }
}

static bool _create_deleting_dialog_widget( void )
{
    dialog_init_t dialog_init;        
    dialog_init.head = DIALOG_HEAD_INFO;
    dialog_init.str_id = g_scene_playing->str_deleting->str_id;
    
    /* init and callback function */
    g_scene_playing->dialog = dialog_create( &dialog_init, _deleting_dialog_widget_cb);
    if ( NULL == g_scene_playing->dialog )
    {
        print_err("dialog_create failed\n\n");
        return false;
    }
    else
    {
        dialog_paint( g_scene_playing->dialog );
    }

    return true;
}

static bool _delete_deleting_dialog_widget( void )
{
    if(g_scene_playing->dialog != NULL)
    {
        dialog_delete( g_scene_playing->dialog );
        g_scene_playing->dialog = NULL;
    }

    return true;
}

static void _deleting_dialog_widget_cb(dialog_t *dialog, widget_sig_dialog_e signal)
{
}

static int _proc_item_delete( menuview_t *menuview, menuview_sig_e signal )
{
    switch ( signal ) 
    {
    case WIDGET_SIG_MENUVIEW_OK:
        _create_del_dialog_widget();
        break;

    case WIDGET_SIG_MENUVIEW_QUIT:
        // return to parent menu
        music_playing_option_exit( menuview );
        break;
         
    default:
        break;
    }    

    return true;
}

static int _proc_item_advanced( menuview_t *menuview, menuview_sig_e signal )
{
    switch ( signal ) 
    {
    case WIDGET_SIG_MENUVIEW_OK:
        // enter menu
        music_playing_option_deinit();
        music_playing_option_advanced_enter( menuview );    
        break;

    case WIDGET_SIG_MENUVIEW_QUIT:
        // return to parent menu
        music_playing_option_exit( menuview );
        break;
         
    default:
        break;
    }    

    return true;
}

static int _proc_item_setupeq( menuview_t *menuview, menuview_sig_e signal )
{
    switch ( signal ) 
    {
    case WIDGET_SIG_MENUVIEW_OK:
        // enter menu
        music_playing_option_deinit();
        music_playing_option_setupeq_enter( menuview );
        break;

    case WIDGET_SIG_MENUVIEW_QUIT:
        // return to parent menu
        music_playing_option_exit( menuview );
        break;
         
    default:
        break;
    }    

    return true;
}

static int _proc_item_info( menuview_t *menuview, menuview_sig_e signal )
{
    switch ( signal ) 
    {
    case WIDGET_SIG_MENUVIEW_OK:
        //enter
        music_playing_option_deinit();
        music_playing_option_info_enter( menuview );
        break;

    case WIDGET_SIG_MENUVIEW_QUIT:
        // return to parent menu
        music_playing_option_exit( menuview );
        break;
         
    default:
        break;
    }    

    return true;
}

bool playing_delete_process( void )
{
    return _create_del_dialog_widget();
}

