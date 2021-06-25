/*******************************************************************************
 *                              USDK130
 *                            Module: GUI
 *                 Copyright(c) 2003-2008 Actions Semiconductor,
 *                            All Rights Reserved.
 *
 * History:
 *      <author>    <time>           <version >             <desc>
 *       ywwang     2006-09-28 15:00     1.0             build this file
 *******************************************************************************/
/*!
 * \file     wm_inc.h
 * \brief    guiģ�鴰�ڹ���������ͷ�ļ�
 * \author   ywwang
 * \version 1.0
 * \date  2006/09/28
 *******************************************************************************/

#ifndef __WM_INC_H__
#define __WM_INC_H__
/*!\cond GUI_API*/

#include "gui_inc.h"

/*********************************************************************
 *
 *              ���ݽṹ
 *
 **********************************************************************
 */
/*!
 *  \brief
 *   ���ڴ���ʱ��ѡ�ķ��
 *  \li WM_CREATE_HIDE : �����󴰿����أ����ɼ���
 *  \li WM_CREATE_SHOW : �����󴰿ڼ��ɼ�
 */
typedef enum
{
    WM_CREATE_HIDE = (0 << 1),
    WM_CREATE_SHOW = (1 << 1)
} gui_wm_create_style_t;

/*!
 *  \brief
 *  GUI��Ϣ����
 *  \li WM_MSG_PAINT : �ػ���Ϣ
 *  \li WM_MSG_CREATE : ���ڴ�����Ϣ
 *  \li WM_MSG_DELETE : ����ɾ����Ϣ
 *  \li WM_MSG_TOUCH : ��������Ϣ
 *  \li WM_MSG_KEY : ������Ϣ
 *  \li WM_MSG_USER : �û��Զ�����Ϣ
 */
typedef enum
{
    WM_MSG_PAINT = 1,
    WM_MSG_CREATE = 3,
    WM_MSG_DELETE = 4,
    WM_MSG_TOUCH = 12,
    WM_MSG_KEY = 13,
    WM_MSG_USER = 200
} gui_wm_message_type_t;

/*!
 * \brief
 *      ������Ϣ����
 */
typedef struct
{
    /*! ��Ϣ���� */
    int msgid;
    /*! Ŀ�Ĵ��� */
    unsigned short hwin;
    /*! Դ����  */
    unsigned short hwinsrc;
    /*! ��Ϣ���� */
    /*!
     * \brief
     *      ���ܵ���Ϣ����
     */
    union
    {
        /*! ����ָ��*/
        void* p;
        /*! ������*/
        int v;
        /*! ��ɫֵ*/
        unsigned int color;
    } data;
} gui_wm_message_t;

/*!
 *  \brief
 *      wm_callback_t(gui_wm_message_t *pmsg) - void
 */
typedef void wm_callback_t(gui_wm_message_t *pmsg);

/*********************************************************************
 *
 *              ���ڹ�����
 *
 **********************************************************************
 */
void gui_wm_show_window(int hwin);

int gui_wm_create_window(int x0, int y0, int xsize, int ysize,
        gui_wm_create_style_t style, wm_callback_t *cb, int num_extra_bytes);

int gui_wm_create_window_as_child(int x0, int y0, int xsize, int ysize,
        int hwin_parent, gui_wm_create_style_t style, wm_callback_t *cb,
        int num_extra_bytes);

int gui_wm_delete_window(int hwin);

int gui_wm_select_window(int hwin);

void gui_wm_invalidate_window(int hwin);

int gui_wm_move_window(int hwin, int dx, int dy);
wm_callback_t* gui_wm_set_callback(int win, wm_callback_t* cb);

void gui_wm_get_client_rect(gui_rect_t* prect);
int gui_wm_get_window_size_x(int hwin);
int gui_wm_get_window_size_y(int hwin);
void gui_wm_default_callback(gui_wm_message_t * pmsg);

void gui_wm_invalidate_rect(int hwin, const gui_rect_t*prect);
int gui_wm_set_size(int hwin, int xsize, int ysize);

int gui_wm_set_focus(int hwin);
int gui_wm_get_focus();
void gui_wm_hide_window(int hwin);

void gui_wm_bring_to_top(int hwin);
void gui_wm_bring_to_bottom(int hwin);

int gui_wm_send_key_message(int key, int pressed);

void gui_wm_process_bring_to_top(int pid);
int gui_wm_check_window_status(int *pids, int max_procs);
int gui_wm_set_process_region(int pid, gui_rect_t *prect);

/*!\endcond*/
#endif   /* _WM_INC_H_ */
