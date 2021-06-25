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
*     读函数，预留，没有实现
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
*     写函数，预留，没有实现
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
*     关闭已经打开的文件句柄
* \param[in]   none
* \param[out]  none       
* \return      the result  
* \retval      0 sucess 
* \retval      1 failed
* \par      exmaple code
* \code 
* int fd;
* fd = open("/dev/radio", O_RDWR);
* ……
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
*  ioctl接口，应用与驱动交互的主要接口，通过不同的命令字方问，命令字的详细说明见下表，
*  关键的数据结构见关键数据结构说明
* \param[in]   unsigned int cmd, define in fm_drv.h
* \param[in]   unsigned long arg, para for cmd
* \param[out]  none       
* \return      the result  
* \retval      0 sucess 
* \retval      other failed
* \code
*   CMD命令字说明
* \endcode 
<div class="generic_text_mapping_table">
 * <table>

 * <tr>
 * <th>命令</th>
 * <th>参数(arg的实际形式与意义)</th>
 * <th>命令参数功能说明</th>
 * </tr>

 * <tr>
 * <td>GET_RADIO_INFO</td>
 * <td> radio_info_t *info</td>
 * <td>获取当前电台的相关信息。传入描述信息的结构体的首指针，由底层向其中写入相关信息</td>
 * </tr>
 
 * <tr>
 * <td> GET_RADIO_INTENSITY </td>
 * <td>  int *arg: 用来保存当前电台强度大小的变量的指针</td>
 * <td>  获取当前电台强度</td>
 * </tr>
 
  * <tr>
 * <td> GET_RADIO_BAND_MODE </td>
 * <td>  arg:  欧美或者日本波段， </td>
 * <td>  获取当前电台的波段</td>
 * </tr>

 * <tr>
 * <td> GET_RADIO_FREQ </td>
 * <td>  arg:  欧美或者日本波段， </td>
 * <td>  获取当前电台的频率</td>
 * </tr>
 
  * <tr>
 * <td> GET_ANTENNA </td>
 * <td> 返回值为0，表示有天线插入，返回值为负值，表示天线未插入 </td>
 * <td> 调用此接口来判断是否已经有天线插入 </td>
 * </tr>
 
  * <tr>
 * <td> SET_RADIO_FREQ </td>
 * <td>  arg: 指定的频率，单位： 1KHz</td>
 * <td>  设定当前的播放频率。设置得到立即响应	返回参数：为0，表示当前频率是一个电台；为负值，表明当前频率无电台</td>
 * </tr>
 
  * <tr>
 * <td> SET_RADIO_PLAY </td>
 * <td> 无 </td>
 * <td> 播放。在调用STOP后,调用此接口恢复播放。（对于radio来说，只是静音） </td>
 * </tr>
 
  * <tr>
 * <td> SET_RADIO_STOP </td>
 * <td>  无</td>
 * <td>  停止播放，不退出应用。可用PLAY恢复</td>
 * </tr>
 
  * <tr>
 * <td> SET_RADIO_BAND_MODE </td>
 * <td>  arg:  US 为美洲模式，EURO为欧洲模式，JAPAN则为日本模式； </td>
 * <td>  调用此接口切换当前的频段模式。设置得到立即响应,返回参数：为0，表示设置成功；为负值，表明设置失败</td>
 * </tr>
 
  * <tr>
 * <td> SET_RADIO_THRESHOLD </td>
 * <td> arg为0－15的电台强度大小 </td>
 * <td> 设置电台强度，此强度作为搜索时的门限值 </td>
 * </tr>
 
 * <tr>
 * <td> SET_RADIO_AUTO_SEEK </td>
 * <td> seek_info_t *arg: 设置搜索条件 </td>
 * <td> 设置搜索条件，开始自动搜台</td>
 * </tr>

 * <tr>
 * <td> SET_RADIO_AUTO_SEEK_BREAK </td>
 * <td> arg: 无参数 </td>
 * <td> 跳出本次自动搜台</td>
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
*  insmod driver, 挂载驱动时调用此函数：注册FM驱动 
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
*  rmmod driver, 卸载驱动时调用此函数，取消注册FM驱动 
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

