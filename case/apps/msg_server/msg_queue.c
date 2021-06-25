/*
*******************************************************************************
*                       NOYA1100
*                (c) Copyright 2002-2008, Actions Co,Ld. 
*                        All Right Reserved 
* FileName: msg_queue.c
* Description:
* History: 
*       <author>    <time>          <version >      <desc>
*       shaofeng    2009/03/11      1.0             建立该文件。
*******************************************************************************
*/ 

#include "include_case.h"
#include "include_msg_server.h"




/* define variable, 变量定义 */
mqd_t mq_key_fd;



/* extern function, 函数声明 */
static mqd_t _create_msg_queue(const char *toke_str, struct mq_attr *attr);


#if 0
#endif
static mqd_t _create_msg_queue(const char *toke_str, struct mq_attr *attr)
{
    mqd_t msg_id;
      
    msg_id = mq_open(toke_str, O_CREAT | O_EXCL | O_WRONLY | O_NONBLOCK, 0644, attr);
    if( ((mqd_t)-1) == msg_id )
    {
        if(EEXIST == errno)
        {
            msg_id = mq_open(toke_str, O_WRONLY | O_NONBLOCK);
            if( ((mqd_t)-1) == msg_id )
            {
                return -1;
            }
        }   
    }
    
    return msg_id;
}


bool msg_queue_init(void)
{
    struct mq_attr attr;

    attr.mq_flags = 0;
    attr.mq_maxmsg = 16;
    attr.mq_msgsize = 16;
    attr.mq_curmsgs = 0;    
    mq_key_fd = _create_msg_queue(GUI_KEY_FIFO, &attr);
    if ( mq_key_fd < 0 )
    {
        return false;
    }

    return true;
}



bool msg_queue_deinit(void)
{
    mq_close(mq_key_fd);
    
    return true;
}

