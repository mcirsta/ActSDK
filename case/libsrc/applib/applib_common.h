/********************************************************************************
 *                              USDK(1100)
 *                             Module: app 
 *                 Copyright(c) 2003-2008 Actions Semiconductor,
 *                            All Rights Reserved. 
 * file : applib_common.h
 * History:         
 *      <author>    <time>           <version >             <desc>
 *      Herb Hu    2009-3-24             v1.0             build this file 
 ********************************************************************************/
/*!
 * \file     applib_common.h
 * \brief    applib库公用接口 
 * \author   Herb Hu 
 * \version 1.0
 * \date  2009/03/24
 *******************************************************************************/



#ifndef __APPLIB_COMMON_H__
#define __APPLIB_COMMON_H__

#include <libc.h>
#include <applib.h>


extern inline bool is_applib_inited(void);
extern const app_info_t * g_this_app_info;
extern bool g_applib_inited;

#endif /*#ifndef __APPLIB_COMMON_H__*/
