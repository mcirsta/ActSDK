
#ifndef __EBOOK_COUNT_PAGE_BUFFER_H__
#define __EBOOK_COUNT_PAGE_BUFFER_H__

#include "ebook.h"

typedef struct
{
    file_type_e (*const get_file_type )(void);
    
    void (*const close_old_book )(void);
    bool (*const open_book_file )(void);
    
    unsigned char* (*const get_current_char_address )(void);
    unsigned char (*const get_current_char )(void);
    unsigned char (*const get_index_char)( unsigned int index );
    void (*const backward_n_char)( unsigned short num );
    bool (*const end_of_file )(void);

    int (*const get_current_reading_point)(void);
    
}count_page_buffer_t;

count_page_buffer_t* create_count_page_buffer(void);

#endif /* __EBOOK_COUNT_PAGE_BUFFER_H__ */

