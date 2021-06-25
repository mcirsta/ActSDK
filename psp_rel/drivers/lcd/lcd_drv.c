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
*    - �������� \n
*         ϵͳ����open���Ի�ö������ķ���Ȩ������һЩϵͳ��Դ�����ڴ棬�жϵȣ�ͬʱ�ú��������豸�ļ���ʶ����
*         Ϊ����ϵͳ��������׼���� \n
*    - ����ԭ�� \n
*         int open(const char *pathname, int flags, int mode); \n
*    - ����������� \n
*         pathname �D�D ����pathname�ǽ�Ҫ���򿪵��豸�ļ������ƣ����磬������������һ����/dev/dsp�� \n \n
*         flags �D�D ����flags����ָ��Ӧ����ʲô��ʽ���豸�ļ�����������O_RDONLY��O_WRONLY����O_RDWR��
*                            �ֱ��ʾ��ֻ����ֻд���߶�д�ķ�ʽ���豸�ļ���\n \n
*         mode �D�D ����modeͨ���ǿ�ѡ�ģ���ֻ����ָ�����豸�ļ�������ʱ�Ż��õ���
*                              ָ���´��� ���ļ�Ӧ�þ���������Ȩ�ޡ� \n
*    - ����������� \n
*         ���openϵͳ�����ܹ��ɹ���ɣ���������һ����������Ϊ�ļ���ʶ���������� ϵͳ��������Ҫ�õ��ñ�ʶ����
*         ���openϵͳ����ʧ�ܣ���������-1��ͬʱ��������ȫ�ֱ���errno��ָ����ʲôԭ�����˴���ķ����� \n
*    - ʾ����˵�� \n
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
*         fd = open("/dev/lcd", O_RDWR);
*         ����
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
*    - �������� \n
*         ϵͳ����ioctl����ʵ�ָ��ֶ��豸�Ŀ������� \n
*    - ����ԭ�� \n
*         int ioctl(int fd, int request, ...); \n
*    - ����������� \n
*         fd �D�D ���򿪵��豸�ļ��ı�ʶ���� \n
*         request �D�D����request��Ŀ�ľ����������ֲ�ͬ�Ŀ������� \n
*         ͨ��˵�����ڶ��豸���п���ʱ����Ҫ��������������Ҫ���ݲ�ͬ�Ŀ����������ȷ����
*          ���ҿ�������Ӳ���豸ֱ����صġ� \n
*    - ����������� \n
*         �ɹ�����Ϊ0��ʧ�ܷ���-1�� \n
*    - ʾ����˵�� \n
*         �й�ioctl������������ʾ����˵��������������ֵ�ʾ����˵����
*
*
* \par  ioctl()����������:
*   ��ϸ����ioctlԭ�ͣ�int ioctl(int fd, int request, unsigned long arg, ...) �в���request�ĸ�ѡ���\n
*   
*
<div class="generic_text_mapping_table">
* <table>
 * <tr>
 * <th>����</th>
 * <th>����(arg��ʵ����ʽ������)</th>
 * <th>�����������˵��</th>
 * </tr>
 
 * <tr>
 * <td>DISPGET_DISP_ATTR</td>
 * <td> disp_dev_t *arg,see display_drv.h </td>
 * <td>��ȡ��ǰ�豸�Ĳ���������</td>
 * </tr>
 
 * <tr>
 * <td>DISPSET_UPDATE</td>
 * <td>disp_frame_info_t *arg��see display_drv.h</td>
 * <td>ˢ��һ֡����</td>
 * </tr>
 
 * <tr>
 * <td>DISPSET_LCD_BACKLIGHT</td>
 * <td>int,����ȼ��� ,0~5</td>
 * <td>��0��Ϊ�ر��⣬��1�������5������</td>
 * </tr>
 
 * <tr>
 * <td>DISPGET_LCD_BACKLIGHT</td>
 * <td>int* �����ر���ȼ� </td>
 * <td>��ȡLCD����ȼ���0~5</td>
 * </tr>
 
 * <tr>
 * <td>DISPSET_LCD_STANDBY</td>
 * <td> int ��0�˳���1����</td>
 * <td>����LCD����/�˳�standby</td>
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
*        ���readϵͳ����ʧ�ܣ���������-1��ͬʱ��������ȫ�ֱ���errno����ָ����ʲôԭ�����˴���ķ����� \n
*    - ʾ����˵�� \n
*       ������ҳ�棭����ʹ��ʾ��;
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
*    - �������� \n
*       ϵͳ����write�������Ӧ�豸д�����ݡ� \n
*    - ����ԭ�� \n
*       size_t write(int fd, const char *buf, size_t count); \n
*    - ����������� \n
*       fd �D�D ���򿪵��豸�ļ��ı�ʶ����  \n
*       buf �D�D ����buf��ָ�򻺳������ַ�ָ�룬�û������򻺴���Ҫд���豸�����ݣ� \n
*       count �D�D ����count�������޶�д���豸������ֽ����� \n
*    - ����������� \n
*       ���writeϵͳ���óɹ���ɣ������������豸ʵ��д����ֽ��������readϵͳ����ʧ�ܣ���������-1��
*        ͬʱ��������ȫ�ֱ���errno����ָ����ʲôԭ�����˴���ķ����� \n
*    - ʾ����˵�� \n
*       ������ҳ�棭����ʹ��ʾ��;
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


