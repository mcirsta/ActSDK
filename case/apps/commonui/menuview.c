/********************************************************************************
 *                              USDK(1100)
 *                             Module: app 
 *                 Copyright(c) 2003-2009 Actions Semiconductor,
 *                            All Rights Reserved. 
 * File : menuview.c
 * History:         
 *      <author>    <time>           <version >             <desc>
 *    songzhining    2009-2-25          v1.0               build this file 
 ********************************************************************************/
/*!
* \file     menuview.c
* \brief    menuview widget
* \author   songzhining
*               
* \version 1.0
* \date  2009/02/25
*******************************************************************************/
    
#include "include_case.h"


#include "commonui_sty.h"

#include "menuview_private.h"

static void _slider_callback( slider_t *slider, int signal );
static bool _proc_callback( menuview_t *menuview, menuview_sig_e signal );
static bool _check_offset( menuview_t *menuview );


static void _slider_callback( slider_t *slider, int signal )
{
}

static bool _proc_callback( menuview_t *menuview, menuview_sig_e signal )
{
    menu_t *menu = menuview->menu;
    menuitem_t *item;    

    item = menu_get_cur_item( menu ); 
    
    if ( ( item != NULL ) && ( item->callback != NULL ) )
    {
        item->callback( menuview, signal );    //send signal to current item callback
    }
    else if ( ( menu != NULL ) && ( menu->callback != NULL ) )
    {
        menu->callback( menuview, signal );    //send signal to current item callback               
    }
    else if ( menuview->callback != NULL )
    {
        menuview->callback( menuview, signal );    //send signal to menuview callback
    }
    else
    {
    }
    
    return true;
}

/* return true if offset changed */
static bool _check_offset( menuview_t *menuview )
{
    menu_t *menu = menuview->menu;
    bool bret = false;
    
    if ( menuview->menu == NULL )
    {
        bret = false;   
    }
    else
    {
        //modify offset
        if ( menu->total > 0 )
        {
            //less than one screen
            if ( menu->total <= menuview->private->lines )
            {
                menu->offset = 0;   
                bret = false;          
            }
            else    //more than one screen
            {  
                //insure screen area inside menu area
                if ( menu->offset < 0 )
                {
                    menu->offset = 0;    
                    bret = true;         
                }
                else if ( ( menu->offset + menuview->private->lines ) > menu->total )
                {
                    menu->offset = menu->total - menuview->private->lines;    
                    bret = true;          
                }
                else
                {        
                }
                
                //insure current item in screen area
                if (  menu->offset > menu->current )
                {
                    menu->offset = menu->current;      
                    bret = true;        
                }
                else if ( ((menu->offset + menuview->private->lines) - 1) < menu->current )
                {
                    menu->offset = menu->current - ( menuview->private->lines - 1 );  
                    bret = true;            
                }    
                else
                {        
                }   
            }            
        }  
        else
        { 
            menu->total = 0;
            menu->current = 0;
            menu->offset = 0; 
            bret = true;    
        } 
    }
    
    return bret;
}

/* menuview external interface */
bool menuview_paint( menuview_t *menuview )
{
    int line;
    
    //check param
    if ( menuview == NULL )
    {
        return false;    
    }
    else
    {
    }

    //check visible
    if ( menuview->private->is_hidden == true )
    {
        return false;    
    }
    else
    {
    }

    //try to kill scroll timer
    kill_timer( menuview->private->scroll_timer );
    menuview->private->scroll_timer = -1;    

    //check bg init
    if ( menuview->private->bg_inited == false )
    {
        //draw menu bg
        menuview_draw_bg( menuview );  
    }

    if ( menuview->menu == NULL )
    {
        return false;    
    }
    else
    {
        //check offset
        _check_offset( menuview );
    }

    //draw all lines
    for ( line = 0; line < menuview->private->lines; line++ )
    {
        menuview_draw_line( menuview, menuview->menu->offset + line );
    }

    //draw slider
    menuview_draw_slider( menuview );
    
    //check bg init
    if ( menuview->private->bg_inited == false )
    {
        menuview->private->bg_inited = true;        
    }

    return true;
}


static bool menuview_proc_key( menuview_t *menuview, key_event_t *pmsg )
{
    menuview_private_t *private;
    menu_t *menu;
    bool offset_changed = false;
    bool update_all = false;
    bool is_option = false;
    int last_index;

    //check param
    if ( menuview == NULL )
    {
        return false;
    }
    else
    {
        private = menuview->private;
        menu = menuview->menu;    
    }

    //option type
    switch( menuview->type )
    {
    case MENUVIEW_OPTION:
    case MENUVIEW_OPTION_INFO:
        is_option = true;
        break;
        
    default:
        is_option = false;
        break;
    }

    switch ( pmsg->val | pmsg->type ) 
    {
    case KEY_PREV | KEY_TYPE_DOWN:
    case KEY_PREV | KEY_TYPE_LONG:
    case KEY_PREV | KEY_TYPE_HOLD:
        {
            if ( menu == NULL )
            {
                return false;
            }
            else
            {
                last_index = menu->current;
            }
            
            //in normal area
            if ( menu->current > private->prefetch )
            {
                menu->current --;    //current dec 
            }
            else    //in prefetch area
            {
                //static all mode
                if ( menu->global_size <= menu->total )
                {
                    if ( menu->current > 0 )
                    {
                        menu->current --;    //current dec 
                    }
                    else
                    { 
                        menu->current = menu->total - 1;
                        update_all = true;
                    }
                }
                else    //dynamic part mode
                {
                    if ( ( menu->current == menu->offset ) //screen first line
                        && ( menu->global_offset > 0 ) )
                    {
                        menu->global_offset --;    //buffer offset dec                                    
                        _proc_callback( menuview, WIDGET_SIG_MENUVIEW_EXCEED_FIRST );    //update data in the buffer
                        
                        menu->current --;    //current dec 
                        update_all = true;
                    }
                    else if ( ( menu->global_offset + menu->current ) == 0 )  //global first item
                    {
                        //goto last item
                        menu->global_offset = menu->global_size - menu->total;    //buffer offset    
                        menu->current = menu->total - 1;    
                        _check_offset( menuview );  //check offset 
                        
                        _proc_callback( menuview, WIDGET_SIG_MENUVIEW_UPDATE_ALL );    //update all data
                        
                        menu->current = menu->total - 1;    //for safe        
                        update_all = true;
                    }
                    else
                    {
                        menu->current --;    //current dec 
                    }
                }
            }
          
            if ( menu->total > 0 )
            {
                offset_changed = _check_offset( menuview );  //check offset 
                    
                if ( ( offset_changed == true ) || ( update_all == true ) )
                {
                    menuview_paint( menuview );    //repaint all
                }
                else
                { 
                    if ( is_option == true )
                    {
                        menuview_draw_line( menuview, last_index + 1 );
                    }
                    
                    menuview_draw_line( menuview, last_index );
                    menuview_draw_line( menuview, menu->current );            
                }
                
                _proc_callback( menuview, WIDGET_SIG_MENUVIEW_UP );    //send signal
            }       
            
            //clear key
            clear_key_msg_queue(-1,-1);
        }
        break;
        
    case KEY_NEXT | KEY_TYPE_DOWN:
    case KEY_NEXT | KEY_TYPE_LONG:
    case KEY_NEXT | KEY_TYPE_HOLD:
        {
            if ( menu == NULL )
            {
                return false;
            }
            else
            {
                last_index = menu->current;
            }
            
            //in normal area
            if ( ( menu->current + private->prefetch ) < ( menu->total - 1 ) )
            {
                menu->current ++;    //current inc
            }
            else    //in prefetch area
            {
                //static all mode
                if ( menu->global_size <= menu->total )
                {
                    if ( menu->current < ( menu->total - 1 ) )
                    {
                        menu->current ++;    //current inc
                    }
                    else
                    { 
                        menu->current = 0;
                        update_all = true;
                    }
                }
                else    //dynamic part mode
                {
                    if ( ( menu->current == ( ( menu->offset + private->lines ) - 1 ) ) //screen last line
                        && ( (menu->global_offset + menu->total) < menu->global_size ) ) 
                    {
                        menu->global_offset++;    //buffer offset inc
                        _proc_callback( menuview, WIDGET_SIG_MENUVIEW_EXCEED_LAST );    //update data in the buffer
                        
                        menu->current++;    //current inc
                        update_all = true;
                    }
                    else if ( (menu->global_offset + menu->current) == (menu->global_size - 1) ) //global last item
                    {
                        //goto first item
                        menu->global_offset = 0;    //buffer offset  
                        menu->current = 0;
                        _check_offset( menuview );  //check offset 
                        
                        _proc_callback( menuview, WIDGET_SIG_MENUVIEW_UPDATE_ALL );    //update all data

                        menu->current = 0;  //for safe
                        update_all = true;
                    }
                    else
                    {
                        menu->current ++;    //current inc
                    }
                }
            }
          
            if ( menu->total > 0 )
            {
                offset_changed = _check_offset( menuview );  //check offset 
                    
                if ( ( offset_changed == true ) || ( update_all == true ) )
                {
                    menuview_paint( menuview );    //repaint all 
                }
                else
                {
                    if ( is_option == true )
                    {
                        menuview_draw_line( menuview, menu->current + 1 );
                    }   
                    
                    menuview_draw_line( menuview, menu->current ); 
                    menuview_draw_line( menuview, last_index );                  
                }
                
                _proc_callback( menuview, WIDGET_SIG_MENUVIEW_DOWN );    //send signal
            }
            
            //clear key
            clear_key_msg_queue(-1,-1);
        }
        break;
        
    case KEY_OK | KEY_TYPE_SHORT_UP:
    case KEY_OK | KEY_TYPE_LONG_UP:
    case KEY_OK | KEY_TYPE_HOLD_UP:
        {
            if ( menu == NULL )
            {
                return false;
            }
            //external process
            _proc_callback( menuview, WIDGET_SIG_MENUVIEW_OK );    
            
            //clear key
            clear_key_msg_queue(-1,-1);
        }
        break;
        
    case KEY_RETURN | KEY_TYPE_SHORT_UP:
    case KEY_RETURN | KEY_TYPE_LONG_UP:
    case KEY_RETURN | KEY_TYPE_HOLD_UP:
        {    
            //external process
            _proc_callback( menuview, WIDGET_SIG_MENUVIEW_QUIT );   
            
            //clear key
            clear_key_msg_queue(-1,-1); 
        }
        break;
        
    case KEY_MENU | KEY_TYPE_LONG:
        {
            //external process
            _proc_callback( menuview, WIDGET_SIG_MENUVIEW_LONG_MENU );  
            
            //clear key
            clear_key_msg_queue(-1,-1);
        }
        break;
        
    case KEY_MENU | KEY_TYPE_SHORT_UP:
        {
            //external process
            _proc_callback( menuview, WIDGET_SIG_MENUVIEW_SHORT_MENU );  
            
            //clear key
            clear_key_msg_queue(-1,-1);
        }
        break;
        
    case KEY_DELETE | KEY_TYPE_DOWN:
        {
            //external process
            _proc_callback( menuview, WIDGET_SIG_MENUVIEW_DELETE );  
            
            //clear key
            clear_key_msg_queue(-1,-1);
        }
        break;
        
    default:
        break;
    }      

    return true;
}



/* menuview entry */
bool menuview_proc_gui( menuview_t *menuview, gui_wm_message_t *pmsg )
{
    bool ret = false;
    
    switch ( pmsg->msgid ) 
    {
//    case WM_MSG_PAINT:
//        ret = menuview_paint( menuview );
//        break;
         
    case WM_MSG_KEY:
        ret = menuview_proc_key( menuview, (key_event_t *)(pmsg->data.p) );
        break;
        
    case WM_MSG_TOUCH:
        break;
        
    default:
        break;
    }
    
    return ret;
}


menuview_t* menuview_create( menuview_type_e type, menuview_cb_t callback )
{
    menuview_t *menuview;
    int slider_type;
    bool ret;
    resgroup_resource_t *group;
    sty_rect_t rect;
       
    if ( callback == NULL )
    {
        print_err("callback is null!");
        return NULL;    
    }
    
    //malloc data
    menuview = (menuview_t *)malloc( sizeof(menuview_t) );        
    if ( menuview == NULL )
    {
        print_err("malloc failed!");
        goto menuview_err;    
    }    
    else
    {
        //initial data
        menuview->menu = NULL;
        menuview->callback = callback;
        menuview->canvas_id = -1;
    }
    
    //malloc private data
    menuview->private = (menuview_private_t *)malloc( sizeof(menuview_private_t) );        
    if ( menuview->private == NULL )
    {
        print_err("malloc failed!");
        goto private_err;    
    }    
    else
    {
        //initial private
        memset( menuview->private, 0, sizeof(menuview_private_t) );
        menuview->private->scroll_timer = -1;
        menuview->private->is_hidden = false;   //控件需要显示
        menuview->private->prefetch = 0;
    }
    
    //create slider
    if ( ( type == MENUVIEW_OPTION ) || ( type == MENUVIEW_OPTION_INFO ) )
    {
        slider_type = W_SB_TYPE_OPTION;
    }
    else
    {
        slider_type = W_SB_TYPE_LIST;
    }
    
    //load style resource
    ret = menuview_load_style( menuview, type );
    if ( ret == false )
    {
        print_err("canvas_create failed!");
        goto style_err;
    }    

    //create canvas
    group = menuview->private->resgroup;
    rect.x = group->absolute_x;
    rect.y = group->absolute_y;
    rect.width = group->width;
    rect.height = group->height;
    
    menuview->canvas_id = canvas_create( &rect, TRANSPARENT ); 
    if ( menuview->canvas_id == -1 )
    {
        print_err("canvas_create failed!");
        goto canvas_err;
    }
    
    //获取dc
    menuview->private->hdc = gui_dc_get( menuview->canvas_id );

    //create slider
    menuview->private->slider = slider_create( slider_type, _slider_callback, menuview->canvas_id );
    if (menuview->private->slider == NULL)
    {
        print_err("slider_create failed!");
        goto slider_err;
    }
    else
    {
        //canvas_disable_focus(menuview->private->slider->canvas_id);
    }
     
    return menuview;    
    
    slider_err:
    canvas_delete(menuview->canvas_id);

    canvas_err:
        
    style_err:
    free( menuview->private );

    private_err:
    free( menuview );

    menuview_err:

    return NULL;
}


bool menuview_delete( menuview_t *menuview )
{
    if ( menuview != NULL )
    {    
        if ( menuview->private != NULL )
        {
            //删除控件
            slider_delete( menuview->private->slider );
            menuview->private->slider = NULL;
            
            //try to kill scroll timer
            kill_timer( menuview->private->scroll_timer );
            menuview->private->scroll_timer = -1;
            
            //delete text scroll
            sty_delete_scroll_string( menuview->private->scroll_info );
            
            //delete scroll bg
            sty_delete_bitmap( menuview->private->scroll_bg );
            sty_delete_bitmap( menuview->private->bmp_bg );
            sty_delete_bitmap( menuview->private->bmp_sel_line );
            sty_delete_bitmap( menuview->private->bmp_sel_icon_bg );
            sty_delete_bitmap( menuview->private->bmp_sel_son );
            sty_delete_bitmap( menuview->private->bmp_unsel_line );
            sty_delete_bitmap( menuview->private->bmp_unsel_icon_bg );
            sty_delete_bitmap( menuview->private->bmp_unsel_son );
            sty_delete_bitmap( menuview->private->bmp_sel_folder );
            sty_delete_bitmap( menuview->private->bmp_unsel_folder );
        }   
    
        //delete canvas
        if ( menuview->canvas_id != -1 )
        {
            canvas_delete( menuview->canvas_id );
            menuview->canvas_id = -1;
        }
         
        //free private 
        if ( menuview->private != NULL )
        {
            free( menuview->private );
            menuview->private = NULL;
        }            

        //free menuview private
        free( menuview );
    }
    
    return true;
}

/* menuview load style resource */
bool menuview_load_style( menuview_t *menuview, menuview_type_e type )
{
    menuview_private_t *private;
    scene_t* scene;    //!< commonui scene
    resgroup_resource_t *group;
    sty_rect_t rect;
    int sty_id;
    
    if ( menuview == NULL )
    {
        print_warning("menuview is null!");
        return false;
    }
    else
    {
        private = menuview->private;
        menuview->type = type;
    }
    
    //get root resource
    scene = sty_get_common_scene();
    if ( scene == NULL )
    {
        print_err("cann't find common scene!");
        return false;    
    }

    //load control group
    group = ( resgroup_resource_t * )get_scene_child( scene, MENUVIEW_RESGROUP );
    
    //get child resgroup
    switch( menuview->type )
    {
    case MENUVIEW_OPTION:
        sty_id = OPTION_RESGROUP;
        break;
        
    case MENUVIEW_OPTION_INFO:
        sty_id = OPTION_INFO_RESGROUP;
        break;        
        
    case MENUVIEW_MENULIST_TEXT:
        sty_id = MENULIST_TEXT_RESGROUP;
        break;
        
    case MENUVIEW_MENULIST_ICON:
        sty_id = MENULIST_ICON_RESGROUP;
        break;
        
    case MENUVIEW_FILELIST_TEXT:
        sty_id = FILELIST_TEXT_RESGROUP;
        break;
        
    case MENUVIEW_FILELIST_ICON:
        sty_id = FILELIST_ICON_RESGROUP;
        break;
        
    case MENUVIEW_PICLIST_MUSIC:
        sty_id = PICLIST_MUSIC_RESGROUP;
        break;        
        
    case MENUVIEW_PICLIST_VIDEO:
        sty_id = PICLIST_VIDEO_RESGROUP;
        break;        
        
    case MENUVIEW_PICLIST_PICTURE:
        sty_id = PICLIST_PICTURE_RESGROUP;
        break;        
        
    case MENUVIEW_PICLIST_EBOOK:
        sty_id = PICLIST_EBOOK_RESGROUP;
        break; 

    case MENUVIEW_PICLIST_INFO:
        sty_id = PICLIST_INFO_RESGROUP;
        break;        

    default:
        sty_id = 0;
        break;
    }
    
    //load resgroup
    private->resgroup = ( resgroup_resource_t * )get_resgroup_child( group, (unsigned int)sty_id );
        
    if ( private->resgroup == NULL )
    {
        print_err("cann't find resgroup!");
        return false;    
    }
    
    //load resource
    private->pic_sel_folder = ( picture_resource_t * )get_resgroup_child( group, PIC_SEL_FOLDER );
    private->pic_unsel_folder = ( picture_resource_t * )get_resgroup_child( group, PIC_UNSEL_FOLDER );
    
    //load bg
    private->pic_bg = ( picture_resource_t * )get_resgroup_child( private->resgroup, PIC_BG );
    
    /* selected line */
    private->pic_sel_line = ( picture_resource_t * )get_resgroup_child( private->resgroup, PIC_SEL_LINE );
    private->pic_sel_icon_bg = ( picture_resource_t * )get_resgroup_child( private->resgroup, PIC_SEL_ICON_BG );
    private->pic_sel_icon = ( picture_resource_t * )get_resgroup_child( private->resgroup, PIC_SEL_ICON );
    private->str_sel_name = ( string_resource_t * )get_resgroup_child( private->resgroup, STR_SEL_NAME );
    private->str_sel_str1 = ( string_resource_t * )get_resgroup_child( private->resgroup, STR_SEL_STR1 );
    private->str_sel_str2 = ( string_resource_t * )get_resgroup_child( private->resgroup, STR_SEL_STR2 );    
    private->pic_sel_son = ( picture_resource_t * )get_resgroup_child( private->resgroup, PIC_SEL_SON );

    /* unselected line */
    private->pic_unsel_line = ( picture_resource_t * )get_resgroup_child( private->resgroup, PIC_UNSEL_LINE );
    private->pic_unsel_icon_bg = ( picture_resource_t * )get_resgroup_child( private->resgroup, PIC_UNSEL_ICON_BG );
    private->pic_unsel_icon = ( picture_resource_t * )get_resgroup_child( private->resgroup, PIC_UNSEL_ICON );
    private->str_unsel_name = ( string_resource_t * )get_resgroup_child( private->resgroup, STR_UNSEL_NAME );
    private->str_unsel_str1 = ( string_resource_t * )get_resgroup_child( private->resgroup, STR_UNSEL_STR1 );
    private->str_unsel_str2 = ( string_resource_t * )get_resgroup_child( private->resgroup, STR_UNSEL_STR2 );    
    private->pic_unsel_son = ( picture_resource_t * )get_resgroup_child( private->resgroup, PIC_UNSEL_SON );

    //free bitmap
    sty_delete_bitmap( private->bmp_bg );
    private->bmp_bg = NULL;
    sty_delete_bitmap( private->bmp_sel_line );
    private->bmp_sel_line = NULL;
    sty_delete_bitmap( private->bmp_sel_icon_bg );
    private->bmp_sel_icon_bg = NULL;
    sty_delete_bitmap( private->bmp_sel_son );
    private->bmp_sel_son = NULL;
    sty_delete_bitmap( private->bmp_unsel_line );
    private->bmp_unsel_line = NULL;
    sty_delete_bitmap( private->bmp_unsel_icon_bg );
    private->bmp_unsel_icon_bg = NULL;
    sty_delete_bitmap( private->bmp_unsel_son );
    private->bmp_unsel_son = NULL;
    sty_delete_bitmap( private->bmp_sel_folder );
    private->bmp_sel_folder = NULL;
    sty_delete_bitmap( private->bmp_unsel_folder );
    private->bmp_unsel_folder = NULL;
    
    //cache bitmap
    if ( private->pic_bg != NULL )
    {
        private->bmp_bg = sty_create_bitmap( (int)private->pic_bg->pic_id );
    }
    
    if ( private->pic_sel_line != NULL )
    {
        private->bmp_sel_line = sty_create_bitmap( (int)private->pic_sel_line->pic_id );
    }
    
    if ( private->pic_sel_icon_bg != NULL )
    {
        private->bmp_sel_icon_bg = sty_create_bitmap( (int)private->pic_sel_icon_bg->pic_id );
    }
    
    if ( private->pic_sel_son != NULL )
    {
        private->bmp_sel_son = sty_create_bitmap( (int)private->pic_sel_son->pic_id );
    }
    
    if ( private->pic_unsel_line != NULL )
    {
        private->bmp_unsel_line = sty_create_bitmap( (int)private->pic_unsel_line->pic_id );
    }
    
    if ( private->pic_unsel_icon_bg != NULL )
    {
        private->bmp_unsel_icon_bg = sty_create_bitmap( (int)private->pic_unsel_icon_bg->pic_id );
    }
    
    if ( private->pic_unsel_son != NULL )
    {
        private->bmp_unsel_son = sty_create_bitmap( (int)private->pic_unsel_son->pic_id );
    }
    
    if ( private->pic_sel_folder != NULL )
    {
        private->bmp_sel_folder = sty_create_bitmap( (int)private->pic_sel_folder->pic_id );
    }
    
    if ( private->pic_unsel_folder != NULL )
    {
        private->bmp_unsel_folder = sty_create_bitmap( (int)private->pic_unsel_folder->pic_id );
    }

    if ( ( private->pic_unsel_line != NULL )
            && ( private->pic_sel_line != NULL )
            && ( private->str_sel_name != NULL ) )
    { 
        //get coordinate for painting
        switch( menuview->type )
        {
        case MENUVIEW_OPTION:
        case MENUVIEW_OPTION_INFO:
            private->sel_height = private->pic_unsel_line->height;
            break;
            
        default:
            private->sel_height = private->pic_sel_line->height;
            break;
        }
        
        private->unsel_height = private->pic_unsel_line->height;
        private->xstart = private->pic_sel_line->x;
        private->ystart = private->pic_sel_line->y;        
        
        //compute lines
        private->lines = ( (((private->resgroup->height - private->ystart) \
                                - private->sel_height) + 2 ) / private->unsel_height) + 1;
        print_dbg( "lines = %d", private->lines );
        
        //update scroll bg
        sty_delete_bitmap( private->scroll_bg );
        rect.x = private->str_sel_name->x - private->pic_sel_line->x;
        rect.y = private->str_sel_name->y - private->pic_sel_line->y;
        rect.width = private->str_sel_name->width;
        rect.height = private->str_sel_name->height;         
        private->scroll_bg = sty_create_sub_bitmap( (int)private->pic_sel_line->pic_id, &rect );
        if ( private->scroll_bg == NULL )
        {
            print_err("cann't create bitmap!");
            return false;    
        }
    }
    else
    {
        return false;
    }
    
    //canvas resize    
    if ( menuview->canvas_id != -1 )
    {
        group = menuview->private->resgroup;
        rect.x = group->absolute_x;
        rect.y = group->absolute_y;
        rect.width = group->width;
        rect.height = group->height;
        
        //set canvas region
        canvas_set_region( menuview->canvas_id, &rect );
        
        //update slider style
        slider_load_style( menuview->private->slider );    
    }

    //bg init flag
    menuview->private->bg_inited = false;
    
    return true;
}

bool menuview_show( menuview_t *menuview )
{
    //change menu private
    if ( menuview != NULL )
    {
        menuview->private->is_hidden = false;   //控件需要显示
        canvas_show( menuview->canvas_id );        
        canvas_show( menuview->private->slider->canvas_id );   
        return true;
    }
    else
    {
        return false;
    }
}

bool menuview_hide( menuview_t *menuview )
{
    //change menu private
    if ( menuview != NULL )
    {
        menuview->private->is_hidden = true;   //控件需要隐藏
        canvas_hide( menuview->canvas_id );     
        canvas_hide( menuview->private->slider->canvas_id );          
        return true;
    }
    else
    {
        return false;
    }
}

/* menuview extra interface */
bool menuview_set_menu( menuview_t *menuview, menu_t* menu )
{    
    //change menu private
    if ( menuview != NULL )
    {
        menuview->menu = menu;        
        return true;
    }
    else
    {
        return false;
    }
}

menu_t* menuview_get_menu( menuview_t *menuview )
{
    menu_t *menu;
    
    if ( menuview != NULL )
    {
        menu = menuview->menu;
    }
    else
    {
        menu = NULL;
    }
    
    return menu;
}

bool menuview_set_lines( menuview_t *menuview, int lines )
{
    if ( ( menuview != NULL ) && ( menuview->private != NULL ) )
    {
        menuview->private->lines = lines;
        return true;
    }
    else
    {
        return false;
    }    
}

int menuview_get_lines( menuview_t *menuview )
{
    if ( ( menuview != NULL ) && ( menuview->private != NULL ) )
    {
        return menuview->private->lines;
    }
    else
    {
        return -1;
    }    
}

/* menu item advanced interface */
int menuview_get_bitmap_width( menuview_t *menuview )
{
    if ( ( menuview != NULL ) && ( menuview->private->pic_sel_icon != NULL ) )
    {
        return menuview->private->pic_sel_icon->width;
    }
    else
    {
        return -1;
    }
}

int menuview_get_bitmap_height( menuview_t *menuview )
{
    if ( ( menuview != NULL ) && ( menuview->private->pic_sel_icon != NULL ) )
    {
        return menuview->private->pic_sel_icon->height;
    }
    else
    {
        return -1;
    }
}

/* set prefetch lines */
bool menuview_set_prefetch_num( menuview_t *menuview, int lines )
{
    if ( ( menuview != NULL ) && ( menuview->private != NULL ) )
    {
        menuview->private->prefetch = lines;
        return true;
    }
    else
    {
        return false;
    }    
}

int menuview_get_prefetch_num( menuview_t *menuview )
{
    if ( ( menuview != NULL ) && ( menuview->private != NULL ) )
    {
        return menuview->private->prefetch;
    }
    else
    {
        return -1;
    }   
}

int menuview_get_prefetch_start( menuview_t *menuview )
{
    int start;
    
    if ( ( menuview != NULL ) && ( menuview->menu != NULL ) )
    {
        start = menuview->menu->current - menuview->private->prefetch;
        if ( start < 0 )
        {
            start = 0;
        }
        
        if ( start > menuview->menu->offset )
        {
            start = menuview->menu->offset; //get min
        }
        
        
        return start;
    }
    else
    {
        return -1;
    }   
}

