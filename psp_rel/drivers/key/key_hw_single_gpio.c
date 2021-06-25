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
 * \file     key_hw_single_gpio.c
 * \brief    ʵ�ֵ���gpioɨ�谴��
 * \author   shaofeng
 * \par      GENERAL DESCRIPTION:
 *
 * \par      EXTERNALIZED FUNCTIONS:
 *
 * \version 1.0
 * \date  2009/05/10
 *******************************************************************************/

/*��ģ���ͷ�ļ�*/
#include "key_config.h"
#include "key_inner.h"

static int powerkeyvalid_flag = 1; //1: power ��ֵ����Ч; 0��Ч
static int powerkeyvalid_order =-1;
static const single_gpio_key_map_t single_gpio_map[] =
{
#if SINGLE_GPIO_KEY_ENABLE > 0
    SINGLE_GPIO_KEY0,
    SINGLE_GPIO_KEY1,
    SINGLE_GPIO_KEY2,
    SINGLE_GPIO_KEY3,
    SINGLE_GPIO_KEY4,
    SINGLE_GPIO_KEY5,
    SINGLE_GPIO_KEY6,
    SINGLE_GPIO_KEY7,
#else
    0
#endif
};

/******************************************************************************/
/*!
 * \par  Description:
 *      initial function
 *******************************************************************************/
int key_single_gpio_init(void)
{
#if SINGLE_GPIO_KEY_ENABLE > 0
    //enable gpio clk and mutilfuction
    unsigned int tmp=0;
    int i=0;
    int gpio_state =1;
    tmp = act_readl(CMU_DEVCLKEN)|(0x1<<26);//bit26 is gpio clk
    act_writel(tmp,CMU_DEVCLKEN);

    tmp = act_readl(GPIO_MFCTL1)|(0x1<<31);
    act_writel(tmp,GPIO_MFCTL1);
    //���ʹ����gpio start������Ҫ
    //ȷ�����������ص���ʾ�����˵������У�start ����̧�𣬷���start������Ч   
    for( i=0; i<SINGLE_GPIO_KEY_NUM; i++ )
    {
        if( (single_gpio_map[i].input_gpio.pin_num <0) 
            || (single_gpio_map[i].input_gpio.pin_num >31) 
            ||(single_gpio_map[i].keyval!= (KEY_OK)) )
        {
            continue;            
        }
        
        powerkeyvalid_order = i;
        gpio_enable_input( single_gpio_map[i].input_gpio.gpio_group, single_gpio_map[i].input_gpio.pin_num );
        gpio_state = gpio_get_state( single_gpio_map[i].input_gpio.gpio_group, single_gpio_map[i].input_gpio.pin_num );
        if ( gpio_state == single_gpio_map[i].input_gpio_state )
        {
            powerkeyvalid_flag = 0; //һ��ʼ�Ͱ��£�power��Ч
        }
        else
        {
            powerkeyvalid_flag = 1;//һ��ʼ��û���£�power��Ч    
        }                 
    }            
#endif
    return 0;
}

/******************************************************************************/
/*!
 * \par  Description:
 *      deinit function
 *******************************************************************************/
int key_single_gpio_exit(void)
{
#if SINGLE_GPIO_KEY_ENABLE > 0

#endif
    return 0;
}

/******************************************************************************/
/*!
 * \par  Description:
 *      ��GPIO����֧�ֶఴ����Ҫȫ�������ŷ���
 * \retval       int: ��ֵ
 *******************************************************************************/
int get_single_gpio_keyval(void)
{
#if SINGLE_GPIO_KEY_ENABLE > 0
    int i = 0;
    int gpio_state;
    int keyval = KEY_NULL;

    //set gpio input
    for( i=0; i<SINGLE_GPIO_KEY_NUM; i++ )
    {
        if( (single_gpio_map[i].input_gpio.pin_num <0)
                || (single_gpio_map[i].input_gpio.pin_num >31)
                ||(single_gpio_map[i].keyval==KEY_NULL) )
        {
            continue;
        }
        else
        {
            gpio_enable_input( single_gpio_map[i].input_gpio.gpio_group, single_gpio_map[i].input_gpio.pin_num );
        }
    }

    //read all gpio
    for( i=0; i<SINGLE_GPIO_KEY_NUM; i++ )
    {
        if( (single_gpio_map[i].input_gpio.pin_num <0)
                || (single_gpio_map[i].input_gpio.pin_num >31)
                ||(single_gpio_map[i].keyval==KEY_NULL) )
        {
            continue;
        }
        else
        {
            gpio_state = \
            gpio_get_state(single_gpio_map[i].input_gpio.gpio_group, single_gpio_map[i].input_gpio.pin_num);
            if ( gpio_state == single_gpio_map[i].input_gpio_state )
            {
                if( (i == powerkeyvalid_order)&&(powerkeyvalid_flag ==0))
                {
                    continue;
                }
                keyval =(keyval | single_gpio_map[i].keyval);
            }
            else
            {
                if(i == powerkeyvalid_order)
                {
                    powerkeyvalid_flag = 1;//power��Ч      
                }
            }            
        }
    }
    return keyval;
#else
    return KEY_NULL;
#endif
}

