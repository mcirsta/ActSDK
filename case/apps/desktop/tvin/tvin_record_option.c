/********************************************************************************
*                              USDK(1100)
*                             Module: app
*                 Copyright(c) 2003-2009 Actions Semiconductor,
*                            All Rights Reserved.
* File : tvin_record_option.c
* History:
*      <author>    <time>           <version >             <desc>
*      Jerry Fu    2009-4-28             v1.0               build this file
********************************************************************************/
/*!
* \file     tvin_record_option.c
* \brief    define tvin options
* \author   Jerry Fu
*
* \version 1.0
* \date  2009/04/28
*******************************************************************************/
#include "tvin.h"
#include "tvin_record_option.h"

int singlerec_num;  //��ʾ�˵��п����õĵ��ζ�ʱ¼��ĸ���

static menu_t  *s_ptvin_menu = NULL;
static tvin_scene_record_t *s_tvin_record;

static int _option_proc_singlerec( menuview_t *menuview, menuview_sig_e signal );
static int _option_proc_repeatrec(menuview_t *menuview, menuview_sig_e  signal);
static int _option_proc_savepath(menuview_t *menuview, menuview_sig_e  signal);
static int _option_proc_setlcd(menuview_t *menuview, menuview_sig_e  signal);
static int _option_proc_setquality(menuview_t *menuview, menuview_sig_e  signal);
static void _adjust_setlcd_callback(adjust_t *adjust, adjust_signal_e signal);
static void _adjust_set_switch_info(adjust_switch_init_t *adjust_switch);
static menu_t* _tvin_option_init(void);

static menuitem_t s_option_items[] =
{   
    { 0, 0, STR_SINGLE_REC1,  true,   (item_cb_t)_option_proc_singlerec,   NULL, NULL, NULL, NULL },
    { 0, 0, STR_SINGLE_REC2,  true,   (item_cb_t)_option_proc_singlerec,   NULL, NULL, NULL, NULL },
    { 0, 0, STR_SINGLE_REC3,  true,   (item_cb_t)_option_proc_singlerec,   NULL, NULL, NULL, NULL },
    { 0, 0, STR_REPEAT_REC1,  true,   (item_cb_t)_option_proc_repeatrec,   NULL, NULL, NULL, NULL },    
    { 0, 0, STR_REPEAT_REC2,  true,   (item_cb_t)_option_proc_repeatrec,   NULL, NULL, NULL, NULL },
    { 0, 0, STR_REPEAT_REC3,  true,   (item_cb_t)_option_proc_repeatrec,   NULL, NULL, NULL, NULL },
    { 0, 0, STR_SAVE_PATH,    true,   (item_cb_t)_option_proc_savepath,    NULL, NULL, NULL, NULL },
    { 0, 0, STR_SET_LCD,      false,  (item_cb_t)_option_proc_setlcd,      NULL, NULL, NULL, NULL },
    { 0, 0, STR_RECORD_QUALITY, true, (item_cb_t)_option_proc_setquality,  NULL, NULL, NULL, NULL },
};
/* max optins items*/
#define  TVIN_OPTION_MAX_ITEMS   ( sizeof(s_option_items)/sizeof(menuitem_t) )

/* options items visible config */
static char *s_options_config[TVIN_OPTION_MAX_ITEMS] =
{
    CFG_TVIN_SINGLEREC1_VISIBLE,
    CFG_TVIN_SINGLEREC2_VISIBLE,
    CFG_TVIN_SINGLEREC3_VISIBLE,
    CFG_TVIN_REPEATREC1_VISIBLE,
    CFG_TVIN_REPEATREC2_VISIBLE,
    CFG_TVIN_REPEATREC3_VISIBLE,
    CFG_TVIN_SAVEPATH_VISIBLE,
    CFG_TVIN_LCD_ONOFF_VISIBLE,
    CFG_TVIN_RECORD_QUALITY_VISIBLE,
};

/* optins items visible flag */
static bool s_options_visible[TVIN_OPTION_MAX_ITEMS];

bool _load_visible_config( void )
{
    static bool s_loaded = false;
    int result;
    bool value = false;
    int num;
    if ( s_loaded == true )
    {
        return true;
    }
    else
    {
        s_loaded = true;
    }
    singlerec_num = 0;
    //load visible flag
    for ( num=0; num<(int)TVIN_OPTION_MAX_ITEMS; num++ )
    {
        result = get_config( s_options_config[num], (char*)&value, sizeof(bool) );
        if( result < 0 )
        {
            value = false;
        }
        s_options_visible[num] = value;
        /*�жϵ�ǰ���õĵ���¼��ĸ���������������ƫ��
        ��Ϊoptions�˵����ظ�¼���indexҪ��ȥǰ��ĵ���¼�����*/
        if((num<3) && (s_options_visible[num]==true))
        {
            singlerec_num++;
        }
    }
    return true;
}

static int  _save_lcd_onoff_cfg(adj_switch_mode_e on_off)
{    
    char buf[32];

    if(ADJUST_ON == on_off)
    {
        sprintf(buf, "LCD_ON");
    }
    else
    {
        sprintf(buf, "LCD_OFF");
    }
    
    memcpy(&g_tvin_cfgs[TVIN_CFG_LCD_ONOFF].text, buf, sizeof(buf));
    g_tvin_cfgs[TVIN_CFG_LCD_ONOFF].changed = true;
    return 1;
}

static int _option_proc_singlerec( menuview_t *menuview, menuview_sig_e signal )
{                                   
    switch ( signal ) 
    {
    case WIDGET_SIG_MENUVIEW_OK:
        // enter menu
        tvin_option_singlerec_enter(menuview);          
        break;
        
    case WIDGET_SIG_MENUVIEW_QUIT:
        // return to parent menu
        tvin_option_exit(menuview);        
        break;
        
    default:
        break;
    } 
    return 1;
}

static int _option_proc_repeatrec(menuview_t *menuview, menuview_sig_e  signal)
{                    
    switch ( signal ) 
    {
    case WIDGET_SIG_MENUVIEW_OK:
        // enter menu
        tvin_option_repeatrec_enter(menuview);      
        break;
        
    case WIDGET_SIG_MENUVIEW_QUIT:
        // return to parent menu
        tvin_option_exit(menuview); 
        break;
        
    default:
        break;
    }
    return 1;
}


static int _option_proc_savepath(menuview_t *menuview, menuview_sig_e  signal)
{    
    switch ( signal )
    {
    case WIDGET_SIG_MENUVIEW_OK:
        // enter menu
        tvin_option_savepath_enter(menuview);
        break;
        
    case WIDGET_SIG_MENUVIEW_QUIT:
        // return to parent menu
        tvin_option_exit(menuview);
        break;
        
    default:
        break;
    } 
    return 1;
}


static void  _adjust_setlcd_callback(adjust_t *adjust, adjust_signal_e signal)
{       
    adj_switch_mode_e  on_off;
    switch(signal)
    {
    case WIDGET_SIG_ADJUST_OK:
        //ȡ�ص�ǰ��LCD������
        adjust_get_switch_status(&on_off);
        _save_lcd_onoff_cfg(on_off);
        s_tvin_record->tvin_config.lcd_on_off = on_off;
        tvin_adjust_blending_delete();
        adjust_delete(adjust);        
        s_tvin_record->adjust = NULL;

        if(ADJUST_ON == on_off)
        {
            sys_forbid_screensaver(true);
        }
        else
        {
            sys_forbid_screensaver(false);
        }                               
        break;
        
    case WIDGET_SIG_ADJUST_CANCEL:
        tvin_adjust_blending_delete();
        adjust_delete(adjust);
        s_tvin_record->adjust = NULL;        
        break;
   
    default:
        break;
    }
}

static void _adjust_set_switch_info(adjust_switch_init_t *adjust_switch)
{
    unsigned short str_id = 0;
    resgroup_resource_t *group;
    string_resource_t *str_res;
    memset(adjust_switch, 0, sizeof(adjust_switch_init_t));
    group = (resgroup_resource_t *)get_scene_child(s_tvin_record->scene, STR_OPTION_RES);
    str_res = (string_resource_t *)get_resgroup_child(group,STR_SET_LCD);
    if(str_res != NULL)
    {
        str_id = str_res->str_id;
    }
    adjust_switch->str_title_id = (int)str_id;
    adjust_switch->on_off_status = s_tvin_record->tvin_config.lcd_on_off;
}

static int _option_proc_setlcd(menuview_t *menuview, menuview_sig_e  signal)
{                    
    adjust_switch_init_t  adjust_switch;
    switch ( signal ) 
    {
    case WIDGET_SIG_MENUVIEW_OK:        
        //׼�� adjust�ؼ�����������
        _adjust_set_switch_info(&adjust_switch);
        //����adjust�İ���������        
        //����adjust�ؼ�
        s_tvin_record->adjust = adjust_create(&adjust_switch, ADJ_STYLE_SWITCH, _adjust_setlcd_callback);        
        tvin_adjust_blending_create();
        tvin_blending_show_spec(TVIN_BLENDING_ADJUST);
        break;
        
    case WIDGET_SIG_MENUVIEW_QUIT:
        tvin_option_exit(menuview);
        break;
        
    default:
        break;
    } 
    return 1;
}

static int _option_proc_setquality(menuview_t *menuview, menuview_sig_e  signal)
{
    switch ( signal ) 
    {
    case WIDGET_SIG_MENUVIEW_OK:        
        tvin_option_quality_enter(menuview);
        break;
        
    case WIDGET_SIG_MENUVIEW_QUIT:
        tvin_option_exit(menuview);
        break;
        
    default:
        break;
    }
    return 1;
}

static menu_t* _tvin_option_init(void)
{
    scene_t* scene;
    menu_t *pMenu;
    tvin_scene_record_t *pRecord;
    string_resource_t *str_res;
    
    menuitem_t item;
    int num;
    pMenu = s_ptvin_menu;
    s_tvin_record = fetch_tvin_record();
    if(NULL == s_tvin_record)
    {
        return NULL;
    }
    
    pRecord = s_tvin_record;
    memset(&item, 0, sizeof(menuitem_t));
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
        
        //load resource group
        pRecord->group = ( resgroup_resource_t * )get_scene_child( scene, STR_OPTION_RES); 
        
        //load visible config
        _load_visible_config();
        
        //create menu
        pMenu = menu_create();
        if ( pMenu == NULL )
        {
            print_err("cann't create menu!");
            return false;
        }
        else
        {    
            for(num=0; num<(int)TVIN_OPTION_MAX_ITEMS; num++)
            {
                if(s_options_visible[num] == true)
                {
                    str_res = (string_resource_t *)get_resgroup_child(pRecord->group, \
                        (unsigned int)s_option_items[num].text_id);
                    if(str_res != NULL)
                    {
                        item.text_id = (int)(str_res->str_id);
                    }
                    item.has_child = s_option_items[num].has_child;
                    item.callback = s_option_items[num].callback;
                    menu_append_item(pMenu, &item);
                }
            }
        }
        menu_set_cur_index( pMenu, 0 );
    }    
    return pMenu;
}


static void _option_fusion_cb( unsigned char *framebuffer, int width, int height )
{
    usleep( 100*1 );
}





/* �������˵� */
bool tvin_option_enter( menuview_t *menuview, fusion_enter_e fenter )
{
    fusion_effect_t eff;
    fusion_get_config(FUSION_TYPE_OPTION_YUV, fenter, &eff);
    fusion_set_effect(&eff);

    menuview_load_style( menuview, MENUVIEW_OPTION );
    //option��Ч��һ֡��Ҫ���
    canvas_erase_all( menuview->canvas_id );
    /* ���ô˽���֮ǰ������, ��Ϊ��Ч��һ֡ */
    if ( FUSION_FORWARD == fenter )
    {
        tvin_option_fusion_set_first_frame( menuview );
    }
    
    //blending�Ĵ�����paint֮ǰ, ����blending�л���paint������, ������Ч��һ֡�Ѿ������һ֡
    //tvin_option_blending_create_show();

    if(NULL == s_ptvin_menu)
    {
        s_ptvin_menu = _tvin_option_init();
    }
    menuview_set_menu(menuview, s_ptvin_menu);
    menuview_paint( menuview );    
    /* ���ô˽��������, ��Ϊ��Ч���һ֡ */
    tvin_option_fusion_set_last_frame(menuview);
    
    /* ��ʾ��Ч */
    if ( FUSION_FORWARD == fenter )
    {
        /* ���ô˽��������, ��Ϊ��Ч���һ֡ */
        tvin_option_fusion_set_last_frame(menuview);
        /*ע��:����fusion_display��ʾǰ�п������������ȼ����߳�����ˢ����
        ���������˵����ݣ����һ֡��������ˢ�µ���Ļ�����������
        ��������գ��ٴ���bleeding����
        */
        canvas_erase_all( menuview->canvas_id );
        tvin_option_blending_create_show();
        fusion_display(  _option_fusion_cb );
    }
    else
    {
        tvin_option_blending_create_show();
    }
    return true;
}

bool tvin_option_exit(menuview_t *menuview)
{
    tvin_option_blending_delete();
    tvin_widget_option_delete(menuview);
    tvin_option_deinit();
    return true;
}


bool tvin_option_deinit(void)
{
    if(NULL != s_ptvin_menu)
    {
        menu_delete(s_ptvin_menu); 
        s_ptvin_menu = NULL;
    }
    return true;
}


