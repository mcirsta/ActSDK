/********************************************************************************
 *                              USDK(1100)
 *                             Module: app 
 *                 Copyright(c) 2003-2008 Actions Semiconductor,
 *                            All Rights Reserved. 
 * file : msgbox_private.h
 * History:         
 *      <author>    <time>           <version >             <desc>
 *      Herb Hu    2009-6-2             v1.0             build this file 
 ********************************************************************************/

#ifndef __MSG_BOX_PRIVATE_H__
#define __MSG_BOX_PRIVATE_H__

#include <include_case.h>
#include <include_psp.h>

#include "commonui_sty.h"

typedef enum 
{
    /* 鏃犵晫闈㈡秷鎭� */
    MSGBOX_SHOW_TYPE_NONE = 0,

    /* 鏅�氭彁绀烘绫荤晫闈㈡秷鎭� */
    MSGBOX_SHOW_TYPE_DIALOG,

    /* usb閫夋嫨妗嗙晫闈㈡秷鎭� */
    MSGBOX_SHOW_TYPE_USB_SELECT_BOX,
}msgbox_show_type_e;

typedef enum 
{
    MSGBOX_MSG_PRIO_0 = 0, // min
    MSGBOX_MSG_PRIO_1 ,
    MSGBOX_MSG_PRIO_2 ,
    MSGBOX_MSG_PRIO_3 ,
    MSGBOX_MSG_PRIO_4 ,
    MSGBOX_MSG_PRIO_5 ,
    MSGBOX_MSG_PRIO_6 ,
    MSGBOX_MSG_PRIO_7 , // max
}msgbox_msg_priority_e;

typedef enum 
{
    MSGBOX_MSG_TYPE_EARPHONE_NOT_INSERT = 0 ,
    MSGBOX_MSG_TYPE_LOW_VOLTAGE ,
    MSGBOX_MSG_TYPE_KEYLOCK ,
    MSGBOX_MSG_TYPE_USB_B_IN ,
    MSGBOX_MSG_TYPE_USB_B_OUT ,
    MSGBOX_MSG_TYPE_USB_ADAPTER_IN ,
    MSGBOX_MSG_TYPE_USB_ADAPTER_OUT ,
    MSGBOX_MSG_TYPE_USB_HOST_DISK_IN ,
    MSGBOX_MSG_TYPE_USB_HOST_DISK_OUT ,
    MSGBOX_MSG_TYPE_CARD_IN ,
    MSGBOX_MSG_TYPE_CARD_OUT ,
    MSGBOX_MSG_TYPE_TVIN_ALARM_IN ,
    MSGBOX_MSG_TYPE_LINE_IN_INSERT,
    MSGBOX_MSG_TYPE_SHORTCUT_TVOUT,
    MSGBOX_MSG_TYPE_SHORTCUT_CAMERA,
    MSGBOX_MSG_TYPE_SHORTCUT_PLAYBACK,
    MSGBOX_MSG_TYPE_SHORTCUT_DELETE,
    MSGBOX_MSG_TYPE_NEED_GEN_PLIST,
    MSGBOX_MSG_TYPE_CARD_INIT_FAILED,
    MSGBOX_MSG_TYPE_CARD_MOUNT_FAILED,
    MSGBOX_MSG_TYPE_USB_HOST_DISK_INIT_FAILED,
}msgbox_msg_type_e;

typedef struct 
{
    /* 鐣岄潰绫绘秷鎭搷浣滃嚱鏁� */
    bool (* create)(void * param); 
    bool (* delete)(void * param); 
    bool (* paint)(void * param); 
    bool (* load_style)(void * param); 
    bool (* proc_gui)(void * param, gui_wm_message_t * pmsg); 
    int (* default_process)(void * param); 
    int (* process)(void * param); 
}msgbox_msg_op_t;

typedef struct 
{
    dialog_init_t dialog_init;
    dialog_t * dialog;
    dialog_cb_t callback;
}msgbox_show_type_dialog_param_t;

typedef struct 
{
    usb_select_box_type_e usb_select_box_type;
    usb_select_box_t * usb_select_box;
    usb_select_box_cb_t callback;
}msgbox_show_type_usb_select_box_param_t;

typedef struct msgbox_msg_object_s
{
    msgbox_msg_type_e msg_type;
    msgbox_show_type_e show_type;
    msgbox_msg_priority_e prior;
    int canvas_id;
    int default_process_timer_id;
    void * param;
    msgbox_msg_op_t op;
    struct msgbox_msg_object_s * next;
}msgbox_msg_object_t;

struct msgbox_private_data_s
{
    msgbox_msg_object_t* msg_queue;
    msgbox_msg_object_t* cur_msg;
}msgbox_private_data_t; 

extern msgbox_t * g_msgbox ;

#endif /* #ifndef __MSG_BOX_PRIVATE_H__ */ 

