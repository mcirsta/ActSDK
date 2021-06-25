#ifndef __IME_H__
#define __IME_H__

#include <libc.h>

int ime_engine_init(void);
int ime_engine_get_candidates(char *pybuf, unsigned short *candidates,
        int bufferlen, int *hanzicnt, int get_child);
int ime_engine_exit(void);

#endif //__IME__H__
