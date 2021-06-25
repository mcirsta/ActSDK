
#include "ebook_count_page_buffer.h"
#include "jbook.h"


static FILE* s_file = NULL;
static int s_file_type;
static file_format_e s_file_format;

static bool s_end_of_file;

/* 存放刚从文件读进来的内容 */
static unsigned char s_buffer[ BOOK_BUFFER_LENGTH ];
static unsigned int s_buffer_len;
static unsigned int s_buffer_offset; 

static void close_old_book(void);
static bool open_book_file(void);
static void check_file_type(void);

static file_type_e get_file_type(void);
static int read_file_a_sector(void);

static unsigned char* get_current_char_address(void);
static unsigned char get_current_char(void);
static unsigned char get_index_char( unsigned int cindex );
static void backward_n_char( unsigned short num );
static bool end_of_file(void);


static int _back_read_n_sector( int back_read_num );

static int get_current_reading_point(void);


static count_page_buffer_t s_count_page_buffer_api = 
{
    get_file_type,
    
    close_old_book,
    open_book_file,

    get_current_char_address,
    get_current_char,
    get_index_char,
    backward_n_char,
    end_of_file,

    get_current_reading_point
    
};

static void close_old_book(void)
{
    if ( s_file != NULL )
    {
        if ( s_file_format == TXT_FILE )
        {
            fclose( s_file );
        }
        else  /* if ( s_file_format == JAR_FILE ) */
        {
            jbook_close( s_file );
        }
        
        s_file = NULL;
    }
}

static bool open_book_file(void)
{
    int err;
    char* string = NULL;
    char* file_path = NULL;
    struct stat state;

    file_path = g_scene_read->get_file_path();
    if ( file_path == NULL )
    {
        print_err(" err : open new book : path = NULL !!!  \n" );
        return false;
    }

    print_info(" count page buffer : new book : path = %s \n", file_path );

    /* 判断文件的格式 */
    string = strstr( file_path, LOW_TXT_FILE_FORMAT );
    if ( string != NULL )
    {
        s_file_format = TXT_FILE;
    }
    else
    {
        string = strstr( file_path, TXT_FILE_FORMAT );
        if ( string != NULL )
        {
            s_file_format = TXT_FILE;
        }
        else
        {
            s_file_format = JAR_FILE;
        }
    }

    if ( s_file_format == TXT_FILE )
    {
        s_file = fopen( file_path, READ_ONLY );
    }
    else /* if ( s_file_format == JAR_FILE ) */
    {
        s_file = jbook_open( file_path );
    }

    if( s_file == NULL )
    {
        err = geterror();
        if( err == ENAMETOOLONG )
        {
            print_err("the file name is too long!\n");
        }
        else if(err == ENOMEM )
        {
            print_err("there are no enouth memory\n");
        }
        else
        {
            print_err("physical error!\n");
        }

        return false;
    }

    stat( file_path, &state );
    print_dbg(" file size = %d ", state.st_size );
    if ( state.st_size > MAX_BOOK_LENGTH )
    {        
        return false;
    }

    s_buffer_len = 0;
    s_buffer_offset = -1;
    s_end_of_file = false;

    check_file_type();
    
        
    return true;
}


static void check_file_type(void)
{
    int ret;
    file_type_e type;
    char check[3] = { 0, 0, 0 };

    ret = (int)fread( check, 1, 3,  s_file );

    if ( (check[0] == (char)0xFF) && (check[1] == (char)0xFE) )
    {
        type = TXT_UNICODE_LITTLE;
    }
    else if ((check[0] == (char)0xFE) && (check[1] == (char)0xFF))
    {
        type =  TXT_UNICODE_BIG;
    }
    else if ((check[0] == (char)0xEF) && (check[1] == (char)0xBB) && (check[2] == (char)0xBF) )
    {
        type = TXT_UTF8;
    }
    else
    {
        type = TXT_ASCII_MBCS;
    }

    /* 为了和 scene_read 读文件时的当前读取点一致
        (修改字体，更正页数时需要)，都从文件头开始。 */
    ret = fseek( s_file, 0, SEEK_SET );
    if ( ret == -1 )
    {
        print_err( " fseek error ! " );
    }

    s_file_type = type;
}


static file_type_e get_file_type(void)
{
    return s_file_type;
}

static int read_file_a_sector(void)
{
    memset( s_buffer, 0, BOOK_BUFFER_LENGTH );
    s_buffer_len = fread( s_buffer, 1, ONCE_READ_SIZE, s_file );
    if ( s_buffer_len == 0 )
    {
        s_buffer_offset = -1;
    
//          if ( feof( s_file ) > 0 )
//          {
//              s_end_of_file = true;
//              
//          }

        s_end_of_file = true;
        return false;
    }

    s_buffer_offset = -1;

    return true;
}

static unsigned char* get_current_char_address(void)
{
    return s_buffer + s_buffer_offset;
}

static unsigned char get_current_char(void)
{
    unsigned char current;
    if ( end_of_file() ==  true )
    {
        return NULL_CHAR;
    }

    s_buffer_offset++;
    if ( s_buffer_offset >= s_buffer_len )
    {
        read_file_a_sector();
        if ( end_of_file() ==  true )
        {
            return NULL_CHAR;
        }
        
        s_buffer_offset++;
    }
    
    current = s_buffer[ s_buffer_offset ];

    return current;
}

static unsigned char get_index_char( unsigned int cindex )
{
    return s_buffer[ s_buffer_offset + cindex ]; /* 越界也不会崩溃 */
}

static void backward_n_char( unsigned short num )
{
    int ret;
    int margin = 0;

    if ( s_buffer_offset == -1 ) /* 还未读入任何字符 */
    {
        return;
    }
    
    margin = (int)(num - s_buffer_offset); /* 差额 */
    if ( margin <= 0 )
    {
        /* 还在当前 buffer 以内 */
        s_buffer_offset = s_buffer_offset - num;
    }
    else /* > 0 */
    {    
        /* 在前一次的读取 buffer 中 */

        /* 先回到当前 buffer 的开头 */
        ret = fseek( s_file, (int)(-s_buffer_len), SEEK_CUR );
        if ( ret == -1 )
        {    
            print_err( " fseek error !" );
            return;
        }

        /* 读出前一次的 buffer */
        _back_read_n_sector( 1 );
        
        s_buffer_offset = s_buffer_len - (unsigned int)margin;
    }
}

/* 返回 1 表示读到了文件的开始 */
static int _back_read_n_sector( int back_read_num )
{
    int ret;
    bool start_of_file = false;
    
    ret = fseek( s_file, -( ONCE_READ_SIZE * back_read_num ), SEEK_CUR );
    if ( ret == -1 )
    {    
        ret = fseek( s_file, 0, SEEK_SET );
        
        start_of_file = true;
        if ( ret == -1 )
        {
            print_err( " fseek error !" );
            return 1;
        }
    }

    read_file_a_sector();
    if ( s_end_of_file == true )
    {
        start_of_file = true;
    }


    if ( start_of_file == true )
    {
        return 1;
    }
    
    return 0;
}

static bool end_of_file(void)
{
    return s_end_of_file;
}

static int get_current_reading_point(void)
{
    int ret;
    
    ret = ftell( s_file );
    if ( ret == -1 )
    {
        print_err( " ftell error !" );
        return 0;
    }

    return ( ( ret - (int)s_buffer_len ) + (int)s_buffer_offset );
}


    
count_page_buffer_t* create_count_page_buffer(void)
{
    return &s_count_page_buffer_api;
}


