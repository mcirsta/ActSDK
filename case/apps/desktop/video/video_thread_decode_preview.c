/*******************************************************************************
 *                              NOYA1100
 *                            Module: video
 *                 Copyright(c) 2003-2008 Actions Semiconductor,
 *                            All Rights Reserved. 
 * History:         
 *      <author>    <time>           <version >             <desc>
 *      shaofeng    2009-06-19          1.0             build this file 
*******************************************************************************/
/*!
 * \file     video_thread_decode_preview.c
 * \brief    
 * \author   shaofeng
 * \par      GENERAL DESCRIPTION:
 *               
 * \par      EXTERNALIZED FUNCTIONS:
 *               
 * \version 1.0
 * \date  2009/06/19
*******************************************************************************/

#include "include_psp.h"
#include "include_case.h"

/* include file of this application, ��Ӧ�õ�ͷ�ļ� */
#include "include_video.h"




/* define variable, �������� */
static pthread_t s_pthread;

/* ��Ҫ�����߳��˳�ʱͨ���˱����˳� */
static bool s_pthread_break_flag = false;


#if 0
#endif
/******************************************************************************/
/*!                    
* \par  Description:
*      ��ȡfullpath�ļ���timeλ�õ� preview����
*      �˹�����mmm�ļ����������ʵ��. �ڼ��������м����ͨ��stop��־�˳��˺���, �����˳��߳�
* \param[in]    char *: preview�ļ���ȫ·��
* \param[in]    int: tag��time����, ��λms
* \param[out]   mmm_cur_video_t *: preview����ָ��
* \retval       bool
*******************************************************************************/
bool video_thread_get_one_preview( char *fullpath, unsigned int time, mmm_cur_video_t *preview_info )
{
    bool result = true;
    
    if ( (NULL == fullpath) || (NULL == preview_info) || ( NULL == preview_info->buf ) )
    {
        return false;
    }
    
    //SET_FILEǰ��ֹ����err״̬, ��Ҫ��stop
    video_control( V_CMD_STOP, NULL );
    result = video_control( V_CMD_SET_FILE, fullpath );
    if( (false == result) || (true == s_pthread_break_flag) )
    {
        goto VIDEO_THREAD_GET_ONE_PREVIEW_ERR;
    }
    
    //���time��Ϊ0, ��Ҫseek����Ҫ������ͼ�Ĳ��ŵ�
    if ( time > 0)
    {
        result = video_control( V_CMD_SEEK, &time );
        if( (false == result) || (true == s_pthread_break_flag) )
        {
            goto VIDEO_THREAD_GET_ONE_PREVIEW_ERR;
        }
    }

    //��ȡһ֡ͼƬ��ע���ڵ��øýӿ�ʱ��Ҫ��video��������򿪣�
    //��֡ͼƬ��ֱ����ʾ����Ļ�ϡ�
    result = video_control( V_CMD_SHOW_NEW_FRAME, 0 );
    if( (false == result) || (true == s_pthread_break_flag) )
    {
        goto VIDEO_THREAD_GET_ONE_PREVIEW_ERR;
    }
     
    //��ȡ����ͼ
    result = video_control( V_CMD_PRINT_SCREEN, preview_info );
    
    VIDEO_THREAD_GET_ONE_PREVIEW_ERR:
    video_control( V_CMD_STOP, NULL );
    if ( true == s_pthread_break_flag )
    {
        result = false;
    }
    
    return result;
}


/******************************************************************************/
/*!                    
* \par  Description:
*      �����߳̽�����־. ͨ�����ý�����־����, ʹ�߳������м��⵽�˱�־�������˳�
* \retval       bool
*******************************************************************************/
bool video_thread_decode_preview_stop( void )
{
    s_pthread_break_flag = true;
    
    return true;
}


/******************************************************************************/
/*!                    
* \par  Description:
*      ����join���߳�, ��������ͼ����
* \retval       bool
*   ע��: 
*       �������̱߳������̵�һ�����߳�, ��֤������������
*       ���ô˺���ǰ��video_thread_decode_preview_delete, ��֤�߳��˳��ٴ���
*******************************************************************************/
bool video_thread_decode_preview_create( void *(*start_rtn)(void*) )
{
    int result = -1;
    pthread_attr_t pthread_attr;
    struct sched_param param;
//    struct sched_param self_param;
    int policy;
    
    s_pthread_break_flag = false;

    //�����������̵�һ�����߳�, ��֤������������
    pthread_attr_init(&pthread_attr);
    pthread_attr_setinheritsched(&pthread_attr, PTHREAD_EXPLICIT_SCHED);
    pthread_attr_setdetachstate(&pthread_attr, PTHREAD_CREATE_JOINABLE);
    pthread_attr_setscope(&pthread_attr, PTHREAD_SCOPE_PROCESS);
    
    //�����������̵�һ�����߳�, ��֤������������
    pthread_getschedparam( pthread_self(), &policy, &param );
    param.sched_priority = param.sched_priority - 2;
    pthread_attr_setschedparam(&pthread_attr, &param);
    print_dbg(" manager_msg_loop  thread's priority is: %d\n", param.sched_priority );

    result = pthread_create( &s_pthread, &pthread_attr, start_rtn, NULL );
    pthread_attr_destroy( &pthread_attr );
    if( result != 0 )
    {
        print_err("can��t create thread, result:%d\n", result);
        return false;
    }
    return true;
}

/******************************************************************************/
/*!                    
* \par  Description:
*      ɾ��join���߳�
* \retval       bool
*   ע��: 
*       �Դ�����join���߳�, �˺�����ȴ�, ֱ��Ŀ��thread����ֹ
*******************************************************************************/
bool video_thread_decode_preview_delete( void )
{
    if ( (0 != s_pthread.struct_id.pid) && (0 != s_pthread.struct_id.tid) )
    {
        pthread_join( s_pthread, NULL );
    }
    memset( &s_pthread, 0x00, sizeof(s_pthread) );
    
    return true;
}

