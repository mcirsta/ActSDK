/********************************************************************************
 *                              USDK(1100)
 *                             Module: app 
 *                 Copyright(c) 2003-2009 Actions Semiconductor,
 *                            All Rights Reserved. 
 * File : music_list_category.c
 * History:         
 *      <author>    <time>           <version >             <desc>
 *    songzhining    2009-2-25          v1.0               build this file 
 ********************************************************************************/
/*!
* \file     music_list_category.c
* \brief    music_list_category widget
* \author   songzhining
*               
* \version 1.0
* \date  2009/02/25
*******************************************************************************/

#include "music_list_category.h"

/* 退出主菜单 */
bool music_list_category_exit( menuview_t *menuview );

/* 初始化菜单数据 */
menu_t* music_list_category_init( void );

/* 删除菜单数据 */
int music_list_category_deinit( void );

/* listmenu entry */
static int _proc_items( menuview_t *menuview, menuview_sig_e signal );
static bool _category_check_rotate( void );

static bool _create_softkeyboard_widget( void );
static bool _delete_softkeyboard_widget( void );
static void _softkeyboard_callback(softkeyboard_t *keyboard, softkeyboard_sig_e signal);

static bool _create_loadingbox_widget( void );
static bool _delete_loadingbox_widget( void );
static void _loadingbox_widget_cb(loadingbox_t *loadingbox, loadingbox_sig_e signal);

static void _album_init_buffer( menuview_t *menuview );
static void _album_deinit_buffer( void );
static void _album_start_decode( menuview_t *menuview );
static void _album_decode_callback( void );

static bool _create_info_widget( unsigned short str_id );
static bool _delete_info_widget( void );
static void _info_widget_cb(dialog_t *dialog, widget_sig_dialog_e signal);

/* const strings */
static char *str_unkonw = "unknown";
static char *str_unkonw_artist = "Unknown Artist";
static char *str_unkonw_album = "Unknown Album";
static char *str_unkonw_genre = "Unknown Genre";

/* menu list data define */
static menu_t *s_category = NULL;
static int s_category_offset = 0;
static int s_category_current = 0;
static int s_category_global_offset = 0;

/* album image decode */
static gui_bitmap_info_t *s_album_bmp;
static int s_album_index = 0;
static bool s_album_coverflow = true;    //coverflow mode config

/* category init items */
static int _category_init_item( menuitem_t *items, int offset, int num )
{
    unsigned int str_len;
    int rec_num;
    menuitem_t *item;
    plist_detail_info_t *id3_info = NULL;
    plistclass_state_e state;
    char *text = NULL;
    char *default_text = str_unkonw;

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
    
        switch( state )
        {
        case STATE_ARTIST_LIST:
            text = id3_info->artist;
            default_text = str_unkonw_artist;
            break;
            
        case STATE_ALBUM_LIST:
            text = id3_info->album;
            default_text = str_unkonw_album;
            break;
            
        case STATE_GENRE_LIST:
            text = id3_info->genre;
            default_text = str_unkonw_genre;
            break;   
            
        default:
            item->text = NULL;
            break;
        }

        if ( text != NULL)
        {
            str_len = strlen( text );
/*            
            if( str_len <= 0 || strcmp( text, str_unkonw ) == 0 )
            {
                item->text = malloc( strlen(default_text) + 1 );
                strcpy( item->text, default_text );
            }
            else
*/                
            {
                item->text = malloc( str_len + 1 );
                if ( item->text != NULL )
                {
                    strcpy( item->text, text );
                }
            }
        }
        else
        {
            item->text = NULL;
        }

        //next record
        id3_info ++;
    }

    return rec_num;
}

/* category deinit items */
static bool _category_deinit_item( menuitem_t *item )
{
    if ( item != NULL )
    {
        if ( item->text != NULL )
        {
            free( item->text );
            item->text = NULL;
        }
        
        if ( item->info1 != NULL )
        {
            free( item->info1 );
            item->info1 = NULL;
        }
        
        if ( item->info2 != NULL )
        {
            free( item->info2 );
            item->info2 = NULL;
        }
        
        if ( item->bitmap != NULL )
        {
            free( (char*)item->bitmap->pdata );
            free( item->bitmap );
            item->bitmap = NULL;
        }
    }
    else
    {
    }

    return true;
}

/* category deinit items */
static bool _category_deinit_bitmap( menu_t *menu )
{
    menuitem_t *item;
    int num;
    
    for( num = 0; num < menu->total; num ++ )
    {
        item = menu_get_item( menu, num );
        
        if ( ( item != NULL ) && ( item->bitmap != NULL ) )
        {
            free( (char*)item->bitmap->pdata );
            free( item->bitmap );
            item->bitmap = NULL;
        }
        
        if ( true == g_coverflow_mode )
        {
            //clear slide bmp
            coverflow_set_slide( menu, num );
        }        
    }
    
    return true;
}

/* category check rotate */
static bool _category_check_rotate( void )
{
    bool need_rotate = false;
    plistclass_state_e state;
    rotation_e angle;
    
    //set display style
    state = plistclass_get_state( g_plist_level );     
    switch( state )
    {
    case STATE_ALBUM_LIST:
        if ( true == s_album_coverflow )
        {
            // 取视图角度
            angle = sty_get_view_rotation();
            if ( ( angle == ROTATION_0 ) || ( angle == ROTATION_180 ) )
            {
                need_rotate = false;
            }
            else
            {
                need_rotate = true;
            }            
        }
        break; 
        
    default:
        break;     
    }       

    return need_rotate;
}

/* 进入主菜单 */
bool music_list_category_enter( menuview_t *menuview, fusion_enter_e fenter )
{
    plistclass_state_e state;
    menu_t *cur_menu;  
    fusion_effect_t eff;
    bool need_rotate = false;

    //check rotate
    need_rotate = _category_check_rotate();

    //set first
    if ( need_rotate == true )
    {
        fusion_get_config(FUSION_TYPE_ROTATE, FUSION_FORWARD, &eff);
        fusion_set_effect(&eff);
        fusion_set_first_frame(NULL);
    }
    else
    {
        fusion_get_config(FUSION_TYPE_COMMON, fenter, &eff);
        fusion_set_effect(&eff);
        scene_fusion_set_first_frame(g_scene_list->scene);
    }    
    
    //clear filter string
    plistclass_set_filter( NULL );    

    cur_menu = music_list_category_init();
    
    //check menu
    if ( cur_menu == NULL )
    {
        return false;
    }

    menuview_set_menu( menuview, cur_menu );

    //set display style
    state = plistclass_get_state( g_plist_level );     
    switch( state )
    {
    case STATE_ARTIST_LIST: 
        if ( menuview->type != MENUVIEW_MENULIST_TEXT )
        {
            menuview_load_style( menuview, MENUVIEW_MENULIST_TEXT );
        }
        menuview_paint( menuview );
        break;   
        
    case STATE_ALBUM_LIST:      
        if ( menuview->type != MENUVIEW_PICLIST_MUSIC )
        {
            menuview_load_style( menuview, MENUVIEW_PICLIST_MUSIC );
        }

        if ( cur_menu->total >= 0 )
        {
            g_coverflow_mode = s_album_coverflow; 
        }
        
        if ( true == g_coverflow_mode )
        {
            menuview_hide( menuview );
            
            //rotate
            if ( need_rotate == true )
            {
                scene_list_rotation();
            }
            else
            {
                //init coverflow
                coverflow_init( cur_menu );
                coverflow_paint( g_scene_list->hdc, cur_menu ); 
            } 
        }
        else
        {
            menuview_show( menuview );
            menuview_paint( menuview );
        }
        
        //init album buffer
        _album_init_buffer( menuview );
         
        //abort album decode
        album_abort_decode();
        
        //start album decode
        s_album_index = menuview_get_prefetch_start( menuview );  //cur_menu->offset;
        kill_timer( g_scene_list->single_timer );
        g_scene_list->single_timer = set_single_shot_timer( 1, (timer_proc)_album_start_decode, menuview );
        break;   
        
    case STATE_GENRE_LIST:
        if ( menuview->type != MENUVIEW_MENULIST_TEXT )
        {
            menuview_load_style( menuview, MENUVIEW_MENULIST_TEXT );
        }
        menuview_paint( menuview );
        break; 
        
    default:
        break;     
    }       

    //set last
    if ( need_rotate == true )
    {
        fusion_set_last_frame(NULL);
        fusion_display(NULL);
    }
    else
    {
        scene_fusion_set_last_frame(g_scene_list->scene);
        fusion_display(NULL);
    }
    
    //if null, prompt
    if( cur_menu->total <= 0 )
    {
        _create_info_widget(g_scene_list->str_nofile->str_id); 
    }
    
    return true;
}

/* 退出主菜单 */
bool music_list_category_exit( menuview_t *menuview )
{
    bool need_rotate = false;  
    fusion_effect_t eff;  
    fusion_enter_e fenter;

    //delete main menu
    music_list_category_deinit();

    //check rotate
    need_rotate = _category_check_rotate();

    //set first
    if ( need_rotate == true )
    {
        fusion_get_config(FUSION_TYPE_ROTATE, FUSION_FORWARD, &eff);
        fusion_set_effect(&eff);
        fusion_set_first_frame(NULL);
        
        scene_list_rotation();
    }

    //clear filter string
    plistclass_set_filter( NULL );    

    s_category_offset = 0;
    s_category_current = 0;
    s_category_global_offset = 0;
    
    //return to mainmenu
    plistclass_prev_level( g_plist_level );
    
    if ( need_rotate == true )
    {
        fenter = FUSION_MAX;
    }
    else
    {
        fenter = FUSION_BACKWARD;
    }
    
    menuview_show( g_scene_list->list );
    music_list_mainmenu_enter( menuview, fenter );    
        
    //set last
    if ( need_rotate == true )
    {
        fusion_set_last_frame(NULL);
        fusion_display(NULL);
    }

    return true;
}

menu_t* music_list_category_init( void )
{
    menuitem_t items[ID3_MAX_ITEMS];
    int total;
    int offset;
    int rec_num;
    
    //initial menu    
    if( s_category == NULL )
    {
        //create menu
        s_category = menu_create();        
    }
    else
    {
        //clear menu
        menu_remove_all_items( s_category );        
    }
    
    //generate display list
    plistclass_get_files( g_plist_level, 0, 0, NULL );
    
    //get display list total
    total = plistclass_get_item_num( PLIST_SHOWLIST );
    offset = s_category_global_offset;

    //init items
    rec_num = _category_init_item( items, offset, ID3_MAX_ITEMS );    
    menu_append_items( s_category, items, rec_num );    

    //init global index
    menu_set_global_offset( s_category, s_category_global_offset );
    menu_set_global_size( s_category, total );
    
    //init index
    menu_set_offset( s_category, s_category_offset );
    menu_set_cur_index( s_category, s_category_current );

    //set menu callback
    if( s_category != NULL )
    {
        s_category->callback = (menu_cb_t)_proc_items;       
    }    
    
    //disable MENU keymap
    music_keymap_control( KEY_MENU, false );
    
    return s_category;
}

int music_list_category_deinit( void )
{    
    int cur_index;
    menuitem_t *item; 

    //deinit coverflow
    coverflow_deinit( s_category );
        
    if ( true == g_coverflow_mode )
    {
        g_coverflow_mode = false;         
        scene_list_paint();
    }
    
    //delete menu    
    if( s_category != NULL )
    {
        //save index
        s_category_offset = s_category->offset;
        s_category_current = s_category->current;
        s_category_global_offset = s_category->global_offset;

        //free item buffers
        for( cur_index=0; cur_index<s_category->total; cur_index++ )
        {
            item = menu_get_item( s_category, cur_index );
            _category_deinit_item( item );
        }
        
        //delete menu
        menu_delete( s_category );
        s_category = NULL;
    }

    //abort album decode
    album_abort_decode();

    //deinit album buffer
    _album_deinit_buffer();  

    //kill timer
    kill_timer( g_scene_list->single_timer );
    g_scene_list->single_timer = -1;
    
    //enable MENU keymap
    music_keymap_control( KEY_MENU, true );
    
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
    bool need_rotate = false;  
    fusion_effect_t eff;  
    fusion_enter_e fenter;    
    
    //enter process
    state = plistclass_get_state( g_plist_level ); 
        
    switch ( signal ) 
    {
    case WIDGET_SIG_MENUVIEW_OK: 
        if ( cur_menu->total > 0 )
        {
            //save selected content
            if ( cur_item != NULL )
            {
                strcpy( g_usrsel_content.itemstr, cur_item->text );
            }
            
            music_list_category_deinit();   
            
            //check rotate
            need_rotate = _category_check_rotate();
            
            //set first
            if ( need_rotate == true )
            {
                fusion_get_config(FUSION_TYPE_ROTATE, FUSION_FORWARD, &eff);
                fusion_set_effect(&eff);
                fusion_set_first_frame(NULL);
                
                scene_list_rotation();
            }
            
            if ( need_rotate == true )
            {
                fenter = FUSION_MAX;
            }
            else
            {
                fenter = FUSION_FORWARD;
            }        

            menuview_show( g_scene_list->list );        

            //clear filter string
            plistclass_set_filter( NULL );    

            switch( state )
            {
            //第2级进入处理
            case STATE_ARTIST_LIST:
                plistclass_next_level( g_plist_level, LEVEL_TWO_LIST, LEVEL_THREE_FILELIST );
                
                //save selected content
                plistclass_store_usrselcontent( ARTIST, &g_usrsel_content );
                
                music_list_filelist_enter( menuview, fenter );
                break;
                
            case STATE_ALBUM_LIST:
                plistclass_next_level( g_plist_level, LEVEL_TWO_LIST, LEVEL_THREE_FILELIST );
                
                //save selected content
                plistclass_store_usrselcontent( ALBUM, &g_usrsel_content );
                
                music_list_filelist_enter( menuview, fenter );
                break;
                
            case STATE_GENRE_LIST:
                plistclass_next_level( g_plist_level, LEVEL_TWO_LIST, LEVEL_THREE_FILELIST );
                
                //save selected content
                plistclass_store_usrselcontent( GENRE, &g_usrsel_content );
                
                music_list_filelist_enter( menuview, fenter );
                break;
                
            default:
                break;            
            }        
            
            //set last
            if ( need_rotate == true )
            {
                fusion_set_last_frame(NULL);
                fusion_display(NULL);
            }
        }
        else
        {
            _create_info_widget(g_scene_list->str_nofile->str_id);
        }
        break;


    case WIDGET_SIG_MENUVIEW_QUIT:
        // return to parent menu
        music_list_category_exit( menuview );
        break;

          
    case WIDGET_SIG_MENUVIEW_EXCEED_FIRST:
        // add one item to first
        _category_init_item( items, cur_menu->global_offset, 1 );
        menu_prepend_item( cur_menu, &items[0] );
        
        // remove last item
        cur_item = menu_get_item( cur_menu, cur_menu->total - 1 );
        _category_deinit_item( cur_item );
        
        if ( true == g_coverflow_mode )
        {
            //clear slide bmp
            coverflow_set_slide( s_category, cur_menu->total );
        }
        
        menu_remove_last_item( cur_menu );
        break;

        
    case WIDGET_SIG_MENUVIEW_EXCEED_LAST:
        // remove first item
        cur_item = menu_get_item( cur_menu, 0 );
        _category_deinit_item( cur_item );
        
        if ( true == g_coverflow_mode )
        {
            //clear slide bmp
            coverflow_set_slide( s_category, -1 );
        }
        
        menu_remove_first_item( cur_menu );
        
        // add one item to last
        _category_init_item( items, cur_menu->global_offset+cur_menu->total, 1 );
        menu_append_item( cur_menu, &items[0] );
        break;

        
    case WIDGET_SIG_MENUVIEW_UPDATE_ALL:
        //read new items
        rec_num = _category_init_item( items, cur_menu->global_offset, cur_menu->total );
        
        //update all items
        for ( cur_index = 0; cur_index < rec_num; cur_index ++ )
        {
            cur_item = menu_get_item( cur_menu, cur_index );

            //deinit item
            _category_deinit_item( cur_item );
            
            if ( true == g_coverflow_mode )
            {
                //clear slide bmp
                coverflow_set_slide( s_category, cur_index );
            }
        
            //modify item
            menu_set_item( cur_menu, cur_index, &items[cur_index] );
        }
        break;
        
    case WIDGET_SIG_MENUVIEW_DOWN:          
        if ( state == STATE_ALBUM_LIST )
        {  
            if ( true == g_coverflow_mode )
            {
                coverflow_next( s_category );
                //draw coverflow
                coverflow_animation( g_scene_list->hdc, s_category );            
            }
             
            //abort album decode
            album_abort_decode();
            
            //album decode
            s_album_index = menuview_get_prefetch_start( menuview );  //cur_menu->offset;
            kill_timer( g_scene_list->single_timer );
            g_scene_list->single_timer = set_single_shot_timer( 1, (timer_proc)_album_start_decode, menuview );
        } 
        break;
        
    case WIDGET_SIG_MENUVIEW_UP:
        if ( state == STATE_ALBUM_LIST )
        {
            if ( true == g_coverflow_mode )
            {
                coverflow_prev( s_category );
                //draw coverflow
                coverflow_animation( g_scene_list->hdc, s_category );
            }
             
            //abort album decode
            album_abort_decode();
        
            //album decode
            s_album_index = menuview_get_prefetch_start( menuview );  //cur_menu->offset;
            kill_timer( g_scene_list->single_timer );
            g_scene_list->single_timer = set_single_shot_timer( 1, (timer_proc)_album_start_decode, menuview );
        } 
        break;
        
    case WIDGET_SIG_MENUVIEW_SHORT_MENU:      
        if ( cur_menu->total > 0 )
        {
            _create_softkeyboard_widget();
        }
        break;        
        
    case WIDGET_SIG_MENUVIEW_LONG_MENU:
        if ( state == STATE_ALBUM_LIST )
        {
            //abort album decode
            album_abort_decode();
            
            //kill timer
            kill_timer( g_scene_list->single_timer );
            g_scene_list->single_timer = -1;            

            //clear bitmap
            _category_deinit_bitmap( cur_menu );

            s_album_coverflow = ! s_album_coverflow;
            
            g_coverflow_mode = s_album_coverflow;
            if ( true == g_coverflow_mode )
            {
                menuview_hide( menuview );
            }
            else
            {
                menuview_show( menuview );
            }
                     
            scene_list_rotation();            
            
            if ( true == g_coverflow_mode )
            {
                //init coverflow
                coverflow_init( cur_menu );
            
                menuview_hide( menuview );
                coverflow_paint( g_scene_list->hdc, cur_menu );
            }
            else
            {
                //deinit coverflow
                coverflow_deinit( cur_menu );
            
                menuview_show( menuview );
                menuview_paint( menuview );
            }
             
            //abort album decode
            album_abort_decode();
        
            //start album decode
            s_album_index = menuview_get_prefetch_start( menuview );  //cur_menu->offset;
            kill_timer( g_scene_list->single_timer );
            g_scene_list->single_timer = set_single_shot_timer( 1, (timer_proc)_album_start_decode, menuview );
        }
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
        if ( true == g_coverflow_mode )
        {
            //hide coverflow
            coverflow_hide();
        }
        else
        {
            //hide mainmenu
            menuview_hide(g_scene_list->list);
        }
        
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
    bool save_mode;
    plistclass_state_e state;

    //enter process
    state = plistclass_get_state( g_plist_level );         

    //get display list number
    total = plistclass_get_item_num( PLIST_SHOWLIST );    
    if ( total <= 0 )
    {
        //clear filter string
        plistclass_set_filter( NULL );

        //update files
        plistclass_get_files( g_plist_level, 0, 0, NULL );
        
    }
    
    save_mode = g_coverflow_mode;  

    //delete category      
    music_list_category_deinit();
    s_category_offset = 0;
    s_category_current = 0;
    s_category_global_offset = 0;
    
    //update category
    cur_menu = music_list_category_init();    
    
    //check menu
    if ( cur_menu == NULL )
    {
        return;
    }

    menuview_set_menu( g_scene_list->list, cur_menu );        
    
    g_coverflow_mode = save_mode;  

    if ( true == g_coverflow_mode )
    {
        //init coverflow
        coverflow_init( cur_menu );
        coverflow_hide();
    }

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
        
        if ( true == g_coverflow_mode )
        {
            //draw coverflow
            coverflow_show();
            coverflow_paint( g_scene_list->hdc, cur_menu );
        }
        else
        {
            //show mainmenu
            menuview_show(g_scene_list->list);
            menuview_paint( g_scene_list->list ); 
        }
    }
     
    if ( state == STATE_ALBUM_LIST )
    {  
        //abort album decode
        album_abort_decode();
        
        //start album decode
        s_album_index = menuview_get_prefetch_start( g_scene_list->list );  //cur_menu->offset;
        kill_timer( g_scene_list->single_timer );
        g_scene_list->single_timer = set_single_shot_timer( 1, (timer_proc)_album_start_decode, 
                                                            g_scene_list->list );  
    } 
}

static void _deal_searching( void )
{
    //searching in thread
    plistclass_get_files( g_plist_level, 0, 0, NULL );

    //process after searching
    set_single_shot_timer( 1, (timer_proc)_searching_done, NULL );
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

        //searching
        plistclass_get_files( g_plist_level, 0, 0, NULL );
        
        //create loadingbox
        _create_loadingbox_widget();

        //create thread to search
        music_create_thread( _deal_searching, NULL );
        break;

    case WIDGET_SIG_SKB_CANCEL:
        //delete softkeyboard
        _delete_softkeyboard_widget();
        
        if ( true == g_coverflow_mode )
        {
            //draw coverflow
            coverflow_show();
            coverflow_paint( g_scene_list->hdc, g_scene_list->list->menu );
        }
        else
        {
            //show mainmenu
            menuview_show(g_scene_list->list);
            menuview_paint(g_scene_list->list);
        }
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

static bool _album_size_changed( gui_bitmap_info_t *bmp )
{
    int width;
    int height;

    if ( bmp == NULL )
    {
        return true;
    }

    if ( true == g_coverflow_mode )
    {
        width = coverflow_get_slide_width();
        height = coverflow_get_slide_height();
    }
    else
    {
        width = menuview_get_bitmap_width( g_scene_list->list );
        height = menuview_get_bitmap_height( g_scene_list->list );
    }

    if ( ( bmp->xsize != width ) || ( bmp->ysize != height ) )
    {
        return true;
    }
    else
    {
        return false;
    }
}

static bool _album_align_bitmap( gui_bitmap_info_t *bmp, mmm_img_info_t *img_info )
{
    char *buffer;
    unsigned int len;
    unsigned int line;
    char *src;
    char *dst;
    int xstart, ystart;
    
    if ( ( bmp->xsize < img_info->img_width ) || ( bmp->ysize < img_info->img_height ) )
    {
        return false;
    }
    
    if ( true == g_coverflow_mode )
    {
        //copy bitmap
        len = img_info->img_width * img_info->img_height * img_info->bpp;
        buffer = malloc( len );
        if ( buffer == NULL )
        {
            return false;
        }
        else
        {
            memcpy( buffer, img_info->buf, len );
        }        

        //clear bitmap
        len = bmp->xsize * bmp->ysize * bmp->bytesperpixel;
        memset( (char*)bmp->pdata, 0xff, len );

        //align bitmap
        xstart = (int)(bmp->xsize - img_info->img_width)/2;
        ystart = (int)(bmp->ysize - img_info->img_height)/2;
        
        src = buffer;
        dst = (char*)bmp->pdata + ( ( (ystart * bmp->xsize) + xstart ) * bmp->bytesperpixel );

        for ( line=0; line < img_info->img_height; line ++ )
        {
            memcpy( dst, src,  img_info->img_width * img_info->bpp );
            src +=  img_info->img_width * img_info->bpp;
            dst += bmp->xsize * bmp->bytesperpixel;
        }
        
        free( buffer );
    }
    else
    {
        bmp->xsize = (unsigned short)img_info->img_width;
        bmp->ysize = (unsigned short)img_info->img_height;
    }

    return true;
}

static void _album_init_buffer( menuview_t *menuview )
{
    int width;
    int height;
    int bpp = 2;
    
    if ( true == g_coverflow_mode )
    {
        width = coverflow_get_slide_width();
        height = coverflow_get_slide_height();
    }
    else
    {
        width = menuview_get_bitmap_width( menuview );
        height = menuview_get_bitmap_height( menuview );
    }

    if ( width < 0 )
    {
        width = 0;
    }

    if ( height < 0 )
    {
        height = 0;
    }

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
    
    memset( (char*)s_album_bmp->pdata, 0xff, (unsigned int)(width * height * bpp) );
    
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
    int rec_num;
    plist_detail_info_t *id3_info = NULL;
    bool repaint = false;

    for( ; s_album_index < s_category->total; s_album_index ++ )
    {
        item = menu_get_item( s_category, s_album_index );
        
        if( ( item == NULL ) || ( _album_size_changed(item->bitmap) != true ) )
        {
        }
        else
        {            
            if ( ( item->info2 == NULL ) && ( g_coverflow_mode == false ) )
            {   
                rec_num = plistclass_get_album_file_num( item->text );

                if( rec_num < 0 )
                {
                    rec_num = 0;
                }

                if ( item->info2 != NULL )
                {
                    free(item->info2);
                }
                
                //fill info2
                item->info2 = malloc(16);
                if ( item->info2 != NULL )
                {
                    snprintf(item->info2, 16, "%d", rec_num);
                }
                repaint = true;
            }
            
            plistclass_get_files( g_plist_level, s_category->global_offset+s_album_index, 1, &id3_info );
                
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

    //repaint
    if ( repaint == true )
    {
        menuview_paint( g_scene_list->list );
    }
}

static void _album_decode_callback( void )
{
    mmm_img_info_t img_info;
    menuitem_t *item;

    item = menu_get_item( s_category, s_album_index );
    if( item == NULL )
    {
        print_err();
        return;
    }
    else
    {
        //change bitmap size
        album_get_image_info( &img_info );
        _album_align_bitmap( s_album_bmp, &img_info );

        //set album bmp to item
        sty_delete_bitmap( item->bitmap );
        item->bitmap = s_album_bmp;
        s_album_bmp = NULL;
        
        if ( true == g_coverflow_mode )
        {
            //set slide bmp
            coverflow_set_slide( s_category, s_album_index );
            
            //draw coverflow
            coverflow_paint( g_scene_list->hdc, s_category );
        }
        else
        {
            //draw bitmap
            menuview_paint_bitmap( g_scene_list->list, s_album_index );
        }
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
//            music_list_category_exit( g_scene_list->list );
        }
        break;

    default:
        break;
    }
}

