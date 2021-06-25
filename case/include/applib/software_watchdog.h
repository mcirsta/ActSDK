/********************************************************************************
 *                              USDK(1100)
 *                             Module: app 
 *                 Copyright(c) 2003-2008 Actions Semiconductor,
 *                            All Rights Reserved. 
 * file : software_watchdog.h
 * History:         
 *      <author>    <time>           <version >             <desc>
 *      Herb Hu    2009-3-24             v1.0             build this file 
 ********************************************************************************/
/*!
 * \file     software_watchdog.h
 * \brief    
 * \author   Herb Hu 
 * \version 1.0
 * \date  2009/03/24
 *******************************************************************************/



#ifndef __SOFTWARE_WATCHDOG_H__
#define __SOFTWARE_WATCHDOG_H__

/*! 
 * \defgroup   applib_software_watchdog   ���Ź��ӿ� 
 */

/******************************************************************************/
/*!                    
 * \par  Description:
 *      ��⵱ǰ�Ƿ��ֹ���Ź���
 * \ingroup applib_software_watchdog
 * \return �������ֹ����true, ���򷵻�false
 *******************************************************************************/
extern bool  sys_is_soft_watchdog_forbid(void);

/******************************************************************************/
/*!
 * \par  Description:
 *      �Ƿ��ֹ���Ź�
 * \ingroup applib_software_watchdog
 * \param[in] forbid -- true: ��ֹ��false: ����ֹ
 * \return  void
 *******************************************************************************/
extern void sys_forbid_soft_watchdog(bool forbid);

/******************************************************************************/
/*!
 * \par  Description:
 *      �������Ź���ʱʱ�� 
 * \ingroup applib_software_watchdog
 * \param[in] timeout -- ��ʱʱ�䣬��λΪ���룬�����Ϊ0����ر����Ź� 
 * \return void
 *******************************************************************************/
extern void set_soft_watchdog_timeout(unsigned int timeout);

/******************************************************************************/
/*!                    
 * \par  Description:
 *      �������Ӧ�ý��̵�software watchdog, ��������г�ʱ�ģ�kill ��Ӧ�ý���
 * \ingroup applib_software_watchdog
 * \return void
 * \note �ýӿڽ��� manager Ӧ�õ���
 *******************************************************************************/
extern void check_soft_watchdogs(void);

#endif /*#ifndef __SOFTWARE_WATCHDOG_H__*/
