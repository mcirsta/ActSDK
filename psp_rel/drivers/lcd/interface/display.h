

/********************************************************************************
*                              NOYA
*                            Module: display driver
*                 Copyright(c) 2003-2008 Actions Semiconductor,
*                            All Rights Reserved. 
*
* History:         
*      <author>    <time>           <version >             <desc>
*       xkping     2008-08-28 10:00     1.0             build this file 
********************************************************************************/
/*!
* \file     display.h
* \brief    显示设备驱动的头文件，显示设备包括 tv，cpu lcd, rgb lcd;
* \author   xkping
* \version 1.0
* \date  2008/08/28
*******************************************************************************/

#ifndef __DISPLAY_H__
#define __DISPLAY_H__

//for commom
#define DISPGET_DISP_ATTR        0x4702   /*获取当前显示设备属性*/
#define DISPSET_UPDATE           0x4705   /*更新数据*/

//for lcd
#define DISPSET_LCD_SCREEN_MODE  0x4710  /*设置LCD的屏模式：横屏 或 竖屏*/
#define DISPSET_LCD_BACKLIGHT    0x4711  /*设置LCD背光等级*/
#define DISPGET_LCD_BACKLIGHT    0x4712  /*获取LCD背光等级*/
#define DISPSET_LCD_STANDBY      0x4713  /*设置LCD进入/退出standby*/

//for tvout
#define DISPSET_TV_MODE          0X4720  /*设置电视制式：PAL 0x1, NTSC:0x10*/ 
#define DISPGET_TV_MODE          0X4721  /*获取电视制式：PAL 0x1, NTSC:0x10**/



/*!
 *  \brief
 *  屏的模式
 *  \li SIDELONG_MODE : 横屏(长边作为GUI的X方向)  
 *  \li UPRIGHT_MODE : 竖屏(短边作为GUI的X方向) 
 */ 
enum SCREEN_MODE
{  
    FB_SIDELONG_MODE =0x1,
    FB_UPRIGHT_MODE =0x2
};

/*!
 *  \brief
 *  进入或退出standby状态选择
 *  \li ENTER : 屏进入standby状态  
 *  \li EXIT : 屏退出standby状态，进入正常工作模式 
 */ 
enum STANDBY_CMD
{  
    ENTER =0x1,
    EXIT =0x2
};


/*!
 *  \brief
 *  电视制式
 *  \li PAL_MODE :  PAL 制式 
 *  \li NTSC_MODE : NTSC制式 
 */ 
enum TV_MODE
{  
    PAL_MODE =0x01,
    NTSC_MODE =0x10
};

/*!
 *  \brief
 * 数据格式
 *  \li RGB565 :  RGB格式， 数据排列是 R 5bit，G 6bit，B 5bit
 *  \li YUV_422 : yuv 422标准格式，数据排列是 yuyv
 *  \li YUV_4Y4U : YUV422 actions格式，数据排列是4y4u4y4v
 *  \li 其他预留
 */ 
enum FORMAT_TYPE
{  
    RGB_565 =0x01,
    YUV_422 =0x40,
    YUV_4Y4U =0x80
};

/*!
 *  \brief
 * 数据排列方式
 *  \li ROW_FIRST:    数据以行优先方式排列
 *  \li COLUMN_FIRST: 数据以列优先方式排列
 *  \li 其他预留
 */ 
enum ARRANGE_TYPE
{  
    ROW_FIRST =0x01,
    COLUMN_FIRST = 0x80
};

/*!
 * \brief  
 *      disp_frame_info_t,刷新帧的属性 .
 */
typedef struct  
{
    /*! 数据存放的起始物理地址，physical address that dma will use*/
    void *start_addr;   
    
    /*! 起始位置坐标X轴坐标值，坐标原点为左上角（与GUI/QT相同）*/
    int x;
    
    /*! 起始位置坐标Y轴坐标值，坐标原点为左上角（与GUI/QT相同）*/
    int y;
    
    /*! 刷新帧的宽*/
    int width;
    
    /*! 刷新帧的高*/
    int height;
    
    /*! FORMAT_TYPE RGB565: RGB格式，数据排列是 R 5bit，G 6bit，B 5bit;
               YUV422 :  YUV422格式，数据排列是4y4u4y4v*/
    int format; 

    /*! arrange_mode ARRANGE_TYPE 数据排列方式，ROW_FIRST:数据以行优先方式排列;
                                               COLUMN_FIRST: 数据以列优先方式排列*/
    int arrange_mode; 
}disp_frame_info_t;



/*!
 * \brief  
 *      disp_dev_t,显示设备属性 .
 */
typedef struct  
{
    /*!设备支持数据排放方式*/
    int support_arrange_mode;
                /*bit 31-------8  7  6  5  4  3  2  1  0*/
                /*      -- -----  X                    X*/
                /*支持的数据类型：第 0 bit ==1: 支持ROW_FIRST   排列方式; 
                                  第 7 bit ==1：支持COLUMN_FIRST排列方式
                                  其他比特预留**/  
                        
    /*!设备显示支持的数据类型*/
    int support_format;
                /*bit 31-------8  7  6  5  4  3  2  1  0*/
                /*      -- -----  X                    X*/
                /*支持的数据类型：第 0 bit ==1: 支持RGB565格式; 
                                  第 6 bit ==1：支持YUV422,yuyv格式,
                                  第 7 bit ==1：支持YUV4224y4u4y4v,
                                  其他比特预留**/    
    /*!设备是否支持分块显示*/
    int support_partial_update; 
                /*设备是否支持分块显示：0，不支持； 1,支持*/
    
    /*!设备支持的辨率 xres*/                
    int xres;           /* visible resolution       */
    
    /*!设备支持的辨率 yres*/
    int yres;

}disp_dev_t;


#endif /*end of file */

