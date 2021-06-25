/*
 ********************************************************************************
 *                       linux213x
 *         actions decoder interface structure
 *                (c) Copyright 2002-2003, Actions Co,Ld.
 *                        All Right Reserved
 *
 * File   : act_decoder.h
 * by	 : cs/fw
 * Version: 1> v1.00     first version     02.24.2006
 ********************************************************************************
 */
#ifndef __ACT_KSC_H__
#define __ACT_KSC_H__

#ifdef __cplusplus
extern "C"
{
#endif

/*!
 * \brief
 *      ����������ʾ������Ϣ�йص����ݽṹ
 */
typedef struct
{
    /*! ���ϽǺ����� */
    int tx_pos;
    /*! ���Ͻ������� */
    int ty_pos;
    /*! �� */
    int width;
    /*! �� */
    int height;
} ksc_display_area_t; //��ʾ������

/*!
 * \brief
 *      ��������������Ϣ�йص����ݽṹ
 */
typedef struct
{
    /*! ԭ����������ɫ */
    int OriginalFontColor;
    /*! ������������ɫ */
    int ProgressFontColor;
    /*! �������ͣ�����塢���� */
    int FontType;
    /*! �����С */
    int FontSize;
} ksc_font_property_t;

/*!
 * \brief
 *      ��������ksc ��ʼ����Ϣ�йص����ݽṹ
 */
typedef struct
{
    /*! �ļ���Ϊȫ·�������ڿ�ʵ��ʱ��Ҫ�жϵ�ǰ��ksc�ļ�����ǰһ��ʹ�õ��Ƿ���ͬ������ͬ�����κβ��������������صĲ����� */
    char *FileName;
    /*! framebuffer����ʼ��ַ */
    char *FbStartAddr;
    /*! ��Ļ��ʾ */
    ksc_font_property_t CurFont;
    /*! ��Ļ����ʾ���� */
    ksc_display_area_t DisplayArea;
} ksc_init_t;

/*! �ͷ������� */
int Libkscdispose();

/*! ��ʼ������ */
int LibkscInit(ksc_init_t *param);

/*!
 * \brief
 *      ��������ksc ˢ��������
 * ����˵����
 *        NowTime:  ��ǰʱ��
 *        framebuf: ��ǰˢ����ַ
 *        format: ������������Ϊ1������Ϊ0
 *        cmd:    0,yuv��ʾ���У�1��yuv������ʾ˫�У�2��rgb��ʾ���У�3��rgb��ʾ˫��
 *        DisplayArea: �������л�ʱ��λ�úͿ������
 */
int LibkscLrcDisplay(int NowTime, char *framebuf, int format, int cmd,
        ksc_display_area_t *DisplayArea);

#ifdef __cplusplus
}
#endif

#endif
