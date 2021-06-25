/********************************************************************************
 *                              USDK(1100)
 *                             Module: app
 *                 Copyright(c) 2003-2009 Actions Semiconductor,
 *                            All Rights Reserved.
 * File : ebook_count_page.c
 * History:
 *      <author>    <time>           <version >             <desc>
 *    zhaowenjie    2009-2-25          v1.0               build this file
 ********************************************************************************/
/*!
* \file     ebook_count_page.c
* \brief    read scene
* \author   zhaowenjie
*
* \version 1.0
* \date  2009/02/25
*******************************************************************************/

#include "ebook.h"
#include "ebook_count_page_buffer.h"
#include "ebook_count_page.h"

//  #define DEBUG_READ_CHARS    /* 测试用:输出读取的字符串 */

#define SKIP_POINT_BUFFER_LENGTH 512

static unsigned int s_skip_point_buffer[ SKIP_POINT_BUFFER_LENGTH ];
static unsigned int s_skip_point_buffer_index = 0;
static unsigned int s_cur_page_start_point = 0;

static pthread_t s_pthread_id;
/* 是否已经启动线程 */
static bool s_pthread_start = false;
static bool s_running = false;

static int s_hdc;

static count_page_buffer_t* s_count_page_buffer;


static unsigned int s_total_page = 0;
static unsigned int s_last_total_page = 0;

static unsigned char* s_ascii_width;
static unsigned char* s_font_width_buffer;

static font_layout_t* s_font_layout;

static unsigned int s_show_lines = 0; /* 需要显示的行数 */


static void set_font_layout( font_layout_t* font_layout );

static bool start_count_page( unsigned char* ascii_width, unsigned char* font_width_buffer );
static bool is_running(void);
static void stop_count_page(void);
static void* _count_total_page( void* arg );

static bool _count_next_page(void);
static void _count_next_multi_lines(void);
static bool _count_a_line(void);
static bool _count_a_ascii_line(void);
static bool _count_a_unicode_line(void);
static bool _count_a_utf8_line(void);
static bool _end_of_file( unsigned int chars_sum );

static void _count_page_exit(void);

static char _get_char_width_ext( unsigned int char_add, int encoding );

//  static void _try_update_ui_total_page(void);
static int get_total_page(void);

static void get_skip_point( unsigned int percent, skip_point_t* point );

void set_hdc( int hdc );

#ifdef DEBUG_READ_CHARS

/* 临时存放要显示的字符 */
static unsigned char s_show_buffer[ SHOW_BUFFER_SIZE ];
static void _print_text(void);

#endif

static count_page_t s_count_page_api =
{
    set_hdc,
    set_font_layout,
    
    start_count_page,
    is_running,
    stop_count_page,

    get_total_page,

    get_skip_point
    
};

/* scene entry function */
static bool start_count_page( unsigned char* ascii_width, unsigned char* font_width_buffer )
{
    int ret;
    pthread_attr_t attr;
    struct sched_param param;
    int policy;

    print_dbg(" ************   start count page thread  ************* ");

    s_count_page_buffer->close_old_book();

    if ( ( s_count_page_buffer->open_book_file() ) == false )
    {
        dialog_init_t init;
        string_resource_t* string;
        string = g_ebook_main->get_file_open_fail_string();
        
        init.head = DIALOG_HEAD_FAIL;
        init.str_id = string->str_id;

        dialog_modal( &init );
    
        return false;
    }

    s_ascii_width = ascii_width;
    s_font_width_buffer = font_width_buffer;
    
    /* 设置线程属性 */
    pthread_attr_init(&attr);
    pthread_attr_setinheritsched( &attr, PTHREAD_EXPLICIT_SCHED );
    pthread_attr_setdetachstate( &attr, PTHREAD_CREATE_JOINABLE );

    //动态获取当前线程的优先级属性
    pthread_getschedparam( pthread_self(), &policy, &param);
    param.sched_priority -= 2; /* 低一个级别 */
    if( param.sched_priority < 0 )
    {
        pthread_attr_destroy( &attr );
        return false;
    }

    pthread_attr_setschedparam(&attr, &param);
     
    ret = pthread_create( &s_pthread_id, &attr, _count_total_page, NULL );
    if( ret != 0 )
    {
        print_err( "can’t create thread: %s \n", strerror( ret ) );

        pthread_attr_destroy(&attr);
        return false;
    }

    s_pthread_start = true;

    pthread_attr_destroy(&attr);

//      print_dbg(" ************  end count page thread  ************* ");

    return true;
}

static void* _count_total_page( void* arg )
{
    bool ret;
    
    s_last_total_page = 0;
    s_total_page = 0;
    s_running = true;

    s_skip_point_buffer_index = 0;
    memset( s_skip_point_buffer, 0, SKIP_POINT_BUFFER_LENGTH );

    while( s_running == true )
    {
//          print_dbg(" ************  counting.......  ************* ");

        ret = _count_next_page();
        if ( ret == false )
        {
            s_running = false;  /* 读完了所有页，退出任务。 */            
        }

//	        print_dbg(" total page === %d ", s_total_page );
    }

    _count_page_exit();

    return NULL;
}

static void _count_page_exit(void)
{
    print_dbg(" total page === %d ", s_total_page );

    s_count_page_buffer->close_old_book();
}


static bool _count_next_page(void)
{
//      printf(" show_next_page()  .............................\n ");

    unsigned int new_page_point;

    s_show_lines = 0;

    #ifdef DEBUG_READ_CHARS
    memset( s_show_buffer, 0, SHOW_BUFFER_SIZE );
    #endif

    new_page_point = (unsigned int)( s_count_page_buffer->get_current_reading_point() + 1 ); 
    
    _count_next_multi_lines();

    if ( s_show_lines == s_font_layout->max_line )
    {        
        s_cur_page_start_point = new_page_point;

        /* 保存跳转点 */
        if ( ( s_total_page % SKIP_POINT_DISTANCE )  == 0)
        {
            s_skip_point_buffer[s_skip_point_buffer_index] = s_cur_page_start_point;
            s_skip_point_buffer_index++; 
        }

        #ifdef DEBUG_READ_CHARS
        _print_text();
        #endif

        s_total_page++;

        return true;
    }

    if ( s_show_lines > 0)
    {
        
        s_cur_page_start_point = new_page_point;

        /* 保存跳转点 */
        if ( ( s_total_page % SKIP_POINT_DISTANCE )  == 0)
        {
            s_skip_point_buffer[s_skip_point_buffer_index] = s_cur_page_start_point;
            s_skip_point_buffer_index++; 
        }

        s_total_page++;
    }

    #ifdef DEBUG_READ_CHARS
    _print_text();
    #endif

    return false;
}


static void _count_next_multi_lines(void)
{
    unsigned int line;

    for ( line = 0; line < s_font_layout->max_line; line++ )
    {    
        if ( _count_a_line( ) == false )
        {
            return;
        }
    }
}


static bool _count_a_line(void)
{
    bool result = true;
    file_type_e type = s_count_page_buffer->get_file_type();
    
    if ( type == TXT_ASCII_MBCS)
    {
        result = _count_a_ascii_line( );
    }
    else if ( type == TXT_UTF8)
    {
        result = _count_a_utf8_line( );
    }
    else
    {
        result = _count_a_unicode_line( );
    }
    
    return result;
}


/*  fill_buffer == true 表示是真的读取显示字符。
    fill_buffer == false 用于在向前翻页时，统计行数，所以不能填 s_buffer(越界)。

    返回 true 表示需要继续读下一行；
    返回 false 表示文件已经结束，不能再读了。
*/
    
static bool _count_a_ascii_line(void)
{
    unsigned int chars_sum;
    unsigned int chars_width;
    int word_length;

    unsigned char cur_char;
    unsigned char second_char;
    unsigned char* char_address;
    
    int cur_reading_point;
    int cur_page_start_point;
    
    chars_sum = 0;
    chars_width = 0;        

    #ifdef DEBUG_READ_CHARS
    char* show_buffer;
    int show_offset;
    
    show_offset = 0;
    show_buffer = s_show_buffer + ( s_show_lines * LINE_MAX_CHARS );
    #endif

    #pragma __PRQA_IGNORE_START__
    while ( s_running == true )
    {
        {
            /* 读进新的字符 */
            cur_char = s_count_page_buffer->get_current_char(); /* 取出当前字节 */
            if ( _end_of_file( chars_sum ) == true )
            {
                return false;
            }
            
            char_address = s_count_page_buffer->get_current_char_address(); /* 取出当前字节的地址 */

            cur_reading_point = s_count_page_buffer->get_current_reading_point();
        }
    
        word_length = gui_get_word_length( char_address, (int)g_scene_read->get_language_mode() );
    
        /* 处理换行符 */
        if ( ( word_length == 1 ) && ( cur_char == 0x0D ) )
        {
            cur_page_start_point = g_scene_read->get_current_page_start_point();
    
            if ( cur_reading_point == cur_page_start_point ) 
            {
                g_scene_read->set_current_page_num( s_total_page + 1 );
            }
        
            continue;
        }
        if ( ( word_length == 1 ) && ( cur_char == 0x0A ) ) /* 换行*/
        {           
            cur_page_start_point = g_scene_read->get_current_page_start_point();
    
            if ( cur_reading_point == cur_page_start_point ) 
            {
                g_scene_read->set_current_page_num( s_total_page + 1 );
            }
        
            s_show_lines++;
            return true;
        }
    
    
        /* 是否超出屏幕宽度 */
        if ( word_length == 1 )
        {
            chars_width += s_ascii_width[ cur_char ];
        }
        else
        {
            chars_width += (unsigned int)(s_font_layout->font_size);
        }
    
        if ( chars_width >= s_font_layout->width )
        {
            s_count_page_buffer->backward_n_char( 1 );
    
            s_show_lines++;
            return true;
        }

        /* 成功读到一个字符 */
        chars_sum++;

        if ( word_length == 2 )
        {
            second_char = s_count_page_buffer->get_current_char();
        }

        #ifdef DEBUG_READ_CHARS
        if ( word_length == 1 )
        {
            show_buffer[ show_offset ] = cur_char;
            show_offset++;
        }
        else
        {
            show_buffer[ show_offset++ ] = cur_char;
            show_buffer[ show_offset++ ] = second_char;
        }
        #endif

        /* 改变字体大小后，纠正当前页码 */
        cur_page_start_point = g_scene_read->get_current_page_start_point();
    
        if ( cur_reading_point == cur_page_start_point ) 
        {
            g_scene_read->set_current_page_num( s_total_page + 1 );
//	            g_scene_read->update_cur_total_page();
        }

        if ( word_length == 2 )
        {
            if ( ( cur_reading_point + 1 ) == cur_page_start_point )
            {
                /* 当前字节刚好是双字节字符的第二个字节 ，
                    前移一个字节 */
                g_scene_read->revise_current_page_start_point( cur_page_start_point - 1 );
            
                g_scene_read->set_current_page_num( s_total_page + 1 );
//	                g_scene_read->update_cur_total_page();
            }
        }
                
    } /* while ( s_running == true ) */
    #pragma __PRQA_IGNORE_END__

    return false;
}

static bool _count_a_unicode_line(void)
{
    unsigned char first_char;
    unsigned char second_char;
    unsigned short word = 0;

    unsigned int chars_sum;
    unsigned int chars_width;

    unsigned int cur_reading_point;
    unsigned int cur_page_start_point;
    
    chars_sum = 0;
    chars_width = 0;

    #ifdef DEBUG_READ_CHARS
    char* show_buffer;
    int show_offset;
    
    show_offset = 0;
    show_buffer = s_show_buffer + ( s_show_lines * LINE_MAX_CHARS );
    #endif

    #pragma __PRQA_IGNORE_START__
    while ( s_running == true )
    {        
        first_char = s_count_page_buffer->get_current_char();   /* 取出当前字节 */            
        if ( _end_of_file( chars_sum ) == true )
        {
            return false;
        }

        second_char = s_count_page_buffer->get_current_char();  /* 取出当前字节 */
        if ( _end_of_file( chars_sum ) == true )
        {
            return false;
        }

//              print_dbg(" first == 0x%x , second = 0x%x ", first_char, second_char );

        /* first char 的点 */
        cur_reading_point = (unsigned int)(s_count_page_buffer->get_current_reading_point() - 1);
            


        /* 跳过文件头 bom */
        if ( ( ( first_char == 0xFF) && ( second_char == 0xFE ) )
                    || ( ( first_char == 0xFE ) && ( second_char == 0xFF ) ) )
        {
            continue;
        }

        
        /* 处理换行符 */
        if ( ( ( first_char == 0x0D) && ( second_char == 0x00 ) )
                    || ( ( first_char == 0x00) && ( second_char == 0x0D ) ) )
        {
            cur_page_start_point = (unsigned int)(g_scene_read->get_current_page_start_point());
        
            if ( cur_reading_point == cur_page_start_point )
            {
                g_scene_read->set_current_page_num( s_total_page + 1 );
            }
        
            continue;
        }
        else if ( ( ( first_char == 0x0A ) && ( second_char == 0x00 ) )
                    || ( ( first_char == 0x00) && ( second_char == 0x0A ) ) )
        {
            cur_page_start_point = (unsigned int)(g_scene_read->get_current_page_start_point());
        
            if ( cur_reading_point == cur_page_start_point )
            {
                g_scene_read->set_current_page_num( s_total_page + 1 );
            }
        
            s_show_lines++;
            return true;
        }
        else
        {
        }
    
        /* 是否超出屏幕宽度 */
        if ( s_count_page_buffer->get_file_type() == TXT_UNICODE_LITTLE )
        {
            word = (unsigned short)first_char | ( ( (unsigned short)second_char ) << 8 );
        }
        else if ( s_count_page_buffer->get_file_type() == TXT_UNICODE_BIG )
        {
            word = ( ( (unsigned short)first_char ) << 8 ) | (unsigned short)second_char  ;
        }
        else
        {
        }
       
        chars_width += (unsigned int)_get_char_width_ext( word, GUI_UNICODE_ENCODE );
    
        if ( chars_width >= s_font_layout->width )
        {
            s_count_page_buffer->backward_n_char( 2 );
    
            s_show_lines++;
            return true;
        }
    
        /* 成功读到一个字符 */
        chars_sum++;

        #ifdef DEBUG_READ_CHARS
        show_buffer[ show_offset++ ] = first_char;
        show_buffer[ show_offset++ ] = second_char;
        #endif

        /* 改变字体大小后，纠正当前页码 */
        cur_page_start_point = (unsigned int)(g_scene_read->get_current_page_start_point());
    
        if ( cur_reading_point == cur_page_start_point )
        {
            g_scene_read->set_current_page_num( s_total_page + 1 );
        }
    
    } /* while ( s_running == true ) */
    #pragma __PRQA_IGNORE_END__

    return false;
}

/* 把所有字符都存成两个字节的 unicode 字符来显示。 */
static bool _count_a_utf8_line(void)
{
    unsigned char first_char;
    unsigned char second_char = NULL_CHAR;
    unsigned char third_char = NULL_CHAR;
    
    unsigned int chars_sum = 0;
    
    unsigned int chars_width;
    unsigned int cur_reading_point = 0;
    unsigned int cur_page_start_point;
    
    chars_width = 0;
    
    #ifdef DEBUG_READ_CHARS
    char* show_buffer;
    int show_offset;
    
    show_offset = 0;
    show_buffer = s_show_buffer + ( s_show_lines * LINE_MAX_CHARS );
    #endif

    #pragma __PRQA_IGNORE_START__
    while ( s_running == true )
    {    
        unsigned short word_chars_num;
        unsigned short word = NULL_CHAR;

        first_char = s_count_page_buffer->get_current_char();   /* 取出当前字节 */
        if ( _end_of_file( chars_sum ) == true )
        {
            return false;
        }

        cur_reading_point = (unsigned int)(s_count_page_buffer->get_current_reading_point());           

        /* 跳过文件头 bom */
        if ( ( first_char == 0xEF ) && ( s_count_page_buffer->get_index_char(1) == 0XBB )
                && ( s_count_page_buffer->get_index_char(2) == 0xBF ))
        {
            s_count_page_buffer->get_current_char();
            s_count_page_buffer->get_current_char();
            if ( _end_of_file( chars_sum ) == true )
            {
                return false;
            }
            
            continue;
        }
    
        /* 对不同字节数的字符进行不同的处理，得到字符的宽度 */
        if ( first_char < 0x80 ) /* ASCII 码 */
        {        
            word_chars_num = 1;
            
            /* 处理换行符 */
            if ( first_char == 0x0D )
            {
                continue;
            }
            if ( first_char == 0x0A ) /* 换行*/
            {                  
                cur_page_start_point = (unsigned int)(g_scene_read->get_current_page_start_point());
        
                if ( cur_reading_point == cur_page_start_point )
                {
                    g_scene_read->set_current_page_num( s_total_page + 1 );
                }
            
                s_show_lines++;
                return true;
            }

            /* 先填到 buffer 中 ，也保存成两个字节的 unicode 字符 */
            #ifdef DEBUG_READ_CHARS
            show_buffer[ show_offset++ ] = first_char;
            show_buffer[ show_offset++ ] = 0;
            #endif

            chars_width += s_ascii_width[ first_char ];            
        }
        else if (( first_char & 0xe0) == 0xc0) /* 双字节的字符 */
        {
            word_chars_num = 2;

            /* 取出第二个字节 */
            second_char = s_count_page_buffer->get_current_char();  
            if ( _end_of_file( chars_sum ) == true )
            {
                return false;
            }

            /* 得到实际的 unicode 字符 */
            word = first_char << 6;
            word |= second_char & 0x3F;
            word &= 0x7FF;
            

            /* 先填到 buffer 中 */
            #ifdef DEBUG_READ_CHARS
            show_buffer[show_offset++] = (unsigned char) (word & 0xFF);
            show_buffer[show_offset++] = (unsigned char) (word >> 8);
            #endif
            
            chars_width += (unsigned int)_get_char_width_ext( word, GUI_UNICODE_ENCODE );            
        }
        else if (( first_char & 0xf0) == 0xe0) /* 三字节的字符 */
        {
            word_chars_num = 3;

            /* 取出第二，三个字节 */
            second_char = s_count_page_buffer->get_current_char();
            if ( _end_of_file( chars_sum ) == true )
            {
                return false;
            }

            third_char = s_count_page_buffer->get_current_char();
            if ( _end_of_file( chars_sum ) == true )
            {
                return false;
            }
            
            /* 将三字节的 utf8 转到两个字节的 unicode 字符*/
            word = (unsigned short)( first_char & 0x0F ) << 12;
            word |= ( ( second_char & 0x3F ) << 6);
            word |= ( third_char & 0x3F );
            
            /* 先填到 buffer 中 */
            #ifdef DEBUG_READ_CHARS
            show_buffer[show_offset++] = (unsigned char) (word & 0xFF);
            show_buffer[show_offset++] = (unsigned char) (word >> 8);
            #endif
            
            chars_width += (unsigned int)_get_char_width_ext( word, GUI_UNICODE_ENCODE ); 
        }
        else  /* 其他情况 */
        {
            word_chars_num = 1;
            
            chars_width += s_ascii_width[ first_char ];
        }

        /* 是否宽度超出显示区域的宽度 */
        if ( chars_width >= s_font_layout->width )
        {
            /* 清空已经填进去的字符 */
            #ifdef DEBUG_READ_CHARS
            show_buffer[ show_offset - 2 ] = 0;
            show_buffer[ show_offset - 1 ] = 0;
            #endif
        
            s_count_page_buffer->backward_n_char( word_chars_num );
    
            s_show_lines++;
            return true;
        }

        chars_sum++;

        /* 改变字体大小后，纠正当前页码 */
        cur_page_start_point = (unsigned int)(g_scene_read->get_current_page_start_point());
    
        if ( cur_reading_point == cur_page_start_point )
        {
            print_dbg(" cur_reading_point == cur_page_start_point , point = %d"
                            ,cur_reading_point );
            g_scene_read->set_current_page_num( s_total_page + 1 );
        }
        
    } /* while ( s_running == true ) */
    #pragma __PRQA_IGNORE_END__

    return false;
}

static bool _end_of_file( unsigned int chars_sum )
{
    if ( s_count_page_buffer->end_of_file() == true ) /* 是否已经到达文件结尾 */
    {
        if ( chars_sum > 0 )
        {
            s_show_lines++;
        }
                
        return true;
    }

    return false;
}

static void set_font_layout( font_layout_t* font_layout )
{
    s_font_layout = font_layout;
}

static char _get_char_width_ext( unsigned int char_addr, int encoding )
{
    char* temp_addr;

    if ( char_addr <= 0x00FF )
    {
        return s_ascii_width[ char_addr ];
    }

    if ((char_addr >= 0x3400) && (char_addr <= 0x9FBF))
    {
        /* 中文字等宽高 */
        return s_font_layout->font_size;
    }

    temp_addr = s_font_width_buffer + char_addr;

    if ( *temp_addr != 0)
    {
        return *temp_addr;

    }
    else
    {
        /* 修改绘图字体大小(计算字体宽度需要)*/
//          gui_select_font_size( s_font_layout->font_size );

//          print_dbg(" char_addr === 0x%x ", char_addr );
        
//          *temp_add = (char)gui_get_char_dist_x( char_add, encoding);
        *temp_addr = (char)gui_dc_get_char_dist_x( s_hdc, char_addr, encoding );

//          print_dbg(" -============================= ", char_addr );
        
        return *temp_addr;
    }

}

static int get_total_page(void)
{
    return s_total_page;
}

static void stop_count_page(void)
{
    int ret;

    /* 退出数页 */
    s_running = false;

    /* 还未启动 */
    if ( s_pthread_start == false )
    {
        return;
    }

    /* 挂起直到该线程结束。 */
    ret = pthread_join( s_pthread_id, NULL );
    if ( ret != 0 )
    {
        print_err("pthread_join( s_pthread_id, NULL ) error !");
    }

    s_pthread_start = false;
}

static void get_skip_point( unsigned int percent, skip_point_t* point )
{
    unsigned int page;
    unsigned int pindex;
    unsigned int distance;

    if ( percent > 100 )
    {
        percent = 100;
    }
    
    page = ( percent * s_total_page ) / 100;
    if ( page == 0 )
    {
        page = 1;
    }
    
    distance = page % SKIP_POINT_DISTANCE;
  
    pindex = page / SKIP_POINT_DISTANCE;    
    point->skip_point = s_skip_point_buffer[ pindex ];

    
    page = ( page / SKIP_POINT_DISTANCE ) * SKIP_POINT_DISTANCE;

    point->skip_point_page = page;
    point->page_distance = distance;
}

void set_hdc( int hdc )
{
    s_hdc = hdc;
}

static bool is_running(void)
{
    return s_running;
}

#ifdef DEBUG_READ_CHARS

static void _print_text(void)
{
    int line;
    for ( line = 0; line < s_show_lines; line++ )
    {
        print_dbg("  count page txt ===========  %s ",( s_show_buffer + line * LINE_MAX_CHARS ) );
    }
}

#endif

count_page_t* create_count_page(void)
{
    s_count_page_buffer = create_count_page_buffer();

    return &s_count_page_api;
}

