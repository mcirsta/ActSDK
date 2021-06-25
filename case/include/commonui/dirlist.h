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
 * \defgroup common_dirlist dirlist接口函数
 */
#ifndef __DIRLIST_H__
#define __DIRLIST_H__

#include "include_case.h"

/*!
 *  \par dirlist显示类型
 *  \li DIRLIST_MUSIC : 音乐目录列表
 *  \li DIRLIST_VIDEO : 视频目录列表
 *  \li DIRLIST_PHOTO : 图片目录列表
 *  \li DIRLIST_EBOOK : 电子书目录列表
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
 *  \par dirlist控件对外信号类型
 *  \li WIDGET_SIG_DIRLIST_OK: 选中文件
 *  \li WIDGET_SIG_DIRLIST_QUIT:    目录列表退出
 *  \li WIDGET_SIG_DIRLIST_DECODE:  解码当前多媒体文件图片信息
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
 *  dirlist 控件句柄结构定义
 */
typedef struct dirlist_s
{

    /*! 目录列表显示类型 */
    dirlist_type_e type;

    /*! 直接进入目录界面时的顶级目录 */
    char*   top_path_name;

    /*! 初始进入目录 界面时的初始目录*/
    char*   init_path_name;

    char*   filter_string;

    /*! 画布ID */
    int canvas_id;

    /*! 信号回调函数 */
    void (*callback)(struct dirlist_s*, dirlist_sig_e);

    /*! 当前目录全路径名*/
    const char* path_name;

    /*! 当前选中文件名*/
    const char* file_name;

    /*! 内部私有数据结构指针，仅供 dirlist 控件内部实现使用 */
    struct dirlist_private_data_s* data;
}dirlist_t;

/*!
 *  \brief
 *  dirlist 控件的信号回调函数类型
 */
typedef void (*dirlist_cb_t)(dirlist_t* dirlist, dirlist_sig_e);

/* dirlist 控件基本接口 */


/******************************************************************************/
/*!
 * \par  Description:
 *	  dirlist 控件创建函数.
 * \ingroup common_dirlist
 * \param[in]    type  列表显示类型
 * \param[in]    callback   信号回调函数
 * \return       返回dirlist控件句柄,创建失败时返回NULL.
 ******************************************************************************/
dirlist_t* dirlist_create(dirlist_init_t* init, dirlist_cb_t callback);

/******************************************************************************/
/*!
* \par  Description:
*     删除一个dirlist控件
* \ingroup common_dirlist
* \param [in] dirlist: dirlist控件句柄
* \return  成功返回true, 失败返回false
*******************************************************************************/
bool dirlist_delete(dirlist_t* dirlist);

/******************************************************************************/
/*!
* \par  Description:
*     dirlist 绘图接口
* \ingroup common_dirlist
* \param [in] dirlist: dirlist控件句柄
* \return  成功返回true, 失败返回false
* \note 此函数绘制dirlist控件的所有元素，一般用于需要整体更新dirlist控件时
*******************************************************************************/
bool dirlist_paint(dirlist_t* dirlist);

/******************************************************************************/
/*!
* \par  Description:
*     dirlist控件GUI消息处理函数，调用者需要将GUI消息传递给此函数
* \ingroup common_dirlist
* \param [in] dirlist: dirlist控件句柄
* \param [in] pmsg: GUI消息
* \return  成功返回true, 失败返回false
* \note 控件内部有事件发生时,先调用当前菜单项的回调, 再调用控件的信号回调函数
*******************************************************************************/
bool dirlist_proc_gui(dirlist_t* dirlist, gui_wm_message_t* pmsg);

/******************************************************************************/
/*!
 * \par  Description:
 *     dirlist控件系统消息处理函数，调用者需要将系统消息传递给此函数
 * \ingroup common_dirlist
 * \param [in] dirlist: dirlist控件句柄
 * \param [in] pmsg: 系统消息
 * \return  成功返回true, 失败返回false
 *******************************************************************************/
bool dirlist_proc_sys(dirlist_t* dirlist, msg_apps_t *pmsg );

/******************************************************************************/
/*!
* \par  Description:
*     dirlist 控件加载style资源
* \ingroup common_dirlist
* \param [in] dirlist: dirlist控件句柄
* \return  成功返回true, 失败返回false
* \note 外部调用仅用于动态切换显示风格时
*******************************************************************************/
bool dirlist_load_style(dirlist_t* dirlist);

/******************************************************************************/
/*!
* \par  Description:
*     显示dirlist控件
* \ingroup common_dirlist
* \param [in] dirlist: dirlist控件句柄
* \return  成功返回true, 失败返回false
*******************************************************************************/
bool dirlist_show( dirlist_t *dirlist );

/******************************************************************************/
/*!
* \par  Description:
*     隐藏dirlist控件
* \ingroup common_dirlist
* \param [in] dirlist: dirlist控件句柄
* \return  成功返回true, 失败返回false
*******************************************************************************/
bool dirlist_hide( dirlist_t *dirlist );
/******************************************************************************/
/*!
* \par  Description:
*     dirlist 控件获取动态图片的宽度
* \ingroup common_dirlist
* \param [in] dirlist: dirlist控件句柄
* \return  成功返回一个 动态图片的宽度, 失败返回 -1
* \note dirlist 中动态图片由外部提供解码数据, 如music的专辑图片或video预览图,
*       解码前先获取图片尺寸
*******************************************************************************/
int dirlist_get_bitmap_width(dirlist_t* dirlist);
/******************************************************************************/
/*!
* \par  Description:
*     dirlist 控件获取动态图片的高度
* \ingroup common_dirlist
* \param [in] dirlist: dirlist控件句柄
* \return  成功返回一个 动态图片的高度, 失败返回 -1
* \note dirlist 中动态图片由外部提供解码数据, 如music的专辑图片或video预览图,
*       解码前先获取图片尺寸
*******************************************************************************/
int dirlist_get_bitmap_height(dirlist_t* dirlist);

/******************************************************************************/
/*!
* \par  Description:
*     dirlist 控件获取指定index位置的menuitem的bitmap数据
* \ingroup common_dirlist
* \param [in] dirlist: dirlist控件句柄
* \param [in] index:   屏幕数据项的索引
* \return  成功返回一个指定index位置的menuitem的bitmap数据, 若不存在则返回NULL
* \note dirlist 中动态图片由外部提供解码数据，控件外部可通过调用此接口获取index位置的
*               menuitem的bitmap数据填充情况
*******************************************************************************/
gui_bitmap_info_t* dirlist_get_item_bitmap(dirlist_t* dirlist, int idx);

/******************************************************************************/
/*!
* \par  Description:
*     dirlist 控件设置指定index位置的menuitem的bitmap数据
* \ingroup common_dirlist
* \param [in] dirlist: dirlist控件句柄
* \param [in] index:   屏幕数据项的索引
* \param [in] bitmap:  欲设置的bitmap数据
* \return  成功设置返回true,失败返回false
* \note dirlist 中动态图片由外部提供解码数据，控件外部可通过调用此接口设置index位置的
*               menuitem的bitmap数据
*******************************************************************************/
bool dirlist_set_item_bitmap(dirlist_t* dirlist, int idx, gui_bitmap_info_t* bitmap);

/******************************************************************************/
/*!
* \par  Description:
*     dirlist 控件重绘指定index位置的menuitem的bitmap数据
* \ingroup common_dirlist
* \param [in] dirlist: dirlist控件句柄
* \param [in] index:   屏幕数据项的索引
* \return  成功设置返回true,失败返回false
* \note dirlist 中动态图片由外部提供解码数据，控件外部可通过调用此接口重绘指定index位置
*               的menuitem的bitmap数据
*******************************************************************************/
bool dirlist_paint_bitmap(dirlist_t* dirlist, int idx );

/******************************************************************************/
/*!
* \par  Description:
*     dirlist 控件设置搜索字符串
* \ingroup common_dirlist
* \param [in] dirlist: dirlist控件句柄
* \param [in] search_string: 搜索字符串
* \return  返回搜索结果数目
* \note dirlist 支持在当前目录下进行搜索操作。 dirlist会在当前目录下的子目录或文件名
*               中搜索search_string子字符串。符合此条件的条目会成为搜索结果。
*               如需重新获得未设搜索条件的条目集合，可重新设置search_string为""。
*               当 dirlist进入某子目录，或向上返回父目录时，搜索条件失效。
*                *  AP使用dirlist 的搜索功能需按照如下方式：
 *                  1.  隐藏dirlist, 显示softkeyboard
 *                  2. 获取sk返回的搜索字符串
 *                  3. 调用dirlist的搜索接口
 *                  4. 显示dirlist
*******************************************************************************/
int dirlist_set_search_string(dirlist_t* dirlist,char* search_string);

/******************************************************************************/
/*!
* \par  Description:
*     dirlist 控件设置搜索字符串
* \ingroup common_dirlist
* \param [in] dirlist: dirlist控件句柄
* \return  成功返回当前使用的文件选择器句柄,失败返回NULL
* \note dirlist 使用文件选择器实现文件目录的浏览功能，此接口返回当前使用的文件
*               选择器句柄。
*******************************************************************************/
FSEL_HANDLE dirlist_get_fsel_handle(dirlist_t* dirlist);

/******************************************************************************/
/*!
* \par  Description:
*     dirlist 获取菜单项的总数
* \ingroup common_dirlist
* \param [in] dirlist: dirlist控件句柄
* \return  成功返回 菜单项的总数, 失败返回 -1
* \note 此处返回的数目为dirlist当前填入到menuview控件中的菜单条目数量。
*******************************************************************************/
int dirlist_get_total( dirlist_t* dirlist );

/******************************************************************************/
/*!
* \par  Description:
*     dirlist 获取菜单项的显示起始行的索引
* \ingroup common_dirlist
* \param [in] dirlist: dirlist控件句柄
* \return  成功返回 显示起始行的索引。 失败返回 -1
* \note 此处返回的数目为dirlist当前填入到menuview控件中的显示起始行的索引。
*******************************************************************************/
int dirlist_get_offset( dirlist_t *dirlist );

/******************************************************************************/
/*!
* \par  Description:
*     dirlist 获取菜单项的当前高亮行的索引
* \ingroup common_dirlist
* \param [in] dirlist: dirlist控件句柄
* \return  成功返回 当前高亮行的索引。 失败返回 -1
* \note 此处返回的数目为dirlist当前填入到menuview控件中的高亮行的索引。
*******************************************************************************/
int dirlist_get_current( dirlist_t *dirlist );

/******************************************************************************/
/*!
* \par  Description:
*     dirlist 获取菜单的index条目的文件名
* \ingroup common_dirlist
* \param [in] dirlist: dirlist控件句柄
* \return  成功返回 菜单的index条目的文件名。 失败返回 NULL
* \note 此处返回的是dirlist的index条目的文件名，此处文件名不是全路径名。
*******************************************************************************/
char* dirlist_get_item_name(dirlist_t* dirlist, int idx);


/******************************************************************************/
/*!
* \par  Description:
*     dirlist 判断菜单的index条目的是否为文件夹
* \ingroup common_dirlist
* \param [in] dirlist: dirlist控件句柄
* \return  若index条目为文件夹返回true, 为文件返回false
* \note
*******************************************************************************/
bool dirlist_is_item_dir(dirlist_t* dirlist, int idx);
/******************************************************************************/
/*!
* \par  Description:
*     dirlist 获取当前目录下的子菜单和文件合计数
* \ingroup common_dirlist
* \param [in] dirlist: dirlist控件句柄
* \return  成功返回 当前目录下的子菜单和文件合计数, 失败返回 -1
* \note 此处返回的数目为当前目录下的子菜单和文件合计数。
*******************************************************************************/
int dirlist_get_global_size( dirlist_t *dirlist );

/******************************************************************************/
/*!
* \par  Description:
*     dirlist 获取当前显示菜单在全部当前目录下项目总数中的偏移
* \ingroup common_dirlist
* \param [in] dirlist: dirlist控件句柄
* \return  成功返回 当前菜单起始行在全体数据中的索引。, 失败返回 -1
* \note 此处返回的数目为当前显示菜单在全部当前目录下项目总数中的偏移。
*******************************************************************************/
int dirlist_get_global_offset( dirlist_t *dirlist );

#endif /* __DIRLIST_H__ */
