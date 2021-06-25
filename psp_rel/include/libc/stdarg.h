/********************************************************************************
 *                              NOYA  1100
 *                            Module: libc_sysindep_api
 *                 Copyright(c) 2003-2008 Actions Semiconductor,
 *                            All Rights Reserved.
 *
 * History:
 *      <author>    <time>           <version >             <desc>
 *       lbkang     2008-08-07 15:00     1.0             build this file
 ********************************************************************************/
/*!
 * \file     stdarg.h
 * \brief    可选性变量列表函数定义
 * \author   lbkang
 * \version 1.0
 * \date  2008/08/07
 *******************************************************************************/
#ifndef __STDARG_H__
#define __STDARG_H__
#include "stddef.h"
#ifndef va_arg

#ifndef _VALIST
#define _VALIST
typedef char *va_list;
#endif /* _VALIST */

/*
 * Storage alignment properties
 */
#define  _AUPBND                (sizeof (unsigned int) - 1)
#define  _ADNBND                (sizeof (unsigned int) - 1)

/*
 * Variable argument list macro definitions
 */
#define _bnd(X, bnd)            (((sizeof (X)) + (bnd)) & (~(bnd)))
#define va_arg(ap, T)           (*(T *)(((ap) += (_bnd (T, _AUPBND))) - (_bnd (T,_ADNBND))))
#define va_end(ap)              (void) ((ap) = 0)
#define va_start(ap, A)         (void) ((ap) = (((char *) &(A)) + (_bnd (A,_AUPBND))))

#endif /* va_arg */
#endif
