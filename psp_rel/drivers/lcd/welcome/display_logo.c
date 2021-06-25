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
* \file Display_logo.c
* \brief simple lcd init and write for brec for usdk130
* \author shaofeng
* \par GENERAL DESCRIPTION:
* This file	to provide a simple lcm init and update screen, 
*       including lcd init, RGB fullscreen update
* \par DECLARATIONS:
*       (c) Copyright 2002-2004, Actions Co,Ld. All Right Reserved.
*
* \version 1.0
* \date 2007/11/25
*/


#include "display_logo.h"

/*
#define PIC_WIDTH 320
#define PIC_HEIGHT 240
#define PIC_START_X  (XRES-PIC_WIDTH)/2
#define PIC_START_Y  (YRES-PIC_HEIGHT)/2
*/
//the start up picture data array format[5,6,5]


char  low_flag=0;

#pragma __PRQA_IGNORE_START__
#if IS_UPRIGHT
/*!
* record the startup picture screen's range info
*/
static disp_frame_info_t start_up_picture_info = {
	start_addr: (void *)0,
	x:	0,
	y:	0,
	width:	S_XRES,
	height:	S_YRES,
	format:      0x01,    
	arrange_mode:	0x01,
};

/*!
* record the lowpower picture screen's range info
*/
static disp_frame_info_t lowpower_picture_info = {
	start_addr: (void *)0,
	x:	0,
	y:	0,
	width:	S_XRES,
	height:	S_YRES,
	format:      0x01,    
	arrange_mode:	0x01,
};

#else

/*!
* record the startup picture screen's range info
*/
static disp_frame_info_t start_up_picture_info = {
	start_addr:	(void *)0,
	x:	0,
	y:	0,
	width:	H_XRES,
	height:	H_YRES,
	format:      0x01,    
	arrange_mode:	0x01,
};

/*!
* record the lowpower picture screen's range info
*/
static disp_frame_info_t lowpower_picture_info = {
	start_addr:	(void *)0,
	x:	0,
	y:	0,
	width:	H_XRES,
	height:	H_YRES,
	format:      0x01,    
	arrange_mode:	0x01,
};
#endif
#pragma __PRQA_IGNORE_END__

/*
* internal function prototype
*/
static void  welcome_hw_init(void);
static void  welcome_update_data(disp_frame_info_t* frame_info_pt);


void power_on_hook(void);
/*static void delay()
{
    int tmp = 0x1ffff;
    while (tmp--);
}*/



/*!
* lcd initial, show logo picture.
*
* \par  Description:
*       when system power on, it will initial lcd then detect system state. \n
*       if system is lowpower, it will switch to standby; if detect play key, 
*          it will switch to ADFU, or system will start!
* \return       start system or switch to ADFU
* \retval       0 start system
* \retval       -1 switch to ADFU
*/
int  display_poweron_logo(welcome_cfg_t *welcome_p)
{    
    char flag=0;
    int result = 0;
    unsigned int batPower;
    unsigned int tmp=0;
    
//    tmp = act_readl(GPIO_AOUTEN);
//    tmp |=(0x1<<26);
//    act_writel(tmp,GPIO_AOUTEN);
//    
//    tmp = act_readl(GPIO_AINEN);
//    tmp &=(~(0x1<<26));
//    act_writel(tmp,GPIO_AINEN);
//    
//    tmp = act_readl(GPIO_ADAT);
//    tmp |=(0x1<<26);
//    act_writel(tmp,GPIO_ADAT);
//    
//    
//    tmp =10;
//    while( tmp>0)
//    {//for delay
//        tmp--;
//    }
//
//    tmp = act_readl(GPIO_AINEN);
//    tmp |=(0x1<<26);
//    act_writel(tmp,GPIO_AINEN);
//    
//    tmp = act_readl(GPIO_AOUTEN);
//    tmp &=(~(0x1<<26));
//    act_writel(tmp,GPIO_AOUTEN);
    
    

    /*软开关*/
    act_writel(act_readl(GPIO_AOUTEN)|(0x01<<12), GPIO_AOUTEN);
    act_writel(act_readl(GPIO_ADAT)|(0x01<<12), GPIO_ADAT);


    act_writel(act_readl(PMU_CTL) |(0x01<<31), PMU_CTL);
    act_writel((act_readl(PMU_CHG) &0xfffffffc) | 0x02, PMU_CHG);
    
    act_writel(( act_readl(GPIO_MFCTL0) |( 0x1<<15) ) & (~(0x1<<14) ), GPIO_MFCTL0);  //set ceb3 to yuv2rgb cs pin
    
    power_on_hook();

    if((is_usb_connected((char)(welcome_p->ic_version)) != 0) || (((act_readl(PMU_LRADC)>>31)&0x1)==1))      
    {
        testusb:
        batPower = 1;
        act_writel(act_readl(PMU_CTL) & 0x7fffffff, PMU_CTL);
    }  
    else
    {
        batPower = (act_readl(PMU_LRADC)>>16) & 0x3f;/*0x20---3.3v,????*/ 
        if(batPower <= 0x20)
        {
            goto standby;
        }
        if(batPower > 0x22)
        {
            batPower = 1;
        }
        else
        {
            batPower = 0;
        }
    }
    /*------lcd init and clear screen */
    welcome_hw_init();

    lcd_module_init();
//    act213xfb_set_window_mod(&full_screen_info, 0);
//    clear_full_screen();

   //  对传入的背光参数进行检测，如果非法，设置为默认；
    if ((welcome_p->light_level <= 0) || (welcome_p->light_level >30))
    {
        welcome_p->light_level = 24;
    }



/*----show start logo */
    if(batPower !=0 )
    {
//        act213xfb_set_window_mod(&start_up_picture_info, 0);
        start_up_picture_info.start_addr = (void *)((unsigned int)welcome_p->boot_pic_addr + 32);
        welcome_update_data(&start_up_picture_info);

    }
    else
    {
 //       act213xfb_set_window_mod(&lowpower_picture_info, 0);
        lowpower_picture_info.start_addr = (void *)((unsigned int)welcome_p->low_power_pic_addr + 32);
        welcome_update_data(&lowpower_picture_info);
    }

    lcd_mdelay(100);
/*----set default backlight level 3	*/
    welcome_set_back_light(welcome_p->light_level);
 //   act_writel(( act_readl(GPIO_MFCTL0) |( 0x1<<15) ) & (~(0x1<<14) ), GPIO_MFCTL0);  //set ceb3 to yuv2rgb cs pin

    if(!batPower)
    {
        int i;
        for(i=0;i<0x100000;i++)
        {
        }
        welcome_set_back_light(0);
        lcd_power_off();
        standby:
        if(is_usb_connected((char)(welcome_p->ic_version)) != 0)  
        {
            goto testusb;
        }
        act_buttons_init();
        if(welcome_p->ic_version=='E')
        {
            flag=1;
        }
        lcd_memcpy((void *)0xb4049000,(void *)0xc4000000,0x3000);//长度根据standby1.bin的长度
        ((int (*)(void))0xb4049000)();
    }

/*  check if press button to enter adfu */
    result = brec_switch_adfu();

    return result;
//    return 0;
//    return 0;
}


/*!
* lcd hardware initial.
*
* \par  Description:
*       initial lcd module's clock. \n
* \return       none
*/
static void  welcome_hw_init(void)
{
    unsigned int value;

    /* set cmu dev clk, enable related bit */
    value = act_readl(CMU_DEVCLKEN);
    value = value | (0x1<<1);// | (0x1<<17);
    act_writel(value, CMU_DEVCLKEN);

    /* set cmu dma clk */
    value = act_readl(CMU_DMACLK);
    value |= 0x3;
    act_writel(value, CMU_DMACLK);
	
#ifdef BL_PWM 
    act_writel(act_readl(CMU_FMCLK) | 0x1 << 5, CMU_FMCLK);
    act_writel(act_readl(CMU_FMCLK) | (0x1 << 4), CMU_FMCLK);        //750khz   pwm
    act_writel(act_readl(CMU_FMCLK) | (0x2 << 2), CMU_FMCLK);
//	act_writel(act_readl(CMU_FMCLK) & ~(0x1 << 4), CMU_FMCLK);    //32khz     pwm
//	act_writel(act_readl(CMU_FMCLK) & ~(0x3 << 2), CMU_FMCLK);
#endif

    set_yuv2rgb_clk(60);	
    
    value = act_readl(GPIO_MFCTL1) | 0x80000000;
    value &= ~(0x1 << 13);
    act_writel(value, GPIO_MFCTL1);
    //act_writel(0x80000000, GPIO_MFCTL1);
    return;
}

void lcd_set_gram_para(disp_frame_info_t* frame_info_pt);
//void (*p_set_window)(disp_frame_info_t* frame_info);
/*!
* update a full screen data.
*
* \par  Description:
*       use AHB bus to update a full screen data to display.
* \return       none
*/
static void  welcome_update_data(disp_frame_info_t* frame_info_pt)
{
    int piexl_cnt ;
    int i;
    unsigned int merge_two_short_piexl;
    unsigned short *parray;

    parray = (unsigned short *)frame_info_pt->start_addr;
    piexl_cnt = frame_info_pt->width * frame_info_pt->height;

    lcd_get_dev( );
    
    /*  set window mode  */
    lcd_set_gram_para(frame_info_pt);

    lcd_wait_fifo_empty();
    
    lcd_prepare_for_colordata();
    //	act_writel(0x000a0aae, YUV2RGB_CTL);
    act_writel( (Y2R_Ctrl_CMD | (0x1<<17)) | (RGB_DATA_WIDTH_18<<11), YUV2RGB_CTL);	/* ahb bus,RGB transfer */
    lcd_udelay(1);
    lcd_wait_fifo_empty();
    act_writel(0x00000408, YUV2RGB_CLKCTL);
    lcd_udelay(1);

    lcd_wait_fifo_empty();
    piexl_cnt = piexl_cnt/2;
    for (i=0; i<piexl_cnt; i++)
    {
        merge_two_short_piexl = (unsigned int) ( parray[2*i] | ( (unsigned int)parray[(2*i)+1] << 16));
        act_writel(merge_two_short_piexl, YUV2RGB_FIFODATA);

        /* if lcd_udelay is not used, the soft lock kernel bug will up. */
        lcd_wait_fifo_empty();
    }

    lcd_release_dev( );
    return;
}


/*!
* memory copy.
*
* \par  Description:
*       copy requested bytes from source to destination.
* \param [in]   dest address of destination data
* \param [in]   src address of source data
* \param [in]   count number want to copied
* \return       address of destination
* \retval       address
*/
void * lcd_memcpy(void * dest,const void *src,unsigned int count)
{
    char *tmp = (char *) dest, *s = (char *) src;
    while (count != 0)
    {
        count--;
        *tmp = *s;
        tmp++;
        s++;
    }
    return dest;
}

