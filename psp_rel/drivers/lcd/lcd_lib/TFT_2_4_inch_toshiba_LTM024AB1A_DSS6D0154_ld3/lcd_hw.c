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
    act_writel(0x000802ae, YUV2RGB_CTL);
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
    act_writel(0x000902ae, YUV2RGB_CTL);      //ahb ,write data(register value)
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
    act_writel(0x22, YUV2RGB_FIFODATA);
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
    _write_lcd_reg(0x0080,  0x008d);     
    _write_lcd_reg(0x0092,  0x0010);   
    _write_lcd_reg(0x0011,  0x001a);       
    _write_lcd_reg(0x0012,  0x1212);       
    _write_lcd_reg(0x0013,  0x0069);       
    _write_lcd_reg(0x0014,  0x586b);       
    _write_lcd_reg(0x0010,  0x0400); 
    lcd_mdelay(20);   
    _write_lcd_reg(0x0011,  0x101a);
    lcd_mdelay(30);
    _write_lcd_reg(0x0081,  0x0000);    
    _write_lcd_reg(0x0001,  0x00000328);    
    _write_lcd_reg(0x0002,  0x0100);       
    _write_lcd_reg(0x0003,  0x00001010);  
    _write_lcd_reg(0x0007,  0x1004);       
    _write_lcd_reg(0x0008,  0x0503);       
    _write_lcd_reg(0x000b,  0x1100);       
    _write_lcd_reg(0x000c,  0x0000);      
    _write_lcd_reg(0x0015,  0x0000);       
    _write_lcd_reg(0x0029,  0x0001);        					 
    _write_lcd_reg(0x0030,  0x0000);       
    _write_lcd_reg(0x0036,  0x00ef);          
    _write_lcd_reg(0x0037,  0x0000);           
    _write_lcd_reg(0x0038,  0x013f);       
    _write_lcd_reg(0x0039,  0x0000);       
    _write_lcd_reg(0x0050,  0x0000);    
    _write_lcd_reg(0x0051,  0x0a01); 
    _write_lcd_reg(0x0052,  0x0c09);       
    _write_lcd_reg(0x0053,  0x0200);       
    _write_lcd_reg(0x0058,  0x1000);       
    _write_lcd_reg(0x0054,  0x090c);     
    _write_lcd_reg(0x0055,  0x010a); 
    _write_lcd_reg(0x0056,  0x0000);       
    _write_lcd_reg(0x0057,  0x0002);       
    _write_lcd_reg(0x0059,  0x0010);       
    _write_lcd_reg(0x0020,  0x0000); 
    _write_lcd_reg(0x0021,  0x0000); 
    lcd_mdelay(30); 
    _write_lcd_reg(0x0022,  0x0000);
    lcd_mdelay(30);        
    _write_lcd_reg(0x000f,  0x0a01);       
    _write_lcd_reg(0x0007,  0x1016);
    lcd_mdelay(30);        
    _write_lcd_reg(0x0007,  0x1017); 
        

    return;
}

void lcd_set_window_area( void )
{
    _write_lcd_reg(0x0037,  (unsigned int)window_info.horizontal_begin);       
    _write_lcd_reg(0x0036,  (unsigned int)window_info.horizontal_end);       
    _write_lcd_reg(0x0039,  (unsigned int)window_info.vertical_begin); 
    _write_lcd_reg(0x0038,  (unsigned int)window_info.vertical_end);    
          	
    _write_lcd_reg(0x0020,  (unsigned int)window_info.horizontal_start); 
    _write_lcd_reg(0x0021,  (unsigned int)window_info.vertical_start);          	
    return;
}

void lcd_hw_pow_off( void )
{
    _write_lcd_reg(0x0015,  0x0000);        
    _write_lcd_reg(0x0007,   0x1016);   
    lcd_mdelay(50);     
    _write_lcd_reg(0x0007,   0x0004); 
    lcd_mdelay(20);   
    _write_lcd_reg(0x0010,   0x0402);     
    lcd_mdelay(20);	     
   	
    return;			
}

void lcd_set_pow_on( void )
{
// need not to be implemented now!
}

/*GRAM 原点在0度视角的 右上角落*/
void lcd_update_window_0(disp_frame_info_t* frame_info)
{
    window_info.horizontal_begin   = frame_info->y;
    window_info.horizontal_end      =  (frame_info->y + frame_info->height )- 1;
    window_info.vertical_begin        = 320 - frame_info->x - frame_info->width;
    window_info.vertical_end           = 319 - frame_info->x;
    window_info.horizontal_start    = window_info.horizontal_begin;
    window_info.vertical_start         = window_info.vertical_end;	
    return;     
}

void lcd_update_window_90(disp_frame_info_t* frame_info)
{
    window_info.horizontal_begin   = 240 - frame_info->x - frame_info->width;
    window_info.horizontal_end      = 239 - frame_info->x;
    window_info.vertical_begin        =  320 - frame_info->y - frame_info->height;
    window_info.vertical_end           = 319 - frame_info->y;
    window_info.horizontal_start    = window_info.horizontal_end;
    window_info.vertical_start         = window_info.vertical_end;
    return;    
}

void lcd_update_window_180(disp_frame_info_t* frame_info)
{
    window_info.horizontal_begin   = 240 - frame_info->y - frame_info->height;
    window_info.horizontal_end      = 239 - frame_info->y;
    window_info.vertical_begin        = frame_info->x;
    window_info.vertical_end           =  (frame_info->x + frame_info->width) - 1;
    window_info.horizontal_start    = window_info.horizontal_end;
    window_info.vertical_start         = window_info.vertical_begin;
    return;    
}

void lcd_update_window_270(disp_frame_info_t* frame_info)
{
    window_info.horizontal_begin  = frame_info->x;
    window_info.horizontal_end     = (frame_info->x + frame_info->width) - 1;
    window_info.vertical_begin       = frame_info->y;
    window_info.vertical_end           =  (frame_info->y + frame_info->height) - 1;
    window_info.horizontal_start    = window_info.horizontal_begin;
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
    _write_lcd_reg(0x0003,  0x00001038);  
    _write_lcd_reg(0x0001,  0x0328);         
    return;
}

void  lcd_update_direction_90(void)
{
    _write_lcd_reg(0x0003,  0x00001020);  
    _write_lcd_reg(0x0001,  0x0328);         
    return;
}

void  lcd_update_direction_180(void)
{
    _write_lcd_reg(0x0003,  0x00001008);  
    _write_lcd_reg(0x0001,  0x0328);         
    return;
}

void  lcd_update_direction_270(void)
{
    _write_lcd_reg(0x0003,  0x00001010);  
    _write_lcd_reg(0x0001,  0x0328);         
    return;
}

