/********************************************************************************
 *                              USDK(1100)
 *                             Module: app 
 *                 Copyright(c) 2003-2008 Actions Semiconductor,
 *                            All Rights Reserved. 
 * file : utilities.c
 * History:         
 *      <author>    <time>           <version >             <desc>
 *      Herb Hu    2009-5-6             v1.0             build this file 
 ********************************************************************************/

#include <include_case.h>
#include <include_psp.h>


void print_buf(const char * buf, unsigned int size)
{
    unsigned int r, offset;
    unsigned int rows = size / 16;
    unsigned int remain = size % 16;

    for(r = 0 ; r < rows; r++)
    {
        printf("%04x  ", r*16);
        for(offset  = 0 ; offset < 16 ; offset++)
        {
            printf("%02x ", (unsigned char)buf[(r * 16) + offset]);
            if(offset == 7)
            {
                printf(" ");
            }
        }

        printf("  ");
        printf("|");
        for(offset  = 0 ; offset < 16 ; offset++)
        {
            if( (buf[(r * 16) + offset] >= 32) && (buf[(r * 16) + offset] <= 126) )
            {
                printf("%c", buf[(r * 16) + offset]);
            }
            else
            {
                printf(".");
            }
        }
        printf("|");
        printf("\n");
    }

    if(remain != 0)
    {
        printf("%04x  ", r*16);
        for(offset  = 0 ; offset < 16 ; offset++)
        {
            if(offset < remain)
            {
                printf("%02x ", (unsigned char)buf[(r * 16) + offset]);
            }
            else
            {
                printf("   ");
            }

            if(offset == 7)
            {
                printf(" ");
            }
        }

        printf("  ");
        printf("|");
        for(offset  = 0 ; offset < 16 ; offset++)
        {
            if(offset < remain)
            {
                if( (buf[(r * 16) + offset] >= 32) && (buf[(r * 16) + offset] <= 126) )
                {
                    printf("%c", buf[(r * 16) + offset]);
                }
                else
                {
                    printf(".");
                }
            }
            else
            {
                printf(" ");
            }
        }
        printf("|");
    }

    printf("\n");
}

unsigned short int_sqrt(unsigned long a)
{
    unsigned long rem = 0;
    unsigned long root_val = 0;
    unsigned long divisor = 0;
    int i;

    for(i=0; i<16; ++i)
    {
        root_val <<= 1;
        rem = ((rem << 2) + (a >> 30));
        a <<= 2;
        divisor = (root_val<<1) + 1;
        if(divisor <= rem)
        {
            rem -= divisor;
            root_val++;
        }
    }

    return (unsigned short)(root_val);
}

char *basename(const char *filename)
{
    char *p = strrchr(filename, '/');
    return (p != NULL) ? (p + 1) : ((char *) filename );
}

char * dirname(char *path)
{
    static const char dot[] = ".";
    char *last_slash;

    /* Find last '/'.  */
    last_slash =( (path != NULL) ? strrchr(path, '/') : NULL );

    if ((last_slash != NULL) &&  (last_slash == path ))
    {
        /* The last slash is the first character in the string.  We have to
         return "/".  */
        last_slash++;
    }
    else if ((last_slash != NULL) && (last_slash[1] == '\0'))
    {
        /* The '/' is the last character, we have to look further.  */
        last_slash = memchr(path, last_slash - path, '/');
    }
    else
    {
        print_dbg();
    }

    if (last_slash != NULL)
    {
        /* Terminate the path.  */
        last_slash[0] = '\0';
    }
    else
    {
        /* This assignment is ill-designed but the XPG specs require to
         return a string containing "." in any case no directory part is
         found and so a static and constant string is required.  */
        path = (char *) dot;
    }
    return path;
}


