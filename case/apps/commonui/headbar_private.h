/********************************************************************************
 *                              USDK(1100)
 *                             Module: app 
 *                 Copyright(c) 2003-2009 Actions Semiconductor,
 *                            All Rights Reserved. 
 * History:         
 *      <author>    <time>           <version >             <desc>
 *      shaofeng    2009-4-17          v1.0               build this file 
 ********************************************************************************/
/*!
* \file     headbar_private.h
* \brief    
* \author   shaofeng
*               
* \version 1.0
* \date  2009/04/17
*******************************************************************************/

#ifndef __HEADBAR_PRIVATE_H__
#define __HEADBAR_PRIVATE_H__


#include "include_case.h"



/*!
*\brief
    电池显示级数, 从1开始 */
#define MAX_HEADBAR_BATTERY_LEVEL   6

/*!
*\brief
    第1级电池最大电压 */
#define VOL_HEADBAR_BATTERY_LEVEL1  LI360

/*!
*\brief
    第2级电池最大电压 */
#define VOL_HEADBAR_BATTERY_LEVEL2  LI365

/*!
*\brief
    第3级电池最大电压 */
#define VOL_HEADBAR_BATTERY_LEVEL3  LI375

/*!
*\brief
    第4级电池最大电压 */
#define VOL_HEADBAR_BATTERY_LEVEL4  LI385

/*!
*\brief
    第5级电池最大电压. 第6级显示的电压为: 大于此电压 */
#define VOL_HEADBAR_BATTERY_LEVEL5  LI395



/*!
*\brief
    显示time字符串的最大长度 */
#define HEADBAR_BUFFER_LENGTH_STR_TIME  (16)

/*!
*\brief
    显示title字符串的最大长度 */
#define HEADBAR_BUFFER_LENGTH_STR_TITLE (60)


/*!
*\brief
    控件保存的显示状态 结构
*/
typedef struct
{
    char bg;
    char music_play;
    char keylock;
    char card;
    char usb;
    
    char head;
    char battery;
    
    char title;
    char time;
}headbar_saved_show_t;


/*!
*\brief
    控件显示状态 结构
*/
typedef struct
{
    char bg;
    
    char music_play;
    char keylock;
    char card;
    char usb;

    char head;
    char battery;
    unsigned short battery_frame;
    
    char title;
    char time;
}headbar_show_t;


/*!
*\brief
    headbar控件 结构
*/
typedef struct headbar_private_s
{
    /*! 用于控制流控制显示状态, 需要用户改变, 以完成显示 */
    headbar_show_t show;
    /*! 用于保存当前显示状态, 用户不需要关心 */
    headbar_saved_show_t saved_show;
    
    /*! request resource of system  */
    int timer_id;
    int handle_gui_dc;
    
    /*! resource of show */
    resgroup_resource_t *resgroup;
    
    picture_resource_t *pic_bg;
    picture_resource_t *pic_music_play;
    picture_resource_t *pic_keylock;
    picture_resource_t *pic_card;
    picture_resource_t *pic_usb;
    picregion_resource_t *reg_head;
    picregion_resource_t *reg_battery;
    
    string_resource_t *str_title;
    string_resource_t *str_time;
    /*! buffer of show time */
    char buffer_str_time[HEADBAR_BUFFER_LENGTH_STR_TIME];
    time_format_e   time_format;
}headbar_private_t;



extern void headbar_draw_bg(headbar_t *headbar, char show);
extern void headbar_draw_music_play(headbar_t *headbar, char show);
extern void headbar_draw_keylock(headbar_t *headbar, char show);
extern void headbar_draw_card(headbar_t *headbar, char show);
extern void headbar_draw_usb(headbar_t *headbar, char show);
extern void headbar_draw_head(headbar_t *headbar, char show, unsigned short pic_id);
extern void headbar_draw_battery(headbar_t *headbar, char show, unsigned short frame);
extern void headbar_draw_title_by_id(headbar_t *headbar, char show, unsigned short str_id);
extern void headbar_draw_title_by_buffer(headbar_t *headbar, char show, char *ptr);
extern void headbar_draw_time(headbar_t *headbar, char show, char *p_time);


#endif  /* __HEADBAR_PRIVATE_H__ */

