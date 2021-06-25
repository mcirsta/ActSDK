/*
*******************************************************************************
*                       NOYA1100
*                (c) Copyright 2002-2008, Actions Co,Ld. 
*                        All Right Reserved 
* FileName: .h
* Description:
* History: 
*       <author>    <time>          <version >      <desc>
*       shaofeng    2009/02/28      1.0             �������ļ���
*******************************************************************************
*/ 

#ifndef __CFG_VOLTAGE_CHARGE_H__
#define __CFG_VOLTAGE_CHARGE_H__

#include "include_case.h"

//ѡ��Actions���ܳ������, ���Ǳ�׼�������(�����ں�ѹ�׶��Ƿ���ʵ�ʵ�ص�ѹ)
//1: Actions���ܳ��; 0:��׼���
#define CHARGE_TYPE 1




//���Ӧ��
    //������
    #define CHARGE_FULL_VOL     
    #define CHARGE_MAX_CURRENT  




//����߳�
    //����߳�, ��򿪴˺궨��
    #define CHARGE_PLAYING  1
    
    //������
    #define CHARGE_PLAYING_FULL_VOL     
    #define CHARGE_PLAYING_MAX_CURRENT  




//USB���
    //USB ���г��, ��򿪴˺궨��
    #define CHARGE_USB_IDLE  1
    
    //���ж೤ʱ�俪ʼ���, ��λ:��
    #define CHARGE_USB_IDLE_TIME  10
    //������
    #define CHARGE_USB_FULL_VOL     
    #define CHARGE_USB_MAX_CURRENT  




//������Ƶ���
    //������Ƶʱ���, ��򿪴˺궨��
    #define CHARGE_VIDEO  1
    
    //������
    #define CHARGE_VIDEO_FULL_VOL     
    #define CHARGE_VIDEO_MAX_CURRENT  




//�����ʾ����, ��������ѹ
    #define VOL_BATTERY_LEVELS 4
    #define VOL_BATTERY_LEVEL0 
    #define VOL_BATTERY_LEVEL1 
    #define VOL_BATTERY_LEVEL2 
    #define VOL_BATTERY_LEVEL3 




//���ֵ�ѹ
    //��������ѹ
    #define VOL_NOT_START_UP   
    //�͵���ʾ��ѹ, ����:�����͵���ʾ��ػ�, �Ϳ�����ĵ͵���ʾ
    #define VOL_LOW_VOLTAGE             LI350
    //�͵�ػ���ѹ
    #define VOL_POWER_OFF               LI330
    //����video�����޵�ѹ
    #define VOL_THRESHOLD_PLAY_VIDEO    LI360
    //��ʼ¼�������޵�ѹ
    #define VOL_THRESHOLD_PLAY_RECORD
    //��ʼ�Զ����������޵�ѹ
    #define VOL_THRESHOLD_UPGRADE






#endif /* __CFG_VOLTAGE_CHARGE_H__ */

