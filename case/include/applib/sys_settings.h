/********************************************************************************
 *                              USDK(1100)
 *                             Module: app 
 *                 Copyright(c) 2003-2008 Actions Semiconductor,
 *                            All Rights Reserved. 
 * file : sys_settings.h
 * History:         
 *      <author>    <time>           <version >             <desc>
 *      Herb Hu    2009-3-24             v1.0             build this file 
 ********************************************************************************/
/*!
 * \file     sys_settings.h
 * \brief    
 * \author   Herb Hu 
 * \version 1.0
 * \date  2009/03/24
 *******************************************************************************/


#ifndef __SYS_SETTINGS_H__
#define __SYS_SETTINGS_H__

#include <libc.h>
#include <rtc.h>

/*! 
 * \defgroup   applib_system_settings    ϵͳ���ýӿ� 
 */

typedef struct 
{
    /*! ����ʱ�� ��λ: �� */
    unsigned int screensaver_time;
    /*! ʡ��ػ�ʱ�� ��λ: �� */
    unsigned int auto_standby_time;
    /*! ˯��ʱ�� ��λ���� */
    unsigned int timed_shutdown_time;
    /*! �������ȵȼ�  ����0~5 */
    unsigned int lcd_backlight;
}system_settings_t;

/******************************************************************************/
/*!                    
 * \par  Description:
 *     �Ƿ��ֹ����
 * \ingroup applib_system_settings
 * \param[in] forbid -- true: ��ֹ, false: ����ֹ
 * \return void      
 *******************************************************************************/
extern void sys_forbid_screensaver(bool forbid);

/******************************************************************************/
/*!                    
 * \par  Description:
 *    ������������ʱ�Ŀ���ʱ�� 
 * \ingroup applib_system_settings
 * \param[in] time -- ʱ��ֵ����λΪ��
 * \return �ɹ����� true, ʧ�ܷ���false      
 *******************************************************************************/
extern bool sys_set_screensaver_idle_time(unsigned int time);

/******************************************************************************/
/*!                    
 * \par  Description:
 *     ��鵱ǰ�����Ƿ񱻽�ֹ(ֻҪ��һ�����̽�ֹ��������������������ֹ) 
 * \ingroup applib_system_settings
 * \return �������ֹ����true, ���򷵻�false      
 *******************************************************************************/
extern bool sys_is_screensaver_forbidden(void);

/******************************************************************************/
/*!                    
 * \par  Description:
 *    �Ƿ��ֹʡ��ػ�
 * \ingroup applib_system_settings
 * \param[in] forbid -- true: ��ֹ��false: ����ֹ
 * \return  void      
 *******************************************************************************/
extern void sys_forbid_auto_standby(bool forbid);

/******************************************************************************/
/*!                    
 * \par  Description:
 *    ����ʡ��ػ�ʱ�Ŀ���ʱ�� 
 * \ingroup applib_system_settings
 * \param[in] time -- ʱ��ֵ����λΪ��
 * \return �ɹ����� true, ���򷵻�false      
 *******************************************************************************/
extern bool sys_set_auto_standby_idle_time(unsigned int time);

/******************************************************************************/
/*!                    
 * \par  Description:
 *    ���ʡ��ػ��Ƿ񱻽�ֹ(ֻҪ��һ�����̽�ֹ��������ʡ��ػ����Ʊ���ֹ) 
 * \ingroup applib_system_settings
 * \return �������ֹ����true, ���򷵻�false      
 *******************************************************************************/
extern bool sys_is_auto_standby_forbidden(void);

/******************************************************************************/
/*!                    
 * \par  Description:
 *    �Ƿ��ֹ��ʱ�ػ�
 * \ingroup applib_system_settings
 * \param[in] forbid -- true: ��ֹ��false: ����ֹ
 * \return  void
 *******************************************************************************/
extern void sys_forbid_timed_shutdown(bool forbid);

/******************************************************************************/
/*!
 * \par  Description:
 *    ���ö�ʱ�ػ���ʱ�� 
 * \ingroup applib_system_settings
 * \param[in] time -- ʱ��ֵ����λΪ��
 * \return �ɹ����� true, ���򷵻�false      
 *******************************************************************************/
extern bool sys_set_timed_shutdown_time(unsigned int time);

/******************************************************************************/
/*!                    
 * \par  Description:
 *    ��鶨ʱ�ػ��Ƿ񱻽�ֹ(ֻҪ��һ�����̽�ֹ��������ʡ��ػ����Ʊ���ֹ)
 * \ingroup applib_system_settings
 * \return �������ֹ����true, ���򷵻�false
 *******************************************************************************/
extern bool sys_is_timed_shutdown_forbidden(void);

/******************************************************************************/
/*!
 * \par  Description:
 *    ����LCD�������ȵȼ� 
 * \ingroup applib_system_settings
 * \param[in] val -- ���ȵȼ���0~5 
 * \return �ɹ����� true, ���򷵻�false      
 *******************************************************************************/
extern bool sys_set_lcd_backlight(unsigned int val);

/******************************************************************************/
/*!
 * \par  Description:
 *    ����rtc���ں�ʱ�� 
 * \ingroup applib_system_settings
 * \param[in] time -- ʱ��ṹ��ָ��, ΪNULLʱ, �������ø��� 
 * \param[in] date -- ���ڽṹ��ָ��, ΪNULLʱ, �������ø��� 
 * \return �ɹ�����true, ʧ�ܷ���false
 * \note ���time��date��ĳ��ֵΪ��ָ��NULL, �򲻻����ø���
 *******************************************************************************/
extern bool sys_set_rtc_date_time(rtc_time_t *time, rtc_date_t *date);

/******************************************************************************/
/*!
 * \par  Description:
 *    ��ȡrtc���ں�ʱ�� 
 * \ingroup applib_system_settings
 * \param[out] time -- ʱ��ṹ��ָ��, ΪNULLʱ, �����ȡ���� 
 * \param[out] date -- ���ڽṹ��ָ��, ΪNULLʱ, �����ȡ���� 
 * \return �ɹ�����true, ʧ�ܷ���false
 * \note ���time��date��ĳ��ֵΪ��ָ��NULL���򲻻��ȡ����
 *******************************************************************************/
extern bool sys_get_rtc_date_time(rtc_time_t *time, rtc_date_t *date);

/******************************************************************************/
/*!
 * \par  Description:
 *    ����drm rtc���ں�ʱ�� 
 * \ingroup applib_system_settings
 * \param[in] drm_time -- ʱ��ṹ��ָ��, ΪNULLʱ, �������ø��� 
 * \param[in] drm_date -- ���ڽṹ��ָ��, ΪNULLʱ, �������ø��� 
 * \return �ɹ�����true, ʧ�ܷ���false
 * \note ���drm_time��drm_date��ĳ��ֵΪ��ָ��NULL���򲻻����ø���
 *       һ��Ӧ���������
 *******************************************************************************/
extern bool sys_set_rtc_drm_date_time(rtc_time_t *drm_time, rtc_date_t *drm_date);

/******************************************************************************/
/*!
 * \par  Description:
 *   ��ȡdrm rtc���ں�ʱ�� 
 * \ingroup applib_system_settings
 * \param[out] drm_time -- ʱ��ṹ��ָ��, ΪNULLʱ, �����ȡ���� 
 * \param[out] drm_date -- ���ڽṹ��ָ��, ΪNULLʱ, �����ȡ���� 
 * \return �ɹ�����true, ʧ�ܷ���false
 * \note ���drm_time��drm_date��ĳ��ֵΪ��ָ��NULL, �򲻻��ȡ����
 *******************************************************************************/
extern bool sys_get_rtc_drm_date_time(rtc_time_t *drm_time, rtc_date_t *drm_date);

/******************************************************************************/
/*!
 * \par  Description:
 *    ������ʱ��ת���ɱ�׼�� 
 * \ingroup applib_system_settings
 * \param[in] time -- ʱ��ṹ��ָ��, ����ΪNULLָ��
 * \param[in] date -- ���ڽṹ��ָ��, ����ΪNULLָ��
 * \param[out] stdsec --  ��Ž��,  ����ΪNULLָ��
 * \return �ɹ�����true, ʧ�ܷ���false
 *******************************************************************************/
bool sys_rtc_change_to_stdsec(rtc_time_t *time, rtc_date_t *date, unsigned long * stdsec);

/******************************************************************************/
/*!
 * \par  Description:
 *    ��rtc�豸�ļ�
 * \ingroup applib_system_settings
 * \return �ɹ�����true, ʧ�ܷ���false
 * \note applib���ڲ�����, Ӧ���������
 *******************************************************************************/
extern bool sys_open_rtc_dev(void);

/******************************************************************************/
/*!
 * \par  Description:
 *    �ر�rtc�豸�ļ�
 * \ingroup applib_system_settings
 * \return �ɹ�����true, ʧ�ܷ���false
 * \note applib���ڲ�����, Ӧ���������
 *******************************************************************************/
extern bool sys_close_rtc_dev(void);

/******************************************************************************/
/*!
 * \par  Description:
 *    ���ð�����ʹ�����. ���ڽ��� �˳�audioӦ��, �رմ򿪰��������� 
 * \ingroup applib_system_settings
 * \param[in] flag_enable -- 1: ʹ�ܰ�������0����ֹ������ 
 * \return �ɹ�����true, ʧ�ܷ���false
 *******************************************************************************/
extern bool sys_set_key_tone_enable( int flag_enable );

/******************************************************************************/
/*!
 * \par  Description:
 *    �����Ƿ�����ϵͳ�¼�(USB���룬�������)�ļ�� 
 * \ingroup applib_system_settings
 * \param[in] on -- true : ����false�������� 
 * \return �ɹ�����true, ʧ�ܷ���false
 *******************************************************************************/
extern bool sys_enable_sys_evt_detect(bool on);

#endif /*#ifndef __SYS_SETTINGS_H__*/
