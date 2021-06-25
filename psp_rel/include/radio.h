/*! \cond FM_API*/

/********************************************************************************
 *                              NOYA
 *                            Module: FM Drvier
 *                 Copyright(c) 2001-2007 Actions Semiconductor,
 *                            All Rights Reserved.
 *
 * History:
 *      <author>    <time>           <version >             <desc>
 *       xkping     2008-07-03 09:00     1.0             build this file
 ********************************************************************************/
/*!
 * \file   radio.h
 * \brief  describle the command to access the FM driver and the data struct used in the driver
 * \author xkping
 * \par GENERAL DESCRIPTION:
 *       In This File
 *       1)Define some Commands which are used to interact with the driver
 *       2)Define some data type which will be used in driver and FM application
 * \par EXTERNALIZED FUNCTIONS:
 *     Copyright(c) 2001-2007 Actions Semiconductor, All Rights Reserved.
 *
 *  \version 1.0
 *  \date  2008/7/3
 *******************************************************************************/
#ifndef __RADIO_H__
#define __RADIO_H__

#define RADIO_GET_STEREO_MODE     0x1          // 1:stereo mode; 0 singled mode 获取电台频段模式
#define RADIO_GET_INFO            0X2          //获取当前radio的一些状态信息
#define RADIO_GET_INTENSITY       0x3          //获取电台的信号强度
#define RADIO_GET_BAND_MODE       0x4          //获取电台波段
#define RADIO_GET_FREQ            0x5          //获取电台频率
#define RADIO_GET_ANTENNA         0x6          //获取天线是否插入
#define RADIO_SET_FREQ            0x31        //设置收音频率
#define RADIO_SET_THRESHOLD       0x32        //设置搜索电台的信号强度门限值
#define RADIO_SET_BAND_MODE       0x33        // 0 :US mode; 1:EURO mode 2：jpan mode
#define RADIO_SET_PLAY            0x34        //播放FM声音，打开 dac FM mute和FM IC MUTE，
#define RADIO_SET_STOP            0x35        //停止FM声音，关闭 dac FM mute和FM IC MUTE
#define RADIO_SET_ENTER_STANDBY   0x36        //设置FM 进入standby
#define RADIO_SET_AUTO_SEEK       0x37        //设置FM 自动搜台
#define RADIO_SET_AUTO_SEEK_BREAK 0x38        //跳出本次自动搜台
#define RADIO_SET_FREQ_NO_CHECK   0X39

#define RADIO_CHECK_LINEIN        0x50        //检测linein线是否插入；

/*!
 *  波段 \n
 *  \li US :   美洲波段
 *  \li EURO:  欧洲波段
 *  \li JAPAN: 日本波段
 *
 */
enum BAND_MODE
{
    US = 0,
    EURO,
    JAPAN
};

/*!
 *  搜索方向 \n
 *  \li AUTO_ADD :   自增方向
 *  \li AUTO_MINUS:  自减方向
 *
 */
enum SEEK_DIRECTION
{
    AUTO_MINUS = 0,
    AUTO_ADD
};

enum SEEK_STATUS
{
    FOUND_STATION = 0,
    FOUND_NOSTAION,
    INVALID_STATION,
    REACH_BANDLIMIT

};
enum CHECK_LINEIN_STATE
{
    LINEIN_IN = 0,
    LINEIN_OUT
};

//#define ADD_FM_PLAY_STANDBY         0           // 控制在FM的正在收听界面,进STANDBY
//#define SWITCH_LOW_OSC_SEARCH       0           // 控制在96MHz附近用低频搜索

/*!
 * \brief
 *radio_info_t,收音机当前状态信息.
 */
struct radio_info_t
{
    /*!当前电台的频率*/
    unsigned int current_freq;

    /*!当前电台的声道信息*/
    unsigned int stereo_status;

    /*!当前电台的信号强度 */
    unsigned int intensity;

    /*!当前电台的波段模式*/
    unsigned int band_mode;
    /*!搜索模式下的搜索状态*/
    unsigned int seek_status;

};

/*!
 * \brief
 *seek_info_t,搜索条件设置.
 */
struct radio_seek_info_t
{
    /*!开始搜索频率，单位是KHZ */
    unsigned int search_freq;

    /*!搜索步长，单位是KHZ */
    unsigned int search_step;

    /*!搜索方向，AUTO_ADD，增加方向；AUTO_MINUS: 自减方向*/
    unsigned int search_direction;

};
/*! \endcond*/
#endif //end of   '#ifndef FM_DRV_H'

