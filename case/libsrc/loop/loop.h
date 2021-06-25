/********************************************************************************
 *                              USDK(1100)
 *                             Module: lib 
 *                 Copyright(c) 2003-2009 Actions Semiconductor,
 *                            All Rights Reserved. 
 * History:         
 *      <author>    <time>           <version >             <desc>
 *      xusongzhou  2008-07-16          v1.0               build this file 
 *      shaofeng    2009-05-17          v1.1               优化loopmode
 *      shaofeng    2009-05-18          v1.2               按照case代码规范整理代码
 ********************************************************************************/
/*!
* \file     loop.h
* \brief    
* \author   xusongzhou
*               
* \version 1.0
* \date  2008/07/16
*******************************************************************************/
/*!
 */
#ifndef __LOOP_H__
#define __LOOP_H__

typedef struct
{
    /*the current index*/
    int current;
    /*the total number*/
    int total;
    /*loop mode*/
    loop_mode_e loop_mode;
}loop_t;

#endif  /* __LOOP_H__ */

