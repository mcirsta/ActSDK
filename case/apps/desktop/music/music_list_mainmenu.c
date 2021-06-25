/********************************************************************************
 *                              USDK(1100)
 *                             Module: app 
 *                 Copyright(c) 2003-2009 Actions Semiconductor,
 *                            All Rights Reserved. 
 * File : music_list_mainmenu.c
 * History:         
 *      <author>    <time>           <version >             <desc>
 *    songzhining    2009-2-25          v1.0               build this file 
 ********************************************************************************/
/*!
* \file     music_list_mainmenu.c
* \brief    music_list_mainmenu widget
* \author   songzhining
*               
* \version 1.0
* \date  2009/02/25
*******************************************************************************/

#include "music_list_mainmenu.h"

menu_t* music_list_mainmenu_init( void );
int music_list_mainmenu_deinit( void );
/* 退出主菜单 */
bool music_list_mainmenu_exit( menuview_t *menuview );

/* listmenu entry */
static int _proc_item_nowplaying( menuview_t *menuview, menuview_sig_e signal );
static int _proc_item_allmusic( menuview_t *menuview, menuview_sig_e signal );
static int _proc_item_artist( menuview_t *menuview, menuview_sig_e signal );
static int _proc_item_album( menuview_t *menuview, menuview_sig_e signal );
static int _proc_item_genre( menuview_t *menuview, menuview_sig_e signal );
static int _proc_item_mylist( menuview_t *menuview, menuview_sig_e signal );
static int _proc_item_dirlist( menuview_t *menuview, menuview_sig_e signal );
static int _proc_item_recfile( menuview_t *menuview, menuview_sig_e signal );
static int _proc_item_level( menuview_t *menuview, menuview_sig_e signal );
static int _proc_item_tag( menuview_t *menuview, menuview_sig_e signal );

static bool _load_config( void );

/* menu list data define */
static menu_t *s_mainmenu = NULL;
static int s_mainmenu_offset = 0;
static int s_mainmenu_current = 0;

static unsigned int s_mainmenu_total = 0;

/* menu items data define */
static style_menuitem_t s_mainmenu_items[] =
{    
    // name, has_child, callback
    { STR_NOWPLAYING,   true,   (item_cb_t)_proc_item_nowplaying, 0, 0, 0, 0, 0 },
    { STR_ALLMUSIC,     true,   (item_cb_t)_proc_item_allmusic, 0, 0, 0, 0, 0 },
    { STR_ARTIST,       true,   (item_cb_t)_proc_item_artist, 0, 0, 0, 0, 0 },
    { STR_ALBUM,        true,   (item_cb_t)_proc_item_album, 0, 0, 0, 0, 0 },
    { STR_GENRE,        true,   (item_cb_t)_proc_item_genre, 0, 0, 0, 0, 0 },
    { STR_MYPLAYLIST,   true,   (item_cb_t)_proc_item_mylist, 0, 0, 0, 0, 0 },  
    { STR_DIRLIST,      true,   (item_cb_t)_proc_item_dirlist, 0, 0, 0, 0, 0 },   
    { STR_RECLIST,      true,   (item_cb_t)_proc_item_recfile, 0, 0, 0, 0, 0 },
    { STR_MYRATING,     true,   (item_cb_t)_proc_item_level, 0, 0, 0, 0, 0 },   
    { STR_TAGLIST,      true,   (item_cb_t)_proc_item_tag, 0, 0, 0, 0, 0 },
};

/* menu list max items */
#define MENU_LIST_MAX_ITEMS  ( sizeof(s_mainmenu_items)/sizeof(style_menuitem_t) )

/* menu items visible config */
static char *s_mainmenu_config[MENU_LIST_MAX_ITEMS] =
{
    CFG_MUSIC_NOWPLAYING_VISIBLE,  
    CFG_MUSIC_ALLMUSIC_VISIBLE,
    CFG_MUSIC_ARTIST_VISIBLE, 
    CFG_MUSIC_ALBUM_VISIBLE,
    CFG_MUSIC_GENRE_VISIBLE,
    CFG_MUSIC_MYPLAYLIST_VISIBLE,
    CFG_MUSIC_DIRLIST_VISIBLE,
    CFG_MUSIC_RECLIST_VISIBLE,
    CFG_MUSIC_MYRATING_VISIBLE,
    CFG_MUSIC_TAGLIST_VISIBLE,
};

/* menu items visible flag */
static bool s_mainmenu_visible[MENU_LIST_MAX_ITEMS];


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
        result = get_config( s_mainmenu_config[num], (char*)&value, sizeof(bool) );
        if( result < 0 )
        {
            value = true;
        }
        
        s_mainmenu_visible[num] = value;
    }

    return true;
}

/* 进入主菜单 */
bool music_list_mainmenu_enter( menuview_t *menuview, fusion_enter_e fenter )
{
    menu_t *cur_menu;
    fusion_effect_t eff;

    if( fenter < FUSION_MAX )
    {
        if(fenter == FUSION_FORWARD)
        {
            fusion_get_config(FUSION_TYPE_APP_CHANGE, fenter, &eff);
        }
        else
        {
            fusion_get_config(FUSION_TYPE_COMMON, fenter, &eff);
        }
        
        fusion_set_effect(&eff);        
        scene_fusion_set_first_frame(g_scene_list->scene); 
    }   

    cur_menu = music_list_mainmenu_init();
    menuview_set_menu( menuview, cur_menu );
    
    if ( menuview->type != MENUVIEW_MENULIST_TEXT )
    {
        menuview_load_style( menuview, MENUVIEW_MENULIST_TEXT );
    }
    
    menuview_show( menuview );
    menuview_paint( menuview );

    if( fenter < FUSION_MAX )
    {
        scene_fusion_set_last_frame(g_scene_list->scene);
        fusion_display(NULL);
    }

    return true;
}

/* 退出主菜单 */
bool music_list_mainmenu_exit( menuview_t *menuview )
{
    //clear filter string
    plistclass_set_filter( NULL );    

    //delete main menu
    music_list_mainmenu_deinit();

    //reset index
    s_mainmenu_offset = 0;
    s_mainmenu_current = 0;

    //exit menu_ui
    music_list_widget_menu_exit( menuview, NULL );
    
    return true;
}

menu_t* music_list_mainmenu_init( void )
{
    menuitem_t item;
    int num;
    int start;
    bool nowplaying;
    
    //initial menu    
    if( s_mainmenu == NULL )
    {
        //load visible config
        _load_config();
        
        //create menu
        s_mainmenu = menu_create();

        //load style
        menu_load_style( MAIN_MENU_RESGROUP, s_mainmenu_items, MENU_LIST_MAX_ITEMS );

        //check now playing
        nowplaying = music_get_nowplaying();//plistclass_get_item_num(PLIST_NOWPLAYING);

        if ( nowplaying == false )
        {
            start = 1;
            s_mainmenu_total = MENU_LIST_MAX_ITEMS - 1;
        }
        else
        {
            //add now playing item, change index
            if ( s_mainmenu_total == ( MENU_LIST_MAX_ITEMS - 1 ) )
            {
                s_mainmenu_current += 1;
            }
            
            start = 0;
            s_mainmenu_total = MENU_LIST_MAX_ITEMS;
        }

        //init items
        memset( &item, 0, sizeof(menuitem_t) );

        for ( num=start; num<(int)MENU_LIST_MAX_ITEMS; num++ )
        {
            if ( s_mainmenu_visible[num] == true )
            {
                item.text_id = s_mainmenu_items[num].str_id;
                item.has_child = s_mainmenu_items[num].has_child;
                item.callback = s_mainmenu_items[num].callback;
                
                menu_append_item( s_mainmenu, &item );
            }
        }

        //init offset index
        menu_set_offset( s_mainmenu, s_mainmenu_offset );

        //init current index
        menu_set_cur_index( s_mainmenu, s_mainmenu_current );  
    }

    return s_mainmenu;
}

int music_list_mainmenu_deinit( void )
{    
    //delete menu    
    if( s_mainmenu != NULL )
    {
        //save index
        s_mainmenu_offset = s_mainmenu->offset;
        s_mainmenu_current = s_mainmenu->current;
        
        //create menu
        menu_delete( s_mainmenu );
        s_mainmenu = NULL;
    }

    return true;
}

static int _proc_item_nowplaying( menuview_t *menuview, menuview_sig_e signal )
{
    menu_t *cur_menu;
    
    switch ( signal ) 
    {
    case WIDGET_SIG_MENUVIEW_OK:
        g_file_index = -1;
      
        //check file
        if ( music_list_check_file( NULL, g_file_index ) == false )
        {                
            music_list_mainmenu_deinit();
            cur_menu = music_list_mainmenu_init();
            menuview_set_menu( menuview, cur_menu );
            menuview_paint( menuview );                
            break;
        }

        //exit menu
        music_list_mainmenu_deinit();  
        g_scene_list->exit_type = SCENE_LIST_NEXT;
        music_list_widget_menu_exit( menuview, NULL );
        break;

    case WIDGET_SIG_MENUVIEW_QUIT:
        // return to parent menu
        music_list_mainmenu_exit( menuview );
        break;
         
    default:
        break;
    }    

    return true;
}

static int _proc_item_allmusic( menuview_t *menuview, menuview_sig_e signal )
{        
    switch ( signal ) 
    {
    case WIDGET_SIG_MENUVIEW_OK:
        //set level info
        plistclass_next_level( g_plist_level, LEVEL_ONE_ALLMUSIC, LEVEL_TWO_LIST );
    
        //enter filelist
        music_list_mainmenu_deinit();
        music_list_filelist_enter( menuview, FUSION_FORWARD );
        break;

    case WIDGET_SIG_MENUVIEW_QUIT:
        // return to parent menu
        music_list_mainmenu_exit( menuview );
        break;
         
    default:
        break;
    }        

    return true;
}

static int _proc_item_artist( menuview_t *menuview, menuview_sig_e signal )
{        
    switch ( signal ) 
    {
    case WIDGET_SIG_MENUVIEW_OK:
        //set level info
        plistclass_next_level( g_plist_level, LEVEL_ONE_ARTISTS, LEVEL_TWO_LIST );
    
        //enter filelist
        music_list_mainmenu_deinit();
        music_list_category_enter( menuview, FUSION_FORWARD );
        break;

    case WIDGET_SIG_MENUVIEW_QUIT:
        // return to parent menu
        music_list_mainmenu_exit( menuview );
        break;
         
    default:
        break;
    }    

    return true;
}

static int _proc_item_album( menuview_t *menuview, menuview_sig_e signal )
{        
    switch ( signal ) 
    {
    case WIDGET_SIG_MENUVIEW_OK:
        //set level info
        plistclass_next_level( g_plist_level, LEVEL_ONE_ALBUMS, LEVEL_TWO_LIST );
    
        //enter filelist
        music_list_mainmenu_deinit();
        music_list_category_enter( menuview, FUSION_FORWARD );
        break;

    case WIDGET_SIG_MENUVIEW_QUIT:
        // return to parent menu
        music_list_mainmenu_exit( menuview );
        break;
         
    default:
        break;
    }    

    return true;
}

static int _proc_item_genre( menuview_t *menuview, menuview_sig_e signal )
{        
    switch ( signal ) 
    {
    case WIDGET_SIG_MENUVIEW_OK:
        //set level info
        plistclass_next_level( g_plist_level, LEVEL_ONE_GENRES, LEVEL_TWO_LIST );
    
        //enter filelist
        music_list_mainmenu_deinit();
        music_list_category_enter( menuview, FUSION_FORWARD );
        break;

    case WIDGET_SIG_MENUVIEW_QUIT:
        // return to parent menu
        music_list_mainmenu_exit( menuview );
        break;
         
    default:
        break;
    }    

    return true;
}

static int _proc_item_mylist( menuview_t *menuview, menuview_sig_e signal )
{        
    switch ( signal ) 
    {
    case WIDGET_SIG_MENUVIEW_OK:        
        //set level info
        plistclass_next_level( g_plist_level, LEVEL_ONE_MY_PLAYLIST, LEVEL_TWO_LIST );
        
        //enter artist list
        music_list_mainmenu_deinit();
        music_list_mylist_enter( menuview, FUSION_FORWARD );
        break;

    case WIDGET_SIG_MENUVIEW_QUIT:
        // return to parent menu
        music_list_mainmenu_exit( menuview );
        break;
         
    default:
        break;
    }    

    return true;
}

static int _proc_item_dirlist( menuview_t *menuview, menuview_sig_e signal )
{
    switch ( signal ) 
    {
    case WIDGET_SIG_MENUVIEW_OK:  
        //enter dirlist
        music_list_mainmenu_deinit();
        memset( g_dirlist_top, 0, sizeof(g_dirlist_top) );
        music_list_dirlist_enter( menuview, FUSION_FORWARD );
        break;

    case WIDGET_SIG_MENUVIEW_QUIT:
        // return to parent menu
        music_list_mainmenu_exit( menuview );
        break;
         
    default:
        break;
    }    

    return true;
}

static int _proc_item_level( menuview_t *menuview, menuview_sig_e signal )
{        
    switch ( signal ) 
    {
    case WIDGET_SIG_MENUVIEW_OK:
        //set level info
        plistclass_next_level( g_plist_level, LEVEL_ONE_MY_RATING, LEVEL_TWO_LIST );
        
        //enter level menu
        music_list_mainmenu_deinit();
        music_list_level_enter( menuview, FUSION_FORWARD );
        break;

    case WIDGET_SIG_MENUVIEW_QUIT:
        // return to parent menu
        music_list_mainmenu_exit( menuview );
        break;
         
    default:
        break;
    }    

    return true;
}

static int _proc_item_recfile( menuview_t *menuview, menuview_sig_e signal )
{
    switch ( signal ) 
    {
    case WIDGET_SIG_MENUVIEW_OK:        
        //set level info
        plistclass_next_level( g_plist_level, LEVEL_ONE_MY_RECORDS, LEVEL_TWO_VOCREC );
        
        //enter recfile menu
        music_list_mainmenu_deinit();
        music_list_recfile_enter( menuview, FUSION_FORWARD );
        break;

    case WIDGET_SIG_MENUVIEW_QUIT:
        // return to parent menu
        music_list_mainmenu_exit( menuview );
        break;
         
    default:
        break;
    }    

    return true;
}

static int _proc_item_tag( menuview_t *menuview, menuview_sig_e signal )
{
    switch ( signal ) 
    {
    case WIDGET_SIG_MENUVIEW_OK:       
        //set level info
        plistclass_next_level( g_plist_level, LEVEL_ONE_MUSICTAGLIST, LEVEL_TWO_LIST );
             
        //enter recfile menu
        music_list_mainmenu_deinit();
        music_list_filelist_enter( menuview, FUSION_FORWARD );
        break;

    case WIDGET_SIG_MENUVIEW_QUIT:
        // return to parent menu
        music_list_mainmenu_exit( menuview );
        break;
         
    default:
        break;
    }    

    return true;
}
