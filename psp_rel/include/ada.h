/*
 ********************************************************************************
 *                   usdk130 --- adc/dac driver
 *                (c) Copyright 2003-2008, Actions Co,Ld.
 *                        All Right Reserved
 *
 * History:
 *      <author>      <time>       <version >    <desc>
 *       houjingkun      2008/07/30     1.0         build this file
 *       new char device driver for adc/dac, 1017 branch version.
 ********************************************************************************/
/*!
 * \file   ada.h
 * \brief
 *      adc/dac driver's headfile.
 * \author houjingkun
 * \par GENERAL DESCRIPTION:
 * \par EXTERNALIZED FUNCTIONS:
 *       null
 *
 *  Copyright(c) 2003-2008 Actions Semiconductor, All Rights Reserved.
 *
 * \version 1.0
 * \date  2008/07/30
 *******************************************************************************/

#ifndef __ADA_H__
#define __ADA_H__

/*
 * the macro value that set with DACIO_SET_PCM_RATE.
 * refer to ic spec:
 *
 * Audio Clock Divisor,output is FS*256
 * PLL CLK  24.576M,   22.5792M
 *
 * 0   0000:/1 96k 1000:Useless
 * 1   0001:/2 48k 1001:44.1k
 * 2   0010:/3 32k 1010:Useless
 * 3   0011:/4 24k 1011:22.05k
 * 4   0100:/6 16k 1100:Useless
 * 5   0101:/8 12k 1101:11.025k
 * 6   0110:/12 8k 1110:Useless
 * 7   0111:Reserved 1111:Useless
 */
#define ACT_SAMPLE_96K      0x0
#define ACT_SAMPLE_48K      0x1
#define ACT_SAMPLE_44K      0x9
#define ACT_SAMPLE_32K      0x2
#define ACT_SAMPLE_24K      0x3
#define ACT_SAMPLE_22K      0xb
#define ACT_SAMPLE_16K      0x4
#define ACT_SAMPLE_12K      0x5
#define ACT_SAMPLE_11K      0xd
#define ACT_SAMPLE_8K       0x6

/*!
 *  \brief
 *    ADA_VOICE_CHANNEL value
 *  \li ADA_STEREO: stereo voice;
 *  \li ADA_MONO: mono voice;
 */
enum ADA_VOICE_CHANNEL
{
    ADA_STEREO = 0,
    ADA_MONO,
    ADA_NONE,
};

/*!
 * \brief
 *   adc input source setting.
 */
typedef struct
{
    /*! mic: set 1 to enable. */
    unsigned mic :1;
    /*! fm: set 1 to enable. */
    unsigned fm :1;
    /*! linein: set 1 to enable. */
    unsigned linein :1;
    /*! mix: set 1 to enable. */
    unsigned mix :1;
} adc_input_t;

/*!
 * \brief
 *   adc input source gain setting.
 */
typedef struct
{
    /*! mic_gain: the range is 0 -- 7 (increase) */
    unsigned char mic_gain;
    /*! linein_gain: the range is 0 -- 7 (increase) */
    unsigned char linein_gain;
    /*! fm_gain: the range is 0 -- 7 (increase) */
    unsigned char fm_gain;
    /*! adc_gain: the range is 0 -- 7 (increase) */
    unsigned char adc_gain;
} adc_gain_t;

/******************* DAC related ioctl command request. ***********************/
#define DACIO_CMD_BASE          0x40046400

/* sample_rate is originally set to ACT_SAMPLE_48K, you can set accordding the macro of ACT_SAMPLE_XXX */
#define DACIO_SET_PCM_RATE       (DACIO_CMD_BASE + 1)
#define DACIO_GET_PCM_RATE       (DACIO_CMD_BASE + 2)

#define DACIO_SET_PCM_CHANNEL    (DACIO_CMD_BASE + 5)
#define DACIO_GET_PCM_CHANNEL    (DACIO_CMD_BASE + 6)

#define DACIO_SET_PCM_BITS       (DACIO_CMD_BASE + 7)  /* reserved, currently always 32 bit. */
#define DACIO_GET_PCM_BITS       (DACIO_CMD_BASE + 8)

#define DACIO_SET_VOLUME         (DACIO_CMD_BASE + 3)  /* the volum's value is 0x0 ~ 0x1f */
#define DACIO_GET_VOLUME         (DACIO_CMD_BASE + 4)
#define DACIO_SET_MAX_SYS_VOLUME (DACIO_CMD_BASE + 19)

#define DACIO_SET_MUTE          (DACIO_CMD_BASE + 9)  /* set mute or not. arg -- 0: unmute;  1: mute; */

#define DACIO_PCM_PLAY_ON       (DACIO_CMD_BASE + 11) /* default is off, if set on, dac fifo just access by DMA or CPU, not by DSP. */
#define DACIO_SET_PCM_ADDR      (DACIO_CMD_BASE + 13)
#define DACIO_SET_PCM_LEN       (DACIO_CMD_BASE + 15)
#define DACIO_SET_PCM_PLAY      (DACIO_CMD_BASE + 17)
#define DACIO_GET_PCM_SYNC      (DACIO_CMD_BASE + 18)

#define DACIO_SET_FIFO           (DACIO_CMD_BASE + 20)
#define DACIO_GET_FIFO           (DACIO_CMD_BASE + 21)

/******************* ADC related ioctl command request. ***********************/
#define ADCIO_CMD_BASE          0x40046100

#define ADCIO_SET_INPUT         (ADCIO_CMD_BASE + 3)
#define ADCIO_GET_INPUT         (ADCIO_CMD_BASE + 4)

#define ADCIO_SET_GAIN          (ADCIO_CMD_BASE + 5)
#define ADCIO_GET_GAIN          (ADCIO_CMD_BASE + 6)

#define ADCIO_SET_MONITOR       (ADCIO_CMD_BASE + 7)

/* sample_rate is originally set to ACT_SAMPLE_48K, you can set accordding the macro of ACT_SAMPLE_XXX */
#define ADCIO_SET_PCM_RATE      (ADCIO_CMD_BASE + 9)
#define ADCIO_GET_PCM_RATE      (ADCIO_CMD_BASE + 10)

#define ADCIO_SET_PCM_CHANNEL   (ADCIO_CMD_BASE + 11)  /* reserved, currently always stereo. */
#define ADCIO_GET_PCM_CHANNEL   (ADCIO_CMD_BASE + 12)

#define ADCIO_SET_PCM_BITS      (ADCIO_CMD_BASE + 13)  /* reserved, currently always 32 bit. */
#define ADCIO_GET_PCM_BITS      (ADCIO_CMD_BASE + 14)

#define ADCIO_SET_FIFO           (ADCIO_CMD_BASE + 15)
#define ADCIO_GET_FIFO           (ADCIO_CMD_BASE + 16)
/*
 * this function is provided for dsp driver, and don't call it in application code.
 * input -- is_mute != 0, dac set to mute, is_mute == 0, dac set to unmute.
 */
int ada_set_dsp_mute(int is_mute);

/*
 * this function is provided for dsp driver, and don't call it in application code.
 * get current system vloum state: max_sys_vol , max volum level support in sdk
 *                                 cur_vol, current volum level
 */
int ada_get_volum_state(unsigned int *max_sys_vol, unsigned int * cur_vol);

extern int dac_ioctl(unsigned int cmd, unsigned long arg);

extern int adc_ioctl(unsigned int cmd, unsigned long arg);

#endif  /* __ADA_H__ */

