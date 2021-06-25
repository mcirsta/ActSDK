/********************************************************************************
 *                              USDK(1100)
 *                             Module: app 
 *                 Copyright(c) 2003-2009 Actions Semiconductor,
 *                            All Rights Reserved. 
 * File : music_playing_option_setupeq.c
 * History:         
 *      <author>    <time>           <version >             <desc>
 *    songzhining    2009-2-25          v1.0               build this file 
 ********************************************************************************/
/*!
* \file     music_playing_option_setupeq.c
* \brief    music_playing_option_setupeq widget
* \author   songzhining
*               
* \version 1.0
* \date  2009/02/25
*******************************************************************************/

#include "music_playing_option_setupeq.h"

/* 退出主菜单 */
bool music_playing_option_setupeq_exit( menuview_t *menuview );

/* 初始化菜单数据 */
menu_t* music_playing_option_setupeq_init( void );

/* 删除菜单数据 */
int music_playing_option_setupeq_deinit( void );

/* listmenu entry */
static int _proc_item_usereq( menuview_t *menuview, menuview_sig_e signal );
static int _proc_item_srsuser( menuview_t *menuview, menuview_sig_e signal );

/* menu list data define */
static menu_t *s_option_setupeq = NULL;
static int s_option_setupeq_offset = 0;
static int s_option_setupeq_current = 0;

/* menu items data define */
static style_menuitem_t s_option_setupeq_items[] =
{    
    // name, has_child, callback
    { STR_USEREQ,   false,  (item_cb_t)_proc_item_usereq, 0, 0, 0, 0, 0 },
    { STR_SRSUSER,   false,   (item_cb_t)_proc_item_srsuser, 0, 0, 0, 0, 0 },
};

/* menu list max items */
#define MENU_OPTION_SETUPEQ_MAX_ITEMS  ( sizeof(s_option_setupeq_items)/sizeof(style_menuitem_t) )

/* eq value for save */
static mengine_eq_t s_save_eq;


/* 进入主菜单 */
bool music_playing_option_setupeq_enter( menuview_t *menuview )
{
    menu_t *cur_menu;

    cur_menu = music_playing_option_setupeq_init();
    menuview_set_menu( menuview, cur_menu );
    
    if ( menuview->type != MENUVIEW_OPTION )
    {
        menuview_load_style( menuview, MENUVIEW_OPTION );
    }
    menuview_paint( menuview );

    return true;
}

/* 退出主菜单 */
bool music_playing_option_setupeq_exit( menuview_t *menuview )
{
    //delete main menu
    music_playing_option_setupeq_deinit();

    //reset index
    s_option_setupeq_offset = 0;
    s_option_setupeq_current = 0;

    //return
    music_playing_option_enter( menuview );
    
    return true;
}

menu_t* music_playing_option_setupeq_init( void )
{
    menuitem_t item;
    unsigned int num;
    
    //initial menu    
    if( s_option_setupeq == NULL )
    {
        //create menu
        s_option_setupeq = menu_create();

        //load style
        menu_load_style( SETEQ_MENU_RESGROUP, s_option_setupeq_items, MENU_OPTION_SETUPEQ_MAX_ITEMS );
        
        //init items
        memset( &item, 0, sizeof(menuitem_t) );
        
        for ( num=0; num<MENU_OPTION_SETUPEQ_MAX_ITEMS; num++ )
        {
            item.text_id = s_option_setupeq_items[num].str_id;
            item.has_child = s_option_setupeq_items[num].has_child;
            item.callback = s_option_setupeq_items[num].callback;
            
            menu_append_item( s_option_setupeq, &item );
        }

        //init index
        menu_set_offset( s_option_setupeq, s_option_setupeq_offset );
        menu_set_cur_index( s_option_setupeq, s_option_setupeq_current );
    }

    return s_option_setupeq;
}


int music_playing_option_setupeq_deinit( void )
{    
    //delete menu    
    if( s_option_setupeq != NULL )
    {
        //save index
        s_option_setupeq_offset = s_option_setupeq->offset;
        s_option_setupeq_current = s_option_setupeq->current;
        
        //create menu
        menu_delete( s_option_setupeq );
        s_option_setupeq = NULL;
    }

    return true;
}

static int _proc_item_usereq( menuview_t *menuview, menuview_sig_e signal )
{
    switch ( signal ) 
    {
    case WIDGET_SIG_MENUVIEW_OK:
        com_create_usereq_widget(); 
        break;

    case WIDGET_SIG_MENUVIEW_QUIT:
        // return to parent menu
        music_playing_option_setupeq_exit( menuview );
        break;
         
    default:
        break;
    }    

    return true;
}


static int _proc_item_srsuser( menuview_t *menuview, menuview_sig_e signal )
{
    switch ( signal ) 
    {
    case WIDGET_SIG_MENUVIEW_OK:
        com_create_srsuser_widget(); 
        break;

    case WIDGET_SIG_MENUVIEW_QUIT:
        // return to parent menu
        music_playing_option_setupeq_exit( menuview );
        break;
         
    default:
        break;
    }    

    return true;
}

bool com_create_usereq_widget( void )
{
    mengine_eq_t *eq_val = &g_scene_playing->engine.config.eq_val;
    
    if ( g_scene_playing->usereq == NULL )
    {
        g_scene_playing->usereq = usereq_widget_create( eq_val->user_param, 
                                                    g_scene_playing->bmp_bg, com_usereq_widget_cb );                    
        if( g_scene_playing->usereq == NULL )
        {
            print_err(); 
            return false;
        }
        else
        {
            usereq_widget_paint( g_scene_playing->usereq );

            //save eq
            s_save_eq = (*eq_val);            

            //set eq
            eq_val->eq_mode = EQ_USR_MODE;
            music_set_eq( eq_val );
        }    
    }   
    
    return true;
}

bool com_delete_usereq_widget( void )
{
    if(g_scene_playing->usereq != NULL)
    {
        usereq_widget_delete( g_scene_playing->usereq );
        g_scene_playing->usereq = NULL;
        
        //restore eq
        music_set_eq( &s_save_eq );
    }

    return true;
}

void com_usereq_widget_cb( usereq_widget_t *usereq_widget, usereq_widget_sig_e signal )
{
    mengine_eq_t *eq_val = &g_scene_playing->engine.config.eq_val;
    
    switch ( signal ) 
    {
    case WIDGET_SIG_USEREQ_OK:
        //get usereq        
        usereq_widget_get_value( usereq_widget, eq_val->user_param );
        
        //save usereq
        s_save_eq = (*eq_val);
        
        //delete usereq
        com_delete_usereq_widget();
        
        //paint config info
        playing_paint_config( g_scene_playing );
        break;
            
    case WIDGET_SIG_USEREQ_CHANGED:
        //get usereq        
        usereq_widget_get_value( usereq_widget, eq_val->user_param );
        
        //set usereq
        music_set_eq( eq_val );
        break;
            
    case WIDGET_SIG_USEREQ_QUIT:
        //delete usereq
        com_delete_usereq_widget();
        break;
         
    default:
        break;
    }    
}

bool com_create_srsuser_widget( void )
{
    mengine_eq_t *eq_val = &g_scene_playing->engine.config.eq_val;
    
    if ( g_scene_playing->srsuser == NULL )
    {
        g_scene_playing->srsuser = srsuser_widget_create( eq_val->srs_param, 
                                                g_scene_playing->bmp_bg, com_srsuser_widget_cb );                   
        if( g_scene_playing->srsuser == NULL )
        {
            print_err(); 
            return false;
        }
        else
        {
            srsuser_widget_paint( g_scene_playing->srsuser );
            
            //save eq
            s_save_eq = (*eq_val);            

            //set eq
            eq_val->eq_mode = EQ_SRSUSR_MODE;
            music_set_eq( eq_val );
        }    
    }

    return true;
}

bool com_delete_srsuser_widget( void )
{
    if(g_scene_playing->srsuser != NULL)
    {
        srsuser_widget_delete( g_scene_playing->srsuser );
        g_scene_playing->srsuser = NULL;
        
        //restore eq
        music_set_eq( &s_save_eq );
    }

    return true;
}

void com_srsuser_widget_cb( srsuser_widget_t *srsuser_widget, srsuser_widget_sig_e signal )
{
    mengine_eq_t *eq_val = &g_scene_playing->engine.config.eq_val;
        
    switch ( signal ) 
    {
    case WIDGET_SIG_SRSUSER_OK:
        //get usereq        
        srsuser_widget_get_value( srsuser_widget, eq_val->srs_param );
          
        //save usereq
        s_save_eq = (*eq_val);
        
        //delete srsuser
        com_delete_srsuser_widget();
        
        //paint config info
        playing_paint_config( g_scene_playing );
        break;
            
    case WIDGET_SIG_SRSUSER_CHANGED:
        //get usereq        
        srsuser_widget_get_value( srsuser_widget, eq_val->srs_param );
          
        //set usereq
        music_set_eq( eq_val );
        break;
            
    case WIDGET_SIG_SRSUSER_QUIT:
        //delete srsuser
        com_delete_srsuser_widget();
        break;
         
    default:
        break;
    }    
}

