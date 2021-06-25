/********************************************************************************
 *                              USDK(1100)
 *                             Module: app
 *                 Copyright(c) 2003-2008 Actions Semiconductor,
 *                            All Rights Reserved.
 * file : app_running_list.h
 * History:
 *      <author>    <time>           <version >             <desc>
 *      Herb Hu    2009-3-24             v1.0             build this file
 ********************************************************************************/
/*!
 * \file     app_running_list.h
 * \brief    Ӧ�ý�����Ϣ�б����ģ��
 * \author   Herb Hu
 * \version 1.0
 * \date  2009/03/24
 *******************************************************************************/



#ifndef __APP_RUNNING_LIST_H__
#define __APP_RUNNING_LIST_H__

/*! 
 * \defgroup   applib_app_running_list    Ӧ�ý�����Ϣ�б����
 */

#include <libc.h>

#define MAX_GRAB_MSG   (32)
#define MAX_FILTER_MSG (32)
#define MAX_GLOBAL_MQ  (32)
#define MAX_NAME_LEN   (64)


/*!
 * \par  Ӧ�ý��̻״̬ 
 *  \li  APP_STATE_INACTIVE: û�л 
 *  \li  APP_STATE_ACTIVE: � 
 */
typedef enum {
    APP_STATE_INACTIVE, 
    APP_STATE_ACTIVE
}app_state_e;

/*!
 * \par  Ӧ�ý������� 
 *  \li  APP_TYPE_GUI: GUI����(ǰ̨����, ���Խ���GUI��Ϣ) 
 *  \li  APP_TYPE_CONSOLE: CONSOLE����(��̨����, �����Խ���GUI��Ϣ) 
 */
typedef enum 
{
    APP_TYPE_GUI, 
    APP_TYPE_CONSOLE
}app_type_e;

/*!
 * \brief
 * ϵͳ��Ϣ�ص��������Ͷ���
 */
typedef int (* sys_msg_proc)(msg_apps_t *msg);

/*!
 * \brief
 *  �������ӵ�ȫ����Ϣ������Ϣ 
 */
typedef struct 
{
    char mq_name[MAX_NAME_LEN];
    int mqdes;
    sys_msg_proc proc;
}global_mq_data_t;

/*!
 * \brief
 *  ����һ��Ӧ�ý��̵������Ϣ
 */
typedef struct
{
    /*! ����ID */
    pid_t pid;
    /*! ������ */
    char name[32];
    /*! ����ר����Ϣ������ */
    char mq_name[MAX_NAME_LEN];
    /*! ����ר����Ϣ���������� */
    mqd_t mqdes;
    /*! �������� */
    app_type_e type;
    /*! ���̻״̬ */
    app_state_e state;
    /*! ���Ź��´γ�ʱ��ʱ�� */
    unsigned long soft_watchdog_timeout_expires;
    /*! �Ƿ��ֹ���Ź� */
    bool soft_watchdog_forbidden;
    /*! �Ƿ��ֹ���� */
    bool screensaver_forbidden;
    /*! �Ƿ��ֹʡ��ػ� */
    bool auto_standby_forbidden;
    /*! �Ƿ��ֹ˯�߹ػ� */
    bool timed_shutdown_forbidden;
    /*! ��grab��msg*/
    int msg_grabed[MAX_GRAB_MSG];
    /*! ��ʱ���Ƿ���Ч*/
    bool timer_valid;
    /*! ��filter��msg*/
    int msg_filtered[MAX_FILTER_MSG];
    /*! �Ƿ�filter���е�msg*/
    bool filter_all_msg;
    /*! ���ӵ�ȫ����Ϣ���� */
    global_mq_data_t global_mq[MAX_GLOBAL_MQ];
}app_info_t;

/*!
 * \brief
 * Ӧ�ý�����Ϣ�б��㶨��
 */
typedef struct app_list
{
    const app_info_t * app_info;
    struct app_list * next;
}app_running_list_t;

/******************************************************************************/
/*!
 * \par  Description:
 *     ��ʼ��������Ϣ�б�
 * \par ϸ�� 
 *     ���ӵ��洢������Ϣ�б�Ĺ����ڴ�, ����ù����ڴ治���ڣ��򴴽�֮
 * \ingroup applib_app_running_list
 * \return �ɹ�����true, ʧ�ܷ���false  
 *******************************************************************************/
extern bool app_running_list_init(void);

/******************************************************************************/
/*!
 * \par  Description:
 *     ע��������Ϣ�б�
 * \par ϸ�� 
 *     �Ͽ����洢������Ϣ�б�Ĺ����ڴ������
 * \ingroup applib_app_running_list
 * \return �ɹ�����true, ʧ�ܷ���false  
 *******************************************************************************/
extern bool app_running_list_deinit(void);

/******************************************************************************/
/*!
 * \par  Description:
 *     ͨ����������ѯһ�������Ƿ�����ڽ��̹�����Ϣ�б���
 * \ingroup applib_app_running_list
 * \param[in] app_name -- ������
 * \return �����̴���, ����true, ���򷵻�false
 *******************************************************************************/
extern bool is_app_exist(const char *app_name);

/******************************************************************************/
/*!
 * \par  Description:
 *    ͨ����������ѯӦ�ý�������(GUI���̻�console����)
 * \ingroup applib_app_running_list
 * \param[in]  app_name -- ������
 * \param[out] type -- ���ؽ������� 
 * \return �ɹ�����true, ���򷵻�false(���̲�����)
 *******************************************************************************/
extern bool get_app_type(const char *app_name, app_type_e * type);

/******************************************************************************/
/*!
 * \par  Description:
 *     ͨ����������ȡ�ý��̵�pid
 * \ingroup applib_app_running_list
 * \param[in] app_name -- ������
 * \return �����̴���, ������pid, ���򷵻�-1
 *******************************************************************************/
extern pid_t get_app_pid(const char * app_name);

/******************************************************************************/
/*!
 * \par  Description:
 *     ͨ������pid��ȡ�ý��̵Ľ�����
 * \ingroup applib_app_running_list
 * \param[in] pid -- ����pid
 * \return �����̴���, �����������, ���򷵻�NULL
 *******************************************************************************/
extern const char * get_app_name(pid_t pid);

/******************************************************************************/
/*!
 * \par  Description:
 *    ��ȡָ��Ӧ�ý��̵���Ϣ
 * \ingroup applib_app_running_list
 * \param[in] app_name -- ������
 * \return �����̴���, ���������ý�����Ϣ�Ľṹ��ָ��, ���򷵻�NULL
 *******************************************************************************/
extern const app_info_t * app_running_list_get_app_info(const char * app_name);

/******************************************************************************/
/*!
 * \par  Description:
 *    ��ȡ��Ӧ�ý��̵���Ϣ
 * \ingroup applib_app_running_list
 * \return �����̴���, ���������ý�����Ϣ�Ľṹ��ָ��, ���򷵻�NULL
 *******************************************************************************/
extern const app_info_t * app_running_list_get_this_app_info(void);

/******************************************************************************/
/*!
 * \par  Description:
 *    �޸�ָ��Ӧ�õ���Ϣ
 * \ingroup applib_app_running_list
 * \param[in] app_name -- ������
 * \param[in] app_info -- �µĽ�����Ϣ�ṹ��ָ��
 * \return �޸ĳɹ�����true, ���򷵻�false
 * \note ����manager �� applib ���ڲ�ʹ��
 *******************************************************************************/
extern bool app_running_list_set_app_info(const char * app_name, const app_info_t * app_info);

/******************************************************************************/
/*!
 * \par  Description:
 *    �޸ı�Ӧ�õ���Ϣ
 * \ingroup applib_app_running_list
 * \param[in] app_info -- �µĽ�����Ϣ�ṹ��ָ��
 * \return �޸ĳɹ�����true, ���򷵻�false
 * \note ����manager �� applib ���ڲ�ʹ��
 *******************************************************************************/
extern bool app_running_list_set_this_app_info(const app_info_t * app_info);

/******************************************************************************/
/*!
 * \par  Description:
 *     ��ȡ��ǰ����Ӧ�ý�����Ϣ�б�
 * \ingroup applib_app_running_list
 * \return �ɹ����ؽ�����Ϣ�б�ָ��, ���򷵻�NULL
 * \note ����manager �� applib ���ڲ�ʹ��
 *******************************************************************************/
extern  const app_running_list_t * app_running_list_get_list(void);

/******************************************************************************/
/*!
 * \par  Description:
 *     ����һ��Ӧ�ý�����Ϣ��������Ϣ�б�
 * \ingroup applib_app_running_list
 * \param[in] appinfo -- ָ��һ��������Ϣ�ṹ��
 * \return �ɹ�����true, ���򷵻�flase
 * \note ���� applib ���ڲ�ʹ��
 *******************************************************************************/
extern bool app_running_list_add(const app_info_t * appinfo);

/******************************************************************************/
/*!
 * \par  Description:
 *    ��Ӧ�ý�����Ϣ�б���ɾ��ָ��Ӧ�õ���Ϣ
 * \ingroup applib_app_running_list
 * \param[in] app_name -- Ӧ�ý�����
 * \return �ɹ�����true, ���򷵻�flase
 * \note ���� applib ���ڲ�ʹ��
 *******************************************************************************/
extern bool app_running_list_delete(const char * app_name);

/******************************************************************************/
/*!
 * \par  Description:
 *    ��ӡ������Ϣ�б�
 * \ingroup applib_app_running_list
 * \return void
 *******************************************************************************/
extern void print_app_running_list(void);

#endif /*#ifndef __APP_RUNNING_LIST_H__*/
