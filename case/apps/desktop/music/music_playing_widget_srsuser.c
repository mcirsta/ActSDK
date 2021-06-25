/********************************************************************************
 *                              USDK(1100)
 *                             Module: app 
 *                 Copyright(c) 2003-2009 Actions Semiconductor,
 *                            All Rights Reserved. 
 * File : music_playing_widget_srsuser.c
 * History:         
 *      <author>    <time>           <version >             <desc>
 *    songzhining    2009-2-25          v1.0               build this file 
 ********************************************************************************/
/*!
* \file     music_playing_widget_srsuser.c
* \brief    music_playing_widget_srsuser widget
* \author   songzhining
*               
* \version 1.0
* \date  2009/02/25
*******************************************************************************/

#include "music_playing_widget_srsuser.h"

static bool _erase_picture( srsuser_widget_t *srsuser_widget, picture_resource_t *pic );
static bool _erase_string( srsuser_widget_t *srsuser_widget, string_resource_t *str );
static bool _srsuser_widget_draw_bg( srsuser_widget_t *srsuser_widget );
static bool _srsuser_widget_draw_eq( srsuser_widget_t *srsuser_widget, int cur_index, bool erase );


/* max value */
static int s_srs_maxval[SRSUSER_MAX] = 
{
    10, 10, 10, 10, 10, 2, 1,
};
    
/* min value */
static int s_srs_minval[SRSUSER_MAX] = 
{
    0, 0, 0, 1, 0, 0, 0,
};

static bool _erase_picture( srsuser_widget_t *srsuser_widget, picture_resource_t *pic )
{
    sty_rect_t rect;
    
    //draw bg
    if ( ( srsuser_widget != NULL ) && ( pic != NULL ) )
    {
        rect.x = pic->x + srsuser_widget->group->x;
        rect.y = pic->y + srsuser_widget->group->y;
        rect.width = pic->width;
        rect.height = pic->height;        
        sty_draw_sub_bitmap( srsuser_widget->hdc, srsuser_widget->bmp_app_bg, pic->x, pic->y, &rect );
        
        rect.x = pic->x;
        rect.y = pic->y;
        rect.width = pic->width;
        rect.height = pic->height;        
        sty_draw_sub_bitmap( srsuser_widget->hdc, srsuser_widget->bmp_bg, pic->x, pic->y, &rect );
    }

    return true;
}

static bool _erase_string( srsuser_widget_t *srsuser_widget, string_resource_t *str )
{
    sty_rect_t rect;
    
    //draw bg
    if ( ( srsuser_widget != NULL ) && ( str != NULL ) )
    {
        rect.x = str->x + srsuser_widget->group->x;
        rect.y = str->y + srsuser_widget->group->y;
        rect.width = str->width;
        rect.height = str->height;        
        sty_draw_sub_bitmap( srsuser_widget->hdc, srsuser_widget->bmp_app_bg, str->x, str->y, &rect );
        
        rect.x = str->x;
        rect.y = str->y;
        rect.width = str->width;
        rect.height = str->height;        
        sty_draw_sub_bitmap( srsuser_widget->hdc, srsuser_widget->bmp_bg, str->x, str->y, &rect );
    }

    return true;
}

/* menuview draw background picture*/
static bool _srsuser_widget_draw_bg( srsuser_widget_t *srsuser_widget )
{
    sty_rect_t rect;    

    //erase
    canvas_erase_all( srsuser_widget->canvas_id );

    //draw bg
    if ( srsuser_widget->bmp_bg != NULL )
    {
        sty_draw_bitmap( srsuser_widget->hdc, srsuser_widget->bmp_bg, 0, 0 );
    } 
    else
    {
        if ( srsuser_widget->bmp_app_bg != NULL )
        {
            rect.x = srsuser_widget->group->x;
            rect.y = srsuser_widget->group->y;
            rect.width = srsuser_widget->group->width;
            rect.height = srsuser_widget->group->height;
            
            sty_draw_sub_bitmap( srsuser_widget->hdc, srsuser_widget->bmp_app_bg, 0, 0, &rect );
        }   
    }
    
    //draw title
    if ( srsuser_widget->str_title != NULL )
    {
        sty_draw_string_res( srsuser_widget->hdc, srsuser_widget->str_title );
    }

    //draw val 0
    if ( srsuser_widget->pic_val_0 != NULL )
    {
        sty_draw_picture_res( srsuser_widget->hdc, srsuser_widget->pic_val_0 );
    }

    //draw val 10
    if ( srsuser_widget->pic_val_10 != NULL )
    {
        sty_draw_picture_res( srsuser_widget->hdc, srsuser_widget->pic_val_10 );
    }
    
    //draw val 60
    if ( srsuser_widget->pic_val_60 != NULL )
    {
        sty_draw_picture_res( srsuser_widget->hdc, srsuser_widget->pic_val_60 );
    }
    
    //draw val 100
    if ( srsuser_widget->pic_val_100 != NULL )
    {
        sty_draw_picture_res( srsuser_widget->hdc, srsuser_widget->pic_val_100 );
    }
    
    //draw val 150
    if ( srsuser_widget->pic_val_150 != NULL )
    {
        sty_draw_picture_res( srsuser_widget->hdc, srsuser_widget->pic_val_150 );
    }

    return true;
}

/* menuview draw background picture*/
static bool _srsuser_widget_draw_eq( srsuser_widget_t *srsuser_widget, int cur_index, bool erase )
{
    srsuser_res_t *srsuser_res = &srsuser_widget->eq_resgroup[cur_index];
    picture_resource_t *btn;
    picture_resource_t *slider;
    string_resource_t *name;
    picture_resource_t *val_bg;
    string_resource_t *value;
    int current;
    int total;
    int x, y;
    char buf[32];
    
    //current eq
    if ( cur_index == srsuser_widget->eq_offset )
    {
        btn = srsuser_res->pic_btn_sel;
        slider = srsuser_res->pic_slider_sel;
        name = srsuser_res->str_btn_sel;
        val_bg = srsuser_res->pic_val_sel;
        value = srsuser_res->str_val_sel;
    }
    else //other eq
    {
        btn = srsuser_res->pic_btn_unsel;
        slider = srsuser_res->pic_slider_unsel;
        name = srsuser_res->str_btn_unsel;
        val_bg = srsuser_res->pic_val_unsel;
        value = srsuser_res->str_val_unsel;
    }

    //erase eq area
    if ( erase == true )
    {
        _erase_picture( srsuser_widget, btn );
        _erase_picture( srsuser_widget, srsuser_res->pic_bar_bg );
        _erase_string( srsuser_widget, value );
    }    

    //draw bar bg
    if ( srsuser_res->pic_bar_bg != NULL )
    {
        sty_draw_picture_res( srsuser_widget->hdc, srsuser_res->pic_bar_bg );
    }
    
    //draw btn
    if ( btn != NULL )
    {
        sty_draw_picture_res( srsuser_widget->hdc, btn );
    }    
    
    //draw slider
    if ( ( slider != NULL ) && ( srsuser_res->pic_bar_bg != NULL ) )
    {
        //last item
        if ( cur_index == ( SRSUSER_MAX - 1 ) )
        {
            if ( srsuser_widget->srs_usereq[cur_index] == 0 )
            {                
                sty_draw_picture_res( srsuser_widget->hdc, srsuser_res->pic_slider_unsel );
            }
            else
            {
                sty_draw_picture_res( srsuser_widget->hdc, srsuser_res->pic_slider_sel );
            }  
        }
        else
        {
            //compute slider position            
            if ( srsuser_res->pic_bar_bg->width > srsuser_res->pic_bar_bg->height )
            {         
                total = srsuser_res->pic_bar_bg->width - slider->width;
                current = (( srsuser_widget->srs_usereq[cur_index] - s_srs_minval[cur_index] ) * total)
                            / ( s_srs_maxval[cur_index] - s_srs_minval[cur_index] );       
                x = slider->x + current;
                y = slider->y;  
            }
            else
            {
                total = srsuser_res->pic_bar_bg->height - slider->height;
                current = (( srsuser_widget->srs_usereq[cur_index] - s_srs_minval[cur_index] ) * total)
                            / ( s_srs_maxval[cur_index] - s_srs_minval[cur_index] ); 
                x = slider->x;
                y = ( slider->y + total ) - current;  
            } 
            
            sty_draw_picture_id( srsuser_widget->hdc, (int)slider->pic_id, x, y );
        }        
    }

    //draw name
    if ( name != NULL )
    {
        sty_draw_string_res( srsuser_widget->hdc, name );
    }    
    
    //draw value bg
    if ( val_bg != NULL )
    {
        sty_draw_picture_res( srsuser_widget->hdc, val_bg );
    }    
    
    //draw value
    if ( value != NULL )
    {
        //last item
        if ( cur_index == ( SRSUSER_MAX - 1 ) )
        {
            sty_draw_string_res( srsuser_widget->hdc, srsuser_res->str_val_unsel );
            sty_draw_string_res( srsuser_widget->hdc, srsuser_res->str_val_sel );
        }
        else
        {
            snprintf( buf, sizeof(buf), "%d", srsuser_widget->srs_usereq[cur_index] );
            sty_draw_string_res_ext( srsuser_widget->hdc, value, buf );
        }
    }    
    
    return true;
}

/* srsuser_widget external interface */
bool srsuser_widget_paint( srsuser_widget_t *srsuser_widget )
{
    int cur_index;
    
    //check param
    if ( srsuser_widget == NULL )
    {
        return false;    
    }
    else
    {
    }

    //draw menu bg
    _srsuser_widget_draw_bg( srsuser_widget );

    //draw all lines
    for ( cur_index = 0; cur_index < SRSUSER_MAX; cur_index ++ )
    {
        _srsuser_widget_draw_eq( srsuser_widget, cur_index, false );
    }    

    return true;
}


static bool srsuser_widget_proc_key( srsuser_widget_t *srsuser_widget, key_event_t *pmsg )
{
    int *cur_eq;
    int cur_index;

    //check param
    if ( srsuser_widget == NULL )
    {
        return false;
    }
    else
    {
        cur_eq = &srsuser_widget->srs_usereq[srsuser_widget->eq_offset];    
    }

    switch ( pmsg->val | pmsg->type ) 
    {
    case KEY_PREV | KEY_TYPE_DOWN:
    case KEY_PREV | KEY_TYPE_LONG:
    case KEY_PREV | KEY_TYPE_HOLD:
        {
            if ( (*cur_eq) > s_srs_minval[srsuser_widget->eq_offset] )
            {
                (*cur_eq)--;    //current dec 
                
                //repaint
                _srsuser_widget_draw_eq( srsuser_widget, srsuser_widget->eq_offset, true );
                
                //external process
                srsuser_widget->callback( srsuser_widget, WIDGET_SIG_SRSUSER_CHANGED );  
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
            if ( (*cur_eq) < s_srs_maxval[srsuser_widget->eq_offset] )
            {
                (*cur_eq)++;    //current dec 
                
                //repaint
                _srsuser_widget_draw_eq( srsuser_widget, srsuser_widget->eq_offset, true );
                
                //external process
                srsuser_widget->callback( srsuser_widget, WIDGET_SIG_SRSUSER_CHANGED ); 
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
            cur_index = srsuser_widget->eq_offset;
            
            srsuser_widget->eq_offset ++;
            srsuser_widget->eq_offset = ( srsuser_widget->eq_offset % SRSUSER_MAX );

            //repaint
            _srsuser_widget_draw_eq( srsuser_widget, cur_index, true );
            _srsuser_widget_draw_eq( srsuser_widget, srsuser_widget->eq_offset, true );
        }
        break;
        
    case KEY_OK | KEY_TYPE_SHORT_UP:
    case KEY_OK | KEY_TYPE_LONG_UP:
    case KEY_OK | KEY_TYPE_HOLD_UP:
        {
            //external process
            srsuser_widget->callback( srsuser_widget, WIDGET_SIG_SRSUSER_OK );    
        }
        break;
        
    case KEY_RETURN | KEY_TYPE_SHORT_UP:
    case KEY_RETURN | KEY_TYPE_LONG_UP:
    case KEY_RETURN | KEY_TYPE_HOLD_UP:
        {    
            //external process
            srsuser_widget->callback( srsuser_widget, WIDGET_SIG_SRSUSER_QUIT );    
        }
        break;
        
    default:
        break;
    }   

    //clear key
    clear_key_msg_queue(-1,-1);

    return true;
}



/* srsuser_widget entry */
bool srsuser_widget_proc_gui( srsuser_widget_t *srsuser_widget, gui_wm_message_t *pmsg )
{
    bool ret = false;
    
    switch ( pmsg->msgid ) 
    {
//    case WM_MSG_PAINT:
//        ret = srsuser_widget_paint( srsuser_widget );
//        break;
         
    case WM_MSG_KEY:
        ret = srsuser_widget_proc_key( srsuser_widget, (key_event_t *)(pmsg->data.p) );
        break;
        
    case WM_MSG_TOUCH:
        break;
        
    default:
        break;
    }
    
    return ret;
}


srsuser_widget_t* srsuser_widget_create( int *srs_usereq, gui_bitmap_info_t *bmp_bg, srsuser_widget_cb_t callback )
{
    srsuser_widget_t *srsuser_widget;
    int cur_index;
    bool ret;
       
    if ( callback == NULL )
    {
        print_err("callback is null!");
        return NULL;    
    }
    
    //malloc data
    srsuser_widget = (srsuser_widget_t *)malloc( sizeof(srsuser_widget_t) );        
    if ( srsuser_widget == NULL )
    {
        print_err("malloc failed!");
        return NULL;    
    }
    else
    {
        //initial data
        srsuser_widget->eq_offset = 0;
        
        for ( cur_index = 0; cur_index < SRSUSER_MAX; cur_index ++ )
        {
            srsuser_widget->srs_usereq[cur_index] = srs_usereq[cur_index];
        }
        
        srsuser_widget->bmp_app_bg = bmp_bg;
        srsuser_widget->bmp_bg = NULL;
        srsuser_widget->callback = callback;
        srsuser_widget->canvas_id = -1;
    }
    
    //load style resource
    ret = srsuser_widget_load_style( srsuser_widget );
    if ( ret == false )
    {
        print_err("canvas_create failed!");
        free( srsuser_widget );
        return NULL;
    }    

    //获取dc
    srsuser_widget->hdc = gui_dc_get( srsuser_widget->canvas_id );

    return srsuser_widget;
}

bool srsuser_widget_delete( srsuser_widget_t *srsuser_widget )
{
    if ( srsuser_widget != NULL )
    {
        //free bitmap
        sty_delete_bitmap( srsuser_widget->bmp_bg );
        srsuser_widget->bmp_bg = NULL;
    
        //delete canvas
        if ( srsuser_widget->canvas_id != -1 )
        {
            canvas_delete( srsuser_widget->canvas_id );
            srsuser_widget->canvas_id = 0;
        }

        //free srsuser_widget private
        free( srsuser_widget );
    }
    
    return true;
}

/* srsuser_widget load style resource */
bool srsuser_widget_load_style( srsuser_widget_t *srsuser_widget )
{
    scene_t* scene;    //!< app scene
    resgroup_resource_t *group;
    srsuser_res_t *srsuser_res;
    sty_rect_t rect;
    int cur_index;
    
    unsigned int resgroup_id[SRSUSER_MAX] =
    {
        SRS_EQ1_RESGROUP,
        SRS_EQ2_RESGROUP,
        SRS_EQ3_RESGROUP,
        SRS_EQ4_RESGROUP,
        SRS_EQ5_RESGROUP,
        SRS_EQ6_RESGROUP,
        SRS_EQ7_RESGROUP,
    };
    
    if ( srsuser_widget == NULL )
    {
        print_warning("srsuser_widget is null!");
        return false;
    }
    else
    {
        srsuser_widget->type = SRSUSER_TYPE_NULL;
    }
    
    //get root resource
    scene = sty_get_app_scene();
    if ( scene == NULL )
    {
        print_err("cann't find common scene!");
        return false;    
    }

    //load control group
    group = ( resgroup_resource_t * )get_scene_child( scene, SRS_EQ_RESGROUP );
    if ( group == NULL )
    {
        print_err("cann't find resgroup!");
        return false;    
    }
    srsuser_widget->group = group;    
    
    //load resgroup
    for ( cur_index = 0; cur_index < SRSUSER_MAX; cur_index ++ )
    {
        srsuser_widget->resgroup[cur_index] = ( resgroup_resource_t * )get_resgroup_child( group, 
                                                                            resgroup_id[cur_index] );
    }
    
    //load resource
    srsuser_widget->pic_bg = ( picture_resource_t * )get_resgroup_child( group, PIC_BG );
    srsuser_widget->str_title = ( string_resource_t * )get_resgroup_child( group, STR_TITLE ); 
    srsuser_widget->pic_val_0 = ( picture_resource_t * )get_resgroup_child( group, PIC_VAL_0 );
    srsuser_widget->pic_val_10 = ( picture_resource_t * )get_resgroup_child( group, PIC_VAL_10 );
    srsuser_widget->pic_val_60 = ( picture_resource_t * )get_resgroup_child( group, PIC_VAL_60 );
    srsuser_widget->pic_val_100 = ( picture_resource_t * )get_resgroup_child( group, PIC_VAL_100 );
    srsuser_widget->pic_val_150 = ( picture_resource_t * )get_resgroup_child( group, PIC_VAL_150 );

    //free bitmap
    sty_delete_bitmap( srsuser_widget->bmp_bg );
    srsuser_widget->bmp_bg = NULL;

    if ( ( srsuser_widget->pic_bg != NULL ) && ( srsuser_widget->bmp_bg == NULL ) )
    {
        srsuser_widget->bmp_bg = sty_create_bitmap( (int)srsuser_widget->pic_bg->pic_id );
    }

    for ( cur_index = 0; cur_index < SRSUSER_MAX; cur_index ++ )
    {
        group = srsuser_widget->resgroup[cur_index];
        srsuser_res = &srsuser_widget->eq_resgroup[cur_index];
        
        srsuser_res->pic_bar_bg = ( picture_resource_t * )get_resgroup_child( group, PIC_BAR_BG );
        srsuser_res->pic_btn_unsel = ( picture_resource_t * )get_resgroup_child( group, PIC_BTN_UNSEL );
        srsuser_res->pic_btn_sel = ( picture_resource_t * )get_resgroup_child( group, PIC_BTN_SEL );
        srsuser_res->pic_slider_unsel = ( picture_resource_t * )get_resgroup_child( group, PIC_SLIDER_UNSEL );
        srsuser_res->pic_slider_sel = ( picture_resource_t * )get_resgroup_child( group, PIC_SLIDER_SEL );
        srsuser_res->str_btn_unsel = ( string_resource_t * )get_resgroup_child( group, STR_BTN_UNSEL ); 
        srsuser_res->str_btn_sel = ( string_resource_t * )get_resgroup_child( group, STR_BTN_SEL ); 
        srsuser_res->pic_val_unsel = ( picture_resource_t * )get_resgroup_child( group, PIC_VAL_UNSEL );
        srsuser_res->pic_val_sel = ( picture_resource_t * )get_resgroup_child( group, PIC_VAL_SEL );
        srsuser_res->str_val_unsel = ( string_resource_t * )get_resgroup_child( group, STR_VAL_UNSEL ); 
        srsuser_res->str_val_sel = ( string_resource_t * )get_resgroup_child( group, STR_VAL_SEL ); 
    }

    //canvas
    group = srsuser_widget->group;
    rect.x = group->absolute_x;
    rect.y = group->absolute_y;
    rect.width = group->width;
    rect.height = group->height;
    
    if ( srsuser_widget->canvas_id == -1 )
    {
        //create canvas
        srsuser_widget->canvas_id = canvas_create( &rect, TRANSPARENT );
        if ( srsuser_widget->canvas_id == -1 )
        {
            print_err("canvas_create failed!");
            return false;
        }   
    }
    else
    {
        //set canvas region
        canvas_set_region( srsuser_widget->canvas_id, &rect );
    }
    
    return true;
}

/******************************************************************************/
/*!                    
* \par  Description:
*     srsuser_widget_ 控件设置菜单数据
* \ingroup music
* \param [in] srsuser_widget_: srsuser_widget_控件句柄 
* \param [in] srsuser: user eq数据
* \return  成功返回true, 失败返回false      
* \note srsuser_widget_ 只负责显示和输入处理, 菜单数据必须由外部设置和保存
*******************************************************************************/
bool srsuser_widget_set_value( srsuser_widget_t *srsuser_widget, int *srs_usereq )
{
    int cur_index;
    
    if ( srsuser_widget == NULL )
    {
        return false;    
    }
    else
    {
        for ( cur_index = 0; cur_index < SRSUSER_MAX; cur_index ++ )
        {
            srsuser_widget->srs_usereq[cur_index] = srs_usereq[cur_index];
        }
        
        return true;    
    } 
}

/******************************************************************************/
/*!                    
* \par  Description:
*     srsuser_widget_ 控件获取菜单数据
* \ingroup music
* \param [in] srsuser_widget_: srsuser_widget_控件句柄     
* \param [out] srsuser: user eq数据
* \return  成功返回true, 失败返回false      
*******************************************************************************/
bool srsuser_widget_get_value( srsuser_widget_t *srsuser_widget, int *srs_usereq )
{
    int cur_index;
    
    if ( srsuser_widget == NULL )
    {
        return false;    
    }
    else
    {
        for ( cur_index = 0; cur_index < SRSUSER_MAX; cur_index ++ )
        {
            srs_usereq[cur_index] = srsuser_widget->srs_usereq[cur_index];
        }
        
        return true;    
    } 
}

