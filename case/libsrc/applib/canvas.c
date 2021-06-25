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
 *     ��ȡ�������ڵĲ��(1~7)
 * \param[in]     None
 * \return      layer no. �������(1~7), �Ҳ�������ʱ����0
 * \par          exmaple code
 * \code
 *                layer_no = canvas_get_layer( canvas_id );    //��ȡ�������ڵĲ��
 * \endcode
 ************************************************************************************/
static int canvas_get_layer( int canvas_id )
{
    int layer;

    //����id���ڵ�layer
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
 *     ����������Ĭ��Ϊ��ʾ״̬
 * \param[in]     rect   ��������
 * \param[in]     mode   ����ģʽ
 * \return          �ɹ����� �������, ʧ�ܷ���-1
 * \par          exmaple code
 * \code
 *                rect.x = 0;
 *                rect.y = 0;
 *                rect.width = 240;
 *                rect.height = 320; 
 *                canvas_id = canvas_create( &rect, TRANSPARENT );    //����͸������
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
    
    //��������
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

    //����ռ�
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
        //���
        memset( layer->buf, 0, (unsigned int)(rect->width * rect->height * bpp) );
        
        //����ͼ��
        gui_fb_device_control( s_layer_no, GUI_LAYER_CREATE_FBDEV, layer );
        
        //ѡ����ͼ��
        gui_select_layer( (unsigned int)s_layer_no );
        
         //��������
        canvas->id = gui_wm_create_window( rect->x, rect->y, rect->width, rect->height, WM_CREATE_SHOW, NULL, 0);

        //ѡ��ͼ��0
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
 *     ɾ������
 * \param[in]     canvas id   �������
 * \return      the result
 * \retval       true    ���óɹ�
 * \retval       false    ����ʧ��
 * \par          exmaple code
 * \code
 *                result = canvas_delete( canvas_id );    //����͸������
 * \endcode
 ************************************************************************************/
bool canvas_delete( int canvas_id )
{
    canvas_t* canvas;
    int layer;

    //����id���ڵ�layer
    layer = canvas_get_layer( canvas_id );
    
    if ( layer > 0 )    //found
    {
        canvas = &s_canvas_layer[layer];
        
        //ɾ������
        gui_wm_delete_window( canvas->id );
        canvas->id = 0;
        canvas->visible = false;
        canvas->focus = false;

        //ɾ��layer
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
        
        //����layer����        
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
 *     ��ʾ����
 * \param[in]     canvas id   �������
 * \return       the result
 * \retval       true    ���óɹ�
 * \retval       false    ����ʧ��
 * \par          exmaple code
 * \code
 *                result = canvas_show( canvas_id );    //��ʾ����
 * \endcode
 ************************************************************************************/
bool canvas_show( int canvas_id )
{
    canvas_t* canvas;
    int layer;

    //����id���ڵ�layer
    layer = canvas_get_layer( canvas_id );
    
    if ( layer > 0 )    //found
    {
        //����GUIͼ��
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
 *     ���ػ���
 * \param[in]     canvas id   �������
 * \return       the result
 * \retval       true    ���óɹ�
 * \retval       false    ����ʧ��
 * \par          exmaple code
 * \code
 *                result = canvas_hide( canvas_id );    //���ػ���
 * \endcode
 ************************************************************************************/
bool canvas_hide( int canvas_id )
{
    canvas_t* canvas;
    int layer;

    //����id���ڵ�layer
    layer = canvas_get_layer( canvas_id );
    
    if ( layer > 0 )    //found
    {
        //����GUIͼ��
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
 *     �ı仭������
 * \param[in]     canvas id   �������
 * \param[in]     rect   ��������
 * \return       the result
 * \retval       true    ���óɹ�
 * \retval       false    ����ʧ��
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

    //����id���ڵ�layer
    layer = canvas_get_layer( canvas_id );
    
    if ( layer > 0 )    //found
    {
        canvas = &s_canvas_layer[layer];
        layer_info = &canvas->layer_info;

        //��С�Ƿ�仯
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
            
            //����ռ�            
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
            
        //�ı䴰������ʹ�С
        dx = rect->x - layer_info->rect1.x0;
        dy = rect->y - layer_info->rect1.y0;
        gui_wm_move_window( canvas->id, dx, dy );
        gui_wm_set_size( canvas->id, rect->width, rect->height );        
        
        //��������
        layer_info->rect1.x0 = rect->x;
        layer_info->rect1.y0 = rect->y;
        layer_info->rect1.x1 = ( rect->x + rect->width ) - 1;
        layer_info->rect1.y1 = ( rect->y + rect->height ) - 1; 

        //�ı�ͼ������
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
 *     ��������ָ������(�ָ��ɱ���: ��ɫ/͸��)
 * \param[in]     canvas id   �������
 * \param[in]     rect   ��������
 * \return       the result
 * \retval       true    ���óɹ�
 * \retval       false    ����ʧ��
 * \par          exmaple code
 * \code
 *                rect.x = 0;
 *                rect.y = 0;
 *                rect.width = 240;
 *                rect.height = 320; 
 *                result = canvas_erase( canvas_id, &rect );    //��������ָ������
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

    //����id���ڵ�layer
    layer = canvas_get_layer( canvas_id );
    
    if ( layer > 0 )    //found
    {
        //����GUIͼ��
        prect = &s_canvas_layer[layer].layer_info.rect1;
        max_width = ( prect->x1 - prect->x0 ) + 1;
        max_height = ( prect->y1 - prect->y0 ) + 1;

        //�������
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
 *     ��������ȫ������(�ָ��ɱ���: ��ɫ/͸��)
 * \param[in]     canvas id   �������
 * \return       the result
 * \retval       true    ���óɹ�
 * \retval       false    ����ʧ��
 * \par          exmaple code
 * \code
 *                result = canvas_erase( canvas_id );    //��������ȫ������
 * \endcode
 ************************************************************************************/
bool canvas_erase_all( int canvas_id )
{
    int layer;
    gui_rect_t *rect;
    gui_layer_cleaninfo_t clean;

    //����id���ڵ�layer
    layer = canvas_get_layer( canvas_id );
    
    if ( layer > 0 )    //found
    {
        //����GUIͼ��
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
 *     ���û�����ʾ��͸����(alphaֵ)
 * \param[in]     canvas id   �������
 * \param[in]     alpha   ������ʾ��alphaֵ(0~255)
 * \return       the result
 * \retval       true    ���óɹ�
 * \retval       false    ����ʧ��
 * \par          exmaple code
 * \code
 *                result = canvas_set_alpha( canvas_id, 10 );    //���û�����ʾ��͸����
 * \endcode
 ************************************************************************************/
bool canvas_set_alpha( int canvas_id, int alpha )
{
    int layer;

    //����id���ڵ�layer
    layer = canvas_get_layer( canvas_id );
    
    if ( layer > 0 )    //found
    {
        //����GUIͼ��
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
 *     ��ȡ����ɼ������㻭��
 * \param[in]     None
 * \return          �ɹ����� �������, ʧ�ܷ���-1
 * \par          exmaple code
 * \code
 *                canvas_id = canvas_get_top();    // ��ȡ��������㻭��
 * \endcode
 ************************************************************************************/
int canvas_get_top( void )
{
    int canvas_id;
    int layer;
    
    //���ó�����ֵ
    if ( s_layer_no <= 0 )
    {
        canvas_id = ERR_NOCANVAS;
    }
    else
    {
        canvas_id = ERR_NOFOCUS;
    }
    
    //�������ϲ�ɼ������㻭��
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
 *     ����Ϊ���㻭��
 * \param[in]     canvas id   �������
 * \return       the result
 * \retval       true    ���óɹ�
 * \retval       false    ����ʧ��
 ************************************************************************************/
bool canvas_set_top( int canvas_id )
{
    return false;
}


/************************************************************************************/
/*!
 * \par  Description:
 *     ��ȡframebuffer��������Ϣ
 * \param[in]    fb_info ��Ļ����ָ��
 * \return       the result
 * \retval        true    ���óɹ�
 * \retval        false    ����ʧ��
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

    //��ȡ��Ļ����
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
 *     ��ȡ������Ϣ
 * \param[in]     canvas id   �������(Ϊ0ʱ��ʾGUI��0��)
 * \param[in]     info   ������Ϣָ��
 * \return       the result
 * \retval       true    �ɹ�
 * \retval       false    ʧ��
 * \par          exmaple code
 * \code
 *                result = canvas_get_info( canvas_id, &info );
 *                result = canvas_get_info( 0, &info ); //��ȡ��0��
 * \endcode
 ************************************************************************************/
bool canvas_get_info( int canvas_id, canvas_info_t *info )
{
    int layer;
    canvas_t* canvas;
    gui_rect_t *rect;
    fb_info_t fb_info = {0};

    //��ȡ��0��
    if ( canvas_id == 0 )
    {
        //get framebuffer info
        _get_framebuffer_info( &fb_info );

        //��ȡ��Ϣ
        info->mode = OPACITY;
        
        info->rect.x = 0;
        info->rect.y = 0;
        info->rect.width = (short)fb_info.xres;
        info->rect.height = (short)fb_info.yres;

        //����GUIͼ��
        gui_fb_device_control( 0, GUI_LAYER_GET_LAYER_0_BUFFER, ( void* )&info->pdata );

        return true;
    }

    //����id���ڵ�layer
    layer = canvas_get_layer( canvas_id );
    
    if ( layer > 0 )    //found
    {
        canvas = &s_canvas_layer[layer];
        rect = &canvas->layer_info.rect1;

        //��ȡ��Ϣ
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
 *     ��ȡ���㻭��(blendingʱ������canvas�Ƿ�ɼ�)
 * \param[in]     None
 * \return          �ɹ����� �������, ʧ�ܷ���-1
 * \par          exmaple code
 * \code
 *                canvas_id = canvas_get_blending_top();    // ��ȡ���㻭��
 * \endcode
 ************************************************************************************/
int canvas_get_blending_top( void )
{
    int canvas_id;
    int layer;
    
    //���ó�����ֵ
    if ( s_layer_no <= 0 )
    {
        canvas_id = ERR_NOCANVAS;
    }
    else
    {
        canvas_id = ERR_NOFOCUS;
    }
    
    //�������ϲ�ɼ������㻭��
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
 *     ���û���ΪYUV blendingģʽ
 * \param[in]    is_blending   1��ʾYUV blendingģʽ
 * \return       the result
 * \retval       true    ���óɹ�
 * \retval       false    ����ʧ��
 * \note        blendingģʽ: �������������ռ�(dmaʹ��);  ����GUIΪYUVģʽ
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
 *     ��ֹ��������
 * \ingroup applib_canvas
 * \param[in]     canvas id   �������
 * \return       the result
 * \retval       true    ���óɹ�
 * \retval       false    ����ʧ��
 ************************************************************************************/
bool canvas_disable_focus( int canvas_id )
{
    canvas_t* canvas;
    int layer;

    //����id���ڵ�layer
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
 *     ʹ�ܻ�������
 * \ingroup applib_canvas
 * \param[in]     canvas id   �������
 * \return       the result
 * \retval       true    ���óɹ�
 * \retval       false    ����ʧ��
 ************************************************************************************/
bool canvas_enable_focus( int canvas_id )
{
    canvas_t* canvas;
    int layer;

    //����id���ڵ�layer
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
 *     ��ȡ��ǰ���㻭��
 * \param[in]     None
 * \return          �ɹ����� �������, ʧ�ܷ���-1
 * \par          exmaple code
 * \code
 *                canvas_id = canvas_get_focus(pmsg);    // ��ȡ��ǰ���㻭��
 * \endcode
 * \note        ��KEY DOWNʱ���л�����: ��֤�����������д��ݸ�1������
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
 *     ��ȡ��ǰ���㻭��(blendingʱ������canvas�Ƿ�ɼ�)
 * \param[in]     None
 * \return          �ɹ����� �������, ʧ�ܷ���-1
 * \par          exmaple code
 * \code
 *                canvas_id = canvas_get_blending_focus(pmsg);    // ��ȡ��ǰ���㻭��
 * \endcode
 * \note        ��KEY DOWNʱ���л�����: ��֤�����������д��ݸ�1������
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

