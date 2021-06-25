/********************************************************************************
 *                              USDK(1100)
 *                            Module: OS
 *                 Copyright(c) 2003-2008 Actions Semiconductor,
 *                            All Rights Reserved.
 *
 * History:
 *      <author>    <time>           <version >             <desc>
 *       hmwei     2009-1-10 16:45     1.0             build this file
 ********************************************************************************/
/*!
 * \file     os.h
 * \brief    将ucos代码规范化后的的数据结构和函数声明，提供给应用和内核模块使用
 * \author   hmwei
 * \version 1.0
 * \date  2009/1/10
 *******************************************************************************/

/*
 *********************************************************************************************************
 *                                             MISCELLANEOUS
 *********************************************************************************************************
 */
#ifndef __OS_H__
#define __OS_H__

#include <ucos/os_cfg.h>

#define  OS_VERSION              252                    /* Version of uC/OS-II (Vx.yy mult. by 100)    */

#ifdef   OS_GLOBALS
#define  OS_EXT
#else
#define  OS_EXT  extern
#endif

#ifndef  FALSE
#define  FALSE                     0
#endif

#ifndef  TRUE
#define  TRUE                      1
#endif

#define  OS_PRIO_SELF           0xFF                    /* Indicate SELF priority                      */

#if OS_TASK_STAT_EN > 0
#define  OS_N_SYS_TASKS            2                    /* Number of system tasks                      */
#else
#define  OS_N_SYS_TASKS            1
#endif

#define  OS_STAT_PRIO       (OS_LOWEST_PRIO - 1)        /* Statistic task priority                     */
#define  OS_IDLE_PRIO       (OS_LOWEST_PRIO)            /* IDLE      task priority                     */

#define  OS_EVENT_TBL_SIZE (((OS_LOWEST_PRIO) / 8) + 1)   /* Size of event table                         */
#define  OS_RDY_TBL_SIZE   (((OS_LOWEST_PRIO) / 8) + 1)   /* Size of ready table                         */

#define  OS_TASK_IDLE_ID       65535                    /* I.D. numbers for Idle and Stat tasks        */
#define  OS_TASK_STAT_ID       65534

#define  OS_EVENT_EN       (((OS_Q_EN > 0) && (OS_MAX_QS > 0)) || (OS_MBOX_EN > 0) || (OS_SEM_EN > 0) || (OS_MUTEX_EN > 0))

/*
 *********************************************************************************************************
 *                              TASK STATUS (Bit definition for OSTCBStat)
 *********************************************************************************************************
 */
#define  OS_STAT_RDY            0x00        /* Ready to run                                            */
#define  OS_STAT_SEM            0x01        /* Pending on semaphore                                    */
#define  OS_STAT_MBOX           0x02        /* Pending on mailbox                                      */
#define  OS_STAT_Q              0x04        /* Pending on queue                                        */
#define  OS_STAT_SUSPEND        0x08        /* Task is suspended                                       */
#define  OS_STAT_MUTEX          0x10        /* Pending on mutual exclusion semaphore                   */
#define  OS_STAT_FLAG           0x20        /* Pending on event flag group                             */

/*
 *********************************************************************************************************
 *                                        OS_EVENT types
 *********************************************************************************************************
 */
#define  OS_EVENT_TYPE_UNUSED      0
#define  OS_EVENT_TYPE_MBOX        1
#define  OS_EVENT_TYPE_Q           2
#define  OS_EVENT_TYPE_SEM         3
#define  OS_EVENT_TYPE_MUTEX       4
#define  OS_EVENT_TYPE_FLAG        5

/*
 *********************************************************************************************************
 *                                         EVENT FLAGS
 *********************************************************************************************************
 */
#define  OS_FLAG_WAIT_CLR_ALL      0        /* Wait for ALL    the bits specified to be CLR (i.e. 0)   */
#define  OS_FLAG_WAIT_CLR_AND      0

#define  OS_FLAG_WAIT_CLR_ANY      1        /* Wait for ANY of the bits specified to be CLR (i.e. 0)   */
#define  OS_FLAG_WAIT_CLR_OR       1

#define  OS_FLAG_WAIT_SET_ALL      2        /* Wait for ALL    the bits specified to be SET (i.e. 1)   */
#define  OS_FLAG_WAIT_SET_AND      2

#define  OS_FLAG_WAIT_SET_ANY      3        /* Wait for ANY of the bits specified to be SET (i.e. 1)   */
#define  OS_FLAG_WAIT_SET_OR       3

#define  OS_FLAG_CONSUME        0x80        /* Consume the flags if condition(s) satisfied             */

#define  OS_FLAG_CLR               0
#define  OS_FLAG_SET               1

/*
 *********************************************************************************************************
 *       Possible values for 'opt' argument of OSSemDel(), OSMboxDel(), OSQDel() and OSMutexDel()
 *********************************************************************************************************
 */
#define  OS_DEL_NO_PEND            0
#define  OS_DEL_ALWAYS             1

/*
 *********************************************************************************************************
 *                                     OS???PostOpt() OPTIONS
 *
 * These #defines are used to establish the options for OSMboxPostOpt() and OSQPostOpt().
 *********************************************************************************************************
 */
#define  OS_POST_OPT_NONE       0x00        /* Post to highest priority task waiting                   */
#define  OS_POST_OPT_BROADCAST  0x01        /* Broadcast message to ALL tasks waiting                  */
#define  OS_POST_OPT_FRONT      0x02        /* Post to highest priority task waiting                   */

/*
 *********************************************************************************************************
 *                                 TASK OPTIONS (see OSTaskCreateExt())
 *********************************************************************************************************
 */
#define  OS_TASK_OPT_STK_CHK  0x0001        /* Enable stack checking for the task                      */
#define  OS_TASK_OPT_STK_CLR  0x0002        /* Clear the stack when the task is create                 */
#define  OS_TASK_OPT_SAVE_FP  0x0004        /* Save the contents of any floating-point registers       */

/*
 *********************************************************************************************************
 *                                             ERROR CODES
 *********************************************************************************************************
 */
#define OS_NO_ERR                 0

#define OS_ERR_EVENT_TYPE         1
#define OS_ERR_PEND_ISR           2
#define OS_ERR_POST_NULL_PTR      3
#define OS_ERR_PEVENT_NULL        4
#define OS_ERR_POST_ISR           5
#define OS_ERR_QUERY_ISR          6
#define OS_ERR_INVALID_OPT        7
#define OS_ERR_TASK_WAITING       8

#define OS_TIMEOUT               10
#define OS_TASK_NOT_EXIST        11
#define OS_PNAME_NULL            12
#define OS_MBOX_FULL             20

#define OS_Q_FULL                30

#define OS_PRIO_EXIST            40
#define OS_PRIO_ERR              41
#define OS_PRIO_INVALID          42

#define OS_SEM_OVF               50

#define OS_TASK_DEL_ERR          60
#define OS_TASK_DEL_IDLE         61
#define OS_TASK_DEL_REQ          62
#define OS_TASK_DEL_ISR          63
#define OS_TASK_NAME_TOO_LONG    64

#define OS_NO_MORE_TCB           70

#define OS_TIME_NOT_DLY          80
#define OS_TIME_INVALID_MINUTES  81
#define OS_TIME_INVALID_SECONDS  82
#define OS_TIME_INVALID_MILLI    83
#define OS_TIME_ZERO_DLY         84

#define OS_TASK_SUSPEND_PRIO     90
#define OS_TASK_SUSPEND_IDLE     91

#define OS_TASK_RESUME_PRIO     100
#define OS_TASK_NOT_SUSPENDED   101

#define OS_MEM_INVALID_PART     110
#define OS_MEM_INVALID_BLKS     111
#define OS_MEM_INVALID_SIZE     112
#define OS_MEM_NO_FREE_BLKS     113
#define OS_MEM_FULL             114
#define OS_MEM_INVALID_PBLK     115
#define OS_MEM_INVALID_PMEM     116
#define OS_MEM_INVALID_PDATA    117
#define OS_MEM_INVALID_ADDR     118

#define OS_ERR_NOT_MUTEX_OWNER  120

#define OS_TASK_OPT_ERR         130

#define OS_ERR_DEL_ISR          140
#define OS_ERR_CREATE_ISR       141

#define OS_FLAG_INVALID_PGRP    150
#define OS_FLAG_ERR_WAIT_TYPE   151
#define OS_FLAG_ERR_NOT_RDY     152
#define OS_FLAG_INVALID_OPT     153
#define OS_FLAG_GRP_DEPLETED    154

#define USER_DEFINE_NO_RUNNING	200

#ifndef _ASSEMBLER_
/*
 *********************************************************************************************************
 *                                          EVENT CONTROL BLOCK
 *********************************************************************************************************
 */

#if (OS_EVENT_EN > 0) && (OS_MAX_EVENTS > 0)
#define OS_EVENT_EXT_TYPE_COND 0
#define OS_EVENT_EXT_TYPE_MUTEX 1
#define OS_EVENT_EXT_TYPE_SEM 2
#define OS_EVENT_EXT_TYPE_MQ 3
typedef struct
{
    unsigned char os_event_ext_type;
    void *os_event_ext_ptr;
}os_event_ext_t;

typedef struct
{
    unsigned char os_event_type; /* Type of event control block (see OS_EVENT_TYPE_???)      */
    unsigned char os_event_grp; /* Group corresponding to tasks waiting for event to occur  */
    unsigned short os_event_cnt; /* Semaphore Count (not used if other EVENT type)           */
    void *os_event_ptr; /* Pointer to message or queue structure                    */
    unsigned char os_event_tbl[OS_EVENT_TBL_SIZE]; /* List of tasks waiting for event to occur                 */
#if OS_EVENT_NAME_SIZE > 1
    unsigned char os_event_name[OS_EVENT_NAME_SIZE];
#endif
    os_event_ext_t *os_event_ext_ptr;
}os_event_t;
#endif

/*
 *********************************************************************************************************
 *                                       EVENT FLAGS CONTROL BLOCK
 *********************************************************************************************************
 */

#if (OS_VERSION >= 251) && (OS_FLAG_EN > 0) && (OS_MAX_FLAGS > 0)
typedef struct
{ /* Event Flag Group                                        */
    unsigned char os_flag_type; /* Should be set to OS_EVENT_TYPE_FLAG                     */
    void *os_flag_wait_list; /* Pointer to first NODE of task waiting on event flag     */
    unsigned short os_flag_flags; /* 8, 16 or 32 bit flags                                   */
#if OS_FLAG_NAME_SIZE > 1
    unsigned char os_flag_name[OS_FLAG_NAME_SIZE];
#endif
    void *os_flag_ext_ptr;
}os_flag_grp_t;

typedef struct
{ /* Event Flag Wait List Node                               */
    void *os_flag_node_next; /* Pointer to next     NODE in wait list                   */
    void *os_flag_node_prev; /* Pointer to previous NODE in wait list                   */
    void *os_flag_node_tcb; /* Pointer to TCB of waiting task                          */
    void *os_flag_node_flag_grp; /* Pointer to Event Flag Group                             */
    unsigned short os_flag_node_flags; /* Event flag to wait on                                   */
    unsigned char os_flag_node_wait_type; /* Type of wait:                                           */
    /*      OS_FLAG_WAIT_AND                                   */
    /*      OS_FLAG_WAIT_ALL                                   */
    /*      OS_FLAG_WAIT_OR                                    */
    /*      OS_FLAG_WAIT_ANY                                   */
}os_flag_node_t;
#endif

/*
 *********************************************************************************************************
 *                                          MESSAGE QUEUE DATA
 *********************************************************************************************************
 */

#if OS_Q_EN > 0

typedef struct
{
    /*<<<<<edit by wurui for posix queue*/
    unsigned short type; // 消息的类型。
    unsigned short sub_type; // 消息的类型。
    /*>>>>>*/
    union // 消息的内容或参数。

    {
        unsigned int pointer;
        unsigned short data[2];
    }msg;
}msg_t;

typedef struct os_q_s
{ /* QUEUE CONTROL BLOCK                                         */
    struct os_q_s *os_q_ptr; /* link to next queue control block in list of free blocks     */
    msg_t *os_q_start; /* pointer to start of queue data                              */
    msg_t *os_q_end; /* pointer to end   of queue data                              */
    msg_t *os_q_in; /* pointer to where next message will be inserted  in   the q  */
    msg_t *os_q_out; /* pointer to where next message will be extracted from the q  */
    unsigned short os_q_size; /* size of queue (maximum number of entries)                   */
    unsigned short os_q_entries; /* current number of entries in the queue                      */
}os_q_t;

typedef struct
{
    void *os_msg; /* pointer to next message to be extracted from queue          */
    unsigned short os_n_msgs; /* number of messages in message queue                         */
    unsigned short os_q_size; /* size of message queue                                       */
    unsigned char os_event_tbl[OS_EVENT_TBL_SIZE]; /* List of tasks waiting for event to occur         */
    unsigned char os_event_grp; /* Group corresponding to tasks waiting for event to occur     */
}os_q_data_t;
#endif

/*
 *********************************************************************************************************
 *                                           SEMAPHORE DATA
 *********************************************************************************************************
 */

#if OS_SEM_EN > 0
typedef struct
{
    unsigned short os_cnt; /* Semaphore count                                         */
    unsigned char os_event_tbl[OS_EVENT_TBL_SIZE]; /* List of tasks waiting for event to occur                */
    unsigned char os_event_grp; /* Group corresponding to tasks waiting for event to occur */
}os_sem_data_t;
#endif

/*
 *********************************************************************************************************
 *                                          TASK CONTROL BLOCK
 *********************************************************************************************************
 */

typedef struct os_tcb_s
{
    unsigned int *os_tcb_stk_ptr; /* Pointer to current top of stack                              */

    void *os_tcb_ext_ptr; /* Pointer to user definable data for TCB extension             */
    unsigned int *os_tcb_stk_bottom; /* Pointer to bottom of stack                                   */
    unsigned short os_tcb_id; /* Task ID (0..65535)                                           */
    unsigned int os_tcb_stk_size; /* Size of task stack (in number of stack elements)             */

#if OS_TASK_CREATE_EXT_EN > 0
    unsigned short os_tcb_opt; /* Task options as passed by OSTaskCreateExt()                  */
#endif

    struct os_tcb_s *os_tcb_next; /* Pointer to next     TCB in the TCB list                      */
    struct os_tcb_s *os_tcb_prev; /* Pointer to previous TCB in the TCB list                      */

#if ((OS_Q_EN > 0) && (OS_MAX_QS > 0)) || (OS_MBOX_EN > 0) || (OS_SEM_EN > 0) || (OS_MUTEX_EN > 0)
    os_event_t *os_tcb_event_ptr; /* Pointer to event control block                               */
#endif

#if ((OS_Q_EN > 0) && (OS_MAX_QS > 0)) || (OS_MBOX_EN > 0)
    void *os_tcb_msg; /* Message received from OSMboxPost() or OSQPost()              */
#endif

#if (OS_VERSION >= 251) && (OS_FLAG_EN > 0) && (OS_MAX_FLAGS > 0)
#if OS_TASK_DEL_EN > 0
    os_flag_node_t *os_tcb_flag_node; /* Pointer to event flag node                                   */
#endif
    unsigned short os_tcb_flags_rdy; /* Event flags that made task ready to run                      */
#endif

    unsigned short os_tcb_dly; /* Nbr ticks to delay task or, timeout waiting for event        */
    unsigned char os_tcb_stat; /* Task status                                                  */
    unsigned char os_tcb_prio; /* Task priority (0 == highest, 63 == lowest)                   */

    unsigned char os_tcb_x; /* Bit position in group  corresponding to task priority (0..7) */
    unsigned char os_tcb_y; /* Index into ready table corresponding to task priority        */
    unsigned char os_tcb_bit_x; /* Bit mask to access bit position in ready table               */
    unsigned char os_tcb_bit_y; /* Bit mask to access bit position in ready group               */

#if OS_TASK_DEL_EN > 0
    unsigned char os_tcb_del_req; /* Indicates whether a task needs to delete itself              */
#endif

#if OS_TASK_NAME_SIZE>1
    unsigned char os_tcb_task_name[OS_TASK_NAME_SIZE];
#endif
    unsigned int os_tcb_sched_cnt;
    unsigned int os_tcb_exec_time; /*uint: ms*/
} os_tcb_t;

/*
 *********************************************************************************************************
 *                                         EVENT FLAGS MANAGEMENT
 *********************************************************************************************************
 */

#if (OS_VERSION >= 251) && (OS_FLAG_EN > 0) && (OS_MAX_FLAGS > 0)

#if OS_FLAG_ACCEPT_EN > 0
unsigned short os_flag_accept(os_flag_grp_t *pgrp, unsigned short flags, unsigned char wait_type, unsigned char *err);
#endif

os_flag_grp_t *os_flag_create(unsigned short flags, unsigned char *err);

#if OS_FLAG_DEL_EN > 0
os_flag_grp_t *os_flag_del(os_flag_grp_t *pgrp, unsigned char opt, unsigned char *err);
#endif

unsigned short os_flag_pend(os_flag_grp_t *pgrp, unsigned short flags, unsigned char wait_type, unsigned short timeout, unsigned char *err);

unsigned short os_flag_post(os_flag_grp_t *pgrp, unsigned short flags, unsigned char operation, unsigned char *err);

#if OS_FLAG_QUERY_EN > 0
unsigned short os_flag_query(os_flag_grp_t *pgrp, unsigned char *err);
#endif
#endif

/*
 *********************************************************************************************************
 *                                         MESSAGE QUEUE MANAGEMENT
 *********************************************************************************************************
 */

#if (OS_Q_EN > 0) && (OS_MAX_QS > 0)
#if OS_Q_ACCEPT_EN > 0
unsigned char os_q_accept(os_event_t *pevent, msg_t *msg);
#endif

os_event_t *os_q_create (msg_t *start, unsigned short size);

#if OS_Q_DEL_EN > 0
os_event_t *os_q_del (os_event_t *pevent, unsigned char opt, unsigned char *err);
#endif

#if OS_Q_FLUSH_EN > 0
unsigned char os_q_flush (os_event_t *pevent);
#endif

unsigned char os_q_pend(os_event_t *pevent, msg_t *msg, unsigned short timeout);

#if OS_Q_POST_EN > 0
unsigned char os_q_post (os_event_t *pevent, msg_t *msg);
#endif

/*<<<<<add by wurui for posix queue*/
#if OS_Q_POST_BY_PRIO_EN > 0
unsigned char os_q_post_by_prio (os_event_t *pevent, msg_t *msg, unsigned char flag, unsigned short timeout);
#endif
/*>>>>>*/

#if OS_Q_POSTISR_EN > 0
unsigned char os_q_post_isr(os_event_t *pevent, msg_t *msg);
#endif

#if OS_Q_POST_FRONT_EN > 0
unsigned char os_q_post_front (os_event_t *pevent, msg_t *msg);
#endif

#if OS_Q_POST_OPT_EN > 0
unsigned char os_q_post_opt (os_event_t *pevent, msg_t *msg, unsigned char opt);
#endif

#endif

/*
 *********************************************************************************************************
 *                                          SEMAPHORE MANAGEMENT
 *********************************************************************************************************
 */
#if OS_SEM_EN > 0

#if OS_SEM_ACCEPT_EN > 0
unsigned short os_sem_accept(os_event_t *pevent);
#endif

os_event_t *os_sem_create(unsigned short cnt);

#if OS_SEM_DEL_EN > 0
os_event_t *os_sem_del(os_event_t *pevent, unsigned char opt, unsigned char *err);
#endif

void os_sem_pend(os_event_t *pevent, unsigned short timeout, unsigned char *err);
unsigned char os_sem_post(os_event_t *pevent);

#if OS_SEM_QUERY_EN > 0
unsigned char os_sem_query(os_event_t *pevent, os_sem_data_t *pdata);
#endif

#endif

/*
 *********************************************************************************************************
 *                                            TASK MANAGEMENT
 *********************************************************************************************************
 */
#if OS_TASK_CHANGE_PRIO_EN > 0
unsigned char os_task_change_prio(unsigned char oldprio, unsigned char newprio);
#endif

#if OS_TASK_CREATE_EN > 0
//unsigned char         os_task_create(void (*task)(void *pd), void *pdata, unsigned int *ptos, unsigned char prio, unsigned short id);
unsigned char os_task_create (void (*task)(void *pd), void *pdata, unsigned int *ptos, unsigned char prio, unsigned short id, unsigned int *pbos);

#endif

#if OS_TASK_CREATE_EXT_EN > 0
unsigned char os_task_create_ext(void (*task)(void *pd),
        void *pdata,
        unsigned int *ptos,
        unsigned char prio,
        unsigned short id,
        unsigned int *pbos,
        unsigned int stk_size,
        void *pext,
        unsigned short opt);
unsigned char os_task_create_for_libc (void (*task)(void *pd),
        void *pdata,
        unsigned int task_entry,
        unsigned char prio,
        unsigned short id,
        unsigned int *pbos,
        unsigned int stk_size,
        void *pext,
        unsigned short opt);
#endif

#if OS_TASK_DEL_EN > 0
unsigned char os_task_del(unsigned char prio);
unsigned char os_task_del_req(unsigned char prio);
unsigned char os_task_del_for_libc(unsigned char prio, void * p_stack);
#endif

#if OS_TASK_SUSPEND_EN > 0
unsigned char os_task_resume(unsigned char prio);
unsigned char os_task_suspend(unsigned char prio);
#endif

#if OS_TASK_QUERY_EN > 0
unsigned char os_task_query(unsigned char prio, os_tcb_t *pdata);
#endif

#if OS_TASK_NAME_SIZE>1
unsigned char os_task_name_get(unsigned char prio, unsigned char *pname, unsigned char *err);
void os_task_name_set(unsigned char prio, unsigned char *pname, unsigned char *err);
#endif

/*
 *********************************************************************************************************
 *                                            TIME MANAGEMENT
 *********************************************************************************************************
 */

void os_time_dly(unsigned short ticks);

#if OS_TIME_DLY_HMSM_EN > 0
unsigned char os_time_dly_hmsm(unsigned char hours, unsigned char minutes, unsigned char seconds, unsigned short milli);
#endif

#if OS_TIME_DLY_RESUME_EN > 0
unsigned char os_time_dly_resume(unsigned char prio);
#endif

#if OS_TIME_GET_SET_EN > 0
unsigned int os_time_get(void);
void os_time_set(unsigned int ticks);
#endif

/*
 *********************************************************************************************************
 *                                             MISCELLANEOUS
 *********************************************************************************************************
 */

#if OS_SCHED_LOCK_EN > 0
void os_sched_lock(void);
void os_sched_unlock(void);
#endif

void os_sched(unsigned int cause, void *event);

void *os_get_task_own_ptr(unsigned char prio);
void os_set_task_own_ptr(void *p, unsigned char prio);
unsigned short os_get_id_from_prio(unsigned char prio);
unsigned char os_get_task_id(void);

/*
 *********************************************************************************************************
 *                                          FUNCTION PROTOTYPES
 *                                  (Compiler Specific ISR prototypes)
 *********************************************************************************************************
 */

void cp0_v_enable_im(unsigned int wcp0_im);

#endif/*_ASSEMBLER_*/
#endif/*__OS_H__*/
