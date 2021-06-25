/*
*******************************************************************************
*                       NOYA1100
*                (c) Copyright 2002-2008, Actions Co,Ld. 
*                        All Right Reserved 
* FileName: .h
* Description:
* History: 
*       <author>    <time>          <version >      <desc>
*       shaofeng    2009/03/10      1.0             建立该文件。
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
    int long_key_time;      /* 单位: ms */
    int hold_key_interval;  /* 单位: ms */

    key_map_t key_map_list[MAX_NUM_KEY_MAP];  /* 键值映射表 */

    key_shortcut_t key_shortcut_list[MAX_NUM_KEY_SHORTCUT]; /* 快捷键表 */
}key_para_t;


/* 按键映射表: map list, 参见: key_map_t
*最大映射个数: MAX_NUM_KEY_MAP, 如果更改请一并更改msg_server初始化映射部分
*宏定义4各参数意义依次为: 映射前键值,映射前按键类型,  映射后键值,映射后的按键类型
*
*注意:  保证映射表连续, 即如果KEY_MAP*填写了映射值, 其前所有KEY_MAP*都必须有映射值
*
*       映射前按键类型如果是: KEY_TYPE_ALL: 将此键值转换成另一键值, 按键类型保持不变, 映射后的按键类型参数无效
*       映射前按键类型可以是key_type_e的多种组合, 但映射后的按键类型必须只能为一种
*示例:
    //将MENU长按 转换成 VOL短按, 即:长按MENU弹出VOL窗口
    #define KEY_MAP1    KEY_MENU,KEY_TYPE_LONG, KEY_VOL,KEY_TYPE_DOWN
    
    //将PREV 转换成 VSUB, 即:PREV键实现VSUB全部功能
    #define KEY_MAP2    KEY_PREV,KEY_TYPE_ALL,  KEY_VSUB,KEY_TYPE_ALL
*/
/* 将MENU长按 转换成 VOL短按, 即:长按MENU弹出VOL窗口 */
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
* 快捷键映射, 参见: key_shortcut_t
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

