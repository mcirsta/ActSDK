/*******************************************************************************
 *                              NOYA1100
 *                            Module: drv
 *                 Copyright(c) 2003-2008 Actions Semiconductor,
 *                            All Rights Reserved.
 * History:
 *      <author>    <time>           <version >             <desc>
 *      shaofeng    2009-05-21          1.0             build this file
 *******************************************************************************/
/*!
 * \file     operate_gpio.c
 * \brief    操作GPIO函数
 * \author   shaofeng
 * \par      GENERAL DESCRIPTION:
 *
 * \par      EXTERNALIZED FUNCTIONS:
 *
 * \version 1.0
 * \date  2009/05/10
 *******************************************************************************/

/*本模块的头文件*/
#include "operate_gpio.h"
#include <asm-mips/mach-atj213x/atj213x.h>
#include <asm-mips/mach-atj213x/actions_reg_213x.h>


/******************************************************************************/
/*!
 * \par  Description:
 *
 * \param[in]    gpio_group_e: gpio 组号
 * \param[in]    int: pin脚号
 * \retval       int:
 *******************************************************************************/
int gpio_enable_output(gpio_group_e gpio_group, int pin_num)
{
    unsigned int tmp;

    if ((pin_num < 0) || (pin_num > 31))
    {
        return -1;
    }

    if (GPIO_GROUP_A == gpio_group)
    {
        //disable GPIOIN
        tmp = act_readl(GPIO_AINEN);
        tmp &= (unsigned int) (~(0x1 << pin_num));
        act_writel(tmp, GPIO_AINEN);

        //enable GPIOOUT
        tmp = act_readl(GPIO_AOUTEN);
        tmp |= (unsigned int) (0x1 << pin_num);
        act_writel(tmp, GPIO_AOUTEN);
    }
    else  
    {
        //disable GPIOIN
        tmp = act_readl(GPIO_BINEN);
        tmp &= (unsigned int) (~(0x1 << pin_num));
        act_writel(tmp, GPIO_BINEN);

        //enable GPIOOUT
        tmp = act_readl(GPIO_BOUTEN);
        tmp |= (unsigned int) (0x1 << pin_num);
        act_writel(tmp, GPIO_BOUTEN);
    }

    return 0;
}

/******************************************************************************/
/*!
 * \par  Description:
 *
 * \param[in]    gpio_group_e: gpio 组号
 * \param[in]    int: pin脚号
 * \retval       int:
 *******************************************************************************/
int gpio_enable_input(gpio_group_e gpio_group, int pin_num)
{
    unsigned int tmp;

    if ((pin_num < 0) || (pin_num > 31))
    {
        return -1;
    }

    if (GPIO_GROUP_A == gpio_group)
    {
        //disable GPIOOUT
        tmp = act_readl(GPIO_AOUTEN);
        tmp &= (unsigned int) (~(0x1 << pin_num));
        act_writel(tmp, GPIO_AOUTEN);

        //enable GPIOIN
        tmp = act_readl(GPIO_AINEN);
        tmp |= (unsigned int) (0x1 << pin_num);
        act_writel(tmp, GPIO_AINEN);
    }
    else  
    {
        //disable GPIOOUT
        tmp = act_readl(GPIO_BOUTEN);
        tmp &=(unsigned int)(~(0x1 << pin_num));
        act_writel(tmp, GPIO_BOUTEN);

        //enable GPIOIN
        tmp = act_readl(GPIO_BINEN);
        tmp |= (unsigned int)(0x1 << pin_num);
        act_writel(tmp, GPIO_BINEN);
    }
    return 0;
}

/******************************************************************************/
/*!
 * \par  Description:
 *
 * \param[in]    gpio_group_e: gpio 组号
 * \param[in]    int: pin脚号
 * \param[in]    int: 1:高, 0:低状态
 * \retval       int:
 *******************************************************************************/
int gpio_set_state(gpio_group_e gpio_group, int pin_num, int state)
{
    unsigned int tmp;
    if ((pin_num < 0) || (pin_num > 31))
    {
        return -1;
    }

    if (GPIO_GROUP_A == gpio_group)
    {
        //set low
        if (0 == state)
        {
            tmp = act_readl(GPIO_ADAT);
            tmp &= (unsigned int) (~(0x1 << pin_num));
            act_writel(tmp, GPIO_ADAT);
        }
        //set high
        else
        {
            tmp = act_readl(GPIO_ADAT);
            tmp |= (unsigned int) (0x1 << pin_num);
            act_writel(tmp, GPIO_ADAT);
        }
    }
    else  
    {
        //set low
        if (0 == state)
        {
            tmp = act_readl(GPIO_BDAT);
            tmp &= (unsigned int)(~(0x1 << pin_num));
            act_writel(tmp, GPIO_BDAT);
        }
        //set high
        else
        {
            tmp = act_readl(GPIO_BDAT);
            tmp |= (unsigned int)(0x1 << pin_num);
            act_writel(tmp, GPIO_BDAT);
        }
    }
    return 0;
}

/******************************************************************************/
/*!
 * \par  Description:
 *
 * \param[in]    gpio_group_e: gpio 组号
 * \param[in]    int: pin脚号
 * \retval       int: 1:高, 0:低
 *******************************************************************************/
int gpio_get_state(gpio_group_e gpio_group, int pin_num)
{
    unsigned int tmp = 0;
    if ((pin_num < 0) || (pin_num > 31))
    {
        return -1;
    }
    if (GPIO_GROUP_A == gpio_group)
    {
        tmp = act_readl(GPIO_ADAT);
        tmp &= (unsigned int) (0x1 << pin_num);
    }
    else  
    {
        tmp = act_readl(GPIO_BDAT);
        tmp &= (unsigned int) (0x1 << pin_num);
    }

    if (0 == tmp)
    {
        return 0;
    }
    else
    {
        return 1;
    }
}

