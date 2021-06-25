/********************************************************************************
 *                              USDK(1100)
 *                             Module: app 
 *                 Copyright(c) 2003-2009 Actions Semiconductor,
 *                            All Rights Reserved. 
 * File : music_list_filelist.c
 * History:         
 *      <author>    <time>           <version >             <desc>
 *    songzhining    2009-2-25          v1.0               build this file 
 ********************************************************************************/
/*!
* \file     music_list_filelist.c
* \brief    music_list_filelist widget
* \author   songzhining
*               
* \version 1.0
* \date  2009/02/25
*******************************************************************************/

#include "music_list_filelist.h"

/* 退出主菜单 */
bool music_list_filelist_exit( menuview_t *menuview );

/* 初始化菜单数据 */
menu_t* music_list_filelist_init( void );

/* 删除菜单数据 */
int music_list_filelist_deinit( void );

/* listmenu entry */
static int _proc_items( menuview_t *menuview, menuview_sig_e signal );

static bool _create_softkeyboard_widget( void );
static bool _delete_softkeyboard_widget( void );
static void _softkeyboard_callback(softkeyboard_t *keyboard, softkeyboard_sig_e signal);

static bool _create_loadingbox_widget( void );
static bool _delete_loadingbox_widget( void );
static void _loadingbox_widget_cb(loadingbox_t *loadingbox, loadingbox_sig_e signal);

static bool _create_del_dialog_widget( unsigned short str_id );
static bool _delete_del_dialog_widget( void );
static void _del_dialog_widget_cb(dialog_t *dialog, widget_sig_dialog_e signal);

static bool _create_deleting_dialog_widget( void );
static bool _delete_deleting_dialog_widget( void );
static void _deleting_dialog_widget_cb(dialog_t *dialog, widget_sig_dialog_e signal);

static void _album_init_buffer( menuview_t *menuview );
static void _album_deinit_buffer( void );
static void _album_start_decode( menuview_t *menuview );
static void _album_decode_callback( void );

static bool _create_info_widget( unsigned short str_id );
static bool _delete_info_widget( void );
static void _info_widget_cb(dialog_t *dialog, widget_sig_dialog_e signal);

/* menu list data define */
static menu_t *s_filelist = NULL;
static int s_filelist_offset = 0;
static int s_filelist_current = 0;
static int s_filelist_global_offset = 0;

/* album image decode */
static gui_bitmap_info_t *s_album_bmp;
static int s_album_index = 0;

/* filelist init items */
static int _filelist_init_item( menuitem_t *items, int offset, int num )
{
    unsigned int str_len;
    int rec_num;
    menuitem_t *item;
    plist_detail_info_t *id3_info = NULL;
    plistclass_state_e state;
    int time;
    int hour, minute, second;

    //check size
    if ( num > ID3_MAX_ITEMS )
    {
        num = ID3_MAX_ITEMS;
    }
    
    //read tmp list
    rec_num = plistclass_get_files( g_plist_level, offset, num, &id3_info );
    if ( ( rec_num <= 0 ) || ( id3_info == NULL ) )
    {
        return 0;
    }
    else
    {
        print_dbg("rec_num=%d", rec_num);
        print_dbg("total=%d", num);
    }

    //get current plist state
    state = plistclass_get_state( g_plist_level );     

    //fill item
    for ( num=0; num<rec_num; num++ )
    {
        item = &items[num];
        
        //init item
        memset( item, 0, sizeof(menuitem_t) ); 

        //fill tag list
        if ( state == STATE_TAGLIST_FILE )
        {        
            //fill text
            if ( id3_info->filename != NULL )
            {
                str_len = strlen(id3_info->filename);
                item->text = malloc( str_len + 1 );
                if ( item->text != NULL )
                {
                    strcpy( item->text, id3_info->filename );
                }
            }
            
            //fill info2
            time = (int)id3_info->markinfo.time / 1000;
            if ( time == 0 )
            {
                hour = 0;
                minute = 0;
                second = 0;
            }
            else
            {
                hour = time / 3600;
                minute = (time % 3600) / 60;
                second = time % 60; 
            }
            
            item->info2 = malloc( 16 );
            if ( item->info2 != NULL )
            {
                snprintf( item->info2, 16, "%02d:%02d:%02d", hour, minute, second );   
            }     
        }
        else    //fill other file list
        {            
            //fill title
            if ( id3_info->title != NULL )
            {
                str_len = strlen(id3_info->title);
                item->text = malloc( str_len + 1 );
                if ( item->text != NULL )
                {
                    strcpy( item->text, id3_info->title );
                }
            }
            
            //fill info1
            if ( id3_info->artist != NULL )
            {
                str_len = strlen(id3_info->artist);
                item->info1 = malloc( str_len + 1 );
                if ( item->info1 != NULL )
                {
                    strcpy( item->info1, id3_info->artist );
                }
            }
            
             //fill info2
            if ( id3_info->album != NULL )
            {
                str_len = strlen(id3_info->album);
                item->info2 = malloc( str_len + 1 );
                if ( item->info2 != NULL )
                {
                    strcpy( item->info2, id3_info->album );
                }
            }
        }

        //next record
        id3_info ++;
    }

    return rec_num;
}

/* filelist deinit items */
static bool _filelist_deinit_item( menuitem_t *item )
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
bool music_list_filelist_enter( menuview_t *menuview, fusion_enter_e fenter )
{
    menu_t *cur_menu;    
    fusion_effect_t eff;
    plistclass_state_e state;
    
    if( fenter < FUSION_MAX )
    {
        fusion_get_config(FUSION_TYPE_COMMON, fenter, &eff);
        fusion_set_effect(&eff);
        scene_fusion_set_first_frame(g_scene_list->scene);  
    }

    //clear filter string
    plistclass_set_filter( NULL );
    
    cur_menu = music_list_filelist_init();    

    //check menu
    if ( cur_menu == NULL )
    {
        return false;
    }

    menuview_set_menu( menuview, cur_menu );
    if ( menuview->type != MENUVIEW_PICLIST_MUSIC )
    {
        menuview_load_style( menuview, MENUVIEW_PICLIST_MUSIC );
    }
    menuview_paint( menuview );

    //init album buffer
    _album_init_buffer( menuview );
     
    //abort album decode
    album_abort_decode();
        
    //start album decode
    s_album_index = menuview_get_prefetch_start( menuview );  //cur_menu->offset;
    kill_timer( g_scene_list->single_timer );
    g_scene_list->single_timer = set_single_shot_timer( 1, (timer_proc)_album_start_decode, menuview );

    if( fenter < FUSION_MAX )
    {
        scene_fusion_set_last_frame(g_scene_list->scene);
        fusion_display(NULL); 
    }

    state = plistclass_get_state( g_plist_level );   

    //if null, prompt
    if( cur_menu->total <= 0 )
    {
        //tag list
        if ( state == STATE_TAGLIST_FILE )
        {
            _create_info_widget(g_scene_list->str_notag->str_id); 
        }
        else    //other file list
        {
            _create_info_widget(g_scene_list->str_nofile->str_id); 
        }  
    }

    return true;
}

/* 退出主菜单 */
bool music_list_filelist_exit( menuview_t *menuview )
{
    plistclass_state_e state;

    //clear filter string
    plistclass_set_filter( NULL );    

    //delete main menu
    music_list_filelist_deinit();
    
    s_filelist_offset = 0;
    s_filelist_current = 0;
    s_filelist_global_offset = 0;

    //return process
    plistclass_prev_level( g_plist_level );
    
    state = plistclass_get_state( g_plist_level );     
    switch( state )
    {
    //第2级处理
    case STATE_FMREC:
    case STATE_VOICEREC:
        music_list_recfile_enter( menuview, FUSION_BACKWARD );
        break;   
        
    case STATE_ARTIST_LIST:  
    case STATE_ALBUM_LIST:        
    case STATE_GENRE_LIST:
        //return to parent        
        music_list_category_enter( menuview, FUSION_BACKWARD );
        break;
        
    case STATE_MYLIST_LIST:
        music_list_mylist_enter( menuview, FUSION_BACKWARD );
        break;
        
    case STATE_MYRATING_LIST:
        music_list_level_enter( menuview, FUSION_BACKWARD );
        break;        
        
    //第1级处理
    default:
        //return to mainmenu
        music_list_mainmenu_enter( menuview, FUSION_BACKWARD );
        break;
    }

    return true;
}

menu_t* music_list_filelist_init( void )
{
    menuitem_t items[ID3_MAX_ITEMS];
    int total;
    int offset;
    int rec_num;

    //initial menu    
    if( s_filelist == NULL )
    {
        //create menu
        s_filelist = menu_create();        
    }
    else
    {
        //clear menu
        menu_remove_all_items( s_filelist );        
    }    
   
    //generate display list
    plistclass_get_files( g_plist_level, 0, 0, NULL );
    
    //get display list number
    total = plistclass_get_item_num( PLIST_SHOWLIST );
    offset = s_filelist_global_offset;

    //init items
    rec_num = _filelist_init_item( items, offset, ID3_MAX_ITEMS );
    menu_append_items( s_filelist, items, rec_num );    
    
    //init global index
    menu_set_global_offset( s_filelist, s_filelist_global_offset );
    menu_set_global_size( s_filelist, total );
    
    //init index
    menu_set_offset( s_filelist, s_filelist_offset );
    menu_set_cur_index( s_filelist, s_filelist_current );
    
    //set menu callback  
    if( s_filelist != NULL )
    {
        s_filelist->callback = (menu_cb_t)_proc_items;       
    }    
    
    return s_filelist;
}

int music_list_filelist_deinit( void )
{    
    int cur_index;
    menuitem_t *item;
    
    //delete menu    
    if( s_filelist != NULL )
    {
        //save index
        s_filelist_offset = s_filelist->offset;
        s_filelist_current = s_filelist->current;
        s_filelist_global_offset = s_filelist->global_offset;
        
        //free item buffers
        for( cur_index=0; cur_index<s_filelist->total; cur_index++ )
        {
            item = menu_get_item( s_filelist, cur_index );
            _filelist_deinit_item( item );
        }
        
        //delete menu
        menu_delete( s_filelist );
        s_filelist = NULL;
    }

    //abort album decode
    album_abort_decode();

    //deinit album buffer
    _album_deinit_buffer();
    
    //kill timer
    kill_timer( g_scene_list->single_timer );
    g_scene_list->single_timer = -1;    

    return true;
}

static int _proc_items( menuview_t *menuview, menuview_sig_e signal )
{        
    menu_t *cur_menu = menuview->menu;
    menuitem_t *cur_item = menu_get_cur_item( cur_menu );
    plistclass_state_e state;
    menuitem_t items[ID3_MAX_ITEMS];
    int rec_num;
    int cur_index;
    dialog_init_t dialog_init;
    plist_detail_info_t *id3_info = NULL;
    
    state = plistclass_get_state( g_plist_level ); 
    
    switch ( signal ) 
    {
    case WIDGET_SIG_MENUVIEW_OK:    
        if ( cur_menu->total > 0 )
        {
            //set now playing
            plistclass_add_showlist_to_np();    //add all
            
            rec_num = plistclass_get_item_num( PLIST_NOWPLAYING );
            
            //save file path
            g_file_index = cur_menu->global_offset + cur_menu->current;

            //check now playing table
            if( rec_num <= 0 )
            {
                //模态提示框
                dialog_init.head = DIALOG_HEAD_WARN;
                dialog_init.str_id = g_scene_list->str_nospace->str_id;
                dialog_modal( &dialog_init );
                break;
            }

            //check file
            if ( music_list_check_file( NULL, g_file_index ) == false )
            {
                //delete records         
                rec_num = plistclass_get_files( g_plist_level, g_file_index, 1, &id3_info );                
                if( ( rec_num > 0 ) && ( id3_info != NULL ) )
                {
                    plistclass_del_items_from_showlist( g_file_index, 1 );
                    plistclass_sync_to_fs( id3_info->fullpath );
                }

                //abort album decode
                album_abort_decode();                

                //update
                music_list_filelist_deinit();
                cur_menu = music_list_filelist_init();
                menuview_set_menu( menuview, cur_menu );
                menuview_paint( menuview );  
                
                //start album decode
                s_album_index = menuview_get_prefetch_start( g_scene_list->list );  //cur_menu->offset;
                kill_timer( g_scene_list->single_timer );
                g_scene_list->single_timer = set_single_shot_timer( 1, (timer_proc)_album_start_decode, 
                                                                    g_scene_list->list );  
                break;
            }

//            if ( rec_num > 0 )
            {
                //clear filter string
                plistclass_set_filter( NULL );    

                //exit menu
                music_list_filelist_deinit();  
                g_scene_list->exit_type = SCENE_LIST_NEXT;
                music_list_widget_menu_exit( menuview, music_list_filelist_enter );
            }            
        }
        else
        {
            //tag list
            if ( state == STATE_TAGLIST_FILE )
            {
                _create_info_widget(g_scene_list->str_notag->str_id); 
            }
            else    //other file list
            {
                _create_info_widget(g_scene_list->str_nofile->str_id); 
            }  
        }
        break;


    case WIDGET_SIG_MENUVIEW_QUIT:
        // return to parent menu
        music_list_filelist_exit( menuview );
        break;


    case WIDGET_SIG_MENUVIEW_EXCEED_FIRST:
        // add one item to first
        _filelist_init_item( items, cur_menu->global_offset, 1 );
        menu_prepend_item( cur_menu, &items[0] );
        
        // remove last item
        cur_item = menu_get_item( cur_menu, cur_menu->total - 1 );
        _filelist_deinit_item( cur_item );
        menu_remove_last_item( cur_menu );
        break;

        
    case WIDGET_SIG_MENUVIEW_EXCEED_LAST:
        // remove first item
        cur_item = menu_get_item( cur_menu, 0 );
        _filelist_deinit_item( cur_item );
        menu_remove_first_item( cur_menu );
        
        // add one item to last
        _filelist_init_item( items, cur_menu->global_offset+cur_menu->total, 1 );
        menu_append_item( cur_menu, &items[0] );
        break;

        
    case WIDGET_SIG_MENUVIEW_UPDATE_ALL:
        //read new items
        rec_num = _filelist_init_item( items, cur_menu->global_offset, cur_menu->total );
        
        //update all items
        for ( cur_index = 0; cur_index < rec_num; cur_index ++ )
        {
            cur_item = menu_get_item( cur_menu, cur_index );

            //deinit item
            _filelist_deinit_item( cur_item );
            
            //modify item
            menu_set_item( cur_menu, cur_index, &items[cur_index] );
        }
        break;

        
    case WIDGET_SIG_MENUVIEW_SHORT_MENU:  
        if ( cur_menu->total > 0 )
        {
            //tag list
            if ( state == STATE_TAGLIST_FILE )
            {
                _create_del_dialog_widget( g_scene_list->str_del_tag->str_id );
            }
            else if ( state == STATE_MYLIST_FILE )
            {
                _create_del_dialog_widget( g_scene_list->str_del_file->str_id );
            }
            else    //other file list
            {
                _create_softkeyboard_widget();
            }  
        }
        break;

        
    case WIDGET_SIG_MENUVIEW_DELETE:  
        if ( cur_menu->total > 0 )
        {
            //tag list
            if ( state == STATE_TAGLIST_FILE )
            {
                _create_del_dialog_widget( g_scene_list->str_del_tag->str_id );
            }
            else if ( state == STATE_MYLIST_FILE )
            {
                _create_del_dialog_widget( g_scene_list->str_del_file->str_id );
            }
            else    //other file list
            {
            }  
        }
        break;
        
        
    case WIDGET_SIG_MENUVIEW_UP:  
    case WIDGET_SIG_MENUVIEW_DOWN:  
        //abort album decode
        album_abort_decode();

        //start album decode
        s_album_index = menuview_get_prefetch_start( menuview );  //cur_menu->offset;
        kill_timer( g_scene_list->single_timer );
        g_scene_list->single_timer = set_single_shot_timer( 1, (timer_proc)_album_start_decode, menuview );
        break;
        

    default:
        break;
    }    

    return true;
}

static bool _create_softkeyboard_widget( void )
{
    //create widget
    g_scene_list->softkeyboard = softkeyboard_create(IME_TYPE_PINYIN, _softkeyboard_callback );
    if( g_scene_list->softkeyboard == NULL )
    {
        print_err("create softkeyboard widget failed!");
        return false;
    }
    else
    {
        //hide mainmenu
        menuview_hide(g_scene_list->list);
        
        softkeyboard_paint( g_scene_list->softkeyboard );
    }

    return true;
}

static bool _delete_softkeyboard_widget( void )
{
    softkeyboard_delete( g_scene_list->softkeyboard );
    g_scene_list->softkeyboard = NULL;
    
    return true;
}

static void _searching_done( void )
{
    int total;    
    menu_t *cur_menu;   
    dialog_init_t dialog_init;

    //get display list number
    total = plistclass_get_item_num( PLIST_SHOWLIST );
    if ( total <= 0 )
    {
        //clear filter string
        plistclass_set_filter( NULL );

        //update files
        plistclass_get_files( g_plist_level, 0, 0, NULL );        
    }
    
    //delete filelist
    music_list_filelist_deinit();
    s_filelist_offset = 0;
    s_filelist_current = 0;
    s_filelist_global_offset = 0;
    
    //update filelist
    cur_menu = music_list_filelist_init();    

    //check menu
    if ( cur_menu == NULL )
    {
        return;
    }

    menuview_set_menu( g_scene_list->list, cur_menu ); 
     
    //delete searching dialog
    _delete_loadingbox_widget();
    
    if ( total <= 0 )
    {
        //show cann't found dialog
        dialog_init.head = DIALOG_HEAD_WARN;
        dialog_init.str_id = g_scene_list->str_notfound->str_id;
        dialog_modal( &dialog_init );    
    }
    else
    {
        //delete softkeyboard
        _delete_softkeyboard_widget();

        //show mainmenu
        menuview_show(g_scene_list->list);
        menuview_paint(g_scene_list->list);     
    }
    
    //abort album decode
    album_abort_decode();
    
    //start album decode
    s_album_index = menuview_get_prefetch_start( g_scene_list->list );  //cur_menu->offset;
    kill_timer( g_scene_list->single_timer );
    g_scene_list->single_timer = set_single_shot_timer( 1, (timer_proc)_album_start_decode, 
                                                        g_scene_list->list );          
}

static void _deal_searching( void )
{
    //searching in thread
    plistclass_get_files( g_plist_level, 0, 0, NULL );

    //process after searching
    kill_timer( g_scene_list->single_timer );
    g_scene_list->single_timer = set_single_shot_timer( 1, (timer_proc)_searching_done, NULL );
}

static void _softkeyboard_callback(softkeyboard_t *keyboard, softkeyboard_sig_e signal)
{
    char text[128];
    
    switch ( signal ) 
    {
    case WIDGET_SIG_SKB_OK:  
        //abort album decode
        album_abort_decode();
                  
        //get text
        softkeyboard_get_text( g_scene_list->softkeyboard, text, sizeof(text) );    
        
        //set filter string
        plistclass_set_filter( text );

        //create loadingbox
        _create_loadingbox_widget();

        //create thread to search
        music_create_thread( _deal_searching, NULL );
        break;

    case WIDGET_SIG_SKB_CANCEL:
        //delete softkeyboard
        _delete_softkeyboard_widget();
        
        //show mainmenu
        menuview_show(g_scene_list->list);
        menuview_paint(g_scene_list->list);
        break;
        
    default:
        break;
    }
}

static bool _create_loadingbox_widget( void )
{
    if(g_scene_list->loadingbox == NULL)
    {
        g_scene_list->loadingbox = loadingbox_create( LOADINGBOX_SEARCHING, _loadingbox_widget_cb );
        if ( NULL == g_scene_list->loadingbox )
        {
            print_err("loadingbox_create failed\n\n");
            return false;
        }
        else
        {
            //paint
            loadingbox_paint( g_scene_list->loadingbox );    
        }
    }

    return true;
}

static bool _delete_loadingbox_widget( void )
{    
    if(g_scene_list->loadingbox != NULL)
    {
        loadingbox_delete( g_scene_list->loadingbox );
        g_scene_list->loadingbox = NULL;
    }
    
    return true;
}

static void _loadingbox_widget_cb(loadingbox_t *loadingbox, loadingbox_sig_e signal)
{
}

static bool _create_del_dialog_widget( unsigned short str_id )
{
    dialog_init_t dialog_init;        
    dialog_init.head = DIALOG_HEAD_QUERY;
    dialog_init.str_id = str_id;
    dialog_init.def_sel = WIDGET_SIG_DIALOG_CANCAL;
    
    /* init and callback function */
    g_scene_list->dialog = dialog_create( &dialog_init, _del_dialog_widget_cb);
    if ( NULL == g_scene_list->dialog )
    {
        print_err("dialog_create failed\n\n");
        return false;
    }
    else
    {
        dialog_paint( g_scene_list->dialog );
    }

    return true;
}

static bool _delete_del_dialog_widget( void )
{
    if(g_scene_list->dialog != NULL)
    {
        dialog_delete( g_scene_list->dialog );
        g_scene_list->dialog = NULL;
    }
    
    return true;
}

static void _del_dialog_widget_cb(dialog_t *dialog, widget_sig_dialog_e signal)
{
    int current;
    menuitem_t *item;
    menuitem_t items[ID3_MAX_ITEMS];
    int rec_num;
    int result;
    
    switch ( signal )
    {
    case WIDGET_SIG_DIALOG_OK:
        //点击了:yes, 执行yes的操作. 并删除dialog
        _delete_del_dialog_widget();

        //show deleting dialog
        _create_deleting_dialog_widget();
        gui_screen_update();        

        //delete bookmark
        current = s_filelist->global_offset + s_filelist->current;
//        plistclass_del_items_from_bmlist( current, 1 );
        result = plistclass_del_items_from_showlist( current, 1 );
        if ( result < 0 )
        {
            _delete_deleting_dialog_widget();
            _create_info_widget(g_scene_list->str_del_fail->str_id);
            break;
        }

        //update global size
//        rec_num = plistclass_get_item_num( PLIST_BOOKMARK );
        rec_num = plistclass_get_item_num( PLIST_SHOWLIST );
        menu_set_global_size( s_filelist, rec_num );

        // remove item
        current = s_filelist->current;
        item = menu_get_item( s_filelist, current );
        _filelist_deinit_item( item );
        menu_remove_item( s_filelist, current );
            
        //process item
        current = s_filelist->global_offset + s_filelist->total;
        if ( current < s_filelist->global_size )    //need to append item
        {
            //add item to menu
            _filelist_init_item( items, current, 1 );        
            menu_append_item( s_filelist, &items[0] );
        }
        else if ( s_filelist->global_offset > 0 )   //need to prepend item
        {
            //add item to menu
            s_filelist->global_offset --;
            _filelist_init_item( items, s_filelist->global_offset, 1 );        
            menu_prepend_item( s_filelist, &items[0] );
            s_filelist->offset ++;
        }
        else
        {
        }
        
        //repaint
        menuview_paint( g_scene_list->list );           
         
        //abort album decode
        album_abort_decode();
        
        //start album decode
        s_album_index = menuview_get_prefetch_start( g_scene_list->list );  //cur_menu->offset;
        kill_timer( g_scene_list->single_timer );
        g_scene_list->single_timer = set_single_shot_timer( 1, (timer_proc)_album_start_decode, 
                                                            g_scene_list->list );
        
        _delete_deleting_dialog_widget();
        break;

    case WIDGET_SIG_DIALOG_CANCAL:     
        //点击了:no, 或者return, 不执行任何操作. 删除dialog   
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
    dialog_init.str_id = g_scene_list->str_deleting->str_id;
    
    /* init and callback function */
    g_scene_list->dialog = dialog_create( &dialog_init, _deleting_dialog_widget_cb);
    if ( NULL == g_scene_list->dialog )
    {
        print_err("dialog_create failed\n\n");
        return false;
    }
    else
    {
        dialog_paint( g_scene_list->dialog );
    }

    return true;
}

static bool _delete_deleting_dialog_widget( void )
{
    if(g_scene_list->dialog != NULL)
    {
        dialog_delete( g_scene_list->dialog );
        g_scene_list->dialog = NULL;
    }
    
    return true;
}

static void _deleting_dialog_widget_cb(dialog_t *dialog, widget_sig_dialog_e signal)
{
}


static void _album_init_buffer( menuview_t *menuview )
{
    int width = menuview_get_bitmap_width( menuview );
    int height = menuview_get_bitmap_height( menuview );
    int bpp = 2;

    if ( s_album_bmp == NULL )
    {        
        s_album_bmp = malloc( sizeof(gui_bitmap_info_t) );
        if ( s_album_bmp == NULL )
        {
            print_err();
            return;
        }
        
        s_album_bmp->pdata = malloc( (unsigned int)(width * height * bpp) );
        if ( s_album_bmp->pdata == NULL )
        {
            print_err();
            return;
        }
    }
    
    s_album_bmp->bytesperpixel = (unsigned short)bpp;
    s_album_bmp->xsize = (unsigned short)width;
    s_album_bmp->ysize = (unsigned short)height;
}

static void _album_deinit_buffer( void )
{
    //free bitmap
    if ( s_album_bmp != NULL )
    {
        free( (char*)s_album_bmp->pdata );
        free( s_album_bmp );
        s_album_bmp = NULL;
    }
}

static void _album_start_decode( menuview_t *menuview )
{
    bool bret;
    menuitem_t *item;
    int offset;
    int rec_num;
    plist_detail_info_t *id3_info = NULL;

    for( ; s_album_index < s_filelist->total; s_album_index ++ )
    {
        item = menu_get_item( s_filelist, s_album_index );
        
        if( ( item == NULL ) || ( item->bitmap != NULL ) )
        {
        }
        else
        {
            //read one item from show list
            offset = s_filelist->global_offset + s_album_index;
            rec_num = plistclass_get_files( g_plist_level, offset, 1, &id3_info );
            
            if( ( rec_num <= 0 ) || ( id3_info == NULL ) )
            {
                continue;
            }

            //check album info
            bret = album_check_file( id3_info );
            if ( bret == true )
            {
                //init album bmp                
                _album_init_buffer( menuview );
                
                //start decode
                album_abort_decode();
                album_start_decode( id3_info, s_album_bmp, _album_decode_callback );
                break;
            }
        }
    }

}

static void _album_decode_callback( void )
{
    mmm_img_info_t img_info;
    menuitem_t *item;

    item = menu_get_item( s_filelist, s_album_index );
    if( item == NULL )
    {
        print_err();
        return;
    }
    else
    {
        //change bitmap size
        album_get_image_info( &img_info );
        s_album_bmp->xsize = (unsigned short)img_info.img_width;
        s_album_bmp->ysize = (unsigned short)img_info.img_height;

        //set album bmp to item
        item->bitmap = s_album_bmp;
        s_album_bmp = NULL;

        //draw bitmap
        menuview_paint_bitmap( g_scene_list->list, s_album_index );
    }

    /* 向场景发送预览解码完毕消息，解码下一幅 */
    s_album_index ++;
    kill_timer( g_scene_list->single_timer );
    g_scene_list->single_timer = set_single_shot_timer( 1, (timer_proc)_album_start_decode, g_scene_list->list );
}

static bool _create_info_widget( unsigned short str_id )
{
    dialog_init_t dialog_init;        
    dialog_init.head = DIALOG_HEAD_WARN;
    dialog_init.str_id = str_id;
    
    /* init and callback function */
    g_scene_list->dialog = dialog_create( &dialog_init, _info_widget_cb );
    if ( NULL == g_scene_list->dialog )
    {
        print_err("dialog_create failed\n\n");
        return false;
    }
    else
    {
        dialog_paint( g_scene_list->dialog );
    }

    return true;
}

static bool _delete_info_widget( void )
{
    if(g_scene_list->dialog != NULL)
    {
        dialog_delete( g_scene_list->dialog );
        g_scene_list->dialog = NULL;
    }
    
    return true;
}

static void _info_widget_cb(dialog_t *dialog, widget_sig_dialog_e signal)
{
    menu_t *cur_menu;
    
    switch ( signal )
    {
    case WIDGET_SIG_DIALOG_CANCAL:     
        _delete_info_widget();
        
        cur_menu = g_scene_list->list->menu;
        if ( cur_menu->total <= 0 )
        {
            // return to parent menu
//            music_list_filelist_exit( g_scene_list->list );
        }
        break;

    default:
        break;
    }
}

/* 检查文件是否可以播放*/
bool music_list_check_file( char *filepath, int file_index )
{
    bool bret;
    mengine_t mengine = {0};
    unsigned short str_id = 0;

    if ( filepath != NULL )
    {
        bret = music_set_filepath( filepath );
    }
    else if ( file_index == -1 )
    {
        bret = music_set_last_file();
    }
    else
    {
        bret = music_set_fileindex( file_index );
    }
    
    if ( ( bret == false ) && ( g_scene_list != NULL ) )
    {
        //隐藏正在播放项
        music_set_nowplaying(false);
                
        if ( music_get_engine_info( &mengine ) == false )
        {
            return false;
        }
        
        switch( mengine.eg_status.err_status )
        {
        case EG_ERR_OPEN_FILE:
            str_id = g_scene_list->str_err_openfile->str_id;
            break;
            
        case EG_ERR_NOT_SUPPORT:
            str_id = g_scene_list->str_err_notsupport->str_id;
            break;
            
        case EG_ERR_DECODER_ERROR:
            str_id = g_scene_list->str_err_decode->str_id;
            break;
            
        case EG_ERR_NO_LICENSE:
            str_id = g_scene_list->str_err_nolicense->str_id;
            break;
            
        case EG_ERR_SECURE_CLOCK:
            str_id = g_scene_list->str_err_clock->str_id;
            break;
            
        case EG_ERR_DRM_INFO:
            str_id = g_scene_list->str_err_drminfo->str_id;
            break;
            
        default:
            break;
        }
        _create_info_widget( str_id );
    }
    
    return bret;
}

