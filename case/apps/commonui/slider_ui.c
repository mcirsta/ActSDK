/*******************************************************************************
 *                              USDK(1100)
 *                            Module: common_slider
 *                 Copyright(c) 2003-2008 Actions Semiconductor,
 *                            All Rights Reserved.
 *
 * History:
 *      <author>    <time>           <version >             <desc>
 *        2009-4-22 13:12   1.0             build this file
 *******************************************************************************/
/*!
 * \file     slider_ui.c
 * \brief    概述
 * \author
 * \par      GENERAL DESCRIPTION:
 *               slider控件重绘相关
 * \par      EXTERNALIZED FUNCTIONS:
 *               用到外面的模块
 * \version 1.0
 * \date  2009-4-22
 *******************************************************************************/

#include "slider_private.h"
/******************************************************************************/
/*!
 * \par  Description:
 *	  slider_paint-- 重绘slider控件
 * \param[in]     none
 * \param[in]     none
 * \param[in]     none
 * \param[out]   none
 * \return       the result
 * \retval
 * \retval
 * \ingroup      common_slider
 * \par          exmaple code  none
 * \code
 *
 * \endcode
 * \note
 *******************************************************************************/
bool slider_paint(slider_t *slider)
{
    int hdc;
    short y, h, total_h;
    slider_private_data_t *private;

    if (slider == NULL)
    {
        return false;
    }
    else
    {
        private = slider->data;    
    }

    if (( private->bmp_bg == NULL ) || ( private->bmp_cur == NULL ))
    {
        return false;
    }

    hdc = gui_dc_get(slider->canvas_id);
    
    if( slider->extern_canvas == false )
    {
        canvas_erase_all(slider->canvas_id);
    }    

    print_dbg("inIndex=%d----\n", slider->init.page_index);

    /* 每次都画背景，避免非法输入数据的时候不更新背景图片 */
#if 0
    sty_draw_picture_res(hdc, slider->data->pic_bg); 
#else
    sty_draw_bitmap( hdc, private->bmp_bg,
                        slider->data->pic_bg->x + slider->xoffset, slider->data->pic_bg->y + slider->yoffset );
#endif
        
    if ((0 == slider->init.total_item) || (0 == slider->init.item_perpage)) //!item_perpage >= 1
    {
        print_err("init value is zero0");//有可能在子菜单为0项,所以画背景前移
        return false;
    }

    if (0 == slider->init.page_index) //!page_index >= 1
    {
        print_err("err cur zero0");
        return false;
    }

    if (slider->init.page_index > slider->init.total_item)
    {
        slider->init.page_index = slider->init.total_item;
        return false;
    }

    switch (slider->type)
    {
        case W_SB_TYPE_LIST:
        case W_SB_TYPE_OPTION:
        print_dbg("---total=%d-----", slider->init.total_item);
        print_dbg("pageindex=%d----pagesize=%d-----\n", slider->init.page_index, slider->init.item_perpage);

        total_h = slider->data->pic_bg->height - (slider->data->pic_cur->y - slider->data->pic_bg->y) * 2;
        if ((slider->init.total_item) <= (slider->init.item_perpage))
        {
            h = 0;//total_h;
            y = 0;//ui 风格选为小于一页时候只画黑条
            //y = slider->data->sb_pic_cur->y;	//满格
        }
        else
        {
            h = (short)(total_h * (slider->init.item_perpage) / (slider->init.total_item));
            y = (short)(((slider->init.page_index) - 1) * (total_h - h) / ((slider->init.total_item)
                    - (slider->init.item_perpage)));
                
            print_dbg("index=%d----th=%d-----\n", slider->init.page_index, total_h);
            print_dbg("total=%d-------\n", slider->init.total_item);
                
        }
        
        print_dbg("y=%d----h=%d-----total_h=%d\n", y, h, total_h);
        
        if ((0 == y) && (0 == h))
        {
            return false;
        }
        else
        {
            sty_rect_t srect;
            srect.x     = 0;
            srect.y     = y;
            srect.width = (slider->data->pic_cur->width);
            srect.height= h;

            #if 0
            sty_draw_sub_picture_id(hdc, slider->data->pic_cur->pic_id, (int) (slider->data->pic_cur->x),
                    (int) (slider->data->pic_cur->y + y),&srect);//!show high light
            #else
            sty_draw_sub_bitmap( hdc, private->bmp_cur,(int) (slider->data->pic_cur->x + slider->xoffset),
                    (int) (slider->data->pic_cur->y + y + slider->yoffset),&srect);
            #endif
        }

        break;

        default:
        break;

    }
    return true;

}

