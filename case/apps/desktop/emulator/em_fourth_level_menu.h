/*
********************************************************************************
*                       项目名---emulator app
*                (c) Copyright 2003-2008, Actions Co,Ld. 
*                        All Right Reserved 
*
* FileName: em_fourth_level_menu.h     Author:  张三        Date:2008/04/27
* Description:   em_fourth_level_menu.c中的全局函数，应用中其他的C文件用到的 
* Others:         
* History:        
*      <author>    <time>       <version >    <desc>
*         张三      2008/04/27     1.0         build this file
********************************************************************************
*/ 

#ifndef      __EM_FOURTH_LEVEL_MENU_H__
#define      __EM_FOURTH_LEVEL_MENU_H__

#include "emulator_scene.h"

bool em_fourth_level_menu_enter(scene_emulator_t* my_scene);
bool set_keymap_proc_key(key_event_t * key_event);

#endif
