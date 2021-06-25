/********************************************************************************
 *                              USDK(1100)
 *                             Module: app common
 *                 Copyright(c) 2003-2009 Actions Semiconductor,
 *                            All Rights Reserved. 
 * File : style_dc.h
 * History:         
 *      <author>    <time>           <version >             <desc>
 *    songzhining    2009-2-25          v1.0               build this file 
 ********************************************************************************/
/*!
* \file     style_dc.h
* \brief    style resource operation based on dc interface
* \author   songzhining
*               
* \version 1.0
* \date  2009/02/25
*******************************************************************************/

#ifndef __STYLE_DC_H__
#define __STYLE_DC_H__


#include <style.h>

/*! 
 * \defgroup   applib_style_dc   风格相关DC绘图接口 
 */
 
#define RES_NULL 0

/*!
 *  \brief
 *  画布区域
 */ 
typedef struct
{
    /*! 左上角x坐标*/
    short x;
    
    /*! 左上角y坐标*/
    short y;
    
    /*! 宽度*/
    short width;
    
    /*! 高度*/
    short height;
    
}sty_rect_t;

/* res资源操作函数*/
/************************************************************************************/
/*!
 * \par  Description:
 *     打开公共图片资源
 * \ingroup applib_style_dc
 * \param[in]    pic_res   图片资源全路径
 * \return       the result
 * \retval        true    设置成功
 * \retval        false    设置失败
 ************************************************************************************/
bool sty_open_common_pic_res( unsigned char *pic_res );

/************************************************************************************/
/*!
 * \par  Description:
 *     打开公共字符串资源
 * \ingroup applib_style_dc
 * \param[in]    str_res   字符串资源全路径
 * \return       the result
 * \retval        true    设置成功
 * \retval        false    设置失败
 ************************************************************************************/
bool sty_open_common_str_res( unsigned char *str_res );

/************************************************************************************/
/*!
 * \par  Description:
 *     关闭公共图片资源
 * \ingroup applib_style_dc
 * \return       the result
 * \retval        true    设置成功
 * \retval        false    设置失败
 ************************************************************************************/
bool sty_close_common_pic_res( void );

/************************************************************************************/
/*!
 * \par  Description:
 *     关闭公共字符串资源
 * \ingroup applib_style_dc
 * \return       the result
 * \retval        true    设置成功
 * \retval        false    设置失败
 ************************************************************************************/
bool sty_close_common_str_res( void );

/************************************************************************************/
/*!
 * \par  Description:
 *     打开应用私有图片资源
 * \ingroup applib_style_dc
 * \param[in]    pic_res   图片资源全路径
 * \return       the result
 * \retval        true    设置成功
 * \retval        false    设置失败
 ************************************************************************************/
bool sty_open_app_pic_res( unsigned char *pic_res );

/************************************************************************************/
/*!
 * \par  Description:
 *     打开应用私有字符串资源
 * \ingroup applib_style_dc
 * \param[in]    str_res   字符串资源全路径
 * \return       the result
 * \retval        true    设置成功
 * \retval        false    设置失败
 ************************************************************************************/
bool sty_open_app_str_res( unsigned char *str_res );

/************************************************************************************/
/*!
 * \par  Description:
 *     关闭应用私有图片资源
 * \ingroup applib_style_dc
 * \return       the result
 * \retval        true    设置成功
 * \retval        false    设置失败
 ************************************************************************************/
bool sty_close_app_pic_res( void );

/************************************************************************************/
/*!
 * \par  Description:
 *     关闭应用私有字符串资源
 * \ingroup applib_style_dc
 * \return       the result
 * \retval        true    设置成功
 * \retval        false    设置失败
 ************************************************************************************/
bool sty_close_app_str_res( void );


/* style资源操作函数*/
/************************************************************************************/
/*!
 * \par  Description:
 *     设置应用style场景资源
 * \ingroup applib_style_dc
 * \param[in]     scene   style场景指针
 * \return       the result
 * \retval        true    设置成功
 * \retval        false    设置失败
 ************************************************************************************/
bool sty_set_app_scene( scene_t* scene );

/************************************************************************************/
/*!
 * \par  Description:
 *     获取应用style场景资源
 * \ingroup applib_style_dc
 * \return       res   style资源指针,失败返回NULL
 ************************************************************************************/
scene_t* sty_get_app_scene( void );

/************************************************************************************/
/*!
 * \par  Description:
 *     设置公共style资源
 * \ingroup applib_style_dc
 * \param[in]     scene   style场景指针
 * \return        the result
 * \retval        true    设置成功
 * \retval        false    设置失败
 ************************************************************************************/
bool sty_set_common_scene( scene_t* scene );

/************************************************************************************/
/*!
 * \par  Description:
 *     获取公共style资源
 * \ingroup applib_style_dc
 * \return       res   style资源指针,失败返回NULL
 ************************************************************************************/
scene_t* sty_get_common_scene( void );


/* 语言编码函数*/
/************************************************************************************/
/*!
 * \par  Description:
 *     设置当前的字符语言编码
 * \ingroup applib_style_dc
 * \param[in]    encoding    语言编码
 * \return       the result
 * \retval        true    设置成功
 * \retval        false    设置失败
 ************************************************************************************/
bool sty_set_text_encoding( gui_text_encode_t encoding );

/************************************************************************************/
/*!
 * \par  Description:
 *     获取当前的字符语言编码
 * \ingroup applib_style_dc
 * \return       encoding    语言编码
 ************************************************************************************/
gui_text_encode_t sty_get_text_encoding( void );


/* 字符串绘图函数*/
/************************************************************************************/
/*!
 * \par  Description:
 *     获取字符串内容
 * \ingroup applib_style_dc
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
const char* sty_get_string( int str_id );

/************************************************************************************/
/*!
 * \par  Description:
 *     根据指定宽度截断字符串
 * \ingroup applib_style_dc
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
char* sty_truncate_string_by_width( int hdc, char *src_str, char *dst_str, int pixel_width );

/************************************************************************************/
/*!
 * \par  Description:
 *     设置style中的字符串显示属性(修改DC的属性)
 * \ingroup applib_style_dc
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
bool sty_set_string_format( int hdc, string_resource_t *string );

/************************************************************************************/
/*!
 * \par  Description:
 *     恢复上次的字符串显示属性(修改DC的属性)
 * \ingroup applib_style_dc
 * \param[in]    hdc 绘图句柄
 * \return       the result
 * \retval        true    设置成功
 * \retval        false    设置失败
 ************************************************************************************/
bool sty_restore_string_format( int hdc );

/************************************************************************************/
/*!
 * \par  Description:
 *     绘制缓冲区中的字符串
 * \ingroup applib_style_dc
 * \param[in]    hdc 绘图句柄
 * \param[in]    buf 字符串
 * \param[in]    rect   显示区域
 * \param[in]    align   对齐方式
 * \return       the result
 * \retval        true    设置成功
 * \retval        false    设置失败
 ************************************************************************************/
bool sty_draw_string_buf( int hdc, unsigned char *buf, sty_rect_t *rect, int align );

/************************************************************************************/
/*!
 * \par  Description:
 *     绘制RES资源中的字符串
 * \ingroup applib_style_dc
 * \param[in]    hdc 绘图句柄
 * \param[in]    str_id 字符串ID
 * \param[in]    rect   显示区域
 * \param[in]    align   对齐方式
 * \return       the result
 * \retval        true    设置成功
 * \retval        false    设置失败
 ************************************************************************************/
bool sty_draw_string_id( int hdc, int str_id, sty_rect_t *rect, int align );

/************************************************************************************/
/*!
 * \par  Description:
 *     绘制style中的字符串资源
 * \ingroup applib_style_dc
 * \param[in]    hdc 绘图句柄
 * \param[in]    string style中的字符串资源
 * \return       the result
 * \retval        true    设置成功
 * \retval        false    设置失败
 ************************************************************************************/
bool sty_draw_string_res( int hdc, string_resource_t *string );

/************************************************************************************/
/*!
 * \par  Description:
 *     绘制style中的字符串资源(可由外部传入字符串内容，使用style中的坐标和风格)
 * \ingroup applib_style_dc
 * \param[in]    hdc 绘图句柄
 * \param[in]    string style中的字符串资源
 * \param[in]    buf 字符串
 * \return       the result
 * \retval        true    设置成功
 * \retval        false    设置失败
 ************************************************************************************/
bool sty_draw_string_res_ext( int hdc, string_resource_t *string, unsigned char *buf );


/* 滚动字符串操作函数*/
/************************************************************************************/
/*!
 * \par  Description:
 *    创建字符串的滚动信息
 * \ingroup applib_style_dc
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
gui_scroll_info_t * sty_create_scroll_string( int hdc, string_resource_t *string, char *buf );

/************************************************************************************/
/*!
 * \par  Description:
 *     滚动绘制字符串
 * \ingroup applib_style_dc
 * \param[in]    hdc 绘图句柄
 * \param[in]    string style中的字符串资源
 * \param[in]    scroll 字符串滚动信息
 * \return       the result
 * \retval        true    设置成功
 * \retval        false    设置失败
 ************************************************************************************/
bool sty_draw_scroll_string( int hdc, string_resource_t *string, gui_scroll_info_t *scroll );

/************************************************************************************/
/*!
 * \par  Description:
 *     删除滚动字符串信息
 * \ingroup applib_style_dc
 * \param[in]    scroll 字符串滚动信息
 * \return       the result
 * \retval        true    设置成功
 * \retval        false    设置失败
 ************************************************************************************/
bool sty_delete_scroll_string( gui_scroll_info_t *scroll );


/* 图片绘图函数*/
/************************************************************************************/
/*!
 * \par  Description:
 *     绘制缓冲区中的图片
 * \ingroup applib_style_dc
 * \param[in]    hdc 绘图句柄
 * \param[in]    buf 图片缓冲区
 * \param[in]    bpp 每象素字节数
 * \param[in]    rect   图片显示区域
 * \return       the result
 * \retval        true    设置成功
 * \retval        false    设置失败
 ************************************************************************************/
bool sty_draw_picture_buf( int hdc, unsigned char *buf, int bpp, sty_rect_t *rect );

/************************************************************************************/
/*!
 * \par  Description:
 *     绘制RES资源中的图片
 * \ingroup applib_style_dc
 * \param[in]    hdc 绘图句柄
 * \param[in]    pic_id 图片ID
 * \param[in]    x   图片显示区域左上角x坐标
 * \param[in]    y   图片显示区域左上角y坐标
 * \return       the result
 * \retval        true    设置成功
 * \retval        false    设置失败
 ************************************************************************************/
bool sty_draw_picture_id( int hdc, int pic_id, int x, int y );

/************************************************************************************/
/*!
 * \par  Description:
 *     绘制style中的图片资源
 * \ingroup applib_style_dc
 * \param[in]    hdc 绘图句柄
 * \param[in]    picture style中的图片资源
 * \return       the result
 * \retval        true    设置成功
 * \retval        false    设置失败
 ************************************************************************************/
bool sty_draw_picture_res( int hdc, picture_resource_t *picture );

/************************************************************************************/
/*!
 * \par  Description:
 *     绘制style中的图片区域中一帧图片
 * \ingroup applib_style_dc
 * \param[in]    hdc 绘图句柄
 * \param[in]    picregion style中的图片区域资源
 * \param[in]    frame     style中的图片区域指定帧号
 * \return       the result
 * \retval       true    设置成功
 * \retval       false    设置失败
 ************************************************************************************/
bool sty_draw_picregion_res( int hdc, picregion_resource_t *picregion, unsigned short frame );

/************************************************************************************/
/*!
 * \par  Description:
 *     绘制缓冲区图片的部分区域
 * \ingroup applib_style_dc
 * \param[in]    hdc 绘图句柄
 * \param[in]    buf 图片缓冲区
 * \param[in]    bpp 每象素字节数
 * \param[in]    rect   图片显示区域
 * \param[in]    srect   图片数据区域
 * \return       the result
 * \retval        true    设置成功
 * \retval        false    设置失败
 ************************************************************************************/
bool sty_draw_sub_picture_buf( int hdc, unsigned char *buf, int bpp, sty_rect_t *rect, sty_rect_t *srect );

/************************************************************************************/
/*!
 * \par  Description:
 *     绘制RES资源图片的部分区域
 * \ingroup applib_style_dc
 * \param[in]    hdc 绘图句柄
 * \param[in]    pic_id 图片ID
 * \param[in]    x   图片显示区域左上角x坐标
 * \param[in]    y   图片显示区域左上角y坐标
 * \param[in]    srect   图片数据区域
 * \return       the result
 * \retval        true    设置成功
 * \retval        false    设置失败
 ************************************************************************************/
bool sty_draw_sub_picture_id( int hdc, int pic_id, int x, int y, sty_rect_t *srect );


/* 绘制bitmap接口 */

/************************************************************************************/
/*!
 * \par  Description:
 *     绘制bitmap位图
 * \ingroup applib_style_dc
 * \param[in]    hdc 绘图句柄
 * \param[in]    bmp 位图指针
 * \param[in]    x   图片显示区域左上角x坐标
 * \param[in]    y   图片显示区域左上角y坐标
 * \return       the result
 * \retval        true    设置成功
 * \retval        false    设置失败
 ************************************************************************************/
bool sty_draw_bitmap( int hdc, gui_bitmap_info_t *bmp, int x, int y );

/************************************************************************************/
/*!
 * \par  Description:
 *     绘制bitmap位图部分区域
 * \ingroup applib_style_dc
 * \param[in]    hdc 绘图句柄
 * \param[in]    bmp 位图指针
 * \param[in]    x   图片显示区域左上角x坐标
 * \param[in]    y   图片显示区域左上角y坐标
 * \param[in]    srect   图片数据区域
 * \return       the result
 * \retval        true    设置成功
 * \retval        false    设置失败
 ************************************************************************************/
bool sty_draw_sub_bitmap( int hdc, gui_bitmap_info_t *bmp, int x, int y, sty_rect_t *srect );


/* 图片缓存接口 */

/************************************************************************************/
/*!
 * \par  Description:
 *     载入RES图片资源并创建bitmap位图
 * \ingroup applib_style_dc
 * \param[in]    pic_id 图片ID
 * \return       成功返回bitmap位图指针, 失败返回NULL 
 * \par          exmaple code
 * \code
 *                bmp = sty_create_bitmap( BG );	//载入RES图片资源到bitmap
 *                result = sty_draw_bitmap( hdc, bmp, 0, 0 );   //绘制bitmap
  *               result =  sty_delete_bitmap( bmp );   //删除bitmap
 * \endcode
 ************************************************************************************/
gui_bitmap_info_t* sty_create_bitmap( int pic_id );

/************************************************************************************/
/*!
 * \par  Description:
 *     载入RES图片资源部分区域并创建bitmap位图
 * \ingroup applib_style_dc
 * \param[in]    pic_id 图片ID
 * \param[in]    rect   图片区域
 * \return       成功返回bitmap位图指针, 失败返回NULL
 ************************************************************************************/
gui_bitmap_info_t* sty_create_sub_bitmap( int pic_id, sty_rect_t *rect );

/************************************************************************************/
/*!
 * \par  Description:
 *     删除bitmap位图数据
 * \ingroup applib_style_dc
 * \param[in]    bmp 位图指针
 * \return       the result
 * \retval        true    设置成功
 * \retval        false    设置失败
 ************************************************************************************/
bool sty_delete_bitmap( gui_bitmap_info_t *bmp );

/* 背景图保存和获取接口 */
/************************************************************************************/
/*!
 * \par  Description:
 *     保存bitmap位图数据
 * \ingroup applib_style_dc
 * \param[in]    path 位图保存路径
 * \param[in]    bmp 位图指针
 * \return       the result
 * \retval        true    设置成功
 * \retval        false    设置失败
  * \par          exmaple code
 * \code
 *                bmp = sty_create_bitmap( BG );	//载入RES图片资源到bitmap
 *                result = sty_save_bitmap( "/mnt/diska/apps/bg.bin", bmp );	//保存bitmap文件
  *               result =  sty_delete_bitmap( bmp );   //删除bitmap
 * \endcode
 ************************************************************************************/
bool sty_save_bitmap( unsigned char *path, gui_bitmap_info_t *bmp );

/************************************************************************************/
/*!
 * \par  Description:
 *     恢复bitmap位图数据
 * \ingroup applib_style_dc
 * \param[in]    path 位图保存路径
 * \return       成功返回bitmap位图指针, 失败返回NULL 
 * \par          exmaple code
 * \code
 *                bmp = sty_restore_bitmap( "/mnt/diska/apps/bg.bin" );	//读取bitmap文件
 *                result = sty_draw_bitmap( hdc, bmp, 0, 0 );   //绘制bitmap
  *               result =  sty_delete_bitmap( bmp );   //删除bitmap
 * \endcode
 ************************************************************************************/
gui_bitmap_info_t* sty_restore_bitmap( unsigned char *path );

/* 获取视图角度*/
/************************************************************************************/
/*!
 * \par  Description:
 *     获取视图角度
 * \ingroup applib_style_dc
 * \return        当前视图角度
 ************************************************************************************/

rotation_e sty_get_view_rotation( void );

/* 设置屏幕方向 */
/************************************************************************************/
/*!
 * \par  Description:
 *     设置当前屏幕方向
 * \ingroup applib_style_dc
 * \param[in]    angle 屏幕角度(0/90/180/270)
 * \return       the result
 * \retval        true    设置成功
 * \retval        false    设置失败
 ************************************************************************************/
bool sty_set_screen_rotation( rotation_e angle );

/* 获取屏幕方向 */
/************************************************************************************/
/*!
 * \par  Description:
 *     获取当前屏幕方向
 * \ingroup applib_style_dc
 * \return       angle 屏幕角度(0/90/180/270)
 ************************************************************************************/
rotation_e sty_get_screen_rotation( void );

/************************************************************************************/
/*!
 * \par  Description:
 *     将framebuffer数据刷新到LCD上
 * \ingroup applib_style_dc
 * \return       the result
 * \retval        true    设置成功
 * \retval        false    设置失败
 ************************************************************************************/
bool sty_lcd_update( void );

#endif

