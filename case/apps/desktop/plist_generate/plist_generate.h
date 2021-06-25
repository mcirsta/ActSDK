/*******************************************************************************
 *                              USDK(1100)
 *                             Module: app 
 *                 Copyright(c) 2003-2009 Actions Semiconductor,
 *                            All Rights Reserved. 
 * History:         
 *      <author>    <time>           <version >             <desc>
 *      shaofeng    2009-05-25          v1.0               build this file 
*******************************************************************************/
/*!
 * \file     plist_generate.h
 * \brief    
 * \author   shaofeng
 *               
 * \version 1.0
 * \date  2009/05/25
*******************************************************************************/

#ifndef __PLIST_GENERATE_H__
#define __PLIST_GENERATE_H__

#include "include_case.h"


/*本应用的头文件*/
#include "plist_generate_scene.h"


#define PLIST_GENERATE_TOTAL_PERCENT    (100)

#define PLIST_GENERATE_TIMER_INTERVAL   (300)

typedef struct
{
    /* scene play resource */
    plist_generate_scene_t scene;
    
    /* system resource */
    void *applib_lib;
    void *commonui_lib;
    void *apconfig_lib;
    void *gui_lib;
    void *sty_lib;
    int fontdesc;
    
    void *sqlite_lib;
    void *mnavi_lib;
    void *playlist_lib;
    
    style_t *com_style;
    style_t *app_style;
}plist_generate_vars_t;



/************************ extern variable, 变量声明 ***************************/
extern plist_generate_vars_t plist_generate_vars;


/************************ extern function, 函数声明 ***************************/
extern int plist_generate_proc_sys_msg(msg_apps_t *msg);



#endif  /* __PLIST_GENERATE_H__ */

