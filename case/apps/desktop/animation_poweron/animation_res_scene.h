/********************************************************************************
 *                              USDK(1100)
 *                             Module: app 
 *                 Copyright(c) 2003-2009 Actions Semiconductor,
 *                            All Rights Reserved. 
 * History:         
 *      <author>    <time>           <version >             <desc>
 *      shaofeng    2009-05-09          v1.0               build this file 
*******************************************************************************/
/*!
 * \file     animation_res_scene.h
 * \brief    
 * \author   shaofeng
 *               
 * \version 1.0
 * \date  2009/05/09
*******************************************************************************/

#ifndef __ANIMATION_RES_SCENE_H__
#define __ANIMATION_RES_SCENE_H__


#include "animation_config.h"



typedef struct
{
    app_scene_t app_scene;
    
    picregion_resource_t *reg;
}res_scene_t;



/************************ extern function, º¯ÊýÉùÃ÷ ***************************/
extern bool res_load_style( res_scene_t *scene );
extern void animation_res_scene( res_scene_t *scene );



#endif  /* __ANIMATION_RES_SCENE_H__ */

