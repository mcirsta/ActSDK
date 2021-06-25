/********************************************************************************
 *                              NOYA
 *                            Module: pthread.h
 *                 Copyright(c) 2003-2008 Actions Semiconductor,
 *                            All Rights Reserved.
 *
 * History:
 *      <author>    <time>           <version >             <desc>
 *       wurui     2008-09-12 10:00     1.0             build this file
 ********************************************************************************/
/*!
 * \file     pthread.h
 * \brief    posix标准线程库头文件
 * \author   wurui
 * \version 1.0
 * \date  2008/09/12
 *******************************************************************************/

#ifndef __PTHREAD_H__
#define __PTHREAD_H__

#include <libc/unistd.h>
#include <libc/types.h>
#include <libc/time.h>
#include <libc/sched.h>
#include <libc/sys/pthread.h>

extern int pthread_create(pthread_t *tidp, const pthread_attr_t *attr,
        void *(*start_rtn)(void*), void *arg);
extern void pthread_exit(void *rval_ptr);
extern int pthread_equal(pthread_t t1, pthread_t t2);
extern pthread_t pthread_self(void);
extern int pthread_join(pthread_t thread, void **value_ptr);
extern int pthread_detach(pthread_t thread);
extern int pthread_cancel(pthread_t tid);
extern int
        pthread_once(pthread_once_t *once_control, void(*init_routine)(void));
extern int pthread_key_create(pthread_key_t *key, void(*destructor)(void *));
extern int pthread_key_delete(pthread_key_t key);
extern void *pthread_getspecific(pthread_key_t key);
extern int pthread_setspecific(pthread_key_t key, const void *value);
extern void pthread_cleanup_push(void(*routine)(void*), void *arg);
extern void pthread_cleanup_pop(int execute);
extern int pthread_setconcurrency(int new_level);
extern int pthread_getconcurrency(void);
extern int pthread_setschedprio(pthread_t thread, int prio);
extern int pthread_getschedparam(pthread_t thread, int *policy,
        struct sched_param *param);
extern int pthread_setschedparam(pthread_t thread, int policy,
        const struct sched_param *param);
extern int pthread_attr_init(pthread_attr_t *tattr);
extern int pthread_attr_destroy(pthread_attr_t *tattr);
extern int pthread_attr_setinheritsched(pthread_attr_t *attr, int inheritsched);
extern int pthread_attr_getinheritsched(const pthread_attr_t *attr,
        int *inheritsched);
extern int pthread_attr_setdetachstate(pthread_attr_t *tattr, int detachstate);
extern int pthread_attr_getdetachstate(const pthread_attr_t *tattr,
        int *detachstate);
extern int pthread_attr_setguardsize(pthread_attr_t *attr, size_t guardsize);
extern int pthread_attr_getguardsize(const pthread_attr_t *attr,
        size_t *guardsize);
extern int pthread_attr_setscope(pthread_attr_t *tattr, int scope);
extern int pthread_attr_getscope(pthread_attr_t *tattr, int *scope);
extern int pthread_attr_setschedpolicy(pthread_attr_t *attr, int policy);
extern int
        pthread_attr_getschedpolicy(const pthread_attr_t * attr, int *policy);
extern int pthread_attr_setschedparam(pthread_attr_t *attr,
        const struct sched_param *param);
extern int pthread_attr_getschedparam(const pthread_attr_t *attr,
        struct sched_param *param);
extern int pthread_attr_setstacksize(pthread_attr_t *attr, size_t stacksize);
extern int pthread_attr_getstacksize(const pthread_attr_t *attr,
        size_t *stacksize);
extern int pthread_attr_setstack(pthread_attr_t *attr, void *stackaddr,
        size_t stacksize);
extern int pthread_attr_getstack(const pthread_attr_t *attr, void **stackaddr,
        size_t *stacksize);

extern int pthread_mutex_init(pthread_mutex_t *mutex,
        const pthread_mutexattr_t *attr);
extern int pthread_mutex_destroy(pthread_mutex_t *mutex);
extern int pthread_mutex_lock(pthread_mutex_t *mutex);
extern int pthread_mutex_trylock(pthread_mutex_t *mutex);
extern int pthread_mutex_unlock(pthread_mutex_t *mutex);
extern int pthread_mutexattr_init(pthread_mutexattr_t *attr);
extern int pthread_mutexattr_destroy(pthread_mutexattr_t *attr);
extern int pthread_mutexattr_setpshared(pthread_mutexattr_t *attr, int pshared);
extern int pthread_mutexattr_getpshared(const pthread_mutexattr_t *attr,
        int *pshared);
extern int pthread_mutexattr_settype(pthread_mutexattr_t *attr, int type);
extern int
        pthread_mutexattr_gettype(const pthread_mutexattr_t *attr, int *type);

extern int pthread_cond_init(pthread_cond_t *cond,
        const pthread_condattr_t *attr);
extern int pthread_cond_destroy(pthread_cond_t *cond);
extern int pthread_cond_wait(pthread_cond_t *cond, pthread_mutex_t *mutex);
extern int pthread_cond_timedwait(pthread_cond_t *cond, pthread_mutex_t *mutex,
        const struct timespec *abstime);
extern int pthread_cond_signal(pthread_cond_t *cond);
extern int pthread_cond_broadcast(pthread_cond_t *cond);
extern int pthread_condattr_init(pthread_condattr_t *attr);
extern int pthread_condattr_destroy(pthread_condattr_t *attr);
extern int pthread_condattr_getpshared(const pthread_condattr_t * attr,
        int *pshared);
extern int pthread_condattr_setpshared(pthread_condattr_t *attr, int pshared);

#endif /* #ifndef __PTHREAD_H__ */
