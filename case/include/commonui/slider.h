/*******************************************************************************
*                              USDK(1100)
*                            Module: common
*                 Copyright(c) 2003-2009 Actions Semiconductor,
*                            All Rights Reserved. 
*
* History:         
*      <author>    <time>           <version >             <desc>
*    Albert_lyu 2009-4-23  1.0             build this file 
*******************************************************************************/
/*!
* \file    slider.h
* \brief    slider����ͷ�ļ� 
* \author   Albert_lyu
* \version 1.0
* \date  2009-4-23
*******************************************************************************/
/*! 

 * \defgroup   common_slider    slider�ӿں���

 */


#ifndef _SLIDER_H_
#define _SLIDER_H_

#include "include_case.h"

typedef enum
{
    /*! slider����1-list  */
    W_SB_TYPE_LIST    = 1, 
    /*! slider����2-option */
    W_SB_TYPE_OPTION,
    /*! slider�������� */
    W_SB_TYPE_TOTAL
}slider_type_e;

/***
 slider����ҳ��ĵ�һ���仯���ƶ�

 sliderbar use example:

 01,	xxxx
 02,	xxxx
 ___________
 |03,	xxxx     |<----page_index = 3;				-----
 |04,	xxxx     |									    ^
 |05,	xxxx     |
 |06,	xxxx     |
 |**,	xxxx     | <-----select item ;cur_item = 7
 |08,	xxxx     |									item_perpage=8
 |09,	xxxx     |
 |10,	xxxx     |									    v
 __________|									------
 11,	xxxx
 12,	xxxx 	<--------total = 12

 cur_item = 07,total_item =12,item_perpage=8,page_index=3;
 ***/

/*!
 * \brief
 *      --slider init use patameter    ��Ҫ����Ĳ���
 */
typedef struct slider_init_s
{
    /*! total of list menu  �б������� */
    int total_item;
    /*! ÿҳ���� */
    int item_perpage;
    /*! ��ǰҳ����������������� */
    int page_index;
}slider_init_t;

typedef struct slide_s
{
    /*! ������� */
    slider_init_t init;
    /*! ����id */
    int canvas_id;
    /*! slider���� */
    int type;
    /*!callback*/
    void (*callback)(struct slide_s *, int);
    /* �ڲ�˽�����ݽṹָ�룬����slider�ؼ��ڲ�ʵ��ʹ�� */	
    struct slider_private_data_s * data;
    
    /*! �ⲿ���� */
    bool extern_canvas;
    /*! xƫ�� */
    int xoffset;
    /*! yƫ�� */
    int yoffset;
    
}slider_t;

//static int slider_timer_proc(void *param);
/******************************************************************************/
/*!
 * \par  Description:
 *	  slider_set_parameter-- ���ô������
 * \param[in]       slider_t* slider:�ؼ���� 
 * \param[in]       slider_init_t* parameter:�贫��Ĳ���
 * \param[out]   none
 * \return         �ɹ�����true, ʧ�ܷ���false     
 * \retval
 * \retval
 * \ingroup      common_slider
 * \par          exmaple code      none
 * \code
 *
 * \endcode
 * \note
 ************************************************ *******************************/
int slider_set_parameter(slider_t*slider, slider_init_t* parameter);
typedef void (*slider_cb_t)(slider_t *slider, int signal);
/******************************************************************************/
/*!
 * \par  Description:
 *	  slider_proc_gui-- �ؼ�GUI��Ϣ����������������Ҫ��GUI��Ϣ���ݸ��˺���
 * \param[in]       slider_t *slider:�ؼ���� 
 * \param[in]       gui_wm_message_t *pmsg: GUI��Ϣ 
 * \param[in]
 * \param[out]   none
 * \return       �ɹ�����true, ʧ�ܷ���false     
 * \retval
 * \retval
 * \ingroup      common_slider
 * \par          exmaple code   none
 * \code
 *
 * \endcode
 * \note
 ************************************************ *******************************/
int slider_proc_gui(slider_t *slider, gui_wm_message_t *pmsg);
/******************************************************************************/
/*!
 * \par  Description:
 *	  slider_create-- ����slider
 * \param[in]              int type:ָ��Ҫ����������
 * \param[in]              slider_cb_t callback:���� slider �źŵĻص����� 
 * \param[in]
 * \param[out]   none
 * \return        �ɹ�����true, ʧ�ܷ���false     
 * \retval
 * \retval
 * \ingroup      common_slider
 * \par          exmaple code  none
 * \code
 *
 * \endcode
 * \note
 *******************************************************************************/
slider_t* slider_create(int type, slider_cb_t callback, int canvas_id);
/******************************************************************************/
/*!
 * \par  Description:
 *	  slider_delete-- ɾ��slider�ؼ�
 * \param[in]       slider_t *slider:�ؼ���� 
 * \param[in]
 * \param[in]
 * \param[out]   none
 * \return         �ɹ�����true, ʧ�ܷ���false     
 * \retval
 * \retval
 * \ingroup      common_slider
 * \par          exmaple code  none
 * \code
 *
 * \endcode
 * \note
 ************************************************** *****************************/
bool slider_delete(slider_t *slider);
/******************************************************************************/
/*!
 * \par  Description:
 *	  slider_paint-- �ػ�slider�ؼ�
 * \param[in]     slider_t *slider:�ؼ���� 
 * \param[in]     none
 * \param[in]     none
 * \param[out]   none
 * \return         �ɹ�����true, ʧ�ܷ���false     
 * \retval
 * \retval
 * \ingroup      common_slider
 * \par          exmaple code  none
 * \code
 *
 * \endcode
 * \note
 *************************************************** ****************************/
bool slider_paint(slider_t *slider);
/******************************************************************************/
/*!
 * \par  Description:
 *	  slider_switch_type_test-- ���Բ�ͬ����л�
 * \param[in]            int type:ָ���л�����
 * \param[in]           slider_t *slider:�ؼ���� 
 * \param[in]
 * \param[out]   none
 * \return         �ɹ�����true, ʧ�ܷ���false     
 * \retval
 * \retval
 * \ingroup      common_slider
 * \par          exmaple code  none
 * \code
 *
 * \endcode
 * \note
 ****************************************************** *************************/
bool slider_switch_type_test(int type, slider_t *slider);

/******************************************************************************/
/*!
 * \par  Description:
 *	  slider_load_style-- װ��slider����Ҫ����Դ
 * \param[in]     none
 * \param[in]     none
 * \param[in]     none
 * \param[out]   none
 * \return       the result
 * \retval
 * \retval
 * \ingroup      common_slider
 * \par          exmaple code  none
 * \code
 *
 * \endcode
 * \note
 **************************************************** ***************************/
int slider_load_style(slider_t *slider);


#endif   /* _SLIDER_H_ */
