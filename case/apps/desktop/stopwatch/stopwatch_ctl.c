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


int cur_label_idx = 0; /* 当前正在计时的 label的index */
watchvalue_t watchs[MAX_WATCH];
watchvalue_t curwatchvalue; /* 当前值 */
int status = STATE_STOP;
int timerEnabled = 0; /*定时器是否有必要调用处理代码 */
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
 *       当按下 key_ok 键时，有不同的开始方式
 * \param[in]    scene_stopwatch_t *: 场景结构指针
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
        sys_forbid_auto_standby(false); //取消禁止省电关机
        sys_forbid_screensaver(false); //取消禁止背光屏保
        break;

        case STATE_PAUSE:
        status = STATE_RUNNING;
        do_gettimeofday(&startTicks);
        timerEnabled = 1;
        sys_forbid_auto_standby(true); //禁止省电关机
        sys_forbid_screensaver(true); //禁止背光屏保
        break;

        case STATE_STOP:
        status = STATE_RUNNING;
        do_gettimeofday(&startTicks);
        timerEnabled = 1;
        sys_forbid_auto_standby(true); //禁止省电关机
        sys_forbid_screensaver(true); //禁止背光屏保

        /* watch1-str_index 字体颜色变黑 */
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
 *       当按下 key_next 键
 * \param[in]    scene_stopwatch_t *: 场景结构指针
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

    /* 已达最后的watch */
    if (cur_label_idx == MAX_WATCH)
    {
        cur_label_idx = 0;
    }

    /* 根据状态重绘下一个计时器的计时标志位 */
    status = STATE_RUNNING;
    timerEnabled = 1;

    /* 当 next 时，焦点的移动和图的更新 */
    when_key_next_draw(my_scene, old_lable);
    draw_cur_watch_time(my_scene);
}

/******************************************************************************/
/*!
 * \par  Description:
 *       清楚所有的watch，以便重新计时
 * \param[in]    scene_stopwatch_t *: 场景结构指针
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

    /* 设置字体风格,颜色,大小 */
    gui_dc_set_text_mode(my_scene->scene_common.handle_gui_dc, GUI_DRAW_MODE_TRANS);
    gui_dc_set_font_size(my_scene->scene_common.handle_gui_dc, 20);
    gui_dc_set_pen_size(my_scene->scene_common.handle_gui_dc, 2);

    return true;
}

/******************************************************************************/
/*!
 * \par  Description:
 *       当按下 key_nemu 键，弹出菜单，提示是否重新计时
 * \param[in]    scene_stopwatch_t *: 场景结构指针
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
 *      创建"重新计时" 对话框
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
 *      "重新计时" 对话框 的回调函数
 * \param[in]    dialog_t *: 对话框结构指针
 * \param[in]    int: 按键消息
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

    /*基本的图片*/
    paint_necessary_when_rotate(g_scene_stopwatch);
    /*当前计时*/
    update_watch_head(g_scene_stopwatch);


    if (is_upright_disp() == true) //竖屏
    {
        if (watchs[cur_label_idx].hundredSeconds > 0)
        {
            draw_cur_watch_time(g_scene_stopwatch);
        }
        /*画有记录的watch*/
        for (i = 0; i < MAX_WATCH; i++)
        {
            if ( (i != cur_label_idx) && (watchs[i].hundredSeconds > 0) )
            {
                draw_watch_time(i, i, color);
                draw_watch_idx(i, i, color);
            }
        }
    }
    else //横屏
    {
        if (cur_label_idx < 4)
        {
            if (watchs[cur_label_idx].hundredSeconds > 0)
            {
                draw_cur_watch_time(g_scene_stopwatch);
            }
            /*画有记录的watch*/
            for (i = 0; i < 4; i++) //横屏只有4个watch
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

            stopwatch_draw_frame(g_scene_stopwatch, 3, 1); //当前 焦点 高亮
            draw_watch_idx(cur_label_idx, 3, focus_color);
            draw_watch_time(cur_label_idx, 3, focus_color);
        }
    }
    
    //如果还没有计时（即所有watch都为0),高亮第一个watch
    if ((watchs[0].hundredSeconds == 0) && (cur_label_idx == 0))
    {
        stopwatch_draw_frame(g_scene_stopwatch, 0, 1);
    }
    return true;
}

