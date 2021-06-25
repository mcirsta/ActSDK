/********************************************************************************
 *                              USDK(1100)
 *                             Module: app
 *                 Copyright(c) 2003-2009 Actions Semiconductor,
 *                            All Rights Reserved.
 *
 * History:
 *      <author>    <time>           <version >             <desc>
 *       binannan    2009-05-05         1.0              initial version.
*******************************************************************************/
/*!
 * \file     dirlist.h
 * \brief    dirlist widget interface
 * \author   binannan
 * \version  0.8
 * \date     2009-05-05
*******************************************************************************/
/*!
 * \defgroup common_dirlist dirlist�ӿں���
 */
#ifndef __DIRLIST_H__
#define __DIRLIST_H__

#include "include_case.h"

/*!
 *  \par dirlist��ʾ����
 *  \li DIRLIST_MUSIC : ����Ŀ¼�б�
 *  \li DIRLIST_VIDEO : ��ƵĿ¼�б�
 *  \li DIRLIST_PHOTO : ͼƬĿ¼�б�
 *  \li DIRLIST_EBOOK : ������Ŀ¼�б�
 */

typedef enum
{
    DIRLIST_MUSIC,
    DIRLIST_VIDEO,
    DIRLIST_PHOTO,
    DIRLIST_EBOOK,
    DIRLIST_GAME,
    DIRLIST_DEFAULT,
}dirlist_type_e;


/*!
 *  \par dirlist�ؼ������ź�����
 *  \li WIDGET_SIG_DIRLIST_OK: ѡ���ļ�
 *  \li WIDGET_SIG_DIRLIST_QUIT:    Ŀ¼�б��˳�
 *  \li WIDGET_SIG_DIRLIST_DECODE:  ���뵱ǰ��ý���ļ�ͼƬ��Ϣ
 *  \li WIDGET_SIG_DIRLIST_SHORT_MENU:  emit when menu key short press
 *  \li WIDGET_SIG_DIRLIST_LONG_MENU:  emit when menu key long press
 */
typedef enum
{
    WIDGET_SIG_DIRLIST_OK,      //  emit when user press ok on a file item
    WIDGET_SIG_DIRLIST_QUIT,    //  emit when user press return in the first level
    WIDGET_SIG_DIRLIST_DECODE,  //  emit when a multimedia file need to show thumbnails
    WIDGET_SIG_DIRLIST_SHORT_MENU,
    WIDGET_SIG_DIRLIST_LONG_MENU,
}dirlist_sig_e;

typedef struct dirlist_init_s
{
    dirlist_type_e  type;
    char*           filter_string;
    char*           top_path_name;
    char*           init_path_name;
}dirlist_init_t;

/*!
 *  \brief
 *  dirlist �ؼ�����ṹ����
 */
typedef struct dirlist_s
{

    /*! Ŀ¼�б���ʾ���� */
    dirlist_type_e type;

    /*! ֱ�ӽ���Ŀ¼����ʱ�Ķ���Ŀ¼ */
    char*   top_path_name;

    /*! ��ʼ����Ŀ¼ ����ʱ�ĳ�ʼĿ¼*/
    char*   init_path_name;

    char*   filter_string;

    /*! ����ID */
    int canvas_id;

    /*! �źŻص����� */
    void (*callback)(struct dirlist_s*, dirlist_sig_e);

    /*! ��ǰĿ¼ȫ·����*/
    const char* path_name;

    /*! ��ǰѡ���ļ���*/
    const char* file_name;

    /*! �ڲ�˽�����ݽṹָ�룬���� dirlist �ؼ��ڲ�ʵ��ʹ�� */
    struct dirlist_private_data_s* data;
}dirlist_t;

/*!
 *  \brief
 *  dirlist �ؼ����źŻص���������
 */
typedef void (*dirlist_cb_t)(dirlist_t* dirlist, dirlist_sig_e);

/* dirlist �ؼ������ӿ� */


/******************************************************************************/
/*!
 * \par  Description:
 *	  dirlist �ؼ���������.
 * \ingroup common_dirlist
 * \param[in]    type  �б���ʾ����
 * \param[in]    callback   �źŻص�����
 * \return       ����dirlist�ؼ����,����ʧ��ʱ����NULL.
 ******************************************************************************/
dirlist_t* dirlist_create(dirlist_init_t* init, dirlist_cb_t callback);

/******************************************************************************/
/*!
* \par  Description:
*     ɾ��һ��dirlist�ؼ�
* \ingroup common_dirlist
* \param [in] dirlist: dirlist�ؼ����
* \return  �ɹ�����true, ʧ�ܷ���false
*******************************************************************************/
bool dirlist_delete(dirlist_t* dirlist);

/******************************************************************************/
/*!
* \par  Description:
*     dirlist ��ͼ�ӿ�
* \ingroup common_dirlist
* \param [in] dirlist: dirlist�ؼ����
* \return  �ɹ�����true, ʧ�ܷ���false
* \note �˺�������dirlist�ؼ�������Ԫ�أ�һ��������Ҫ�������dirlist�ؼ�ʱ
*******************************************************************************/
bool dirlist_paint(dirlist_t* dirlist);

/******************************************************************************/
/*!
* \par  Description:
*     dirlist�ؼ�GUI��Ϣ����������������Ҫ��GUI��Ϣ���ݸ��˺���
* \ingroup common_dirlist
* \param [in] dirlist: dirlist�ؼ����
* \param [in] pmsg: GUI��Ϣ
* \return  �ɹ�����true, ʧ�ܷ���false
* \note �ؼ��ڲ����¼�����ʱ,�ȵ��õ�ǰ�˵���Ļص�, �ٵ��ÿؼ����źŻص�����
*******************************************************************************/
bool dirlist_proc_gui(dirlist_t* dirlist, gui_wm_message_t* pmsg);

/******************************************************************************/
/*!
 * \par  Description:
 *     dirlist�ؼ�ϵͳ��Ϣ����������������Ҫ��ϵͳ��Ϣ���ݸ��˺���
 * \ingroup common_dirlist
 * \param [in] dirlist: dirlist�ؼ����
 * \param [in] pmsg: ϵͳ��Ϣ
 * \return  �ɹ�����true, ʧ�ܷ���false
 *******************************************************************************/
bool dirlist_proc_sys(dirlist_t* dirlist, msg_apps_t *pmsg );

/******************************************************************************/
/*!
* \par  Description:
*     dirlist �ؼ�����style��Դ
* \ingroup common_dirlist
* \param [in] dirlist: dirlist�ؼ����
* \return  �ɹ�����true, ʧ�ܷ���false
* \note �ⲿ���ý����ڶ�̬�л���ʾ���ʱ
*******************************************************************************/
bool dirlist_load_style(dirlist_t* dirlist);

/******************************************************************************/
/*!
* \par  Description:
*     ��ʾdirlist�ؼ�
* \ingroup common_dirlist
* \param [in] dirlist: dirlist�ؼ����
* \return  �ɹ�����true, ʧ�ܷ���false
*******************************************************************************/
bool dirlist_show( dirlist_t *dirlist );

/******************************************************************************/
/*!
* \par  Description:
*     ����dirlist�ؼ�
* \ingroup common_dirlist
* \param [in] dirlist: dirlist�ؼ����
* \return  �ɹ�����true, ʧ�ܷ���false
*******************************************************************************/
bool dirlist_hide( dirlist_t *dirlist );
/******************************************************************************/
/*!
* \par  Description:
*     dirlist �ؼ���ȡ��̬ͼƬ�Ŀ��
* \ingroup common_dirlist
* \param [in] dirlist: dirlist�ؼ����
* \return  �ɹ�����һ�� ��̬ͼƬ�Ŀ��, ʧ�ܷ��� -1
* \note dirlist �ж�̬ͼƬ���ⲿ�ṩ��������, ��music��ר��ͼƬ��videoԤ��ͼ,
*       ����ǰ�Ȼ�ȡͼƬ�ߴ�
*******************************************************************************/
int dirlist_get_bitmap_width(dirlist_t* dirlist);
/******************************************************************************/
/*!
* \par  Description:
*     dirlist �ؼ���ȡ��̬ͼƬ�ĸ߶�
* \ingroup common_dirlist
* \param [in] dirlist: dirlist�ؼ����
* \return  �ɹ�����һ�� ��̬ͼƬ�ĸ߶�, ʧ�ܷ��� -1
* \note dirlist �ж�̬ͼƬ���ⲿ�ṩ��������, ��music��ר��ͼƬ��videoԤ��ͼ,
*       ����ǰ�Ȼ�ȡͼƬ�ߴ�
*******************************************************************************/
int dirlist_get_bitmap_height(dirlist_t* dirlist);

/******************************************************************************/
/*!
* \par  Description:
*     dirlist �ؼ���ȡָ��indexλ�õ�menuitem��bitmap����
* \ingroup common_dirlist
* \param [in] dirlist: dirlist�ؼ����
* \param [in] index:   ��Ļ�����������
* \return  �ɹ�����һ��ָ��indexλ�õ�menuitem��bitmap����, ���������򷵻�NULL
* \note dirlist �ж�̬ͼƬ���ⲿ�ṩ�������ݣ��ؼ��ⲿ��ͨ�����ô˽ӿڻ�ȡindexλ�õ�
*               menuitem��bitmap����������
*******************************************************************************/
gui_bitmap_info_t* dirlist_get_item_bitmap(dirlist_t* dirlist, int idx);

/******************************************************************************/
/*!
* \par  Description:
*     dirlist �ؼ�����ָ��indexλ�õ�menuitem��bitmap����
* \ingroup common_dirlist
* \param [in] dirlist: dirlist�ؼ����
* \param [in] index:   ��Ļ�����������
* \param [in] bitmap:  �����õ�bitmap����
* \return  �ɹ����÷���true,ʧ�ܷ���false
* \note dirlist �ж�̬ͼƬ���ⲿ�ṩ�������ݣ��ؼ��ⲿ��ͨ�����ô˽ӿ�����indexλ�õ�
*               menuitem��bitmap����
*******************************************************************************/
bool dirlist_set_item_bitmap(dirlist_t* dirlist, int idx, gui_bitmap_info_t* bitmap);

/******************************************************************************/
/*!
* \par  Description:
*     dirlist �ؼ��ػ�ָ��indexλ�õ�menuitem��bitmap����
* \ingroup common_dirlist
* \param [in] dirlist: dirlist�ؼ����
* \param [in] index:   ��Ļ�����������
* \return  �ɹ����÷���true,ʧ�ܷ���false
* \note dirlist �ж�̬ͼƬ���ⲿ�ṩ�������ݣ��ؼ��ⲿ��ͨ�����ô˽ӿ��ػ�ָ��indexλ��
*               ��menuitem��bitmap����
*******************************************************************************/
bool dirlist_paint_bitmap(dirlist_t* dirlist, int idx );

/******************************************************************************/
/*!
* \par  Description:
*     dirlist �ؼ����������ַ���
* \ingroup common_dirlist
* \param [in] dirlist: dirlist�ؼ����
* \param [in] search_string: �����ַ���
* \return  �������������Ŀ
* \note dirlist ֧���ڵ�ǰĿ¼�½������������� dirlist���ڵ�ǰĿ¼�µ���Ŀ¼���ļ���
*               ������search_string���ַ��������ϴ���������Ŀ���Ϊ���������
*               �������»��δ��������������Ŀ���ϣ�����������search_stringΪ""��
*               �� dirlist����ĳ��Ŀ¼�������Ϸ��ظ�Ŀ¼ʱ����������ʧЧ��
*                *  APʹ��dirlist �����������谴�����·�ʽ��
 *                  1.  ����dirlist, ��ʾsoftkeyboard
 *                  2. ��ȡsk���ص������ַ���
 *                  3. ����dirlist�������ӿ�
 *                  4. ��ʾdirlist
*******************************************************************************/
int dirlist_set_search_string(dirlist_t* dirlist,char* search_string);

/******************************************************************************/
/*!
* \par  Description:
*     dirlist �ؼ����������ַ���
* \ingroup common_dirlist
* \param [in] dirlist: dirlist�ؼ����
* \return  �ɹ����ص�ǰʹ�õ��ļ�ѡ�������,ʧ�ܷ���NULL
* \note dirlist ʹ���ļ�ѡ����ʵ���ļ�Ŀ¼��������ܣ��˽ӿڷ��ص�ǰʹ�õ��ļ�
*               ѡ���������
*******************************************************************************/
FSEL_HANDLE dirlist_get_fsel_handle(dirlist_t* dirlist);

/******************************************************************************/
/*!
* \par  Description:
*     dirlist ��ȡ�˵��������
* \ingroup common_dirlist
* \param [in] dirlist: dirlist�ؼ����
* \return  �ɹ����� �˵��������, ʧ�ܷ��� -1
* \note �˴����ص���ĿΪdirlist��ǰ���뵽menuview�ؼ��еĲ˵���Ŀ������
*******************************************************************************/
int dirlist_get_total( dirlist_t* dirlist );

/******************************************************************************/
/*!
* \par  Description:
*     dirlist ��ȡ�˵������ʾ��ʼ�е�����
* \ingroup common_dirlist
* \param [in] dirlist: dirlist�ؼ����
* \return  �ɹ����� ��ʾ��ʼ�е������� ʧ�ܷ��� -1
* \note �˴����ص���ĿΪdirlist��ǰ���뵽menuview�ؼ��е���ʾ��ʼ�е�������
*******************************************************************************/
int dirlist_get_offset( dirlist_t *dirlist );

/******************************************************************************/
/*!
* \par  Description:
*     dirlist ��ȡ�˵���ĵ�ǰ�����е�����
* \ingroup common_dirlist
* \param [in] dirlist: dirlist�ؼ����
* \return  �ɹ����� ��ǰ�����е������� ʧ�ܷ��� -1
* \note �˴����ص���ĿΪdirlist��ǰ���뵽menuview�ؼ��еĸ����е�������
*******************************************************************************/
int dirlist_get_current( dirlist_t *dirlist );

/******************************************************************************/
/*!
* \par  Description:
*     dirlist ��ȡ�˵���index��Ŀ���ļ���
* \ingroup common_dirlist
* \param [in] dirlist: dirlist�ؼ����
* \return  �ɹ����� �˵���index��Ŀ���ļ����� ʧ�ܷ��� NULL
* \note �˴����ص���dirlist��index��Ŀ���ļ������˴��ļ�������ȫ·������
*******************************************************************************/
char* dirlist_get_item_name(dirlist_t* dirlist, int idx);


/******************************************************************************/
/*!
* \par  Description:
*     dirlist �жϲ˵���index��Ŀ���Ƿ�Ϊ�ļ���
* \ingroup common_dirlist
* \param [in] dirlist: dirlist�ؼ����
* \return  ��index��ĿΪ�ļ��з���true, Ϊ�ļ�����false
* \note
*******************************************************************************/
bool dirlist_is_item_dir(dirlist_t* dirlist, int idx);
/******************************************************************************/
/*!
* \par  Description:
*     dirlist ��ȡ��ǰĿ¼�µ��Ӳ˵����ļ��ϼ���
* \ingroup common_dirlist
* \param [in] dirlist: dirlist�ؼ����
* \return  �ɹ����� ��ǰĿ¼�µ��Ӳ˵����ļ��ϼ���, ʧ�ܷ��� -1
* \note �˴����ص���ĿΪ��ǰĿ¼�µ��Ӳ˵����ļ��ϼ�����
*******************************************************************************/
int dirlist_get_global_size( dirlist_t *dirlist );

/******************************************************************************/
/*!
* \par  Description:
*     dirlist ��ȡ��ǰ��ʾ�˵���ȫ����ǰĿ¼����Ŀ�����е�ƫ��
* \ingroup common_dirlist
* \param [in] dirlist: dirlist�ؼ����
* \return  �ɹ����� ��ǰ�˵���ʼ����ȫ�������е�������, ʧ�ܷ��� -1
* \note �˴����ص���ĿΪ��ǰ��ʾ�˵���ȫ����ǰĿ¼����Ŀ�����е�ƫ�ơ�
*******************************************************************************/
int dirlist_get_global_offset( dirlist_t *dirlist );

#endif /* __DIRLIST_H__ */
