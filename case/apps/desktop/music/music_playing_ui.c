/********************************************************************************
 *                              USDK(1100)
 *                             Module: app 
 *                 Copyright(c) 2003-2009 Actions Semiconductor,
 *                            All Rights Reserved. 
 * File : music_playing_ui.c
 * History:         
 *      <author>    <time>           <version >             <desc>
 *    songzhining    2009-2-25          v1.0               build this file 
 ********************************************************************************/
/*!
* \file     music_playing_ui.c
* \brief    music_playing_ui widget
* \author   songzhining
*               
* \version 1.0
* \date  2009/02/25
*******************************************************************************/
    
#include "music.h"
#include "music_playing_ui.h"

static bool _erase_picture( scene_play_t* scene_play, picture_resource_t *pic )
{
    sty_rect_t rect;
    
    //draw bg
    if ( ( scene_play != NULL ) && ( scene_play->bg_inited == true ) && ( pic != NULL ) )
    {
        rect.x = pic->x;
        rect.y = pic->y;
        rect.width = pic->width;
        rect.height = pic->height;
        
        sty_draw_sub_bitmap( scene_play->hdc, scene_play->bmp_bg, pic->x, pic->y, &rect );
    }

    return true;
}

static bool _erase_picregion( scene_play_t* scene_play, picregion_resource_t *picreg )
{
    sty_rect_t rect;
    
    //draw bg
    if ( ( scene_play != NULL ) && ( scene_play->bg_inited == true ) && ( picreg != NULL ) )
    {
        rect.x = picreg->x;
        rect.y = picreg->y;
        rect.width = picreg->width;
        rect.height = picreg->height;
        
        sty_draw_sub_bitmap( scene_play->hdc, scene_play->bmp_bg, picreg->x, picreg->y, &rect );
    }

    return true;
}

static bool _erase_string( scene_play_t* scene_play, string_resource_t *str )
{
    sty_rect_t rect;
    
    //draw bg
    if ( ( scene_play != NULL ) && ( scene_play->bg_inited == true ) && ( str != NULL ) )
    {
        rect.x = str->x;
        rect.y = str->y;
        rect.width = str->width;
        rect.height = str->height;
        
        sty_draw_sub_bitmap( scene_play->hdc, scene_play->bmp_bg, str->x, str->y, &rect );
    }

    return true;
}

/* draw spectrum */
static bool _draw_spectrum( scene_play_t* scene_play )
{
    unsigned char *spec;
    int idx;
    int current;
    int total;
    int x, y;
    sty_rect_t rect;
    int top_h;
    static int top_y[18] = {0};
    static int top_v[18] = {0};

    if ( scene_play == NULL )
    {
        return false;    
    }
    else
    {
        spec = scene_play->engine.music_status.spectrum;
    }

    //erase effect area
    _erase_picture( scene_play, scene_play->pic_effect_bg );

    //draw effect bg
    x = scene_play->pic_effect_bg->x;
    y = scene_play->pic_effect_bg->y;
    sty_draw_bitmap( scene_play->hdc, scene_play->bmp_effect_bg, x, y );

    //draw effect bar
    if ( scene_play->pic_effect_top != NULL )
    {    
        top_h = scene_play->pic_effect_top->height;
        total = scene_play->pic_effect_bg->height;
    }
    else
    {
        top_h = 0;
        total = scene_play->pic_effect_bg->height;
    }

    for ( idx = 0; idx < 18; idx ++ )
    {
        current = ( spec[idx] * ( total - top_h - 1 ) ) / 31;

        rect.x = 0;
        rect.y = (short)( scene_play->bmp_effect_dot->ysize - current );
        rect.width = (short)scene_play->bmp_effect_dot->xsize;
        rect.height = (short)current;

        x = scene_play->pic_effect_bg->x
                + ( idx * ( scene_play->bmp_effect_dot->xsize + 1 ) );
        y = ( scene_play->pic_effect_bg->y + total ) - rect.height;

        sty_draw_sub_bitmap(scene_play->hdc, scene_play->bmp_effect_dot, x, y, &rect);

        //top process
        top_v[idx] += 1;
        top_y[idx] += top_v[idx];

        if( ( y <= top_y[idx] ) || ( top_y[idx] < scene_play->pic_effect_bg->y ) )
        {
            top_y[idx] = y;
            top_v[idx] = 0;
        }
        
        if( top_y[idx] > ( scene_play->pic_effect_bg->y + total ) )
        {
            top_y[idx] = scene_play->pic_effect_bg->y + total;
        }

        if( current > 0 )
        {
            y = top_y[idx] - top_h - 1;
        }
        else
        {
            y = top_y[idx] - top_h;
        }

        sty_draw_bitmap( scene_play->hdc, scene_play->bmp_effect_top, x, y );
    }

    return true;
}

/* draw fire effect */
static bool _draw_fire( scene_play_t* scene_play )
{
    int frq[18];
    se_env_t ssf_env;
    unsigned int num;
    int offset;
    canvas_info_t canvas_info;
    int x0, y0, x1, y1;

    //copy scene_play
    for ( num=0; num < (sizeof(frq)/sizeof(int)); num++ )
    {
        frq[num] = (int)scene_play->engine.music_status.spectrum[num];
    }

    if ( scene_play->ssf_handle == NULL )
    {
        //get layer 0 buffer
        canvas_get_info( 0, &canvas_info );
        
        ssf_env.bpp = 2;
        offset = scene_play->pic_effect_bg->x + ( scene_play->pic_effect_bg->y * canvas_info.rect.width );
        ssf_env.buf = (char*)canvas_info.pdata + ( offset * (int)ssf_env.bpp );
        ssf_env.buf_width_in_pixel = (unsigned int)canvas_info.rect.width;
        ssf_env.output_w = (unsigned int)scene_play->pic_effect_bg->width;
        ssf_env.output_h = (unsigned int)scene_play->pic_effect_bg->height;
        
        scene_play->ssf_handle = se_ssf_open(&ssf_env);
    }

    //生成频点
    if ( scene_play->eff_frame == 0 )
    {
        se_ssf_ioctl(scene_play->ssf_handle, SSF_NEW_FRQ, (int)frq);    
    }

    //update frame
    se_ssf_ioctl(scene_play->ssf_handle, SSF_PLAY, 0);
    
    //gui_framebuffer_update();
    //sty_lcd_update();
    
    //set update region
    x0 = scene_play->pic_effect_bg->x;
    y0 = scene_play->pic_effect_bg->y;
    x1 = ( scene_play->pic_effect_bg->x + scene_play->pic_effect_bg->width ) - 1;
    y1 = ( scene_play->pic_effect_bg->y + scene_play->pic_effect_bg->height ) - 1;
    gui_set_update_region( x0, y0, x1, y1 );

    //next frame
    scene_play->eff_frame = ( scene_play->eff_frame + 1 ) % 20;

    return true;
}

/* draw wave effect */
static bool _draw_wave( scene_play_t* scene_play )
{
    int frq[18];
    se_env_t ssf_env;
    unsigned int num;
    int offset;
    canvas_info_t canvas_info;
    int x0, y0, x1, y1;

    //copy scene_play
    for ( num=0; num < (sizeof(frq)/sizeof(int)); num++ )
    {
        frq[num] = (int)scene_play->engine.music_status.spectrum[num];
    }

    if ( scene_play->wav_handle == NULL )
    {        
        //get layer 0 buffer
        canvas_get_info( 0, &canvas_info );
        
        ssf_env.bpp = 2;
        offset = scene_play->pic_effect_bg->x + ( scene_play->pic_effect_bg->y * canvas_info.rect.width );
        ssf_env.buf = (char*)canvas_info.pdata + ( offset * (int)ssf_env.bpp );
        ssf_env.buf_width_in_pixel = (unsigned int)canvas_info.rect.width;
        ssf_env.output_w = (unsigned int)scene_play->pic_effect_bg->width;
        ssf_env.output_h = (unsigned int)scene_play->pic_effect_bg->height;
    
        scene_play->wav_handle = se_wav_open(&ssf_env);
    }
    
    //生成频点
    if ( scene_play->eff_frame == 0 )
    {
        se_wav_ioctl(scene_play->wav_handle, SSF_NEW_FRQ, (int)frq);   
    }

    //update frame
    se_wav_ioctl(scene_play->wav_handle, SSF_PLAY, 0);
    
    //gui_framebuffer_update();
    //sty_lcd_update();
    
    //set update region
    x0 = scene_play->pic_effect_bg->x;
    y0 = scene_play->pic_effect_bg->y;
    x1 = ( scene_play->pic_effect_bg->x + scene_play->pic_effect_bg->width ) - 1;
    y1 = ( scene_play->pic_effect_bg->y + scene_play->pic_effect_bg->height ) - 1;
    gui_set_update_region( x0, y0, x1, y1 );

    //next frame
    scene_play->eff_frame = ( scene_play->eff_frame + 1 ) % 20;

    return true;
}

/* draw lyric */
static bool _draw_lyric( scene_play_t *scene_play )
{
    int cur_time;
    int str_width;
    int line_width;
    unsigned int color;
    char buf[PATH_MAX];
    char *prev_str;
    char *cur_pstr;
    char *next_pstr;
    
    //erase effect area
    _erase_picture( scene_play, scene_play->pic_effect_bg );

    if ( scene_play->lyric_flag == true )
    {
        //get lyric
        cur_time = scene_play->engine.music_status.cur_time;
        lyric_set_current_time( cur_time );
        cur_time = cur_time / 1000;
        lyric_get_prev( scene_play->prev_lyric, sizeof(scene_play->prev_lyric) );
        lyric_get_current( scene_play->cur_lyric, sizeof(scene_play->cur_lyric) );
        lyric_get_next( scene_play->next_lyric, sizeof(scene_play->next_lyric) );

        prev_str = scene_play->prev_lyric;
        cur_pstr = scene_play->cur_lyric;
        next_pstr = scene_play->next_lyric;

        //set font size
        gui_dc_set_font_size( scene_play->hdc, (int)scene_play->str_lyric1->font_height );

        //show lyric
        sty_set_text_encoding( GUI_MBCS_ENC );

        line_width = scene_play->str_lyric1->width;
        str_width = gui_dc_get_string_dist_x( scene_play->hdc, cur_pstr, GUI_MBCS_ENC );        

        if( str_width < line_width )
        {
            //current lyric is one line
            sty_draw_string_res_ext( scene_play->hdc, scene_play->str_lyric1, prev_str );
            sty_draw_string_res_ext( scene_play->hdc, scene_play->str_lyric2, cur_pstr );
            sty_draw_string_res_ext( scene_play->hdc, scene_play->str_lyric3, next_pstr );
        }
        else if( str_width < ( line_width * 2 ) )
        {
            //current lyric is two line
            sty_draw_string_res_ext( scene_play->hdc, scene_play->str_lyric1, prev_str );
            
            cur_pstr = sty_truncate_string_by_width( scene_play->hdc, cur_pstr, buf, line_width );
            sty_draw_string_res_ext( scene_play->hdc, scene_play->str_lyric2, buf );
            
            color = scene_play->str_lyric3->foreground;
            scene_play->str_lyric3->foreground = scene_play->str_lyric2->foreground;
            sty_draw_string_res_ext( scene_play->hdc, scene_play->str_lyric3, cur_pstr );
            scene_play->str_lyric3->foreground = color;
        }
        else
        {
            //current lyric is three line
            cur_pstr = sty_truncate_string_by_width( scene_play->hdc, cur_pstr, buf, line_width );
            
            color = scene_play->str_lyric1->foreground;
            scene_play->str_lyric1->foreground = scene_play->str_lyric2->foreground;
            sty_draw_string_res_ext( scene_play->hdc, scene_play->str_lyric1, buf );
            scene_play->str_lyric1->foreground = color;
            
            cur_pstr = sty_truncate_string_by_width( scene_play->hdc, cur_pstr, buf, line_width );
            sty_draw_string_res_ext( scene_play->hdc, scene_play->str_lyric2, buf );
            
            color = scene_play->str_lyric3->foreground;
            scene_play->str_lyric3->foreground = scene_play->str_lyric2->foreground;
            sty_draw_string_res_ext( scene_play->hdc, scene_play->str_lyric3, cur_pstr );
            scene_play->str_lyric3->foreground = color;
        }
        
        sty_set_text_encoding( GUI_UTF8_ENC );
    }

    return true;
}

/* timer function to scroll text */
static void _scroll_timer_event( void *param )
{
    scene_play_t *scene_play = ( scene_play_t* )param;

    //draw scroll bg
    _erase_string( scene_play, scene_play->str_title );
    
    //scroll text    
    sty_draw_scroll_string( scene_play->hdc, scene_play->str_title, scene_play->scroll_info ); 
}

/* bg display */
bool playing_paint_bg( scene_play_t *scene_play )
{
    //draw bg
    sty_draw_bitmap( scene_play->hdc, scene_play->bmp_bg, 
                    scene_play->pic_bg->x, scene_play->pic_bg->y );
    
    //draw disk icon
    sty_draw_picture_res( scene_play->hdc, scene_play->pic_disk );
    
    //draw album bg
    sty_draw_picture_res( scene_play->hdc, scene_play->pic_album_bg );

    //draw artist icon
    sty_draw_picture_res( scene_play->hdc, scene_play->pic_artist );
    
    //draw album icon
    sty_draw_picture_res( scene_play->hdc, scene_play->pic_album );
    
    //draw genre icon
    sty_draw_picture_res( scene_play->hdc, scene_play->pic_genre );
    
    return true;
}

/* file info display */
bool playing_paint_file_info( scene_play_t *scene_play )
{
    char buf[16];
    int total_time;
    int hour, minute, second;
    mengine_config_t *config = &scene_play->engine.config;
    
    //erase config area
    _erase_string( scene_play, scene_play->str_index );
    _erase_string( scene_play, scene_play->str_artist );
    _erase_string( scene_play, scene_play->str_album );
    _erase_string( scene_play, scene_play->str_genre );
    _erase_picregion( scene_play, scene_play->reg_star );
    _erase_string( scene_play, scene_play->str_total_time );
    
    //draw album bitmap
    playing_paint_album( scene_play );

    //draw index & total
    if ( config->file_index < 0 )
    {
        config->file_index = 0;
    }
    if ( config->file_total < 0 )
    {
        config->file_total = 1;
    }
    snprintf( buf, sizeof(buf), "%d/%d", config->file_index+1, config->file_total );
    sty_draw_string_res_ext( scene_play->hdc, scene_play->str_index, buf ); 

    if ( scene_play->id3_info != NULL )
    {
        //draw artist
        sty_draw_string_res_ext( scene_play->hdc, scene_play->str_artist, scene_play->id3_info->artist );
        
        //draw album
        sty_draw_string_res_ext( scene_play->hdc, scene_play->str_album, scene_play->id3_info->album );
        
        //draw genre
        sty_draw_string_res_ext( scene_play->hdc, scene_play->str_genre, scene_play->id3_info->genre );

        //draw star level
        sty_draw_picregion_res( scene_play->hdc, scene_play->reg_star, (unsigned short)scene_play->id3_info->rating );
    }
    
    //draw total time
    total_time = (int)scene_play->engine.file_info.total_time;    
    hour = (int)(total_time / 3600);
    minute = (int)((total_time % 3600) / 60);
    second = (int)(total_time % 60);
    snprintf( buf, sizeof(buf), "%02d:%02d:%02d", hour, minute, second );
    sty_draw_string_res_ext( scene_play->hdc, scene_play->str_total_time, buf );
    
    //process scroll text
    if ( scene_play->id3_info != NULL )
    {
        //try to kill scroll timer
        if ( scene_play->scroll_timer == -1 )
        {
            //erase title
            _erase_string( scene_play, scene_play->str_title );
            
            //draw title
            sty_draw_string_res_ext( scene_play->hdc, scene_play->str_title, scene_play->id3_info->title );
            
            //text scroll process
            sty_delete_scroll_string( scene_play->scroll_info );
            scene_play->scroll_info = sty_create_scroll_string( scene_play->hdc, 
                                        scene_play->str_title, scene_play->id3_info->title );
            if ( scene_play->scroll_info != NULL )
            {
                //start scroll timer
                scene_play->scroll_timer = set_timer( SCROLL_INTERVAL, (timer_proc)_scroll_timer_event, scene_play );
            }
        }
    }
    else
    {
        //erase title
        _erase_string( scene_play, scene_play->str_title );
    }

    return true;
}

/* config display */
bool playing_paint_config( scene_play_t *scene_play )
{
    mengine_config_t *config = &scene_play->engine.config;
    mengine_status_t *eg_status = &scene_play->engine.eg_status;
    int cur_index;

    //erase config area
    _erase_string( scene_play, scene_play->str_effect );
    _erase_picregion( scene_play, scene_play->reg_fast );
    _erase_picregion( scene_play, scene_play->reg_lyric );
    _erase_picregion( scene_play, scene_play->reg_ab );
    _erase_picregion( scene_play, scene_play->reg_playmode );

    //draw audio effect
    cur_index = config->eq_val.eq_mode;
    scene_play->str_effect->str_id = scene_play->str_eq[cur_index]->str_id;
    sty_draw_string_res( scene_play->hdc, scene_play->str_effect );

    //draw fast play icon
    if ( config->play_speed != 0 )
    {
        cur_index = 1;
    }
    else
    {
        cur_index = 0;
    }
    sty_draw_picregion_res( scene_play->hdc, scene_play->reg_fast, (unsigned short)cur_index );

    //draw lyric icon
    if ( config->display_mode == EG_LYRIC )
    {
        cur_index = 1;
    }
    else
    {
        cur_index = 0;
    }
    sty_draw_picregion_res( scene_play->hdc, scene_play->reg_lyric, (unsigned short)cur_index );

    //draw a-b repeat icon
    if ( eg_status->play_status == EG_PLAY_AB )
    {
        switch(eg_status->ab_status)
        {
        case EG_PLAY_AB_NULL:
            cur_index = 0;
            break;
            
        case EG_PLAY_AB_A:
            cur_index = 1;
            break;
            
        case EG_PLAY_AB_B:
            cur_index = 2;
            break;
            
        case EG_PLAY_AB_DO:
            cur_index = 3;
            break;
            
        default:
            break;
        }
    }
    else
    {
        cur_index = 0;
    }    
    sty_draw_picregion_res( scene_play->hdc, scene_play->reg_ab, (unsigned short)cur_index );

    //draw play mode icon
    sty_draw_picregion_res( scene_play->hdc, scene_play->reg_playmode, (unsigned short)config->play_mode );

    return true;
}

/* effect display */
bool playing_paint_effect( scene_play_t *scene_play )
{
    mengine_config_t *config = &scene_play->engine.config;
    mengine_status_t *eg_status = &scene_play->engine.eg_status;
    static display_mode_e s_displaymode = EG_SPECTRUM;
    static unsigned int s_effect_time = 0;
    unsigned int timeout = s_effect_time;

    switch(eg_status->play_status)
    {
    case EG_PLAY_FAST:
        return false;   //unchanged state
        
    case EG_PAUSE:
    case EG_STOP:
        if ( config->display_mode != EG_LYRIC )
        {
            _erase_picture( scene_play, scene_play->pic_effect_bg );  //just erase
            return true;   //unchanged state
        }
        break;
        
    default:
        break;
    }
    
    //dynamic modify mode
    if ( config->display_mode != s_displaymode )
    {
        s_displaymode = config->display_mode;
        scene_play->eff_frame = 0;
    }

    switch ( config->display_mode )
    {
    case EG_SPECTRUM:
        _draw_spectrum( scene_play );
        timeout = 100;
        break;
        
    case EG_LYRIC:
        _draw_lyric( scene_play );
        timeout = 250;
        break;
        
    case EG_EFFECT1:
        _draw_fire( scene_play );
        timeout = 50;
        break;
        
    case EG_EFFECT2:
        _draw_wave( scene_play );
        timeout = 50;
        break;
        
    case EG_EFFECT3:
        //toDo: ---
        break;
        
    default:
        break;
    }

    //dynamic modify timer
    if ( timeout != s_effect_time )
    {
        s_effect_time = timeout;
        modify_timer( scene_play->timer_id, timeout );
    }

    return true;
}

/* progress display */
bool playing_paint_progress( scene_play_t *scene_play )
{
    static int current = 0;
    int total;
    int hour, minute, second;
    char buf[16];    
    int cur_time;
    int total_time;
    sty_rect_t rect;
    int x, y;
    
    //erase progress area
    if ( scene_play->pic_pb_dot != NULL )
    {
        scene_play->pic_pb_dot->x = (short)( ( scene_play->pic_pb_full->x + current )
                                     - ( scene_play->pic_pb_dot->width / 2 ) );
        _erase_picture( scene_play, scene_play->pic_pb_dot );
    }
    
    _erase_picture( scene_play, scene_play->pic_pb_bg );
    _erase_string( scene_play, scene_play->str_cur_time );
    
    //draw progressbar bg
    sty_draw_bitmap( scene_play->hdc, scene_play->bmp_pb_bg,
                    scene_play->pic_pb_bg->x, scene_play->pic_pb_bg->y );
    
    //compute progress bar
    cur_time = (int)(scene_play->engine.music_status.cur_time / 1000);
    total_time = (int)scene_play->engine.file_info.total_time;
    
    if ( cur_time > total_time )
    {
        cur_time = total_time;
        print_err("current time > total time !");
    }
    
    if ( total_time == 0 )
    {
        current = 0;
    }
    else
    {
        total = scene_play->pic_pb_full->width;
        current = ( total * cur_time ) / total_time;
    }
    
    //draw bar slice
    rect.x = 0;
    rect.y = 0;
    rect.width = (short)current;
    rect.height = scene_play->pic_pb_full->height;
    sty_draw_sub_bitmap( scene_play->hdc, scene_play->bmp_pb_full,
                        scene_play->pic_pb_full->x, scene_play->pic_pb_full->y, &rect );

    //draw bar dot
    if ( ( scene_play->pic_pb_dot != NULL ) && ( scene_play->bmp_pb_dot != NULL ) )
    {
        x = ( scene_play->pic_pb_full->x + current )
                 - ( scene_play->pic_pb_dot->width / 2 );
        y = scene_play->pic_pb_dot->y;
        sty_draw_bitmap( scene_play->hdc, scene_play->bmp_pb_dot, x, y );
    }
    
    //draw current time
    hour = cur_time / 3600;
    minute = (cur_time % 3600) / 60;
    second = cur_time % 60;    
    snprintf( buf, sizeof(buf), "%02d:%02d:%02d", hour, minute, second );
    sty_draw_string_res_ext( scene_play->hdc, scene_play->str_cur_time, buf );
    
    return true;
}

/* button display */
bool playing_paint_button( scene_play_t *scene_play )
{
    mengine_status_t *eg_status = &scene_play->engine.eg_status;
    int cur_index, cur_index2;
    
    //erase button area
    _erase_picregion( scene_play, scene_play->reg_prev );
    _erase_picregion( scene_play, scene_play->reg_state );
    _erase_picregion( scene_play, scene_play->reg_next );
    
    //draw prev/next icon
    if ( eg_status->play_status == EG_PLAY_FAST )
    {
        cur_index = 2;
        cur_index2 = 2;
    }
    else
    {
        cur_index = 0;
        cur_index2 = 0;
    }
    
    //play highlight icon
    if ( scene_play->press_prev == true )
    {
        cur_index ++;
    }
    
    if ( scene_play->press_next == true )
    {
        cur_index2 ++;
    }

    sty_draw_picregion_res( scene_play->hdc, scene_play->reg_prev, (unsigned short)cur_index );
    sty_draw_picregion_res( scene_play->hdc, scene_play->reg_next, (unsigned short)cur_index2 );
    
    //draw icon play    
    switch ( eg_status->play_status )
    {
    case EG_PLAY:
    case EG_PLAY_AB:
    case EG_PLAY_FAST:
        cur_index = 0;
        break;
        
    case EG_PAUSE:
        cur_index = 2;
        break;
        
    case EG_STOP:
        cur_index = 4;
        break;
        
    default:
        break;
    }

    //play highlight icon
    if ( scene_play->press_state == true )
    {
        cur_index ++;
    }
    
    sty_draw_picregion_res( scene_play->hdc, scene_play->reg_state, (unsigned short)cur_index );
    
    return true;
}

/* album display */
bool playing_paint_album( scene_play_t *scene_play )
{
    int x, y;
    int w, h;
    gui_bitmap_info_t *bmp = scene_play->album_bmp;
    
    //draw album photo
    if ( scene_play->pic_def_album != NULL )
    {   
        if ( scene_play->album_flag == true )
        {        
            x = scene_play->pic_def_album->x;
            y = scene_play->pic_def_album->y;
            w = scene_play->pic_def_album->width;
            h = scene_play->pic_def_album->height;
            
            //fill white color
            gui_dc_set_background_color( scene_play->hdc, GUI_COLOR_WHITE );
            gui_dc_clear_rect( scene_play->hdc, x, y, (x + w) - 1, (y + h) -1 );
            
            x = x + (( w - bmp->xsize )/2);
            y = y + (( h - bmp->ysize )/2);
            
            //draw bitmap
            gui_dc_draw_bitmap_ext( scene_play->hdc, (unsigned char*)bmp->pdata, 
                                    x, y, (int)bmp->xsize, (int)bmp->ysize, (int)bmp->bytesperpixel );
        }
        else
        {
            //draw default bitmap
            sty_draw_picture_res( scene_play->hdc, scene_play->pic_def_album );
        }
    }

    return true;
}

