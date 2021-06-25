/*
********************************************************************************
*                       usdk130
*                    Module: LCM Driver
*                Copyright(c) 2001-2007 Actions Semiconductor, 
*                        All Right Reserved 
*
* History:         
*      <author>    <time>       <version >    <desc>
*      shaofeng    2007/11/25     1.0         build this file 
********************************************************************************
*/


/*!
* \file Lcd_yuv2rgb.c
* \brief yuv2rgb module api functions
* \author shaofeng
* \par GENERAL DESCRIPTION:
*       This file provide the yuv2rgb module functions.
* \par DECLARATIONS:
*       (c) Copyright 2002-2004, Actions Co,Ld. All Right Reserved.
*
* \version 1.0
* \date 2007/11/25
*/


#include "lcd_inner.h"
#include "lcd_yuv2rgb.h"


/*!
* the saved gpio_multipin0 val for multifunc pin
*/
static unsigned long val_gpio_mfctl0;


//this funciton onlu used when lcd init/reinit
void set_yuv2rgb_clk(int ahb_clk)
{  
    unsigned int val;

    if ( (ahb_clk >150)||(ahb_clk <=0) )
    {
        printf("set_yuv2rgb_clk error!\n");
        act_writel(0x050a, YUV2RGB_CLKCTL);   
        return;
    }
    
    if (ahb_clk >=75)
    {
        val = 0x0408;  //CLK_DIV_168;
    }
    else if(ahb_clk >=50)
    {
        val = 0x0306;  // CLK_DIV_120 ;
    }
    else if(ahb_clk >=25)
    {
        val = 0x0204;  //  ;
    }
    else 
    {
        val = 0x0102;  //  ;
    }           

#if 0     //观测系统频率点时打开
    static int num =0;
    if(num!=0)
        printf("set_yuv2rgb_clk ahb_clk:%d,val:%x  !\n",ahb_clk,val);                
    num++;  
#endif    
    /*
     * 0x00000102 1/3 clock division;
     * 0x00000303 1/6 clock division;
     */
    act_writel(val, YUV2RGB_CLKCTL);   
    return;  
}
/*!
* initial yuv2rgb module
*
* \par  Description:
*       This function initial yuv2rgb module, and enable it. \n
* \return       none
*/
static void _dev_enable(void)
{
    unsigned int val;
    

    val = act_readl(YUV2RGB_CTL);
    val |= (0x1 << 19);
    act_writel(val, YUV2RGB_CTL);	//SP bus
    return;
};


/*!
* disable yuv2rgb module
*
* \par  Description:
*       This function disable yuv2rgb module. \n
* \return       none
*/
static void _dev_disable(void)
{
    unsigned int val;
    val = act_readl(YUV2RGB_CTL);
    val &= ~(0x1 << 19);
    act_writel(val, YUV2RGB_CTL);	//SP bus
    return;
};



/*!
* wait yuv2rgb module's fifo buffer empty
*
* \par  Description:
*       This function will return until fifo is empty. \n
* \return       the fifo is empty or time out!
* \retval       0 empty
* \retval       -1 err, and time out
*/
int  lcd_wait_fifo_empty(void)
{
    volatile unsigned int val;
    unsigned int count;

    count = 50000;
    do 
    {
        count--;
        if (!count)
        {
            printf("lcd: wait yuv fifo empty time out \n");
            /*printf("lcd dbg: YUV2RGB_CTL %x \n", act_readl(YUV2RGB_CTL));*/
            return -1;
        }
        val = act_readl(YUV2RGB_CTL);
        val &= (0x1 << 18);
    } while (!val);

    return 0;
};


/*!
* set yuv2rgb module working
*
* \par  Description:
*       This function initial yuv2rgb module, and set GPIO pins in yuv2rgb module . \n
* \return       always return 0
*/
int  lcd_get_dev(void)
{
    OS_INT_SAVE_AREA                 
    if ( multip_get_pinowner(0,0,0,0) != 1)
    {
        return -1;
    }		    
    _dev_enable(); 			
    OS_ENTER_CRITICAL();
    val_gpio_mfctl0 = act_readl(GPIO_MFCTL0);
    act_writel( (val_gpio_mfctl0 &(~GPIO_MULTI_LCD_MASK)) \
                                                   | GPIO_MULTI_LCD_VAL, GPIO_MFCTL0);
    OS_EXIT_CRITICAL();
    return 0;
}


/*!
* release yuv2rgb module
*
* \par  Description:
*       This function disable yuv2rgb module, and set GPIO pins in previous function. \n
* \return       always return 0
*/
int  lcd_release_dev(void)
{
    OS_INT_SAVE_AREA    

    lcd_wait_fifo_empty();
    lcd_udelay(10);
    _dev_disable();
    /* restore the gpio regs value and the each multipin chan owner */
    OS_ENTER_CRITICAL();	
    act_writel(val_gpio_mfctl0, GPIO_MFCTL0);
    OS_EXIT_CRITICAL();
    multip_release_pinowner(0,0);
    return 0;
}

 
