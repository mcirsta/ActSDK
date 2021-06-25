/********************************************************************************
 *                              USDK(1100)
 *                             Module: app 
 *                 Copyright(c) 2003-2009 Actions Semiconductor,
 *                            All Rights Reserved. 
 * History:         
 *      <author>    <time>           <version >             <desc>
 *      shaofeng    2009-07-28          v1.0               build this file 
 ********************************************************************************/
/*!
* \file     animation_decoder_scene.h
* \brief    
* \author   shaofeng
*               
* \version 1.0
* \date  2009/07/28
*******************************************************************************/

#ifndef __ANIMATION_DECODER_SCENE_H__
#define __ANIMATION_DECODER_SCENE_H__

#include "include_case.h"
#include "include_psp.h"



#include "animation_config.h"


typedef struct
{
    app_scene_t app_scene;
    
}decoder_scene_t;



/************************ extern function, º¯ÊýÉùÃ÷ ***************************/
extern bool animation_decoder_scene( decoder_scene_t *decoder_scene );


#endif  /* __ANIMATION_DECODER_SCENE_H__ */

