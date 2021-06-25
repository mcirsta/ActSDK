/********************************************************************************
 *                              USDK(1100)
 *                             Module: app 
 *                 Copyright(c) 2003-2009 Actions Semiconductor,
 *                            All Rights Reserved. 
 * History:         
 *      <author>    <time>           <version >             <desc>
 *      shaofeng    2009-2-25          v1.0               build this file 
 ********************************************************************************/
/*!
* \file     dialog_private.h
* \brief    
* \author   shaofeng
*               
* \version 1.0
* \date  2009/02/25
*******************************************************************************/

#ifndef __DIALOG_PRIVATE_H__
#define __DIALOG_PRIVATE_H__


#include "include_case.h"




/*!
*\brief
    显示的字符的 行间距 */
#define LINE_HEIGHT 19

/*!
*\brief
    dialog(带button)最多显示的行数 */
#define MAX_LINE_DIALOG 2

/*!
*\brief
    clip(不带button)最多显示的行数 */
#define MAX_LINE_CLIP 4

/*!
*\brief
    每行最多显示的字符个数 */
#define MAX_CHAR_PER_LINE   50



/*
* clew 显示时的参数
*/
typedef struct
{
    /*! clew第一个字母在dialog中显示的行号 */
    char start_line;
    /*! clew经转换的总行数 */
    char total_line;
    /*! 用于显示的二维数组, 第一维与每一行一一对应 */
    char buffer_clew[MAX_LINE_CLIP][MAX_CHAR_PER_LINE+2];
}dialog_clew_param_t;


/*!
*\brief
    控件保存的显示状态 结构
*/
typedef struct
{
    char bg;
    char head;
    char yes;
    char no;
    char clew;
}dialog_saved_show_t;


/*!
*\brief
    控件显示状态 结构
*/
typedef struct
{
    char bg;
    
    char head;
    unsigned short head_frame;
    
    char yes;
    unsigned short yes_frame;
    
    char no;
    unsigned short no_frame;
    
    char clew;
}dialog_show_t;


/*!
*\brief
    dailog控件 结构
*/
typedef struct dialog_private_s
{
    /*! 用于控制流控制显示状态, 需要用户改变, 以完成显示 */
    dialog_show_t show;
    /*! 用于保存当前显示状态, 用户不需要关心 */
    dialog_saved_show_t saved_show;
    
    dialog_clew_param_t clew_param;
    /*! request resource of system  */
    int timer_id;
    int handle_gui_dc;
    
    /*! resource of show */
    resgroup_resource_t *dialog_resgroup;
    picture_resource_t *pic_bg;
    picregion_resource_t *reg_head;
    picregion_resource_t *reg_yes;
    picregion_resource_t *reg_no;
    string_resource_t *str_clew;
}dialog_private_t;




extern void dialog_draw_bg(dialog_t *dialog, char show);
extern void dialog_draw_head(dialog_t *dialog, char show, unsigned short frame);
extern void dialog_draw_yes(dialog_t *dialog, char show, unsigned short frame);
extern void dialog_draw_no(dialog_t *dialog, char show, unsigned short frame);
extern bool dialog_get_clew_param( dialog_t *dialog, unsigned short str_id, dialog_clew_param_t *param );
extern void dialog_draw_clew(dialog_t *dialog, char show, dialog_clew_param_t *clew_param);


#endif  /* __DIALOG_PRIVATE_H__ */

