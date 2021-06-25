/*********************************************************************************
*                       213x---gsensor driver
*                (c) Copyright 2007-2010, Actions Co,Ld.
*                        All Right Reserved 
*
* FileName:gsensor_inner.h    Author:yangrui  Date:2009/05/11
* Description: declaration of functions.                     
* Others:      
* History:     
*      <author>    <time>       <version >    <desc>
*      yangrui     2009/05/11     1.0         build this moudle 
*********************************************************************************/
#ifndef  _GSENSOR_INNER_H_
#define  _GSENSOR_INNER_H_

#include "gsensor_sys.h"
#include "gsensor_hw.h"

extern int act_gsensor_open(void);
extern ssize_t act_gsensor_read( char * buffer, size_t count);
extern ssize_t act_gsensor_write(void);
extern int act_gsensor_release(void);
extern int act_gsensor_ioctl(unsigned int cmd, unsigned long interval);

#endif
