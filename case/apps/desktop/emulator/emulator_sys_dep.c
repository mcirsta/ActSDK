#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <errno.h>
#include <mqueue.h>
#include <pthread.h>

#include "emulator_sys_dep.h"
#include "include_case.h"

//inner function
static mqd_t _create_msgqueue(const char *toke_str, struct mq_attr *attr);



int emutest_mq_create(simcomm_mq_handle_t *phandle, const char *name)
{

    struct mq_attr attr;

    attr.mq_flags = 0;
    //最多个数消息
    attr.mq_maxmsg = 30;
    //单个消息最大
    attr.mq_msgsize = sizeof(emu_ctrl_t);
    attr.mq_curmsgs = 0;
    *phandle = (unsigned int) _create_msgqueue(name, &attr);

    return 0;
}

//accept
int emutest_mq_accept(simcomm_mq_handle_t *phandle, char *pmsg, int len)
{
    int msq_prio;
    int num_bytes;

    num_bytes = (int)mq_receive((mqd_t) * phandle, (char *) pmsg, (unsigned int)len, &msq_prio);
    if (-1 == num_bytes)
    {
        //print_info("%s errno=%d\n", __FUNCTION__, errno);
    }
    return num_bytes;
}
//pend
int emutest_mq_pend(simcomm_mq_handle_t *phandle, char *pmsg, int len)
{
    int msq_prio;
    int num_bytes = 0;

    while (num_bytes <= 0)
    {
        usleep(10000);
        num_bytes = (int)mq_receive((mqd_t)* phandle, (char *)pmsg, (unsigned int)len, &msq_prio);
    }
    return num_bytes;
}

//pend_timeout
int emutest_mq_pend_timeout(simcomm_mq_handle_t *phandle, char *pmsg, int len, int timeout)
{
    int msq_prio;
    int ret;
    struct timespec ts;
    
    if(0 != clock_gettime(CLOCK_REALTIME, &ts))
    {	
        return -1;
    }
    
    ts.tv_nsec += (long)(timeout * 1000000 ); //timeout为毫秒，1毫秒=1000微秒=1000000皮秒   
    ret = (int)mq_timedreceive((mqd_t)* phandle, (char *)pmsg, (unsigned int)len, &msq_prio, &ts);
    
    return ret;
}

//post
int emutest_mq_post(simcomm_mq_handle_t *phandle, char *pmsg, int len)
{
    int size;

    size = mq_send((mqd_t) * phandle, (char *) pmsg, (unsigned int)len, 1);

    if (-1 == size)
    {
        return -1;
    }
    return 0;
}

//destroy
int emutest_mq_destroy(simcomm_mq_handle_t *phandle)
{
    if (phandle != NULL)
    {
        mq_close((mqd_t) * phandle);
        return 0;
    }
    else
    {
        return -1;
    }

}

//load
int emutest_lib_load(simcomm_lib_handle_t *phandle)
{
    if (0 == phandle)
    {
        return -1;
    }
    phandle->plib = dlopen(phandle->name, RTLD_LAZY | RTLD_NOW);
    return 0;
}

//close
int emutest_lib_close(simcomm_lib_handle_t *phandle)
{
    int eret;
    if (0 == phandle)
    {
        return -1;
    }
    eret = dlclose(phandle->plib);
    return 0;
}

static mqd_t _create_msgqueue(const char *toke_str, struct mq_attr *attr)
{
    int group_flags;
    mqd_t msg_id;

    group_flags = O_CREAT | O_EXCL | O_RDONLY;

    msg_id = mq_open(toke_str, O_CREAT | O_EXCL | O_RDWR, 0644, attr);
    if (msg_id == (mqd_t) - 1)
    {
        if (errno == EEXIST)
        {
            msg_id = mq_open(toke_str, O_RDWR | O_NONBLOCK);
            if (msg_id == (mqd_t) - 1)
            {
                exit(-1);
            }
            return msg_id;
        }
        return (mqd_t) - 1;
    }
    return msg_id;
}


