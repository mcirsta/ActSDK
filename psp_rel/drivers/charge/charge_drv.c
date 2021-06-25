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
*    - �������� \n
*         ϵͳ����open���Ի�ö������ķ���Ȩ������һЩϵͳ��Դ�����ڴ棬�жϵȣ�
*                 ͬʱ�ú��������豸�ļ���ʶ����Ϊ����ϵͳ��������׼���� \n
*    - ����ԭ�� \n
*         int open(const char *pathname, int flags, int mode); \n
*    - ����������� \n
*         pathname �D�D ����pathname�ǽ�Ҫ���򿪵��豸�ļ������ƣ����磬������������һ����/dev/dsp�� \n \n
*         flags �D�D ����flags����ָ��Ӧ����ʲô��ʽ���豸�ļ�����������O_RDONLY��
*                           O_WRONLY����O_RDWR���ֱ��ʾ��ֻ����ֻд���߶�д�ķ�ʽ���豸�ļ���\n \n
*         mode �D�D ����modeͨ���ǿ�ѡ�ģ���ֻ����ָ�����豸�ļ�������ʱ�Ż��õ���
 *                          ָ���´��� ���ļ�Ӧ�þ���������Ȩ�ޡ� \n
*    - ����������� \n
*         ���openϵͳ�����ܹ��ɹ���ɣ���������һ����������Ϊ�ļ���ʶ���������� ϵͳ��������Ҫ�õ�
*          �ñ�ʶ�������openϵͳ����ʧ�ܣ���������-1��ͬʱ��������ȫ�ֱ���errno��ָ����ʲôԭ�����˴���ķ����� \n
*    - ʾ����˵�� \n
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
*    - �������� \n
*         һ��Ӧ�ó��������closeϵͳ���ã��ں˾ͻ��ͷ���֮��صĸ�����Դ��
*                                    ��˽����ڲ���Ҫ��ʱ������ʱ�ر��Ѿ��򿪵��豸�� \n
*    - ����ԭ�� \n
*         int close(int fd); \n
*    - ����������� \n
*         fd �D�D ���򿪵��豸�ļ��ı�ʶ����  \n
*    - ����������� \n
*         �ɹ�����Ϊ0��ʧ�ܷ���-1�� \n
*    - ʾ����˵�� \n
* \code
*         int fd;
*         fd = open("/dev/charge", O_RDWR);
*         ����
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
*    - �������� \n
*         ϵͳ����ioctl����ʵ�ָ��ֶ��豸�Ŀ������� \n
*    - ����ԭ�� \n
*         int ioctl(int fd, int request, ...); \n
*    - ����������� \n
*         fd �D�D ���򿪵��豸�ļ��ı�ʶ���� \n
*         request �D�D����request��Ŀ�ľ����������ֲ�ͬ�Ŀ������� \n
*         ͨ��˵�����ڶ��豸���п���ʱ����Ҫ��������������Ҫ���ݲ�ͬ�Ŀ����������ȷ����
*           ���ҿ�������Ӳ���豸ֱ����صġ� \n
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
 * <td>CHARGE_IOSET_START</td>
 * <td> no in use</td>
 * <td>��ʼ�������</td>
 * </tr>
 * <tr>
 * <td>CHARGE_IOSET_STOP</td>
 * <td> no in use</td>
 * <td>ֹͣ�������</td>
 * </tr>
 * <tr>
 * <td>CHARGE_IOSET_PARAM</td>
 * <td>  charge_param_t * ���� charge_drv.h </td>
 * <td> ���ó���������� charge_drv.h ��voltage��status��Ա��Ϊ����ֵ��no use ��</td>
 * </tr>
 * <tr>
 * <td>CHARGE_IOGET_PARAM</td>
 * <td> charge_param_t * ���� charge_drv.h </td>
 * <td>��ȡ����������ǰ���״̬</td>
 * </tr>
 * <tr>
 * <td>CHARGE_IOSET_PROCESS</td>
 * <td> no in use</td>
 * <td>ִ��һ�γ��ѭ������</td>
 * </tr>
 * <tr>
 * <td>CHARGE_IOGET_USB_STATE</td>
 * <td> int *,ָ�봫��ֵ</td>
 * <td>����USBӦ�ó����»�ȡ��ǰUSB����״̬��0~off, 1~on</td>
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
*    - �������� \n
*       ϵͳ����read�����Ӷ�Ӧ�豸��ȡ���ݡ�������ʵ��Ϊ�ա� \n
*    - ����ԭ�� \n
*       int read(int fd, char *buf, size_t count); \n
*    - ����������� \n
*       fd �D�D ���򿪵��豸�ļ��ı�ʶ���� \n
*       buf �D�D ����buf��ָ�򻺳������ַ�ָ�룬������������豸��õ����ݣ� \n
*       count �D�D ����count�������޶����豸��õ�����ֽ����� \n
*    - ����������� \n
*       ���readϵͳ���óɹ���ɣ��������ش��豸ʵ�ʶ�ȡ���ֽ�����ͨ��������count��ֵҪСһЩ��
*      ���readϵͳ����ʧ�ܣ���������-1��ͬʱ��������ȫ�ֱ���errno����ָ����ʲôԭ�����˴���ķ����� \n
*    - ʾ����˵�� \n
*       ������ҳ�棭����ʹ��ʾ��;
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
*    - �������� \n
*       ϵͳ����write�������Ӧ�豸д�����ݡ�������ʵ��Ϊ�ա� \n
*    - ����ԭ�� \n
*       size_t write(int fd, const char *buf, size_t count); \n
*    - ����������� \n
*       fd �D�D ���򿪵��豸�ļ��ı�ʶ����  \n
*       buf �D�D ����buf��ָ�򻺳������ַ�ָ�룬�û������򻺴���Ҫд���豸�����ݣ� \n
*       count �D�D ����count�������޶�д���豸������ֽ����� \n
*    - ����������� \n
*       ���writeϵͳ���óɹ���ɣ������������豸ʵ��д����ֽ��������readϵͳ����ʧ�ܣ���������-1��
*          ͬʱ��������ȫ�ֱ���errno����ָ����ʲôԭ�����˴���ķ����� \n
*    - ʾ����˵�� \n
*       ������ҳ�棭����ʹ��ʾ��;
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
