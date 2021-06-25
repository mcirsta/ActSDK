/********************************************************************************
 *                              USDK130
 *                            Module: EXCEPTION
 *                 Copyright(c) 2001-2008 Actions Semiconductor,
 *                            All Rights Reserved.
 *
 * History:
 *      <author>    <time>           <version >             <desc>
 *       hmwei     2008-6-12 9:42     1.0             build this file
 ********************************************************************************/
/*!
 * \file     exception.h
 * \brief    异常处理栈桢相关宏定义和数据结构
 * \author   open source
 * \version 1.0
 * \date  2008/6/12
 *******************************************************************************/
#ifndef __EXCEPTION_H__
#define __EXCEPTION_H__
/*
 * important register numbers
 */

#define REG_EPC			37
#define REG_FP			72
#define REG_SP			29

/*
 * Stack layout for the GDB exception handler
 * Derived from the stack layout described in asm-mips/stackframe.h
 *
 * The first PTRSIZE*6 bytes are argument save space for C subroutines.
 */
#define NUMREGS			55
#define GDB_GENGERAL_NUMREGS  38

#define GDB_FR_REG0		(PTRSIZE*6)			/* 0 */
#define GDB_FR_REG1		((GDB_FR_REG0) + LONGSIZE)	/* 1 */
#define GDB_FR_REG2		((GDB_FR_REG1) + LONGSIZE)	/* 2 */
#define GDB_FR_REG3		((GDB_FR_REG2) + LONGSIZE)	/* 3 */
#define GDB_FR_REG4		((GDB_FR_REG3) + LONGSIZE)	/* 4 */
#define GDB_FR_REG5		((GDB_FR_REG4) + LONGSIZE)	/* 5 */
#define GDB_FR_REG6		((GDB_FR_REG5) + LONGSIZE)	/* 6 */
#define GDB_FR_REG7		((GDB_FR_REG6) + LONGSIZE)	/* 7 */
#define GDB_FR_REG8		((GDB_FR_REG7) + LONGSIZE)	/* 8 */
#define GDB_FR_REG9	        ((GDB_FR_REG8) + LONGSIZE)	/* 9 */
#define GDB_FR_REG10		((GDB_FR_REG9) + LONGSIZE)	/* 10 */
#define GDB_FR_REG11		((GDB_FR_REG10) + LONGSIZE)	/* 11 */
#define GDB_FR_REG12		((GDB_FR_REG11) + LONGSIZE)	/* 12 */
#define GDB_FR_REG13		((GDB_FR_REG12) + LONGSIZE)	/* 13 */
#define GDB_FR_REG14		((GDB_FR_REG13) + LONGSIZE)	/* 14 */
#define GDB_FR_REG15		((GDB_FR_REG14) + LONGSIZE)	/* 15 */
#define GDB_FR_REG16		((GDB_FR_REG15) + LONGSIZE)	/* 16 */
#define GDB_FR_REG17		((GDB_FR_REG16) + LONGSIZE)	/* 17 */
#define GDB_FR_REG18		((GDB_FR_REG17) + LONGSIZE)	/* 18 */
#define GDB_FR_REG19		((GDB_FR_REG18) + LONGSIZE)	/* 19 */
#define GDB_FR_REG20		((GDB_FR_REG19) + LONGSIZE)	/* 20 */
#define GDB_FR_REG21		((GDB_FR_REG20) + LONGSIZE)	/* 21 */
#define GDB_FR_REG22		((GDB_FR_REG21) + LONGSIZE)	/* 22 */
#define GDB_FR_REG23		((GDB_FR_REG22) + LONGSIZE)	/* 23 */
#define GDB_FR_REG24		((GDB_FR_REG23) + LONGSIZE)	/* 24 */
#define GDB_FR_REG25		((GDB_FR_REG24) + LONGSIZE)	/* 25 */
#define GDB_FR_REG26		((GDB_FR_REG25) + LONGSIZE)	/* 26 */
#define GDB_FR_REG27		((GDB_FR_REG26) + LONGSIZE)	/* 27 */
#define GDB_FR_REG28		((GDB_FR_REG27) + LONGSIZE)	/* 28 */
#define GDB_FR_REG29		((GDB_FR_REG28) + LONGSIZE)	/* 29 */
#define GDB_FR_REG30		((GDB_FR_REG29) + LONGSIZE)	/* 30 */
#define GDB_FR_REG31		((GDB_FR_REG30) + LONGSIZE)	/* 31 */

/*
 * Saved special registers
 */
#define GDB_FR_STATUS		((GDB_FR_REG31) + LONGSIZE)	/* 32 */
#define GDB_FR_LO		((GDB_FR_STATUS) + LONGSIZE)	/* 33 */
#define GDB_FR_HI		((GDB_FR_LO) + LONGSIZE)	/* 34 */
#define GDB_FR_BADVADDR		((GDB_FR_HI) + LONGSIZE)	/* 35 */
#define GDB_FR_CAUSE		((GDB_FR_BADVADDR) + LONGSIZE)	/* 36 */
#define GDB_FR_EPC		((GDB_FR_CAUSE) + LONGSIZE)	/* 37 */
#define GDB_FR_FRP		    ((GDB_FR_EPC) + LONGSIZE)	/* 38 */
#define GDB_FR_CP0_INDEX	((GDB_FR_FRP) + LONGSIZE)	/* 39 */
#define GDB_FR_CP0_RANDOM	((GDB_FR_CP0_INDEX) + LONGSIZE)	/* 40 */
#define GDB_FR_CP0_ENTRYLO0	((GDB_FR_CP0_RANDOM) + LONGSIZE)/* 41 */
#define GDB_FR_CP0_ENTRYLO1	((GDB_FR_CP0_ENTRYLO0) + LONGSIZE)/* 42 */
#define GDB_FR_CP0_CONTEXT	((GDB_FR_CP0_ENTRYLO1) + LONGSIZE)/* 43 */
#define GDB_FR_CP0_PAGEMASK	((GDB_FR_CP0_CONTEXT) + LONGSIZE)/* 44 */
#define GDB_FR_CP0_WIRED	((GDB_FR_CP0_PAGEMASK) + LONGSIZE)/* 45 */
#define GDB_FR_CP0_REG7		((GDB_FR_CP0_WIRED) + LONGSIZE)	/* 46 */
#define GDB_FR_CP0_REG8		((GDB_FR_CP0_REG7) + LONGSIZE)	/* 47 */
#define GDB_FR_CP0_REG9		((GDB_FR_CP0_REG8) + LONGSIZE)	/* 48 */
#define GDB_FR_CP0_ENTRYHI	((GDB_FR_CP0_REG9) + LONGSIZE)	/* 49 */
#define GDB_FR_CP0_REG11	((GDB_FR_CP0_ENTRYHI) + LONGSIZE)/* 50 */
#define GDB_FR_CP0_REG12	((GDB_FR_CP0_REG11) + LONGSIZE)	/* 51 */
#define GDB_FR_CP0_REG13	((GDB_FR_CP0_REG12) + LONGSIZE)	/* 52 */
#define GDB_FR_CP0_REG14	((GDB_FR_CP0_REG13) + LONGSIZE)	/* 53 */
#define GDB_FR_CP0_PRID		((GDB_FR_CP0_REG14) + LONGSIZE)	/* 54 */

#define GDB_FR_SIZE		((((GDB_FR_CP0_PRID) + LONGSIZE) + (PTRSIZE-1)) & ~(PTRSIZE-1))

/*
 * Stack layout for the INT exception handler
 * Derived from the stack layout described in asm-mips/stackframe.h
 *
 * The first PTRSIZE*6 bytes are argument save space for C subroutines.
 */

#define PT_R0		(PTRSIZE*6)			/* 0 */
#define PT_R1		((PT_R0) + LONGSIZE)	/* 1 */
#define PT_R2		((PT_R1) + LONGSIZE)	/* 2 */
#define PT_R3		((PT_R2) + LONGSIZE)	/* 3 */
#define PT_R4		((PT_R3) + LONGSIZE)	/* 4 */
#define PT_R5		((PT_R4) + LONGSIZE)	/* 5 */
#define PT_R6		((PT_R5) + LONGSIZE)	/* 6 */
#define PT_R7		((PT_R6) + LONGSIZE)	/* 7 */
#define PT_R8		((PT_R7) + LONGSIZE)	/* 8 */
#define PT_R9		((PT_R8) + LONGSIZE)	/* 9 */
#define PT_R10		((PT_R9) + LONGSIZE)	/* 10 */
#define PT_R11		((PT_R10) + LONGSIZE)	/* 11 */
#define PT_R12		((PT_R11) + LONGSIZE)	/* 12 */
#define PT_R13		((PT_R12) + LONGSIZE)	/* 13 */
#define PT_R14		((PT_R13) + LONGSIZE)	/* 14 */
#define PT_R15		((PT_R14) + LONGSIZE)	/* 15 */
#define PT_R16		((PT_R15) + LONGSIZE)	/* 16 */
#define PT_R17		((PT_R16) + LONGSIZE)	/* 17 */
#define PT_R18		((PT_R17) + LONGSIZE)	/* 18 */
#define PT_R19		((PT_R18) + LONGSIZE)	/* 19 */
#define PT_R20		((PT_R19) + LONGSIZE)	/* 20 */
#define PT_R21		((PT_R20) + LONGSIZE)	/* 21 */
#define PT_R22		((PT_R21) + LONGSIZE)	/* 22 */
#define PT_R23		((PT_R22) + LONGSIZE)	/* 23 */
#define PT_R24		((PT_R23) + LONGSIZE)	/* 24 */
#define PT_R25		((PT_R24) + LONGSIZE)	/* 25 */
#define PT_R26		((PT_R25) + LONGSIZE)	/* 26 */
#define PT_R27		((PT_R26) + LONGSIZE)	/* 27 */
#define PT_R28		((PT_R27) + LONGSIZE)	/* 28 */
#define PT_R29		((PT_R28) + LONGSIZE)	/* 29 */
#define PT_R30		((PT_R29) + LONGSIZE)	/* 30 */
#define PT_R31		((PT_R30) + LONGSIZE)	/* 31 */

/*
 * Saved special registers
 */
#define PT_STATUS	((PT_R31) + LONGSIZE)	/* 32 */
#define PT_HI		((PT_STATUS) + LONGSIZE)	/* 33 */
#define PT_LO		((PT_HI) + LONGSIZE)	/* 34 */
#define PT_BADVADDR	((PT_LO) + LONGSIZE)	/* 35 */
#define PT_CAUSE	((PT_BADVADDR) + LONGSIZE)	/* 36 */
#define PT_EPC		((PT_CAUSE) + LONGSIZE)	/* 37 */

#define PT_SIZE		((((PT_EPC) + LONGSIZE) + (PTRSIZE-1)) & ~(PTRSIZE-1))

#ifndef _ASSEMBLER_

/*
 * This is the same as above, but for the high-level
 * part of the GDB stub.
 */

struct gdb_regs
{
    /*
     * Pad bytes for argument save space on the stack
     * 24/48 Bytes for 32/64 bit code
     */
    unsigned long pad0[6];

    /*
     * saved main processor registers
     */
    long reg0, reg1, reg2, reg3, reg4, reg5, reg6, reg7;
    long reg8, reg9, reg10, reg11, reg12, reg13, reg14, reg15;
    long reg16, reg17, reg18, reg19, reg20, reg21, reg22, reg23;
    long reg24, reg25, reg26, reg27, reg28, reg29, reg30, reg31;

    /*
     * Saved special registers
     */
    long cp0_status;
    long lo;
    long hi;
    long cp0_badvaddr;
    long cp0_cause;
    long cp0_epc;
    long frame_ptr;

    /*
     * saved cp0 registers
     */
    long cp0_index;
    long cp0_random;
    long cp0_entrylo0;
    long cp0_entrylo1;
    long cp0_context;
    long cp0_pagemask;
    long cp0_wired;
    long cp0_reg7;
    long cp0_reg8;
    long cp0_reg9;
    long cp0_entryhi;
    long cp0_reg11;
    long cp0_reg12;
    long cp0_reg13;
    long cp0_reg14;
    long cp0_prid;
};

/*
 * This is the same as above, but for the high-level
 * part of the INT stub.
 */

struct pt_regs
{
    /* Pad bytes for argument save space on the stack. */
    unsigned long pad0[6];
    /* Saved main processor registers. */
    unsigned long regs[32];
    /* Saved special registers. */
    unsigned long cp0_status;
    unsigned long hi;
    unsigned long lo;
    unsigned long cp0_badvaddr;
    unsigned long cp0_cause;
    unsigned long cp0_epc;
};

//extern unsigned long gdb_fault_jmp_regs[12];
//extern int gdb_may_fault;

/*
 * Prototypes
 */
//extern int gdb_fault_setjmp_aux(unsigned long *curr_context, int sp, int fp);
//extern void gdb_fault_longjmp(unsigned long *curr_context);
//extern int gdb_fault_setjmp(unsigned long *curr_context);
extern void default_exception_server(void);

/*
 set_except_vector
 input:
 n---exception Num,1~31
 addr---new exception handler
 return:
 void *---default exception handler
 */
extern void *set_except_vector(int n, void *addr);
extern void dump_task_info(void);

#endif /* !_ASSEMBLER_ */
#endif /* __EXCEPTION_H__ */
