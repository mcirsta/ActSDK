/********************************************************************************
 *                              USDK(1100)
 *                             Module: app 
 *                 Copyright(c) 2003-2008 Actions Semiconductor,
 *                            All Rights Reserved. 
 * file : adjust.h
 * History:         
 *      <author>    <time>           <version >             <desc>
 *      Jerry Fu    2009-4-14          v1.0             build this file 
 ********************************************************************************/
/*!
 * \file     adjust.h
 * \brief    adjust widget interface
 * \author   Jerry Fu 
 *               
 * \version 1.0
 * \date  2009/04/14
 *******************************************************************************/

/*! 
 * \defgroup   common_adjust    adjust�ؼ��ӿں���
 */
 
 
#ifndef      __ADJUST_H__
#define      __ADJUST_H__

#include "include_case.h"

#define    MAX_DISP_STRING    (16)

/*!
 *  \par  adjust �ؼ��ķ��
 *  \li ADJ_STYLE_TEXTDIGIT :�������ֻ����ı���Ϣ�����ݿ�
 *  \li ADJ_STYLE_SWITCH :����ON��OFF���Ե����ݵ��ڿ�
 *  \li ADJ_STYLE_TIME :����ʱ��ֵ�����ݵ��ڿ�
 *  \li ADJ_STYLE_DATE :����ʱ����ֵ�����ݵ��ڿ�
 *  \li ADJ_STYLE_RECLEN :����¼��ʱ������ݵ��ڿ�
 */
typedef  enum
{
    ADJ_STYLE_TEXTDIGIT,
    ADJ_STYLE_SWITCH,
    ADJ_STYLE_TIME,
    ADJ_STYLE_DATE,
    ADJ_STYLE_RECLEN,
}adjust_style_e;


/*!
 * \par  adjust�ؼ��ص��������ź�
 *  \li WIDGET_SIG_ADJUST_OK : ��������Ч
 *  \li WIDGET_SIG_ADJUST_CANCEL :������ȡ��
 *  \li WIDGET_SIG_ADJUST_SWITCH :�������ض���
 *  \li WIDGET_SIG_ADJUST_UP :��ֵ���ϵ���
 *  \li WIDGET_SIG_ADJUST_DOWN: ��ֵ���µ���
 */
typedef  enum
{
    WIDGET_SIG_ADJUST_OK,
    WIDGET_SIG_ADJUST_CANCEL,    
    WIDGET_SIG_ADJUST_SWITCHON,
    WIDGET_SIG_ADJUST_SWITCHOFF,
    WIDGET_SIG_ADJUST_UP,
    WIDGET_SIG_ADJUST_DOWN,
}adjust_signal_e;


/*!
 *  \par  ���п��ط������ݵ��ڿ�Ŀ���״̬
 *  \li ADJUST_ON : ���ڴ�״̬ 
 *  \li ADJUST_OFF : ���ڹر�״̬
 */
typedef  enum
{
    ADJUST_ON,
    ADJUST_OFF,
}adj_switch_mode_e;

/*!
 *  \par  ���п��ط������ݵ��ڿ�Ŀ���״̬
 *  \li TIME_MODE_24 : 24Сʱ��ʽ 
 *  \li TIME_FORMAT_12 : 12Сʱ��ʽ
 */
typedef  enum
{
    TIME_MODE_24,
    TIME_MODE_12,
}adj_time_mode_e;


typedef   struct
{
    /*! ���ڿ�Title���ַ�����ԴID�� */           
    int  str_title_id;

    /*! ���ڵ�λ���ַ�����ԴID�� */  
    int  str_unit_id;

    /*! ��ʾ��λ���ַ��� */
    char str_unit[8];

    /*! ������СֵʱҪ��ʾ���ַ�����ԴID�� */  
    int  str_min_id;

    /*! �������ֵʱҪ��ʾ���ַ�����ԴID�� */ 
    int  str_max_id;
    
    /*! ��ʾ�������ݵ��ַ�����ԴID��*/
    int  str_disptext_id[MAX_DISP_STRING];

    /*! �û������ʾ�ַ��� */
    char *str_disp[16];
    
    /*! �������ݵ����� */
    int max_val;
    
    /*! �������ݵ����� */
    int min_val;
    
    /*! ��ǰ�ļ��� */
    int cur_val;
    
    /*! �����Ĳ��� */
    int step;                  
}adjust_textdigit_init_t;


typedef   struct
{
    /*! ���ڿ�Title���ַ�����ԴID�� */           
    int  str_title_id;            
    /*! ���п������Ե����ݵ��ڿ��״̬ */
    adj_switch_mode_e  on_off_status;
    
}adjust_switch_init_t;


/*!
 *  \brief 
 *  ���õ���ʱ���adjust�ؼ��Ľṹ��
 */
typedef  struct
{
    /*! ����ʱ���TITLE*/
    int str_title_id; 
    
    /*! ����ϵͳʱ�� */
    rtc_time_t sys_time;                
}adjust_time_init_t;


/*!
 *  \brief 
 *  ���õ������ڵ�adjust�ؼ��Ľṹ��
 */
typedef  struct
{
    /*! �������ڵ�TITLE */
    int str_title_id;

    /*! ����ϵͳ���� */
    rtc_date_t sys_date;        
}adjust_date_init_t;

/*!
 *  \brief 
 *  ����adjust�ؼ��Ľṹ��
 */
typedef  struct  adjust_s
{   
    /*! ����ID */
    int canvas_id;
    
    /*! ��ͼDC */
    int hdc;

    /*! ����adjust�ؼ���Ԫ�ص���Դ��ָ�� */
    resgroup_resource_t *adjust_resgroup;
        
    /*! adjust�ؼ��ķ�� */
    adjust_style_e  adjust_style;
            
    /*! �����ʼ�������� */
    
    adjust_time_init_t  adjust_time;
    adjust_date_init_t  adjust_date;
    adjust_textdigit_init_t adjust_digit; 
    adjust_switch_init_t    adjust_switch;

    adj_time_mode_e  time_mode;
        
    /*! adjust�ؼ��Ļص����� */
    void ( *callback)(struct adjust_s * pAdjust, int signal);
    
    /*! �ڲ�˽�����ݽṹָ�룬����adjust �ؼ��ڲ�ʵ��ʹ�� */
    struct adjust_private_s * pAdjPrivate;    
}adjust_t;


/*!
 *  \brief 
 *  ����adjust �ؼ����źŻص���������
 */
typedef void (*adjust_cb_t)(adjust_t *pAdjust, adjust_signal_e signal);

/******************************************************************************/
/*!                    
* \par  Description:
*     adjust �ؼ�GUI��Ϣ����������������Ҫ��GUI��Ϣ���ݸ��˺���
* \ingroup common_adjust
* \param [in] data: adjust�ؼ��ľ��
* \param [in] pmsg: GUI��Ϣ
* \return  �ɹ�����true, ʧ�ܷ���false
*******************************************************************************/
bool adjust_proc_gui( adjust_t *pAdjust, gui_wm_message_t *pmsg );

/******************************************************************************/
/*!                    
* \par  Description:
*     ����һ��adjust�ؼ�  
* \ingroup common_adjust
* \param [in] adjust_para: adjust�ؼ���������� 
* \param [in] direction: LCD��ʾ�ķ��򣬺�����������
* \param [in] adjust_style: Ҫ������adjust�ؼ��ķ��
* \param [in] callback: ����adjust�źŵĻص�����
* \return  �ɹ�����һ�� adjust ���, ʧ�ܷ���NULL      
*******************************************************************************/
adjust_t* adjust_create(void *adjust_para,adjust_style_e adjust_style,adjust_cb_t callback );

/******************************************************************************/
/*!                    
* \par  Description:
*      ��common scene�л�ȡ�ؼ�����Դ
* \param[in]    adjust_t *: �ؼ�ָ��
* \retval       true: ��ȡstyle�ɹ�. false: ��ȡstyleʧ��
* \ingroup      common_adjust
*******************************************************************************/
bool adjust_load_style( adjust_t *pAdjust );

/******************************************************************************/
/*!                    
* \par  Description:
*     ɾ��һ��adjust�ؼ�  
* \ingroup common_adjust
* \param [in] keyboard: adjust�ؼ���� 
* \return  �ɹ�����true, ʧ�ܷ���false      
*******************************************************************************/
bool  adjust_delete( adjust_t *data );

/******************************************************************************/
/*!                    
* \par  Description:
*     adjust�ؼ���ͼ�ӿ�  
* \ingroup common_adjust
* \param [in] data: adjust�ؼ���� 
* \return  �ɹ�����true, ʧ�ܷ���false      
* \note �˺�������adjust�ؼ�������Ԫ��
*******************************************************************************/
bool  adjust_paint(adjust_t *data);

/******************************************************************************/
/*!                    
* \par  Description:
*     ��adjust�ؼ���ȡ��ǰ�ĵ���ֵ  
* \ingroup common_adjust
* \param [out] cur_val: ��ȡ�ĵ���ֵ��������
* \return  �ɹ�����true, ʧ�ܷ���false      
*******************************************************************************/
bool adjust_get_current_val(int *cur_val);


/******************************************************************************/
/*!                    
* \par  Description:
*     ��adjust�ؼ���ȡ��ǰ�ĵ���ֵ  
* \ingroup common_adjust
* \param [out] cur_val: ��ȡ�ĵ���ֵ��������
* \return  �ɹ�����true, ʧ�ܷ���false      
*******************************************************************************/
bool adjust_get_current_index(int *cur_index);


/******************************************************************************/
/*!                    
* \par  Description:
*     ��adjust��ȡ����״̬  
* \ingroup common_adjust
* \param [out] on_off_sta: �����ﷵ��adjust�ؼ������Ŀ���״̬
* \return  �ɹ�����true, ʧ�ܷ���false      
*******************************************************************************/
bool adjust_get_switch_status(adj_switch_mode_e *on_off_sta);

/******************************************************************************/
/*!                    
* \par  Description:
*     ��ȡadjust�ؼ���ǰ���趨ֵ
* \ingroup common_adjust
* \param [out] set_time: ��ǰ������ʱ�䣬��adjust�ؼ��ϰ���"OK"�Ժ���Ч
* \return  �ɹ�����true, ʧ�ܷ���false      
*******************************************************************************/
bool adjust_get_setting_time(rtc_time_t *set_time);

/******************************************************************************/
/*!                    
* \par  Description:
*     ��ȡadjust�ؼ���ǰ���趨ֵ
* \ingroup common_adjust
* \param [out] set_date: ��ǰ���������ڣ���adjust�ؼ��ϰ���"OK"�Ժ���Ч
* \return  �ɹ�����true, ʧ�ܷ���false      
*******************************************************************************/
bool adjust_get_setting_date(rtc_date_t *set_date);


/******************************************************************************/
/*!                    
* \par  Description:
*     ��ȡadjust�ؼ���ǰ���趨ֵ
* \ingroup common_adjust
* \param [out] record_length: ��ǰ���õ�¼�񳤶ȣ���adjust�ؼ��ϰ���"OK"�Ժ���Ч
* \return  �ɹ�����true, ʧ�ܷ���false      
*******************************************************************************/
bool adjust_get_recordtime(rtc_time_t *record_length);


#endif

