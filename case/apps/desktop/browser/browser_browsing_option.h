/********************************************************************************
 *                              USDK(1100)
 *                             Module: app
 *                 Copyright(c) 2003-2009 Actions Semiconductor,
 *                            All Rights Reserved.
 * File : browser_option.h
 * History:
 *      <author>    <time>           <version >             <desc>
 *      binannan      2009-05-09        v1.0               build this file
 ********************************************************************************/

#ifndef __BROWSER_BROWSING_OPTION_H__
#define __BROWSER_BROWSING_OPTION_H__

#include "include_case.h"
#include "include_psp.h"

#include "browser_sty.h"



/* 进入主菜单 */
bool browser_browsing_option_enter( menuview_t *menuview );

/* 退出主菜单 */
bool browser_browsing_option_exit( menuview_t *menuview );

/* menu data init */
menu_t* browser_browsing_option_init( void );
int browser_browsing_option_deinit( void );

/* 删除操作 */
int opt_delete_process(menuview_t *data, menuview_sig_e signal);

#endif
