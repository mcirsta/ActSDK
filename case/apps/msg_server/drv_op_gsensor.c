/********************************************************************************
 *                              USDK(1100)
 *                             Module: app 
 *                 Copyright(c) 2003-2008 Actions Semiconductor,
 *                            All Rights Reserved. 
 * file : drv_op_gsensor.c
 * History:         
 *      <author>    <time>           <version >             <desc>
 *      Herb Hu    2009-5-25             v1.0             build this file 
 ********************************************************************************/

#include "drv_op_gsensor.h"


static int s_dev_fd = -1;
static bool s_opened = false;

bool drv_op_gsensor_open_dev( void )
{
    if(s_dev_fd > 0)
    {
        print_warning("%s is already opened!", DEVICE_GSENSOR);
        return true;
    }

    s_dev_fd = open(DEVICE_GSENSOR, O_RDWR, 0);

    if (s_dev_fd < 0)
    {
        s_dev_fd = -1;
        print_err("open %s failed!", DEVICE_GSENSOR);
        return false;
    }
  
    s_opened = true;
    return true;
}

bool drv_op_gsensor_is_dev_opened(void)
{
    return s_opened;
}

bool drv_op_gsensor_close_dev( void )
{
    if (-1 != s_dev_fd)
    {
        if(close(s_dev_fd) != 0)
        {
            print_err("close gsensor device failed!");
            return false;
        }
        else
        {
            s_opened = false;
            s_dev_fd = -1;
            return true;
        }
    }

    print_warning("gsensor device has not opened!");
    return true;
}

bool drv_op_gsensor_enable( void )
{
    if (-1 == s_dev_fd)
    {
        print_warning("gsensor device has not opened!");
        return false;
    }

    if(ioctl(s_dev_fd, GSENSOR_ENABLE, 0) != 0)
    {
        print_err("enable gsensor failed!");
        return false;
    }

    return true;
}

bool drv_op_gsensor_disable( void )
{
    if (-1 == s_dev_fd)
    {
        print_warning("gsensor device has not opened!");
        return false;
    }

    if(ioctl(s_dev_fd, GSENSOR_DISABLE, 0) != 0)
    {
        print_err("disable gsensor failed!");
        return false;
    }

    return true;
}

bool drv_op_gsensor_read(gsensor_data_t * gsensor_data)
{
    int len;
    gsensor_data_t data = {0};

    if (-1 == s_dev_fd)
    {
        print_warning("gsensor device has not opened!");
        return false;
    }

    len = read(s_dev_fd, &data, sizeof(gsensor_data_t));
    if ((unsigned int)len != sizeof(gsensor_data_t))
    {
        print_warning("error on read!");
        return false;
    }
    else
    {
        *gsensor_data = data;
        return true;
    }
}


