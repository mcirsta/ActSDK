/*
 ********************************************************************************
 *                         uSDK130
 *
 *
 *
 *                     (c) Copyright 2002-2003, Actions Co,Ld.
 *                                       All Right Reserved
 *
 * File   :  Date.c
 * By     :  ywwang
 * Purpose:   完成年月日到date的转换
 *
 * Version:       first version     2006-11-20
 ********************************************************************************
 ********************************************************************************
 */

#include "date.h"
#include <include_case.h>

//#define false 0
//#define true 1

static const unsigned int FIRST_DAY = 2361222; // Julian day for 1752-09-14
static const int FIRST_YEAR = 1752;

const short monthDays[] =
{ 0, 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31 };

static int isLeapYear(int year);
static int isValid2(int y, int m, int d);
static unsigned int gregorianToJulian(int y, int m, int d);
static void julianToGregorian(unsigned int jd, int *y, int *m, int *d);

static int isValid(unsigned int *jd)
{
    return *jd >= FIRST_DAY;
}

/**
 *      设置当前年月
 *
 */

void setCurYMD(int curYear, int curMonth, int curDay, unsigned int *jd)
{

    *jd = 0;
    setYMD(curYear, curMonth, curDay, jd);

}

int year(unsigned int *jd)
{
    int y, m, d;
    julianToGregorian(*jd, &y, &m, &d);
    return y;
}

int month(unsigned int *jd)
{
    int y, m, d;
    julianToGregorian(*jd, &y, &m, &d);
    return m;
}

int day(unsigned int *jd)
{
    int y, m, d;
    julianToGregorian(*jd, &y, &m, &d);
    return d;
}

int dayOfWeek(unsigned int *jd)
{
    return (*jd % 7) + 1;
}

int dayOfYear(unsigned int *jd)
{
    return *jd - gregorianToJulian(year(jd), 1, 1) + 1;
}

int daysInMonth(unsigned int *jd)
{
    int y, m, d;
    julianToGregorian(*jd, &y, &m, &d);
    //printf(" y= %d  m %d  d=%d \n", y,m,d);

    if ((m == 2) && (isLeapYear(y) == 1)) //QAC
    {
        return 29;
    }
    else
    {
        return monthDays[m];
    }
}

int daysInYear(unsigned int *jd)
{
    int y, m, d;
    julianToGregorian(*jd, &y, &m, &d);
    //return isLeapYear(y) ? 366 : 365;  //QAC
    if (isLeapYear(y) == 1)
    {
        return 366;	
    }
    else
    {
        return 365;	
    }
}

int weekNumber(int *yearNumber, unsigned int *jd)
{
    int varyear = year(jd);
    int yday = dayOfYear(jd) - 1;
    int wday = dayOfWeek(jd);
    int w;

    if (isValid(jd) == 0)
    {
        return 0;
    }

    if (wday == 7)
    {
        wday = 0;
    }

    for (;;)
    {
        int len;
        int bot;
        int top;

        //len = isLeapYear(varyear) ? 366 : 365; //QAC 不通过
        if (isLeapYear(varyear) == 1)
        {
            len = 366;
        }
        else
        {
            len = 365;
        }
        
        /*
         ** What yday (-3 ... 3) does
         ** the ISO year begin on?
         */
        bot = (((yday + 11) - wday) % 7) - 3;
        /*
         ** What yday does the NEXT
         ** ISO year begin on?
         */
        top = bot - (len % 7);
        if (top < -3)
        {
            top += 7;
        }
        top += len;
        if (yday >= top)
        {
            ++varyear;
            w = 1;
            break;
        }
        if (yday >= bot)
        {
            w = 1 + ((yday - bot) / 7);
            break;
        }
        --varyear;
        //yday += isLeapYear(varyear) ? 366 : 365; //QAC 不通过
        if (isLeapYear(varyear) == 1)
        {
            yday += 366; 	
        }
        else
        {
            yday += 365; 	
        }
    }
    
    if (yearNumber != 0)
    {
        *yearNumber = varyear;
    }
    
    return w;
}

int setYMD(int y, int m, int d, unsigned int *jd)
{
    if ((year(jd) == y) && (month(jd) == m) && (day(jd) == d))
    {
        return isValid(jd);
    }
    
    if (!isValid2(y, m, d))
    {
        jd = 0;
        return false;
    }
    
    *jd = gregorianToJulian(y, m, d);
    return true;
}

void addDays(int ndays, unsigned int *jd)
{
    *jd = *jd + (unsigned int)ndays;
}

void addMonths(int nmonths, unsigned int *jd)
{
    int y, m, d;
    julianToGregorian(*jd, &y, &m, &d);

    while (nmonths != 0)
    {
        if ((nmonths < 0) && ((nmonths + 12) <= 0))
        {
            y--;
            nmonths += 12;
        }
        
        if (nmonths < 0)
        {
            m += nmonths;
            nmonths = 0;
            if (m <= 0)
            {
                --y;
                m += 12;
            }
        }
        else if ((nmonths - 12) >= 0)
        {
            y++;
            nmonths -= 12;
        }
        else if (m == 12)
        {
            y++;
            m = 0;
        }
        else
        {
            m += nmonths;
            nmonths = 0;
            if (m > 12)
            {
                ++y;
                m -= 12;
            }
        }
    }

    /*  这个地方要注意，还未处理
     QDate tmp(y, m, 1);
     if (d > tmp.daysInMonth())
     d = tmp.daysInMonth();
     */
    *jd = 0;
    setYMD(y, m, 1, jd);
    if (d > daysInMonth(jd))
    {
        d = daysInMonth(jd);
    }
    *jd = 0;
    setYMD(y, m, d, jd);
}

void addYears(int nyears, unsigned int *jd)
{
    int y, m, d;
    
    julianToGregorian(*jd, &y, &m, &d);
    y += nyears;

    /* */
    //QDate tmp(y,m,1);
    *jd = 0;
    setYMD(y, m, 1, jd);

    if (d > daysInMonth(jd))
    {
        d = daysInMonth(jd);
    }

    *jd = 0;
    setYMD(y, m, d, jd);
}

int isValid2(int y, int m, int d)
{
    int result = 0;
    int result2 = 0; 
       
    if ((y >= 0) && (y <= 99))
    {
        y += 1900;
    }
    else if ((y < FIRST_YEAR) || ((y == FIRST_YEAR) && ((m < 9) || ((m == 9) && (d < 14)))))
    {
        return false;
    }
    else
    {
        ; // qac	
    }
    //return (((d > 0) && (m > 0)) && (m <= 12)) && ((d <= monthDays[m]) || (((d == 29) && (m == 2)) && isLeapYear(y)));
    //return (d > 0 && m > 0 && m <= 12) && (d <= monthDays[m] || (d == 29 && m == 2 && isLeapYear(y)));
    result = ((d > 0) && (m > 0)) && (m <= 12);
    result2 = ((d == 29) && (m == 2)) && (isLeapYear(y) == 1);
    result2 = result2 || (d <= monthDays[m]);
    result = result && result2;
    
    return result;
}

static int isLeapYear(int y)
{
    return (((y % 4) == 0) && ((y % 100) != 0)) || ((y % 400) == 0);
}

static unsigned int gregorianToJulian(int y, int m, int d)
{
    unsigned int c, ya;
    
    if (y <= 99)
    {
        y += 1900;
    }
    if (m > 2)
    {
        m -= 3;
    }
    else
    {
        m += 9;
        y--;
    }
    c = (unsigned int)y;
    c /= 100;
    ya = (unsigned int)y - (100 * c);
    return ((1721119 + (unsigned int)d) + ((146097 * c) / 4 + (1461 * ya) / 4)) + (((153 * (unsigned int)m) + 2) / 5);
}

static void julianToGregorian(unsigned int jd, int *y, int *m, int *d)
{
    unsigned int x;
    unsigned int j = jd - 1721119;
    
    *y = ((int)(j * 4) - 1) / 146097;
    j = ((j * 4) - (unsigned int)(146097 * (*y))) - 1;
    x = j / 4;
    j = ((x * 4) + 3) / 1461;
    *y = (100 * (*y)) + (int)j;
    x = ((x * 4) + 3) - (1461 * j);
    x = (x + 4) / 4;
    *m = ((int)(5 * x) - 3) / 153;
    x = ((5 * x) - 3) - (unsigned int)(153 * (*m));
    *d = ((int)(x + 5)) / 5;
    if (*m < 10)
    {
        *m += 3;
    }
    else
    {
        *m -= 9;
        y++;
    }
}
