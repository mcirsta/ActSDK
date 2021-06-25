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
 * \defgroup   case_tvin    tvin ģ������ݽṹ�ͽӿں���
 */
 
#ifndef   __TVIN_H__
#define   __TVIN_H__


#include "tvin_scene_record.h"
#include "tvin_scene_palntsc.h"
#include "tvin_fusion.h"


/*!
 * \par  tvin.app������ģʽ
 *  \li APP_START_NORMAL: �����˵��ֶ�����tvin.app
 *  \li APP_START_RTC : RTC�趨��¼��ʱ�䵽������tvin.app
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
    /*!�����˳���������һ������*/  
    scene_tvin_ret_e  next_scene;
    /*!Ӧ��������ʽ:0-����������1-��ʱ����*/
    app_start_mode_e  app_start_mode;
    /*!¼�񳡾�ָ��*/
    tvin_scene_record_t *pTvinRecord;
    /*!�б���ָ��*/
    tvin_scene_palntsc_t *palntsc_data;
    /*!style�ļ�ָ��*/
    style_t*    tvin_style;
    style_t*    commonui_style;
    /*!��ʱ¼����������ǰ����Ч��ʱ����*/
    int alarm_clock;
    /*!��¼¼��ǰ��Ļ����*/
    rotation_e  save_angle;
    /*!video-in�����Ƿ��ѳɹ����ر�־*/
    bool  dev_installed;
}tvin_appdata_t;


tvin_appdata_t  *fetch_app_data(void);
bool  scene_palntsc( tvin_appdata_t *app_data );
bool  scene_tvin_record(tvin_appdata_t *app_data);

#endif

