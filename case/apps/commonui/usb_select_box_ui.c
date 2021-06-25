/********************************************************************************
 *                              USDK(1100)
 *                             Module: app 
 *                 Copyright(c) 2003-2009 Actions Semiconductor,
 *                            All Rights Reserved. 
 * File : usb_select_box_ui.c
 * History:         
 *      <author>    <time>           <version >             <desc>
 *    herb hu      2009-07-14          v1.0               build this file 
 ********************************************************************************/
/*!
* \file     usb_select_box_ui.c
* \brief    usb select box widget
* \author   herb hu 
*               
* \version 1.0
* \date  2009/07/14
*******************************************************************************/

#include "usb_select_box_private.h"

/* loadingbox draw background picture*/
bool usb_select_box_draw_bg( usb_select_box_t * usb_select_box )
{
    usb_select_box_private_t *private = usb_select_box->private;
    
    if ( private->pic_bg != NULL )
    {
        canvas_erase_all( usb_select_box->canvas_id );
        sty_draw_picture_res(private->hdc, private->pic_bg);
    }

    return true;
}

/* usb select box draw items */
bool usb_select_box_draw_items( usb_select_box_t * usb_select_box )
{
    int i;
    usb_select_box_private_t *private = usb_select_box->private;
    
    /* draw icon */
    if ( private->pic_icon != NULL )
    {
        sty_draw_picture_res(private->hdc, private->pic_icon);
    }

    if(private->str_highlight != NULL)
    {
        private->str_highlight->str_id = private->str_items[private->current_item]->str_id;
        private->str_highlight->x = private->str_items[private->current_item]->x;
        private->str_highlight->y = private->str_items[private->current_item]->y;
        private->str_highlight->width = private->str_items[private->current_item]->width;
        private->str_highlight->height = private->str_items[private->current_item]->height;
    }

    if(private->pic_sel_frame != NULL)
    {
        private->pic_sel_frame->y = private->str_items[private->current_item]->y;
    }
    
    /***************************************************************
     * draw highlight item 
     * *************************************************************
     */

    /* draw highlight frame frist */ 
    if ( private->pic_sel_frame != NULL )
    {
        sty_draw_picture_res(private->hdc, private->pic_sel_frame);
    }

    /* then highlight item string */ 
    if ( private->str_highlight != NULL )
    {
        sty_draw_string_res(private->hdc, private->str_highlight);
    }

    /***************************************************************
     * draw other items
     * *************************************************************
     */

    for(i = 0; i < private->max_items; i++)
    {
        if(i != private->current_item)
        {
            if ( private->str_items[i] != NULL )
            {
                sty_draw_string_res(private->hdc, private->str_items[i]);
            }
        }
    }

    return true;
}

