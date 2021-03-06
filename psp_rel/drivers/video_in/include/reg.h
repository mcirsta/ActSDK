/*
********************************************************************************
                        3996 -- ic register headfile
*                (c) Copyright 2007 - 2010, Actions Co,Ld. 
*                        All Right Reserved 
*
* History:        
*      <author>      <time>       <version >    <desc>
*       Author  : jason  jiangys@actions-semi.com
*       faler  lchf@actions-semi.com
*       Revision History:                                                         
*       2005/12/28 Version 1.0  Create the file 
*       2005/12/30 Version 1.1  change ATA I/F register define
*       2006/01/05 Version 1.2  change ATA_CLK_CON define(0x00000004-->0x00000014) 
*       2006/01/18 Version 1.3  
*       Add the module's name to the signs of CMU,LOSC,USB 
*       I2CD_IV--->I2C2_DIV
*       SPDIF_STA->SPDIF_STAT  
*       PCM_CTRL-->PCM_CTL 
*       OTG_CTRL-->OTG_CTL
*       I2C1_STA-->I2C1_STAT
*       I2C2_STA-->I2C2_STAT
*       SPI_STA--->SPI_STAT  
*
*       2006/01/18 Version 1.4 
*       modefied by GL3963_Spec_0_30_20060331.doc
*        
*       2006/04/10 Version 1.5 
*       SDR_RFSH  改为SDR_AUTORFC
*       NAND_ADDR01  改为 NAND_ADDRLO1234
*       NAND_ADDR23  改为 NAND_ADDRLO56
*       NAND_ADDR45  改为 NAND_ADDRHI1234
*       NAND_ADDR67  改为 NAND_ADDRHI56
*       MHA_DCSCL0  改为  MHA_DCSCL01
*       MHA_DCSCL1  改为  MHA_DCSCL23
*       MHA_DCSCL2  改为  MHA_DCSCL45
*       MHA_DCSCL3  改为  MHA_DCSCL67
*       GPIO_ADATA  改为  GPIO_ADAT 
*       GPIO_BDATA  改为  GPIO_BDAT

*       modified by 3963map20060410.xls
*       LOSC  改为 RTC
*       IC 改为 INTC    

*       2008.04.28:
*       current version is 3996.
*
*       houjingkun  2008/06/18  1.1  add commentary and modify for code checking.
********************************************************************************/ 
/*!
 * \file   reg.h
 * \brief  
 *      3996 ic register's headfile. 
 * \author houjingkun
 * \par GENERAL DESCRIPTION:
 *      provide ic register's access address define.
 * \par EXTERNALIZED FUNCTIONS:
 *       null
 *
 *     Copyright(c) 2008-2012 Actions Semiconductor, All Rights Reserved.
 *
 * \version 1.0
 * \date  2008/06/18
 *******************************************************************************/

#ifndef _REG_H_
#define _REG_H_ 

//--------PMU/LRADC--------------------------------------------------
#define  PMU_CTL                     (0xB0000000 + 0x00000000)
#define  PMU_LRADC                   (0xB0000000 + 0x00000004)
#define  PMU_CHG                     (0xB0000000 + 0x00000008)

//--------CMU/HOSC--------------------------------------------------
#define  CMU_COREPLL                 (0xB0010000 + 0x00000000)
#define  CMU_DSPPLL                  (0xB0010000 + 0x00000004)
#define  CMU_AUDIOPLL                (0xB0010000 + 0x00000008)
#define  CMU_BUSCLK                  (0xB0010000 + 0x0000000C)
                                     
#define  CMU_SDRCLK                  (0xB0010000 + 0x00000010)
#define  CMU_ATACLK                  (0xB0010000 + 0x00000014)
#define  CMU_NANDCLK                 (0xB0010000 + 0x00000018)
#define  CMU_SDCLK                   (0xB0010000 + 0x0000001C)
#define  CMU_MHACLK                  (0xB0010000 + 0x00000020)
#define  CMU_BTCLK                   (0xB0010000 + 0x00000024)
#define  CMU_UART1CLK                (0xB0010000 + 0x00000028)
#define  CMU_UART2CLK                (0xB0010000 + 0x0000002C)
#define  CMU_DMACLK                  (0xB0010000 + 0x00000030)
#define  CMU_FMCLK                   (0xB0010000 + 0x00000034)
#define  CMU_MCACLK                  (0xB0010000 + 0x00000038)

#define  CMU_DEVCLKEN                (0xB0010000 + 0x00000080)
#define  CMU_DEVRST                  (0xB0010000 + 0x00000084)

//--------RTC/LOSC/WD--------------------------------------------
#define  RTC_CTL                    (0xB0018000 + 0x00000000)
#define  RTC_DHMS                   (0xB0018000 + 0x00000004)
#define  RTC_YMD                    (0xB0018000 + 0x00000008)
#define  RTC_DHMSALM                (0xB0018000 + 0x0000000C)
#define  RTC_YMDALM                 (0xB0018000 + 0x00000010)
#define  RTC_WDCTL                  (0xB0018000 + 0x00000014)
#define  RTC_T0CTL                  (0xB0018000 + 0x00000018)
#define  RTC_T0                     (0xB0018000 + 0x0000001C)
#define  RTC_T1CTL                  (0xB0018000 + 0x00000020)
#define  RTC_T1                     (0xB0018000 + 0x00000024)

//--------INTERRUPT CONTROLLER---------------------------------------
#define  INTC_PD                       (0xB0020000 + 0x00000000)
#define  INTC_MSK                      (0xB0020000 + 0x00000004)
#define  INTC_CFG0                     (0xB0020000 + 0x00000008)
#define  INTC_CFG1                     (0xB0020000 + 0x0000000C)
#define  INTC_CFG2                     (0xB0020000 + 0x00000010)
#define  INTC_EXTCTL                   (0xB0020000 + 0x00000014)

//--------SRAM ON CHIP-----------------------------------------------
#define  SRAMOC_CTL                  (0xB0030000 + 0x00000000)
#define  SRAMOC_STAT                 (0xB0030000 + 0x00000004)

//--------NOR FLASH/SRAM/BROM/ID-------------------------------------
#define  NOR_CTL                     (0xB0038000 + 0x00000000)
#define  NOR_BROMCTL                 (0xB0038000 + 0x00000004)
#define  NOR_CHIPID                  (0xB0038000 + 0x00000008)

//--------32BIT RISC CORE--------------------------------------------
#define  PCNT_CTL                    (0xB0040000 + 0x00000000)
#define  PCNT_PC0                    (0xB0040000 + 0x00000004)
#define  PCNT_PC1                    (0xB0040000 + 0x00000008)

//--------DSP CONTROL------------------------------------------------
#define  DSP_HDR0                    (0xB0050000 + 0x00000000)
#define  DSP_HDR1                    (0xB0050000 + 0x00000004)
#define  DSP_HDR2                    (0xB0050000 + 0x00000008)
#define  DSP_HDR3                    (0xB0050000 + 0x0000000C)
#define  DSP_HDR4                    (0xB0050000 + 0x00000010)
#define  DSP_HDR5                    (0xB0050000 + 0x00000014)
#define  DSP_HSR6                    (0xB0050000 + 0x00000018)
#define  DSP_HSR7                    (0xB0050000 + 0x0000001C)
#define  DSP_CTL                     (0xB0050000 + 0x00000020)

//--------DMA CONTROLLER---------------------------------------------
//DMA Configuration Register
#define  DMA_CTL                     (0xB0060000 + 0x00000000)
#define  DMA_IRQEN                   (0xB0060000 + 0x00000004)
#define  DMA_IRQPD                   (0xB0060000 + 0x00000008)
//DMA0 Control Register                      
#define  DMA_MODE0                   (0xB0060100 + 0x00000000)
#define  DMA_SRC0                    (0xB0060100 + 0x00000004)
#define  DMA_DST0                    (0xB0060100 + 0x00000008)
#define  DMA_CNT0                    (0xB0060100 + 0x0000000C)
#define  DMA_REM0                    (0xB0060100 + 0x00000010)
#define  DMA_CMD0                    (0xB0060100 + 0x00000014)
//DMA1 Control Register                      
#define  DMA_MODE1                   (0xB0060120 + 0x00000000)
#define  DMA_SRC1                    (0xB0060120 + 0x00000004)
#define  DMA_DST1                    (0xB0060120 + 0x00000008)
#define  DMA_CNT1                    (0xB0060120 + 0x0000000C)
#define  DMA_REM1                    (0xB0060120 + 0x00000010)
#define  DMA_CMD1                    (0xB0060120 + 0x00000014)
//DMA2 Control Register                          
#define  DMA_MODE2                   (0xB0060140 + 0x00000000)
#define  DMA_SRC2                    (0xB0060140 + 0x00000004)
#define  DMA_DST2                    (0xB0060140 + 0x00000008)
#define  DMA_CNT2                    (0xB0060140 + 0x0000000C)
#define  DMA_REM2                    (0xB0060140 + 0x00000010)
#define  DMA_CMD2                    (0xB0060140 + 0x00000014)
//DMA3 Control Register                          
#define  DMA_MODE3                   (0xB0060160 + 0x00000000)
#define  DMA_SRC3                    (0xB0060160 + 0x00000004)
#define  DMA_DST3                    (0xB0060160 + 0x00000008)
#define  DMA_CNT3                    (0xB0060160 + 0x0000000C)
#define  DMA_REM3                    (0xB0060160 + 0x00000010)
#define  DMA_CMD3                    (0xB0060160 + 0x00000014)
//DMA4 Control Register                          
#define  DMA_MODE4                   (0xB0060180 + 0x00000000)
#define  DMA_SRC4                    (0xB0060180 + 0x00000004)
#define  DMA_DST4                    (0xB0060180 + 0x00000008)
#define  DMA_CNT4                    (0xB0060180 + 0x0000000C)
#define  DMA_REM4                    (0xB0060180 + 0x00000010)
#define  DMA_CMD4                    (0xB0060180 + 0x00000014)
#define  DMA_VIDEO4                  (0xB0060180 + 0x00000018)
//DMA5 Control Register                          
#define  DMA_MODE5                   (0xB00601A0 + 0x00000000)
#define  DMA_SRC5                    (0xB00601A0 + 0x00000004)
#define  DMA_DST5                    (0xB00601A0 + 0x00000008)
#define  DMA_REM5                    (0xB00601A0 + 0x00000010)
#define  DMA_CNT5                    (0xB00601A0 + 0x0000000C)
#define  DMA_CMD5                    (0xB00601A0 + 0x00000014)
//DMA6 Control Register                          
#define  DMA_MODE6                   (0xB00601C0 + 0x00000000)
#define  DMA_SRC6                    (0xB00601C0 + 0x00000004)
#define  DMA_DST6                    (0xB00601C0 + 0x00000008)
#define  DMA_CNT6                    (0xB00601C0 + 0x0000000C)
#define  DMA_REM6                    (0xB00601C0 + 0x00000010)
#define  DMA_CMD6                    (0xB00601C0 + 0x00000014)
//DMA7 Control Register                          
#define  DMA_MODE7                   (0xB00601E0 + 0x00000000)
#define  DMA_SRC7                    (0xB00601E0 + 0x00000004)
#define  DMA_DST7                    (0xB00601E0 + 0x00000008)
#define  DMA_CNT7                    (0xB00601E0 + 0x0000000C)
#define  DMA_REM7                    (0xB00601E0 + 0x00000010)
#define  DMA_CMD7                    (0xB00601E0 + 0x00000014)

//--------SDRAM/DDR2 CONTROLLER-    -----------------------------------
#define  SDR_CTL                     (0xB0070000 + 0x00000000)
#define  SDR_ADDRCFG                 (0xB0070000 + 0x00000004)
#define  SDR_EN                      (0xB0070000 + 0x00000008)
#define  SDR_CMD                     (0xB0070000 + 0x0000000C)
#define  SDR_STAT                    (0xB0070000 + 0x00000010)
#define  SDR_RFSH                    (0xB0070000 + 0x00000014)
#define  SDR_MODE                    (0xB0070000 + 0x00000018)
#define  SDR_MOBILE                  (0xB0070000 + 0x0000001C)

//--------MCA(WMA)---------------------------------------------------
#define  MCA_CTL                     (0xB0080000 + 0x00000000)

//--------ATA I/F----------------------------------------------------
#define  ATA_CONFIG                  (0xB0090000 + 0x00000000)
#define  ATA_UDMACTL                 (0xB0090000 + 0x00000004)
#define  ATA_DATA                    (0xB0090000 + 0x00000008)
#define  ATA_FEATURE                 (0xB0090000 + 0x0000000C)
#define  ATA_SECCNT                  (0xB0090000 + 0x00000010)                                      
#define  ATA_SECNUM                  (0xB0090000 + 0x00000014)
#define  ATA_CLDLOW                  (0xB0090000 + 0x00000018)
#define  ATA_CLDHI                   (0xB0090000 + 0x0000001C)
#define  ATA_HEAD                    (0xB0090000 + 0x00000020)
#define  ATA_CMD                     (0xB0090000 + 0x00000024)
#define  ATA_BYTECNT                 (0xB0090000 + 0x00000028)
#define  ATA_FIFOCTL                 (0xB0090000 + 0x0000002C)                                     
#define  ATA_FIFOCFG                 (0xB0090000 + 0x00000030)
#define  ATA_ADDRDEC                 (0xB0090000 + 0x00000034)
#define  ATA_IRQCTL                  (0xB0090000 + 0x00000038)

//--------FLASH I/F--------------------------------------------------
#define  NAND_CTL                    (0xB00A0000 + 0x00000000)
#define  NAND_STATUS                 (0xB00A0000 + 0x00000004)
#define  NAND_FIFOTIM                (0xB00A0000 + 0x00000008)
#define  NAND_CLKCTL                 (0xB00A0000 + 0x0000000C)
#define  NAND_BYTECNT                (0xB00A0000 + 0x00000010)
#define  NAND_ADDRLO1234             (0xB00A0000 + 0x00000014)
#define  NAND_ADDRLO56               (0xB00A0000 + 0x00000018)
#define  NAND_ADDRHI1234             (0xB00A0000 + 0x0000001C)
#define  NAND_ADDRHI56               (0xB00A0000 + 0x00000020)
#define  NAND_BUF0                   (0xB00A0000 + 0x00000024)
#define  NAND_BUF1                   (0xB00A0000 + 0x00000028)
#define  NAND_CMD                    (0xB00A0000 + 0x0000002C)
#define  NAND_ECCCTL                 (0xB00A0000 + 0x00000030)
#define  NAND_HAMECC0                (0xB00A0000 + 0x00000034)
#define  NAND_HAMECC1                (0xB00A0000 + 0x00000038)
#define  NAND_HAMECC2                (0xB00A0000 + 0x0000003C)
#define  NAND_HAMCEC                 (0xB00A0000 + 0x00000040)
#define  NAND_RSE0                   (0xB00A0000 + 0x00000044)
#define  NAND_RSE1                   (0xB00A0000 + 0x00000048)
#define  NAND_RSE2                   (0xB00A0000 + 0x0000004C)
#define  NAND_RSE3                   (0xB00A0000 + 0x00000050)
#define  NAND_RSPS0                  (0xB00A0000 + 0x00000054)
#define  NAND_RSPS1                  (0xB00A0000 + 0x00000058)
#define  NAND_RSPS2                  (0xB00A0000 + 0x0000005C)
#define  NAND_FIFODATA               (0xB00A0000 + 0x00000060)
#define  NAND_DEBUG                  (0xB00A0000 + 0x00000070)

//--------SD I/F-----------------------------------------------------
#define  SD_CTL                      (0xB00B0000 + 0x00000000)
#define  SD_CMDRSP                   (0xB00B0000 + 0x00000004)
#define  SD_RW                       (0xB00B0000 + 0x00000008)
#define  SD_FIFOCTL                  (0xB00B0000 + 0x0000000C)
#define  SD_CMD                      (0xB00B0000 + 0x00000010)
#define  SD_ARG                      (0xB00B0000 + 0x00000014)
#define  SD_CRC7                     (0xB00B0000 + 0x00000018)

#define  SD_RSPBUF0                  (0xB00B0000 + 0x0000001C)
#define  SD_RSPBUF1                  (0xB00B0000 + 0x00000020)
#define  SD_RSPBUF2                  (0xB00B0000 + 0x00000024)
#define  SD_RSPBUF3                  (0xB00B0000 + 0x00000028)
#define  SD_RSPBUF4                  (0xB00B0000 + 0x0000002C)

#define  SD_DAT                      (0xB00B0000 + 0x00000030)
#define  SD_CLK                      (0xB00B0000 + 0x00000034)
#define  SD_BYTECNT                  (0xB00B0000 + 0x00000038)

//--------MHA(MJPEG)-------------------------------------------------
#define  MHA_CTL                     (0xB00C0000 + 0x00000000)
#define  MHA_CFG                     (0xB00C0000 + 0x00000004)
#define  MHA_DCSCL01                 (0xB00C0000 + 0x00000010)
#define  MHA_DCSCL23                 (0xB00C0000 + 0x00000014)
#define  MHA_DCSCL45                 (0xB00C0000 + 0x00000018)
#define  MHA_DCSCL67                 (0xB00C0000 + 0x0000001C)
#define  MHA_QSCL                    (0xB00C0000 + 0x00000020)

//--------ITU656 PORT------------------------------------------------
//For Output Part
#define  BTO_MODESEL                   (0xB00D0000 + 0x00000000)
#define  BT_FIFO_ODAT                  (0xB00D0000 + 0x00000004)          
#define  BT_VEICTL                     (0xB00D0000 + 0x00000008)
#define  BT_VEIALSEOF                  (0xB00D0000 + 0x0000000C)
#define  BT_VEIALSEEF                  (0xB00D0000 + 0x00000010)
#define  BT_VEIADLSE                   (0xB00D0000 + 0x00000014)
#define  BT_VEIFTP                     (0xB00D0000 + 0x00000018)
#define  BT_VEIFIFOCTL                 (0xB00D0000 + 0x0000001C)
                         
//For Input Part                     
#define  BTI_MODESEL                   (0xB00D0000 + 0x00000020)
#define  BT_FIFO_IDAT                  (0xB00D0000 + 0x00000024)
#define  BT_VDICTL                     (0xB00D0000 + 0x00000028)
#define  BT_CSICTL                     (0xB00D0000 + 0x00000028)
#define  BT_TSICTL                     (0xB00D0000 + 0x00000028)
#define  BT_VDIHSPOS                   (0xB00D0000 + 0x0000002C)
#define  BT_CSIHSPOS                   (0xB00D0000 + 0x0000002C)
#define  BT_VDIVSEPOF                  (0xB00D0000 + 0x00000030)
#define  BT_CSIVSEPOF                  (0xB00D0000 + 0x00000030)
#define  BT_VDIVSEPEF                  (0xB00D0000 + 0x00000034)
#define  BT_VDIIRQSTAT                 (0xB00D0000 + 0x00000038)
#define  BT_CSIRQSTAT                  (0xB00D0000 + 0x00000038)
#define  BT_VDIFIFOCTL                 (0xB00D0000 + 0x0000003C)
#define  BT_CSIFIFOCTL                 (0xB00D0000 + 0x0000003C)
#define  BT_TSIFIFOCTL                 (0xB00D0000 + 0x0000003C)

//Integrated Video Encoder           
#define  BT_IVECTL                     (0xB00D0000 + 0x00000050)
#define  BT_IVEOUTCTL                  (0xB00D0000 + 0x00000054)
#define  BT_IVECOTCTL                  (0xB00D0000 + 0x00000058)
#define  BT_IVEBRGCTL                  (0xB00D0000 + 0x0000005C)
#define  BT_IVECSATCTL                 (0xB00D0000 + 0x00000060)
#define  BT_IVECBURCTL                 (0xB00D0000 + 0x00000064)
#define  BT_IVESYNCAMCTL               (0xB00D0000 + 0x00000068)

//--------USB OTG----------------------------------------------------- 
#define  OTG_IRQ                     (0xB00E0000 + 0x000001BC)
#define  OTG_FSMSTAT                 (0xB00E0000 + 0x000001BD)
#define  OTG_CTRL                    (0xB00E0000 + 0x000001BE)
#define  OTG_STAT                    (0xB00E0000 + 0x000001BF)
#define  OTG_IEN                     (0xB00E0000 + 0x000001C0)
                                     
#define  OTG_TAAIDLBDIS              (0xB00E0000 + 0x000001C1)
#define  OTG_TAWAITBCON              (0xB00E0000 + 0x000001C2)
#define  OTG_TBVBUSPLS               (0xB00E0000 + 0x000001C3)
#define  OTG_TBVBUSDISPLS            (0xB00E0000 + 0x000001C7)

//--------YVU2RGB----------------------------------------------------
#define  YUV2RGB_CTL                 (0xB00F0000 + 0x00000000)
#define  YUV2RGB_FIFODATA            (0xB00F0000 + 0x00000004)
#define  YUV2RGB_CLKCTL              (0xB00F0000 + 0x00000008)
#define  YUV2RGB_FRAMECOUNT          (0xB00F0000 + 0x0000000C)


//--------DAC+PA/IIS-------------------------------------------------
#define  DAC_CTL                     (0xB0100000 + 0x00000000)
#define  DAC_FIFOCTL                 (0xB0100000 + 0x00000004)
#define  DAC_DAT                     (0xB0100000 + 0x00000008)
#define  DAC_DEBUG                   (0xB0100000 + 0x0000000C)
#define  DAC_ANALOG                  (0xB0100000 + 0x00000010)

//--------ADC--------------------------------------------------------
#define  ADC_CTL                     (0xB0110000 + 0x00000000)
#define  ADC_FIFOCTL                 (0xB0110000 + 0x00000004)
#define  ADC_DAT                     (0xB0110000 + 0x00000008)
#define  ADC_ANALOG                  (0xB0110000 + 0x0000000C)
#define  ADC_DEBUG                   (0xB0110000 + 0x00000010)

//--------TP---------------------------------------------------------
#define  TP_CTL                      (0xB0120000 + 0x00000000)
#define  TP_DAT                      (0xB0120000 + 0x00000004)

//--------SPIDIF-----------------------------------------------------
#define  SPDIF_CTL                   (0xB0140000 + 0x00000000)
#define  SPDIF_STAT                  (0xB0140000 + 0x00000004)
#define  SPDIF_TXDAT                 (0xB0140000 + 0x00000008)
#define  SPDIF_RXDAT                 (0xB0140000 + 0x0000000C)
#define  SPDIF_TXCSTAT               (0xB0140000 + 0x00000010)
#define  SPDIF_RXCSTAT               (0xB0140000 + 0x00000014)

//--------PCM--------------------------------------------------------
#define  PCM_CTL                     (0xB0150000 + 0x00000000)
#define  PCM_STAT                    (0xB0150000 + 0x00000004)
#define  PCM_RXDAT                   (0xB0150000 + 0x00000008)
#define  PCM_TXDAT                   (0xB0150000 + 0x0000000C)

//--------UART-2-----------------------------------------------------
#define  UART1_CTL                   (0xB0160000 + 0x00000000)
#define  UART1_RXDAT                 (0xB0160000 + 0x00000004)
#define  UART1_TXDAT                 (0xB0160000 + 0x00000008)
#define  UART1_STAT                  (0xB0160000 + 0x0000000C)

#define  UART2_CTL                   (0xB0160000 + 0x00000020)
#define  UART2_RXDAT                 (0xB0160000 + 0x00000024)
#define  UART2_TXDAT                 (0xB0160000 + 0x00000028)
#define  UART2_STAT                  (0xB0160000 + 0x0000002C)

//--------IR--------------------------------------------------------
#define  IR_PL                       (0xB0160000 + 0x00000010)
#define  IR_RBC                      (0xB0160000 + 0x00000014)

//--------IIC--------------------------------------------------------
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


//--------SPI--------------------------------------------------------
#define  SPI_CTL                     (0xB0190000 + 0x00000000)
#define  SPI_CLKDIV                  (0xB0190000 + 0x00000004)
#define  SPI_STAT                    (0xB0190000 + 0x00000008)
#define  SPI_RXDAT                   (0xB0190000 + 0x0000000C)
#define  SPI_TXDAT                   (0xB0190000 + 0x00000010)

//--------KEY SCAN------------------------------------------
#define  KEY_CTL                     (0xB01A0000 + 0x00000000)
#define  KEY_DAT0                    (0xB01A0000 + 0x00000004)
#define  KEY_DAT1                    (0xB01A0000 + 0x00000008)
#define  KEY_DAT2                    (0xB01A0000 + 0x0000000C)
#define  KEY_DAT3                    (0xB01A0000 + 0x00000010)

//--------GPIO----------------------------------------------
#define  GPIO_AOUTEN                 (0xB01C0000 + 0x00000000)
#define  GPIO_AINEN                  (0xB01C0000 + 0x00000004)
#define  GPIO_ADAT                   (0xB01C0000 + 0x00000008)
#define  GPIO_BOUTEN                 (0xB01C0000 + 0x0000000C)
#define  GPIO_BINEN                  (0xB01C0000 + 0x00000010)
#define  GPIO_BDAT                   (0xB01C0000 + 0x00000014)
#define  GPIO_MFCTL0                 (0xB01C0000 + 0x00000018)
#define  GPIO_MFCTL1                 (0xB01C0000 + 0x0000001C)

//-------OVER-------------------------------------------------------
#endif  /* _REG_H_ */

