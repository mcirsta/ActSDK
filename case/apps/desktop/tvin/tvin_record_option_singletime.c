/********************************************************************************
*                              USDK(1100)
*                             Module: app
*                 Copyright(c) 2003-2009 Actions Semiconductor,
*                            All Rights Reserved.
* File : tvin_record_option_singletime.c
* History:
*      <author>    <time>           <version >             <desc>
*      Jerry Fu    2009-4-28             v1.0               build this file
********************************************************************************/
/*!
* \file     tvin_record_option_singletime.c
* \brief    设置单次录制功能菜单
* \author   Jerry Fu
*
* \version 1.0
* \date  2009/04/28
*******************************************************************************/
#include "tvin.h"

/* menu max items */
#define  TVIN_OPTION_SINGLEREC_MAX_ITEMS   (4)
#define  TVIN_SINGLE_ALARM_ON             (1)
#define  TVIN_SINGLE_ALARM_OFF            (0)


static menu_t  *pMenuSingle = NULL;
static menu_t  *upper_level_menu;
static tvin_scene_record_t *s_tvin_record;
static int upper_level_index;
static bool single_alarm_onoff;

static bool  time_is_changed = false;
static bool  date_is_changed = false;

static unsigned  int  tvin_option_singlerec_textid[TVIN_OPTION_SINGLEREC_MAX_ITEMS+1] =
{
    STR_START_DATE,
    STR_START_TIME,
    STR_RECORD_LENGTH,
    STR_TV_ON,
    STR_TV_OFF,    
};


static void _singlerec_adjust_startdate_cb(adjust_t *adjust, adjust_signal_e signal);
static void _singlerec_adjust_starttime_cb(adjust_t *adjust, adjust_signal_e signal);
static void _singlerec_adjust_reclen_cb(adjust_t *adjust, adjust_signal_e signal);

static int _option_singlerec_startdate( menuview_t *menuview, menuview_sig_e signal );
static int _option_singlerec_starttime( menuview_t *menuview, menuview_sig_e signal );
static int  _option_singlerec_recordlen(menuview_t *menuview, menuview_sig_e  signal);
static int _option_singlerec_onoff(menuview_t *menuview, menuview_sig_e  signal);
static bool _option_singlerec_exit( menuview_t *menuview );
static  menu_t* _option_singlerec_init(menuview_t *menuview);


static void _save_single_startdate_cfg(char *cfg_date)
{
    tvin_cfg_type_e  cfg_index;
    tvin_cfg_t  *tvin_cfg;
    
    cfg_index = TVIN_CFG_SINGLE1_DATE + upper_level_index;
    tvin_cfg = &(g_tvin_cfgs[cfg_index]);
    memcpy(tvin_cfg->text, cfg_date, 32);
    tvin_cfg->changed = true; 
    tvin_cfg_write(&g_tvin_cfgs[cfg_index]);
}

static void _save_single_starttime_cfg(char *cfg_time)
{
    tvin_cfg_type_e  cfg_index;
    tvin_cfg_t  *tvin_cfg;
    
    cfg_index = TVIN_CFG_SINGLE1_TIME + upper_level_index;
    tvin_cfg = &(g_tvin_cfgs[cfg_index]);
    memcpy(tvin_cfg->text, cfg_time, 32);
    tvin_cfg->changed = true;
    tvin_cfg_write(&g_tvin_cfgs[cfg_index]);
}

static void _save_single_reclen_cfg(char *cfg_length)
{    
    tvin_cfg_type_e  cfg_index;
    tvin_cfg_t  *tvin_cfg;
      
    cfg_index = TVIN_CFG_SINGLE1_LENGTH+ upper_level_index;
    tvin_cfg = &(g_tvin_cfgs[cfg_index]);
    memcpy(tvin_cfg->text, cfg_length, 32);
    tvin_cfg->changed = true;
    tvin_cfg_write(&g_tvin_cfgs[cfg_index]);
}

static void _save_single_onoff_cfg(bool onoff)
{
    tvin_cfg_type_e  cfg_index;
    tvin_cfg_t  *tvin_cfg;
    char buf[32];

    if(true == onoff)
    {
        strcpy(buf, "ON");
    }
    else
    {
        strcpy(buf, "OFF");
    }
    
    cfg_index = TVIN_CFG_SINGLE1_ONOFF + upper_level_index;
    tvin_cfg = &(g_tvin_cfgs[cfg_index]);
    memcpy(tvin_cfg->text, buf, sizeof(buf));
    tvin_cfg->changed = true;
    tvin_cfg_write(&g_tvin_cfgs[cfg_index]);
}


static void _singlerec_adjust_startdate_cb(adjust_t *adjust, adjust_signal_e signal)
{   
    rtc_date_t *ptemp;
    rtc_date_t  adjust_date;    
    char buf[32];
      
    switch(signal)
    {
    case WIDGET_SIG_ADJUST_OK:
        ptemp = &(s_tvin_record->tvin_config.single_record[upper_level_index].start_date);

        adjust_get_setting_date(&adjust_date);
        
        if((adjust_date.year != ptemp->year) ||
            (adjust_date.month != ptemp->month) ||
            (adjust_date.day != ptemp->day)
          )
        {  
            date_is_changed = true;
            *ptemp = adjust_date;
            snprintf(buf, 32, "%.4d-%.2d-%.2d",ptemp->year,ptemp->month,ptemp->day);
            _save_single_startdate_cfg(buf);
        } 
        tvin_adjust_blending_delete();    
        adjust_delete(s_tvin_record->adjust);
        s_tvin_record->adjust = NULL;
        break;
        
    case WIDGET_SIG_ADJUST_CANCEL:
        tvin_adjust_blending_delete();
        adjust_delete(s_tvin_record->adjust);
        s_tvin_record->adjust = NULL;
        break;
        
    default:
        break;
    }
}

static void _singlerec_adjust_starttime_cb(adjust_t *adjust, adjust_signal_e signal)
{   
    rtc_time_t *ptemp;
    rtc_time_t  adjust_time;
    char buf[32];

    switch(signal)
    {
    case WIDGET_SIG_ADJUST_OK:                
        ptemp = &(s_tvin_record->tvin_config.single_record[upper_level_index].start_time);
        adjust_get_setting_time(&adjust_time);
        if((adjust_time.hour != ptemp->hour) ||
           (adjust_time.minute != ptemp->minute)
          )
        {
            time_is_changed = true;
            *ptemp = adjust_time;
            sprintf(buf, "%.2d:%.2d", ptemp->hour,ptemp->minute);            
            g_tvin_cfgs[TVIN_CFG_SINGLE1_TIME+upper_level_index].changed = true;
            _save_single_starttime_cfg(buf);
        }
         
        tvin_adjust_blending_delete();
        adjust_delete(s_tvin_record->adjust);
        s_tvin_record->adjust = NULL;
        break;
    case WIDGET_SIG_ADJUST_CANCEL:
        tvin_adjust_blending_delete();
        adjust_delete(s_tvin_record->adjust);
        s_tvin_record->adjust = NULL;
        break;
        
    default:
        break;
    }
}


static void _singlerec_adjust_reclen_cb(adjust_t *adjust, adjust_signal_e signal)
{
    rtc_time_t *ptemp;
    rtc_time_t  adjust_length;
    char buf[32];
    
    switch(signal)
    {
    case WIDGET_SIG_ADJUST_OK:
        ptemp = &(s_tvin_record->tvin_config.single_record[upper_level_index].record_time);        
        adjust_get_recordtime(&adjust_length);

        if((adjust_length.hour != ptemp->hour) ||
           (adjust_length.minute != ptemp->minute)
          )
        {
            *ptemp = adjust_length;
            sprintf(buf, "%.2d:%.2d", ptemp->hour,ptemp->minute); 
            _save_single_reclen_cfg(buf);
        }  
        tvin_adjust_blending_delete();
        adjust_delete(s_tvin_record->adjust); 		
        s_tvin_record->adjust = NULL;
        break;
        
    case WIDGET_SIG_ADJUST_CANCEL:
        tvin_adjust_blending_delete();
        adjust_delete(s_tvin_record->adjust);
        s_tvin_record->adjust = NULL;
        break;
        
    default:
        break;
    }
}


static int _option_singlerec_startdate( menuview_t *menuview, menuview_sig_e signal )
{                   
    adjust_date_init_t  start_date;
    
    memset(&start_date, 0, sizeof(adjust_date_init_t));
    
    sscanf(g_tvin_cfgs[TVIN_CFG_SINGLE1_DATE+upper_level_index].text,"%d-%d-%d",
        &(start_date.sys_date.year), &(start_date.sys_date.month), &(start_date.sys_date.day));
          
    switch ( signal ) 
    {
    case WIDGET_SIG_MENUVIEW_OK:
        s_tvin_record->adjust = \
            adjust_create(&start_date, ADJ_STYLE_DATE, _singlerec_adjust_startdate_cb);

        tvin_adjust_blending_create();
        tvin_blending_show_spec(TVIN_BLENDING_ADJUST);

        if(NULL != s_tvin_record->adjust)
        {
            return 1;
        }
        else
        {
            return 0;
        }
        
    case WIDGET_SIG_MENUVIEW_QUIT:
        // return to parent menu
        _option_singlerec_exit(menuview);
        break;
        
    default:
        break;
    }
    return 1;
}

static int _option_singlerec_starttime( menuview_t *menuview, menuview_sig_e signal )
{                       
    adjust_time_init_t  adjust_time;
    memset(&adjust_time, 0, sizeof(adjust_time_init_t));

    sscanf(g_tvin_cfgs[TVIN_CFG_SINGLE1_TIME+upper_level_index].text,"%d:%d",
        &(adjust_time.sys_time.hour), &(adjust_time.sys_time.minute));
    
    switch ( signal ) 
    {
    case WIDGET_SIG_MENUVIEW_OK:        
        s_tvin_record->adjust = \
            adjust_create(&adjust_time, ADJ_STYLE_TIME, _singlerec_adjust_starttime_cb);              

        tvin_adjust_blending_create();
        tvin_blending_show_spec(TVIN_BLENDING_ADJUST);        
        break;
        
    case WIDGET_SIG_MENUVIEW_QUIT:
        // return to parent menu
        _option_singlerec_exit(menuview);
        break;
        
    default:
        break;
    }
    return 1;
}

static int  _option_singlerec_recordlen(menuview_t *menuview, menuview_sig_e  signal)
{
    adjust_time_init_t  record_length;
    memset(&record_length, 0, sizeof(adjust_time_init_t));
    
    sscanf(g_tvin_cfgs[TVIN_CFG_SINGLE1_LENGTH+upper_level_index].text,"%d:%d",
        &(record_length.sys_time.hour), &(record_length.sys_time.minute));
        
    switch ( signal ) 
    {
    case WIDGET_SIG_MENUVIEW_OK:
        s_tvin_record->adjust = \
            adjust_create(&record_length, ADJ_STYLE_RECLEN, _singlerec_adjust_reclen_cb);              

        tvin_adjust_blending_create();
        tvin_blending_show_spec(TVIN_BLENDING_ADJUST);
        break;
        
    case WIDGET_SIG_MENUVIEW_QUIT:  
        _option_singlerec_exit(menuview);
        break;
        
    default:
        break;
    }
    return 1;
}

static bool  _single_alarm_delete(void)
{
    alarm_delete_param_t  delete_alarm;
    memset(&delete_alarm, 0, sizeof(alarm_delete_param_t));
    time_is_changed = false;
    date_is_changed = false;        
    delete_alarm.mode_no = TVIN_MODE_NO;
    delete_alarm.clock_no = s_tvin_record->tvin_config.alarm_clknum[upper_level_index];
    tvin_delete_alarm_clock(&delete_alarm);
    _save_single_onoff_cfg(false);    
    return true;
}


static bool  _single_alarm_create(void)
{
    alarm_new_param_t  new_alarm;
    memset(&new_alarm, 0, sizeof(alarm_new_param_t));
    
    new_alarm.in.mode_no = TVIN_MODE_NO;
    new_alarm.in.alarm_clock.type = ALARM_ONCE;
    new_alarm.in.alarm_clock.time = \
        s_tvin_record->tvin_config.single_record[upper_level_index].start_time;
    
    new_alarm.in.alarm_clock.special_date.date = \
        s_tvin_record->tvin_config.single_record[upper_level_index].start_date;            

    time_is_changed = false;
    date_is_changed = false;

    tvin_create_alarm_clock(&new_alarm);
    s_tvin_record->tvin_config.alarm_clknum[upper_level_index] = new_alarm.out.clock_no;
    g_tvin_cfgs[TVIN_CFG_SINGLE1_CLKNUM+upper_level_index].value = new_alarm.out.clock_no;
    g_tvin_cfgs[TVIN_CFG_SINGLE1_CLKNUM+upper_level_index].changed = true;
    tvin_cfg_write(&g_tvin_cfgs[TVIN_CFG_SINGLE1_CLKNUM+upper_level_index]);
    _save_single_onoff_cfg(true);    
    return true;
}


static bool _single_alarm_modify(void)
{
    bool ret = true;
    alarm_modify_param_t  modify_alarm;
    memset(&modify_alarm, 0, sizeof(modify_alarm));
    if(time_is_changed || date_is_changed)
    {        
        time_is_changed = false;
        date_is_changed = false;
        
        modify_alarm.mode_no = TVIN_MODE_NO;
        modify_alarm.clock_no = s_tvin_record->tvin_config.alarm_clknum[upper_level_index];
        modify_alarm.alarm_info.type = ALARM_ONCE;

        modify_alarm.alarm_info.time =\
            s_tvin_record->tvin_config.single_record[upper_level_index].start_time;

        modify_alarm.alarm_info.special_date.date =\
            s_tvin_record->tvin_config.single_record[upper_level_index].start_date;
                 
        ret = tvin_modify_alarm_clock(&modify_alarm);       
    }
    return ret;
}



static int _option_singlerec_onoff(menuview_t *menuview, menuview_sig_e  signal)
{    
    menu_t *cur_menu = NULL;
    menuitem_t *cur_item = NULL;
    int text_id;
    resgroup_resource_t *group;
    string_resource_t *str_res;
    int tv_on_id = 0;
    int tv_off_id = 0;

    cur_menu = menuview->menu;
    if(NULL == cur_menu)
    {
        return -1;
    }
    
    cur_item = menu_get_cur_item( cur_menu );
    if(NULL == cur_item)
    {
        return -1;
    }
    
    group = get_scene_child(s_tvin_record->scene, STR_OPTION_RES);
    if(group != NULL)
    {
        str_res = (string_resource_t *)get_resgroup_child(group, STR_TV_ON);
        if(str_res != NULL)
        {
            tv_on_id = (int)(str_res->str_id);
        }
        str_res = (string_resource_t *)get_resgroup_child(group, STR_TV_OFF);
        if(str_res != NULL)
        {
            tv_off_id = (int)(str_res->str_id);
        }
    }
    switch ( signal ) 
    {
    case WIDGET_SIG_MENUVIEW_OK:
        text_id = cur_item->text_id;
        if(tv_on_id == text_id)
        {    
            cur_item->text_id = tv_off_id;
            s_tvin_record->tvin_config.single_record[upper_level_index].onoff = false;
        }
        else
        {
            cur_item->text_id = tv_on_id;
            s_tvin_record->tvin_config.single_record[upper_level_index].onoff = true;
        }        
        menuview_paint(menuview);
        break;
        
    case WIDGET_SIG_MENUVIEW_QUIT:
        _option_singlerec_exit(menuview);
        break;
        
    default:
        break;
    }
    return 1;
}

/* 退出单次录制设置菜单 */
static bool _option_singlerec_exit( menuview_t *menuview )
{    
    tvin_scene_record_t *prec;       
    prec = fetch_tvin_record();
    if(NULL == prec)
    {
        return false;
    }
    
    //如果alarm的设置发生了变化(ON->OFF, OFF->ON)
    if(single_alarm_onoff != (prec->tvin_config.single_record[upper_level_index].onoff))
    {
        if(true == (prec->tvin_config.single_record[upper_level_index].onoff))
        {
            if(_single_alarm_create() == false)
            {
                prec->tvin_config.single_record[upper_level_index].onoff = false;
            }
        }
        else
        {
            if( _single_alarm_delete() == false)
            {
                prec->tvin_config.single_record[upper_level_index].onoff = true;
            }
        }
    }
    else
    {
        //TVIN的闹钟已经创建
        if(true == (prec->tvin_config.single_record[upper_level_index].onoff))
        {
            _single_alarm_modify();
        }        
    }
                        
    //退回到上级菜单    
    menuview_set_menu(menuview, upper_level_menu);
    menuview_paint(menuview);
    tvin_option_singlerec_deinit();
    return true;
}

static  menu_t* _option_singlerec_init(menuview_t *menuview)
{
    scene_t* scene;
    menu_t *pMenu;
    tvin_scene_record_t *pRecord;
    string_resource_t *str_res;            
    menuitem_t item[TVIN_OPTION_SINGLEREC_MAX_ITEMS];
    int num;    
    pMenu = pMenuSingle;

    upper_level_menu = menuview->menu;
    if(NULL == upper_level_menu)
    {
        return NULL;
    }
    
    upper_level_index = menu_get_cur_index(upper_level_menu);
    
    s_tvin_record = fetch_tvin_record();
    if(NULL == s_tvin_record)
    {
        return NULL;
    }
    
    pRecord = s_tvin_record;
    memset(item, 0, TVIN_OPTION_SINGLEREC_MAX_ITEMS*sizeof(menuitem_t));
    //initial menu
    if( NULL == pMenu)
    {
        //get scene
        scene = pRecord->scene;
        if ( scene == NULL )
        {
            print_err("cann't find app scene!");
            return false;
        }
                                       
        //create menu
        pMenu = menu_create();
        if ( pMenu == NULL )
        {
            print_err("cann't create menu!");
            return false;
        }
        else
        {    
            for(num=0; num<3; num++)
            {
                item[num].unsel_icon_id = RES_NULL;
                item[num].sel_icon_id = RES_NULL;

                str_res = (string_resource_t *)get_resgroup_child(pRecord->group, tvin_option_singlerec_textid[num]);
                if(str_res != NULL)
                {
                    item[num].text_id = (int)(str_res->str_id);
                }
                item[num].has_child = false;
            }
            
            item[0].callback = (item_cb_t)_option_singlerec_startdate;            
            item[1].callback = (item_cb_t)_option_singlerec_starttime;
            item[2].callback = (item_cb_t)_option_singlerec_recordlen;
            item[3].callback = (item_cb_t)_option_singlerec_onoff;

            if(true == (s_tvin_record->tvin_config.single_record[upper_level_index].onoff))
            {
                //在这里判断下alarm驱动中的时钟是否有效
                alarm_query_param_t  query_clock_param;
                query_clock_param.in.mode_no = 1;         
                query_clock_param.in.clock_no = s_tvin_record->tvin_config.alarm_clknum[upper_level_index];
     
                if(true == tvin_query_alarm_clock(&query_clock_param))
                {
                    print_dbg("out year= %d\n",query_clock_param.out.alarm_clock.special_date.date.year); 
                    print_dbg("out month= %d\n",query_clock_param.out.alarm_clock.special_date.date.month); 
                    print_dbg("out date = %d\n",query_clock_param.out.alarm_clock.special_date.date.day);
                    print_dbg("out time our= %d\n",query_clock_param.out.alarm_clock.time.hour); 
                    print_dbg("out time minute= %d\n",query_clock_param.out.alarm_clock.time.minute); 
                    str_res = (string_resource_t *)get_resgroup_child(pRecord->group, STR_TV_ON);
                    single_alarm_onoff = true;
                }
                else
                {
                    //如果无效，则删除
                    _single_alarm_delete();
                    s_tvin_record->tvin_config.single_record[upper_level_index].onoff = false;
                    str_res = (string_resource_t *)get_resgroup_child(pRecord->group, STR_TV_OFF);
                    single_alarm_onoff = false;
                }
            }
            else
            {
                str_res = (string_resource_t *)get_resgroup_child(pRecord->group, STR_TV_OFF);
                single_alarm_onoff = false;
            }
 
            if(str_res != NULL)
            {
                item[3].text_id = (int)(str_res->str_id);
            }
        }        
        menu_append_items(pMenu,item,TVIN_OPTION_SINGLEREC_MAX_ITEMS);
        menu_set_cur_index( pMenu, 0 );
    }
    return pMenu;
}

/* 进入主菜单 */
bool tvin_option_singlerec_enter( menuview_t *menuview )
{
    if(NULL == pMenuSingle)
    {
        pMenuSingle = _option_singlerec_init(menuview);
    }
    menuview_set_menu(menuview, pMenuSingle);
    menuview_paint( menuview );
    return true;
}


bool tvin_option_singlerec_deinit( void )
{    
    //delete menu   
    if( pMenuSingle != NULL )
    {
        menu_delete( pMenuSingle );
        pMenuSingle = NULL;
    }    
    return true;
}

menu_t *fetch_menu_singletime(void)
{
    return pMenuSingle;
}

void exit_option_singlerec(void)
{
    tvin_scene_record_t *tmp_pRecord;
    tmp_pRecord = fetch_tvin_record();
    _option_singlerec_exit(tmp_pRecord->pRecMView);
}


