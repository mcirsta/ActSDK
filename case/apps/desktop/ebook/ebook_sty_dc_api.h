
#ifndef __STY_DC_API_H__
#define __STY_DC_API_H__

#include "include_case.h"


#include "style.h"

typedef struct
{
    bool (*const sty_set_screen_rotation )( rotation_e angle );
    rotation_e (*const sty_get_view_rotation)( void );
    
    bool (*const sty_set_text_encoding )( gui_text_encode_t encoding );
    gui_text_encode_t (*const sty_get_text_encoding)();

    bool (*const sty_open_common_pic_res)( unsigned char *pic_res );
    bool (*const sty_open_common_str_res)( unsigned char *str_res );
    bool (*const sty_close_common_pic_res)( void );
    bool (*const sty_close_common_str_res)( void );

    bool (*const sty_open_app_pic_res)( unsigned char *pic_res );
    bool (*const sty_open_app_str_res)( unsigned char *str_res );
    bool (*const sty_close_app_pic_res)( void );
    bool (*const sty_close_app_str_res)( void );




    
    bool (*const sty_set_common_scene ) ( scene_t* scene );
    bool (*const sty_set_app_scene ) ( scene_t* scene );

    gui_bitmap_info_t* (*const sty_create_bitmap)( int pic_id );
    bool (*const sty_delete_bitmap)( gui_bitmap_info_t *bmp );
    bool (*const sty_draw_bitmap)( int hdc, gui_bitmap_info_t *bmp, int x, int y );
    bool (*const sty_draw_sub_bitmap)( int hdc, gui_bitmap_info_t *bmp, int x, int y, sty_rect_t *srect );
    
    bool (*const sty_draw_picture_res ) ( int hdc, picture_resource_t *picture );
    bool (*const sty_draw_sub_picture_id ) ( int hdc, int pic_id, int x, int y, sty_rect_t *srect );

    bool (*const sty_draw_picregion_res ) ( int hdc, picregion_resource_t *picregion, unsigned short frame );

    bool (*const sty_draw_string_res_ext ) ( int hdc, string_resource_t *string, unsigned char *buf );
    bool (*const sty_draw_string_buf ) ( int hdc, unsigned char *buf, sty_rect_t *rect, int align );
    
}sty_dc_api_t;

sty_dc_api_t* get_sty_dc(void);

#endif /* __STY_DC_API_H__ */

