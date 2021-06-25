/********************************************************************************
 *                              USDK(1100)
 *                             Module: app 
 *                 Copyright(c) 2003-2009 Actions Semiconductor,
 *                            All Rights Reserved. 
 * File : music_list_mylist.c
 * History:         
 *      <author>    <time>           <version >             <desc>
 *    songzhining    2009-2-25          v1.0               build this file 
 ********************************************************************************/
/*!
* \file     music_list_mylist.c
* \brief    music_list_mylist widget
* \author   songzhining
*               
* \version 1.0
* \date  2009/02/25
*******************************************************************************/

#include "music_list_mylist.h"

/* 退出主菜单 */
bool music_list_mylist_exit( menuview_t *menuview );

/* 初始化菜单数据 */
menu_t* music_list_mylist_init( void );

/* 删除菜单数据 */
int music_list_mylist_deinit( void );

/* listmenu entry */
static int _proc_items( menuview_t *menuview, menuview_sig_e signal );

/* menu list max items */
#define MYLIST_MAX_ITEMS  5

/* const strings */
static char *str_myplaylist[MYLIST_MAX_ITEMS] =
{
    "My Playlist 1",
    "My Playlist 2",
    "My Playlist 3",
    "My Playlist 4",
    "My Playlist 5",
};

/* menu list data define */
static menu_t *s_mylist = NULL;
static int s_mylist_offset = 0;
static int s_mylist_current = 0;
static int s_mylist_global_offset = 0;


/* mylist init items */
static int _mylist_init_item( menuitem_t *items, int offset, int num )
{
    unsigned int str_len;
    int rec_num;
    menuitem_t *item;
    plist_name_t plist_name[ID3_MAX_ITEMS];
    int start = 0;
    int number;
    int ii;
    int cur_index;

    //check size
    if ( num > ID3_MAX_ITEMS )
    {
        num = ID3_MAX_ITEMS;
    }
    
    //read mtp list    
    if ( ( offset+num ) > MYLIST_MAX_ITEMS )
    {
        if ( offset < MYLIST_MAX_ITEMS )
        {
            start = 0;
            number = ( offset + num ) - MYLIST_MAX_ITEMS;
            rec_num = plistclass_get_mtplist_name( plist_name, start, number ) + (MYLIST_MAX_ITEMS-offset);
        }
        else
        {
            start = offset - MYLIST_MAX_ITEMS;
            number = num;
            rec_num = plistclass_get_mtplist_name( plist_name, start, number );
        }
    }
    else
    {
        rec_num = num;
    }

    //fill item
    for ( ii=0; ii<rec_num; ii++ )
    {
        item = &items[ii];
        
        //init item
        memset( item, 0, sizeof(menuitem_t) ); 
        
        cur_index = ii + offset;

        //fill tag list
        if ( cur_index < MYLIST_MAX_ITEMS )
        {            
            //fill text
            str_len = strlen(str_myplaylist[cur_index]);
            item->text = malloc( str_len + 1 );
            if ( item->text != NULL )
            {
                strcpy( item->text, str_myplaylist[cur_index] );                  
            }
        }
        else    //fill other file list
        {            
            cur_index = cur_index - start - MYLIST_MAX_ITEMS;
            
            //fill title
            str_len = strlen(plist_name[cur_index].list_name);
            item->text = malloc( str_len + 1 );
            if ( item->text != NULL )
            {
                strcpy( item->text, plist_name[cur_index].list_name );
            }
        }
        
        item->has_child = true;
        
        item->callback = (item_cb_t)_proc_items;
    }

    return rec_num;
}

/* filelist deinit items */
static bool _mylist_deinit_item( menuitem_t *item )
{
    if ( item != NULL )
    {
        if ( item->text != NULL )
        {
            free( item->text );
        }
        
        if ( item->info1 != NULL )
        {
            free( item->info1 );
        }
        
        if ( item->info2 != NULL )
        {
            free( item->info2 );
        }
        
        if ( item->bitmap != NULL )
        {
            free( (char*)item->bitmap->pdata );
            free( item->bitmap );
        }
    }
    else
    {
    }

    return true;
}


/* 进入主菜单 */
bool music_list_mylist_enter( menuview_t *menuview, fusion_enter_e fenter )
{
    menu_t *cur_menu;
    fusion_effect_t eff;

    fusion_get_config(FUSION_TYPE_COMMON, fenter, &eff);
    fusion_set_effect(&eff);
    scene_fusion_set_first_frame(g_scene_list->scene);

    cur_menu = music_list_mylist_init();
    menuview_set_menu( menuview, cur_menu );
    
    if ( menuview->type != MENUVIEW_MENULIST_TEXT )
    {
        menuview_load_style( menuview, MENUVIEW_MENULIST_TEXT );
    }
    menuview_paint( menuview );

    scene_fusion_set_last_frame(g_scene_list->scene);
    fusion_display(NULL);

    return true;
}

/* 退出主菜单 */
bool music_list_mylist_exit( menuview_t *menuview )
{
    //delete main menu
    music_list_mylist_deinit();

    s_mylist_offset = 0;
    s_mylist_current = 0;
    s_mylist_global_offset = 0;

    //return
    plistclass_prev_level( g_plist_level );
    music_list_mainmenu_enter( menuview, FUSION_BACKWARD );    
    
    return true;
}

menu_t* music_list_mylist_init( void )
{
    menuitem_t items[ID3_MAX_ITEMS];
    int total;
    int offset;
    int rec_num;

    //initial menu    
    if( s_mylist == NULL )
    {
        //create menu
        s_mylist = menu_create();        
    }
    else
    {
        //clear menu
        menu_remove_all_items( s_mylist );        
    }    

    //get display list number
    total = plistclass_get_mtplist_num() + MYLIST_MAX_ITEMS;
    offset = s_mylist_global_offset;

    //init items
    rec_num = _mylist_init_item( items, offset, ID3_MAX_ITEMS );
    menu_append_items( s_mylist, items, rec_num );    
    
    //init global index
    menu_set_global_offset( s_mylist, s_mylist_global_offset );
    menu_set_global_size( s_mylist, total );
    
    //init index
    menu_set_offset( s_mylist, s_mylist_offset );
    menu_set_cur_index( s_mylist, s_mylist_current );

    return s_mylist;
}

int music_list_mylist_deinit( void )
{    
    int cur_index;
    menuitem_t *item;
    
    //delete menu    
    if( s_mylist != NULL )
    {
        //save index
        s_mylist_offset = s_mylist->offset;
        s_mylist_current = s_mylist->current;
        s_mylist_global_offset = s_mylist->global_offset;
        
        //free item buffers
        for( cur_index=0; cur_index<s_mylist->total; cur_index++ )
        {
            item = menu_get_item( s_mylist, cur_index );
            _mylist_deinit_item( item );
        }
        
        //delete menu
        menu_delete( s_mylist );
        s_mylist = NULL;
    }

    return true;
}

static int _proc_items( menuview_t *menuview, menuview_sig_e signal )
{        
    menu_t *cur_menu = menuview->menu;
    menuitem_t *cur_item = menu_get_cur_item( cur_menu );
    menuitem_t items[ID3_MAX_ITEMS];
    int rec_num;
    int cur_index;
    
    switch ( signal ) 
    {
    case WIDGET_SIG_MENUVIEW_OK:
        //save selected content
        g_usrsel_content.rating = cur_menu->global_offset + cur_menu->current + 1;
        plistclass_store_usrselcontent( RATING, &g_usrsel_content );
        
        //set level info
        plistclass_next_level( g_plist_level, LEVEL_TWO_LIST, LEVEL_THREE_FILELIST );
    
        //enter filelist
        music_list_mylist_deinit();
        music_list_filelist_enter( menuview, FUSION_FORWARD );
        break;

    case WIDGET_SIG_MENUVIEW_QUIT:
        // return to parent menu
        music_list_mylist_exit( menuview );
        break;


    case WIDGET_SIG_MENUVIEW_EXCEED_FIRST:
        // add one item to first
        _mylist_init_item( items, cur_menu->global_offset, 1 );
        menu_prepend_item( cur_menu, &items[0] );
        
        // remove last item
        cur_item = menu_get_item( cur_menu, cur_menu->total - 1 );
        _mylist_deinit_item( cur_item );
        menu_remove_last_item( cur_menu );
        break;

        
    case WIDGET_SIG_MENUVIEW_EXCEED_LAST:
        // remove first item
        cur_item = menu_get_item( cur_menu, 0 );
        _mylist_deinit_item( cur_item );
        menu_remove_first_item( cur_menu );
        
        // add one item to last
        _mylist_init_item( items, cur_menu->global_offset+cur_menu->total, 1 );
        menu_append_item( cur_menu, &items[0] );
        break;

        
    case WIDGET_SIG_MENUVIEW_UPDATE_ALL:
        //read new items
        rec_num = _mylist_init_item( items, cur_menu->global_offset, cur_menu->total );
        
        //update all items
        for ( cur_index = 0; cur_index < rec_num; cur_index ++ )
        {
            cur_item = menu_get_item( cur_menu, cur_index );

            //deinit item
            _mylist_deinit_item( cur_item );
            
            //modify item
            menu_set_item( cur_menu, cur_index, &items[cur_index] );
        }
        break;
        

    default:
        break;
    }  

    return true;
}



