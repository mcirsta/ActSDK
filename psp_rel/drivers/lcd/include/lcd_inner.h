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
* \file Fb_drv.h
* \brief general LCM Driver information
* \author shaofeng
* \par GENERAL DESCRIPTION:
*       This file provide the basic type and macro. \n
*       
* \par DECLARATIONS:
*       (c) Copyright 2002-2004, Actions Co,Ld. All Right Reserved.
*
* \version 1.0
* \date 2007/11/25
*/


#include "kmod_calls.h"
#include "lcd_yuv2rgb.h"
#include "display.h"
#include "lcd_config.h"
#include "pm.h"
#ifndef _LCD_INNER_H_
#define _LCD_INNER_H_

/*! \cond LCM_API*/


/*   in boot process, some system service are not available  */
#ifdef _IN_WELCOME_

#undef       OS_INT_SAVE_AREA
#undef       OS_ENTER_CRITICAL()
#undef       OS_EXIT_CRITICAL()

#define      OS_INT_SAVE_AREA
#define 	OS_ENTER_CRITICAL()  do{}while(0)  
#define 	OS_EXIT_CRITICAL()  do{}while(0)  

#define multip_get_pinowner(x...)  1
#define multip_release_pinowner(x...)  

#define printf(x...) 

#define getCorePll()   120
//#define lcm_ready()	1
#define getICType()	0x63
#endif


/*!
* window infomation, will be writed to lcd register to set window dispalying in lcd.
*/
typedef struct window_offset 
{
    int horizontal_begin;
    int horizontal_end;
    int vertical_begin;
    int vertical_end;
    int horizontal_start;
    int vertical_start;	
} window_offset_t;


//extern unsigned int XRES;
//extern unsigned int YRES;
extern disp_dev_t  act_lcd_para;

/*!
the default backlight level is 3, defined in lcd_set_backlight.c
*/

extern unsigned int backlight_level_map[];
extern unsigned int saved_backlight_level;
/**
internal funcs
*/

void lcd_hw_init(void);
void lcd_set_window_area( void );
void lcd_hw_pow_off( void );
void lcd_reg_pow_on( void );

int lcd_set_back_light(unsigned int bl_level);
unsigned int lcd_get_back_light(void);

int  lcd_wait_fifo_empty(void);


int  lcd_get_dev(void);
int  lcd_release_dev(void);
void lcd_prepare_for_colordata(void);

int switch_window_mod(unsigned int type);

void  set_window_upright(disp_frame_info_t* frame_info);
void  set_window_video(disp_frame_info_t* frame_info) ;

void  set_increment_direction_sidelong(void);
void  set_increment_direction_upright(void);
void  set_increment_direction_video(void);
int	backlight_level_init(void);
int     lcd_rotate(unsigned int type);
int  lcd_module_init(void);

irqreturn_t lcd_dma_irq_handle(int irq, void *dev_id, struct pt_regs * p);
int act_lcd_ioctl( unsigned int cmd, unsigned long arg);

int lcd_frame_update(disp_frame_info_t* frame_info_pt);

int  lcd_power_off(void);
int  lcd_exit_standby( void );

int  lcd_dev_init(void);
int  lcd_dev_exit(void);

void lcd_udelay(unsigned int tick);
void lcd_mdelay(unsigned int tick);

void lcd_update_direction_0(void);    
void lcd_update_window_0(disp_frame_info_t* frame_info_pt);             
void lcd_update_direction_90(void);    
void lcd_update_window_90(disp_frame_info_t* frame_info_pt);             
void lcd_update_direction_180(void);    
void lcd_update_window_180(disp_frame_info_t* frame_info_pt);             
void lcd_update_direction_270(void);   
void lcd_update_window_270(disp_frame_info_t* frame_info_pt); 

void lcd_clk_cb(unsigned int clk, curclk_t *curclk, void *dev_id, unsigned int cmd, unsigned int direction);

/*! \endcond*/

#endif

