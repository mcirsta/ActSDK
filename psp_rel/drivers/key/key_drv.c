/********************************************************************************
*                              ������д��Ŀ��
*                            Module: key Drvier
*                 Copyright(c) 2001-2007 Actions Semiconductor,
*                            All Rights Reserved. 
*
* History:         
*      <author>    <time>           <version >             <desc>
*       xkping     2008-07-03 09:00     1.0             build this file 
********************************************************************************/
/*!
 * \file   key_drv.c
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

#include "key_inner.h"




#ifdef  _ACT_LINUX_KERNEL_2_6_
MODULE_LICENSE("GPL");
#endif  //end of '#ifdef  _ACT_LINUX_KERNEL_2_6_'


/*! \cond KEY_API*/
/******************************************************************************/
/*!                    
* \par  Description:
*   ��ʼ��key scanģ�飬��ʼ�������������͵����ã�������Դ����ʼ��ȫ�ֱ���  
* \param[in]   none
* \param[out]  none       
* \return      the result  
* \retval      0 sucess 
* \retval      other failed
* \par      exmaple code
* \code 
*          int fd;
*          fd = open("/dev/buttons", O_RDWR);

* \endcode
*******************************************************************************/

int buttons_open(struct inode *inode, struct file *filp)
{
    int flag = act_buttons_open();
    
    return flag;
}
/*! \endcond*/

/*! \cond KEY_API*/
/******************************************************************************/
/*!                    
* \par  Description:
* we simulate a queue in a arrar buffer, since it's small size, we just shift left  
* when we read the key messages.
* \param[in]   INT32U * buffer, dest. addr of reading
* \param[in]   INT32U cnt, the number of byte to read 
* \param[out]  none       
* \return      the result  
* \retval      cnt ,reading how many byte 
* \retval      1 failed
* \par      exmaple code
* \code 
*           �ο���ȡ������ʾ������
* \endcode
*******************************************************************************/
ssize_t buttons_read(struct file *filp, char *buf, size_t count,loff_t *f_pos)
{
    return act_buttons_read(buf, count);
}
/*! \endcond*/

/*! \cond KEY_API*/
/******************************************************************************/
/*!                    
* \par  Description:
*    buttons_write,write data to driver,reserve not use ��Ԥ�� ��û��ʵ�� 
* \param[in]   none
* \param[out]  none       
* \return      the result  
* \retval      0 sucess 
* \retval      other failed
* \par      exmaple code
* \code 
*           none
* \endcode
*******************************************************************************/
ssize_t  buttons_write(struct file *filp, const char *buf, size_t count, loff_t *f_pos)
{
    return  act_buttons_write();
}
/*! \endcond*/


/*! \cond KEY_API*/
/******************************************************************************/
/*!                    
* \par  Description:
*     this function is called when close key driver,
*     realse irq isr and relase some important resource 
* \param[in]   none
* \param[out]  none       
* \return      the result  
* \retval      0 sucess 
* \retval      other failed
* \par      exmaple code
* \code 
*   int fd;
*   fd = open("/dev/buttons", O_RDWR);
*   ��
*   close(fd);
* \endcode
*******************************************************************************/

int buttons_release(struct inode *inode, struct file *filp)
{
    return  act_buttons_release();
}
/*! \endcond*/


/*! \cond KEY_API*/
/******************************************************************************/
/*!                    
* \par  Description:
*  ioctl�ӿڣ�Ӧ����������������Ҫ�ӿڣ�ͨ����ͬ�������ַ��ʣ������ֵ���ϸ˵�����±�
*  �ؼ������ݽṹ���ؼ����ݽṹ˵��
* \param[in]   unsigned int cmd, define in key_drv.h
* \param[in]   unsigned long arg, para for cmd
* \param[out]  unsigned long arg       
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
 * <td>KEY_GET_LOCKKEY_STATU</td>
 * <td> int *lock_statu,�������</td>
 * <td>��ȡ������״̬, lock_statu�� 0: unhold; 1: hold</td>
 * </tr>
 
 * </table>
 * </div>

*******************************************************************************/
int buttons_ioctl(struct inode *inode, struct file *filp, unsigned int cmd, unsigned long arg)
{
    return  act_buttons_ioctl(cmd, arg);
}
/*! \endcond*/



#if 0
static unsigned int buttons_select(struct file *file,struct poll_table_struct *wait)
{
    return act_buttons_select(file,wait);
}
#endif

#pragma __PRQA_IGNORE_START__
struct file_operations button_fops = 
{
    owner:   THIS_MODULE,
    ioctl:   buttons_ioctl,
//  poll:    buttons_select,
    read:    buttons_read,
    open:    buttons_open,
    release: buttons_release,
};
#pragma __PRQA_IGNORE_END__
/*! \cond KEY_API*/
/******************************************************************************/
/*!                    
* \par  Description:
*  insmod driver, ��������ʱ���ô˺�����ע�ᰴ������ 
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
int button_init_module(void)
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
    result = register_chrdev(MAJOR_KEY, "key", &button_fops);
    if (result < 0) 
    {
        printk("button: can't get major %d\n", MAJOR_KEY);
        return result;
    }
    
    key_init();
    
    return 0; /* succeed */
}
/*! \endcond*/


/*! \cond KEY_API*/
/*!                    
* \par  Description:
*  rmmod driver, ж������ʱ���ô˺�����ȡ��ע�ᰴ������ 
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

void button_cleanup_module(void)
{
    unregister_chrdev(MAJOR_KEY, "key");
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

    key_exit();
       
    return;
}
/*! \endcond*/
#pragma __PRQA_IGNORE_START__
module_init(button_init_module);
module_exit(button_cleanup_module);
#pragma __PRQA_IGNORE_END__

EXPORT_SYMBOL(buttons_open);
EXPORT_SYMBOL(buttons_ioctl);
EXPORT_SYMBOL(buttons_read);
EXPORT_SYMBOL(buttons_write);
EXPORT_SYMBOL(buttons_release);
EXPORT_SYMBOL(button_init_module);
EXPORT_SYMBOL(button_cleanup_module);

