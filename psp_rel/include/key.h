/*
 *******************************************************************************
 *                       NOYA1100
 *                (c) Copyright 2002-2008, Actions Co,Ld.
 *                        All Right Reserved
 * FileName: key.h
 * Description:
 * History:
 *       <author>    <time>          <version >      <desc>
 *       shaofeng    2009/03/10      1.0             建立该文件。
 *******************************************************************************
 */
#ifndef __KEY_H__
#define __KEY_H__

/* drv/key命令 */
#define KEY_GET_LOCKKEY_STATU  0x4001 /*读取锁键的状态*/

/* 驱动层设定的按键抬起标志, 除msg_server和key driver外, 其他应用驱动无需调用 */
#define MASK_KEY_UP  0x40000000
#define IS_KEY_UP(key_val)      ( (((key_val) & MASK_KEY_UP) != 0)? 1:0 )
#define SET_KEY_UP(key_val)     ( (key_val) | MASK_KEY_UP )
#define CLEAR_KEY_UP(key_val)   ( (key_val) & ~MASK_KEY_UP )

typedef enum
{
    KEY_NULL = 0x00,
    KEY_PREV = 0x01,
    KEY_NEXT = 0x02,
    KEY_VOL = 0x04,
    KEY_VADD = 0x08,
    KEY_VSUB = 0x10,
    KEY_MENU = 0x20,
    KEY_OK = 0x40,
    KEY_RETURN = 0x80,
    KEY_TVOUT = 0x100,
    KEY_CAMERA = 0x200,

    KEY_PLAYBACK = 0x400,
    KEY_DELETE = 0x800,
    KEY_REVERSE3 = 0x1000,
    KEY_REVERSE4 = 0x2000,
    KEY_REVERSE5 = 0x4000,
    KEY_REVERSE6 = 0x8000,
    KEY_REVERSE7 = 0x10000,
    KEY_REVERSE8 = 0x20000,
    KEY_REVERSE9 = 0x40000,
    KEY_REVERSE10 = 0x80000,
    KEY_REVERSE11 = 0x100000,
    KEY_REVERSE12 = 0x200000,
    KEY_REVERSE13 = 0x400000,
    KEY_REVERSE14 = 0x800000,
    KEY_LOCK = 0x1000000, //xkp added
    /* the max value of key, it must be less than  key_type_e*/
    KEY_REVERSE16 = 0x2000000,
} key_val_e;

#endif  /* __KEY_H__ */

