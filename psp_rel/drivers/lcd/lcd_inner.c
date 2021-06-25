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
* \file Fb_drv.c
* \brief framebuffer driver for usdk130
* \author shaofeng
* \par GENERAL DESCRIPTION:
*       This file provide APIs to manipulate LCD screen. \n
*       
* \par DECLARATIONS:
*       (c) Copyright 2002-2004, Actions Co,Ld. All Right Reserved.
*
* \version 1.0
* \date 2007/11/25
*/ 

#include "lcd_inner.h"
#include "lcd_yuv2rgb.h"
#include "pm.h"
#pragma __PRQA_IGNORE_START__
/*!
* LCD physical attribute
*
* typedef struct
* {	
*	int support_arrange_mode;
*	int support_format;
*	int support_partial_update;
*	int xres;
*	int yres;
* } lcd_para_t;
*/
disp_dev_t  act_lcd_para = 
{
	support_arrange_mode: 0x81,/*第 0bit =1,支持ROW_FIRST; 第 7 bit =1：支持COLUMN_FIRST*/
	support_format: 0x81,   /*第 0 bit =1: 支持RGB565; 第 7 bit =1：支持YUV422*/
	support_partial_update: 0x1,/**设备是否支持分块显示：0，不支持； 1,支持*/
	support_rotate :0x01,
	current_direction : ROTATION_0,
#ifdef  GUI_UPRIGHT
	xres: S_XRES,
	yres: S_YRES,
#else	    
	xres: H_XRES,
	yres: H_YRES,
#endif	    
};

/*!
* Used to store window information for "act213xfb_set_window_mod"
*
* typedef struct frame_info
* {
*	unsigned long start_addr;	// physical address that dma will use;
*	int length;
*	int x;
*	int y;
*	int width;
*	int height;
*	int color_mod;	// set pixel color format; color_mod == 0 is rgb, color_mod == 1 is yuv;
* } disp_frame_info_t;
*/
window_offset_t   window_info = {
	horizontal_begin: 0,
	horizontal_end:    0,
	vertical_begin: 0,
	vertical_end:    0,
	horizontal_start:   0,
	vertical_start:   0,
};
#pragma __PRQA_IGNORE_END__

/*调频锁，调频期间禁止刷屏操作*/
static int lcd_clk_lock = 0;

/*!
* dma special channel number
*/
static unsigned int dma_sp_no = 0xffff;

int (*p_request_clkadjust)(unsigned int clk, void (*handler)\
         (unsigned int clk, curclk_t *curclk, void *dev_id, unsigned int cmd, unsigned int direction), void *dev_id);

int (*p_free_clkadjust)(unsigned int clk, void *dev_id);

int (*p_get_clock)(unsigned int cmd, unsigned int *result);

static int _check_para(disp_frame_info_t* frame_info)
{
    if ( ( (frame_info->x + frame_info->width) > act_lcd_para.xres )\
        ||( (frame_info->y + frame_info->height ) > act_lcd_para.yres )\
        ||(frame_info->x < 0) || (frame_info->y < 0) || (frame_info->start_addr == NULL))
    {
        return -1;
    }
    else
    {    
        return 0;
    }
}



/*!
* init DMA register
*
* \par  Description:
*       This function request the special DAM channel, the initial it.
* \return       none
*/
static int  _lcd_dma_init(void)
{
    dma_sp_no = (unsigned int) request_act213x_dma(DMA_CHAN_TYPE_SPECIAL, "fb", 
                (irqreturn_t (*)( int, void *,struct pt_regs *))lcd_dma_irq_handle, SA_SHIRQ, (void *)0);
    if ((dma_sp_no<4)||(dma_sp_no>7))
    {
        printf("request_act213x_dma failed!\n");
        return  -1;
    }
    /* init sp bus dma */
    reset_dma(dma_sp_no);	
    lcd_udelay(1);
    enable_dma_tcirq(dma_sp_no);
    //disable_dma_tcirq(dma_sp_no);
    disable_dma_htcirq(dma_sp_no);
    clear_dma_tcirq_pend(dma_sp_no);
    clear_dma_htcirq_pend(dma_sp_no);
    return 0;
}


void static _lcd_reset(void)
{
    unsigned int value;    
#ifdef   GPIO_A_RESET
    value = act_readl(GPIO_AOUTEN);     //GPIO_A16 as lcd reset pin, 0926,by cccw
    value |= 0x1 << RESET_PIN_NUM;
    act_writel(value, GPIO_AOUTEN);
    value = act_readl(GPIO_ADAT);
    value |= 0x1 << RESET_PIN_NUM;
    act_writel(value, GPIO_ADAT);
    lcd_mdelay(1);
    value &=(~(0x1 << RESET_PIN_NUM));
    act_writel(value, GPIO_ADAT);
    lcd_mdelay(3);
    value |= 0x1 << RESET_PIN_NUM;
    act_writel(value, GPIO_ADAT);
    lcd_mdelay(5);
#endif

#ifdef   GPIO_B_RESET
    value = act_readl(GPIO_BOUTEN);     //GPIO_B16 as lcd reset pin, 0926,by cccw
    value |= 0x1 << RESET_PIN_NUM;
    act_writel(value, GPIO_BOUTEN);
    value = act_readl(GPIO_BDAT);
    value |= 0x1 << RESET_PIN_NUM;
    act_writel(value, GPIO_BDAT);
    lcd_mdelay(1);
    value &=(~(0x1 << RESET_PIN_NUM));
    act_writel(value, GPIO_BDAT);
    lcd_mdelay(3);
    value |= 0x1 << RESET_PIN_NUM;
    act_writel(value, GPIO_BDAT);
    lcd_mdelay(5);
#endif    
    return;
}

/*return value: 1---busy, 0---idle*/
int lcd_dma_status(void)
{
    return dma_started(dma_sp_no);
}
/*!
* cycle query the dma whether be busy
*
* \par  Description:
*       This function query the dma whether busy, and will query the DAM state 10000 times, 
*           if always busy, return -1, or return 0.
* \return       whether the dma busy
* \retval       1 busy
* \retval       0 idle
*/
static  int _cycle_query_dma(void)
{
    int time;
    int ret;
    time = 50000;

    do 
    {
        time--;
        if (time < 0)
        {
            printf("lcm dbg: dma always busy! something is error!\n");
            return -1;
        }
//        ret = query_act213xdma_busy();
        ret = dma_started(dma_sp_no);
    } while (ret != 0);

    return 0;
}


int lcd_set_gram_para(disp_frame_info_t* frame_info_pt)
{    
    switch (act_lcd_para. current_direction)
    {
    case ROTATION_0:
        lcd_update_direction_0();    
        lcd_update_window_0(frame_info_pt);             
        break;        
    case ROTATION_90:
        lcd_update_direction_90();    
        lcd_update_window_90(frame_info_pt);             
        break;       
     case ROTATION_180:
        lcd_update_direction_180();    
        lcd_update_window_180(frame_info_pt);             
        break;      
     case ROTATION_270:
        lcd_update_direction_270();   
        lcd_update_window_270(frame_info_pt);              
        break;                  
    default:
        return -1;
    }	
    lcd_set_window_area( );      
    return 0;      
}
    
int lcd_set_gram_para_video(disp_frame_info_t* frame_info_pt)
{    
    switch (act_lcd_para. current_direction)
    {
    case ROTATION_0:
        lcd_update_direction_270();    
        lcd_update_window_0(frame_info_pt);             
        break;          
     case ROTATION_180:
        lcd_update_direction_90();    
        lcd_update_window_180(frame_info_pt);             
        break;       
    default:
        return -1;
    }	
    window_info.vertical_start   = (window_info.vertical_begin+window_info.vertical_end) - window_info.vertical_start;
    lcd_set_window_area( );      
    return 0;      
}


/*!
* DMA interrupt process function
*
* \par  Description:
*       This function process the DAM interrupt, it's a parameter of the request_act213x_dma().
* \param [in]   irq
* \param [in]   dev_id
* \return       none
*/
// (*irqhandler)(int, void *, struct pt_regs *)
irqreturn_t lcd_dma_irq_handle(int irq, void *dev_id, struct pt_regs * p)
{
    if (get_dma_tcirq_pend(dma_sp_no) != 0)
    {
        clear_dma_tcirq_pend(dma_sp_no);
        lcd_release_dev();
    }
    return 0;
}



/*!
* initial lcd register
*
* \par  Description:
*       This function first reset lcd, then initial lcd register to make it work. \n
* \return       none
*/
int  lcd_module_init(void)
{
    _lcd_reset();

    if(lcd_get_dev() !=0)
    {
        return -1;
    }    

    act_writel(Y2R_Ctrl_CMD, YUV2RGB_CTL);   
    lcd_wait_fifo_empty();

    lcd_hw_init();

    lcd_release_dev(); 
    lcd_mdelay(1);  /* to insure  lcd be stable*/
    return 0;
}


/*!
* write lcd register to shut down lcd
*
* \par  Description:
*       This function will power off the lcd. \n
* \return       none
*/
int  lcd_power_off(void)
{

    if(lcd_get_dev() != 0)
    {
        return -1;
    }
        
    lcd_wait_fifo_empty();
    act_writel(Y2R_Ctrl_CMD, YUV2RGB_CTL);
    lcd_udelay(1);

    /* standby, display off */
    lcd_hw_pow_off( );
    /* End of Standby Mode */

    lcd_release_dev();		
    return 0;
}

int  lcd_exit_standby(void)  
{
    return lcd_module_init();
}
                              







/*!
* update one frame of  displaying data
*
* \par  Description:
*       This function will update video data to display. \n
*       and be called by ioctl(fd, FBIOSET_UPDATE_VIDEO, &start_addr).
* param [in]    video frame's start address
* \return       whether update video data successfully
* \retval       0 success
* \retval       -1 fail
*/
int lcd_frame_update(disp_frame_info_t* frame_info_pt)
{
    int format ;
    int arrange_mode;
    unsigned int frame_count;
    OS_INT_SAVE_AREA
    
    if (lcd_clk_lock !=  0 )
    {    
        return -1;     /*调频期间禁止刷屏操作*/
    }        
    
    if(_check_para(frame_info_pt) != 0 )  
    {    
        return -1;     
    }
      
    format = frame_info_pt ->format;
    arrange_mode = frame_info_pt ->arrange_mode;
                  
    if (_cycle_query_dma() != 0)
    {
        return -1;
    }
    if (frame_info_pt->height== 0)
    {
        return 0;
    }

    if(lcd_get_dev() !=0)
    {
        return -1;
    }    		

    OS_ENTER_CRITICAL();
    
    act_writel(Y2R_Ctrl_CMD, YUV2RGB_CTL);
    lcd_udelay(2);
#if 1    
    if (arrange_mode ==0x1)
    {
        lcd_set_gram_para(frame_info_pt);
    }
    else if (arrange_mode ==0x80)
    {
        lcd_set_gram_para_video(frame_info_pt);
    }
    else   /* arrange_mode not support*/
    {
        OS_EXIT_CRITICAL();
        lcd_release_dev();               
        return -1;
    }	    	
#else
    lcd_set_gram_para(frame_info_pt);	    
#endif    
//     
    lcd_wait_fifo_empty();			
    lcd_prepare_for_colordata();

    if (format == 0x80)
    {
       /*为避免播放视频时画面上下抖动，必须在刷yuv格式数据时写寄存器YUV2RGB_FRAMECOUNT*/ 
        frame_count = (unsigned int)(((frame_info_pt->width & 0x1ff) << 8 )|(frame_info_pt->height & 0xff));
        act_writel(frame_count,YUV2RGB_FRAMECOUNT);
        act_writel((Y2R_Ctrl_CMD&(~(0x1<<9)) ) | (0x3<<16) | (RGB_DATA_WIDTH_18<<11),
                   YUV2RGB_CTL);/*SP bus,YUV transfer*/
    }
    else if (format ==0x01)
    {
        act_writel((Y2R_Ctrl_CMD&(~(0x1<<9)) ) | (0x1<<17) | (RGB_DATA_WIDTH_18<<11), 
                   YUV2RGB_CTL);	/*SP bus, RGB transfer*/
    }
    else   /* format not support*/
    {
        OS_EXIT_CRITICAL();
        lcd_release_dev(); 
        return -1;
    }
          /*lcd_wait_fifo_empty(); */

    /* because in the lcd_get_dev function the wait_dma_finish function
    currently not add quiting operation when timeout, so we should be sure
    that when use this func dma is not busy already! */
    reset_dma(dma_sp_no);
    lcd_udelay(1);
    clear_dma_tcirq_pend(dma_sp_no);
    clear_dma_htcirq_pend(dma_sp_no);


    set_dma_src_addr(dma_sp_no, ((unsigned int)(frame_info_pt->start_addr))&0x1fffffff );
    
    /* 0xa1c4a084 DMA mode-Sdram burst=8
    0x01c46084 DMA mode-Sdram burst=4
    0x01c40084 DMA mode-Sdram single  */
    
     /*对于不是32B对齐的地址，采取single模式传输。强烈建议使用32B对齐的地址*/	  
    if ( (((unsigned int)(frame_info_pt->start_addr)) & 0x1f) != 0)
    {
        set_dma_mode(dma_sp_no, 0x01c40084);
    }    	
    else
    {
        set_dma_mode(dma_sp_no, 0x01c4a084);
    }
        
    set_dma_dst_addr(dma_sp_no, 0x100f0004);
    set_dma_count(dma_sp_no, (unsigned int)(frame_info_pt->width * frame_info_pt->height * 2));

    start_dma(dma_sp_no);				//DMA Start

    OS_EXIT_CRITICAL();
    return 0;
};




/*!
* set window mode 
*
* \par  Description:
*       This function set the window coordinate and lcd increment direction.
* \param [in]   type the windows mode
* \return       whether set window mode successfully
* \retval       0 success
* \retval       -1 fail
*/
int lcd_rotate(unsigned int type)
{	   
    if( ( type == ROTATION_0 )||( type == ROTATION_180 ))
    {
        act_lcd_para.xres = H_XRES;
        act_lcd_para.yres = H_YRES;    
          
    }
    else if ( ( type == ROTATION_90 )||( type == ROTATION_270 ))   
    {
        act_lcd_para.xres = S_XRES;
        act_lcd_para.yres = S_YRES;        
       
    }
    else
    {
        return -1;
    }      

//   screen_direction =  type;   
    act_lcd_para. current_direction= (int)type;
    return 0;

}


/*!
* initialize frambuffer information and DMA
*
* \par  Description:
*       This function is the Initialization function. \n
* \return       always return 0
*/
int  lcd_dev_init(void)
{
    int ahb_clk;
    saved_backlight_level = backlight_level_map[DEFAUT_BACKLIGHT_LEVEL];
    
    if( _lcd_dma_init() != 0)
    {
        return -1;
    }
    
    if( backlight_level_init() != 0)
    {
        return -1;
    }
    p_request_clkadjust = kernel_sym( "request_clkadjust" );
    p_free_clkadjust = kernel_sym( "free_clkadjust" ); 
    p_get_clock = kernel_sym( "get_clock" ); 
    
    if(p_request_clkadjust !=NULL)  
    {
        p_request_clkadjust(CLK_YVU, lcd_clk_cb, "lcd_clk");
    }
    
    if(p_get_clock !=NULL)  
    {
        p_get_clock(GET_SCLK, &ahb_clk);
        set_yuv2rgb_clk(ahb_clk);            
    }    
    
    
    return 0;
}


/*!
* initialize frambuffer information and DMA
*
* \par  Description:
*       This function is the Initialization function. \n
* \return       always return 0
*/
int  lcd_dev_exit(void)
{
    free_act213x_dma( dma_sp_no );
    if(p_free_clkadjust != NULL)
    {
        p_free_clkadjust(CLK_YVU,"lcd_clk");
    }
    return 0;
}


void lcd_clk_cb(unsigned int clk, curclk_t *curclk, void *dev_id, unsigned int cmd, unsigned int direction)
{    
    if(cmd == CHANGEPLL_END)
    {
        set_yuv2rgb_clk(curclk->corepll / (curclk->cclk_div  * curclk->sclk_div));    
        lcd_clk_lock = 0;
    }
    else if(cmd == CHANGEPLL_START)
    {
        lcd_clk_lock = 1;
        _cycle_query_dma();
    }
    else
    {       
    }
    
    return ;
}


/*!
* us delay
*
* \par  Description:
*       This function delay, and the unit is us.
* \param [in]   tick the number of delay us.
* \return       none
*/
void lcd_udelay(unsigned int tick)
{
    unsigned int i = tick;
    while (i != 0)
    {
        i--;
    }
}


/*!
* ms delay
*
* \par  Description:
*       This function delay, and the unit is ms.
* \param [in]   tick the number of delay ms.
* \return       none
*/


#ifdef _IN_WELCOME_
void lcd_mdelay(unsigned int tick)
{
    unsigned int i = tick;
    while( i-- ) 
        lcd_udelay(645);	
        
}

#else

void lcd_mdelay( unsigned int tick)
{
   
    tick = tick / 10;
    if (tick == 0)
    {
        tick = 1;
    }
    os_time_dly(tick);	
    
}
#endif
