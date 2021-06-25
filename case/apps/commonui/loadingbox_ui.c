/********************************************************************************
 *                              USDK(1100)
 *                             Module: app 
 *                 Copyright(c) 2003-2009 Actions Semiconductor,
 *                            All Rights Reserved. 
 * File : loadingbox_ui.c
 * History:         
 *      <author>    <time>           <version >             <desc>
 *    songzhining    2009-2-25          v1.0               build this file 
 ********************************************************************************/
/*!
* \file     loadingbox_ui.c
* \brief    loadingbox widget
* \author   songzhining
*               
* \version 1.0
* \date  2009/02/25
*******************************************************************************/
    
#include "include_case.h"


#include "commonui_sty.h"

#include "loadingbox_private.h"

static bool _erase_picregion( loadingbox_t *loadingbox, picregion_resource_t *picreg )
{
    loadingbox_private_t *private = loadingbox->private;
    sty_rect_t rect;
    
    //draw bg
    if ( ( private != NULL ) && ( picreg != NULL ) )
    {
        rect.x = picreg->x;
        rect.y = picreg->y;
        rect.width = picreg->width;
        rect.height = picreg->height;
        
        canvas_erase( loadingbox->canvas_id, &rect );
        
        if ( private->bmp_bg != NULL )
        {
            sty_draw_sub_bitmap( private->hdc, private->bmp_bg, picreg->x, picreg->y, &rect );
        }
    }

    return true;
}

static bool _erase_string( loadingbox_t *loadingbox, string_resource_t *str )
{
    loadingbox_private_t *private = loadingbox->private;
    sty_rect_t rect;
    
    //draw bg
    if ( ( private != NULL ) && ( str != NULL ) )
    {
        rect.x = str->x;
        rect.y = str->y;
        rect.width = str->width;
        rect.height = str->height;
        
        canvas_erase( loadingbox->canvas_id, &rect );
        
        if ( private->bmp_bg != NULL )
        {
            sty_draw_sub_bitmap( private->hdc, private->bmp_bg, str->x, str->y, &rect );
        }
    }

    return true;
}

/* loadingbox draw background picture*/
bool loadingbox_draw_bg( loadingbox_t *loadingbox )
{
    loadingbox_private_t *private = loadingbox->private;
    
    //erase
    canvas_erase_all( loadingbox->canvas_id );

    //draw menu bg
    if ( private->pic_bg != NULL )
    {
        sty_draw_bitmap( private->hdc, private->bmp_bg,
                        private->pic_bg->x, private->pic_bg->y );
    }

    return true;
}

/* loadingbox draw one line */
bool loadingbox_draw_one_frame( loadingbox_t *loadingbox )
{
    loadingbox_private_t *private = loadingbox->private;
    
    //erase area
    _erase_picregion( loadingbox, private->reg_icon );
    _erase_string( loadingbox, private->str_name );
    
    //draw text
    if ( private->str_name != NULL )
    {
        sty_draw_string_res( private->hdc, private->str_name );
    }
    
    //draw icon
    if ( ( private->total > 0 ) && ( private->reg_icon != NULL ) )
    {
        sty_draw_picregion_res( private->hdc, private->reg_icon, (unsigned short)private->current );
        
        //next frame
        private->current = ( private->current + 1 ) % private->total;
    }
    
    return true;
}
