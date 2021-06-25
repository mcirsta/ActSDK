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
* \file Lcd_set_back_light.c
* \brief framebuffer driver for gui
* \author shaofeng
* \par GENERAL DESCRIPTION:
*       This file provide the functions to control lcd back light.
* \par DECLARATIONS:
*       (c) Copyright 2002-2004, Actions Co,Ld. All Right Reserved.
*
* \version 1.0
* \date 2007/11/25
*/

#include "lcd_inner.h"

unsigned int saved_backlight_level = 0;
static unsigned int  max_input = 5;

#ifdef BL_PWM
unsigned int backlight_level_map[11] ={0, 3, 5, 15, 24, 28,30,30,30,30,30};
static int  _set_pwm_mode(unsigned int val)
{
    unsigned int value;
    if (val > 31) 
    {
                    /*printf("voltage is out of range!\n");*/
        return -1;
    }
    /* set ic's internal dc-dc module */
    act_writel(act_readl(PMU_CTL) | 0x1 << 15, PMU_CTL);
  
    value = act_readl(PMU_CHG);
    value = value | 0xc000;
    value &= 0xffffe0ff;
    value |= (val << 8);		
    act_writel(value, PMU_CHG);    
    return 0;
}
#define _SET_BACKLIGHT   _set_pwm_mode
#endif


#ifdef BL_PFM
unsigned int backlight_level_map[11] ={0, 1, 2, 3, 4, 5, 5, 5, 5, 5, 5};
static int  _set_pfm_mode(unsigned int val)
{
    unsigned int value;
    unsigned char ic_type;
    
    if (val > 8) {
        //		printf("voltage is out of range!\n");
        return -1;
    }
    
         ic_type = getICType(); //to distinguish 3996/3963, cccw 20080320   
    /* set ic's internal dc-dc module */
    /* 5-3	BLVS	Back Light Voltage Set Output */
    /* 000 10V; 001 11V; 010* 12V; 011 13V; 100 14V; 101 15V; 110 16V; 111 current mode;default is 0x2 */
    value = act_readl(PMU_CTL);
    //value = value | 0xb0000000 | 0x8000;	/* enable back light(0x8000) */
    if (val) 
    {
        value = value | 0x8000;	/* enable back light(0x8000) */
        value &= 0xffffffc7;
    
        if(ic_type == 0x96)
        {
            val = 8 - val;
        }
        else
        {
            val--;
        }    
    
        value |= (val << 3);
    } 
    else
    {
        value = value & (~(0x8000));	/* disable back light */
    }
    act_writel(value, PMU_CTL);
    return 0;  
}
#define _SET_BACKLIGHT   _set_pfm_mode
#endif


#ifdef BL_GPIO
unsigned int backlight_level_map[11] ={0, 5, 10, 15, 20, 24,30,30,30,30,30};
static int  _set_gpio_mode(unsigned int val)  //#ifdef  back light  IC is DS9364, gpioB31 pin
{
    static unsigned int saved_val = 0;    
    unsigned int value;
    int i;   
    if (val > 15) 
    {
        //printf("voltage is out of range!\n");
        return -1;
    }    
    if (val == saved_val)
    {
        return 0;
    }  

    /* enable  gpio pin out. */
    value = act_readl(GPIO_BOUTEN);
    value |= (0x1 << 31);
    act_writel(value, GPIO_BOUTEN);    
    /* output clk */
    value = act_readl(GPIO_BDAT);
    value &= (~(0x1 << 31));
    act_writel(value, GPIO_BDAT);       
    /* shouldn't be too long, otherwise eyes will detect backlight is shut down. */
    lcd_mdelay(3);    
    if (val == 0)
    {
        saved_val = 0;
        return 0;
    }
    else 
    {
        i = 16 - val;
    }
    
    for (; i > 0; i--) 
    { 
        value |= (0x1 << 31);	/* output clk */
        act_writel(value, GPIO_BDAT);
        lcd_udelay(400);
        value &= (~(0x1 << 31));
        act_writel(value, GPIO_BDAT);
        lcd_udelay(100);
    }
    value |= (0x1 << 31);	/* output clk */
    act_writel(value, GPIO_BDAT);
    lcd_udelay(100);
    
    saved_val = val;       
    return 0;   
}
#define _SET_BACKLIGHT   _set_gpio_mode
/*附备选方案：可用BL_NDR引脚发脉冲控制DS9364，这样可省出一个通用GPIO。
设置占空比为0，通过控制输出级性来发出脉冲，流程为：
prepare：
    act_writel(act_readl(CMU_FMCLK) | 0x1 << 5, CMU_FMCLK);//set pwm fre
    act_writel(act_readl(CMU_FMCLK) & ~(0x1 << 4), CMU_FMCLK);       
    act_writel(act_readl(CMU_FMCLK) | (0x3 << 2), CMU_FMCLK);
    act_writel(act_readl(PMU_CTL) | 0x1 << 15, PMU_CTL);     //enable output
    act_writel(act_readl(PMU_CHG) &0xffffe0ff, PMU_CHG);   //设置0占空比
set low :   
    act_writel(act_readl(PMU_CHG) | 0x1 << 14, PMU_CHG);   
set high :   
    act_writel(act_readl(PMU_CHG) &(~( 0x1 << 14)), PMU_CHG);    
*/

#endif



/*!
* set lcd back light level
*
* \par  Description:
*       This function set lcd back light level. \n
* \param [in]   bl_level lcd back light level
* \return       whether set back light successfully
* \retval       0 success
* \retval       -1 fail
*/
int lcd_set_back_light(unsigned int bl_level)
{
    if (bl_level > max_input)
    {  
        return -1; //failed
    }   
//    _SET_BACKLIGHT(	backlight_level_map[bl_level] );	
    
    welcome_set_back_light(backlight_level_map[bl_level]);	
    saved_backlight_level = bl_level;
    return 0;//error
}

int welcome_set_back_light(unsigned int level)
{
    if (level > 31)
    {  
        return -1; //failed
    }   
    _SET_BACKLIGHT(	level );		
//    saved_backlight_level = bl_level;
    return 0;//error
}


/*!
* get lcd back light level
*
* \par  Description:
*       This function set lcd back light level. \n
* \return       lcd back light level
* \retval       back light level
*/
unsigned int lcd_get_back_light(void)
{
    return saved_backlight_level;
}



/*!
* initial lcd back light level
*
* \par  Description:
*       back light level information get form vram. \n
* \return       none
*/
int	backlight_level_init(void)
{
//    int i;
//    int num;
//    vram_param_t param;
//    
//    syscfg_t *syscfg = (void*)malloc(sizeof(syscfg_t));
//    if(syscfg == 0)
//    {
//        return -1;
//    }
//    param.pbuf = (void*)syscfg;
//    param.length = sizeof(syscfg_t);
//    param.offset = 0;
//    param.info_id = APP_INFO_PARTION;
//    param.info_subid = APP_SYSCFG;
//
//    vram_read(&param);
//
//    num = syscfg->light_level.max_level ;
//    max_input = num;
//    num++;
//    
//    for (i = 0; i < num; i++)
//    {
//        backlight_level_map[i] = syscfg->light_level.level[i];
//    }
//    free(syscfg);
    
    return 0;
}


