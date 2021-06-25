/********************************************************************************
*                                    NOYA
*                            Module: libc_general
*                 Copyright(c) 2001-2008 Actions Semiconductor,
*                            All Rights Reserved. 
*
* History:         
*      <author>         <time>           <version >             <desc>
*        wurui     2008-09-12 10:00     1.0             build this file 
********************************************************************************/
/*!
 * 
 * \brief  libc中需要常驻内存的接口的注册
 * \author wurui
 * \par 相关描述: 
 * libc实现中需要常驻内存部分的接口封装
 * \par EXTERNALIZED FUNCTIONS:
 *     Copyright(c) 2001-2008 Actions Semiconductor, All Rights Reserved.
 *
 *  \version 1.0
 *  \date  2008/09/12
 *******************************************************************************/


#include "include_case.h"

static const unsigned int commonui_apitbl[]=
{
    (unsigned int)adjust_proc_gui,
    (unsigned int)adjust_create,
    (unsigned int)adjust_load_style,
    (unsigned int)adjust_delete,
    (unsigned int)adjust_paint,
    (unsigned int)adjust_get_current_val,
    (unsigned int)adjust_get_current_index,
    (unsigned int)adjust_get_switch_status,
    (unsigned int)adjust_get_setting_time,
    (unsigned int)adjust_get_setting_date,
    (unsigned int)adjust_get_recordtime,
    (unsigned int)dialog_load_style,
    (unsigned int)dialog_paint,
    (unsigned int)dialog_proc_gui,
    (unsigned int)dialog_create,
    (unsigned int)dialog_delete,
    
    (unsigned int)dialog_modal,
    (unsigned int)dirlist_create,
    (unsigned int)dirlist_delete,
    (unsigned int)dirlist_paint,
    (unsigned int)dirlist_proc_gui,
    (unsigned int)dirlist_proc_sys,
    (unsigned int)dirlist_load_style,
    (unsigned int)dirlist_show,
    (unsigned int)dirlist_hide,
    (unsigned int)dirlist_get_bitmap_width,
    (unsigned int)dirlist_get_bitmap_height,
    (unsigned int)dirlist_get_item_bitmap,
    (unsigned int)dirlist_set_item_bitmap,
    (unsigned int)dirlist_paint_bitmap,
    (unsigned int)dirlist_set_search_string,
    (unsigned int)dirlist_get_fsel_handle,
    
    (unsigned int)dirlist_get_total,
    (unsigned int)dirlist_get_offset,
    (unsigned int)dirlist_get_current,
    (unsigned int)dirlist_get_item_name,
    (unsigned int)dirlist_get_global_size,
    (unsigned int)dirlist_get_global_offset,
    (unsigned int)headbar_load_style,
    (unsigned int)headbar_paint,
    (unsigned int)headbar_proc_gui,
    (unsigned int)headbar_create,
    (unsigned int)headbar_delete,
    (unsigned int)loadingbox_create,
    (unsigned int)loadingbox_delete,
    (unsigned int)loadingbox_paint,
    (unsigned int)loadingbox_proc_gui,
    (unsigned int)loadingbox_load_style,
    
    (unsigned int)menuitem_create,
    (unsigned int)menuitem_delete,
    (unsigned int)menu_create,
    (unsigned int)menu_delete,
    (unsigned int)menu_set_cur_index,
    (unsigned int)menu_get_cur_index,
    (unsigned int)menu_set_offset,
    (unsigned int)menu_get_offset,
    (unsigned int)menu_get_total,
    (unsigned int)menu_get_cur_item,
    (unsigned int)menu_get_item,
    (unsigned int)menu_set_item,
    (unsigned int)menu_insert_item,
    (unsigned int)menu_prepend_item,
    (unsigned int)menu_append_item,
    (unsigned int)menu_append_items,
    
    (unsigned int)menu_remove_item,
    (unsigned int)menu_remove_first_item,
    (unsigned int)menu_remove_last_item,
    (unsigned int)menu_remove_all_items,
    (unsigned int)menu_set_global_offset,
    (unsigned int)menu_get_global_offset,
    (unsigned int)menu_set_global_size,
    (unsigned int)menu_get_global_size,
    (unsigned int)menuview_create,
    (unsigned int)menuview_delete,
    (unsigned int)menuview_paint,
    (unsigned int)menuview_proc_gui,
    (unsigned int)menuview_load_style,
    (unsigned int)menuview_show,
    (unsigned int)menuview_hide,
    (unsigned int)menuview_set_menu,
    
    (unsigned int)menuview_get_menu,
    (unsigned int)menuview_set_lines,
    (unsigned int)menuview_get_lines,
    (unsigned int)menuview_get_bitmap_width,
    (unsigned int)menuview_get_bitmap_height,
    (unsigned int)menuview_paint_bitmap,
    (unsigned int)msgbox_proc_gui,
    (unsigned int)msgbox_proc_sys,
    (unsigned int)msgbox_create,
    (unsigned int)msgbox_paint,
    (unsigned int)msgbox_delete,
    (unsigned int)msgbox_load_style,
    (unsigned int)slider_set_parameter,
    (unsigned int)slider_proc_gui,
    (unsigned int)slider_create,
    (unsigned int)slider_delete,
    
    (unsigned int)slider_paint,
    (unsigned int)slider_switch_type_test,
    (unsigned int)slider_load_style,
    (unsigned int)softkeyboard_proc_gui,
    (unsigned int)softkeyboard_create,
    (unsigned int)softkeyboard_paint,
    (unsigned int)softkeyboard_delete,
    (unsigned int)softkeyboard_get_text,
    (unsigned int)softkeyboard_set_text,
    (unsigned int)softkeyboard_clear,
    (unsigned int)softkeyboard_load_style,
    (unsigned int)volumebar_proc_gui,
    (unsigned int)volumebar_create,
    (unsigned int)volumebar_delete,
    (unsigned int)volumebar_paint,
    (unsigned int)volumebar_load_style,

    (unsigned int)dialog_modal_load_style,
    (unsigned int)dialog_modal_paint,

    (unsigned int)usb_select_box_create,
    (unsigned int)usb_select_box_delete,
    (unsigned int)usb_select_box_paint,
    (unsigned int)usb_select_box_proc_gui,
    (unsigned int)usb_select_box_load_style,
    
    (unsigned int)dirlist_is_item_dir,

    (unsigned int)progress_box_create,
    (unsigned int)progress_box_delete,
    (unsigned int)progress_box_paint,
    (unsigned int)progress_box_proc_gui,
    (unsigned int)progress_box_load_style,
    (unsigned int)progress_box_set_cur_step,
    (unsigned int)progress_box_set_text,
   
    (unsigned int)headbar_set_title_id,
    (unsigned int)headbar_set_title_buffer,
    
    (unsigned int)menuview_set_prefetch_num,
    (unsigned int)menuview_get_prefetch_num,
    (unsigned int)menuview_get_prefetch_start,    
    
    (unsigned int)headbar_set_time_format,
    (unsigned int)headbar_repaint,
};                
                  

/******************************************************************************/
/*!                    
* \par  Description:
*      init函数
* \note
* 使用宏__init确保函数定位在.ctors段
*******************************************************************************/
int __attribute__((constructor)) commonui_api_init(void)
{
    api_install(APINO_COMMONUI, (unsigned)commonui_apitbl);
    return 0;
}

/******************************************************************************/
/*!                    
* \par  Description:
*      exit函数
* \note
* 使用宏__exit确保函数定位在.dtors段
*******************************************************************************/
void __attribute__((destructor)) commonui_api_exit(void)
{
    api_uninstall(APINO_COMMONUI);
    return ;
}

