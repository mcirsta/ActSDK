/*
*******************************************************************************
*                       NOYA1100
*                (c) Copyright 2002-2008, Actions Co,Ld. 
*                        All Right Reserved 
* FileName: post_msg.c
* Description:
* History: 
*       <author>    <time>          <version >      <desc>
*       shaofeng    2009/03/11      1.0             建立该文件。
*******************************************************************************
*/ 

#include "include_case.h"
#include "include_msg_server.h"



/* extern function, 函数声明 */
static bool _check_queue_full(mqd_t mq_fd);



#if 0
#endif
/******************************************************************************/
/*!                    
* \par  Description:
*       检查消息队列是否还差一个满, 差一个用于保存最后一个关键数据, 如key up消息
* \param[in]    mqd_t: 需要检查的消息队列
* \retval       true: 满; false: 不满
*******************************************************************************/
static bool _check_queue_full(mqd_t mq_fd)
{
    struct mq_attr attr;
    
    mq_getattr(mq_fd,&attr); 

    if ( attr.mq_curmsgs >= (attr.mq_maxmsg-1) )
    {
        print_warning("full\n");
        return true;
    }
    else
    {
        return false;   
    }    
} 


/******************************************************************************/
/*!                    
* \par  Description:
*       将按键消息发送到消息队列中
* \param[in]    mqd_t: 需要发送的消息队列
* \param[in]    input_gui_msg_t: 需要发送的消息
* \param[in]    int: 1
* \retval       
*******************************************************************************/
bool post_key_msg(mqd_t mq_fd, input_gui_msg_t * input_msg, int force)
{
    int result;
    
    print_dbg("input_msg.type:%d, keyval:0x%x, keytype:0x%x", \
            input_msg->type, input_msg->data.kmsg.val, input_msg->data.kmsg.type);
    /*如果队列满,则丢弃*/
    if ( (0==force) && (true == _check_queue_full(mq_fd)) )
    {
        return false;
    }
    
    result = mq_send(mq_fd, (char *)input_msg, sizeof(input_gui_msg_t), 1);
    if (result < 0)
    {
        print_warning("mq_send faild, result:%d, mq_key_fd:%d\n", result, mq_fd);
        return false;
    }
    
    return true;
}



/******************************************************************************/
/*!                    
* \par  Description:
*       发送广播消息
* \param[in]    msg_apps_type_e: 需要发送的消息
* \retval       
*******************************************************************************/
bool post_broadcase_msg( msg_apps_type_e msg_type )
{
    msg_apps_t msg;

//    printf("msg_type:%d\n", msg_type);
    print_dbg("msg_type:%d", msg_type);
    msg.type = msg_type;
//    send_async_msg(const char * app_name, &msg);
    broadcast_msg( &msg );
    
    return true;
}

