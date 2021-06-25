/*
*******************************************************************************
*                       NOYA1100
*                (c) Copyright 2002-2008, Actions Co,Ld. 
*                        All Right Reserved 
* FileName: .h
* Description:
* History: 
*       <author>    <time>          <version >      <desc>
*       shaofeng    2009/03/10      1.0             �������ļ���
*******************************************************************************
*/ 
#ifndef __DRV_MAP_H__
#define __DRV_MAP_H__

#include "key.h"
#include "app_msg.h"



/********************************** key map ********************************/
#define MAX_NUM_KEY_MAP 10
#define MAX_NUM_KEY_SHORTCUT 5


typedef enum
{
    /* the max value of key type, it must be less than  MASK_KEY_UP*/
    KEY_TYPE_NULL       = 0x00000000,
    KEY_TYPE_DOWN       = 0x20000000,
    KEY_TYPE_LONG       = 0x10000000,
    KEY_TYPE_HOLD       = 0x08000000,
    KEY_TYPE_SHORT_UP   = 0x04000000,
    KEY_TYPE_LONG_UP    = 0x02000000,
    KEY_TYPE_HOLD_UP    = 0x01000000,
    KEY_TYPE_ALL        = 0x3F000000,
}key_type_e;


typedef struct
{
   key_val_e    val;
   key_type_e   type;
}key_event_t;


/*!
* \par  Description:
*       key map struct, to make the key which is key_val_src and key_type_src, 
*       to be another key which is key_val_dest and key_type_dest.
*   please refer to marc of KEY_MAP1
*/
typedef struct
{
    bool        enable;
    key_val_e   key_val_src;
    key_type_e  key_type_src;
    key_val_e   key_val_dest;
    key_type_e  key_type_dest;
}key_map_t;


typedef struct
{
    key_event_t     key_event;
    msg_apps_type_e msg_type;
}key_shortcut_t;


typedef struct
{
    int long_key_time;      /* ��λ: ms */
    int hold_key_interval;  /* ��λ: ms */

    key_map_t key_map_list[MAX_NUM_KEY_MAP];  /* ��ֵӳ��� */

    key_shortcut_t key_shortcut_list[MAX_NUM_KEY_SHORTCUT]; /* ��ݼ��� */
}key_para_t;


/* ����ӳ���: map list, �μ�: key_map_t
*���ӳ�����: MAX_NUM_KEY_MAP, ���������һ������msg_server��ʼ��ӳ�䲿��
*�궨��4��������������Ϊ: ӳ��ǰ��ֵ,ӳ��ǰ��������,  ӳ����ֵ,ӳ���İ�������
*
*ע��:  ��֤ӳ�������, �����KEY_MAP*��д��ӳ��ֵ, ��ǰ����KEY_MAP*��������ӳ��ֵ
*
*       ӳ��ǰ�������������: KEY_TYPE_ALL: ���˼�ֵת������һ��ֵ, �������ͱ��ֲ���, ӳ���İ������Ͳ�����Ч
*       ӳ��ǰ�������Ϳ�����key_type_e�Ķ������, ��ӳ���İ������ͱ���ֻ��Ϊһ��
*ʾ��:
    //��MENU���� ת���� VOL�̰�, ��:����MENU����VOL����
    #define KEY_MAP1    KEY_MENU,KEY_TYPE_LONG, KEY_VOL,KEY_TYPE_DOWN
    
    //��PREV ת���� VSUB, ��:PREV��ʵ��VSUBȫ������
    #define KEY_MAP2    KEY_PREV,KEY_TYPE_ALL,  KEY_VSUB,KEY_TYPE_ALL
*/
/* ��MENU���� ת���� VOL�̰�, ��:����MENU����VOL���� */
#define KEY_MAP1    true,  KEY_MENU,KEY_TYPE_LONG, KEY_VOL,KEY_TYPE_DOWN
#define KEY_MAP2    false, 0,0, 0,0
#define KEY_MAP3    false, 0,0, 0,0
#define KEY_MAP4    false, 0,0, 0,0
#define KEY_MAP5    false, 0,0, 0,0
#define KEY_MAP6    false, 0,0, 0,0
#define KEY_MAP7    false, 0,0, 0,0
#define KEY_MAP8    false, 0,0, 0,0
#define KEY_MAP9    false, 0,0, 0,0
#define KEY_MAP10   false, 0,0, 0,0


/*
* ��ݼ�ӳ��, �μ�: key_shortcut_t
*/
#define KEY_SHORTCUT1    {KEY_OK, KEY_TYPE_LONG}, MSG_POWER_OFF
#define KEY_SHORTCUT2    {KEY_CAMERA, KEY_TYPE_LONG}, MSG_SHORTCUT_CAMERA
#define KEY_SHORTCUT3    {KEY_PLAYBACK, KEY_TYPE_DOWN}, MSG_SHORTCUT_PLAYBACK
#define KEY_SHORTCUT4    {KEY_RETURN, KEY_TYPE_LONG}, MSG_SHORTCUT_RETURN
#define KEY_SHORTCUT5    {0,0}, 0



/********************************** key map ********************************/

typedef struct
{
   int pressed; 
   int x;
   int y; 
}touch_event_t;




#endif  /* __DRV_MAP_H__ */

