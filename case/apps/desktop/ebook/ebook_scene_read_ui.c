
#include "ebook.h"
#include "ebook_scene_read_ui.h"

#define TEXT_START_X 10
#define TEXT_START_Y 44
#define TEXT_MAX_WIDTH 220  /* 一行字符的最大长度，显示框的宽度 */

#pragma __PRQA_IGNORE_START__

/* 比 s_font_background_colors 多一项，因为第一项放置缺省字体的颜色 */
static int s_font_colors[ FONT_MAX_COLOR + 1 ] = 
{ 
    GUI_COLOR_BLACK, 
        
    GUI_COLOR_BLACK,
    GUI_COLOR_WHITE,
    GUI_COLOR_LIGHTGRAY,
    GUI_COLOR_BLUE
};

static int s_font_background_colors[ FONT_MAX_COLOR ] = 
{ 
    GUI_COLOR_WHITE, 
    GUI_COLOR_BLACK,
    GUI_COLOR_BLACK,
    GUI_COLOR_WHITE
};

#pragma __PRQA_IGNORE_END__

static int s_hdc = 0;

static scene_t* s_scene = NULL;

//  static int s_fwqy_fontdesc;
//  static int s_ttf_fontdesc;

static picture_resource_t* s_book_picture;
static gui_bitmap_info_t* s_book_picture_bitmap;

static picture_resource_t* s_bar_picture;
static gui_bitmap_info_t* s_bar_picture_bitmap;

static picregion_resource_t* s_hand_auto_picregion;
static string_resource_t* s_filename_string;
static string_resource_t* s_page_string;

/* 是否使用颜色背景；false 表示不使用，只使用背景图片 */
static bool s_use_color_background = true;
/* 在 s_use_color_background 为 true 的前提下，当前是否使用颜色背景 */
static bool s_color_background = false;

static sty_rect_t s_bg_picture_erase_text_rect;

static bool set_hdc( int hdc );
static void set_dc_font_size( int size );
static void set_ui_font_color( unsigned int color );

static bool load_style_resource( scene_t* scene );
static void deinit(void);
    
static bool paint_initial_ui( control_mode_e control_mode, unsigned char* filename );
static bool paint_text_background(void);
static bool paint_text( char* show_buffer, font_layout_t layout, 
                        unsigned int show_lines, file_type_e type, unsigned int language );
static bool paint_control_mode( control_mode_e control_mode );
static void paint_cur_total_page( unsigned char* page_string );

void set_font_colors( unsigned int cindex, int color, int bg_color );
void set_use_color_background( bool use );
bool use_color_background(void);

void _display_string_buf( unsigned char *buf, sty_rect_t *rect, int align, gui_text_encode_t encoding );

static scene_read_ui_t s_scene_read_ui_api =
{
    load_style_resource,
    deinit,
        
    set_hdc,
    set_dc_font_size,
    set_ui_font_color,
    
    paint_initial_ui,
    paint_text_background,
    paint_text,
    paint_control_mode,
    paint_cur_total_page,

    set_font_colors,
    set_use_color_background,
    use_color_background,
};

static bool set_hdc( int hdc )
{
    s_hdc = hdc;
    
//      s_fwqy_fontdesc = gui_create_font("FWQY", 16);
//      if( s_fwqy_fontdesc < 0 )
//      {
//          print_err("%s----%d-----\n",__FILE__,__LINE__); 
//          return false;
//      }
        
    return true;
}

static void set_dc_font_size( int size )
{
    if ( s_hdc != 0 )
    {
        gui_dc_set_font_size( s_hdc, size );
    }
}

static void set_ui_font_color( unsigned int color )
{
    if ( s_hdc != 0 )
    {
        int value = s_font_colors[ color ];
        gui_dc_set_color( s_hdc, value );

        if ( s_use_color_background == true )
        {
            if ( color == DEFAULT_COLOR_INDEX )
            {
                /* 默认颜色 */
                s_color_background = false;
            }
            else
            {
                int bgcolor;
                bgcolor = s_font_background_colors[ color - 1 ];
            
                s_color_background = true;
                
                gui_dc_set_background_color( s_hdc, bgcolor ); 
            }
        }
    }
}

static bool load_style_resource( scene_t* scene )
{
    if ( scene == NULL )
    {
        print_err(" read scene load scene failure\n"); 
        return false;
    }
    else
    {
        string_resource_t* temp;
        resgroup_resource_t* read_resgroup;
        resgroup_resource_t* temp_resgroup;
        
        read_resgroup = (resgroup_resource_t*)g_style_api->get_scene_child( scene, READ_RESGROUP );
        if( read_resgroup == NULL)
        {
            print_err(" read scene load read resgroup failure "); 
            return false;
        }
    
        /* load resource */
        s_book_picture = ( picture_resource_t * )g_style_api->get_resgroup_child( read_resgroup, PIC_BOOK );
        s_bar_picture = ( picture_resource_t * )g_style_api->get_resgroup_child( read_resgroup, PIC_BAR );
        s_hand_auto_picregion = ( picregion_resource_t * )g_style_api->get_resgroup_child( read_resgroup, 
                                                                            REG_HAND_AUTO );

        
        s_filename_string = ( string_resource_t* )g_style_api->get_resgroup_child( read_resgroup, STR_FILE_NAME );
        s_page_string = ( string_resource_t* )g_style_api->get_resgroup_child( read_resgroup, STR_PAGE );
        

        if( ( s_book_picture == NULL ) || ( s_hand_auto_picregion == NULL )
            || ( s_filename_string == NULL ) || ( s_page_string == NULL ) )
        {
            print_err(" read_ui : load style resources failure \n" ); 
            return false;
        }

        s_book_picture_bitmap = g_sty_dc->sty_create_bitmap( s_book_picture->pic_id );
        if ( s_book_picture_bitmap == NULL )
        {
            print_err(" read_ui : sty_create_bitmap failure \n" ); 
            return false;
        }

        if ( s_bar_picture != NULL )
        {
            s_bar_picture_bitmap = g_sty_dc->sty_create_bitmap( s_bar_picture->pic_id );
            if ( s_bar_picture_bitmap == NULL )
            {
                print_err(" read_ui : sty_create_bitmap failure \n" ); 
                return false;
            }
        }

        /* 取得布局相关的配置 */
        temp_resgroup = (resgroup_resource_t*)g_style_api->get_resgroup_child( read_resgroup, LAYOUT_RESGROUP );
        if( temp_resgroup == NULL)
        {
            print_err(" read scene load LAYOUT_RESGROUP failure "); 
            return false;
        }

        /* 字体 布局 */
        temp = ( string_resource_t* )g_style_api->get_resgroup_child( temp_resgroup, STR_FONT_LAYOUT1 );
        if( temp == NULL)
        {
            print_err(" read scene load STR_FONT_LAYOUT1 failure "); 
            return false;
        }
        g_scene_read->set_font_layout( 0, temp->x, temp->y, temp->width, temp->font_height, temp->height, temp->space );

        temp = ( string_resource_t* )g_style_api->get_resgroup_child( temp_resgroup, STR_FONT_LAYOUT2 );
        if( temp == NULL)
        {
            print_err(" read scene load STR_FONT_LAYOUT2 failure "); 
            return false;
        }
        g_scene_read->set_font_layout( 1, temp->x, temp->y, temp->width, temp->font_height, temp->height, temp->space );

        temp = ( string_resource_t* )g_style_api->get_resgroup_child( temp_resgroup, STR_FONT_LAYOUT3 );
        if( temp == NULL)
        {
            print_err(" read scene load STR_FONT_LAYOUT3 failure "); 
            return false;
        }
        g_scene_read->set_font_layout( 2, temp->x, temp->y, temp->width, temp->font_height, temp->height, temp->space );


        /* 取得使用背景图片清除text的区域 */
        temp_resgroup = (resgroup_resource_t*)g_style_api->get_resgroup_child( read_resgroup, ERASE_TEXT_RESGROUP );
        if( temp_resgroup == NULL)
        {
            print_err(" read scene load ERASE_TEXT_RESGROUP failure "); 
            return false;
        }
        s_bg_picture_erase_text_rect.x = temp_resgroup->x;
        s_bg_picture_erase_text_rect.y = temp_resgroup->y;
        s_bg_picture_erase_text_rect.width= temp_resgroup->width;
        s_bg_picture_erase_text_rect.height= temp_resgroup->height;

        /* 取得使用背景图片清除text的区域 */
        temp_resgroup = (resgroup_resource_t*)g_style_api->get_resgroup_child( read_resgroup, FUSION_RECT_RESGROUP );
        if( temp_resgroup == NULL)
        {
            print_err(" read scene load FUSION_RECT_RESGROUP failure "); 
            return false;
        }
        g_scene_read->set_fusion_rect( temp_resgroup->x, temp_resgroup->y,
                                        temp_resgroup->width, temp_resgroup->height );
        
        
    }

    s_scene = scene;

    return true;
}

static bool paint_initial_ui( control_mode_e control_mode, unsigned char* filename )
{
    g_sty_dc->sty_draw_bitmap( s_hdc, s_book_picture_bitmap, s_book_picture->x, s_book_picture->y );

    if ( s_bar_picture != NULL )
    {
        g_sty_dc->sty_draw_bitmap( s_hdc, s_bar_picture_bitmap, s_bar_picture->x, s_bar_picture->y );
    }
    
    g_sty_dc->sty_draw_picregion_res( s_hdc, s_hand_auto_picregion, control_mode );
    g_sty_dc->sty_draw_string_res_ext( s_hdc, s_filename_string, filename );

    return true;
}

static bool paint_text_background(void)
{
    int x;
    int y;
    int x1;
    int y1;
    sty_rect_t rect;

    x = s_book_picture->x + s_bg_picture_erase_text_rect.x;
    y = s_book_picture->y + s_bg_picture_erase_text_rect.y;
        
    rect.x = s_bg_picture_erase_text_rect.x;
    rect.y = s_bg_picture_erase_text_rect.y;
    rect.width = s_bg_picture_erase_text_rect.width;
    rect.height = s_bg_picture_erase_text_rect.height;

    /* 画背景图片的大块, 清掉 text  */
    if ( s_use_color_background == true )
    {
        if ( s_color_background == true )
        {
            
            x1 = ( x + rect.width ) - 1;
            y1 = ( y + rect.height ) - 1;
        
            gui_dc_clear_rect( s_hdc, x, y, x1, y1 ); 
        }
        else
        {
            g_sty_dc->sty_draw_sub_bitmap(s_hdc, s_book_picture_bitmap, x, y, &rect);
        }
    }
    else
    {
        g_sty_dc->sty_draw_sub_bitmap(s_hdc, s_book_picture_bitmap, x, y, &rect);
    }
    

    return true;
}

static bool paint_text( char* show_buffer, font_layout_t layout, unsigned int show_lines, 
                          file_type_e type, unsigned int language )
{
    unsigned int line;
    sty_rect_t rect;
    gui_text_encode_t last_encode;
    gui_encode_e encode;
    
    
    int align;
    rect.x = (short)layout.x;
    rect.width = (short)layout.width;
    rect.height = (short)layout.line_height;


    align = GUI_ALIGN_LEFT | GUI_ALIGN_MIDDLE;

    last_encode = g_sty_dc->sty_get_text_encoding();
    if ( type == TXT_ASCII_MBCS )
    {
        encode = GUI_MBCS_ENCODE;
    
//          g_sty_dc->sty_set_text_encoding( GUI_MBCS_ENCODE );
    }
    else
    {
        encode = GUI_UNICODE_ENCODE;
    
//          g_sty_dc->sty_set_text_encoding( GUI_UNICODE_ENCODE );
    }

    /* paint the text  */
    for ( line = 0; line < show_lines; line++ )
    {
//	        if ( type == TXT_ASCII_MBCS )
//	        {
//	            print_dbg("show txt ===========  %s ",( show_buffer + ( line * LINE_MAX_CHARS ) ) );
//	        }

        rect.y = (short)( (unsigned int)layout.y + ( layout.line_height * line ) );
        
        _display_string_buf( ( show_buffer + ( line * LINE_MAX_CHARS ) ),
                                        &rect, align, (gui_text_encode_t)encode );
    }


    /* 还原 */
//      g_sty_dc->sty_set_text_encoding( last_encode );
    
    return true;
}

static bool paint_control_mode( control_mode_e control_mode )
{
    short int x;
    short int y;
    sty_rect_t rect;

    /* 更换左下角的自动，手动图片 */
    x = s_hand_auto_picregion->x;
    y = s_hand_auto_picregion->y;

    rect.x = x;
    rect.width = s_hand_auto_picregion->width;
    rect.height = s_hand_auto_picregion->height;

    /* 先用背景清空原来的。 */
    rect.y = y - s_book_picture->y;
    
    g_sty_dc->sty_draw_sub_bitmap(s_hdc, s_book_picture_bitmap, x, y, &rect);
    
    if ( s_bar_picture != NULL )
    {
        rect.y = y - s_bar_picture->y;
        g_sty_dc->sty_draw_sub_bitmap(s_hdc, s_bar_picture_bitmap, x, y, &rect);

    }

    /* 画手动，自动图片 */
    g_sty_dc->sty_draw_picregion_res( s_hdc, s_hand_auto_picregion, control_mode );

    return true;
}

static void paint_cur_total_page( unsigned char* page_string )
{
    short int x;
    short int y;
    sty_rect_t rect;

    if ( s_scene == NULL )
    {
        /* count_page_thread 会调用该函数，
         * 在重新进入阅读场景时，可能 ui 的资源还未装载完。
         */
        return;
    }

    x = s_page_string->x;
    y = s_page_string->y;

    rect.x = x;
    rect.y = y;
    rect.width = s_page_string->width;
    rect.height = s_page_string->height;

    
    /* 先用背景清空原来的。 */
    g_sty_dc->sty_draw_sub_bitmap(s_hdc, s_book_picture_bitmap, x, y, &rect);

    if ( s_bar_picture != NULL )
    {
        rect.y = y - s_bar_picture->y;
        g_sty_dc->sty_draw_sub_bitmap(s_hdc, s_bar_picture_bitmap, x, y, &rect);
    }

    g_sty_dc->sty_draw_string_res_ext( s_hdc, s_page_string, page_string );
}

void set_font_colors( unsigned int cindex, int color, int bg_color )
{
    s_font_colors[ cindex ] = color;

    if ( cindex == DEFAULT_COLOR_INDEX )
    {
        return;
    }

    s_font_background_colors[ cindex - 1 ] = bg_color;
}

void set_use_color_background( bool use )
{
    s_use_color_background = use;
}

bool use_color_background(void)
{
    return s_use_color_background;
}

static void deinit(void)
{
    g_sty_dc->sty_delete_bitmap( s_book_picture_bitmap );
    g_sty_dc->sty_delete_bitmap( s_bar_picture_bitmap );

//      gui_destroy_font( s_fwqy_fontdesc );
//      gui_destroy_font( s_ttf_fontdesc );

    s_book_picture = NULL;
    s_book_picture_bitmap = NULL;

    s_bar_picture = NULL;
    s_bar_picture_bitmap = NULL;

    s_hand_auto_picregion = NULL;
    s_filename_string = NULL;
    s_page_string = NULL;
    s_scene = NULL;
}

void _display_string_buf( unsigned char *buf, sty_rect_t *rect, 
                            int align, gui_text_encode_t encoding )
{
    gui_rect_t r;

    if ( buf != NULL )
    {
        r.x0 = rect->x;
        r.y0 = rect->y;
        r.x1 = rect->x + rect->width - 1;
        r.y1 = rect->y + rect->height - 1;

        gui_dc_display_string_in_rect( s_hdc, buf, &r, align, encoding );
        
    }
}

scene_read_ui_t* create_scene_read_ui(void)
{
    return &s_scene_read_ui_api;
}
