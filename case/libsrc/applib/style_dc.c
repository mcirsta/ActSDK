/********************************************************************************
 *                              USDK(1100)
 *                             Module: app common
 *                 Copyright(c) 2003-2009 Actions Semiconductor,
 *                            All Rights Reserved.
 * File : style_dc.c
 * History:
 *      <author>    <time>           <version >             <desc>
 *    songzhining    2009-2-25          v1.0               build this file
 ********************************************************************************/
/*!
* \file     style_dc.c
* \brief    style resource operation based on dc interface
* \author   songzhining
*
* \version 1.0
* \date  2009/02/25
*******************************************************************************/

#include "include_case.h"
#include "include_psp.h"

/* app资源文件句柄 */
static FILE* s_app_pic_res = NULL;
static FILE* s_app_str_res = NULL;

/* 字符编码 */
static gui_text_encode_t s_encoding = GUI_MBCS_ENC;

/* commonui风格文件和场景资源 */
static scene_t *s_com_scene = NULL;

/* app场景资源 */
static scene_t *s_app_scene  = NULL;

/* 字符串保存属性 */
static string_resource_t s_str_format1;
static string_resource_t s_str_format2;

/* 内部静态函数列表 */
static bool _set_string_format( int hdc, string_resource_t *string, string_resource_t *str_format );
static bool _restore_string_format( int hdc, string_resource_t *str_format );


/* res资源操作函数*/

/************************************************************************************/
/*!
 * \par  Description:
 *     打开公共图片资源
 * \param[in]    pic_res   图片资源全路径
 * \return       the result
 * \retval        true    设置成功
 * \retval        false    设置失败
 ************************************************************************************/
bool sty_open_common_pic_res( unsigned char *pic_res )
{
    int result;

    //初始化公用图片资源
    result = gui_set_common_ui_res( pic_res );

    if ( result < 0 )
    {
        return false;
    }
    else
    {
        return true;
    }
}

/************************************************************************************/
/*!
 * \par  Description:
 *     打开公共字符串资源
 * \param[in]    str_res   字符串资源全路径
 * \return       the result
 * \retval        true    设置成功
 * \retval        false    设置失败
 ************************************************************************************/
bool sty_open_common_str_res( unsigned char *str_res )
{
    int result;

    //初始化公用字符串资源
    result = gui_set_common_string_res( str_res );

    if ( result < 0 )
    {
        return false;
    }
    else
    {
        return true;
    }
}

/************************************************************************************/
/*!
 * \par  Description:
 *     关闭公共图片资源
 * \param[in]    None
 * \return       the result
 * \retval        true    设置成功
 * \retval        false    设置失败
 ************************************************************************************/
bool sty_close_common_pic_res( void )
{
    return true;
}

/************************************************************************************/
/*!
 * \par  Description:
 *     关闭公共字符串资源
 * \param[in]    None
 * \return       the result
 * \retval        true    设置成功
 * \retval        false    设置失败
 ************************************************************************************/
bool sty_close_common_str_res( void )
{
    return true;
}

/************************************************************************************/
/*!
 * \par  Description:
 *     打开应用私有图片资源
 * \param[in]    pic_res   图片资源全路径
 * \return       the result
 * \retval        true    设置成功
 * \retval        false    设置失败
 ************************************************************************************/
bool sty_open_app_pic_res( unsigned char *pic_res )
{
    if ( s_app_pic_res != NULL )
    {
        gui_close_resource_file( s_app_pic_res );
        s_app_pic_res = NULL;
    }
    
    //打开图片资源文件
    s_app_pic_res = gui_open_ui_res( pic_res );

    if ( s_app_pic_res == NULL )
    {
        return false;
    }
    else
    {
        return true;
    }
}

/************************************************************************************/
/*!
 * \par  Description:
 *     打开应用私有字符串资源
 * \param[in]    str_res   字符串资源全路径
 * \return       the result
 * \retval        true    设置成功
 * \retval        false    设置失败
 ************************************************************************************/
bool sty_open_app_str_res( unsigned char *str_res )
{
    if ( s_app_str_res != NULL )
    {
        gui_close_resource_file( s_app_str_res );
        s_app_str_res = NULL;
    }
    
    //打开字符串资源文件
    s_app_str_res = gui_open_string_res( str_res );

    if ( s_app_str_res == NULL )
    {
        return false;
    }
    else
    {
        return true;
    }
}

/************************************************************************************/
/*!
 * \par  Description:
 *     关闭应用私有图片资源
 * \param[in]    None
 * \return       the result
 * \retval        true    设置成功
 * \retval        false    设置失败
 ************************************************************************************/
bool sty_close_app_pic_res( void )
{
    //关闭图片资源文件
    if ( s_app_pic_res != NULL )
    {
        gui_close_resource_file( s_app_pic_res );
        s_app_pic_res = NULL;
    }

    return true;
}

/************************************************************************************/
/*!
 * \par  Description:
 *     关闭应用私有字符串资源
 * \param[in]    None
 * \return       the result
 * \retval        true    设置成功
 * \retval        false    设置失败
 ************************************************************************************/
bool sty_close_app_str_res( void )
{
    //关闭字符串资源文件
    if ( s_app_str_res != NULL )
    {
        gui_close_resource_file( s_app_str_res );
        s_app_str_res = NULL;
    }
    
    return true;
}

/* style资源操作函数*/

/************************************************************************************/
/*!
 * \par  Description:
 *     设置应用style场景资源
 * \param[in]    res   style资源
 * \return       the result
 * \retval        true    设置成功
 * \retval        false    设置失败
 ************************************************************************************/
bool sty_set_app_scene( scene_t* scene )
{
    s_app_scene = scene;
    return true;
}

/************************************************************************************/
/*!
 * \par  Description:
 *     获取应用style场景资源
 * \param[in]    None
 * \return       res   style资源指针,失败返回NULL
 ************************************************************************************/
scene_t* sty_get_app_scene( void )
{
    return s_app_scene;
}

/************************************************************************************/
/*!
 * \par  Description:
 *     设置公共style资源
 * \param[in]    res   style资源
 * \return       the result
 * \retval        true    设置成功
 * \retval        false    设置失败
 ************************************************************************************/
bool sty_set_common_scene( scene_t* scene )
{
    s_com_scene = scene;
    return true;
}

/************************************************************************************/
/*!
 * \par  Description:
 *     获取公共style资源
 * \param[in]    None
 * \return       res   style资源指针,失败返回NULL
 ************************************************************************************/
scene_t* sty_get_common_scene( void )
{
    return s_com_scene;
}

/* 语言编码函数*/

/************************************************************************************/
/*!
 * \par  Description:
 *     设置当前的字符语言编码
 * \param[in]    encoding    语言编码
 * \return       the result
 * \retval        true    设置成功
 * \retval        false    设置失败
 ************************************************************************************/
bool sty_set_text_encoding( gui_text_encode_t encoding )
{
    s_encoding = encoding;
    return true;
}

/************************************************************************************/
/*!
 * \par  Description:
 *     获取当前的字符语言编码
 * \param[in]    None
 * \return       encoding    语言编码
 ************************************************************************************/
gui_text_encode_t sty_get_text_encoding( void )
{
    return s_encoding;
}


/************************************************************************************/
/*!
 * \par  Description:
 *     设置style中的字符串显示属性(修改DC的属性)
 * \param[in]    hdc 绘图句柄
 * \param[in]    string style中的字符串资源
 * \return       the result
 * \retval        true    设置成功
 * \retval        false    设置失败
 * \par          exmaple code
 * \code
            //设置DC字符串显示格式
            sty_set_string_format( hdc, string );

            //绘制字符串(文本内容处理复杂的情况下使用)
            sty_draw_string_buf( hdc, buffer, &rect, string->text_align );

            //恢复DC字符串显示格式
            sty_restore_string_format( hdc );
 * \endcode
 ************************************************************************************/
bool sty_set_string_format( int hdc, string_resource_t *string )
{
    if ( ( string != NULL ) && ( true == string->visible ) )
    {
        //save & set dc format
        return _set_string_format( hdc, string, &s_str_format2 );
    }
    else
    {
        return false;
    }
}

/************************************************************************************/
/*!
 * \par  Description:
 *     恢复上次的字符串显示属性(修改DC的属性)
 * \param[in]    hdc 绘图句柄
 * \return       the result
 * \retval        true    设置成功
 * \retval        false    设置失败
 ************************************************************************************/
bool sty_restore_string_format( int hdc )
{
    return _restore_string_format( hdc, &s_str_format2 );
}


/* 字符串绘图函数*/
/************************************************************************************/
/*!
 * \par  Description:
 *     设置style中的字符串显示属性
 * \param[in]    hdc 绘图句柄
 * \param[in]    string style中的字符串资源
 * \param[out]    str_format 保存style中的字符串显示属性
 * \return       the result
 * \retval        true    设置成功
 * \retval        false    设置失败
 ************************************************************************************/
static bool _set_string_format( int hdc, string_resource_t *string, string_resource_t *str_format )
{
    if ( ( string != NULL ) && ( true == string->visible ) )
    {
        //save dc format
        str_format->mode = (unsigned char)gui_dc_get_text_mode( hdc );
        str_format->text_align = (unsigned char)gui_dc_get_text_align( hdc );
        str_format->foreground = (unsigned int)gui_dc_get_color( hdc );
        str_format->background = (unsigned int)gui_dc_get_background_color( hdc );
        str_format->font_height = (unsigned char)gui_dc_get_fontsize( hdc );

        //set dc format
        gui_dc_set_text_mode( hdc, (int)string->mode );
        gui_dc_set_text_align( hdc, (int)string->text_align );
        gui_dc_set_color( hdc, (int)string->foreground );
        gui_dc_set_background_color( hdc, (int)string->background );
        gui_dc_set_font_size( hdc, (int)string->font_height );

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
 *     恢复上次的字符串显示属性
 * \param[in]    hdc 绘图句柄
 * \param[in]    str_format 保存style中的字符串显示属性
 * \return       the result
 * \retval        true    设置成功
 * \retval        false    设置失败
 ************************************************************************************/
static bool _restore_string_format( int hdc, string_resource_t *str_format )
{
    //set dc format
    gui_dc_set_text_mode( hdc, (int)str_format->mode );
    gui_dc_set_text_align( hdc, (int)str_format->text_align );
    gui_dc_set_color( hdc, (int)str_format->foreground );
    gui_dc_set_background_color( hdc, (int)str_format->background );
    gui_dc_set_font_size( hdc, (int)str_format->font_height );

    return true;
}

/************************************************************************************/
/*!
 * \par  Description:
 *     根据指定宽度截断字符串
 * \param[in]    hdc 绘图句柄
 * \param[in]    src_str 源字符串
 * \param[in]    dst_str 目标字符串(截断字符串前一段的结果)
 * \param[in]    pixel_width 指定像素宽度
 * \return       the result
 * \retval       next_string 截断字符串的下一段指针
 * \par          exmaple code
 * \code
            char buf[512];
            char *cur_pstr;

            //截断字符串src_str, 分成两行显示
            cur_pstr = sty_truncate_string_by_width( hdc, src_str, buf, line_width );
            sty_draw_string_res_ext( hdc, str_lyric1, buf );
            sty_draw_string_res_ext( hdc, str_lyric2, cur_pstr );
 * \endcode
 ************************************************************************************/
char* sty_truncate_string_by_width( int hdc, char *src_str, char *dst_str, int pixel_width )
{
    int chardist, xdist = 0;
    unsigned char *str = src_str;
    int word_len;
    unsigned int char_tmp;

    switch ( s_encoding )
    {
    case GUI_MBCS_ENC:
        for (;;)
        {
            word_len = gui_get_word_length( str, (int)gui_get_language_mode() );
            if (word_len == 1)
            {
                chardist = gui_dc_get_char_dist_x( hdc, *str, s_encoding );
                xdist += chardist;
            }
            else if (word_len == 2)
            {
                chardist = gui_dc_get_char_dist_x( hdc, (unsigned short)*str | (((unsigned short)*(str+1))<<8), 
                                                    s_encoding );
                xdist += chardist;
            }
            else
            {
                word_len = 1;
                chardist = 0;
            }

            if (xdist > pixel_width)
            {
                break;
            }

            str += word_len;

            if (*str == '\0')
            {
                break;
            }
        }
        break;


    case GUI_UTF8_ENC:
        for (;;)
        {
            if ( ( *str & 0x80 ) == 0x0 )   // 1 byte
            {
                char_tmp = (unsigned int)*str;
                word_len = 1;
            }
            else if ( ( *str & 0xE0 ) == 0xC0 ) // 2 byte
            {
                char_tmp = (unsigned int)*str | (((unsigned int)*(str+1))<<8);
                word_len = 2;
            }
            else if ( ( *str & 0xF0 ) == 0xE0 ) // 3 byte
            {
                char_tmp = (unsigned int)*str | (((unsigned int)*(str+1))<<8) | (((unsigned int)*(str+2))<<16);
                word_len = 3;
            }
            else    // 4 byte
            {
                char_tmp = (unsigned int)*str | (((unsigned int)*(str+1))<<8) 
                            | (((unsigned int)*(str+2))<<16) | (((unsigned int)*(str+3))<<24);
                word_len = 4;
            }

            chardist = gui_dc_get_char_dist_x( hdc, char_tmp, s_encoding );
            xdist += chardist;

            if (xdist > pixel_width)
            {
                break;
            }

            str += word_len;

            if (*str == '\0')
            {
                break;
            }
        }
        break;

        
    case GUI_UNICODE_ENC:
        for (;;)
        {
            chardist = gui_dc_get_char_dist_x( hdc, (unsigned short)*str | (((unsigned short)*(str+1))<<8),
                                                s_encoding );
            xdist += chardist;

            if (xdist > pixel_width)
            {
                break;
            }

            str += 2;

            if (((unsigned short) * str | (((unsigned short) * (str + 1)) << 8)) == 0)
            {
                break;
            }
        }
        break;


    default:
        break;
    }
    
    if ( dst_str != NULL )
    {
        word_len = (int)((char*)str - src_str);
        memcpy( dst_str, src_str, (unsigned int)word_len );
        dst_str[(unsigned int)word_len] = '\0';
        dst_str[(unsigned int)word_len+1] = '\0';
    }

    return str;
}

/*超出屏幕的长度范围，则截断，并显示“...”*/
static void _disp_string_by_cut(int hdc, char *str, gui_rect_t* prect, int align, gui_text_encode_t encoding)
{
    int xdist = 0;
    int width = ( prect->x1 - prect->x0 ) + 1;
    int dotslen;
    char *pstr;
    char save[8];

    //check string width
    xdist = gui_dc_get_string_dist_x( hdc, str, encoding );
    if ( xdist <= width )
    {
        gui_dc_display_string_in_rect( hdc, str, prect, align, encoding );
        return;
    }

    //get new width
    dotslen = gui_dc_get_string_dist_x( hdc, "...", encoding );
    width = width - dotslen;

    //truncate string
    pstr = sty_truncate_string_by_width( hdc, str, NULL, width );
    if ( *pstr == '\0' )
    {
        gui_dc_display_string_in_rect( hdc, str, prect, align, encoding );
    }
    else
    {
        //save
        save[0] = *pstr;
        save[1] = *(pstr+1);
        *pstr = '\0';
        *(pstr+1) = '\0';
        width = gui_dc_get_string_dist_x( hdc, str, encoding );

        gui_dc_display_string_in_rect( hdc, str, prect, (align & 0x0c) | GUI_ALIGN_LEFT, encoding );

        prect->x0 += (short)width;
        gui_dc_display_string_in_rect( hdc, "...", prect, (align & 0x0c) | GUI_ALIGN_LEFT, encoding );

        prect->x0 -= (short)width;
        *pstr = save[0];
        *(pstr+1) = save[1];
    }
}

/************************************************************************************/
/*!
 * \par  Description:
 *     获取字符串
 * \param[in]    str_id 字符串ID
 * \return       成功返回字符串指针,失败返回NULL
 * \note        若要保存字符串, 由应用分配空间再调用strcpy复制数据
 * \par          exmaple code
 * \code
 *                unsigned char* str;
 *                char title[64];
 *                str = sty_get_string( READING );    //获取字符串
 *                strcpy( title, str );     //保存字符串
 * \endcode
 ************************************************************************************/
const char* sty_get_string( int str_id )
{
    gui_text_encode_t encode;
    int len = 0;

    return gui_get_string_by_id( s_app_str_res, str_id, &len, &encode );
}

/************************************************************************************/
/*!
 * \par  Description:
 *     绘制缓冲区中的字符串
 * \param[in]    hdc 绘图句柄
 * \param[in]    buf 字符串
 * \param[in]    rect   显示区域
 * \param[in]    align   对齐方式
 * \return       the result
 * \retval        true    设置成功
 * \retval        false    设置失败
 ************************************************************************************/
bool sty_draw_string_buf( int hdc, unsigned char *buf, sty_rect_t *rect, int align )
{
    gui_rect_t r;

    if ( buf != NULL )
    {
        r.x0 = rect->x;
        r.y0 = rect->y;
        r.x1 = ( rect->x + rect->width ) - 1;
        r.y1 = ( rect->y + rect->height ) - 1;

        //gui_dc_display_string_in_rect( hdc, buf, &r, align, s_encoding );
        _disp_string_by_cut( hdc, buf, &r, align, s_encoding );
    }
    else
    {
        return false;
    }

    return true;
}

/************************************************************************************/
/*!
 * \par  Description:
 *     绘制RES资源中的字符串
 * \param[in]    hdc 绘图句柄
 * \param[in]    str_id 字符串ID
 * \param[in]    rect   显示区域
 * \param[in]    align   对齐方式
 * \return       the result
 * \retval        true    设置成功
 * \retval        false    设置失败
 ************************************************************************************/
bool sty_draw_string_id( int hdc, int str_id, sty_rect_t *rect, int align )
{
    int len = 0;
    gui_text_encode_t encode;
    unsigned char* text = 0;
    bool ret = 0;

    if ( str_id != 0 )
    {
        text = ( unsigned char* )gui_get_string_by_id( s_app_str_res, str_id, &len, &encode );
        ret = sty_draw_string_buf( hdc, text, rect, align );
    }
    else
    {
        ret = false;
    }

    return ret;
}

/************************************************************************************/
/*!
 * \par  Description:
 *     绘制style中的字符串资源
 * \param[in]    hdc 绘图句柄
 * \param[in]    string style中的字符串资源
 * \return       the result
 * \retval        true    设置成功
 * \retval        false    设置失败
 ************************************************************************************/
bool sty_draw_string_res( int hdc, string_resource_t *string )
{
    return sty_draw_string_res_ext( hdc, string, NULL );
}

/************************************************************************************/
/*!
 * \par  Description:
 *     绘制style中的字符串资源(可由外部传入字符串内容，使用style中的坐标和风格)
 * \param[in]    hdc 绘图句柄
 * \param[in]    string style中的字符串资源
 * \param[in]    buf 字符串
 * \return       the result
 * \retval        true    设置成功
 * \retval        false    设置失败
 ************************************************************************************/
bool sty_draw_string_res_ext( int hdc, string_resource_t *string, unsigned char *buf )
{
    gui_text_encode_t encode;
    int len = 0;
    char* text = 0;
    gui_rect_t rect;
    unsigned char align = 0;

    if ( ( string != NULL ) && ( true == string->visible ) )
    {
        //set dc style
        _set_string_format( hdc, string, &s_str_format1 );

        if ( buf == NULL )
        {
            text = ( char* )gui_get_string_by_id( s_app_str_res, (int)string->str_id, &len, &encode );
        }
        else
        {
            text = buf;
        }

        rect.x0 = string->x;
        rect.y0 = string->y;
        rect.x1 = ( rect.x0 + string->width ) - 1;
        rect.y1 = ( rect.y0 + string->height ) - 1;

        align = string->text_align; /* 对齐方式 */

        if ( string->scroll == 0 )  /* 不需要滚动 */
        {
            //gui_dc_display_string_in_rect( hdc, text, &rect, align, s_encoding );
            _disp_string_by_cut( hdc, text, &rect, (int)align, s_encoding );
        }
        else    /* 需要滚动 */
        {
            gui_scroll_info_t scroll;
            gui_dc_set_scroll_string_in_rect ( hdc, text, &rect, (int)align, &scroll,
                        (int)string->direction, (int)string->pixel, string->space, s_encoding );
        }

        //restore dc style
        _restore_string_format( hdc, &s_str_format1 );
    }
    else
    {
        return false;
    }

    return true;
}

/************************************************************************************/
/*!
 * \par  Description:
 *    创建字符串的滚动信息
 * \param[in]    hdc 绘图句柄
 * \param[in]    string style中的字符串资源
 * \param[in]    buf 字符串(当buf为NULL时使用style中的内容)
 * \return       scroll 字符串滚动信息, 失败返回NULL
 * \par          exmaple code
 * \code
 *                scroll = sty_create_scroll_string( hdc, str_sel, NULL );   //创建滚动信息
 *                result = sty_draw_scroll_string( hdc, str_sel, scroll );   //绘制滚动字符串
  *               result =  sty_delete_scroll_string( scroll );   //删除滚动信息
 * \endcode
 ************************************************************************************/
gui_scroll_info_t * sty_create_scroll_string( int hdc, string_resource_t *string, char *buf )
{
    gui_scroll_info_t *scroll = NULL;
    int dist = 0;
    gui_text_encode_t encode;
    unsigned int len = 0;
    char* text = 0;
    char* buffer = 0;
    gui_rect_t rect;
    unsigned char align = 0;

    if ( ( string != NULL ) && ( true == string->visible ) && ( true == string->scroll ) )
    {
        //set dc style
        _set_string_format( hdc, string, &s_str_format1 );

        //get string
        if ( buf == NULL )
        {
            text = ( char* )gui_get_string_by_id( s_app_str_res, (int)string->str_id, &len, &encode );
        }
        else
        {
            text = buf;
            len = strlen( text );
        }

        //check string width
        dist = gui_dc_get_string_dist_x( hdc, text, s_encoding );    
        if ( dist <= string->width )
        {
            return NULL;
        }

        //malloc data
        scroll = (gui_scroll_info_t *)malloc( sizeof(gui_scroll_info_t) );
        if ( scroll == NULL )
        {
            print_err("malloc failed!");
            goto scroll_err;
        }
        else
        {
            memset( scroll, 0, sizeof(gui_scroll_info_t) );
        }

        //malloc text data
        buffer = ( char * )malloc( len + 2 );
        if ( buffer == NULL )
        {
            print_err("malloc failed!");
            goto text_err;
        }
        else
        {
            memset( buffer, 0, len + 2 );
            memcpy( buffer, text, len );
        }


        rect.x0 = string->x;
        rect.y0 = string->y;
        rect.x1 = ( rect.x0 + string->width ) - 1;
        rect.y1 = ( rect.y0 + string->height ) - 1;

        align = string->text_align; /* 对齐方式 */

        gui_dc_set_scroll_string_in_rect ( hdc, buffer, &rect, (int)align, scroll,
                        (int)string->direction, (int)string->pixel, string->space, s_encoding );

        //restore dc style
        _restore_string_format( hdc, &s_str_format1 );
    }
    else
    {
        return NULL;
    }

    return scroll;

    text_err:
    free( scroll );
        
    scroll_err:

    return NULL;
}

/************************************************************************************/
/*!
 * \par  Description:
 *     滚动绘制字符串
 * \param[in]    hdc 绘图句柄
 * \param[in]    string style中的字符串资源
 * \param[in]    scroll 字符串滚动信息
 * \return       the result
 * \retval        true    设置成功
 * \retval        false    设置失败
 ************************************************************************************/
bool sty_draw_scroll_string( int hdc, string_resource_t *string, gui_scroll_info_t *scroll )
{
    if ( ( string != NULL ) && ( scroll != NULL ) )
    {
        //set dc style
        _set_string_format( hdc, string, &s_str_format1 );

        gui_dc_scroll_string_in_rect( hdc, scroll, s_encoding );

        //restore dc style
        _restore_string_format( hdc, &s_str_format1 );
    }
    else
    {
        return false;
    }

    return true;
}

/************************************************************************************/
/*!
 * \par  Description:
 *     删除滚动字符串信息
 * \param[in]    scroll 字符串滚动信息
 * \return       the result
 * \retval        true    设置成功
 * \retval        false    设置失败
 ************************************************************************************/
bool sty_delete_scroll_string( gui_scroll_info_t *scroll )
{
    if ( scroll != NULL )
    {
        //free text buffer
        free( scroll->s_info.original_str);

        //free data
        free( scroll );
    }
    else
    {
        return false;
    }

    return true;
}


/* 图片绘图函数*/
/************************************************************************************/
/*!
 * \par  Description:
 *     绘制缓冲区中的图片
 * \param[in]    hdc 绘图句柄
 * \param[in]    buf 图片缓冲区
 * \param[in]    bpp 每象素字节数
 * \param[in]    rect   图片显示区域
 * \return       the result
 * \retval        true    设置成功
 * \retval        false    设置失败
 ************************************************************************************/
bool sty_draw_picture_buf( int hdc, unsigned char *buf, int bpp, sty_rect_t *rect )
{
    int result;

    if ( buf == NULL )
    {
        return false;
    }

    result = gui_dc_draw_bitmap_ext( hdc, buf, rect->x, rect->y, rect->width, rect->height, bpp );

    if ( result < 0 )
    {
        return false;
    }
    else
    {
        return true;
    }
}

/************************************************************************************/
/*!
 * \par  Description:
 *     绘制RES资源中的图片
 * \param[in]    hdc 绘图句柄
 * \param[in]    pic_id 图片ID
 * \param[in]    x   图片显示区域左上角x坐标
 * \param[in]    y   图片显示区域左上角y坐标
 * \return       the result
 * \retval        true    设置成功
 * \retval        false    设置失败
 ************************************************************************************/
bool sty_draw_picture_id( int hdc, int pic_id, int x, int y )
{
    int result;

    result = gui_dc_show_picture( hdc, s_app_pic_res, pic_id, x, y );

    if ( result < 0 )
    {
        return false;
    }
    else
    {
        return true;
    }
}

/************************************************************************************/
/*!
 * \par  Description:
 *     绘制style中的图片资源
 * \param[in]    hdc 绘图句柄
 * \param[in]    picture style中的图片资源
 * \return       the result
 * \retval        true    设置成功
 * \retval        false    设置失败
 ************************************************************************************/
bool sty_draw_picture_res( int hdc, picture_resource_t *picture )
{
    if ( ( picture != NULL ) && ( true == picture->visible ) )
    {
        return sty_draw_picture_id( hdc, (int)picture->pic_id, picture->x, picture->y );
    }
    else
    {
        return false;
    }
}

/************************************************************************************/
/*!
 * \par  Description:
 *     绘制style中的图片区域中一帧图片
 * \param[in]    hdc 绘图句柄
 * \param[in]    picregion style中的图片区域资源
 * \param[in]    frame 第几帧
 * \return       the result
 * \retval        true    设置成功
 * \retval        false    设置失败
 ************************************************************************************/
bool sty_draw_picregion_res( int hdc, picregion_resource_t *picregion, unsigned short frame )
{
    picregion_frame_t pic;
    int ret;

    if ( ( picregion != NULL ) && ( true == picregion->visible ) )
    {
        //get one frame
        ret = get_picregion_frame( picregion, frame, &pic );

        if ( ret < 0 )
        {
            return false;
        }
        else
        {
            return sty_draw_picture_id( hdc, (int)pic.pic_id, pic.x, pic.y );
        }
    }
    else
    {
        return false;
    }
}

/************************************************************************************/
/*!
 * \par  Description:
 *     绘制缓冲区图片的部分区域
 * \param[in]    hdc 绘图句柄
 * \param[in]    buf 图片缓冲区
 * \param[in]    bpp 每象素字节数
 * \param[in]    rect   图片显示区域
 * \param[in]    srect   图片数据区域
 * \return       the result
 * \retval        true    设置成功
 * \retval        false    设置失败
 ************************************************************************************/
bool sty_draw_sub_picture_buf( int hdc, unsigned char *buf, int bpp, sty_rect_t *rect, sty_rect_t *srect )
{
    int result;

    if ( ( buf == NULL ) || ( rect == NULL ) || ( srect == NULL ) )
    {
        return false;
    }

    result = gui_dc_draw_sub_bitmap_ext( hdc, buf,
                                    srect->x, srect->y, srect->width, srect->height,
                                    rect->x, rect->y, rect->width, rect->height, bpp );
    if ( result < 0 )
    {
        return false;
    }
    else
    {
        return true;
    }
}

/************************************************************************************/
/*!
 * \par  Description:
 *     绘制RES资源图片的部分区域
 * \param[in]    hdc 绘图句柄
 * \param[in]    pic_id 图片ID
 * \param[in]    x   图片显示区域左上角x坐标
 * \param[in]    y   图片显示区域左上角y坐标
 * \param[in]    srect   图片数据区域
 * \return       the result
 * \retval        true    设置成功
 * \retval        false    设置失败
 ************************************************************************************/
bool sty_draw_sub_picture_id( int hdc, int pic_id, int x, int y, sty_rect_t *srect )
{
    gui_rect_t rect;
    int result;

    if ( srect == NULL )
    {
        return false;
    }

    rect.x0 = srect->x;
    rect.y0 = srect->y;
    rect.x1 = ( srect->x + srect->width ) - 1;
    rect.y1 = ( srect->y + srect->height ) - 1;

    result = gui_dc_show_sub_picture( hdc, s_app_pic_res, pic_id, x, y, &rect );

    if ( result < 0 )
    {
        return false;
    }
    else
    {
        return true;
    }
}

/* 绘制bitmap接口 */

/************************************************************************************/
/*!
 * \par  Description:
 *     绘制bitmap位图
 * \param[in]    hdc 绘图句柄
 * \param[in]    bmp 位图指针
 * \param[in]    x   图片显示区域左上角x坐标
 * \param[in]    y   图片显示区域左上角y坐标
 * \return       the result
 * \retval        true    设置成功
 * \retval        false    设置失败
 ************************************************************************************/
bool sty_draw_bitmap( int hdc, gui_bitmap_info_t *bmp, int x, int y )
{
    int result;

    if ( bmp == NULL )
    {
        return false;
    }

    result = gui_dc_draw_bitmap_ext( hdc, (unsigned char*)bmp->pdata, x, y, 
                                    (int)bmp->xsize, (int)bmp->ysize, (int)bmp->bytesperpixel);

    if ( result < 0 )
    {
        return false;
    }
    else
    {
        return true;
    }
}

/************************************************************************************/
/*!
 * \par  Description:
 *     绘制bitmap位图部分区域
 * \param[in]    hdc 绘图句柄
 * \param[in]    bmp 位图指针
 * \param[in]    x   图片显示区域左上角x坐标
 * \param[in]    y   图片显示区域左上角y坐标
 * \param[in]    srect   图片数据区域
 * \return       the result
 * \retval        true    设置成功
 * \retval        false    设置失败
 ************************************************************************************/
bool sty_draw_sub_bitmap( int hdc, gui_bitmap_info_t *bmp, int x, int y, sty_rect_t *srect )
{
    int result;

    if ( ( bmp == NULL ) || ( srect == NULL ) )
    {
        return false;
    }

    result = gui_dc_draw_sub_bitmap_ext( hdc, (unsigned char*)bmp->pdata, srect->x, srect->y, 
                                srect->width, srect->height,
                                x, y, (int)bmp->xsize, (int)bmp->ysize, (int)bmp->bytesperpixel );

    if ( result < 0 )
    {
        return false;
    }
    else
    {
        return true;
    }
}


/* 图片缓存接口 */

/************************************************************************************/
/*!
 * \par  Description:
 *     载入RES图片资源并创建bitmap位图
 * \param[in]    pic_id 图片ID
 * \return       成功返回bitmap位图指针, 失败返回NULL
 * \par          exmaple code
 * \code
 *                bmp = sty_create_bitmap( BG );    //载入RES图片资源到bitmap
 *                result = sty_draw_bitmap( hdc, bmp, 0, 0 );   //绘制bitmap
  *               result =  sty_delete_bitmap( bmp );   //删除bitmap
 * \endcode
 ************************************************************************************/
gui_bitmap_info_t* sty_create_bitmap( int pic_id )
{
    gui_bitmap_info_t *bmp;
    int size;
    int result;

    bmp = malloc( sizeof( gui_bitmap_info_t ) );

    if ( bmp != NULL )
    {
        //get metrics
        result = gui_bitmap_metrics( s_app_pic_res, pic_id, bmp );
        if ( result < 0 )
        {
            print_err("get bmp metrics error!\n");
            goto metrics_err;
        }

        //malloc bmp buffer
        size = bmp->xsize * bmp->ysize * bmp->bytesperpixel;
        bmp->pdata = malloc( (unsigned int)size );
        if ( bmp->pdata == NULL )
        {
            print_err("malloc error!\n");
            goto pdata_err;
        }

        //load res into bmp buffer
        result = gui_load_bitmap_into_buffer( s_app_pic_res, pic_id, (unsigned char*)bmp->pdata, size, bmp );
        if ( result < 0 )
        {
            print_err("load bmp error!\n");
            goto load_err;
        }

        return bmp;
    }
    else
    {
        print_err("malloc error!\n");
        goto bmp_err;
    }

    load_err:
    free( (unsigned char*)bmp->pdata );
    
    pdata_err:

    metrics_err:
    free( bmp );
    
    bmp_err:
        
    return NULL;
}

/************************************************************************************/
/*!
 * \par  Description:
 *     载入RES图片资源部分区域并创建bitmap位图
 * \param[in]    pic_id 图片ID
 * \param[in]    rect   图片区域
 * \return       成功返回bitmap位图指针, 失败返回NULL
 ************************************************************************************/
gui_bitmap_info_t* sty_create_sub_bitmap( int pic_id, sty_rect_t *rect )
{
    gui_bitmap_info_t *bmp;
    int size;
    int result;
    gui_bitmap_info_t info;
    gui_rect_t r;

    bmp = malloc( sizeof( gui_bitmap_info_t ) );

    if ( bmp != NULL )
    {
        //get metrics
        result = gui_bitmap_metrics( s_app_pic_res, pic_id, bmp );
        if ( result < 0 )
        {
            print_err("get bmp metrics error!\n");
            goto metrics_err;
        }

        //malloc bmp buffer
        bmp->xsize = (unsigned short)rect->width;
        bmp->ysize = (unsigned short)rect->height;
        size = bmp->xsize * bmp->ysize * bmp->bytesperpixel;
        bmp->pdata = malloc( (unsigned int)size );
        if ( bmp->pdata == NULL )
        {
            print_err("malloc error!\n");
            goto pdata_err;
        }

        //load res into bmp buffer
        r.x0 = rect->x;
        r.y0 = rect->y;
        r.x1 = ( rect->x + rect->width ) - 1;
        r.y1 = ( rect->y + rect->height ) - 1;

        result = gui_load_sub_bitmap_into_buffer( s_app_pic_res, pic_id, (unsigned char*)bmp->pdata, size, &info, &r );
        if ( result < 0 )
        {
            print_err("load sub bmp error!\n");
            goto load_err;
        }

        return bmp;
    }
    else
    {
        print_err("malloc error!\n");
        goto bmp_err;
    }
    
    load_err:
    free( (unsigned char*)bmp->pdata );
    
    pdata_err:

    metrics_err:
    free( bmp );
    
    bmp_err:
        
    return NULL;
}

/************************************************************************************/
/*!
 * \par  Description:
 *     删除bitmap位图数据
 * \param[in]    bmp 位图指针
 * \return       the result
 * \retval        true    设置成功
 * \retval        false    设置失败
 ************************************************************************************/
bool sty_delete_bitmap( gui_bitmap_info_t *bmp )
{
    if ( bmp != NULL )
    {
        if ( bmp->pdata != NULL )
        {
            free( (unsigned char*)bmp->pdata );
        }

        free( bmp );

        return true;
    }
    else
    {
        return false;
    }
}

/* 背景图保存和获取接口 */

/************************************************************************************/
/*!
 * \par  Description:
 *     保存bitmap位图数据
 * \param[in]    path 位图保存路径
 * \param[in]    bmp 位图指针
 * \return       the result
 * \retval        true    设置成功
 * \retval        false    设置失败
  * \par          exmaple code
 * \code
 *                bmp = sty_create_bitmap( BG );    //载入RES图片资源到bitmap
 *                result = sty_save_bitmap( "/mnt/diska/apps/bg.bin", bmp );    //保存bitmap文件
  *               result =  sty_delete_bitmap( bmp );   //删除bitmap
 * \endcode
 ************************************************************************************/
bool sty_save_bitmap( unsigned char *path, gui_bitmap_info_t *bmp )
{
    int file;
    int size;
    int result;

    // param check
    if ( ( path == NULL ) || ( bmp == NULL ) )
    {
        print_err( "param is NULL!" );
        return false;
    }

    // open file
    file = open( path, O_CREAT|O_WRONLY );
    if ( file < 0 )
    {
        print_err( "open file %s failed!", path );
        return false;
    }

    // write head
    result = write( file, bmp, sizeof( gui_bitmap_info_t ) );
    if ( (unsigned int)result != sizeof( gui_bitmap_info_t ) )
    {
        close( file );
        print_err( "write head failed! result: %d", result );
        return false;
    }

    // write body
    size = bmp->xsize * bmp->ysize * bmp->bytesperpixel;
    result = write( file, bmp->pdata, (unsigned int)size );
    if ( result != size )
    {
        close( file );
        print_err( "write body failed! result: %d", result );
        return false;
    }

    // close file
    close( file );

    return true;
}

/************************************************************************************/
/*!
 * \par  Description:
 *     恢复bitmap位图数据
 * \param[in]    path 位图保存路径
 * \return       成功返回bitmap位图指针, 失败返回NULL
 * \par          exmaple code
 * \code
 *                bmp = sty_restore_bitmap( "/mnt/diska/apps/bg.bin" );    //读取bitmap文件
 *                result = sty_draw_bitmap( hdc, bmp, 0, 0 );   //绘制bitmap
  *               result =  sty_delete_bitmap( bmp );   //删除bitmap
 * \endcode
 ************************************************************************************/
gui_bitmap_info_t* sty_restore_bitmap( unsigned char *path )
{
    int file;
    int size;
    int result;
    gui_bitmap_info_t *bmp;

    // param check
    if ( path == NULL )
    {
        print_err( "param is NULL!" );
        goto path_err;
    }

    // malloc bitmap
    bmp = malloc( sizeof( gui_bitmap_info_t ) );
    if ( bmp == NULL )
    {
        print_err("malloc error!\n");
        goto bmp_err;
    }

    // open file
    file = open( path, O_RDONLY );
    if ( file < 0 )
    {
        print_err( "open file %s failed!", path );
        goto open_err;
    }

    // read head
    result = read( file, bmp, sizeof( gui_bitmap_info_t ) );
    if ( (unsigned int)result != sizeof( gui_bitmap_info_t ) )
    {
        print_err( "read head failed! result: %d", result );
        goto head_err;
    }

    // malloc bitmap data
    size = bmp->xsize * bmp->ysize * bmp->bytesperpixel;
    if ( size > (2*1024*1024) )
    {
        print_err("size error!\n");
        goto pdata_err;
    }
    bmp->pdata = malloc( (unsigned int)size );
    if ( bmp->pdata == NULL )
    {
        print_err("malloc error!\n");
        goto pdata_err;
    }

    // read body
    result = read( file, (unsigned char*)bmp->pdata, (unsigned int)size );
    if ( result != size )
    {
        print_err( "read body failed! result: %d", result );
        goto body_err;
    }

    // close file
    close( file );

    return bmp;

    body_err:
    free( (unsigned char*)bmp->pdata );
        
    pdata_err:
        
    head_err:
    close( file );
    
    open_err:
    free( bmp );
    
    bmp_err:
    path_err:

    return NULL;
}

/* 获取视图角度*/
/************************************************************************************/
/*!
 * \par  Description:
 *     获取视图角度
 * \param[in]   None
 * \return       angle 屏幕角度(0/90/180/270)
 * \retval        true    设置成功
 * \retval        false    设置失败
 ************************************************************************************/
rotation_e sty_get_view_rotation( void )
{
    rotation_e angle;
    char cfg_screen_mode[128] = {0};
    msg_apps_t msg;
    msg_reply_t reply = {0};
    struct timespec timeout;

    /* 读取配置(Auto/Horizontal/Vertical) 
     * Auto: get_gsensor_rotation()
     * Horizontal: ROTATION_0
     * Vertical: ROTATION_90
     * */

    timeout.tv_sec = 1;
    timeout.tv_nsec = 0;

    angle = ROTATION_90;

    if(get_config(CFG_SCREEN_MODE, cfg_screen_mode, sizeof(cfg_screen_mode)) < 0)
    {
        print_err("read CFG_SCREEN_MODE failed, use \"Vertical\" as default");
        strcpy(cfg_screen_mode, "Vertical");
    }

    if(strcmp(cfg_screen_mode, "Auto") == 0)
    {
        /* G-sensor自动旋转 */
        msg.type = MSG_GET_GSENSOR_ROTATION_SYNC;
        if(send_sync_msg("msg_serv.app", &msg, &reply, &timeout) == true)
        {
            if(reply.type == MSG_REPLY_SUCCESS)
            {
                memcpy(&angle, reply.content, sizeof(angle));
            }
        }
    }
    else if(strcmp(cfg_screen_mode, "Horizontal") == 0)
    {
        /* 横屏 */
        angle = ROTATION_0;
    }
    else
    {
        /* 竖屏 */
        angle = ROTATION_90;
    }

    print_info("get view rotation:%d", angle);

    return angle;
}

/************************************************************************************/
/*!
 * \par  Description:
 *     设置当前屏幕方向
 * \param[in]    angle 屏幕角度(0/90/180/270)
 * \return       the result
 * \retval        true    设置成功
 * \retval        false    设置失败
 ************************************************************************************/
bool sty_set_screen_rotation( rotation_e angle )
{
    int fd;
    fb_info_t fb_info = {0};
    int ret;

    fd = open(DEVICE_FB, O_RDWR);
    if (fd < 0)
    {
        print_err("open %s failed!", DEVICE_FB);
        return false;
    }

    //设置横竖屏方向
    ret = ioctl( fd, FBIOSET_SCREEN_ROTATE, angle);
    if ( ret < 0 )
    {
        print_err("ioctl device fb failed! ret=%d\n", ret);
        close( fd );
        return false;
    }

    //获取屏幕参数
    ret = ioctl( fd, FBIOGET_FB_PARAM, (int)&fb_info );
    if ( ret < 0 )
    {
        print_err("ioctl device fb failed! ret=%d\n", ret);
        close( fd );
        return false;
    }

    close( fd );

    //设置GUI宽高
    gui_set_dimension( fb_info.xres, fb_info.yres );

    return true;
}

/* 获取屏幕方向 */
/************************************************************************************/
/*!
 * \par  Description:
 *     获取当前屏幕方向
 * \param[in]   none
 * \return       angle 屏幕角度(0/90/180/270)
 ************************************************************************************/
rotation_e sty_get_screen_rotation( void )
{
    int fd;
    rotation_e angle = ROTATION_90;
    int ret;

    fd = open(DEVICE_FB, O_RDWR);
    if (fd < 0)
    {
        print_err("open %s failed!", DEVICE_FB);
        return angle;
    }

    //获取横竖屏方向
    ret = ioctl( fd, FBIOGET_SCREEN_ROTATE, (unsigned int)&angle );
    if ( ret < 0 )
    {
        print_err("ioctl device fb failed! ret=%d\n", ret);
        close( fd );
        return angle;
    }

    close( fd );

    return angle;
}

/************************************************************************************/
/*!
 * \par  Description:
 *     将framebuffer数据刷新到LCD上
 * \param[in]    None
 * \return       the result
 * \retval        true    设置成功
 * \retval        false    设置失败
 ************************************************************************************/
bool sty_lcd_update( void )
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

    //update fb
    ret = sys_ioctl( (unsigned int)fd, FBIOSET_UPDATE_FB, (int)0 );
    if ( ret < 0 )
    {
        print_err("ioctl device fb failed! ret=%d\n", ret);
        sys_close( (unsigned int)fd );
        return false;
    }

    sys_close( (unsigned int)fd );

    return true;
}

