/********************************************************************************
 *                              USDK(1100)
 *                            Module: ATOMIC
 *                 Copyright(c) 2003-2008 Actions Semiconductor,
 *                            All Rights Reserved.
 *
 * History:
 *      <author>    <time>           <version >             <desc>
 *       hmwei     2008-8-30 16:45     1.0             build this file
 ********************************************************************************/
/*!
 * \file     atomic.h
 * \brief    原子操作头文件
 * \author   open source
 * \version 1.0
 * \date  2008/8/30
 *******************************************************************************/
#ifndef __ASM_ATOMIC_H__
#define __ASM_ATOMIC_H__

#include <asm-mips/config.h>
#include <ucos/spinlock.h>
#include <ucos/types.h>

//extern spinlock_t NULL;

/*!
 * \brief
 *      原子类型结构定义.
 */
typedef struct
{
    /*! volatile int型计数值*/
    volatile int counter;
} atomic_t;

#define ATOMIC_INIT(i)    { (i) }

/*!
 * \brief
 *      read atomic variable
 * \li  v: pointer of type atomic_t
 * \par          exmaple code
 * \code
 int a;
 atomic_t v;
 a = atomic_read(v);
 * \endcode
 */
#define atomic_read(v)		((v)->counter)

/*!
 * \brief
 *      set atomic variable
 * \li  v: pointer of type atomic_t
 * \li  i: required value
 * \par          exmaple code
 * \code
 atomic_t v;
 atomic_set(v, 100);
 * \endcode
 */
#define atomic_set(v,i)		((v)->counter = (i))

/*! \cond KMODULE_API*/
/******************************************************************************/
/*!
 * \par  Description:
 *	  add integer to atomic variable
 * \param[in]    i  integer value to add
 * \param[in]    v  pointer of type atomic_t
 * \return       null
 * \ingroup      module name
 * \par          exmaple code
 * \code
 atomic_t v;
 atomic_set(v, 100);
 atomic_add(1, v);
 * \endcode
 *******************************************************************************/
static inline void atomic_add(int i, atomic_t * v)
{
    if (cpu_has_llsc && R10000_LLSC_WAR)
    {
        unsigned long temp;

        __asm__ __volatile__(
                "	.set	mips3					\n"
                "1:	ll	%0, %1		# atomic_add		\n"
                "	addu	%0, %2					\n"
                "	sc	%0, %1					\n"
                "	beqzl	%0, 1b					\n"
                "	.set	mips0					\n"
                : "=&r" (temp), "=m" (v->counter)
                : "Ir" (i), "m" (v->counter));
    }
    else if (cpu_has_llsc)
    {
        unsigned long temp;

        __asm__ __volatile__(
                "	.set	mips3					\n"
                "1:	ll	%0, %1		# atomic_add		\n"
                "	addu	%0, %2					\n"
                "	sc	%0, %1					\n"
                "	beqz	%0, 1b					\n"
                "	.set	mips0					\n"
                : "=&r" (temp), "=m" (v->counter)
                : "Ir" (i), "m" (v->counter));
    }
    else
    {
        unsigned long flags;

        spin_lock_irqsave(NULL, flags);
        v->counter += i;
        spin_unlock_irqrestore(NULL, flags);
    }
}

/******************************************************************************/
/*!
 * \par  Description:
 *	  subtract the atomic variable
 * \param[in]    i  integer value to add
 * \param[in]    v  pointer of type atomic_t
 * \return       null
 * \ingroup      module name
 * \par          exmaple code
 * \code
 atomic_t v;
 atomic_set(v, 100);
 atomic_sub(1, v);
 * \endcode
 *******************************************************************************/
static inline void atomic_sub(int i, atomic_t * v)
{
    if (cpu_has_llsc && R10000_LLSC_WAR)
    {
        unsigned long temp;

        __asm__ __volatile__(
                "	.set	mips3					\n"
                "1:	ll	%0, %1		# atomic_sub		\n"
                "	subu	%0, %2					\n"
                "	sc	%0, %1					\n"
                "	beqzl	%0, 1b					\n"
                "	.set	mips0					\n"
                : "=&r" (temp), "=m" (v->counter)
                : "Ir" (i), "m" (v->counter));
    }
    else if (cpu_has_llsc)
    {
        unsigned long temp;

        __asm__ __volatile__(
                "	.set	mips3					\n"
                "1:	ll	%0, %1		# atomic_sub		\n"
                "	subu	%0, %2					\n"
                "	sc	%0, %1					\n"
                "	beqz	%0, 1b					\n"
                "	.set	mips0					\n"
                : "=&r" (temp), "=m" (v->counter)
                : "Ir" (i), "m" (v->counter));
    }
    else
    {
        unsigned long flags;

        spin_lock_irqsave(NULL, flags);
        v->counter -= i;
        spin_unlock_irqrestore(NULL, flags);
    }
}
/*! \endcond */
/*
 * Same as above, but return the result value
 */
static inline int atomic_add_return(int i, atomic_t * v)
{
    unsigned long result;

    if (cpu_has_llsc && R10000_LLSC_WAR)
    {
        unsigned long temp;

        __asm__ __volatile__(
                "	.set	mips3					\n"
                "1:	ll	%1, %2		# atomic_add_return	\n"
                "	addu	%0, %1, %3				\n"
                "	sc	%0, %2					\n"
                "	beqzl	%0, 1b					\n"
                "	addu	%0, %1, %3				\n"
                "	sync						\n"
                "	.set	mips0					\n"
                : "=&r" (result), "=&r" (temp), "=m" (v->counter)
                : "Ir" (i), "m" (v->counter)
                : "memory");
    }
    else if (cpu_has_llsc)
    {
        unsigned long temp;

        __asm__ __volatile__(
                "	.set	mips3					\n"
                "1:	ll	%1, %2		# atomic_add_return	\n"
                "	addu	%0, %1, %3				\n"
                "	sc	%0, %2					\n"
                "	beqz	%0, 1b					\n"
                "	addu	%0, %1, %3				\n"
                "	sync						\n"
                "	.set	mips0					\n"
                : "=&r" (result), "=&r" (temp), "=m" (v->counter)
                : "Ir" (i), "m" (v->counter)
                : "memory");
    }
    else
    {
        unsigned long flags;

        spin_lock_irqsave(NULL, flags);
        result = v->counter;
        result += i;
        v->counter = result;
        spin_unlock_irqrestore(NULL, flags);
    }

    return result;
}

static inline int atomic_sub_return(int i, atomic_t * v)
{
    unsigned long result;

    if (cpu_has_llsc && R10000_LLSC_WAR)
    {
        unsigned long temp;

        __asm__ __volatile__(
                "	.set	mips3					\n"
                "1:	ll	%1, %2		# atomic_sub_return	\n"
                "	subu	%0, %1, %3				\n"
                "	sc	%0, %2					\n"
                "	beqzl	%0, 1b					\n"
                "	subu	%0, %1, %3				\n"
                "	sync						\n"
                "	.set	mips0					\n"
                : "=&r" (result), "=&r" (temp), "=m" (v->counter)
                : "Ir" (i), "m" (v->counter)
                : "memory");
    }
    else if (cpu_has_llsc)
    {
        unsigned long temp;

        __asm__ __volatile__(
                "	.set	mips3					\n"
                "1:	ll	%1, %2		# atomic_sub_return	\n"
                "	subu	%0, %1, %3				\n"
                "	sc	%0, %2					\n"
                "	beqz	%0, 1b					\n"
                "	subu	%0, %1, %3				\n"
                "	sync						\n"
                "	.set	mips0					\n"
                : "=&r" (result), "=&r" (temp), "=m" (v->counter)
                : "Ir" (i), "m" (v->counter)
                : "memory");
    }
    else
    {
        unsigned long flags;

        spin_lock_irqsave(NULL, flags);
        result = v->counter;
        result -= i;
        v->counter = result;
        spin_unlock_irqrestore(NULL, flags);
    }

    return result;
}

/*
 * atomic_sub_if_positive - add integer to atomic variable
 * @v: pointer of type atomic_t
 *
 * Atomically test @v and decrement if it is greater than 0.
 * The function returns the old value of @v minus 1.
 */
static inline int atomic_sub_if_positive(int i, atomic_t * v)
{
    long result;

    if (cpu_has_llsc && R10000_LLSC_WAR)
    {
        long temp;

        __asm__ __volatile__(
                "	.set	mips3					\n"
                "1:	ll	%1, %2		# atomic_sub_if_positive\n"
                "	subu	%0, %1, %3				\n"
                "	bltz	%0, 1f					\n"
                "	sc	%0, %2					\n"
                "	beqzl	%0, 1b					\n"
                "	sync						\n"
                "1:							\n"
                "	.set	mips0					\n"
                : "=&r" (result), "=&r" (temp), "=m" (v->counter)
                : "Ir" (i), "m" (v->counter)
                : "memory");
    }
    else if (cpu_has_llsc)
    {
        long temp;

        __asm__ __volatile__(
                "	.set	mips3					\n"
                "1:	ll	%1, %2		# atomic_sub_if_positive\n"
                "	subu	%0, %1, %3				\n"
                "	bltz	%0, 1f					\n"
                "	sc	%0, %2					\n"
                "	beqz	%0, 1b					\n"
                "	sync						\n"
                "1:							\n"
                "	.set	mips0					\n"
                : "=&r" (result), "=&r" (temp), "=m" (v->counter)
                : "Ir" (i), "m" (v->counter)
                : "memory");
    }
    else
    {
        unsigned long flags;

        spin_lock_irqsave(NULL, flags);
        result = v->counter;
        result -= i;
        if (result >= 0)
        {
            v->counter = result;
        }
        spin_unlock_irqrestore(NULL, flags);
    }

    return result;
}

#define atomic_dec_return(v) atomic_sub_return(1,(v))
#define atomic_inc_return(v) atomic_add_return(1,(v))

/*
 * atomic_sub_and_test - subtract value from variable and test result
 * @i: integer value to subtract
 * @v: pointer of type atomic_t
 *
 * Atomically subtracts @i from @v and returns
 * true if the result is zero, or false for all
 * other cases.
 */
#define atomic_sub_and_test(i,v) (atomic_sub_return((i), (v)) == 0)

/*
 * atomic_inc_and_test - increment and test
 * @v: pointer of type atomic_t
 *
 * Atomically increments @v by 1
 * and returns true if the result is zero, or false for all
 * other cases.
 */
#define atomic_inc_and_test(v) (atomic_inc_return(v) == 0)

/*
 * atomic_dec_and_test - decrement by 1 and test
 * @v: pointer of type atomic_t
 *
 * Atomically decrements @v by 1 and
 * returns true if the result is 0, or false for all other
 * cases.
 */
#define atomic_dec_and_test(v) (atomic_sub_return(1, (v)) == 0)

/*
 * atomic_dec_if_positive - decrement by 1 if old value positive
 * @v: pointer of type atomic_t
 */
#define atomic_dec_if_positive(v)	atomic_sub_if_positive(1, v)

/*
 * atomic_inc - increment atomic variable
 * @v: pointer of type atomic_t
 *
 * Atomically increments @v by 1.
 */
#define atomic_inc(v) atomic_add(1,(v))

/*
 * atomic_dec - decrement and test
 * @v: pointer of type atomic_t
 *
 * Atomically decrements @v by 1.
 */
#define atomic_dec(v) atomic_sub(1,(v))

/*
 * atomic_add_negative - add and test if negative
 * @v: pointer of type atomic_t
 * @i: integer value to add
 *
 * Atomically adds @i to @v and returns true
 * if the result is negative, or false when
 * result is greater than or equal to zero.
 */
#define atomic_add_negative(i,v) (atomic_add_return(i, (v)) < 0)

#ifdef CONFIG_64BIT

typedef struct
{   volatile __s64 counter;}atomic64_t;

#define ATOMIC64_INIT(i)    { (i) }

/*
 * atomic64_read - read atomic variable
 * @v: pointer of type atomic64_t
 *
 */
#define atomic64_read(v)	((v)->counter)

/*
 * atomic64_set - set atomic variable
 * @v: pointer of type atomic64_t
 * @i: required value
 */
#define atomic64_set(v,i)	((v)->counter = (i))

/*
 * atomic64_add - add integer to atomic variable
 * @i: integer value to add
 * @v: pointer of type atomic64_t
 *
 * Atomically adds @i to @v.
 */
static inline void atomic64_add(long i, atomic64_t * v)
{
    if (cpu_has_llsc && R10000_LLSC_WAR)
    {
        unsigned long temp;

        __asm__ __volatile__(
                "	.set	mips3					\n"
                "1:	lld	%0, %1		# atomic64_add		\n"
                "	addu	%0, %2					\n"
                "	scd	%0, %1					\n"
                "	beqzl	%0, 1b					\n"
                "	.set	mips0					\n"
                : "=&r" (temp), "=m" (v->counter)
                : "Ir" (i), "m" (v->counter));
    }
    else if (cpu_has_llsc)
    {
        unsigned long temp;

        __asm__ __volatile__(
                "	.set	mips3					\n"
                "1:	lld	%0, %1		# atomic64_add		\n"
                "	addu	%0, %2					\n"
                "	scd	%0, %1					\n"
                "	beqz	%0, 1b					\n"
                "	.set	mips0					\n"
                : "=&r" (temp), "=m" (v->counter)
                : "Ir" (i), "m" (v->counter));
    }
    else
    {
        unsigned long flags;

        spin_lock_irqsave(NULL, flags);
        v->counter += i;
        spin_unlock_irqrestore(NULL, flags);
    }
}

/*
 * atomic64_sub - subtract the atomic variable
 * @i: integer value to subtract
 * @v: pointer of type atomic64_t
 *
 * Atomically subtracts @i from @v.
 */
static inline void atomic64_sub(long i, atomic64_t * v)
{
    if (cpu_has_llsc && R10000_LLSC_WAR)
    {
        unsigned long temp;

        __asm__ __volatile__(
                "	.set	mips3					\n"
                "1:	lld	%0, %1		# atomic64_sub		\n"
                "	subu	%0, %2					\n"
                "	scd	%0, %1					\n"
                "	beqzl	%0, 1b					\n"
                "	.set	mips0					\n"
                : "=&r" (temp), "=m" (v->counter)
                : "Ir" (i), "m" (v->counter));
    }
    else if (cpu_has_llsc)
    {
        unsigned long temp;

        __asm__ __volatile__(
                "	.set	mips3					\n"
                "1:	lld	%0, %1		# atomic64_sub		\n"
                "	subu	%0, %2					\n"
                "	scd	%0, %1					\n"
                "	beqz	%0, 1b					\n"
                "	.set	mips0					\n"
                : "=&r" (temp), "=m" (v->counter)
                : "Ir" (i), "m" (v->counter));
    }
    else
    {
        unsigned long flags;

        spin_lock_irqsave(NULL, flags);
        v->counter -= i;
        spin_unlock_irqrestore(NULL, flags);
    }
}

/*
 * Same as above, but return the result value
 */
static inline long atomic64_add_return(long i, atomic64_t * v)
{
    unsigned long result;

    if (cpu_has_llsc && R10000_LLSC_WAR)
    {
        unsigned long temp;

        __asm__ __volatile__(
                "	.set	mips3					\n"
                "1:	lld	%1, %2		# atomic64_add_return	\n"
                "	addu	%0, %1, %3				\n"
                "	scd	%0, %2					\n"
                "	beqzl	%0, 1b					\n"
                "	addu	%0, %1, %3				\n"
                "	sync						\n"
                "	.set	mips0					\n"
                : "=&r" (result), "=&r" (temp), "=m" (v->counter)
                : "Ir" (i), "m" (v->counter)
                : "memory");
    }
    else if (cpu_has_llsc)
    {
        unsigned long temp;

        __asm__ __volatile__(
                "	.set	mips3					\n"
                "1:	lld	%1, %2		# atomic64_add_return	\n"
                "	addu	%0, %1, %3				\n"
                "	scd	%0, %2					\n"
                "	beqz	%0, 1b					\n"
                "	addu	%0, %1, %3				\n"
                "	sync						\n"
                "	.set	mips0					\n"
                : "=&r" (result), "=&r" (temp), "=m" (v->counter)
                : "Ir" (i), "m" (v->counter)
                : "memory");
    }
    else
    {
        unsigned long flags;

        spin_lock_irqsave(NULL, flags);
        result = v->counter;
        result += i;
        v->counter = result;
        spin_unlock_irqrestore(NULL, flags);
    }

    return result;
}

static inline long atomic64_sub_return(long i, atomic64_t * v)
{
    unsigned long result;

    if (cpu_has_llsc && R10000_LLSC_WAR)
    {
        unsigned long temp;

        __asm__ __volatile__(
                "	.set	mips3					\n"
                "1:	lld	%1, %2		# atomic64_sub_return	\n"
                "	subu	%0, %1, %3				\n"
                "	scd	%0, %2					\n"
                "	beqzl	%0, 1b					\n"
                "	subu	%0, %1, %3				\n"
                "	sync						\n"
                "	.set	mips0					\n"
                : "=&r" (result), "=&r" (temp), "=m" (v->counter)
                : "Ir" (i), "m" (v->counter)
                : "memory");
    }
    else if (cpu_has_llsc)
    {
        unsigned long temp;

        __asm__ __volatile__(
                "	.set	mips3					\n"
                "1:	lld	%1, %2		# atomic64_sub_return	\n"
                "	subu	%0, %1, %3				\n"
                "	scd	%0, %2					\n"
                "	beqz	%0, 1b					\n"
                "	subu	%0, %1, %3				\n"
                "	sync						\n"
                "	.set	mips0					\n"
                : "=&r" (result), "=&r" (temp), "=m" (v->counter)
                : "Ir" (i), "m" (v->counter)
                : "memory");
    }
    else
    {
        unsigned long flags;

        spin_lock_irqsave(NULL, flags);
        result = v->counter;
        result -= i;
        v->counter = result;
        spin_unlock_irqrestore(NULL, flags);
    }

    return result;
}

/*
 * atomic64_sub_if_positive - add integer to atomic variable
 * @v: pointer of type atomic64_t
 *
 * Atomically test @v and decrement if it is greater than 0.
 * The function returns the old value of @v minus 1.
 */
static inline long atomic64_sub_if_positive(long i, atomic64_t * v)
{
    unsigned long result;

    if (cpu_has_llsc && R10000_LLSC_WAR)
    {
        unsigned long temp;

        __asm__ __volatile__(
                "	.set	mips3					\n"
                "1:	lld	%1, %2		# atomic64_sub_if_positive\n"
                "	dsubu	%0, %1, %3				\n"
                "	bltz	%0, 1f					\n"
                "	scd	%0, %2					\n"
                "	beqzl	%0, 1b					\n"
                "	sync						\n"
                "1:							\n"
                "	.set	mips0					\n"
                : "=&r" (result), "=&r" (temp), "=m" (v->counter)
                : "Ir" (i), "m" (v->counter)
                : "memory");
    }
    else if (cpu_has_llsc)
    {
        unsigned long temp;

        __asm__ __volatile__(
                "	.set	mips3					\n"
                "1:	lld	%1, %2		# atomic64_sub_if_positive\n"
                "	dsubu	%0, %1, %3				\n"
                "	bltz	%0, 1f					\n"
                "	scd	%0, %2					\n"
                "	beqz	%0, 1b					\n"
                "	sync						\n"
                "1:							\n"
                "	.set	mips0					\n"
                : "=&r" (result), "=&r" (temp), "=m" (v->counter)
                : "Ir" (i), "m" (v->counter)
                : "memory");
    }
    else
    {
        unsigned long flags;

        spin_lock_irqsave(NULL, flags);
        result = v->counter;
        result -= i;
        if (result >= 0)
        v->counter = result;
        spin_unlock_irqrestore(NULL, flags);
    }

    return result;
}

#define atomic64_dec_return(v) atomic64_sub_return(1,(v))
#define atomic64_inc_return(v) atomic64_add_return(1,(v))

/*
 * atomic64_sub_and_test - subtract value from variable and test result
 * @i: integer value to subtract
 * @v: pointer of type atomic64_t
 *
 * Atomically subtracts @i from @v and returns
 * true if the result is zero, or false for all
 * other cases.
 */
#define atomic64_sub_and_test(i,v) (atomic64_sub_return((i), (v)) == 0)

/*
 * atomic64_inc_and_test - increment and test
 * @v: pointer of type atomic64_t
 *
 * Atomically increments @v by 1
 * and returns true if the result is zero, or false for all
 * other cases.
 */
#define atomic64_inc_and_test(v) (atomic64_inc_return(v) == 0)

/*
 * atomic64_dec_and_test - decrement by 1 and test
 * @v: pointer of type atomic64_t
 *
 * Atomically decrements @v by 1 and
 * returns true if the result is 0, or false for all other
 * cases.
 */
#define atomic64_dec_and_test(v) (atomic64_sub_return(1, (v)) == 0)

/*
 * atomic64_dec_if_positive - decrement by 1 if old value positive
 * @v: pointer of type atomic64_t
 */
#define atomic64_dec_if_positive(v)	atomic64_sub_if_positive(1, v)

/*
 * atomic64_inc - increment atomic variable
 * @v: pointer of type atomic64_t
 *
 * Atomically increments @v by 1.
 */
#define atomic64_inc(v) atomic64_add(1,(v))

/*
 * atomic64_dec - decrement and test
 * @v: pointer of type atomic64_t
 *
 * Atomically decrements @v by 1.
 */
#define atomic64_dec(v) atomic64_sub(1,(v))

/*
 * atomic64_add_negative - add and test if negative
 * @v: pointer of type atomic64_t
 * @i: integer value to add
 *
 * Atomically adds @i to @v and returns true
 * if the result is negative, or false when
 * result is greater than or equal to zero.
 */
#define atomic64_add_negative(i,v) (atomic64_add_return(i, (v)) < 0)

#endif /* CONFIG_64BIT */

#endif /* __ASM_ATOMIC_H__ */
