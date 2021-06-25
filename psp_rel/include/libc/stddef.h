/********************************************************************************
 *                              USDK(1100)
 *                            Module: TYPES
 *                 Copyright(c) 2003-2008 Actions Semiconductor,
 *                            All Rights Reserved.
 *
 * History:
 *      <author>    <time>           <version >             <desc>
 *       wurui     2009-1-4 16:42     1.0             build this file
 ********************************************************************************/
/*!
 * \file     stddef.h
 * \brief    标准宏定义
 * \author   wurui
 * \version 1.0
 * \date  2009/01/04 16:42
 *******************************************************************************/

#ifndef _STDDEF_H
#define _STDDEF_H

#undef restrict
#define restrict

#undef NULL
#define NULL ((void *)0)

//enum {
//	false	= 0,
//	true	= 1
//};

#undef offsetof
#define offsetof(TYPE, MEMBER) ((size_t) &((TYPE *)0)->MEMBER)

#endif /* _STDDEF_H*/
