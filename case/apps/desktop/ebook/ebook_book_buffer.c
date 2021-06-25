
#include "ebook_book_buffer.h"
#include "jbook.h"

static FILE* s_file = NULL;
static file_format_e s_file_format;
static int s_file_type;
static bool s_end_of_file;

/* ��Ÿմ��ļ������������� */
static unsigned char s_buffer[ BOOK_BUFFER_LENGTH ];
static unsigned int s_buffer_len;
static unsigned int s_buffer_offset; 

static void close_old_book(void);
static open_file_result_e open_book_file(void);
static void check_file_type(void);

static file_type_e get_file_type(void);
static int read_file_a_sector(void);

static unsigned char* get_current_char_address(void);
static unsigned char get_current_char(void);
static unsigned char get_index_char( unsigned int cindex );
static void backward_n_char( unsigned short num );
static bool end_of_file(void);

static int get_current_reading_point(void);
static int find_prev_newline( unsigned short back_read_num );
static int _back_read_n_sector( unsigned short num );
static bool set_file_current_point( int point );

static book_buffer_t s_book_buffer_api = 
{
    get_file_type,
    
    close_old_book,
    open_book_file,
    
    get_current_char_address,
    get_current_char,
    get_index_char,
    backward_n_char,
    end_of_file,

    get_current_reading_point,
    find_prev_newline,
    set_file_current_point
};

static void close_old_book(void)
{
    if ( s_file != NULL )
    {
        if ( s_file_format == TXT_FILE )
        {
            fclose( s_file );
        }
        else
        {
            jbook_close( s_file );
        }
           
        s_file = NULL;
    }
}

static open_file_result_e open_book_file(void)
{
    int err;
    int fd;
    char* string = NULL;
    char* file_path = NULL;
    struct stat state;

    file_path = g_scene_read->get_file_path();
    if ( file_path == NULL )
    {
        print_err(" err : open new book : path = NULL !!!  \n" );
        return false;
    }

    print_info(" new book : path = %s \n", file_path );

    /* �ж��ļ��ĸ�ʽ */
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
//          print_dbg(" fopen( file_path, READ_ONLY ); ");
        s_file = fopen( file_path, READ_ONLY );
    }
    else
    {
//          print_dbg(" jbook_open( file_path ); ");
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

        return OPEN_FAIL;
    }

    stat( file_path, &state );
    print_dbg(" file size = %d ", state.st_size );
    if ( state.st_size > MAX_BOOK_LENGTH )
    {        
        return TOO_LARGE;
    }

    /* ��߶�д�ٶ� */
    fd = fileno( s_file );
    fcntl( fd, F_BUILDINDEX, 0 );

    s_buffer_len = 0;
    s_buffer_offset = -1;
    s_end_of_file = false;

    check_file_type( );
    
        
    return OPEN_SUCCESS;
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
    
//          if ( feof( s_file ) )
//          {
//              s_end_of_file = true;
//              
//          }

        /* ����u�̱��ε�����������Ҳ�鵽�������� */
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

static void backward_n_char( unsigned short num )
{
    int ret;
    int margin = 0;

    if ( s_buffer_offset == -1 ) /* ��δ�����κ��ַ� */
    {
        return;
    }

    
    margin = (int)(num - s_buffer_offset); /* ��� */
    if ( margin <= 0 )
    {
        /* ���ڵ�ǰ buffer ���� */
        s_buffer_offset = s_buffer_offset - num;
    }
    else /* > 0 */
    {    
        /* ��ǰһ�εĶ�ȡ buffer �� */

        /* �Ȼص���ǰ buffer �Ŀ�ͷ */
        ret = fseek( s_file, (int)(-s_buffer_len), SEEK_CUR );
        if ( ret == -1 )
        {    
            print_err( " fseek error !" );
        }

        /* ����ǰһ�ε� buffer */
        _back_read_n_sector( 1 );
        
        s_buffer_offset = s_buffer_len - ( unsigned int )margin;
    }
}

static unsigned char get_index_char( unsigned int cindex )
{
    return s_buffer[ s_buffer_offset + cindex ]; /* Խ��Ҳ������� */
}

static bool end_of_file(void)
{
    return s_end_of_file;
}

static bool set_file_current_point( int point )
{
    int ret;
    
    ret = fseek( s_file, point, SEEK_SET );
    if ( ret == -1 )
    {        
        return false;
    }

    s_buffer_len = 0;
    s_buffer_offset = -1;
    s_end_of_file = false;

    return true;
}


static int find_prev_newline( unsigned short back_read_num )
{
    int ret;
    s_end_of_file = false;

    while ( 1 )
    {
        ret = _back_read_n_sector( back_read_num );
        if ( ret == 1 )
        {
            /* �ص����ļ��Ŀ�ͷ */
            
            return -1;
        }

        #pragma __PRQA_IGNORE_START__
        /* �ҵ����з� */
        for ( s_buffer_offset = 0 ; s_buffer_offset < ( s_buffer_len - 1 ); s_buffer_offset++ ) 
        {
//              if ( ( s_buffer[ s_buffer_offset ] == 0x0a ) ||
//                      ( s_buffer[ s_buffer_offset ] == 0x00 ) )
//              {
//                  print_dbg( "book buffer : 0x%x ", s_buffer[ s_buffer_offset ] );
//              }
        
            if (( s_file_type == TXT_ASCII_MBCS)
                        || ( s_file_type == TXT_UTF8))
            {
                if ( s_buffer[ s_buffer_offset ] == 0x0a) //0x0a�ǻ��з���0x0d�ǻس���
                {
//                      s_buffer_offset = s_buffer_offset + 1; /* �������з� */
                    break;
                }
            }
            else if ( s_file_type == TXT_UNICODE_LITTLE)
            {
                if (( s_buffer[ s_buffer_offset ] == 0x0a )
                        && ( s_buffer[ s_buffer_offset + 1] == 0x00 ))
                {
                    s_buffer_offset = s_buffer_offset + 1; /* �������з� */
                    break;
                }
            }
            else  /* if ( s_file_type == TXT_UNICODE_BIG)*/
            {
                if (( s_buffer[ s_buffer_offset ] == 0x00)
                        && ( s_buffer[ s_buffer_offset + 1] == 0x0a))
                {
                    s_buffer_offset = s_buffer_offset + 1; /* �������з� */
                    break;
                }
            }
            
        } /* for */
        #pragma __PRQA_IGNORE_END__

        if ( s_buffer_offset < ( s_buffer_len - 1 ) )
        {
            /* �ҵ����з� */
            break;
        }

        back_read_num++;
    }

    return get_current_reading_point();
}


/* ���� 1 ��ʾ�������ļ��Ŀ�ʼ */
static int _back_read_n_sector( unsigned short back_read_num )
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
            return -1;
        }
    }

    ret = ftell( s_file );
    if ( ( ret > 0 ) && ( ret <= 3 ) )  /* �պ��� bom �� */
    {
        ret = fseek( s_file, 0, SEEK_SET );
        
        start_of_file = true;
        if ( ret == -1 )
        {
            print_err( " fseek error !" );
            return -1;
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


static int get_current_reading_point(void)
{
    int ret;
    
    ret = ftell( s_file );
    if ( ret == -1 )
    {
        print_err( " ftell error !" );
        return 0;
    }

    print_dbg(" ftell  = %d, s_buffer_len = %d, s_buffer_offset = %d ",
                ret, s_buffer_len, s_buffer_offset );

    return ( ( ret - (int)s_buffer_len ) + (int)s_buffer_offset );
}
    
book_buffer_t* create_book_buffer(void)
{
    return &s_book_buffer_api;
}


