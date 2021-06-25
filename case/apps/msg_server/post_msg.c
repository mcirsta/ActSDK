/*
*******************************************************************************
*                       NOYA1100
*                (c) Copyright 2002-2008, Actions Co,Ld. 
*                        All Right Reserved 
* FileName: post_msg.c
* Description:
* History: 
*       <author>    <time>          <version >      <desc>
*       shaofeng    2009/03/11      1.0             �������ļ���
*******************************************************************************
*/ 

#include "include_case.h"
#include "include_msg_server.h"



/* extern function, �������� */
static bool _check_queue_full(mqd_t mq_fd);



#if 0
#endif
/******************************************************************************/
/*!                    
* \par  Description:
*       �����Ϣ�����Ƿ񻹲�һ����, ��һ�����ڱ������һ���ؼ�����, ��key up��Ϣ
* \param[in]    mqd_t: ��Ҫ������Ϣ����
* \retval       true: ��; false: ����
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
*       ��������Ϣ���͵���Ϣ������
* \param[in]    mqd_t: ��Ҫ���͵���Ϣ����
* \param[in]    input_gui_msg_t: ��Ҫ���͵���Ϣ
* \param[in]    int: 1
* \retval       
*******************************************************************************/
bool post_key_msg(mqd_t mq_fd, input_gui_msg_t * input_msg, int force)
{
    int result;
    
    print_dbg("input_msg.type:%d, keyval:0x%x, keytype:0x%x", \
            input_msg->type, input_msg->data.kmsg.val, input_msg->data.kmsg.type);
    /*���������,����*/
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
*       ���͹㲥��Ϣ
* \param[in]    msg_apps_type_e: ��Ҫ���͵���Ϣ
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

