/*
 *********************************************************************************************************
 *                                                uC/OS-II
 *                                          The Real-Time Kernel
 *
 *                           (c) Copyright 1992-2002, Jean J. Labrosse, Weston, FL
 *                                           All Rights Reserved
 *
 *                                  uC/OS-II Configuration File for V2.60
 *
 * File : OS_CFG.H
 * By   : Jean J. Labrosse
 *********************************************************************************************************
 */
#ifndef __OS_CFG_H__
#define __OS_CFG_H__
/* ---------------------- MISCELLANEOUS ----------------------- */
#define OS_ARG_CHK_EN             1    /* Enable (1) or Disable (0) argument checking                  */
#define OS_CPU_HOOKS_EN           1    /* uC/OS-II hooks are found in the processor port files         */

#define OS_EVENT_NAME_SIZE       12    /* Determine the size of the name of a Sem, Mutex, Mbox or Q    */
#define OS_LOWEST_PRIO           32    /* Defines the lowest priority that can be assigned ...         */
/* ... MUST NEVER be higher than 63!                            */
/* ... MUST be higher than OS_MAX_TASKS!                         */

#define OS_MAX_EVENTS            60    /* Max. number of event control blocks in your application      */
#define OS_MAX_FLAGS             10    /* Max. number of Event Flag Groups    in your application      */
#define OS_MAX_MEM_PART           1    /* Max. number of memory partitions                             */
#define OS_MAX_MEMBYTE_PART       2
#define OS_MAX_QS                 10    /* Max. number of queue control blocks in your application      */
#define OS_MAX_TASKS             30    /* Max. number of tasks in your application, MUST be >= 2       */

#define OS_SCHED_LOCK_EN          1    /*     Include code for OSSchedLock() and OSSchedUnlock()       */

#define OS_TASK_IDLE_STK_SIZE   128   /* Idle task stack size (# of OS_STK wide entries)              */

#define OS_TASK_STAT_EN           0    /* Enable (1) or Disable(0) the statistics task                 */
#define OS_TASK_STAT_STK_SIZE    64    /* Statistics task stack size (# of OS_STK wide entries)        */
#define OS_TASK_STAT_STK_CHK_EN   0    /* Check task stacks from statistic task                        */
#define OS_TICKS_PER_SEC        100    /* Set the number of ticks in one second                        */

/* ----------------------- EVENT FLAGS ------------------------ */
#define OS_FLAG_EN                1    /* Enable (1) or Disable (0) code generation for EVENT FLAGS    */
#define OS_FLAG_WAIT_CLR_EN       1    /* Include code for Wait on Clear EVENT FLAGS                   */
#define OS_FLAG_ACCEPT_EN         1    /*     Include code for OSFlagAccept()                          */
#define OS_FLAG_DEL_EN            1    /*     Include code for OSFlagDel()                             */
#define OS_FLAG_NAME_SIZE        12    /*     Determine the size of the name of an event flag group    */
#define OS_FLAG_QUERY_EN          1    /*     Include code for OSFlagQuery()                           */

/* -------------------- MESSAGE MAILBOXES --------------------- */
#define OS_MBOX_EN                0    /* Enable (1) or Disable (0) code generation for MAILBOXES      */
#define OS_MBOX_ACCEPT_EN         0    /*     Include code for OSMboxAccept()                          */
#define OS_MBOX_DEL_EN            0    /*     Include code for OSMboxDel()                             */
#define OS_MBOX_POST_EN           0    /*     Include code for OSMboxPost()                            */
#define OS_MBOX_POST_OPT_EN       0    /*     Include code for OSMboxPostOpt()                         */
#define OS_MBOX_QUERY_EN          0    /*     Include code for OSMboxQuery()                           */

/* --------------------- MEMORY MANAGEMENT -------------------- */
#define OS_MEM_EN                 0    /* Enable (1) or Disable (0) code generation for MEMORY MANAGER */
#define OS_MEM_QUERY_EN           0    /*     Include code for OSMemQuery()                            */
#define OS_MEM_NAME_SIZE         32    /*     Determine the size of a memory partition name            */

/* ---------------- MUTUAL EXCLUSION SEMAPHORES --------------- */
#define OS_MUTEX_EN               0    /* Enable (1) or Disable (0) code generation for MUTEX          */
#define OS_MUTEX_ACCEPT_EN        0    /*     Include code for OSMutexAccept()                         */
#define OS_MUTEX_DEL_EN           0    /*     Include code for OSMutexDel()                            */
#define OS_MUTEX_QUERY_EN         0    /*     Include code for OSMutexQuery()                          */

/* ---------------------- MESSAGE QUEUES ---------------------- */
#define OS_Q_EN                   1    /* Enable (1) or Disable (0) code generation for QUEUES         */
#define OS_Q_ACCEPT_EN            1    /*     Include code for OSQAccept()                             */
#define OS_Q_DEL_EN               1    /*     Include code for OSQDel()                                */
#define OS_Q_FLUSH_EN             1    /*     Include code for OSQFlush()                              */
#define OS_Q_POST_EN              1    /*     Include code for OSQPost()                               */
/*<<<<<add by wurui for posix queue*/
#define OS_Q_POST_BY_PRIO_EN      1    /*     Include code for OSQPostByPrio()                         */
/*>>>>>*/
#define OS_Q_POSTISR_EN           1    /*     Include code for OSQPost()                               */
#define OS_Q_POST_FRONT_EN        0    /*     Include code for OSQPostFront()                          */
#define OS_Q_POST_OPT_EN          0    /*     Include code for OSQPostOpt()                            */
#define OS_Q_QUERY_EN             0    /*     Include code for OSQQuery()                              */

/* ------------------------ SEMAPHORES ------------------------ */
#define OS_SEM_EN                 1    /* Enable (1) or Disable (0) code generation for SEMAPHORES     */
#define OS_SEM_ACCEPT_EN          1    /*    Include code for OSSemAccept()                            */
#define OS_SEM_DEL_EN             1    /*    Include code for OSSemDel()                               */
#define OS_SEM_QUERY_EN           1    /*    Include code for OSSemQuery()                             */

/* --------------------- TASK MANAGEMENT ---------------------- */
#define OS_TASK_CHANGE_PRIO_EN    1    /*     Include code for OSTaskChangePrio()                      */
#define OS_TASK_CREATE_EN         1    /*     Include code for OSTaskCreate()                          */
#define OS_TASK_CREATE_EXT_EN     1    /*     Include code for OSTaskCreateExt()                       */
#define OS_TASK_DEL_EN            1    /*     Include code for OSTaskDel()                             */
#define OS_TASK_NAME_SIZE        12    /*     Determine the size of a task name                        */
#define OS_TASK_PROFILE_EN        1    /*     Include variables in OS_TCB for profiling                */
#define OS_TASK_QUERY_EN          1    /*     Include code for OSTaskQuery()                           */
#define OS_TASK_SUSPEND_EN        1    /*     Include code for OSTaskSuspend() and OSTaskResume()      */
#define OS_TASK_SW_HOOK_EN        1    /*     Include code for OSTaskSwHook()                          */

/* --------------------- TIME MANAGEMENT ---------------------- */
#define OS_TIME_DLY_HMSM_EN       0    /*     Include code for OSTimeDlyHMSM()                         */
#define OS_TIME_DLY_RESUME_EN     1    /*     Include code for OSTimeDlyResume()                       */
#define OS_TIME_GET_SET_EN        1    /*     Include code for OSTimeGet() and OSTimeSet()             */

/*!
 *   ELF文件名最大长度
 */
#define MAX_ELF_NAME            128

/*!
 *   共享库支持的最大进程数
 */
#define MAX_PROCESS_NUM         16
/*!
 *   共享库支持的最小pid值
 */
#define MIN_PID_VALUE           10
/*!
 *   单个进程打开共享库的最大数
 */
#define MAX_SO_NUM              24

/*!
 *   软中断任务优先级配置
 */
#define SOFTIRQ_TASK_PRIO       0
/*!
 *   工作队列任务优先级配置
 */
#define WORKQUEUE_TASK_PRIO 1
/*!
 *   软中断任务Id配置
 */
#define SOFTIRQ_TASK_ID     0x1000
/*!
 *   工作队列任务Id配置
 */
#define WORKQUEUE_TASK_ID       0x1001

/*!
 *   sdram好区数
 */
#define GOOD_ZONES   11
/*!
 *   线程栈大小的上限值（64K）
 */
#define MAX_TASK_STACK_SIZE  0x10000

/*!
 *   线程内核栈大小（2K）
 */
#define KERNEL_STACK_SIZE  0x800

/*!
 *   连续物理内存的大小界限(*4KB)
 */
#define LARGE_DMA_MEM_MIN_SIZE  30

/*!
 *   配置是否使能内存页整理
 */
#define ENABLE_MAP_PAGE_DEFRAG 0

/*!
 *   配置是否使能只读页交换
 */
#define ENABLE_RO_PAGE_SWAP 0

/*!
 *   只读页交换监控周期(s)
 */
#define RO_PAGE_MONITOR_PERIOD 0

/*!
 *   DMA区页数
 */
#define ZONE_DMA_SMALL_PAGES   512
/*!
 *   映射区不交换页数
 */
#define ZONE_MAP_SWAP_PAGES   512
/*!
 *   映射区交换页数
 */
#define ZONE_MAP_UNSWAP_PAGES   256

/*!
 *   保留给UNSWAP区的物理页
 */
#define RESERVED_PAGE_FOR_UNSWAP 0

/*!
 *   保留给DMA_SMALL区的物理页
 */
#define RESERVED_PAGE_FOR_DMA_SMALL 0

/*!
 *   配置MALLOC的的实现方式（0---noya方式；1---kmalloc方式）
 */
#define MALLOC_REALIZE_TYPE 0

/*!
 *   配置是否使能MALLOC的slab功能（malloc为size较小的bucket保留1页）
 */
#define ENABLE_MALLOC_SLAB 0

/*!
 *   配置是否使能MALLOC延迟分配
 */
#define ENABLE_MALLOC_LAZY 0

/*!
 *   配置是否使能延迟加载
 */
#define ENABLE_LAZY_LOAD 0

/*!
 *   配置是否使能缺页异常中开中断
 */
#define ENABLE_PAGE_INVALID_EI  1

/*!
 *   vfs支持最大文件句柄
 */
#define MAX_FD_BITS 8
#define MAX_FD (2<<MAX_FD_BITS)

/*!
 *   字符设备管理数组的个数配置（超过的部分将通过动态申请/释放）
 */
#define MAX_CHRDEV      16
/*!
 *   块设备管理数组的个数配置（超过的部分将通过动态申请/释放）
 */
#define MAX_BLKDEV      8
/*!
 *   LFI/LFIBAK的目录项缓冲buf大小配置
 */
#define RBUF_SIZE   0x200

/*!
 *   内核符号管理数组的个数配置（该个数只会影响hash的碰撞几率）
 */
#define MAX_MODFUNC 0x64

/*!
 *   配置是否使能加载时代码校验
 */
#define ENALBE_LOAD_CODECHECK 1
/*!
 *   配置是否使能chipid
 */
#define ENABLE_CHIPID 0
/*!
 *   配置串口输出buf大小
 */
#define CFG_PBSIZE  256
/*!
 *   配置系统消息池层数
 */
#define MSGPOOL_DEPTH 10

/*!
 *   配置是否使能进程句柄监控
 */
#define ENABLE_FD_MONITOR 0

/*!
 *   配置是否使能系统资源监控功能
 */
#define  RESOURCE_MONITOR   0

#endif/*__OS_CFG_H__*/
