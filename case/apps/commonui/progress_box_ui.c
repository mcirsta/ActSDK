/********************************************************************************
 *                              USDK(1100)
 *                             Module: app 
 *                 Copyright(c) 2003-2009 Actions Semiconductor,
 *                            All Rights Reserved. 
 * File : progress_box_ui.c
 * History:         
 *      <author>    <time>           <version >             <desc>
 *    herb hu      2009-07-27          v1.0               build this file 
 ********************************************************************************/
/*!
* \file     progress_box_ui.c
* \brief    progress_box widget
* \author   herb hu 
*               
* \version 1.0
* \date  2009/07/27
*******************************************************************************/
    
#include "progress_box_private.h"

static bool _erase_picregion( progress_box_t *progress_box, picregion_resource_t *picreg );
static bool _erase_text( progress_box_t *progress_box, string_resource_t *str );
static bool _erase_progress_bar( progress_box_t *progress_box, picture_resource_t *pic );


static bool _erase_picregion( progress_box_t *progress_box, picregion_resource_t *picreg )
{
    progress_box_private_t *private = progress_box->private;
    sty_rect_t rect;
    
    //draw bg
    if ( ( private != NULL ) && ( picreg != NULL ) )
    {
        rect.x = picreg->x;
        rect.y = picreg->y;
        rect.width = picreg->width;
        rect.height = picreg->height;
        
        canvas_erase( progress_box->canvas_id, &rect );
        
        if ( private->bmp_bg != NULL )
        {
            sty_draw_sub_bitmap( private->hdc, private->bmp_bg, picreg->x, picreg->y, &rect );
        }
    }

    return true;
}

static bool _erase_text( progress_box_t *progress_box, string_resource_t *str )
{
    progress_box_private_t *private = progress_box->private;
    sty_rect_t rect;
    
    //draw bg
    if ( ( private != NULL ) && ( str != NULL ) )
    {
        rect.x = str->x;
        rect.y = str->y;
        rect.width = str->width;
        rect.height = str->height;
        
        canvas_erase( progress_box->canvas_id, &rect );
        
        if ( private->bmp_bg != NULL )
        {
            sty_draw_sub_bitmap( private->hdc, private->bmp_bg, str->x, str->y, &rect );
        }
    }

    return true;
}

static bool _erase_progress_bar( progress_box_t *progress_box, picture_resource_t *pic )
{
    progress_box_private_t *private = progress_box->private;
    sty_rect_t rect;

    //draw bg
    if ( ( private != NULL ) && ( pic != NULL ) )
    {
        rect.x = pic->x;
        rect.y = pic->y;
        rect.width = pic->width;
        rect.height = pic->height;

        canvas_erase( progress_box->canvas_id, &rect );

        if ( private->bmp_bg != NULL )
        {
            sty_draw_sub_bitmap( private->hdc, private->bmp_bg, pic->x, pic->y, &rect );
        }
    }

    return true;
}

bool progress_box_draw_bg( progress_box_t *progress_box )
{
    progress_box_private_t *private = progress_box->private;
    
    canvas_erase_all( progress_box->canvas_id );

    if( (private->bmp_bg != NULL) && (private->pic_bg != NULL) )
    {
        sty_draw_bitmap( private->hdc, private->bmp_bg,
                        private->pic_bg->x, private->pic_bg->y );
    }

    return true;
}

bool progress_box_draw_text( progress_box_t *progress_box )
{
    progress_box_private_t *private = progress_box->private;
    
    _erase_text( progress_box, private->text_str);

    //draw text
    if ( private->text_str != NULL )
    {
        sty_draw_string_res( private->hdc, private->text_str);
    }

    return true;
}

bool progress_box_draw_wait_animation( progress_box_t *progress_box )
{
    progress_box_private_t *private = progress_box->private;
    
    //erase area
    _erase_picregion( progress_box, private->frame_wait);
    
    //draw wait frame 
    if ( ( private->total_frame > 0 ) && ( private->frame_wait != NULL ) )
    {
        sty_draw_picregion_res( private->hdc, private->frame_wait, (unsigned short)private->current_frame );
        
        //next frame
        private->current_frame = ( private->current_frame + 1 ) % private->total_frame;
    }
    
    return true;
}

bool progress_box_draw_progress_bar( progress_box_t *progress_box )
{
    progress_box_private_t *private = progress_box->private;
    unsigned int bar_width = 0 ;
    unsigned int i = 0 ;
    unsigned int n = 0 ;

    //erase area
    _erase_progress_bar( progress_box, private->pic_pbar_bg );

    if ( (private->bmp_pbar_bg != NULL) && (private->pic_pbar_bg != NULL) )
    {
        sty_draw_bitmap( private->hdc, private->bmp_pbar_bg,
                private->pic_pbar_bg->x, private->pic_pbar_bg->y );
    }

    if ( (private->bmp_pbar_hl != NULL) && (private->pic_pbar_hl != NULL) && (private->pic_pbar_bg != NULL) )
    {
        bar_width =  (((unsigned int)private->pic_pbar_bg->width - 2) * private->init_data.cur_step ) \
                     / private->init_data.total_step;
        n = bar_width / (unsigned int)private->pic_pbar_hl->width;

        for(i = 0; i < n; i++)
        {
            sty_draw_bitmap( private->hdc, private->bmp_pbar_hl,
            ((int)private->pic_pbar_hl->x + ((int)i*(int)private->pic_pbar_hl->width)), (int)private->pic_pbar_hl->y);
        }
    }

    return true;
}
