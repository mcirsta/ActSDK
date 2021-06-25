/********************************************************************************
 *                              USDK(1100)
 *                             Module: app 
 *                 Copyright(c) 2003-2008 Actions Semiconductor,
 *                            All Rights Reserved. 
 * file : app_timer.h
 * History:         
 *      <author>    <time>           <version >             <desc>
 *      Herb Hu    2009-3-24             v1.0             build this file 
 ********************************************************************************/
/*!
 * \file     app_timer.h
 * \brief    Ӧ�ü���ʱ���ӿ� 
 * \author   Herb Hu 
 * \version 1.0
 * \date  2009/03/24
 *******************************************************************************/

#ifndef __APP_TIMER_H__
#define __APP_TIMER_H__

#include <libc.h>

/*! 
 * \defgroup   applib_app_timer    Ӧ�ü���ʱ��
 */

/*!
 * \brief
 *  ��ʱ������ʱ�Ļص��������Ͷ���
 */
typedef int (* timer_proc)(void * param);

/******************************************************************************/
/*!
 * \par  Description:
 *     ����һ����ʱ��
 * \ingroup applib_app_timer
 * \param[in] timeout -- ��ʱ����ʱʱ�䣬��λΪ����
 * \param[in] func_proc -- ��ʱ������ʱ�Ļص�����
 * \param[in] param -- ���ݸ��ص������Ĳ��� 
 * \return �ɹ����ظö�ʱ����id, ʧ�ܷ���-1  
 * \note ��ʱʱ�䲻��������Ϊ0
 *******************************************************************************/
extern int set_timer(unsigned int timeout, timer_proc func_proc, void * param);

/******************************************************************************/
/*!
 * \par  Description:
 *     ����һ����������ʱ��
 * \ingroup applib_app_timer
 * \param[in] timeout -- ��ʱ����ʱʱ�䣬��λΪ����
 * \param[in] func_proc -- ��ʱ������ʱ�Ļص�����
 * \param[in] param -- ���ݸ��ص������Ĳ��� 
 * \return �ɹ����ظö�ʱ����id, ʧ�ܷ���-1  
 * \note ��ʱʱ�䲻��������Ϊ0 
 *******************************************************************************/
extern int set_single_shot_timer(unsigned int timeout, timer_proc func_proc, void * param);

/******************************************************************************/
/*!
 * \par  Description:
 *     �޸�һ����ʱ���Ķ�ʱʱ��
 * \ingroup applib_app_timer
 * \param[in] timer_id -- ��ʱ��id
 * \param[in] timeout -- �µĳ�ʱʱ�� 
 * \return �ɹ�����true, ʧ�ܷ���false  
 *******************************************************************************/
extern bool modify_timer(int timer_id, unsigned int timeout);

/******************************************************************************/
/*!
 * \par  Description:
 *     ֹͣһ����ʱ�� 
 * \ingroup applib_app_timer
 * \param[in] timer_id -- ��ʱ��id
 * \return �ɹ�����true, ʧ�ܷ���false  
 *******************************************************************************/
extern bool stop_timer(int timer_id);

/******************************************************************************/
/*!
 * \par  Description:
 *     ��������һ����ʱ��
 * \ingroup applib_app_timer
 * \param[in] timer_id -- ��ʱ��id
 * \return �ɹ�����true, ʧ�ܷ���false  
 *******************************************************************************/
extern bool restart_timer(int timer_id);

/******************************************************************************/
/*!
 * \par  Description:
 *     ɱ��ָ����ʱ��
 * \ingroup applib_app_timer
 * \param[in] timer_id -- ��ʱ��id
 * \return �ɹ�����true, ʧ�ܷ���false  
 *******************************************************************************/
extern bool kill_timer(int timer_id);

/******************************************************************************/
/*!
 * \par  Description:
 *     ɱ������Ӧ�ó����������õĶ�ʱ�� 
 * \return void
 * \ingroup applib_app_timer
 *******************************************************************************/
extern void kill_timers(void);




/******************************************************************************/
/*!
 * \par  Description:
 *     ����һ��applib�ڲ�ʹ�õĶ�ʱ�� 
 * \ingroup applib_app_timer
 * \param [in] timeout   ��ʱ����ʱʱ�䣬��λΪ����
 * \param [in] func_proc ��ʱ������ʱ�Ļص�����
 * \param [in] param     ���ݸ��ص������Ĳ��� 
 * \return �ɹ����ظö�ʱ����id��ʧ�ܷ���-1 
 * \note ����applib���ڲ�ʹ��
 *******************************************************************************/
extern int set_inner_timer(unsigned int timeout, timer_proc func_proc, void * param);

/******************************************************************************/
/*!
 * \par  Description:
 *     ����һ��applib�ڲ�ʹ�õĵ�������ʱ��
 * \ingroup applib_app_timer
 * \param [in] timeout   ��ʱ����ʱʱ�䣬��λΪ����
 * \param [in] func_proc ��ʱ������ʱ�Ļص�����
 * \param [in] param     ���ݸ��ص������Ĳ��� 
 * \return �ɹ����ظö�ʱ����id, ʧ�ܷ���-1  
 * \note ����applib���ڲ�ʹ��
 *******************************************************************************/
extern int set_inner_single_shot_timer(unsigned int timeout, timer_proc func_proc, void * param);

/******************************************************************************/
/*!
 * \par  Description:
 *     �޸�applib�ڲ���ʱ����ʱʱ��
 * \ingroup applib_app_timer
 * \param [in] timer_id ��ʱ��id
 * \param [in] timeout �µĳ�ʱʱ�� 
 * \return �ɹ�����true, ʧ�ܷ���false  
 * \note ����applib�ڲ�ʹ��
 *******************************************************************************/
extern bool modify_inner_timer(int timer_id, unsigned int timeout);

/******************************************************************************/
/*!
 * \par  Description:
 *     ֹͣһ��applib�ڲ���ʱ�� 
 * \ingroup applib_app_timer
 * \param [in] timer_id ��ʱ��id
 * \return �ɹ�����true, ʧ�ܷ���false  
 * \note ����applib�ڲ�ʹ��
 *******************************************************************************/
extern bool stop_inner_timer(int timer_id);

/******************************************************************************/
/*!
 * \par  Description:
 *     ��������һ��applib�ڲ���ʱ��
 * \ingroup applib_app_timer
 * \param [in] timer_id ��ʱ��id
 * \return �ɹ�����true, ʧ�ܷ���false  
 * \note ����applib�ڲ�ʹ��
 *******************************************************************************/
extern bool restart_inner_timer(int timer_id);

/******************************************************************************/
/*!
 * \par  Description:
 *      ɱ��applib�ڲ�ʹ�õ�ָ����ʱ�� 
 * \ingroup applib_app_timer
 * \param [in] timer_id ��ʱ��id
 * \return �ɹ�����true, ʧ�ܷ���false  
 * \note ����applib�ڲ�ʹ��
 *******************************************************************************/
extern bool kill_inner_timer(int timer_id);

/******************************************************************************/
/*!
 * \par  Description:
 *     ɱ������applib�ڲ�ʹ�õĶ�ʱ��
 * \ingroup applib_app_timer
 * \note ����applib�ڲ�ʹ��
 *******************************************************************************/
extern void kill_inner_timers(void);

/******************************************************************************/
/*!
 * \par  Description:
 *     ɾ������Ӧ�ó����������õĶ�ʱ�� 
 * \ingroup applib_app_timer
 * \note ����applib�ڲ�ʹ��
 *******************************************************************************/
extern void delete_timers(void);

/******************************************************************************/
/*!
 * \par  Description:
 *     ɾ������applib�ڲ�ʹ�õĶ�ʱ��
 * \ingroup applib_app_timer
 * \note ����applib�ڲ�ʹ��
 *******************************************************************************/
extern void delete_inner_timers(void);

/******************************************************************************/
/*!
 * \par  Description:
 *    ��ѯ�Ƿ��г�ʱ�Ķ�ʱ��, ����ҵ��͵����䴦����
 * \ingroup applib_app_timer
 * \note ����applib�ڲ�ʹ��
 *******************************************************************************/
extern void handle_timers(void);

/******************************************************************************/
/*!
 * \par  Description:
 *    Get current tick time in millisecond;
 * \ingroup applib_app_timer
 *******************************************************************************/
extern unsigned int get_tick_time_ms(void);

#endif /*#ifndef __APP_TIMER_H__*/
