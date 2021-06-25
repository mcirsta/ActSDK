/********************************************************************************
 *                              USDK(1100)
 *                             Module: app 
 *                 Copyright(c) 2003-2008 Actions Semiconductor,
 *                            All Rights Reserved. 
 * file : sys_resource.h
 * History:         
 *      <author>    <time>           <version >             <desc>
 *      Herb Hu    2009-5-11             v1.0             build this file 
 ********************************************************************************/

#ifndef __SYS_RESOURCE_H__
#define __SYS_RESOURCE_H__

#include "sys_info.h"

/*! 
 * \defgroup   applib_system_resouce   系统资源访问接口 
 */

/*!
 * \par  主盘应用目录类型 
 */
typedef enum 
{
    DIR_MUSIC = 0,              
    DIR_VIDEO,          
    DIR_PHOTO,          
    DIR_EBOOK,           
    DIR_VIDEO_CAMERA,   
    DIR_PHOTO_CAMERA,    
    DIR_GAME,            
    DIR_RECORDER,        
    DIR_RADIO_RECORD,    
    DIR_RECORDER_MIC,    
    DIR_RECORDER_LINEIN, 
    DIR_TVIN,            
    DIR_SYSTEM,          
    DIR_SYSTEM_FM,       
    DIR_SYSTEM_FONT,     
    DIR_SYSTEM_PLAYLIST, 
    DIR_ROOT,              
    DIR_ALL,                 
}dir_type_e;


typedef struct  support_language_list_s
{  
    /*! 依据ISO 936和ISO 3166 标准的语言和地区名称编码，例如zh_CN, en_GB */
    char * codes;   

    /*! 语言名称，例如 "简体中文", "English(America)" */
    char * name;

    /*! 语言 id */ 
    int id;

    struct support_language_list_s * next;
}support_language_list_t;


/******************************************************************************/
/*!                    
 * \par  Description:
 *     获取应用当前主题资源文件的存放目录 
 * \ingroup applib_system_resouce
 * \return 应用当前主题资源文件存放目录     
 *******************************************************************************/
extern const char* sys_get_app_theme_res_dir(void);

/******************************************************************************/
/*!                    
 * \par  Description:
 *     获取commonui当前主题资源文件的存放目录 
 * \ingroup applib_system_resouce
 * \return commonui当前主题资源文件存放目录     
 *******************************************************************************/
extern const char* sys_get_common_theme_res_dir(void);

/******************************************************************************/
/*!                    
 * \par  Description:
 *     获取当前主题名称 
 * \ingroup applib_system_resouce
 * \return 当前主题名称     
 *******************************************************************************/
extern const char* sys_get_cur_theme_name(void);

/******************************************************************************/
/*!                    
 * \par  Description:
 *     获取应用自身多国语言字符串资源文件的存放目录 
 * \ingroup applib_system_resouce
 * \return  应用自身多国语言字符串资源文件的存放目录    
 *******************************************************************************/
extern const char* sys_get_app_string_res_dir(void);

/******************************************************************************/
/*!                    
 * \par  Description:
 *     获取commonui的多国语言字符串资源文件的存放目录 
 * \ingroup applib_system_resouce
 * \return  commonui的多国语言字符串资源文件的存放目录    
 *******************************************************************************/
extern const char* sys_get_common_string_res_dir(void);

/******************************************************************************/
/*!                    
 * \par  Description:
 *     获取当前语言的编码名称，例如：zh_CN, en_GB等 
 * \ingroup applib_system_resouce
 * \return  当前语言的编码名称    
 *******************************************************************************/
extern const char* sys_get_cur_language_codes(void);

/******************************************************************************/
/*!                    
 * \par  Description:
 *     获取当前语言id 
 * \ingroup applib_system_resouce
 * \return  当前语言id    
 *******************************************************************************/
extern int sys_get_cur_language_id(void);

/******************************************************************************/
/*!                    
 * \par  Description:
 *     根据语言id查找对应的语言编码名称
 * \ingroup applib_system_resouce
 * \param[in] lang_id -- 指定语言id
 * \return  成功返回给定语言id的编码名称，否则返回NULL    
 *******************************************************************************/
extern const char* sys_get_language_codes(int lang_id);

/******************************************************************************/
/*!                    
 * \par  Description:
 *     根据语言编码名称查找对于的语言id
 * \ingroup applib_system_resouce
 * \param[in] lang_codes -- 指定语言编码名称
 * \return  成功返回给定语言编码名称的语言id，否则返回-1    
 *******************************************************************************/
extern int sys_get_language_id(const char* lang_codes);

/******************************************************************************/
/*!                    
 * \par  Description:
 *     获取当前支持语言的列表 
 * \ingroup applib_system_resouce
 * \return  成功返回当前支持语言的列表头指针，否则返回NULL    
 *******************************************************************************/
extern support_language_list_t* sys_get_cur_support_language_list(void);

/******************************************************************************/
/*!                    
 * \par  Description:
 *     删除当前支持的语言列表 
 * \ingroup applib_system_resouce
 * \param[in] list -- 当前支持语言列表 
 * \return void 
 *******************************************************************************/
extern void sys_delete_cur_support_language_list(support_language_list_t * list);

/******************************************************************************/
/*!                    
 * \par  Description:
 *     获取当前默认字体文件名 
 * \ingroup applib_system_resouce
 * \return  当前默认字体文件名    
 *******************************************************************************/
extern const char* sys_get_default_font_file(void);

/******************************************************************************/
/*!
 * \par  Description:
 * 获取媒体资源目录 
 * \param[in] dev --  存储设备，
 * \param[in] type -- 媒体资源目录类型，
 * \result 指定类型的媒体资源目录 
 *******************************************************************************/
extern const char * sys_get_media_dir(hotplugin_dev_type_e dev, dir_type_e type);

/******************************************************************************/
/*!
 * \par  Description:
 * 获取主盘上的应用目录 
 * \param[in] type -- 应用目录类型，
 * \result 目录存在，返回该路径，否则返回NULL
 *******************************************************************************/
extern const char * sys_get_main_disk_dir(dir_type_e type);

/******************************************************************************/
/*!
 * \par  Description:
 * 检查磁盘上的多媒体资源文件目录是否存在
 * \param[in] path_sequence -- 指定目录全路径(可以是多个全路径的序列，用';'隔开)，
 *             可使用app_define.h中的宏，
 * \param[in] root_dir -- 指定开始检查的根路径
 * \result 如果检查的目录不存在，将自动创建之，如果创建失败，返回false, 否则返回true
 *******************************************************************************/
extern bool sys_check_media_dir(const char * path_sequence, const char * root_dir);

#endif /* #ifndef __SYS_RESOURCE_H__ */

