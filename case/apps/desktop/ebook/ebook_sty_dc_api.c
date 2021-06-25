
#include "ebook_sty_dc_api.h"
#include "include_case.h"

static sty_dc_api_t s_sty_dc_api = 
{
    sty_set_screen_rotation,
    sty_get_view_rotation,
    
    sty_set_text_encoding,
    sty_get_text_encoding,


    
    sty_open_common_pic_res,
    sty_open_common_str_res,
    sty_close_common_pic_res,
    sty_close_common_str_res,
    
    sty_open_app_pic_res,
    sty_open_app_str_res,
    sty_close_app_pic_res,
    sty_close_app_str_res,




    
    sty_set_common_scene,
    sty_set_app_scene,

    sty_create_bitmap,
    sty_delete_bitmap,
    sty_draw_bitmap,
    sty_draw_sub_bitmap,
    
    sty_draw_picture_res,
    sty_draw_sub_picture_id,

    sty_draw_picregion_res,

    sty_draw_string_res_ext,
    sty_draw_string_buf
};

    
sty_dc_api_t* get_sty_dc(void)
{
    return &s_sty_dc_api;
}

