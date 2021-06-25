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


#ifndef __EBOOK_SCENE_DIRLIST_H__
#define __EBOOK_SCENE_DIRLIST_H__

#include "include_case.h"


typedef struct
{    
    void (*const enter_scene_dirlist)(void);

    void (*const remove_dirlist)( void );

    FSEL_HANDLE (*const get_fsel_handle)( void );
}scene_dirlist_t;

scene_dirlist_t* create_scene_dirlist(void);

#endif /* __EBOOK_SCENE_DIRLIST_H__ */
