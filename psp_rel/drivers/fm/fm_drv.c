/********************************************************************************
*                              
*                            Module: FM Drvier
*                 Copyright(c) 2001-2007 Actions Semiconductor,
*                            All Rights Reserved. 
*
* History:         
*      <author>    <time>           <version >             <desc>
*       xkping     2008-07-03 09:00     1.0             build this file 
********************************************************************************/
/*!
 * \file   fm_drv.c
 * \brief  
 * \author xkping
 * \par GENERAL DESCRIPTION:
 *       In This File 
 *       1)Define the functions which will be called by application        
 * \par EXTERNALIZED FUNCTIONS:
 *     Copyright(c) 2001-2007 Actions Semiconductor, All Rights Reserved.
 *
 *  \version 1.0
 *  \date  2008/7/3
 *******************************************************************************/

#include "fm_inner.h"


#define FM_MAJOR  239

#ifdef  _ACT_LINUX_KERNEL_2_6_
MODULE_LICENSE("GPL");
#endif  //end of '#ifdef  _ACT_LINUX_KERNEL_2_6_'

/*! \cond FM_API*/
/******************************************************************************/
/*!                    
* \par  Description:
*     init host hardware register;
*     init fm hardware register; 
*     init some global value;
* \param[in]   none
* \param[out]  none       
* \return      the result  
* \retval      0 sucess 
* \retval      other failed
* \par      exmaple code
* \code 
*   int fd;
*   fd = open("/dev/radio", O_RDWR);
* \endcode
*******************************************************************************/

int radio_open(struct inode *inode, struct file *filp)
{
    int flag = act_radio_open();
    
    return flag;
}

/*! \endcond*/


/*! \cond FM_API*/
/******************************************************************************/
/*!                    
* \par  Description:
*     ��������Ԥ����û��ʵ��
* \param[in]   none
* \param[out]  none           
* \return      the result  
* \retval      0 sucess 
* \retval      1 failed
* \par      exmaple code
* \code 
*            
* \endcode
*******************************************************************************/

ssize_t radio_read(struct file *filp, char *buf, size_t count,
                loff_t *f_pos)
{
    return act_radio_read();
}

/*! \endcond*/


/*! \cond FM_API*/
/******************************************************************************/
/*!                    
* \par  Description:
*     д������Ԥ����û��ʵ��
* \param[in]   none
* \param[out]  none           
* \return      the result  
* \retval      0 sucess 
* \retval      1 failed
* \par      exmaple code
* \code 
*           none
* \endcode
*******************************************************************************/

ssize_t  radio_write(struct file *filp, const char *buf, size_t count,
                loff_t *f_pos)
{
    return  act_radio_write();
}
/*! \endcond*/


/*! \cond FM_API*/
/******************************************************************************/
/*!                    
* \par  Description:
*     �ر��Ѿ��򿪵��ļ����
* \param[in]   none
* \param[out]  none       
* \return      the result  
* \retval      0 sucess 
* \retval      1 failed
* \par      exmaple code
* \code 
* int fd;
* fd = open("/dev/radio", O_RDWR);
* ����
* close(fd);
          
* \endcode
*******************************************************************************/

int radio_release(struct inode *inode, struct file *filp)
{
    return  act_radio_release();
}
/*! \endcond*/


/*! \cond FM_API*/
/******************************************************************************/
/*!                    
* \par  Description:
*  ioctl�ӿڣ�Ӧ����������������Ҫ�ӿڣ�ͨ����ͬ�������ַ��ʣ������ֵ���ϸ˵�����±�
*  �ؼ������ݽṹ���ؼ����ݽṹ˵��
* \param[in]   unsigned int cmd, define in fm_drv.h
* \param[in]   unsigned long arg, para for cmd
* \param[out]  none       
* \return      the result  
* \retval      0 sucess 
* \retval      other failed
* \code
*   CMD������˵��
* \endcode 
<div class="generic_text_mapping_table">
 * <table>

 * <tr>
 * <th>����</th>
 * <th>����(arg��ʵ����ʽ������)</th>
 * <th>�����������˵��</th>
 * </tr>

 * <tr>
 * <td>GET_RADIO_INFO</td>
 * <td> radio_info_t *info</td>
 * <td>��ȡ��ǰ��̨�������Ϣ������������Ϣ�Ľṹ�����ָ�룬�ɵײ�������д�������Ϣ</td>
 * </tr>
 
 * <tr>
 * <td> GET_RADIO_INTENSITY </td>
 * <td>  int *arg: �������浱ǰ��̨ǿ�ȴ�С�ı�����ָ��</td>
 * <td>  ��ȡ��ǰ��̨ǿ��</td>
 * </tr>
 
  * <tr>
 * <td> GET_RADIO_BAND_MODE </td>
 * <td>  arg:  ŷ�������ձ����Σ� </td>
 * <td>  ��ȡ��ǰ��̨�Ĳ���</td>
 * </tr>

 * <tr>
 * <td> GET_RADIO_FREQ </td>
 * <td>  arg:  ŷ�������ձ����Σ� </td>
 * <td>  ��ȡ��ǰ��̨��Ƶ��</td>
 * </tr>
 
  * <tr>
 * <td> GET_ANTENNA </td>
 * <td> ����ֵΪ0����ʾ�����߲��룬����ֵΪ��ֵ����ʾ����δ���� </td>
 * <td> ���ô˽ӿ����ж��Ƿ��Ѿ������߲��� </td>
 * </tr>
 
  * <tr>
 * <td> SET_RADIO_FREQ </td>
 * <td>  arg: ָ����Ƶ�ʣ���λ�� 1KHz</td>
 * <td>  �趨��ǰ�Ĳ���Ƶ�ʡ����õõ�������Ӧ	���ز�����Ϊ0����ʾ��ǰƵ����һ����̨��Ϊ��ֵ��������ǰƵ���޵�̨</td>
 * </tr>
 
  * <tr>
 * <td> SET_RADIO_PLAY </td>
 * <td> �� </td>
 * <td> ���š��ڵ���STOP��,���ô˽ӿڻָ����š�������radio��˵��ֻ�Ǿ����� </td>
 * </tr>
 
  * <tr>
 * <td> SET_RADIO_STOP </td>
 * <td>  ��</td>
 * <td>  ֹͣ���ţ����˳�Ӧ�á�����PLAY�ָ�</td>
 * </tr>
 
  * <tr>
 * <td> SET_RADIO_BAND_MODE </td>
 * <td>  arg:  US Ϊ����ģʽ��EUROΪŷ��ģʽ��JAPAN��Ϊ�ձ�ģʽ�� </td>
 * <td>  ���ô˽ӿ��л���ǰ��Ƶ��ģʽ�����õõ�������Ӧ,���ز�����Ϊ0����ʾ���óɹ���Ϊ��ֵ����������ʧ��</td>
 * </tr>
 
  * <tr>
 * <td> SET_RADIO_THRESHOLD </td>
 * <td> argΪ0��15�ĵ�̨ǿ�ȴ�С </td>
 * <td> ���õ�̨ǿ�ȣ���ǿ����Ϊ����ʱ������ֵ </td>
 * </tr>
 
 * <tr>
 * <td> SET_RADIO_AUTO_SEEK </td>
 * <td> seek_info_t *arg: ������������ </td>
 * <td> ����������������ʼ�Զ���̨</td>
 * </tr>

 * <tr>
 * <td> SET_RADIO_AUTO_SEEK_BREAK </td>
 * <td> arg: �޲��� </td>
 * <td> ���������Զ���̨</td>
 * </tr> 
 * </table>
 * </div>

*******************************************************************************/

int radio_ioctl(struct inode *inode, struct file *filp, unsigned int cmd,
             unsigned long arg)
{
    return  act_radio_ioctl(cmd, arg);
}
/*! \endcond*/

#pragma __PRQA_IGNORE_START__
struct file_operations radio_fops = {
    open:       radio_open,
    release:    radio_release,
    read:       radio_read,
    write:      radio_write,    
    ioctl:      radio_ioctl,
};
#pragma __PRQA_IGNORE_END__
/*! \cond FM_API*/
/******************************************************************************/
/*!                    
* \par  Description:
*  insmod driver, ��������ʱ���ô˺�����ע��FM���� 
* \param[in]   none
* \param[out]  none       
* \return      the result  
* \retval      0 sucess 
* \retval      other failed
* \par      exmaple code
* \code 
*            
* \endcode
*******************************************************************************/

int radio_init_module(void)
{
    int result;
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
    result = register_chrdev(FM_MAJOR, "fm", &radio_fops);

    if (result < 0) 
    {
        printk(KERN_WARNING "fm: can't get major %d\n", FM_MAJOR);
        return result;
    }
    return 0; /* succeed */
}

/*! \endcond*/


/*! \cond FM_API*/
/*!                    
* \par  Description:
*  rmmod driver, ж������ʱ���ô˺�����ȡ��ע��FM���� 
* \param[in]   none
* \param[out]  none       
* \return      the result  
* \retval      0 sucess 
* \retval      other failed
* \par      exmaple code
* \code 
*            
* \endcode
*******************************************************************************/

void radio_cleanup_module(void)
{
    unregister_chrdev(FM_MAJOR, "radio");

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
/*! \endcond*/


#pragma __PRQA_IGNORE_START__
module_init(radio_init_module);
module_exit(radio_cleanup_module);

#pragma __PRQA_IGNORE_END__

