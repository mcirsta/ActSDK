#ifndef __EMULATOR_SYS_DEP_H__
#define __EMULATOR_SYS_DEP_H__

#include "emulator.h"

typedef struct _simcomm_lib_handle
{
    char name[512];
    void *plib;

} simcomm_lib_handle_t;

//信号量服务
//create
int emutest_mq_create(simcomm_mq_handle_t *phandle, const char *mqname);

//accept
int emutest_mq_accept(simcomm_mq_handle_t *handle, char *pmsg, int len);

//pend
int emutest_mq_pend(simcomm_mq_handle_t *handle, char *pmsg, int len);
//pend
int emutest_mq_pend_timeout(simcomm_mq_handle_t *phandle, char *pmsg, int len, int timeout);

//post
int emutest_mq_post(simcomm_mq_handle_t *phandle, char *pmsg, int len);

//destroy
int emutest_mq_destroy(simcomm_mq_handle_t *phandle);

//库加载服务
//load
int emutest_lib_load(simcomm_lib_handle_t *phandle);

//close
int emutest_lib_close(simcomm_lib_handle_t *phandle);

//调整优先级
int adjust_task_priority(int newprio);

#endif
