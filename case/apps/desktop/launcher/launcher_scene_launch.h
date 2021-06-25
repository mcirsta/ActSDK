/********************************************************************************
 *                              USDK(1100)
 *                             Module: app
 *                 Copyright(c) 2003-2009 Actions Semiconductor,
 *                            All Rights Reserved.
 * File : launcher_scene_launch.h
 * History:
 *      <author>    <time>           <version >             <desc>
 *       chencm    2009-0-23          v1.0               build this file
 ********************************************************************************/
/*!
* \file     launcher_scene_launch.h
* \brief    launch scene with no gui window
* \author   chencm
*
* \version 1.0
* \date  2009/04/23
*******************************************************************************/


#ifndef __LAUNCHER_SCENE_LAUNCH_H__
#define __LAUNCHER_SCENE_LAUNCH_H__

#include <include_case.h>

#include "launcher.h"
#include "desktop_appinfo.h"

//typedef enum
//{
//	SCENE_LAUNCH_EXIT,
//	SCENE_LAUNCH_ENTER_DESKTOP,
//}scene_launch_ret_e;

/* define scene data */
typedef struct
{
    appinfo_list_t * launch_app;
    scene_status_e exit_type;

}scene_launch_t;

/* scene entry */
int launcher_scene_launch(scene_launch_t * data);

extern bool language_changed;
extern bool app_exited;

#endif /* #ifndef __LAUNCHER_SCENE_LAUNCH_H__*/
