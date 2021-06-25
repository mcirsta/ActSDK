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

static const unsigned int applib_apitbl[]=
{
    (unsigned int)app_running_list_init,
    (unsigned int)app_running_list_deinit,
    (unsigned int)is_app_exist,
    (unsigned int)get_app_type,
    (unsigned int)get_app_pid,
    (unsigned int)get_app_name,
    (unsigned int)app_running_list_get_app_info,
    (unsigned int)app_running_list_get_this_app_info,
    (unsigned int)app_running_list_set_app_info,
    (unsigned int)app_running_list_set_this_app_info,
    (unsigned int)app_running_list_get_list,
    (unsigned int)app_running_list_add,
    (unsigned int)app_running_list_delete,
    (unsigned int)print_app_running_list,
    (unsigned int)0,
    (unsigned int)set_timer,
    
    (unsigned int)set_single_shot_timer,
    (unsigned int)modify_timer,
    (unsigned int)stop_timer,
    (unsigned int)restart_timer,
    (unsigned int)kill_timer,
    (unsigned int)kill_timers,
    (unsigned int)set_inner_timer,
    (unsigned int)set_inner_single_shot_timer,
    (unsigned int)modify_inner_timer,
    (unsigned int)stop_inner_timer,
    (unsigned int)restart_inner_timer,
    (unsigned int)kill_inner_timer,
    (unsigned int)kill_inner_timers,
    (unsigned int)delete_timers,
    (unsigned int)delete_inner_timers,
    (unsigned int)handle_timers,
    
    (unsigned int)get_tick_time_ms,
    (unsigned int)set_user_bg_pic,
    (unsigned int)restore_default_bg_pic,
    (unsigned int)get_user_bg_pic,
    (unsigned int)canvas_create,
    (unsigned int)canvas_delete,
    (unsigned int)canvas_show,
    (unsigned int)canvas_hide,
    (unsigned int)canvas_set_region,
    (unsigned int)canvas_erase,
    (unsigned int)canvas_erase_all,
    (unsigned int)canvas_set_alpha,
    (unsigned int)canvas_get_top,
    (unsigned int)canvas_set_top,
    (unsigned int)canvas_get_info,
    (unsigned int)canvas_get_blending_top,
    
    (unsigned int)canvas_set_blending_mode,
    (unsigned int)desktop_parser_create,
    (unsigned int)desktop_parser_get_key,
    (unsigned int)desktop_parser_delete,
    (unsigned int)applib_register_app,
    (unsigned int)applib_unregister_app,
    (unsigned int)applib_init,
    (unsigned int)applib_quit,
    (unsigned int)get_msg,
    (unsigned int)send_sync_msg,
    (unsigned int)reply_sync_msg,
    (unsigned int)send_async_msg,
    (unsigned int)broadcast_msg,
    (unsigned int)dispatch_msg,
    (unsigned int)register_sys_dispatcher,
    (unsigned int)unregister_sys_dispatcher,
    
    (unsigned int)exit_msg_loop,
    (unsigned int)clear_sys_msg_queue,
    (unsigned int)clear_key_msg_queue,
    (unsigned int)enter_background_mode,
    (unsigned int)enter_foreground_mode,
    (unsigned int)set_soft_watchdog_timeout,
    (unsigned int)check_soft_watchdogs,
    (unsigned int)sty_open_common_pic_res,
    (unsigned int)sty_open_common_str_res,
    (unsigned int)sty_close_common_pic_res,
    (unsigned int)sty_close_common_str_res,
    (unsigned int)sty_open_app_pic_res,
    (unsigned int)sty_open_app_str_res,
    (unsigned int)sty_close_app_pic_res,
    (unsigned int)sty_close_app_str_res,
    (unsigned int)sty_set_app_scene,
    
    (unsigned int)sty_get_app_scene,
    (unsigned int)sty_set_common_scene,
    (unsigned int)sty_get_common_scene,
    (unsigned int)sty_set_text_encoding,
    (unsigned int)sty_get_text_encoding,
    (unsigned int)sty_get_string,
    (unsigned int)sty_truncate_string_by_width,
    (unsigned int)sty_set_string_format,
    (unsigned int)sty_restore_string_format,
    (unsigned int)sty_draw_string_buf,
    (unsigned int)sty_draw_string_id,
    (unsigned int)sty_draw_string_res,
    (unsigned int)sty_draw_string_res_ext,
    (unsigned int)sty_create_scroll_string,
    (unsigned int)sty_draw_scroll_string,
    (unsigned int)sty_delete_scroll_string,
    
    (unsigned int)sty_draw_picture_buf,
    (unsigned int)sty_draw_picture_id,
    (unsigned int)sty_draw_picture_res,
    (unsigned int)sty_draw_picregion_res,
    (unsigned int)sty_draw_sub_picture_buf,
    (unsigned int)sty_draw_sub_picture_id,
    (unsigned int)sty_draw_bitmap,
    (unsigned int)sty_draw_sub_bitmap,
    (unsigned int)sty_create_bitmap,
    (unsigned int)sty_create_sub_bitmap,
    (unsigned int)sty_delete_bitmap,
    (unsigned int)sty_save_bitmap,
    (unsigned int)sty_restore_bitmap,
    (unsigned int)sty_get_view_rotation,
    (unsigned int)sty_set_screen_rotation,
    (unsigned int)sty_get_screen_rotation,
    
    (unsigned int)sty_lcd_update,
    (unsigned int)sys_get_hotplugin_dev_info,
    (unsigned int)sys_get_app_theme_res_dir,
    (unsigned int)sys_get_cur_theme_name,
    (unsigned int)sys_get_app_string_res_dir,
    (unsigned int)sys_get_cur_language_codes,
    (unsigned int)sys_get_cur_language_id,
    (unsigned int)sys_get_language_codes,
    (unsigned int)sys_get_language_id,
    (unsigned int)sys_get_cur_support_language_list,
    (unsigned int)sys_delete_cur_support_language_list,
    (unsigned int)sys_get_default_font_file,
    (unsigned int)sys_check_media_dir,
    (unsigned int)sys_forbid_screensaver,
    (unsigned int)sys_set_screensaver_idle_time,
    (unsigned int)sys_is_screensaver_forbidden,
    
    (unsigned int)sys_forbid_auto_standby,
    (unsigned int)sys_set_auto_standby_idle_time,
    (unsigned int)sys_is_auto_standby_forbidden,
    (unsigned int)sys_set_timed_shutdown_time,
    (unsigned int)sys_set_lcd_backlight,
    (unsigned int)sys_set_rtc_date_time,
    (unsigned int)sys_get_rtc_date_time,
    (unsigned int)sys_set_rtc_drm_date_time,
    (unsigned int)sys_get_rtc_drm_date_time,
    (unsigned int)sys_rtc_change_to_stdsec,
    (unsigned int)sys_open_rtc_dev,
    (unsigned int)sys_close_rtc_dev,
    (unsigned int)print_buf,
    (unsigned int)int_sqrt,
    (unsigned int)sys_set_key_tone_enable,

    (unsigned int)sys_pm_set_clk_domain,
    (unsigned int)sys_pm_clear_clk_domain,
    (unsigned int)sys_pm_get_clk_info,

    (unsigned int)basename,
    (unsigned int)dirname,
    (unsigned int)sys_get_common_theme_res_dir,
    (unsigned int)sys_get_common_string_res_dir,

    (unsigned int)sys_power_threshold_check,

    (unsigned int)grab_msg,
    (unsigned int)release_msg,

    (unsigned int)restore_default_all_animation,
    
    (unsigned int)canvas_disable_focus,
    (unsigned int)canvas_enable_focus,

    (unsigned int)sys_get_main_disk,
    (unsigned int)sys_set_main_disk,

    (unsigned int)sys_get_main_disk_dir,
    
    (unsigned int)canvas_get_focus,
    (unsigned int)canvas_get_blending_focus,

    (unsigned int)sys_enable_sys_evt_detect,

    (unsigned int)filter_msg,
    (unsigned int)unfilter_msg,
    (unsigned int)filter_all_msg,
    (unsigned int)unfilter_all_msg,

    (unsigned int)sys_get_media_dir,

    (unsigned int)create_global_msg_queue,
    (unsigned int)delete_global_msg_queue,
    (unsigned int)connect_global_msg_queue,
    (unsigned int)disconnect_global_msg_queue,
    (unsigned int)disconnect_all_global_msg_queue,

    (unsigned int)get_sys_dispatcher,
    (unsigned int)send_async_msg_to_global_mq,

    (unsigned int)sys_forbid_timed_shutdown,
    (unsigned int)sys_is_timed_shutdown_forbidden,
    (unsigned int)sys_forbid_soft_watchdog,
    (unsigned int)sys_is_soft_watchdog_forbid,
};                
                  
/******************************************************************************/
/*!                    
* \par  Description:
*      init函数
* \note
* 使用宏__init确保函数定位在.ctors段
*******************************************************************************/
int __attribute__((constructor)) applib_api_init(void)
{
    api_install(APINO_APPLIB, (unsigned)applib_apitbl);
    return 0;
}

/******************************************************************************/
/*!                    
* \par  Description:
*      exit函数
* \note
* 使用宏__exit确保函数定位在.dtors段
*******************************************************************************/
void __attribute__((destructor)) api_api_exit(void)
{
    api_uninstall(APINO_APPLIB);
    return;
}

