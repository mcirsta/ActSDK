/*
********************************************************************************
*                       项目名---emulator app
*                (c) Copyright 2003-2008, Actions Co,Ld. 
*                        All Right Reserved 
*
* FileName: em_first_level_menu.h     Author:  张三        Date:2008/04/27
* Description:   em_first_level_menu.c中的全局函数，应用中其他的C文件用到的 
* Others:         
* History:        
*      <author>    <time>       <version >    <desc>
*         张三      2008/04/27     1.0         build this file
********************************************************************************
*/ 

#ifndef      __EM_FIRST_LEVEL_MENU_H__
#define      __EM_FIRST_LEVEL_MENU_H__

#include "emulator_scene.h"

bool em_dirlist_enter(scene_emulator_t* my_scene, int flag);
bool em_dirlist_exit(dirlist_t *dirlist);

#endif
