/*
*******************************************************************************
*                       NOYA1100
*                (c) Copyright 2002-2008, Actions Co,Ld. 
*                        All Right Reserved 
* FileName: msg_server.h
* Description:
* History: 
*       <author>    <time>          <version >      <desc>
*       shaofeng    2009/03/11      1.0             �������ļ���
*******************************************************************************
*/ 

#ifndef __MSG_SERVER_H__
#define __MSG_SERVER_H__


#include "include_case.h"



#define GUI_KEY_FIFO  "/tmp/ucgui/.gui_key_fifo"
#define SYS_MSG_FIFO  "/tmp/ucgui/.sys_msg_fifo"



/*!
*\brief
    �����gui����Ϣ���� */
typedef enum
{
    /*! �������͵���Ϣ */
    INPUT_MSG_KEY,
    /*! ���������͵���Ϣ */
    INPUT_MSG_TOUCH,
}input_msg_type_e;

/*!
*\brief
    �����gui����Ϣ�ṹ�� */
typedef struct
{
    input_msg_type_e type;
    union
    {
        key_event_t kmsg;
        touch_event_t tmsg;
    }data;
}input_gui_msg_t;

/*!
 * \brief
 * ����USB�ߵ�����
 */
typedef enum
{
    /*! USBδ���� */
    USB_LINE_NONE,
    /*! USB A��*/
    USB_LINE_A,
    /*! USB B��*/
    USB_LINE_B,
}usb_line_type_e;

/*!
*\brief
    Ӳ����״̬�ṹ��, msg_server������Ӳ��״̬��¼�ڵ�ǰ�ṹ���� */
typedef struct
{
    /*! �Ƿ��� */
    bool keylock;
    /*! �Ƿ��п����� */
    bool card;
    /*! �Ƿ���Բ�ڳ�������� */
    bool adapter;
    /*! �Ƿ���usb����, ����usb�����ӵ�pc�ͳ�����Լ�U�� */
    USB_CONNECTION_T usb;
    /*! ��ǰ�����USB������ */
    usb_line_type_e usb_linetype;
    /*! ��ǰ�ĳ��״̬ */
    charge_status_t charge;
    /*! ֱ�ӻ�ȡ��صĵ�ѹ */
    charge_voltage_t voltage;
    /*! ����headbar��ʾ�ĵ�ѹ, ������ƽ������ */
    charge_voltage_t voltage_show;
}hw_status_t;



#endif /* __MSG_SERVER_H__ */

