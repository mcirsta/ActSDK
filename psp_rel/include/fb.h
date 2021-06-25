/********************************************************************************
 *                              NOYA
 *                            Module: soft image layer
 *                 Copyright(c) 2003-2008 Actions Semiconductor,
 *                            All Rights Reserved.
 *
 * History:
 *      <author>    <time>           <version >             <desc>
 *       xkping     2008-09-10 15:00     1.0             build this file
 ********************************************************************************/
/*!
 * \file     fb.h
 * \brief    ��ͼ���ͷ�ļ�
 * \author   xkping
 * \version 1.0
 * \date  2008/9/10
 *******************************************************************************/

#ifndef __FB_H__
#define __FB_H__
#include "display.h"

/*! ����ϵͳ�������õ�blending����*/
#define MAX_BLENDING_AREA       0x08

/*! blend ����Ŀ�handleֵ*/
#define BLENDING_HANLE_NULL     -1

/*��ص����ݽṹ*/
/*!
 * \brief
 *      color_para_t, ��ɫ���� .
 */
typedef struct
{
    /*! ��ɫֵռ�õ�λ��*/
    int length;

    /*! ��ɫֵ��32λ�е�ƫ��*/
    int offset;
} fb_color_para_t;

/*FB�������Ϣ,ͨ��FBIOGET_FB_PARAM��ȡ,FB����ʼ��ַ*/
/*��linux����ͨ��mmapʵ��,��usdk��ͨ��FBIOGET_FB_ADDR��ȡ*/
/*!
 * \brief
 *      fb_info_t, framebuffer�������Ϣ .
 */
typedef struct
{
    /*! framebuffer X����ֱ���*/
    int xres;

    /*! framebuffer Y����ֱ���*/
    int yres;

    /*! ÿ�����ص���ռ��bit��*/
    int bits_per_pixel;

    /*! ÿ�еĳ���*/
    int line_length;

    /*! RGB��ԭɫ��R������*/
    fb_color_para_t red;

    /*! RGB��ԭɫ��G������*/
    fb_color_para_t green;

    /*! RGB��ԭɫ��B������*/
    fb_color_para_t blue;
} fb_info_t;

/*!
 * \brief
 *      rect_area_t,��Ƶ��ʾ��������,����һ���������� .
 */
typedef struct
{
    /*! ����֮x����*/
    int x;
    /*! ����֮y����*/
    int y;
    /*! ����������*/
    int w;
    /*! ��������߶�*/
    int h;
} fb_rect_area_t;

/*!
 * \brief
 *      blending_param_t, ���ݻ�ϲ���.
 */
typedef struct
{
    /*! �˵����ݵĹؼ�colorֵ*/
    unsigned int color_key;
    /*! ���ݻ�ϵ��ӵı���ֵ, ȡֵ��ΧΪ(0-255)*/
    unsigned int alpha;
    /*! ���ݻ�ϵ��ӵ�����*/
    fb_rect_area_t area;
    /*! blending������Ҫ���ӵ�����buffer��ַ*/
    unsigned char *blending_buf;
} fb_blending_param_t;

/*!
 * \brief
 *      FBIOSET_BLENDING_AREA_CREATE�Ĳ������ͣ�����һ�������fb_blending_param_t���Ժ�һ�����ص�handleֵ��
 * */
typedef struct
{
    /*! ����blend����󷵻ص�handleֵ����ֵΪFBIOSET_BLENDING_AREA_CREATE�ӿڵ����ֵ*/
    int handle;
    /*! �������Ҫ������blend���������*/
    fb_blending_param_t blend_param;
} fb_blending_create_t;

/*!
 *  \brief
 *      fb_blending_attr_t, blending���������
 */
typedef struct
{
    /*! �˵����ݵĹؼ�colorֵ*/
    unsigned int color_key;
    /*! ���ݻ�ϵ��ӵı���ֵ, ȡֵ��ΧΪ(0-255)*/
    unsigned int alpha;
} fb_blending_attr_t;

/*!
 *  \brief
 *    fb_blending_setattr_t, ����blending��������ԵĲ�����������Ҫ���õ�handleֵ��
 *
 * */
typedef struct
{
    /*! ��Ҫ�������Ե�blending�����handle*/
    int handle;
    /*! blending��������ԣ�����colorkey and alpha*/
    fb_blending_attr_t blending_attr;
} fb_blending_setattr_t;

/*!
 *  \brief
 *    blending_area_manage_t, ��ȡblending����״̬�Ĳ�����
 * */
typedef struct
{
    /*! blending area ��id�ţ� area��ţ���Ŵ�[0--(MAX_BLENDING_AREA-1)*/
    int area_id;
    /*! blending area ��״̬����ʾ�����أ�0-hide, 1-show*/
    int is_show;
    /*! blending area �����������*/
    fb_blending_param_t blending_param;
} fb_blending_area_manage_t;

/*!
 *  \brief
 *  ��ʾ�豸
 */
enum DISPLAYER_MODE
{
    /*!  CPU_LCD_DISPLAYER : CPU LCD*/
    CPU_LCD_DISPLAYER = 0x01,
    /*! RGB_LCD_DISPLAYER:RGB LCD*/
    RGB_LCD_DISPLAYER = 0x02,
    /*! TV_DISPLAYER : TVOUT in pal mode*/
    TV_PAL_DISPLAYER = 0x04,
    /*! TV_DISPLAYER : TVOUT in ntsc mode*/
    TV_NTSC_DISPLAYER = 0x08,
};

/*����frmebuffer���������Ƿ����video���״̬*/
/*!������Ƶ���״̬ */
#define FBIOSET_START_VIDEO             0x4660
/*!ֹͣ��Ƶ���״̬ */
#define FBIOSET_END_VIDEO               0x4661

/*!��ȡFB��ز�������FB�ķֱ��ʡ�֧�ֵ����ݸ�ʽ */
#define FBIOGET_FB_PARAM                0x4662
/*!��ȡFB����ʼ��ַ */
#define FBIOGET_FB_ADDR                 0x4663

/*!������Ƶ��ʾ���� */
#define FBIOSET_VIDEO_AREA              0x4664
/*!��ȡ��Ƶ��ʾ���� */
#define FBIOGET_VIDEO_AREA              0x4665

/*!������豸�ϸ���һ��fb���ݼ��˵����� */
#define FBIOSET_UPDATE_FB               0x4666
/*!������豸�����һ֡��Ƶ���� */
#define FBIOSET_UPDATE_VIDEO            0x4667

/*!�л�ͼ������豸��LCD_DISPLAYER, TV_DISPLAYER */
#define FBIOSET_DISPLAYER               0x4668
/*!��ȡ��case֧�ֵ�����ͼ������豸��LCD_DISPLAYER, TV_DISPLAYER */
#define FBIOGET_DISPLAYER_LIST          0x4669
/*!��ȡ��ʾ�豸����ز�������֧�ֵ����ݸ�ʽ����ˢ����ˢ�����ô�����ʱʹ��displayer_attr_t�ӿ� */
#define FBIOGET_DISPLAYER_ATTR          0x4670

/*!��FB���������������е����ݲ�������Ļ�ϸ��� */
#define FBIOSET_LOCK_FB                 0x4671
/*!��FB���� */
#define FBIOSET_UNLOCK_FB               0x4672

/*!�������ķ�ʽ */
#define FBIOSET_SCREEN_ROTATE           0x4673
/*!����mmm video�ı���buffer��ַ */
#define FBIOSET_VIDEO_BACKUP_BUF        0x4674

/*!����blending area */
#define FBIOSET_BLENDING_AREA_CREATE    0x4675
/*!show blending area */
#define FBIOSET_BLENDING_AREA_SHOW      0x4676
/*!hide blending area */
#define FBIOSET_BLENDING_AREA_HIDE      0x4677
/*!set blending area ������ */
#define FBIOSET_BLENDING_AREA_SETATTR   0x4678
/*!����blending area */
#define FBIOSET_BLENDING_AREA_DESTROY   0x4679
/*!��ȡblending���� */
#define FBIOGET_BLENDING_AREA           0x4680

/*!������鵱ǰ����ʾ�豸*/
#define FBIOSET_DEV_DETECT              0x4681
/*!��ȡ��֧����ʾ�豸������xres*yres,����Ƶʹ��*/
#define FBIOGET_DEV_MAX_RES             0x4682
/*!��ȡ��Ļ�ķ���*/
#define FBIOGET_SCREEN_ROTATE           0x4683

/*!��ȡ��ǰ����ʾ�豸*/
#define FBIOGET_DISPLAYER               0x4684

#if 1
/*!ˢ��Fb���첽�ģ�fb����������ʱ*/
#define FBIOSET_UPDATE_FB_ASYNC         0x4685
/*!��ȡlcd��ˢ��״̬���Ƿ�DMA���ڰ�����*/
#define FBIOGET_STATUS			        0x4686
#endif
#endif

