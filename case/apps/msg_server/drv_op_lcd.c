/********************************************************************************
 *                              USDK(1100)
 *                             Module: app 
 *                 Copyright(c) 2003-2008 Actions Semiconductor,
 *                            All Rights Reserved. 
 * file : drv_op_lcd.c
 * History:         
 *      <author>    <time>           <version >             <desc>
 *      Herb Hu    2009-6-8             v1.0             build this file 
 ********************************************************************************/

#include "drv_op_lcd.h"


static int s_dev_fd = -1;


bool drv_op_lcd_open_dev( void )
{
    s_dev_fd = open(DEVICE_LCD, O_RDWR);

    if (s_dev_fd < 0)
    {
        s_dev_fd = -1;
        print_err("open %s failed!", DEVICE_LCD);
        return false;
    }

    return true;
}

bool drv_op_lcd_close_dev( void )
{
    if (-1 != s_dev_fd)
    {
        if(close(s_dev_fd) != 0)
        {
            print_err("close lcd device failed!");
            return false;
        }
        else
        {
            return true;
        }
    }

    print_warning("lcd device has not opened!");
    return false;
}

bool drv_op_lcd_get_backlight(unsigned int * val)
{
    unsigned int tmp_val;

    if (-1 == s_dev_fd)
    {
        print_warning("lcd device has not opened!");
        return false;
    }

    if(ioctl(s_dev_fd, DISPGET_LCD_BACKLIGHT, (int)&tmp_val) != 0)
    {
        print_err("get lcd backlight failed!");
        return false;
    }

    *val = tmp_val;

    print_info("get lcd backlight :%d", *val);

    return true;
}

bool drv_op_lcd_set_backlight(unsigned int val)
{
    if (-1 == s_dev_fd)
    {
        print_warning("lcd device has not opened!");
        return false;
    }

    print_info("set lcd backlight :%d", val);

    if(ioctl(s_dev_fd, DISPSET_LCD_BACKLIGHT, val) != 0)
    {
        print_err("set lcd backlight failed!");
        return false;
    }

    return true;
}

bool drv_op_lcd_enter_standby(void)
{
    if (-1 == s_dev_fd)
    {
        print_warning("lcd device has not opened!");
        return false;
    }

    if(ioctl(s_dev_fd, DISPSET_LCD_STANDBY, ENTER) != 0)
    {
        print_err("enter lcd standby failed!");
        return false;
    }

    return true;
}

bool drv_op_lcd_exit_standby(void)
{
    if (-1 == s_dev_fd)
    {
        print_warning("lcd device has not opened!");
        return false;
    }

    if(ioctl(s_dev_fd, DISPSET_LCD_STANDBY, EXIT) != 0)
    {
        print_err("exit lcd standby failed!");
        return false;
    }

    usleep(200000);

    return true;
}
