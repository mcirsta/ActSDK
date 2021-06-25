/*
 ********************************************************************************
 213x -- i2c driver
 *                (c) Copyright 2007 - 2010, Actions Co,Ld.
 *                        All Right Reserved
 *
 * History:
 *      <author>      <time>       <version >    <desc>
 *       houjingkun      2008/06/18     1.0         build this file
 ********************************************************************************/
/*!
 * \file   i2c.c
 * \brief
 *      i2c driver hardware layer source file.
 * \author houjingkun
 * \par GENERAL DESCRIPTION:
 *      provide some functions such as i2c_read and i2c_write and so on.
 * \par EXTERNALIZED FUNCTIONS:
 *       null
 *
 *     Copyright(c) 2008-2012 Actions Semiconductor, All Rights Reserved.
 *
 * \version 1.0
 * \date  2008/06/18
 *******************************************************************************/

#include "i2c.h"
#include "fm_sys.h"
#include "fm_config.h"
/* i2c driver's info. */
typedef struct
{
    i2c_type_t i2c_type; /* reserved, maybe used for sensor's sccb bus(a bus semiliar with i2c bus) and so on. */
    unsigned int i2c_port; /* i2c port selection. */

    /* below is i2c related register's address. */
    unsigned int I2C_CTL;
    unsigned int I2C_CLKDIV;
    unsigned int I2C_STAT;
    unsigned int I2C_ADDR;
    unsigned int I2C_DAT;
} i2c_dev_t;

/*********** below is static Function used just in this file. **************/
//void udelay(unsigned int time);
//int i2c_start(void *inst, unsigned int addr);
//void i2c_stop(void *inst);
//int trans_ov(void *inst);
//int recv_ov(void *inst);
/***************************************************************************/

#ifdef  _I2C_NO_OS_FUNC_
static i2c_dev_t i2c_dev;

/********************************************************************************
 udelay:
 Input Parameter:
 usecond count;
 Output Parameter:
 void;
 ********************************************************************************/
//void udelay(unsigned int time)
//{
//    volatile unsigned int i, j;
//
//    for (i = 0; i < time; i++)
//    {
//        for (j = 0; j < 12000; j++)
//        {
//            do {;} while (0);
//        }
//    }
//    return;
//}
#endif  /* _I2C_NO_OS_FUNC_ */

unsigned int saved_gpio1;
unsigned int i2c_gpio;
unsigned int saved_i2c;
void i2c_get_gpio(int port)
{
    
    OS_INT_SAVE_AREA
    OS_ENTER_CRITICAL();
    if (port == 0)
    {
        saved_gpio1 = act_readl(GPIO_MFCTL1);
        saved_i2c = (saved_gpio1 & 0x00000030)>>4;
        i2c_gpio = saved_gpio1 & 0xffffffcf; //multi-function: I2C1 SCL and SDA
        i2c_gpio = i2c_gpio | 0x80000000; //multi-function enable
        act_writel(i2c_gpio, GPIO_MFCTL1);
    }
    else
    {
        saved_gpio1 = act_readl(GPIO_MFCTL1);
        saved_i2c = (saved_gpio1 & 0x000000c0)>>6;
        i2c_gpio = saved_gpio1 | 0x00000080;
        i2c_gpio = i2c_gpio & 0xffffffbf; //multi-function: I2C1 SCL and SDA
        i2c_gpio = i2c_gpio | 0x80000000; //multi-function enable
        act_writel(i2c_gpio, GPIO_MFCTL1);	
    }
    OS_EXIT_CRITICAL();
    udelay(10);
}
void i2c_release_gpio(int port)
{
    OS_INT_SAVE_AREA
    OS_ENTER_CRITICAL();
    if (port == 0) 
    {
        saved_gpio1 = act_readl(GPIO_MFCTL1);
        saved_gpio1 = saved_gpio1 & 0xffffffcf;
        saved_gpio1 = saved_gpio1 | (saved_i2c << 4);
        act_writel(saved_gpio1, GPIO_MFCTL1);
    }
    else
    {
        saved_gpio1 = act_readl(GPIO_MFCTL1);
        saved_i2c = (saved_gpio1 & 0x000000c0)>>6;
        i2c_gpio = saved_gpio1 | 0x00000080;
        i2c_gpio = i2c_gpio & 0xffffffbf; //multi-function: I2C1 SCL and SDA
        i2c_gpio = i2c_gpio | 0x80000000; //multi-function enable
        act_writel(i2c_gpio, GPIO_MFCTL1);	
    }
    OS_EXIT_CRITICAL();
    udelay(10);
}

/********************************************************************************
 I2C Transfer complete:
 Input Parameter:
 inst -- a instance's pointer that i2c_open returned.
 Output Parameter:
 0:ACK;
 -1:NACK
 ********************************************************************************/
int trans_ov(void *inst)
{
    unsigned int tmp = 0;
    unsigned int times = MAX_ACK_TIME;
    i2c_dev_t *pI2c = (i2c_dev_t *) inst;

    do
    {
        tmp = act_readl(pI2c->I2C_STAT);
        tmp &= 0x80;
        --times;
    } while ((tmp == 0) && (times > 0));

    if (times == 0)
    {
        printk("trans_ov time out\n ");
        return -1; //failure
    }

    act_writel(0x00000080, pI2c->I2C_STAT);
    tmp = act_readl(pI2c->I2C_CTL);
    tmp = tmp & 0x1; //ack or nack

    if (tmp != 0)
    {
        return 0;
    }
    else
    {
        return -1;
    }
}

/********************************************************************************
 I2C Start:
 Input Parameter:
 inst -- a instance's pointer that i2c_open returned.
 addr -- i2c slave address that read or write.
 Output Parameter:
 0 - success;
 else fail.
 ********************************************************************************/
int i2c_start(void *inst, unsigned int addr)
{
    i2c_dev_t *pI2c = (i2c_dev_t *) inst;

    act_writel(addr, pI2c->I2C_ADDR); //slave address
    act_writel(0x00000086, pI2c->I2C_CTL); //start condition

    return trans_ov(inst); //1:ack  0:nack
}

/********************************************************************************
 I2C i2c_restart:
 Input Parameter:
 inst -- a instance's pointer that i2c_open returned.
 addr -- i2c slave address that read or write.
 Output Parameter:
 0 - success;
 else fail.
 ********************************************************************************/
int i2c_restart(void *inst, unsigned int addr)
{
    i2c_dev_t *pI2c = (i2c_dev_t *) inst;

    act_writel(addr, pI2c->I2C_ADDR); //slave address
    act_writel(0x0000008e, pI2c->I2C_CTL); //start condition

    return trans_ov(inst); //1:ack  0:nack
}

/********************************************************************************
 I2C Stop:
 Input Parameter:
 inst -- a instance's pointer that i2c_open returned.
 Output Parameter:     No
 ********************************************************************************/
void i2c_stop(void *inst)
{
    unsigned int tmp;
    unsigned int times = MAX_ACK_TIME;
    i2c_dev_t *pI2c = (i2c_dev_t *) inst;

    act_writel(0x0000008b, pI2c->I2C_CTL); //stop condition
    do
    {
        tmp = act_readl(pI2c->I2C_STAT); //wait stop
        tmp = (tmp & 0x40);
        --times;
    } while ((tmp == 0) && (times > 0));

    act_writel(0x00000040, pI2c->I2C_STAT); //clear stop status
    return;
}

/********************************************************************************
 I2C Receive complete:
 Input Parameter:
 inst -- a instance's pointer that i2c_open returned.
 Output Parameter:
 0 ----- success.
 -1 ----- failed.
 ********************************************************************************/
int recv_ov(void *inst)
{
    unsigned int tmp;
    unsigned int times = MAX_ACK_TIME;
    i2c_dev_t *pI2c = (i2c_dev_t *) inst;

    do
    {
        tmp = act_readl(pI2c->I2C_STAT);
        tmp = tmp & (0x80);
        --times;
    } while ((tmp == 0) && (times > 0));

    if (!times)
    {
        printk("recv_ov time out\n ");
        return -1;
    }

    act_writel(0x00000080, pI2c->I2C_STAT);
    return 0;
}

/******************************************************************************/
/*!
 * \par  Description:
 *       I2C Initialization; \n
 *       Request some i2c related resource, used before i2c_read and i2c_write.
 * \param[in]   type -- refer to i2c_type_t. \n
 *                   -- 0 is standard i2c.   \n
 * \param[in]   port -- 0, use i2c_1;  1, use i2c_2.
 * \param[out]
 * \return      instance pointer of i2c_dev_t.
 * \retval      NULL -- failed
 * \par      exmaple code
 * \code
 *           list example code in here
 * \endcode
 *******************************************************************************/
void *i2c_open(int type, int port)
{
    i2c_dev_t * pI2c;

#ifdef  _I2C_NO_OS_FUNC_
    pI2c = &i2c_dev;
#else
    pI2c = malloc(sizeof(i2c_dev_t));
#endif  /* _I2C_NO_OS_FUNC_ */

    pI2c->i2c_type = type;

    switch (port)
    {
        case 0:
        pI2c->I2C_CTL = I2C1_CTL;
        pI2c->I2C_CLKDIV = I2C1_CLKDIV;
        pI2c->I2C_STAT = I2C1_STAT;
        pI2c->I2C_ADDR = I2C1_ADDR;
        pI2c->I2C_DAT = I2C1_DAT;
        break;
        case 1:
        pI2c->I2C_CTL = I2C2_CTL;
        pI2c->I2C_CLKDIV = I2C2_CLKDIV;
        pI2c->I2C_STAT = I2C2_STAT;
        pI2c->I2C_ADDR = I2C2_ADDR;
        pI2c->I2C_DAT = I2C2_DAT;
        break;
        default:
        return NULL;
    }
    pI2c->i2c_port = (unsigned int) port;

//    act_writel(0x00000000, pI2c->I2C_CTL); //reset I2C
//    udelay(1);
//    act_writel(0x00000080, pI2c->I2C_CTL); //enable I2C
//    udelay(1);
//
//    //TODO: 动态调分频比
//    act_writel(0x00000020, pI2c->I2C_CLKDIV); //here must set the PCLK to set the speed 100K or 400K

    return (void *) pI2c;
}

/******************************************************************************/
/*!
 * \par  Description:
 *           I2C Exit; \n
 *           release some i2c related resource.
 * \param[in]
 *           inst -- a instance's pointer that i2c_open returned.
 * \return
 * \retval   0 -- success.
 *******************************************************************************/
int i2c_close(void *inst)
{
    i2c_dev_t *pI2c = (i2c_dev_t *) inst;

    switch (pI2c->i2c_port)
    {
        case 0:
        case 1:
        act_writel(0x00000000, pI2c->I2C_CTL);
        break;
        default:
        return -1;
    }

#ifndef  _I2C_NO_OS_FUNC_
    free(pI2c);
#endif  /* _I2C_NO_OS_FUNC_ */
    return 0;
}

/******************************************************************************/
/*!
 * \par  Description:
 *           I2C Write;  \n
 *           release some i2c related resource.
 * \param[in]
 *           inst -- a instance's pointer that i2c_open returned.
 * \param[in]
 *           buf -- buffer pointer that store some data ready to write.
 * \param[in]
 *           cnt -- data count prepraring to write, counted by sizeof(int).
 * \return
 * \retval   0 -- success.
 *******************************************************************************/
int i2c_write(void *inst, unsigned int wt_addr, unsigned int *buf, int cnt)
{
    unsigned int tmp;
    int ret = 0;
    int multip;
    i2c_dev_t *pI2c = (i2c_dev_t *) inst;
    
    multip = multip_get_pinowner(PIN_SHARE_I2C, 0, 0, 0);
    if (multip != 1)
    {
        //printk("*************get I2C pin failed!!!************\n");
        return -1;
    }
    else
    {
        //printk("*************get I2C pin success!!!************\n");
    }
   
    i2c_get_gpio(I2C_ChAN_NUM);  //switch I2C pin
    //    /*! open I2C CLK */
    act_writel(((0x1 << 23) | act_readl(CMU_DEVCLKEN)), CMU_DEVCLKEN);
    udelay(1);
    act_writel(0x00000000, pI2c->I2C_CTL); //reset I2C
    udelay(1);
    act_writel(0x00000080, pI2c->I2C_CTL); //enable I2C
    udelay(1);

    //TODO: 动态调分频比
    act_writel(0x00000020, pI2c->I2C_CLKDIV); //here must set the PCLK to set the speed 100K or 400K
    udelay(1);

    ret = i2c_start(inst, wt_addr);
    if (ret != 0)
    {
        printk("******* W: start err*****\n");
        goto exit;
    }

    while (cnt > 0)
    {
        cnt--;

        act_writel(*buf, pI2c->I2C_DAT); //transfer data
        act_writel(0x00000082, pI2c->I2C_CTL); //realse bus
        ret = trans_ov(inst);
        if (ret != 0) //ack or nack
        {
            printk("**********W: no ack err ********\n");
            goto exit;
        }
        buf++;

    }

    exit: i2c_stop(inst);
        
    act_writel(0x00000000, pI2c->I2C_CTL);
    /*! I2C clk disable*/
    tmp = act_readl(CMU_DEVCLKEN);
    tmp &= ~(0x1 << 23);
    act_writel(tmp, CMU_DEVCLKEN);    
    i2c_release_gpio(I2C_ChAN_NUM);    
    multip_release_pinowner(PIN_SHARE_I2C, 0);
    
    return ret;
}

/******************************************************************************/
/*!
 * \par  Description:
 *           I2C Read;  \n
 *           release some i2c related resource.
 * \param[in]
 *           inst -- a instance's pointer that i2c_open returned.
 * \param[in]
 *           buf -- buffer pointer to store data.
 * \param[in]
 *           cnt -- data count prepraring to read, counted by sizeof(int).
 * \return
 * \retval   0 -- success.
 *******************************************************************************/
int i2c_read(void *inst, unsigned int rd_addr, unsigned int *buf, int cnt)
{
    unsigned int tmp;
    int ret = 0;
    int multip;
    i2c_dev_t *pI2c = (i2c_dev_t *) inst;

    multip = multip_get_pinowner(PIN_SHARE_I2C, 0, 0, 0);
    if (multip != 1)
    {
        //printk("*************get I2C pin failed!!!************\n");
        return -1;
    }
    else
    {
        //printk("*************get I2C pin success!!!************\n");
    }
    
    i2c_get_gpio(I2C_ChAN_NUM);  //switch I2C pin
    
    //    /*! open I2C CLK */
    act_writel(((0x1 << 23) | act_readl(CMU_DEVCLKEN)), CMU_DEVCLKEN);
    udelay(1);
    act_writel(0x00000000, pI2c->I2C_CTL); //reset I2C
    udelay(1);
    act_writel(0x00000080, pI2c->I2C_CTL); //enable I2C
    udelay(1);

    //TODO: 动态调分频比
    act_writel(0x00000020, pI2c->I2C_CLKDIV); //here must set the PCLK to set the speed 100K or 400K
    udelay(1);
    
    ret = i2c_start(inst, rd_addr);
    if (ret != 0)
    {
        printk("***********R: start err************\n");
        goto exit;
    }

    while (cnt > 0)
    {
        //udelay(1);
        if (cnt != 1)
        {
            act_writel(0x00000082, pI2c->I2C_CTL); //realse bus and ack
        }
        else
        {
            act_writel(0x00000083, pI2c->I2C_CTL); //realse bus and ack
        }
        cnt--;
        ret = recv_ov(inst);
        if (ret != 0)
        {
            printf("**********R: no ack err ***********\n");
            goto exit;
        }
        *buf = act_readl(pI2c->I2C_DAT);
        buf++;

    }

    exit: i2c_stop(inst);
        
    act_writel(0x00000000, pI2c->I2C_CTL);    
    /*! I2C clk disable*/
    tmp = act_readl(CMU_DEVCLKEN);
    tmp &= ~(0x1 << 23);
    act_writel(tmp, CMU_DEVCLKEN);
    i2c_release_gpio(I2C_ChAN_NUM);
    multip_release_pinowner(PIN_SHARE_I2C, 0);

    return ret;
}
