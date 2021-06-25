/********************************************************************************
 *                              USDK(1100)
 *                             Module: app
 *                 Copyright(c) 2003-2009 Actions Semiconductor,
 *                            All Rights Reserved.
 * File : tvin_scene_palntsc.c
 * History:
 *      <author>    <time>           <version >             <desc>
 *      Jerry Fu    2009-4-28             v1.0               build this file
 ********************************************************************************/
/*!
* \file     tvin_scene_palntsc.c
* \brief    tvin  palntsc scene
* \author   Jerry Fu
*
* \version 1.0
* \date  2009/04/28
*******************************************************************************/

#include "tvin.h"

static tvin_appdata_t  *tvin_appdata = NULL;

/* global variable define */
static tvin_scene_palntsc_t  *s_palntsc = NULL;

static bool _tvin_palntsc_create_headbar( headbar_init_t *headbar_init );
static bool _tvin_palntsc_delete_headbar( void);
static void _tvin_headbar_cb(headbar_t *headbar, widget_sig_headbar_e signal);

static bool _create_msgbox(void);
static bool _delete_msgbox(void);
static void  _msgbox_cb(msgbox_t *msg_box, msgbox_sig_e  msgbox_type);

static bool _scene_palntsc_rotate(void);

static bool _scene_palntsc_open_theme(void);
static bool _scene_palntsc_close_theme(void);

static void _scene_palntsc_proc_gui(gui_wm_message_t *pmsg);
static void  _scene_palntsc_callback(gui_wm_message_t *pmsg);
static int _scene_palntsc_proc_sys(msg_apps_t *msg);

static bool _load_palntsc_style(tvin_scene_palntsc_t *palntsc);
static bool  _scene_palntsc_init( tvin_appdata_t *app_data );
static bool  _scene_palntsc_deinit( void );

//返回palntsc场景数据结构的指针
tvin_scene_palntsc_t *fetch_tvin_palntsc(void)
{
    return s_palntsc;
}


static bool  _get_tvout_info(unsigned int *tvout_info)
{
    int fd;
    int ret;
    
    fd = open(DEVICE_FB, O_RDWR);
    if(fd < 0)
    {
        print_err("open alarm driver error\n");
        return false;
    }    

    ret = ioctl(fd, FBIOGET_DISPLAYER, (int)tvout_info);
    if(ret < 0)
    {
        print_err("get fb displayer failed\n");
        close(fd);
        return false;
    }
    close(fd);
    print_dbg("fb displayer=%x\n",*tvout_info);
    return true;
}



/******************************************************************************/
/*!                    
* \par  Description:
*      dialog信息提示框的回调函数
* \param[in]    dialog   dialog结构体指针
* \param[in]    dialog   信号的类型
* \retval       void
* \ingroup      tvin
*******************************************************************************/
static void _palntsc_dialog_cb(dialog_t *dialog, widget_sig_dialog_e signal)
{
    switch ( signal )
    {
    case WIDGET_SIG_DIALOG_OK:    
    case WIDGET_SIG_DIALOG_CANCAL:
        //点击了:no, 或者return, 不执行任何操作. 删除dialog               
        if(true == dialog_delete(s_palntsc->dialog))
        {
            s_palntsc->dialog = NULL;
        }
        else
        {
            print_err("delete dialog error\n");
        }
        s_palntsc->next_scene = SCENE_TVIN_EXIT;       
        exit_msg_loop();
        break;

    default:
        break;
    }
}


/******************************************************************************/
/*!                    
* \par  Description:
*      创建dialog信息提示框
* \param[in]    void
* \retval       成功: true  失败:false
* \ingroup      tvin
*******************************************************************************/
static bool _palntsc_create_dialog(unsigned short str_id, dialog_head_e head)
{
    dialog_init_t  dialog_init;
    
    memset(&dialog_init, 0, sizeof(dialog_init_t));

    dialog_init.head = head;
    dialog_init.str_id = str_id;
    	
    /* init and callback function */
    s_palntsc->dialog = dialog_create(&dialog_init, _palntsc_dialog_cb);
    if ( NULL == s_palntsc->dialog )
    {
        print_err("dialog_create failed\n\n");
        return false;
    }  
    dialog_paint(s_palntsc->dialog);
    return true;
}

static bool _display_dev_init_failed(void)
{
    bool result = false;
    result = _palntsc_create_dialog(s_palntsc->str_dev_installed->str_id, DIALOG_HEAD_FAIL);
    return result;
}

static bool _display_tvout_is_on(void)
{
    bool result = false;
    result = _palntsc_create_dialog(s_palntsc->str_tvout_info->str_id, DIALOG_HEAD_WARN);
    return result;
}


static bool _display_avline_notinsert(void)
{
    bool result = false;
    result = _palntsc_create_dialog(s_palntsc->str_cable_info->str_id, DIALOG_HEAD_WARN);
    return result;
}


static bool _is_tvout_on(void)
{
    int tvout_info;
    if(true == _get_tvout_info(&tvout_info))
    {
        print_dbg("get tvout info is: %x\n",tvout_info);
        if((tvout_info & TV_PAL_DISPLAYER) || (tvout_info & TV_NTSC_DISPLAYER))
        {
            return true;  /*假如TVOUT已经打开了*/
        }
        else
        {
            return false; /*TVOUT没有打开*/
        }
    }
    else
    {
        /*假如读取TVOUT的信息出错了，当作TVOUT已经打开了*/
        return true;
    }
}

//palntsc 场景绘图函数
static void _scene_palntsc_paint(void)
{
    s_palntsc->hdc = gui_dc_get((unsigned short)(s_palntsc->win_id));    
    sty_draw_bitmap( s_palntsc->hdc, s_palntsc->bg_bitmap_info, 0, 0 );
}

//headbar 控件的回调函数
static void _tvin_headbar_cb(headbar_t *headbar, widget_sig_headbar_e signal)
{
    print_dbg("signal:%d", signal);
    switch ( signal )
    {
    case WIDGET_SIG_HEADBAR_NULL:
        break;

    default:
        break;
    }
}

static bool _tvin_palntsc_create_headbar( headbar_init_t *headbar_init )
{
    /* init type and callback function */
    s_palntsc->hbar = headbar_create(headbar_init, _tvin_headbar_cb);
    if ( NULL == s_palntsc->hbar )
    {
        print_err("headbar_create failed\n\n");
        return false;
    }
    return true;
}

static bool _tvin_palntsc_delete_headbar( void)
{
    bool bret;
    bret = headbar_delete( s_palntsc->hbar);
    s_palntsc->hbar = NULL;
    return bret;
}


static void  _msgbox_cb(msgbox_t *msg_box, msgbox_sig_e  msgbox_type)
{
    switch(msgbox_type)
    {
    case WIDGET_SIG_MSGBOX_SHOW:
        print_dbg("message box is show\n");
        break;
    
    case WIDGET_SIG_MSGBOX_QUIT:
        print_dbg("message box will be quit\n"); 
        break;
        
    default:
        break;
    }
}


static bool _create_msgbox(void)
{
    s_palntsc->msgbox = msgbox_create(_msgbox_cb);
    if (NULL == s_palntsc->msgbox)
    {
        print_err("create msgbox failed!");
        return false;
    }
    return true;
}


static bool _delete_msgbox(void)
{
    if(s_palntsc->msgbox != NULL)
    {
        msgbox_delete(s_palntsc->msgbox);
        s_palntsc->msgbox = NULL;
    }
    return true;
}

static bool _scene_palntsc_rotate(void)
{
    fusion_effect_t fusion_effect = {0};
    fusion_get_config(FUSION_TYPE_ROTATE, FUSION_FORWARD, &fusion_effect);
    fusion_set_effect(&fusion_effect);
    	
    fusion_set_first_frame(NULL);
    _scene_palntsc_close_theme();
    _scene_palntsc_open_theme();
    _load_palntsc_style(s_palntsc);

    gui_wm_set_size(s_palntsc->win_id, s_palntsc->scene->width, s_palntsc->scene->height);

    _scene_palntsc_paint();

    if ( s_palntsc->menuview != NULL)
    {
        tvin_widget_menu_load_style(s_palntsc->menuview);
        menuview_paint(s_palntsc->menuview);
    }

    if ( s_palntsc->hbar != NULL)
    {
        headbar_load_style(s_palntsc->hbar);
        headbar_paint(s_palntsc->hbar);
    }
              
    if(s_palntsc->msgbox != NULL)
    {
        msgbox_load_style(s_palntsc->msgbox);
        msgbox_paint(s_palntsc->msgbox);
    }

    fusion_set_last_frame(NULL);
    fusion_display( NULL);

    return true;
}

static bool _scene_palntsc_open_theme(void)
{
    rotation_e angle;
    char common_file[PATH_MAX], app_file[PATH_MAX];

    angle = sty_get_view_rotation(); // 取视图角度
    sty_set_screen_rotation(angle);  // 设置屏幕方向
    tvin_appdata->save_angle = angle;

    if( (angle == ROTATION_0) || (angle == ROTATION_180) )
    {
        s_palntsc->common_scene = load_scene(tvin_appdata->commonui_style, COMMONUI_SIDELONG_SCENE);
        s_palntsc->scene = load_scene(tvin_appdata->tvin_style, TVIN_PALNTSC_SIDELONG_SCENE);
    }
    else
    {
        s_palntsc->common_scene = load_scene(tvin_appdata->commonui_style, COMMONUI_UPRIGHT_SCENE);
        s_palntsc->scene = load_scene(tvin_appdata->tvin_style, TVIN_PALNTSC_UPRIGHT_SCENE);
    }
    
    if( (s_palntsc->common_scene == NULL) || (s_palntsc->scene == NULL) )
    {
        print_err("load desktop scene failed");
        return false;
    }
    else
    {
        /*设置 commonui的style资源  给style驱动的封装层，为sty_get_common_scene()做准备*/
        sty_set_common_scene(s_palntsc->common_scene);
        // 设置应用的 style 场景,为sty_get_app_scene()做准备
        sty_set_app_scene(s_palntsc->scene); 
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

static bool _scene_palntsc_close_theme(void)
{

    free_scene(s_palntsc->common_scene);
    s_palntsc->common_scene = NULL;

    free_scene(s_palntsc->scene);
    s_palntsc->scene = NULL;

    if(sty_close_common_pic_res() == false)
    {
        print_err("close common picture resource error!");
        return false;
    }

    if(sty_close_app_pic_res() == false)
    {
        print_err("close app picture resource error!");
        return false;
    } 
    return true;
}


static bool _scene_palntsc_open_str_res(void)
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


static bool _scene_tvin_palntsc_close_str_res(void)
{
    if(sty_close_common_str_res() == false)
    {
        print_err("close common string resource error!");
        return false;
    }

    if(sty_close_app_str_res() == false)
    {
        print_err("close app string resource error!");
        return false;
    } 

    return true;
}


static void _scene_palntsc_proc_gui(gui_wm_message_t *pmsg)
{  
    key_event_t *kmsg;
    tvin_scene_palntsc_t *palntsc;
    tvin_scene_record_t *prec = NULL;
    palntsc = s_palntsc;
    
    //main window process
    switch (pmsg->msgid) 
    {
    case WM_MSG_CREATE:
        break;
        
    case WM_MSG_PAINT:
        break;
        
    case WM_MSG_KEY:
        prec = fetch_tvin_record();
        if(NULL != prec)
        {
            prec->wake_up = 0;  
        }
                
        kmsg = (key_event_t *)(pmsg->data.p);
        switch ( kmsg->val | kmsg->type)
        {
        case  KEY_OK | KEY_TYPE_SHORT_UP:		
            palntsc->next_scene = SCENE_TVIN_RECORD;
            exit_msg_loop();       
            break;
                            
        case KEY_RETURN | KEY_TYPE_DOWN:
            palntsc->next_scene = SCENE_TVIN_EXIT;
            exit_msg_loop();       
            break;
            
        default:
            break;
        } 
        break;
        
    default:
        gui_wm_default_callback(pmsg);
        break;
    }
}



static void  _scene_palntsc_callback(gui_wm_message_t *pmsg)
{
    int top;

    _scene_palntsc_proc_gui(pmsg);
    
    top = canvas_get_focus(pmsg);
    if((s_palntsc->menuview != NULL) && (top == s_palntsc->menuview->canvas_id))
    {
        tvin_widget_palntsc_list_proc_gui(s_palntsc->menuview, pmsg);   
    }
    else if((s_palntsc->msgbox != NULL) && (top == s_palntsc->msgbox->canvas_id))
    {
        msgbox_proc_gui(s_palntsc->msgbox, pmsg);
    }
    else
    {
        
    }
}


static int _scene_palntsc_proc_sys(msg_apps_t *msg)
{
    int clock_no;
    print_info("msgtype:%d, content[0]:0x%x", msg->type, msg->content[0]);
    switch (msg->type)
    {
    case MSG_GSENSOR_ROTATION_CHANGED:
        print_info("received msg: MSG_GSENSOR_ROTATION_CHANGED");
        _scene_palntsc_rotate();
        break;

    case MSG_APP_QUIT:
        exit_msg_loop();
        s_palntsc->next_scene = SCENE_TVIN_EXIT;
        break;

    case MSG_TVIN_ALARM_IN:
        memcpy(&clock_no, msg->content, sizeof(clock_no));
        tvin_appdata->alarm_clock = clock_no;
        tvin_appdata->app_start_mode = APP_START_RTC;  
        s_palntsc->next_scene = SCENE_TVIN_RECORD;
        exit_msg_loop();
        break;
        
    default:
        if(s_palntsc->msgbox != NULL)
        {
            /* 控件msgbox用来处理默认系统消息(低电提示、卡插入和拔出提示、USB插入功能选择提示等) */
            msgbox_proc_sys(s_palntsc->msgbox, msg);
        }
        break;
    }

    return 0;
}

static bool _load_palntsc_style(tvin_scene_palntsc_t *palntsc)
{
    palntsc->group = (resgroup_resource_t *)get_scene_child(palntsc->scene, STR_PALNTSC_RES);            
    if(NULL == palntsc->group)    
    {
        print_err("get scene palntsc resgroup error\n");
        return false;
    }

    palntsc->str_tvin_pal = (string_resource_t *)get_resgroup_child(palntsc->group, STR_TV_PAL);
    if(NULL == palntsc->str_tvin_pal)
    {
        print_err("get pal string error\n");
        return false;
    }

    palntsc->str_tvin_ntsc = (string_resource_t *)get_resgroup_child(palntsc->group, STR_TV_NTSC);    
    if(NULL == palntsc->str_tvin_ntsc)
    {
        print_err("get ntsc string error\n");
        return false;
    }

    palntsc->str_hb_title = (string_resource_t *)get_scene_child(palntsc->scene, STR_HB_TITLE);
    if(NULL == palntsc->str_hb_title)
    {
        print_err("get headbar title string error\n");
        return false;
    }

    palntsc->str_cable_info = (string_resource_t *)get_scene_child(palntsc->scene, STR_CABLE_INSERT_INFO);
    if(NULL == palntsc->str_cable_info)
    {
        print_err("get av cable insert string error\n");
        return false;
    }

    palntsc->str_tvout_info = (string_resource_t *)get_scene_child(palntsc->scene, STR_TVOUT_INFO);
    if(NULL == palntsc->str_tvout_info)
    {
        print_err("get tvout string error\n");
        return false;
    }

    palntsc->str_dev_installed = (string_resource_t *)get_scene_child(palntsc->scene, STR_DEV_FAILED);
    if(NULL == palntsc->str_tvout_info)
    {
        print_err("get tvout string error\n");
        return false;
    }

    palntsc->pic_hb_icon = (picture_resource_t *)get_scene_child(palntsc->scene, PIC_TVIN_ICON);    
    if(NULL == palntsc->pic_hb_icon)
    {
        print_err("get headbar icon error\n");
        return false;
    }

    //load pic and pic region from scene or resource group
    palntsc->bg_bitmap_info = get_user_bg_pic(BG_TVIN);
    if ( NULL == palntsc->bg_bitmap_info )
    {
        palntsc->pic_bg = ( picture_resource_t * )get_scene_child( palntsc->scene, PIC_PALNTSC_BG );
        if(NULL == palntsc->pic_bg)
        {
            print_err("open scene background picture error\n");
            return false;
        }

        palntsc->bg_bitmap_info = sty_create_bitmap((int)palntsc->pic_bg->pic_id );
        if ( NULL == palntsc->bg_bitmap_info )
        {
            print_err("get background picture error\n");
            return false;
        }
    }    
    return true;
}

static bool _scene_palntsc_set_default_font(void)
{
    /* crate font */
    if(s_palntsc->fontdes > 0)
    {
        return false;
    }

    s_palntsc->fontdes = gui_create_font((unsigned char*)sys_get_default_font_file(), 16);
    if(s_palntsc->fontdes < 0)
    {
        print_err("call gui_create_font failed");
        return false;
    }

    if(gui_dc_set_default_fontface(s_palntsc->fontdes) < 0) 
    {
        print_err("gui_dc_set_default_fontface failed");
        return false;
    }

    return true;
}

static bool _scene_palntsc_unset_default_font(void)
{
    /* crate font */
    if(s_palntsc->fontdes <= 0)
    {
        return false;
    }

    gui_destroy_font(s_palntsc->fontdes);
    s_palntsc->fontdes = 0; 
   
    return true;
}


/* scene initial function */
static bool  _scene_palntsc_init( tvin_appdata_t *app_data )
{
    bool ret_result;    
    headbar_init_t headbar_init;
    fusion_effect_t fusion_effect = {0};

    fusion_get_config(FUSION_TYPE_APP_CHANGE, FUSION_FORWARD, &fusion_effect);
    fusion_set_effect(&fusion_effect);

    sys_forbid_auto_standby(false);
    sys_forbid_screensaver(false);

    //分配场景数据空间
    if( app_data->palntsc_data == NULL )
    {
        app_data->palntsc_data = malloc( sizeof(tvin_scene_palntsc_t) );
        
        if( app_data->palntsc_data == NULL )
        {
            print_err("malloc scene palntsc data structure error\n");
            return false;
        }
    }

    s_palntsc = app_data->palntsc_data;
    memset( app_data->palntsc_data, 0, sizeof(tvin_scene_palntsc_t) );//clear                                     

    if(strcmp(g_tvin_cfgs[TVIN_CFG_TVIN_SIGNAL].text, "PAL") == 0)
    {
        s_palntsc->tvin_signal_palntsc = TV_SIGNAL_PAL;
    }
    else
    {
        s_palntsc->tvin_signal_palntsc = TV_SIGNAL_NTSC;
    }            
          
    s_palntsc->next_scene = SCENE_TVIN_EXIT;
            
    /* load theme 资源文件 */
    if(_scene_palntsc_open_theme() == false)
    {
        return false;
    }

    /* load 字符串资源文件 */
    if(_scene_palntsc_open_str_res() == false)
    {
        return false;
    }

    /* 创建字体 */
    _scene_palntsc_set_default_font();

    if(false == _load_palntsc_style(s_palntsc))
    {
        return false;
    }
                      
    //创建窗口
    s_palntsc->win_id = gui_wm_create_window(s_palntsc->scene->x, s_palntsc->scene->y,\
        s_palntsc->scene->width, s_palntsc->scene->height, WM_CREATE_SHOW, _scene_palntsc_callback, 0);

    if( s_palntsc->win_id == 0)
    {  
        return false;
    }
    //设置焦点
    gui_wm_set_focus( s_palntsc->win_id ); 

    if(NULL == fetch_tvin_record())
    {
        tvin_scene_fusion_set_first_frame(s_palntsc->scene);
    }
        
    //draw backgroud picture
    headbar_init.type = HEADBAR_TYPE_NORMAL;
    headbar_init.pic_id_head = s_palntsc->pic_hb_icon->pic_id;
    headbar_init.str_id_title = s_palntsc->str_hb_title->str_id;    
    ret_result = _tvin_palntsc_create_headbar(&headbar_init);
    if(true == ret_result)
    {
        headbar_paint(s_palntsc->hbar);
    }    

    s_palntsc->menuview = tvin_widget_palntsc_list_create(); 
    if(NULL == s_palntsc->menuview)
    {
        return false;
    }
    
    _create_msgbox();

#if  1
    if(false == app_data->dev_installed)
    {
        _display_dev_init_failed();
    }
    else if(true == _is_tvout_on())
    {
        _display_tvout_is_on();
    }
    else if(false == is_tvin_cable_insert())
    {         
        _display_avline_notinsert();
    }
    else
    {

    }
#endif
        
    _scene_palntsc_paint();

    if(NULL == fetch_tvin_record())
    {
        tvin_scene_fusion_set_last_frame(s_palntsc->scene);
        fusion_display( NULL);
    }

    clear_key_msg_queue(-1,-1);
    return true;
}


/* scene exit function */
static bool  _scene_palntsc_deinit( void )
{ 
    //删除message box
    _delete_msgbox();

    //删除headbar
    _tvin_palntsc_delete_headbar();

    //删除list控件
    tvin_palntsc_list_exit(s_palntsc->menuview);
    
    //删除背景图
    if (NULL != s_palntsc->bg_bitmap_info)
    {
        sty_delete_bitmap(s_palntsc->bg_bitmap_info);
        s_palntsc->bg_bitmap_info = NULL;
    }
    
    //删除窗体
    gui_wm_delete_window( s_palntsc->win_id );
    s_palntsc->win_id = 0;

    /* 删除所创建的字体 */
    _scene_palntsc_unset_default_font();
    
    //退出场景
    if(false == _scene_palntsc_close_theme())
    {
        return false;
    }

    //释放字符串资源
    if(false == _scene_tvin_palntsc_close_str_res())
    {
        return false;
    }    
    return true;
}


/* scene entry function */
bool  scene_palntsc( tvin_appdata_t *app_data )
{
    bool  ret;
    msg_apps_t  msg;

    tvin_appdata = app_data;
    /* 场景初始化 */
    ret = _scene_palntsc_init(app_data);
    if(false == ret)
    {
        print_err("scene init error! \n");
        return false;
    }
    else
    {
        print_dbg();
    }
           
    register_sys_dispatcher(_scene_palntsc_proc_sys);

    while(true == get_msg(&msg))
    {
        dispatch_msg(&msg);
    }

    unregister_sys_dispatcher();


    /* 场景退出 */
    ret = _scene_palntsc_deinit();
    if(false == ret)
    {
        print_err("scene exit error! \n");
        return false;
    }
    else
    {
        print_dbg();
    }    
    return true;
}


