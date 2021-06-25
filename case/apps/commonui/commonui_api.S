#include "asm-mips/asm.h"
#include "asm-mips/regdef.h"
#include "ucos/api.h"

//<><><><><><><><><><><><><><><><><><><><><><><><><><>
LEAF(adjust_proc_gui)
	.set noreorder
	.text
adjust_proc_gui:
	li	v1, COMMONUI_API_START+0x00
	syscall
END (adjust_proc_gui)

LEAF(adjust_create)
	.set noreorder
	.text
adjust_create:
	li	v1, COMMONUI_API_START+0x01
	syscall
END (adjust_create)

LEAF(adjust_load_style)
	.set noreorder
	.text
adjust_load_style:
	li	v1, COMMONUI_API_START+0x02
	syscall
END (adjust_load_style)

LEAF(adjust_delete)
	.set noreorder
	.text
adjust_delete:
	li	v1, COMMONUI_API_START+0x03
	syscall
END (adjust_delete)

LEAF(adjust_paint)
	.set noreorder
	.text
adjust_paint:
	li	v1, COMMONUI_API_START+0x04
	syscall
END (adjust_paint)

LEAF(adjust_get_current_val)
	.set noreorder
	.text
adjust_get_current_val:
	li	v1, COMMONUI_API_START+0x05
	syscall
END (adjust_get_current_val)

LEAF(adjust_get_current_index)
	.set noreorder
	.text
adjust_get_current_index:
	li	v1, COMMONUI_API_START+0x06
	syscall
END (adjust_get_current_index)

LEAF(adjust_get_switch_status)
	.set noreorder
	.text
adjust_get_switch_status:
	li	v1, COMMONUI_API_START+0x07
	syscall
END (adjust_get_switch_status)

LEAF(adjust_get_setting_time)
	.set noreorder
	.text
adjust_get_setting_time:
	li	v1, COMMONUI_API_START+0x08
	syscall
END (adjust_get_setting_time)

LEAF(adjust_get_setting_date)
	.set noreorder
	.text
adjust_get_setting_date:
	li	v1, COMMONUI_API_START+0x09
	syscall
END (adjust_get_setting_date)

LEAF(adjust_get_recordtime)
	.set noreorder
	.text
adjust_get_recordtime:
	li	v1, COMMONUI_API_START+0x0a
	syscall
END (adjust_get_recordtime)

LEAF(dialog_load_style)
	.set noreorder
	.text
dialog_load_style:
	li	v1, COMMONUI_API_START+0x0b
	syscall
END (dialog_load_style)

LEAF(dialog_paint)
	.set noreorder
	.text
dialog_paint:
	li	v1, COMMONUI_API_START+0x0c
	syscall
END (dialog_paint)

LEAF(dialog_proc_gui)
	.set noreorder
	.text
dialog_proc_gui:
	li	v1, COMMONUI_API_START+0x0d
	syscall
END (dialog_proc_gui)

LEAF(dialog_create)
	.set noreorder
	.text
dialog_create:
	li	v1, COMMONUI_API_START+0x0e
	syscall
END (dialog_create)

LEAF(dialog_delete)
	.set noreorder
	.text
dialog_delete:
	li	v1, COMMONUI_API_START+0x0f
	syscall
END (dialog_delete)

LEAF(dialog_modal)
	.set noreorder
	.text
dialog_modal:
	li	v1, COMMONUI_API_START+0x10
	syscall
END (dialog_modal)

LEAF(dirlist_create)
	.set noreorder
	.text
dirlist_create:
	li	v1, COMMONUI_API_START+0x11
	syscall
END (dirlist_create)

LEAF(dirlist_delete)
	.set noreorder
	.text
dirlist_delete:
	li	v1, COMMONUI_API_START+0x12
	syscall
END (dirlist_delete)

LEAF(dirlist_paint)
	.set noreorder
	.text
dirlist_paint:
	li	v1, COMMONUI_API_START+0x13
	syscall
END (dirlist_paint)

LEAF(dirlist_proc_gui)
	.set noreorder
	.text
dirlist_proc_gui:
	li	v1, COMMONUI_API_START+0x14
	syscall
END (dirlist_proc_gui)




LEAF(dirlist_proc_sys)
	.set noreorder
	.text
dirlist_proc_sys:
	li	v1, COMMONUI_API_START+0x15
	syscall
END (dirlist_proc_sys)

LEAF(dirlist_load_style)
	.set noreorder
	.text
dirlist_load_style:
	li	v1, COMMONUI_API_START+0x16
	syscall
END (dirlist_load_style)

LEAF(dirlist_show)
	.set noreorder
	.text
dirlist_show:
	li	v1, COMMONUI_API_START+0x17
	syscall
END (dirlist_show)

LEAF(dirlist_hide)
	.set noreorder
	.text
dirlist_hide:
	li	v1, COMMONUI_API_START+0x18
	syscall
END (dirlist_hide)

LEAF(dirlist_get_bitmap_width)
	.set noreorder
	.text
dirlist_get_bitmap_width:
	li	v1, COMMONUI_API_START+0x19
	syscall
END (dirlist_get_bitmap_width)

LEAF(dirlist_get_bitmap_height)
	.set noreorder
	.text
dirlist_get_bitmap_height:
	li	v1, COMMONUI_API_START+0x1a
	syscall
END (dirlist_get_bitmap_height)




LEAF(dirlist_get_item_bitmap)
	.set noreorder
	.text
dirlist_get_item_bitmap:
	li	v1, COMMONUI_API_START+0x1b
	syscall
END (dirlist_get_item_bitmap)

LEAF(dirlist_set_item_bitmap)
	.set noreorder
	.text
dirlist_set_item_bitmap:
	li	v1, COMMONUI_API_START+0x1c
	syscall
END (dirlist_set_item_bitmap)

LEAF(dirlist_paint_bitmap)
	.set noreorder
	.text
dirlist_paint_bitmap:
	li	v1, COMMONUI_API_START+0x1d
	syscall
END (dirlist_paint_bitmap)

LEAF(dirlist_set_search_string)
	.set noreorder
	.text
dirlist_set_search_string:
	li	v1, COMMONUI_API_START+0x1e
	syscall
END (dirlist_set_search_string)

LEAF(dirlist_get_fsel_handle)
	.set noreorder
	.text
dirlist_get_fsel_handle:
	li	v1, COMMONUI_API_START+0x1f
	syscall
END (dirlist_get_fsel_handle)

LEAF(dirlist_get_total)
	.set noreorder
	.text
dirlist_get_total:
	li	v1, COMMONUI_API_START+0x20
	syscall
END (dirlist_get_total)

LEAF(dirlist_get_offset)
	.set noreorder
	.text
dirlist_get_offset:
	li	v1, COMMONUI_API_START+0x21
	syscall
END (dirlist_get_offset)

LEAF(dirlist_get_current)
	.set noreorder
	.text
dirlist_get_current:
	li	v1, COMMONUI_API_START+0x22
	syscall
END (dirlist_get_current)

LEAF(dirlist_get_item_name)
	.set noreorder
	.text
dirlist_get_item_name:
	li	v1, COMMONUI_API_START+0x23
	syscall
END (dirlist_get_item_name)

LEAF(dirlist_get_global_size)
	.set noreorder
	.text
dirlist_get_global_size:
	li	v1, COMMONUI_API_START+0x24
	syscall
END (dirlist_get_global_size)

LEAF(dirlist_get_global_offset)
	.set noreorder
	.text
dirlist_get_global_offset:
	li	v1, COMMONUI_API_START+0x25
	syscall
END (dirlist_get_global_offset)




LEAF(headbar_load_style)
	.set noreorder
	.text
headbar_load_style:
	li	v1, COMMONUI_API_START+0x26
	syscall
END (headbar_load_style)

LEAF(headbar_paint)
	.set noreorder
	.text
headbar_paint:
	li	v1, COMMONUI_API_START+0x27
	syscall
END (headbar_paint)

LEAF(headbar_proc_gui)
	.set noreorder
	.text
headbar_proc_gui:
	li	v1, COMMONUI_API_START+0x28
	syscall
END (headbar_proc_gui)

LEAF(headbar_create)
	.set noreorder
	.text
headbar_create:
	li	v1, COMMONUI_API_START+0x29
	syscall
END (headbar_create)

LEAF(headbar_delete)
	.set noreorder
	.text
headbar_delete:
	li	v1, COMMONUI_API_START+0x2a
	syscall
END (headbar_delete)

LEAF(loadingbox_create)
	.set noreorder
	.text
loadingbox_create:
	li	v1, COMMONUI_API_START+0x2b
	syscall
END (loadingbox_create)

LEAF(loadingbox_delete)
	.set noreorder
	.text
loadingbox_delete:
	li	v1, COMMONUI_API_START+0x2c
	syscall
END (loadingbox_delete)

LEAF(loadingbox_paint)
	.set noreorder
	.text
loadingbox_paint:
	li	v1, COMMONUI_API_START+0x2d
	syscall
END (loadingbox_paint)

LEAF(loadingbox_proc_gui)
	.set noreorder
	.text
loadingbox_proc_gui:
	li	v1, COMMONUI_API_START+0x2e
	syscall
END (loadingbox_proc_gui)

LEAF(loadingbox_load_style)
	.set noreorder
	.text
loadingbox_load_style:
	li	v1, COMMONUI_API_START+0x2f
	syscall                            
END (loadingbox_load_style)                             
                                       
LEAF(menuitem_create)                             
	.set noreorder                     
	.text                              
menuitem_create:                                  
	li	v1, COMMONUI_API_START+0x30
	syscall                            
END (menuitem_create)                             
                                       
LEAF(menuitem_delete)                             
	.set noreorder                     
	.text                              
menuitem_delete:                                  
	li	v1, COMMONUI_API_START+0x31
	syscall                            
END (menuitem_delete)                             
                                       
LEAF(menu_create)                             
	.set noreorder                     
	.text                              
menu_create:                                  
	li	v1, COMMONUI_API_START+0x32
	syscall                            
END (menu_create)                             
                                       
LEAF(menu_delete)                             
	.set noreorder                     
	.text                              
menu_delete:                                  
	li	v1, COMMONUI_API_START+0x33
	syscall                            
END (menu_delete)                             
                                       
LEAF(menu_set_cur_index)                             
	.set noreorder                     
	.text                              
menu_set_cur_index:                                  
	li	v1, COMMONUI_API_START+0x34
	syscall                            
END (menu_set_cur_index)                             
                                       
LEAF(menu_get_cur_index)                             
	.set noreorder                     
	.text                              
menu_get_cur_index:                                  
	li	v1, COMMONUI_API_START+0x35
	syscall                            
END (menu_get_cur_index)                             
                                       
LEAF(menu_set_offset)                             
	.set noreorder                     
	.text                              
menu_set_offset:                                  
	li	v1, COMMONUI_API_START+0x36
	syscall                            
END (menu_set_offset)                             
                                       
LEAF(menu_get_offset)                             
	.set noreorder                     
	.text                              
menu_get_offset:                                  
	li	v1, COMMONUI_API_START+0x37
	syscall                            
END (menu_get_offset)                             
                                       
LEAF(menu_get_total)                             
	.set noreorder                     
	.text                              
menu_get_total:                                  
	li	v1, COMMONUI_API_START+0x38
	syscall                            
END (menu_get_total)                             
                                       


LEAF(menu_get_cur_item)
	.set noreorder
	.text
menu_get_cur_item:
	li	v1, COMMONUI_API_START+0x39
	syscall                            
END (menu_get_cur_item)                             
                                       
LEAF(menu_get_item)                             
	.set noreorder                     
	.text                              
menu_get_item:                                  
	li	v1, COMMONUI_API_START+0x3a
	syscall                            
END (menu_get_item)                             
                                       
LEAF(menu_set_item)                             
	.set noreorder                     
	.text                              
menu_set_item:                                  
	li	v1, COMMONUI_API_START+0x3b
	syscall                            
END (menu_set_item)                             
                                       
LEAF(menu_insert_item)                             
	.set noreorder                     
	.text                              
menu_insert_item:                                  
	li	v1, COMMONUI_API_START+0x3c
	syscall                            
END (menu_insert_item)                             
                                       
LEAF(menu_prepend_item)                             
	.set noreorder                     
	.text                              
menu_prepend_item:                                  
	li	v1, COMMONUI_API_START+0x3d
	syscall                            
END (menu_prepend_item)                             
                                       
LEAF(menu_append_item)                             
	.set noreorder                     
	.text                              
menu_append_item:                                  
	li	v1, COMMONUI_API_START+0x3e
	syscall                            
END (menu_append_item)                             
                                       
LEAF(menu_append_items)                             
	.set noreorder                     
	.text                              
menu_append_items:                                  
	li	v1, COMMONUI_API_START+0x3f
	syscall                            
END (menu_append_items)                             
                                       
LEAF(menu_remove_item)                             
	.set noreorder                     
	.text                              
menu_remove_item:                                  
	li	v1, COMMONUI_API_START+0x40
	syscall                            
END (menu_remove_item)                             
                                       
LEAF(menu_remove_first_item)                             
	.set noreorder                     
	.text                              
menu_remove_first_item:                                  
	li	v1, COMMONUI_API_START+0x41
	syscall                            
END (menu_remove_first_item)                             
                                       
LEAF(menu_remove_last_item)                             
	.set noreorder                     
	.text                              
menu_remove_last_item:                                  
	li	v1, COMMONUI_API_START+0x42
	syscall                            
END (menu_remove_last_item)                             
                                       
LEAF(menu_remove_all_items)                             
	.set noreorder                     
	.text                              
menu_remove_all_items:                                  
	li	v1, COMMONUI_API_START+0x43
	syscall                            
END (menu_remove_all_items)                             
                                       
       

LEAF(menu_set_global_offset)              
	.set noreorder                
	.text                         
menu_set_global_offset:                   
	li	v1, COMMONUI_API_START+0x44
	syscall                            
END (menu_set_global_offset)                   
                                       
LEAF(menu_get_global_offset)              
	.set noreorder                
	.text                         
menu_get_global_offset:                   
	li	v1, COMMONUI_API_START+0x45
	syscall                            
END (menu_get_global_offset)                   
                                       
LEAF(menu_set_global_size)              
	.set noreorder                
	.text                         
menu_set_global_size:                   
	li	v1, COMMONUI_API_START+0x46
	syscall                            
END (menu_set_global_size)                   
                                       
LEAF(menu_get_global_size)              
	.set noreorder                
	.text                         
menu_get_global_size:                   
	li	v1, COMMONUI_API_START+0x47
	syscall                            
END (menu_get_global_size)                   
                                       
LEAF(menuview_create)                     
	.set noreorder                     
	.text                              
menuview_create:                          
	li	v1, COMMONUI_API_START+0x48
	syscall                            
END (menuview_create)                     
                                       
LEAF(menuview_delete)                    
	.set noreorder                     
	.text                              
menuview_delete:                         
	li	v1, COMMONUI_API_START+0x49
	syscall                            
END (menuview_delete)                    
                                       
LEAF(menuview_paint)                     
	.set noreorder                     
	.text                              
menuview_paint:                          
	li	v1, COMMONUI_API_START+0x4a
	syscall                            
END (menuview_paint)                     
                                       
LEAF(menuview_proc_gui)                     
	.set noreorder                     
	.text                              
menuview_proc_gui:                          
	li	v1, COMMONUI_API_START+0x4b
	syscall                            
END (menuview_proc_gui)                     
                                       
LEAF(menuview_load_style)                             
	.set noreorder                     
	.text                              
menuview_load_style:                                  
	li	v1, COMMONUI_API_START+0x4c
	syscall                            
END (menuview_load_style)                             
                                       
LEAF(menuview_show)                             
	.set noreorder                     
	.text                              
menuview_show:                                  
	li	v1, COMMONUI_API_START+0x4d
	syscall                            
END (menuview_show)                             
                                       
LEAF(menuview_hide)                             
	.set noreorder                     
	.text                              
menuview_hide:                                  
	li	v1, COMMONUI_API_START+0x4e
	syscall                            
END (menuview_hide)                             
                                       
LEAF(menuview_set_menu)                             
	.set noreorder                     
	.text                              
menuview_set_menu:                                  
	li	v1, COMMONUI_API_START+0x4f
	syscall                            
END (menuview_set_menu)                             
                                       
LEAF(menuview_get_menu)                             
	.set noreorder                     
	.text                              
menuview_get_menu:                                  
	li	v1, COMMONUI_API_START+0x50
	syscall                            
END (menuview_get_menu)                             
                                       
LEAF(menuview_set_lines)                             
	.set noreorder                     
	.text                              
menuview_set_lines:                                  
	li	v1, COMMONUI_API_START+0x51
	syscall                            
END (menuview_set_lines)                             
                                       
LEAF(menuview_get_lines)                             
	.set noreorder                     
	.text                              
menuview_get_lines:                                  
	li	v1, COMMONUI_API_START+0x52
	syscall                            
END (menuview_get_lines)                             
                                       
LEAF(menuview_get_bitmap_width)                             
	.set noreorder                     
	.text                              
menuview_get_bitmap_width:                                  
	li	v1, COMMONUI_API_START+0x53
	syscall                            
END (menuview_get_bitmap_width)                             
                                       
LEAF(menuview_get_bitmap_height)                             
	.set noreorder                     
	.text                              
menuview_get_bitmap_height:                                  
	li	v1, COMMONUI_API_START+0x54
	syscall                            
END (menuview_get_bitmap_height)                             
                                       
LEAF(menuview_paint_bitmap)                             
	.set noreorder                     
	.text                              
menuview_paint_bitmap:                                  
	li	v1, COMMONUI_API_START+0x55
	syscall                            
END (menuview_paint_bitmap)                             
                                       
LEAF(msgbox_proc_gui)                             
	.set noreorder                     
	.text                              
msgbox_proc_gui:                                  
	li	v1, COMMONUI_API_START+0x56
	syscall
END (msgbox_proc_gui)

LEAF(msgbox_proc_sys)
	.set noreorder
	.text
msgbox_proc_sys:
	li	v1, COMMONUI_API_START+0x57
	syscall                            
END (msgbox_proc_sys)                             
                                       
LEAF(msgbox_create)                             
	.set noreorder                     
	.text                              
msgbox_create:                                  
	li	v1, COMMONUI_API_START+0x58
	syscall                            
END (msgbox_create)                             
                                       
LEAF(msgbox_paint)                             
	.set noreorder                     
	.text                              
msgbox_paint:                                  
	li	v1, COMMONUI_API_START+0x59
	syscall                            
END (msgbox_paint)                             
                                       
LEAF(msgbox_delete)                             
	.set noreorder                     
	.text                              
msgbox_delete:                                  
	li	v1, COMMONUI_API_START+0x5a
	syscall                            
END (msgbox_delete)                             
                                       
LEAF(msgbox_load_style)                             
	.set noreorder                     
	.text                              
msgbox_load_style:                                  
	li	v1, COMMONUI_API_START+0x5b
	syscall                            
END (msgbox_load_style)                             
                                       
LEAF(slider_set_parameter)                             
	.set noreorder                     
	.text                              
slider_set_parameter:                                  
	li	v1, COMMONUI_API_START+0x5c
	syscall                            
END (slider_set_parameter)                             
                                       
LEAF(slider_proc_gui)                             
	.set noreorder                     
	.text                              
slider_proc_gui:                                  
	li	v1, COMMONUI_API_START+0x5d
	syscall                            
END (slider_proc_gui)                             
                                       
LEAF(slider_create)                             
	.set noreorder                     
	.text                              
slider_create:                                  
	li	v1, COMMONUI_API_START+0x5e
	syscall                            
END (slider_create)                             
                                       
LEAF(slider_delete)                             
	.set noreorder                     
	.text                              
slider_delete:                                  
	li	v1, COMMONUI_API_START+0x5f
	syscall                            
END (slider_delete)                             
                                       
LEAF(slider_paint)                             
	.set noreorder                     
	.text                              
slider_paint:                                  
	li	v1, COMMONUI_API_START+0x60
	syscall                            
END (slider_paint)                             
                                       
LEAF(slider_switch_type_test)                             
	.set noreorder                     
	.text                              
slider_switch_type_test:                                  
	li	v1, COMMONUI_API_START+0x61
	syscall                            
END (slider_switch_type_test)                             
                                       
LEAF(slider_load_style)                             
	.set noreorder                     
	.text                              
slider_load_style:                                  
	li	v1, COMMONUI_API_START+0x62
	syscall                            
END (slider_load_style)                             
                                       
LEAF(softkeyboard_proc_gui)                             
	.set noreorder                     
	.text                              
softkeyboard_proc_gui:                                  
	li	v1, COMMONUI_API_START+0x63
	syscall                            
END (softkeyboard_proc_gui)                             
                                       
LEAF(softkeyboard_create)                             
	.set noreorder                     
	.text                              
softkeyboard_create:                                  
	li	v1, COMMONUI_API_START+0x64
	syscall                            
END (softkeyboard_create)                             
                                       
LEAF(softkeyboard_paint)                             
	.set noreorder                     
	.text                              
softkeyboard_paint:                                  
	li	v1, COMMONUI_API_START+0x65
	syscall                            
END (softkeyboard_paint)                             
                                       
LEAF(softkeyboard_delete)                             
	.set noreorder                     
	.text                              
softkeyboard_delete:                                  
	li	v1, COMMONUI_API_START+0x66
	syscall
END (softkeyboard_delete)

LEAF(softkeyboard_get_text)
	.set noreorder
	.text
softkeyboard_get_text:
	li	v1, COMMONUI_API_START+0x67
	syscall                            
END (softkeyboard_get_text)                             
                                       
LEAF(softkeyboard_set_text)                             
	.set noreorder                     
	.text                              
softkeyboard_set_text:                                  
	li	v1, COMMONUI_API_START+0x68
	syscall                            
END (softkeyboard_set_text)                             
                                       
LEAF(softkeyboard_clear)                             
	.set noreorder                     
	.text                              
softkeyboard_clear:                                  
	li	v1, COMMONUI_API_START+0x69
	syscall                            
END (softkeyboard_clear)                             
                                       
LEAF(softkeyboard_load_style)                             
	.set noreorder                     
	.text                              
softkeyboard_load_style:                                  
	li	v1, COMMONUI_API_START+0x6a
	syscall                            
END (softkeyboard_load_style)                             
                                       
LEAF(volumebar_proc_gui)                             
	.set noreorder                     
	.text                              
volumebar_proc_gui:                                  
	li	v1, COMMONUI_API_START+0x6b
	syscall                            
END (volumebar_proc_gui)                             
                                       
LEAF(volumebar_create)                             
	.set noreorder                     
	.text                              
volumebar_create:                                  
	li	v1, COMMONUI_API_START+0x6c
	syscall                            
END (volumebar_create)

LEAF(volumebar_delete)                             
	.set noreorder                     
	.text                              
volumebar_delete:                                  
	li	v1, COMMONUI_API_START+0x6d
	syscall                            
END (volumebar_delete)                             

LEAF(volumebar_paint)                             
	.set noreorder                     
	.text                              
volumebar_paint:                                  
	li	v1, COMMONUI_API_START+0x6e
	syscall                            
END (volumebar_paint)                             

LEAF(volumebar_load_style)                             
	.set noreorder                     
	.text                              
volumebar_load_style:                                  
	li	v1, COMMONUI_API_START+0x6f
	syscall                            
END (volumebar_load_style)   

LEAF(dialog_modal_load_style)                             
	.set noreorder                     
	.text                              
dialog_modal_load_style:                                  
	li	v1, COMMONUI_API_START+0x70
	syscall                            
END (dialog_modal_load_style)   

LEAF(dialog_modal_paint)                             
	.set noreorder                     
	.text                              
dialog_modal_paint:                                  
	li	v1, COMMONUI_API_START+0x71
	syscall                            
END (dialog_modal_paint)   

LEAF(usb_select_box_create)
	.set noreorder
	.text
usb_select_box_create:
	li	v1, COMMONUI_API_START+0x72
	syscall
END (usb_select_box_create)

LEAF(usb_select_box_delete)
	.set noreorder
	.text
usb_select_box_delete:
	li	v1, COMMONUI_API_START+0x73
	syscall
END (usb_select_box_delete)

LEAF(usb_select_box_paint)
	.set noreorder
	.text
usb_select_box_paint:
	li	v1, COMMONUI_API_START+0x74
	syscall
END (usb_select_box_paint)

LEAF(usb_select_box_proc_gui)
	.set noreorder
	.text
usb_select_box_proc_gui:
	li	v1, COMMONUI_API_START+0x75
	syscall
END (usb_select_box_proc_gui)

LEAF(usb_select_box_load_style)
	.set noreorder
	.text
usb_select_box_load_style:
	li	v1, COMMONUI_API_START+0x76
	syscall                            
END (usb_select_box_load_style)                             

LEAF(dirlist_is_item_dir)
	.set noreorder
	.text
dirlist_is_item_dir:
	li	v1, COMMONUI_API_START+0x77
	syscall                            
END (dirlist_is_item_dir)                             

LEAF(progress_box_create)
	.set noreorder
	.text
progress_box_create:
	li	v1, COMMONUI_API_START+0x78
	syscall                            
END (progress_box_create)                             

LEAF(progress_box_delete)
	.set noreorder
	.text
progress_box_delete:
	li	v1, COMMONUI_API_START+0x79
	syscall                            
END (progress_box_delete)                             

LEAF(progress_box_paint)
	.set noreorder
	.text
progress_box_paint:
	li	v1, COMMONUI_API_START+0x7a
	syscall                            
END (progress_box_paint)                             

LEAF(progress_box_proc_gui)
	.set noreorder
	.text
progress_box_proc_gui:
	li	v1, COMMONUI_API_START+0x7b
	syscall                            
END (progress_box_proc_gui)                             

LEAF(progress_box_load_style)
	.set noreorder
	.text
progress_box_load_style:
	li	v1, COMMONUI_API_START+0x7c
	syscall                            
END (progress_box_load_style)                             

LEAF(progress_box_set_cur_step)
	.set noreorder
	.text
progress_box_set_cur_step:
	li	v1, COMMONUI_API_START+0x7d
	syscall                            
END (progress_box_set_cur_step)                             

LEAF(progress_box_set_text)
	.set noreorder
	.text
progress_box_set_text:
	li	v1, COMMONUI_API_START+0x7e
	syscall                            
END (progress_box_set_text)

LEAF(headbar_set_title_id)
	.set noreorder
	.text
headbar_set_title_id:
	li	v1, COMMONUI_API_START+0x7f
	syscall
END (headbar_set_title_id)

LEAF(headbar_set_title_buffer)
	.set noreorder
	.text
headbar_set_title_buffer:
	li	v1, COMMONUI_API_START+0x80
	syscall
END (headbar_set_title_buffer)

LEAF(menuview_set_prefetch_num)
	.set noreorder
	.text
menuview_set_prefetch_num:
	li	v1, COMMONUI_API_START+0x81
	syscall
END (menuview_set_prefetch_num)

LEAF(menuview_get_prefetch_num)
	.set noreorder
	.text
menuview_get_prefetch_num:
	li	v1, COMMONUI_API_START+0x82
	syscall
END (menuview_get_prefetch_num)

LEAF(menuview_get_prefetch_start)
	.set noreorder
	.text
menuview_get_prefetch_start:
	li	v1, COMMONUI_API_START+0x83
	syscall
END (menuview_get_prefetch_start)

LEAF(headbar_set_time_format)
	.set noreorder
	.text
headbar_set_time_format:
	li	v1, COMMONUI_API_START+0x84
	syscall
END (headbar_set_time_format)

LEAF(headbar_repaint)
	.set noreorder
	.text
headbar_repaint:
	li	v1, COMMONUI_API_START+0x85
	syscall
END (headbar_repaint)

