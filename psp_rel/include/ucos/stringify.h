/********************************************************************************
 *                               USDK(1100)
 *                            Module: STRINGIFY
 *                 Copyright(c) 2003-2008 Actions Semiconductor,
 *                            All Rights Reserved.
 *
 * History:
 *      <author>    <time>           <version >             <desc>
 *       hmwei     2008-6-12 9:42     1.0             build this file
 ********************************************************************************/
/*!
 * \file     stringify.h
 * \brief    ×Ö·û´®»¯ºê¶¨Òå
 * \author   open source
 * \version 1.0
 * \date  2008/6/12
 *******************************************************************************/
#ifndef __STRINGIFY_H__
#define __STRINGIFY_H__

/* Indirect stringification.  Doing two levels allows the parameter to be a
 * macro itself.  For example, compile with -DFOO=bar, __stringify(FOO)
 * converts to "bar".
 */

#define __stringify_1(x)	#x
#define __stringify(x)		__stringify_1(x)

#endif	/* __STRINGIFY_H__ */
