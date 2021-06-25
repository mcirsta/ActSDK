/*
 ********************************************************************************
 *                   video in device's driver
 *                (c) Copyright 2007 - 2010, Actions Co,Ld.
 *                        All Right Reserved
 *
 * History:
 *      <author>      <time>       <version >    <desc>
 *       houjingkun      2009/01/23     1.0      first version.
 ********************************************************************************/
/*!
 * \file   sensor.h
 * \brief
 *      sensor 驱动独有的接口头文件，当视频输入设备为sensor的时候，需要使用到该头文件里面的接口。
 * \author houjingkun
 * \par GENERAL DESCRIPTION:
 *      sensor 驱动独有的接口头文件，当视频输入设备为sensor的时候，需要使用到该头文件里面的接口。
 * \par EXTERNALIZED FUNCTIONS:
 *       null
 *
 *     Copyright(c) 2008-2012 Actions Semiconductor, All Rights Reserved.
 *
 *  \version 1.1
 *  \date  2008/06/18
 *******************************************************************************/

#ifndef __SENSOR_H__
#define __SENSOR_H__

/********************************************************/
/* below is video in driver ioctl func's command. */
#define SENSOR_CMD_BASE   0x47400
/********************************************************/

/* sensor's some param. */
#define SENSOR_IOSET_EXPOSE_MODE    (SENSOR_CMD_BASE + 1)
#define SENSOR_IOSET_SPECIAL_MODE   (SENSOR_CMD_BASE + 2)
#define SENSOR_IOSET_CONTRAST       (SENSOR_CMD_BASE + 3)
#define SENSOR_IOSET_WHITE_BALANCE  (SENSOR_CMD_BASE + 4)
#define SENSOR_IOSET_FOCUS          (SENSOR_CMD_BASE + 5)
#define SENSOR_IOSET_BRIGHTNESS     (SENSOR_CMD_BASE + 6)
#define SENSOR_IOSET_FLASHLIGHT     (SENSOR_CMD_BASE + 7)

#define SENSOR_IOGET_EXPOSE_MODE    (SENSOR_CMD_BASE + 11)
#define SENSOR_IOGET_SPECIAL_MODE   (SENSOR_CMD_BASE + 12)
#define SENSOR_IOGET_CONTRAST       (SENSOR_CMD_BASE + 13)
#define SENSOR_IOGET_WHITE_BALANCE  (SENSOR_CMD_BASE + 14)
#define SENSOR_IOGET_FOCUS          (SENSOR_CMD_BASE + 15)
#define SENSOR_IOGET_BRIGHTNESS     (SENSOR_CMD_BASE + 16)

/*
 * ******* below's enum type is defined for sensor's effect param. *******
 * SS_XXX means sensor's something.
 */

/*!
 * VI_IOSET_WHITE_BALANCE used.
 * \li SS_AUTO_BALANCE
 * \li SS_INDOOR_BALANCE
 * \li SS_OUTDOOR_BALANCE
 * \li SS_FLUORE_BALANCE
 */
typedef enum
{
    SS_AUTO_BALANCE,
    SS_INDOOR_BALANCE,
    SS_OUTDOOR_BALANCE,
    SS_FLUORE_BALANCE,
    SS_CLOUDY_BALANCE
} ss_white_balance_t; /* white balance param. */

/*!
 * VI_IOSET_EXPOSE_MODE used.
 * \li SS_AUTO_EXPOSURE
 * \li SS_INDOOR_EXPOSURE
 * \li SS_OUTDOOR_EXPOSURE
 */
typedef enum
{
    SS_AUTO_EXPOSURE,
    SS_INDOOR_EXPOSURE,
    SS_OUTDOOR_EXPOSURE
} ss_expose_mode_t; /* exposure param. */

/*!
 * VI_IOSET_SPECIAL_MODE used.
 * \li SS_NONE_SPECIAL
 * \li SS_WHITE_AND_BLACK
 * \li SS_SEPIA
 * \li SS_SOLARIZE
 */
typedef enum
{
    SS_NONE_SPECIAL,
    SS_WHITE_AND_BLACK,
    SS_SEPIA,
    SS_SOLARIZE
} ss_special_effect_t; /* special effect param. */

typedef enum
{
    SS_FL_OFF,
    SS_FL_ON,
    SS_FL_AUTO
} ss_flashlight_t; /* special effect param. */

#endif  /* __SENSOR_H__ */

