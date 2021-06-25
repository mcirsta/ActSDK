/*
 *******************************************************************************
 *                       NOYA1100
 *                (c) Copyright 2002-2008, Actions Co,Ld.
 *                        All Right Reserved
 * FileName: app_config.h
 * Description: 定义配置key名字(set_config/get_config函数的第一个参数)，
 *              默认配置值见images/config.txt
 * History:
 *       <author>    <time>          <version >      <desc>
 *       cheason     2009/06/01      1.0             建立该文件
 *******************************************************************************
 */

#ifndef __APP_CONFIG_H__
#define __APP_CONFIG_H__


/*
 * 全局配置
 */
#define CFG_UPGRADE_FLAG            "UPGRADE_FLAG"
#define CFG_PID                     "PID"
#define CFG_VID                     "VID"
#define CFG_USBATTRI_8              "USBATTRI_8"
#define CFG_USBIDENTIFICATION_16    "USBIDENTIFICATION_16"
#define CFG_USBPRODUCTVER_4         "USBPRODUCTVER_4"
#define CFG_USBDESCRIPSTR_23        "USBDESCRIPSTR_23"
#define CFG_UDISKSERIALNUMFLAG      "UdiskSerialNumFlag"
#define CFG_UDISK_VOLUME            "UdiskVolume"
#define CFG_MTPPID                  "MTPPID"
#define CFG_MTPVID                  "MTPVID"
#define CFG_MTPMFTINFO              "MTPMFTINFO"
#define CFG_MTPPRODUCTINFO          "MTPPRODUCTINFO"
#define CFG_MTPPRODUCTVER           "MTPPRODUCTVER"
#define CFG_VER                     "VER"
#define CFG_DATE                    "DATE"
#define CFG_TIME                    "TIME"
#define CFG_TIME_FORMAT             "TIME_FORMAT"
#define CFG_BACKLIGHT_BRIGHTNESS    "BACKLIGHT_BRIGHTNESS"
#define CFG_BACKLIGHT_DURATION      "BACKLIGHT_DURATION"
#define CFG_IDLE_TIME               "IDLE_TIME"
#define CFG_SLEEP_TIME              "SLEEP_TIME"
#define CFG_LANG_CODES              "LANG_CODES"
#define CFG_VOICE_VOLUM             "VOICE_VOLUM"
#define CFG_BOOT_LOGO               "BOOT_LOGO"

#define CFG_SCREEN_MODE             "SCREEN_MODE"
#define CFG_TVOUT                   "TVOUT"
#define CFG_CONNECT_TYPE            "CONNECT_TYPE"
#define CFG_DEFAULT_FONT            "DEFAULT_FONT"
#define CFG_USER_FONT               "USER_FONT"
#define CFG_USER_FONT_VALID         "USER_FONT_VALID"
#define CFG_THEMES                  "THEMES"
#define CFG_CUR_THEME               "CUR_THEME"
#define CFG_KEY_TONE_ON             "KEY_TONE_ON"
#define CFG_KEY_TONE_FULLPATH       "KEY_TONE_FULLPATH"
#define CFG_KEY_TONE_CUR            "KEY_TONE_CUR"
#define CFG_KEY_TONES               "KEY_TONES"
#define CFG_KEY_TONE_1              "KEY_TONE_1"
#define CFG_KEY_TONE_2              "KEY_TONE_2"
#define CFG_KEY_TONE_3              "KEY_TONE_3"
#define CFG_KEY_TONE_4              "KEY_TONE_4"
#define CFG_USERNAME                "USERNAME"
#define CFG_USERNAME                "USERNAME"
#define CFG_SEARCH_FOLDER           "SEARCH_FOLDER"
#define CFG_GSENSOR_SHAKE_SUPPORT   "GSENSOR_SHAKE_SUPPORT"
#define CFG_GSENSOR_SHAKE_THRESHOLD   "GSENSOR_SHAKE_THRESHOLD"
#define CFG_UHOST_SUPPORT           "UHOST_SUPPORT"
#define CFG_CARD_SUPPORT            "CARD_SUPPORT"

#define CFG_MUSIC_MUTEX_APPS        "music_mutex_apps"
#define CFG_RADIO_MUTEX_APPS        "radio_mutex_apps"

#define CFG_FSEL_SORT_STYLE         "fsel_sort_style"

#define CFG_DRM_TIME_OFF            "DRM_TIME_OFF"

#define CFG_SCREEN_X                "SCREEN_X"
#define CFG_SCREEN_Y                "SCREEN_Y"
#define CFG_SCREEN_Z                "SCREEN_Z"

#define CFG_DEF_WATCHDOG_TIME       "WATCHDOG_TIME"
#define CFG_NEED_GEN_PLIST          "need_gen_plist"
#define CFG_IGNORE_GEN_PLIST        "ignore_gen_plist"

#define CFG_MUSIC_NOWPLAYING        "music_nowplaying"
#define CFG_PHOTO_NOWPLAYING        "photo_nowplaying"
#define CFG_VIDEO_NOWPLAYING        "video_nowplaying"
#define CFG_VIDEO_RESUME_VISIBLE    "video_resume_visible"
#define CFG_VIDEO_FSEL_MODE         "video_fsel_mode"

#define CFG_NANDBASE                "NANDBASE"
#define CFG_MAIN_DISK               "MAIN_DISK"

/*
 * 开关机画面
 */
#define CFG_AMINATION_POWERON_ENABLE		     "poweron_enable"
#define CFG_AMINATION_POWEROFF_ENABLE		     "poweroff_enable"

#define CFG_ANIMATION_POWERON_START_MODE             "poweron_start_mode"
#define CFG_ANIMATION_POWEROFF_START_MODE            "poweroff_start_mode"
#define CFG_ANIMATION_POWERON_PIC_UPRIGHT_NAME       "poweron_pic_upright_name"
#define CFG_ANIMATION_POWERON_PIC_SIDELONG_NAME      "poweron_pic_sidelong_name"
#define CFG_ANIMATION_POWEROFF_PIC_UPRIGHT_NAME      "poweroff_pic_upright_name"
#define CFG_ANIMATION_POWEROFF_PIC_SIDELONG_NAME     "poweroff_pic_sidelong_name"
#define CFG_DEF_ANIMATION_POWERON_PIC_UPRIGHT_NAME   "def_poweron_pic_upright_name"
#define CFG_DEF_ANIMATION_POWERON_PIC_SIDELONG_NAME  "def_poweron_pic_sidelong_name"
#define CFG_DEF_ANIMATION_POWEROFF_PIC_UPRIGHT_NAME  "def_poweroff_pic_upright_name"
#define CFG_DEF_ANIMATION_POWEROFF_PIC_SIDELONG_NAME "def_poweroff_pic_sidelong_name"


/*
 * 界面特效配置
 */
#define CFG_MODE_COMMON             "mode_common"
#define CFG_FORWAED_COMMON          "forward_common"
#define CFG_BACKWARD_COMMON         "backward_common"

#define CFG_MODE_APP_CHANGE         "mode_app_change"
#define CFG_FORWAED_APP_CHANGE      "forward_app_change"
#define CFG_BACKWARD_APP_CHANGE     "backward_app_change"

#define CFG_MODE_OPTION_RGB         "mode_option_rgb"
#define CFG_FORWARD_OPTION          "forward_option"
#define CFG_BACKWARD_OPTION         "backward_option"
#define CFG_MODE_OPTION_YUV         "mode_option_yuv"

#define CFG_MODE_ROTATE             "mode_rotate"
#define CFG_FORWARD_ROTATE          "forward_rotate"
#define CFG_BACKWARD_ROTATE         "backward_rotate"

#define CFG_MODE_DIALOG             "mode_dialog"
#define CFG_FORWARD_DIALOG          "forward_dialog"
#define CFG_BACKWARD_DIALOG         "backward_dialog"



/*
 * 多媒体文件类型配置
 */
#define CFG_APP_SUPPORT_EXT_MUSIC        "app_support_ext_music"
                                  
#define CFG_APP_SUPPORT_EXT_VIDEO        "app_support_ext_video"
#define CFG_APP_SUPPORT_EXT_VIDEO_SWF    "app_support_ext_video_swf"
                                  
#define CFG_APP_SUPPORT_EXT_PHOTO        "app_support_ext_photo"
                                  
#define CFG_APP_SUPPORT_EXT_EBOOK        "app_support_ext_ebook"
#define CFG_APP_SUPPORT_EXT_EMULATOR     "app_support_ext_emulator"
                                  
#define CFG_APP_SUPPORT_EXT_ALL          "app_support_ext_all"

#define CFG_APP_SUPPORT_EXT_RECORDER     "app_support_ext_recorder"


/*
 * Launcher应用配置
 */
#define CFG_APPLICATIONS            "Applications"
#define CFG_FOCUS_APPID             "FOCUS_APPID"
#define CFG_DESKTOP_PATH            "DESKTOP_PATH"
#define CFG_THIRD_PARTY_PATH        "THIRD_PARTY_PATH"

/*
 * photo应用配置
 */
#define CFG_PHOTO_PREVIEWMODE       "photo_previewmode"
#define CFG_PHOTO_SLIDE_EFFECT_MODE "photo_slide_effect_mode"
#define CFG_PHOTO_SLIDE_INTERVAL    "photo_slide_interval"
#define CFG_PHOTO_PATH_TYPE         "photo_path_type"
#define CFG_PHOTO_CURRENT_INDEX     "photo_current_index"
#define CFG_PHOTO_NOWPLAY_PATH      "photo_nowplaypath"
#define CFG_PHOTO_ENABLE_BACKGROUND "photo_enable_background"

/*
 * fm应用配置
 */
#define CFG_FM_BAND                 "fm_band"
#define CFG_FM_INTENSITY            "fm_intensity"
#define CFG_FM_FREQUENCY            "fm_freq"
#define CFG_FM_RECORD_QUALITY       "fm_record_quality"
#define CFG_FM_RECORD_GAIN          "fm_record_gain"
#define CFG_FM_RECORD_PATH          "fm_record_path"
#define CFG_FM_LINEIN_CHECK         "fm_line_in_check" 
#define CFG_FM_USE_PRESET           "fm_use_preset"
#define CFG_FM_CUR_PRESET_ID        "fm_cur_preset_id"
#define CFG_FM_USERCHANNEL_MODE     "fm_userchannel_mode"
#define CFG_FM_USERCHANNEL_ID       "fm_userchannel_id"
#define CFG_FM_USERCHANNEL_PATH     "fm_userchannel_path"

/*
 * dc/dv应用配置
 */
#define CFG_DSC_HAVE_STICKER							"dsc_have_sticker"
#define CFG_DSC_STIKCER_FULLNAME_1				       "dsc_stikcer_fullname_1"
#define CFG_DSC_STIKCER_FULLNAME_2				       "dsc_stikcer_fullname_2"
#define CFG_DSC_STIKCER_FULLNAME_3				       "dsc_stikcer_fullname_3"
#define CFG_DSC_STIKCER_TMP_1							"dsc_stikcer_tmp_1"
#define CFG_DSC_STIKCER_TMP_2							"dsc_stikcer_tmp_2"
#define CFG_DSC_STIKCER_TMP_3							"dsc_stikcer_tmp_3"
#define CFG_DSC_HAVE_FLASHLAMP						"dsc_have_flashlamp"
#define CFG_DSC_HAVE_SHUTTER_SOUND				"dsc_have_shutter_sound" 
#define CFG_DSC_SHUTTER_SOUND_FULLNAME_1	"dsc_shutter_sound_fullname_1"
#define CFG_DSC_SHUTTER_SOUND_FULLNAME_2	"dsc_shutter_sound_fullname_2"
#define CFG_DSC_SHUTTER_SOUND_FULLNAME_3	"dsc_shutter_sound_fullname_3"

#define	CFG_DSC_PLAYBACK            "dsc_playback"
#define	CFG_DSC_WHITEBALANCE        "dsc_whitebalance"
#define	CFG_DSC_BRIGHTNESS          "dsc_brightness"
#define	CFG_DSC_EXPOSURE_MODE       "dsc_exposure_mode"
#define	CFG_DSC_FLASHLAMP           "dsc_flashlamp"
#define	CFG_DSC_FACE_IDENTIFY       "dsc_face_identify"
#define	CFG_DSC_STICKERPHOTO        "dsc_stickerphoto"
#define	CFG_DSC_SPECIALEFFECT       "dsc_specialeffect"
#define	CFG_DSC_SAVEPATH            "dsc_savepath"
#define	CFG_DSC_RESOLUTION          "dsc_resolution"
#define	CFG_DSC_SHUTTER_SOUND       "dsc_shutter_sound"
#define	CFG_DSC_DATER               "dsc_dater"
#if 1//(SHORTCUT_PLAYBACK_DSC == 1)
#define	CFG_DSC_PLAYBACK_ROOT    "dsc_playback_root"
#define	CFG_DSC_PLAYBACK_FULLNAME "dsc_playback_fullname"
#endif /* SHORTCUT_CAMERA */
#define CFG_DSC_MACHINENAME          "dsc_machinename"


#define	CFG_DV_PLAYBACK             "dv_playback"
#define	CFG_DV_WHITEBALANCE         "dv_whitebalance"
#define	CFG_DV_BRIGHTNESS           "dv_brightness"
#define	CFG_DV_EXPOSURE_MODE        "dv_exposure_mode"
#define	CFG_DV_SPECIALEFFECT        "dv_specialeffect"
#define	CFG_DV_SAVEPATH             "dv_savepath"
#define	CFG_DV_RESOLUTION           "dv_resolution"
#define	CFG_DV_RECORD_QUALITY       "dv_record_quality"
#define	CFG_DV_RECORD_GAIN          "dv_record_gain"
#define	CFG_DV_ACTIVITY_DETECT      "dv_activity_detect"
#if 1//(SHORTCUT_PLAYBACK_DV == 1)
#define	CFG_DV_PLAYBACK_ROOT    "dv_playback_root"
#define	CFG_DV_PLAYBACK_FULLNAME "dv_playback_fullname"
#endif /* SHORTCUT_CAMERA */
#define CFG_DV_REC_FRAME_RATE_MAX   "dv_rec_frame_rate_max"
#define CFG_DV_REC_FRAME_RATE_MIN   "dv_rec_frame_rate_min"

#define  CFG_CAM_PLAYBACK_SCENE "cam_playback_scene"
#define  CFG_CAM_DEFAULT_SCENE "cam_default_scene"

#define	CFG_DSC_VISIBLE_PLAYBACK            "dsc_visible_playback"
#define	CFG_DSC_VISIBLE_WHITEBALANCE        "dsc_visible_whitebalance"
#define	CFG_DSC_VISIBLE_BRIGHTNESS          "dsc_visible_brightness"
#define	CFG_DSC_VISIBLE_EXPOSURE_MODE       "dsc_visible_exposure_mode"
#define	CFG_DSC_VISIBLE_FLASHLAMP           "dsc_visible_flashlamp"
#define	CFG_DSC_VISIBLE_FACE_IDENTIFY       "dsc_visible_face_identify"
#define	CFG_DSC_VISIBLE_STICKERPHOTO        "dsc_visible_stickerphoto"
#define	CFG_DSC_VISIBLE_SPECIALEFFECT       "dsc_visible_specialeffect"
#define	CFG_DSC_VISIBLE_SAVEPATH            "dsc_visible_savepath"
#define	CFG_DSC_VISIBLE_RESOLUTION          "dsc_visible_resolution"
#define	CFG_DSC_VISIBLE_FREE_SPACE          "dsc_visible_free_space"
#define	CFG_DSC_VISIBLE_SELFTIMER						"dsc_visible_selftimer"
#define	CFG_DSC_VISIBLE_CONTSHOOTING				"dsc_visible_contshooting"
#define	CFG_DSC_VISIBLE_SHUTTER_SOUND       "dsc_visible_shutter_sound"
#define	CFG_DSC_VISIBLE_DATER               "dsc_visible_dater"

#define	CFG_DV_VISIBLE_PLAYBACK             "dv_visible_playback"
#define	CFG_DV_VISIBLE_WHITEBALANCE         "dv_visible_whitebalance"
#define	CFG_DV_VISIBLE_BRIGHTNESS           "dv_visible_brightness"
#define	CFG_DV_VISIBLE_EXPOSURE_MODE        "dv_visible_exposure_mode"
#define	CFG_DV_VISIBLE_SPECIALEFFECT        "dv_visible_specialeffect"
#define	CFG_DV_VISIBLE_SAVEPATH             "dv_visible_savepath"
#define	CFG_DV_VISIBLE_RESOLUTION           "dv_visible_resolution"
#define	CFG_DV_VISIBLE_FREE_SPACE          "dv_visible_free_space"
#define	CFG_DV_VISIBLE_RECORD_QUALITY       "dv_visible_record_quality"
#define	CFG_DV_VISIBLE_RECORD_GAIN          "dv_visible_record_gain"
#define	CFG_DV_VISIBLE_ACTIVITY_DETECT      "dv_visible_activity_detect"

/*
 * music应用配置
 */
#define CFG_MUSIC_PLAY_MODE         "music_play_mode"
#define CFG_MUSIC_DISPLAY_MODE      "music_display_mode"
#define CFG_MUSIC_DISPLAY_EFFECT   "music_display_effect"
#define CFG_MUSIC_EQ_MODE           "music_eq_mode"
#define CFG_MUSIC_FADE              "music_fade"
#define CFG_MUSIC_PLAY_SPEED        "music_play_speed"
#define CFG_MUSIC_FFB_SPEED         "music_ffb_speed"
#define CFG_MUSIC_FFB_TIME          "music_ffb_time"
#define CFG_MUSIC_AB_COUNT          "music_ab_count"
#define CFG_MUSIC_AB_TIME           "music_ab_time"
#define CFG_MUSIC_AB_AUTOPLAY       "music_ab_autoplay"

#define CFG_MUSIC_USEREQ_PARAM      "music_usereq_param"
#define CFG_MUSIC_SRSEQ_PARAM       "music_srseq_param"

#define CFG_MUSIC_FILE_TOTAL        "music_file_total"
#define CFG_MUSIC_FILE_INDEX        "music_file_index"
#define CFG_MUSIC_FILE_PATH         "music_file_path"
#define CFG_MUSIC_TAG_INFO          "music_tag_info"
#define CFG_MUSIC_FSEL_MODE         "music_fsel_mode"


/* main menu */
#define CFG_MUSIC_NOWPLAYING_VISIBLE    "music_nowplaying_visible"   
#define CFG_MUSIC_ALLMUSIC_VISIBLE      "music_allmusic_visible"  
#define CFG_MUSIC_ARTIST_VISIBLE        "music_artist_visible"  
#define CFG_MUSIC_ALBUM_VISIBLE         "music_album_visible"  
#define CFG_MUSIC_GENRE_VISIBLE         "music_genre_visible"  
#define CFG_MUSIC_MYPLAYLIST_VISIBLE    "music_myplaylist_visible"  
#define CFG_MUSIC_DIRLIST_VISIBLE       "music_dirlist_visible"  
#define CFG_MUSIC_RECLIST_VISIBLE       "music_reclist_visible"  
#define CFG_MUSIC_MYRATING_VISIBLE      "music_myrating_visible"  
#define CFG_MUSIC_TAGLIST_VISIBLE       "music_taglist_visible"  


/* option */
#define CFG_MUSIC_SELEQ_VISIBLE         "music_seleq_visible"   
#define CFG_MUSIC_PLAYMODE_VISIBLE      "music_playmode_visible"  
#define CFG_MUSIC_DISPLAYMODE_VISIBLE   "music_displaymode_visible"  
#define CFG_MUSIC_ADDLIST_VISIBLE       "music_addlist_visible"  
#define CFG_MUSIC_SETRATING_VISIBLE     "music_setrating_visible"  
#define CFG_MUSIC_ADDTAG_VISIBLE        "music_addtag_visible"  
#define CFG_MUSIC_ABREP_VISIBLE         "music_abrep_visible"  
#define CFG_MUSIC_PLAYSPEED_VISIBLE     "music_playspeed_visible"  
#define CFG_MUSIC_DELETE_VISIBLE        "music_delete_visible"  
#define CFG_MUSIC_ADVANCED_VISIBLE      "music_advanced_visible"  
#define CFG_MUSIC_SETUPEQ_VISIBLE       "music_setupeq_visible" 
#define CFG_MUSIC_INFO_VISIBLE          "music_info_visible"   


/*
 * video应用配置
 */
#define CFG_VIDEO_SWF_SUPPORT        "video_swf_support"




/*
 * ebook应用配置
 */

#define CFG_EBOOK_HAS_CHANGE_FONT_SIZE_ITEM "ebook_has_change_font_size_item"

#define CFG_EBOOK_FONT_TYPE         "ebook_font_type"

#define CFG_EBOOK_DEFAULT_FONT_COLOR "ebook_deault_font_color"

#define CFG_EBOOK_FONT_COLOR                 "ebook_font_color"
#define CFG_EBOOK_BG_COLOR                 "ebook_bg_color"

#define CFG_EBOOK_USE_COLOR_BACKGROUND  "ebook_use_color_background"

#define CFG_EBOOK_FILE_NAME         "ebook_file_name"
#define CFG_EBOOK_FILE_FULL_PATH    "ebook_file_full_path"
#define CFG_EBOOK_PAGE_START_POINT    "ebook_page_start_point"

#define CFG_EBOOK_CUR_FONT_COLOR        "ebook_cur_font_color"
#define CFG_EBOOK_LANGUAGE_MODE     "ebook_language_mode"
#define CFG_EBOOK_AUTO_PLAY_GAP     "ebook_auto_play_gap"
#define CFG_EBOOK_FUSION_MODE     "ebook_fusion_mode"


/*
 * usb 应用配置
 */

#define CFG_USB_UDISK_PID "udisk_PID"
#define CFG_USB_UDISK_VID "udisk_VID"
#define CFG_USB_UDISK_RELEASE "udisk_release"
#define CFG_USB_UDISK_VENDOR "udisk_vendor"
#define CFG_USB_UDISK_PRODUCT "udisk_product"
#define CFG_USB_UDISK_SERIAL "udisk_serial"

#define CFG_USB_MTP_PID "mtp_PID"
#define CFG_USB_MTP_VID "mtp_VID"
#define CFG_USB_MTP_EXT "mtp_EXT"

#define CFG_USB_MTP_DEV_MANUFACTURER "mtp_dev_manufacturer"
#define CFG_USB_MTP_DEV_MODEL "mtp_dev_model"
#define CFG_USB_MTP_DEV_VERSION "mtp_dev_version"
#define CFG_USB_MTP_DEV_SERIAL "mtp_dev_serial"

#define CFG_USB_MTP_STORE_INNER_DESC "mtp_store_inner_desc"
#define CFG_USB_MTP_STORE_PORTABLE_DESC "mtp_store_portable_desc"

#define CFG_USB_MTP_DEV_EXT_DESC0 "mtp_dev_ext_desc0"
#define CFG_USB_MTP_DEV_EXT_DESC1 "mtp_dev_ext_desc1"


#define CFG_USB_SHOW_MANUFACTURER_DISK  "usb_show_manufacturer_disk"
#define CFG_USB_SHOW_U_DISK  "usb_show_u_disk"
#define CFG_USB_SHOW_CARD_DISK  "usb_show_card_disk"
#define CFG_USB_UPGRADE_FW_FILE     "usb_upgrade_fw_file"

/*
 * record应用配置
 */
#define CFG_RECORDER_FORMAT          "recorder_format"
#define CFG_RECORDER_QUALITY         "recorder_quality"
#define CFG_RECORDER_MODE            "recorder_mode"
#define CFG_RECORDER_GAIN            "recorder_gain"
#define CFG_RECORDER_PATH            "recorder_path"
#define CFG_RECORDER_LINEIN_SUPPORT  "recorder_line_in_support" 


/*
 * tvin应用配置
 */

#define    CFG_TVIN_SIGNAL               "tvin_signal"
#define    CFG_TVIN_SAVEPATH             "tvin_savepath"
#define    CFG_TVIN_RECORD_QUALITY       "tvin_record_quality"
#define    CFG_TVIN_LCD_ONOFF            "tvin_lcd_onoff"
#define    CFG_TVIN_SINGLE1_DATE         "tvin_single1_date"
#define    CFG_TVIN_SINGLE2_DATE         "tvin_single2_date"
#define    CFG_TVIN_SINGLE3_DATE         "tvin_single3_date"

#define    CFG_TVIN_SINGLE1_TIME         "tvin_single1_time"
#define    CFG_TVIN_SINGLE2_TIME         "tvin_single2_time"
#define    CFG_TVIN_SINGLE3_TIME         "tvin_single3_time"

#define    CFG_TVIN_SINGLE1_LENGTH       "tvin_single1_length"
#define    CFG_TVIN_SINGLE2_LENGTH       "tvin_single2_length"
#define    CFG_TVIN_SINGLE3_LENGTH       "tvin_single3_length"

#define    CFG_TVIN_SINGLE1_ONOFF        "tvin_single1_onoff"
#define    CFG_TVIN_SINGLE2_ONOFF        "tvin_single2_onoff"
#define    CFG_TVIN_SINGLE3_ONOFF        "tvin_single3_onoff"

#define    CFG_TVIN_REPEAT1_TIME         "tvin_repeat1_time"
#define    CFG_TVIN_REPEAT2_TIME         "tvin_repeat2_time"
#define    CFG_TVIN_REPEAT3_TIME         "tvin_repeat3_time"


#define    CFG_TVIN_REPEAT1_LENGTH       "tvin_repeat1_length"
#define    CFG_TVIN_REPEAT2_LENGTH       "tvin_repeat2_length"
#define    CFG_TVIN_REPEAT3_LENGTH       "tvin_repeat3_length"

#define    CFG_TVIN_REPEAT1_WEEKDAY      "tvin_repeat1_weekday"
#define    CFG_TVIN_REPEAT2_WEEKDAY      "tvin_repeat2_weekday"
#define    CFG_TVIN_REPEAT3_WEEKDAY      "tvin_repeat3_weekday"

#define    CFG_TVIN_REPEAT1_ONOFF        "tvin_repeat1_onoff"
#define    CFG_TVIN_REPEAT2_ONOFF        "tvin_repeat2_onoff"
#define    CFG_TVIN_REPEAT3_ONOFF        "tvin_repeat3_onoff"

#define    CFG_TVIN_SINGLE1_CLOCKNUM     "tvin_single1_clocknum"
#define    CFG_TVIN_SINGLE2_CLOCKNUM     "tvin_single2_clocknum"
#define    CFG_TVIN_SINGLE3_CLOCKNUM     "tvin_single3_clocknum"

#define    CFG_TVIN_REPEAT1_CLOCKNUM     "tvin_repeat1_clocknum"
#define    CFG_TVIN_REPEAT2_CLOCKNUM     "tvin_repeat2_clocknum"
#define    CFG_TVIN_REPEAT3_CLOCKNUM     "tvin_repeat3_clocknum"

#define    CFG_TVIN_SINGLEREC1_VISIBLE   "tvin_singlerec1_visible"
#define    CFG_TVIN_SINGLEREC2_VISIBLE   "tvin_singlerec2_visible"
#define    CFG_TVIN_SINGLEREC3_VISIBLE   "tvin_singlerec3_visible"
#define    CFG_TVIN_REPEATREC1_VISIBLE   "tvin_repeatrec1_visible"
#define    CFG_TVIN_REPEATREC2_VISIBLE   "tvin_repeatrec2_visible"
#define    CFG_TVIN_REPEATREC3_VISIBLE   "tvin_repeatrec3_visible"
#define    CFG_TVIN_SAVEPATH_VISIBLE     "tvin_savepath_visible"
#define    CFG_TVIN_LCD_ONOFF_VISIBLE    "tvin_lcd_onoff_visible"
#define    CFG_TVIN_RECORD_QUALITY_VISIBLE "tvin_record_quality_visible"

/*************************************************************************
 * setting应用界面配置
 ************************************************************************/
/* main menu */
#define CFG_MENU_THEME_VISIBLE                     "setting_menu_theme_visible"                 
#define CFG_MENU_SCR_MODE_VISIBLE                  "setting_menu_scr_mode_visible"              
#define CFG_MENU_DISPLAY_VISIBLE                   "setting_menu_display_visible"                
#define CFG_MENU_POWER_VISIBLE                     "setting_menu_power_visible"                 
#define CFG_MENU_DATE_TIME_VISIBLE                 "setting_menu_date_time_visible"               
#define CFG_MENU_LANGUAGE_VISIBLE                  "setting_menu_language_visible"               
#define CFG_MENU_TVOUT_VISIBLE                     "setting_menu_tvout_visible"                   
#define CFG_MENU_SYNC_PLIST_VISIBLE                "setting_menu_sync_plist_visible"                   
#define CFG_MENU_ADVANCED_VISIBLE                  "setting_menu_advanced_visible"               
                                           
/* screen mode menu */
#define CFG_MENU_SCR_MODE_HORIZONTAL_VISIBLE       "setting_menu_scr_mode_horizontal_visible"  
#define CFG_MENU_SCR_MODE_VERITCAL_VISIBLE         "setting_menu_scr_mode_veritcal_visible"
#define CFG_MENU_SCR_MODE_AUTO_VISIBLE             "setting_menu_scr_mode_auto_visible"
                                           
/* display menu */
#define CFG_MENU_DISPLAY_BRIGHTNESS_VISIBLE        "setting_menu_display_brightness_visible" 
#define CFG_MENU_DISPLAY_BACKLIGHT_VISIBLE         "setting_menu_display_backlight_visible" 
                                           
/* power menu */
#define CFG_MENU_POWER_IDLE_TIME_VISIBLE           "setting_menu_power_idle_time_visible" 
#define CFG_MENU_POWER_SLEEP_TIME_VISIBLE          "setting_menu_power_sleep_time_visible" 
                                           
/* date and time menu */
#define CFG_MENU_DATE_TIME_ADJUST_DATE_VISIBLE     "setting_menu_date_time_adjust_date_visible" 
#define CFG_MENU_DATE_TIME_ADJUST_TIME_VISIBLE     "setting_menu_date_time_adjust_time_visible" 
#define CFG_MENU_DATE_TIME_DRM_TIME_VISIBLE        "setting_menu_date_time_drm_time_visible" 
                                           
/* advanced menu */
#define CFG_MENU_ADVANCED_INFO_VISIBLE             "setting_menu_advanced_info_visible"  
#define CFG_MENU_ADVANCED_CONNECT_TYPE_VISIBLE     "setting_menu_advanced_connect_type_visible"  
#define CFG_MENU_ADVANCED_FONT_VISIBLE             "setting_menu_advanced_font_visible" 
#define CFG_MENU_ADVANCED_KEY_TONE_VISIBLE         "setting_menu_advanced_key_tone_visible"  
#define CFG_MENU_ADVANCED_SHOW_EFFECT_VISIBLE      "setting_menu_advanced_show_effect_visible"     
#define CFG_MENU_ADVANCED_SEARCH_FOLDER_VISIBLE    "setting_menu_advanced_search_folder_visible" 
#define CFG_MENU_ADVANCED_GSENSOR_SHAKE_SUPPORT_VISIBLE  "setting_menu_advanced_gsensor_shake_support_visible" 
#define CFG_MENU_ADVANCED_FORMAT_INTERNAL_VISIBLE  "setting_menu_advanced_format_internal_visible"     
#define CFG_MENU_ADVANCED_FORMAT_CARD_VISIBLE      "setting_menu_advanced_format_card_visible"       
#define CFG_MENU_ADVANCED_FORMAT_EXTERNAL_VISIBLE  "setting_menu_advanced_format_external_visible"   
#define CFG_MENU_ADVANCED_SET_USERNAME_VISIBLE     "setting_menu_advanced_set_username_visible"     
#define CFG_MENU_ADVANCED_CLEAR_DRM_VISIBLE        "setting_menu_advanced_clear_drm_visible"        
#define CFG_MENU_ADVANCED_CHECK_SYS_VISIBLE        "setting_menu_advanced_check_sys_visible"        
#define CFG_MENU_ADVANCED_RESET_VISIBLE            "setting_menu_advanced_reset_visible"            
                                           
/* information menu */
#define CFG_MENU_INFO_USERNAME_VISIBLE             "setting_menu_info_username_visible"          
#define CFG_MENU_INFO_VERSION_VISIBLE              "setting_menu_info_version_visible"                
#define CFG_MENU_INFO_INTERNAL_SPACE_VISIBLE       "setting_menu_info_internal_space_visible"        
#define CFG_MENU_INFO_INTERNAL_FREE_VISIBLE        "setting_menu_info_internal_free_visible"        
#define CFG_MENU_INFO_CARD_SPACE_VISIBLE           "setting_menu_info_card_space_visible"           
#define CFG_MENU_INFO_CARD_FREE_VISIBLE            "setting_menu_info_card_free_visible"            
#define CFG_MENU_INFO_EXTERNAL_SPACE_VISIBLE       "setting_menu_info_external_space_visible"        
#define CFG_MENU_INFO_EXTERNAL_FREE_VISIBLE        "setting_menu_info_external_free_visible"   

/*
 * emlator 应用配置
 * 显示模式（非全屏0，全屏1）
 * 声音输出（无声0，有声1）
 * 按键映射（8个按键：a,b,start,select,left,right,up,down）
 */
#define CFG_EMU_SAVE_FILE_PATH     "emu_save_file_path"
#define CFG_EMU_DISP_MODE          "emu_disp_mode"
#define CFG_EMU_SOUND_MODE         "emu_sound_mode"
#define CFG_EMU_KEY_A              "emu_key_a"
#define CFG_EMU_KEY_B              "emu_key_b"
#define CFG_EMU_KEY_START          "emu_key_start"
#define CFG_EMU_KEY_SELECT         "emu_key_select"     
#define CFG_EMU_KEY_LEFT           "emu_key_left"
#define CFG_EMU_KEY_RIGHT          "emu_key_right"
#define CFG_EMU_KEY_UP             "emu_key_up"
#define CFG_EMU_KEY_DOWN           "emu_key_down"


/* media path cfg define */

#define CFG_PUT_PLIST_IN_ADISK                   "put_plist_in_adisk" 
                                                                                       
#define CFG_MEDIA_DIR_UDISK_MUSIC                   "media_dir_udisk_music"            
#define CFG_MEDIA_DIR_UDISK_VIDEO                   "media_dir_udisk_video"            
#define CFG_MEDIA_DIR_UDISK_PHOTO                   "media_dir_udisk_photo"            
#define CFG_MEDIA_DIR_UDISK_EBOOK                   "media_dir_udisk_ebook"            
#define CFG_MEDIA_DIR_UDISK_VIDEO_CAMERA            "media_dir_udisk_video_camera"     
#define CFG_MEDIA_DIR_UDISK_PHOTO_CAMERA            "media_dir_udisk_photo_camera"     
#define CFG_MEDIA_DIR_UDISK_GAME                    "media_dir_udisk_game"             
#define CFG_MEDIA_DIR_UDISK_RECORDER                "media_dir_udisk_recorder"         
#define CFG_MEDIA_DIR_UDISK_RADIO_RECORD            "media_dir_udisk_radio_record"     
#define CFG_MEDIA_DIR_UDISK_RECORDER_MIC            "media_dir_udisk_recorder_mic"     
#define CFG_MEDIA_DIR_UDISK_RECORDER_LINEIN         "media_dir_udisk_recorder_linein"  
#define CFG_MEDIA_DIR_UDISK_TVIN                    "media_dir_udisk_tvin"             
#define CFG_MEDIA_DIR_UDISK_SYSTEM                  "media_dir_udisk_system"           
#define CFG_MEDIA_DIR_UDISK_SYSTEM_FM               "media_dir_udisk_system_fm"        
#define CFG_MEDIA_DIR_UDISK_SYSTEM_FONT             "media_dir_udisk_system_font"      
#define CFG_MEDIA_DIR_UDISK_SYSTEM_PLAYLIST         "media_dir_udisk_system_playlist"  
                                                                                     
#define CFG_MEDIA_DIR_CARD_MUSIC                    "media_dir_card_music"             
#define CFG_MEDIA_DIR_CARD_VIDEO                    "media_dir_card_video"             
#define CFG_MEDIA_DIR_CARD_PHOTO                    "media_dir_card_photo"             
#define CFG_MEDIA_DIR_CARD_EBOOK                    "media_dir_card_ebook"             
#define CFG_MEDIA_DIR_CARD_VIDEO_CAMERA             "media_dir_card_video_camera"      
#define CFG_MEDIA_DIR_CARD_PHOTO_CAMERA             "media_dir_card_photo_camera"      
#define CFG_MEDIA_DIR_CARD_GAME                     "media_dir_card_game"              
#define CFG_MEDIA_DIR_CARD_RECORDER                 "media_dir_card_recorder"          
#define CFG_MEDIA_DIR_CARD_RADIO_RECORD             "media_dir_card_radio_record"      
#define CFG_MEDIA_DIR_CARD_RECORDER_MIC             "media_dir_card_recorder_mic"      
#define CFG_MEDIA_DIR_CARD_RECORDER_LINEIN          "media_dir_card_recorder_linein"   
#define CFG_MEDIA_DIR_CARD_TVIN                     "media_dir_card_tvin"              
#define CFG_MEDIA_DIR_CARD_SYSTEM                   "media_dir_card_system"            
#define CFG_MEDIA_DIR_CARD_SYSTEM_FM                "media_dir_card_system_fm"         
#define CFG_MEDIA_DIR_CARD_SYSTEM_FONT              "media_dir_card_system_font"       
#define CFG_MEDIA_DIR_CARD_SYSTEM_PLAYLIST          "media_dir_card_system_playlist"   
                                                                                     
#define CFG_MEDIA_DIR_EXTERNAL_MUSIC                "media_dir_external_music"           
#define CFG_MEDIA_DIR_EXTERNAL_VIDEO                "media_dir_external_video"           
#define CFG_MEDIA_DIR_EXTERNAL_PHOTO                "media_dir_external_photo"           
#define CFG_MEDIA_DIR_EXTERNAL_EBOOK                "media_dir_external_ebook"           
#define CFG_MEDIA_DIR_EXTERNAL_VIDEO_CAMERA         "media_dir_external_video_camera"    
#define CFG_MEDIA_DIR_EXTERNAL_PHOTO_CAMERA         "media_dir_external_photo_camera"    
#define CFG_MEDIA_DIR_EXTERNAL_GAME                 "media_dir_external_game"            
#define CFG_MEDIA_DIR_EXTERNAL_RECORDER             "media_dir_external_recorder"        
#define CFG_MEDIA_DIR_EXTERNAL_RADIO_RECORD         "media_dir_external_radio_record"    
#define CFG_MEDIA_DIR_EXTERNAL_RECORDER_MIC         "media_dir_external_recorder_mic"    
#define CFG_MEDIA_DIR_EXTERNAL_RECORDER_LINEIN      "media_dir_external_recorder_linein" 
#define CFG_MEDIA_DIR_EXTERNAL_TVIN                 "media_dir_external_tvin"            
#define CFG_MEDIA_DIR_EXTERNAL_SYSTEM               "media_dir_external_system"          
#define CFG_MEDIA_DIR_EXTERNAL_SYSTEM_FM            "media_dir_external_system_fm"       
#define CFG_MEDIA_DIR_EXTERNAL_SYSTEM_FONT          "media_dir_external_system_font"     
#define CFG_MEDIA_DIR_EXTERNAL_SYSTEM_PLAYLIST      "media_dir_external_system_playlist" 

#endif  /* #ifndef __MEDIA_CONFIG_H__ */

