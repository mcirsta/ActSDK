/********************************************************************************
 *                              USDK(1100)
 *                             Module: app
 *                 Copyright(c) 2003-2009 Actions Semiconductor,
 *                            All Rights Reserved.
 * File : desktop_parser.c
 * History:
 *      <author>    <time>           <version >             <desc>
 *      chencm    2009-04-22          v1.0               build this file
 ********************************************************************************/
/*!
 * \file     desktop_parser.c
 * \brief    parse desktop file
 * \author   chencm
 *
 * \version 1.0
 * \date  2009/04/22
 *******************************************************************************/

#include <include_case.h>
#include <include_psp.h>

//#define FILE_SUFFIX ".desktop"
#define SKIP_SPACE(str) while( ( (*(str) == ' ') || (*(str) == '\t') ) && ( *(str) != '\0') ){ (str)++;}
#define SKIP_BACK_SPACE(begin, str) while( ( (*(str) == ' ') || ( *(str) == '\t') ) && ( (str) > (begin) ) ){ (str)--;}
#define SKIP_TO_NEXT_LINE(str) \
    do \
{ \
    while( (*(str) != '\0') && (*(str) != '\n') ){ (str)++;} \
    while( (*(str) == '\r') || (*(str) == '\n') ){ (str)++;} \
}while(0);

#define DP_STRDUP_TRIM(p) strdup_trim(p)
#define DP_FREE(p) if((p)!=NULL){free(p);(p)=NULL;}

static char* strndup(const char* str, unsigned int length)
{
    char* str_new = (char*) malloc(length + 1);

    if (str_new != NULL)
    {
        strncpy(str_new, str, length);
        str_new[length] = '\0';
    }

    return str_new;
}

static inline char* strdup_trim(const char* str)
{
    unsigned int i = 0;
    char* new_str = NULL;
    const char* start = str;

    if (start != NULL)
    {
        SKIP_SPACE(start)

        if( *start != '\0')
        {
            new_str = strdup(start);
            if (new_str != NULL)
            {
                i = strlen(new_str) - 1;

                while ( ( (new_str[i] == ' ') || (new_str[i] == '\t') ) && (i > 0))
                {
                    i--;
                }

                new_str[i + 1] = '\0';
            }
            else
            {
                print_err();
            }
        }
    }
    return new_str;
}


char* desktop_parse_group(char* group_ptr)
{
    while ( (*group_ptr != ']') && (*group_ptr != '\0') )
    {
        group_ptr++;
    }

    SKIP_TO_NEXT_LINE(group_ptr) //here it is no need to parse the group
    return group_ptr;
}

char* desktop_parse_comment(char* comment_ptr)
{
    SKIP_TO_NEXT_LINE(comment_ptr) //here it is no need to parse the comment
    return comment_ptr;
}

char* desktop_parse_key(char* key_ptr, desktop_pair_t* current_pair)
{
    char* cur = NULL;
    char* begin = NULL;
    char* end = NULL;

    cur = key_ptr;
    //save key
    SKIP_SPACE(cur)
    begin = cur;

    while ( ( (*cur != '\n') && (*cur != '=') ) && (*cur != '\0') )
    {
        cur++;
    }

    end = cur-1;    //ignore '='
    SKIP_BACK_SPACE(begin,end)
    current_pair->key = strndup(begin, (unsigned int)(end - begin+1));

    //save value
    if (*cur != '\0')
    {
        cur++; //now, *cur is '=' or '\n',so need to ignore '=' or '\n'.

        SKIP_SPACE(cur)
        begin = cur;

        while ( (*cur != '\r') && (*cur != '\n') && (*cur != '\0') && (*cur != '#') )
        {
            cur++;
        }

        end = cur-1;    //ignore '#'
        SKIP_BACK_SPACE(begin,end)
        current_pair->value = strndup(begin, (unsigned int)(end - begin+1));
    }
    return cur;
}

desktop_pair_t* desktop_parse_buffer(char* buffer)
{
    char* ptr = NULL;
    desktop_pair_t* head = NULL;
    desktop_pair_t* tail = NULL;
    desktop_pair_t* parse_data = NULL;

    if(buffer == NULL)
    {
        return NULL;
    }

    ptr = buffer;

    while (*ptr != '\0')
    {
        if ( (*ptr == '\n') || (*ptr == '\r') )
        {
            SKIP_TO_NEXT_LINE(ptr)
        }
        else if (*ptr == '[')
        {
            ptr = desktop_parse_group(ptr);
        }
        else if (*ptr == '#')
        {
            ptr = desktop_parse_comment(ptr);
        }
        else
        {
            parse_data = (desktop_pair_t*) malloc( sizeof(desktop_pair_t) );

            if (parse_data == NULL)
            {
                print_err("malloc failed! ");
                continue;
            }

            /*
               parse_data->next = head; //FILO list
               head = parse_data;
               ptr = desktop_parse_key(ptr, parse_data);
               */                  
            parse_data->next = NULL;  

            if (head != NULL)
            {
                tail->next = parse_data;
            }
            else
            {
                head = parse_data;  //record the first node to the list head
            }

            tail = parse_data;
            ptr = desktop_parse_key(ptr, parse_data);
        }
    }

    return head;
}

desktop_parser_handle desktop_parser_create(char* filename)
{
    FILE* fp = NULL;
    desktop_info_t* handle = NULL;
    char* buffer = NULL;
    unsigned int length = 0;

    if(filename == NULL)
    {
        return NULL;
    }

    fp = fopen(filename, "rb");
    if ( fp == NULL)
    {
        print_err("fopen(%s) failed! ", filename);
        return NULL;
    }

    handle = (desktop_info_t*) malloc(sizeof(desktop_info_t));
    if (handle == NULL)
    {
        print_err("malloc faile!");
        goto malloc_handle_err;
    }

    handle->filename = strndup(filename, strlen(filename));
    fseek(fp, 0, SEEK_END);
    length = (unsigned int)ftell(fp);
    fseek(fp, 0, SEEK_SET);
    buffer = (char*) malloc(length + 1);

    if (buffer == NULL)
    {
        print_err("malloc faile!");
        goto malloc_buffer_err;
    }

    fread(buffer, length, 1, fp);
    buffer[length] = '\0';

    handle->pairs = desktop_parse_buffer(buffer);

    if (handle->pairs == NULL)
    {
        print_err("parse buffer failed!");
        goto parse_buffer_err;
    }

    free(buffer);

    fclose(fp);

    return (desktop_parser_handle)handle;

    /* <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<< goto! <<<<<<<<<<<<<<<< goto! <<<<<<<< goto! <<<< goto! << */
    parse_buffer_err:
    free(buffer);

    /* <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<< goto! <<<<<<<<<<<<<<<< goto! <<<<<<<< goto! <<<< goto! << */
    malloc_buffer_err:
    if(handle->filename != NULL)
    {
        free(handle->filename);
    }
    free(handle);

    /* <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<< goto! <<<<<<<<<<<<<<<< goto! <<<<<<<< goto! <<<< goto! << */
    malloc_handle_err:
    fclose(fp);

    return NULL;
}

char * desktop_parser_get_key(desktop_parser_handle handle, char* user_key)
{
    desktop_info_t * parser = (desktop_info_t *) handle;
    desktop_pair_t * cur_pair = NULL;

    const char * suffix = FILE_SUFFIX;

    if (parser == NULL)
    {
        print_err("parser handle is NULL!");
    }
    else
    {
        //check the filename suffix in the handle
        unsigned int offset = strlen(parser->filename) - strlen(suffix);
        if (strcmp((const char *)(&(parser->filename[offset])), suffix) != 0)
        {
            print_err("Invalid parser handle!");
        }
        else
        {
            //find the user_key in the pairs list
            cur_pair = parser->pairs;
            while (cur_pair != NULL)
            {
                if ( strcmp(cur_pair->key, user_key) != 0)
                {
                    cur_pair = cur_pair->next;
                }
                else //find the user_key
                {
                    return cur_pair->value;
                }
            }
        }
    }
    return NULL;
}

bool desktop_parser_delete(desktop_parser_handle handle)
{
    bool ret = false;
    desktop_info_t* cur = (desktop_info_t*) handle;
    desktop_pair_t* temp = NULL;

    if (cur == NULL)
    {
        print_err("Can not destroy NULL handle!");
    }
    else
    {
        //        temp = cur->pairs;
        while (cur->pairs != NULL)
        {
            temp = cur->pairs->next;
            DP_FREE(cur->pairs->key)
            DP_FREE(cur->pairs->value)
            DP_FREE(cur->pairs)
            cur->pairs = temp;
        }

        cur = (desktop_info_t*) handle;
        DP_FREE(cur->filename)
        DP_FREE(cur)
        ret = true;
    }
    return ret;
}
