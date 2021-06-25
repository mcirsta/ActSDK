#include "lcd_module.h"

/*	change RS bit，for driver IC like ILI9320 etc..	*/   
/*!
* set YUV2RGB module to send command
*
* \par  Description:
*       This function set YUV2RGB module to send data, using to write lcd register address.
* \return       none
*/
static void  _prepare_for_cmd(void)         //ready to send register index
{
    lcd_wait_fifo_empty();
    lcd_udelay(1);
    act_writel(0x802ae, YUV2RGB_CTL);
    return;
}


/*!
* set YUV2RGB module to send register data
*
* \par  Description:
*       This function set YUV2RGB module to send command, using to write lcd register data.
* \return       none
*/
static void  _prepare_for_val(void)                    //ready to send register value
{
    lcd_wait_fifo_empty();
    lcd_udelay(1);
    act_writel(0x902ae, YUV2RGB_CTL);      //ahb ,write data(register value)
    return;
}



/*!
* set YUV2RGB module to send RGB or YUV data
*
* \par  Description:
*       This function set YUV2RGB module to send data, using to write lcd RGB or YUV data.
* \return       none
*/
void  lcd_prepare_for_colordata(void)
{
    _prepare_for_cmd();	                    /* Start send/read data. */
    act_writel(0x0202, YUV2RGB_FIFODATA);
    lcd_udelay(1);
    act_writel(0xa0aae, YUV2RGB_CTL);       /* ahb ,RGB data framebuffer transfer */
    return;	
}

static void  _write_lcd_reg(unsigned int reg_index,  unsigned int reg_val)                  
{
    _prepare_for_cmd();	// Start internal OSC.
    act_writel(reg_index, YUV2RGB_FIFODATA); 	
    lcd_udelay(1);
    _prepare_for_val();
    act_writel(reg_val, YUV2RGB_FIFODATA); 
    return;
}


void  lcd_hw_init( void)
{
    _write_lcd_reg(0x0606,  0x0000);     
    _write_lcd_reg(0x0007,  0x0001);   
    _write_lcd_reg(0x0110,  0x0001);       
    _write_lcd_reg(0x0100,  0x17B0);       
    _write_lcd_reg(0x0101,  0x0147);       
    _write_lcd_reg(0x0102,  0x019D);       
    _write_lcd_reg(0x0103,  0x3600);       
    _write_lcd_reg(0x0281,  0x0010);       
    _write_lcd_reg(0x0102,  0x01BD);       
    _write_lcd_reg(0x0000,  0x0000);       
    _write_lcd_reg(0x0001,  0x0000);       
    _write_lcd_reg(0x0002,  0x0100);       
    _write_lcd_reg(0x0003,  0x5030);       
    _write_lcd_reg(0x0006,  0x0000);       
    _write_lcd_reg(0x0008,  0x0808);      
    _write_lcd_reg(0x0009,  0x0001);       
    _write_lcd_reg(0x000B,  0x0010);     
    lcd_mdelay(100);							// Dis-charge capacitor power voltage  
    _write_lcd_reg(0x0010,  0x1290);       
    _write_lcd_reg(0x000C,  0x0000);  
    lcd_mdelay(20);         
    _write_lcd_reg(0x000F,  0x0000);         
    lcd_mdelay(20);   
    _write_lcd_reg(0x0007,  0x0001);       
    _write_lcd_reg(0x0010,  0x0013);       
    _write_lcd_reg(0x0011,  0x0202);   
    lcd_mdelay(20);   
    _write_lcd_reg(0x0012,  0x0300); 
    _write_lcd_reg(0x0020,  0x021E);       
    _write_lcd_reg(0x0021,  0x0202);       
    _write_lcd_reg(0x0022,  0x0100);       
    _write_lcd_reg(0x0090,  0x8000);     
    _write_lcd_reg(0x0092,  0x0000); 
    _write_lcd_reg(0x0100,  0x14B0);       
    _write_lcd_reg(0x0101,  0x0147);       
    _write_lcd_reg(0x0102,  0x01BD);       
    _write_lcd_reg(0x0103,  0x2D00); 
    _write_lcd_reg(0x0107,  0x0000); 
    _write_lcd_reg(0x0110,  0x0001);       
    _write_lcd_reg(0x0210,  0x0000);       
    _write_lcd_reg(0x0211,  0x00EF);       
    _write_lcd_reg(0x0212,  0x0000); 
    _write_lcd_reg(0x0213,  0x018F);       
    _write_lcd_reg(0x0280,  0x0000);       
    _write_lcd_reg(0x0281,  0x0004);       
    _write_lcd_reg(0x0282,  0x0000);     
    _write_lcd_reg(0x0300,  0x0101);       
    _write_lcd_reg(0x0301,  0x0025);       
    _write_lcd_reg(0x0302,  0x1328);       
    _write_lcd_reg(0x0303,  0x2813); 
    _write_lcd_reg(0x0304,  0x2500);       
    _write_lcd_reg(0x0305,  0x0100);       
    _write_lcd_reg(0x0306,  0x1B04); 
    _write_lcd_reg(0x0307,  0x041B);       
    _write_lcd_reg(0x0308,  0x0006);       
    _write_lcd_reg(0x0309,  0x0103); 
    _write_lcd_reg(0x030A,  0x0F04);       
    _write_lcd_reg(0x030B,  0x0F00);       
    _write_lcd_reg(0x030C,  0x000F);
    
    
    _write_lcd_reg(0x030D,  0x050F);  
    _write_lcd_reg(0x030E,  0x0301);
    _write_lcd_reg(0x030F,  0x0600);
    _write_lcd_reg(0x0400,  0x3500);  
    _write_lcd_reg(0x0401,  0x0001);  
    _write_lcd_reg(0x0404,  0x0000);  
    _write_lcd_reg(0x0500,  0x0000);  
    _write_lcd_reg(0x0501,  0x0000);  
    _write_lcd_reg(0x0502,  0x0000);  
    _write_lcd_reg(0x0503,  0x0000);  
    _write_lcd_reg(0x0504,  0x0000);  
    _write_lcd_reg(0x0505,  0x0000);  
    _write_lcd_reg(0x0600,  0x0000);  
    _write_lcd_reg(0x0606,  0x0000);  
    _write_lcd_reg(0x06F0,  0x0000);  
    _write_lcd_reg(0x07F0,  0x5420);  
    _write_lcd_reg(0x07F3,  0x288C);  
    _write_lcd_reg(0x07F4,  0x0022);  
    _write_lcd_reg(0x07F5,  0x0001);  
    _write_lcd_reg(0x0007,  0x0173);  

               

    lcd_mdelay(100);
    return;
}

void lcd_set_window_area( void )
{
    _write_lcd_reg(0x0210,  (unsigned int)window_info.horizontal_begin);       
    _write_lcd_reg(0x0211,  (unsigned int)window_info.horizontal_end);       
    _write_lcd_reg(0x0212,  (unsigned int)window_info.vertical_begin); 
    _write_lcd_reg(0x0213,  (unsigned int)window_info.vertical_end);    
          	
    _write_lcd_reg(0x0200,  (unsigned int)window_info.horizontal_start); 
    _write_lcd_reg(0x0201,  (unsigned int)window_info.vertical_start);          	
    return;
}

void lcd_hw_pow_off( void )
{
    _write_lcd_reg(0x0007,  0x0131);    
    lcd_mdelay(10);    
    _write_lcd_reg(0x0007,   0x0130);   
    lcd_mdelay(10);     
    _write_lcd_reg(0x0007,   0x0000);    
    _write_lcd_reg(0x0010,   0x0080);    
    _write_lcd_reg(0x0011,   0x0000);    
    _write_lcd_reg(0x0012,   0x0000);    
    _write_lcd_reg(0x0013,   0x0000);   
    lcd_mdelay(100);	     
    _write_lcd_reg(0x0010,   0x0081);    	
    return;			
}

void lcd_set_pow_on( void )
{
// need not to be implemented now!
}

/*GRAM 原点在0度视角的 右下角落*/
void lcd_update_window_0(disp_frame_info_t* frame_info)
{
    window_info.horizontal_begin   = H_YRES - frame_info->y - frame_info->height ;
    window_info.horizontal_end      =  H_YRES - 1 - frame_info->y;
    window_info.vertical_begin        = H_XRES - frame_info->x - frame_info->width;
    window_info.vertical_end           = H_XRES - 1 - frame_info->x;
    window_info.horizontal_start    = window_info.horizontal_end;
    window_info.vertical_start         = window_info.vertical_end;	
    return;     
}

void lcd_update_window_90(disp_frame_info_t* frame_info)
{
    window_info.horizontal_begin   = frame_info->x;
    window_info.horizontal_end      = (frame_info->x + frame_info->width) - 1;
    window_info.vertical_begin        =  H_XRES - frame_info->y - frame_info->height;
    window_info.vertical_end           = H_XRES - 1 - frame_info->y;
    window_info.horizontal_start    = window_info.horizontal_begin;
    window_info.vertical_start         = window_info.vertical_end;
    return;    
}

void lcd_update_window_180(disp_frame_info_t* frame_info)
{
    window_info.horizontal_begin   = frame_info->y;
    window_info.horizontal_end      = (frame_info->y + frame_info->height) - 1;
    window_info.vertical_begin        = frame_info->x;
    window_info.vertical_end           =  (frame_info->x + frame_info->width) - 1;
    window_info.horizontal_start    = window_info.horizontal_begin;
    window_info.vertical_start         = window_info.vertical_begin;
    return;    
}

void lcd_update_window_270(disp_frame_info_t* frame_info)
{
    window_info.horizontal_begin  = H_YRES - frame_info->x - frame_info->width;
    window_info.horizontal_end     = H_YRES - 1 - frame_info->x;
    window_info.vertical_begin       = frame_info->y;
    window_info.vertical_end           =  (frame_info->y + frame_info->height) - 1;
    window_info.horizontal_start    = window_info.horizontal_end;
    window_info.vertical_start          = window_info.vertical_begin;
    return;
}
/*!
* transform the frame coordinate to windows coordinate
*
* \par  Description:
*       This function transform the frame: GUI is sidelong, and YUV data is upright, for scanmode：0x21. \n
*       after compute the window_info parameters, write them to lcd register. \n
*       the frame coordinate's origin is the left down of person's vision, \n
*       the windows coordinate's origin is fixed, please refer it's spec. \n
* \param [in]   frame_info the frame information which will be transformed
* \return       none
*/


void  lcd_update_direction_0(void)
{
    _write_lcd_reg(0x0003,  0x1008);  
    return;
}

void  lcd_update_direction_90(void)
{
    _write_lcd_reg(0x0003,  0x1010);  
    return;
}

void  lcd_update_direction_180(void)
{
    _write_lcd_reg(0x0003,  0x1038);  
    return;
}

void  lcd_update_direction_270(void)
{
    _write_lcd_reg(0x0003,  0x1020);  
    return;
}

