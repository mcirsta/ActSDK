/*******************************************************************************
 *                              NOYA1100
 *                            Module: video
 *                 Copyright(c) 2003-2008 Actions Semiconductor,
 *                            All Rights Reserved. 
 * History:         
 *      <author>    <time>           <version >             <desc>
 *      shaofeng    2009-05-15          1.0             build this file 
*******************************************************************************/
/*!
 * \file     video_playing_option.c
 * \brief    
 * \author   shaofeng
 * \par      GENERAL DESCRIPTION:
 *               
 * \par      EXTERNALIZED FUNCTIONS:
 *               
 * \version 1.0
 * \date  2009/05/15
*******************************************************************************/

#include "include_psp.h"
#include "include_case.h"

/* include file of this application, ��Ӧ�õ�ͷ�ļ� */
#include "include_video.h"

extern int g_enter_timer;

/* extern function, �������� */
static void _option_fusion_cb( unsigned char *framebuffer, int width, int height );

static menu_t * _option_init( void );
static bool _option_deinit( void );

static bool _option_blending_create_show( void );
static bool _option_blending_delete( void );

static int _proc_item_playmode( menuview_t *menuview, menuview_sig_e signal );
static int _proc_item_scanspeed( menuview_t *menuview, menuview_sig_e signal );
static int _proc_item_addtag( menuview_t *menuview, menuview_sig_e signal );
static int _proc_item_displaymode( menuview_t *menuview, menuview_sig_e signal );
static int _proc_item_resume( menuview_t *menuview, menuview_sig_e signal );
static int _proc_item_delete( menuview_t *menuview, menuview_sig_e signal );
static int _proc_item_info( menuview_t *menuview, menuview_sig_e signal );

/* define variable, �������� */
static menu_t *s_option = NULL;
const static video_menu_info_t s_option_info[] = 
{
    { STR_PLAYMODE,      0, 0 },
    { STR_SCANSPEED,     0, 0 },
    { STR_ADDTAG,        0, 0 },
    { STR_DISPLAY_MODE,  0, 0 },    
    { STR_RESUME,        0, 0 },
    { STR_DELETE,        0, 0 },
    { STR_INFO,          0, 0 },
};
/* menu items */
static menuitem_t s_option_items[] =
{   
    { 0, 0, 0,  true,   (item_cb_t)_proc_item_playmode,     NULL, NULL, NULL, NULL },
    { 0, 0, 0,  false,  (item_cb_t)_proc_item_scanspeed,    NULL, NULL, NULL, NULL },
    { 0, 0, 0,  false,  (item_cb_t)_proc_item_addtag,       NULL, NULL, NULL, NULL },
    { 0, 0, 0,  true,   (item_cb_t)_proc_item_displaymode,  NULL, NULL, NULL, NULL },    
    { 0, 0, 0,  false,  (item_cb_t)_proc_item_resume,       NULL, NULL, NULL, NULL },
    { 0, 0, 0,  false,  (item_cb_t)_proc_item_delete,       NULL, NULL, NULL, NULL },
    { 0, 0, 0,  true,   (item_cb_t)_proc_item_info,         NULL, NULL, NULL, NULL },
};



#if 0
#endif
/******************************************************************************/
/*!                    
* \par  Description:
*       ����option
* \param[in]    menuview_t *: �ؼ�ָ��
* \param[in]    fusion_enter_e: ������Ч
* \retval       true:�ɹ�; false:ʧ��
*   ע��: 
* fusion_set_first_frame(); fusion_set_last_frame(); fusion_display()����һ��ʹ��, �����ڴ�й©
*******************************************************************************/
bool video_playing_option_enter( menuview_t *menuview, fusion_enter_e fenter )
{
    fusion_effect_t eff;
    menu_t *cur_menu;

    menuview_load_style( menuview, MENUVIEW_OPTION );
    //option��Ч��һ֡��Ҫ���
    canvas_erase_all( menuview->canvas_id );
    /* ���ô˽���֮ǰ������, ��Ϊ��Ч��һ֡ */
    if ( FUSION_FORWARD == fenter )
    {
        fusion_get_config(FUSION_TYPE_OPTION_YUV, fenter, &eff);
        fusion_set_effect(&eff);
        
        option_fusion_set_first_frame( menuview );
    }
    
    //blending�Ĵ�����paint֮ǰ, ����blending�л���paint������, ������Ч��һ֡�Ѿ������һ֡
    //_option_blending_create_show();
    
    cur_menu = _option_init();
    menuview_set_menu( menuview, cur_menu );
    
    //����Ϊoption ���
    menuview_paint( menuview );
/*    
    // ��ʾ��Ч 
    if ( FUSION_FORWARD == fenter )
    {
        // ���ô˽��������, ��Ϊ��Ч���һ֡ 
        option_fusion_set_last_frame(menuview);
        fusion_display( _option_fusion_cb );
    }
*/

    /* ��ʾ��Ч */
    if ( FUSION_FORWARD == fenter )
    {
        /* ���ô˽��������, ��Ϊ��Ч���һ֡ */
        option_fusion_set_last_frame(menuview);
        canvas_erase_all( menuview->canvas_id );
        _option_blending_create_show();
        fusion_display( _option_fusion_cb );
    }
    else
    {
        _option_blending_create_show();
    }

    
    return true;
}


/* �˳��˵� */
bool video_playing_option_exit( void )
{
    _option_blending_delete();
    
    //delete main menu
    _option_deinit();
    
    return true;
}

static void _option_fusion_cb( unsigned char *framebuffer, int width, int height )
{
    usleep( 0 );
}


static menu_t * _option_init( void )
{
    menuitem_t menuitem;
    unsigned int i;
    
    //initial menu  
    if( s_option == NULL )
    {
        //create menu
        s_option = menu_create();
        video_load_menuitem( OPTION_RESGROUP, s_option_info, \
                        sizeof(s_option_items)/sizeof(menuitem_t), s_option_items );
        
        //init items
        memset( &menuitem, 0, sizeof(menuitem_t) );
        for ( i=0; i<(sizeof(s_option_items)/sizeof(menuitem_t)); i++ )
        {
            menuitem.text_id   = s_option_items[i].text_id;
            menuitem.has_child = s_option_items[i].has_child;
            menuitem.callback  = s_option_items[i].callback;
            
            menu_append_item( s_option, &menuitem );
        }

        //init current index
        menu_set_cur_index( s_option, 0 );
    }

    return s_option;
}


static bool _option_deinit( void )
{    
    //delete menu   
    if( NULL == s_option )
    {
        return false;
    }
    
    menu_delete( s_option );
    s_option = NULL;

    return true;
}


/******************************************************************************/
/*!                    
* \par  Description:
*       ����commonui��blending����. 
*       option����֮����Ҫ������ʾ, ������Ҫ:create and show
*       commonui��canvas����Ϊshow״̬, ��������ͣʱ�޷�ˢ��
* \param[in]    
* \retval       
* ע��: ��playing����, �˺���ֱ�ӷ���, ����������blending
*******************************************************************************/
static bool _option_blending_create_show( void )
{
    bool result;
    canvas_info_t canvas_info = {0};
    
    //������playing scene, ����Ҫ����blending
    if ( video_vars.scene_playing.app_scene.win_id <= 0 )
    {
        return false;
    }
    if ( 1 == video_vars.blending.option_is_show )
    {
        return false;
    }
    
    //playing�����canvasΪ�����ʾЧ��, ��canvas��Ϊ����
    canvas_hide( video_vars.menuview->canvas_id );
    //���canvas��blending_buf, ���ڴ���blending
    canvas_get_info( video_vars.menuview->canvas_id, &canvas_info );
    
    /* ����dialog blend����, create dialog blending area*/
    video_vars.blending.option_is_show     = 0;
    video_vars.blending.option.blend_param.color_key   = VIDEO_DEFAULT_BLENDING_COLOR_KEY;
    video_vars.blending.option.blend_param.alpha       = VIDEO_DEFAULT_BLENDING_ALPHA;
    video_vars.blending.option.blend_param.area.x      = canvas_info.rect.x;
    video_vars.blending.option.blend_param.area.y      = canvas_info.rect.y;
    video_vars.blending.option.blend_param.area.w      = canvas_info.rect.width;
    video_vars.blending.option.blend_param.area.h      = canvas_info.rect.height;
    video_vars.blending.option.blend_param.blending_buf = (unsigned char *)canvas_info.pdata;
    video_vars.blending.option.handle = -1;
    result = video_control(V_CMD_BLENDING_AREA_CREATE, &video_vars.blending.option);
    if ( (false == result) || (video_vars.blending.option.handle < 0) )
    {
        print_warning("send AREA_CREATE err\n\n");
        return false;
    }
    
    memset( &video_vars.blending.blending_handle, BLENDING_HANLE_NULL, sizeof(video_vars.blending.blending_handle) );
    video_vars.blending.blending_handle[0] = video_vars.blending.option.handle;
    result = video_control(V_CMD_BLENDING_AREA_SHOW, video_vars.blending.blending_handle);
    if ( false == result )
    {
        print_warning("send AREA_SHOW err\n\n");
        return false;
    }
    video_vars.blending.option_is_show = 1;
    
    return true;
}

static bool _option_blending_delete( void )
{
    if ( video_vars.scene_playing.app_scene.win_id <= 0 )
    {
        return false;
    }
    
    video_vars.blending.option_is_show = 0;
    if ( -1 != video_vars.blending.option.handle )
    {
        video_control(V_CMD_BLENDING_AREA_DESTROY, &video_vars.blending.option.handle);
        video_vars.blending.option.handle = -1;
    }
    
    return true;
}


static int _proc_item_playmode( menuview_t *menuview, menuview_sig_e signal )
{   
    switch ( signal ) 
    {
    case WIDGET_SIG_MENUVIEW_OK:
        video_playing_option_playmode_enter( menuview );
        break;

    case WIDGET_SIG_MENUVIEW_QUIT:
        video_playing_option_exit();
        
        video_playing_widget_option_delete( menuview );
        video_vars.menuview = NULL;
        break;

    default:
        break;
    }   
    return 1;
}


static int _proc_item_scanspeed( menuview_t *menuview, menuview_sig_e signal )
{   
    switch ( signal ) 
    {
    case WIDGET_SIG_MENUVIEW_OK:
        video_adjust_scanspeed_create();
        break;

    case WIDGET_SIG_MENUVIEW_QUIT:
        video_playing_option_exit();
        
        video_playing_widget_option_delete( menuview );
        video_vars.menuview = NULL;
        break;

    default:
        break;
    }   
    return 1;
}


static int _proc_item_addtag( menuview_t *menuview, menuview_sig_e signal )
{
    bool result = true;
    int tag_num;
    mmm_video_status_t status;
    
    switch ( signal ) 
    {
    case WIDGET_SIG_MENUVIEW_OK:
        video_playing_option_exit();
        video_playing_widget_option_delete( menuview );
        video_vars.menuview = NULL;
        
        tag_num = plist_get_item_num( video_vars.taglist );
        if ( tag_num >= VIDEO_TAGLIST_MAX_NUM )
        {
#if VIDEO_CFG_ADD_NEWTAG_REPLACE_FIRST_WHEN_TAG_FULL == 0
            g_enter_timer = 1;

            //��ʾtag��, �˳�
            video_dialog_create( VIDEO_DIALOG_TAG_FULL, NULL );
            break;
#else
            //ɾ����һ��tag, ��tag��ӵ����
            result = video_taglist_del_item( 0 );
            if ( false == result )
            {
                //ɾ����һ��ʧ��, �������, ����tag����Խ��, ֱ���˳�
                break;
            }
#endif
        }
        
        //����tag
        video_control( V_CMD_GET_PLAYER_STATUS, &status );
        result = video_taglist_add_item( video_vars.plist.np_index, status.cur_time );
        break;

    case WIDGET_SIG_MENUVIEW_QUIT:
        video_playing_option_exit();
        
        video_playing_widget_option_delete( menuview );
        video_vars.menuview = NULL;
        break;

    default:
        break;
    }   
    return 1;
}

static int _proc_item_displaymode( menuview_t *menuview, menuview_sig_e signal )
{   
    switch ( signal ) 
    {
    case WIDGET_SIG_MENUVIEW_OK:
        video_playing_option_displaymode_enter( menuview );
        break;

    case WIDGET_SIG_MENUVIEW_QUIT:
        video_playing_option_exit();
        
        video_playing_widget_option_delete( menuview );
        video_vars.menuview = NULL;
        break;

    default:
        break;
    }   
    return 1;
}


static int _proc_item_resume( menuview_t *menuview, menuview_sig_e signal )
{
    switch ( signal ) 
    {
    case WIDGET_SIG_MENUVIEW_OK:
        video_adjust_resume_create();
        break;

    case WIDGET_SIG_MENUVIEW_QUIT:
        video_playing_option_exit();
        
        video_playing_widget_option_delete( menuview );
        video_vars.menuview = NULL;
        break;

    default:
        break;
    }   
    return 1;
}


static void _dialog_cb_delete_file( dialog_t *dialog, widget_sig_dialog_e signal )
{    
    switch ( signal ) 
    {
    case WIDGET_SIG_DIALOG_OK:
        video_dialog_delete();
        video_scene_playing_delete_file();
        
        video_playing_option_exit();
        video_playing_widget_option_delete( video_vars.menuview );
        video_vars.menuview = NULL;
         
        break;
        
    case WIDGET_SIG_DIALOG_CANCAL:
        video_dialog_delete();
        break;
    default:
        break;
    }
}

static int _proc_item_delete( menuview_t *menuview, menuview_sig_e signal )
{
    switch ( signal ) 
    {
    case WIDGET_SIG_MENUVIEW_OK:
        g_enter_timer = 1;

        video_dialog_create( VIDEO_DIALOG_DELETE_FILE, _dialog_cb_delete_file );
        break;

    case WIDGET_SIG_MENUVIEW_QUIT:
        video_playing_option_exit();
        
        video_playing_widget_option_delete( menuview );
        video_vars.menuview = NULL;
        break;

    default:
        break;
    }   
    return 1;
}


static int _proc_item_info( menuview_t *menuview, menuview_sig_e signal )
{   
    switch ( signal )
    {
    case WIDGET_SIG_MENUVIEW_OK:
        video_playing_option_info_enter( menuview );
        break;

    case WIDGET_SIG_MENUVIEW_QUIT:
        video_playing_option_exit();
        
        video_playing_widget_option_delete( menuview );
        video_vars.menuview = NULL;
        break;

    default:
        break;
    }
    return 1;
}




