/********************************************************************************
 *                              USDK(1100)
 *                             Module: app
 *                 Copyright(c) 2003-2009 Actions Semiconductor,
 *                            All Rights Reserved.
 * File : tvin_scene_record.c
 * History:
 *      <author>    <time>           <version >             <desc>
 *      Jerry Fu    2009-4-28             v1.0               build this file
 ********************************************************************************/
/*!
* \file     tvin_scene_record.c
* \brief    tvin  record scene
* \author   Jerry Fu
*
* \version 1.0
* \date  2009/04/28
*******************************************************************************/
#include "tvin.h"
#include "tvin_record_ui.h"

#define   TVIN_TIMER_INTERVAL                     (500)  /*每500毫秒检查一次录制的状态*/

#define   PLAY_STATUS_REFRESH_INTERVAL            (1000) /*UI界面每秒刷新一次*/
#define   UI_AUTO_HIDE_INTERVAL                   (3000) /*3s自动隐藏ui界面*/

#define   RECORD_MANUALLY                         (8)  /*手动录制*/


static  tvin_scene_record_t*   s_scene_record = NULL;
static  tvin_appdata_t  *tvin_appdata = NULL;

static void  _msgbox_cb(msgbox_t *msg_box, msgbox_sig_e  msgbox_type);
static bool _record_create_msgbox(void);
static bool _record_delete_msgbox(void);

static int _record_timer_proc(void *param);
static void _tvin_set_timer(void);
static void _tvin_kill_timer(void);

static void  _restore_config_data(void);

static bool _scene_tvin_record_open_theme(void);
static bool _scene_tvin_record_close_theme(void);
static bool _scene_tvin_record_open_str_res(void);
static bool _scene_tvin_record_close_str_res(void);

static bool _load_tvin_record_style(tvin_scene_record_t *pTvinRecord);

static void  _tvin_scene_key_proc(key_event_t *kmsg);
static void _scene_tvin_callback(gui_wm_message_t *pmsg);
static bool _scene_tvin_record_init( tvin_appdata_t *app_data );
static bool _scene_tvin_deinit(tvin_appdata_t *app_data);


tvin_scene_record_t *fetch_tvin_record(void)
{
    return s_scene_record;
}


void tvin_scene_record_paint(tvin_scene_record_t *scene_record)
{
    scene_record->hdc = gui_dc_get((unsigned short)(scene_record->win_id));
    if(false == scene_record->first_draw)
    {
        gui_dc_clear(scene_record->hdc);     
    }
   
#if   TEST_MMMM_DV
    /*根据需求去掉进度条的显示，把剩余时间叠加到已录时间上*/
    //根据需求去掉进度条的显示，把剩余时间叠加到已录时间上
    //显示成:00:00:00 / 01:30:59
    tvin_draw_status(scene_record);
    //tvin_draw_pbar_bg(scene_record);
    tvin_draw_recordtime(scene_record);//里面已叠加了剩余时间
    //tvin_draw_remaintime(scene_record);
    //tvin_draw_pbar(scene_record);
#endif  
    scene_record->first_draw = true;
}



static void  _msgbox_cb(msgbox_t *msg_box, msgbox_sig_e  msgbox_type)
{
    string_resource_t *str_dialog_info;
    bool ret_result;
    switch(msgbox_type)
    {
    case WIDGET_SIG_MSGBOX_SHOW:
        print_dbg("message box will be show\n");
        tvin_msgbox_blending_create_show();
        break;
    
    case WIDGET_SIG_MSGBOX_QUIT:
        print_dbg("message box will be quit\n");
        tvin_msgbox_blending_delete();
        break;
        
    case WIDGET_SIG_MSGBOX_SHOW_LOW_POWER:
        break;

    case WIDGET_SIG_MSGBOX_QUIT_LOW_POWER:
        if((s_scene_record->tvin_record_sta == TVIN_RECORD_PLAY) ||
           (s_scene_record->tvin_record_sta == TVIN_RECORD_PAUSE))
        {
            tvin_stop_record_file();
            s_scene_record->tvin_record_sta = TVIN_RECORD_STOP;
            s_scene_record->record_status_is_show = false;
            tvin_scene_record_paint(s_scene_record);
            str_dialog_info = (string_resource_t *)get_resgroup_child(s_scene_record->group, STR_SAVE_RECORD_FILE);
            if(str_dialog_info != NULL)
            {
                ret_result = tvin_create_dialog(str_dialog_info->str_id, DIALOG_HEAD_INFO);
                if (true == ret_result)
                {
                    dialog_paint(s_scene_record->dialog);
                } 
            }
        }
        break;
    
    default:
        break;
    }
}


static bool _record_create_msgbox(void)
{
    tvin_scene_record_t *prec;
    prec = fetch_tvin_record();
    if(NULL == prec)
    {
        return false;
    }
    
    s_scene_record->msgbox = msgbox_create(_msgbox_cb);
    if (NULL == s_scene_record->msgbox)
    {
        print_err("create msgbox failed!");
        return false;
    }
    return true;
}


static bool _record_delete_msgbox(void)
{
    if(s_scene_record->msgbox != NULL)
    {
        msgbox_delete(s_scene_record->msgbox);
        s_scene_record->msgbox = NULL;
    }    
    return true;
}


static void  _show_ui_during_recording(void)
{
    tvin_scene_record_t *prec;
    prec = fetch_tvin_record();
    if(NULL == prec)
    {
        return;
    }
    
    tvin_record_blending_ui_show();
    prec->record_status_is_show = false;
    prec->remain_time_is_show = false;
    prec->record_time_is_show = false;
    tvin_scene_record_paint(prec);
}
    
static int _record_timer_proc(void *param)
{   
    static unsigned int tick_number = 0;
    tvin_scene_record_t *prec = s_scene_record;
    mmm_dv_status_t record_status;
    mmm_disk_info_t disk_info;
    
    tick_number++;      
    switch(s_scene_record->tvin_record_sta)
    {
    case TVIN_RECORD_PLAY:
    case TVIN_RECORD_PLAYHL:
        if(true == tvin_get_recorder_status(&record_status))
        {
            switch(record_status.status)
            {
            case MMM_RECORDER_RECORDING:
                break;

            case MMM_RECORDER_PAUSED:
                prec->tvin_record_sta = TVIN_RECORD_PAUSE;
                _show_ui_during_recording();
                break;
            
            case MMM_RECORDER_ERROR:   
                print_err("mmm_recorder_error!");
                tvin_stop_record_file();

                
            case MMM_RECORDER_STOPPED:
                prec->tvin_record_sta = TVIN_RECORD_STOP;                
                _show_ui_during_recording();
                return 1;
                
            default:
                break;
            }
            
            prec->cur_record_time = record_status.time;
        }
        
        if(0 == prec->record_remain_time)
        {              
            tvin_stop_record_file();            
            prec->tvin_record_sta = TVIN_RECORD_STOP;

            //如果磁盘已满，则提示"空间满"
            if (true == tvin_create_dialog(s_scene_record->str_disk_full->str_id, DIALOG_HEAD_WARN))
            {
                dialog_paint(prec->dialog);
            }  
            _show_ui_during_recording();
        }
        else
        {
            if((tick_number%(PLAY_STATUS_REFRESH_INTERVAL/TVIN_TIMER_INTERVAL)) == 0)
            {
                if(true == tvin_get_disk_info(&disk_info))
                {
                    prec->record_remain_time = disk_info.info;
                }
            }
            if(prec->blending.show_blending_hbar == true)
            {                                                   
                prec->record_time_is_show = false;
                prec->remain_time_is_show = false;
                //prec->record_status_is_show = false;                   
                if(prec->time_relate.hide_ui_time != 0)
                {                           
                    prec->time_relate.hide_ui_time--;
                    if(prec->time_relate.hide_ui_time == 0)
                    {                       
                        tvin_record_blending_ui_hide();           
                    }               
                    else
                    {                                                           
                        tvin_scene_record_paint(prec);
                    }
                }                    
            }
            /*如果是定时录像，定时时长到则停止录像*/
            if((prec->alarm_len_time!=0) && (prec->cur_record_time>=prec->alarm_len_time))
            {
                tvin_stop_record_file();            
                prec->tvin_record_sta = TVIN_RECORD_STOP;
                _show_ui_during_recording();
            }
        }
        break;

    case TVIN_RECORD_STOP:
    case TVIN_RECORD_STOPHL:        
    case TVIN_RECORD_PAUSE:
    case TVIN_RECORD_PAUSEHL:        
        break;
        
    default:
        break;
    }                                
    return 1;
}


static void _tvin_set_timer(void)
{
    /*由于tvin定时器不可停止，所以用inner_timer*/
    s_scene_record->timer_id = set_inner_timer(TVIN_TIMER_INTERVAL, _record_timer_proc, NULL); 
}


static void _tvin_kill_timer(void)
{
    /*由于tvin定时器不可停止，所以用inner_timer*/
    kill_inner_timer(s_scene_record->timer_id);  
}



static void  _restore_config_data(void)
{
    unsigned int temp;    
    rtc_date_t *pdate;
    rtc_time_t *ptime;
    int i;
    int cmp_res;

    //s_scene_record->tvin_install = false;
    s_scene_record->first_draw = 0;
    s_scene_record->headbar_is_show = false;
    s_scene_record->pbar_bg_is_show = false;
    s_scene_record->record_status_is_show = false;
    s_scene_record->record_time_is_show = false;
    s_scene_record->remain_time_is_show = false;
    s_scene_record->record_statusbg_is_show = false;

    s_scene_record->time_relate.hide_ui_time = UI_AUTO_HIDE_INTERVAL/TVIN_TIMER_INTERVAL;

    //restore save path
    s_scene_record->tvin_config.save_path = g_tvin_cfgs[TVIN_CFG_SAVEPATH].value;

    //restore record quality
    s_scene_record->tvin_config.record_quality = g_tvin_cfgs[TVIN_CFG_RECORD_QUALITY].value;


    //restore tvin signal mode  pal or ntsc
    temp = strlen(g_tvin_cfgs[TVIN_CFG_TVIN_SIGNAL].text);
    if(temp != 0)
    {
        cmp_res = strcmp(g_tvin_cfgs[TVIN_CFG_TVIN_SIGNAL].text, "PAL");
        if(cmp_res == 0)
        {
            s_scene_record->tvin_config.tv_signal = ZONE_PALD;
            set_tvin_mode(ZONE_PALD);
        }
        else
        {
            s_scene_record->tvin_config.tv_signal = ZONE_NTSC;
            set_tvin_mode(ZONE_NTSC);
        }        
    }
    
    //restore lcd on off status
    temp = strlen(g_tvin_cfgs[TVIN_CFG_LCD_ONOFF].text);
    if(temp != 0)
    {
        cmp_res = strcmp(g_tvin_cfgs[TVIN_CFG_LCD_ONOFF].text, "LCD_ON");
        if(cmp_res == 0)
        {
            s_scene_record->tvin_config.lcd_on_off = ADJUST_ON;
            sys_forbid_screensaver(true);
        }
        else
        {
            s_scene_record->tvin_config.lcd_on_off = ADJUST_OFF;
            sys_forbid_screensaver(false);
        }        
    }        
     
    //restore single1 start date
    for(i=0;i<3;i++)
    {
        temp = strlen(g_tvin_cfgs[TVIN_CFG_SINGLE1_DATE+i].text);
        if(temp != 0)
        {
            pdate = &(s_scene_record->tvin_config.single_record[i].start_date);
            sscanf(g_tvin_cfgs[TVIN_CFG_SINGLE1_DATE+i].text, "%d-%d-%d",
                &(pdate->year),&(pdate->month),&(pdate->day));
        }
    }         

    //restore single start time
    for(i=0;i<3;i++)
    {
        temp = strlen(g_tvin_cfgs[TVIN_CFG_SINGLE1_TIME+i].text);
        if(temp != 0)
        {
            ptime = &(s_scene_record->tvin_config.single_record[i].start_time);
            sscanf(g_tvin_cfgs[TVIN_CFG_SINGLE1_TIME+i].text, "%d:%d",
                &(ptime->hour),&(ptime->minute));
        }
    }

    //restore single record length
    for(i=0;i<3;i++)
    {
        temp = strlen(g_tvin_cfgs[TVIN_CFG_SINGLE1_LENGTH+i].text);
        if(temp != 0)
        {
            ptime = &(s_scene_record->tvin_config.single_record[i].record_time);
            sscanf(g_tvin_cfgs[TVIN_CFG_SINGLE1_LENGTH+i].text, "%d:%d",
                &(ptime->hour),&(ptime->minute));
        }
    }

    //restore single record timer on off status
    for(i=0; i<3; i++)
    {
        temp = strlen(g_tvin_cfgs[TVIN_CFG_SINGLE1_ONOFF+i].text);
        if(temp != 0)
        {
            cmp_res = strcmp(g_tvin_cfgs[TVIN_CFG_SINGLE1_ONOFF+i].text, "ON");
            if(0 == cmp_res)
            {
                s_scene_record->tvin_config.single_record[i].onoff = true;
            }
            else
            {
                s_scene_record->tvin_config.single_record[i].onoff = false;
            }
        }
    }

    
    //resotore repeat start time
    for(i=0;i<3;i++)
    {
        temp = strlen(g_tvin_cfgs[TVIN_CFG_REPEAT1_TIME+i].text);
        if(temp != 0)
        {
            ptime = &(s_scene_record->tvin_config.repeat_record[i].start_time);
            sscanf(g_tvin_cfgs[TVIN_CFG_REPEAT1_TIME+i].text, "%d:%d",
                &(ptime->hour),&(ptime->minute));
        }
    }

    //restore repeat record length
    for(i=0;i<3;i++)
    {
        temp = strlen(g_tvin_cfgs[TVIN_CFG_REPEAT1_LENGTH+i].text);
        if(temp != 0)
        {
            ptime = &(s_scene_record->tvin_config.repeat_record[i].record_time);
            sscanf(g_tvin_cfgs[TVIN_CFG_REPEAT1_LENGTH+i].text, "%d:%d",
                &(ptime->hour),&(ptime->minute));
        }
    }

    //restore repeat week days
    for(i=0;i<3;i++)
    {
        s_scene_record->tvin_config.repeat_record[i].repeat_day_flag = \
            (unsigned char)(g_tvin_cfgs[TVIN_CFG_REPEAT1_WEEKDAY+i].value);

    }

    //restore repeat record timer on off status
    for(i=0; i<3; i++)
    {
        temp = strlen(g_tvin_cfgs[TVIN_CFG_REPEAT1_ONOFF+i].text);
        if(temp != 0)
        {
            cmp_res = strcmp(g_tvin_cfgs[TVIN_CFG_REPEAT1_ONOFF+i].text, "ON");
            if(0 == cmp_res)
            {
                s_scene_record->tvin_config.repeat_record[i].onoff = true;
            }
            else
            {
                s_scene_record->tvin_config.repeat_record[i].onoff = false;
            }
        }
    }

    //restore alarm clock number
    for(i=0; i<6; i++)
    {        
        s_scene_record->tvin_config.alarm_clknum[i] = g_tvin_cfgs[TVIN_CFG_SINGLE1_CLKNUM+i].value;       
        print_dbg("%d: clock num=%d\n",i,s_scene_record->tvin_config.alarm_clknum[i]);
    }
}


static void  _tvin_scene_key_proc(key_event_t *kmsg)
{
    bool ret_result;
    string_resource_t *str_dialog_info;
    
    if((s_scene_record->tvin_record_sta == TVIN_RECORD_PLAY) ||
        (s_scene_record->blending.show_blending_hbar == false))
    {      
        s_scene_record->record_time_is_show = false;
        s_scene_record->remain_time_is_show = false;
        tvin_scene_record_paint(s_scene_record);
        gui_screen_update();
         
        tvin_record_blending_ui_show();
        s_scene_record->time_relate.hide_ui_time = UI_AUTO_HIDE_INTERVAL/TVIN_TIMER_INTERVAL;
    }
      
    switch ( kmsg->val | kmsg->type)
    {        
    case KEY_OK | KEY_TYPE_SHORT_UP:
        switch(s_scene_record->tvin_record_sta)
        {
        case TVIN_RECORD_STOP:
            s_scene_record->record_status_is_show = false;
            s_scene_record->time_relate.hide_ui_time = UI_AUTO_HIDE_INTERVAL/TVIN_TIMER_INTERVAL;

            if(false == sys_power_threshold_check(POWER_THRESHOLD_TVIN_REC))
            {
                str_dialog_info = (string_resource_t *)get_resgroup_child(s_scene_record->group, STR_TV_LOW_POWER);
                if(str_dialog_info != NULL)
                {
                    if(true == tvin_create_dialog(str_dialog_info->str_id, DIALOG_HEAD_LOWBAT))
                    {
                        dialog_paint(s_scene_record->dialog);
                    }
                }
                break;
            }

            if(true == tvin_start_record_file(RECORD_MANUALLY))
            {
                s_scene_record->tvin_record_sta = TVIN_RECORD_STOPHL;
                s_scene_record->record_status_is_show = false;
                s_scene_record->pbar_bg_is_show = false;
                tvin_scene_record_paint(s_scene_record);
                gui_screen_update();
                usleep(50000);
                
                s_scene_record->tvin_record_sta = TVIN_RECORD_PLAY;
                s_scene_record->record_status_is_show = false;
                tvin_scene_record_paint(s_scene_record);
            }
            break;

        case TVIN_RECORD_PAUSE:
            s_scene_record->tvin_record_sta = TVIN_RECORD_PAUSEHL;
            s_scene_record->record_status_is_show = false;
            tvin_scene_record_paint(s_scene_record);
            gui_screen_update();
            usleep(50000);
            
            s_scene_record->tvin_record_sta = TVIN_RECORD_PLAY;
            s_scene_record->time_relate.hide_ui_time = UI_AUTO_HIDE_INTERVAL/TVIN_TIMER_INTERVAL;
            s_scene_record->record_status_is_show = false;
            tvin_scene_record_paint(s_scene_record);
            tvin_resume_record();            
            break;

        case TVIN_RECORD_PLAY:
            s_scene_record->tvin_record_sta = TVIN_RECORD_PLAYHL;
            s_scene_record->record_status_is_show = false;
            tvin_scene_record_paint(s_scene_record);
            gui_screen_update();
            usleep(50000);
        
            s_scene_record->tvin_record_sta = TVIN_RECORD_PAUSE;
            s_scene_record->record_status_is_show = false;
            tvin_pause_record();
            tvin_scene_record_paint(s_scene_record);
            break;
            
        case TVIN_RECORD_STOPHL:
        case TVIN_RECORD_PAUSEHL:
        case TVIN_RECORD_PLAYHL:
            break;
            
        default:
            break;
        }
        //clear key
        clear_key_msg_queue(-1,-1);
        break;
        
    case  KEY_MENU | KEY_TYPE_DOWN:
        switch(s_scene_record->tvin_record_sta)
        {
        case TVIN_RECORD_PLAY:
        case TVIN_RECORD_PAUSE:
            tvin_stop_record_file();
            s_scene_record->tvin_record_sta = TVIN_RECORD_STOP;
            s_scene_record->record_status_is_show = false;
            tvin_scene_record_paint(s_scene_record);
            str_dialog_info = (string_resource_t *)get_resgroup_child(s_scene_record->group, STR_SAVE_RECORD_FILE);
            if(str_dialog_info != NULL)
            {
                ret_result = tvin_create_dialog(str_dialog_info->str_id, DIALOG_HEAD_INFO);
                if (true == ret_result)
                {
                    dialog_paint(s_scene_record->dialog);
                }
            }
            break;

        case TVIN_RECORD_STOP:        
            s_scene_record->pRecMView = tvin_widget_option_create();
            break;

        case TVIN_RECORD_PLAYHL:
        case TVIN_RECORD_PAUSEHL:
        case TVIN_RECORD_STOPHL:
            break;

        default:
            break;
        }
        break;

    case KEY_RETURN | KEY_TYPE_SHORT_UP:
        switch(s_scene_record->tvin_record_sta)
        {
        case TVIN_RECORD_PAUSE:
        case TVIN_RECORD_PLAY:
            s_scene_record->next_scene = SCENE_TVIN_PALNTSC; 
            tvin_stop_record_file();
            s_scene_record->tvin_record_sta = TVIN_RECORD_STOP;
            s_scene_record->record_status_is_show = false;
            tvin_scene_record_paint(s_scene_record);
            str_dialog_info = (string_resource_t *)get_resgroup_child(s_scene_record->group, STR_SAVE_RECORD_FILE);
            if(str_dialog_info != NULL)
            {
                ret_result = tvin_create_dialog(str_dialog_info->str_id, DIALOG_HEAD_INFO);
                if (true == ret_result)
                {
                    dialog_paint(s_scene_record->dialog);
                }
            }
            break;

        case TVIN_RECORD_STOP:            
            s_scene_record->next_scene = SCENE_TVIN_PALNTSC;
            exit_msg_loop();
            print_dbg("exit scene tvin\n");
            break;

        case TVIN_RECORD_PLAYHL:
        case TVIN_RECORD_PAUSEHL:
        case TVIN_RECORD_STOPHL:
            break;
            
        default:
            break;
        }
        break;

    case KEY_VOL | KEY_TYPE_DOWN:
    case KEY_VSUB | KEY_TYPE_DOWN:
    case KEY_VADD | KEY_TYPE_DOWN:
        if(NULL == s_scene_record->volume_bar)
        {
            tvin_create_volumebar();
        }        
        break;
    
    default:
        break;
    }
}


static void  _scene_tvin_proc_gui(gui_wm_message_t *pmsg)
{        
    switch (pmsg->msgid)
    {
    case WM_MSG_CREATE:
        break;

    case WM_MSG_PAINT:        
        break;

    case WM_MSG_KEY:
        /* 如果有按键动作，清除休眠唤醒标志*/
        s_scene_record->wake_up = 0;
        _tvin_scene_key_proc((key_event_t *)(pmsg->data.p));
        break;

    default:
        gui_wm_default_callback(pmsg);
        break;
    }
}



/* scene callback function */
static void _scene_tvin_callback(gui_wm_message_t *pmsg)
{
    int top;
          
    top = canvas_get_blending_focus(pmsg);
    
    if((NULL != s_scene_record->pRecMView) && (top == s_scene_record->pRecMView->canvas_id))
    {
        tvin_widget_option_proc_gui(s_scene_record->pRecMView, pmsg);
    }
    else if((NULL != s_scene_record->adjust) && (top == s_scene_record->adjust->canvas_id))
    {
        adjust_proc_gui(s_scene_record->adjust,pmsg);
    }
    else if((NULL != s_scene_record->dialog) && (top == s_scene_record->dialog->canvas_id))
    {
        dialog_proc_gui(s_scene_record->dialog,pmsg);
    }
    else if((NULL != s_scene_record->msgbox) && (top == s_scene_record->msgbox->canvas_id))
    {
        msgbox_proc_gui(s_scene_record->msgbox, pmsg);
    }
    else if((NULL != s_scene_record->volume_bar) && (top == s_scene_record->volume_bar->canvas_id))
    {
        volumebar_proc_gui(s_scene_record->volume_bar, pmsg);
    }
    else
    {
        _scene_tvin_proc_gui(pmsg);
    }
}


static bool _load_tvin_record_style(tvin_scene_record_t *pTvinRecord)
{    
    pTvinRecord->group = (resgroup_resource_t *)get_scene_child(pTvinRecord->scene, STR_OPTION_RES);
    if(NULL == pTvinRecord->group)
    {
        return false;
    }
    
    pTvinRecord->pic_record_status_bg = \
        (picture_resource_t *)get_scene_child(pTvinRecord->scene, PIC_TVIN_BUTTON);

    if(NULL == pTvinRecord->pic_record_status_bg)
    {
        return false;
    }    

    pTvinRecord->pic_pbar_bg = \
        (picture_resource_t *)get_scene_child(pTvinRecord->scene, PIC_TVIN_PROGRESS);

    if(NULL == pTvinRecord->pic_pbar_bg)
    {
        return false;
    }

    pTvinRecord->pic_pbar_hl = \
        (picture_resource_t *)get_scene_child(pTvinRecord->scene, PIC_TVIN_PROGRESS_HL);

    if(NULL == pTvinRecord->pic_pbar_hl)
    {
        return false;
    }

    pTvinRecord->pic_pbar = \
        (picture_resource_t *)get_scene_child(pTvinRecord->scene, PIC_TVIN_PROGRESS_DEHL);

    if(NULL == pTvinRecord->pic_pbar)
    {
        return false;
    }

    pTvinRecord->pic_hb_icon = \
        (picture_resource_t *)get_scene_child(pTvinRecord->scene, PIC_TVIN_ICON);

    if(NULL == pTvinRecord->pic_hb_icon)
    {
        return false;
    }

    pTvinRecord->picregion_record_status = \
        (picregion_resource_t *)get_scene_child(pTvinRecord->scene, PICREGION_BUTTON_ICON);

    if(NULL == pTvinRecord->picregion_record_status)
    {
        return false;
    }
           
    pTvinRecord->str_record_time = \
        (string_resource_t *)get_scene_child(pTvinRecord->scene, STR_RECORD_TIME);

    if(NULL == pTvinRecord->str_record_time)
    {
        return false;
    }

    pTvinRecord->str_remain_time = \
        (string_resource_t *)get_scene_child(pTvinRecord->scene, STR_RECORD_REMAIN);

    if(NULL == pTvinRecord->str_remain_time)
    {
        return false;
    }

    pTvinRecord->str_disk_full = \
        (string_resource_t *)get_scene_child(pTvinRecord->scene, STR_DISK_FULL);

    if(NULL == pTvinRecord->str_disk_full)
    {
        return false;
    }

    
    pTvinRecord->str_hb_title = \
        (string_resource_t *)get_scene_child(pTvinRecord->scene, STR_HB_TITLE);

    if(NULL == pTvinRecord->str_hb_title)
    {
        return false;
    }

    pTvinRecord->pbar_hl_position_resgroup = \
        (resgroup_resource_t *)get_scene_child(pTvinRecord->scene, PBAR_POSITION_RES);

    if(NULL == pTvinRecord->pbar_hl_position_resgroup)
    {
        return false;
    }

    pTvinRecord->xstart_pos = (unsigned int)(pTvinRecord->pbar_hl_position_resgroup->x - \
        pTvinRecord->pic_pbar_bg->x);
    return true;
}


static bool _scene_record_set_default_font(void)
{
    /* crate font */
    if(s_scene_record->fontdes > 0)
    {
        return false;
    }

    s_scene_record->fontdes = gui_create_font((unsigned char*)sys_get_default_font_file(), 16);
    if(s_scene_record->fontdes < 0)
    {
        print_err("call gui_create_font failed");
        return false;
    }

    if(gui_dc_set_default_fontface(s_scene_record->fontdes) < 0) 
    {
        print_err("gui_dc_set_default_fontface failed");
        return false;
    }

    return true;
}

static bool _scene_record_unset_default_font(void)
{
    /* crate font */
    if(s_scene_record->fontdes <= 0)
    {
        return false;
    }

    gui_destroy_font(s_scene_record->fontdes);
    s_scene_record->fontdes = 0; 
   
    return true;
}



static bool _scene_tvin_record_open_theme(void)
{    
    rotation_e angle;
    char common_file[PATH_MAX], app_file[PATH_MAX];

    angle = sty_get_view_rotation(); // 取视图角度
    tvin_appdata->save_angle = angle;
    angle = ROTATION_0;
    sty_set_screen_rotation(angle); // 设置屏幕方向

    s_scene_record->common_scene = load_scene(tvin_appdata->commonui_style, COMMONUI_SIDELONG_SCENE);
    s_scene_record->scene = load_scene(tvin_appdata->tvin_style, TVIN_RECORD_SCENE);
    
    if( (s_scene_record->common_scene == NULL) || (s_scene_record->scene == NULL) )
    {
        print_err("load desktop scene failed");
        return false;
    }
    else
    {
        sty_set_common_scene(s_scene_record->common_scene);
        sty_set_app_scene(s_scene_record->scene);
    }

    // 加载commonui 的图片资源
    sprintf(common_file, "%s/%s.res", sys_get_common_theme_res_dir(), "commonui");
    if( sty_open_common_pic_res(common_file) == false ) // 打开 commonui 图片资源文件
    {
        print_err("open %s failed!", common_file);
        return false;
    }

    // 加载应用的图片资源
    sprintf(app_file, "%s/%s.res", sys_get_app_theme_res_dir(), "tvin"); //获取应用的图片资源文件名
    if( sty_open_app_pic_res(app_file) == false ) // 打开应用的图片资源文件
    {
        print_err("open %s failed!", app_file);
        return false;
    }
    return true;
}

static bool _scene_tvin_record_close_theme(void)
{
    free_scene(s_scene_record->common_scene);
    s_scene_record->common_scene = NULL;

    free_scene(s_scene_record->scene);
    s_scene_record->scene = NULL;

    if(false == sty_close_common_pic_res())
    {
        print_err("close common picture resource error!");
        return false;
    }

    if(false == sty_close_app_pic_res())
    {
        print_err("close app picture resource error!");
        return false;
    }
    return true;
}

static bool _scene_tvin_record_open_str_res(void)
{
    char file[PATH_MAX];

    // 获取commonui 字符串资源文件名
    sprintf(file, "%s/%s_%s.str", sys_get_common_string_res_dir(), "commonui", sys_get_cur_language_codes());
    print_dbg("load %s", file);
    // 打开 commonui 字符串资源文件
    if( sty_open_common_str_res(file) == false )
    {
        print_err("open %s failed!", file);
        return false;
    }

    // 获取setting应用字符串资源文件名
    sprintf(file, "%s/%s_%s.str", sys_get_app_string_res_dir(), "tvin", sys_get_cur_language_codes());
    print_dbg("load %s", file);
    // 打开应用的字符串资源文件
    if( sty_open_app_str_res(file) == false )
    {
        print_err("open %s failed!", file);
        return false;
    }

    // 设置语言编码
    sty_set_text_encoding(GUI_UTF8_ENC);
    gui_set_language_mode(sys_get_cur_language_id());

    return true;
}

static bool _scene_tvin_record_close_str_res(void)
{
    if(false == sty_close_common_str_res())
    {
        print_err("close common string resource error!");
        return false;
    }

    if(false == sty_close_app_str_res())
    {
        print_err("close app string resource error!");
        return false;
    }
    return true;
}



/* scene initial function */
static bool _scene_tvin_record_init( tvin_appdata_t *app_data )
{
    canvas_set_blending_mode( true );
    sys_forbid_auto_standby(false);
    sys_set_key_tone_enable(0);
    //分配场景数据空
    if( app_data->pTvinRecord == NULL )
    {
        app_data->pTvinRecord = malloc( sizeof(tvin_scene_record_t) );

        if( app_data->pTvinRecord  == NULL )
        {
            print_err("malloc memory error\n");
            goto malloc_scene_memory_error;
        }        
    }
        
    memset( app_data->pTvinRecord, 0, sizeof(tvin_scene_record_t) );

    //if(NULL != s_scene_record)
    //{
        //s_scene_record->tvin_install = true;
    //}
        
    s_scene_record = app_data->pTvinRecord;
    /* 进入record场景以后初始化一些数据*/
    _restore_config_data();
    

    //初始化场景数据
    s_scene_record->next_scene = SCENE_TVIN_RECORD;
       
    /* load theme 资源文件 */
    if(_scene_tvin_record_open_theme() == false)
    {
        print_err();
        goto record_open_theme_error;
    }

    /* load 字符串资源文件 */
    if(_scene_tvin_record_open_str_res() == false)
    {
        print_err();
        goto record_open_str_res_error;
    }

    if(false == _load_tvin_record_style(s_scene_record))
    {
        print_err();
        goto load_record_scene_res_error;
    }

    if(false == tvin_create_bitmap())
    {
        print_err();
        goto create_bitmap_error;
    }

    /* 创建字体 */
    if(false == _scene_record_set_default_font())
    {
        print_err();
        goto set_default_font_error;
    }
                    
    //创建窗口
    s_scene_record->win_id = gui_wm_create_window(s_scene_record->scene->x, s_scene_record->scene->y, \
        s_scene_record->scene->width, s_scene_record->scene->height, \
        WM_CREATE_SHOW, _scene_tvin_callback, 0);
    if( s_scene_record->win_id == 0)
    {        
        goto create_window_error;
    }

    //设置焦点
    gui_wm_set_focus( s_scene_record->win_id );
            
    if(false == s_scene_record->tvin_install)
    {                 
        if(true == tvin_mmm_init())
        {                       
            s_scene_record->tvin_install = true;                        
        }
        else
        {
            print_err();
            goto start_preview_error;
        }        
    }

    if(false == tvin_start_preview())
    {
        print_err();
        goto start_preview_error;
    }

    /*创建headbar*/
    if(false == tvin_create_headbar())
    {
        print_err();
        goto create_headbar_error;
    }
    
    /*创建消息框*/
    if(false == _record_create_msgbox())
    {
        print_err();
        goto create_msgbox_error;
    }
    if(false == tvin_ui_blending_create())
    {
        print_err();
        goto ui_blending_create_error;
    }

    tvin_record_blending_ui_show();
        
    if(APP_START_RTC == app_data->app_start_mode)
    {
        tvin_alarmin_proc(app_data->alarm_clock);                      
    }   
    tvin_scene_record_paint(s_scene_record);        

    //启动timer
    _tvin_set_timer();
    
    clear_key_msg_queue(-1,-1);

    return true;

                                                                
    ui_blending_create_error:
    _record_delete_msgbox();

    create_msgbox_error:
    tvin_delete_headbar();

    start_preview_error:
    create_headbar_error:
    gui_wm_delete_window( s_scene_record->win_id );

    create_window_error:
    _scene_record_unset_default_font();

    set_default_font_error:
    create_bitmap_error:
    load_record_scene_res_error:    
    _scene_tvin_record_close_str_res();
 
    record_open_str_res_error:
    _scene_tvin_record_close_theme();
    
    record_open_theme_error:
    free(app_data->pTvinRecord);
    app_data->pTvinRecord = NULL;
    s_scene_record = NULL;

    malloc_scene_memory_error:
    canvas_set_blending_mode( false );
    sys_set_key_tone_enable(1);
    return false;    
}

/* scene exit function */
static bool _scene_tvin_deinit(tvin_appdata_t *app_data)
{
    tvin_stop_preview();
    tvin_ui_blending_delete();
    tvin_option_blending_delete();    
    /* 删除headbar*/
    if(false == tvin_delete_headbar())
    {
        print_err("delete headbar error\n");
        return false;
    }

    /* 删除dialog*/
    if(false == tvin_delete_dialog())
    {
        print_err("delete dialog error\n");
        return false;
    }
        
    /* 删除消息框*/
    if(false == _record_delete_msgbox())
    {
        print_err("delete msgbox error\n");
        return false;
    }
    /*删除adjust*/
    if(NULL != s_scene_record->adjust)
    {
        tvin_adjust_blending_delete();
        adjust_delete(s_scene_record->adjust);
        s_scene_record->adjust = NULL;
    }
    
    //停止timer
    _tvin_kill_timer();
        
    _scene_record_unset_default_font();
    if(NULL != app_data->pTvinRecord)
    {
        if(true == app_data->pTvinRecord->tvin_install)
        {
            print_dbg();
            tvin_mmm_Close();
            app_data->pTvinRecord->tvin_install = false;
        }        
    }
            
    //删除窗体
    gui_wm_delete_window( s_scene_record->win_id );
        
    //退出场景
    if(false == _scene_tvin_record_close_theme())
    {
        return false;
    }

    if(false == _scene_tvin_record_close_str_res())
    {
        return false;
    }
        
    if(false == tvin_cfg_write_all(g_tvin_cfgs, TVIN_CFG_INVALID))
    {
        return false;
    }

    //释放所创建的图片
    tvin_delete_bitmap();
     
    gui_set_display_mode(GUI_DISPLAY_MODE_NON_YUV);
    canvas_set_blending_mode( false );
    sys_set_key_tone_enable(1);
    return true;
}


static int _scene_record_proc_sys(msg_apps_t *msg)
{
    int clock_no;
    string_resource_t *str_dialog_info;
    mmm_disk_info_t  disk_info;
    
    switch (msg->type)
    {           
    case MSG_APP_QUIT:
        /*为避免设置定时后直接长按return退出导致设置定时无效
        所以先判断是否在设置定时子菜单，如果是则先返回上一级*/
        if(NULL != fetch_menu_singletime())
        {
            exit_option_singlerec();
        }
        else if (NULL != fetch_menu_repeattime())
        {
            exit_option_repeatrec();
        }
        
        s_scene_record->next_scene = SCENE_TVIN_EXIT;
        tvin_return_check_recorder();
        break;

    case MSG_TVIN_ALARM_IN:
        //如果当前正在进行录制，那么本次闹钟引起的录制无效
        if(TVIN_RECORD_PLAY == s_scene_record->tvin_record_sta)
        {
            break;
        }

        if(false == sys_power_threshold_check(POWER_THRESHOLD_TVIN_REC))
        {
            str_dialog_info = (string_resource_t *)get_resgroup_child(s_scene_record->group, STR_TV_LOW_POWER);
            if(str_dialog_info != NULL)
            {
                if(true == tvin_create_dialog(str_dialog_info->str_id, DIALOG_HEAD_LOWBAT))
                {
                    dialog_paint(s_scene_record->dialog);
                }
            }
            break;
        }
        
        {
            memcpy(&clock_no, msg->content, sizeof(clock_no));
            tvin_alarmin_proc(clock_no);
            s_scene_record->time_relate.hide_ui_time = UI_AUTO_HIDE_INTERVAL/TVIN_TIMER_INTERVAL;
        }
        break;

    case MSG_CARD_IN:
    case MSG_CARD_OUT:
    case MSG_USB_HOST_DISK_IN:
    case MSG_USB_HOST_DISK_OUT:
        if(s_scene_record->msgbox != NULL)
        {
            /* 控件msgbox用来处理默认系统消息(低电提示、卡插入和拔出提示、USB插入功能选择提示等) */
            msgbox_proc_sys(s_scene_record->msgbox, msg);
        }
        if(NULL != fetch_menu_savepath())
        {
            update_menu_savepath();
        }
        /*磁盘变动时需重新检测当前保存路径并更新剩余容量*/
        tvin_set_record_path(s_scene_record->tvin_config.save_path);
        if(true == tvin_get_disk_info(&disk_info))
        {
            s_scene_record->total_time = disk_info.info;
            s_scene_record->record_remain_time = s_scene_record->total_time;
            s_scene_record->cur_record_time = 0;                             
        }
        else
        {
            print_err("get disk info err!\n");
            s_scene_record->total_time = 0;
            s_scene_record->record_remain_time = 0;
            s_scene_record->cur_record_time = 0;  
        }
        s_scene_record->record_time_is_show = false;
        s_scene_record->remain_time_is_show = false;
        tvin_scene_record_paint(s_scene_record);
        gui_screen_update();   
        
        break;
                
    default:
        if(msg->type == MSG_LOW_VOLTAGE)
        {
            if (s_scene_record->dialog != NULL) 
            {
                str_dialog_info = (string_resource_t *)get_resgroup_child(s_scene_record->group, STR_TV_LOW_POWER);
                if((str_dialog_info != NULL) && (s_scene_record->dialog->str_id== str_dialog_info->str_id))
                {
                    break;
                }
            }
        }
        if(s_scene_record->msgbox != NULL)
        {
            /* 控件msgbox用来处理默认系统消息(低电提示、卡插入和拔出提示、USB插入功能选择提示等) */
            msgbox_proc_sys(s_scene_record->msgbox, msg);
        }
        break;
    }
    return 0;
}


bool  scene_tvin_record(tvin_appdata_t *app_data)
{
    bool  ret;
    /* 场景消息循环 */
    msg_apps_t  msg;
    
    tvin_appdata = app_data;
    /* 场景初始化 */
    ret = _scene_tvin_record_init(app_data);
        
    if(false == ret)
    {
        print_err("scene_tvin init error! \n");        
        return false;
    }
    else
    {
        print_dbg();
    }

    register_sys_dispatcher((sys_msg_proc)_scene_record_proc_sys);

    while(true == get_msg(&msg))
    {
        dispatch_msg(&msg);
    }

    unregister_sys_dispatcher();
    
    /* 场景退出 */
    ret = _scene_tvin_deinit(app_data);
    if(false == ret)
    {
        print_err("scene deinit error\n");
        return false;
    }
    app_data->app_start_mode = APP_START_NORMAL;
    print_dbg();    
    return true;
}


