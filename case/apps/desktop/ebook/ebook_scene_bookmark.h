/********************************************************************************
 *                              USDK(1100)
 *                             Module: app 
 *                 Copyright(c) 2003-2009 Actions Semiconductor,
 *                            All Rights Reserved. 
 * File : scene_listmenu.h
 * History:         
 *      <author>    <time>           <version >             <desc>
 *    zhaowenjie    2009-2-25          v1.0               build this file 
 ********************************************************************************/
/*!
* \file     scene_listmenu.h
* \brief    listmenu scene
* \author   zhaowenjie
*               
* \version 1.0
* \date  2009/02/25
*******************************************************************************/


#ifndef __EBOOK_SCENE_BOOKMARK_H__
#define __EBOOK_SCENE_BOOKMARK_H__

#include "include_case.h"

typedef enum
{
    BOOKMARK_FULL, /* 已经到达 32 个标签了 */
    ADD_BOOKMARK_FAIL, /* 磁盘不能读写，磁盘已满之类 */
    ADD_BOOKMARK_SUCCESS

}add_bookmark_result_e;

typedef struct
{
    bool (*const open_bookmark)(void);
    add_bookmark_result_e (*const add_a_bookmark)( char* path, char* name, unsigned int page, unsigned int point, unsigned int font );
    bool (*const quit_bookmark)(void);
    
    void (*const enter_scene_bookmark)(void);
}scene_bookmark_t;

scene_bookmark_t* create_scene_bookmark(void);

#endif /* __EBOOK_SCENE_BOOKMARK_H__ */
