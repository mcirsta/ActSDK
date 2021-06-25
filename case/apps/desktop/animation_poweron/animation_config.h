/********************************************************************************
 *                              USDK(1100)
 *                             Module: app 
 *                 Copyright(c) 2003-2009 Actions Semiconductor,
 *                            All Rights Reserved. 
 * History:         
 *      <author>    <time>           <version >             <desc>
 *      shaofeng    2009-07-28          v1.0               build this file 
********************************************************************************/
/*!
 * \file     animation_config.h
 * \brief    
 * \author   shaofeng
 *               
 * \version 1.0
 * \date  2009/07/28
*******************************************************************************/

#ifndef __ANIMATION_CONFIG_H__
#define __ANIMATION_CONFIG_H__

#include "include_psp.h"
#include "include_case.h"


//���ٵ���ʾ��ʽ, ���ڶ���ʱ�����ó�ʱ��, ��ϵͳ��̨������������
#define CFG_ANIMATION_FAST_DISPLAY        1 

//���ٵ� ��resͼƬ����ʾ��ʽÿ֡�м��usleepʱ��
#define CFG_ANIMATION_RES_USLEEP_TIME        (20*1000) 

//����gifͼƬ����ʾ��ʽÿ֡�м��usleepʱ��
#define CFG_ANIMATION_GIF_USLEEP_TIME        (20*1000) 




/*!
* scene type define
*/
typedef enum
{
    SCENE_ERR = 0,
    SCENE_EXIT,
    SCENE_RETURN,
    
    SCENE_LIST,
    SCENE_PLAYING,
}scene_type_e;

/* scene of all apps */
typedef struct
{
    scene_type_e scene_type;
    
    /* style resource */
    scene_t* scene;
    
    /* window resource */
    int win_id;
    int handle_gui_dc;
}app_scene_t;






/************************ extern function, �������� ***************************/
extern bool animation_cfg_read_start_mode( void );
extern bool animation_cfg_read_filename( rotation_e angle, char *filename );
extern bool animation_cfg_read_def_filename( rotation_e angle, char *filename );



#endif  /* __ANIMATION_CONFIG_H__ */

