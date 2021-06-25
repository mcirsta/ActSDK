/*******************************************************************************
 *                              NOYA1100
 *                            Module: video
 *                 Copyright(c) 2003-2008 Actions Semiconductor,
 *                            All Rights Reserved. 
 * History:         
 *      <author>    <time>           <version >             <desc>
 *      shaofeng    2009-04-17          1.0             build this file 
*******************************************************************************/
/*!
 * \file     headbar.c
 * \brief    实现headbar控件
 * \author   shaofeng
 * \par      GENERAL DESCRIPTION:
 *               
 * \par      EXTERNALIZED FUNCTIONS:
 *               
 * \version 1.0
 * \date  2009/04/17
*******************************************************************************/

#include "include_case.h"


/*本应用的头文件*/
#include "headbar_private.h"
#include "commonui_sty.h"



static bool _get_param_music_play( void );
static int _get_param_battery_level(charge_status_t charge, charge_voltage_t voltage);
static bool _get_param_time( headbar_t *headbar, char *buffer_time );
static bool _get_param( headbar_t *headbar );
static int _proc_timer(void *param);
static bool _init( headbar_t *headbar );
static int _proc_key( headbar_t *headbar, key_event_t *key_event );


#if 0
#endif
/******************************************************************************/
/*!                    
* \par  Description:
*      获得music引擎的play状态
* \param[in]    void
* \retval       true: 有且处于play ff ab复读等状态. false: 没有引擎, 或者有但pause stop err状态
* \ingroup      common_headbar
*******************************************************************************/
static bool _get_param_music_play( void )
{
    bool            result = true;
    struct timespec timeout;
    msg_apps_t      msg;
    msg_reply_t     reply;
    mengine_t       mengine;
    
    memset( &mengine, 0x00, sizeof(mengine) );
    
    timeout.tv_sec = 1;
    timeout.tv_nsec = 0;
    
    msg.type = MSG_MENGINE_GET_ENGINE_INFO_SYNC;
    result = send_sync_msg(APP_NAME_MUSIC_ENGINE, &msg, &reply, &timeout);
    //发送给引擎的消息失败, 说明没有引擎
    if ( false == result )
    {
        return false;
    }
    if ( MSG_REPLY_SUCCESS != reply.type )
    {
        return false;
    }
    
    memcpy( &mengine, reply.content, sizeof(mengine) );
    if ( (EG_PAUSE == mengine.eg_status.play_status) \
        || (EG_STOP == mengine.eg_status.play_status) \
        || (EG_ERROR == mengine.eg_status.play_status) )
    {
        return false;
    }
    
    return true;
}


/******************************************************************************/
/*!                    
* \par  Description:
*      将充电状态和电压转换成headbar需要显示的电池图标
* \param[in]    charge_status_t: 充电状态
* \param[in]    charge_voltage_t: 当前电压
* \retval       headbar电池显示的级数
* shaofeng 注意: 对电量的平滑处理, 只舍不入
* \ingroup      common_headbar
*******************************************************************************/
static int _get_param_battery_level(charge_status_t charge, charge_voltage_t voltage)
{
    static int battery_level = 0;
    
    print_dbg("charge:%d, voltage:0x%x", charge, voltage);
    if (CHARGE_CHARGING == charge)
    {
        battery_level++;
        print_dbg("battery_level:%d", battery_level);
        battery_level %= MAX_HEADBAR_BATTERY_LEVEL;
        print_dbg("battery_level:%d", battery_level);
    }
    else
    {
        if (voltage < VOL_HEADBAR_BATTERY_LEVEL1)
        {
            battery_level = 0;
        }
        else if (voltage < VOL_HEADBAR_BATTERY_LEVEL2)
        {
            battery_level = 1;
        }
        else if (voltage < VOL_HEADBAR_BATTERY_LEVEL3)
        {
            battery_level = 2;
        }
        else if (voltage < VOL_HEADBAR_BATTERY_LEVEL4)
        {
            battery_level = 3;
        }
        else if (voltage < VOL_HEADBAR_BATTERY_LEVEL5)
        {
            battery_level = 4;
        }
        else
        {
            battery_level = 5;
        }
    }/* end if (CHARGE_CHARGING == charge) */
    
    print_dbg("battery_level:%d", battery_level);
    return battery_level;
}


/******************************************************************************/
/*!                    
* \par  Description:
*      获得系统时间, 并将时间转换成字符串, 直接用于headbar显示
* \param[in]    char *: 将时间转换后的字符串指针
* \retval       true: 获得时间正确. false: 获得的时间错误
* \ingroup      common_headbar
24进制转换成12进制
00:00   12:00am
00:30   12:30am
11:59   11:59am
12:00   12:00pm
12:30   12:30pm
13:00   01:00pm
23:59   11:59pm
*******************************************************************************/
static bool _get_param_time( headbar_t *headbar, char *buffer_time )
{
    bool result = true;
    rtc_time_t time;
    rtc_date_t date;
    
    if ( NULL == buffer_time )
    {
        print_warning("buffer_time is NULL\n\n");
        return false;
    }
    
    result = sys_get_rtc_date_time( &time, &date );
    print_dbg("time:%.2d:%.2d", time.hour, time.minute);
    if ( true == result )
    {
        switch( headbar->private->time_format )
        {
        case TIME_FORMAT_12:
            if ( time.hour >= 12 )
            {
                if ( time.hour >= 13 )
                {
                    time.hour -= 12;
                }
                sprintf(buffer_time, "%.2d:%.2d pm", time.hour, time.minute);
            }
            else
            {
                if ( 0 == time.hour )
                {
                    time.hour = 12;
                }
                sprintf(buffer_time, "%.2d:%.2d am", time.hour, time.minute);
            }
            break;
        
        case TIME_FORMAT_112:
            if ( time.hour > 12 )
            {
                time.hour -= 12;
                sprintf(buffer_time, "%.4d/%.2d/%.2d %.2d:%.2d pm", \
                    date.year, date.month, date.day, time.hour, time.minute);
            }
            else
            {
                if ( 0 == time.hour )
                {
                    time.hour = 12;
                }
                sprintf(buffer_time, "%.4d/%.2d/%.2d %.2d:%.2d am", \
                    date.year, date.month, date.day, time.hour, time.minute);
            }
            break;
        
        case TIME_FORMAT_212:
            if ( time.hour > 12 )
            {
                time.hour -= 12;
                sprintf(buffer_time, "%.4d-%.2d-%.2d %.2d:%.2d pm", \
                    date.year, date.month, date.day, time.hour, time.minute);
            }
            else
            {
                if ( 0 == time.hour )
                {
                    time.hour = 12;
                }
                sprintf(buffer_time, "%.4d-%.2d-%.2d %.2d:%.2d am", \
                    date.year, date.month, date.day, time.hour, time.minute);
            }
            break;
        case TIME_FORMAT_24:
            sprintf(buffer_time, "%.2d:%.2d", time.hour, time.minute);
            break;
            
        case TIME_FORMAT_124:
            sprintf(buffer_time, "%.4d/%.2d/%.2d %.2d:%.2d", \
                date.year, date.month, date.day, time.hour, time.minute);
            break;
            
        case TIME_FORMAT_224:
            sprintf(buffer_time, "%.4d-%.2d-%.2d %.2d:%.2d", \
                date.year, date.month, date.day, time.hour, time.minute);
            break;
            
        default:
            break;
        }/* end switch( headbar->private->time_format ) */
    }
    else
    {
        print_warning("tm_get_time err\n\n");
        result = false;
    }   /* end if ( true == result ) */
    
    return result;
}


/******************************************************************************/
/*!                    
* \par  Description:
*      获得headbar参数, 用于headbar显示
* \param[in]    char *: 控件指针
* \retval       true: 获得参数成功. false: 获取参数失败
* \ingroup      common_headbar
*******************************************************************************/
static bool _get_param( headbar_t *headbar )
{
    bool result = true;
    hw_status_t     hw_status;
    struct timespec timeout;
    msg_apps_t      msg;
    msg_reply_t     reply;
    unsigned short  battery_frame;
    
    memset( &hw_status, 0x00, sizeof(hw_status) );
    
    timeout.tv_sec = 1;
    timeout.tv_nsec = 0;

    msg.type = MSG_GET_HW_STATUS_SYNC;
    result = send_sync_msg(APP_NAME_MSG_SERV, &msg, &reply, &timeout);
    if (false == result)
    {
        print_warning("send_sync_msg failed");
        return false;
    }
    if ( MSG_REPLY_SUCCESS != reply.type )
    {
        return false;
    }
    memcpy(&hw_status, reply.content, sizeof(hw_status_t));
    
    headbar->private->show.keylock  = (char)hw_status.keylock;
    headbar->private->show.card     = (char)hw_status.card;
    if (PLUG_IN == hw_status.usb)
    {
        headbar->private->show.usb = 1;
    }
    else
    {
        headbar->private->show.usb = 0;
    }
    
    battery_frame = (unsigned short)_get_param_battery_level(hw_status.charge, hw_status.voltage_show);
    headbar->private->show.battery_frame  = battery_frame;
    
    headbar->private->show.music_play= (char)_get_param_music_play();
    
    _get_param_time( headbar, headbar->private->buffer_str_time );
    
    return result;
}

/******************************************************************************/
/*!                    
* \par  Description:
*      定时器处理函数. 定时获得headbar各显示元素状态, 并主动显示
* \param[in]    void *: 控件指针
* \retval       always be 1
* \ingroup      common_headbar
*******************************************************************************/
static int _proc_timer(void *param)
{
    headbar_t *headbar = (headbar_t *)param;
    
    _get_param( headbar );
    
    //shaofeng 自刷新, 还是外部控制刷新
//  headbar->callback( headbar, NULL );
    headbar_paint( headbar );
    
    return 1;
}


bool headbar_load_style( headbar_t *headbar )
{
    scene_t* scene;
    resgroup_resource_t *resgroup_top;
    unsigned int id_sub_resgoup;
    sty_rect_t rect;

    if ( NULL == headbar )
    {
        return false;
    }
    
    /* 重新load style, 需要整个控件全部重绘, 则将saved_show = -1, 
       来保证第一次show与saved_show不同, 而一定执行draw函数 */
    memset(&headbar->private->saved_show, -1, sizeof(headbar->private->saved_show));
    
    scene = sty_get_common_scene();
    if ( scene == NULL )
    {
        print_dbg("sty_get_common_scene fail\n\n");
        return false;   
    }
    //load top resource group
    resgroup_top = ( resgroup_resource_t * )get_scene_child( scene, HEADBAR_RESGROUP );
    if (NULL == resgroup_top)
    {
        return false;
    }
    
    //get child resgroup
    switch( headbar->type )
    {
    case HEADBAR_TYPE_NORMAL:
        id_sub_resgoup = HEADBAR_RESGROUP;
        break;
        
    case HEADBAR_TYPE_SIMPLE:
        id_sub_resgoup = HEADBAR_SIMPLE_RESGROUP;
        break;
        
    case HEADBAR_TYPE_YUV:
        id_sub_resgoup = HEADBAR_YUV_RESGROUP;
        break;
        
    default:
        print_err("headbar type:%d, out of range", headbar->type);
        return false;
    }

    //load sub resource group
    headbar->private->resgroup = ( resgroup_resource_t * )get_resgroup_child( resgroup_top, id_sub_resgoup );
    if (NULL == headbar->private->resgroup)
    {
        return false;
    }
    
    //load pic from resource group
    headbar->private->pic_bg        = ( picture_resource_t * )get_resgroup_child( \
                headbar->private->resgroup, PIC_BG );
    headbar->private->pic_music_play= ( picture_resource_t * )get_resgroup_child( \
                headbar->private->resgroup, PIC_MUSIC_PLAY );
    headbar->private->pic_keylock   = ( picture_resource_t * )get_resgroup_child( \
                headbar->private->resgroup, PIC_KEYLOCK );
    headbar->private->pic_card      = ( picture_resource_t * )get_resgroup_child( \
                headbar->private->resgroup, PIC_CARD );
    headbar->private->pic_usb       = ( picture_resource_t * )get_resgroup_child( \
                headbar->private->resgroup, PIC_USB );

    //load pic region from resource group
    headbar->private->reg_head      = ( picregion_resource_t * )get_resgroup_child( \
                headbar->private->resgroup, REG_HEAD);
    headbar->private->reg_battery   = ( picregion_resource_t * )get_resgroup_child( \
                headbar->private->resgroup, REG_BATTERY);
    
    //load string from resource group
    headbar->private->str_title     = ( string_resource_t * )get_resgroup_child( \
                headbar->private->resgroup, STR_TITLE);
    headbar->private->str_time      = ( string_resource_t * )get_resgroup_child( \
                headbar->private->resgroup, STR_TIME);

    //canvas resize    
    if ( headbar->canvas_id != -1 )
    {
        rect.x = headbar->private->resgroup->absolute_x;
        rect.y = headbar->private->resgroup->absolute_y;
        rect.width = headbar->private->resgroup->width;
        rect.height = headbar->private->resgroup->height;
        
        //set canvas region
        canvas_set_region( headbar->canvas_id, &rect );
        
        canvas_erase_all(headbar->canvas_id);
    }

    return true;
}


static bool _init( headbar_t *headbar )
{
    /* init show status */
    headbar->private->show.bg = 1;
    
    headbar->private->show.head = 1;
    headbar->private->show.battery = 1;
    
    headbar->private->show.title = 1;
    headbar->private->show.time = 1;
    
    get_config( CFG_TIME_FORMAT, &headbar->private->time_format, sizeof(headbar->private->time_format) );

    /* request timer */
    headbar->private->timer_id = set_timer(500, _proc_timer, (void *)headbar);
    if ( headbar->private->timer_id < 0 )
    {
        print_err("set timer fail\n\n");
        return false;
    }
    //create后立即获取参数
    _get_param( headbar );

    return true;
}


static bool _deinit( headbar_t *headbar )
{
    //kill timer
    if ( -1 != headbar->private->timer_id )
    {
        kill_timer(headbar->private->timer_id);
        headbar->private->timer_id = -1;
    }
    
    return true;
}


bool headbar_set_title_id( headbar_t *headbar, unsigned short id )
{
    if ( NULL == headbar )
    {
        return false;
    }
    
    headbar->str_id_title = id;
    headbar->str_buffer_title = NULL;
    
    //先擦除title, 再显示新内容
    headbar->private->show.title = 0;
    headbar_paint( headbar );
    headbar->private->show.title = 1;
    headbar_paint( headbar );
    
    return true;
}


bool headbar_set_title_buffer( headbar_t *headbar, char *buffer )
{
    if ( (NULL == headbar) || (NULL == buffer) )
    {
        return false;
    }
    
    headbar->str_buffer_title = buffer;
    
    //先擦除title, 再显示新内容
    headbar->private->show.title = 0;
    headbar_paint( headbar );
    headbar->private->show.title = 1;
    headbar_paint( headbar );
    
    return true;
}


bool headbar_set_time_format( headbar_t *headbar, time_format_e time_format )
{
    if ( NULL == headbar )
    {
        return false;
    }
    
    headbar->private->time_format = time_format;
    
    //先擦除time, 再显示新内容
    headbar->private->show.time = 0;
    headbar_paint( headbar );
    headbar->private->show.time = 1;
    headbar_paint( headbar );
    
    return true;
}


//bool headbar_paint( headbar_t *headbar, gui_update_mode_e mode )
bool headbar_paint( headbar_t *headbar )
{
    print_dbg("");
    if ( NULL== headbar )
    {
        return false;
    }
    
    headbar_draw_bg(headbar, headbar->private->show.bg);
    
    headbar_draw_music_play(headbar, headbar->private->show.music_play);
    headbar_draw_keylock(headbar, headbar->private->show.keylock);
    headbar_draw_card(headbar, headbar->private->show.card);
    headbar_draw_usb(headbar, headbar->private->show.usb);
    
    headbar_draw_head(headbar, headbar->private->show.head, headbar->pic_id_head);
    headbar_draw_battery(headbar, headbar->private->show.battery, headbar->private->show.battery_frame);
    
    if ( NULL == headbar->str_buffer_title )
    {
        headbar_draw_title_by_id(headbar, headbar->private->show.title, headbar->str_id_title);
    }
    else
    {
        headbar_draw_title_by_buffer(headbar, headbar->private->show.title, headbar->str_buffer_title);
    }
    
    headbar_draw_time(headbar, headbar->private->show.time, headbar->private->buffer_str_time);
    
    return true;
}


bool headbar_repaint( headbar_t *headbar )
{
    print_dbg("");
    if ( NULL== headbar )
    {
        return false;
    }
    
    memset( &headbar->private->saved_show, -1, sizeof(headbar->private->saved_show) );
    headbar_paint( headbar );
    
    return true;
}


static int _proc_key( headbar_t *headbar, key_event_t *key_event )
{
    /* 调用应用的回调 */
    
    /* 自身处理按键消息 */
    switch ( key_event->val | key_event->type ) 
    {
    case KEY_VOL | KEY_TYPE_DOWN:
        break;
    case KEY_VSUB | KEY_TYPE_DOWN:
        break;
    case KEY_VADD | KEY_TYPE_DOWN:
        break;
        
    case KEY_PREV | KEY_TYPE_DOWN:
        break;
    case KEY_NEXT | KEY_TYPE_DOWN:
        break;
        
    case KEY_MENU | KEY_TYPE_DOWN:
        break;
        
    case KEY_OK | KEY_TYPE_DOWN:
        break;
        
    case KEY_RETURN | KEY_TYPE_DOWN:
        break;

    default:
        break;
    }
    
    return 1;
}


int headbar_proc_gui( headbar_t *headbar, gui_wm_message_t *pmsg)
{
    print_dbg("msgid:%d", pmsg->msgid);
    if ( NULL == headbar )
    {
        return false;
    }
    
    switch ( pmsg->msgid )
    {
    case WM_MSG_KEY:
        _proc_key( headbar, (key_event_t *)(pmsg->data.p));
        break;
        
    default:
        break;
    }
    
    return 1;
}


headbar_t* headbar_create( headbar_init_t *init, headbar_cb_t callback )
{
    bool result = true;
    sty_rect_t srect;
    headbar_t *headbar;

//    print_dbg("type:%d, pic_id_head:0x%x, str:%s", init->type, init->pic_id_head, init->str_id_title);
    //对传入的参数进行检查
    if ( (NULL == init) || (init->type >= HEADBAR_TYPE_MAX) || (NULL == callback) )
    {
        print_err("create param err\n\n");
        return NULL;
    }
    //NORMAL 类型需要检查pic_id_head
    if ( (HEADBAR_TYPE_NORMAL == init->type) || (HEADBAR_TYPE_YUV == init->type) )
    {
        if ( 0 == init->pic_id_head )
        {
            print_err("create param err\n\n");
            return NULL;
        }
    }
    
    //malloc headbar
    headbar = (headbar_t *)malloc( sizeof(headbar_t) );     
    if ( NULL == headbar)
    {
        print_err("malloc headbar_t fail\n\n");
        goto HEADBAR_ERR_MALLOC_DIALOG;
    }
    memset(headbar, 0x00, sizeof(headbar_t));
    
    headbar->private = (headbar_private_t *)malloc( sizeof(headbar_private_t) );        
    if ( NULL == headbar->private)
    {
        print_err("malloc headbar_private_t fail\n\n");
        goto HEADBAR_ERR_MALLOC_PRIVATE;
    }
    memset(headbar->private, 0x00, sizeof(headbar_private_t));
    
    /* init headbar */
    headbar->type       = init->type;
    headbar->pic_id_head= init->pic_id_head;
    headbar->str_id_title= init->str_id_title;
    headbar->callback   = callback;
    headbar->canvas_id = -1;
    result = _init( headbar );
    if (false == result)
    {
        print_err("dialog_init fail\n\n");
        goto HEADBAR_ERR_INIT;
    }
    
    //load style resource
    result = headbar_load_style( headbar );
    if (false == result)
    {
        print_err("headbar_load_style fail\n\n");
        goto HEADBAR_ERR_INIT;
    }
    
    //create canvas必须使用绝对坐标
    srect.x     = headbar->private->resgroup->absolute_x;
    srect.y     = headbar->private->resgroup->absolute_y;
    srect.width = headbar->private->resgroup->width;
    srect.height= headbar->private->resgroup->height;
    headbar->canvas_id = canvas_create( &srect, TRANSPARENT );
    if ( -1 == headbar->canvas_id )
    {
        print_warning("canvas_create fail\n\n");
        goto HEADBAR_ERR_CANVAS;
    }
    canvas_erase_all(headbar->canvas_id);
    //禁止headbar为焦点窗口
    canvas_disable_focus(headbar->canvas_id);
    
    //获取gui dc
    headbar->private->handle_gui_dc = gui_dc_get( headbar->canvas_id );
    if (headbar->private->handle_gui_dc <= 0)
    {
        print_err("gui_dc_get fail\n\n");
        goto HEADBAR_ERR;
    }
    
    return headbar;
    
    HEADBAR_ERR:
    canvas_delete( headbar->canvas_id );
    headbar->canvas_id = -1;
    
    HEADBAR_ERR_CANVAS:
    kill_timer(headbar->private->timer_id);
    headbar->private->timer_id = -1;
    
    HEADBAR_ERR_INIT:
    free( headbar->private );
    
    HEADBAR_ERR_MALLOC_PRIVATE:
    free( headbar );
    
    HEADBAR_ERR_MALLOC_DIALOG:
    return NULL;
}


bool headbar_delete( headbar_t *headbar )
{
    print_dbg();

    if ( NULL == headbar )
    {
        print_warning("headbar is NULL\n\n");
        return false;
    }
    
    _deinit( headbar );
    
    //delete canvas
    if ( headbar->canvas_id >= 0 )
    {
        canvas_delete( headbar->canvas_id );
        headbar->canvas_id = -1;
    }
    
    //free memory
    free( headbar->private );
    headbar->private = (headbar_private_t *)NULL;
    free( headbar );
    headbar = (headbar_t *)NULL;
    
    return true;
}

