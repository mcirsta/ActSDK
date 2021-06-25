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


#ifndef EBOOK_SCENE_READ_OPTION_H
#define EBOOK_SCENE_READ_OPTION_H

#include "include_case.h"

#include "ebook_scene_read.h"

typedef struct
{
    void ( *const set_has_change_size_item )(void);

    void ( *const scene_read_option_set_scene )(scene_t* scene);
    menuview_t* (*const scene_read_option_enter )(void);
    bool ( *const scene_read_option_exit )(void);
}scene_read_option_t;

scene_read_option_t* create_scene_read_option(void);

#endif /* EBOOK_SCENE_READ_OPTION_H */

