/*
********************************************************************************
*                   213x --- usdk130 source file
*                (c) Copyright 2003 - 2008, Actions Co,Ld. 
*                        All Right Reserved 
*
* History:        
*      <author>      <time>       <version >    <desc>
*       cccw        2008/07/10     1.0      build this file 
*                                               lcd char device driver.
********************************************************************************/ 
/*!
 * \file   lcd_drv.c
 * \brief  
 *      lcd char device driver.
 * \author cccw
 * \par GENERAL DESCRIPTION:
 * \par EXTERNALIZED FUNCTIONS:
 *       null
 *
 *  Copyright(c) 2003-2008 Actions Semiconductor, All Rights Reserved.
 *
 * \version 1.0
 * \date  2008/07/10
 *******************************************************************************/

#include "lcd_sys.h"
#include "lcd_inner.h"


/******************************************************************************/
/*!                    
* \par  Description:
*   export function example 
* \param[in]   null
* \retval      null
*******************************************************************************/
int act_lcd_open(void)
{
    return 0;
}

int act_lcd_release(void)
{
    return 0;
}

int act_lcd_ioctl(unsigned int cmd,unsigned long arg)
{
    switch (cmd)
    {
        case DISPSET_UPDATE:
        return lcd_frame_update( (disp_frame_info_t*)arg );     
            
        case DISPGET_DISP_ATTR:
        if ((int*)arg == NULL)
        {
            return -1;
        }  
        memcpy((unsigned char *)arg,&act_lcd_para,sizeof(disp_dev_t));  // copy_to_user()
        break;
            
        case DISPSET_LCD_BACKLIGHT:
        return lcd_set_back_light(arg);
    
        case DISPSET_LCD_ROTATE:
        return lcd_rotate(arg);
        
        case DISPGET_LCD_BACKLIGHT:
        if ((int*)arg == NULL)
        {
            return -1;
        }
        *((unsigned int *)arg) = lcd_get_back_light();                                 // // copy_to_user()
        break;
        
        case DISPSET_LCD_STANDBY:
        if(arg ==ENTER)
        {
            return lcd_power_off();
        }
        else if(arg == EXIT)
        {
            return lcd_exit_standby();
        }   
        else   
        {
            return -1;
        }                  
            
        case DISPGET_DMA_STATUS:
        if ((int*)arg == NULL)
        {
            return -1;
        }
        *(int *)(arg) = lcd_dma_status();
        break;                 
             
        default:
        return -1;
    }
    
    return 0;      
}


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
*         系统调用open可以获得对驱动的访问权，申请一些系统资源，如内存，中断等；同时该函数返回设备文件标识符，
*         为随后的系统调用做好准备。 \n
*    - 函数原型 \n
*         int open(const char *pathname, int flags, int mode); \n
*    - 输入参数描述 \n
*         pathname ―― 参数pathname是将要被打开的设备文件的名称，比如，对于声卡来讲一般是/dev/dsp。 \n \n
*         flags ―― 参数flags用来指明应该以什么方式打开设备文件，它可以是O_RDONLY、O_WRONLY或者O_RDWR，
*                            分别表示以只读、只写或者读写的方式打开设备文件；\n \n
*         mode ―― 参数mode通常是可选的，它只有在指定的设备文件不存在时才会用到，
*                              指明新创建 的文件应该具有怎样的权限。 \n
*    - 输出参数描述 \n
*         如果open系统调用能够成功完成，它将返回一个正整数作为文件标识符，在随后的 系统调用中需要用到该标识符。
*         如果open系统调用失败，它将返回-1，同时还会设置全局变量errno，指明是什么原因导致了错误的发生。 \n
*    - 示例与说明 \n
* \code
*         int fd;
*         fd = open("/dev/lcd", O_RDWR);
* \endcode
*******************************************************************************/
static int lcd_open(struct inode *inode, struct file *file)
{ 
    return act_lcd_open();
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
*         因此建议在不需要的时候尽量及时关闭已经打开的设备。 \n
*    - 函数原型 \n
*         int close(int fd); \n
*    - 输入参数描述 \n
*         fd ―― 被打开的设备文件的标识符。  \n
*    - 输出参数描述 \n
*         成功返回为0，失败返回-1。 \n
*    - 示例与说明 \n
* \code
*         int fd;
*         fd = open("/dev/lcd", O_RDWR);
*         ……
*         close(fd);
* \endcode
*******************************************************************************/
static int lcd_release(struct inode *inode, struct file *file)
{
 
    return act_lcd_release();
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
*          并且可能是与硬件设备直接相关的。 \n
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
 * <td>DISPGET_DISP_ATTR</td>
 * <td> disp_dev_t *arg,see display_drv.h </td>
 * <td>获取当前设备的参数及属性</td>
 * </tr>
 
 * <tr>
 * <td>DISPSET_UPDATE</td>
 * <td>disp_frame_info_t *arg，see display_drv.h</td>
 * <td>刷新一帧数据</td>
 * </tr>
 
 * <tr>
 * <td>DISPSET_LCD_BACKLIGHT</td>
 * <td>int,背光等级数 ,0~5</td>
 * <td>“0”为关背光，“1”最暗，“5”最亮</td>
 * </tr>
 
 * <tr>
 * <td>DISPGET_LCD_BACKLIGHT</td>
 * <td>int* ，返回背光等级 </td>
 * <td>获取LCD背光等级，0~5</td>
 * </tr>
 
 * <tr>
 * <td>DISPSET_LCD_STANDBY</td>
 * <td> int ，0退出；1进入</td>
 * <td>设置LCD进入/退出standby</td>
 * </tr>
 
* </table>
* </div>
*
*******************************************************************************/
static int lcd_ioctl(struct inode *inode, struct file *file, unsigned int cmd, unsigned long arg)
{
    //printk("lcd_ioctl:%x, %d\n", cmd, cmd);
    return act_lcd_ioctl( cmd, arg );
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
*       系统调用read用来从对应设备读取数据。 \n
*    - 函数原型 \n
*       int read(int fd, char *buf, size_t count); \n
*    - 输入参数描述 \n
*       fd ―― 被打开的设备文件的标识符。 \n
*       buf ―― 参数buf是指向缓冲区的字符指针，它用来保存从设备获得的数据； \n
*       count ―― 参数count则用来限定从设备获得的最大字节数。 \n
*    - 输出参数描述 \n
*       如果read系统调用成功完成，它将返回从设备实际读取的字节数，通常情况会比count的值要小一些；
*        如果read系统调用失败，它将返回-1，同时还会设置全局变量errno，来指明是什么原因导致了错误的发生。 \n
*    - 示例与说明 \n
*       见其他页面－驱动使用示例;
*******************************************************************************/
static ssize_t lcd_read(struct file *file, char __user *buf,
            size_t count, loff_t *ppos)
{

    return 0;
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
*       系统调用write用来向对应设备写入数据。 \n
*    - 函数原型 \n
*       size_t write(int fd, const char *buf, size_t count); \n
*    - 输入参数描述 \n
*       fd ―― 被打开的设备文件的标识符。  \n
*       buf ―― 参数buf是指向缓冲区的字符指针，该缓冲区域缓存了要写入设备的数据； \n
*       count ―― 参数count则用来限定写入设备的最大字节数。 \n
*    - 输出参数描述 \n
*       如果write系统调用成功完成，它将返回向设备实际写入的字节数；如果read系统调用失败，它将返回-1，
*        同时还会设置全局变量errno，来指明是什么原因导致了错误的发生。 \n
*    - 示例与说明 \n
*       见其他页面－驱动使用示例;
*******************************************************************************/
static ssize_t lcd_write(struct file *file, const char __user *buf,
             size_t count, loff_t *ppos)
{


    return 0;
}

/*! \cond INNER_LCD_API*/
/******************************************************************************/
/*!                    
* \par  Description:
*     realized for file operation -- llseek();
* \param[in]   file -- opened file's file structure pointer, refer to struct file.
* \param[in]   offset -- seek's offset value.
* \param[in]   origin -- seek's start position.
* \retval      current file position.
*******************************************************************************/
static loff_t lcd_llseek(struct file *file, loff_t offset, int origin)
{

    return 0; 
}

#pragma __PRQA_IGNORE_START__
/* The various file operations we support. */
static struct file_operations lcd_fops = {
    .owner      = THIS_MODULE,
    .llseek     = lcd_llseek,
    .read       = lcd_read,
    .write      = lcd_write,
    .open       = lcd_open,
    .release    = lcd_release,
    .ioctl      = lcd_ioctl,
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
static int lcd_init(void)
{
    int ret = 0;
#ifdef __COVTEST__    
	 unsigned int addr, addr1;
	 extern unsigned int _ctor_start, _ctor_end;
	 for (addr = &_ctor_start; addr<&_ctor_end; addr+=4)
	 {
	     addr1 = *(volatile unsigned int *)(addr);
	     printf("%s---%d, addr: 0x%08x, 0x%08x\n", __FILE__,__LINE__,addr, addr1);
	     if (addr1 != 0)
	     {
	         ((void (*)(void))(addr1))();
	     }
	 }
#endif
    ret = lcd_dev_init();
    if (ret < 0)
    {
        goto out;
    }
    ret = register_chrdev(MAJOR_LCD, "lcd", &lcd_fops);
    if (ret < 0)
    {
        printk(KERN_ERR "lcd: unable to get major %d\n", MAJOR_LCD);
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
static void lcd_exit(void)
{
    lcd_dev_exit();
    unregister_chrdev(MAJOR_LCD, "lcd");
#ifdef __COVTEST__
	 unsigned int addr, addr1;
	 extern unsigned int _dtor_start, _dtor_end;
	 for (addr = &_dtor_start; addr < &_dtor_end; addr += 4)
	 {
	     addr1 = *(volatile unsigned int *)(addr);
	     printf("%s---%d, addr: 0x%08x, 0x%08x\n", __FILE__,__LINE__,addr, addr1);
	     if (addr1 != 0)
	     {
	         ((void (*)(void))(addr1))();
	     }
	 }
#endif
    return;
}



EXPORT_SYMBOL(act_lcd_open);
EXPORT_SYMBOL(act_lcd_release);
EXPORT_SYMBOL(act_lcd_ioctl);
EXPORT_SYMBOL(lcd_init);
EXPORT_SYMBOL(lcd_exit);

#pragma __PRQA_IGNORE_START__

module_init(lcd_init);
module_exit(lcd_exit);

#pragma __PRQA_IGNORE_END__

/*! \endcond*/
/* end of cond INNER_LCD_API */


