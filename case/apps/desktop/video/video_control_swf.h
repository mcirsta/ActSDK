/********************************************************************************
 *                              USDK(1100)
 *                             Module: app 
 *                 Copyright(c) 2003-2009 Actions Semiconductor,
 *                            All Rights Reserved. 
 * History:         
 *      <author>    <time>           <version >             <desc>
 *      shaofeng    2009-09-02          v1.0               build this file 
 ********************************************************************************/
/*!
 * \file     video_control_swf.h
 * \brief    
 * \author   shaofeng
 *               
 * \version 1.0
 * \date  2009/09/02
*******************************************************************************/

#ifndef   __VIDEO_CONTROL_SWF_H__
#define   __VIDEO_CONTROL_SWF_H__

#include "include_case.h"
#include "include_psp.h"



/************************ extern function, º¯ÊýÉùÃ÷ ***************************/
extern bool swf_control(v_cmd_type_e v_cmd_type, void * param);
extern bool video_mmm_swf_init( void );
extern bool video_mmm_swf_deinit( void );

extern bool video_nplist_setplay_swf_by_index( int np_index );


#endif  /* __VIDEO_CONTROL_SWF_H__ */

