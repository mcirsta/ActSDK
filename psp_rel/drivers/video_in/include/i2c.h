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
 * \file   i2c.h
 * \brief  
 *      i2c driver hardware layer headfile. 
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

#ifndef _I2C_H_
#define _I2C_H_ 

/************* config i2c driver below. ************/
#define  _I2C_NO_OS_FUNC_

#define MAX_ACK_TIME   0x7ffff
/*********** end of i2c driver config. *************/

/* register address. */
#if 0
/* --------IIC------------------------------------------------------ */
#define  I2C1_CTL                    (0xB0180000 + 0x00000000)
#define  I2C1_CLKDIV                 (0xB0180000 + 0x00000004)
#define  I2C1_STAT                   (0xB0180000 + 0x00000008)
#define  I2C1_ADDR                   (0xB0180000 + 0x0000000C)
#define  I2C1_DAT                    (0xB0180000 + 0x00000010)

#define  I2C2_CTL                    (0xB0180000 + 0x00000020)
#define  I2C2_CLKDIV                 (0xB0180000 + 0x00000024)
#define  I2C2_STAT                   (0xB0180000 + 0x00000028)
#define  I2C2_ADDR                   (0xB0180000 + 0x0000002C)
#define  I2C2_DAT                    (0xB0180000 + 0x00000030)
#endif

#ifndef NULL
#define NULL (void *)0
#endif

typedef enum 
{
    NORMAL_SPEED_I2C = 0,
    LOW_SPEED_I2C,
    HIGH_SPEED_I2C,
    SCCB_I2C,
} i2c_type_t;

/* register write func */
static inline void act_writel(unsigned int val, unsigned int reg)
{
    *(volatile unsigned int *)(reg) = val;
}

/* register read func */
static inline unsigned int act_readl(unsigned int port)
{
    return (*(volatile unsigned int *)port);
}

int i2c_start(void *inst, unsigned int addr);

void i2c_stop(void *inst); 

int trans_ov(void *inst);

int recv_ov(void *inst);


void *i2c_open(unsigned int type, unsigned int port, unsigned int rd_addr, unsigned int wt_addr);

int i2c_close(void *inst);

int i2c_write_data(void *inst, unsigned int *buf, int cnt);

int i2c_read_data(void *inst, unsigned int *buf, int cnt);

int i2c_write_reg(void *inst, unsigned int reg, unsigned int val);

int i2c_read_reg(void *inst, unsigned int reg, unsigned int *val);

int i2c_read_reg_data(void *inst, unsigned int reg, unsigned int *buf, int cnt);

unsigned int tvin_i2c_read_reg(void *inst, unsigned int reg_addr);

#endif /* _I2C_H_ */

