/********************************************************************************
 *                              USDK(1100)
 *                             Module: app 
 *                 Copyright(c) 2003-2009 Actions Semiconductor,
 *                            All Rights Reserved. 
 * File : music_playing_widget_usereq.c
 * History:         
 *      <author>    <time>           <version >             <desc>
 *    songzhining    2009-2-25          v1.0               build this file 
 ********************************************************************************/
/*!
* \file     music_playing_widget_usereq.c
* \brief    music_playing_widget_usereq widget
* \author   songzhining
*               
* \version 1.0
* \date  2009/02/25
*******************************************************************************/

#include "music_playing_widget_usereq.h"

static bool _erase_picture( usereq_widget_t *usereq_widget, picture_resource_t *pic );
static bool _usereq_widget_draw_bg( usereq_widget_t *usereq_widget );
static bool _usereq_widget_draw_eq( usereq_widget_t *usereq_widget, int cur_index, bool erase );


static bool _erase_picture( usereq_widget_t *usereq_widget, picture_resource_t *pic )
{
    sty_rect_t rect;
    
    //draw bg
    if ( ( usereq_widget != NULL ) && ( pic != NULL ) )
    {
        rect.x = pic->x + usereq_widget->group->x;
        rect.y = pic->y + usereq_widget->group->y;
        rect.width = pic->width;
        rect.height = pic->height;        
        sty_draw_sub_bitmap( usereq_widget->hdc, usereq_widget->bmp_app_bg, pic->x, pic->y, &rect );
        
        rect.x = pic->x;
        rect.y = pic->y;
        rect.width = pic->width;
        rect.height = pic->height;   
        sty_draw_sub_bitmap( usereq_widget->hdc, usereq_widget->bmp_bg, pic->x, pic->y, &rect );
    }

    return true;
}

/* menuview draw background picture*/
static bool _usereq_widget_draw_bg( usereq_widget_t *usereq_widget )
{
    sty_rect_t rect;   
   
    //erase
    canvas_erase_all( usereq_widget->canvas_id );

    //draw bg    
    if ( usereq_widget->bmp_bg != NULL )
    {
        sty_draw_bitmap( usereq_widget->hdc, usereq_widget->bmp_bg, 0, 0 );
    }
    else
    {
        if ( usereq_widget->bmp_app_bg != NULL )
        {
            rect.x = usereq_widget->group->x;
            rect.y = usereq_widget->group->y;
            rect.width = usereq_widget->group->width;
            rect.height = usereq_widget->group->height;
            
            sty_draw_sub_bitmap( usereq_widget->hdc, usereq_widget->bmp_app_bg, 0, 0, &rect );
        }
    }    
    
    //draw title
    if ( usereq_widget->str_title != NULL )
    {
        sty_draw_string_res( usereq_widget->hdc, usereq_widget->str_title );
    }

    //draw max
    if ( usereq_widget->pic_max != NULL )
    {
        sty_draw_picture_res( usereq_widget->hdc, usereq_widget->pic_max );
    }

    //draw mid
    if ( usereq_widget->pic_mid != NULL )
    {
        sty_draw_picture_res( usereq_widget->hdc, usereq_widget->pic_mid );
    }

    //draw min
    if ( usereq_widget->pic_min != NULL )
    {
        sty_draw_picture_res( usereq_widget->hdc, usereq_widget->pic_min );
    }

    return true;
}

/* menuview draw background picture*/
static bool _usereq_widget_draw_eq( usereq_widget_t *usereq_widget, int cur_index, bool erase )
{
    usereq_res_t *usereq_res = &usereq_widget->eq_resgroup[cur_index];
    picture_resource_t *btn;
    picture_resource_t *slider;  
    int current;
    int total;
    int x, y;
    
    //current eq
    if ( cur_index == usereq_widget->eq_offset )
    {
        btn = usereq_res->pic_btn_sel;
        slider = usereq_res->pic_slider_sel;
    }
    else //other eq
    {
        btn = usereq_res->pic_btn_unsel;
        slider = usereq_res->pic_slider_unsel;
    }

    //erase eq area
    if ( erase == true )
    {
        _erase_picture( usereq_widget, btn );
        _erase_picture( usereq_widget, usereq_res->pic_bar_bg );
    }

    //draw bar bg
    if ( usereq_res->pic_bar_bg != NULL )
    {
        sty_draw_picture_res( usereq_widget->hdc, usereq_res->pic_bar_bg );
    }    

    //draw btn
    if ( btn != NULL )
    {
        sty_draw_picture_res( usereq_widget->hdc, btn );
    }
    
    
    //draw slider
    if ( ( slider != NULL ) && ( usereq_res->pic_bar_bg != NULL ) )
    {
        //compute slider position        
        if ( usereq_res->pic_bar_bg->width > usereq_res->pic_bar_bg->height )
        {               
            total = usereq_res->pic_bar_bg->width - slider->width;
            current = (( usereq_widget->user_eq[cur_index] - USEREQ_VAL_MIN ) * total)
                        / ( USEREQ_VAL_MAX - USEREQ_VAL_MIN ); 
            x = slider->x + current;
            y = slider->y;  
        }
        else
        {
            total = usereq_res->pic_bar_bg->height - slider->height;
            current = (( usereq_widget->user_eq[cur_index] - USEREQ_VAL_MIN ) * total)
                        / ( USEREQ_VAL_MAX - USEREQ_VAL_MIN ); 
            x = slider->x;
            y = ( slider->y + total ) - current;  
        } 
        
        sty_draw_picture_id( usereq_widget->hdc, (int)slider->pic_id, x, y );
    }

    return true;
}

/* usereq_widget external interface */
bool usereq_widget_paint( usereq_widget_t *usereq_widget )
{
    int cur_index;
    
    //check param
    if ( usereq_widget == NULL )
    {
        return false;    
    }
    else
    {
    }

    //draw menu bg
    _usereq_widget_draw_bg( usereq_widget );

    //draw all lines
    for ( cur_index = 0; cur_index < USEREQ_MAX; cur_index ++ )
    {
        _usereq_widget_draw_eq( usereq_widget, cur_index, false );
    }    
    

    return true;
}


static bool usereq_widget_proc_key( usereq_widget_t *usereq_widget, key_event_t *pmsg )
{
    int *cur_eq;
    int cur_index;

    //check param
    if ( usereq_widget == NULL )
    {
        return false;
    }
    else
    {
        cur_eq = &usereq_widget->user_eq[usereq_widget->eq_offset];    
    }

    switch ( pmsg->val | pmsg->type ) 
    {
    case KEY_PREV | KEY_TYPE_DOWN:
    case KEY_PREV | KEY_TYPE_LONG:
    case KEY_PREV | KEY_TYPE_HOLD:
        {
            if ( (*cur_eq) > USEREQ_VAL_MIN )
            {
                (*cur_eq)--;    //current dec 
                
                //repaint
                _usereq_widget_draw_eq( usereq_widget, usereq_widget->eq_offset, true );
                
                //external process
                usereq_widget->callback( usereq_widget, WIDGET_SIG_USEREQ_CHANGED );  
            }
            else    //current is first item
            {
            }
        }
        break;
        
    case KEY_NEXT | KEY_TYPE_DOWN:
    case KEY_NEXT | KEY_TYPE_LONG:
    case KEY_NEXT | KEY_TYPE_HOLD:
        {
            if ( (*cur_eq) < USEREQ_VAL_MAX )
            {
                (*cur_eq)++;    //current dec 
                
                //repaint
                _usereq_widget_draw_eq( usereq_widget, usereq_widget->eq_offset, true );
                
                //external process
                usereq_widget->callback( usereq_widget, WIDGET_SIG_USEREQ_CHANGED );   
            }
            else    //current is first item
            {
            }
        }
        break;
        
    case KEY_MENU | KEY_TYPE_DOWN:
    case KEY_MENU | KEY_TYPE_LONG:
    case KEY_MENU | KEY_TYPE_HOLD:
        {
            cur_index = usereq_widget->eq_offset;
            
            usereq_widget->eq_offset ++;
            usereq_widget->eq_offset = ( usereq_widget->eq_offset % USEREQ_MAX );

            //repaint
            _usereq_widget_draw_eq( usereq_widget, cur_index, true );
            _usereq_widget_draw_eq( usereq_widget, usereq_widget->eq_offset, true );
        }
        break;
        
    case KEY_OK | KEY_TYPE_SHORT_UP:
    case KEY_OK | KEY_TYPE_LONG_UP:
    case KEY_OK | KEY_TYPE_HOLD_UP:
        {
            //external process
            usereq_widget->callback( usereq_widget, WIDGET_SIG_USEREQ_OK );    
        }
        break;
        
    case KEY_RETURN | KEY_TYPE_SHORT_UP:
    case KEY_RETURN | KEY_TYPE_LONG_UP:
    case KEY_RETURN | KEY_TYPE_HOLD_UP:
        {    
            //external process
            usereq_widget->callback( usereq_widget, WIDGET_SIG_USEREQ_QUIT );    
        }
        break;
        
    default:
        break;
    }   

    //clear key
    clear_key_msg_queue(-1,-1);

    return true;
}



/* usereq_widget entry */
bool usereq_widget_proc_gui( usereq_widget_t *usereq_widget, gui_wm_message_t *pmsg )
{
    bool ret = false;
    
    switch ( pmsg->msgid ) 
    {
//    case WM_MSG_PAINT:
//        ret = usereq_widget_paint( usereq_widget );
//        break;
         
    case WM_MSG_KEY:
        ret = usereq_widget_proc_key( usereq_widget, (key_event_t *)(pmsg->data.p) );
        break;
        
    case WM_MSG_TOUCH:
        break;
        
    default:
        break;
    }
    
    return ret;
}


usereq_widget_t* usereq_widget_create( int *user_eq, gui_bitmap_info_t *bmp_bg, usereq_widget_cb_t callback )
{
    usereq_widget_t *usereq_widget;
    int cur_index;
    bool ret;
       
    if ( callback == NULL )
    {
        print_err("callback is null!");
        return NULL;    
    }
    
    //malloc data
    usereq_widget = (usereq_widget_t *)malloc( sizeof(usereq_widget_t) );        
    if ( usereq_widget == NULL )
    {
        print_err("malloc failed!");
        return NULL;    
    }
    else
    {
        //initial data
        usereq_widget->eq_offset = 0;
        
        for ( cur_index = 0; cur_index < USEREQ_MAX; cur_index ++ )
        {
            usereq_widget->user_eq[cur_index] = user_eq[cur_index];
        }
        
        usereq_widget->bmp_app_bg = bmp_bg;
        usereq_widget->bmp_bg = NULL;
        usereq_widget->callback = callback;
        usereq_widget->canvas_id = -1;
    }
    
    //load style resource
    ret = usereq_widget_load_style( usereq_widget );
    if ( ret == false )
    {
        print_err("canvas_create failed!");
        free( usereq_widget );
        return NULL;
    }    

    //获取dc
    usereq_widget->hdc = gui_dc_get( usereq_widget->canvas_id );

    return usereq_widget;
}

bool usereq_widget_delete( usereq_widget_t *usereq_widget )
{
    if ( usereq_widget != NULL )
    {
        //free bitmap
        sty_delete_bitmap( usereq_widget->bmp_bg );
        usereq_widget->bmp_bg = NULL;
    
        //delete canvas
        if ( usereq_widget->canvas_id != -1 )
        {
            canvas_delete( usereq_widget->canvas_id );
            usereq_widget->canvas_id = 0;
        }

        //free usereq_widget private
        free( usereq_widget );
    }
    
    return true;
}

/* usereq_widget load style resource */
bool usereq_widget_load_style( usereq_widget_t *usereq_widget )
{
    scene_t* scene;    //!< app scene
    resgroup_resource_t *group;
    usereq_res_t *usereq_res;
    sty_rect_t rect;
    int cur_index;
    
    unsigned int resgroup_id[USEREQ_MAX] =
    {
        USER_EQ1_RESGROUP,
        USER_EQ2_RESGROUP,
        USER_EQ3_RESGROUP,
        USER_EQ4_RESGROUP,
        USER_EQ5_RESGROUP,
    };
    
    if ( usereq_widget == NULL )
    {
        print_warning("usereq_widget is null!");
        return false;
    }
    else
    {
        usereq_widget->type = USEREQ_TYPE_NULL;
    }
    
    //get root resource
    scene = sty_get_app_scene();
    if ( scene == NULL )
    {
        print_err("cann't find common scene!");
        return false;    
    }

    //load control group
    group = ( resgroup_resource_t * )get_scene_child( scene, USER_EQ_RESGROUP );   
    if ( group == NULL )
    {
        print_err("cann't find resgroup!");
        return false;    
    }
    usereq_widget->group = group; 
    
    //load resgroup
    for ( cur_index = 0; cur_index < USEREQ_MAX; cur_index ++ )
    {
        usereq_widget->resgroup[cur_index] = ( resgroup_resource_t * )get_resgroup_child( group, 
                                                                            resgroup_id[cur_index] );
    }
    
    //load resource
    usereq_widget->pic_bg = ( picture_resource_t * )get_resgroup_child( group, PIC_BG );    
    usereq_widget->str_title = ( string_resource_t * )get_resgroup_child( group, STR_TITLE ); 
    usereq_widget->pic_max = ( picture_resource_t * )get_resgroup_child( group, PIC_VAL_MAX );
    usereq_widget->pic_mid = ( picture_resource_t * )get_resgroup_child( group, PIC_VAL_MID );
    usereq_widget->pic_min = ( picture_resource_t * )get_resgroup_child( group, PIC_VAL_MIN );

    //free bitmap
    sty_delete_bitmap( usereq_widget->bmp_bg );
    usereq_widget->bmp_bg = NULL;

    if ( ( usereq_widget->pic_bg != NULL ) && ( usereq_widget->bmp_bg == NULL ) )
    {
        usereq_widget->bmp_bg = sty_create_bitmap( (int)usereq_widget->pic_bg->pic_id );
    }

    for ( cur_index = 0; cur_index < USEREQ_MAX; cur_index ++ )
    {
        group = usereq_widget->resgroup[cur_index];
        usereq_res = &usereq_widget->eq_resgroup[cur_index];
        
        usereq_res->pic_bar_bg = ( picture_resource_t * )get_resgroup_child( group, PIC_BAR_BG );
        usereq_res->pic_btn_unsel = ( picture_resource_t * )get_resgroup_child( group, PIC_BTN_UNSEL );
        usereq_res->pic_btn_sel = ( picture_resource_t * )get_resgroup_child( group, PIC_BTN_SEL );
        usereq_res->pic_slider_unsel = ( picture_resource_t * )get_resgroup_child( group, PIC_SLIDER_UNSEL );
        usereq_res->pic_slider_sel = ( picture_resource_t * )get_resgroup_child( group, PIC_SLIDER_SEL );
    }

    //canvas
    group = usereq_widget->group;
    rect.x = group->absolute_x;
    rect.y = group->absolute_y;
    rect.width = group->width;
    rect.height = group->height;

    if ( usereq_widget->canvas_id == -1 )
    {
        //create canvas
        usereq_widget->canvas_id = canvas_create( &rect, TRANSPARENT );
        if ( usereq_widget->canvas_id == -1 )
        {
            print_err("canvas_create failed!");
            return false;
        }   
    }
    else
    {
        //set canvas region
        canvas_set_region( usereq_widget->canvas_id, &rect );
    }
    
    return true;
}

/******************************************************************************/
/*!                    
* \par  Description:
*     usereq_widget_ 控件设置菜单数据
* \ingroup music
* \param [in] usereq_widget_: usereq_widget_控件句柄 
* \param [in] usereq: user eq数据
* \return  成功返回true, 失败返回false      
* \note usereq_widget_ 只负责显示和输入处理, 菜单数据必须由外部设置和保存
*******************************************************************************/
bool usereq_widget_set_value( usereq_widget_t *usereq_widget, int *user_eq )
{
    int cur_index;
    
    if ( usereq_widget == NULL )
    {
        return false;    
    }
    else
    {
        for ( cur_index = 0; cur_index < USEREQ_MAX; cur_index ++ )
        {
            usereq_widget->user_eq[cur_index] = user_eq[cur_index];
        }
        
        return true;    
    } 
}

/******************************************************************************/
/*!                    
* \par  Description:
*     usereq_widget_ 控件获取菜单数据
* \ingroup music
* \param [in] usereq_widget_: usereq_widget_控件句柄     
* \param [out] usereq: user eq数据
* \return  成功返回true, 失败返回false      
*******************************************************************************/
bool usereq_widget_get_value( usereq_widget_t *usereq_widget, int *user_eq )
{
    int cur_index;
    
    if ( usereq_widget == NULL )
    {
        return false;    
    }
    else
    {
        for ( cur_index = 0; cur_index < USEREQ_MAX; cur_index ++ )
        {
            user_eq[cur_index] = usereq_widget->user_eq[cur_index];
        }
        
        return true;    
    } 
}

