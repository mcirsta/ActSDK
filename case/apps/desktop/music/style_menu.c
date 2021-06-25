/********************************************************************************
 *                              USDK(1100)
 *                             Module: app 
 *                 Copyright(c) 2003-2009 Actions Semiconductor,
 *                            All Rights Reserved. 
 * File : style_menu.c
 * History:         
 *      <author>    <time>           <version >             <desc>
 *    songzhining    2009-2-25          v1.0               build this file 
 ********************************************************************************/
/*!
* \file     style_menu.c
* \brief    menulist widget
* \author   songzhining
*               
* \version 1.0
* \date  2009/02/25
*******************************************************************************/

#include "style_menu.h"


bool menu_load_style( int resgroup_id, style_menuitem_t *items, int total )
{
    scene_t* scene;    //!< commonui scene
    resgroup_resource_t *group;
    string_resource_t *string;
    picture_resource_t *picture;
    int num;
    
    //get root resource
    scene = sty_get_app_scene();
    if ( scene == NULL )
    {
        print_err("cann't find app scene!");
        return false;
    }

    //load resource group
    group = ( resgroup_resource_t * )get_scene_child( scene, (unsigned int)resgroup_id );
    
    //load resource
    for ( num=0; num < total; num++ )
    {
        //get string
        string = ( string_resource_t * )get_resgroup_child( group, (unsigned int)items[num].sty_str ); 
        if ( string != NULL )
        {
            items[num].str_id = (int)string->str_id;
        }
        else
        {
            items[num].str_id = 0;
        }

        //for test
        //printf("str_id = %d\n", string->str_id);
        
        //get sel icon
        picture = ( picture_resource_t * )get_resgroup_child( group, (unsigned int)items[num].sty_pic_sel ); 
        if ( picture != NULL )
        {
            items[num].pic_sel_id = (int)picture->pic_id;
        }
        else
        {
            items[num].pic_sel_id = 0;
        }
        
        //get unsel icon
        picture = ( picture_resource_t * )get_resgroup_child( group, (unsigned int)items[num].sty_pic_unsel ); 
        if ( picture != NULL )
        {
            items[num].pic_unsel_id = (int)picture->pic_id;
        }
        else
        {
            items[num].pic_unsel_id = 0;
        }
    }
    
    return true;
}

