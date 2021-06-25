/********************************************************************************
 *                              USDK(1100)
 *                             Module: app 
 *                 Copyright(c) 2003-2008 Actions Semiconductor,
 *                            All Rights Reserved. 
 * file : drv_op_gsensor.h
 * History:         
 *      <author>    <time>           <version >             <desc>
 *      Herb Hu    2009-5-25             v1.0             build this file 
 ********************************************************************************/

#ifndef __DRV_OP_GSENSOR_H__
#define __DRV_OP_GSENSOR_H__

#include <include_psp.h>
#include <include_case.h>
#include "include_msg_server.h"

extern bool drv_op_gsensor_open_dev(void);
extern bool drv_op_gsensor_is_dev_opened(void);
extern bool drv_op_gsensor_close_dev(void);
extern bool drv_op_gsensor_enable(void);
extern bool drv_op_gsensor_disable(void);

extern bool drv_op_gsensor_read(gsensor_data_t * gsensor_data);

#endif /* __DRV_OP_GSENSOR_H__ */

