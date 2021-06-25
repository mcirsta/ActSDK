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
 * \file     filelist.h
 * \brief    filelist widget interface
 * \author   binannan
 * \version  0.8
 * \date     2009-05-05
 *******************************************************************************/
/*!
 * \defgroup common_filelist filelist�ӿں���
 */

#ifndef  __BROWSER_BROWSING_WIDGET_FILELIST__
#define  __BROWSER_BROWSING_WIDGET_FILELIST__

#include "include_case.h"
#include "include_psp.h"
#include "file_operation.h"


/*!
 *  \par filelist��ǰ��ʾ���б�����
 *  \li ENTRY : ��ǰ��ʾ�б�Ϊ�洢��ѡ���б�
 *  \li CONTENT : ��ǰ��ʾ�б�Ϊ�ļ�ϵͳĿ¼�б�
 */
typedef enum
{
    ENTRY, CONTENT,
} filelist_mode_e;

typedef enum
{
    ENTRY_INTERNAL,ENTRY_CARD,ENTRY_EXTERNAL,ENTRY_NONE,
}entryitem_e;


typedef enum
{
    ENTRYOP_CD,ENTRYOP_CDUP,ENTRYOP_NONE,
}entryop_e;

typedef enum
{
    CONTENT_NORMAL, CONTENT_MOVE, CONTENT_COPY, CONTENT_DELETE,
} content_mode_e;

/*!
 *  \par filelist�ؼ������ź�����
 *  \li WIDGET_SIG_FILELIST_OK: ѡ���ļ�
 *  \li WIDGET_SIG_FILELIST_QUIT:    Ŀ¼�б��˳�
 *  \li WIDGET_SIG_FILELIST_DECODE:  ���뵱ǰ��ý���ļ�ͼƬ��Ϣ
 */
typedef enum
{
    WIDGET_SIG_FILELIST_OK, //  emit when user press ok on a file item
    WIDGET_SIG_FILELIST_QUIT, //  emit when user press return in the first level
    WIDGET_SIG_FILELIST_DECODE,
    WIDGET_SIG_FILELIST_SHORT_MENU,
    WIDGET_SIG_FILELIST_DELETE,
    WIDGET_SIG_FILELIST_CONTENT_RELOAD,

} filelist_sig_e;

typedef struct dir_level_s
{
    char* workdir; //��ǰ����Ŀ¼

    int dirs; //��ǰĿ¼�µ���Ŀ¼��
    int dir_startno; //�б���ʾ�е���Ŀ¼��Ŀ�Ŀ�ʼ�ţ�����ļ�ѡ������
    int dir_num; //�б���ʾ�е���Ŀ¼��Ŀ��

    int files;
    int file_startno;
    int file_num;

    int item_group_size;
    menu_t* item_group; //������ʾ�Ĳ˵����ݼ���


    struct dir_level_s* parent;
    struct dir_level_s* child;
} dir_level_t;

typedef struct entry_list_s
{
    int canvas_id;
    menuview_t* entry_list;
    menu_t* entry_menu;
    menuview_type_e entry_list_type;

    string_resource_t* str_int_mem;
    string_resource_t* str_ext_mem;
    string_resource_t* str_car_mem;
    string_resource_t*  str_nodisk;

} entry_list_t;

typedef struct content_list_s
{
    int canvas_id;
    menuview_t* content_list;
    menuview_type_e content_list_type;

    void* p_mnavi;
    FSEL_HANDLE p_fsel;
    /*
     picture_resource_t* pic_fold;
     picture_resource_t* pic_fold_sel;
     picture_resource_t* pic_foldh;
     picture_resource_t* pic_foldh_sel;

     picture_resource_t* pic_music;
     picture_resource_t* pic_music_sel;
     picture_resource_t* pic_musich;
     picture_resource_t* pic_musich_sel;

     picture_resource_t* pic_txt;
     picture_resource_t* pic_txt_sel;
     picture_resource_t* pic_txth;
     picture_resource_t* pic_txth_sel;

     picture_resource_t* pic_video;
     picture_resource_t* pic_video_sel;
     picture_resource_t* pic_videoh;
     picture_resource_t* pic_videoh_sel;

     picture_resource_t* pic_photo;
     picture_resource_t* pic_photo_sel;
     picture_resource_t* pic_photoh;
     picture_resource_t* pic_photoh_sel;

     picture_resource_t* pic_file;
     picture_resource_t* pic_file_sel;
     picture_resource_t* pic_fileh;
     picture_resource_t* pic_fileh_sel;
     */
    picture_resource_t* pic_array[TYPE_DEFAULT + 1];
    picture_resource_t* pic_sel_array[TYPE_DEFAULT + 1];
    picture_resource_t* pich_array[TYPE_DEFAULT + 1];
    picture_resource_t* pich_sel_array[TYPE_DEFAULT + 1];

    string_resource_t*  str_nofile;

    string_resource_t* str_no_matching_item;

    bool select_mode;
    bool* select_flag;

    content_mode_e mode;

    char* default_path_name;

    dir_level_t* p_root;
    dir_level_t* p_cur_dir;

} content_list_t;

typedef struct filelist_private_data_s
{
    entry_list_t* entry;
    content_list_t* content;
} filelist_private_data_t;

/*!
 *  \brief
 *  filelist �ؼ�����ṹ����
 */
typedef struct filelist_s
{

    /*! ��ǰ��ʾ���б����� */
    filelist_mode_e list_inuse;

    /*! ��ǰ��ʾ��content�б������ */
    content_mode_e content_mode;

    /*! ��ǰ�Ƿ��ڶ��ļ�ѡ��ģʽ */
    bool select_mode;

    /*! ��ǰ�Ƿ����ļ����������� */
    bool in_file_op;

    /*! ������ͬ���ļ�/�ļ���ʱ�Ƿ񸲸� */
    bool replace_yes;

    /*! ����ID */
    int canvas_id;

    /*! �źŻص����� */
    void (*callback)(struct filelist_s*, filelist_sig_e);

    /*! ��ǰĿ¼ȫ·����*/
    const char* path_name;

    /*! ��ǰѡ���ļ���*/
    const char* file_name;

    /*! ��ǰʹ�õ������ַ��� */
    char* search_string;

    /*! ��ǰ�Ƿ�������Ŀ¼ */
    bool is_search_subdir;

    /*! �ڲ�˽�����ݽṹָ�룬���� filelist �ؼ��ڲ�ʵ��ʹ�� */
    struct filelist_private_data_s* data;

} filelist_t;

extern FSEL_HANDLE g_fsel_handle;

extern bool g_file_op_success;


/*!
 *  \brief
 *  filelist �ؼ����źŻص���������
 */
typedef void (*filelist_cb_t)(filelist_t* filelist, filelist_sig_e);

/* filelist �ؼ������ӿ� */

/******************************************************************************/
/*!
 * \par  Description:
 *	  filelist �ؼ���������.
 * \ingroup common_filelist
 * \param[in]    type  �б���ʾ����
 * \param[in]    callback   �źŻص�����
 * \return       ����filelist�ؼ����,����ʧ��ʱ����NULL.
 ******************************************************************************/
filelist_t* browser_widget_filelist_create(filelist_cb_t callback);

/******************************************************************************/
/*!
 * \par  Description:
 *     ɾ��һ��filelist�ؼ�
 * \ingroup common_filelist
 * \param [in] filelist: filelist�ؼ����
 * \return  �ɹ�����true, ʧ�ܷ���false
 *******************************************************************************/
bool browser_widget_filelist_delete(filelist_t* filelist);




bool browser_widget_filelist_enterbackgroud(filelist_t* filelist);

/*
bool browser_widget_filelist_enterforegroud(filelist_t* filelist);
*/

/******************************************************************************/
/*!
 * \par  Description:
 *     �˳���filelist�ؼ�
 * \ingroup common_filelist
 * \param [in] filelist: filelist�ؼ����
 * \return  �ɹ�����true, ʧ�ܷ���false
 *******************************************************************************/

bool browser_widget_filelist_exit(filelist_t* filelist);
/******************************************************************************/
/*!
 * \par  Description:
 *     filelist ��ͼ�ӿ�
 * \ingroup common_filelist
 * \param [in] filelist: filelist�ؼ����
 * \return  �ɹ�����true, ʧ�ܷ���false
 * \note �˺�������filelist�ؼ�������Ԫ�أ�һ��������Ҫ�������filelist�ؼ�ʱ
 *******************************************************************************/
void browser_widget_filelist_paint(filelist_t* filelist);

/******************************************************************************/
/*!
 * \par  Description:
 *     filelist�ؼ�GUI��Ϣ����������������Ҫ��GUI��Ϣ���ݸ��˺���
 * \ingroup common_filelist
 * \param [in] filelist: filelist�ؼ����
 * \param [in] pmsg: GUI��Ϣ
 * \return  �ɹ�����true, ʧ�ܷ���false
 * \note �ؼ��ڲ����¼�����ʱ,�ȵ��õ�ǰ�˵���Ļص�, �ٵ��ÿؼ����źŻص�����
 *******************************************************************************/
void browser_widget_filelist_proc_gui(filelist_t* filelist, gui_wm_message_t* pmsg);



void browser_widget_filelist_hide(filelist_t* filelist);


void browser_widget_filelist_show(filelist_t* filelist);

/******************************************************************************/
/*!
 * \par  Description:
 *     filelist�ؼ�ϵͳ��Ϣ����������������Ҫ��ϵͳ��Ϣ���ݸ��˺���
 * \ingroup common_filelist
 * \param [in] filelist: filelist�ؼ����
 * \param [in] pmsg: ϵͳ��Ϣ
 * \return  �ɹ�����true, ʧ�ܷ���false
 *******************************************************************************/
void browser_widget_filelist_proc_sys(filelist_t* filelist, msg_apps_t *pmsg );

/******************************************************************************/
/*!
 * \par  Description:
 *     filelist �ؼ�����style��Դ
 * \ingroup common_filelist
 * \param [in] filelist: filelist�ؼ����
 * \return  �ɹ�����true, ʧ�ܷ���false
 * \note �ⲿ���ý����ڼ�����Դ�ļ�ʱ
 *******************************************************************************/
bool browser_widget_filelist_load_resource(filelist_t* filelist);

/******************************************************************************/
/*!
 * \par  Description:
 *     filelist �ؼ�ˢ��style����
 * \ingroup common_filelist
 * \param [in] filelist: filelist�ؼ����
 * \return  �ɹ�����true, ʧ�ܷ���false
 * \note �ⲿ���ý����ڶ�̬�л���ʾ���ʱ
 *******************************************************************************/
bool browser_widget_filelist_load_style(filelist_t* filelist);

/******************************************************************************/
/*!
 * \par  Description:
 *     filelist �ؼ���ȡ��̬ͼƬ�Ŀ��
 * \ingroup common_filelist
 * \param [in] filelist: filelist�ؼ����
 * \return  �ɹ�����һ�� ��̬ͼƬ�Ŀ��, ʧ�ܷ��� -1
 * \note filelist �ж�̬ͼƬ���ⲿ�ṩ��������, ��music��ר��ͼƬ��videoԤ��ͼ,
 *       ����ǰ�Ȼ�ȡͼƬ�ߴ�
 *******************************************************************************/
int browser_widget_filelist_get_bitmap_width(filelist_t* filelist);

/******************************************************************************/
/*!
 * \par  Description:
 *     filelist �ؼ���ȡ��̬ͼƬ�ĸ߶�
 * \ingroup common_filelist
 * \param [in] filelist: filelist�ؼ����
 * \return  �ɹ�����һ�� ��̬ͼƬ�ĸ߶�, ʧ�ܷ��� -1
 * \note filelist �ж�̬ͼƬ���ⲿ�ṩ��������, ��music��ר��ͼƬ��videoԤ��ͼ,
 *       ����ǰ�Ȼ�ȡͼƬ�ߴ�
 *******************************************************************************/
int browser_widget_filelist_get_bitmap_height(filelist_t* filelist);

void browser_widget_filelist_toggle_select_mode(void);

void browser_widget_filelist_enter_move_mode(void);
void browser_widget_filelist_enter_copy_mode(void);

void browser_widget_filelist_exit_fileop_mode(void);
void browser_widget_filelist_exec_fileop(void);

void browser_widget_filelist_exec_delete_fileop(void);
void browser_widget_filelist_exit_delete_fileop_mode(void);

void browser_widget_filelist_enter_search_mode(char* search_string);

void browser_widget_filelist_show_no_matching_items(void);


#endif   /* ----- #ifndef __BROWSER_BROWSING_WIDGET_FILELIST__  ----- */
