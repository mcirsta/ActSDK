/********************************************************************************
 *                              USDK(1100)
 *                             Module: app
 *                 Copyright(c) 2003-2009 Actions Semiconductor,
 *                            All Rights Reserved.
 * File : app_desktop_info.h
 * History:
 *      <author>    <time>           <version >             <desc>
 *       herb hu    2009-0-23          v1.0               build this file
 ********************************************************************************/
/*!
 * \file     app_desktop_info.h
 * \brief    application desktop information manager
 * \author   herb hu
 *
 * \version 1.0
 * \date  2009/04/23
 *******************************************************************************/

#ifndef __DESKTOP_APPINFO_H__
#define __DESKTOP_APPINFO_H__
#include <include_case.h>

#define SKIP_SPACE(str) \
while(((*(str) == ' ') || (*(str) == '\t')) && (*(str) != '\0')) \
{\
    (str)++;\
}

#define SKIP_BACK_SPACE(begin, str) \
while(((*(str) == ' ') || (*(str) == '\t')) && ((str) > (begin))) \
{\
    (str)--;\
}

//desktop type
#define DESKTOP_TYPE_APP "application"  //application
#define DESKTOP_TYPE_DIR "directory"    //directory
typedef enum
{
    DKEY_COMMENT = 0x00,
    DKEY_PATH,
    DKEY_TYPE,
    DKEY_CHILD,
    DKEY_EXEC,
    DKEY_NAME,
    DKEY_NAME_LANG,
    DKEY_ICON,
    DKEY_ICON_ID,
    DKEY_COUNT
} desktop_key_e;

/* by ccm, the structure will save all info. from the .desktop file */
// Applications info. list
typedef struct appinfo_list
{
    char * filename;
    int app_index;
    char * value[DKEY_COUNT];

    struct appinfo_list* prev;
    struct appinfo_list* next;
    struct appinfo_list* parent;
    struct appinfo_list* child;
} appinfo_list_t;

/* application desktop information structure,
 * it can be built from application desktop file.
 * */
//typedef struct desktop_info_s
//{
//    /* comment information */
//    char * comment;
//
//    /* application path name */
//    char * path;
//
//    /* desktop type */
//    desktop_type_e type;
//
//    /* icon id */
//    int icon_id;
//
//    /* application name for show on main menu */
//    char * name; // for test
//    // int name_id;
//
//    /* application executable file name for launch */
//    char * exec;
//
//    /* application launch arg */
//    char * arg;
//
//    /* other information */
//
//    /* link to prev item */
//    struct desktop_info_s * prev;
//
//    /* link to next item */
//    struct desktop_info_s * next;
//
//    /* link to parent item */
//    struct desktop_info_s * parent;
//
//    /* link to child item */
//    struct desktop_info_s * child;
//
//} appinfo_list_t;
//
///* just for example */
//
///* find all valid application desktop files, and build a whole linked list structure */
//appinfo_list_t * desktop_info_list_create(void);
//
///* destory a desktop list structure */
//bool desktop_info_list_delete(appinfo_list_t * plist);
//
///* parse a desktop file, and fill values to the given appinfo_list_t structure */
//bool parse_desktop_file(const char * file_name, appinfo_list_t * desktop_info);
//
extern appinfo_list_t * appinfo_list_create(int appid);
extern bool appinfo_list_delete(appinfo_list_t * appinfo_list);
extern bool appinfo_list_update_name(appinfo_list_t * appinfo_list);

#endif /*  #ifndef __APP_INFO_H__ */
