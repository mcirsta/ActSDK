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
    ��ʾ���ַ��� �м�� */
#define LINE_HEIGHT 19

/*!
*\brief
    dialog(��button)�����ʾ������ */
#define MAX_LINE_DIALOG 2

/*!
*\brief
    clip(����button)�����ʾ������ */
#define MAX_LINE_CLIP 4

/*!
*\brief
    ÿ�������ʾ���ַ����� */
#define MAX_CHAR_PER_LINE   50



/*
* clew ��ʾʱ�Ĳ���
*/
typedef struct
{
    /*! clew��һ����ĸ��dialog����ʾ���к� */
    char start_line;
    /*! clew��ת���������� */
    char total_line;
    /*! ������ʾ�Ķ�ά����, ��һά��ÿһ��һһ��Ӧ */
    char buffer_clew[MAX_LINE_CLIP][MAX_CHAR_PER_LINE+2];
}dialog_clew_param_t;


/*!
*\brief
    �ؼ��������ʾ״̬ �ṹ
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
    �ؼ���ʾ״̬ �ṹ
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
    dailog�ؼ� �ṹ
*/
typedef struct dialog_private_s
{
    /*! ���ڿ�����������ʾ״̬, ��Ҫ�û��ı�, �������ʾ */
    dialog_show_t show;
    /*! ���ڱ��浱ǰ��ʾ״̬, �û�����Ҫ���� */
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

