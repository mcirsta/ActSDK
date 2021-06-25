/********************************************************************************
 *                              USDK(1100)
 *                             Module: app
 *                 Copyright(c) 2003-2009 Actions Semiconductor,
 *                            All Rights Reserved.
 * File : stopwatch_main.c
 * History:
 *      <author>    <time>           <version >             <desc>
 *     dupengcheng    2009-04-28          v1.0               build this file
 ********************************************************************************/
/*!
 * \file     stopwatch_ctl.c
 * \brief    stopwatch application
 * \author   dupengcheng
 *
 * \version 1.0
 * \date  2009/04/14
 *******************************************************************************/
#include "stopwatch_scene.h"
#include "stopwatch_ui.h"
#include "stopwatch_ctl.h"


int cur_label_idx = 0; /* ��ǰ���ڼ�ʱ�� label��index */
watchvalue_t watchs[MAX_WATCH];
watchvalue_t curwatchvalue; /* ��ǰֵ */
int status = STATE_STOP;
int timerEnabled = 0; /*��ʱ���Ƿ��б�Ҫ���ô������ */
extern struct timeval startTicks;
extern scene_stopwatch_t* g_scene_stopwatch;


static bool _reset_to_begin(scene_stopwatch_t* my_scene);
static void _watch_restart_dlg(void);
static void _watch_restart_dlg_cb(dialog_t *dlg, widget_sig_dialog_e signal);


//bool startCurWatch(scene_stopwatch_t* my_scene);
//void startNextWatch(scene_stopwatch_t* my_scene);
//bool watch_menu(scene_stopwatch_t* my_scene);

/******************************************************************************/
/*!
 * \par  Description:
 *       ������ key_ok ��ʱ���в�ͬ�Ŀ�ʼ��ʽ
 * \param[in]    scene_stopwatch_t *: �����ṹָ��
 * \retval       void
 * \ingroup      stopwatch
 *******************************************************************************/
bool startCurWatch(scene_stopwatch_t* my_scene)
{
    switch (status)
    {
        case STATE_RUNNING:
        status = STATE_PAUSE;
        timerEnabled = 0;
        sys_forbid_auto_standby(false); //ȡ����ֹʡ��ػ�
        sys_forbid_screensaver(false); //ȡ����ֹ��������
        break;

        case STATE_PAUSE:
        status = STATE_RUNNING;
        do_gettimeofday(&startTicks);
        timerEnabled = 1;
        sys_forbid_auto_standby(true); //��ֹʡ��ػ�
        sys_forbid_screensaver(true); //��ֹ��������
        break;

        case STATE_STOP:
        status = STATE_RUNNING;
        do_gettimeofday(&startTicks);
        timerEnabled = 1;
        sys_forbid_auto_standby(true); //��ֹʡ��ػ�
        sys_forbid_screensaver(true); //��ֹ��������

        /* watch1-str_index ������ɫ��� */
        draw_watch1_idx(my_scene);
        break;

        default:
        break;
    }
    return true;
}

/******************************************************************************/
/*!
 * \par  Description:
 *       ������ key_next ��
 * \param[in]    scene_stopwatch_t *: �����ṹָ��
 * \retval       void
 * \ingroup      stopwatch
 *******************************************************************************/
void startNextWatch(scene_stopwatch_t* my_scene)
{
    int old_lable;

    old_lable = cur_label_idx;
    do_gettimeofday(&startTicks);
    if (status == STATE_STOP)
    {
        cur_label_idx = 0;
    }
    else
    {
        cur_label_idx++;
    }

    /* �Ѵ�����watch */
    if (cur_label_idx == MAX_WATCH)
    {
        cur_label_idx = 0;
    }

    /* ����״̬�ػ���һ����ʱ���ļ�ʱ��־λ */
    status = STATE_RUNNING;
    timerEnabled = 1;

    /* �� next ʱ��������ƶ���ͼ�ĸ��� */
    when_key_next_draw(my_scene, old_lable);
    draw_cur_watch_time(my_scene);
}

/******************************************************************************/
/*!
 * \par  Description:
 *       ������е�watch���Ա����¼�ʱ
 * \param[in]    scene_stopwatch_t *: �����ṹָ��
 * \retval       void
 * \ingroup      stopwatch
 *******************************************************************************/
static bool _reset_to_begin(scene_stopwatch_t* my_scene)
{
    int i;

    watch_scene_init_paint(my_scene);

    memset(&curwatchvalue, 0, sizeof(watchvalue_t));
    for (i = 0; i < MAX_WATCH; i++)
    {
        memset(&watchs[i], 0, sizeof(watchvalue_t));
    }

    cur_label_idx = 0;
    status = STATE_STOP;
    timerEnabled = 0;
    memset(&startTicks, 0, sizeof(struct timeval));

    /* ����������,��ɫ,��С */
    gui_dc_set_text_mode(my_scene->scene_common.handle_gui_dc, GUI_DRAW_MODE_TRANS);
    gui_dc_set_font_size(my_scene->scene_common.handle_gui_dc, 20);
    gui_dc_set_pen_size(my_scene->scene_common.handle_gui_dc, 2);

    return true;
}

/******************************************************************************/
/*!
 * \par  Description:
 *       ������ key_nemu ���������˵�����ʾ�Ƿ����¼�ʱ
 * \param[in]    scene_stopwatch_t *: �����ṹָ��
 * \retval       void
 * \ingroup      stopwatch
 *******************************************************************************/
bool watch_menu(scene_stopwatch_t* my_scene)
{
    bool rc = true;

    _watch_restart_dlg();

    return rc;
}

/******************************************************************************/
/*!
 * \par  Description:
 *      ����"���¼�ʱ" �Ի���
 * \param[in]    none
 * \retval       void
 * \ingroup      stopwatch
 *******************************************************************************/
static void _watch_restart_dlg(void)
{
    dialog_init_t dlg_init;

    dlg_init.head = DIALOG_HEAD_QUERY;
    dlg_init.str_id = g_scene_stopwatch->restart_str->str_id;
    dlg_init.def_sel=WIDGET_SIG_DIALOG_CANCAL;
    g_scene_stopwatch->dialog = dialog_create(&dlg_init, _watch_restart_dlg_cb);
    if (g_scene_stopwatch->dialog != NULL)
    {
        g_scene_stopwatch->dialog->total_button = 2;
        dialog_paint(g_scene_stopwatch->dialog);
    }
}

/******************************************************************************/
/*!
 * \par  Description:
 *      "���¼�ʱ" �Ի��� �Ļص�����
 * \param[in]    dialog_t *: �Ի���ṹָ��
 * \param[in]    int: ������Ϣ
 * \retval       void
 * \ingroup      stopwatch
 *******************************************************************************/
static void _watch_restart_dlg_cb(dialog_t *dlg, widget_sig_dialog_e signal)
{
    switch (signal)
    {
        case WIDGET_SIG_DIALOG_OK:
        /* delete dialog then restart */
        dialog_delete(g_scene_stopwatch->dialog);
        g_scene_stopwatch->dialog = NULL;
        _reset_to_begin(g_scene_stopwatch);
        break;

        case WIDGET_SIG_DIALOG_CANCAL:
        default:
        /* do nothing */
        dialog_delete(g_scene_stopwatch->dialog);
        g_scene_stopwatch->dialog = NULL;
        break;
    }
}

bool paint_when_rotate(void)
{
    int i;
    int color;
    int focus_color;

    color = (int)g_scene_stopwatch->watch1.str_res->background;
    focus_color = (int)g_scene_stopwatch->watch1.str_res->foreground;

    /*������ͼƬ*/
    paint_necessary_when_rotate(g_scene_stopwatch);
    /*��ǰ��ʱ*/
    update_watch_head(g_scene_stopwatch);


    if (is_upright_disp() == true) //����
    {
        if (watchs[cur_label_idx].hundredSeconds > 0)
        {
            draw_cur_watch_time(g_scene_stopwatch);
        }
        /*���м�¼��watch*/
        for (i = 0; i < MAX_WATCH; i++)
        {
            if ( (i != cur_label_idx) && (watchs[i].hundredSeconds > 0) )
            {
                draw_watch_time(i, i, color);
                draw_watch_idx(i, i, color);
            }
        }
    }
    else //����
    {
        if (cur_label_idx < 4)
        {
            if (watchs[cur_label_idx].hundredSeconds > 0)
            {
                draw_cur_watch_time(g_scene_stopwatch);
            }
            /*���м�¼��watch*/
            for (i = 0; i < 4; i++) //����ֻ��4��watch
            {
                if ( (i != cur_label_idx) && (watchs[i].hundredSeconds > 0) )
                {
                    draw_watch_time(i, i, color);
                    draw_watch_idx(i, i, color);
                }
            }
        }
        else
        {
            draw_watch_idx(cur_label_idx - 3, 0, color);
            draw_watch_time(cur_label_idx - 3, 0, color);

            draw_watch_idx(cur_label_idx - 2, 1, color);
            draw_watch_time(cur_label_idx - 2, 1, color);

            draw_watch_idx(cur_label_idx - 1, 2, color);
            draw_watch_time(cur_label_idx - 1, 2, color);

            stopwatch_draw_frame(g_scene_stopwatch, 3, 1); //��ǰ ���� ����
            draw_watch_idx(cur_label_idx, 3, focus_color);
            draw_watch_time(cur_label_idx, 3, focus_color);
        }
    }
    
    //�����û�м�ʱ��������watch��Ϊ0),������һ��watch
    if ((watchs[0].hundredSeconds == 0) && (cur_label_idx == 0))
    {
        stopwatch_draw_frame(g_scene_stopwatch, 0, 1);
    }
    return true;
}

