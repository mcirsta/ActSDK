#ifndef      __CALENDAR_UI_H_
#define      __CALENDAR_UI_H_

#include "calendar_scene.h"



void drawDateBar(scene_calendar_t *my_scene);
void drawInfoBar(scene_calendar_t *my_scene);
void drawStrAnyWhere(scene_calendar_t *my_scene, char *str, gui_rect_t *rect, int size, int color, int align);
void drawYellowNumPic(scene_calendar_t *my_scene, int number, int x, int y);
void drawWhiteNumPic(scene_calendar_t *my_scene, int number, int x, int y);
void wipe_pic(scene_calendar_t *my_scene, int x, int y, int pic_id, sty_rect_t *rect);
void rePaintWindowArea(scene_calendar_t *my_scene);
void drawCurDay(scene_calendar_t *my_scene);

#endif

