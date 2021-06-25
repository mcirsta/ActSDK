/*
********************************************************************************
*                   213x --- usdk130 source file
*                (c) Copyright 2003 - 2008, Actions Co,Ld. 
*                        All Right Reserved 
*
* History:        
*      <author>      <time>       <version >    <desc>
*       houjingkun      2008/07/10     1.0      build this file 
*                                               ada char device driver.
********************************************************************************/ 
/*!
 * \file   ada_drv.c
 * \brief  
 *      ada char device driver.
 * \author houjingkun
 * \par GENERAL DESCRIPTION:
 *      ada驱动包括对DAC和ADC的控制，用以实现音频播放和录制的硬件相关控制。
 * \par EXTERNALIZED FUNCTIONS:
 *       null
 *
 *  Copyright(c) 2003-2008 Actions Semiconductor, All Rights Reserved.
 *
 * \version 1.0
 * \date  2008/07/10
 *******************************************************************************/

#include "ada_config.h"
#include "ada_hw.h"
#include "ada_sys.h"
#include "ada_inner.h"


/******************************************************************************/
/*!                    
* \par  Description:
*     realized for file operation -- open();
* \param[in]   inode -- opened file's inode structure pointer, refer to struct inode.
* \param[in]   file -- opened file's file structure pointer, refer to struct file.
* \retval      0 success, else failed.
*******************************************************************************/
static int ada_open(struct inode *inode, struct file *file)
{
    ada_dev_t *p_ada;
    int ret = 0;

    //p_ada = (ada_dev_t *)malloc_dma(sizeof(ada_dev_t));
    p_ada = (ada_dev_t *) &ada_dev;
    file->private_data = p_ada;

    p_ada->open_cnt++;
    if (p_ada->open_cnt > 1)
    {
        return 0;
    }
    ret = ada_inner_open();

    return ret;
}

/******************************************************************************/
/*!                    
* \par  Description:
*     realized for file operation -- close();
* \param[in]   inode -- opened file's inode structure pointer, refer to struct inode.
* \param[in]   file -- opened file's file structure pointer, refer to struct file.
* \retval      0 success, else failed.
*******************************************************************************/
static int ada_release(struct inode *inode, struct file *file)
{
    ada_dev_t *p_ada = (ada_dev_t *)file->private_data;
    int ret = 0;

    p_ada->open_cnt--;
    if (p_ada->open_cnt > 0)
    {
        return 0;
    }
    ret = ada_inner_close();

    return ret;
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
*******************************************************************************/
static int ada_ioctl(struct inode *inode, struct file *file, unsigned int cmd, unsigned long arg)
{
    //printk("ada_ioctl:%x, %d\n", cmd, cmd);
    switch (cmd & 0xffffff00) 
    {
    case DACIO_CMD_BASE:
        return dac_ioctl(cmd, arg);
    case ADCIO_CMD_BASE:
        return adc_ioctl(cmd, arg);
    default:
        return -1;
    }
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
*******************************************************************************/
static ssize_t ada_read(struct file *file, char __user *buf,
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
*******************************************************************************/
static ssize_t ada_write(struct file *file, const char __user *buf,
             size_t count, loff_t *ppos)
{
    unsigned int ret = 0;
    ada_dev_t *p_ada = file->private_data;

#if USE_PCM_PLAY_WRITE
    unsigned int phy_addr, buf_len;
    unsigned int cnt;
    char *buffer;
    static int index = 0;

    buffer = (char *)buf;

    /* reserve pre setting. */
    phy_addr = p_ada->phy_addr;
    buf_len = p_ada->buf_len;

    do
    {
        if (count <= PCM_FRAME_LEN)
        {
            //printk("least cnt:%d\n", count);
            cnt = count;

            /* if write buffer's lenth is less than MIN_FRAME_LEN, will no time to enter irq hander. */
#if 0
            if (cnt < MIN_FRAME_LEN)
                return 0;
#endif
        }
        else
        {
            cnt = PCM_FRAME_LEN;
        }

        copy_from_user((void *)p_ada->pcm_data[index], buffer, cnt);
        buffer += cnt;
        p_ada->phy_addr = _PA(p_ada->pcm_data[index]);
        p_ada->buf_len = cnt;
        ret += cnt;

        pcm_play();

        if (index != 0)
        {
            index = 0;
        }
        else
        {
            index = 1;
        }

        count -= cnt;
    }
    while (count != 0);

    p_ada->phy_addr = phy_addr;
    p_ada->buf_len = buf_len; 
#endif  /* USE_PCM_PLAY_WRITE */ 

    return ret;
}

/*! \cond INNER_ADA_API*/
/******************************************************************************/
/*!                    
* \par  Description:
*     realized for file operation -- llseek();
* \param[in]   file -- opened file's file structure pointer, refer to struct file.
* \param[in]   offset -- seek's offset value.
* \param[in]   origin -- seek's start position.
* \retval      current file position.
*******************************************************************************/
static loff_t ada_llseek(struct file *file, loff_t offset, int origin)
{
    int ret = 0;
    /* TODO */
    return ret; 
}


#pragma __PRQA_IGNORE_START__
/* The various file operations we support. */
static struct file_operations ada_fops = {
    .owner      = THIS_MODULE,
    .llseek     = ada_llseek,
    .read       = ada_read,
    .write      = ada_write,
    .open       = ada_open,
    .release    = ada_release,
    .ioctl      = ada_ioctl,
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
static int ada_init(void)
{
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
    int ret = 0;

    ret = ada_dev_init();
    if (ret < 0)
    {
        goto out;
    }
    ret = register_chrdev(MAJOR_ADA, "ada", &ada_fops);
    if (ret < 0)
    {
        printk(KERN_ERR "ada: unable to get major %d\n", MAJOR_ADA);
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
static void ada_exit(void)
{
    ada_dev_exit();
    unregister_chrdev(MAJOR_ADA, "ada");
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

EXPORT_SYMBOL(ada_set_dsp_mute);
EXPORT_SYMBOL(ada_get_volum_state);
EXPORT_SYMBOL(dac_ioctl);
EXPORT_SYMBOL(adc_ioctl);

#pragma __PRQA_IGNORE_START__
module_init(ada_init);
module_exit(ada_exit);
#pragma __PRQA_IGNORE_END__
/*! \endcond*/
/* end of cond INNER_ADA_API */


