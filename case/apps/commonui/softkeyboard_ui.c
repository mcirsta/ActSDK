/********************************************************************************
 *                              USDK(1100)
 *                             Module: app 
 *                 Copyright(c) 2003-2008 Actions Semiconductor,
 *                            All Rights Reserved. 
 * file : softkeyboard_ui.c
 * History:         
 *      <author>    <time>           <version >             <desc>
 *      Herb Hu    2009-5-6             v1.0             build this file 
 ********************************************************************************/

#include <include_case.h>

#include "softkeyboard_private.h"


void paint_key_table(softkeyboard_t * keyboard)
{
    sty_rect_t rect;

    if(IME_TYPE_PINYIN == keyboard->data->ime)
    {
        rect.x = ckb_resource.pic_ckb_key_table_bg->x;
        rect.y = ckb_resource.pic_ckb_key_table_bg->y;
        rect.width = ckb_resource.pic_ckb_key_table_bg->width;
        rect.height = ckb_resource.pic_ckb_key_table_bg->height;

        canvas_erase(keyboard->canvas_id, &rect);  
        sty_draw_picture_res( keyboard->hdc, ckb_resource.pic_ckb_key_table_bg);
        if(keyboard->data->ckb_data.state == CKB_STATE_INPUT)
        {
            sty_draw_picture_res( keyboard->hdc, ckb_key_table[keyboard->data->ckb_data.curr_key].pic_res);
        }
        
        paint_candidacy(keyboard);
        paint_left_arrow(keyboard, false);
        paint_right_arrow(keyboard, false);
    }
    else if(IME_TYPE_EN == keyboard->data->ime)
    { 
        rect.x = ekb_resource.pic_ekb_key_table_bg->x;
        rect.y = ekb_resource.pic_ekb_key_table_bg->y;
        rect.width = ekb_resource.pic_ekb_key_table_bg->width;
        rect.height = ekb_resource.pic_ekb_key_table_bg->height;

        canvas_erase(keyboard->canvas_id, &rect);
        sty_draw_picture_res( keyboard->hdc, ekb_resource.pic_ekb_key_table_bg);
        sty_draw_picture_res( keyboard->hdc, ekb_key_table[keyboard->data->ekb_data.curr_key].pic_res);
    }
    else
    {
        rect.x = nkb_resource.pic_nkb_key_table_bg->x;
        rect.y = nkb_resource.pic_nkb_key_table_bg->y;
        rect.width = nkb_resource.pic_nkb_key_table_bg->width;
        rect.height = nkb_resource.pic_nkb_key_table_bg->height;

        canvas_erase(keyboard->canvas_id, &rect);
        sty_draw_picture_res( keyboard->hdc, nkb_resource.pic_nkb_key_table_bg);
        sty_draw_picture_res( keyboard->hdc, nkb_key_table[keyboard->data->nkb_data.curr_key].pic_res);
    }

}

void paint_display_text(softkeyboard_t * keyboard)
{
    int len;
    int len_2;
    sty_rect_t rect;
    gui_text_encode_t  cur_encoding;
    unsigned short first_line_text[TEXT_MAX_LEN] = {0};
    unsigned short * next_line_text = NULL;
    string_resource_t str_first_line;
    string_resource_t str_next_line;

    rect.x = common_resource.pic_display->x;
    rect.y = common_resource.pic_display->y;
    rect.width = common_resource.pic_display->width;
    rect.height = common_resource.pic_display->height;

    cur_encoding = sty_get_text_encoding();
    sty_set_text_encoding(GUI_UNICODE_ENC);

    canvas_erase(keyboard->canvas_id, &rect); 
    sty_draw_picture_res( keyboard->hdc, common_resource.pic_display);

    gui_dc_set_font_size( keyboard->hdc, (int)common_resource.str_display->font_height );
    len = gui_dc_get_string_dist_x(keyboard->hdc, (char *)keyboard->data->text, GUI_UNICODE_ENC);

    if(len < common_resource.str_display->width)
    {
        sty_draw_string_res_ext( keyboard->hdc,  common_resource.str_display, (char *)keyboard->data->text);
    }
    else
    {
        gui_dc_set_font_size( keyboard->hdc, (int)common_resource.str_display_two_line->font_height );
        len = gui_dc_get_string_dist_x(keyboard->hdc, (char *)keyboard->data->text, GUI_UNICODE_ENC);

        if(len < common_resource.str_display_two_line->width)
        {
            sty_draw_string_res_ext(keyboard->hdc,common_resource.str_display_two_line,(char *)keyboard->data->text);
        }
        else 
        {
            next_line_text =(unsigned short *)sty_truncate_string_by_width(keyboard->hdc,(char *)keyboard->data->text, 
                    (char *)first_line_text, common_resource.str_display->width);    
            len_2 = gui_dc_get_string_dist_x(keyboard->hdc, (char *)next_line_text, GUI_UNICODE_ENC);

            if(len_2 < common_resource.str_display_two_line->width)
            {
                str_first_line = *(common_resource.str_display_two_line);
                str_first_line.height = str_first_line.height / 2;

                str_next_line = *(common_resource.str_display_two_line);
                str_next_line.height = str_next_line.height / 2;
                str_next_line.y = str_first_line.y + str_first_line.height;

                sty_draw_string_res_ext( keyboard->hdc, &str_first_line, (char *)first_line_text);
                sty_draw_string_res_ext( keyboard->hdc, &str_next_line, (char *)next_line_text);
            }
        }
    }

    sty_set_text_encoding(cur_encoding);
}

void paint_candidacy(softkeyboard_t * keyboard)
{
    int i;
    struct ckb_data_s * ckb_data;
    unsigned short unicode_str[2];
    gui_rect_t gui_rect;
    sty_rect_t sty_rect;

    ckb_data = &(keyboard->data->ckb_data);

    sty_rect.x = ckb_resource.pic_ckb_candidacy->x;
    sty_rect.y = ckb_resource.pic_ckb_candidacy->y;
    sty_rect.width = ckb_resource.pic_ckb_candidacy->width;
    sty_rect.height = ckb_resource.pic_ckb_candidacy->height;

    canvas_erase(keyboard->canvas_id, &sty_rect);

    sty_rect.x = ckb_resource.pic_ckb_candidacy->x - ckb_resource.pic_ckb_key_table_bg->x;
    sty_rect.y = ckb_resource.pic_ckb_candidacy->y - ckb_resource.pic_ckb_key_table_bg->y;

    sty_draw_sub_picture_id( keyboard->hdc, ckb_resource.pic_ckb_key_table_bg->pic_id,
                  ckb_resource.pic_ckb_candidacy->x, ckb_resource.pic_ckb_candidacy->y, &sty_rect);

    sty_draw_picture_res( keyboard->hdc, ckb_resource.pic_ckb_candidacy);

    /* draw chinese pinying strings */

    sty_rect.x = ckb_resource.str_ckb_pinyin->x;
    sty_rect.y = ckb_resource.str_ckb_pinyin->y;
    sty_rect.width = ckb_resource.str_ckb_pinyin->width;
    sty_rect.height = ckb_resource.str_ckb_pinyin->height;

    canvas_erase(keyboard->canvas_id, &sty_rect);

    sty_rect.x = ckb_resource.str_ckb_pinyin->x - ckb_resource.pic_ckb_key_table_bg->x;
    sty_rect.y = ckb_resource.str_ckb_pinyin->y - ckb_resource.pic_ckb_key_table_bg->y;

    sty_draw_sub_picture_id( keyboard->hdc, ckb_resource.pic_ckb_key_table_bg->pic_id,
                  ckb_resource.str_ckb_pinyin->x, ckb_resource.str_ckb_pinyin->y, &sty_rect);

    sty_draw_string_res_ext( keyboard->hdc,  ckb_resource.str_ckb_pinyin, ckb_data->pinyin_buf);

    /* if find candidacy characters */ 
    if(ckb_data->candidates > 0)
    {
        /* draw focus background picture */
        if(keyboard->data->ckb_data.state == CKB_STATE_CANDIDACY)
        {
            sty_draw_picture_id(keyboard->hdc, ckb_resource.pic_ckb_focus->pic_id, \
                ckb_resource.pic_ckb_focus->x + (ckb_resource.pic_ckb_focus->width * ckb_data->candidacy_view_focus),\
                ckb_resource.pic_ckb_focus->y);
        }

        /* draw candidacy characters */
        unicode_str[1] = 0;

        gui_dc_set_text_mode( keyboard->hdc, ckb_resource.str_ckb_candidacy->mode );
        gui_dc_set_color( keyboard->hdc, (int)ckb_resource.str_ckb_candidacy->foreground );
        gui_dc_set_font_size( keyboard->hdc, ckb_resource.str_ckb_candidacy->font_height );

        for(i = 0; i <= (ckb_data->candidacy_view_end - ckb_data->candidacy_view_start); i++)
        {
            print_dbg("index:%d, start:%d, end:%d, focus:%d", ckb_data->candidacy_view_start + i, \
                    ckb_data->candidacy_view_start, ckb_data->candidacy_view_end, ckb_data->candidacy_view_focus);

            unicode_str[0] = ckb_data->candidacy_buf[ckb_data->candidacy_view_start + i];

            gui_rect.x0 = (short)(ckb_resource.pic_ckb_focus->x + (ckb_resource.pic_ckb_focus->width * i));
            gui_rect.y0 = ckb_resource.pic_ckb_focus->y;
            gui_rect.x1 = gui_rect.x0 + ckb_resource.pic_ckb_focus->width - 1;
            gui_rect.y1 = gui_rect.y0 + ckb_resource.pic_ckb_focus->height - 1;

            gui_dc_display_string_in_rect(keyboard->hdc, (char *)unicode_str, &gui_rect, \
                    ckb_resource.str_ckb_candidacy->text_align, GUI_UNICODE_ENC);
        }
    }
}

void  paint_left_arrow(softkeyboard_t * keyboard, bool highlight)
{
    sty_rect_t rect;

    rect.x = ckb_resource.pic_ckb_left_arrow_1->x;
    rect.y = ckb_resource.pic_ckb_left_arrow_1->y;
    rect.width = ckb_resource.pic_ckb_left_arrow_1->width;
    rect.height = ckb_resource.pic_ckb_left_arrow_1->height;

    canvas_erase(keyboard->canvas_id, &rect); 

    rect.x = ckb_resource.pic_ckb_left_arrow_1->x - ckb_resource.pic_ckb_key_table_bg->x;
    rect.y = ckb_resource.pic_ckb_left_arrow_1->y - ckb_resource.pic_ckb_key_table_bg->y;

    sty_draw_sub_picture_id( keyboard->hdc, ckb_resource.pic_ckb_key_table_bg->pic_id,
            ckb_resource.pic_ckb_left_arrow_1->x, ckb_resource.pic_ckb_left_arrow_1->y, &rect);

    if(true == highlight)
    {
        sty_draw_picture_res( keyboard->hdc, ckb_resource.pic_ckb_left_arrow_2);
    }
    else
    {
        sty_draw_picture_res( keyboard->hdc, ckb_resource.pic_ckb_left_arrow_1);
    }
}

void  paint_right_arrow(softkeyboard_t * keyboard, bool highlight)
{
    sty_rect_t rect;

    rect.x = ckb_resource.pic_ckb_right_arrow_1->x;
    rect.y = ckb_resource.pic_ckb_right_arrow_1->y;
    rect.width = ckb_resource.pic_ckb_right_arrow_1->width;
    rect.height = ckb_resource.pic_ckb_right_arrow_1->height;

    canvas_erase(keyboard->canvas_id, &rect);

    rect.x = ckb_resource.pic_ckb_right_arrow_1->x - ckb_resource.pic_ckb_key_table_bg->x;
    rect.y = ckb_resource.pic_ckb_right_arrow_1->y - ckb_resource.pic_ckb_key_table_bg->y;

    sty_draw_sub_picture_id( keyboard->hdc, ckb_resource.pic_ckb_key_table_bg->pic_id,
            ckb_resource.pic_ckb_right_arrow_1->x, ckb_resource.pic_ckb_right_arrow_1->y, &rect);

    if(true == highlight)
    {
        sty_draw_picture_res( keyboard->hdc, ckb_resource.pic_ckb_right_arrow_2);
    }
    else
    {
        sty_draw_picture_res( keyboard->hdc, ckb_resource.pic_ckb_right_arrow_1);
    }
}
