/********************************************************************************
 *                              USDK(1100)
 *                             Module: app
 *                 Copyright(c) 2003-2009 Actions Semiconductor,
 *                            All Rights Reserved.
 * History:
 *      <author>    <time>           <version >             <desc>
 *      dupengcheng    2009-04-01          v1.0               build this file
 ********************************************************************************/
/*!
 * \file     stopwatch_ctl.h
 * \brief
 * \author   dupengcheng
 *
 * \version 1.0
 * \date  2009/04/30
 *******************************************************************************/

#ifndef __STOPWATCH_CTL_H__
#define __STOPWATCH_CTL_H__

#include "stopwatch_scene.h"


#define MAX_WATCH 6
#define STATE_RUNNING 	0
#define STATE_PAUSE     1
#define STATE_STOP      2

typedef struct
{
    int hour;
    int minutes;
    int seconds;
    int hundredSeconds;
} watchvalue_t;


extern bool paint_when_rotate(void);
extern bool watch_menu(scene_stopwatch_t* my_scene);
extern void startNextWatch(scene_stopwatch_t* my_scene);
extern bool startCurWatch(scene_stopwatch_t* my_scene);



#endif   /* __STOPWATCH_CTL_H__ */
