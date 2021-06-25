/*********************************************************************************
 *                       213x---gsensor driver
 *                (c) Copyright 2007-2010, Actions Co,Ld.
 *                        All Right Reserved
 *
 * FileName:gsensor_drv.c    Author:yangrui  Date:2009/05/11
 * Description: This file defines functions in struct file_operations.
 * Others:
 * History:
 *      <author>    <time>       <version >    <desc>
 *      yangrui     2009/05/11     1.0         build this moudle
 *********************************************************************************/
#include "gsensor_inner.h"
#include "gsensor.h"
#include "data_buf.h"

#ifdef  _ACT_LINUX_KERNEL_2_6_
MODULE_LICENSE("GPL");
#endif  //end of '#ifdef  _ACT_LINUX_KERNEL_2_6_'
/*******************************************************************************
 * Function:       gsensor_open
 * Description:    open a gsensor device
 * Input£º         inode, filp
 * Output£º        0:  success of open operation
 *                 non-zero: failure of open operation
 * Other£º         //
 *******************************************************************************/
int gsensor_open(struct inode *inode, struct file *filp)
{
    int g_ret;
    g_ret = act_gsensor_open();
    return g_ret;
}

/*******************************************************************************
 * Function:       gsensor_read
 * Description:    read datas from a gsensor device.
 * Input£º         filp,
 *                 buf: address of user's buffer
 *                 count: amount of chars to be read.
 * Output£º        amount of chars that have been read.
 * Other£º         //
 *******************************************************************************/
ssize_t gsensor_read(struct file *filp, char *buf, size_t count, loff_t *f_pos)
{
    ssize_t ret;
    ret = act_gsensor_read(buf, count);
    return ret;
}

/*******************************************************************************
 * Function:       gsensor_write
 * Description:    write datas to a gsensor device.(This function doesn't do any
 *                 actual operations in this module.)
 * Input£º         //
 * Output£º        //

 * Other£º         //
 *******************************************************************************/
ssize_t gsensor_write(struct file *filp, const char *buf, size_t count, loff_t *f_pos)
{
    ssize_t ret;
    ret = act_gsensor_write();
    return ret;
}

/*******************************************************************************
 * Function:       gsensor_release
 * Description:    realse a device of gsensor.
 * Input£º         inode, filp
 * Output£º        0:  success of release operation
 *                 non-zero: failure of release operation
 * Other£º         //
 *******************************************************************************/
int gsensor_release(struct inode *inode, struct file *filp)
{
    int ret;
    ret = act_gsensor_release();
    return ret;
}

/*******************************************************************************
 * Function:       gsensor_ioctl
 * Description:    control the gsensor device
 * Input£º         inode, filp, cmd, arg
 * Output£º        0:  success of control operation
 *                 EBADRQC: failure of control operation
 * Other£º         //
 *******************************************************************************/
int gsensor_ioctl(struct inode *inode, struct file *filp, unsigned int cmd, unsigned long arg)
{
    int ret;
    ret = act_gsensor_ioctl(cmd, arg);
    return ret;
}

#pragma __PRQA_IGNORE_START__
struct file_operations gsensor_fops =
{ .owner = THIS_MODULE, .ioctl = gsensor_ioctl, .read = gsensor_read, .open = gsensor_open, .write = gsensor_write,
        .release = gsensor_release, };
#pragma __PRQA_IGNORE_END__

/*******************************************************************************
 * Function:       gsensor_module_init
 * Description:    initialize the gsensor module. The function is called when the
 *                 gsensor module is installed.
 * Input£º         void
 * Output£º        0:  success of initialization operation
 *                 non zero: failure of initialization
 * Other£º
 *******************************************************************************/
int gsensor_module_init(void)
{
    int result;
    result = register_chrdev((unsigned int)MAJOR_GSENSOR, "gsensor", &gsensor_fops);
    if (result < 0)
    {
        printk("gsensor: can't get major %d\n", MAJOR_GSENSOR);
        return result;
    }
    return 0;
}

/*******************************************************************************
 * Function:       gsensor_module_cleanup
 * Description:    clean up the module. The function is called when the module is
 *                 uninstalled.
 * Input£º         void
 * Output£º        void
 * Other£º         //
 *******************************************************************************/
void gsensor_module_cleanup(void)
{
    unregister_chrdev((unsigned)MAJOR_GSENSOR, "gsensor");
}

#pragma __PRQA_IGNORE_START__
module_init(gsensor_module_init);
module_exit(gsensor_module_cleanup);
#pragma __PRQA_IGNORE_END__

EXPORT_SYMBOL(gsensor_open);
EXPORT_SYMBOL(gsensor_ioctl);
EXPORT_SYMBOL(gsensor_read);
EXPORT_SYMBOL(gsensor_write);
EXPORT_SYMBOL(gsensor_release);
EXPORT_SYMBOL(gsensor_module_init);
EXPORT_SYMBOL(gsensor_module_cleanup);

