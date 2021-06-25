/*******************************************************************************
 *                              USDK(1100)
 *                            Module: common_slider
 *                 Copyright(c) 2003-2008 Actions Semiconductor,
 *                            All Rights Reserved.
 *
 * History:
 *      <author>    <time>           <version >             <desc>
 *        2009-4-22 13:11   1.0             build this file
 *******************************************************************************/
/*!
 * \file     slider.c
 * \brief    概述
 * \author
 * \par      GENERAL DESCRIPTION:
 *               slider控件实现
 * \par      EXTERNALIZED FUNCTIONS:
 *               用到外面的模块
 * \version 1.0
 * \date  2009-4-22
 *******************************************************************************/

#include "slider_private.h"

/******************************************************************************/
/*!
 * \par  Description:
 *	  slider_set_parameter-- 设置传入参数
 * \param[in]       slider_t* slider
 * \param[in]       slider_init_t* parameter
 * \param[out]   none
 * \return       the result
 * \retval
 * \retval
 * \ingroup      common_slider
 * \par          exmaple code      none
 * \code
 *
 * \endcode
 * \note
 *******************************************************************************/
int slider_set_parameter(slider_t* slider, slider_init_t* parameter)
{

    slider->init.total_item = parameter->total_item;
    slider->init.item_perpage = parameter->item_perpage;
    slider->init.page_index = parameter->page_index;
    print_dbg("page_index %d----para %d-----\n", slider->init.page_index, parameter->page_index);

    return 0;
}

/******************************************************************************/
/*!
 * \par  Description:
 *	  slider_proc_gui-- gui消息处理
 * \param[in]       slider_t *slider
 * \param[in]       gui_wm_message_t *pmsg
 * \param[in]
 * \param[out]   none
 * \return       the result
 * \retval
 * \retval
 * \ingroup      common_slider
 * \par          exmaple code   none
 * \code
 *
 * \endcode
 * \note
 *******************************************************************************/
int slider_proc_gui(slider_t *slider, gui_wm_message_t *pmsg)
{
    print_dbg("msgid:%d", pmsg->msgid);
#if 0
    switch (pmsg->msgid)
    {

        default:
        //     	print_dbg("pmsg->msgid:%d out of range", pmsg->msgid);
        break;
    }
#endif
    return 1;
}
/******************************************************************************/
/*!
 * \par  Description:
 *	  slider_switch_type_test-- 测试不同风格切换
 * \param[in]            int type
 * \param[in]           slider_t *slider
 * \param[in]
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
bool slider_switch_type_test(int type, slider_t *slider)
{
    sty_rect_t srect;

    if (NULL == slider)
    {
        return false;
    }
    if (0 == slider->canvas_id)
    {
        return false;
    }

    canvas_delete(slider->canvas_id);
    slider->type = type;
    print_dbg("new type use =%d-----\n", type);
    slider_load_style(slider);

    srect.x     =  slider->data->slider_resgroup->x;
    srect.y     = slider->data->slider_resgroup->y;
    srect.width = slider->data->slider_resgroup->width;
    srect.height= slider->data->slider_resgroup->height;

	
    slider->canvas_id = canvas_create(&srect, TRANSPARENT);
    if ((int) NULL == slider->canvas_id)
    {
        print_err(" ");
        return false;
    }
    return true;
}
/******************************************************************************/
/*!
 * \par  Description:
 *	  slider_load_style-- 装载slider所需要的资源
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
int slider_load_style(slider_t *slider)
{
    slider_private_data_t *private;    
    scene_t* scene;
    sty_rect_t rect;
    unsigned int res_id[W_SB_TYPE_TOTAL] =
    { 0, LIST_RESGROP, OPTION_RESGROP };
    canvas_info_t info;


    if ( slider == NULL )
    {
        print_err("slider is null!");
        return -1;
    }
    else
    {
        private = slider->data;
    }

    scene = sty_get_common_scene();
    if (scene == NULL)
    {
        print_dbg("slider sty_get_common_scene fail\n\n");
        return -1;
    }

    /*
     The ui tree
     ---comm_ui
     |  --commui-scene
     |     --slider_resgroup
     |     --list group
     |        --pic-bg
     |        --pic-cur
     |     --option group
     |        --pic-gb
     |        --........

     */
    print_dbg("scene*p=%d-----\n", scene);

    //load resource group
    slider->data->slider_resgroup = (resgroup_resource_t *) get_scene_child(scene, SLIDER_RESGROUP);
    print_dbg("sb_scenel*p=%d-----\n", slider->data->slider_resgroup);
    if (slider->data->slider_resgroup == NULL)
    {
        return -1;
    }
    if ((slider->type < 1) || (slider->type >= W_SB_TYPE_TOTAL))	
    {
        print_err("slider type err!");
    }
    slider->data->slider_resgroup = (resgroup_resource_t *) get_resgroup_child(slider->data->slider_resgroup,
            res_id[slider->type]);
    if (slider->data->slider_resgroup == NULL)
    {
        return -1;
    }
    print_dbg("sb_scenel*p=%d-----\n", slider->data->slider_resgroup);
    slider->data->pic_bg = (picture_resource_t *) get_resgroup_child(slider->data->slider_resgroup, PIC_BG);
    slider->data->pic_cur = (picture_resource_t *) get_resgroup_child(slider->data->slider_resgroup, PIC_CUR);

    //free bitmap
    sty_delete_bitmap( private->bmp_bg );
    private->bmp_bg = NULL;
    sty_delete_bitmap( private->bmp_cur );
    private->bmp_cur = NULL;
    
    //cache bitmap
    if ( private->pic_bg != NULL )
    {
        private->bmp_bg = sty_create_bitmap( (int)private->pic_bg->pic_id );
    }
    
    if ( private->pic_cur!= NULL )
    {
        private->bmp_cur= sty_create_bitmap( (int)private->pic_cur->pic_id );
    }

    //canvas process    
    if ( slider->extern_canvas == true )
    {
        //extern canvas process   
        canvas_get_info( slider->canvas_id, &info );
        
        //compute offset
        slider->xoffset = slider->data->slider_resgroup->absolute_x - info.rect.x;
        slider->yoffset = slider->data->slider_resgroup->absolute_y - info.rect.y;
    }
    else
    {
        //canvas resize    
        if ( slider->canvas_id != -1 )
        {
            rect.x = slider->data->slider_resgroup->absolute_x;
            rect.y = slider->data->slider_resgroup->absolute_y;
            rect.width = slider->data->slider_resgroup->width;
            rect.height = slider->data->slider_resgroup->height;
            
            //set canvas region
            canvas_set_region( slider->canvas_id, &rect );
        }
    }

    return 0;
}
/******************************************************************************/
/*!
 * \par  Description:
 *	  slider_create-- 创建slider
 * \param[in]              int type
 * \param[in]              slider_cb_t callback
 * \param[in]
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
slider_t* slider_create(int type, slider_cb_t callback, int canvas_id)
{
    sty_rect_t srect;
    slider_t *slider;

    if (0 == type)
    {
        print_err("type err fail\n\n");
        return NULL;
    }
    slider = (slider_t *) malloc(sizeof(slider_t));
    if (NULL == slider)
    {
        print_err("malloc slider_t fail\n\n");
        return NULL;
    }

    slider->data = (struct slider_private_data_s*) malloc(sizeof(struct slider_private_data_s));
    if (NULL == slider->data)
    {
        print_err("malloc failed!");
        free(slider);
        return NULL;
    }

    slider->init.total_item = 0;
    slider->init.page_index = 0;
    slider->init.item_perpage = 0;
    slider->callback = callback;
    if ( canvas_id <= 0 )
    {
        slider->canvas_id   = -1;
        slider->extern_canvas = false;
    }
    else
    {
        slider->canvas_id   = canvas_id;
        slider->extern_canvas = true;
    }
    slider->xoffset = 0;
    slider->yoffset = 0;

    slider->data->bg_pic = 0;
    slider->data->pic_cur = NULL;
    slider->data->bmp_bg = NULL;
    slider->data->bmp_cur = NULL;
    slider->type = type;

    //load style resource
    slider_load_style(slider);

    //create canvas
    //增加转换成绝对坐标接口

    //create canvas必须使用绝对坐标
    if ( slider->canvas_id <= 0 )
    {
        srect.x     = slider->data->slider_resgroup->x;
        srect.y     = slider->data->slider_resgroup->y;
        srect.width = slider->data->slider_resgroup->width;
        srect.height= slider->data->slider_resgroup->height;
        slider->canvas_id = canvas_create(&srect, TRANSPARENT);
        if (slider->canvas_id <= 0)
        {
            free(slider);
            print_err("canvas_create fail\n\n");
            return NULL;
        }
    }
    
    return slider;
}

/******************************************************************************/
/*!
 * \par  Description:
 *	  slider_delete-- 删除slider控件
 * \param[in]       slider_t *slider
 * \param[in]
 * \param[in]
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
bool slider_delete(slider_t *slider)
{
    bool result = true;
    slider_private_data_t *private;

    if (slider == NULL)
    {
        return false;
    }
    else
    {
        private = slider->data;    
    }
    
    if(private != NULL)
    {
        sty_delete_bitmap( private->bmp_bg );
        private->bmp_bg = NULL;
        sty_delete_bitmap( private->bmp_cur);
        private->bmp_cur = NULL;
    }
    
    if( slider->extern_canvas == true )
    {
        slider->canvas_id = -1;
    }
    
    //delete canvas
    if (slider->canvas_id != -1)
    {
        canvas_delete(slider->canvas_id);
        slider->canvas_id = -1;
    }
    else
    {
        result = false;
    }

    free(slider->data);      
    slider->data=NULL;
        
    free(slider);
    slider = (slider_t *) NULL;

    return result;


}

