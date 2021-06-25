/********************************************************************************
 *                              USDK(1100)
 *                             Module: tvin 
 *                 Copyright(c) 2003-2008 Actions Semiconductor,
 *                            All Rights Reserved. 
 * file : tvin.h
 * History:         
 *      <author>     <time>        <version >            <desc>
 *      Jerry Fu     2009-4-27          v1.0             build this file 
 ********************************************************************************/
/*!
 * \file       tvin.h
 * \brief     define  data structure of tvin moudle
 * \author   Jerry Fu 
 *               
 * \version 1.0
 * \date  2009/04/28
 *******************************************************************************/

/*! 
 * \defgroup   case_tvin    tvin 模块的数据结构和接口函数
 */
 
#ifndef   __TVIN_H__
#define   __TVIN_H__


#include "tvin_scene_record.h"
#include "tvin_scene_palntsc.h"
#include "tvin_fusion.h"


/*!
 * \par  tvin.app启动的模式
 *  \li APP_START_NORMAL: 从主菜单手动进入tvin.app
 *  \li APP_START_RTC : RTC设定的录像时间到而启动tvin.app
 */
typedef  enum
{
    APP_START_NORMAL,
    APP_START_RTC,
}app_start_mode_e;


typedef struct
{
    /*! system resource */
    void *applib_lib;
    void *commonui_lib;
    void *fusion_lib;
    void *gui_lib;
    void *sty_lib;
    void *config_lib;
    /*!场景退出后进入的下一个场景*/  
    scene_tvin_ret_e  next_scene;
    /*!应用启动方式:0-正常启动，1-定时启动*/
    app_start_mode_e  app_start_mode;
    /*!录像场景指针*/
    tvin_scene_record_t *pTvinRecord;
    /*!列表场景指针*/
    tvin_scene_palntsc_t *palntsc_data;
    /*!style文件指针*/
    style_t*    tvin_style;
    style_t*    commonui_style;
    /*!定时录像启动，当前已生效定时器号*/
    int alarm_clock;
    /*!记录录像前屏幕方向*/
    rotation_e  save_angle;
    /*!video-in驱动是否已成功加载标志*/
    bool  dev_installed;
}tvin_appdata_t;


tvin_appdata_t  *fetch_app_data(void);
bool  scene_palntsc( tvin_appdata_t *app_data );
bool  scene_tvin_record(tvin_appdata_t *app_data);

#endif

