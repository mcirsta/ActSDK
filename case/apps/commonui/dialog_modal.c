/*******************************************************************************
 *                              NOYA1100
 *                            Module: video
 *                 Copyright(c) 2003-2008 Actions Semiconductor,
 *                            All Rights Reserved. 
 * History:         
 *      <author>    <time>           <version >             <desc>
 *      shaofeng    2009-05-25          1.0             build this file 
*******************************************************************************/
/*!
 * \file     dialog_modal.c
 * \brief    实现dialog控件
 * \author   shaofeng
 * \par      GENERAL DESCRIPTION:
 *               
 * \par      EXTERNALIZED FUNCTIONS:
 *               
 * \version 1.0
 * \date  2009/05/25
*******************************************************************************/

#include "include_case.h"


/*本应用的头文件*/


typedef struct
{
    int win_id;
    dialog_t *dialog;
    int sig;
}dialog_modal_t;


dialog_modal_t dialog_modal_vars;

static int s_auto_quit_timer_id = -1;
static int auto_quit_timer_proc(void * param);



#if 0
#endif
static void _dialog_cb( dialog_t *dialog, widget_sig_dialog_e signal )
{
    dialog_modal_vars.sig = (int)signal;
    
    switch ( signal ) 
    {
    case WIDGET_SIG_DIALOG_OK:
        exit_msg_loop();
        break;
    case WIDGET_SIG_DIALOG_CANCAL:
        exit_msg_loop();
        break;
    default:
        break;
    }
}

static void _dialog_modal_win_cb(gui_wm_message_t *pmsg)
{
    if(pmsg->msgid == WM_MSG_KEY)
    {
        if(s_auto_quit_timer_id > 0)
        {
            restart_inner_timer(s_auto_quit_timer_id);
        }
    }

    dialog_proc_gui( dialog_modal_vars.dialog, pmsg );
}

bool dialog_modal_load_style( void )
{
    bool result = true;
    
    if ( NULL == dialog_modal_vars.dialog )
    {
        return false;
    }
    
    result = dialog_load_style( dialog_modal_vars.dialog );
    
    return result;
}

bool dialog_modal_paint( void )
{
    bool result = true;
    
    if ( NULL == dialog_modal_vars.dialog )
    {
        return false;
    }
    
    result = dialog_paint( dialog_modal_vars.dialog );
    
    return result;
}

static int auto_quit_timer_proc(void * param)
{
    dialog_modal_vars.sig = (int)WIDGET_SIG_DIALOG_CANCAL;
    exit_msg_loop();
    return 0;
}


/******************************************************************************/
/*!                    
* \par  Description:
*      模态对话框. 从此函数进入, 也从此函数返回
* \param[in]    unsigned short, dialog显示字符串id
* \retval       -1: err, 其他为用户选择结果, 请参照:widget_sig_dialog_e
* \ingroup      common_dialog
*******************************************************************************/
int dialog_modal( dialog_init_t *dialog_init )
{
    msg_apps_t  msg;
    int win_id_saved_focus = -1;
    scene_t *app_scene = NULL;
    
    if ( NULL == dialog_init )
    {
        print_warning("param is NULL");
        return -1;
    }

    grab_msg(MSG_POWER_OFF);
    grab_msg(MSG_SHORTCUT_RETURN);
    
    filter_msg(MSG_POWER_OFF);
    filter_msg(MSG_SHORTCUT_RETURN);
    
    memset( &dialog_modal_vars, 0x00, sizeof(dialog_modal_vars) );
    dialog_modal_vars.sig = -1;
    
    win_id_saved_focus = gui_wm_get_focus();
    
    app_scene = sty_get_app_scene();
    if ( NULL == app_scene )
    {
        print_err("get app scene fail");
        return -1;
    }
    
    //选择图层0, 防止modal dialog窗口对之前应用的窗口产生影响
    gui_select_layer( 0 );
    dialog_modal_vars.win_id = gui_wm_create_window(app_scene->x, app_scene->y, \
            app_scene->width, app_scene->height, WM_CREATE_HIDE, _dialog_modal_win_cb, 0);
    if( dialog_modal_vars.win_id < 0 )
    {
        print_err("gui_wm_create_window failed\n\n");
        return -1;
    }
    gui_wm_set_focus( dialog_modal_vars.win_id );
    
    dialog_modal_vars.dialog = dialog_create( dialog_init, _dialog_cb );
    if( NULL == dialog_modal_vars.dialog )
    {
        print_err("dialog_create failed\n\n");
        goto DIALOG_MODAL_ERR;
    }
    dialog_paint( dialog_modal_vars.dialog );

    if(dialog_init->head == DIALOG_HEAD_QUERY)
    {
        s_auto_quit_timer_id = set_inner_single_shot_timer(3000, auto_quit_timer_proc, NULL);
    }
    
    while( true == get_msg(&msg) )
    {
        dispatch_msg(&msg);
    }
    
    dialog_delete( dialog_modal_vars.dialog );
    dialog_modal_vars.dialog = NULL;
    
    DIALOG_MODAL_ERR:
    //删除窗体
    if ( dialog_modal_vars.win_id > 0 )
    {
        gui_wm_delete_window( dialog_modal_vars.win_id );
        dialog_modal_vars.win_id = -1;
    }
    //恢复焦点
    gui_wm_set_focus( win_id_saved_focus );

    unfilter_msg(MSG_POWER_OFF);
    unfilter_msg(MSG_SHORTCUT_RETURN);

    release_msg(MSG_POWER_OFF);
    release_msg(MSG_SHORTCUT_RETURN);

    if(s_auto_quit_timer_id > 0)
    {
        kill_inner_timer(s_auto_quit_timer_id);
    }
    
    return dialog_modal_vars.sig;
}

