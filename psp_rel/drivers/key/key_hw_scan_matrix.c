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
 * \file     key_hw_scan_matrix.c
 * \brief    实现矩阵中断扫描按键
 * \author   xkping
 * \par      GENERAL DESCRIPTION:
 *
 * \par      EXTERNALIZED FUNCTIONS:
 *
 * \version 1.0
 * \date  2009/05/27
 *******************************************************************************/

#include "key_config.h"
#include "key_inner.h"

#define KEY_CTL_PENDING_BIT 0x00020000

#ifndef BUTTON_INTERRUPT_NUM
#define BUTTON_INTERRUPT_NUM 13
#endif

unsigned int key_dat0_value = 0;
unsigned int last_key_val = 0;
unsigned int time_count = 0;

//检测是否有scan matrix key 按下
static int detec_scan_matrix_key(unsigned int irq, void *dev_id);

/*计算有几个键同时按下*/
static unsigned int count_key_num(unsigned int keyValue);

/*计算每个键对应的irq value*/
static unsigned int count_current_irqvalue(unsigned int keyValue, unsigned int num);

//按键映射对应
static unsigned int act_button_button_value(unsigned int irq_val);

int key_scan_matrix_init(void)
{
#if(SCAN_MATIRX_KEY_ENABLE >0)
    unsigned int tmp;

    tmp = act_readl(GPIO_MFCTL1)|(0x1<<31)|(0x1<<12);//change pin to key scan module
    act_writel(tmp,GPIO_MFCTL1);

    tmp = act_readl(CMU_DEVCLKEN);
    tmp |= 0x1<<25; //enable key scan module clk
    act_writel(tmp, CMU_DEVCLKEN);

    act_writel(KEY_CTL_INIT,KEY_CTL);

    request_irq(BUTTON_INTERRUPT_NUM, detec_scan_matrix_key, 0, "buttons", (void *)0);
    //printf("KEY_CTL_INIT = %x  %x\n",KEY_CTL_INIT,act_readl(KEY_CTL));
#else

    unsigned int tmp;
    tmp = act_readl(CMU_DEVCLKEN);
    tmp &= (~(0x1 << 25)); //diable key scan module clk
    act_writel(tmp, CMU_DEVCLKEN);

    act_writel(0x0, KEY_CTL);
#endif
    return 0;
}

int key_scan_matrix_exit(void)
{
#if(SCAN_MATIRX_KEY_ENABLE >0)
    unsigned int tmp;
    tmp = act_readl(CMU_DEVCLKEN);
    tmp &= (~(0x1<<25)); //diable key scan module clk
    act_writel(tmp, CMU_DEVCLKEN);

    act_writel(0x0,KEY_CTL);
#else

#endif
    return 0;
}

int get_scan_matrix_keyval(void)
{
#if(SCAN_MATIRX_KEY_ENABLE >0)

    int keyval = KEY_NULL;

    if(last_key_val !=0)
    {
        keyval = last_key_val;
        time_count ++;
        if(time_count >=2)
        {
            last_key_val =0;
        }
    }
    return keyval;
#else
    return KEY_NULL;
#endif
}

//检测是否有scan matrix key 按下
//若有，则读取key dat0
static int detec_scan_matrix_key(unsigned int irq, void *dev_id)
{
    unsigned int key_ctl_val;
    unsigned int tmp;
    int level, i;
    int keyval = KEY_NULL;

    key_ctl_val = act_readl(KEY_CTL);

    if ((key_ctl_val & KEY_CTL_PENDING_BIT) == 0)
    {
        key_ctl_val = key_ctl_val | KEY_CTL_PENDING_BIT;
        act_writel(key_ctl_val, KEY_CTL);
        return -1;
    }

    key_dat0_value = act_readl(KEY_DAT0);
    key_dat0_value = ~key_dat0_value;
    act_writel(key_ctl_val, KEY_CTL);
    //printf("key_dat0_value=%x\n",key_dat0_value);
    level = (int)count_key_num(key_dat0_value);
    for (i = 1; i <= level; i++)
    {
        tmp = count_current_irqvalue(key_dat0_value, (unsigned int)i);
        keyval = (int)((unsigned int) keyval | act_button_button_value(tmp));
    }//end of for i..
    time_count = 0;
    last_key_val = (unsigned int)keyval;
    return 0;
}

/*计算有几个键同时按下*/
static unsigned int count_key_num(unsigned int irq_val)
{
    unsigned int tmp;
    int count = 0;
    int i;
    /* count how much key is pressed the same time. */
    tmp = irq_val;

    for (i = 0; i < 32; i++)
    {
        if ((tmp & 0x1) != 0)
        {
            count++;
        }
        tmp = tmp >> 1;
    }

    return count;
}

/*计算每个键对应的irq value*/
static unsigned int count_current_irqvalue(unsigned int irq_val, unsigned int num)
{
    unsigned int tmp;
    unsigned int bit_count, count;
    int i;
    bit_count = 0;
    count = 0;
    /* count the num'th key is pressed  */
    tmp = irq_val;

    for (i = 0; i < 32; i++)
    {
        if ((tmp & 0x1) != 0)
        {
            count++;
            if (count == num)
            {
                break;
            }
        }
        tmp = tmp >> 1;
        bit_count++;
    }

    tmp = irq_val;
    tmp = tmp & (unsigned int) (0x01 << bit_count);

    return tmp;
}

//按键映射对应
static unsigned int act_button_button_value(unsigned int irq_val)
{

    unsigned int cur_key = 0;

    switch (irq_val)
    {
        /* KSIN0 */
        case 0x00000001: //KSOUT0
        cur_key = KIN0_KOU0;
        break;
        case 0x00000100: //KSOUT1
        cur_key = KIN0_KOU1;
        break;
        case 0x00010000: //KSOUT2
        cur_key = KIN0_KOU2;
        break;
        case 0x01000000: //KSOUT3
        cur_key = KIN0_KOU3;
        break;

        /* KSIN1 */
        case 0x00000002: //KSOUT0
        cur_key = KIN1_KOU0;
        break;
        case 0x00000200: //KSOUT1
        cur_key = KIN1_KOU1;
        break;
        case 0x00020000: //KSOUT2
        cur_key = KIN1_KOU2;
        break;
        case 0x02000000: //KSOUT3
        cur_key = KIN1_KOU3;
        break;

        /* KSIN2*/
        case 0x00000004: //KSOUT0
        cur_key = KIN2_KOU0;
        break;
        case 0x00000400: //KSOUT1
        cur_key = KIN2_KOU1;
        break;
        case 0x00040000: //KSOUT2
        cur_key = KIN2_KOU2;
        break;
        case 0x04000000: //KSOUT3
        cur_key = KIN2_KOU3;
        break;

        /* KSIN3 */
        case 0x00000008: //KSOUT0
        cur_key = KIN3_KOU0;
        break;
        case 0x00000800: //KSOUT1
        cur_key = KIN3_KOU1;
        break;
        case 0x00080000: //KSOUT2
        cur_key = KIN3_KOU2;
        break;
        case 0x08000000: //KSOUT3
        cur_key = KIN3_KOU3;
        break;

//    /* KSIN6 */
//    case 0x00000040:    //KSOUT0
//        cur_key = KIN6_KOU0; 
//        break;
//    case 0x00004000:    //KSOUT1
//        cur_key = KIN6_KOU1; 
//        break;
//    case 0x00400000:    //KSOUT2
//        cur_key = KIN6_KOU2; 
//        break;
//    case 0x40000000:    //KSOUT3
//        cur_key = KIN6_KOU3; 
//        break;

        /* default */
        default:
        cur_key = KEY_NULL;
        break;
    }
    return cur_key;

}

