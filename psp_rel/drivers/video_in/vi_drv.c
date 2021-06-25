/*
********************************************************************************
*                   video in device's driver
*                (c) Copyright 2007 - 2010, Actions Co,Ld. 
*                        All Right Reserved 
*
* History:        
*      <author>      <time>       <version >    <desc>
*       houjingkun      2007/08/14     1.0   build this file 
*       houjingkun      2008/06/18     1.1   check code for QAC and add commentary.
********************************************************************************/ 
/*!
 * \file   vi_drv.c
 * \brief  
 *      realize Sensor driver's API for app or midware. 
 * \author houjingkun
 * \par GENERAL DESCRIPTION:
 *      realize Sensor driver's API for app or midware. 
 * \par EXTERNALIZED FUNCTIONS:
 *       null
 *
 *     Copyright(c) 2008-2012 Actions Semiconductor, All Rights Reserved.
 *
 *  \version 1.1
 *  \date  2008/06/18
 *******************************************************************************/

#include "video_in.h"
#include "vi_config.h"
#include "vi_common.h"
#include "sensor_inner.h"
#include "tvin_inner.h"
#include "vi_sys.h"

#if (defined(_ACT_UC_OS_) && defined(_USE_OLD_SENSOR_API_))

/*! \cond INNER_SENSOR_API*/
/************************************************************************
* Function:       sensor_ioctl
* Description:    set sensor's function and character.
*         cmd's macro define please refer to video_in.h.
* Input：         cmd -- the corresponding command.
* Output：        0 -- success; others -- failed.
* Other：         
*************************************************************************/
int sensor_ioctl(unsigned int cmd, unsigned long arg)
{
    //SS_PRINTF("cmd:%x %d; arg: %x %d \n", cmd, cmd, arg, arg);

    if (vi_dev.app_type == VI_CVBS_TVIN_MODE) 
    {
        return tvin_ioctl(cmd, arg);
    } 
    else 
    {
        return ss_sensor_ioctl(cmd, arg);
    }
};

/************************************************************************
* Function:       sensor_init
* Description:    init sensor device.
* Input：         type -- indicate is sensor dc, dv, pc camera or tvin.
* Output：        0 -- success; others -- failed.
* Other：         
*************************************************************************/
int sensor_init(unsigned int type)
{
    //SS_PRINTF("init type: %d \n", type);
    //type = VI_CVBS_TVIN_MODE;
    
    if (type == VI_CVBS_TVIN_MODE) 
    {
        return tvin_init(VI_CVBS_TVIN_MODE);
    } 
    else 
    {
        return ss_sensor_init(type);        /* succeed */
    }
    return 0;
};

/************************************************************************
* Function:       sensor_exit
* Description:    release sensor device.
* Input：         
* Output：        0 -- success; others -- failed.
* Other：         
*************************************************************************/
int sensor_exit(void)
{
    //SS_PRINTF("sensor exit!\n");

    if (vi_dev.app_type == VI_CVBS_TVIN_MODE) 
    {
        return tvin_exit();
    } 
    else 
    {
        return ss_sensor_exit();
    }
};

/************************************************************************
* Function:       sensor_open
* Description:    open sensor device.
* Input：         filp -- indicating DC, DV or Pc camera application.
*
typedef enum 
{
    VI_DC_MODE = 0,
    VI_DV_MODE,
    VI_PC_CAMERA_MODE,
    VI_ANALOG_TVIN_MODE,
    VI_CVBS_TVIN_MODE
} vi_work_mode_t
*
* Output：        0 -- success; others -- failed.
* Other：         
*************************************************************************/
int sensor_open(unsigned int type)
{
    //SS_PRINTF("sensor open type: %d \n", type);

    if (vi_dev.app_type == VI_CVBS_TVIN_MODE) 
    {
        return tvin_open(type);
    } 
    else 
    {
        return ss_sensor_open(type);
    }
};

/************************************************************************
* Function:       sensor_close
* Description:    close sensor device.
* Input：         filp -- currently no use.
* Output：        0 -- success; others -- failed.
* Other：         
*************************************************************************/
int sensor_close(unsigned int type)
{
    //SS_PRINTF("sensor close type: %d \n", type);

    if (vi_dev.app_type == VI_CVBS_TVIN_MODE) 
    {
        return tvin_close(vi_dev.app_type);
    } 
    else 
    {
        return ss_sensor_close(vi_dev.app_type);
    }
};
/*! \endcond*/
/* end of cond INNER_SENSOR_API */

#else

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
*         同时该函数返回设备文件标识符，为随后的系统调用做好准备。 \n
*    - 函数原型 \n
*         int open(const char *pathname, int flags, int mode); \n
*    - 输入参数描述 \n
*         pathname ―― 参数pathname是将要被打开的设备文件的名称，比如，对于声卡来讲一般是/dev/dsp。\n 
*         flags ―― 参数flags用来指明应该以什么方式打开设备文件，它可以是O_RDONLY、
*         O_WRONLY或者O_RDWR，分别表示以只读、只写或者读写的方式打开设备文件；\n \n
*         mode ―― 参数mode通常是可选的，它只有在指定的设备文件不存在时才会用到，
*         指明新创建 的文件应该具有怎样的权限。 \n
*    - 输出参数描述 \n
*         如果open系统调用能够成功完成，它将返回一个正整数作为文件标识符，在随后的
*         系统调用中需要用到该标识符。如果open系统调用失败，它将返回-1，
*         同时还会设置全局变量errno，指明是什么原因导致了错误的发生。 \n
*    - 示例与说明 \n
* \code
*         int fd;
*         fd = open("/dev/ada", O_RDWR);
* \endcode
*******************************************************************************/
static int act_sensor_open(struct inode *inode, struct file *filp)
{
    //SS_PRINTF("ss_open\n");
    if (vi_dev.app_type == VI_CVBS_TVIN_MODE) 
    {
        return tvin_open(vi_dev.app_type);
    } 
    else 
    {
        return ss_sensor_open(vi_dev.app_type);
    }
};

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
*         fd = open("/dev/ada", O_RDWR);
*         ……
*         close(fd);
* \endcode
*******************************************************************************/
static int act_sensor_release(struct inode *inode, struct file *filp)
{
    //SS_PRINTF("ss_close\n");
    if (vi_dev.app_type == VI_CVBS_TVIN_MODE) 
    {
        return tvin_close(vi_dev.app_type);
    } 
    else 
    {
        return ss_sensor_close(vi_dev.app_type);
    }
};

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
*       如果read系统调用失败，它将返回-1，同时还会设置全局变量errno，
*       来指明是什么原因导致了错误的发生。 \n
*    - 示例与说明 \n
*       见其他页面－驱动使用示例;
*******************************************************************************/
static ssize_t act_sensor_read(struct file *filp, char *buf, size_t count,
                loff_t *f_pos)
{
    return 0;
};

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
*       如果write系统调用成功完成，它将返回向设备实际写入的字节数；如果read系统调用失败，
*       它将返回-1，同时还会设置全局变量errno，来指明是什么原因导致了错误的发生。 \n
*    - 示例与说明 \n
*       见其他页面－驱动使用示例;
*******************************************************************************/
static ssize_t act_sensor_write(struct file *filp, const char *buf, size_t count,
                loff_t *f_pos)
{
    return 0;
};

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
*         并且可能是与硬件设备直接相关的。 \n
*    - 输出参数描述 \n
*         成功返回为0，失败返回-1。 \n
*    - 示例与说明 \n
*         有关ioctl的请求命令字示例与说明，详见各命令字的示例与说明。
*
*/
static int act_sensor_ioctl(struct inode *inode, struct file *filp, unsigned int cmd,
        unsigned long arg)
{
    //SS_PRINTF("ss_ioctl: %x %d\n", cmd, (cmd - VI_IO_CMD_BASE));
    
    if (vi_dev.app_type == VI_CVBS_TVIN_MODE) 
    {
        return tvin_ioctl(cmd, arg);
    } 
    else 
    {
        return ss_sensor_ioctl(cmd, arg);
    }
};

#ifdef _ACT_LINUX_KERNEL_2_6_
/******************************************************************************/
/*!                    
* \par  Description:
*     realized for file operation -- mmap();
* \param[in]   file -- opened file's file structure pointer, refer to struct file.
* \param[in]   vma -- pointer of struct vm_area_struct.
* \retval      0 success, else failed.
*
* \par  Related User Api Description:
* \par  mmap()
*    - 功能描述 \n
*         系统调用mmap用来实现映射一段物理内存到用户空间。
*    - 函数原型 \n
*    - 输入参数描述 \n
*    - 输出参数描述 \n
*    - 示例与说明 \n
*         有关mmap请查看相关linux下系统调用接口文档。
*/
static int act_sensor_mmap(struct file *file, struct vm_area_struct *vma)
{
    unsigned int len;
    unsigned long start=0, off;

    if (vma->vm_pgoff > (~0UL >> PAGE_SHIFT)) 
    {
        return -EINVAL;
    }
    
    start = vi_dev.phy_addr & PAGE_MASK;
    len = PAGE_ALIGN((start & ~PAGE_MASK) + vi_dev.mem_len);

    off = vma->vm_pgoff << PAGE_SHIFT;

    if ((vma->vm_end - vma->vm_start + off) > len) 
    {
        return -EINVAL;
    }

    off += start;
    vma->vm_pgoff = off >> PAGE_SHIFT;

#if 1   /* uncache. */
    vma->vm_page_prot = pgprot_noncached(vma->vm_page_prot);
    pgprot_val(vma->vm_page_prot) |= _CACHE_MASK; /* CCA=7 */
#else   /* write through. */
    pgprot_val(vma->vm_page_prot) &= ~_CACHE_MASK;  /* write through, no write allocate */
    /* write back */
    //pgprot_val(vma->vm_page_prot) &= ~_CACHE_MASK; 
    /* write back, write allocate */
    //pgprot_val(vma->vm_page_prot) |= _CACHE_CACHABLE_NONCOHERENT; 
#endif

    vma->vm_flags |= VM_IO;
    
    if (remap_pfn_range(vma, vma->vm_start, vma->vm_pgoff,
        vma->vm_end - vma->vm_start,
        vma->vm_page_prot)) 
    {
        return -EAGAIN;
    }

    return 0;
};
#endif /* _ACT_LINUX_KERNEL_2_6_ */

#pragma __PRQA_IGNORE_START__
struct file_operations sensor_fops =  
{
    open:       act_sensor_open,
    release:    act_sensor_release,
    read:       act_sensor_read,
    write:      act_sensor_write,   
    ioctl:      act_sensor_ioctl,
#ifdef _ACT_LINUX_KERNEL_2_6_
    mmap:       act_sensor_mmap,
    //poll:     sensor_poll,
#endif /* _ACT_LINUX_KERNEL_2_6_ */
};
#pragma __PRQA_IGNORE_END__

/*! \cond INNER_SENSOR_API*/

static int sensor_init_module(void)
{
    int ret;

#ifdef __COVTEST__
    unsigned int addr, addr1;
    extern unsigned int _ctor_start, _ctor_end;

    for (addr = &_ctor_start; addr<&_ctor_end; addr+=4)
    {
        addr1 = *(volatile unsigned int *)(addr);
        if (addr1 != 0)
        {
            ((void (*)(void))(addr1))();
        }
    }
#endif

    //SET_MODULE_OWNER(&sensor_fops);
    //SS_PRINTF("vi_app_type: %d \n", vi_app_type);

    if (vi_app_type == VI_CVBS_TVIN_MODE) 
    {
        ret = tvin_init(VI_CVBS_TVIN_MODE);
    } 
    else 
    {
        ret = ss_sensor_init(vi_app_type);
    }
    if (ret < 0) 
    {
        return ret;
    }

    ret = register_chrdev(MAJOR_VIDEO_IN, "video_in", &sensor_fops);
    if (ret < 0) 
    {
        printk(KERN_WARNING "sensor: can't get major %d\n", MAJOR_VIDEO_IN);
        return ret;
    }
    //EXPORT_NO_SYMBOLS; /* otherwise, leave global symbols visible */

    return 0; /* succeed */
}

static void sensor_cleanup_module(void)
{
#ifdef __COVTEST__
     unsigned int addr, addr1;
     extern unsigned int _dtor_start, _dtor_end;
#endif

    if (vi_dev.app_type == VI_CVBS_TVIN_MODE) 
    {
        tvin_exit();
    } 
    else 
    {
        ss_sensor_exit();
    }
    unregister_chrdev(MAJOR_VIDEO_IN, "video_in");

#ifdef __COVTEST__
    for (addr = &_dtor_start; addr < &_dtor_end; addr += 4)
    {
        addr1 = *(volatile unsigned int *)(addr);
        if (addr1 != 0)
        {
            ((void (*)(void))(addr1))();
        }
    }
#endif
    return;
}

MODULE_LICENSE("GPL");

#ifdef _ACT_UC_OS_
MODULE_PARM(vi_app_type, 'i');
#endif /* _ACT_UC_OS_ */
#ifdef _ACT_LINUX_KERNEL_2_6_
module_param(vi_app_type, uint, S_IRUGO);
#endif /* _ACT_LINUX_KERNEL_2_6_ */

EXPORT_SYMBOL(ss_sensor_open);
EXPORT_SYMBOL(ss_sensor_close);
EXPORT_SYMBOL(ss_sensor_ioctl);

#pragma __PRQA_IGNORE_START__
module_init(sensor_init_module);
module_exit(sensor_cleanup_module);
#pragma __PRQA_IGNORE_END__
/*! \endcond*/
/* end of cond INNER_SENSOR_API */

#endif  /* use old ucos driver api or not. */

