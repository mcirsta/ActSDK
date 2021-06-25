/********************************************************************************
 *                              USDK(1100)
 *                             Module: app 
 *                 Copyright(c) 2003-2008 Actions Semiconductor,
 *                            All Rights Reserved. 
 * file : drv_op_lcd.h
 * History:         
 *      <author>    <time>           <version >             <desc>
 *      Herb Hu    2009-6-8             v1.0             build this file 
 ********************************************************************************/

#ifndef __DRV_OP_LCD_H__
#define __DRV_OP_LCD_H__

#include <include_case.h>
#include "include_msg_server.h"

extern bool drv_op_lcd_open_dev(void);
extern bool drv_op_lcd_close_dev(void);
extern bool drv_op_lcd_get_backlight(unsigned int * val);
extern bool drv_op_lcd_set_backlight(unsigned int val);
extern bool drv_op_lcd_enter_standby(void);
extern bool drv_op_lcd_exit_standby(void);


#endif /* __DRV_OP_LCD_H__ */

