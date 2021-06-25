/********************************************************************************
 *                              USDK(1100)
 *                             Module: app 
 *                 Copyright(c) 2003-2009 Actions Semiconductor,
 *                            All Rights Reserved. 
 * File : loadingbox.c
 * History:         
 *      <author>    <time>           <version >             <desc>
 *    songzhining    2009-2-25          v1.0               build this file 
 ********************************************************************************/
/*!
* \file     loadingbox.c
* \brief    loadingbox widget
* \author   songzhining
*               
* \version 1.0
* \date  2009/02/25
*******************************************************************************/
    
#include "include_case.h"


#include "commonui_sty.h"

#include "loadingbox_private.h"

static bool _proc_callback( loadingbox_t *loadingbox, loadingbox_sig_e signal );
static bool _timer_proc( loadingbox_t *loadingbox );


static bool _proc_callback( loadingbox_t *loadingbox, loadingbox_sig_e signal )
{
    if ( ( loadingbox != NULL ) && ( loadingbox->callback != NULL ) )
    {
        loadingbox->callback( loadingbox, signal );    //send signal to callback                  
    }
    else
    {
    }
    
    return true;
}

static bool _timer_proc( loadingbox_t *loadingbox )
{
    //paint one frame
    loadingbox_draw_one_frame( loadingbox );
    
    return true;
}

/* loadingbox external interface */
bool loadingbox_paint( loadingbox_t *loadingbox )
{
    //check param
    if ( loadingbox == NULL )
    {
        return false;    
    }
    else
    {
        //paint bg
        loadingbox_draw_bg( loadingbox );
        
        //paint one frame
        loadingbox_draw_one_frame( loadingbox );
    }

    return true;
}


static bool loadingbox_proc_key( loadingbox_t *loadingbox, key_event_t *pmsg )
{
    //check param
    if ( loadingbox == NULL )
    {
        return false;
    }
    else
    { 
    }

    switch ( pmsg->val | pmsg->type ) 
    {
    case KEY_OK | KEY_TYPE_SHORT_UP:
    case KEY_OK | KEY_TYPE_LONG_UP:
    case KEY_OK | KEY_TYPE_HOLD_UP:
        {
            //external process
            _proc_callback( loadingbox, WIDGET_SIG_LOADINGBOX_OK );    
        }
        break;
        
    case KEY_RETURN | KEY_TYPE_SHORT_UP:
    case KEY_RETURN | KEY_TYPE_LONG_UP:
    case KEY_RETURN | KEY_TYPE_HOLD_UP:
        {    
            //external process
            _proc_callback( loadingbox, WIDGET_SIG_LOADINGBOX_QUIT );    
        }
        break;
        
    default:
        break;
    }   

    return true;
}



/* loadingbox entry */
bool loadingbox_proc_gui( loadingbox_t *loadingbox, gui_wm_message_t *pmsg )
{
    bool ret = false;
    
    switch ( pmsg->msgid ) 
    {
//    case WM_MSG_PAINT:
//        ret = loadingbox_paint( loadingbox );
//        break;
         
    case WM_MSG_KEY:
        ret = loadingbox_proc_key( loadingbox, (key_event_t *)(pmsg->data.p) );
        break;
        
    case WM_MSG_TOUCH:
        break;
        
    default:
        break;
    }
    
    return ret;
}


loadingbox_t* loadingbox_create( loadingbox_type_e type, loadingbox_cb_t callback )
{
    loadingbox_t *loadingbox;
    bool ret;
    resgroup_resource_t *group;
    sty_rect_t rect;
       
    if ( callback == NULL )
    {
        print_err("callback is null!");
        return NULL;    
    }
    
    //malloc data
    loadingbox = (loadingbox_t *)malloc( sizeof(loadingbox_t) );        
    if ( loadingbox == NULL )
    {
        print_err("malloc failed!");
        return NULL;    
    }    
    else
    {
        //initial data
        loadingbox->type = type;
        loadingbox->callback = callback;
        loadingbox->canvas_id = -1;
    }
    
    //malloc private data
    loadingbox->private = (loadingbox_private_t *)malloc( sizeof(loadingbox_private_t) );        
    if ( loadingbox->private == NULL )
    {
        print_err("malloc failed!");
        free( loadingbox );
        return NULL;    
    }    
    else
    {
        //initial private
        memset( loadingbox->private, 0, sizeof(loadingbox_private_t) );
        loadingbox->private->timer_id = -1;
    }
        
    //load style resource
    ret = loadingbox_load_style( loadingbox );
    if ( ret == false )
    {
        print_err("canvas_create failed!");
        free( loadingbox->private );
        free( loadingbox );
        return NULL;
    }

    //create canvas
    group = loadingbox->private->resgroup;
    rect.x = group->absolute_x;
    rect.y = group->absolute_y;
    rect.width = group->width;
    rect.height = group->height;
    
    loadingbox->canvas_id = canvas_create( &rect, TRANSPARENT ); 
    if ( loadingbox->canvas_id == -1 )
    {
        print_err("canvas_create failed!");
        free( loadingbox->private );
        free( loadingbox );
        return false;
    }
    
    //»ñÈ¡dc
    loadingbox->private->hdc = gui_dc_get( loadingbox->canvas_id );

    //start timer
    loadingbox->private->timer_id = set_timer( ANIMATION_INTERVAL, (timer_proc)_timer_proc, (void*)loadingbox );
    
    return loadingbox;
}


bool loadingbox_delete( loadingbox_t *loadingbox )
{
    if ( loadingbox != NULL )
    {
        //try to kill scroll timer
        if ( loadingbox->private != NULL )
        {
            kill_timer( loadingbox->private->timer_id );
            loadingbox->private->timer_id = -1;
            
            //free bitmap
            sty_delete_bitmap( loadingbox->private->bmp_bg );
            loadingbox->private->bmp_bg = NULL;        
        }        
        
        //delete canvas
        if ( loadingbox->canvas_id != -1 )
        {
            canvas_delete( loadingbox->canvas_id );
            loadingbox->canvas_id = -1;
        }

        //free private 
        if ( loadingbox->private != NULL )
        {
            free( loadingbox->private );
            loadingbox->private = NULL;
        }            

        //free loadingbox private
        free( loadingbox );
    }
    
    return true;
}

/* loadingbox load style resource */
bool loadingbox_load_style( loadingbox_t *loadingbox )
{
    loadingbox_private_t *private;
    scene_t* scene;    //!< commonui scene
    resgroup_resource_t *group;
    sty_rect_t rect;
    int sty_id;
    
    if ( loadingbox == NULL )
    {
        print_warning("loadingbox is null!");
        return false;
    }
    else
    {
        private = loadingbox->private;
    }
    
    //get root resource
    scene = sty_get_common_scene();
    if ( scene == NULL )
    {
        print_err("cann't find common scene!");
        return false;    
    }

    //load control group
    group = ( resgroup_resource_t * )get_scene_child( scene, LOADINGBOX_RESGROUP );
    
    //get child resgroup
    switch( loadingbox->type )
    {
    case LOADINGBOX_SEARCHING:
        sty_id = SEARCHING_RESGROUP;
        break;
        
    case LOADINGBOX_WAITING:
        sty_id = WAITING_RESGROUP;
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
    private->pic_bg = ( picture_resource_t * )get_resgroup_child( private->resgroup, PIC_BG );
    private->reg_icon = ( picregion_resource_t * )get_resgroup_child( private->resgroup, REG_ICON );
    private->str_name = ( string_resource_t * )get_resgroup_child( private->resgroup, STR_NAME );

    //free bitmap
    sty_delete_bitmap( private->bmp_bg );
    private->bmp_bg = NULL;
    
    //cache bitmap
    if ( private->pic_bg != NULL )
    {
        private->bmp_bg = sty_create_bitmap( (int)private->pic_bg->pic_id );
    }
    
    if( private->reg_icon != NULL )
    {
        private->current = 0; 
        private->total = (int)private->reg_icon->frames; 
    }
    else
    {
        private->current = 0; 
        private->total = 0; 
    }
    
    //canvas resize    
    if ( loadingbox->canvas_id != -1 )
    {
        group = loadingbox->private->resgroup;
        rect.x = group->absolute_x;
        rect.y = group->absolute_y;
        rect.width = group->width;
        rect.height = group->height;
        
        //set canvas region
        canvas_set_region( loadingbox->canvas_id, &rect );
    }

    return true;
}
