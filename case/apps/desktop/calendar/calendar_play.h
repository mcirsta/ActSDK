/*
 ********************************************************************************
 *                         uSDK130
 *
 *
 *
 *                     (c) Copyright 2002-2003, Actions Co,Ld.
 *                                       All Right Reserved
 *
 * File   :  calendar_play.h
 * By     :  ywwang
 * Purpose:
 * Version:       first version     2006-11-20
 ********************************************************************************
 ********************************************************************************
 */

#ifndef CALENDAR_H
#define  CALENDAR_H

#include "calendar_scene.h"

#define MAX_YEAR                    2099         /*万年历中最大，最小可转换年*/
#define MIN_YEAR                    1901

#define TOTALCOLS   7
#define TOTALROWS   6

typedef struct
{
    int year;
    int month;
    int day;
} DateVarType;

int getLunarString(char *str, const DateVarType *scolar, DateVarType *lunar, int languageId);
/*根据语言环境,返回某年,某月某日的节日*/
int getHolidayStr(char *str, const DateVarType *scolar, DateVarType *lunar, int lanid);


void calendar_load(scene_calendar_t *my_scene);
int get_language_mode(void);
void paintCalendarTable(scene_calendar_t *my_scene);
bool get_date_panes_coordinate(void);
void calendar_key_mamage(scene_calendar_t *my_scene, key_event_t *kmsg);

#endif
