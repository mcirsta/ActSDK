#include "asm-mips/asm.h"
#include "asm-mips/regdef.h"
#include "ucos/api.h"

//<><><><><><><><><><><><><><><><><><><><><><><><><><>
LEAF(app_running_list_init)
	.set noreorder
	.text
app_running_list_init:
	li	v1, APPLIB_API_START+0x00
	syscall
END (app_running_list_init)

LEAF(app_running_list_deinit)
	.set noreorder
	.text
app_running_list_deinit:
	li	v1, APPLIB_API_START+0x01
	syscall
END (app_running_list_deinit)

LEAF(is_app_exist)
	.set noreorder
	.text
is_app_exist:
	li	v1, APPLIB_API_START+0x02
	syscall
END (is_app_exist)

LEAF(get_app_type)
	.set noreorder
	.text
get_app_type:
	li	v1, APPLIB_API_START+0x03
	syscall
END (get_app_type)

LEAF(get_app_pid)
	.set noreorder
	.text
get_app_pid:
	li	v1, APPLIB_API_START+0x04
	syscall
END (get_app_pid)

LEAF(get_app_name)
	.set noreorder
	.text
get_app_name:
	li	v1, APPLIB_API_START+0x05
	syscall
END (get_app_name)

LEAF(app_running_list_get_app_info)
	.set noreorder
	.text
app_running_list_get_app_info:
	li	v1, APPLIB_API_START+0x06
	syscall
END (app_running_list_get_app_info)

LEAF(app_running_list_get_this_app_info)
	.set noreorder
	.text
app_running_list_get_this_app_info:
	li	v1, APPLIB_API_START+0x07
	syscall
END (app_running_list_get_this_app_info)

LEAF(app_running_list_set_app_info)
	.set noreorder
	.text
app_running_list_set_app_info:
	li	v1, APPLIB_API_START+0x08
	syscall
END (app_running_list_set_app_info)

LEAF(app_running_list_set_this_app_info)
	.set noreorder
	.text
app_running_list_set_this_app_info:
	li	v1, APPLIB_API_START+0x09
	syscall
END (app_running_list_set_this_app_info)

LEAF(app_running_list_get_list)
	.set noreorder
	.text
app_running_list_get_list:
	li	v1, APPLIB_API_START+0x0a
	syscall
END (app_running_list_get_list)

LEAF(app_running_list_add)
	.set noreorder
	.text
app_running_list_add:
	li	v1, APPLIB_API_START+0x0b
	syscall
END (app_running_list_add)

LEAF(app_running_list_delete)
	.set noreorder
	.text
app_running_list_delete:
	li	v1, APPLIB_API_START+0x0c
	syscall
END (app_running_list_delete)

LEAF(print_app_running_list)
	.set noreorder
	.text
print_app_running_list:
	li	v1, APPLIB_API_START+0x0d
	syscall
END (print_app_running_list)

LEAF(set_timer)
	.set noreorder
	.text
set_timer:
	li	v1, APPLIB_API_START+0x0f
	syscall
END (set_timer)

LEAF(set_single_shot_timer)
	.set noreorder
	.text
set_single_shot_timer:
	li	v1, APPLIB_API_START+0x10
	syscall
END (set_single_shot_timer)

LEAF(modify_timer)
	.set noreorder
	.text
modify_timer:
	li	v1, APPLIB_API_START+0x11
	syscall
END (modify_timer)

LEAF(stop_timer)
	.set noreorder
	.text
stop_timer:
	li	v1, APPLIB_API_START+0x12
	syscall
END (stop_timer)

LEAF(restart_timer)
	.set noreorder
	.text
restart_timer:
	li	v1, APPLIB_API_START+0x13
	syscall
END (restart_timer)

LEAF(kill_timer)
	.set noreorder
	.text
kill_timer:
	li	v1, APPLIB_API_START+0x14
	syscall
END (kill_timer)

LEAF(kill_timers)
	.set noreorder
	.text
kill_timers:
	li	v1, APPLIB_API_START+0x15
	syscall
END (kill_timers)

LEAF(set_inner_timer)
	.set noreorder
	.text
set_inner_timer:
	li	v1, APPLIB_API_START+0x16
	syscall
END (set_inner_timer)

LEAF(set_inner_single_shot_timer)
	.set noreorder
	.text
set_inner_single_shot_timer:
	li	v1, APPLIB_API_START+0x17
	syscall
END (set_inner_single_shot_timer)

LEAF(modify_inner_timer)
	.set noreorder
	.text
modify_inner_timer:
	li	v1, APPLIB_API_START+0x18
	syscall
END (modify_inner_timer)

LEAF(stop_inner_timer)
	.set noreorder
	.text
stop_inner_timer:
	li	v1, APPLIB_API_START+0x19
	syscall
END (stop_inner_timer)

LEAF(restart_inner_timer)
	.set noreorder
	.text
restart_inner_timer:
	li	v1, APPLIB_API_START+0x1a
	syscall
END (restart_inner_timer)




LEAF(kill_inner_timer)
	.set noreorder
	.text
kill_inner_timer:
	li	v1, APPLIB_API_START+0x1b
	syscall
END (kill_inner_timer)

LEAF(kill_inner_timers)
	.set noreorder
	.text
kill_inner_timers:
	li	v1, APPLIB_API_START+0x1c
	syscall
END (kill_inner_timers)

LEAF(delete_timers)
	.set noreorder
	.text
delete_timers:
	li	v1, APPLIB_API_START+0x1d
	syscall
END (delete_timers)

LEAF(delete_inner_timers)
	.set noreorder
	.text
delete_inner_timers:
	li	v1, APPLIB_API_START+0x1e
	syscall
END (delete_inner_timers)

LEAF(handle_timers)
	.set noreorder
	.text
handle_timers:
	li	v1, APPLIB_API_START+0x1f
	syscall
END (handle_timers)

LEAF(get_tick_time_ms)
	.set noreorder
	.text
get_tick_time_ms:
	li	v1, APPLIB_API_START+0x20
	syscall
END (get_tick_time_ms)

LEAF(set_user_bg_pic)
	.set noreorder
	.text
set_user_bg_pic:
	li	v1, APPLIB_API_START+0x21
	syscall
END (set_user_bg_pic)

LEAF(restore_default_bg_pic)
	.set noreorder
	.text
restore_default_bg_pic:
	li	v1, APPLIB_API_START+0x22
	syscall
END (restore_default_bg_pic)

LEAF(get_user_bg_pic)
	.set noreorder
	.text
get_user_bg_pic:
	li	v1, APPLIB_API_START+0x23
	syscall
END (get_user_bg_pic)

LEAF(canvas_create)
	.set noreorder
	.text
canvas_create:
	li	v1, APPLIB_API_START+0x24
	syscall
END (canvas_create)

LEAF(canvas_delete)
	.set noreorder
	.text
canvas_delete:
	li	v1, APPLIB_API_START+0x25
	syscall
END (canvas_delete)




LEAF(canvas_show)
	.set noreorder
	.text
canvas_show:
	li	v1, APPLIB_API_START+0x26
	syscall
END (canvas_show)

LEAF(canvas_hide)
	.set noreorder
	.text
canvas_hide:
	li	v1, APPLIB_API_START+0x27
	syscall
END (canvas_hide)

LEAF(canvas_set_region)
	.set noreorder
	.text
canvas_set_region:
	li	v1, APPLIB_API_START+0x28
	syscall
END (canvas_set_region)

LEAF(canvas_erase)
	.set noreorder
	.text
canvas_erase:
	li	v1, APPLIB_API_START+0x29
	syscall
END (canvas_erase)

LEAF(canvas_erase_all)
	.set noreorder
	.text
canvas_erase_all:
	li	v1, APPLIB_API_START+0x2a
	syscall
END (canvas_erase_all)

LEAF(canvas_set_alpha)
	.set noreorder
	.text
canvas_set_alpha:
	li	v1, APPLIB_API_START+0x2b
	syscall
END (canvas_set_alpha)

LEAF(canvas_get_top)
	.set noreorder
	.text
canvas_get_top:
	li	v1, APPLIB_API_START+0x2c
	syscall
END (canvas_get_top)

LEAF(canvas_set_top)
	.set noreorder
	.text
canvas_set_top:
	li	v1, APPLIB_API_START+0x2d
	syscall
END (canvas_set_top)

LEAF(canvas_get_info)
	.set noreorder
	.text
canvas_get_info:
	li	v1, APPLIB_API_START+0x2e
	syscall
END (canvas_get_info)



LEAF(canvas_get_blending_top)
	.set noreorder
	.text
canvas_get_blending_top:
	li	v1, APPLIB_API_START+0x2f
	syscall
END (canvas_get_blending_top) 
 
LEAF(canvas_set_blending_mode) 
	.set noreorder 
	.text
canvas_set_blending_mode:
	li	v1, APPLIB_API_START+0x30
	syscall
END (canvas_set_blending_mode) 
 
LEAF(desktop_parser_create) 
	.set noreorder 
	.text
desktop_parser_create:
	li	v1, APPLIB_API_START+0x31
	syscall
END (desktop_parser_create) 
 
LEAF(desktop_parser_get_key) 
	.set noreorder 
	.text
desktop_parser_get_key:
	li	v1, APPLIB_API_START+0x32
	syscall
END (desktop_parser_get_key) 
 
LEAF(desktop_parser_delete) 
	.set noreorder 
	.text
desktop_parser_delete:
	li	v1, APPLIB_API_START+0x33
	syscall
END (desktop_parser_delete) 
 
LEAF(applib_register_app) 
	.set noreorder 
	.text
applib_register_app:
	li	v1, APPLIB_API_START+0x34
	syscall
END (applib_register_app) 
 
LEAF(applib_unregister_app) 
	.set noreorder 
	.text
applib_unregister_app:
	li	v1, APPLIB_API_START+0x35
	syscall
END (applib_unregister_app) 
 
LEAF(applib_init) 
	.set noreorder 
	.text
applib_init:
	li	v1, APPLIB_API_START+0x36
	syscall
END (applib_init) 
 
LEAF(applib_quit) 
	.set noreorder 
	.text
applib_quit:
	li	v1, APPLIB_API_START+0x37
	syscall
END (applib_quit) 
 
LEAF(get_msg) 
	.set noreorder 
	.text
get_msg:
	li	v1, APPLIB_API_START+0x38
	syscall
END (get_msg) 
 


LEAF(send_sync_msg)
	.set noreorder
	.text
send_sync_msg:
	li	v1, APPLIB_API_START+0x39
	syscall
END (send_sync_msg) 
 
LEAF(reply_sync_msg) 
	.set noreorder 
	.text
reply_sync_msg:
	li	v1, APPLIB_API_START+0x3a
	syscall
END (reply_sync_msg) 
 
LEAF(send_async_msg) 
	.set noreorder 
	.text
send_async_msg:
	li	v1, APPLIB_API_START+0x3b
	syscall
END (send_async_msg) 
 
LEAF(broadcast_msg) 
	.set noreorder 
	.text
broadcast_msg:
	li	v1, APPLIB_API_START+0x3c
	syscall
END (broadcast_msg) 
 
LEAF(dispatch_msg) 
	.set noreorder 
	.text
dispatch_msg:
	li	v1, APPLIB_API_START+0x3d
	syscall
END (dispatch_msg) 
 
LEAF(register_sys_dispatcher) 
	.set noreorder 
	.text
register_sys_dispatcher:
	li	v1, APPLIB_API_START+0x3e
	syscall
END (register_sys_dispatcher) 
 
LEAF(unregister_sys_dispatcher) 
	.set noreorder 
	.text
unregister_sys_dispatcher:
	li	v1, APPLIB_API_START+0x3f
	syscall
END (unregister_sys_dispatcher) 
 
LEAF(exit_msg_loop) 
	.set noreorder 
	.text
exit_msg_loop:
	li	v1, APPLIB_API_START+0x40
	syscall
END (exit_msg_loop) 
 
LEAF(clear_sys_msg_queue) 
	.set noreorder 
	.text
clear_sys_msg_queue:
	li	v1, APPLIB_API_START+0x41
	syscall
END (clear_sys_msg_queue) 
 
LEAF(clear_key_msg_queue) 
	.set noreorder 
	.text
clear_key_msg_queue:
	li	v1, APPLIB_API_START+0x42
	syscall
END (clear_key_msg_queue) 
 
LEAF(enter_background_mode) 
	.set noreorder 
	.text
enter_background_mode:
	li	v1, APPLIB_API_START+0x43
	syscall
END (enter_background_mode) 
 
 

LEAF(enter_foreground_mode)
	.set noreorder
	.text 
enter_foreground_mode: 
	li	v1, APPLIB_API_START+0x44
	syscall
END (enter_foreground_mode) 
 
LEAF(set_soft_watchdog_timeout)
	.set noreorder
	.text 
set_soft_watchdog_timeout: 
	li	v1, APPLIB_API_START+0x45
	syscall
END (set_soft_watchdog_timeout) 
 
LEAF(check_soft_watchdogs)
	.set noreorder
	.text 
check_soft_watchdogs: 
	li	v1, APPLIB_API_START+0x46
	syscall
END (check_soft_watchdogs) 
 
LEAF(sty_open_common_pic_res)
	.set noreorder
	.text 
sty_open_common_pic_res: 
	li	v1, APPLIB_API_START+0x47
	syscall
END (sty_open_common_pic_res) 
 
LEAF(sty_open_common_str_res) 
	.set noreorder 
	.text
sty_open_common_str_res:
	li	v1, APPLIB_API_START+0x48
	syscall
END (sty_open_common_str_res) 
 
LEAF(sty_close_common_pic_res)
	.set noreorder 
	.text
sty_close_common_pic_res: 
	li	v1, APPLIB_API_START+0x49
	syscall
END (sty_close_common_pic_res)
 
LEAF(sty_close_common_str_res) 
	.set noreorder 
	.text
sty_close_common_str_res:
	li	v1, APPLIB_API_START+0x4a
	syscall
END (sty_close_common_str_res) 
 
LEAF(sty_open_app_pic_res) 
	.set noreorder 
	.text
sty_open_app_pic_res:
	li	v1, APPLIB_API_START+0x4b
	syscall
END (sty_open_app_pic_res) 
 
LEAF(sty_open_app_str_res) 
	.set noreorder 
	.text
sty_open_app_str_res:
	li	v1, APPLIB_API_START+0x4c
	syscall
END (sty_open_app_str_res) 
 
LEAF(sty_close_app_pic_res) 
	.set noreorder 
	.text
sty_close_app_pic_res:
	li	v1, APPLIB_API_START+0x4d
	syscall
END (sty_close_app_pic_res) 
 
LEAF(sty_close_app_str_res) 
	.set noreorder 
	.text
sty_close_app_str_res:
	li	v1, APPLIB_API_START+0x4e
	syscall
END (sty_close_app_str_res) 
 
LEAF(sty_set_app_scene) 
	.set noreorder 
	.text
sty_set_app_scene:
	li	v1, APPLIB_API_START+0x4f
	syscall
END (sty_set_app_scene) 
 
LEAF(sty_get_app_scene) 
	.set noreorder 
	.text
sty_get_app_scene:
	li	v1, APPLIB_API_START+0x50
	syscall
END (sty_get_app_scene) 
 
LEAF(sty_set_common_scene) 
	.set noreorder 
	.text
sty_set_common_scene:
	li	v1, APPLIB_API_START+0x51
	syscall
END (sty_set_common_scene) 
 
LEAF(sty_get_common_scene) 
	.set noreorder 
	.text
sty_get_common_scene:
	li	v1, APPLIB_API_START+0x52
	syscall
END (sty_get_common_scene) 
 
LEAF(sty_set_text_encoding) 
	.set noreorder 
	.text
sty_set_text_encoding:
	li	v1, APPLIB_API_START+0x53
	syscall
END (sty_set_text_encoding) 
 
LEAF(sty_get_text_encoding) 
	.set noreorder 
	.text
sty_get_text_encoding:
	li	v1, APPLIB_API_START+0x54
	syscall
END (sty_get_text_encoding) 
 
LEAF(sty_get_string) 
	.set noreorder 
	.text
sty_get_string:
	li	v1, APPLIB_API_START+0x55
	syscall
END (sty_get_string) 
 
LEAF(sty_truncate_string_by_width) 
	.set noreorder 
	.text
sty_truncate_string_by_width:
	li	v1, APPLIB_API_START+0x56
	syscall
END (sty_truncate_string_by_width)

LEAF(sty_set_string_format)
	.set noreorder
	.text
sty_set_string_format:
	li	v1, APPLIB_API_START+0x57
	syscall
END (sty_set_string_format) 
 
LEAF(sty_restore_string_format) 
	.set noreorder 
	.text
sty_restore_string_format:
	li	v1, APPLIB_API_START+0x58
	syscall
END (sty_restore_string_format) 
 
LEAF(sty_draw_string_buf) 
	.set noreorder 
	.text
sty_draw_string_buf:
	li	v1, APPLIB_API_START+0x59
	syscall
END (sty_draw_string_buf) 
 
LEAF(sty_draw_string_id) 
	.set noreorder 
	.text
sty_draw_string_id:
	li	v1, APPLIB_API_START+0x5a
	syscall
END (sty_draw_string_id) 
 
LEAF(sty_draw_string_res) 
	.set noreorder 
	.text
sty_draw_string_res:
	li	v1, APPLIB_API_START+0x5b
	syscall
END (sty_draw_string_res) 
 
LEAF(sty_draw_string_res_ext) 
	.set noreorder 
	.text
sty_draw_string_res_ext:
	li	v1, APPLIB_API_START+0x5c
	syscall
END (sty_draw_string_res_ext) 
 
LEAF(sty_create_scroll_string) 
	.set noreorder 
	.text
sty_create_scroll_string:
	li	v1, APPLIB_API_START+0x5d
	syscall
END (sty_create_scroll_string) 
 
LEAF(sty_draw_scroll_string) 
	.set noreorder 
	.text
sty_draw_scroll_string:
	li	v1, APPLIB_API_START+0x5e
	syscall
END (sty_draw_scroll_string) 
 
LEAF(sty_delete_scroll_string) 
	.set noreorder 
	.text
sty_delete_scroll_string:
	li	v1, APPLIB_API_START+0x5f
	syscall
END (sty_delete_scroll_string) 
 
LEAF(sty_draw_picture_buf) 
	.set noreorder 
	.text
sty_draw_picture_buf:
	li	v1, APPLIB_API_START+0x60
	syscall
END (sty_draw_picture_buf) 
 
LEAF(sty_draw_picture_id) 
	.set noreorder 
	.text
sty_draw_picture_id:
	li	v1, APPLIB_API_START+0x61
	syscall
END (sty_draw_picture_id) 
 
LEAF(sty_draw_picture_res) 
	.set noreorder 
	.text
sty_draw_picture_res:
	li	v1, APPLIB_API_START+0x62
	syscall
END (sty_draw_picture_res) 
 
LEAF(sty_draw_picregion_res) 
	.set noreorder 
	.text
sty_draw_picregion_res:
	li	v1, APPLIB_API_START+0x63
	syscall
END (sty_draw_picregion_res) 
 
LEAF(sty_draw_sub_picture_buf) 
	.set noreorder 
	.text
sty_draw_sub_picture_buf:
	li	v1, APPLIB_API_START+0x64
	syscall
END (sty_draw_sub_picture_buf) 
 
LEAF(sty_draw_sub_picture_id) 
	.set noreorder 
	.text
sty_draw_sub_picture_id:
	li	v1, APPLIB_API_START+0x65
	syscall
END (sty_draw_sub_picture_id) 
 
LEAF(sty_draw_bitmap) 
	.set noreorder 
	.text
sty_draw_bitmap:
	li	v1, APPLIB_API_START+0x66
	syscall
END (sty_draw_bitmap)

LEAF(sty_draw_sub_bitmap)
	.set noreorder
	.text
sty_draw_sub_bitmap:
	li	v1, APPLIB_API_START+0x67
	syscall
END (sty_draw_sub_bitmap) 
 
LEAF(sty_create_bitmap) 
	.set noreorder 
	.text
sty_create_bitmap:
	li	v1, APPLIB_API_START+0x68
	syscall
END (sty_create_bitmap) 
 
LEAF(sty_create_sub_bitmap) 
	.set noreorder 
	.text
sty_create_sub_bitmap:
	li	v1, APPLIB_API_START+0x69
	syscall
END (sty_create_sub_bitmap) 
 
LEAF(sty_delete_bitmap) 
	.set noreorder 
	.text
sty_delete_bitmap:
	li	v1, APPLIB_API_START+0x6a
	syscall
END (sty_delete_bitmap) 
 
LEAF(sty_save_bitmap) 
	.set noreorder 
	.text
sty_save_bitmap:
	li	v1, APPLIB_API_START+0x6b
	syscall
END (sty_save_bitmap) 
 
LEAF(sty_restore_bitmap) 
	.set noreorder 
	.text
sty_restore_bitmap:
	li	v1, APPLIB_API_START+0x6c
	syscall
END (sty_restore_bitmap)

LEAF(sty_get_view_rotation) 
	.set noreorder 
	.text
sty_get_view_rotation:
	li	v1, APPLIB_API_START+0x6d
	syscall
END (sty_get_view_rotation) 

LEAF(sty_set_screen_rotation) 
	.set noreorder 
	.text
sty_set_screen_rotation:
	li	v1, APPLIB_API_START+0x6e
	syscall
END (sty_set_screen_rotation) 

LEAF(sty_get_screen_rotation) 
	.set noreorder 
	.text
sty_get_screen_rotation:
	li	v1, APPLIB_API_START+0x6f
	syscall
END (sty_get_screen_rotation) 

LEAF(sty_lcd_update) 
	.set noreorder 
	.text
sty_lcd_update:
	li	v1, APPLIB_API_START+0x70
	syscall
END (sty_lcd_update) 



LEAF(sys_get_hotplugin_dev_info)
	.set noreorder
	.text
sys_get_hotplugin_dev_info:
	li	v1, APPLIB_API_START+0x71
	syscall
END (sys_get_hotplugin_dev_info) 
 
LEAF(sys_get_app_theme_res_dir) 
	.set noreorder 
	.text
sys_get_app_theme_res_dir:
	li	v1, APPLIB_API_START+0x72
	syscall
END (sys_get_app_theme_res_dir) 
 
LEAF(sys_get_cur_theme_name) 
	.set noreorder 
	.text
sys_get_cur_theme_name:
	li	v1, APPLIB_API_START+0x73
	syscall
END (sys_get_cur_theme_name) 
 
LEAF(sys_get_app_string_res_dir) 
	.set noreorder 
	.text
sys_get_app_string_res_dir:
	li	v1, APPLIB_API_START+0x74
	syscall
END (sys_get_app_string_res_dir) 
 
LEAF(sys_get_cur_language_codes) 
	.set noreorder 
	.text
sys_get_cur_language_codes:
	li	v1, APPLIB_API_START+0x75
	syscall
END (sys_get_cur_language_codes) 
 
LEAF(sys_get_cur_language_id) 
	.set noreorder 
	.text
sys_get_cur_language_id:
	li	v1, APPLIB_API_START+0x76
	syscall
END (sys_get_cur_language_id) 
 
LEAF(sys_get_language_codes) 
	.set noreorder 
	.text
sys_get_language_codes:
	li	v1, APPLIB_API_START+0x77
	syscall
END (sys_get_language_codes) 
 
LEAF(sys_get_language_id) 
	.set noreorder 
	.text
sys_get_language_id:
	li	v1, APPLIB_API_START+0x78
	syscall
END (sys_get_language_id) 
 
LEAF(sys_get_cur_support_language_list)
	.set noreorder
	.text 
sys_get_cur_support_language_list: 
	li	v1, APPLIB_API_START+0x79
	syscall
END (sys_get_cur_support_language_list) 
 
LEAF(sys_delete_cur_support_language_list)
	.set noreorder 
	.text
sys_delete_cur_support_language_list: 
	li	v1, APPLIB_API_START+0x7a
	syscall
END (sys_delete_cur_support_language_list)
 
LEAF(sys_get_default_font_file) 
	.set noreorder 
	.text
sys_get_default_font_file:
	li	v1, APPLIB_API_START+0x7b
	syscall
END (sys_get_default_font_file) 
 
LEAF(sys_check_media_dir) 
	.set noreorder 
	.text
sys_check_media_dir:
	li	v1, APPLIB_API_START+0x7c
	syscall
END (sys_check_media_dir) 
 
LEAF(sys_forbid_screensaver)
	.set noreorder 
	.text
sys_forbid_screensaver: 
	li	v1, APPLIB_API_START+0x7d
	syscall
END (sys_forbid_screensaver)
 
LEAF(sys_set_screensaver_idle_time) 
	.set noreorder 
	.text
sys_set_screensaver_idle_time:
	li	v1, APPLIB_API_START+0x7e
	syscall
END (sys_set_screensaver_idle_time) 
 
LEAF(sys_is_screensaver_forbidden) 
	.set noreorder 
	.text
sys_is_screensaver_forbidden:
	li	v1, APPLIB_API_START+0x7f
	syscall
END (sys_is_screensaver_forbidden) 
 
LEAF(sys_forbid_auto_standby)
	.set noreorder 
	.text
sys_forbid_auto_standby: 
	li	v1, APPLIB_API_START+0x80
	syscall
END (sys_forbid_auto_standby)
 
LEAF(sys_set_auto_standby_idle_time)
	.set noreorder 
	.text
sys_set_auto_standby_idle_time: 
	li	v1, APPLIB_API_START+0x81
	syscall
END (sys_set_auto_standby_idle_time)
 
LEAF(sys_is_auto_standby_forbidden) 
	.set noreorder 
	.text
sys_is_auto_standby_forbidden:
	li	v1, APPLIB_API_START+0x82
	syscall
END (sys_is_auto_standby_forbidden) 

LEAF(sys_set_timed_shutdown_time) 
	.set noreorder 
	.text
sys_set_timed_shutdown_time:
	li	v1, APPLIB_API_START+0x83
	syscall
END (sys_set_timed_shutdown_time) 
 
LEAF(sys_set_lcd_backlight) 
	.set noreorder 
	.text
sys_set_lcd_backlight:
	li	v1, APPLIB_API_START+0x84
	syscall
END (sys_set_lcd_backlight) 
 
LEAF(sys_set_rtc_date_time) 
	.set noreorder 
	.text
sys_set_rtc_date_time:
	li	v1, APPLIB_API_START+0x85
	syscall
END (sys_set_rtc_date_time) 
 
LEAF(sys_get_rtc_date_time) 
	.set noreorder 
	.text
sys_get_rtc_date_time:
	li	v1, APPLIB_API_START+0x86
	syscall
END (sys_get_rtc_date_time) 
 
LEAF(sys_set_rtc_drm_date_time) 
	.set noreorder 
	.text
sys_set_rtc_drm_date_time:
	li	v1, APPLIB_API_START+0x87
	syscall
END (sys_set_rtc_drm_date_time) 
 
LEAF(sys_get_rtc_drm_date_time) 
	.set noreorder 
	.text
sys_get_rtc_drm_date_time:
	li	v1, APPLIB_API_START+0x88
	syscall
END (sys_get_rtc_drm_date_time) 
 
LEAF(sys_rtc_change_to_stdsec) 
	.set noreorder 
	.text
sys_rtc_change_to_stdsec:
	li	v1, APPLIB_API_START+0x89
	syscall
END (sys_rtc_change_to_stdsec) 
 
LEAF(sys_open_rtc_dev) 
	.set noreorder 
	.text
sys_open_rtc_dev:
	li	v1, APPLIB_API_START+0x8a
	syscall
END (sys_open_rtc_dev) 
 
LEAF(sys_close_rtc_dev) 
	.set noreorder 
	.text
sys_close_rtc_dev:
	li	v1, APPLIB_API_START+0x8b
	syscall
END (sys_close_rtc_dev) 
 
LEAF(print_buf) 
	.set noreorder 
	.text
print_buf:
	li	v1, APPLIB_API_START+0x8c
	syscall
END (print_buf) 
 
LEAF(int_sqrt) 
	.set noreorder 
	.text
int_sqrt:
	li	v1, APPLIB_API_START+0x8d
	syscall
END (int_sqrt) 

LEAF(sys_set_key_tone_enable) 
	.set noreorder 
	.text
sys_set_key_tone_enable:
	li	v1, APPLIB_API_START+0x8e
	syscall
END (sys_set_key_tone_enable) 

LEAF(sys_pm_set_clk_domain) 
	.set noreorder 
	.text
sys_pm_set_clk_domain:
	li	v1, APPLIB_API_START+0x8f
	syscall
END (sys_pm_set_clk_domain) 

LEAF(sys_pm_clear_clk_domain) 
	.set noreorder 
	.text
sys_pm_clear_clk_domain:
	li	v1, APPLIB_API_START+0x90
	syscall
END (sys_pm_clear_clk_domain) 

LEAF(sys_pm_get_clk_info) 
	.set noreorder 
	.text
sys_pm_get_clk_info:
	li	v1, APPLIB_API_START+0x91
	syscall
END (sys_pm_get_clk_info) 

LEAF(basename) 
	.set noreorder 
	.text
basename:
	li	v1, APPLIB_API_START+0x92
	syscall
END (basename)

LEAF(dirname) 
	.set noreorder 
	.text
dirname:
	li	v1, APPLIB_API_START+0x93
	syscall
END (dirname)

LEAF(sys_get_common_theme_res_dir) 
	.set noreorder 
	.text
sys_get_common_theme_res_dir:
	li	v1, APPLIB_API_START+0x94
	syscall
END (sys_get_common_theme_res_dir) 

LEAF(sys_get_common_string_res_dir) 
	.set noreorder 
	.text
sys_get_common_string_res_dir:
	li	v1, APPLIB_API_START+0x95
	syscall
END (sys_get_common_string_res_dir) 

LEAF(sys_power_threshold_check) 
	.set noreorder 
	.text
sys_power_threshold_check:
	li	v1, APPLIB_API_START+0x96
	syscall
END (sys_power_threshold_check) 

LEAF(grab_msg)
	.set noreorder
	.text
grab_msg:
	li	v1, APPLIB_API_START+0x97
	syscall
END (grab_msg)

LEAF(release_msg)
	.set noreorder
	.text
release_msg:
	li	v1, APPLIB_API_START+0x98
	syscall
END (release_msg)

LEAF(restore_default_all_animation) 
	.set noreorder 
	.text
restore_default_all_animation:
	li	v1, APPLIB_API_START+0x99
	syscall
END (restore_default_all_animation)

LEAF(canvas_disable_focus)
	.set noreorder
	.text
canvas_disable_focus:
	li	v1, APPLIB_API_START+0x9a
	syscall
END (canvas_disable_focus)

LEAF(canvas_enable_focus)
	.set noreorder
	.text
canvas_enable_focus:
	li	v1, APPLIB_API_START+0x9b
	syscall
END (canvas_enable_focus)

LEAF(sys_get_main_disk)
	.set noreorder
	.text
sys_get_main_disk:
	li	v1, APPLIB_API_START+0x9c
	syscall
END (sys_get_main_disk)

LEAF(sys_set_main_disk)
	.set noreorder
	.text
sys_set_main_disk:
	li	v1, APPLIB_API_START+0x9d
	syscall
END (sys_set_main_disk)

LEAF(sys_get_main_disk_dir)
	.set noreorder
	.text
sys_get_main_disk_dir:
	li	v1, APPLIB_API_START+0x9e
	syscall
END (sys_get_main_disk_dir)

LEAF(canvas_get_focus)
	.set noreorder
	.text
canvas_get_focus:
	li	v1, APPLIB_API_START+0x9f
	syscall
END (canvas_get_focus)

LEAF(canvas_get_blending_focus)
	.set noreorder
	.text
canvas_get_blending_focus:
	li	v1, APPLIB_API_START+0xa0
	syscall
END (canvas_get_blending_focus)

LEAF(sys_enable_sys_evt_detect)
	.set noreorder
	.text
sys_enable_sys_evt_detect:
	li	v1, APPLIB_API_START+0xa1
	syscall
END (sys_enable_sys_evt_detect)

LEAF(filter_msg)
	.set noreorder
	.text
filter_msg:
	li	v1, APPLIB_API_START+0xa2
	syscall
END (filter_msg)

LEAF(unfilter_msg)
	.set noreorder
	.text
unfilter_msg:
	li	v1, APPLIB_API_START+0xa3
	syscall
END (unfilter_msg)

LEAF(filter_all_msg)
	.set noreorder
	.text
filter_all_msg:
	li	v1, APPLIB_API_START+0xa4
	syscall
END (filter_all_msg)

LEAF(unfilter_all_msg)
	.set noreorder
	.text
unfilter_all_msg:
	li	v1, APPLIB_API_START+0xa5
	syscall
END (unfilter_all_msg)

LEAF(sys_get_media_dir)
	.set noreorder
	.text
sys_get_media_dir:
	li	v1, APPLIB_API_START+0xa6
	syscall
END (sys_get_media_dir)

LEAF(create_global_msg_queue)
	.set noreorder
	.text
create_global_msg_queue:
	li	v1, APPLIB_API_START+0xa7
	syscall
END (create_global_msg_queue)

LEAF(delete_global_msg_queue)
	.set noreorder
	.text
delete_global_msg_queue:
	li	v1, APPLIB_API_START+0xa8
	syscall
END (delete_global_msg_queue)

LEAF(connect_global_msg_queue)
	.set noreorder
	.text
connect_global_msg_queue:
	li	v1, APPLIB_API_START+0xa9
	syscall
END (connect_global_msg_queue)

LEAF(disconnect_global_msg_queue)
	.set noreorder
	.text
disconnect_global_msg_queue:
	li	v1, APPLIB_API_START+0xaa
	syscall
END (disconnect_global_msg_queue)

LEAF(disconnect_all_global_msg_queue)
	.set noreorder
	.text
disconnect_all_global_msg_queue:
	li	v1, APPLIB_API_START+0xab
	syscall
END (disconnect_all_global_msg_queue)

LEAF(get_sys_dispatcher)
	.set noreorder
	.text
get_sys_dispatcher:
	li	v1, APPLIB_API_START+0xac
	syscall
END (get_sys_dispatcher)

LEAF(send_async_msg_to_global_mq)
	.set noreorder
	.text
send_async_msg_to_global_mq:
	li	v1, APPLIB_API_START+0xad
	syscall
END (send_async_msg_to_global_mq)

LEAF(sys_forbid_timed_shutdown)
    .set noreorder
    .text
sys_forbid_timed_shutdown:
    li  v1, APPLIB_API_START+0xae
    syscall
END (sys_forbid_timed_shutdown)

LEAF(sys_is_timed_shutdown_forbidden)
    .set noreorder
    .text
sys_is_timed_shutdown_forbidden:
    li  v1, APPLIB_API_START+0xaf
    syscall
END (sys_is_timed_shutdown_forbidden)

LEAF(sys_forbid_soft_watchdog)
    .set noreorder
    .text
sys_forbid_soft_watchdog:
    li  v1, APPLIB_API_START+0xb0
    syscall
END (sys_forbid_soft_watchdog)

LEAF(sys_is_soft_watchdog_forbid)
    .set noreorder
    .text
sys_is_soft_watchdog_forbid:
    li  v1, APPLIB_API_START+0xb1
    syscall
END (sys_is_soft_watchdog_forbid)
