/********************************************************************************
 *                              USDK(1100)
 *                             Module: app 
 *                 Copyright(c) 2003-2009 Actions Semiconductor,
 *                            All Rights Reserved. 
 * History:         
 *      <author>    <time>           <version >             <desc>
 *      shaofeng    2009-05-15          v1.0               build this file 
********************************************************************************/
/*!
 * \file     video_commonui.h
 * \brief    
 * \author   shaofeng
 *               
 * \version 1.0
 * \date  2009/05/15
*******************************************************************************/

#ifndef __VIDEO_COMMONUI_H__
#define __VIDEO_COMMONUI_H__


#include "include_case.h"

/*本应用的头文件*/

//shaofeng
/*!
* scene type define
*/
typedef enum
{
    SCENE_ERR = 0,
    SCENE_EXIT,
    SCENE_RETURN,
    
    SCENE_LIST,
    SCENE_PLAYING,
	SCENE_SWF,
}scene_type_e;

/* scene of all apps */
typedef struct
{
    scene_type_e scene_type;
    
    /* style resource */
    scene_t* scene;
    
    /* window resource */
    int win_id;
    int handle_gui_dc;
}app_scene_t;


typedef enum
{
    VIDEO_DIALOG_NULL = 0,
    
    VIDEO_DIALOG_DELETE_TAG,
    VIDEO_DIALOG_NO_TAG,
    VIDEO_DIALOG_TAG_FULL,
    VIDEO_DIALOG_ADD_FAIL,
    
    VIDEO_DIALOG_DELETE_FILE,
    VIDEO_DIALOG_OPEN_FAIL,
    VIDEO_DIALOG_NOT_SUPPORT,
    VIDEO_DIALOG_DECODE_ERR,
    
    VIDEO_DIALOG_LOW_BAT,

    VIDEO_DIALOG_NODISK,
    
    VIDEO_DIALOG_NOT_FOUND,

    VIDEO_DIALOG_PL_NOSP,
}video_dialog_e;



/************************ extern function, 函数声明 ***************************/
extern bool video_headbar_create( void );
extern bool video_headbar_delete( void );

extern bool  video_msgbox_create( void );
extern bool video_msgbox_delete( void );

extern bool video_dialog_create( video_dialog_e type, dialog_cb_t dialog_cb );
extern bool video_dialog_delete( void );
extern int  video_dialog_modal( video_dialog_e type );

extern bool video_vbar_create( void );
extern bool video_vbar_delete( void );

extern bool video_adjust_scanspeed_create( void );
extern bool video_adjust_delete( void );
extern bool video_adjust_resume_create(void);

extern bool video_loadingbox_create( void );
extern bool video_loadingbox_delete( void );

extern bool video_softkeyboard_create( softkeyboard_cb_t callback );
extern bool video_softkeyboard_delete( void );

#endif  /* __VIDEO_COMMONUI_H__ */

