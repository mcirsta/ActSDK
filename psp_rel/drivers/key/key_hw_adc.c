/*******************************************************************************
 *                              NOYA1100
 *                            Module: drv
 *                 Copyright(c) 2003-2008 Actions Semiconductor,
 *                            All Rights Reserved.
 * History:
 *      <author>    <time>           <version >             <desc>
 *      shaofeng    2009-05-10          1.0             build this file
 *******************************************************************************/
/*!
 * \file     key_hw_lradc.c
 * \brief    实现adc扫描按键
 * \author   shaofeng
 * \par      GENERAL DESCRIPTION:
 *
 * \par      EXTERNALIZED FUNCTIONS:
 *
 * \version 1.0
 * \date  2009/05/10
 *******************************************************************************/

#include "key_config.h"
#include "key_inner.h"

static const lradc_key_map_t adc_key_maps[] =
{
#if (ADC_KEY_ENABLE > 0)
    ADC_KEY0,
    ADC_KEY1,
    ADC_KEY2,
    ADC_KEY3,
    ADC_KEY4,
    ADC_KEY5,
        //new key add here
#else
    0
#endif
};

/******************************************************************************/
/*!
 * \par  Description:
 *      initial function
 *******************************************************************************/
int key_adc_init(void)
{
#if (ADC_KEY_ENABLE > 0)
    unsigned int tmp;

    //enable LRADC
    tmp = act_readl(PMU_CTL);
    tmp |= (0x01<<12);
    act_writel(tmp, PMU_CTL);
#endif
    return 0;
}

/******************************************************************************/
/*!
 * \par  Description:
 *      exit function
 *******************************************************************************/
int key_adc_exit(void)
{
#if ADC_KEY_ENABLE > 0
    unsigned int tmp;

    //disable LRADC
    tmp = act_readl(PMU_CTL);
    tmp &= ~(0x01<<12);
    act_writel(tmp, PMU_CTL);
#endif
    return 0;
}

/******************************************************************************/
/*!
 * \par  Description:
 *      获得adc的键值,并发送对应按键值
 * \retval       int: 键值
 *******************************************************************************/
int get_adc_keyval(void)
{
#if ADC_KEY_ENABLE > 0

    unsigned int adc_val,i;
    int keyval = KEY_NULL;

    //read LRADC value
    adc_val = act_readl(PMU_LRADC);
    adc_val = (adc_val>>24)&0xf;

    if(adc_val==ADC_NULL_VAL)
    {
        keyval = KEY_NULL;
    }
    else
    {
        for (i=0; i<ADC_KEY_NUM; i++)
        {
            if( (adc_val >= (unsigned int)adc_key_maps[i].min_adc_val) && \
                ( adc_val <= (unsigned int)adc_key_maps[i].max_adc_val ))
            {
                keyval = adc_key_maps[i].keyval;
                break;
            }
            else
            {
                continue;
            }
        }
    }
    //printf("adc:%d, key:0x%x\n", adc_val, keyval);
    return keyval;

#else
    return KEY_NULL;
#endif
}

