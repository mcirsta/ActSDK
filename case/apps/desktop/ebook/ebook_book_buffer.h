
#ifndef __EBOOK_BOOK_BUFFER_H__
#define __EBOOK_BOOK_BUFFER_H__

#include "ebook.h"

typedef struct
{
//	    void (*const set_file_path )( char* full_path );
    file_type_e (*const get_file_type ) (void);
    
    void (*const close_old_book )(void);
    open_file_result_e (*const open_book_file )(void);
    
    unsigned char* (*const get_current_char_address )(void);
    unsigned char (*const get_current_char )(void);
    unsigned char (*const get_index_char)( unsigned int index );
    void (*const backward_n_char)( unsigned short num );
    bool (*const end_of_file )(void);

    int (*const get_current_reading_point )(void);
    int (*const find_prev_newline )(unsigned short back_read_num);
    bool (*const set_file_current_point )( int point );

//	    bool (*const is_reading )(void);
    
}book_buffer_t;

book_buffer_t* create_book_buffer(void);

#endif /* __EBOOK_BOOK_BUFFER_H__ */

