#include "include_case.h"


#include "commonui_sty.h"
#include "volumebar_private.h"



void vbar_draw_bg(volumebar_t *vbar)
{
    if (vbar->private_data->vol_bg_status == need_paint)
    {
        canvas_erase_all( vbar->canvas_id );
        sty_draw_picture_res( vbar->private_data->hdc, vbar->private_data->vb_bg );
    }
    else
    {
        print_dbg("vol bg status = %d\n", vbar->private_data->vol_bg_status);
    }
}

void vbar_draw_dec(volumebar_t *vbar)
{
    sty_rect_t rect;
    
    if (vbar->private_data->vol_dec_status == need_paint)
    {
        if ( 1 == vbar->private_data->dec_pressed )
        {
            rect.x = vbar->private_data->vb_dec->x;
            rect.y = vbar->private_data->vb_dec->y;
            rect.width = vbar->private_data->vb_dec->width;
            rect.height = vbar->private_data->vb_dec->height;
            
            /*! erase canvas region */
            canvas_erase(vbar->canvas_id, &rect);

            /*! draw background region */
            sty_draw_sub_picture_id(vbar->private_data->hdc, vbar->private_data->vb_bg->pic_id, 
                                                        vbar->private_data->vb_dec->x, vbar->private_data->vb_dec->y,
                                                        &rect);

            /*! draw "-" highlight */
            sty_draw_picture_res( vbar->private_data->hdc, vbar->private_data->vb_dech );
        }
        else
        {
            rect.x = vbar->private_data->vb_dec->x;
            rect.y = vbar->private_data->vb_dec->y;
            rect.width = vbar->private_data->vb_dec->width;
            rect.height = vbar->private_data->vb_dec->height;

            /*! erase canvas region */         
            canvas_erase(vbar->canvas_id, &rect );

            /*! draw background region */
            sty_draw_sub_picture_id(vbar->private_data->hdc, vbar->private_data->vb_bg->pic_id, 
                                                        vbar->private_data->vb_dec->x, vbar->private_data->vb_dec->y,
                                                        &rect);
            /*! draw gray "-" */
            sty_draw_picture_res( vbar->private_data->hdc, vbar->private_data->vb_dec );
        }
    }
    else
    {
        print_dbg("vol dec status = %d\n", vbar->private_data->vol_dec_status);
    }
}

void vbar_draw_inc(volumebar_t *vbar)
{
    sty_rect_t rect;

    if (vbar->private_data->vol_inc_status == need_paint)
    {
        if ( 1 == vbar->private_data->inc_pressed )
        {
            rect.x = vbar->private_data->vb_inc->x;
            rect.y = vbar->private_data->vb_inc->y;
            rect.width = vbar->private_data->vb_inc->width;
            rect.height = vbar->private_data->vb_inc->height;
            
            /*!erase canvas region */
            canvas_erase(vbar->canvas_id, &rect );

            /*! draw background region */
            sty_draw_sub_picture_id(vbar->private_data->hdc, vbar->private_data->vb_bg->pic_id, 
                                                        vbar->private_data->vb_inc->x, vbar->private_data->vb_inc->y,
                                                        &rect );

            /*! draw "+" highlight */
            sty_draw_picture_res( vbar->private_data->hdc, vbar->private_data->vb_inch );
        }
        else
        {
            rect.x = vbar->private_data->vb_inc->x;
            rect.y = vbar->private_data->vb_inc->y;
            rect.width = vbar->private_data->vb_inc->width;
            rect.height = vbar->private_data->vb_inc->height;

            /*!erase canvas region */
            canvas_erase(vbar->canvas_id, &rect);

            /*! draw background region */
            sty_draw_sub_picture_id(vbar->private_data->hdc, vbar->private_data->vb_bg->pic_id, 
                                                        vbar->private_data->vb_inc->x, vbar->private_data->vb_inc->y,
                                                        &rect);

            /*! draw gray "+" */
            sty_draw_picture_res( vbar->private_data->hdc, vbar->private_data->vb_inc );
        }
    }
    else
    {
        print_dbg("vol inc status = %d\n", vbar->private_data->vol_inc_status);
    }
}

void vbar_draw_icon(volumebar_t *vbar)
{
    if (vbar->private_data->vol_icon_status == need_paint)
    {
        sty_draw_picture_res( vbar->private_data->hdc, vbar->private_data->vb_icon );
    }
    else
    {
        print_dbg("vol icon status = %d\n", vbar->private_data->vol_icon_status);
    }
}
void vbar_draw_pb(volumebar_t *vbar)
{
    int ival = 0;//0-31
    sty_rect_t rect;
    int decx = 0;
    int decy = 0;
    
    
    if(vbar->private_data->vol_pb_status != need_paint)
    {
        return;
    }

    if( (vbar->private_data->vol_bg_status == need_paint)
        || ((vbar->private_data->dec_pressed == 0) && (vbar->private_data->inc_pressed == 0)))
    {
        sty_draw_picture_res( vbar->private_data->hdc, vbar->private_data->vb_pbbg );
        for(ival = 0; ival < vbar->cur_value; ival++)
        {
            sty_draw_picture_res(vbar->private_data->hdc, vbar->private_data->vb_pb[ival]);  
        }
    }
    else if(vbar->private_data->dec_pressed == 1)
    {
          //cur_value 0-31,vb_pb 0-30
          //-1
          //cur_value 0-30 vb_pb 0-29
        rect.x      = vbar->private_data->vb_pb[vbar->cur_value]->x;
        rect.y      = vbar->private_data->vb_pb[vbar->cur_value]->y;
        rect.width  = vbar->private_data->vb_pb[vbar->cur_value]->width;
        rect.height = vbar->private_data->vb_pb[vbar->cur_value]->height;
        decx        = rect.x;
        decy        = rect.y;
        

        /*! erase canvas region */
        canvas_erase( vbar->canvas_id, &rect );
        /*! draw background region */
        sty_draw_sub_picture_id( vbar->private_data->hdc, vbar->private_data->vb_bg->pic_id, 
                                                decx, decy, &rect);
        
        rect.x     = rect.x - vbar->private_data->vb_pbbg->x;
        rect.y     = rect.y - vbar->private_data->vb_pbbg->y;
        /*!draw dec region */
       

        sty_draw_sub_picture_id( vbar->private_data->hdc, vbar->private_data->vb_pbbg->pic_id, 
                                                decx, decy, &rect);
        
    }
    else if(vbar->private_data->inc_pressed == 1)
    {
        sty_draw_picture_res(vbar->private_data->hdc, vbar->private_data->vb_pb[vbar->cur_value-1]); 
    }
    else
    {
        print_dbg("vol pb status = %d\n", vbar->private_data->vol_pb_status);
    }

}
#if 0
//下面是进度条实现的源码。
void vbar_draw_pb(volumebar_t *vbar)
{
    int TotalWidth;
    int CurrentWidth;
    int StartX;
    int DecWidth;
    int IncWidth;
    sty_rect_t rect;

    TotalWidth = vbar->private_data->vb_pb->width;// progress bar total width
    CurrentWidth = TotalWidth * vbar->cur_value / ( vbar->max_value - vbar->min_value );// progress bar current width

    StartX = vbar->private_data->vb_pbbg->x + CurrentWidth;

    if ( vbar->private_data->vol_pb_status == need_paint 
         && vbar->private_data->dec_pressed == 0 
         && vbar->private_data->inc_pressed == 0 )
    {
        /*! draw gray process bar */
        sty_draw_picture_res( vbar->private_data->hdc, vbar->private_data->vb_pbbg );

        /*! draw highlight process bar */
        rect.x = 0;
        rect.y = 0;
        rect.width = CurrentWidth;
        rect.height = vbar->private_data->vb_pb->height;
        sty_draw_sub_picture_id( vbar->private_data->hdc, vbar->private_data->vb_pb->pic_id, 
                                                    vbar->private_data->vb_pb->x, vbar->private_data->vb_pb->y,
                                                    &rect );
    }
    else if (vbar->private_data->vol_pb_status == need_paint && vbar->private_data->dec_pressed == 1)
    {
        DecWidth = TotalWidth *(vbar->cur_value + 1) / ( vbar->max_value - vbar->min_value ) 
                           - TotalWidth * vbar->cur_value / ( vbar->max_value - vbar->min_value );
        
        rect.x = StartX;
        rect.y = vbar->private_data->vb_pbbg->y;
        rect.width = DecWidth;
        rect.height = vbar->private_data->vb_pbbg->height;

        /*! erase canvas region */
        canvas_erase( vbar->canvas_id, &rect );

        /*! draw background region */
        sty_draw_sub_picture_id( vbar->private_data->hdc, vbar->private_data->vb_bg->pic_id, 
                                                    StartX, vbar->private_data->vb_pbbg->y, 
                                                    &rect);

        /*!draw dec region */
        rect.x = CurrentWidth;
        rect.y = 0;
        rect.width = DecWidth;
        rect.height = vbar->private_data->vb_pbbg->height;
        sty_draw_sub_picture_id( vbar->private_data->hdc, vbar->private_data->vb_pbbg->pic_id, 
                                                    StartX, vbar->private_data->vb_pbbg->y,
                                                    &rect  );
    }
    else if (vbar->private_data->vol_pb_status == need_paint && vbar->private_data->inc_pressed == 1)
    {
        IncWidth = TotalWidth * vbar->cur_value / ( vbar->max_value - vbar->min_value ) 
                            - TotalWidth * ( vbar->cur_value - 1 ) / ( vbar->max_value - vbar->min_value );

        /*! draw inc region */
        rect.x = CurrentWidth - IncWidth;
        rect.y = 0;
        rect.width = IncWidth;
        rect.height = vbar->private_data->vb_pb->height;
        sty_draw_sub_picture_id( vbar->private_data->hdc, vbar->private_data->vb_pb->pic_id, 
                                                    StartX - IncWidth, vbar->private_data->vb_pb->y,
                                                    &rect  );
    }
    else
        print_dbg("vol pb status = %d\n", vbar->private_data->vol_pb_status);
}
#endif

void vbar_draw_logo(volumebar_t *vbar)
{
    if (vbar->private_data->vol_logo_status == need_paint)
    {
        sty_draw_string_res( vbar->private_data->hdc, vbar->private_data->vb_logo );
    }
    else
    {
        print_dbg("vol logo status = %d\n", vbar->private_data->vol_logo_status);
    }
}

void vbar_draw_val(volumebar_t *vbar)
{
    int buf[16];
    sty_rect_t rect;

    if (vbar->private_data->vol_val_status == need_paint)
    {
        rect.x = vbar->private_data->vb_val->x;
        rect.y = vbar->private_data->vb_val->y;
        rect.width = vbar->private_data->vb_val->width;
        rect.height = vbar->private_data->vb_val->height;
        
        /*! erase canvas region */
        canvas_erase(vbar->canvas_id, &rect);

        /*! draw background region */
        sty_draw_sub_picture_id(vbar->private_data->hdc, vbar->private_data->vb_bg->pic_id, 
                                                    vbar->private_data->vb_val->x, vbar->private_data->vb_val->y, 
                                                    &rect);

        /*! draw value */
        sprintf( buf, "%d/%d", vbar->cur_value, vbar->max_value);
        sty_draw_string_res_ext( vbar->private_data->hdc, vbar->private_data->vb_val, buf );
    }
    else
    {
        print_dbg("vol value status = %d\n", vbar->private_data->vol_val_status);    
    }
}
