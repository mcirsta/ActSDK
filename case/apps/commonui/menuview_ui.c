/********************************************************************************
 *                              USDK(1100)
 *                             Module: app 
 *                 Copyright(c) 2003-2009 Actions Semiconductor,
 *                            All Rights Reserved. 
 * File : menuview_ui.c
 * History:         
 *      <author>    <time>           <version >             <desc>
 *    songzhining    2009-2-25          v1.0               build this file 
 ********************************************************************************/
/*!
* \file     menuview_ui.c
* \brief    menuview widget
* \author   songzhining
*               
* \version 1.0
* \date  2009/02/25
*******************************************************************************/
    
#include "include_case.h"


#include "commonui_sty.h"

#include "menuview_private.h"

/* menuview draw background picture*/
bool menuview_draw_bg( menuview_t *menuview )
{
    menuview_private_t *private = menuview->private;
    
    //erase
    canvas_erase_all( menuview->canvas_id );

    //draw menu bg
    if ( private->pic_bg != NULL )
    {
        sty_draw_bitmap( private->hdc, private->bmp_bg,
                        private->pic_bg->x, private->pic_bg->y );
    }

    //try to kill scroll timer
    kill_timer( private->scroll_timer );
    private->scroll_timer = -1;                

    return true;
}

/* timer function to scroll text */
void scroll_timer_event( void *param )
{
    menuview_t* menuview = ( menuview_t* )param;
    menuview_private_t *private = menuview->private;
    sty_rect_t rect;

    //erase rect
    rect.x = (short)private->scroll_bg_x;
    rect.y = (short)private->scroll_bg_y;
    rect.width = private->str_sel_name->width;
    rect.height = private->str_sel_name->height;
    canvas_erase( menuview->canvas_id, &rect );

    //draw menu bg
    if ( private->pic_bg != NULL )
    {
        sty_draw_sub_bitmap( private->hdc, private->bmp_bg, rect.x, rect.y, &rect );
    }
    
    //draw scroll bg
    sty_draw_bitmap( private->hdc,  private->scroll_bg, private->scroll_bg_x, private->scroll_bg_y );
    
    //scroll text	
    sty_draw_scroll_string( private->hdc, private->str_sel_name, private->scroll_info );    
}

/* menuview draw one line */
bool menuview_draw_line( menuview_t *menuview, int cur_index )
{
    menuview_private_t *private = menuview->private;
    menu_t *menu = menuview->menu;
    menuitem_t *item;
    sty_rect_t rect;
    short y, yoffset = 0;
    short temp;
    short width, height;

    int icon_id;
    picture_resource_t *pic_line;
    picture_resource_t *pic_icon_bg;
    picture_resource_t *pic_icon;
    string_resource_t *str_name;
    string_resource_t *str_info1;
    string_resource_t *str_info2;
    picture_resource_t *pic_son;
    
    gui_bitmap_info_t *bmp_line;  //bitmap cache
    gui_bitmap_info_t *bmp_icon_bg;  //bitmap cache
    gui_bitmap_info_t *bmp_son;  //bitmap cache
    gui_bitmap_info_t *bmp_folder;  //bitmap cache
    
    if ( ( cur_index >= menu->offset ) && ( cur_index < (menu->offset + private->lines) ) )
    {
        //compute yoffset
        if ( cur_index <= menu->current )
        {
            yoffset = (short)(private->unsel_height * ( cur_index - menu->offset ));
        }
        else
        {
            yoffset = (short)(private->sel_height + (private->unsel_height * ( (cur_index - menu->offset) - 1 )));
        }
        
        //selected line
        if ( cur_index == menu->current )
        {
            pic_line = private->pic_sel_line;
            pic_icon_bg = private->pic_sel_icon_bg;
            pic_icon = private->pic_sel_icon;
            str_name = private->str_sel_name;
            str_info1 = private->str_sel_str1;
            str_info2 = private->str_sel_str2;
            pic_son = private->pic_sel_son;
            
            bmp_line = private->bmp_sel_line;
            bmp_icon_bg = private->bmp_sel_icon_bg;
            bmp_son = private->bmp_sel_son;
            bmp_folder = private->bmp_sel_folder;

            width = private->pic_sel_line->width;
            height = (short)private->sel_height;
        }
        else    //unselected line
        {
            pic_line = private->pic_unsel_line;
            pic_icon_bg = private->pic_unsel_icon_bg;
            pic_icon = private->pic_unsel_icon;
            str_name = private->str_unsel_name;
            str_info1 = private->str_unsel_str1;
            str_info2 = private->str_unsel_str2;
            pic_son = private->pic_unsel_son;
            
            bmp_line = private->bmp_unsel_line;
            bmp_icon_bg = private->bmp_unsel_icon_bg;
            bmp_son = private->bmp_unsel_son;
            bmp_folder = private->bmp_unsel_folder;
            
            width = private->pic_sel_line->width;
            height = (short)private->unsel_height;
            
            //unselected line is second line
            yoffset -=  (short)private->sel_height;            
        }

        //erase canvas rect
        if ( pic_line != NULL )
        {
            rect.x = (short)private->xstart;//pic_line->x;
            rect.y = pic_line->y + yoffset;
        }
        else
        {
            rect.x = 0;
            rect.y = 0;
        }
        rect.width = width;
        rect.height = height;        
        //check bg init
        if ( private->bg_inited == true )
        {
            canvas_erase( menuview->canvas_id, &rect );
            
            //draw bg
            if ( private->bmp_bg != NULL )
            {
                sty_draw_sub_bitmap( private->hdc, private->bmp_bg, rect.x, rect.y, &rect );
            }        
        }
        
        //get item 
        item = menu_get_item( menu, cur_index );
        if ( item == NULL )
        {
            return false;
        }  

        //selected line
        if ( cur_index == menu->current )
        {
            icon_id = item->sel_icon_id;
        }
        else    //unselected line
        {
            icon_id = item->unsel_icon_id;         
        }

        //draw hight line
        if ( pic_line != NULL )
        {
            y = pic_line->y + yoffset;
            sty_draw_bitmap( private->hdc, bmp_line, pic_line->x, y );
        }
        
        //draw icon bg
        if ( pic_icon_bg != NULL )  //if static icon exist, then don't paint icon bg
        {
            y = pic_icon_bg->y + yoffset;
            
            if ( item->has_child == true )
            {
                sty_draw_bitmap( private->hdc, bmp_folder, pic_icon_bg->x, y );
            }
            else
            {
                sty_draw_bitmap( private->hdc, bmp_icon_bg, pic_icon_bg->x, y );
            }
        }

        //draw icon
        if ( pic_icon != NULL )
        {
            if ( icon_id > 0 )
            {
                y = pic_icon->y + yoffset;
                sty_draw_picture_id( private->hdc, icon_id, pic_icon->x, y );    
            }  
            else if ( item->bitmap != NULL )
            {
                menuview_paint_bitmap( menuview, cur_index );
            }  
            else
            {
            }  
        }        
        
        //draw name    
        if ( str_name != NULL )
        {    
            temp = str_name->y;    //temp save
            str_name->y += yoffset;    //
            
            if ( item->text_id > 0 )
            {
                str_name->str_id = (unsigned short)item->text_id;
                sty_draw_string_res_ext( private->hdc, str_name, NULL );
            }
            else
            {
                sty_draw_string_res_ext( private->hdc, str_name, item->text );
            }
            
            str_name->y = temp;    //restore    
        }
            
        //draw str1
        if ( ( str_info1 != NULL ) && ( item->info1 != NULL ) )
        {
            temp = str_info1->y;    //temp save
            str_info1->y += yoffset;    //
            sty_draw_string_res_ext( private->hdc, str_info1, item->info1 );        
            str_info1->y = temp;    //restore
        }
        
        //draw str2
        if ( ( str_info2 != NULL ) && ( item->info2 != NULL ) )
        {
            temp = str_info2->y;    //temp save
            str_info2->y += yoffset;    //
            sty_draw_string_res_ext( private->hdc, str_info2, item->info2 );        
            str_info2->y = temp;    //restore
        }
        
        //draw to son icon
        if ( ( pic_son != NULL ) && ( true == item->has_child ) )
        {
            y = pic_son->y + yoffset;
            sty_draw_bitmap( private->hdc, bmp_son, pic_son->x, y );   
        }        
        
        //process scroll text
        if ( cur_index == menu->current )
        {
            //try to kill scroll timer
            kill_timer( private->scroll_timer );
            private->scroll_timer = -1;
            
            if ( str_name == NULL )
            {
                return false;
            }      
        
            temp = str_name->y;    //temp save
            str_name->y += yoffset;    //
            
            //text scroll process
            sty_delete_scroll_string( private->scroll_info );
            private->scroll_info = sty_create_scroll_string( private->hdc, str_name, item->text );
            if ( private->scroll_info != NULL )
            {
                //start scroll timer
                private->scroll_timer = set_timer( SCROLL_INTERVAL, (timer_proc)scroll_timer_event, menuview );
                private->scroll_bg_x = str_name->x;
                private->scroll_bg_y = str_name->y;
            }
            
            str_name->y = temp;    //restore    
        }

        return true;
    }
    else
    {
        return false;
    }
}

bool menuview_paint_bitmap( menuview_t *menuview, int cur_index )
{
    menuview_private_t *private = menuview->private;
    menu_t *menu = menuview->menu;
    menuitem_t *item;    
    int yoffset = 0;
    int x, y;
    int w, h;
    gui_bitmap_info_t *bmp;

    if ( ( cur_index >= menu->offset ) && ( cur_index < (menu->offset + private->lines) ) )
    {
        //compute yoffset
        if ( cur_index <= menu->current )
        {
            yoffset = private->unsel_height * ( cur_index - menu->offset );
        }
        else
        {
            yoffset = private->sel_height + (private->unsel_height * ( (cur_index - menu->offset) - 1 ));
        }

        //get item 
        item = menu_get_item( menu, cur_index );      
        if ( item == NULL )
        {
            return false;
        }
        else
        {
            bmp = item->bitmap; 
        }
               
        if ( bmp == NULL )
        {
            return false;
        }
                
        //draw selected line
        if ( cur_index == menu->current )
        {                
            //draw icon
            if ( private->pic_sel_icon != NULL )
            {
                x = private->pic_sel_icon->x;
                y = private->pic_sel_icon->y + yoffset;
                w = private->pic_sel_icon->width;
                h = private->pic_sel_icon->height;
                
                //fill white color
                gui_dc_set_background_color( private->hdc, GUI_COLOR_WHITE );
                gui_dc_clear_rect( private->hdc, x, y, (x + w) - 1, (y + h) -1 );
                
                x = x + (( w - bmp->xsize )/2);
                y = y + (( h - bmp->ysize )/2);

                //draw bitmap
                gui_dc_draw_bitmap_ext( private->hdc, (unsigned char*)bmp->pdata, 
                                        x, y, (int)bmp->xsize, (int)bmp->ysize, (int)bmp->bytesperpixel );
            }
        }
        else    //draw unselected line
        {
            //unselected line is second line
            yoffset -=  private->sel_height;
            
            //draw icon
            if ( private->pic_unsel_icon != NULL )
            {

                x = private->pic_unsel_icon->x;
                y = private->pic_unsel_icon->y + yoffset;
                w = private->pic_unsel_icon->width;
                h = private->pic_unsel_icon->height;
                
                //fill white color
                gui_dc_set_background_color( private->hdc, GUI_COLOR_WHITE );
                gui_dc_clear_rect( private->hdc, x, y, (x + w) - 1, (y + h) -1 );
                
                x = x + (( w - bmp->xsize )/2);
                y = y + (( h - bmp->ysize )/2);

                //draw bitmap
                gui_dc_draw_bitmap_ext( private->hdc, (unsigned char*)bmp->pdata, 
                                        x, y, (int)bmp->xsize, (int)bmp->ysize, (int)bmp->bytesperpixel );
            }
        }

        return true;
    }
    else
    {
        return false;
    }
}

/* menuview draw slider */
bool menuview_draw_slider( menuview_t *menuview )
{
    menuview_private_t *private = menuview->private;
    menu_t *menu = menuview->menu;

    //set parameter
    if ( menu->global_size > menu->total )
    {   
        private->slider_param.total_item = menu->global_size;             
    }
    else
    {
        private->slider_param.total_item = menu->total;
    }
    private->slider_param.page_index = menu->global_offset + menu->offset + 1;
    private->slider_param.item_perpage = private->lines;
    slider_set_parameter( private->slider, &private->slider_param );

    //paint slider
    slider_paint( private->slider );

    return true;
}

