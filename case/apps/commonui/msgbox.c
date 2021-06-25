/********************************************************************************
 *                              USDK(1100)
 *                             Module: app 
 *                 Copyright(c) 2003-2008 Actions Semiconductor,
 *                            All Rights Reserved. 
 * file : msgbox.c
 * History:         
 *      <author>    <time>           <version >             <desc>
 *      Herb Hu    2009-06-01             v1.0             build this file 
 ********************************************************************************/

#include "msgbox_private.h"

typedef struct 
{
    unsigned int style_id;
    unsigned short str_id;
}str_res_t;

typedef enum 
{
    STR_NAME_CARD_IN = 0,
    STR_NAME_CARD_OUT,
    STR_NAME_LOW_POWER,
    STR_NAME_KEY_LOCK,
    STR_NAME_UHOST_IN,
    STR_NAME_UHOST_OUT,
    STR_NAME_ALARM_IN,
    STR_NAME_NO_EARPHONE,
    STR_NAME_LINE_IN_INSERT,
    STR_NAME_CARD_INIT_FAILED,
    STR_NAME_CARD_MOUNT_FAILED,
    STR_NAME_EXTERNAL_INIT_FAILED,
}str_name_e;

static str_res_t str_res_table[] =
{
    {STR_CARD_IN, 0},
    {STR_CARD_OUT, 0},
    {STR_LOW_POWER, 0},
    {STR_KEY_LOCK, 0},
    {STR_UHOST_IN, 0},
    {STR_UHOST_OUT, 0},
    {STR_ALARM_IN, 0},
    {STR_NO_EARPHONE, 0},
    {STR_LINE_IN_INSERT, 0},
    {STR_CARD_INIT_FAILED, 0},
    {STR_CARD_MOUNT_FAILED, 0},
    {STR_EXTERNAL_INIT_FAILED, 0},
};

typedef struct msg_filter_list_s
{
    msg_apps_type_e type;
    struct msg_filter_list_s * next;
}msg_filter_list_t;

static msg_filter_list_t * s_msg_filter_list = NULL;

msgbox_t * g_msgbox = NULL;

static int s_process_next_timer_id = -1;


static void _load_str_res(resgroup_resource_t * res_group, str_res_t res_table[], int count);

static void _process_next_msg(void);
static bool _msgbox_show_cur_msg_box(void);
static bool _msgbox_quit_cur_msg_box(void);
static int _process_next_msg_timer_cb(void * param);

static bool _op_dialog_create(void * param);
static bool _op_dialog_proc_gui(void * param, gui_wm_message_t * pmsg);
static bool _op_dialog_paint(void * param);
static bool _op_dialog_load_style(void * param);
static bool _op_dialog_delete(void * param);
static int  _op_dialog_common_default_process(void * param);
static void _dialog_common_callback(dialog_t *dialog, widget_sig_dialog_e signal);

static int  _op_dialog_alarm_tvin_default_process(void * param);
static void _dialog_alarm_tvin_callback(dialog_t *dialog, widget_sig_dialog_e signal);

static int  _op_dialog_earphone_not_insert_default_process(void * param);
static void _dialog_earphone_not_insert_callback(dialog_t *dialog, widget_sig_dialog_e signal);

static int  _op_dialog_line_in_insert_default_process(void * param);
static void _dialog_line_in_insert_callback(dialog_t *dialog, widget_sig_dialog_e signal);

static bool _op_usb_select_box_create(void * param);
static bool _op_usb_select_box_proc_gui(void * param, gui_wm_message_t * pmsg);
static bool _op_usb_select_box_paint(void * param);
static bool _op_usb_select_box_load_style(void * param);
static bool _op_usb_select_box_delete(void * param);
static int  _op_usb_select_box_mtp_default_process(void * param);
static int  _op_usb_select_box_data_trans_default_process(void * param);
static int  _op_usb_select_box_adapter_default_process(void * param);
static void _usb_select_box_callback( usb_select_box_t * usb_select_box, usb_select_box_sig_e signal);

static msgbox_msg_object_t * _create_msg_object(msg_apps_t * pmsg);
static bool _delete_msg_object(msgbox_msg_object_t * msg_object);

static bool _msg_queue_append(msgbox_msg_object_t ** ppqueue, msgbox_msg_object_t * msg_object);
static msgbox_msg_object_t * _msg_queue_fetch(msgbox_msg_object_t ** ppqueue);
static bool _msg_queue_delete(msgbox_msg_object_t ** ppqueue, msgbox_msg_type_e type);
static void _msg_queue_clear(msgbox_msg_object_t ** ppqueue);
static void _msg_queue_print(msgbox_msg_object_t * pqueue);

static bool _msg_filter_check(msg_apps_type_e type);
static bool _msg_filter_list_add(msg_apps_type_e type);
static bool _msg_filter_list_delete(msg_apps_type_e type);
static bool _msg_filter_list_clear(void);
static void _msg_filter_list_print(void);

static int _op_shortcut_camera_process(void * param);
static int _op_shortcut_playback_process(void * param);

static int _need_gen_plist(void);

static void _request_launch_app(const char * app_name);
static char s_launch_tvin_msg_content[MSG_CONTENT_SIZE] = {0};
static bool s_set_forbid_scrsaver = false; 

static void _set_monitor_mode( USB_MODE_T mode );

static void _load_str_res(resgroup_resource_t * res_group, str_res_t res_table[], int count)
{
    int i ;
    string_resource_t  * str_res;

    for(i = 0; i < count; i++)
    {
        str_res = ( string_resource_t * )get_resgroup_child(res_group, res_table[i].style_id);
        if(str_res != NULL)
        {
            res_table[i].str_id = str_res->str_id;
        }
    }
}

msgbox_t * msgbox_create(msgbox_cb_t callback )
{
    msgbox_t * msgbox;
    scene_t * scene;
    resgroup_resource_t * res_group;
    const char * self_app_name = NULL;

    if(NULL == callback)
    {
        print_warning("input parameter(callback) is a NULL pointer!");
        return NULL;
    }

    scene = sty_get_common_scene();
    if ( scene == NULL )
    {
        print_err("get common scene failed!");
        return NULL;	
    }

    //load msgbox resource group 
    res_group = (resgroup_resource_t *)get_scene_child(scene, MSGBOX_RESGROUP);
    if(NULL == res_group)
    {
        print_err("get resource group MSGBOX_RESGROUP failed!");
        return NULL;	
    }

    _load_str_res(res_group, str_res_table, ARRAY_SIZE(str_res_table));

    msgbox = (msgbox_t *)malloc(sizeof(msgbox_t));		
    if ( msgbox == NULL )
    {
        print_err("malloc failed!");
        return NULL;	
    }	

    msgbox->data = (struct msgbox_private_data_s *)malloc(sizeof(struct msgbox_private_data_s));		
    if ( msgbox->data == NULL )
    {
        print_err("malloc failed!");
        free(msgbox);
        return NULL;	
    }

    memset(msgbox->data, 0, sizeof(msgbox_private_data_t));

    msgbox->callback = callback;
    g_msgbox = msgbox;

    self_app_name = get_app_name(getpid());
    if((self_app_name != NULL ) && (strcmp(self_app_name, basename(APP_NAME_USB)) == 0) )
    {
        _msg_filter_list_add(MSG_SHORTCUT_TVOUT);
        _msg_filter_list_add(MSG_SHORTCUT_CAMERA);
        _msg_filter_list_add(MSG_SHORTCUT_PLAYBACK);
        _msg_filter_list_add(MSG_SHORTCUT_DELETE);
        _msg_filter_list_add(MSG_NEED_GEN_PLIST);
    }

    connect_global_msg_queue(GLOBAL_SYS_MQ_FOR_FOREGROUND_APP, NULL);

    return msgbox;
}

bool msgbox_proc_sys(msgbox_t * msgbox, msg_apps_t *pmsg )
{
    const app_info_t * app_info = NULL;
    msgbox_msg_object_t * msg_object = NULL;
    int value ;

    if(NULL == msgbox)
    {
        print_warning("input parameter(msgbox) is a NULL pointer!");
        return false;
    }

    app_info = app_running_list_get_this_app_info();
    if((app_info != NULL) && (app_info->type != APP_TYPE_GUI))
    {
        print_warning(" console app is no need to deal");
        return false;
    }

    if( (pmsg->type == MSG_USB_B_OUT) 
        && (msgbox->data->cur_msg != NULL) 
        && (msgbox->data->cur_msg->msg_type == MSGBOX_MSG_TYPE_USB_B_IN) )
    {
        print_err();
        _msgbox_quit_cur_msg_box();
        return true;
    }

    if( (pmsg->type == MSG_USB_B_OUT) 
        && (msgbox->data->cur_msg != NULL) 
        && (msgbox->data->cur_msg->msg_type == MSGBOX_MSG_TYPE_USB_ADAPTER_IN) )
    {
        print_err();
        _msgbox_quit_cur_msg_box();
        return true;
    }

    if( pmsg->type == MSG_NEED_GEN_PLIST) 
    {
        value = 1;
        if(set_config(CFG_NEED_GEN_PLIST, (char *)&value, sizeof(value)) < 0)
        {
            print_err("set_config(CFG_NEED_GEN_PLIST) failed!");
        }

        value = 0;
        if(set_config(CFG_IGNORE_GEN_PLIST, (char *)&value, sizeof(value)) < 0)
        {
            print_err("set_config(CFG_IGNORE_GEN_PLIST) failed!");
        }

        return true;
    }

    if( (pmsg->type == MSG_SHORTCUT_RETURN) 
       && (app_info != NULL) 
       && (strcmp(app_info->name, basename(APP_NAME_USB)) != 0 ) )
    {
        msg_apps_t m = {0};
        m.type = MSG_KILL_USER_APPS;
        send_async_msg(APP_NAME_MANAGER, &m);

        return true;
    }

    if(_msg_filter_check(pmsg->type) == false)
    {
        msg_object = _create_msg_object(pmsg);

        if(msg_object == NULL)
        {
            return false;
        }

        if(msgbox->data->cur_msg != NULL )
        {
            if( msg_object->prior < msgbox->data->cur_msg->prior)
            {
                _delete_msg_object(msg_object);
            }
            else
            {
                _msgbox_quit_cur_msg_box();
                _msg_queue_append(&(msgbox->data->msg_queue), msg_object);
                _process_next_msg();
            }
        }
        else
        {
            _msg_queue_append(&(msgbox->data->msg_queue), msg_object);
            _process_next_msg();
        }

        return true;
    }

    return false;
}

bool msgbox_proc_gui(msgbox_t * msgbox, gui_wm_message_t *pmsg )
{
    if(NULL == msgbox)
    {
        print_warning("input parameter(msgbox) is a NULL pointer!");
        return false;
    }

    if( (msgbox->data->cur_msg != NULL) && (msgbox->data->cur_msg->op.proc_gui != NULL))
    {
        msgbox->data->cur_msg->op.proc_gui((void *)msgbox->data->cur_msg, pmsg);
    }

    return true;
}



bool msgbox_paint( msgbox_t * msgbox )
{
    if(NULL == msgbox)
    {
        print_warning("input parameter(msgbox) is a NULL pointer!");
        return false;
    }

    if( (msgbox->data->cur_msg != NULL) && (msgbox->data->cur_msg->op.paint != NULL))
    {
        msgbox->data->cur_msg->op.paint((void *)msgbox->data->cur_msg);
    }

    return true;
}

bool msgbox_load_style( msgbox_t * msgbox)
{
    if(NULL == msgbox)
    {
        print_warning("input parameter(msgbox) is a NULL pointer!");
        return false;
    }

    if( (msgbox->data->cur_msg != NULL) && (msgbox->data->cur_msg->op.load_style != NULL))
    {
        msgbox->data->cur_msg->op.load_style((void *)msgbox->data->cur_msg);
    }

    return true;
}

bool msgbox_delete( msgbox_t * msgbox )
{
    if(NULL == msgbox)
    {
        print_warning("input parameter(msgbox) is a NULL pointer!");
        return false;
    }

    _msg_queue_clear(&(g_msgbox->data->msg_queue));
    _msg_filter_list_clear();

    _msgbox_quit_cur_msg_box();

    if(s_process_next_timer_id > 0)
    {
        kill_timer(s_process_next_timer_id);
        s_process_next_timer_id = -1;
    }

    free(msgbox->data);
    free(msgbox);
    msgbox = NULL;
    g_msgbox = NULL;

    disconnect_global_msg_queue(GLOBAL_SYS_MQ_FOR_FOREGROUND_APP);
    return true;
}

static void _process_next_msg(void)
{
    msgbox_msg_object_t * msg_object = NULL;

    if(g_msgbox == NULL)
    {
        return;
    }

    msg_object =  _msg_queue_fetch(&(g_msgbox->data->msg_queue));
    if(msg_object != NULL)
    {
        g_msgbox->data->cur_msg = msg_object;

        if(msg_object->show_type == MSGBOX_SHOW_TYPE_NONE)
        {
            if(msg_object->op.process != NULL)
            {
                msg_object->op.process(NULL);
            }

            _delete_msg_object(g_msgbox->data->cur_msg);
            g_msgbox->data->cur_msg = NULL;
        }
        else
        {
            _msgbox_show_cur_msg_box();
            if(g_msgbox->data->cur_msg->msg_type == MSGBOX_MSG_TYPE_EARPHONE_NOT_INSERT)
            {
                _msg_filter_list_add(MSG_FMENGINE_EARPHONE_NOT_INSERT);
                _msg_queue_delete(&(g_msgbox->data->msg_queue), MSGBOX_MSG_TYPE_EARPHONE_NOT_INSERT);
            }

            if(g_msgbox->data->cur_msg->msg_type == MSGBOX_MSG_TYPE_LINE_IN_INSERT)
            {
                _msg_filter_list_add(MSG_FMENGINE_LINEIN_INSERT);
                _msg_queue_delete(&(g_msgbox->data->msg_queue), MSGBOX_MSG_TYPE_LINE_IN_INSERT);
            }
        }
    }
}

static bool _msgbox_show_cur_msg_box(void)
{
    msgbox_msg_object_t * cur_msg = NULL;
    cur_msg = g_msgbox->data->cur_msg;

    if( (cur_msg != NULL ) && (cur_msg->op.create != NULL) && (cur_msg->op.paint != NULL))
    {
        if(sys_is_screensaver_forbidden() == false)
        {
            sys_forbid_screensaver(true);
            s_set_forbid_scrsaver = true;
        }
    
        cur_msg->op.create(cur_msg);
        cur_msg->op.paint(cur_msg);

        g_msgbox->canvas_id = cur_msg->canvas_id;

        g_msgbox->callback(g_msgbox, WIDGET_SIG_MSGBOX_SHOW);

        if(cur_msg->msg_type == MSGBOX_MSG_TYPE_LOW_VOLTAGE)
        {
            g_msgbox->callback(g_msgbox, WIDGET_SIG_MSGBOX_SHOW_LOW_POWER);
        }

        return true;
    }

    return false;
}

static bool _msgbox_quit_cur_msg_box(void)
{
    msgbox_msg_object_t * cur_msg = NULL;
    msgbox_msg_type_e msg_type;

    cur_msg = g_msgbox->data->cur_msg;

    if( (cur_msg != NULL ) && (cur_msg->op.delete != NULL))
    {
        cur_msg->op.delete(cur_msg);
        g_msgbox->canvas_id = -1; 

        msg_type = cur_msg->msg_type;
        _delete_msg_object(cur_msg);
        g_msgbox->data->cur_msg = NULL;

        g_msgbox->callback(g_msgbox, WIDGET_SIG_MSGBOX_QUIT);

        if(msg_type == MSGBOX_MSG_TYPE_LOW_VOLTAGE)
        {
            g_msgbox->callback(g_msgbox, WIDGET_SIG_MSGBOX_QUIT_LOW_POWER);
        }

        if(msg_type == MSGBOX_MSG_TYPE_EARPHONE_NOT_INSERT)
        {
            _msg_filter_list_delete(MSG_FMENGINE_EARPHONE_NOT_INSERT);
        }

        if(msg_type == MSGBOX_MSG_TYPE_LINE_IN_INSERT)
        {
            _msg_filter_list_delete(MSG_FMENGINE_LINEIN_INSERT);
        }

        if(s_process_next_timer_id < 0)
        {
            s_process_next_timer_id = set_single_shot_timer(200, _process_next_msg_timer_cb, NULL);
        }
        else
        {
            restart_timer(s_process_next_timer_id);
        }

        if(s_set_forbid_scrsaver == true)
        {
            sys_forbid_screensaver(false);
            s_set_forbid_scrsaver = false;
        }

        return true;
    }

    return false;
}


static bool _op_dialog_create(void * param)
{
    msgbox_msg_object_t * msg_object;
    msgbox_show_type_dialog_param_t * dialog_param;

    msg_object = (msgbox_msg_object_t *)param;
    if(msg_object == NULL)
    {
        return false;
    }

    dialog_param = (msgbox_show_type_dialog_param_t *)(msg_object->param);
    if( (dialog_param != NULL) && (dialog_param->dialog == NULL) )
    {
        dialog_param->dialog = dialog_create(&(dialog_param->dialog_init), dialog_param->callback);
        if (NULL == dialog_param->dialog)
        {
            print_err("create dialog failed!");
            return false;
        }

        msg_object->canvas_id = dialog_param->dialog->canvas_id;

        if(msg_object->op.default_process != NULL)
        {
            if(msg_object->default_process_timer_id < 0)
            {
                msg_object->default_process_timer_id = \
                       set_single_shot_timer(3000, msg_object->op.default_process, msg_object);
            }
        }

        return true;
    }

    return false;
}

static bool _op_dialog_proc_gui(void * param, gui_wm_message_t * pmsg)
{
    msgbox_msg_object_t * msg_object;
    msgbox_show_type_dialog_param_t * dialog_param;

    msg_object = (msgbox_msg_object_t *)param;
    if(msg_object == NULL)
    {
        return false;
    }

    dialog_param = (msgbox_show_type_dialog_param_t *)(msg_object->param);
    if( (dialog_param != NULL) && (dialog_param->dialog != NULL) )
    {
        if( (pmsg->msgid == WM_MSG_KEY ) && (dialog_param->dialog_init.head == DIALOG_HEAD_QUERY))
        {
            restart_timer(msg_object->default_process_timer_id);
        }

        dialog_proc_gui(dialog_param->dialog, pmsg);
        return true;
    }

    return false;
}

static bool _op_dialog_paint(void * param)
{
    msgbox_msg_object_t * msg_object;
    msgbox_show_type_dialog_param_t * dialog_param;

    msg_object = (msgbox_msg_object_t *)param;
    if(msg_object == NULL)
    {
        return false;
    }

    dialog_param = (msgbox_show_type_dialog_param_t *)(msg_object->param);
    if( (dialog_param != NULL) && (dialog_param->dialog != NULL) )
    {
        dialog_paint(dialog_param->dialog);
        return true;
    }

    return false;
}

static bool _op_dialog_load_style(void * param)
{
    msgbox_msg_object_t * msg_object;
    msgbox_show_type_dialog_param_t * dialog_param;

    msg_object = (msgbox_msg_object_t *)param;
    if(msg_object == NULL)
    {
        return false;
    }

    dialog_param = (msgbox_show_type_dialog_param_t *)(msg_object->param);
    if( (dialog_param != NULL) && (dialog_param->dialog != NULL) )
    {
        dialog_load_style(dialog_param->dialog);
        return true;
    }

    return false;
}

static bool _op_dialog_delete(void * param)
{
    msgbox_msg_object_t * msg_object;
    msgbox_show_type_dialog_param_t * dialog_param;

    msg_object = (msgbox_msg_object_t *)param;
    if(msg_object == NULL)
    {
        return false;
    }

    dialog_param = (msgbox_show_type_dialog_param_t *)(msg_object->param);
    if( (dialog_param != NULL) && (dialog_param->dialog != NULL) )
    {
        kill_timer(msg_object->default_process_timer_id);
        msg_object->default_process_timer_id = -1;

        dialog_delete(dialog_param->dialog);
        dialog_param->dialog = NULL;
        dialog_param->callback = NULL; 
        msg_object->canvas_id = -1; 
        return true;
    }

    return false;
}

static void _dialog_common_callback(dialog_t * dialog, widget_sig_dialog_e sig)
{
    bool need_gen_plist = false;

    switch (sig)
    {
        case WIDGET_SIG_DIALOG_CANCAL:
        {

            if((g_msgbox->data->cur_msg != NULL) 
                    &&( (g_msgbox->data->cur_msg->msg_type == MSGBOX_MSG_TYPE_CARD_IN) 
                        ||(g_msgbox->data->cur_msg->msg_type == MSGBOX_MSG_TYPE_CARD_OUT) 
                        ||(g_msgbox->data->cur_msg->msg_type == MSGBOX_MSG_TYPE_USB_HOST_DISK_IN) 
                        ||(g_msgbox->data->cur_msg->msg_type == MSGBOX_MSG_TYPE_USB_HOST_DISK_OUT)) )
            {
                need_gen_plist = true;
            }

            _msgbox_quit_cur_msg_box();

            if(need_gen_plist == true)
            {
                _need_gen_plist();
            }

            break;
        }

        default:
        {
            break;
        }
    }
}

static int _op_dialog_common_default_process(void * param)
{
    /* auto quit */
    bool need_gen_plist = false;

    if((g_msgbox->data->cur_msg != NULL) 
            &&( (g_msgbox->data->cur_msg->msg_type == MSGBOX_MSG_TYPE_CARD_IN) 
                ||(g_msgbox->data->cur_msg->msg_type == MSGBOX_MSG_TYPE_CARD_OUT) 
                ||(g_msgbox->data->cur_msg->msg_type == MSGBOX_MSG_TYPE_USB_HOST_DISK_IN) 
                ||(g_msgbox->data->cur_msg->msg_type == MSGBOX_MSG_TYPE_USB_HOST_DISK_OUT)) )
    {
        need_gen_plist = true;
    }

    _msgbox_quit_cur_msg_box();

    if(need_gen_plist == true)
    {
        _need_gen_plist();
    }

    return 0;
}

static int  _op_dialog_alarm_tvin_default_process(void * param)
{
    int value = 0;
    
    //msgbox ignore gen plist
    value = 1;
    if(set_config(CFG_IGNORE_GEN_PLIST, (char *)&value, sizeof(value)) < 0)
    {
        print_err("set_config(CFG_IGNORE_GEN_PLIST) failed!");
    }
            
    _request_launch_app(s_launch_tvin_msg_content);
    g_msgbox->callback(g_msgbox, WIDGET_SIG_MSGBOX_TVIN_LAUNCHED);
    _msgbox_quit_cur_msg_box();

    return 0;
}

static void _dialog_alarm_tvin_callback(dialog_t *dialog, widget_sig_dialog_e signal)
{
    int value;
    
    switch (signal)
    {
        case WIDGET_SIG_DIALOG_OK:
        {
            //msgbox ignore gen plist
            value = 1;
            if(set_config(CFG_IGNORE_GEN_PLIST, (char *)&value, sizeof(value)) < 0)
            {
                print_err("set_config(CFG_IGNORE_GEN_PLIST) failed!");
            }
            
            _request_launch_app(s_launch_tvin_msg_content);
            g_msgbox->callback(g_msgbox, WIDGET_SIG_MSGBOX_TVIN_LAUNCHED);
            _msgbox_quit_cur_msg_box();
            break;
        }
        case WIDGET_SIG_DIALOG_CANCAL:
        {
            _msgbox_quit_cur_msg_box();

            _need_gen_plist();

            break;
        }

        default:
        {
            break;
        }
    }
}

static int  _op_dialog_earphone_not_insert_default_process(void * param)
{
    /* no need to deal now*/
    param = param;
    return 0;
}

static void _dialog_earphone_not_insert_callback(dialog_t *dialog, widget_sig_dialog_e signal)
{
    switch (signal)
    {
        case WIDGET_SIG_DIALOG_OK:
        {
            _msgbox_quit_cur_msg_box();
            break;
        }
        case WIDGET_SIG_DIALOG_CANCAL:
        {
            msg_apps_t msg = {0};
            msg.type = MSG_KILL_APP;

            strcpy(msg.content, APP_NAME_RADIO);
            send_async_msg(APP_NAME_MANAGER, &msg);

            strcpy(msg.content, APP_NAME_RADIO_ENGINE);
            send_async_msg(APP_NAME_MANAGER, &msg);

            _msgbox_quit_cur_msg_box();
            break;
        }

        default:
        {
            break;
        }
    }
}

static int  _op_dialog_line_in_insert_default_process(void * param)
{
    /* no need to deal now*/
    param = param;

    return 0;
}

static void _dialog_line_in_insert_callback(dialog_t *dialog, widget_sig_dialog_e signal)
{
    switch (signal)
    {
        case WIDGET_SIG_DIALOG_OK:
        {
            _msgbox_quit_cur_msg_box();
            break;
        }
        case WIDGET_SIG_DIALOG_CANCAL:
        {
            msg_apps_t msg = {0};
            msg.type = MSG_KILL_APP;

            strcpy(msg.content, APP_NAME_RADIO);
            send_async_msg(APP_NAME_MANAGER, &msg);

            strcpy(msg.content, APP_NAME_RADIO_ENGINE);
            send_async_msg(APP_NAME_MANAGER, &msg);

            _msgbox_quit_cur_msg_box();
            break;
        }

        default:
        {
            break;
        }
    }
}

static bool _op_usb_select_box_create(void * param)
{
    msgbox_msg_object_t * msg_object;
    msgbox_show_type_usb_select_box_param_t * usb_param;

    msg_object = (msgbox_msg_object_t *)param;
    if(msg_object == NULL)
    {
        return false;
    }

    usb_param = (msgbox_show_type_usb_select_box_param_t *)(msg_object->param);
    if( (usb_param != NULL) && (usb_param->usb_select_box == NULL) )
    {
        usb_param->usb_select_box = \
                  usb_select_box_create(usb_param->usb_select_box_type, (usb_select_box_cb_t)usb_param->callback);
        if (NULL == usb_param->usb_select_box)
        {
            print_err("create dialog failed!");
            return false;
        }

        msg_object->canvas_id = usb_param->usb_select_box->canvas_id;

        if(msg_object->op.default_process != NULL)
        {
            if(msg_object->default_process_timer_id < 0)
            {
                msg_object->default_process_timer_id = \
                   set_single_shot_timer(3000, msg_object->op.default_process, msg_object);
            }
        }

        return true;
    }

    return false;
}

static bool _op_usb_select_box_proc_gui(void * param, gui_wm_message_t * pmsg)
{
    msgbox_msg_object_t * msg_object;
    msgbox_show_type_usb_select_box_param_t * usb_param;

    msg_object = (msgbox_msg_object_t *)param;
    if(msg_object == NULL)
    {
        return false;
    }

    usb_param = (msgbox_show_type_usb_select_box_param_t *)(msg_object->param);
    if( (usb_param != NULL) && (usb_param->usb_select_box != NULL) )
    {
        if( pmsg->msgid == WM_MSG_KEY )
        {
            restart_timer(msg_object->default_process_timer_id);
        }

        usb_select_box_proc_gui(usb_param->usb_select_box, pmsg);
        return true;
    }

    return false;
}

static bool _op_usb_select_box_paint(void * param)
{
    msgbox_msg_object_t * msg_object;
    msgbox_show_type_usb_select_box_param_t * usb_param;

    msg_object = (msgbox_msg_object_t *)param;
    if(msg_object == NULL)
    {
        return false;
    }

    usb_param = (msgbox_show_type_usb_select_box_param_t *)(msg_object->param);
    if( (usb_param != NULL) && (usb_param->usb_select_box != NULL) )
    {
        usb_select_box_paint(usb_param->usb_select_box);
        return true;
    }

    return false;
}

static bool _op_usb_select_box_load_style(void * param)
{
    msgbox_msg_object_t * msg_object;
    msgbox_show_type_usb_select_box_param_t * usb_param;

    msg_object = (msgbox_msg_object_t *)param;
    if(msg_object == NULL)
    {
        return false;
    }

    usb_param = (msgbox_show_type_usb_select_box_param_t *)(msg_object->param);
    if( (usb_param != NULL) && (usb_param->usb_select_box != NULL) )
    {
        usb_select_box_load_style(usb_param->usb_select_box);
        return true;
    }

    return false;
}

static bool _op_usb_select_box_delete(void * param)
{
    msgbox_msg_object_t * msg_object;
    msgbox_show_type_usb_select_box_param_t * usb_param;

    msg_object = (msgbox_msg_object_t *)param;
    if(msg_object == NULL)
    {
        return false;
    }

    usb_param = (msgbox_show_type_usb_select_box_param_t *)(msg_object->param);
    if( (usb_param != NULL) && (usb_param->usb_select_box != NULL) )
    {
        kill_timer(msg_object->default_process_timer_id);

        usb_select_box_delete(usb_param->usb_select_box);
        usb_param->usb_select_box = NULL;
        usb_param->callback = NULL;

        msg_object->canvas_id = -1;
        return true;
    }

    return false;
}

static void _usb_select_box_callback( usb_select_box_t* usb_select_box, usb_select_box_sig_e signal)
{
    bool need_gen_plist = false;
    USB_MODE_T mode = USB_MODE_CHARGE;

    char launch_msg_content[MSG_CONTENT_SIZE] = {0};
    switch ( signal ) 
    {
        case WIDGET_SIG_USB_SEL_BOX_MTP:
        {
            sprintf(launch_msg_content, "%s %s", APP_NAME_USB, USB_ARGV_MTP);
            _request_launch_app(launch_msg_content);
            g_msgbox->callback(g_msgbox, WIDGET_SIG_MSGBOX_USB_LAUNCHED);
            
            mode = USB_MODE_UDISK_MTP;
            break;
        }

        case WIDGET_SIG_USB_SEL_BOX_DATA_TRANS:
        {
            sprintf(launch_msg_content, "%s %s", APP_NAME_USB, USB_ARGV_UDISK);
            _request_launch_app(launch_msg_content);
            g_msgbox->callback(g_msgbox, WIDGET_SIG_MSGBOX_USB_LAUNCHED);
            
            mode = USB_MODE_UDISK_MTP;
            break;
        }

        case WIDGET_SIG_USB_SEL_BOX_CHARGE:
        {
            sprintf(launch_msg_content, "%s %s", APP_NAME_USB, USB_ARGV_CHARGE);
            _request_launch_app(launch_msg_content);
            g_msgbox->callback(g_msgbox, WIDGET_SIG_MSGBOX_USB_LAUNCHED);
            
            break;
        }

        case WIDGET_SIG_USB_SEL_BOX_CHARGE_PLAY:
        {
            need_gen_plist = true;
            break;
        }

        case WIDGET_SIG_USB_SEL_BOX_QUIT:
        {
            need_gen_plist = true;
            break;
        }

        default:
        {
            break;
        }
    }	
    
    _set_monitor_mode( mode );

    _msgbox_quit_cur_msg_box();

    if(need_gen_plist == true)
    {
        _need_gen_plist();
    }
}

static void _set_monitor_mode( USB_MODE_T mode )
{
    int ret;
    int device_fd = -1;
    
    USB_MODE_T arg;
    arg = mode;

    device_fd = open( DEVICE_UMONITOR, O_RDWR, 0);
    if ( device_fd == -1 )
    {
        print_err(" open DEVICE_UMONITOR fd == %d", device_fd );
        return;
    }

    ret = ioctl( device_fd, UMONITOR_IOCTL_SET_USB_MODE, (int)&arg );
    if ( ret < 0 )
    {
        print_err(" ioctl error ! ");
    }

    close( device_fd );
}

static int  _op_usb_select_box_mtp_default_process(void * param)
{
    char launch_msg_content[MSG_CONTENT_SIZE] = {0};
    sprintf(launch_msg_content, "%s %s", APP_NAME_USB, USB_ARGV_MTP);
    _request_launch_app(launch_msg_content);
    g_msgbox->callback(g_msgbox, WIDGET_SIG_MSGBOX_USB_LAUNCHED);

    _msgbox_quit_cur_msg_box();

    return 0;
}

static int  _op_usb_select_box_data_trans_default_process(void * param)
{
    USB_MODE_T mode = USB_MODE_UDISK_MTP;

    char launch_msg_content[MSG_CONTENT_SIZE] = {0};

    sprintf(launch_msg_content, "%s %s", APP_NAME_USB, USB_ARGV_UDISK);

    _request_launch_app(launch_msg_content);

    g_msgbox->callback(g_msgbox, WIDGET_SIG_MSGBOX_USB_LAUNCHED);

    _set_monitor_mode( mode );

    _msgbox_quit_cur_msg_box();

    return 0;

}

static int  _op_usb_select_box_adapter_default_process(void * param)
{
    //char launch_msg_content[MSG_CONTENT_SIZE] = {0};
    //sprintf(launch_msg_content, "%s %s", APP_NAME_USB, USB_ARGV_CHARGE);
    //_request_launch_app(launch_msg_content);
    //g_msgbox->callback(g_msgbox, WIDGET_SIG_MSGBOX_USB_LAUNCHED);

    _msgbox_quit_cur_msg_box();
    
    _need_gen_plist();

    return 0;
}

static int _process_next_msg_timer_cb(void * param)
{
    _process_next_msg();
    kill_timer(s_process_next_timer_id);
    s_process_next_timer_id = -1;
    return 0;
}

static int _op_shortcut_camera_process(void * param)
{
    char launch_msg_content[MSG_CONTENT_SIZE] = {0};

    param = param;

    sprintf(launch_msg_content, "%s", APP_NAME_CAMERA);
    _request_launch_app(launch_msg_content);
    g_msgbox->callback(g_msgbox, WIDGET_SIG_MSGBOX_CAMERA_LAUNCHED);

    return 0;
}

static int _op_shortcut_playback_process(void * param)
{
    char launch_msg_content[MSG_CONTENT_SIZE] = {0};
    char path1[256] = {0};
    char path2[256] = {0};

    param = param;

    if(get_config(CFG_DSC_PLAYBACK_ROOT, path1, sizeof(path1)) < 0)
    {
        print_err("get_config(CFG_DSC_PLAYBACK_ROOT) error");
        return -1;
    }

    if(get_config(CFG_DSC_PLAYBACK_FULLNAME, path2, sizeof(path2)) < 0)
    {
        print_err("get_config(CFG_DSC_PLAYBACK_FULLNAME) error");
        return -1;
    }

    sprintf(launch_msg_content, "%s %s %s %s", APP_NAME_PHOTO, PHOTO_ARGV_PREVIEW, path1, path2);
    _request_launch_app(launch_msg_content);
    g_msgbox->callback(g_msgbox, WIDGET_SIG_MSGBOX_PHOTO_LAUNCHED);

    return 0;
}

static int _need_gen_plist(void)
{
    int value = 0;
    const app_info_t * app_info = NULL;
    char launch_msg_content[MSG_CONTENT_SIZE] = {0};
    hotplugin_dev_type_e main_disk;
    hotplugin_dev_info_t disk_info;

    app_info = app_running_list_get_this_app_info();

    main_disk = sys_get_main_disk();
    sys_get_hotplugin_dev_info(main_disk, &disk_info);

    if((main_disk == HOTPLUGIN_DEV_TYPE_NONE)
       || (disk_info.access != HOTPLUGIN_DEV_ACCESS_OK) )
    {
        value = 0;

        if(set_config(CFG_NEED_GEN_PLIST, (char *)&value, sizeof(value)) < 0)
        {
            print_err("set_config(CFG_NEED_GEN_PLIST) failed!");
        }

        return 0;
    }

    if((app_info != NULL) && ( strcmp( app_info->name , basename( APP_NAME_USB ) ) == 0 ) )
    {
        print_warning(" when the app is usb, return !");
        return 0;
    }

    if(get_config(CFG_IGNORE_GEN_PLIST, (char *)&value, sizeof(value)) >= 0)
    {
        if (value == 1)
        {
            print_warning(" ignore plist gen !");
            return 0;
        }
    }

    if(get_config(CFG_NEED_GEN_PLIST, (char *)&value, sizeof(value)) < 0)
    {
        print_err("get_config(CFG_NEED_GEN_PLIST) failed!");
        value = 0;
    }

    if(value == 1)
    {
        sprintf(launch_msg_content, "%s", APP_NAME_PL_GEN);
        _request_launch_app(launch_msg_content);
        g_msgbox->callback(g_msgbox, WIDGET_SIG_MSGBOX_PL_GEN_LAUNCHED);
    }

    return 0;
}

static msgbox_msg_object_t * _create_msg_object(msg_apps_t * pmsg)
{
    msgbox_msg_object_t * msg_object = NULL;
    msgbox_show_type_dialog_param_t * dialog_param = NULL;
    msgbox_show_type_usb_select_box_param_t * usb_param = NULL;

    msg_object = (msgbox_msg_object_t *)malloc(sizeof(msgbox_msg_object_t));
    if(msg_object == NULL)
    {
        print_err("malloc failed!");
        return NULL;
    }

    memset(msg_object, 0, sizeof(msgbox_msg_object_t));

    msg_object->next = NULL;
    msg_object->param = NULL;
    msg_object->canvas_id = -1;
    msg_object->default_process_timer_id = -1;

    /* default  msg priority */
    msg_object->prior = MSGBOX_MSG_PRIO_0;

    if( (pmsg->type == MSG_SHORTCUT_TVOUT) 
        || (pmsg->type == MSG_SHORTCUT_CAMERA) 
        || (pmsg->type == MSG_SHORTCUT_PLAYBACK) 
        || (pmsg->type == MSG_SHORTCUT_DELETE) 
        || (pmsg->type == MSG_NEED_GEN_PLIST) ) 
    {
        msg_object->show_type = MSGBOX_SHOW_TYPE_NONE;
    }
    else if( (pmsg->type == MSG_USB_B_IN) || (pmsg->type == MSG_USB_ADAPTOR_IN))
    {
        msg_object->show_type = MSGBOX_SHOW_TYPE_USB_SELECT_BOX;

        msg_object->param = (void *)malloc(sizeof(msgbox_show_type_usb_select_box_param_t));

        if(msg_object->param == NULL)
        {
            print_err("malloc failed!");
            goto malloc_err;
        }

        memset(msg_object->param, 0, sizeof(msgbox_show_type_usb_select_box_param_t));

        msg_object->op.create = _op_usb_select_box_create;
        msg_object->op.delete = _op_usb_select_box_delete;
        msg_object->op.paint = _op_usb_select_box_paint;
        msg_object->op.proc_gui = _op_usb_select_box_proc_gui;
        msg_object->op.load_style = _op_usb_select_box_load_style;

        usb_param = (msgbox_show_type_usb_select_box_param_t *)(msg_object->param);
        usb_param->callback = _usb_select_box_callback; 
    }
    else
    {
        msg_object->show_type = MSGBOX_SHOW_TYPE_DIALOG;

        msg_object->param = (void *)malloc(sizeof(msgbox_show_type_dialog_param_t));

        if(msg_object->param == NULL)
        {
            print_err("malloc failed!");
            goto malloc_err;
        }

        memset(msg_object->param, 0, sizeof(msgbox_show_type_dialog_param_t));

        msg_object->op.create = _op_dialog_create;
        msg_object->op.delete = _op_dialog_delete;
        msg_object->op.paint = _op_dialog_paint;
        msg_object->op.proc_gui = _op_dialog_proc_gui;
        msg_object->op.load_style = _op_dialog_load_style;
        msg_object->op.default_process = _op_dialog_common_default_process;

        dialog_param = (msgbox_show_type_dialog_param_t *)(msg_object->param);
        dialog_param->callback = _dialog_common_callback; 
    }

    switch (pmsg->type)
    {
        case MSG_FMENGINE_EARPHONE_NOT_INSERT:
        {
            msg_object->msg_type = MSGBOX_MSG_TYPE_EARPHONE_NOT_INSERT;
            msg_object->prior = MSGBOX_MSG_PRIO_2;

            dialog_param->dialog_init.head = DIALOG_HEAD_QUERY;
            dialog_param->dialog_init.str_id = str_res_table[STR_NAME_NO_EARPHONE].str_id;
            dialog_param->dialog_init.def_sel=WIDGET_SIG_DIALOG_CANCAL;

            dialog_param->callback = _dialog_earphone_not_insert_callback; 
            msg_object->op.default_process = _op_dialog_earphone_not_insert_default_process;

            break;
        }

        case MSG_FMENGINE_LINEIN_INSERT:
        {
            msg_object->msg_type = MSGBOX_MSG_TYPE_LINE_IN_INSERT;
            msg_object->prior = MSGBOX_MSG_PRIO_1;

            dialog_param->dialog_init.head = DIALOG_HEAD_QUERY;
            dialog_param->dialog_init.str_id = str_res_table[STR_NAME_LINE_IN_INSERT].str_id;
            dialog_param->dialog_init.def_sel=WIDGET_SIG_DIALOG_CANCAL;

            dialog_param->callback = _dialog_line_in_insert_callback; 
            msg_object->op.default_process = _op_dialog_line_in_insert_default_process;

            break;
        }

        case MSG_LOW_VOLTAGE :
        {
            msg_object->msg_type = MSGBOX_MSG_TYPE_LOW_VOLTAGE;
            msg_object->prior = MSGBOX_MSG_PRIO_5;

            dialog_param->dialog_init.head = DIALOG_HEAD_LOWBAT;
            dialog_param->dialog_init.str_id = str_res_table[STR_NAME_LOW_POWER].str_id;

            break;
        }
        case MSG_KEYLOCK :
        {
            msg_object->msg_type = MSGBOX_MSG_TYPE_KEYLOCK;
            msg_object->prior = MSGBOX_MSG_PRIO_0;

            dialog_param->dialog_init.head = DIALOG_HEAD_KEYLOCK;
            dialog_param->dialog_init.str_id = str_res_table[STR_NAME_KEY_LOCK].str_id;

            break;
        }
        case MSG_USB_B_IN :
        {
            char connect_type[512] = {0};
            msg_object->msg_type = MSGBOX_MSG_TYPE_USB_B_IN;
            msg_object->prior = MSGBOX_MSG_PRIO_3;

            if(get_config(CFG_CONNECT_TYPE, connect_type, sizeof(connect_type)) < 0)
            {
                print_warning("get_config(CFG_CONNECT_TYPE) failed! ");
                strncpy(connect_type, "UDISK", 512);
            }

            if(strcmp(connect_type, "MTP") == 0)
            {
                usb_param->usb_select_box_type = USB_SELECT_BOX_MTP;
                msg_object->op.default_process = _op_usb_select_box_mtp_default_process;
            }
            else
            {
                usb_param->usb_select_box_type = USB_SELECT_BOX_DATA_TRANS;
                msg_object->op.default_process = _op_usb_select_box_data_trans_default_process;
            }

            break;
        }
        case MSG_USB_ADAPTOR_IN :
        {
            msg_object->msg_type = MSGBOX_MSG_TYPE_USB_ADAPTER_IN;
            msg_object->prior = MSGBOX_MSG_PRIO_3;

            usb_param->usb_select_box_type = USB_SELECT_BOX_ADAPTER;
            msg_object->op.default_process = _op_usb_select_box_adapter_default_process;

            break;
        }
        case MSG_USB_HOST_DISK_IN :
        {
            msg_object->msg_type = MSGBOX_MSG_TYPE_USB_HOST_DISK_IN;
            msg_object->prior = MSGBOX_MSG_PRIO_0;

            dialog_param->dialog_init.head = DIALOG_HEAD_INFO;
            dialog_param->dialog_init.str_id = str_res_table[STR_NAME_UHOST_IN].str_id;

            break;
        }
        case MSG_USB_HOST_DISK_INIT_FAILED:
        {
            msg_object->msg_type = MSGBOX_MSG_TYPE_USB_HOST_DISK_INIT_FAILED;
            msg_object->prior = MSGBOX_MSG_PRIO_0;

            dialog_param->dialog_init.head = DIALOG_HEAD_FAIL;
            dialog_param->dialog_init.str_id = str_res_table[STR_NAME_EXTERNAL_INIT_FAILED].str_id;

            break;
        }
        case MSG_USB_HOST_DISK_OUT :
        {
            msg_object->msg_type = MSGBOX_MSG_TYPE_USB_HOST_DISK_OUT;
            msg_object->prior = MSGBOX_MSG_PRIO_0;

            dialog_param->dialog_init.head = DIALOG_HEAD_INFO;
            dialog_param->dialog_init.str_id = str_res_table[STR_NAME_UHOST_OUT].str_id;

            break;
        }
        case MSG_CARD_IN :
        {
            msg_object->msg_type = MSGBOX_MSG_TYPE_CARD_IN;
            msg_object->prior = MSGBOX_MSG_PRIO_0;

            dialog_param->dialog_init.head = DIALOG_HEAD_CARDIN;
            dialog_param->dialog_init.str_id = str_res_table[STR_NAME_CARD_IN].str_id;

            break;
        }
        case MSG_CARD_INIT_FAILED:
        {
            msg_object->msg_type = MSGBOX_MSG_TYPE_CARD_INIT_FAILED;
            msg_object->prior = MSGBOX_MSG_PRIO_0;

            dialog_param->dialog_init.head = DIALOG_HEAD_FAIL;
            dialog_param->dialog_init.str_id = str_res_table[STR_NAME_CARD_INIT_FAILED].str_id;

            break;
        }
        case MSG_CARD_MOUNT_FAILED:
        {
            msg_object->msg_type = MSGBOX_MSG_TYPE_CARD_MOUNT_FAILED;
            msg_object->prior = MSGBOX_MSG_PRIO_0;

            dialog_param->dialog_init.head = DIALOG_HEAD_FAIL;
            dialog_param->dialog_init.str_id = str_res_table[STR_NAME_CARD_MOUNT_FAILED].str_id;

            break;
        }
        case MSG_CARD_OUT :
        {
            msg_object->msg_type = MSGBOX_MSG_TYPE_CARD_OUT;
            msg_object->prior = MSGBOX_MSG_PRIO_0;

            dialog_param->dialog_init.head = DIALOG_HEAD_CARDOUT;
            dialog_param->dialog_init.str_id = str_res_table[STR_NAME_CARD_OUT].str_id;

            break;
        }
        case MSG_TVIN_ALARM_IN :
        {
            int clock_no;
            memcpy(&clock_no, pmsg->content, sizeof(clock_no));

            msg_object->msg_type = MSGBOX_MSG_TYPE_TVIN_ALARM_IN;
            msg_object->prior = MSGBOX_MSG_PRIO_5;

            dialog_param->dialog_init.head = DIALOG_HEAD_QUERY;
            dialog_param->dialog_init.str_id = str_res_table[STR_NAME_ALARM_IN].str_id;
            dialog_param->dialog_init.def_sel=WIDGET_SIG_DIALOG_CANCAL;

            dialog_param->callback = _dialog_alarm_tvin_callback; 
            msg_object->op.default_process = _op_dialog_alarm_tvin_default_process;

            sprintf(s_launch_tvin_msg_content, "%s %s %d", APP_NAME_TVIN, TVIN_ARGV_ALARM, clock_no);

            break;
        }

        case MSG_SHORTCUT_CAMERA:
        {
            msg_object->msg_type = MSGBOX_MSG_TYPE_SHORTCUT_CAMERA;
            msg_object->prior = MSGBOX_MSG_PRIO_4;
            msg_object->op.process = _op_shortcut_camera_process;
            break;
        }

        case MSG_SHORTCUT_PLAYBACK:
        {
            msg_object->msg_type = MSGBOX_MSG_TYPE_SHORTCUT_PLAYBACK;
            msg_object->prior = MSGBOX_MSG_PRIO_4;
            msg_object->op.process = _op_shortcut_playback_process;
            break;
        }

        default :
        {
            if(msg_object->param != NULL)
            {
                free(msg_object->param);
            }

            free(msg_object);
            msg_object = NULL;

            break;
        }
    }

    return msg_object;

    /* <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<< goto! <<<<<<<<<<<<<<<< goto! <<<<<<<< goto! <<<< goto! << */
    malloc_err:
    free(msg_object);

    return NULL;
}

static bool _delete_msg_object(msgbox_msg_object_t * msg_object)
{
    if(msg_object != NULL)
    {
        if(msg_object->param != NULL)
        {
            free(msg_object->param);
        }

        free(msg_object);

        return true;
    }

    return false;
}

static bool _msg_queue_append(msgbox_msg_object_t ** ppqueue, msgbox_msg_object_t * msg_object)
{
    msgbox_msg_object_t * tail = NULL;
    print_info();

    if(NULL == *ppqueue)
    {
        *ppqueue = msg_object;
        tail = msg_object;
    }
    else
    {
        tail = *ppqueue;
        while(tail->next != NULL) 
        {
            tail = tail->next;
        }

        tail->next = msg_object;
        tail = tail->next;
    }

    //_msg_queue_print(*ppqueue);

    return true;
}

static msgbox_msg_object_t * _msg_queue_fetch(msgbox_msg_object_t ** ppqueue)
{
    msgbox_msg_object_t * node;

    if(*ppqueue != NULL)
    {
        node = *ppqueue;
        *ppqueue = (*ppqueue)->next;

        //_msg_queue_print(*ppqueue);

        return node;
    }

    return NULL;
}

static bool _msg_queue_delete(msgbox_msg_object_t ** ppqueue, msgbox_msg_type_e type)
{
    msgbox_msg_object_t * cur_node = *ppqueue;
    msgbox_msg_object_t * prev_node = *ppqueue;

    while(cur_node != NULL) 
    {
        if(cur_node->msg_type == type)
        {
            if(prev_node == cur_node)
            {
                *ppqueue = (*ppqueue)->next;
                prev_node = *ppqueue;
                _delete_msg_object(cur_node);
                cur_node = *ppqueue;
            }
            else
            {
                prev_node->next = cur_node->next;
                _delete_msg_object(cur_node);
                cur_node = prev_node->next;
            }
        }
        else
        {
            prev_node = cur_node;
            cur_node = cur_node->next;
        }
    }

    //_msg_queue_print(*ppqueue);

    return true;
}

static void _msg_queue_clear(msgbox_msg_object_t ** ppqueue)
{
    msgbox_msg_object_t * list;
    msgbox_msg_object_t * node;

    list = *ppqueue;

    while( list != NULL) 
    {
        node = list;
        list = list->next;
        _delete_msg_object(node);
    }

    //_msg_queue_print(*ppqueue);
}

static void _msg_queue_print(msgbox_msg_object_t * pqueue)
{
    msgbox_msg_object_t* node = pqueue;

    printf("\n##### msg queue #####\n");
    printf("\tmsg type \t show type \t prior\t\n");
    while(node != NULL) 
    {
        printf("\t%-8d\t%-8d\t%-8d\n", node->msg_type
                , node->show_type
                , node->prior);

        node = node->next;
    }
}

static bool _msg_filter_check(msg_apps_type_e type)
{
    msg_filter_list_t * node = s_msg_filter_list;

    while(node != NULL) 
    {
        if(node->type == type)
        {
            return true;
        }

        node = node->next;
    }

    return false;
}

static bool _msg_filter_list_add(msg_apps_type_e type)
{
    msg_filter_list_t * new_node;

    if(_msg_filter_check(type) == true)
    {
        return true;
    }

    new_node = (msg_filter_list_t *) malloc ( sizeof(msg_filter_list_t));
    if( new_node == NULL )
    {
        print_err("malloc failed!");
        return false;
    }

    new_node->type = type;
    new_node->next = s_msg_filter_list;
    s_msg_filter_list = new_node;
    //_msg_filter_list_print();

    return true;
}

static bool _msg_filter_list_delete(msg_apps_type_e type)
{
    msg_filter_list_t * cur_node = s_msg_filter_list;
    msg_filter_list_t * prev_node = s_msg_filter_list;

    while(cur_node != NULL) 
    {
        if(cur_node->type == type)
        {
            if(prev_node == cur_node)
            {
                s_msg_filter_list = s_msg_filter_list->next;
                prev_node = s_msg_filter_list;
                free(cur_node);
                cur_node = s_msg_filter_list;
            }
            else
            {
                prev_node->next = cur_node->next;
                free(cur_node);
                cur_node = prev_node->next;
            }
        }
        else
        {
            prev_node = cur_node;
            cur_node = cur_node->next;
        }
    }

    //_msg_filter_list_print();

    return true;
}

static bool _msg_filter_list_clear(void)
{
    msg_filter_list_t * node;

    while(s_msg_filter_list != NULL) 
    {
        node = s_msg_filter_list;
        s_msg_filter_list = s_msg_filter_list->next;
        free(node);
    }

    //_msg_filter_list_print();

    return true;
}

static void _msg_filter_list_print(void)
{
    msg_filter_list_t * node = s_msg_filter_list;

    printf("\n##### msg filter list #####\n");
    while(node != NULL) 
    {
        printf("\tmsg type:%d\n", node->type);
        node = node->next;
    }
}

static void  _request_launch_app(const char * app_name)
{
    msg_apps_t msg = {0};

    if(app_name == NULL)
    {
        print_err("input parameter(app_name) is a NULL pointer");
        return;
    }

    msg.type = MSG_KILL_USER_APPS_CREAT_APP;
    strncpy(msg.content, app_name, MSG_CONTENT_SIZE);

    print_info("send_async_msg:(%d, %s)", msg.type, msg.content);
    send_async_msg(APP_NAME_MANAGER, &msg); 
}
