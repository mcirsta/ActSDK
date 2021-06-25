/********************************************************************************
*                            Module: libc_general
*                 Copyright(c) 2001-2008 Actions Semiconductor,
*                            All Rights Reserved. 
*
* History:         
*      <author>         <time>           <version >             <desc>
*       chencm      2009-08-7 10:00     1.0             build this file 
********************************************************************************/
/*!
 * 
 * \brief  libc中需要常驻内存的接口的注册
 * \author wurui
 * \par 相关描述: 
 * libc实现中需要常驻内存部分的接口封装
 * \par EXTERNALIZED FUNCTIONS:
 *     Copyright(c) 2001-2008 Actions Semiconductor, All Rights Reserved.
 *
 *  \version 1.0
 *  \date  2008/09/12
 *******************************************************************************/


#include "include_case.h"
#include "./../launcher_scene_desktop.h"

static const unsigned int desktop_apitbl[]=
{
    (unsigned int)launcher_scene_desktop,

};                
                  
/******************************************************************************/
/*!                    
* \par  Description:
*      init函数
* \note
* 使用宏__init确保函数定位在.ctors段
*******************************************************************************/
int __attribute__((constructor)) desktop_api_init(void)
{
    api_install(APINO_DESKTOP, (unsigned)desktop_apitbl);
    return 0;
}

/******************************************************************************/
/*!                    
* \par  Description:
*      exit函数
* \note
* 使用宏__exit确保函数定位在.dtors段
*******************************************************************************/
void __attribute__((destructor)) desktop_api_exit(void)
{
    api_uninstall(APINO_DESKTOP);
    return;
}

