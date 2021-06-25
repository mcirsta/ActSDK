/*
 ********************************************************************************
 *                         uSDK130
 *
 *
 *
 *                     (c) Copyright 2002-2003, Actions Co,Ld.
 *                                       All Right Reserved
 *
 * File   :  date.h
 * By     :  ywwang
 * Purpose:   提供日期转化接口
 *
 * Version:       first version     2006-11-20
 ********************************************************************************
 ********************************************************************************
 */

#ifndef DATETIME_H
#define DATETIME_H

void setCurYMD(int curYear, int curMonth, int curDay, unsigned int *jd);

int year(unsigned int *jd);
int month(unsigned int *jd);
int day(unsigned int *jd);
int dayOfWeek(unsigned int *jd);
int dayOfYear(unsigned int *jd);
int daysInMonth(unsigned int *jd);
int daysInYear(unsigned int *jd);
int weekNumber(int *yearNum, unsigned int *jd);
int setYMD(int y, int m, int d, unsigned int *jd);
void addDays(int days, unsigned int *jd);
void addMonths(int months, unsigned int *jd);
void addYears(int years, unsigned int *jd);

#endif
