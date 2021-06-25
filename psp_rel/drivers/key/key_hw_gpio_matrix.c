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
 * \file     key_hw_matrix_gpio.c
 * \brief    实现矩阵gpio扫描按键
 * \author   shaofeng
 * \par      GENERAL DESCRIPTION:
 *
 * \par      EXTERNALIZED FUNCTIONS:
 *
 * \version 1.0
 * \date  2009/05/10
 *******************************************************************************/

/*本模块的头文件*/
#include "key_config.h"
#include "key_inner.h"

static const gpio_matrix_key_map_t gpio_matrix_map[] =
{
#if GPIO_MATRIX_KEY_ENABLE > 0
    GPIO_MATRIX_KEY0,
    GPIO_MATRIX_KEY1,
    GPIO_MATRIX_KEY2,
    GPIO_MATRIX_KEY3,
    GPIO_MATRIX_KEY4,
    GPIO_MATRIX_KEY5,
#else
    0
#endif
};

/******************************************************************************/
/*!
 * \par  Description:
 *      initial function
 *******************************************************************************/
int key_gpio_matrix_init(void)
{
#if GPIO_MATRIX_KEY_ENABLE > 0

    //enable gpio clk and mutilfuction
    unsigned int tmp=0;
    tmp = act_readl(CMU_DEVCLKEN)|(0x1<<26);//bit26 is gpio clk
    act_writel(tmp,CMU_DEVCLKEN);

    tmp = act_readl(GPIO_MFCTL1)|(0x1<<31);
    act_writel(tmp,GPIO_MFCTL1);

#endif
    return 0;
}

/******************************************************************************/
/*!
 * \par  Description:
 *      deinit function
 *******************************************************************************/
int key_gpio_matrix_exit(void)
{
#if GPIO_MATRIX_KEY_ENABLE > 0
#endif
    return 0;
}

/******************************************************************************/
/*!
 * \par  Description:
 *      GPIO矩阵按键不支持多按键，一旦检测到有同类型按键，马上返回
 * \retval       int: 键值
 *******************************************************************************/
int get_gpio_matrix_keyval(void)
{
#if GPIO_MATRIX_KEY_ENABLE > 0
    int i = 0;
    int gpio_state;
    int gpio_group;
    int gpio_num;
    int keyval = KEY_NULL;

    for( i=0; i<GPIO_MATRIX_KEY_NUM; i++ )
    {
        if( gpio_matrix_map[i].keyval==KEY_NULL)
        {
            continue;
        }
        //set output
        gpio_group = gpio_matrix_map[i].output_gpio.gpio_group;
        gpio_num = gpio_matrix_map[i].output_gpio.pin_num;
        gpio_state = gpio_matrix_map[i].output_gpio_state;
        gpio_enable_output(gpio_group,gpio_num);
        gpio_set_state(gpio_group,gpio_num,gpio_state);

        //printf("output gpio_group=%d gpio_num=%d gpio_state=%d\n",gpio_group,gpio_num,gpio_state);

        //set input
        gpio_group = gpio_matrix_map[i].input_gpio.gpio_group;
        gpio_num = gpio_matrix_map[i].input_gpio.pin_num;
        gpio_enable_input(gpio_group,gpio_num);

        //get input gpio state
        gpio_state = gpio_get_state(gpio_group,gpio_num);

        //printf("input gpio_group=%d gpio_num=%d gpio_state=%d\n\n",gpio_group,gpio_num,gpio_state);

        if(gpio_state==gpio_matrix_map[i].input_gpio_state)
        {
            keyval = (int)gpio_matrix_map[i].keyval;
            break;
        }
    }
    return keyval;

#else
    return KEY_NULL;
#endif
}

