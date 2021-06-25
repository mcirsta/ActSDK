#ifndef _VFILE_H_
#define _VFILE_H_

#include "stdio.h"
typedef FILE* HANDLE;

typedef enum
{
    R,
    W
}OPEN_MODE;

typedef enum
{
    V_SEEK_SET,
    V_SEEK_CUR,
    V_SEEK_END
}SEEK_POS;

HANDLE v_fopen(char * fname,OPEN_MODE m);
int v_fclose(HANDLE file);
int v_fread(void * buff,int size,HANDLE file);
int v_fwrite(void * buff,int size,HANDLE file);
int v_remove(char * fname);
int v_fseek(HANDLE file,int offset,SEEK_POS pos);
int v_ftell(HANDLE file);

#endif
