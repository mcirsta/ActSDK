/*******************************************************************************
 *                              NOYA1100
 *                            Module: video
 *                 Copyright(c) 2003-2008 Actions Semiconductor,
 *                            All Rights Reserved. 
 * History:         
 *      <author>    <time>           <version >             <desc>
 *      shaofeng    2009-04-02          1.0             build this file 
*******************************************************************************/
/*!
 * \file     dialog.c
 * \brief    实现dialog控件
 * \author   shaofeng
 * \par      GENERAL DESCRIPTION:
 *               
 * \par      EXTERNALIZED FUNCTIONS:
 *               
 * \version 1.0
 * \date  2009/04/02
*******************************************************************************/

#include "include_case.h"


/*本应用的头文件*/
#include "dialog_private.h"
#include "commonui_sty.h"



static int _proc_timer( void *param );
static bool _init( dialog_t *dialog );
static int _proc_key( dialog_t *dialog, key_event_t *key_event );



#if 0
#endif
/******************************************************************************/
/*!                    
* \par  Description:
*      定时器处理函数. timer 时间到delete dialog
* \param[in]    void *: 控件指针
* \retval       always be 1
* \ingroup      common_dialog
*******************************************************************************/
static int _proc_timer( void *param )
{
    dialog_t *dialog = (dialog_t *)param;
    
    dialog->callback( dialog, WIDGET_SIG_DIALOG_CANCAL );
    
    return 1;
}


bool dialog_load_style( dialog_t *dialog )
{
    scene_t* scene;
    sty_rect_t rect;
    
    if ( NULL == dialog )
    {
        return false;
    }
    
    /* 重新load style, 需要整个控件全部重绘, 则将saved_show = -1, 
       来保证第一次show与saved_show不同, 而一定执行draw函数 */
    memset(&dialog->private->saved_show, -1, sizeof(dialog->private->saved_show));
    canvas_erase_all(dialog->canvas_id);
    
    scene = sty_get_common_scene();
    if ( scene == NULL )
    {
        print_dbg("sty_get_common_scene fail\n\n");
        return false;   
    }

    //load resource group
    dialog->private->dialog_resgroup = ( resgroup_resource_t * )get_scene_child( scene, DIALOG_RESGROUP );
    if ( NULL == dialog->private->dialog_resgroup )
    {
        return false;
    }
    
    //load pic and pic region from resource group
    dialog->private->pic_bg         = ( picture_resource_t * )get_resgroup_child( \
            dialog->private->dialog_resgroup, PIC_BG );

    dialog->private->reg_head       = ( picregion_resource_t * )get_resgroup_child( \
            dialog->private->dialog_resgroup, REG_HEAD);
    dialog->private->reg_yes        = ( picregion_resource_t * )get_resgroup_child( \
            dialog->private->dialog_resgroup, REG_YES);
    dialog->private->reg_no         = ( picregion_resource_t * )get_resgroup_child( \
            dialog->private->dialog_resgroup, REG_NO);
    
    //load string from resource group
    dialog->private->str_clew       = ( string_resource_t * )get_resgroup_child( \
            dialog->private->dialog_resgroup, STR_CLEW);
    
    //canvas resize    
    if ( dialog->canvas_id != -1 )
    {
        rect.x = dialog->private->dialog_resgroup->absolute_x;
        rect.y = dialog->private->dialog_resgroup->absolute_y;
        rect.width = dialog->private->dialog_resgroup->width;
        rect.height = dialog->private->dialog_resgroup->height;
        
        //set canvas region
        canvas_set_region( dialog->canvas_id, &rect );
    }

    return true;
}


static bool _init( dialog_t *dialog )
{
    /* query类型需要显示button, 其他类型不需要显示button */
    if (DIALOG_HEAD_QUERY == dialog->head)
    {
     
        dialog->total_button = 2;
    }
    else
    {
        dialog->total_button = 0;
        /* clip must request timer to kill itself */
        dialog->private->timer_id = set_inner_single_shot_timer (3000, _proc_timer, (void *)dialog);
        if (dialog->private->timer_id < 0)
        {
            print_err("set timer fail\n\n");
            return false;
        }
    }
    
    /* init show status */
    dialog->private->show.bg = 1;
    
    dialog->private->show.head = 1;
    dialog->private->show.head_frame = dialog->head;
    
    dialog->private->show.yes = 1;
    dialog->private->show.no = 1;
    dialog->private->show.clew = 1;

    /* init show status of yes and no, which belong to "cur_sel" */
    if (WIDGET_SIG_DIALOG_OK  == dialog->cur_sel)
    {
        dialog->private->show.yes_frame = 1;
        dialog->private->show.no_frame = 0;
    }
    else
    {
        dialog->private->show.yes_frame = 0;
        dialog->private->show.no_frame = 1;
    }
    
    return true;
}


static bool _deinit( dialog_t *dialog )
{
    //kill timer
    if ( -1 != dialog->private->timer_id )
    {
        kill_inner_timer(dialog->private->timer_id);
        dialog->private->timer_id = -1;
    }
    
    return true;
}


bool dialog_paint( dialog_t *dialog )
{
    bool tmp = true;
    print_dbg("");
    
    if ( NULL == dialog )
    {
        return false;
    }
    
    dialog_draw_bg(dialog, dialog->private->show.bg);
    
    dialog_draw_head(dialog, dialog->private->show.head, dialog->private->show.head_frame);
    dialog_draw_yes(dialog, dialog->private->show.yes, dialog->private->show.yes_frame);
    dialog_draw_no(dialog, dialog->private->show.no, dialog->private->show.no_frame);
    
    tmp = dialog_get_clew_param(dialog, dialog->str_id, &dialog->private->clew_param);
    if (true == tmp)
    {
        dialog_draw_clew(dialog, dialog->private->show.clew, &dialog->private->clew_param);
    }
    
    return true;
}


static int _proc_key( dialog_t *dialog, key_event_t *key_event )
{
    print_info("key:0x%x", key_event->val | key_event->type);
    
    /* 没有button的clip, 对key消息的处理 */
    if (0 == dialog->total_button)
    {
        switch ( key_event->val | key_event->type ) 
        {
        case KEY_RETURN | KEY_TYPE_SHORT_UP:
        case KEY_RETURN | KEY_TYPE_LONG_UP:
        case KEY_RETURN | KEY_TYPE_HOLD_UP:
            /* 调用应用的回调 */
            //按return键, 发送信号cancal
            dialog->callback( dialog, WIDGET_SIG_DIALOG_CANCAL );
            break;
            
        default:
            break;
        }
        
        return 0;
    }
    
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
    case KEY_NEXT | KEY_TYPE_DOWN:
        dialog->cur_sel++;
        dialog->cur_sel = dialog->cur_sel % dialog->total_button;
        if (0 == dialog->cur_sel)
        {
            dialog->private->show.yes_frame = 1;
            dialog->private->show.no_frame = 0;
        }
        else 
        {
            dialog->private->show.yes_frame = 0;
            dialog->private->show.no_frame = 1;
        }
        dialog_paint(dialog);
        break;
        
    case KEY_MENU | KEY_TYPE_DOWN:
        break;
        
    case KEY_OK | KEY_TYPE_SHORT_UP:
    case KEY_OK | KEY_TYPE_LONG_UP:
    case KEY_OK | KEY_TYPE_HOLD_UP:
        /* 调用应用的回调 */
        if (0 == dialog->cur_sel)
        {
            //选择yes, 发送信号ok
            dialog->callback( dialog, WIDGET_SIG_DIALOG_OK );
        }
        else
        {
            //选择no, 发送信号cancal
            dialog->callback( dialog, WIDGET_SIG_DIALOG_CANCAL );
        }
        break;
        
    case KEY_RETURN | KEY_TYPE_SHORT_UP:
    case KEY_RETURN | KEY_TYPE_LONG_UP:
    case KEY_RETURN | KEY_TYPE_HOLD_UP:
        /* 调用应用的回调 */
        //按return键, 发送信号cancal
        dialog->callback( dialog, WIDGET_SIG_DIALOG_CANCAL );
        break;

    default:
        break;
    }
    
    return 1;
}


int dialog_proc_gui( dialog_t *dialog, gui_wm_message_t *pmsg)
{
    print_dbg("msgid:%d", pmsg->msgid);
    if ( NULL == dialog )
    {
        return false;
    }
    
    switch ( pmsg->msgid )
    {
    case WM_MSG_KEY:
        _proc_key( dialog, (key_event_t *)(pmsg->data.p));
        break;
        
    default:
        break;
    }
    
    return 1;
}


dialog_t* dialog_create( dialog_init_t *init, dialog_cb_t callback )
{
    bool result = true;
    sty_rect_t srect;
    dialog_t *dialog;

    //对传入的参数进行检查, 除str_id可以为空外, 其他必须有内容
    if ( (NULL == init) || (init->head >= DIALOG_HEAD_MAX) || (NULL == callback) )
    {
        print_err("create param err\n\n");
        return NULL;
    }
    print_dbg("head:%d, str_id:%d", init->head, init->str_id);
    
    //malloc dialog
    dialog = (dialog_t *)malloc( sizeof(dialog_t) );
    if ( NULL == dialog)
    {
        print_err("malloc dialog_t fail\n\n");
        goto DIALOG_ERR_MALLOC_DIALOG;
    }
    memset(dialog, 0x00, sizeof(dialog_t));
    
    //malloc dialog private
    dialog->private = (dialog_private_t *)malloc( sizeof(dialog_private_t) );
    if ( NULL == dialog->private)
    {
        print_err("malloc dialog_private_t fail\n\n");
        goto DIALOG_ERR_MALLOC_PRIVATE;
    }
    memset(dialog->private, 0x00, sizeof(dialog_private_t));
    
    /* init dialog */
    dialog->head        = init->head;
    dialog->str_id      = init->str_id;
    dialog->callback    = callback;
    dialog->canvas_id   = -1;
    if(DIALOG_HEAD_QUERY == init->head)
    {
        dialog->cur_sel = init->def_sel;
    }
    
    result = _init(dialog);
    if (false == result)
    {
        print_err("_init fail\n\n");
        goto DIALOG_ERR_INIT;
    }
    
    //load style resource
    result = dialog_load_style( dialog );
    if (false == result)
    {
        print_err("dialog_load_style fail\n\n");
        goto DIALOG_ERR_INIT;
    }
    
    //create canvas必须使用绝对坐标
    srect.x     = dialog->private->dialog_resgroup->absolute_x;
    srect.y     = dialog->private->dialog_resgroup->absolute_y;
    srect.width = dialog->private->dialog_resgroup->width;
    srect.height= dialog->private->dialog_resgroup->height;
    dialog->canvas_id = canvas_create( &srect, TRANSPARENT );
    if ( -1 == dialog->canvas_id )
    {
        print_warning("canvas_create fail\n\n");
        goto DIALOG_ERR_CANVAS;
    }
    canvas_erase_all(dialog->canvas_id);
    
    //获取gui dc
    dialog->private->handle_gui_dc = gui_dc_get( dialog->canvas_id );
    if (dialog->private->handle_gui_dc <= 0)
    {
        print_err("gui_dc_get fail\n\n");
        goto DIALOG_ERR;
    }

    return dialog;
    
    DIALOG_ERR:
    canvas_delete( dialog->canvas_id );
    dialog->canvas_id = -1;
    
    DIALOG_ERR_CANVAS:
    kill_inner_timer(dialog->private->timer_id);
    dialog->private->timer_id = -1;
    
    DIALOG_ERR_INIT:
    free( dialog->private );
    
    DIALOG_ERR_MALLOC_PRIVATE:
    free( dialog );
    
    DIALOG_ERR_MALLOC_DIALOG:
    return NULL;
}


bool dialog_delete( dialog_t *dialog )
{
    print_dbg();

    if ( NULL == dialog )
    {
        print_warning("dialog is NULL\n\n");
        return false;
    }
    
    _deinit( dialog );
    
    //delete canvas
    if ( dialog->canvas_id >= 0 )
    {
        canvas_delete( dialog->canvas_id );
        dialog->canvas_id = -1;
    }
    
    //free memory
    free( dialog->private );
    dialog->private = (dialog_private_t *)NULL;
    free( dialog );
    dialog = (dialog_t *)NULL;
    
    return true;
}

