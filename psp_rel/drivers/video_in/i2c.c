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
 *      provide some functions such as i2c_read_data and i2c_write_data and so on.
 * \par EXTERNALIZED FUNCTIONS:
 *       null
 *
 *     Copyright(c) 2008-2012 Actions Semiconductor, All Rights Reserved.
 *
 * \version 1.0
 * \date  2008/06/18
 *******************************************************************************/

#include "reg.h"
#include "i2c.h"

extern int multip_get_pinowner(unsigned int pin_chan, int type, int (* wait_pin_func)(void *), void * dev_ptr);
extern int multip_release_pinowner(unsigned int pin_chan, int type);

extern int get_sensor_i2c(void);
extern int release_sensor_i2c(void);

/* i2c driver's info. */
typedef struct 
{
    i2c_type_t i2c_type; /* reserved, maybe used for sensor's sccb bus(a bus semiliar with i2c bus) and so on. */
    unsigned int i2c_port; /* i2c port selection. */
    //unsigned int master_addr;
    //unsigned int slave_addr;
    unsigned int rd_addr;   /* read i2c device addr. */
    unsigned int wt_addr;   /* write i2c device addr. */
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
static void udelay(unsigned int time)
{
    volatile unsigned int i, j;

    for (i = 0; i < time; i++) 
    {
        for (j = 0; j < 600; j++) 
        {
            do {} while (0);
        }
    }
    return;
}
#endif  /* _I2C_NO_OS_FUNC_ */

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
    i2c_dev_t *pI2c = (i2c_dev_t *)inst;
    
    do 
    {
        tmp = (unsigned int)act_readl(pI2c->I2C_STAT);
        tmp &= 0x80;
        times--;
    } 
    while ((tmp == 0) && (times > 0));

    if (!times) 
    {
        //printf("trans_ov time out\n ");
        return -1;   //failure
    }

    act_writel(0x00000080, pI2c->I2C_STAT);
    tmp = (unsigned int)act_readl(pI2c->I2C_CTL);
    tmp = tmp & 0x1;    //ack or nack

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
    int ret;
    
    i2c_dev_t *pI2c = (i2c_dev_t *)inst;
    
    ret = multip_get_pinowner(1,0,0,0);
    if (ret != 1)
    {
        return -1;
    }
    get_sensor_i2c();
    //    /*! open I2C CLK */
    act_writel(((0x1 << 23) | act_readl(CMU_DEVCLKEN)), CMU_DEVCLKEN);
    udelay(1);      
    
    act_writel(0x00000000, pI2c->I2C_CTL);   //reset I2C
    udelay(1);
    act_writel(0x00000080, pI2c->I2C_CTL);   //enable I2C
    udelay(1);  
    act_writel(0x00000020, pI2c->I2C_CLKDIV);    //here must set the PCLK to set the speed 100K or 400K

    act_writel(addr, pI2c->I2C_ADDR);   //slave address
    act_writel(0x00000086, pI2c->I2C_CTL);  //start condition

    
    

    return trans_ov(inst);  //1:ack  0:nack
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
    i2c_dev_t *pI2c = (i2c_dev_t *)inst;
    
    act_writel(0x0000008b, pI2c->I2C_CTL);   //stop condition
    do 
    {
        tmp = (unsigned int)act_readl(pI2c->I2C_STAT);    //wait stop
        tmp = (tmp & 0x40);
        times--;
    } 
    while ((tmp == 0) && (times > 0));

    act_writel(0x00000040, pI2c->I2C_STAT);  //clear stop status

    release_sensor_i2c();
    multip_release_pinowner(1,0);
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
    i2c_dev_t *pI2c = (i2c_dev_t *)inst;

    do 
    {
        tmp = (unsigned int)act_readl(pI2c->I2C_STAT);
        tmp = tmp & (0x80);
        times--;
    } 
    while ( (tmp == 0) && (times > 0));

    if (!times) 
    {
        //printf("recv_ov time out\n ");
        return -1;
    }

    act_writel(0x00000080, pI2c->I2C_STAT);
    return 0;
}

/******************************************************************************/
/*!                    
* \par  Description:
*       I2C Initialization; \n
*       Request some i2c related resource, used before i2c_read_data and i2c_write_data.
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
void *i2c_open(unsigned int type, unsigned int port, unsigned int rd_addr, unsigned int wt_addr) 
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
    pI2c->i2c_port = port;
    pI2c->rd_addr = rd_addr;
    pI2c->wt_addr = wt_addr;

    act_writel(0x00000000, pI2c->I2C_CTL);   //reset I2C
    udelay(1);
    act_writel(0x00000080, pI2c->I2C_CTL);   //enable I2C
    udelay(1);
    act_writel(pI2c->wt_addr, pI2c->I2C_ADDR);  //slave address
    udelay(1);

    //act_writel(0x0000001e, pI2c->I2C_CLKDIV);  //SCL=PCLK/(CLKDIV*16)  PCLK:48    SCL:400kbps
    act_writel(0x00000020, pI2c->I2C_CLKDIV);    //here must set the PCLK to set the speed 100K or 400K
    //act_writel(0x00000011, pI2c->I2C_CLKDIV);   /* wangqian's set in tvin. */

    return (void *)pI2c;
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
    i2c_dev_t *pI2c = (i2c_dev_t *)inst;

    switch (pI2c->i2c_port)
    {
    case 0:
    case 1:
        act_writel(0x00000000, pI2c->I2C_CTL);
        break;
    default:
        return -1;
    }

    return 0;
} 

/******************************************************************************/
/*!                    
* \par  Description:
*           I2C Write;  \n
* \param[in] 
*           inst -- a instance's pointer that i2c_open returned.
* \param[in] 
*           buf -- buffer pointer that store some data ready to write.
* \param[in] 
*           cnt -- data count prepraring to write, counted by sizeof(int).
* \return
* \retval   0 -- success.
*******************************************************************************/
int i2c_write_data(void *inst, unsigned int *buf, int cnt)
{
    int ret = 0;
    i2c_dev_t *pI2c = (i2c_dev_t *)inst;

    ret = i2c_start(inst, pI2c->wt_addr);
    if (ret != 0)
    {
        goto exit;
    }

    while (cnt > 0) 
    {
        act_writel(*buf, pI2c->I2C_DAT);    //transfer data
        act_writel(0x00000082, pI2c->I2C_CTL);   //realse bus

        ret = trans_ov(inst);
        if (ret != 0)     //ack or nack
        {
            goto exit;
        }
        buf++;
        cnt--;
    }

    exit:
    i2c_stop(inst);
    return ret;
}

/******************************************************************************/
/*!                    
* \par  Description:
*           I2C Read;  \n
* \param[in] 
*           inst -- a instance's pointer that i2c_open returned.
* \param[in] 
*           buf -- buffer pointer to store data.
* \param[in] 
*           cnt -- data count prepraring to read, counted by sizeof(int).
* \return
* \retval   0 -- success.
*******************************************************************************/
int i2c_read_data(void *inst, unsigned int *buf, int cnt)
{
    int ret = 0;
    i2c_dev_t *pI2c = (i2c_dev_t *)inst;

    ret = i2c_start(inst, pI2c->rd_addr);
    if (ret != 0)
    {
        goto exit;
    }

    while (cnt > 0) 
    {
        //udelay(1);
        if (cnt != 1)
        {
            act_writel(0x00000082, pI2c->I2C_CTL);   //realse bus and ack
        }
        else
        {
            act_writel(0x00000083, pI2c->I2C_CTL);   //realse bus and ack
        }

        ret = recv_ov(inst);
        if (ret != 0)
        {
            goto exit;
        }
        *buf = (unsigned int)act_readl(pI2c->I2C_DAT);
        buf++;
        cnt--;
    }

    exit:
    i2c_stop(inst);
    return ret;
}

/******************************************************************************/
/*!                    
* \par  Description:
*           Read i2c slave device's register;  \n
* \param[in] 
*           inst -- a instance's pointer that i2c_open returned.
* \param[in] 
*           reg -- first register address in i2c slave device.
* \param[in] 
*           buf -- buffer pointer to store data.
* \param[in] 
*           cnt -- data count prepraring to read, counted by sizeof(int).
* \return
* \retval   0 -- success.
*******************************************************************************/
int i2c_read_reg_data(void *inst, unsigned int reg, unsigned int *buf, int cnt)
{
    int ret = 0;
    i2c_dev_t *pI2c = (i2c_dev_t *)inst;

    ret = i2c_start(inst, pI2c->wt_addr);
    if (ret != 0)
    {
        goto exit;
    }

    act_writel(reg, pI2c->I2C_DAT);    //transfer data
    act_writel(0x00000082, pI2c->I2C_CTL);   //realse bus

    ret = trans_ov(inst);
    if (ret != 0)     //ack or nack
    {
        goto exit;
    }

    act_writel(pI2c->rd_addr, pI2c->I2C_ADDR);   //slave address
    act_writel(0x0000008e, pI2c->I2C_CTL);  //start condition
    ret = trans_ov(inst);
    if (ret != 0)     //ack or nack
    {
        goto exit;
    }

    while (cnt > 0) 
    {
        if (cnt != 1)
        {
            act_writel(0x00000082, pI2c->I2C_CTL);   //realse bus and ack
        }
        else
        {
            act_writel(0x00000083, pI2c->I2C_CTL);   //realse bus and ack
        }

        ret = recv_ov(inst);
        if (ret != 0)
        {
            goto exit;
        }
        *buf = (unsigned int)act_readl(pI2c->I2C_DAT);
        buf++;
        cnt--;
    }

    exit:
    i2c_stop(inst);
    return ret;
}

int i2c_write_reg(void *inst, unsigned int reg, unsigned int val)
{
    int ret = 0;
    i2c_dev_t *pI2c = (i2c_dev_t *)inst;

    ret = i2c_start(inst, pI2c->wt_addr);
    if (ret != 0)
    {
        goto exit;
    }

    act_writel(reg, pI2c->I2C_DAT);    //transfer data
    act_writel(0x00000082, pI2c->I2C_CTL);   //realse bus
    ret = trans_ov(inst);
    if (ret != 0)     //ack or nack
    {
        goto exit;
    }
    act_writel(val, pI2c->I2C_DAT);    //transfer data
    act_writel(0x00000082, pI2c->I2C_CTL);   //realse bus
    ret = trans_ov(inst);
    if (ret != 0)     //ack or nack
    {
        goto exit;
    }

    exit:
    i2c_stop(inst);
    return ret;
}

int i2c_read_reg(void *inst, unsigned int reg, unsigned int *val)
{
    int ret = 0;
    i2c_dev_t *pI2c = (i2c_dev_t *)inst;

    ret = i2c_start(inst, pI2c->rd_addr);
    if (ret != 0)
    {
        goto exit;
    }

    act_writel(0x00000082, pI2c->I2C_CTL);   //realse bus and ack
    //act_writel(0x00000083, pI2c->I2C_CTL);   //realse bus and ack, the last data.

    ret = recv_ov(inst);
    if (ret != 0)
    {
        goto exit;
    }
    *val = (unsigned int)act_readl(pI2c->I2C_DAT);

    exit:
    i2c_stop(inst);
    multip_release_pinowner(1,0);
    return ret;
}
/******************************************************************************/
/*!                    
* \par  Description:
*       read sensor's control register.
* \param[in]   reg_addr -- i2c slave device module's register addr.
* \retval      0 sucess, else failed.
*******************************************************************************/
unsigned int tvin_i2c_read_reg(void *inst, unsigned int reg_addr)
{
    unsigned int val = 0;
    int ret = 0;
    i2c_dev_t *pI2c = (i2c_dev_t *)inst;

    act_writel(0x00000000, pI2c->I2C_CTL);   //reset I2C
    udelay(1);
    act_writel(0x00000080, pI2c->I2C_CTL);   //enable I2C
    act_writel(pI2c->wt_addr, pI2c->I2C_ADDR);  //slave address

    //act_writel(0x00000020, pI2c->I2C_CLKDIV);    //here must set the PCLK to set the speed 100K or 400K
    act_writel(0x00000080, pI2c->I2C_CTL);   //enable I2C
    udelay(3);

    i2c_start(inst, pI2c->wt_addr);
    act_writel(reg_addr, pI2c->I2C_DAT);    //transfer data
    act_writel(0x00000083, pI2c->I2C_CTL);  //realse bus
    ret = trans_ov(inst);
    if (ret != 0)        //ack or nack
    {
        goto exit;
    }
    //i2c_stop(inst);

    act_writel(pI2c->wt_addr + 1, pI2c->I2C_ADDR); //write device address of AT24C02

    act_writel(0x8e, pI2c->I2C_CTL);   //generate start condition
    trans_ov(inst);
    act_writel(0x83, pI2c->I2C_CTL);   //release I2C
    recv_ov(inst);
    udelay(10);

    val = act_readl(pI2c->I2C_DAT);
    
    exit:
    i2c_stop(inst);
    if (val != act_readl(pI2c->I2C_DAT))
    {
        printf("old i2c val: %x \n", val);
        val = act_readl(pI2c->I2C_DAT);
        printf("new i2c val: %x \n", val);
    }
    return (unsigned int)val;
}

