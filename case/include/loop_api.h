/********************************************************************************
 *                              USDK(1100)
 *                             Module: lib 
 *                 Copyright(c) 2003-2009 Actions Semiconductor,
 *                            All Rights Reserved. 
 * History:         
 *      <author>    <time>           <version >             <desc>
 *      xusongzhou  2008-07-16          v1.0               build this file 
 *      shaofeng    2009-05-17          v1.1               �Ż�loopmode
 *      shaofeng    2009-05-18          v1.2               ����case����淶�������
 *      songzhining 2009-12-24         v1.3               ����ǿ��ѭ��ģʽ���л�����
 ********************************************************************************/
/*!
* \file     loop_api.h
* \brief    ����ѭ��ģʽ
* \author   xusongzhou
*               
* \version 1.0
 * \date  2008/07/16
*******************************************************************************/

#ifndef __LOOP_API_H__
#define __LOOP_API_H__

/*!
 *  \brief
 *      ��������ѭ��ģʽ
 *  \li LOOP_MODE_SEQUENCE      : ȫ��һ��
 *  \li LOOP_MODE_REPEAT_ONE    : ����ѭ��
 *  \li LOOP_MODE_REPEAT_ALL    : ȫ��ѭ��
 *  \li LOOP_MODE_SHUFFLE       : SHUFFLEһ��
 *  \li LOOP_MODE_SHUFFLE_REPEAT: SHUFFLEѭ��
 *  \li LOOP_MODE_ONLY_ONE      : ����һ�� 
 */ 
typedef enum
{  
    LOOP_MODE_SEQUENCE = 0,
    LOOP_MODE_REPEAT_ONE,
    LOOP_MODE_REPEAT_ALL,
    LOOP_MODE_SHUFFLE,
    LOOP_MODE_SHUFFLE_REPEAT,
    LOOP_MODE_ONLY_ONE,
}loop_mode_e;

/*!
 *  \brief
 *      ���������л�ģʽ
 *  \li LOOP_SWITCH_NORMAL  : ��ͨ( �Զ���ʽ )
 *  \li LOOP_SWITCH_FORCELOOP    : ǿ��ѭ��( �ֶ���ʽ )
 */ 
typedef enum
{  
    LOOP_SWITCH_NORMAL = 0,
    LOOP_SWITCH_FORCELOOP,
}loop_switch_e;

/*!
 *  \brief
 *      ���������л�����
 *  \li LOOP_DIRECT_NEXT    : �����л�
 *  \li LOOP_DIRECT_PREV    : �����л�
 */ 
typedef enum
{  
    LOOP_DIRECT_NEXT = 0,
    LOOP_DIRECT_PREV,
}loop_direct_e;


/*! \cond loop_API_no */
/*
*********************************************************************************
* \Description: Initialize the static variable.
*              NOTE:
*              You should call this function before others.
* \Para<in>  : void
* \Para<out> : none
* \RetVal    : true --initialize ok
* \Others    :
*********************************************************************************/
extern bool loop_init(void);

/*
*********************************************************************************
* \Description: Set the loop mode.
*              you may call it when you need to set or change the loop mode.
*              NOTE:
*              It should be called before "loop_next", "loop_prev" and "loop_delete_index"
* \Para<in>  : loop_mode_e mode --the loop mode
* \Para<in>  : int current --the current index
* \Para<in>  : int total --the total number
* \Para<out> : none
* \RetVal    : true --set ok
* \Others    :
*********************************************************************************/
extern bool loop_set_mode(loop_mode_e mode, int current, int total);

/*
*********************************************************************************
* \Description: Get the current index.
* \Para<in>  : none
* \Para<out> : none
* \RetVal    : loop.current --the next index
* \Others    :
*********************************************************************************/
extern int loop_get_current(void);

/*
*********************************************************************************
* \Description: Get the next index.
* \Para<in>  : loop_switch_e smode -- normal or force loop
* \Para<out> : none
* \RetVal    : -1 --loop come to the end
* \RetVal    : loop.current --the next index
* \Others    :
*********************************************************************************/
extern int loop_get_next(loop_switch_e smode);

/*
*********************************************************************************
* \Description: Get the previous index.
* \Para<in>  : loop_switch_e smode -- normal or force loop
* \Para<out> : none
* \RetVal    : -1 --loop come to the end
* \RetVal    : loop.current --the previous index
* \Others    :
*********************************************************************************/
extern int loop_get_prev(loop_switch_e smode);

/*
*********************************************************************************
* \Description: Delete the index
* \Para<in>  : int del_index :the "delete" index
* \Para<in>  : loop_switch_e smode -- normal or force loop
* \Para<in>  : loop_direct_e dir -- next or prev
* \Para<out> : none
* \RetVal    : -1 --come to the end
* \RetVal    : current index
* \Others    :
*********************************************************************************/
extern int loop_delete_index(int del_index, loop_switch_e smode, loop_direct_e dir );

/*! \endcond*/
#endif  /* __LOOP_API_H__ */

