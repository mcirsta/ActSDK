/********************************************************************************
 *                              USDK(1100)
 *                             Module: app 
 *                 Copyright(c) 2003-2009 Actions Semiconductor,
 *                            All Rights Reserved. 
 * File : usb_select_box.c
 * History:         
 *      <author>    <time>           <version >             <desc>
 *      herb hub   2009-07-14          v1.0               build this file 
 ********************************************************************************/
/*!
 * \file     usb_select_box.c
 * \brief    usb_select_box widget
 * \author   herb hu 
 *               
 * \version 1.0
 * \date  2009/0y/14
 *******************************************************************************/

#include "include_case.h"
#include "commonui_sty.h"

#include "usb_select_box_private.h"

static bool _proc_callback( usb_select_box_t *usb_select_box, usb_select_box_sig_e signal );


static bool _proc_callback( usb_select_box_t *usb_select_box, usb_select_box_sig_e signal )
{
    if ( ( usb_select_box != NULL ) && ( usb_select_box->callback != NULL ) )
    {
        usb_select_box->callback( usb_select_box, signal );    //send signal to callback                  
    }
    else
    {
    }

    return true;
}

/* usb_select_box external interface */
bool usb_select_box_paint( usb_select_box_t *usb_select_box )
{
    //check param
    if ( usb_select_box == NULL )
    {
        return false;    
    }
    else
    {
        //paint bg
        usb_select_box_draw_bg( usb_select_box );

        //paint items 
        usb_select_box_draw_items( usb_select_box );
    }

    return true;
}


static bool usb_select_box_proc_key( usb_select_box_t *usb_select_box, key_event_t *pmsg )
{
    usb_select_box_sig_e sig;

    if ( usb_select_box == NULL )
    {
        return false;
    }

    switch ( pmsg->val | pmsg->type ) 
    {
        case KEY_OK | KEY_TYPE_SHORT_UP:
        case KEY_OK | KEY_TYPE_LONG_UP:
        case KEY_OK | KEY_TYPE_HOLD_UP:
        {
            if(usb_select_box->type == USB_SELECT_BOX_MTP)
            {
                if(usb_select_box->private->current_item == 0)
                {
                    sig = WIDGET_SIG_USB_SEL_BOX_MTP;
                }
                else if(usb_select_box->private->current_item == 1)
                {
                    sig = WIDGET_SIG_USB_SEL_BOX_CHARGE;
                }
                else
                {
                    sig = WIDGET_SIG_USB_SEL_BOX_CHARGE_PLAY;
                }
            }
            else if(usb_select_box->type == USB_SELECT_BOX_DATA_TRANS)
            {
                if(usb_select_box->private->current_item == 0)
                {
                    sig = WIDGET_SIG_USB_SEL_BOX_DATA_TRANS;
                }
                else if(usb_select_box->private->current_item == 1)
                {
                    sig = WIDGET_SIG_USB_SEL_BOX_CHARGE;
                }
                else
                {
                    sig = WIDGET_SIG_USB_SEL_BOX_CHARGE_PLAY;
                }
            }
            else
            {
                if(usb_select_box->private->current_item == 0)
                {
                    sig = WIDGET_SIG_USB_SEL_BOX_CHARGE;
                }
                else if(usb_select_box->private->current_item == 1)
                {
                    sig = WIDGET_SIG_USB_SEL_BOX_CHARGE_PLAY;
                }
                else
                {
                }
            }

            _proc_callback( usb_select_box, sig );    

            break;
        }

        case KEY_PREV | KEY_TYPE_DOWN:
        case KEY_PREV | KEY_TYPE_HOLD:
        {    
            usb_select_box->private->current_item--;
            if(usb_select_box->private->current_item < 0)
            {
                usb_select_box->private->current_item = usb_select_box->private->max_items - 1;
            }

            usb_select_box_paint(usb_select_box); 

            break;
        }

        case KEY_NEXT | KEY_TYPE_DOWN:
        case KEY_NEXT| KEY_TYPE_HOLD:
        {    
            usb_select_box->private->current_item++;
            if(usb_select_box->private->current_item == usb_select_box->private->max_items)
            {
                usb_select_box->private->current_item = 0;
            }

            usb_select_box_paint(usb_select_box); 

            break;
        }

        case KEY_RETURN | KEY_TYPE_SHORT_UP:
        case KEY_RETURN | KEY_TYPE_LONG_UP:
        case KEY_RETURN | KEY_TYPE_HOLD_UP:
        {    
            //external process
            _proc_callback( usb_select_box, WIDGET_SIG_USB_SEL_BOX_QUIT);    

            break;
        }

        default:
        {
            break;
        }
    }   

    return true;
}



/* usb_select_box entry */
bool usb_select_box_proc_gui( usb_select_box_t *usb_select_box, gui_wm_message_t *pmsg )
{
    bool ret = false;

    switch ( pmsg->msgid ) 
    {
        case WM_MSG_KEY:
        {
            ret = usb_select_box_proc_key( usb_select_box, (key_event_t *)(pmsg->data.p) );
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


usb_select_box_t* usb_select_box_create( usb_select_box_type_e type, usb_select_box_cb_t callback )
{
    usb_select_box_t *usb_select_box;
    resgroup_resource_t *group;
    sty_rect_t rect;

    if ( callback == NULL )
    {
        print_err("callback is null!");
        return NULL;    
    }

    //malloc data
    usb_select_box = (usb_select_box_t *)malloc( sizeof(usb_select_box_t) );        
    if ( usb_select_box == NULL )
    {
        print_err("malloc failed!");
        return NULL;    
    }    
    else
    {
        //initial data
        usb_select_box->type = type;
        usb_select_box->callback = callback;
        usb_select_box->canvas_id = -1;
    }

    //malloc private data
    usb_select_box->private = (usb_select_box_private_t *)malloc( sizeof(usb_select_box_private_t) );        
    if ( usb_select_box->private == NULL )
    {
        print_err("malloc failed!");
        free( usb_select_box );
        return NULL;    
    }    
    else
    {
        //initial private
        memset( usb_select_box->private, 0, sizeof(usb_select_box_private_t) );
    }

    //load style resource
    if( usb_select_box_load_style( usb_select_box ) == false)
    {
        print_err("load style failed!");
        free( usb_select_box->private );
        free( usb_select_box );
        return NULL;
    }

    //create canvas
    group = usb_select_box->private->resgroup;
    rect.x = group->absolute_x;
    rect.y = group->absolute_y;
    rect.width = group->width;
    rect.height = group->height;

    usb_select_box->canvas_id = canvas_create( &rect, TRANSPARENT ); 
    if ( usb_select_box->canvas_id == -1 )
    {
        print_err("canvas_create failed!");
        free( usb_select_box->private );
        free( usb_select_box );
        return false;
    }

    //»ñÈ¡dc
    usb_select_box->private->hdc = gui_dc_get( usb_select_box->canvas_id );

    // set current item, if the inserted line is usb adapter or power adapter, we
    // set the charge & play option is the default option, otherwise when the
    // inserted line is usb data line, we set the data transfer option is the default one.
    if(usb_select_box->type == USB_SELECT_BOX_ADAPTER)
    {
        usb_select_box->private->current_item = 1;
    }
    else
    {
        usb_select_box->private->current_item = 0;
    }

    return usb_select_box;
}


bool usb_select_box_delete( usb_select_box_t *usb_select_box )
{
    if ( usb_select_box != NULL )
    {
        //delete canvas
        if ( usb_select_box->canvas_id != -1 )
        {
            canvas_delete( usb_select_box->canvas_id );
            usb_select_box->canvas_id = -1;
        }

        //free private 
        if ( usb_select_box->private != NULL )
        {
            free( usb_select_box->private );
            usb_select_box->private = NULL;
        }            

        //free usb_select_box private
        free( usb_select_box );
    }

    return true;
}

/* usb_select_box load style resource */
bool usb_select_box_load_style( usb_select_box_t *usb_select_box )
{
    usb_select_box_private_t *private;
    scene_t* scene;    //!< commonui scene
    sty_rect_t rect;
    resgroup_resource_t *group;
    int sty_id;

    if ( usb_select_box == NULL )
    {
        print_warning("usb_select_box is null!");
        return false;
    }
    else
    {
        private = usb_select_box->private;
    }

    //get root resource
    scene = sty_get_common_scene();
    if ( scene == NULL )
    {
        print_err("cann't find common scene!");
        return false;    
    }

    //load control group
    group = ( resgroup_resource_t * )get_scene_child( scene, USB_SELECT_BOX_RESGROUP);

    //get child resgroup
    switch( usb_select_box->type )
    {
        case USB_SELECT_BOX_MTP:
        case USB_SELECT_BOX_DATA_TRANS:
        {
            sty_id = USB_RESGROUP;
            private->max_items = 3;
            break;
        }

        case USB_SELECT_BOX_ADAPTER:
        {
            sty_id = ADAPTER_RESGROUP;
            private->max_items = 2;
            break;
        }

        default:
        {
            sty_id = 0;
            break;
        }
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
    private->pic_icon = ( picture_resource_t * )get_resgroup_child( private->resgroup, PIC_ICON );
    private->pic_sel_frame = ( picture_resource_t * )get_resgroup_child( private->resgroup, PIC_SELECT);

    private->str_highlight = ( string_resource_t * )get_resgroup_child( private->resgroup, STR_HIGHLIGHT);
    private->str_items[0] = ( string_resource_t * )get_resgroup_child( private->resgroup, STR_ITEM_1);
    private->str_items[1] = ( string_resource_t * )get_resgroup_child( private->resgroup, STR_ITEM_2);

    if( (usb_select_box->type == USB_SELECT_BOX_MTP) || (usb_select_box->type == USB_SELECT_BOX_DATA_TRANS))
    {
        private->str_items[2] = ( string_resource_t * )get_resgroup_child( private->resgroup, STR_ITEM_3);

        private->str_mtp = ( string_resource_t * )get_resgroup_child( private->resgroup, STR_MTP);
        private->str_data_trans = ( string_resource_t * )get_resgroup_child( private->resgroup, STR_DATA_TRANS);
    }

    private->str_charge = ( string_resource_t * )get_resgroup_child( private->resgroup, STR_CHARGE);
    private->str_charge_play = ( string_resource_t * )get_resgroup_child( private->resgroup, STR_CHARGE_PLAY);

    switch(usb_select_box->type)
    {
        case USB_SELECT_BOX_MTP:
        {
            if(private->str_mtp != NULL)
            {
                private->str_items[0]->str_id = private->str_mtp->str_id;
            }
            if(private->str_charge != NULL)
            {
                private->str_items[1]->str_id = private->str_charge->str_id;
            }
            if(private->str_charge_play != NULL)
            {
                private->str_items[2]->str_id = private->str_charge_play->str_id;
            }

            break;
        }
        case USB_SELECT_BOX_DATA_TRANS:
        {
            if(private->str_data_trans != NULL)
            {
                private->str_items[0]->str_id = private->str_data_trans->str_id;
            }
            if(private->str_charge != NULL)
            {
                private->str_items[1]->str_id = private->str_charge->str_id;
            }
            if(private->str_charge_play != NULL)
            {
                private->str_items[2]->str_id = private->str_charge_play->str_id;
            }
            break;
        }
        case USB_SELECT_BOX_ADAPTER:
        {
            if(private->str_charge != NULL)
            {
                private->str_items[0]->str_id = private->str_charge->str_id;
            }

            if(private->str_charge_play != NULL)
            {
                private->str_items[1]->str_id = private->str_charge_play->str_id;
            }
            break;
        }
        default :
        {
            break;
        }
    }

    //canvas resize    
    if ( usb_select_box->canvas_id != -1 )
    {
        rect.x = private->resgroup->absolute_x;
        rect.y = private->resgroup->absolute_y;
        rect.width = private->resgroup->width;
        rect.height = private->resgroup->height;

        //set canvas region
        canvas_set_region( usb_select_box->canvas_id, &rect );
        canvas_erase_all( usb_select_box->canvas_id );
    }

    return true;
}
