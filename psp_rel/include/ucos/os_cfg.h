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
 *   ELF�ļ�����󳤶�
 */
#define MAX_ELF_NAME            128

/*!
 *   �����֧�ֵ���������
 */
#define MAX_PROCESS_NUM         16
/*!
 *   �����֧�ֵ���Сpidֵ
 */
#define MIN_PID_VALUE           10
/*!
 *   �������̴򿪹����������
 */
#define MAX_SO_NUM              24

/*!
 *   ���ж��������ȼ�����
 */
#define SOFTIRQ_TASK_PRIO       0
/*!
 *   ���������������ȼ�����
 */
#define WORKQUEUE_TASK_PRIO 1
/*!
 *   ���ж�����Id����
 */
#define SOFTIRQ_TASK_ID     0x1000
/*!
 *   ������������Id����
 */
#define WORKQUEUE_TASK_ID       0x1001

/*!
 *   sdram������
 */
#define GOOD_ZONES   11
/*!
 *   �߳�ջ��С������ֵ��64K��
 */
#define MAX_TASK_STACK_SIZE  0x10000

/*!
 *   �߳��ں�ջ��С��2K��
 */
#define KERNEL_STACK_SIZE  0x800

/*!
 *   ���������ڴ�Ĵ�С����(*4KB)
 */
#define LARGE_DMA_MEM_MIN_SIZE  30

/*!
 *   �����Ƿ�ʹ���ڴ�ҳ����
 */
#define ENABLE_MAP_PAGE_DEFRAG 0

/*!
 *   �����Ƿ�ʹ��ֻ��ҳ����
 */
#define ENABLE_RO_PAGE_SWAP 0

/*!
 *   ֻ��ҳ�����������(s)
 */
#define RO_PAGE_MONITOR_PERIOD 0

/*!
 *   DMA��ҳ��
 */
#define ZONE_DMA_SMALL_PAGES   512
/*!
 *   ӳ����������ҳ��
 */
#define ZONE_MAP_SWAP_PAGES   512
/*!
 *   ӳ��������ҳ��
 */
#define ZONE_MAP_UNSWAP_PAGES   256

/*!
 *   ������UNSWAP��������ҳ
 */
#define RESERVED_PAGE_FOR_UNSWAP 0

/*!
 *   ������DMA_SMALL��������ҳ
 */
#define RESERVED_PAGE_FOR_DMA_SMALL 0

/*!
 *   ����MALLOC�ĵ�ʵ�ַ�ʽ��0---noya��ʽ��1---kmalloc��ʽ��
 */
#define MALLOC_REALIZE_TYPE 0

/*!
 *   �����Ƿ�ʹ��MALLOC��slab���ܣ�mallocΪsize��С��bucket����1ҳ��
 */
#define ENABLE_MALLOC_SLAB 0

/*!
 *   �����Ƿ�ʹ��MALLOC�ӳٷ���
 */
#define ENABLE_MALLOC_LAZY 0

/*!
 *   �����Ƿ�ʹ���ӳټ���
 */
#define ENABLE_LAZY_LOAD 0

/*!
 *   �����Ƿ�ʹ��ȱҳ�쳣�п��ж�
 */
#define ENABLE_PAGE_INVALID_EI  1

/*!
 *   vfs֧������ļ����
 */
#define MAX_FD_BITS 8
#define MAX_FD (2<<MAX_FD_BITS)

/*!
 *   �ַ��豸��������ĸ������ã������Ĳ��ֽ�ͨ����̬����/�ͷţ�
 */
#define MAX_CHRDEV      16
/*!
 *   ���豸��������ĸ������ã������Ĳ��ֽ�ͨ����̬����/�ͷţ�
 */
#define MAX_BLKDEV      8
/*!
 *   LFI/LFIBAK��Ŀ¼���buf��С����
 */
#define RBUF_SIZE   0x200

/*!
 *   �ں˷��Ź�������ĸ������ã��ø���ֻ��Ӱ��hash����ײ���ʣ�
 */
#define MAX_MODFUNC 0x64

/*!
 *   �����Ƿ�ʹ�ܼ���ʱ����У��
 */
#define ENALBE_LOAD_CODECHECK 1
/*!
 *   �����Ƿ�ʹ��chipid
 */
#define ENABLE_CHIPID 0
/*!
 *   ���ô������buf��С
 */
#define CFG_PBSIZE  256
/*!
 *   ����ϵͳ��Ϣ�ز���
 */
#define MSGPOOL_DEPTH 10

/*!
 *   �����Ƿ�ʹ�ܽ��̾�����
 */
#define ENABLE_FD_MONITOR 0

/*!
 *   �����Ƿ�ʹ��ϵͳ��Դ��ع���
 */
#define  RESOURCE_MONITOR   0

#endif/*__OS_CFG_H__*/
