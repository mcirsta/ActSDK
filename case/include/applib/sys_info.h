/********************************************************************************
 *                              USDK(1100)
 *                             Module: app 
 *                 Copyright(c) 2003-2008 Actions Semiconductor,
 *                            All Rights Reserved. 
 * file : sys_info.h
 * History:         
 *      <author>    <time>           <version >             <desc>
 *      Herb Hu    2009-07-06             v1.0             build this file 
 ********************************************************************************/
/*!
 * \file     sys_info.h
 * \brief    
 * \author   Herb Hu 
 * \version 1.0
 * \date  2009/03/24
 *******************************************************************************/


#ifndef __SYS_INFO_H__
#define __SYS_INFO_H__

#include <libc.h>

/*! 
 * \defgroup   applib_system_info   ϵͳ�Ȳ���豸��Ϣ��ѯ�ӿ� 
 */

/*!
 * \par  �Ȳ���豸���� 
 *  \li  HOTPLUGIN_DEV_TYPE_LOCAL: ���ش��� 
 *  \li  HOTPLUGIN_DEV_TYPE_CARD:  ���豸 
 *  \li  HOTPLUGIN_DEV_TYPE_UHOST: USB host ����ʱ���U���豸
 */
typedef enum 
{
    HOTPLUGIN_DEV_TYPE_NONE = 0,
    HOTPLUGIN_DEV_TYPE_LOCAL,
    HOTPLUGIN_DEV_TYPE_CARD,
    HOTPLUGIN_DEV_TYPE_UHOST,
}hotplugin_dev_type_e;

/*!
 * \par  �豸����״̬ 
 *  \li  HOTPLUGIN_DEV_IN:     �豸�Ѳ��� 
 *  \li  HOTPLUGIN_DEV_READY:  �豸��׼����
 *  \li  HOTPLUGIN_DEV_ERR:    �豸����
 *  \li  HOTPLUGIN_DEV_OUT:    �豸�Ѱγ�
 */
typedef enum
{
	HOTPLUGIN_DEV_IN = 0,
	HOTPLUGIN_DEV_READY,
	HOTPLUGIN_DEV_ERR,
	HOTPLUGIN_DEV_OUT,
}hotplugin_dev_insert_state_e;

/*!
 * \par  �豸����״̬ 
 *  \li  HOTPLUGIN_DEV_ACCESS_OK:   �豸����ͨ���ļ�ϵͳ���з��� 
 *  \li  HOTPLUGIN_DEV_ACCESS_ERR:  �豸������ͨ���ļ�ϵͳ���з���, ͨ������Ϊ���豸û�и�ʽ�� 
 */
typedef enum
{
	HOTPLUGIN_DEV_ACCESS_OK = 0,
	HOTPLUGIN_DEV_ACCESS_ERR,
}hotplugin_dev_access_state_e;

/*!
 *  \brief 
 *  �Ȳ���豸��Ϣ�ṹ����
 */
typedef struct
{
	 /*! ����״̬*/
    hotplugin_dev_insert_state_e insert;
	 /*! ����״̬*/
	hotplugin_dev_access_state_e access;
}hotplugin_dev_info_t;

/*!
 * \par  ���ղ����������ֵĵ�Դ��ѹ�������Ͷ��� 
 *  \li  POWER_THRESHOLD_GENERAL      : ��ͨ��������     
 *  \li  POWER_THRESHOLD_FILE_OP      : �ļ����� 
 *  \li  POWER_THRESHOLD_TVIN_REC     : ��Ƶ¼�� 
 *  \li  POWER_THRESHOLD_DC_REC       : ���� 
 *  \li  POWER_THRESHOLD_DV_REC       : ¼�� 
 *  \li  POWER_THRESHOLD_VOL_REC      : ¼�� 
 *  \li  POWER_THRESHOLD_UHOST_SUPORT : usb host ����֧�� 
 */
typedef enum 
{
    POWER_THRESHOLD_GENERAL = 0,
    POWER_THRESHOLD_FILE_OP,
    POWER_THRESHOLD_TVIN_REC,
    POWER_THRESHOLD_DC_REC,
    POWER_THRESHOLD_DV_REC,
    POWER_THRESHOLD_VOL_REC,
    POWER_THRESHOLD_UHOST_SUPPORT,
}power_threshold_type_e;

/******************************************************************************/
/*!                    
 * \par  Description:
 *      ��ȡ�Ȳ���豸�����Ϣ 
 * \ingroup applib_system_info
 * \param[in] dev -- �豸����
 * \param[out] info -- �豸��Ϣ
 * \return  �ɹ�����true, ʧ�ܷ���false      
 *******************************************************************************/
extern bool sys_get_hotplugin_dev_info(hotplugin_dev_type_e dev, hotplugin_dev_info_t * info);

/******************************************************************************/
/*!                    
 * \par  Description:
 *     ��鵱ǰ��Դ��ѹ�Ƿ���ĳ���������͵İ�ȫ����֮�ϣ��Ӷ���֤�����İ�ȫ����
 * \ingroup applib_system_info
 * \param[in] type -- ��������
 * \return  �ڰ�ȫ����֮�Ϸ���true, ���򷵻�false      
 *******************************************************************************/
extern bool sys_power_threshold_check(power_threshold_type_e type);

/******************************************************************************/
/*!                    
 * \par  Description:
 *    ��ȡ��ǰ���� 
 * \ingroup applib_system_info
 * \return  ������̴��ڣ����ص�ǰ���̣����򷵻�HOTPLUGIN_DEV_TYPE_NONE      
 *******************************************************************************/
extern hotplugin_dev_type_e sys_get_main_disk(void);

/******************************************************************************/
/*!                    
 * \par  Description:
 *    ���õ�ǰ���� 
 * \ingroup applib_system_info
 * \param[in] type -- ��������
 * \return  �ɹ�����true�����򷵻�false      
 * \note ����msg_serv�ȹ������ʹ��
 *******************************************************************************/
extern bool sys_set_main_disk(hotplugin_dev_type_e type);


#endif /*#ifndef __SYS_INFO_H__*/
