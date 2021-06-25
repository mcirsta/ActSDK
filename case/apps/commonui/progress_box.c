/********************************************************************************
 *                              USDK(1100)
 *                             Module: app 
 *                 Copyright(c) 2003-2009 Actions Semiconductor,
 *                            All Rights Reserved. 
 * File : progress_box.c
 * History:         
 *      <author>    <time>           <version >             <desc>
 *    herb hu      2009-07-27          v1.0               build this file 
 ********************************************************************************/
/*!
* \file     progress_box.c
* \brief    progress_box widget
* \author   herb hu 
*               
* \version 1.0
* \date  2009/07/27
*******************************************************************************/
    
#include "progress_box_private.h"

static bool _proc_callback( progress_box_t *progress_box, progress_box_sig_e signal );
static bool _timer_proc( progress_box_t *progress_box );
static bool _proc_key( progress_box_t *progress_box, key_event_t *pmsg );

static bool _proc_callback( progress_box_t *progress_box, progress_box_sig_e signal )
{
    if ( ( progress_box != NULL ) && ( progress_box->callback != NULL ) )
    {
        progress_box->callback( progress_box, signal );    //send signal to callback                  
    }
    
    return true;
}

static bool _timer_proc( progress_box_t *progress_box )
{
    progress_box_draw_wait_animation( progress_box );
    return true;
}

bool progress_box_paint( progress_box_t *progress_box )
{
    if ( progress_box == NULL )
    {
        return false;    
    }
    else
    {
        progress_box_draw_bg( progress_box );
        progress_box_draw_wait_animation( progress_box );
        progress_box_draw_text( progress_box );
        progress_box_draw_progress_bar( progress_box );
    }

    return true;
}


static bool _proc_key( progress_box_t *progress_box, key_event_t *pmsg )
{
    if ( progress_box == NULL )
    {
        return false;
    }

    switch ( pmsg->val | pmsg->type ) 
    {
        case KEY_OK | KEY_TYPE_SHORT_UP:
        case KEY_OK | KEY_TYPE_LONG_UP:
        case KEY_OK | KEY_TYPE_HOLD_UP:
        {
            break;
        }

        case KEY_RETURN | KEY_TYPE_SHORT_UP:
        case KEY_RETURN | KEY_TYPE_LONG_UP:
        case KEY_RETURN | KEY_TYPE_HOLD_UP:
        {    
            break; 
        }

        default:
        {
            break;
        }
    }   

    return true;
}


bool progress_box_proc_gui( progress_box_t *progress_box, gui_wm_message_t *pmsg )
{
    bool ret = false;

    switch ( pmsg->msgid ) 
    {
        case WM_MSG_KEY:
        {
            ret = _proc_key( progress_box, (key_event_t *)(pmsg->data.p) );
            break;
        }

        case WM_MSG_TOUCH:
        {
            break;
        }

        default:
        {
            break;
        }
    }

    return ret;
}

progress_box_t* progress_box_create( progress_box_init_t *init, progress_box_cb_t callback )
{
    progress_box_t *progress_box;
    bool ret;
    resgroup_resource_t *group;
    sty_rect_t rect;

    if ( init == NULL )
    {
        print_err("init is null!");
        return NULL;    
    }  

    if ( callback == NULL )
    {
        print_err("callback is null!");
        return NULL;    
    }
    
    progress_box = (progress_box_t *)malloc( sizeof(progress_box_t) );        
    if ( progress_box == NULL )
    {
        print_err("malloc failed!");
        return NULL;    
    }    
    else
    {
        //initial data
        progress_box->callback = callback;
        progress_box->canvas_id = -1;
        progress_box->private = NULL;
    }
    
    progress_box->private = (progress_box_private_t *)malloc( sizeof(progress_box_private_t) );        
    if ( progress_box->private == NULL )
    {
        print_err("malloc failed!");
        free( progress_box );
        return NULL;    
    }    
    else
    {
        //initial private
        memset( progress_box->private, 0, sizeof(progress_box_private_t) );
        progress_box->private->timer_id = -1;
        progress_box->private->init_data = *init;
    }
        
    ret = progress_box_load_style( progress_box );
    if ( ret == false )
    {
        print_err("canvas_create failed!");
        free( progress_box->private);
        free( progress_box );
        return NULL;
    }

    //create canvas
    group = progress_box->private->resgroup;
    rect.x = group->absolute_x;
    rect.y = group->absolute_y;
    rect.width = group->width;
    rect.height = group->height;
    
    progress_box->canvas_id = canvas_create( &rect, TRANSPARENT ); 
    if ( progress_box->canvas_id == -1 )
    {
        print_err("canvas_create failed!");
        free( progress_box->private );
        free( progress_box );
        return NULL;
    }
    
    progress_box->private->hdc = gui_dc_get(progress_box->canvas_id );

    progress_box->private->timer_id = set_timer( ANIMATION_INTERVAL, (timer_proc)_timer_proc, (void*)progress_box );
    
    return progress_box;
}


bool progress_box_delete( progress_box_t *progress_box )
{
    if ( progress_box != NULL )
    {
        if( (progress_box->private != NULL) && (progress_box->private->bmp_bg != NULL) )
        {
            sty_delete_bitmap(progress_box->private->bmp_bg);
        }

        if((progress_box->private != NULL) && (progress_box->private->bmp_pbar_bg != NULL) )
        {
            sty_delete_bitmap(progress_box->private->bmp_pbar_bg);
        }

        if((progress_box->private != NULL) && (progress_box->private->bmp_pbar_hl != NULL) )
        {
            sty_delete_bitmap(progress_box->private->bmp_pbar_hl);
        }

        if ( progress_box->private != NULL )
        {
            kill_timer( progress_box->private->timer_id );
            progress_box->private->timer_id = -1;
        }        
        
        if ( progress_box->canvas_id != -1 )
        {
            canvas_delete( progress_box->canvas_id );
            progress_box->canvas_id = -1;
        }

        if ( progress_box->private != NULL )
        {
            free( progress_box->private );
            progress_box->private = NULL;
        }            

        free( progress_box );
    }
    
    return true;
}

bool progress_box_load_style( progress_box_t *progress_box )
{
    progress_box_private_t *private;
    scene_t* scene;    
    sty_rect_t rect;
    
    if ( progress_box == NULL )
    {
        print_warning("progress_box is null!");
        return false;
    }
    else
    {
        private = progress_box->private;
    }
    
    scene = sty_get_common_scene();
    if ( scene == NULL )
    {
        print_err("cann't find common scene!");
        return false;    
    }

    private->resgroup = ( resgroup_resource_t * )get_scene_child( scene, PROGRESS_BOX_RESGROUP );
    if( private->resgroup == NULL )
    {
        print_err("cann't find resgroup!");
        return false;    
    }
    
    private->pic_bg = ( picture_resource_t * )get_resgroup_child( private->resgroup, PIC_BG );
    private->frame_wait = ( picregion_resource_t * )get_resgroup_child( private->resgroup, FRAME_WAIT);
    private->text_str = ( string_resource_t * )get_resgroup_child( private->resgroup, STR_TEXT);
    if(private->text_str != NULL)
    {
        private->text_str->str_id = (unsigned short)private->init_data.text_str_id;
    }

    private->pic_pbar_bg = ( picture_resource_t * )get_resgroup_child( private->resgroup, PIC_PBAR_BG );
    private->pic_pbar_hl = ( picture_resource_t * )get_resgroup_child( private->resgroup, PIC_PBAR_HL );

    sty_delete_bitmap( private->bmp_bg );
    private->bmp_bg = NULL;

    sty_delete_bitmap( private->bmp_pbar_bg );
    private->bmp_pbar_bg = NULL;

    sty_delete_bitmap( private->bmp_pbar_hl );
    private->bmp_pbar_hl = NULL;
    
    if ( private->pic_bg != NULL )
    {
        private->bmp_bg = sty_create_bitmap( (int)private->pic_bg->pic_id );
    }

    if ( private->pic_pbar_bg != NULL )
    {
        private->bmp_pbar_bg = sty_create_bitmap( (int)private->pic_pbar_bg->pic_id );
    }

    if ( private->pic_pbar_hl != NULL )
    {
        private->bmp_pbar_hl = sty_create_bitmap( (int)private->pic_pbar_hl->pic_id );
    }

    if( private->frame_wait != NULL )
    {
        private->current_frame = 0; 
        private->total_frame = (int)private->frame_wait->frames; 
    }
    
    //canvas resize    
    if ( progress_box->canvas_id != -1 )
    {
        private->resgroup = progress_box->private->resgroup;
        rect.x = private->resgroup->absolute_x;
        rect.y = private->resgroup->absolute_y;
        rect.width = private->resgroup->width;
        rect.height = private->resgroup->height;
        
        //set canvas region
        canvas_set_region( progress_box->canvas_id, &rect );
    }

    return true;
}

bool progress_box_set_cur_step( progress_box_t *progress_box, unsigned int step)
{
    progress_box_private_t *private;

    if ( progress_box == NULL )
    {
        print_warning("progress_box is null!");
        return false;
    }
    else
    {
        private = progress_box->private;
    }

    if( step <= private->init_data.total_step )
    {
        private->init_data.cur_step = step;
        progress_box_draw_progress_bar( progress_box );

        if(step == private->init_data.total_step)
        {
            private->text_str->str_id = (unsigned short)private->init_data.complete_text_str_id;
            progress_box_draw_text( progress_box );
            _proc_callback(progress_box, WIDGET_SIG_PROGRESS_BOX_COMPLETE);
        }

        return true;
    }
    else
    {
        return false;
    }

}

bool progress_box_set_text( progress_box_t *progress_box, int text_str_id)
{
    progress_box_private_t *private;

    if ( progress_box == NULL )
    {
        print_warning("progress_box is null!");
        return false;
    }
    else
    {
        private = progress_box->private;
    }

    private->text_str->str_id = (unsigned short)text_str_id;
    progress_box_draw_text( progress_box );

    return true;
}
