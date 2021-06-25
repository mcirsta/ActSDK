/********************************************************************************
 *                              USDK(1100)
 *                             Module: app 
 *                 Copyright(c) 2003-2009 Actions Semiconductor,
 *                            All Rights Reserved. 
 * File : volumebar.h
 * History:         
 *      <author>    <time>           <version >             <desc>
 *    wangsheng    2009-2-25          v1.0               build this file 
 ********************************************************************************/
/*!
* \file     volumebar.h
* \brief    volumebar widget
* \author   wangsheng
*               
* \version 1.0
* \date  2009/02/25
*******************************************************************************/
/*! 
  * \defgroup   common_volumebar    volumebar�ӿں���
  */


#ifndef __VOLUMEBAR_H__
#define __VOLUMEBAR_H__


#include "include_case.h"


/*! define volumebar data */
typedef struct volumebar_s
{
    int cur_value;   //!< the current volume value
    int min_value;   //!< the min volume value
    int max_value;   //!< the max volume value

    void (*callback)( struct volumebar_s *, int );//!< callback function

    /*! resource */
    int canvas_id;             //!< canvas id
    int bg_pic;			//!< background picture
    int type;			//!< display type	
    int timer_id;               //!< timer id

    struct volumebar_private_data_s *private_data;//!< volumebar private data
}volumebar_t;


/*! define widget signals */
typedef enum
{
    /*! �����ı�*/
    WIDGET_SIG_VOLUMEBAR_CHANGED,
    /*! volumebar�˳�*/
    WIDGET_SIG_VOLUMEBAR_QUIT,
}volumebar_sig_e;

/******************************************************************************/
/*!                    
  * \par  Description:
  *      volumebar�ص�����
  * \param[in]    data -- volumebar�����ݽṹ
                          signal -- volumebar�ź�
  * \retval      �ޡ�
  * \par example code
  * \code
  * \endcode
*******************************************************************************/
typedef void (*volumebar_cb_t)(volumebar_t *data, volumebar_sig_e signal);


/******************************************************************************/
/*!                    
* \par  Description:
*      volumebar��ں���
* \param[in]    data -- volumebar�����ݽṹ
                        pmsg -- gui��Ϣ
* \retval      true �ɹ���false ʧ��
* \ingroup      common_volumebar
* \par example code
* \code
*
* \endcode
* \note
*******************************************************************************/
int volumebar_proc_gui( volumebar_t *data, gui_wm_message_t *pmsg);

/*! volumebar external interface */

/******************************************************************************/
/*!                    
* \par  Description:
*      volumebarͼ�㴴������
* \param[in]    type -- volumebar��ʾ����
                       callback -- volumebar�ص�����
* \retval      �洢volumebar��������
* \ingroup      common_volumebar
* \par example code
* \code

* \endcode
* \note
*******************************************************************************/
volumebar_t* volumebar_create( int type, volumebar_cb_t callback );

/******************************************************************************/
/*!                    
* \par  Description:
*      volumebarͼ��ɾ������
* \param[in]    data -- volumebar�����ݽṹ
* \retval      true �ɹ���false ʧ��
* \ingroup      common_volumebar
* \par example code
* \code

* \endcode
* \note
*******************************************************************************/
bool volumebar_delete( volumebar_t *data );

/******************************************************************************/
/*!                    
* \par  Description:
*      volumebar��ͼ����
* \param[in]    data -- volumebar�����ݽṹ
* \retval      always be true 
* \ingroup      common_volumebar
* \par example code
* \code

* \endcode
* \note
*******************************************************************************/
bool volumebar_paint( volumebar_t *data );

/******************************************************************************/
/*!                    
* \par  Description:
*      volumebar��Դ���뺯��
* \param[in]    data -- volumebar�����ݽṹ
* \retval      true �ɹ���false ʧ��
* \ingroup      common_volumebar
* \par example code
* \code

* \endcode
* \note
*******************************************************************************/
bool volumebar_load_style( volumebar_t *data );

#endif
