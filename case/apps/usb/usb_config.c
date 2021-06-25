/********************************************************************************
 *                              USDK(1100)
 *                             Module: app
 *                 Copyright(c) 2003-2009 Actions Semiconductor,
 *                            All Rights Reserved.
 * File : usb_config.c
 * History:
 *      <author>    <time>           <version >             <desc>
 *    zhaowenjie    2009-06-19          v1.0               build this file
 ********************************************************************************/
/*!
 * \file     usb_config.c
 * \brief    usb config
 * \author   zhaowenjie
 *
 * \version 1.0
 * \date  2009/06/19
 *******************************************************************************/

#include "include_case.h"

#include "usb_config.h"
#include "usb.h"


bool get_configs(void)
{
    int value;
    char string[ PATH_MAX ];
    int ret;

    /* UDISK 相关的配置 */

    /* pid */
    ret = get_config( CFG_USB_UDISK_PID, (char*)&value , INT_LENGTH );
    if ( ret >= 0 )
    {
        g_scene_udisk->set_pid( value );
    }

    /* vid */
    ret = get_config( CFG_USB_UDISK_VID, (char*)&value , INT_LENGTH );
    if ( ret >= 0 )
    {
        g_scene_udisk->set_vid( value );
    }

    /* release */
    ret = get_config( CFG_USB_UDISK_RELEASE, (char*)&value , INT_LENGTH );
    if ( ret >= 0 )
    {
        g_scene_udisk->set_release( value );
    }

    /* vendor */
    memset( string, 0, PATH_MAX );
    ret = get_config( CFG_USB_UDISK_VENDOR, (char*)string , PATH_MAX );
    if ( ret >= 0 )
    {
        g_scene_udisk->set_vendor( string );
    }

    /* product */
    memset( string, 0, PATH_MAX );
    ret = get_config( CFG_USB_UDISK_PRODUCT, (char*)string , PATH_MAX );
    if ( ret >= 0 )
    {
        g_scene_udisk->set_product( string );
    }

    /* serial */
    memset( string, 0, PATH_MAX );
    ret = get_config( CFG_USB_UDISK_SERIAL, (char*)string , PATH_MAX );
    if ( ret >= 0 )
    {
        g_scene_udisk->set_serial( string );
    }
    

    /////////////////////////////////////////////////////////////////
    /* MTP 相关的配置 */

    /* pid */
    ret = get_config( CFG_USB_MTP_PID, (char*)&value , INT_LENGTH );
    if ( ret >= 0 )
    {
        g_scene_mtp->set_mtp_pid( value );
    }

    /* vid */
    ret = get_config( CFG_USB_MTP_VID, (char*)&value , INT_LENGTH );
    if ( ret >= 0 )
    {
        g_scene_mtp->set_mtp_vid( value );
    }

    /* ext */
    ret = get_config( CFG_USB_MTP_EXT, (char*)&value , INT_LENGTH );
    if ( ret >= 0 )
    {
        g_scene_mtp->set_mtp_ext( value );
    }

    /* mtp_dev_manufacturer */
    memset( string, 0, PATH_MAX );
    ret = get_config( CFG_USB_MTP_DEV_MANUFACTURER, (char*)string , PATH_MAX );
    if ( ret >= 0 )
    {
        g_scene_mtp->set_mtp_dev_manufacturer( string );
    }

    /* mtp_dev_model */
    memset( string, 0, PATH_MAX );
    ret = get_config( CFG_USB_MTP_DEV_MODEL, (char*)string , PATH_MAX );
    if ( ret >= 0 )
    {
        g_scene_mtp->set_mtp_dev_model( string );
    }

    /* mtp_dev_version */
    memset( string, 0, PATH_MAX );
    ret = get_config( CFG_USB_MTP_DEV_VERSION, (char*)string , PATH_MAX );
    if ( ret >= 0 )
    {
        g_scene_mtp->set_mtp_dev_version( string );
    }

    /* mtp_dev_serial */
    memset( string, 0, PATH_MAX );
    ret = get_config( CFG_USB_MTP_DEV_SERIAL, (char*)string , PATH_MAX );
    if ( ret >= 0 )
    {
        g_scene_mtp->set_mtp_dev_serial( string );
    }

    /* mtp_store_inner_desc */
    memset( string, 0, PATH_MAX );
    ret = get_config( CFG_USB_MTP_STORE_INNER_DESC, (char*)string , PATH_MAX );
    if ( ret >= 0 )
    {
        g_scene_mtp->set_mtp_store_inner_desc( string );
    }

    /* mtp_store_portable_desc */
    memset( string, 0, PATH_MAX );
    ret = get_config( CFG_USB_MTP_STORE_PORTABLE_DESC, (char*)string , PATH_MAX );
    if ( ret >= 0 )
    {
        g_scene_mtp->set_mtp_store_portable_desc( string );
    }

    /* mtp_dev_ext_desc0 */
    memset( string, 0, PATH_MAX );
    ret = get_config( CFG_USB_MTP_DEV_EXT_DESC0, (char*)string , PATH_MAX );
    if ( ret >= 0 )
    {
        g_scene_mtp->set_mtp_dev_ext_desc0( string );
    }

    /* mtp_dev_ext_desc1 */
    memset( string, 0, PATH_MAX );
    ret = get_config( CFG_USB_MTP_DEV_EXT_DESC1, (char*)string , PATH_MAX );
    if ( ret >= 0 )
    {
        g_scene_mtp->set_mtp_dev_ext_desc1( string );
    }
    

    /////////////////////////////////////////////////////////////////////

    ret = get_config( CFG_DEFAULT_FONT, (char*)g_usb_main->get_font_path(), PATH_MAX  );
    if ( ret < 0 )
    {
        print_err(" get system default font failed ! ");
        return false;
    }
    
    /* 是否弹出厂商分区 */
    ret = get_config( CFG_USB_SHOW_MANUFACTURER_DISK, (char*)&value , INT_LENGTH );
    if ( ret >= 0 )
    {
        if ( value == 1 )
        {
            g_scene_udisk->set_show_manufacturer_disk();
        }
    }

    /* 是否弹出 U盘 分区 */
    ret = get_config( CFG_USB_SHOW_U_DISK, (char*)&value , INT_LENGTH );
    if ( ret >= 0 )
    {
        if ( value == 1 )
        {
            g_scene_udisk->set_show_u_disk();
        }
    }

        /* 是否弹出厂商分区 */
    ret = get_config( CFG_USB_SHOW_CARD_DISK, (char*)&value , INT_LENGTH );
    if ( ret >= 0 )
    {
        if ( value == 1 )
        {
            g_scene_udisk->set_show_card_disk();
        }
    }

    ret = get_config( CFG_USB_UPGRADE_FW_FILE, (char*)g_scene_upgrade->get_upgrade_fw_file(), PATH_MAX  );

    return true;
}

