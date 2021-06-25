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
 * \defgroup common_filelist filelist接口函数
 */

#ifndef  __BROWSER_BROWSING_WIDGET_FILELIST__
#define  __BROWSER_BROWSING_WIDGET_FILELIST__

#include "include_case.h"
#include "include_psp.h"
#include "file_operation.h"


/*!
 *  \par filelist当前显示的列表类型
 *  \li ENTRY : 当前显示列表为存储器选择列表
 *  \li CONTENT : 当前显示列表为文件系统目录列表
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
 *  \par filelist控件对外信号类型
 *  \li WIDGET_SIG_FILELIST_OK: 选中文件
 *  \li WIDGET_SIG_FILELIST_QUIT:    目录列表退出
 *  \li WIDGET_SIG_FILELIST_DECODE:  解码当前多媒体文件图片信息
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
    char* workdir; //当前工作目录

    int dirs; //当前目录下的子目录数
    int dir_startno; //列表显示中的子目录条目的开始号（针对文件选择器）
    int dir_num; //列表显示中的子目录条目数

    int files;
    int file_startno;
    int file_num;

    int item_group_size;
    menu_t* item_group; //用于显示的菜单数据集合


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
 *  filelist 控件句柄结构定义
 */
typedef struct filelist_s
{

    /*! 当前显示的列表类型 */
    filelist_mode_e list_inuse;

    /*! 当前显示的content列表的类型 */
    content_mode_e content_mode;

    /*! 当前是否处于多文件选择模式 */
    bool select_mode;

    /*! 当前是否处于文件操作过程中 */
    bool in_file_op;

    /*! 当遇到同名文件/文件夹时是否覆盖 */
    bool replace_yes;

    /*! 画布ID */
    int canvas_id;

    /*! 信号回调函数 */
    void (*callback)(struct filelist_s*, filelist_sig_e);

    /*! 当前目录全路径名*/
    const char* path_name;

    /*! 当前选中文件名*/
    const char* file_name;

    /*! 当前使用的搜索字符串 */
    char* search_string;

    /*! 当前是否搜索子目录 */
    bool is_search_subdir;

    /*! 内部私有数据结构指针，仅供 filelist 控件内部实现使用 */
    struct filelist_private_data_s* data;

} filelist_t;

extern FSEL_HANDLE g_fsel_handle;

extern bool g_file_op_success;


/*!
 *  \brief
 *  filelist 控件的信号回调函数类型
 */
typedef void (*filelist_cb_t)(filelist_t* filelist, filelist_sig_e);

/* filelist 控件基本接口 */

/******************************************************************************/
/*!
 * \par  Description:
 *	  filelist 控件创建函数.
 * \ingroup common_filelist
 * \param[in]    type  列表显示类型
 * \param[in]    callback   信号回调函数
 * \return       返回filelist控件句柄,创建失败时返回NULL.
 ******************************************************************************/
filelist_t* browser_widget_filelist_create(filelist_cb_t callback);

/******************************************************************************/
/*!
 * \par  Description:
 *     删除一个filelist控件
 * \ingroup common_filelist
 * \param [in] filelist: filelist控件句柄
 * \return  成功返回true, 失败返回false
 *******************************************************************************/
bool browser_widget_filelist_delete(filelist_t* filelist);




bool browser_widget_filelist_enterbackgroud(filelist_t* filelist);

/*
bool browser_widget_filelist_enterforegroud(filelist_t* filelist);
*/

/******************************************************************************/
/*!
 * \par  Description:
 *     退出此filelist控件
 * \ingroup common_filelist
 * \param [in] filelist: filelist控件句柄
 * \return  成功返回true, 失败返回false
 *******************************************************************************/

bool browser_widget_filelist_exit(filelist_t* filelist);
/******************************************************************************/
/*!
 * \par  Description:
 *     filelist 绘图接口
 * \ingroup common_filelist
 * \param [in] filelist: filelist控件句柄
 * \return  成功返回true, 失败返回false
 * \note 此函数绘制filelist控件的所有元素，一般用于需要整体更新filelist控件时
 *******************************************************************************/
void browser_widget_filelist_paint(filelist_t* filelist);

/******************************************************************************/
/*!
 * \par  Description:
 *     filelist控件GUI消息处理函数，调用者需要将GUI消息传递给此函数
 * \ingroup common_filelist
 * \param [in] filelist: filelist控件句柄
 * \param [in] pmsg: GUI消息
 * \return  成功返回true, 失败返回false
 * \note 控件内部有事件发生时,先调用当前菜单项的回调, 再调用控件的信号回调函数
 *******************************************************************************/
void browser_widget_filelist_proc_gui(filelist_t* filelist, gui_wm_message_t* pmsg);



void browser_widget_filelist_hide(filelist_t* filelist);


void browser_widget_filelist_show(filelist_t* filelist);

/******************************************************************************/
/*!
 * \par  Description:
 *     filelist控件系统消息处理函数，调用者需要将系统消息传递给此函数
 * \ingroup common_filelist
 * \param [in] filelist: filelist控件句柄
 * \param [in] pmsg: 系统消息
 * \return  成功返回true, 失败返回false
 *******************************************************************************/
void browser_widget_filelist_proc_sys(filelist_t* filelist, msg_apps_t *pmsg );

/******************************************************************************/
/*!
 * \par  Description:
 *     filelist 控件加载style资源
 * \ingroup common_filelist
 * \param [in] filelist: filelist控件句柄
 * \return  成功返回true, 失败返回false
 * \note 外部调用仅用于加载资源文件时
 *******************************************************************************/
bool browser_widget_filelist_load_resource(filelist_t* filelist);

/******************************************************************************/
/*!
 * \par  Description:
 *     filelist 控件刷新style加载
 * \ingroup common_filelist
 * \param [in] filelist: filelist控件句柄
 * \return  成功返回true, 失败返回false
 * \note 外部调用仅用于动态切换显示风格时
 *******************************************************************************/
bool browser_widget_filelist_load_style(filelist_t* filelist);

/******************************************************************************/
/*!
 * \par  Description:
 *     filelist 控件获取动态图片的宽度
 * \ingroup common_filelist
 * \param [in] filelist: filelist控件句柄
 * \return  成功返回一个 动态图片的宽度, 失败返回 -1
 * \note filelist 中动态图片由外部提供解码数据, 如music的专辑图片或video预览图,
 *       解码前先获取图片尺寸
 *******************************************************************************/
int browser_widget_filelist_get_bitmap_width(filelist_t* filelist);

/******************************************************************************/
/*!
 * \par  Description:
 *     filelist 控件获取动态图片的高度
 * \ingroup common_filelist
 * \param [in] filelist: filelist控件句柄
 * \return  成功返回一个 动态图片的高度, 失败返回 -1
 * \note filelist 中动态图片由外部提供解码数据, 如music的专辑图片或video预览图,
 *       解码前先获取图片尺寸
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
