/*******************************************************************************
 *                              NOYA1100
 *                            Module: video
 *                 Copyright(c) 2003-2008 Actions Semiconductor,
 *                            All Rights Reserved. 
 * History:         
 *      <author>    <time>           <version >             <desc>
 *      shaofeng    2009-04-23          1.0             build this file 
******************************************************************************/
/*!
 * \file     video_commonui.c
 * \brief    
 * \author   shaofeng
 * \par      GENERAL DESCRIPTION:
 *               
 * \par      EXTERNALIZED FUNCTIONS:
 *               
 * \version 1.0
 * \date  2009/04/23
*******************************************************************************/

#include "include_psp.h"
#include "include_case.h"

/* include file of this application, ��Ӧ�õ�ͷ�ļ� */
#include "include_video.h"


/* extern function, �������� */
static void _headbar_cb(headbar_t *headbar, widget_sig_headbar_e signal);
static bool _headbar_blending_create( void );
static bool _headbar_blending_delete( void );

static void  _msgbox_cb(msgbox_t *msgbox, msgbox_sig_e signal);
static bool _msgbox_blending_create_show( void );
static bool _msgbox_blending_delete( void );

static bool _get_dialog_init( video_dialog_e type, dialog_init_t *dialog_init );
static void _dialog_cb_default( dialog_t *dialog, widget_sig_dialog_e signal );
static bool _dialog_blending_create_show( void );
static bool _dialog_blending_delete( void );

static void _vbar_cb(volumebar_t *volumebar, int signal);
static bool _vbar_blending_create_show( void );
static bool _vbar_blending_delete( void );

static void  _adjust_cb(adjust_t *adjust, adjust_signal_e signal);
static bool _adjust_blending_create_show( void );
static bool _adjust_blending_delete( void );




#if 0
#endif
static void _headbar_cb(headbar_t *headbar, widget_sig_headbar_e signal)
{
    print_dbg("signal:0x%x", signal);
}

/******************************************************************************/
/*!                    
* \par  Description:
*       ����commonui��blending����. 
*       ����headbar��playing uiһ����ʾ, �������ɴ���, ������ʾ
*       commonui��canvas����Ϊshow״̬, ��������ͣʱ�޷�ˢ��
* \param[in]    
* \retval       bool: true:�����ɹ�; false:����ʧ��
* ע��: ��playing����, �˺���ֱ�ӷ���, ����������blending
*******************************************************************************/
static bool _headbar_blending_create( void )
{
    bool result = true;
    canvas_info_t canvas_info = {0};
    
    //������playing scene, ����Ҫ����blending
    if ( video_vars.scene_playing.app_scene.win_id <= 0 )
    {
        return false;
    }
    
    //playing�����canvasΪ�����ʾЧ��, ��canvas��Ϊ����
    canvas_hide( video_vars.headbar->canvas_id );
    //���canvas��blending_buf, ���ڴ���blending
    canvas_get_info( video_vars.headbar->canvas_id, &canvas_info );
    
    /* ����dialog blend����, create dialog blending area*/
    video_vars.blending.headbar_is_show     = 0;
    video_vars.blending.headbar.blend_param.color_key   = VIDEO_DEFAULT_BLENDING_COLOR_KEY;
    video_vars.blending.headbar.blend_param.alpha       = VIDEO_DEFAULT_BLENDING_ALPHA;
    video_vars.blending.headbar.blend_param.area.x      = canvas_info.rect.x;
    video_vars.blending.headbar.blend_param.area.y      = canvas_info.rect.y;
    video_vars.blending.headbar.blend_param.area.w      = canvas_info.rect.width;
    video_vars.blending.headbar.blend_param.area.h      = canvas_info.rect.height;
    video_vars.blending.headbar.blend_param.blending_buf = (unsigned char *)canvas_info.pdata;
    video_vars.blending.headbar.handle = -1;
    result = video_control(V_CMD_BLENDING_AREA_CREATE, &video_vars.blending.headbar);
    if ( (false == result) || (video_vars.blending.headbar.handle < 0) )
    {
        print_err("send AREA_CREATE err\n\n");
        return false;
    }
    
    return true;
}

static bool _headbar_blending_delete( void )
{
    //playing�����dialog, ����Ҫdelete blending����
    if ( video_vars.scene_playing.app_scene.win_id <= 0 )
    {
        return false;
    }
    
    video_vars.blending.headbar_is_show = 0;
    if ( -1 != video_vars.blending.headbar.handle )
    {
        video_control(V_CMD_BLENDING_AREA_DESTROY, &video_vars.blending.headbar.handle);
        video_vars.blending.headbar.handle = -1;
    }
    
    return true;
}

/******************************************************************************/
/*!                    
* \par  Description:
*       ����headbar
*       ���ݵ�ǰscene�Զ���ɺ������Ĵ���
*       ���ݵ�ǰ�Ƿ���playing����, �Զ����blending�Ĳ���
* \param[in]    
* \retval       bool: true:�����ɹ�; false:����ʧ��
*******************************************************************************/
bool video_headbar_create( void )
{
    headbar_init_t init;
    scene_t *scene = NULL;
    resgroup_resource_t *resgroup = NULL;
    picture_resource_t *pic_res = NULL;
    string_resource_t *str_res = NULL;
    
    scene = sty_get_app_scene();
    resgroup = ( resgroup_resource_t * )get_scene_child( scene, WIDGET_HEADBAR_RESGROUP );
    pic_res  = ( picture_resource_t * )get_resgroup_child( resgroup, PIC_HEADBAR_VIDEO );
    str_res  = ( string_resource_t * )get_resgroup_child( resgroup, STR_HEADBAR_VIDEO );
    if ( (NULL == pic_res) || (NULL == str_res) )
    {
        print_err("get headbar res fail");
        return false;
    }
    
    memset( &init, 0x00, sizeof(init) );
    //playing�����headbar, ����Ϊ:YUV
    if ( video_vars.scene_playing.app_scene.win_id <= 0 )
    {
        init.type = HEADBAR_TYPE_NORMAL;
    }
    else
    {
        init.type = HEADBAR_TYPE_YUV;
    }
    init.pic_id_head = pic_res->pic_id;
    init.str_id_title = str_res->str_id;
    
    video_vars.headbar = headbar_create( &init, (headbar_cb_t)_headbar_cb );
    if ( NULL == video_vars.headbar )
    {
        print_err("headbar_create fail");
        return false;
    }
    
    _headbar_blending_create();
    
    return true;
}

bool video_headbar_delete( void )
{
    _headbar_blending_delete();
    
    headbar_delete( video_vars.headbar );
    video_vars.headbar = NULL;
    
    return true;
}



#if 0
#endif
static void  _msgbox_cb(msgbox_t *msgbox, msgbox_sig_e signal)
{
    switch(signal)
    {
    case WIDGET_SIG_MSGBOX_SHOW:
        _msgbox_blending_create_show();
        break;
        
    case WIDGET_SIG_MSGBOX_QUIT:
        _msgbox_blending_delete();
        break;
        
    default:
        break;
    }
}

/******************************************************************************/
/*!                    
* \par  Description:
*       ����commonui��blending����. 
*       msgbox����֮����Ҫ������ʾ, �յ�show��Ϣ����Ҫ, ������Ҫ:create and show
*       commonui��canvas����Ϊshow״̬, ��������ͣʱ�޷�ˢ��
* \param[in]    
* \retval       bool: true:�����ɹ�; false:����ʧ��
* ע��: ��playing����, �˺���ֱ�ӷ���, ����������blending
*******************************************************************************/
static bool _msgbox_blending_create_show( void )
{
    bool result = true;
    canvas_info_t canvas_info = {0};
    
    //������playing scene, ����Ҫ����blending
    if ( video_vars.scene_playing.app_scene.win_id <= 0 )
    {
        return false;
    }
    
    //playing�����canvasΪ�����ʾЧ��, ��canvas��Ϊ����
    canvas_hide( video_vars.msgbox->canvas_id );
    //���canvas��blending_buf, ���ڴ���blending
    canvas_get_info( video_vars.msgbox->canvas_id, &canvas_info );
    
    /* dialog blend�����ʼ��, dialog blending area init*/
    video_vars.blending.msgbox_is_show     = 0;
    video_vars.blending.msgbox.blend_param.color_key   = VIDEO_DEFAULT_BLENDING_COLOR_KEY;
    video_vars.blending.msgbox.blend_param.alpha       = VIDEO_DEFAULT_BLENDING_ALPHA;
    video_vars.blending.msgbox.blend_param.area.x      = canvas_info.rect.x;
    video_vars.blending.msgbox.blend_param.area.y      = canvas_info.rect.y;
    video_vars.blending.msgbox.blend_param.area.w      = canvas_info.rect.width;
    video_vars.blending.msgbox.blend_param.area.h      = canvas_info.rect.height;
    video_vars.blending.msgbox.blend_param.blending_buf = (unsigned char *)canvas_info.pdata;
    video_vars.blending.msgbox.handle = -1;
    result = video_control(V_CMD_BLENDING_AREA_CREATE, &video_vars.blending.msgbox);
    if ( (false == result) || (video_vars.blending.msgbox.handle < 0) )
    {
        print_warning("send AREA_CREATE err\n\n");
        return false;
    }
    
    memset( &video_vars.blending.blending_handle, BLENDING_HANLE_NULL, sizeof(video_vars.blending.blending_handle) );
    video_vars.blending.blending_handle[0] = video_vars.blending.msgbox.handle;
    result = video_control(V_CMD_BLENDING_AREA_SHOW, video_vars.blending.blending_handle);
    if ( false == result )
    {
        print_warning("send AREA_SHOW err\n\n");
        return false;
    }
    video_vars.blending.msgbox_is_show = 1;
    
    return true;
}

static bool _msgbox_blending_delete( void )
{
    //playing����, ����Ҫdelete blending����
    if ( video_vars.scene_playing.app_scene.win_id <= 0 )
    {
        return false;
    }
    
    video_vars.blending.msgbox_is_show = 0;
    if ( -1 != video_vars.blending.msgbox.handle )
    {
        video_control(V_CMD_BLENDING_AREA_DESTROY, &video_vars.blending.msgbox.handle);
        video_vars.blending.msgbox.handle = -1;
    }
    
    return true;
}


/******************************************************************************/
/*!                    
* \par  Description:
*       ���� digit msgbox, ��ɱ��ٵ���
*       ���ݵ�ǰscene�Զ���ɺ������Ĵ���
*       ���ݵ�ǰ�Ƿ���playing����, �Զ����blending�Ĳ���
* \retval       bool: true:�����ɹ�; false:����ʧ��
*******************************************************************************/
bool  video_msgbox_create( void )
{
    video_vars.msgbox = msgbox_create( _msgbox_cb );
    if ( NULL == video_vars.msgbox )
    {
        return false;
    }
    
    return true;
}

bool video_msgbox_delete( void )
{
    msgbox_delete( video_vars.msgbox );
    video_vars.msgbox = NULL;
    
    return true;
}



#if 0
#endif
/******************************************************************************/
/*!                    
* \par  Description:
*       ����video��dialog����, ��ȡdialog��ʼ������, ���ڴ���dialog
* \param[in]    video_dialog_e: video��dialog����
* \param[out]   dialog_init_t *: ��ȡ��dialog��ʼ���ṹ��
* \retval       bool: true:�����ɹ�; false:����ʧ��
*******************************************************************************/
static bool _get_dialog_init( video_dialog_e type, dialog_init_t *dialog_init )
{
    dialog_init_t init;
    scene_t *scene = NULL;
    resgroup_resource_t *resgroup = NULL;
    string_resource_t *str_res = NULL;
    
    if ( NULL == dialog_init )
    {
        print_warning("param err");
        return false;
    }
    
    memset( &init, 0x00, sizeof(init) );

    //get scene
    scene = sty_get_app_scene();
    resgroup = ( resgroup_resource_t * )get_scene_child( scene, WIDGET_DIALOG_RESGROUP );
    
    switch( type )
    {
    case VIDEO_DIALOG_DELETE_TAG:
        str_res  = ( string_resource_t * )get_resgroup_child( resgroup, STR_DELETE_TAG );
        init.head = DIALOG_HEAD_QUERY;
        init.def_sel = WIDGET_SIG_DIALOG_CANCAL;
        break;
        
    case VIDEO_DIALOG_NO_TAG:
        str_res  = ( string_resource_t * )get_resgroup_child( resgroup, STR_NO_TAG );
        init.head = DIALOG_HEAD_WARN;
        break;
        
    case VIDEO_DIALOG_TAG_FULL:
        str_res  = ( string_resource_t * )get_resgroup_child( resgroup, STR_TAG_FULL );
        init.head = DIALOG_HEAD_INFO;
        break;

    case VIDEO_DIALOG_ADD_FAIL:
        str_res  = ( string_resource_t * )get_resgroup_child( resgroup, STR_ADD_FAIL );
        init.head = DIALOG_HEAD_WARN;
        break;
        
    case VIDEO_DIALOG_DELETE_FILE:
        str_res  = ( string_resource_t * )get_resgroup_child( resgroup, STR_DELETE_FILE );
        init.head = DIALOG_HEAD_QUERY;
        init.def_sel = WIDGET_SIG_DIALOG_CANCAL;
        break;
        
    case VIDEO_DIALOG_OPEN_FAIL:
        str_res  = ( string_resource_t * )get_resgroup_child( resgroup, STR_OPEN_FAIL );
        init.head = DIALOG_HEAD_WARN;
        break;
        
    case VIDEO_DIALOG_NOT_SUPPORT:
        str_res  = ( string_resource_t * )get_resgroup_child( resgroup, STR_NOT_SUPPORT );
        init.head = DIALOG_HEAD_WARN;
        break;
        
    case VIDEO_DIALOG_LOW_BAT:
        str_res  = ( string_resource_t * )get_resgroup_child( resgroup, STR_LOW_BAT );
        init.head = DIALOG_HEAD_INFO;
        break;
        
    case VIDEO_DIALOG_NOT_FOUND:
        str_res  = ( string_resource_t * )get_resgroup_child( resgroup, STR_NOT_FOUND );
        init.head = DIALOG_HEAD_WARN;
        break;

    case VIDEO_DIALOG_NODISK:
        str_res  = ( string_resource_t * )get_resgroup_child( resgroup, STR_NODISK );
        init.head = DIALOG_HEAD_WARN;
        break;
    case VIDEO_DIALOG_DECODE_ERR:
        str_res  = ( string_resource_t * )get_resgroup_child( resgroup, STR_DECODE_ERR );
        init.head = DIALOG_HEAD_WARN;
        break;
    case VIDEO_DIALOG_PL_NOSP:
        str_res  = ( string_resource_t * )get_resgroup_child( resgroup, STR_PL_NOSP);
        init.head = DIALOG_HEAD_WARN;
        break;
        

    default :
        break;
    }
    //dialog��str. û��strʱ, ����Ҫstr_id
    if ( NULL != str_res )
    {
        init.str_id = str_res->str_id;
    }
    
    *dialog_init = init;
    
    return true;
}

/******************************************************************************/
/*!                    
* \par  Description:
*      dialogĬ�ϵĻص�����
*      �����ɾ��dialog����. ��Ҫ�κ�������������createʱע���Լ��Ļص�����
* \param[in]    
* \retval       
*******************************************************************************/
static void _dialog_cb_default( dialog_t *dialog, widget_sig_dialog_e signal )
{
    print_dbg("signal:0x%x", signal);
    switch ( signal ) 
    {
    case WIDGET_SIG_DIALOG_OK:
        video_dialog_delete();
        break;
        
    case WIDGET_SIG_DIALOG_CANCAL:
        video_dialog_delete();
        break;
    default:
        break;
    }
}

/******************************************************************************/
/*!                    
* \par  Description:
*       ����commonui��blending����. 
*       dialog����֮����Ҫ������ʾ, ������Ҫ:create and show
*       commonui��canvas����Ϊshow״̬, ��������ͣʱ�޷�ˢ��
* \param[in]    
* \retval       bool: true:�����ɹ�; false:����ʧ��
* ע��: ��playing����, �˺���ֱ�ӷ���, ����������blending
*******************************************************************************/
static bool _dialog_blending_create_show( void )
{
    bool result = true;
    canvas_info_t canvas_info = {0};
    
    //������playing scene, ����Ҫ����blending
    if ( video_vars.scene_playing.app_scene.win_id <= 0 )
    {
        return false;
    }
    
    //playing�����canvasΪ�����ʾЧ��, ��canvas��Ϊ����
    canvas_hide( video_vars.dialog->canvas_id );
    //���canvas��blending_buf, ���ڴ���blending
    canvas_get_info( video_vars.dialog->canvas_id, &canvas_info );
    
    /* ����dialog blend����, create dialog blending area*/
    video_vars.blending.dialog_is_show     = 0;
    video_vars.blending.dialog.blend_param.color_key   = VIDEO_DEFAULT_BLENDING_COLOR_KEY;
    video_vars.blending.dialog.blend_param.alpha       = VIDEO_DEFAULT_BLENDING_ALPHA;
    video_vars.blending.dialog.blend_param.area.x      = canvas_info.rect.x;
    video_vars.blending.dialog.blend_param.area.y      = canvas_info.rect.y;
    video_vars.blending.dialog.blend_param.area.w      = canvas_info.rect.width;
    video_vars.blending.dialog.blend_param.area.h      = canvas_info.rect.height;
    video_vars.blending.dialog.blend_param.blending_buf = (unsigned char *)canvas_info.pdata;
    video_vars.blending.dialog.handle = -1;
    result = video_control(V_CMD_BLENDING_AREA_CREATE, &video_vars.blending.dialog);
    if ( (false == result) || (video_vars.blending.dialog.handle < 0) )
    {
        print_warning("send AREA_CREATE err\n\n");
        return false;
    }
    
    /* ��ʾblend����, show blending area*/
    memset( &video_vars.blending.blending_handle, BLENDING_HANLE_NULL, sizeof(video_vars.blending.blending_handle) );
    video_vars.blending.blending_handle[0] = video_vars.blending.dialog.handle;
    result = video_control(V_CMD_BLENDING_AREA_SHOW, video_vars.blending.blending_handle);
    if ( false == result )
    {
        print_warning("send AREA_SHOW err\n\n");
        return false;
    }
    video_vars.blending.dialog_is_show = 1;
    
    return true;
}

static bool _dialog_blending_delete( void )
{
    //playing�����dialog, ����Ҫdelete blending����
    if ( video_vars.scene_playing.app_scene.win_id <= 0 )
    {
        return false;
    }
    
    video_vars.blending.dialog_is_show = 0;
    if ( -1 != video_vars.blending.dialog.handle )
    {
        video_control(V_CMD_BLENDING_AREA_DESTROY, &video_vars.blending.dialog.handle);
        video_vars.blending.dialog.handle = -1;
    }
    
    return true;
}

/******************************************************************************/
/*!                    
* \par  Description:
*       ����dialog
*       ���ݵ�ǰscene�Զ���ɺ������Ĵ���
*       ���ݵ�ǰ�Ƿ���playing����, �Զ����blending�Ĳ���
* \param[in]    video_dialog_e:
* \param[in]    dialog_cb_t: NULL:ʹ��_dialog_cb_default; other:�Զ��Ļص�����
* \retval       bool: true:�����ɹ�; false:����ʧ��
*******************************************************************************/
bool video_dialog_create( video_dialog_e type, dialog_cb_t dialog_cb )
{
    bool result = true;
    dialog_init_t init;

    if ( NULL != video_vars.dialog )
    {
        video_dialog_delete();
    }
   
    memset( &init, 0x00, sizeof(init) );
    result = _get_dialog_init( type, &init );
    if ( false == result )
    {
        return false;
    }
    
    //ʹ��Ĭ�ϵ�cb
    if ( NULL == dialog_cb )
    {
        video_vars.dialog = dialog_create( &init, (dialog_cb_t)_dialog_cb_default );
    }
    else
    {
        video_vars.dialog = dialog_create( &init, (dialog_cb_t)dialog_cb );
    }
    if ( NULL == video_vars.dialog )
    {
        print_err("dialog_create fail\n\n");
        return false;
    }
    
    _dialog_blending_create_show();
    
    dialog_paint( video_vars.dialog );
    
    return true;
}

bool video_dialog_delete( void )
{
    _dialog_blending_delete();
    
    dialog_delete( video_vars.dialog );
    video_vars.dialog = NULL;

//    g_enter_timer = 1;

    
    return true;
}


/******************************************************************************/
/*!                    
* \par  Description:
*      videoģ̬�Ի���. �Ӵ˺���������ʾdialog, ֱ��dialog��ʧ�ŴӴ˺�������
* \param[in]    video_dialog_e: video ��dialog����
* \retval       -1: err; ����Ϊ�û�ѡ����, �����:widget_sig_dialog_e
* ע��:������blending�����playing����, ����ʹ��dialog modal. 
*      ��Ϊ:ģ̬�Ի����޷������blending_buf, Ҳ���޷���ʾ
*      ��: V_CMD_SET_VIDEO_AREA����Ϊȫ��video��, ��dialog�޷���ʾ
*      V_CMD_SET_VIDEO_AREA����Ϊ0��, ��dialog����������ʾ
*******************************************************************************/
int video_dialog_modal( video_dialog_e type )
{
    int result = -1;
    bool tmp = true;
    dialog_init_t init;

    if ( NULL != video_vars.dialog )
    {
        video_dialog_delete();
    }
    
    memset( &init, 0x00, sizeof(init) );
    tmp = _get_dialog_init( type, &init );
    if ( false == tmp )
    {
        return -1;
    }
    
    result = dialog_modal( &init );
    
    return result;
}



#if 0
#endif
static void _vbar_cb(volumebar_t *volumebar, int signal)
{
    print_dbg("signal:0x%x", signal);
    switch ( signal ) 
    {
    case WIDGET_SIG_VOLUMEBAR_CHANGED:
        break;
        
    case WIDGET_SIG_VOLUMEBAR_QUIT:
        video_vbar_delete();
        break;
         
    default:
        break;
    }
}

/******************************************************************************/
/*!                    
* \par  Description:
*       ����commonui��blending����. 
*       volumebar����֮����Ҫ������ʾ, ������Ҫ:create and show
*       commonui��canvas����Ϊshow״̬, ��������ͣʱ�޷�ˢ��
* \param[in]    
* \retval       bool: true:�����ɹ�; false:����ʧ��
* ע��: ��playing����, �˺���ֱ�ӷ���, ����������blending
*******************************************************************************/
static bool _vbar_blending_create_show( void )
{
    bool result = true;
    canvas_info_t canvas_info = {0};
    
    //������playing scene, ����Ҫ����blending
    if ( video_vars.scene_playing.app_scene.win_id <= 0 )
    {
        return false;
    }
    
    //playing�����canvasΪ�����ʾЧ��, ��canvas��Ϊ����
    canvas_hide( video_vars.vbar->canvas_id );
    //���canvas��blending_buf, ���ڴ���blending
    canvas_get_info( video_vars.vbar->canvas_id, &canvas_info );
    
    /* dialog blend�����ʼ��, dialog blending area init*/
    video_vars.blending.volumebar_is_show     = 0;
    video_vars.blending.volumebar.blend_param.color_key   = VIDEO_DEFAULT_BLENDING_COLOR_KEY;
    video_vars.blending.volumebar.blend_param.alpha       = VIDEO_DEFAULT_BLENDING_ALPHA;
    video_vars.blending.volumebar.blend_param.area.x      = canvas_info.rect.x;
    video_vars.blending.volumebar.blend_param.area.y      = canvas_info.rect.y;
    video_vars.blending.volumebar.blend_param.area.w      = canvas_info.rect.width;
    video_vars.blending.volumebar.blend_param.area.h      = canvas_info.rect.height;
    video_vars.blending.volumebar.blend_param.blending_buf = (unsigned char *)canvas_info.pdata;
    video_vars.blending.volumebar.handle = -1;
    result = video_control(V_CMD_BLENDING_AREA_CREATE, &video_vars.blending.volumebar);
    if ( (false == result) || (video_vars.blending.volumebar.handle < 0) )
    {
        print_warning("send AREA_CREATE err\n\n");
        return false;
    }
    
    memset( &video_vars.blending.blending_handle, BLENDING_HANLE_NULL, sizeof(video_vars.blending.blending_handle) );
    video_vars.blending.blending_handle[0] = video_vars.blending.volumebar.handle;
    result = video_control(V_CMD_BLENDING_AREA_SHOW, video_vars.blending.blending_handle);
    if ( false == result )
    {
        print_warning("send AREA_SHOW err\n\n");
        return false;
    }
    video_vars.blending.volumebar_is_show = 1;
    
    return true;
}

static bool _vbar_blending_delete( void )
{
    //playing����, ����Ҫdelete blending����
    if ( video_vars.scene_playing.app_scene.win_id <= 0 )
    {
        return false;
    }
    
    video_vars.blending.volumebar_is_show = 0;
    if ( -1 != video_vars.blending.volumebar.handle )
    {
        video_control(V_CMD_BLENDING_AREA_DESTROY, &video_vars.blending.volumebar.handle);
        video_vars.blending.volumebar.handle = -1;
    }
    
    return true;
}

/******************************************************************************/
/*!                    
* \par  Description:
*       ����volumebar
*       ���ݵ�ǰscene�Զ���ɺ������Ĵ���
*       ���ݵ�ǰ�Ƿ���playing����, �Զ����blending�Ĳ���
* \param[in]    0:
* \param[in]    volumebar_cb_t: 
* \retval       bool: true:�����ɹ�; false:����ʧ��
*******************************************************************************/
bool video_vbar_create( void )
{
    video_vars.vbar = volumebar_create( 0, (volumebar_cb_t)_vbar_cb );
    if ( NULL == video_vars.vbar )
    {
        print_err("volumebar_create fail");
        return false;
    }
    
    _vbar_blending_create_show();

    volumebar_paint( video_vars.vbar );
    
    return true;
}

bool video_vbar_delete( void )
{
    bool result = true;
    
    _vbar_blending_delete();
    
    volumebar_delete( video_vars.vbar );
    video_vars.vbar = NULL;
    
    return result;
}



#if 0
#endif
static void  _adjust_cb(adjust_t *adjust, adjust_signal_e signal)
{
    bool result = true;
    int val = 0;
    
    switch(signal)
    {
    case WIDGET_SIG_ADJUST_OK:
        result = adjust_get_current_val( &val );
        if ( true == result )
        {
            video_vars.play_param.ffb_step = val;
        }
        
        video_adjust_delete();
        break;
        
    case WIDGET_SIG_ADJUST_CANCEL:
        video_adjust_delete();
        break;
        
    default:
        break;
    }
}

/******************************************************************************/
/*!                    
* \par  Description:
*       ����commonui��blending����. 
*       adjust����֮����Ҫ������ʾ, ������Ҫ:create and show
*       commonui��canvas����Ϊshow״̬, ��������ͣʱ�޷�ˢ��
* \param[in]    
* \retval       bool: true:�����ɹ�; false:����ʧ��
* ע��: ��playing����, �˺���ֱ�ӷ���, ����������blending
*******************************************************************************/
static bool _adjust_blending_create_show( void )
{
    bool result = true;
    canvas_info_t canvas_info = {0};
    
    //������playing scene, ����Ҫ����blending
    if ( video_vars.scene_playing.app_scene.win_id <= 0 )
    {
        return false;
    }
    
    //playing�����canvasΪ�����ʾЧ��, ��canvas��Ϊ����
    canvas_hide( video_vars.adjust->canvas_id );
    //���canvas��blending_buf, ���ڴ���blending
    canvas_get_info( video_vars.adjust->canvas_id, &canvas_info );
    
    /* dialog blend�����ʼ��, dialog blending area init*/
    video_vars.blending.adjust_is_show     = 0;
    video_vars.blending.adjust.blend_param.color_key   = VIDEO_DEFAULT_BLENDING_COLOR_KEY;
    video_vars.blending.adjust.blend_param.alpha       = VIDEO_DEFAULT_BLENDING_ALPHA;
    video_vars.blending.adjust.blend_param.area.x      = canvas_info.rect.x;
    video_vars.blending.adjust.blend_param.area.y      = canvas_info.rect.y;
    video_vars.blending.adjust.blend_param.area.w      = canvas_info.rect.width;
    video_vars.blending.adjust.blend_param.area.h      = canvas_info.rect.height;
    video_vars.blending.adjust.blend_param.blending_buf = (unsigned char *)canvas_info.pdata;
    video_vars.blending.adjust.handle = -1;
    result = video_control(V_CMD_BLENDING_AREA_CREATE, &video_vars.blending.adjust);
    if ( (false == result) || (video_vars.blending.adjust.handle < 0) )
    {
        print_warning("send AREA_CREATE err\n\n");
        return false;
    }
    
    memset( &video_vars.blending.blending_handle, BLENDING_HANLE_NULL, sizeof(video_vars.blending.blending_handle) );
    video_vars.blending.blending_handle[0] = video_vars.blending.adjust.handle;
    result = video_control(V_CMD_BLENDING_AREA_SHOW, video_vars.blending.blending_handle);
    if ( false == result )
    {
        print_warning("send AREA_SHOW err\n\n");
        return false;
    }
    video_vars.blending.adjust_is_show = 1;
    
    return true;
}

static bool _adjust_blending_delete( void )
{
    //playing����, ����Ҫdelete blending����
    if ( video_vars.scene_playing.app_scene.win_id <= 0 )
    {
        return false;
    }
    
    video_vars.blending.adjust_is_show = 0;
    if ( -1 != video_vars.blending.adjust.handle )
    {
        video_control(V_CMD_BLENDING_AREA_DESTROY, &video_vars.blending.adjust.handle);
        video_vars.blending.adjust.handle = -1;
    }
    
    return true;
}


/******************************************************************************/
/*!                    
* \par  Description:
*       ���� digit adjust, ��ɱ��ٵ���
*       ���ݵ�ǰscene�Զ���ɺ������Ĵ���
*       ���ݵ�ǰ�Ƿ���playing����, �Զ����blending�Ĳ���
* \retval       bool: true:�����ɹ�; false:����ʧ��
*******************************************************************************/
bool  video_adjust_scanspeed_create( void )
{
    adjust_textdigit_init_t  adjust_digit;
    scene_t *scene;
    resgroup_resource_t *resgroup = NULL;
    string_resource_t *str_res = NULL;
    
    //get scene
    scene = sty_get_app_scene();
    resgroup = ( resgroup_resource_t * )get_scene_child( scene, WIDGET_ADJUST_RESGROUP );
    str_res  = ( string_resource_t * )get_resgroup_child( resgroup, STR_SCANSPEED );
    if ( NULL == str_res )
    {
        return false;
    }
    
    memset( &adjust_digit, 0x00, sizeof(adjust_textdigit_init_t) );
    adjust_digit.str_title_id = str_res->str_id;
    adjust_digit.max_val = 4;
    adjust_digit.min_val = 0;
    adjust_digit.cur_val = video_vars.play_param.ffb_step;
    adjust_digit.step = 1;

    video_vars.adjust = adjust_create( &adjust_digit, ADJ_STYLE_TEXTDIGIT, _adjust_cb);
    if ( NULL == video_vars.adjust )
    {
        return false;
    }
    
    _adjust_blending_create_show();
    
    adjust_paint( video_vars.adjust );
    
    return true;
}

bool video_adjust_delete( void )
{
    _adjust_blending_delete();
    
    adjust_delete( video_vars.adjust );
    video_vars.adjust = NULL;
    
    return true;
}
#if 0
#endif


static void _video_resume_cb(adjust_t *adjust, adjust_signal_e signal)
{
    adj_switch_mode_e video_resume_switch_val;
    bool result;
    print_dbg("video_vars.flag_resume before set:%d",video_vars.flag_resume);
    switch(signal)
    {
        case WIDGET_SIG_ADJUST_OK:
        {
            result = adjust_get_switch_status(&video_resume_switch_val);
            if(true == result)
            {
                if(ADJUST_ON ==  video_resume_switch_val)
                {
                    video_vars.flag_resume = true;
                }
                else 
                {
                    video_vars.flag_resume = false;
                }
            }

            video_adjust_delete();
            /*����ϵ㲥�Źرգ������������б�*/
            if( false == video_vars.flag_resume )
            {
                plist_clear( video_vars.recent_list );
            }
            break;
        }

        case WIDGET_SIG_ADJUST_CANCEL:
        {
            video_adjust_delete();
            break;
        }

        default:
        {
            break;
        }
    }
    print_dbg("video_vars.flag_resume after set:%d",video_vars.flag_resume);
}


bool video_adjust_resume_create(void)
{
    adjust_switch_init_t  video_adjust_resume;
    scene_t *scene;
    resgroup_resource_t *resgroup = NULL;
    string_resource_t *str_res = NULL;
    
    //get scene
    scene = sty_get_app_scene();
    resgroup = ( resgroup_resource_t * )get_scene_child( scene, WIDGET_ADJUST_RESGROUP );
    str_res  = ( string_resource_t * )get_resgroup_child( resgroup, STR_RESUME );
    if ( NULL == str_res )
    {
        return false;
    }
    
    memset( &video_adjust_resume, 0x00, sizeof(adjust_switch_init_t) );

    video_adjust_resume.str_title_id = str_res->str_id;

    if( true == video_vars.flag_resume)
    {
        video_adjust_resume.on_off_status = ADJUST_ON;
    }
    else
    {
        video_adjust_resume.on_off_status = ADJUST_OFF;
    }

    video_vars.adjust = adjust_create( &video_adjust_resume, ADJ_STYLE_SWITCH, _video_resume_cb);
    if ( NULL == video_vars.adjust )
    {
        return false;
    }
    
    _adjust_blending_create_show();
    
    adjust_paint( video_vars.adjust );
    
    return true;
}


#if 0
#endif
static void _loadingbox_cb(loadingbox_t *loadingbox, loadingbox_sig_e signal)
{
}

/******************************************************************************/
/*!                    
* \par  Description:
*       ���� loadingbox
* \retval       bool: true:�����ɹ�; false:����ʧ��
*******************************************************************************/
bool video_loadingbox_create( void )
{
    video_vars.loadingbox = loadingbox_create( LOADINGBOX_SEARCHING, _loadingbox_cb );
    if ( NULL == video_vars.loadingbox )
    {
        return false;
    }
    
    loadingbox_paint( video_vars.loadingbox );
    
    return true;
}

bool video_loadingbox_delete( void )
{
    loadingbox_delete( video_vars.loadingbox );
    video_vars.loadingbox = NULL;
    
    return true;
}



#if 0
#endif
/******************************************************************************/
/*!                    
* \par  Description:
*       ���� softkeyboard
* \retval       bool: true:�����ɹ�; false:����ʧ��
*******************************************************************************/
bool video_softkeyboard_create( softkeyboard_cb_t callback )
{
    video_vars.softkeyboard = softkeyboard_create( IME_TYPE_PINYIN, callback );
    if ( NULL == video_vars.softkeyboard )
    {
        return false;
    }
    
    softkeyboard_paint( video_vars.softkeyboard );
    
    return true;
}

bool video_softkeyboard_delete( void )
{
    softkeyboard_delete( video_vars.softkeyboard );
    video_vars.softkeyboard = NULL;
    
    return true;
}

