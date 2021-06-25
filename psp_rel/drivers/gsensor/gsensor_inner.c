/*********************************************************************************
 *                       213x---gsensor driver
 *                (c) Copyright 2007-2010, Actions Co,Ld.
 *                        All Right Reserved
 *
 * FileName:gsensor_inner.c    Author:yangrui  Date:2009/05/11
 * Description:  some functions is defined in this file,they are called by functions defined in
 *              the file of gsensor_drv.c.
 * Others:
 * History:
 *      <author>    <time>       <version >    <desc>
 *      yangrui     2009/05/11     1.0         build this moudle
 *********************************************************************************/
#include "gsensor_inner.h"
#include "gsensor_hw.h"
#include "data_buf.h"

static int module_open_count = 0;
/*******************************************************************************
 * Function:       act_gsensor_open
 * Description:    function of actual device open, called by gsensor_open.
 * Input£º         void
 *Output£º         0:  success of open operation
 *                 non-zero: failure of open operation
 * Other£º         //
 *******************************************************************************/
int act_gsensor_open(void)
{
    OS_INT_SAVE_AREA
    int ret = 0;
    
    OS_ENTER_CRITICAL();
    if( module_open_count == 0 )
    {
        ret =  gsensor_hw_init();
        if( ret == 0 )
        {
            module_open_count++;
            OS_EXIT_CRITICAL();
        
            init_timer(&g_timer); /*initial timer*/
            g_timer.function = timer_handle;
            g_jiffies = kernel_sym("jiffies");
            return 0;
        }
    }
    OS_EXIT_CRITICAL();

    //printk("gsensor has been opened.\n");
    return ret;
}

/*******************************************************************************
 * Function:       act_gsensor_read
 * Description:    function of actual device read, called by gsensor_read.
 * Input£º         buffer: address of user's buffer
 *                 count:  amount of chars to be read.
 *Output£º         amount of chars have been read.
 * Other£º         //
 *******************************************************************************/
ssize_t act_gsensor_read(char * buffer, size_t count)
{
    ssize_t ret;
    ret = data_buf_read(buffer, count);
    return ret;
}

/*******************************************************************************
 * Function:       act_gsensor_write
 * Description:    function of actual device write, called by gsensor_write.
 *                 This function does  nothing actual operation.
 * Input£º         void
 *Output£º         //
 * Other£º         //
 *******************************************************************************/
ssize_t act_gsensor_write(void)
{
    return ((ssize_t) 0);
}

/*******************************************************************************
 * Function:       act_gsensor_release
 * Description:    function of actual device release, called by gsensor_release.
 * Input£º         void
 *Output£º         0:  success of release operation
 *                 non-zero: failure of release operation
 * Other£º         //
 *******************************************************************************/
int act_gsensor_release(void)
{
    OS_INT_SAVE_AREA
    int ret = 0;
    
    OS_ENTER_CRITICAL();
    module_open_count--;
    if (module_open_count != 0)
    {
        OS_EXIT_CRITICAL();
        return 0;
    }
    OS_EXIT_CRITICAL();
    
    del_timer(&g_timer);
    if ((atomic_read(&w_enable) == 1))
    {
        atomic_set(&w_enable, 0);
    }

    ret = gsensor_hw_exit();
    //printk("gsensor has been released.\n");
    return ret;
}

/*******************************************************************************
 * Function:       act_gsensor_ioctl
 * Description:    function of actual device control, called by gsensor_ioctl.
 * Input£º         cmd, interval
 *Output£º         0:  success of control operation
 *                 EBADRQC: failure of control operation
 * Other£º         //
 *******************************************************************************/
int act_gsensor_ioctl(unsigned int cmd, unsigned long interval)
{
    int ret;
    ret = data_buf_ioctl(cmd, interval);
    //printk("gsensor has been controled.\n");
    return ret;
}

