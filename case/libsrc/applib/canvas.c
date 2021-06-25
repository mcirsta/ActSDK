/********************************************************************************
 *                              USDK(1100)
 *                             Module: app common
 *                 Copyright(c) 2003-2009 Actions Semiconductor,
 *                            All Rights Reserved. 
 * File : canvas.c
 * History:         
 *      <author>    <time>           <version >             <desc>
 *    songzhining    2009-04-04          v1.0               build this file 
 ********************************************************************************/
/*!
* \file     canvas.c
* \brief    canvas operation
* \author   songzhining
*               
* \version 1.0
* \date  2009/04/04
*******************************************************************************/
    
#include "include_case.h"
#include "act_mem.h"


/* define max layers */
#define CANVAS_MAX_LAYERS    10

/* define canvas internal struct */
typedef struct canvas_s
{
    int id;
    canvas_mode_e mode;
    gui_layer_createinfo_t layer_info;
    bool visible;
    bool focus;
        
}canvas_t;

/* layer no. of current canvas */
static int s_layer_no = 0;

/* canvas data */
static canvas_t s_canvas_layer[CANVAS_MAX_LAYERS];

/* memory mode: 1--physical continous */
static bool s_physic_continuous = false;

static int s_last_focus = ERR_NOFOCUS;

static bool _get_framebuffer_info( fb_info_t *fb_info );

/************************************************************************************/
/*!
 * \par  Description:
 *     获取画布所在的层次(1~7)
 * \param[in]     None
 * \return      layer no. 画布层次(1~7), 找不到画布时返回0
 * \par          exmaple code
 * \code
 *                layer_no = canvas_get_layer( canvas_id );    //获取画布所在的层次
 * \endcode
 ************************************************************************************/
static int canvas_get_layer( int canvas_id )
{
    int layer;

    //查找id所在的layer
    for ( layer = 0; layer <= s_layer_no; layer ++ )
    {
        if ( s_canvas_layer[layer].id == canvas_id )
            break;
    }
    
    if ( layer > s_layer_no )    //cannot found
    {
        layer = 0;
    }
    else
    {
    }    

    return layer;
}


/************************************************************************************/
/*!
 * \par  Description:
 *     创建画布，默认为显示状态
 * \param[in]     rect   画布区域
 * \param[in]     mode   画布模式
 * \return          成功返回 画布句柄, 失败返回-1
 * \par          exmaple code
 * \code
 *                rect.x = 0;
 *                rect.y = 0;
 *                rect.width = 240;
 *                rect.height = 320; 
 *                canvas_id = canvas_create( &rect, TRANSPARENT );    //创建透明画布
 * \endcode
 ************************************************************************************/
int canvas_create( sty_rect_t *rect, canvas_mode_e mode )
{
    canvas_t* canvas;
    gui_layer_createinfo_t* layer;
    int bpp;

    //if layer is full
    if ( s_layer_no >= (CANVAS_MAX_LAYERS-1) )
    {
        return -1;
    }
    else
    {
        s_layer_no ++;    //next layer
        canvas = &s_canvas_layer[s_layer_no];    //get layer data
        layer = &canvas->layer_info;
        canvas->mode = mode;
        canvas->visible = true;
        canvas->focus = true;
    }
    
    //计算区域
    layer->rect1.x0 = rect->x;
    layer->rect1.y0 = rect->y;
    layer->rect1.x1 = ( rect->x + rect->width ) - 1;
    layer->rect1.y1 = ( rect->y + rect->height ) - 1;
    
    switch( canvas->mode )
    {
    case OPACITY:
        layer->fbformat = GUI_LAYER_FORMAT_RGB565;
        bpp = 2;
        break;
        
    case TRANSPARENT:
        layer->fbformat = GUI_LAYER_FORMAT_RGB565A;
        bpp = 3;
        break;
        
    default:
        layer->fbformat = GUI_LAYER_FORMAT_RGB565A;
        bpp = 3;
        break;
    }    

    //分配空间
    if ( true == s_physic_continuous )
    {
        layer->buf = (char *)sys_mem_allocate( (unsigned int)(rect->width * rect->height * bpp), \
                                                MEM_CONTINUOUS|WRITE_THROUGH_MEM);
    }
    else
    {
        layer->buf = (char *)malloc( (unsigned int)(rect->width * rect->height * bpp));
    }

    if ( layer->buf != NULL )
    {
        //清空
        memset( layer->buf, 0, (unsigned int)(rect->width * rect->height * bpp) );
        
        //创建图层
        gui_fb_device_control( s_layer_no, GUI_LAYER_CREATE_FBDEV, layer );
        
        //选择新图层
        gui_select_layer( (unsigned int)s_layer_no );
        
         //创建窗口
        canvas->id = gui_wm_create_window( rect->x, rect->y, rect->width, rect->height, WM_CREATE_SHOW, NULL, 0);

        //选择图层0
        gui_select_layer( 0 );

        return canvas->id;
    }
    else
    {
        print_err("malloc error!\n");
        canvas->id = 0;

        return -1;
    }
}


/************************************************************************************/
/*!
 * \par  Description:
 *     删除画布
 * \param[in]     canvas id   画布句柄
 * \return      the result
 * \retval       true    设置成功
 * \retval       false    设置失败
 * \par          exmaple code
 * \code
 *                result = canvas_delete( canvas_id );    //创建透明画布
 * \endcode
 ************************************************************************************/
bool canvas_delete( int canvas_id )
{
    canvas_t* canvas;
    int layer;

    //查找id所在的layer
    layer = canvas_get_layer( canvas_id );
    
    if ( layer > 0 )    //found
    {
        canvas = &s_canvas_layer[layer];
        
        //删除窗体
        gui_wm_delete_window( canvas->id );
        canvas->id = 0;
        canvas->visible = false;
        canvas->focus = false;

        //删除layer
        gui_fb_device_control( layer, GUI_LAYER_DESTROY_FBDEV, 0 );

        //free buffer
        if (canvas->layer_info.buf != NULL)
        {                   
            if ( true == s_physic_continuous )
            {
                sys_mem_free( canvas->layer_info.buf );
            }
            else
            {
                free( canvas->layer_info.buf ); 
            }
            canvas->layer_info.buf = NULL;
        }
        
        //更新layer层数        
        if ( layer == s_layer_no )    //delete top layer?
        {            
            for ( ; layer>0; layer-- )
            {
                if ( s_canvas_layer[layer].id != 0 )
                {            
                    break;
                }
            }
            
            s_layer_no = layer;
        }        

        return true;
    }
    else
    {   
        return false;
    }
}


/************************************************************************************/
/*!
 * \par  Description:
 *     显示画布
 * \param[in]     canvas id   画布句柄
 * \return       the result
 * \retval       true    设置成功
 * \retval       false    设置失败
 * \par          exmaple code
 * \code
 *                result = canvas_show( canvas_id );    //显示画布
 * \endcode
 ************************************************************************************/
bool canvas_show( int canvas_id )
{
    canvas_t* canvas;
    int layer;

    //查找id所在的layer
    layer = canvas_get_layer( canvas_id );
    
    if ( layer > 0 )    //found
    {
        //操作GUI图层
        gui_fb_device_control( layer, GUI_LAYER_SET_FBDEV_VISIBLE, ( void* )1 );

        canvas = &s_canvas_layer[layer];
        canvas->visible = true;

        return true;
    }
    else
    {
        return false;
    }
}


/************************************************************************************/
/*!
 * \par  Description:
 *     隐藏画布
 * \param[in]     canvas id   画布句柄
 * \return       the result
 * \retval       true    设置成功
 * \retval       false    设置失败
 * \par          exmaple code
 * \code
 *                result = canvas_hide( canvas_id );    //隐藏画布
 * \endcode
 ************************************************************************************/
bool canvas_hide( int canvas_id )
{
    canvas_t* canvas;
    int layer;

    //查找id所在的layer
    layer = canvas_get_layer( canvas_id );
    
    if ( layer > 0 )    //found
    {
        //操作GUI图层
        gui_fb_device_control( layer, GUI_LAYER_SET_FBDEV_VISIBLE, ( void* )0 );

        canvas = &s_canvas_layer[layer];
        canvas->visible = false;

        return true;
    }
    else
    {
        return false;
    }
}


/************************************************************************************/
/*!
 * \par  Description:
 *     改变画布区域
 * \param[in]     canvas id   画布句柄
 * \param[in]     rect   画布区域
 * \return       the result
 * \retval       true    设置成功
 * \retval       false    设置失败
 ************************************************************************************/
bool canvas_set_region( int canvas_id, sty_rect_t *rect )
{
    canvas_t* canvas;
    gui_layer_createinfo_t* layer_info;
    int layer;
    int bpp;
    int width;
    int height;
    int dx;
    int dy;

    //查找id所在的layer
    layer = canvas_get_layer( canvas_id );
    
    if ( layer > 0 )    //found
    {
        canvas = &s_canvas_layer[layer];
        layer_info = &canvas->layer_info;

        //大小是否变化
        width = ( layer_info->rect1.x1 - layer_info->rect1.x0 ) + 1;
        height = ( layer_info->rect1.y1 - layer_info->rect1.y0 ) + 1;
        if ( (rect->width * rect->height) != (width * height) )
        {
            //free buffer
            if ( layer_info->buf != NULL )
            {
                if ( true == s_physic_continuous )
                {
                    sys_mem_free( layer_info->buf );
                }
                else
                {
                    free( layer_info->buf ); 
                }
                
                layer_info->buf = NULL;
            }        

            switch( canvas->mode )
            {
            case OPACITY:
                layer_info->fbformat = GUI_LAYER_FORMAT_RGB565;
                bpp = 2;
                break;
                
            case TRANSPARENT:
                layer_info->fbformat = GUI_LAYER_FORMAT_RGB565A;
                bpp = 3;
                break;
                
            default:
                layer_info->fbformat = GUI_LAYER_FORMAT_RGB565A;
                bpp = 3;
                break;
            }   
            
            //分配空间            
            if ( true == s_physic_continuous )
            {
                layer_info->buf = (char *)sys_mem_allocate( (unsigned int)(rect->width * rect->height * bpp), \
                                                            MEM_CONTINUOUS|WRITE_THROUGH_MEM);
            }
            else
            {
                layer_info->buf = (char *)malloc( (unsigned int)(rect->width * rect->height * bpp) );
            }
            
            if ( layer_info->buf == NULL )
            {            
                print_err();
                return false;
            }

        }
        else
        {            
            if ( ( rect->x == layer_info->rect1.x0 ) && ( rect->y == layer_info->rect1.y0 ) )
            {
                return true;
            }
        }
            
        //改变窗口坐标和大小
        dx = rect->x - layer_info->rect1.x0;
        dy = rect->y - layer_info->rect1.y0;
        gui_wm_move_window( canvas->id, dx, dy );
        gui_wm_set_size( canvas->id, rect->width, rect->height );        
        
        //计算区域
        layer_info->rect1.x0 = rect->x;
        layer_info->rect1.y0 = rect->y;
        layer_info->rect1.x1 = ( rect->x + rect->width ) - 1;
        layer_info->rect1.y1 = ( rect->y + rect->height ) - 1; 

        //改变图层区域
        gui_fb_device_control( layer, GUI_LAYER_CHANGE_FBDEV_REGION, layer_info );

        return true;
    }
    else
    {   
        return false;
    }
}


/************************************************************************************/
/*!
 * \par  Description:
 *     画布擦除指定区域(恢复成背景: 黑色/透明)
 * \param[in]     canvas id   画布句柄
 * \param[in]     rect   擦除区域
 * \return       the result
 * \retval       true    设置成功
 * \retval       false    设置失败
 * \par          exmaple code
 * \code
 *                rect.x = 0;
 *                rect.y = 0;
 *                rect.width = 240;
 *                rect.height = 320; 
 *                result = canvas_erase( canvas_id, &rect );    //画布擦除指定区域
 * \endcode
 ************************************************************************************/
bool canvas_erase( int canvas_id, sty_rect_t *rect )
{
    int layer;
    gui_rect_t r;
    gui_layer_cleaninfo_t clean;
    gui_rect_t *prect;
    short width, height;
    short max_width, max_height;

    //查找id所在的layer
    layer = canvas_get_layer( canvas_id );
    
    if ( layer > 0 )    //found
    {
        //操作GUI图层
        prect = &s_canvas_layer[layer].layer_info.rect1;
        max_width = ( prect->x1 - prect->x0 ) + 1;
        max_height = ( prect->y1 - prect->y0 ) + 1;

        //区域剪裁
        if( (rect->x + rect->width) > max_width )
        {
            width = max_width - rect->x;
        }
        else
        {
            width = rect->width;
        }
        
        if( (rect->y + rect->height) > max_height )
        {
            height = max_height - rect->y;
        }
        else
        {
            height = rect->height;
        }
        
        r.x0 = rect->x;
        r.y0 = rect->y;
        r.x1 = ( rect->x + width ) - 1;
        r.y1 = ( rect->y + height ) - 1;
        clean.layer = layer;
        clean.hwin = canvas_id;
        clean.rect = r;
        gui_fb_device_control( layer, GUI_LAYER_CLEAN_FBDEV_AREA, &clean );

        return true;
    }
    else
    {
        return false;
    }
}


/************************************************************************************/
/*!
 * \par  Description:
 *     画布擦除全部区域(恢复成背景: 黑色/透明)
 * \param[in]     canvas id   画布句柄
 * \return       the result
 * \retval       true    设置成功
 * \retval       false    设置失败
 * \par          exmaple code
 * \code
 *                result = canvas_erase( canvas_id );    //画布擦除全部区域
 * \endcode
 ************************************************************************************/
bool canvas_erase_all( int canvas_id )
{
    int layer;
    gui_rect_t *rect;
    gui_layer_cleaninfo_t clean;

    //查找id所在的layer
    layer = canvas_get_layer( canvas_id );
    
    if ( layer > 0 )    //found
    {
        //操作GUI图层
        rect = &s_canvas_layer[layer].layer_info.rect1;
        clean.layer = layer;
        clean.hwin = canvas_id;
        clean.rect.x0 = 0;
        clean.rect.y0 = 0;
        clean.rect.x1 = rect->x1 - rect->x0;
        clean.rect.y1 = rect->y1 - rect->y0;
        gui_fb_device_control( layer, GUI_LAYER_CLEAN_FBDEV_AREA, &clean );

        return true;
    }
    else
    {
        return false;
    }
}



/************************************************************************************/
/*!
 * \par  Description:
 *     设置画布显示的透明度(alpha值)
 * \param[in]     canvas id   画布句柄
 * \param[in]     alpha   画布显示的alpha值(0~255)
 * \return       the result
 * \retval       true    设置成功
 * \retval       false    设置失败
 * \par          exmaple code
 * \code
 *                result = canvas_set_alpha( canvas_id, 10 );    //设置画布显示的透明度
 * \endcode
 ************************************************************************************/
bool canvas_set_alpha( int canvas_id, int alpha )
{
    int layer;

    //查找id所在的layer
    layer = canvas_get_layer( canvas_id );
    
    if ( layer > 0 )    //found
    {
        //操作GUI图层
        gui_fb_device_control( layer, GUI_LAYER_SET_FBDEV_ALPHA, ( void* )alpha );

        return true;
    }
    else
    {
        return false;
    }
}


/************************************************************************************/
/*!
 * \par  Description:
 *     获取顶层可见带焦点画布
 * \param[in]     None
 * \return          成功返回 画布句柄, 失败返回-1
 * \par          exmaple code
 * \code
 *                canvas_id = canvas_get_top();    // 获取顶层带焦点画布
 * \endcode
 ************************************************************************************/
int canvas_get_top( void )
{
    int canvas_id;
    int layer;
    
    //设置出错返回值
    if ( s_layer_no <= 0 )
    {
        canvas_id = ERR_NOCANVAS;
    }
    else
    {
        canvas_id = ERR_NOFOCUS;
    }
    
    //查找最上层可见带焦点画布
    for ( layer = s_layer_no; layer > 0; layer -- )
    {
        if ( ( true == s_canvas_layer[layer].visible )
                && ( true == s_canvas_layer[layer].focus ) )
        {
            canvas_id = s_canvas_layer[layer].id;
            break;
        }
    }
        
    return canvas_id;
}


/************************************************************************************/
/*!
 * \par  Description:
 *     设置为顶层画布
 * \param[in]     canvas id   画布句柄
 * \return       the result
 * \retval       true    设置成功
 * \retval       false    设置失败
 ************************************************************************************/
bool canvas_set_top( int canvas_id )
{
    return false;
}


/************************************************************************************/
/*!
 * \par  Description:
 *     获取framebuffer驱动的信息
 * \param[in]    fb_info 屏幕参数指针
 * \return       the result
 * \retval        true    设置成功
 * \retval        false    设置失败
 ************************************************************************************/
static bool _get_framebuffer_info( fb_info_t *fb_info )
{ 
    int fd; 
    int ret;

    fd = sys_open( DEVICE_FB, O_RDWR, 0 );   
    if ( fd < 0 ) 
    {
        print_err("open device fb failed!\n");
        return false;
    }
    else
    {
        print_dbg("fb = %d\n", fd);
    }

    //获取屏幕参数
    ret = sys_ioctl( (unsigned int)fd, FBIOGET_FB_PARAM, (unsigned long)fb_info );  
    if ( ret < 0 ) 
    {
        print_err("ioctl device fb failed! ret=%d\n", ret);
        sys_close( (unsigned int)fd );
        return false;
    }

    sys_close( (unsigned int)fd );
    
    return true;    
}

/************************************************************************************/
/*!
 * \par  Description:
 *     获取画布信息
 * \param[in]     canvas id   画布句柄(为0时表示GUI第0层)
 * \param[in]     info   画布信息指针
 * \return       the result
 * \retval       true    成功
 * \retval       false    失败
 * \par          exmaple code
 * \code
 *                result = canvas_get_info( canvas_id, &info );
 *                result = canvas_get_info( 0, &info ); //获取第0层
 * \endcode
 ************************************************************************************/
bool canvas_get_info( int canvas_id, canvas_info_t *info )
{
    int layer;
    canvas_t* canvas;
    gui_rect_t *rect;
    fb_info_t fb_info = {0};

    //获取第0层
    if ( canvas_id == 0 )
    {
        //get framebuffer info
        _get_framebuffer_info( &fb_info );

        //获取信息
        info->mode = OPACITY;
        
        info->rect.x = 0;
        info->rect.y = 0;
        info->rect.width = (short)fb_info.xres;
        info->rect.height = (short)fb_info.yres;

        //操作GUI图层
        gui_fb_device_control( 0, GUI_LAYER_GET_LAYER_0_BUFFER, ( void* )&info->pdata );

        return true;
    }

    //查找id所在的layer
    layer = canvas_get_layer( canvas_id );
    
    if ( layer > 0 )    //found
    {
        canvas = &s_canvas_layer[layer];
        rect = &canvas->layer_info.rect1;

        //获取信息
        info->mode = canvas->mode;
        
        info->rect.x = rect->x0;
        info->rect.y = rect->y0;
        info->rect.width = ( rect->x1 - rect->x0 ) + 1;
        info->rect.height = ( rect->y1 - rect->y0 ) + 1;

        info->pdata = canvas->layer_info.buf;

        return true;
    }
    else
    {
        return false;
    }
}

/************************************************************************************/
/*!
 * \par  Description:
 *     获取顶层画布(blending时不关心canvas是否可见)
 * \param[in]     None
 * \return          成功返回 画布句柄, 失败返回-1
 * \par          exmaple code
 * \code
 *                canvas_id = canvas_get_blending_top();    // 获取顶层画布
 * \endcode
 ************************************************************************************/
int canvas_get_blending_top( void )
{
    int canvas_id;
    int layer;
    
    //设置出错返回值
    if ( s_layer_no <= 0 )
    {
        canvas_id = ERR_NOCANVAS;
    }
    else
    {
        canvas_id = ERR_NOFOCUS;
    }
    
    //查找最上层可见带焦点画布
    for ( layer = s_layer_no; layer > 0; layer -- )
    {
        if ( true == s_canvas_layer[layer].focus )
        {
            canvas_id = s_canvas_layer[layer].id;
            break;
        }
    }
        
    return canvas_id;
}

/************************************************************************************/
/*!
 * \par  Description:
 *     设置画布为YUV blending模式
 * \param[in]    is_blending   1表示YUV blending模式
 * \return       the result
 * \retval       true    设置成功
 * \retval       false    设置失败
 * \note        blending模式: 分配物理连续空间(dma使用);  设置GUI为YUV模式
 ************************************************************************************/
bool canvas_set_blending_mode( bool is_blending )
{
    //set memory mode
    s_physic_continuous = is_blending;

    //set yuv mode to gui
    if( true == is_blending )
    {
        gui_set_display_mode( GUI_DISPLAY_MODE_YUV );
    }
    else
    {
        gui_set_display_mode( GUI_DISPLAY_MODE_NON_YUV );
    }
    
    return true;
}

/************************************************************************************/
/*!
 * \par  Description:
 *     禁止画布焦点
 * \ingroup applib_canvas
 * \param[in]     canvas id   画布句柄
 * \return       the result
 * \retval       true    设置成功
 * \retval       false    设置失败
 ************************************************************************************/
bool canvas_disable_focus( int canvas_id )
{
    canvas_t* canvas;
    int layer;

    //查找id所在的layer
    layer = canvas_get_layer( canvas_id );
    
    if ( layer > 0 )    //found
    {
        canvas = &s_canvas_layer[layer];
        canvas->focus = false;

        return true;
    }
    else
    {
        return false;
    }
}

/************************************************************************************/
/*!
 * \par  Description:
 *     使能画布焦点
 * \ingroup applib_canvas
 * \param[in]     canvas id   画布句柄
 * \return       the result
 * \retval       true    设置成功
 * \retval       false    设置失败
 ************************************************************************************/
bool canvas_enable_focus( int canvas_id )
{
    canvas_t* canvas;
    int layer;

    //查找id所在的layer
    layer = canvas_get_layer( canvas_id );
    
    if ( layer > 0 )    //found
    {
        canvas = &s_canvas_layer[layer];
        canvas->focus = true;

        return true;
    }
    else
    {
        return false;
    }
}

/************************************************************************************/
/*!
 * \par  Description:
 *     获取当前焦点画布
 * \param[in]     None
 * \return          成功返回 画布句柄, 失败返回-1
 * \par          exmaple code
 * \code
 *                canvas_id = canvas_get_focus(pmsg);    // 获取当前焦点画布
 * \endcode
 * \note        在KEY DOWN时才切换焦点: 保证完整按键序列传递给1个对象
 ************************************************************************************/
int canvas_get_focus( gui_wm_message_t *pmsg )
{
    key_event_t *kmsg;
    static bool s_update_focus = false;

    if ( pmsg == NULL )
    {
        return s_last_focus;
    }
    
    switch ( pmsg->msgid ) 
    {
    case WM_MSG_KEY:
        kmsg = (key_event_t *)(pmsg->data.p);    //get key msg        
        switch ( kmsg->type ) 
        {
        case KEY_TYPE_DOWN:
            s_last_focus = canvas_get_top();
            s_update_focus = true;
            break;
             
        case KEY_TYPE_LONG:
        case KEY_TYPE_HOLD:
            if ( s_update_focus == false )
            {
                s_last_focus = canvas_get_top();
                s_update_focus = true;
            }
            break;
            
        case KEY_TYPE_SHORT_UP:
        case KEY_TYPE_LONG_UP:
        case KEY_TYPE_HOLD_UP:
            if ( s_update_focus == false )
            {
                s_last_focus = canvas_get_top();
            }
            s_update_focus = false;
            break;
            
        default:
            break;
        } 
        break;
         
    case WM_MSG_TOUCH:
        break;
        
    default:
        break;
    }
    
    return s_last_focus;
}

/************************************************************************************/
/*!
 * \par  Description:
 *     获取当前焦点画布(blending时不关心canvas是否可见)
 * \param[in]     None
 * \return          成功返回 画布句柄, 失败返回-1
 * \par          exmaple code
 * \code
 *                canvas_id = canvas_get_blending_focus(pmsg);    // 获取当前焦点画布
 * \endcode
 * \note        在KEY DOWN时才切换焦点: 保证完整按键序列传递给1个对象
 ************************************************************************************/
int canvas_get_blending_focus( gui_wm_message_t *pmsg )
{
    key_event_t *kmsg;

    if ( pmsg == NULL )
    {
        return s_last_focus;
    }
    
    switch ( pmsg->msgid ) 
    {
    case WM_MSG_KEY:
        kmsg = (key_event_t *)(pmsg->data.p);    //get key msg        
        switch ( kmsg->type ) 
        {
        case KEY_TYPE_DOWN:
            s_last_focus = canvas_get_blending_top();
            break;
             
        default:
            break;
        } 
        break;
         
    case WM_MSG_TOUCH:
        break;
        
    default:
        break;
    }
    
    return s_last_focus;
}

