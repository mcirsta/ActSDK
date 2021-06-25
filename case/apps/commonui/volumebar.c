/********************************************************************************
 *                              USDK(1100)
 *                             Module: app 
 *                 Copyright(c) 2003-2009 Actions Semiconductor,
 *                            All Rights Reserved. 
 * File : volumebar.c
 * History:         
 *      <author>    <time>           <version >             <desc>
 *    wangsheng    2009-4-9          v1.0               build this file 
 ********************************************************************************/
/*!
* \file     volumebar.c
* \brief    volumebar widget
* \author   wangsheng
*               
* \version 1.0
* \date  2009/04/09
*******************************************************************************/
        
#include "include_case.h"
#include "include_psp.h"

#include "volumebar_private.h"


static void mount_ada(void);
static bool _set_vol( int vol );
static int _get_vol( void );
static int _proc_timer(void *param);
static int _proc_key( volumebar_t *data, key_event_t *pmsg );


/************************************************************************
* Function:            mount_ada                                   
* Description:         load ada driver                             
* Para1:                                                              
* Return\uFF1A           none                                              
*************************************************************************/
static void mount_ada(void)
{
    insmod("ada.ko",0);
    sys_mknod("/dev/ada", S_IFCHR, makedev(MAJOR_ADA, 0));
}

/*************************************************************************
* Function:            _set_vol                                  
* Description:        \u8BBE\u7F6E\u97F3\u4E50\u64AD\u653E\u7684\u97F3\u91CF,\u8303\u56F4[0,31]                     
* para1\uFF1A           \u97F3\u91CF[0,31]                            
* Return\uFF1A          bool   TRUE:  \u6210\u529F   FALSE: \u5931\u8D25                    
*************************************************************************/
static bool _set_vol( int vol )
{
    int fd = 0;
    int ret;
        
    fd = open( "/dev/ada", O_RDWR );
        
    if( vol < VBAR_MIN_VOL )
    {
        vol = VBAR_MIN_VOL;
    }
    else if( vol > VBAR_MAX_VOL )
    {
        vol = VBAR_MAX_VOL;
    }
    else
    {
        print_info("set vol");//for qac
    }
        
    ret = ioctl( fd, DACIO_SET_VOLUME, vol );
        
    close( fd );
        
    return (ret == 0);
}

/*************************************************************************
* Function:            _get_vol                                  
* Description:       \u83B7\u53D6\u97F3\u4E50\u64AD\u653E\u7684\u97F3\u91CF,\u8303\u56F4[0,31]                     
* para1\uFF1A           none                         
* Return\uFF1A           \u97F3\u91CF[0,31]                     
*************************************************************************/
static int _get_vol( void )
{
    int vol = 0;
    int fd = 0;
    int ret;
        
    fd = open( "/dev/ada", O_RDWR );
        
    ret = ioctl( fd, DACIO_GET_VOLUME, &vol );
        
    if( vol < VBAR_MIN_VOL )
    {
        vol = VBAR_MIN_VOL;
    }
    else if( vol > VBAR_MAX_VOL )
    {
        vol = VBAR_MAX_VOL;
    }
    else
    {
        print_info("get_vol");//for qac
    }
        
    close( fd );
        
    return vol;
}

/*!
* timer\u5904\u7406\u51FD\u6570
*/
static int _proc_timer(void *param)
{
    volumebar_t *vbar = (volumebar_t *)param;
    vbar->callback( vbar, WIDGET_SIG_VOLUMEBAR_QUIT);
    return 1;
}

bool volumebar_paint( volumebar_t *vbar )
{    
    vbar_draw_bg(vbar);
    vbar_draw_icon(vbar);
    vbar_draw_dec(vbar);
    vbar_draw_inc(vbar);
    vbar_draw_pb(vbar);
    vbar_draw_logo(vbar);
    vbar_draw_val(vbar);    
    return true;
}

/*!
* volumebar\u5904\u7406\u6309\u952E\u6D88\u606F\u7684\u51FD\u6570
*/
static int _proc_key( volumebar_t *vbar, key_event_t *pmsg )
{
    switch ( pmsg->val | pmsg->type ) 
    {
    case KEY_PREV | KEY_TYPE_DOWN:
    case KEY_VSUB | KEY_TYPE_DOWN:
        restart_timer(vbar->timer_id);//restart timer
        vbar->private_data->inc_pressed = 0;    //set inc up flag           
        vbar->private_data->dec_pressed = 1;    //set dec press flag

        vbar->private_data->vol_inc_status = need_paint;
        vbar->private_data->vol_dec_status = need_paint;
        vbar->private_data->vol_pb_status = need_paint;
        vbar->private_data->vol_val_status = need_paint;

        vbar->private_data->vol_bg_status = not_need_paint;
        vbar->private_data->vol_icon_status = not_need_paint;
        vbar->private_data->vol_logo_status = not_need_paint;
                
        if ( vbar->cur_value > vbar->min_value )
        {
            vbar->cur_value--;  //value dec     
            _set_vol( vbar->cur_value );
                        
            volumebar_paint( vbar );    //repaint
                        
            vbar->callback( vbar, WIDGET_SIG_VOLUMEBAR_CHANGED);        //vol changed,send signal
        }
        clear_key_msg_queue( -1, KEY_TYPE_DOWN );//清除多余按键；
        break;

    case KEY_PREV | KEY_TYPE_LONG:
    case KEY_PREV | KEY_TYPE_HOLD:
    case KEY_VSUB | KEY_TYPE_LONG:
    case KEY_VSUB | KEY_TYPE_HOLD:
        restart_timer(vbar->timer_id);//restart timer
        
        vbar->private_data->inc_pressed = 0;    //set inc up flag
        vbar->private_data->dec_pressed = 1;    //set dec press flag

        vbar->private_data->vol_inc_status = need_paint;
        vbar->private_data->vol_dec_status = need_paint;
        vbar->private_data->vol_pb_status = need_paint;
        vbar->private_data->vol_val_status = need_paint;

        vbar->private_data->vol_bg_status = not_need_paint;
        vbar->private_data->vol_icon_status = not_need_paint;
        vbar->private_data->vol_logo_status = not_need_paint;
                
        if ( vbar->cur_value > vbar->min_value )
        {
            vbar->cur_value--;  //value dec     
            _set_vol( vbar->cur_value );
                        
            volumebar_paint( vbar );    //repaint
                        
            vbar->callback( vbar, WIDGET_SIG_VOLUMEBAR_CHANGED );       //vol changed,send signal
        }
        clear_key_msg_queue( -1, KEY_TYPE_HOLD );//清除多余按键；

        break;

    case KEY_PREV | KEY_TYPE_SHORT_UP:
    case KEY_PREV | KEY_TYPE_LONG_UP:
    case KEY_PREV | KEY_TYPE_HOLD_UP:
    case KEY_VSUB | KEY_TYPE_SHORT_UP:
    case KEY_VSUB | KEY_TYPE_LONG_UP:
    case KEY_VSUB | KEY_TYPE_HOLD_UP:
        vbar->private_data->dec_pressed = 0;
        vbar->private_data->vol_inc_status = need_paint;
        vbar->private_data->vol_dec_status = need_paint;
        
        vbar->private_data->vol_pb_status = not_need_paint;
        vbar->private_data->vol_val_status = not_need_paint;
        vbar->private_data->vol_bg_status = not_need_paint;
        vbar->private_data->vol_icon_status = not_need_paint;
        vbar->private_data->vol_logo_status = not_need_paint;
        

        volumebar_paint( vbar );        //repaint
        restart_timer(vbar->timer_id);//restart timer 
        clear_key_msg_queue( -1, -1 );//清除多余按键；
        break;

    case KEY_NEXT | KEY_TYPE_DOWN:
    case KEY_VADD | KEY_TYPE_DOWN:
        restart_timer(vbar->timer_id);//restart timer
                
        vbar->private_data->dec_pressed = 0;   //set dec up flag
        vbar->private_data->inc_pressed = 1;    //set inc press flag

        vbar->private_data->vol_dec_status = need_paint;
        vbar->private_data->vol_inc_status = need_paint;
        vbar->private_data->vol_pb_status = need_paint;
        vbar->private_data->vol_val_status = need_paint;

        vbar->private_data->vol_bg_status = not_need_paint;
        vbar->private_data->vol_icon_status = not_need_paint;
        vbar->private_data->vol_logo_status = not_need_paint;
        
        if ( vbar->cur_value < vbar->max_value )
        {
            vbar->cur_value++;  //value inc
            _set_vol( vbar->cur_value );
                        
            volumebar_paint( vbar );    //repaint
                                
            vbar->callback( vbar, WIDGET_SIG_VOLUMEBAR_CHANGED );       //vol changed,send signal
        }
        clear_key_msg_queue( -1, KEY_TYPE_DOWN );//清除多余按键；
        break;

    case KEY_NEXT | KEY_TYPE_LONG:
    case KEY_NEXT | KEY_TYPE_HOLD:
    case KEY_VADD | KEY_TYPE_LONG:
    case KEY_VADD | KEY_TYPE_HOLD:
        restart_timer(vbar->timer_id);//restart timer
        vbar->private_data->dec_pressed = 0;   //set dec up flag
        vbar->private_data->inc_pressed = 1;    //set inc press flag

        vbar->private_data->vol_dec_status = need_paint;
        vbar->private_data->vol_inc_status = need_paint;
        vbar->private_data->vol_pb_status = need_paint;
        vbar->private_data->vol_val_status = need_paint;

        vbar->private_data->vol_bg_status = not_need_paint;
        vbar->private_data->vol_icon_status = not_need_paint;
        vbar->private_data->vol_logo_status = not_need_paint;
                
        if ( vbar->cur_value < vbar->max_value )
        {
            vbar->cur_value++;  //value inc
            _set_vol( vbar->cur_value );
                        
            volumebar_paint( vbar );    //repaint
                                
            vbar->callback( vbar, WIDGET_SIG_VOLUMEBAR_CHANGED );       //vol changed,send signal
        }
        clear_key_msg_queue( -1, KEY_TYPE_HOLD );//清除多余按键；
        break;

    case KEY_NEXT | KEY_TYPE_SHORT_UP:
    case KEY_NEXT | KEY_TYPE_LONG_UP:
    case KEY_NEXT | KEY_TYPE_HOLD_UP:
    case KEY_VADD | KEY_TYPE_SHORT_UP:
    case KEY_VADD | KEY_TYPE_LONG_UP:
    case KEY_VADD | KEY_TYPE_HOLD_UP:
        vbar->private_data->inc_pressed = 0;
        vbar->private_data->vol_dec_status = need_paint;
        vbar->private_data->vol_inc_status = need_paint;

        vbar->private_data->vol_pb_status = not_need_paint;
        vbar->private_data->vol_val_status = not_need_paint;
        vbar->private_data->vol_bg_status = not_need_paint;
        vbar->private_data->vol_icon_status = not_need_paint;
        vbar->private_data->vol_logo_status = not_need_paint;
                
        volumebar_paint( vbar );        //repaint
        restart_timer(vbar->timer_id);//restart timer   
        clear_key_msg_queue( -1, -1 );//清除多余按键；
        break;
         
    case KEY_OK | KEY_TYPE_SHORT_UP:
    case KEY_OK | KEY_TYPE_LONG_UP:
    case KEY_OK | KEY_TYPE_HOLD_UP:
    case KEY_RETURN | KEY_TYPE_SHORT_UP:
    case KEY_RETURN | KEY_TYPE_LONG_UP:
    case KEY_RETURN | KEY_TYPE_HOLD_UP:
        vbar->callback( vbar, WIDGET_SIG_VOLUMEBAR_QUIT ); //hide volumebar
        break;
                
    default:
        break;
    }      
}



/* volumebar entry */
int volumebar_proc_gui( volumebar_t *vbar, gui_wm_message_t *pmsg )
{
    print_dbg("%s----%d-----\n",__FILE__,__LINE__); 
        
    switch ( pmsg->msgid ) 
    {
    case WM_MSG_PAINT:
        break;
         
    case WM_MSG_KEY:
        _proc_key( vbar, (key_event_t *)(pmsg->data.p) );
        break;
        
    default:
        break;
    }
    
    return true;
}


volumebar_t* volumebar_create( int type, volumebar_cb_t callback )
{
    volumebar_t *vbar;
    sty_rect_t rect;

    if(NULL == callback)
    {
        goto vbar_create_err;
    }

    /* malloc interface data */
    vbar = (volumebar_t *)malloc( sizeof(volumebar_t) );                
    if ( NULL == vbar)
    {
        goto vbar_create_err;
    }
    else
    {
        /*!
        * interface data initial
        */
        vbar->type = type;//!< get volumebar type
        vbar->cur_value = _get_vol();//!< read volume from driver
        vbar->min_value = VBAR_MIN_VOL;//!< set min volume
        vbar->max_value = VBAR_MAX_VOL;//< set max volume
        vbar->bg_pic = 0;
        vbar->callback = callback;//!< register callback function
        vbar->canvas_id   = -1;
        vbar->type = type;//load style resource               
        vbar->timer_id = set_single_shot_timer (3000, _proc_timer, (void *)vbar);//set timer
        if(vbar->timer_id < 0)
        {
            goto vbar_timer_create_err;
        }
    }

    /* malloc private data */
    vbar->private_data = (struct volumebar_private_data_s *)malloc(sizeof(struct volumebar_private_data_s));
    if( NULL == vbar->private_data)
    {
        goto vbar_private_data_create_err;
    }
    else
    {
        volumebar_load_style( vbar );

        rect.x = vbar->private_data->vb->absolute_x;
        rect.y = vbar->private_data->vb->absolute_y;
        rect.width = vbar->private_data->vb->width;
        rect.height = vbar->private_data->vb->height;
        vbar->canvas_id = canvas_create( &rect , TRANSPARENT );//create canvas
        if ( -1 == vbar->canvas_id )
        {
            goto canvas_create_err;
        }
      
        /*!
        * private data initial
        */ 
        vbar->private_data->hdc = gui_dc_get((unsigned short)(vbar->canvas_id) );// get hdc
        if(vbar->private_data->hdc == -1)
        {
            goto vbar_hdc_get_err;
        }

        vbar->private_data->dec_pressed = 0;
        vbar->private_data->inc_pressed = 0;    
    }
  
    //insmod ada driver
    //mount_ada();
    print_info("volumebar create ok");

    return vbar;
    
    vbar_hdc_get_err:
    canvas_delete( vbar->canvas_id );
    vbar->canvas_id = -1;
    canvas_create_err:
    free(vbar->private_data);
    vbar->private_data = NULL;
    vbar_private_data_create_err:
    kill_timer(vbar->timer_id);
    vbar->timer_id = -1;
    vbar_timer_create_err:
    free(vbar);
    vbar = NULL;
    vbar_create_err:
    return NULL;
}


bool volumebar_delete( volumebar_t *vbar )
{
    if ( vbar != NULL )
    {
        //delete canvas
        if ( vbar->canvas_id != -1 )
        {
            canvas_delete( vbar->canvas_id );
            vbar->canvas_id = -1;
        }
        //rmmod("ada.ko",0);

        /* kill timer */
        kill_timer(vbar->timer_id);//kill timer
        vbar->timer_id = -1;
    
        //free memory
        free(vbar->private_data);
        vbar->private_data = NULL;
        
        free( vbar );
        vbar = NULL;

        return true;
    }
    else
    {
        return false;
    }
}

bool volumebar_load_style( volumebar_t *vbar )
{
    scene_t* scene;     //!< common ui scene    
    sty_rect_t rect;
        
    //get root resource
    scene = sty_get_common_scene();
    if ( scene == NULL )
    {
        return false;   
    }

    //load resource
    vbar->private_data->vb = ( resgroup_resource_t * )get_scene_child( scene, VOLUMEBAR_RESGROUP);
    if(NULL == vbar->private_data->vb)
    {
        print_err("get resource group volumebar_resgroup failed!");
        return false;   
    }


    vbar->private_data->vb_bg = ( picture_resource_t * )get_resgroup_child( vbar->private_data->vb, PIC_BG );
    vbar->private_data->vb_dec = ( picture_resource_t * )get_resgroup_child( vbar->private_data->vb, PIC_DEC );
    vbar->private_data->vb_dech = ( picture_resource_t * )get_resgroup_child( vbar->private_data->vb, PIC_DECH );
    vbar->private_data->vb_inc = ( picture_resource_t * )get_resgroup_child( vbar->private_data->vb, PIC_INC );
    vbar->private_data->vb_inch = ( picture_resource_t * )get_resgroup_child( vbar->private_data->vb, PIC_INCH );
    vbar->private_data->vb_icon = ( picture_resource_t * )get_resgroup_child( vbar->private_data->vb, PIC_ICON);
    vbar->private_data->vb_pbbg = ( picture_resource_t * )get_resgroup_child( vbar->private_data->vb, PIC_PBBG);
  //  for(i_vol = 0;i_vol < VBAR_MAX_VOL; i_vol++)
  
    vbar->private_data->vb_pb[0] = ( picture_resource_t * )get_resgroup_child( vbar->private_data->vb, PIC_PB1);
    vbar->private_data->vb_pb[1] = ( picture_resource_t * )get_resgroup_child( vbar->private_data->vb, PIC_PB2);
    vbar->private_data->vb_pb[2] = ( picture_resource_t * )get_resgroup_child( vbar->private_data->vb, PIC_PB3);
    vbar->private_data->vb_pb[3] = ( picture_resource_t * )get_resgroup_child( vbar->private_data->vb, PIC_PB4);
    vbar->private_data->vb_pb[4] = ( picture_resource_t * )get_resgroup_child( vbar->private_data->vb, PIC_PB5);
    vbar->private_data->vb_pb[5] = ( picture_resource_t * )get_resgroup_child( vbar->private_data->vb, PIC_PB6);
    vbar->private_data->vb_pb[6] = ( picture_resource_t * )get_resgroup_child( vbar->private_data->vb, PIC_PB7);
    vbar->private_data->vb_pb[7] = ( picture_resource_t * )get_resgroup_child( vbar->private_data->vb, PIC_PB8);
    vbar->private_data->vb_pb[8] = ( picture_resource_t * )get_resgroup_child( vbar->private_data->vb, PIC_PB9);
    vbar->private_data->vb_pb[9] = ( picture_resource_t * )get_resgroup_child( vbar->private_data->vb, PIC_PB10);
    vbar->private_data->vb_pb[10] = ( picture_resource_t * )get_resgroup_child( vbar->private_data->vb, PIC_PB11);
    vbar->private_data->vb_pb[11] = ( picture_resource_t * )get_resgroup_child( vbar->private_data->vb, PIC_PB12);
    vbar->private_data->vb_pb[12] = ( picture_resource_t * )get_resgroup_child( vbar->private_data->vb, PIC_PB13);
    vbar->private_data->vb_pb[13] = ( picture_resource_t * )get_resgroup_child( vbar->private_data->vb, PIC_PB14);
    vbar->private_data->vb_pb[14] = ( picture_resource_t * )get_resgroup_child( vbar->private_data->vb, PIC_PB15);
    vbar->private_data->vb_pb[15] = ( picture_resource_t * )get_resgroup_child( vbar->private_data->vb, PIC_PB16);
    vbar->private_data->vb_pb[16] = ( picture_resource_t * )get_resgroup_child( vbar->private_data->vb, PIC_PB17);
    vbar->private_data->vb_pb[17] = ( picture_resource_t * )get_resgroup_child( vbar->private_data->vb, PIC_PB18);
    vbar->private_data->vb_pb[18] = ( picture_resource_t * )get_resgroup_child( vbar->private_data->vb, PIC_PB19);
    vbar->private_data->vb_pb[19] = ( picture_resource_t * )get_resgroup_child( vbar->private_data->vb, PIC_PB20);
    vbar->private_data->vb_pb[20] = ( picture_resource_t * )get_resgroup_child( vbar->private_data->vb, PIC_PB21);
    vbar->private_data->vb_pb[21] = ( picture_resource_t * )get_resgroup_child( vbar->private_data->vb, PIC_PB22);
    vbar->private_data->vb_pb[22] = ( picture_resource_t * )get_resgroup_child( vbar->private_data->vb, PIC_PB23);
    vbar->private_data->vb_pb[23] = ( picture_resource_t * )get_resgroup_child( vbar->private_data->vb, PIC_PB24);
    vbar->private_data->vb_pb[24] = ( picture_resource_t * )get_resgroup_child( vbar->private_data->vb, PIC_PB25);
    vbar->private_data->vb_pb[25] = ( picture_resource_t * )get_resgroup_child( vbar->private_data->vb, PIC_PB26);
    vbar->private_data->vb_pb[26] = ( picture_resource_t * )get_resgroup_child( vbar->private_data->vb, PIC_PB27);
    vbar->private_data->vb_pb[27] = ( picture_resource_t * )get_resgroup_child( vbar->private_data->vb, PIC_PB28);
    vbar->private_data->vb_pb[28] = ( picture_resource_t * )get_resgroup_child( vbar->private_data->vb, PIC_PB29);
    vbar->private_data->vb_pb[29] = ( picture_resource_t * )get_resgroup_child( vbar->private_data->vb, PIC_PB30);
    vbar->private_data->vb_pb[30] = ( picture_resource_t * )get_resgroup_child( vbar->private_data->vb, PIC_PB31);
    
    vbar->private_data->vb_logo  = ( string_resource_t * )get_resgroup_child( vbar->private_data->vb, STR_LOGO);
    vbar->private_data->vb_val = ( string_resource_t * )get_resgroup_child( vbar->private_data->vb, STR_VALUE );

    vbar->private_data->vol_bg_status = need_paint;// init status
    vbar->private_data->vol_dec_status = need_paint;
    vbar->private_data->vol_inc_status = need_paint;
    vbar->private_data->vol_icon_status = need_paint;
    vbar->private_data->vol_logo_status = need_paint;
    vbar->private_data->vol_pb_status = need_paint;
    vbar->private_data->vol_val_status = need_paint;

    //canvas resize    
    if ( vbar->canvas_id != -1 )
    {
        rect.x = vbar->private_data->vb->absolute_x;
        rect.y = vbar->private_data->vb->absolute_y;
        rect.width = vbar->private_data->vb->width;
        rect.height = vbar->private_data->vb->height;
        
        //set canvas region
        canvas_set_region( vbar->canvas_id, &rect );
    }

    return true;
}
