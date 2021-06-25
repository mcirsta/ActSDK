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

//  #define DEBUG_READ_CHARS    /* ������:�����ȡ���ַ��� */

#define SKIP_POINT_BUFFER_LENGTH 512

static unsigned int s_skip_point_buffer[ SKIP_POINT_BUFFER_LENGTH ];
static unsigned int s_skip_point_buffer_index = 0;
static unsigned int s_cur_page_start_point = 0;

static pthread_t s_pthread_id;
/* �Ƿ��Ѿ������߳� */
static bool s_pthread_start = false;
static bool s_running = false;

static int s_hdc;

static count_page_buffer_t* s_count_page_buffer;


static unsigned int s_total_page = 0;
static unsigned int s_last_total_page = 0;

static unsigned char* s_ascii_width;
static unsigned char* s_font_width_buffer;

static font_layout_t* s_font_layout;

static unsigned int s_show_lines = 0; /* ��Ҫ��ʾ������ */


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

/* ��ʱ���Ҫ��ʾ���ַ� */
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
    
    /* �����߳����� */
    pthread_attr_init(&attr);
    pthread_attr_setinheritsched( &attr, PTHREAD_EXPLICIT_SCHED );
    pthread_attr_setdetachstate( &attr, PTHREAD_CREATE_JOINABLE );

    //��̬��ȡ��ǰ�̵߳����ȼ�����
    pthread_getschedparam( pthread_self(), &policy, &param);
    param.sched_priority -= 2; /* ��һ������ */
    if( param.sched_priority < 0 )
    {
        pthread_attr_destroy( &attr );
        return false;
    }

    pthread_attr_setschedparam(&attr, &param);
     
    ret = pthread_create( &s_pthread_id, &attr, _count_total_page, NULL );
    if( ret != 0 )
    {
        print_err( "can��t create thread: %s \n", strerror( ret ) );

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
            s_running = false;  /* ����������ҳ���˳����� */            
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

        /* ������ת�� */
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

        /* ������ת�� */
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


/*  fill_buffer == true ��ʾ����Ķ�ȡ��ʾ�ַ���
    fill_buffer == false ��������ǰ��ҳʱ��ͳ�����������Բ����� s_buffer(Խ��)��

    ���� true ��ʾ��Ҫ��������һ�У�
    ���� false ��ʾ�ļ��Ѿ������������ٶ��ˡ�
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
            /* �����µ��ַ� */
            cur_char = s_count_page_buffer->get_current_char(); /* ȡ����ǰ�ֽ� */
            if ( _end_of_file( chars_sum ) == true )
            {
                return false;
            }
            
            char_address = s_count_page_buffer->get_current_char_address(); /* ȡ����ǰ�ֽڵĵ�ַ */

            cur_reading_point = s_count_page_buffer->get_current_reading_point();
        }
    
        word_length = gui_get_word_length( char_address, (int)g_scene_read->get_language_mode() );
    
        /* �����з� */
        if ( ( word_length == 1 ) && ( cur_char == 0x0D ) )
        {
            cur_page_start_point = g_scene_read->get_current_page_start_point();
    
            if ( cur_reading_point == cur_page_start_point ) 
            {
                g_scene_read->set_current_page_num( s_total_page + 1 );
            }
        
            continue;
        }
        if ( ( word_length == 1 ) && ( cur_char == 0x0A ) ) /* ����*/
        {           
            cur_page_start_point = g_scene_read->get_current_page_start_point();
    
            if ( cur_reading_point == cur_page_start_point ) 
            {
                g_scene_read->set_current_page_num( s_total_page + 1 );
            }
        
            s_show_lines++;
            return true;
        }
    
    
        /* �Ƿ񳬳���Ļ��� */
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

        /* �ɹ�����һ���ַ� */
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

        /* �ı������С�󣬾�����ǰҳ�� */
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
                /* ��ǰ�ֽڸպ���˫�ֽ��ַ��ĵڶ����ֽ� ��
                    ǰ��һ���ֽ� */
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
        first_char = s_count_page_buffer->get_current_char();   /* ȡ����ǰ�ֽ� */            
        if ( _end_of_file( chars_sum ) == true )
        {
            return false;
        }

        second_char = s_count_page_buffer->get_current_char();  /* ȡ����ǰ�ֽ� */
        if ( _end_of_file( chars_sum ) == true )
        {
            return false;
        }

//              print_dbg(" first == 0x%x , second = 0x%x ", first_char, second_char );

        /* first char �ĵ� */
        cur_reading_point = (unsigned int)(s_count_page_buffer->get_current_reading_point() - 1);
            


        /* �����ļ�ͷ bom */
        if ( ( ( first_char == 0xFF) && ( second_char == 0xFE ) )
                    || ( ( first_char == 0xFE ) && ( second_char == 0xFF ) ) )
        {
            continue;
        }

        
        /* �����з� */
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
    
        /* �Ƿ񳬳���Ļ��� */
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
    
        /* �ɹ�����һ���ַ� */
        chars_sum++;

        #ifdef DEBUG_READ_CHARS
        show_buffer[ show_offset++ ] = first_char;
        show_buffer[ show_offset++ ] = second_char;
        #endif

        /* �ı������С�󣬾�����ǰҳ�� */
        cur_page_start_point = (unsigned int)(g_scene_read->get_current_page_start_point());
    
        if ( cur_reading_point == cur_page_start_point )
        {
            g_scene_read->set_current_page_num( s_total_page + 1 );
        }
    
    } /* while ( s_running == true ) */
    #pragma __PRQA_IGNORE_END__

    return false;
}

/* �������ַ�����������ֽڵ� unicode �ַ�����ʾ�� */
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

        first_char = s_count_page_buffer->get_current_char();   /* ȡ����ǰ�ֽ� */
        if ( _end_of_file( chars_sum ) == true )
        {
            return false;
        }

        cur_reading_point = (unsigned int)(s_count_page_buffer->get_current_reading_point());           

        /* �����ļ�ͷ bom */
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
    
        /* �Բ�ͬ�ֽ������ַ����в�ͬ�Ĵ����õ��ַ��Ŀ�� */
        if ( first_char < 0x80 ) /* ASCII �� */
        {        
            word_chars_num = 1;
            
            /* �����з� */
            if ( first_char == 0x0D )
            {
                continue;
            }
            if ( first_char == 0x0A ) /* ����*/
            {                  
                cur_page_start_point = (unsigned int)(g_scene_read->get_current_page_start_point());
        
                if ( cur_reading_point == cur_page_start_point )
                {
                    g_scene_read->set_current_page_num( s_total_page + 1 );
                }
            
                s_show_lines++;
                return true;
            }

            /* ��� buffer �� ��Ҳ����������ֽڵ� unicode �ַ� */
            #ifdef DEBUG_READ_CHARS
            show_buffer[ show_offset++ ] = first_char;
            show_buffer[ show_offset++ ] = 0;
            #endif

            chars_width += s_ascii_width[ first_char ];            
        }
        else if (( first_char & 0xe0) == 0xc0) /* ˫�ֽڵ��ַ� */
        {
            word_chars_num = 2;

            /* ȡ���ڶ����ֽ� */
            second_char = s_count_page_buffer->get_current_char();  
            if ( _end_of_file( chars_sum ) == true )
            {
                return false;
            }

            /* �õ�ʵ�ʵ� unicode �ַ� */
            word = first_char << 6;
            word |= second_char & 0x3F;
            word &= 0x7FF;
            

            /* ��� buffer �� */
            #ifdef DEBUG_READ_CHARS
            show_buffer[show_offset++] = (unsigned char) (word & 0xFF);
            show_buffer[show_offset++] = (unsigned char) (word >> 8);
            #endif
            
            chars_width += (unsigned int)_get_char_width_ext( word, GUI_UNICODE_ENCODE );            
        }
        else if (( first_char & 0xf0) == 0xe0) /* ���ֽڵ��ַ� */
        {
            word_chars_num = 3;

            /* ȡ���ڶ��������ֽ� */
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
            
            /* �����ֽڵ� utf8 ת�������ֽڵ� unicode �ַ�*/
            word = (unsigned short)( first_char & 0x0F ) << 12;
            word |= ( ( second_char & 0x3F ) << 6);
            word |= ( third_char & 0x3F );
            
            /* ��� buffer �� */
            #ifdef DEBUG_READ_CHARS
            show_buffer[show_offset++] = (unsigned char) (word & 0xFF);
            show_buffer[show_offset++] = (unsigned char) (word >> 8);
            #endif
            
            chars_width += (unsigned int)_get_char_width_ext( word, GUI_UNICODE_ENCODE ); 
        }
        else  /* ������� */
        {
            word_chars_num = 1;
            
            chars_width += s_ascii_width[ first_char ];
        }

        /* �Ƿ��ȳ�����ʾ����Ŀ�� */
        if ( chars_width >= s_font_layout->width )
        {
            /* ����Ѿ����ȥ���ַ� */
            #ifdef DEBUG_READ_CHARS
            show_buffer[ show_offset - 2 ] = 0;
            show_buffer[ show_offset - 1 ] = 0;
            #endif
        
            s_count_page_buffer->backward_n_char( word_chars_num );
    
            s_show_lines++;
            return true;
        }

        chars_sum++;

        /* �ı������С�󣬾�����ǰҳ�� */
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
    if ( s_count_page_buffer->end_of_file() == true ) /* �Ƿ��Ѿ������ļ���β */
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
        /* �����ֵȿ�� */
        return s_font_layout->font_size;
    }

    temp_addr = s_font_width_buffer + char_addr;

    if ( *temp_addr != 0)
    {
        return *temp_addr;

    }
    else
    {
        /* �޸Ļ�ͼ�����С(������������Ҫ)*/
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

    /* �˳���ҳ */
    s_running = false;

    /* ��δ���� */
    if ( s_pthread_start == false )
    {
        return;
    }

    /* ����ֱ�����߳̽����� */
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

