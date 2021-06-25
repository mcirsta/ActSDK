/********************************************************************************
 *                              USDK(1100)
 *                            Module: atj213x
 *                 Copyright(c) 2003-2008 Actions Semiconductor,
 *                            All Rights Reserved.
 *
 * History:
 *      <author>    <time>           <version >             <desc>
 *       SD     2008-02-01 15:00     1.0             build this file
 ********************************************************************************/
/*!
 * \file     atj213x.h
 * \brief    寄存器操作函数以及213x中断号定义
 * \author   SD
 * \version 1.0
 * \date  2008/2/1
 *******************************************************************************/
#ifndef __ATJ213X_H__
#define __ATJ213X_H__

#ifndef _ASSEMBLY_

void static inline act_writeb(unsigned char val, unsigned int reg)
{
    *(volatile unsigned char *) (reg) = val;
}

void static inline act_writew(unsigned short val, unsigned int reg)
{
    *(volatile unsigned short *) (reg) = val;
}
void static inline act_writel(unsigned int val, unsigned int reg)
{
    *(volatile unsigned int *) (reg) = val;
}

static inline unsigned char act_readb(unsigned long port)
{
    return (*(volatile unsigned char *) port);
}

static inline unsigned short act_readw(unsigned long port)
{
    return (*(volatile unsigned short *) port);
}

static inline unsigned int act_readl(unsigned long port)
{
    return (*(volatile unsigned int *) port);
}
#endif /* !defined (_LANGUAGE_ASSEMBLY) */

///* Interrupt Numbers */
//#define ACT213X_MCA_INT				 0
//#define ACT213X_ATA_INT              1
//#define ACT213X_SD_INT               2
//#define ACT213X_MHA_INT				 3
//#define ACT213X_USB_INT				 4
//#define ACT213X_DSP_INT				 5
//#define ACT213X_ITU656_INT			 6
//#define ACT213X_PC_INT				 7
//#define ACT213X_2HZ_INT				 8
//#define ACT213X_COUNT1_INT			 9
//#define ACT213X_COUNT0_INT			 10
//#define ACT213X_RTC_INT				 11
//#define ACT213X_DMA_INT				 12
//#define ACT213X_KEY_INT				 13
//#define ACT213X_EXTERNAL_INT		 14
//#define ACT213X_TP_INT			     15
//#define ACT213X_SPI_INT				 16
//#define ACT213X_I2C2_INT			 17
//#define ACT213X_I2C1_INT			 18
//#define ACT213X_UART2_INT			 19
//#define ACT213X_UART1_INT			 20
//#define ACT213X_ADC_INT				 21
//#define ACT213X_DAC_INT				 22
//#define ACT213X_SPDIF_INT			 23
//#define ACT213X_FLASH_INT			 24
//#define ACT213X_PCM_INT				 25
//#define ACT213X_YUV2RGB_INT			 26
//
//#define ACT213X_LAST_INTC0_INT       ACT213X_YUV2RGB_INT
//#define ACT213X_GPIO0_INT			 32
//#define ACT213X_GPIO1_INT			 33
//
//#define ACT213X_MAX_INTR             33

#endif /*__ATJ213X_H__*/
