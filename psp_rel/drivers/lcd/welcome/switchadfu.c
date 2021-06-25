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
* \file Switchadfu.c
* \brief whether to enter ADFU for brec.
* \author shaofeng
* \par GENERAL DESCRIPTION:
* This file	to provide key init and key detect functions.
* \par DECLARATIONS:
*       (c) Copyright 2002-2004, Actions Co,Ld. All Right Reserved.
*
* \version 1.0
* \date 2007/11/25
*/


/*!
* The Address of Key Scan registers
*/
#include "display_logo.h"
#define KEY_CON_ADDR	0xB01A0000
#define KEY_CON_INIT	0x00F30f05//0x00030F05
#define DEV_CLK_EN	0xB0010080

//#define GPIO_MFCTL1	0xB01C001C

#define KEY_CON_PENDING_BIT 0x00020000

#define KEY_DATA0_ADDR	0xB01A0004
#define KEY_DATA1_ADDR	0xB01A0008
#define KEY_DATA2_ADDR	0xB01A000C
#define KEY_DATA3_ADDR	0xB01A0010


static void delay(void);
static inline unsigned int actions_readl(unsigned long);
void static inline actions_writel(unsigned int, int);
static void inline init_button_reg(void);
static int act_buttons_detect(void);



/*!
* whether enter ADFU, for brec.
*
* \par  Description:
*       initial key, and detect key state, then determine whether enter ADFU, for brec. \n
* \return       whether switch to ADFU
* \retval       0 not switch to ADFU
* \retval       -1 switch to ADFU
*/
int brec_switch_adfu( void )
{
    int key_ret = 0;
#if 1
    unsigned int tmp,tmp_a,tmp_a_1;
    unsigned int cur_matrix_flag = 0;
    
          
    // first step: ksin0,ksin1,ksin2 as input ,分别对应GPIO_A8,A9, A10   
    tmp=act_readl(GPIO_AINEN); 
    tmp |= (0x7<<8);
    act_writel(tmp,GPIO_AINEN);    
    tmp=act_readl(GPIO_AOUTEN); 
    tmp &= ~(0x7<<8);
    act_writel(tmp,GPIO_AOUTEN);    
    delay();
    delay();
    tmp_a = act_readl(GPIO_ADAT);
    //mdelay(1);
    delay();
    tmp_a_1 = act_readl(GPIO_ADAT);
    if(tmp_a != tmp_a_1)
    {
        return 0;    
    }

    tmp = (tmp_a >>ENTER_ADFU_GPIO_NUM) & 0x1;  //ksin0
    if(tmp == 0)
    //    cur_matrix_flag |=(0x1<<S8);	
    {
        return -1;//adfu mode	    
    }
    tmp = (tmp_a >>ENTER_STUB_GPIO_NUM) & 0x1;  //ksin1
    if(tmp == 0)
     //   cur_matrix_flag |=(0x1<<S9);
    {
        return 2;////debug mode
    }
    
//	tmp = tmp_a >>10 & 0x1;  //ksin2
//	if(tmp == 0)
//	    cur_matrix_flag |=(0x1<<S10);
//	    
//    if(cur_matrix_flag !=0)
//    {
//        return cur_matrix_flag;
//    }    
#endif    

#if 0 //demo 机的按键
    act_buttons_init();

    delay();

    key_ret = act_buttons_detect();
#endif
    return key_ret;
}


/*!
* delay a time.
*
* \par  Description:
*       just delay, and the time is fixed.
* \return       none
*/
static void delay( void )
{
    int tmp = 0x1fffff;
    while (tmp != 0)
    {
        tmp--;
    }
    return;
}


/*!
* read the value of requested register.
*
* \par  Description:
*       get the register value.
* \param [in]   reg address of register
* \return       register value
* \retval       register value
*/
static inline unsigned int actions_readl(unsigned long reg)
{
    return (*(volatile unsigned int*)reg);
}


/*!
* write data to requested register.
*
* \par  Description:
*       write data to the register.
* \param [in]   val data wanted to write
* \param [in]   reg address of register
* \return       none
*/
void static inline actions_writel(unsigned int val, int reg)
{
    *(volatile unsigned int*)(reg) = val;
}


/*!
* initail key register.
*
* \par  Description:
*       initial key module's clock and make it working as irq scan mode.
* \return       none
*/
static void inline init_button_reg( void )
{
    unsigned long tmp;
    tmp = actions_readl(DEV_CLK_EN);
    tmp |= 0x06000000;
    actions_writel(tmp, DEV_CLK_EN);

    tmp = actions_readl(GPIO_MFCTL1);
//  tmp &= ~0x00002000;
    tmp |= 0x80001000;
    actions_writel(tmp, GPIO_MFCTL1);
    actions_writel(KEY_CON_INIT, KEY_CON_ADDR);
}


/*!
* detect key state.
*
* \par  Description:
*       detect key state.
* \return       key state
* \retval       0 play key not pressed
* \retval       -1 play key pressed
*/
static int act_buttons_detect( void )
{
    unsigned int keycon;
    unsigned int tmp;
    unsigned int irq_key_value;
    keycon = actions_readl(KEY_CON_ADDR);
    tmp = keycon & KEY_CON_PENDING_BIT;

    if (tmp!=0)    /* Detect key IRQ */
    {
        irq_key_value = actions_readl(KEY_DATA0_ADDR);
        irq_key_value = ~irq_key_value;

        actions_writel(keycon, KEY_CON_ADDR);

        if ((irq_key_value&0x00020000) == 0x00020000)
        {/*press play key*/
            return -1;	//adfu mode
        }
        else if (irq_key_value == 0x00000200)
        {/*press return key*/
            return 1;	//test mode
        }
        else if ((irq_key_value&0x000004) == 0x000004)
        {/*press navi key*/
            return 2;	//debug mode
        }    
        else if ((irq_key_value&0x00000002) == 0x00000002)
        {/*press ok key*/
            return 3;	//usbprt mode
        }            
        else
        {
        }
    }

    return 0;
}


/*!
* initail key register.
*
* \par  Description:
*       initial key module's clock and make it working as irq scan mode.
* \return       none
*/
void act_buttons_init(void)
{
    init_button_reg();
}

void power_on_hook(void)
{
    act_writel((act_readl(GPIO_BDAT) & (~(0x01<<29))), GPIO_BDAT);
    act_writel((act_readl(GPIO_BOUTEN) | (0x01<<29)), GPIO_BOUTEN);
    lcd_mdelay(50);
    act_writel((act_readl(GPIO_BDAT) | (0x01<<29)), GPIO_BDAT);
    lcd_mdelay(1);
    act_writel((act_readl(GPIO_BDAT) & (~(0x01<<29))), GPIO_BDAT);
    lcd_mdelay(50);
    act_writel((act_readl(GPIO_BDAT) | (0x01<<29)), GPIO_BDAT);
    lcd_mdelay(1);
    act_writel((act_readl(GPIO_BDAT) & (~(0x01<<29))), GPIO_BDAT);
    act_writel((act_readl(GPIO_BOUTEN) & (~(0x01<<29))), GPIO_BOUTEN);     

}
