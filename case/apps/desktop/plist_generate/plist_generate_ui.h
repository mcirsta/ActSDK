/*******************************************************************************
 *                              USDK(1100)
 *                             Module: app 
 *                 Copyright(c) 2003-2009 Actions Semiconductor,
 *                            All Rights Reserved. 
 * History:         
 *      <author>    <time>           <version >             <desc>
 *      shaofeng    2009-05-25          v1.0               build this file 
*******************************************************************************/
/*!
 * \file     plist_generate_ui.h
 * \brief    
 * \author   shaofeng
 *               
 * \version 1.0
 * \date  2009/05/25
*******************************************************************************/

#ifndef __PLIST_GENERATE_UI_H__
#define __PLIST_GENERATE_UI_H__




/************************ extern function, º¯ÊýÉùÃ÷ ***************************/
extern void plist_generate_draw_bg(plist_generate_scene_t *plist_generate_scene, char show);
extern void plist_generate_draw_wait_animate(plist_generate_scene_t *plist_generate_scene, char show, char frame);
extern void plist_generate_draw_pbar_bg(plist_generate_scene_t *plist_generate_scene, char show);
extern bool plist_generate_progress_to_pbar_coordinate( int cur_time, int total_time, int *x_coordinate );
extern void plist_generate_draw_pbar(plist_generate_scene_t *plist_generate_scene, char show, int x);
extern void plist_generate_draw_str(plist_generate_scene_t *plist_generate_scene, char show);



#endif  /* __PLIST_GENERATE_UI_H__ */

