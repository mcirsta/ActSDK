/*
********************************************************************************
*                   213x --- usdk130 source file
*                (c) Copyright 2003 - 2008, Actions Co,Ld. 
*                        All Right Reserved 
*
* History:        
*      <author>      <time>       <version >    <desc>
*         cccw       2008/07/10        1.0      build this file 
*                                 charge  device driver.
********************************************************************************/ 
/*!
 * \file   charge_drv.c
 * \brief  
 *      charge char device driver.
 * \author cccw
 * \par GENERAL DESCRIPTION:
 * \par EXTERNALIZED FUNCTIONS:
 *       null
 *
 *  Copyright(c) 2003-2008 Actions Semiconductor, All Rights Reserved.
 *
 * \version 1.0
 * \date  2008/10/10
 *******************************************************************************/
#include "charge_sys.h"
#include "charge.h"
#include "charge_inner.h"




/******************************************************************************/
/*!                    
* \par  Description:
*     realized for file operation -- open();
* \param[in]   inode -- opened file's inode structure pointer, refer to struct inode.
* \param[in]   file -- opened file's file structure pointer, refer to struct file.
* \retval      0 success, else failed.
*
* \par  Related User Api Description:
* \par  open()
*    - 功能描述 \n
*         系统调用open可以获得对驱动的访问权，申请一些系统资源，如内存，中断等；
*                 同时该函数返回设备文件标识符，为随后的系统调用做好准备。 \n
*    - 函数原型 \n
*         int open(const char *pathname, int flags, int mode); \n
*    - 输入参数描述 \n
*         pathname ―― 参数pathname是将要被打开的设备文件的名称，比如，对于声卡来讲一般是/dev/dsp。 \n \n
*         flags ―― 参数flags用来指明应该以什么方式打开设备文件，它可以是O_RDONLY、
*                           O_WRONLY或者O_RDWR，分别表示以只读、只写或者读写的方式打开设备文件；\n \n
*         mode ―― 参数mode通常是可选的，它只有在指定的设备文件不存在时才会用到，
 *                          指明新创建 的文件应该具有怎样的权限。 \n
*    - 输出参数描述 \n
*         如果open系统调用能够成功完成，它将返回一个正整数作为文件标识符，在随后的 系统调用中需要用到
*          该标识符。如果open系统调用失败，它将返回-1，同时还会设置全局变量errno，指明是什么原因导致了错误的发生。 \n
*    - 示例与说明 \n
* \code
*         int fd;
*         fd = open("/dev/charge", O_RDWR);
* \endcode
*******************************************************************************/
static int charge_open(struct inode *inode, struct file *file)
{
    return 0;//act_charge_open();
}

/******************************************************************************/
/*!                    
* \par  Description:
*     realized for file operation -- close();
* \param[in]   inode -- opened file's inode structure pointer, refer to struct inode.
* \param[in]   file -- opened file's file structure pointer, refer to struct file.
* \retval      0 success, else failed.
*
* \par  Related User Api Description:
* \par  close()
*    - 功能描述 \n
*         一旦应用程序调用了close系统调用，内核就会释放与之相关的各种资源，
*                                    因此建议在不需要的时候尽量及时关闭已经打开的设备。 \n
*    - 函数原型 \n
*         int close(int fd); \n
*    - 输入参数描述 \n
*         fd ―― 被打开的设备文件的标识符。  \n
*    - 输出参数描述 \n
*         成功返回为0，失败返回-1。 \n
*    - 示例与说明 \n
* \code
*         int fd;
*         fd = open("/dev/charge", O_RDWR);
*         ……
*         close(fd);
* \endcode
*******************************************************************************/
static int charge_release(struct inode *inode, struct file *file)
{
    return 0;//act_charge_release();
}

/******************************************************************************/
/*!                    
* \par  Description:
*     realized for file operation -- ioctl();
* \param[in]   inode -- opened file's inode structure pointer, refer to struct inode.
* \param[in]   file -- opened file's file structure pointer, refer to struct file.
* \param[in]   cmd -- command for ioctl.
* \param[in]   arg -- argument for ioctl.
* \retval      0 success, else failed.
*
* \par  Related User Api Description:
* \par  ioctl()
*    - 功能描述 \n
*         系统调用ioctl用来实现各种对设备的控制请求。 \n
*    - 函数原型 \n
*         int ioctl(int fd, int request, ...); \n
*    - 输入参数描述 \n
*         fd ―― 被打开的设备文件的标识符； \n
*         request ――参数request的目的就是用来区分不同的控制请求； \n
*         通常说来，在对设备进行控制时还需要有其它参数，这要根据不同的控制请求才能确定，
*           并且可能是与硬件设备直接相关的。 \n
*    - 输出参数描述 \n
*         成功返回为0，失败返回-1。 \n
*    - 示例与说明 \n
*         有关ioctl的请求命令字示例与说明，详见各命令字的示例与说明。
*
*
* \par  ioctl()命令字描述:
*   详细描述ioctl原型：int ioctl(int fd, int request, unsigned long arg, ...) 中参数request的各选择项。\n
*   
*
<div class="generic_text_mapping_table">
 * <table>
 * <tr>
 * <th>命令</th>
 * <th>参数(arg的实际形式与意义)</th>
 * <th>命令参数功能说明</th>
 * </tr>
 * <tr>
 * <td>CHARGE_IOSET_START</td>
 * <td> no in use</td>
 * <td>开始充电命令</td>
 * </tr>
 * <tr>
 * <td>CHARGE_IOSET_STOP</td>
 * <td> no in use</td>
 * <td>停止充电命令</td>
 * </tr>
 * <tr>
 * <td>CHARGE_IOSET_PARAM</td>
 * <td>  charge_param_t * ，见 charge_drv.h </td>
 * <td> 设置充电参数，详见 charge_drv.h ，voltage及status成员可为任意值（no use ）</td>
 * </tr>
 * <tr>
 * <td>CHARGE_IOGET_PARAM</td>
 * <td> charge_param_t * ，见 charge_drv.h </td>
 * <td>获取充电参数及当前充电状态</td>
 * </tr>
 * <tr>
 * <td>CHARGE_IOSET_PROCESS</td>
 * <td> no in use</td>
 * <td>执行一次充电循环流程</td>
 * </tr>
 * <tr>
 * <td>CHARGE_IOGET_USB_STATE</td>
 * <td> int *,指针传递值</td>
 * <td>用于USB应用场景下获取当前USB连接状态，0~off, 1~on</td>
 * </tr>  
 * </table>
* </div>
*
*******************************************************************************/
static int charge_ioctl(struct inode *inode, struct file *file, unsigned int cmd, unsigned long arg)
{
    //printf("charge_ioctl:%x, %d\n", cmd, arg);
//    return act_charge_ioctl(cmd,arg);	
    switch (cmd) 
    {
    case CHARGE_IOSET_START:
        return  act_set_pmu_charge(1);

    case CHARGE_IOSET_STOP:
        return  act_set_pmu_charge(0);
  
    case CHARGE_IOSET_PROCESS:
        act_charge();
        return 0;     
          
     case CHARGE_IOSET_PARAM:       
        return act_set_max_param((charge_param_t* )arg);     
              
    case CHARGE_IOGET_PARAM:
        return act_get_max_param((charge_param_t* )arg);
        
    case CHARGE_IOGET_STATE:       
        return charge_get_charge_status((charge_status_t *) arg);  
        
    case CHARGE_IOGET_BATTERY:
        if((void*)arg ==NULL)
        {
            return -1;		
        }
        else
        {
            *((unsigned int*)arg) = bat_power;
            return 0;
        }                     
    case CHARGE_IOGET_BATTERY_SHOW:    	
        if((void*)arg ==NULL)
        {
            return -1;		
        }
        else
        {
            if (charge_status == CHARGE_NO_BATTERY)
            {
                *((unsigned int*)arg) =  0x00;
            }
            else
            {                        
                *((unsigned int*)arg) = bat_show;
            }	           
            return 0;
        }         
    case CHARGE_IOGET_BATTERY_SHOW_LOGIC:    	
        if((void*)arg ==NULL)
        {
            return -1;		
        }
        else
        {
            if (charge_status == CHARGE_NO_BATTERY)
            {
                *((unsigned int*)arg) =  0x00;
            }
            else
            {                        
                *((unsigned int*)arg) =(unsigned int) lookup_bat_table((int)bat_show);
            }	    
            return 0;
        }                     
    case CHARGE_IOGET_DET_ADAPTER:    
  
        if((void*)arg ==NULL)
        {
            return -1;		
        }
        else
        {
            *((unsigned char*)arg) = charge_is_dc5v_on();
            return 0;
        }
    case CHARGE_IOGET_DET_SPK:        
        if((void*)arg ==NULL)
        {
            return -1;		
        }
        else
        {
            *((unsigned int*)arg) = charge_is_speaker_on();
            return 0;
        }                                            
                      
    default:
        return -1;
    }   
//    return 0;  
} 

/******************************************************************************/
/*!                    
* \par  Description:
*     realized for file operation -- read();
* \param[in]   file -- opened file's file structure pointer, refer to struct file.
* \param[in]   buf -- buffer pointer for saving read data.
* \param[in]   count -- data count to be read, unit is byte.
* \param[in]   ppos -- start position in file .
* \retval      bytes that has read.
*
* \par  Related User Api Description:
* \par  read()
*    - 功能描述 \n
*       系统调用read用来从对应设备读取数据。本驱动实现为空。 \n
*    - 函数原型 \n
*       int read(int fd, char *buf, size_t count); \n
*    - 输入参数描述 \n
*       fd ―― 被打开的设备文件的标识符。 \n
*       buf ―― 参数buf是指向缓冲区的字符指针，它用来保存从设备获得的数据； \n
*       count ―― 参数count则用来限定从设备获得的最大字节数。 \n
*    - 输出参数描述 \n
*       如果read系统调用成功完成，它将返回从设备实际读取的字节数，通常情况会比count的值要小一些；
*      如果read系统调用失败，它将返回-1，同时还会设置全局变量errno，来指明是什么原因导致了错误的发生。 \n
*    - 示例与说明 \n
*       见其他页面－驱动使用示例;
*******************************************************************************/
static ssize_t charge_read(struct file *file, char __user *buf,
            size_t count, loff_t *ppos)
{
    int ret = 0;
    //ada_dev_t *p_ada = file->private_data;
    /* TODO */
    return ret;
}

/******************************************************************************/
/*!                    
* \par  Description:
*     realized for file operation -- write();
* \param[in]   file -- opened file's file structure pointer, refer to struct file.
* \param[in]   buf -- buffer pointer that ready to write.
* \param[in]   count -- data count to wite, unit is byte.
* \param[in]   ppos -- start position in file .
* \retval      bytes that has written.
*
* \par  Related User Api Description:
* \par  write()
*    - 功能描述 \n
*       系统调用write用来向对应设备写入数据。本驱动实现为空。 \n
*    - 函数原型 \n
*       size_t write(int fd, const char *buf, size_t count); \n
*    - 输入参数描述 \n
*       fd ―― 被打开的设备文件的标识符。  \n
*       buf ―― 参数buf是指向缓冲区的字符指针，该缓冲区域缓存了要写入设备的数据； \n
*       count ―― 参数count则用来限定写入设备的最大字节数。 \n
*    - 输出参数描述 \n
*       如果write系统调用成功完成，它将返回向设备实际写入的字节数；如果read系统调用失败，它将返回-1，
*          同时还会设置全局变量errno，来指明是什么原因导致了错误的发生。 \n
*    - 示例与说明 \n
*       见其他页面－驱动使用示例;
*******************************************************************************/
static ssize_t charge_write(struct file *file, const char __user *buf,
             size_t count, loff_t *ppos)
{
    int ret = 0;
//  dev_t *p_ada = file->private_data;
    return ret;
}


/******************************************************************************/
/*!                    
* \par  Description:
*     realized for file operation -- llseek();
* \param[in]   file -- opened file's file structure pointer, refer to struct file.
* \param[in]   offset -- seek's offset value.
* \param[in]   origin -- seek's start position.
* \retval      current file position.
*******************************************************************************/
static loff_t charge_llseek(struct file *file, loff_t offset, int origin)
{
    int ret = 0;
    /* TODO */
    return ret; 
}

#pragma __PRQA_IGNORE_START__
/* The various file operations we support. */
static struct file_operations charge_fops = {
    .owner      = THIS_MODULE,
    .llseek     = charge_llseek,
    .read       = charge_read,
    .write      = charge_write,
    .open       = charge_open,
    .release    = charge_release,
    .ioctl      = charge_ioctl,
    //.readdir    = (void *)0,
    .mmap       = (void *)0,
};
#pragma __PRQA_IGNORE_END__

/******************************************************************************/
/*!                    
* \par  Description:
*   module or driver install entry. 
* \param[in]   null
* \retval      0 success, else failed.
*******************************************************************************/
static int charge_init(void)
{
    int ret = 0;

    ret = charge_dev_init();
    if (ret < 0)
    {
        goto out;
    }
    ret = register_chrdev(MAJOR_CHARGER, "charge", &charge_fops);
    if (ret < 0)
    {
        printk(KERN_ERR "charge: unable to get major %d\n", MAJOR_CHARGER);
        goto out;
    }

    out:    
    return ret;
}

/******************************************************************************/
/*!                    
* \par  Description:
*   module or driver uninstall entry.
* \param[in]   null
* \retval      0 success, else failed.
*******************************************************************************/
static void charge_exit(void)
{
    charge_dev_exit();
    unregister_chrdev(MAJOR_CHARGER, "charge");
    return;
}

/******************************************************************************/
/*!                    
* \par  Description:
*   export function example 
* \param[in]   null
* \retval      null
*******************************************************************************/


//EXPORT_SYMBOL(act_charge_open);
//EXPORT_SYMBOL(act_charge_release);
//EXPORT_SYMBOL(act_charge_ioctl);
EXPORT_SYMBOL(charge_open);
EXPORT_SYMBOL(charge_release);
EXPORT_SYMBOL(charge_ioctl);


#pragma __PRQA_IGNORE_START__
module_init(charge_init);
module_exit(charge_exit);
#pragma __PRQA_IGNORE_END__
