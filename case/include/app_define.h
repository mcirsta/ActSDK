/********************************************************************************
 *                              USDK(1100)
 *                             Module: app
 *                 Copyright(c) 2003-2009 Actions Semiconductor,
 *                            All Rights Reserved.
 * History:
 *      <author>    <time>           <version >             <desc>
 *      shaofeng    2009-06-10          v1.0               build this file
 ********************************************************************************/
/*!
 * \file     app_define.h
 * \brief
 * \author   shaofeng
 *
 * \version 1.0
 * \date  2009/06/10
 *******************************************************************************/
#ifndef __APP_DEFINE_H__
#define __APP_DEFINE_H__

#include "include_case.h"

#define ADISK_ROOT                         "/mnt/diska"
#define UDISK_ROOT                         "/mnt/disk0"
#define CARD_ROOT                          "/mnt/card"
#define EXTERNAL_ROOT                      "/mnt/external"

/*!
 * �����б������ϵͳ�����Դ���·���궨��
 * ����Ӧ�ó���ʹ�õģ��û�һ�������޸ĵ��ļ���Ŵ�
 */
#define ADISK_SYSTEM                ADISK_ROOT"/sys/"
#define ADISK_SYSTEM_PLAYLIST       ADISK_ROOT"/sys/"

/*!
 * ��Ӧ�õĽ��� ����
 * ����Ӧ�ü������Ϣ����, ͳһʹ�ú���
 */
#define APP_NAME_MANAGER       ADISK_ROOT"/apps/manager.app"
#define APP_NAME_MSG_SERV      ADISK_ROOT"/apps/msg_serv.app"
#define APP_NAME_CONFIG        ADISK_ROOT"/apps/config.app"

#define APP_NAME_USB           ADISK_ROOT"/apps/usb/usb.app"
#define APP_NAME_BROWSER       ADISK_ROOT"/apps/browser/browser.app"
#define APP_NAME_CAMERA        ADISK_ROOT"/apps/camera/camera.app"
#define APP_NAME_EBOOK         ADISK_ROOT"/apps/ebook/ebook.app"
#define APP_NAME_LAUNCHER      ADISK_ROOT"/apps/launcher/launcher.app"
#define APP_NAME_MUSIC         ADISK_ROOT"/apps/music/music.app"
#define APP_NAME_PHOTO         ADISK_ROOT"/apps/photo/photo.app"
#define APP_NAME_RADIO         ADISK_ROOT"/apps/radio/radio.app"
#define APP_NAME_RECORDER      ADISK_ROOT"/apps/recorder/recorder.app"
#define APP_NAME_SETTING       ADISK_ROOT"/apps/setting/setting.app"
#define APP_NAME_TVIN          ADISK_ROOT"/apps/tvin/tvin.app"
#define APP_NAME_VIDEO         ADISK_ROOT"/apps/video/video.app"
#define APP_NAME_EMULATOR      ADISK_ROOT"/apps/emulator/emulator.app"
#define APP_NAME_ANIM_ON       ADISK_ROOT"/apps/anim_on/anim_on.app"
#define APP_NAME_ANIM_OFF      ADISK_ROOT"/apps/anim_off/anim_off.app"
#define APP_NAME_PL_GEN        ADISK_ROOT"/apps/pl_gener/pl_gener.app"

#define APP_NAME_MUSIC_ENGINE  ADISK_ROOT"/apps/m_engine.app"
#define APP_NAME_RADIO_ENGINE  ADISK_ROOT"/apps/fmengine.app"


#define DRM_HDS_FILE_PATH       "/mnt/sdisk/HDSFrame.hds"

/*!
 * ����ͼ·��������
 */
#define APP_BG_UPRIGHT_ROOT     "/mnt/diska/bgpic/upright"
#define APP_BG_SIDELONG_ROOT    "/mnt/diska/bgpic/sidelong"

#define BG_MUSIC                "bg_music.bin"
#define BG_PHOTO                "bg_photo.bin"
#define BG_VIDEO                "bg_video.bin"
#define BG_EBOOK                "bg_ebook.bin"
#define BG_CAMERA               "bg_camera.bin"
#define BG_RADIO                "bg_radio.bin"
#define BG_RECORDER             "bg_recorder.bin"
#define BG_TVIN                 "bg_tvin.bin"
#define BG_SETTING              "bg_setting.bin"
#define BG_LAUNCHER             "bg_launcher.bin"
#define BG_CALCULATOR           "bg_calculator.bin"
#define BG_CALENDAR             "bg_calendar.bin"
#define BG_STOPWATCH            "bg_stopwatch.bin"
#define BG_EMULATOR             "bg_emulator.bin"
#define BG_BROWSER              "bg_browser.bin"
#define BG_POWERON              "bg_poweron.bin"
#define BG_POWEROFF             "bg_poweroff.bin"

#define BG_ALL  (   BG_MUSIC ";" \
                    BG_PHOTO ";" \
                    BG_VIDEO ";" \
                    BG_EBOOK ";" \
                    BG_CAMERA ";" \
                    BG_RADIO ";" \
                    BG_RECORDER ";" \
                    BG_TVIN ";" \
                    BG_SETTING ";" \
                    BG_LAUNCHER ";" \
                    BG_CALCULATOR ";" \
                    BG_CALENDAR ";" \
                    BG_STOPWATCH ";" \
                    BG_EMULATOR ";" \
                    BG_BROWSER ";" \
                    BG_POWERON ";" \
                    BG_POWEROFF ";" \
                )
                
/*!
 * ��Ӧ�����ڲ����б�ı���
 */
#define TBLNAME_MUSIC_NOWPLAYING    "npmusiclist"
#define TBLNAME_VIDEO_NOWPLAYING    "vide_nplist"
#define TBLNAME_PHOTO_DEFAULT       "photo_np_list"
#define TBLNAME_PHOTO_SPECIAL       "photo_np_list2"

/*!
 * �������ñ���
 */
#define TBLNAME_MUSIC_SUBLIB        "submusiclib"

#endif  /* __APP_DEFINE_H__ */

