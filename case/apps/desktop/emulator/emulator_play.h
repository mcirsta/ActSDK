/*
********************************************************************************
*                       ��Ŀ��---emulator app
*                (c) Copyright 2003-2008, Actions Co,Ld. 
*                        All Right Reserved 
*
* FileName: emulator_play.h     Author:  ����        Date:2008/04/27
* Description:   emulator_play.c�е�ȫ�ֺ�����Ӧ����������C�ļ��õ��� 
* Others:         
* History:        
*      <author>    <time>       <version >    <desc>
*         ����      2008/04/27     1.0         build this file
********************************************************************************
*/ 

#ifndef      __EMULATOR_PLAY_H__
#define      __EMULATOR_PLAY_H__

#include "emulator_scene.h"
#include "emulator_play.h"


#define OMIT_SFC_MD  0


//void deal_key_when_playing(key_event_t *key_event);
bool open_emulator(void);
int engine_cmd(emu_ctrl_t *pmsg);
int emu_play(const char *romname);
int emu_reset(void);
int emu_continue(void);
int emu_pause(void);
int emu_load(void);
int emu_save(void);
int emu_exit(void);
int emu_set_sound(int value);
int emu_set_disp_mode(int value);
void return_to_ui_wiodows(void);
void return_to_dirlist(void);
void return_to_game_wiodows(void);
void emu_init(void);
int emu_set_keymap(void);
void msg_key_server_enable(int enable);

#endif
