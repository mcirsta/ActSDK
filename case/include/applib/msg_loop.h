/********************************************************************************
 *                              USDK(1100)
 *                             Module: app
 *                 Copyright(c) 2003-2008 Actions Semiconductor,
 *                            All Rights Reserved.
 * file : msg_loop.h
 * History:
 *      <author>    <time>           <version >             <desc>
 *      Herb Hu    2009-3-24             v1.0             build this file
 ********************************************************************************/
/*!
 * \file     msg_loop.h
 * \brief    Ӧ����Ϣѭ��
 * \author   Herb Hu
 * \version 1.0
 * \date  2009/03/24
 *******************************************************************************/



#ifndef __MSG_LOOP_H__
#define __MSG_LOOP_H__

#include <app_msg.h>
#include "app_running_list.h"

/*! 
 * \defgroup   applib_msg_loop  Ӧ����Ϣѭ����ͨ�Žӿ�ģ�� 
 */

/******************************************************************************/
/*!
 * \par  Description:
 *      Ϊ��Ӧ�ô����ͳ�ʼ��ר����Ϣ���У����ڽ��̹�����Ϣ�б���Ϊ�䴴��һ����Ŀ
 * \ingroup applib_msg_loop
 * \param[in] app_name -- Ӧ�ý�����
 * \return �ɹ�����true, ʧ�ܷ���false
 * \note ���� manager����������һ��Ӧ��ǰ����
 *******************************************************************************/
extern bool applib_register_app(const char * app_name);

/******************************************************************************/
/*!
 * \par  Description:
 *     Ӧ���˳�ʱ������ר����Ϣ������Դ, ���ڽ�����Ϣ�б���ɾ���������Ŀ
 * \ingroup applib_msg_loop
 * \param[in] app_name -- Ӧ�ý�����
 * \return �ɹ�����true, ʧ�ܷ���false
 * \note ���� manager�����ڼ�⵽һ��Ӧ�ý����˳������
 *******************************************************************************/
extern bool applib_unregister_app(const char * app_name);

/******************************************************************************/
/*!
 * \par  Description:
 *    ���ӹ��������Ϣ�б�Ĺ����ڴ������򿪱�����ר����Ϣ����
 * \ingroup applib_msg_loop
 * \param[in] argc -- ��main()���������argc����
 * \param[in] argv -- ��main()���������argv����
 * \param[in] type -- ָ����������(console���̻���gui����)
 * \return �ɹ�����true, ʧ�ܷ���false
 *******************************************************************************/
extern bool applib_init(int argc , const char * argv[], app_type_e type);

/******************************************************************************/
/*!
 * \par  Description:
 *    �Ͽ����������Ϣ�б�Ĺ����ڴ������رձ�����ר����Ϣ����
 * \ingroup applib_msg_loop
 * \return �ɹ�����true, ʧ�ܷ���false
 *******************************************************************************/
extern bool applib_quit(void);

/******************************************************************************/
/*!
 * \par  Description:
 *    ��ȡ�����̵�ϵͳ��Ϣ��
 * \ingroup applib_msg_loop
 * \param[out] msg -- ��Ż�ȡ����ϵͳ��Ϣ
 * \return �յ���Ϣ����true, ����е���exit_msg_loop()�򷵻�false
 * \note ֻ���յ���Ϣ�������exit_msg_loop(), get_msg()�Ż᷵��
 *******************************************************************************/
extern bool get_msg(msg_apps_t *msg);

/******************************************************************************/
/*!
 * \par  Description:
 *    ��ָ�����̷���ͬ����Ϣ�����𱾽��̣�ֱ���Է��յ���Ϣ����ɴ���
 * \ingroup applib_msg_loop
 * \param[in] app_name -- Ŀ�������
 * \param[in] msg -- ��������Ϣ��ṹָ��
 * \param[out] reply -- ��Ż�ִ��Ϣ�Ľṹ��ָ��
 * \param[in] timeout -- ָ����ʱ�ȴ�ʱ��, ��timeout = NULL, ��ʾһֱ����ֱ���Է������Ϣ����
 * \return �ɹ�����true, ʧ�ܷ���false
 * \note �����Ը��Լ�����ͬ����Ϣ
 *******************************************************************************/
extern bool send_sync_msg(const char * app_name, const msg_apps_t *msg, msg_reply_t * reply, struct timespec *timeout);

/******************************************************************************/
/*!
 * \par  Description:
 *    �����յ�ͬ����Ϣ����Է����ػ�ִ��Ϣ
 * \ingroup applib_msg_loop
 * \param[in] reply -- �����صĻ�ִ��Ϣ�ṹ��ָ��
 * \return �ɹ�����true, ʧ�ܷ���false
 *******************************************************************************/
extern bool reply_sync_msg(const msg_reply_t * reply);

/******************************************************************************/
/*!
 * \par  Description:
 *    ��ָ�����̷����첽��Ϣ����ɺ��������أ�����ȴ��Է������Ϣ����
 * \ingroup applib_msg_loop
 * \param[in] app_name -- Ŀ�������
 * \param[in] msg -- ��������Ϣ��ṹָ��
 * \return �ɹ�����true,  ʧ�ܷ���false
 *******************************************************************************/
extern bool send_async_msg(const char * app_name, const msg_apps_t *msg);

/******************************************************************************/
/*!
 * \par  Description:
 *    ���͹㲥��Ϣ����ɺ��������أ�����ȴ��Է������Ϣ����
 * \ingroup applib_msg_loop
 * \param[in] msg -- ��������Ϣ��ṹָ��
 * \return �ɹ�����true, ʧ�ܷ���false
 *******************************************************************************/
extern bool broadcast_msg(const msg_apps_t *msg);

/******************************************************************************/
/*!
 * \par  Description:
 *    ץס��Ϣ����ֹ����Ӧ�ý��̽��ܴ���Ϣ
 * \ingroup applib_msg_loop
 * \param[in] msg_type -- ��Ҫץס����Ϣ���� 
 * \return �ɹ�����true, ʧ�ܷ���false
 * \note �˽ӿ�ֻ��Ӱ��㲥��Ϣ��Ŀǰ���ֻ֧��ͬʱgrab 32����Ϣ
 *******************************************************************************/
extern bool grab_msg(msg_apps_type_e msg_type);

/******************************************************************************/
/*!
 * \par  Description:
 *    �ͷ�grab����Ϣ��ʹ������Ӧ�ý���Ҳ���Խ��ܴ���Ϣ
 * \ingroup applib_msg_loop
 * \param[in] msg_type -- ��Ҫ�ͷŵ���Ϣ���� 
 * \return �ɹ�����true, ʧ�ܷ���false
 *******************************************************************************/
extern bool release_msg(msg_apps_type_e msg_type);

/******************************************************************************/
/*!
 * \par  Description:
 *    ������Ϣ��ʹ�ñ�Ӧ�ý��̲��ٽ��ܴ���Ϣ
 * \ingroup applib_msg_loop
 * \param[in] msg_type -- ��Ҫץס����Ϣ���� 
 * \return �ɹ�����true, ʧ�ܷ���false
 * \note �˽ӿڶ�ͬ�����첽�͹㲥��Ϣ����Ч��Ŀǰ���ֻ֧��ͬʱfilter 32����Ϣ
 *******************************************************************************/
extern bool filter_msg(msg_apps_type_e msg_type);

/******************************************************************************/
/*!
 * \par  Description:
 *    ���ĳ����Ϣ�Ĺ��ˣ�ʹ�ñ�Ӧ�ý��̿��Խ��ܴ���Ϣ
 * \ingroup applib_msg_loop
 * \param[in] msg_type -- ��Ҫ�ͷŵ���Ϣ���� 
 * \return �ɹ�����true, ʧ�ܷ���false
 *******************************************************************************/
extern bool unfilter_msg(msg_apps_type_e msg_type);

/******************************************************************************/
/*!
 * \par  Description:
 *    �������е�ϵͳ��Ϣ��ʹ�ñ�Ӧ�ý��̲��ٽ����κ�ϵͳ��Ϣ
 * \ingroup applib_msg_loop
 * \return �ɹ�����true, ʧ�ܷ���false
 * \note �˽ӿڶ�ͬ�����첽�͹㲥��Ϣ����Ч
 *******************************************************************************/
extern bool filter_all_msg(void);

/******************************************************************************/
/*!
 * \par  Description:
 *    �������ϵͳ��Ϣ�Ĺ��ˣ�ʹ�ñ�Ӧ�ý��̿��Խ�������ϵͳ��Ϣ
 * \ingroup applib_msg_loop
 * \return �ɹ�����true, ʧ�ܷ���false
 *******************************************************************************/
extern bool unfilter_all_msg(void);

/******************************************************************************/
/*!
 * \par  Description:
 *    ��ָ��ȫ����Ϣ���з����첽��Ϣ
 * \ingroup applib_msg_loop
 * \param[in] mq_name -- ȫ����Ϣ������ 
 * \param[in] msg -- ��������Ϣ��ṹָ��
 * \return �ɹ�����true,  ʧ�ܷ���false
 *******************************************************************************/
extern bool send_async_msg_to_global_mq(const char * mq_name, const msg_apps_t *msg);

/******************************************************************************/
/*!
 * \par  Description:
 *    ����ϵͳ��Ϣ��Ӧ�ý���ע���ϵͳ��Ϣ����������δע����ִ��Ĭ�ϴ�����
 * \ingroup applib_msg_loop
 * \param[in] msg -- ϵͳ��Ϣ�ṹ��ָ��
 *******************************************************************************/
extern void dispatch_msg(msg_apps_t *msg);

/******************************************************************************/
/*!
 * \par  Description:
 *    ע��Ӧ�ý��̵�ǰ��ϵͳ��Ϣ��������
 * \ingroup applib_msg_loop
 * \param[in] proc -- ϵͳ��Ϣ�ص�����ָ�� 
 * \return �ɹ�����true��ʧ�ܷ���false
 *******************************************************************************/
extern bool register_sys_dispatcher(sys_msg_proc proc);

/******************************************************************************/
/*!
 * \par  Description:
 *    ��ȡӦ�ý��̵�ǰ��ϵͳ��Ϣ��������
 * \ingroup applib_msg_loop
 * \return �ɹ����ص�ǰ����ע���ϵͳ��Ϣ������ָ�룬ʧ�ܷ���NULL
 *******************************************************************************/
extern sys_msg_proc get_sys_dispatcher(void);

/******************************************************************************/
/*!
 * \par  Description:
 *    ע��Ӧ�ý��̵�ǰ��ϵͳ��Ϣ��������
 * \ingroup applib_msg_loop
 * \return �ɹ�����true��ʧ�ܷ���false
 *******************************************************************************/
extern bool unregister_sys_dispatcher(void);

/******************************************************************************/
/*!                    
 * \par  Description:
 *     �˳���Ϣѭ��  
 * \ingroup applib_msg_loop
 * \return void 
 * \note ���ô˺�����get_msg()�᷵��false, �Ӷ��˳�while(get_msg())��Ϣѭ���ṹ 
 *******************************************************************************/
extern void exit_msg_loop(void);

/******************************************************************************/
/*!                    
 * \par  Description:
 *     ��ϵͳ��Ϣ�����������Ϣ  
 * \ingroup applib_msg_loop
 * \param[in] sys_msg_type -- ϵͳ��Ϣ���ͣ� ���Ϊ -1 ��������е�ϵͳ��Ϣ(ͬ����Ϣ����) 
 * \return  �ɹ�����true, ʧ�ܷ���false      
 * \note �˺���ֻ�����Ӧ�õ�ϵͳ��Ϣ���У��Ҳ������ͬ����Ϣ 
 * \par ����
 * \code
 * clear_sys_msg_queue(-1);              //������е�ϵͳ��Ϣ
 * clear_sys_msg_queue(MSG_CARD_IN);     //����������ϵͳ��Ϣ
 * \endcode
 *******************************************************************************/
extern bool clear_sys_msg_queue(int sys_msg_type);

/******************************************************************************/
/*!                    
 * \par  Description:
 *     �Ӱ�����Ϣ���������������Ϣ  
 * \ingroup applib_msg_loop
 * \param[in] key_val -- ����ֵ�����Ϊ-1����������еİ���״̬Ϊkey_type�İ�����Ϣ 
 * \param[in] key_type -- ����״̬�����Ϊ-1�����������ֵΪkey_val������״̬����Ϣ 
 * \return  �ɹ�����true, ʧ�ܷ���false      
 * \par ����
 * \code
 *  clear_key_msg_queue(-1, -1);                   // ������еİ�����Ϣ
 *  clear_key_msg_queue(KEY_PREV, -1);             // ��հ���ֵΪKEY_PREV��������Ϣ
 *  clear_key_msg_queue(KEY_PREV, KEY_TYPE_HOLD);  // ��հ���ֵΪKEY_PREV������hold״̬��Ϣ
 *  clear_key_msg_queue(-1, KEY_TYPE_HOLD);        // �������״̬Ϊhold�İ�����Ϣ
 * \endcode
 *******************************************************************************/
extern bool clear_key_msg_queue(int key_val, int key_type);

/******************************************************************************/
/*!                    
 * \par  Description:
 *    ����ǰӦ�ý��������̨����(console���̣����ٽ���GUI��Ϣ)
 * \ingroup applib_msg_loop
 * \return  �ɹ�����true, ʧ�ܷ���false      
 *******************************************************************************/
extern bool enter_background_mode(void);

/******************************************************************************/
/*!                    
 * \par  Description:
 *    ����ǰӦ�ý�������ǰ̨����(GUI���̣��ɽ���GUI��Ϣ)
 * \ingroup applib_msg_loop
 * \return  �ɹ�����true, ʧ�ܷ���false      
 * \note ��һ��Ӧ�ý�����Ϊǰ̨�󣬻���Ҫ���豾Ӧ�õĴ��ڽ��㣬������ȷ�յ�GUI��Ϣ 
 *******************************************************************************/
extern bool enter_foreground_mode(void);

/******************************************************************************/
/*!                    
 * \par  Description:
 *   ����ȫ����Ϣ���� 
 * \ingroup applib_msg_loop
 * \param[in] mq_name -- ��������ȫ����Ϣ������ 
 * \return  �ɹ�����true, ʧ�ܷ���false      
 *******************************************************************************/
extern bool create_global_msg_queue(const char * mq_name);

/******************************************************************************/
/*!                    
 * \par  Description:
 *   ɾ��ȫ����Ϣ���� 
 * \ingroup applib_msg_loop
 * \param[in] mq_name -- ��ɾ����ȫ����Ϣ������ 
 * \return  �ɹ�����true, ʧ�ܷ���false      
 *******************************************************************************/
extern bool delete_global_msg_queue(const char * mq_name);

/******************************************************************************/
/*!                    
 * \par  Description:
 *   ����һ��ȫ����Ϣ���У�ʹ�õ�ǰ��Ϣѭ�����Խ��մ�ȫ����Ϣ���е���Ϣ 
 * \ingroup applib_msg_loop
 * \param[in] mq_name -- �����ӵ�ȫ����Ϣ������ 
 * \param[in] proc --    ��Ϣ������ 
 * \return  �ɹ�����true, ʧ�ܷ���false      
 * \note ���procָ��ΪNULL����ʹ��Ĭ�ϵ�ϵͳ��Ϣ����������register_sys_dispatcher()
 *       ��ע���ϵͳ��Ϣ��������
 *******************************************************************************/
extern bool connect_global_msg_queue(const char * mq_name, sys_msg_proc proc);

/******************************************************************************/
/*!                    
 * \par  Description:
 *   ���һ��ȫ����Ϣ���е����ӣ�ʹ�õ�ǰ��Ϣѭ�����ٽ��մ�ȫ����Ϣ���е���Ϣ 
 * \ingroup applib_msg_loop
 * \param[in] mq_name -- ��������ӵ�ȫ����Ϣ������ 
 * \return  �ɹ�����true, ʧ�ܷ���false      
 *******************************************************************************/
extern bool disconnect_global_msg_queue(const char * mq_name);

/******************************************************************************/
/*!                    
 * \par  Description:
 *   �����ǰ���̵�����ȫ����Ϣ�������� 
 * \ingroup applib_msg_loop
 * \return  �ɹ�����true, ʧ�ܷ���false      
 *******************************************************************************/
extern bool disconnect_all_global_msg_queue(void);


#endif /* #ifndef __MSG_LOOP_H__ */
