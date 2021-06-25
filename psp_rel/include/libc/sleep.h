/********************************************************************************
 *                              USDK(1100)
 *                            Module: SLEEP
 *                 Copyright(c) 2003-2008 Actions Semiconductor,
 *                            All Rights Reserved.
 *
 * History:
 *      <author>    <time>           <version >             <desc>
 *       hmwei     2008-8-30 16:44     1.0             build this file
 ********************************************************************************/
/*!
 * \file     sleep.h
 * \brief    ÑÓÊ±º¯ÊýÉùÃ÷
 * \author   hmwei
 * \version 1.0
 * \date  2008-8-30 16:44
 *******************************************************************************/

#ifndef __SLEEP_H__
#define __SLEEP_H__

#include "types.h"

#ifndef _ASSEMBLER_

extern unsigned sleep(unsigned seconds);
extern int usleep(useconds_t useconds);

#endif /*_ASSEMBLER_*/

#endif /*__SLEEP_H__*/
