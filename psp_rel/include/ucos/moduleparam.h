/********************************************************************************
 *                               USDK(1100)
 *                            Module: 内核模块
 *                 Copyright(c) 2003-2008 Actions Semiconductor,
 *                            All Rights Reserved.
 *
 * History:
 *      <author>    <time>           <version >             <desc>
 *       hmwei     2008-6-12 9:42     1.0             build this file
 ********************************************************************************/
/*!
 * \file     moduleparam.h
 * \brief    内核模块相关宏定义
 * \author   open source
 * \version 1.0
 * \date  2008/6/12
 *******************************************************************************/
#ifndef __MODULEPARAM_H__
#define __MODULEPARAM_H__

#include <ucos/init.h>
#include <ucos/stringify.h>
#include <ucos/kernel.h>

#define ___module_cat(a,b) __mod_ ## a ## b
#define __module_cat(a,b) ___module_cat(a,b)
#define __MODULE_INFO(tag, name, info)					  \
static const char __module_cat(name,__LINE__)[]				  \
  __attribute__((section(".modinfo"),unused)) = __stringify(tag) "=" info

#define __MODULE_PARM_TYPE(name, _type)					  \
  __MODULE_INFO(parmtype, name##type, #name ":" _type)

#endif  /* __MODULEPARAM_H__ */
