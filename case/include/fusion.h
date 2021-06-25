
/*******************************************************************************
 *                                  US213F
 *                            Module: Fusion
 *                 Copyright(c) 2003-2008 Actions Semiconductor,
 *                            All Rights Reserved.
 *
 * History:
 *      <author>    <time>           <version >             <desc>
 *       carrie     2009-04-07 10:00     1.0             build this file
*******************************************************************************/
/*!
 * \file     Fusion.h
 * \brief    the ui display fusion header file
 * \author   Carrie
 * \version 1.0
 * \date  2009/4/7
*******************************************************************************/
#ifndef  __FUSION_H__
#define  __FUSION_H__

#include "include_case.h"
#include "transimage.h"

/*!
*\brief
	��Ч����
*/
typedef enum
{
    /*! ����Ч*/
    FUSION_NORMAL = 0,

    /*! ���� */
    FUSION_PUSH,
    /*! ��ת */
    FUSION_REVERSE,
    /*! ������ */
    FUSION_CUBE,

    /*! GUI��RGB���浯��͸���Ӳ˵�*/
    FUSION_POPUPALPHA,
    /*! ����Ƶ��YUV�����ϵ����Ӳ˵� */
    FUSION_MOVEALPHA,

    /*! ��ת */
    FUSION_ROTATE,
    /*! �����Ӳ˵�*/
    FUSION_POPUP,
    /*! ����*/
    FUSION_FADE,
    /*! ����*/
    FUSION_CHECK,
    /*! ��Ҷ��*/
    FUSION_LOUVER,
    /*! �������*/
    FUSION_RANDOMLINE,
    /*! ˮ����*/
    FUSION_WAVE,
    /*! PPT*/
    FUSION_PPT,
    /*! ��ҳ ��ƽ���Ϸ������������ҳ */
    FUSION_PAGE,
    /*! �����Ч*/
    FUSION_RANDOM,
    /*! �и� */
    FUSION_CUT,
    /*! ���� */
    FUSION_ERASE,

    /*! ��Ч���ֵ */
    FUSION_MODE_MAX,
}fusion_mode_e;

/*!
 * \brief
 *   ��Ч�ص������������һ֡����Чʱ������һ�Σ�Ӧ�ÿ����Լ�����ͼƬ
 * \param[in]   framebuffer framebuffer�׵�ַ
 * \param[in]   width       framebuffer��
 * \param[in]   height      framebuffer��
 */
typedef void (*fusion_callback_t)(unsigned char *framebuffer, int width, int height);

/*!
 * \brief
 *     ��Ч֡��Ϣ
 */
typedef struct
{
    sty_rect_t      rect;   /*! ��Ļ������ */
    unsigned char   *pdata; /*! �������� */
    bool            alpha;  /*! �����Ƿ��alpha */
}fusion_frame_t;

/*!
 * \brief
 *     ��Чģʽ������
 */
typedef struct
{
    fusion_mode_e   mode;
    trans_direct_t  direct;
}fusion_effect_t;

/*!
 * \brief
 *     ��Ч��ҳ��������
 */
typedef enum
{
	/*! ��ǰһ������ */
    FUSION_FORWARD = 0,
    /*! �Ӻ�һ������ */     
    FUSION_BACKWARD,       
    /*! ���ֵ */     
    FUSION_MAX,
}fusion_enter_e;

/*!
 * \brief
 *     ��Ч����
 */
typedef enum
{
	/*! ��ͨҳ�� */
    FUSION_TYPE_COMMON = 0,
    /*! Ӧ���л� */
    FUSION_TYPE_APP_CHANGE,
    /*! Option�˵�RGB */
    FUSION_TYPE_OPTION_RGB,
    /*! Option�˵�YUV */
    FUSION_TYPE_OPTION_YUV,
    /*! ��ת�� */
    FUSION_TYPE_ROTATE,
    /*! �Ի��� */
    FUSION_TYPE_DIALOG,
    /*! ���͵����ֵ */
    FUSION_TYPE_MAX,        
}fusion_type_e;


/******************************************************************************/
/*!
 *\par  Description:
 *      ������Ч����
 *\param[in]   effect ��Ч�ṹ��ָ�루������Чģʽ�� ��Ч�ƶ�����
 *\retval  true  ���óɹ�
 *\retval  false ����ʧ��
 *\par
 *\note
 *******************************************************************************/
bool fusion_set_effect(fusion_effect_t *effect);


/******************************************************************************/
/*!
 *\par  Description:
 *      ������Ч�ĵ�һ֡
 *\param[in]   frame    fusion_frame_t�ṹ��ָ�룬����ΪNULL��
 *                      ���ΪNULL����ȡ��ǰȫ����framebuffer���ݣ�
 *                      ��MOVEALPHA����ʱ, frame->alpha����Ϊtrue
 *\retval  true  ���óɹ�
 *\retval  false ����ʧ��
 *\par
 *\note ������fusion_set_last_frame��fusion_displayһ�����
 *\note
 *******************************************************************************/
bool fusion_set_first_frame(fusion_frame_t *frame);


/******************************************************************************/
/*!
 *\par  Description:
 *      ������Ч�����һ֡
 *\param[in]   frame    fusion_frame_t�ṹ��ָ�룬����ΪNULL��
 *                      ���ΪNULL����ȡ��ǰȫ����framebuffer���ݣ�
 *                      ��POPUPALPHA��MOVEALPHA����ʱ, frame->alpha����Ϊtrue
 *\retval  true  ���óɹ�
 *\retval  false ����ʧ��
 *\par
 *\note ������fusion_set_first_frame��fusion_displayһ�����
 *\note
 *******************************************************************************/
bool fusion_set_last_frame(fusion_frame_t *frame);


/******************************************************************************/
/*!
 *\par  Description:
 *      ָ����Чģʽ��ʾ��Ч������fusion_set_type�趨�����ͼ�����
 *\param[in]   callback     ��Ч�ص�����ָ�룬ÿ���һ֡�����ã�����ΪNULL
 *\retval  true  ��ʾ�ɹ�
 *\retval  false ��ʾʧ��
 *\par
 *\note ������fusion_set_first_frame��fusion_set_last_frameһ�����
 *\par Example:
 *\code
 *  1. ȫ����Ч
 *      fusion_effect_t eff;
 *      fusion_get_config(FUSION_TYPE_COMMON, FUSION_FORWARD, &eff);
 *      fusion_set_effect(&eff);
 *      fusion_set_first_frame(NULL);
 *      gui_paint_somthing(...)
 *      fusion_set_last_frame(NULL);
 *      fusion_display(NULL);
 *
 *  2. ��ȫ��
 *      fusion_effect_t eff;
 *      fusion_frame_t frame;
 *      frame.rect.x = 10;
 *      frame.rect.y = 20;
 *      frame.rect.width = 100;
 *      frame.rect.height = 100;
 *      frame.pdata = NULL;
 *      frame.alpha = false;
 *      fusion_get_config(FUSION_TYPE_COMMON, FUSION_FORWARD, &eff);
 *      fusion_set_effect(&eff);
 *      fusion_set_first_frame(&frame);
 *      gui_paint_somthing(...)
 *      fusion_set_last_frame(&frame);
 *      fusion_display(NULL);
 *
 *  3. Option�˵�
 *      fusion_effect_t eff;
 *      canvas_info_t info;
 *      fusion_get_config(FUSION_TYPE_COMMON, FUSION_FORWARD, &eff);
 *      fusion_set_effect(&eff);
 *      fusion_set_first_frame(NULL);
 *      paint_option...;
 *      int result = canvas_get_info(canvas_id, &info); // �Ȼ�ȡ�Ӳ˵�ͼ������
 *      fusion_frame_t frame;
 *      frame.rect.x = info.rect.x;
 *      frame.rect.y = info.rect.y;
 *      frame.rect.width = info.rect.width;
 *      frame.rect.height = info.rect.height;
 *      frame.pdata = info.pdata;
 *      frame.alpha = true;
 *      fusion_set_last_frame(&frame);
 *      fusion_get_config(FUSION_TYPE_OPTION_RGB, FUSION_FORWARD, &eff);
 *      fusion_display(NULL);
 * \endcode
 *******************************************************************************/
bool fusion_display(fusion_callback_t callback);


/******************************************************************************/
/*!
 *\par  Description:
 *      ������Ч����ֵ
 *\param[in]   type     ��Ч����
 *\param[in]   enter    ���뵱ǰҳ�������ǰһ��ҳ����롢��һ��ҳ�淵��
 *\param[in]   effect   ��Ч����
 *\retval  true  ���óɹ�
 *\retval  false ����ʧ��
 *\par Example:
 *\code
 *    fusion_effect_t eff;
 *    eff.type = TRANS_LEFT;
 *    eff.mode = FUSION_CUBE;
 *    fusion_set_config(FUSION_TYPE_COMMON, FUSION_DIR_FORWARD, &eff);
 * \endcode
 *******************************************************************************/
bool fusion_set_config(fusion_type_e type, fusion_enter_e enter, const fusion_effect_t *effect);


/******************************************************************************/
/*!
 *\par  Description:
 *      ��ȡ��Ч����ֵ
 *\param[in]   type     ��Ч����
 *\param[in]   enter    ���뵱ǰҳ�������ǰһ��ҳ����롢��һ��ҳ�淵��
 *\param[out]  effect   ��Ч����
 *\retval  true  ���óɹ�
 *\retval  false ����ʧ��
 *\par Example:
 *\code
 *    fusion_effect_t eff;
 *    fusion_get_config(FUSION_TYPE_COMMON, FUSION_DIR_FORWARD, &eff);
 *    fusion_display(&eff, NULL);
 * \endcode
 *******************************************************************************/
bool fusion_get_config(fusion_type_e type, fusion_enter_e enter, fusion_effect_t *effect);

/******************************************************************************/
/*!
 *\par  Description:
 *      ������Ч����ֵ��setting�ָ���Ĭ������֮������ô˽ӿڣ�
 *\retval  true  ���óɹ�
 *\retval  false ����ʧ��
 *\par Example:
 *\code
 *    reset_all_config();
 *    fusion_update_config();
 * \endcode
 *******************************************************************************/
bool fusion_update_config(void);

#endif   /* ----- #ifndef FUSION_H  ----- */


