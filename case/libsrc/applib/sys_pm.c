/********************************************************************************
 *                              USDK(1100)
 *                             Module: app 
 *                 Copyright(c) 2003-2008 Actions Semiconductor,
 *                            All Rights Reserved. 
 * File : sys_settings.c
 * History:         
 *      <author>    <time>           <version >             <desc>
 *      Herb Hu    2009-3-24             v1.0             build this file 
 ********************************************************************************/

#include <include_case.h>
#include "applib_common.h"

static int s_dev_fd = -1;

static bool _open_dev(void);
static bool _close_dev(void);

bool sys_pm_set_clk_domain(const clk_domain_t * domain)
{
    bool result = true;
    if((_open_dev() == true) && (domain != NULL))
    {
        if(ioctl(s_dev_fd, SET_CLK_DOMAIN, (unsigned long)(domain)) != 0)
        {
            print_err("ioctl failed!");
            result = false;
        }

        _close_dev();
        return result;
    }

    return false;
}

bool sys_pm_clear_clk_domain(const clk_domain_t * domain)
{
    bool result = true;
    if((_open_dev() == true) && (domain != NULL))
    {
        if(ioctl(s_dev_fd, CLEAR_CLK_DOMAIN, (unsigned long)(domain)) != 0)
        {
            print_err("ioctl failed!");
            result = false;
        }

        _close_dev();
        return result;
    }

    return false;
}

bool sys_pm_get_clk_info(sys_clk_info_t * sys_clk_info)
{
    sys_clk_info_t info = {0};
    clock_info_t op = {0};

    bool result = true;
    if((_open_dev() == true) && (sys_clk_info != NULL))
    {
        op.cmd = GET_COREPLL;
        ioctl(s_dev_fd, GET_CLOCK, (int)&op);
        info.corepll = (int)op.result;

        op.cmd = GET_CCLK;
        ioctl(s_dev_fd, GET_CLOCK, (int)&op);
        info.cclk = (int)op.result;

        op.cmd = GET_SCLK;
        ioctl(s_dev_fd, GET_CLOCK, (int)&op);
        info.sclk = (int)op.result;

        op.cmd = GET_PCLK;
        ioctl(s_dev_fd, GET_CLOCK, (int)&op);
        info.pclk = (int)op.result;

        op.cmd = GET_CLK_LEVEL;
        ioctl(s_dev_fd, GET_CLOCK, (int)&op);
        info.clk_level = (int)op.result;
        _close_dev();

        *sys_clk_info = info;

        return result;
    }

    return false;
}

static bool _open_dev(void)
{
    s_dev_fd = open(DEVICE_PM, O_RDWR);

    if (s_dev_fd < 0)
    {
        s_dev_fd = -1;
        print_err("open %s failed!", DEVICE_PM);
        return false;
    }

    return true;
}

static bool _close_dev(void)
{
    if (-1 != s_dev_fd)
    {
        if(close(s_dev_fd) != 0)
        {
            print_err("close %s failed!", DEVICE_PM);
            return false;
        }
        else
        {
            s_dev_fd = -1;
            return true;
        }
    }

    return false;
}
