/********************************************************************************
 *                              USDK(1100)
 *                            Module: MODULE
 *                 Copyright(c) 2003-2008 Actions Semiconductor,
 *                            All Rights Reserved.
 *
 * History:
 *      <author>    <time>           <version >             <desc>
 *       hmwei     2009-2-25 19:26     1.0             build this file
 ********************************************************************************/
/*!
 * \file     process.h
 * \brief    processͷ�ļ�
 * \author   hmwei
 * \version 1.0
 * \date  2009/2/25
 *******************************************************************************/
#ifndef __PROCESS_H__
#define __PROCESS_H__

#ifndef _ASSEMBLER_

#include <ucos/list.h>
#include <ucos/compiler.h>
#include <ucos/os_cfg.h>

#if (ENABLE_FD_MONITOR == 1)
struct pid_addr
{
    int fd;
    int addr;
    struct list_head list;
};

struct pid_fd
{
    struct list_head list;
};

extern struct pid_addr pid_addr_info[];
extern struct pid_fd pid_fd_info[];

#endif

/*!
 * \brief
 *      ������Ϣ���ݽṹ
 */
struct process
{
    /* ��������ڵ� */
    struct list_head list;

    /* Ӧ�ó������ڵ�ַ */
    unsigned int entry;

    /* Ӧ�ó���ĳ�ʼ������ */
    int (*_init)(void);

    /* Ӧ�ó���Ľ������� */
    void (*_fini)(void);
};

#endif /*_ASSEMBLER_*/
#endif  /* __PROCESS_H__ */
