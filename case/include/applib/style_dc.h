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
 * \defgroup   applib_style_dc   ������DC��ͼ�ӿ� 
 */
 
#define RES_NULL 0

/*!
 *  \brief
 *  ��������
 */ 
typedef struct
{
    /*! ���Ͻ�x����*/
    short x;
    
    /*! ���Ͻ�y����*/
    short y;
    
    /*! ���*/
    short width;
    
    /*! �߶�*/
    short height;
    
}sty_rect_t;

/* res��Դ��������*/
/************************************************************************************/
/*!
 * \par  Description:
 *     �򿪹���ͼƬ��Դ
 * \ingroup applib_style_dc
 * \param[in]    pic_res   ͼƬ��Դȫ·��
 * \return       the result
 * \retval        true    ���óɹ�
 * \retval        false    ����ʧ��
 ************************************************************************************/
bool sty_open_common_pic_res( unsigned char *pic_res );

/************************************************************************************/
/*!
 * \par  Description:
 *     �򿪹����ַ�����Դ
 * \ingroup applib_style_dc
 * \param[in]    str_res   �ַ�����Դȫ·��
 * \return       the result
 * \retval        true    ���óɹ�
 * \retval        false    ����ʧ��
 ************************************************************************************/
bool sty_open_common_str_res( unsigned char *str_res );

/************************************************************************************/
/*!
 * \par  Description:
 *     �رչ���ͼƬ��Դ
 * \ingroup applib_style_dc
 * \return       the result
 * \retval        true    ���óɹ�
 * \retval        false    ����ʧ��
 ************************************************************************************/
bool sty_close_common_pic_res( void );

/************************************************************************************/
/*!
 * \par  Description:
 *     �رչ����ַ�����Դ
 * \ingroup applib_style_dc
 * \return       the result
 * \retval        true    ���óɹ�
 * \retval        false    ����ʧ��
 ************************************************************************************/
bool sty_close_common_str_res( void );

/************************************************************************************/
/*!
 * \par  Description:
 *     ��Ӧ��˽��ͼƬ��Դ
 * \ingroup applib_style_dc
 * \param[in]    pic_res   ͼƬ��Դȫ·��
 * \return       the result
 * \retval        true    ���óɹ�
 * \retval        false    ����ʧ��
 ************************************************************************************/
bool sty_open_app_pic_res( unsigned char *pic_res );

/************************************************************************************/
/*!
 * \par  Description:
 *     ��Ӧ��˽���ַ�����Դ
 * \ingroup applib_style_dc
 * \param[in]    str_res   �ַ�����Դȫ·��
 * \return       the result
 * \retval        true    ���óɹ�
 * \retval        false    ����ʧ��
 ************************************************************************************/
bool sty_open_app_str_res( unsigned char *str_res );

/************************************************************************************/
/*!
 * \par  Description:
 *     �ر�Ӧ��˽��ͼƬ��Դ
 * \ingroup applib_style_dc
 * \return       the result
 * \retval        true    ���óɹ�
 * \retval        false    ����ʧ��
 ************************************************************************************/
bool sty_close_app_pic_res( void );

/************************************************************************************/
/*!
 * \par  Description:
 *     �ر�Ӧ��˽���ַ�����Դ
 * \ingroup applib_style_dc
 * \return       the result
 * \retval        true    ���óɹ�
 * \retval        false    ����ʧ��
 ************************************************************************************/
bool sty_close_app_str_res( void );


/* style��Դ��������*/
/************************************************************************************/
/*!
 * \par  Description:
 *     ����Ӧ��style������Դ
 * \ingroup applib_style_dc
 * \param[in]     scene   style����ָ��
 * \return       the result
 * \retval        true    ���óɹ�
 * \retval        false    ����ʧ��
 ************************************************************************************/
bool sty_set_app_scene( scene_t* scene );

/************************************************************************************/
/*!
 * \par  Description:
 *     ��ȡӦ��style������Դ
 * \ingroup applib_style_dc
 * \return       res   style��Դָ��,ʧ�ܷ���NULL
 ************************************************************************************/
scene_t* sty_get_app_scene( void );

/************************************************************************************/
/*!
 * \par  Description:
 *     ���ù���style��Դ
 * \ingroup applib_style_dc
 * \param[in]     scene   style����ָ��
 * \return        the result
 * \retval        true    ���óɹ�
 * \retval        false    ����ʧ��
 ************************************************************************************/
bool sty_set_common_scene( scene_t* scene );

/************************************************************************************/
/*!
 * \par  Description:
 *     ��ȡ����style��Դ
 * \ingroup applib_style_dc
 * \return       res   style��Դָ��,ʧ�ܷ���NULL
 ************************************************************************************/
scene_t* sty_get_common_scene( void );


/* ���Ա��뺯��*/
/************************************************************************************/
/*!
 * \par  Description:
 *     ���õ�ǰ���ַ����Ա���
 * \ingroup applib_style_dc
 * \param[in]    encoding    ���Ա���
 * \return       the result
 * \retval        true    ���óɹ�
 * \retval        false    ����ʧ��
 ************************************************************************************/
bool sty_set_text_encoding( gui_text_encode_t encoding );

/************************************************************************************/
/*!
 * \par  Description:
 *     ��ȡ��ǰ���ַ����Ա���
 * \ingroup applib_style_dc
 * \return       encoding    ���Ա���
 ************************************************************************************/
gui_text_encode_t sty_get_text_encoding( void );


/* �ַ�����ͼ����*/
/************************************************************************************/
/*!
 * \par  Description:
 *     ��ȡ�ַ�������
 * \ingroup applib_style_dc
 * \param[in]    str_id �ַ���ID
 * \return       �ɹ������ַ���ָ��,ʧ�ܷ���NULL
 * \note        ��Ҫ�����ַ���, ��Ӧ�÷���ռ��ٵ���strcpy��������
 * \par          exmaple code
 * \code
 *                unsigned char* str;
 *                char title[64];
 *                str = sty_get_string( READING );    //��ȡ�ַ���
 *                strcpy( title, str );     //�����ַ���
 * \endcode
 ************************************************************************************/
const char* sty_get_string( int str_id );

/************************************************************************************/
/*!
 * \par  Description:
 *     ����ָ����Ƚض��ַ���
 * \ingroup applib_style_dc
 * \param[in]    hdc ��ͼ���
 * \param[in]    src_str Դ�ַ���
 * \param[in]    dst_str Ŀ���ַ���(�ض��ַ���ǰһ�εĽ��)
 * \param[in]    pixel_width ָ�����ؿ��
 * \return       the result
 * \retval       next_string �ض��ַ�������һ��ָ��
 * \par          exmaple code
 * \code
            char buf[512];
            char *cur_pstr;

            //�ض��ַ���src_str, �ֳ�������ʾ
            cur_pstr = sty_truncate_string_by_width( hdc, src_str, buf, line_width );
            sty_draw_string_res_ext( hdc, str_lyric1, buf );            
            sty_draw_string_res_ext( hdc, str_lyric2, cur_pstr );
 * \endcode 
 ************************************************************************************/
char* sty_truncate_string_by_width( int hdc, char *src_str, char *dst_str, int pixel_width );

/************************************************************************************/
/*!
 * \par  Description:
 *     ����style�е��ַ�����ʾ����(�޸�DC������)
 * \ingroup applib_style_dc
 * \param[in]    hdc ��ͼ���
 * \param[in]    string style�е��ַ�����Դ
 * \return       the result
 * \retval        true    ���óɹ�
 * \retval        false    ����ʧ��
 * \par          exmaple code
 * \code
            //����DC�ַ�����ʾ��ʽ
            sty_set_string_format( hdc, string );
            
            //�����ַ���(�ı����ݴ����ӵ������ʹ��)
            sty_draw_string_buf( hdc, buffer, &rect, string->text_align );
            
            //�ָ�DC�ַ�����ʾ��ʽ
            sty_restore_string_format( hdc );
 * \endcode 
 ************************************************************************************/
bool sty_set_string_format( int hdc, string_resource_t *string );

/************************************************************************************/
/*!
 * \par  Description:
 *     �ָ��ϴε��ַ�����ʾ����(�޸�DC������)
 * \ingroup applib_style_dc
 * \param[in]    hdc ��ͼ���
 * \return       the result
 * \retval        true    ���óɹ�
 * \retval        false    ����ʧ��
 ************************************************************************************/
bool sty_restore_string_format( int hdc );

/************************************************************************************/
/*!
 * \par  Description:
 *     ���ƻ������е��ַ���
 * \ingroup applib_style_dc
 * \param[in]    hdc ��ͼ���
 * \param[in]    buf �ַ���
 * \param[in]    rect   ��ʾ����
 * \param[in]    align   ���뷽ʽ
 * \return       the result
 * \retval        true    ���óɹ�
 * \retval        false    ����ʧ��
 ************************************************************************************/
bool sty_draw_string_buf( int hdc, unsigned char *buf, sty_rect_t *rect, int align );

/************************************************************************************/
/*!
 * \par  Description:
 *     ����RES��Դ�е��ַ���
 * \ingroup applib_style_dc
 * \param[in]    hdc ��ͼ���
 * \param[in]    str_id �ַ���ID
 * \param[in]    rect   ��ʾ����
 * \param[in]    align   ���뷽ʽ
 * \return       the result
 * \retval        true    ���óɹ�
 * \retval        false    ����ʧ��
 ************************************************************************************/
bool sty_draw_string_id( int hdc, int str_id, sty_rect_t *rect, int align );

/************************************************************************************/
/*!
 * \par  Description:
 *     ����style�е��ַ�����Դ
 * \ingroup applib_style_dc
 * \param[in]    hdc ��ͼ���
 * \param[in]    string style�е��ַ�����Դ
 * \return       the result
 * \retval        true    ���óɹ�
 * \retval        false    ����ʧ��
 ************************************************************************************/
bool sty_draw_string_res( int hdc, string_resource_t *string );

/************************************************************************************/
/*!
 * \par  Description:
 *     ����style�е��ַ�����Դ(�����ⲿ�����ַ������ݣ�ʹ��style�е�����ͷ��)
 * \ingroup applib_style_dc
 * \param[in]    hdc ��ͼ���
 * \param[in]    string style�е��ַ�����Դ
 * \param[in]    buf �ַ���
 * \return       the result
 * \retval        true    ���óɹ�
 * \retval        false    ����ʧ��
 ************************************************************************************/
bool sty_draw_string_res_ext( int hdc, string_resource_t *string, unsigned char *buf );


/* �����ַ�����������*/
/************************************************************************************/
/*!
 * \par  Description:
 *    �����ַ����Ĺ�����Ϣ
 * \ingroup applib_style_dc
 * \param[in]    hdc ��ͼ���
 * \param[in]    string style�е��ַ�����Դ
 * \param[in]    buf �ַ���(��bufΪNULLʱʹ��style�е�����)
 * \return       scroll �ַ���������Ϣ, ʧ�ܷ���NULL
 * \par          exmaple code
 * \code
 *                scroll = sty_create_scroll_string( hdc, str_sel, NULL );   //����������Ϣ
 *                result = sty_draw_scroll_string( hdc, str_sel, scroll );   //���ƹ����ַ���
  *               result =  sty_delete_scroll_string( scroll );   //ɾ��������Ϣ
 * \endcode
 ************************************************************************************/
gui_scroll_info_t * sty_create_scroll_string( int hdc, string_resource_t *string, char *buf );

/************************************************************************************/
/*!
 * \par  Description:
 *     ���������ַ���
 * \ingroup applib_style_dc
 * \param[in]    hdc ��ͼ���
 * \param[in]    string style�е��ַ�����Դ
 * \param[in]    scroll �ַ���������Ϣ
 * \return       the result
 * \retval        true    ���óɹ�
 * \retval        false    ����ʧ��
 ************************************************************************************/
bool sty_draw_scroll_string( int hdc, string_resource_t *string, gui_scroll_info_t *scroll );

/************************************************************************************/
/*!
 * \par  Description:
 *     ɾ�������ַ�����Ϣ
 * \ingroup applib_style_dc
 * \param[in]    scroll �ַ���������Ϣ
 * \return       the result
 * \retval        true    ���óɹ�
 * \retval        false    ����ʧ��
 ************************************************************************************/
bool sty_delete_scroll_string( gui_scroll_info_t *scroll );


/* ͼƬ��ͼ����*/
/************************************************************************************/
/*!
 * \par  Description:
 *     ���ƻ������е�ͼƬ
 * \ingroup applib_style_dc
 * \param[in]    hdc ��ͼ���
 * \param[in]    buf ͼƬ������
 * \param[in]    bpp ÿ�����ֽ���
 * \param[in]    rect   ͼƬ��ʾ����
 * \return       the result
 * \retval        true    ���óɹ�
 * \retval        false    ����ʧ��
 ************************************************************************************/
bool sty_draw_picture_buf( int hdc, unsigned char *buf, int bpp, sty_rect_t *rect );

/************************************************************************************/
/*!
 * \par  Description:
 *     ����RES��Դ�е�ͼƬ
 * \ingroup applib_style_dc
 * \param[in]    hdc ��ͼ���
 * \param[in]    pic_id ͼƬID
 * \param[in]    x   ͼƬ��ʾ�������Ͻ�x����
 * \param[in]    y   ͼƬ��ʾ�������Ͻ�y����
 * \return       the result
 * \retval        true    ���óɹ�
 * \retval        false    ����ʧ��
 ************************************************************************************/
bool sty_draw_picture_id( int hdc, int pic_id, int x, int y );

/************************************************************************************/
/*!
 * \par  Description:
 *     ����style�е�ͼƬ��Դ
 * \ingroup applib_style_dc
 * \param[in]    hdc ��ͼ���
 * \param[in]    picture style�е�ͼƬ��Դ
 * \return       the result
 * \retval        true    ���óɹ�
 * \retval        false    ����ʧ��
 ************************************************************************************/
bool sty_draw_picture_res( int hdc, picture_resource_t *picture );

/************************************************************************************/
/*!
 * \par  Description:
 *     ����style�е�ͼƬ������һ֡ͼƬ
 * \ingroup applib_style_dc
 * \param[in]    hdc ��ͼ���
 * \param[in]    picregion style�е�ͼƬ������Դ
 * \param[in]    frame     style�е�ͼƬ����ָ��֡��
 * \return       the result
 * \retval       true    ���óɹ�
 * \retval       false    ����ʧ��
 ************************************************************************************/
bool sty_draw_picregion_res( int hdc, picregion_resource_t *picregion, unsigned short frame );

/************************************************************************************/
/*!
 * \par  Description:
 *     ���ƻ�����ͼƬ�Ĳ�������
 * \ingroup applib_style_dc
 * \param[in]    hdc ��ͼ���
 * \param[in]    buf ͼƬ������
 * \param[in]    bpp ÿ�����ֽ���
 * \param[in]    rect   ͼƬ��ʾ����
 * \param[in]    srect   ͼƬ��������
 * \return       the result
 * \retval        true    ���óɹ�
 * \retval        false    ����ʧ��
 ************************************************************************************/
bool sty_draw_sub_picture_buf( int hdc, unsigned char *buf, int bpp, sty_rect_t *rect, sty_rect_t *srect );

/************************************************************************************/
/*!
 * \par  Description:
 *     ����RES��ԴͼƬ�Ĳ�������
 * \ingroup applib_style_dc
 * \param[in]    hdc ��ͼ���
 * \param[in]    pic_id ͼƬID
 * \param[in]    x   ͼƬ��ʾ�������Ͻ�x����
 * \param[in]    y   ͼƬ��ʾ�������Ͻ�y����
 * \param[in]    srect   ͼƬ��������
 * \return       the result
 * \retval        true    ���óɹ�
 * \retval        false    ����ʧ��
 ************************************************************************************/
bool sty_draw_sub_picture_id( int hdc, int pic_id, int x, int y, sty_rect_t *srect );


/* ����bitmap�ӿ� */

/************************************************************************************/
/*!
 * \par  Description:
 *     ����bitmapλͼ
 * \ingroup applib_style_dc
 * \param[in]    hdc ��ͼ���
 * \param[in]    bmp λͼָ��
 * \param[in]    x   ͼƬ��ʾ�������Ͻ�x����
 * \param[in]    y   ͼƬ��ʾ�������Ͻ�y����
 * \return       the result
 * \retval        true    ���óɹ�
 * \retval        false    ����ʧ��
 ************************************************************************************/
bool sty_draw_bitmap( int hdc, gui_bitmap_info_t *bmp, int x, int y );

/************************************************************************************/
/*!
 * \par  Description:
 *     ����bitmapλͼ��������
 * \ingroup applib_style_dc
 * \param[in]    hdc ��ͼ���
 * \param[in]    bmp λͼָ��
 * \param[in]    x   ͼƬ��ʾ�������Ͻ�x����
 * \param[in]    y   ͼƬ��ʾ�������Ͻ�y����
 * \param[in]    srect   ͼƬ��������
 * \return       the result
 * \retval        true    ���óɹ�
 * \retval        false    ����ʧ��
 ************************************************************************************/
bool sty_draw_sub_bitmap( int hdc, gui_bitmap_info_t *bmp, int x, int y, sty_rect_t *srect );


/* ͼƬ����ӿ� */

/************************************************************************************/
/*!
 * \par  Description:
 *     ����RESͼƬ��Դ������bitmapλͼ
 * \ingroup applib_style_dc
 * \param[in]    pic_id ͼƬID
 * \return       �ɹ�����bitmapλͼָ��, ʧ�ܷ���NULL 
 * \par          exmaple code
 * \code
 *                bmp = sty_create_bitmap( BG );	//����RESͼƬ��Դ��bitmap
 *                result = sty_draw_bitmap( hdc, bmp, 0, 0 );   //����bitmap
  *               result =  sty_delete_bitmap( bmp );   //ɾ��bitmap
 * \endcode
 ************************************************************************************/
gui_bitmap_info_t* sty_create_bitmap( int pic_id );

/************************************************************************************/
/*!
 * \par  Description:
 *     ����RESͼƬ��Դ�������򲢴���bitmapλͼ
 * \ingroup applib_style_dc
 * \param[in]    pic_id ͼƬID
 * \param[in]    rect   ͼƬ����
 * \return       �ɹ�����bitmapλͼָ��, ʧ�ܷ���NULL
 ************************************************************************************/
gui_bitmap_info_t* sty_create_sub_bitmap( int pic_id, sty_rect_t *rect );

/************************************************************************************/
/*!
 * \par  Description:
 *     ɾ��bitmapλͼ����
 * \ingroup applib_style_dc
 * \param[in]    bmp λͼָ��
 * \return       the result
 * \retval        true    ���óɹ�
 * \retval        false    ����ʧ��
 ************************************************************************************/
bool sty_delete_bitmap( gui_bitmap_info_t *bmp );

/* ����ͼ����ͻ�ȡ�ӿ� */
/************************************************************************************/
/*!
 * \par  Description:
 *     ����bitmapλͼ����
 * \ingroup applib_style_dc
 * \param[in]    path λͼ����·��
 * \param[in]    bmp λͼָ��
 * \return       the result
 * \retval        true    ���óɹ�
 * \retval        false    ����ʧ��
  * \par          exmaple code
 * \code
 *                bmp = sty_create_bitmap( BG );	//����RESͼƬ��Դ��bitmap
 *                result = sty_save_bitmap( "/mnt/diska/apps/bg.bin", bmp );	//����bitmap�ļ�
  *               result =  sty_delete_bitmap( bmp );   //ɾ��bitmap
 * \endcode
 ************************************************************************************/
bool sty_save_bitmap( unsigned char *path, gui_bitmap_info_t *bmp );

/************************************************************************************/
/*!
 * \par  Description:
 *     �ָ�bitmapλͼ����
 * \ingroup applib_style_dc
 * \param[in]    path λͼ����·��
 * \return       �ɹ�����bitmapλͼָ��, ʧ�ܷ���NULL 
 * \par          exmaple code
 * \code
 *                bmp = sty_restore_bitmap( "/mnt/diska/apps/bg.bin" );	//��ȡbitmap�ļ�
 *                result = sty_draw_bitmap( hdc, bmp, 0, 0 );   //����bitmap
  *               result =  sty_delete_bitmap( bmp );   //ɾ��bitmap
 * \endcode
 ************************************************************************************/
gui_bitmap_info_t* sty_restore_bitmap( unsigned char *path );

/* ��ȡ��ͼ�Ƕ�*/
/************************************************************************************/
/*!
 * \par  Description:
 *     ��ȡ��ͼ�Ƕ�
 * \ingroup applib_style_dc
 * \return        ��ǰ��ͼ�Ƕ�
 ************************************************************************************/

rotation_e sty_get_view_rotation( void );

/* ������Ļ���� */
/************************************************************************************/
/*!
 * \par  Description:
 *     ���õ�ǰ��Ļ����
 * \ingroup applib_style_dc
 * \param[in]    angle ��Ļ�Ƕ�(0/90/180/270)
 * \return       the result
 * \retval        true    ���óɹ�
 * \retval        false    ����ʧ��
 ************************************************************************************/
bool sty_set_screen_rotation( rotation_e angle );

/* ��ȡ��Ļ���� */
/************************************************************************************/
/*!
 * \par  Description:
 *     ��ȡ��ǰ��Ļ����
 * \ingroup applib_style_dc
 * \return       angle ��Ļ�Ƕ�(0/90/180/270)
 ************************************************************************************/
rotation_e sty_get_screen_rotation( void );

/************************************************************************************/
/*!
 * \par  Description:
 *     ��framebuffer����ˢ�µ�LCD��
 * \ingroup applib_style_dc
 * \return       the result
 * \retval        true    ���óɹ�
 * \retval        false    ����ʧ��
 ************************************************************************************/
bool sty_lcd_update( void );

#endif

