/**
 * setjmp/longjmp support for Non-OS/USDK
 * based on the setjmp/longjmp routine from glibc-2.3.5
 *
 * sangwei@actions-semi.com
 *
 */
#ifndef __SETJMP_H__
#define __SETJMP_H__

typedef void * __ptr_t;

typedef struct __jmp_buf_s
{
    /* Program counter.  */
    __ptr_t __pc;

    /* Stack pointer.  */
    __ptr_t __sp;

    /* Callee-saved registers s0 through s7.  */
    __ptr_t __regs[8];

    /* The frame pointer.  */
    __ptr_t __fp;

    /* The global pointer.  */
    __ptr_t __gp;

/* we have no floating point register in ATJ213x */
} jmp_buf[1];

extern int setjmp(jmp_buf __env);
extern void longjmp(jmp_buf env, int val_arg);

#endif /* __SETJMP_H__ */
