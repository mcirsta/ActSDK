/**
 * architecture depend initialize routine
 *
 * unit test for USDK(mips, ucos-ii based)
 *
 * sangwei@actions-semi.com
 *
 */
#ifndef __CUNIT_ARCH_H__
#define __CUNIT_ARCH_H__

#if (__COVENV__ == 0) // NOSYS
#ifndef NULL
#define NULL    0
#endif
#endif /* end of (__COVENV__ == 0) */

#define EXIT_SUCCESS    0
#define EXIT_FAILURE    1

int cunit_init(void);

void cunit_exit(int retcode);

void cunit_assert(int val);

#endif /* __CUNIT_ARCH_H__ */
