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
* Input��         cmd -- the corresponding command.
* Output��        0 -- success; others -- failed.
* Other��         
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
* Input��         type -- indicate is sensor dc, dv, pc camera or tvin.
* Output��        0 -- success; others -- failed.
* Other��         
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
* Input��         
* Output��        0 -- success; others -- failed.
* Other��         
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
* Input��         filp -- indicating DC, DV or Pc camera application.
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
* Output��        0 -- success; others -- failed.
* Other��         
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
* Input��         filp -- currently no use.
* Output��        0 -- success; others -- failed.
* Other��         
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
*    - �������� \n
*         ϵͳ����open���Ի�ö������ķ���Ȩ������һЩϵͳ��Դ�����ڴ棬�жϵȣ�
*         ͬʱ�ú��������豸�ļ���ʶ����Ϊ����ϵͳ��������׼���� \n
*    - ����ԭ�� \n
*         int open(const char *pathname, int flags, int mode); \n
*    - ����������� \n
*         pathname �D�D ����pathname�ǽ�Ҫ���򿪵��豸�ļ������ƣ����磬������������һ����/dev/dsp��\n 
*         flags �D�D ����flags����ָ��Ӧ����ʲô��ʽ���豸�ļ�����������O_RDONLY��
*         O_WRONLY����O_RDWR���ֱ��ʾ��ֻ����ֻд���߶�д�ķ�ʽ���豸�ļ���\n \n
*         mode �D�D ����modeͨ���ǿ�ѡ�ģ���ֻ����ָ�����豸�ļ�������ʱ�Ż��õ���
*         ָ���´��� ���ļ�Ӧ�þ���������Ȩ�ޡ� \n
*    - ����������� \n
*         ���openϵͳ�����ܹ��ɹ���ɣ���������һ����������Ϊ�ļ���ʶ����������
*         ϵͳ��������Ҫ�õ��ñ�ʶ�������openϵͳ����ʧ�ܣ���������-1��
*         ͬʱ��������ȫ�ֱ���errno��ָ����ʲôԭ�����˴���ķ����� \n
*    - ʾ����˵�� \n
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
*    - �������� \n
*         һ��Ӧ�ó��������closeϵͳ���ã��ں˾ͻ��ͷ���֮��صĸ�����Դ��
*         ��˽����ڲ���Ҫ��ʱ������ʱ�ر��Ѿ��򿪵��豸�� \n
*    - ����ԭ�� \n
*         int close(int fd); \n
*    - ����������� \n
*         fd �D�D ���򿪵��豸�ļ��ı�ʶ����  \n
*    - ����������� \n
*         �ɹ�����Ϊ0��ʧ�ܷ���-1�� \n
*    - ʾ����˵�� \n
* \code
*         int fd;
*         fd = open("/dev/ada", O_RDWR);
*         ����
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
*    - �������� \n
*       ϵͳ����read�����Ӷ�Ӧ�豸��ȡ���ݡ� \n
*    - ����ԭ�� \n
*       int read(int fd, char *buf, size_t count); \n
*    - ����������� \n
*       fd �D�D ���򿪵��豸�ļ��ı�ʶ���� \n
*       buf �D�D ����buf��ָ�򻺳������ַ�ָ�룬������������豸��õ����ݣ� \n
*       count �D�D ����count�������޶����豸��õ�����ֽ����� \n
*    - ����������� \n
*       ���readϵͳ���óɹ���ɣ��������ش��豸ʵ�ʶ�ȡ���ֽ�����ͨ��������count��ֵҪСһЩ��
*       ���readϵͳ����ʧ�ܣ���������-1��ͬʱ��������ȫ�ֱ���errno��
*       ��ָ����ʲôԭ�����˴���ķ����� \n
*    - ʾ����˵�� \n
*       ������ҳ�棭����ʹ��ʾ��;
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
*    - �������� \n
*       ϵͳ����write�������Ӧ�豸д�����ݡ� \n
*    - ����ԭ�� \n
*       size_t write(int fd, const char *buf, size_t count); \n
*    - ����������� \n
*       fd �D�D ���򿪵��豸�ļ��ı�ʶ����  \n
*       buf �D�D ����buf��ָ�򻺳������ַ�ָ�룬�û������򻺴���Ҫд���豸�����ݣ� \n
*       count �D�D ����count�������޶�д���豸������ֽ����� \n
*    - ����������� \n
*       ���writeϵͳ���óɹ���ɣ������������豸ʵ��д����ֽ��������readϵͳ����ʧ�ܣ�
*       ��������-1��ͬʱ��������ȫ�ֱ���errno����ָ����ʲôԭ�����˴���ķ����� \n
*    - ʾ����˵�� \n
*       ������ҳ�棭����ʹ��ʾ��;
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
*    - �������� \n
*         ϵͳ����ioctl����ʵ�ָ��ֶ��豸�Ŀ������� \n
*    - ����ԭ�� \n
*         int ioctl(int fd, int request, ...); \n
*    - ����������� \n
*         fd �D�D ���򿪵��豸�ļ��ı�ʶ���� \n
*         request �D�D����request��Ŀ�ľ����������ֲ�ͬ�Ŀ������� \n
*         ͨ��˵�����ڶ��豸���п���ʱ����Ҫ��������������Ҫ���ݲ�ͬ�Ŀ����������ȷ����
*         ���ҿ�������Ӳ���豸ֱ����صġ� \n
*    - ����������� \n
*         �ɹ�����Ϊ0��ʧ�ܷ���-1�� \n
*    - ʾ����˵�� \n
*         �й�ioctl������������ʾ����˵��������������ֵ�ʾ����˵����
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
*    - �������� \n
*         ϵͳ����mmap����ʵ��ӳ��һ�������ڴ浽�û��ռ䡣
*    - ����ԭ�� \n
*    - ����������� \n
*    - ����������� \n
*    - ʾ����˵�� \n
*         �й�mmap��鿴���linux��ϵͳ���ýӿ��ĵ���
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

