/********************************************************************************
 *                              USDK(1100)
 *                             Module: app 
 *                 Copyright(c) 2003-2009 Actions Semiconductor,
 *                            All Rights Reserved. 
 * File : music_list_coverflow.c
 * History:         
 *      <author>    <time>           <version >             <desc>
 *    songzhining    2009-2-25          v1.0               build this file 
 ********************************************************************************/
/*!
* \file     music_list_coverflow.c
* \brief    music_list_coverflow widget
* \author   songzhining
*               
* \version 1.0
* \date  2009/02/25
*******************************************************************************/

#include "music_list_coverflow.h"


static coverflow_view_t *s_coverflow = NULL;

/* s_coverflow load style resource */
static bool _load_style( void )
{
    scene_t* scene;    //!< app scene    
    
    //get root resource
    scene = sty_get_app_scene();
    if ( scene == NULL )
    {
        print_err("cann't find common scene!");
        return false;    
    }

    //load resource
    s_coverflow->pic_bg = ( picture_resource_t * )get_scene_child( scene, PIC_BG );  
    s_coverflow->pic_coverflow = ( picture_resource_t * )get_scene_child( scene, PIC_COVERFLOW );
    s_coverflow->str_coverflow = ( string_resource_t * )get_scene_child( scene, STR_COVERFLOW );

    //set size
    if ( s_coverflow->pic_coverflow != NULL )
    {
        s_coverflow->pic_width = s_coverflow->pic_coverflow->width;
        s_coverflow->pic_height = s_coverflow->pic_coverflow->height;
    }
    
    //bitmap cache
    if ( s_coverflow->pic_bg != NULL )
    {
        s_coverflow->bmp_bg = sty_create_bitmap( (int)s_coverflow->pic_bg->pic_id );
    }
    else
    {
        s_coverflow->bmp_bg = NULL;
    }
    
    if ( s_coverflow->pic_coverflow != NULL )
    {
        s_coverflow->bmp_coverflow = sty_create_bitmap( (int)s_coverflow->pic_coverflow->pic_id );
        s_coverflow->bmp_album = sty_create_bitmap( (int)s_coverflow->pic_coverflow->pic_id );
    }
    else
    {
        s_coverflow->bmp_coverflow = NULL;
        s_coverflow->bmp_album = NULL;
    }    
    
    return true;
}

static bool _inner_res_init( void )
{
    bool ret;
    
    if ( s_coverflow != NULL )
    {
        return true;
    }
    
    //malloc data
    s_coverflow = (coverflow_view_t *)malloc( sizeof(coverflow_view_t) );        
    if ( s_coverflow == NULL )
    {
        print_err("malloc failed!");
        return false;    
    }
    else
    {
        //initial data
        memset( s_coverflow, 0, sizeof(coverflow_view_t) );
    }

    //load style resource    
    ret = _load_style();
    if ( ret == false )
    {
        print_err("load_style failed!");
        free( s_coverflow );
        s_coverflow = NULL;
        return false;
    }
    
    return true;    
}

static bool _inner_res_deinit( void )
{
    if ( s_coverflow != NULL )
    {
        //free bitmap
        sty_delete_bitmap( s_coverflow->bmp_bg );
        sty_delete_bitmap( s_coverflow->bmp_coverflow );
        sty_delete_bitmap( s_coverflow->bmp_album );
    
        //free s_coverflow private
        free( s_coverflow );
        s_coverflow = NULL;
    }
    
    return true;
}

//内存分配
void* coverflow_malloc( unsigned int m_size )
{
    return malloc( m_size );
}

//内存释放
void coverflow_free( void *ptr )
{
    return free( ptr );
}

bool coverflow_init( menu_t *menu )
{
    int result;
    canvas_info_t canvas_info;
    int count = menu->global_size;
    int cur_index;
    unsigned int len;
    
    if ( s_coverflow != NULL )
    {
        //set current item
        cur_index = menu->global_offset + menu->current;
        result = pf_activeSlide( s_coverflow->lib_handle, cur_index );
        
        //set default slide
        for ( cur_index = 0; cur_index < count; cur_index ++ )
        {
            result = pf_setSlide( s_coverflow->lib_handle, cur_index, &s_coverflow->image_default );
        }        
        
        return true;
    }
    
    //inner init
    _inner_res_init();

    //get layer 0 info
    canvas_get_info( 0, &canvas_info );
    s_coverflow->rect = canvas_info.rect;

    //init coverflow image data
    s_coverflow->image_bg.width = s_coverflow->bmp_bg->xsize;
    s_coverflow->image_bg.height = s_coverflow->bmp_bg->ysize;
    s_coverflow->image_bg.bpp = (unsigned char)s_coverflow->bmp_bg->bytesperpixel;
    s_coverflow->image_bg.data = (unsigned char*)s_coverflow->bmp_bg->pdata;
    
    s_coverflow->image_default.width = s_coverflow->bmp_coverflow->xsize;
    s_coverflow->image_default.height = s_coverflow->bmp_coverflow->ysize;
    s_coverflow->image_default.bpp = (unsigned char)s_coverflow->bmp_coverflow->bytesperpixel;
    s_coverflow->image_default.data = (unsigned char*)s_coverflow->bmp_coverflow->pdata;
    
    //coverflow lib init
    result = pf_open( &s_coverflow->lib_handle );
    result = pf_resize( s_coverflow->lib_handle, canvas_info.rect.width, canvas_info.rect.height );
    result = pf_setSlideSize( s_coverflow->lib_handle, s_coverflow->pic_width, s_coverflow->pic_height );
    result = pf_setSlideCount( s_coverflow->lib_handle, count );

    //set default slide
    for ( cur_index = 0; cur_index < count; cur_index ++ )
    {
        result = pf_setSlide( s_coverflow->lib_handle, cur_index, &s_coverflow->image_default );
    }
    
    result = pf_setZoom( s_coverflow->lib_handle, 5 );
    
    cur_index = menu->global_offset + menu->current;
    result = pf_activeSlide( s_coverflow->lib_handle, cur_index );

    /* 设置生成canvas数据放置的地方 */
    result = pf_setCanvasBuffer( s_coverflow->lib_handle, (void*)canvas_info.pdata );

    /*必须在setcanvasBuffer之后调用*/
    result = pf_changeMode( s_coverflow->lib_handle, DISPLAY_NORMAL ); //DISPLAY_LOOP

    //set default bg
    result = pf_setCanvasImage( s_coverflow->lib_handle, &s_coverflow->image_bg );
    
    /* 两边显示三幅图 */
    result = pf_setSideSlideCount( s_coverflow->lib_handle, SIDE_SLIDE_NUM );
    
    /* slide间隔25像素 */
    result = pf_setSlideSpace( s_coverflow->lib_handle, SLIDE_INTERVAL );
    
    /* 设置帧数为 14 */
    result = pf_setSpeed( s_coverflow->lib_handle, 7 );

    /* 分配空间 */
    s_coverflow->image_total = count;
    len = sizeof(RImage_t) * (unsigned int)count;
    s_coverflow->image_list = malloc( len );
    if( s_coverflow->image_list == NULL )
    {
        print_err();
        return false;
    }
    else
    {
        memset( s_coverflow->image_list, 0, len );
    }
    
    return true;
}

bool coverflow_deinit( menu_t *menu )
{
    if ( s_coverflow == NULL )
    {
        return true;
    }
    
    /* 释放空间 */
    s_coverflow->image_total = 0;
    if( s_coverflow->image_list != NULL )
    {
        free(s_coverflow->image_list);
        s_coverflow->image_list = NULL;
    }
    
    //coverflow lib exit
    pf_close( s_coverflow->lib_handle );    

    //inner init
    _inner_res_deinit();

    return true;
}

bool coverflow_set_slide( menu_t *menu, int cur_index )
{
    menuitem_t *item;
    int offset = menu->global_offset + cur_index;
    RImage_t *image;

    item = menu_get_item( menu, cur_index );
    if ( ( item == NULL ) || ( item->bitmap == NULL ) )
    {
        image = &s_coverflow->image_default;
        
        if ( ( offset > 0 ) && ( offset < s_coverflow->image_total ) )
        {
            s_coverflow->image_list[offset] = s_coverflow->image_default;
            image = &s_coverflow->image_list[offset];
        }
    }
    else
    {
        image = &s_coverflow->image_list[offset];
        image->width = item->bitmap->xsize;
        image->height = item->bitmap->ysize;
        image->bpp = (unsigned char)item->bitmap->bytesperpixel;
        image->data = (unsigned char*)item->bitmap->pdata;
    }
        
    pf_setSlide( s_coverflow->lib_handle, offset, image );
    
    return true;
}

bool coverflow_next( menu_t *menu )
{
    int offset;
    
    if ( menu == NULL )
    {
        return false;
    }
    
    offset = menu->global_offset + menu->current;
    
    if ( offset == 0 )
    {
        pf_showHead( s_coverflow->lib_handle );
    }
    else
    {
        pf_showNext( s_coverflow->lib_handle );
    }
    
    return true;
}

bool coverflow_prev( menu_t *menu )
{
    int offset;
    
    if ( menu == NULL )
    {
        return false;
    }
    
    offset = menu->global_offset + menu->current;
    
    if ( offset == ( menu->global_size - 1 ) )
    {
        pf_showTail( s_coverflow->lib_handle );
    }
    else
    {
        pf_showPrevious( s_coverflow->lib_handle );
    }
    
    return true;
}

bool coverflow_paint( int hdc, menu_t *menu )
{
    menuitem_t *item;
    int x0, y0, x1, y1;
    
    if ( ( s_coverflow->is_hidden == true )
        || ( menu == NULL ) || ( menu->total <= 0 ) )
    {
        return false;
    }
    
    item = menu_get_cur_item( menu );

    //draw coverflow
    pf_render( s_coverflow->lib_handle );
        
    //draw name
    if ( item != NULL )
    {
        sty_draw_string_res_ext( hdc, s_coverflow->str_coverflow, item->text );
    }
    
    //set update region
    x0 = s_coverflow->rect.x;
    y0 = s_coverflow->rect.y;
    x1 = ( s_coverflow->rect.x + s_coverflow->rect.width ) - 1;
    y1 = ( s_coverflow->rect.y + s_coverflow->rect.height ) - 1;
    gui_set_update_region( x0, y0, x1, y1 );
    
    return true;
}

/*
 * 描述：先调用pf_render，然后调用pf_isOnAnimation判断动画是否结束。
         如果没有结束，继续pf_render直到动画完成
 */
bool coverflow_animation( int hdc, menu_t *menu )
{
    menuitem_t *item;
    
    if ( s_coverflow->is_hidden == true )
    {
        return false;
    }
    
    item = menu_get_cur_item( menu );

    //draw coverflow
    while( 1 )
    {        
        pf_render( s_coverflow->lib_handle );
        //printf("\n >>>>>>>> pf_render <<<<<<< \n");
        
        //draw name
        if ( item != NULL )
        {
            sty_draw_string_res_ext( hdc, s_coverflow->str_coverflow, item->text );
        }
    
        gui_framebuffer_update();
        sty_lcd_update();        

        if(pf_isOnAnimation( s_coverflow->lib_handle ) == 0)
        {
            break;
        }
    }

    return true;
}


int coverflow_get_slide_width( void )
{
    if ( s_coverflow != NULL )
    {
        return s_coverflow->pic_width;        
    }
    else
    {
        return 0;        
    }    
}

int coverflow_get_slide_height( void )
{
    if ( s_coverflow != NULL )
    {
        return s_coverflow->pic_height;        
    }
    else
    {
        return 0;        
    }    
}

bool coverflow_show( void )
{
    if ( s_coverflow != NULL )
    {
        s_coverflow->is_hidden = false;        
        return true;     
    }
    else
    {
        return false;        
    }    
}

bool coverflow_hide( void )
{
    if ( s_coverflow != NULL )
    {
        s_coverflow->is_hidden = true;        
        scene_list_paint();
        
        return true;     
    }
    else
    {
        return false;        
    }
}

