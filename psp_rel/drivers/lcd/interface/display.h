

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
* \brief    ��ʾ�豸������ͷ�ļ�����ʾ�豸���� tv��cpu lcd, rgb lcd;
* \author   xkping
* \version 1.0
* \date  2008/08/28
*******************************************************************************/

#ifndef __DISPLAY_H__
#define __DISPLAY_H__

//for commom
#define DISPGET_DISP_ATTR        0x4702   /*��ȡ��ǰ��ʾ�豸����*/
#define DISPSET_UPDATE           0x4705   /*��������*/

//for lcd
#define DISPSET_LCD_SCREEN_MODE  0x4710  /*����LCD����ģʽ������ �� ����*/
#define DISPSET_LCD_BACKLIGHT    0x4711  /*����LCD����ȼ�*/
#define DISPGET_LCD_BACKLIGHT    0x4712  /*��ȡLCD����ȼ�*/
#define DISPSET_LCD_STANDBY      0x4713  /*����LCD����/�˳�standby*/

//for tvout
#define DISPSET_TV_MODE          0X4720  /*���õ�����ʽ��PAL 0x1, NTSC:0x10*/ 
#define DISPGET_TV_MODE          0X4721  /*��ȡ������ʽ��PAL 0x1, NTSC:0x10**/



/*!
 *  \brief
 *  ����ģʽ
 *  \li SIDELONG_MODE : ����(������ΪGUI��X����)  
 *  \li UPRIGHT_MODE : ����(�̱���ΪGUI��X����) 
 */ 
enum SCREEN_MODE
{  
    FB_SIDELONG_MODE =0x1,
    FB_UPRIGHT_MODE =0x2
};

/*!
 *  \brief
 *  ������˳�standby״̬ѡ��
 *  \li ENTER : ������standby״̬  
 *  \li EXIT : ���˳�standby״̬��������������ģʽ 
 */ 
enum STANDBY_CMD
{  
    ENTER =0x1,
    EXIT =0x2
};


/*!
 *  \brief
 *  ������ʽ
 *  \li PAL_MODE :  PAL ��ʽ 
 *  \li NTSC_MODE : NTSC��ʽ 
 */ 
enum TV_MODE
{  
    PAL_MODE =0x01,
    NTSC_MODE =0x10
};

/*!
 *  \brief
 * ���ݸ�ʽ
 *  \li RGB565 :  RGB��ʽ�� ���������� R 5bit��G 6bit��B 5bit
 *  \li YUV_422 : yuv 422��׼��ʽ������������ yuyv
 *  \li YUV_4Y4U : YUV422 actions��ʽ������������4y4u4y4v
 *  \li ����Ԥ��
 */ 
enum FORMAT_TYPE
{  
    RGB_565 =0x01,
    YUV_422 =0x40,
    YUV_4Y4U =0x80
};

/*!
 *  \brief
 * �������з�ʽ
 *  \li ROW_FIRST:    �����������ȷ�ʽ����
 *  \li COLUMN_FIRST: �����������ȷ�ʽ����
 *  \li ����Ԥ��
 */ 
enum ARRANGE_TYPE
{  
    ROW_FIRST =0x01,
    COLUMN_FIRST = 0x80
};

/*!
 * \brief  
 *      disp_frame_info_t,ˢ��֡������ .
 */
typedef struct  
{
    /*! ���ݴ�ŵ���ʼ�����ַ��physical address that dma will use*/
    void *start_addr;   
    
    /*! ��ʼλ������X������ֵ������ԭ��Ϊ���Ͻǣ���GUI/QT��ͬ��*/
    int x;
    
    /*! ��ʼλ������Y������ֵ������ԭ��Ϊ���Ͻǣ���GUI/QT��ͬ��*/
    int y;
    
    /*! ˢ��֡�Ŀ�*/
    int width;
    
    /*! ˢ��֡�ĸ�*/
    int height;
    
    /*! FORMAT_TYPE RGB565: RGB��ʽ������������ R 5bit��G 6bit��B 5bit;
               YUV422 :  YUV422��ʽ������������4y4u4y4v*/
    int format; 

    /*! arrange_mode ARRANGE_TYPE �������з�ʽ��ROW_FIRST:�����������ȷ�ʽ����;
                                               COLUMN_FIRST: �����������ȷ�ʽ����*/
    int arrange_mode; 
}disp_frame_info_t;



/*!
 * \brief  
 *      disp_dev_t,��ʾ�豸���� .
 */
typedef struct  
{
    /*!�豸֧�������ŷŷ�ʽ*/
    int support_arrange_mode;
                /*bit 31-------8  7  6  5  4  3  2  1  0*/
                /*      -- -----  X                    X*/
                /*֧�ֵ��������ͣ��� 0 bit ==1: ֧��ROW_FIRST   ���з�ʽ; 
                                  �� 7 bit ==1��֧��COLUMN_FIRST���з�ʽ
                                  ��������Ԥ��**/  
                        
    /*!�豸��ʾ֧�ֵ���������*/
    int support_format;
                /*bit 31-------8  7  6  5  4  3  2  1  0*/
                /*      -- -----  X                    X*/
                /*֧�ֵ��������ͣ��� 0 bit ==1: ֧��RGB565��ʽ; 
                                  �� 6 bit ==1��֧��YUV422,yuyv��ʽ,
                                  �� 7 bit ==1��֧��YUV4224y4u4y4v,
                                  ��������Ԥ��**/    
    /*!�豸�Ƿ�֧�ַֿ���ʾ*/
    int support_partial_update; 
                /*�豸�Ƿ�֧�ַֿ���ʾ��0����֧�֣� 1,֧��*/
    
    /*!�豸֧�ֵı��� xres*/                
    int xres;           /* visible resolution       */
    
    /*!�豸֧�ֵı��� yres*/
    int yres;

}disp_dev_t;


#endif /*end of file */

