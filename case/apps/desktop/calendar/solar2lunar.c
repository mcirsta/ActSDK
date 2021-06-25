/*
 ********************************************************************************
 *                         uSDK130
 *
 *
 *
 *                     (c) Copyright 2002-2003, Actions Co,Ld.
 *                                       All Right Reserved
 *
 * File   :  Solar2Lunar.c
 * By     :  ywwang
 * Purpose:   完成公历纪年到农历年，节气的转换
 *
 * Version:       first version     2006-11-20
 ********************************************************************************
 ********************************************************************************
 */

/*本文件向外部提供的唯一接口*/
/**
 *  str:  用户已分配空间的字符串指针，用于返回农历字符串
 *  year, month, day: 公历的年/月/日
 *
 *
 */

#include "calendar_play.h"

static unsigned char daysInGregorianMonth[] = { 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31 };


/*
** 农历月份大小压缩表，两个字节表示一年。两个字节共十六个二进制位数，
** 前四个位数表示闰月月份，后十二个位数表示十二个农历月份的大小。
*/
static unsigned char chineseMonths[] =
{ 
    0x00, 0x04, 0xad, 0x08, 0x5a, 0x01, 0xd5, 0x54, 0xb4, 0x09, 0x64, 0x05, 0x59, 0x45, 0x95, 0x0a, 0xa6, 0x04,
    0x55, 0x24, 0xad, 0x08, 0x5a, 0x62, 0xda, 0x04, 0xb4, 0x05, 0xb4, 0x55, 0x52, 0x0d, 0x94, 0x0a, 0x4a, 0x2a,
    0x56, 0x02, 0x6d, 0x71, 0x6d, 0x01, 0xda, 0x02, 0xd2, 0x52, 0xa9, 0x05, 0x49, 0x0d, 0x2a, 0x45, 0x2b, 0x09,
    0x56, 0x01, 0xb5, 0x20, 0x6d, 0x01, 0x59, 0x69, 0xd4, 0x0a, 0xa8, 0x05, 0xa9, 0x56, 0xa5, 0x04, 0x2b, 0x09,
    0x9e, 0x38, 0xb6, 0x08, 0xec, 0x74, 0x6c, 0x05, 0xd4, 0x0a, 0xe4, 0x6a, 0x52, 0x05, 0x95, 0x0a, 0x5a, 0x42,
    0x5b, 0x04, 0xb6, 0x04, 0xb4, 0x22, 0x6a, 0x05, 0x52, 0x75, 0xc9, 0x0a, 0x52, 0x05, 0x35, 0x55, 0x4d, 0x0a,
    0x5a, 0x02, 0x5d, 0x31, 0xb5, 0x02, 0x6a, 0x8a, 0x68, 0x05, 0xa9, 0x0a, 0x8a, 0x6a, 0x2a, 0x05, 0x2d, 0x09,
    0xaa, 0x48, 0x5a, 0x01, 0xb5, 0x09, 0xb0, 0x39, 0x64, 0x05, 0x25, 0x75, 0x95, 0x0a, 0x96, 0x04, 0x4d, 0x54,
    0xad, 0x04, 0xda, 0x04, 0xd4, 0x44, 0xb4, 0x05, 0x54, 0x85, 0x52, 0x0d, 0x92, 0x0a, 0x56, 0x6a, 0x56, 0x02,
    0x6d, 0x02, 0x6a, 0x41, 0xda, 0x02, 0xb2, 0xa1, 0xa9, 0x05, 0x49, 0x0d, 0x0a, 0x6d, 0x2a, 0x09, 0x56, 0x01,
    0xad, 0x50, 0x6d, 0x01, 0xd9, 0x02, 0xd1, 0x3a, 0xa8, 0x05, 0x29, 0x85, 0xa5, 0x0c, 0x2a, 0x09, 0x96, 0x54,
    0xb6, 0x08, 0x6c, 0x09, 0x64, 0x45, 0xd4, 0x0a, 0xa4, 0x05, 0x51, 0x25, 0x95, 0x0a, 0x2a, 0x72, 0x5b, 0x04,
    0xb6, 0x04, 0xac, 0x52, 0x6a, 0x05, 0xd2, 0x0a, 0xa2, 0x4a, 0x4a, 0x05, 0x55, 0x94, 0x2d, 0x0a, 0x5a, 0x02,
    0x75, 0x61, 0xb5, 0x02, 0x6a, 0x03, 0x61, 0x45, 0xa9, 0x0a, 0x4a, 0x05, 0x25, 0x25, 0x2d, 0x09, 0x9a, 0x68,
    0xda, 0x08, 0xb4, 0x09, 0xa8, 0x59, 0x54, 0x03, 0xa5, 0x0a, 0x91, 0x3a, 0x96, 0x04, 0xad, 0xb0, 0xad, 0x04,
    0xda, 0x04, 0xf4, 0x62, 0xb4, 0x05, 0x54, 0x0b, 0x44, 0x5d, 0x52, 0x0a, 0x95, 0x04, 0x55, 0x22, 0x6d, 0x02,
    0x5a, 0x71, 0xda, 0x02, 0xaa, 0x05, 0xb2, 0x55, 0x49, 0x0b, 0x4a, 0x0a, 0x2d, 0x39, 0x36, 0x01, 0x6d, 0x80,
    0x6d, 0x01, 0xd9, 0x02, 0xe9, 0x6a, 0xa8, 0x05, 0x29, 0x0b, 0x9a, 0x4c, 0xaa, 0x08, 0xb6, 0x08, 0xb4, 0x38,
    0x6c, 0x09, 0x54, 0x75, 0xd4, 0x0a, 0xa4, 0x05, 0x45, 0x55, 0x95, 0x0a, 0x9a, 0x04, 0x55, 0x44, 0xb5, 0x04,
    0x6a, 0x82, 0x6a, 0x05, 0xd2, 0x0a, 0x92, 0x6a, 0x4a, 0x05, 0x55, 0x0a, 0x2a, 0x4a, 0x5a, 0x02, 0xb5, 0x02,
    0xb2, 0x31, 0x69, 0x03, 0x31, 0x73, 0xa9, 0x0a, 0x4a, 0x05, 0x2d, 0x55, 0x2d, 0x09, 0x5a, 0x01, 0xd5, 0x48,
    0xb4, 0x09, 0x68, 0x89, 0x54, 0x0b, 0xa4, 0x0a, 0xa5, 0x6a, 0x95, 0x04, 0xad, 0x08, 0x6a, 0x44, 0xda, 0x04,
    0x74, 0x05, 0xb0, 0x25, 0x54, 0x03 
};


static char map0[] =
{ 7, 6, 6, 6, 6, 6, 6, 6, 6, 5, 6, 6, 6, 5, 5, 6, 6, 5, 5, 5, 5, 5, 5, 5, 5, 4, 5, 5 };
static char map1[] =
{ 5, 4, 5, 5, 5, 4, 4, 5, 5, 4, 4, 4, 4, 4, 4, 4, 4, 3, 4, 4, 4, 3, 3, 4, 4, 3, 3, 3 };
static char map2[] =
{ 6, 6, 6, 7, 6, 6, 6, 6, 5, 6, 6, 6, 5, 5, 6, 6, 5, 5, 5, 6, 5, 5, 5, 5, 4, 5, 5, 5, 5 };
static char map3[] =
{ 5, 5, 6, 6, 5, 5, 5, 6, 5, 5, 5, 5, 4, 5, 5, 5, 4, 4, 5, 5, 4, 4, 4, 5, 4, 4, 4, 4, 5 };
static char map4[] =
{ 6, 6, 6, 7, 6, 6, 6, 6, 5, 6, 6, 6, 5, 5, 6, 6, 5, 5, 5, 6, 5, 5, 5, 5, 4, 5, 5, 5, 5 };
static char map5[] =
{ 6, 6, 7, 7, 6, 6, 6, 7, 6, 6, 6, 6, 5, 6, 6, 6, 5, 5, 6, 6, 5, 5, 5, 6, 5, 5, 5, 5, 4, 5, 5, 5, 5 };
static char map6[] =
{ 7, 8, 8, 8, 7, 7, 8, 8, 7, 7, 7, 8, 7, 7, 7, 7, 6, 7, 7, 7, 6, 6, 7, 7, 6, 6, 6, 7, 7 };
static char map7[] =
{ 8, 8, 8, 9, 8, 8, 8, 8, 7, 8, 8, 8, 7, 7, 8, 8, 7, 7, 7, 8, 7, 7, 7, 7, 6, 7, 7, 7, 6, 6, 7, 7, 7 };
static char map8[] =
{ 8, 8, 8, 9, 8, 8, 8, 8, 7, 8, 8, 8, 7, 7, 8, 8, 7, 7, 7, 8, 7, 7, 7, 7, 6, 7, 7, 7, 7 };
static char map9[] =
{ 9, 9, 9, 9, 8, 9, 9, 9, 8, 8, 9, 9, 8, 8, 8, 9, 8, 8, 8, 8, 7, 8, 8, 8, 7, 7, 8, 8, 8 };
static char map10[] =
{ 8, 8, 8, 8, 7, 8, 8, 8, 7, 7, 8, 8, 7, 7, 7, 8, 7, 7, 7, 7, 6, 7, 7, 7, 6, 6, 7, 7, 7 };
static char map11[] =
{ 7, 8, 8, 8, 7, 7, 8, 8, 7, 7, 7, 8, 7, 7, 7, 7, 6, 7, 7, 7, 6, 6, 7, 7, 6, 6, 6, 7, 7 };

static char *sectionalTermMap[] =
{ map0, map1, map2, map3, map4, map5, map6, map7, map8, map9, map10, map11 };

static unsigned char termyear0[] =
{ 13, 49, 85, 117, 149, 185, 201, 250, 250 };
static unsigned char termyear1[] =
{ 13, 45, 81, 117, 149, 185, 201, 250, 250 };
static unsigned char termyear2[] =
{ 13, 48, 84, 112, 148, 184, 200, 201, 250 };
static unsigned char termyear3[] =
{ 13, 45, 76, 108, 140, 172, 200, 201, 250 };
static unsigned char termyear4[] =
{ 13, 44, 72, 104, 132, 168, 200, 201, 250 };
static unsigned char termyear5[] =
{ 5, 33, 68, 96, 124, 152, 188, 200, 201 };
static unsigned char termyear6[] =
{ 29, 57, 85, 120, 148, 176, 200, 201, 250 };
static unsigned char termyear7[] =
{ 13, 48, 76, 104, 132, 168, 196, 200, 201 };
static unsigned char termyear8[] =
{ 25, 60, 88, 120, 148, 184, 200, 201, 250 };
static unsigned char termyear9[] =
{ 16, 44, 76, 108, 144, 172, 200, 201, 250 };
static unsigned char termyear10[] =
{ 28, 60, 92, 124, 160, 192, 200, 201, 250 };
static unsigned char termyear11[] =
{ 17, 53, 85, 124, 156, 188, 200, 201, 250 };

static unsigned char *sectionalTermYear[] =
{   
    termyear0, termyear1, termyear2, termyear3, termyear4, termyear5, termyear6, 
    termyear7, termyear8, termyear9, termyear10, termyear11 
};
static unsigned char termmap0[] =
{ 21, 21, 21, 21, 21, 20, 21, 21, 21, 20, 20, 21, 21, 20, 20, 20, 20, 20, 20, 20, 20, 19, 20, 20, 20, 19, 19, 20 };
static char termmap1[] =
{ 20, 19, 19, 20, 20, 19, 19, 19, 19, 19, 19, 19, 19, 18, 19, 19, 19, 18, 18, 19, 19, 18, 18, 18, 18, 18, 18, 18 };
static unsigned char termmap2[] =
{ 21, 21, 21, 22, 21, 21, 21, 21, 20, 21, 21, 21, 20, 20, 21, 21, 20, 20, 20, 21, 20, 20, 20, 20, 19, 20, 20, 20, 20 };
static unsigned char termmap3[] =
{ 20, 21, 21, 21, 20, 20, 21, 21, 20, 20, 20, 21, 20, 20, 20, 20, 19, 20, 20, 20, 19, 19, 20, 20, 19, 19, 19, 20, 20 };
static unsigned char termmap4[] =
{ 21, 22, 22, 22, 21, 21, 22, 22, 21, 21, 21, 22, 21, 21, 21, 21, 20, 21, 21, 21, 20, 20, 21, 21, 20, 20, 20, 21, 21 };
static unsigned char termmap5[] =
{ 22, 22, 22, 22, 21, 22, 22, 22, 21, 21, 22, 22, 21, 21, 21, 22, 21, 21, 21, 21, 20, 21, 21, 21, 20, 20, 21, 21, 21 };
static unsigned char termmap6[] =
{ 23, 23, 24, 24, 23, 23, 23, 24, 23, 23, 23, 23, 22, 23, 23, 23, 22, 22, 23, 23, 22, 22, 22, 23, 22, 22, 22, 22, 23 };
static unsigned char termmap7[] =
{ 23, 24, 24, 24, 23, 23, 24, 24, 23, 23, 23, 24, 23, 23, 23, 23, 22, 23, 23, 23, 22, 22, 23, 23, 22, 22, 22, 23, 23 };
static unsigned char termmap8[] =
{ 23, 24, 24, 24, 23, 23, 24, 24, 23, 23, 23, 24, 23, 23, 23, 23, 22, 23, 23, 23, 22, 22, 23, 23, 22, 22, 22, 23, 23 };
static unsigned char termmap9[] =
{ 24, 24, 24, 24, 23, 24, 24, 24, 23, 23, 24, 24, 23, 23, 23, 24, 23, 23, 23, 23, 22, 23, 23, 23, 22, 22, 23, 23, 23 };
static unsigned char termmap10[] =
{ 23, 23, 23, 23, 22, 23, 23, 23, 22, 22, 23, 23, 22, 22, 22, 23, 22, 22, 22, 22, 21, 22, 22, 22, 21, 21, 22, 22, 22 };
static unsigned char termmap11[] =
{ 22, 22, 23, 23, 22, 22, 22, 23, 22, 22, 22, 22, 21, 22, 22, 22, 21, 21, 22, 22, 21, 21, 21, 22, 21, 21, 21, 21, 22 };

static unsigned char * principleTermMap[] =
{ (unsigned char *) termmap0, (unsigned char *) termmap1, (unsigned char *) termmap2, (unsigned char *) termmap3,
  (unsigned char *) termmap4, (unsigned char *) termmap5, (unsigned char *) termmap6, (unsigned char *) termmap7,
  (unsigned char *) termmap8, (unsigned char *) termmap9, (unsigned char *) termmap10,
  (unsigned char *) termmap11 
};

static unsigned char ptyear0[] =
{ 13, 45, 81, 113, 149, 185, 201 };
static unsigned char ptyear1[] =
{ 21, 57, 93, 125, 161, 193, 201 };
static unsigned char ptyear2[] =
{ 21, 56, 88, 120, 152, 188, 200, 201 };
static unsigned char ptyear3[] =
{ 21, 49, 81, 116, 144, 176, 200, 201 };
static unsigned char ptyear4[] =
{ 17, 49, 77, 112, 140, 168, 200, 201 };
static unsigned char ptyear5[] =
{ 28, 60, 88, 116, 148, 180, 200, 201 };
static unsigned char ptyear6[] =
{ 25, 53, 84, 112, 144, 172, 200, 201 };
static unsigned char ptyear7[] =
{ 29, 57, 89, 120, 148, 180, 200, 201 };
static unsigned char ptyear8[] =
{ 17, 45, 73, 108, 140, 168, 200, 201 };
static unsigned char ptyear9[] =
{ 28, 60, 92, 124, 160, 192, 200, 201 };
static unsigned char ptyear10[] =
{ 16, 44, 80, 112, 148, 180, 200, 201 };
static unsigned char ptyear11[] =
{ 17, 53, 88, 120, 156, 188, 200, 201 };
static unsigned char *principleTermYear[] =
{ ptyear0, ptyear1, ptyear2, ptyear3, ptyear4, ptyear5, ptyear6, ptyear7, ptyear8, ptyear9, ptyear10, ptyear11 };
static int bigLeapMonthYears[] =
{
    //大闰月的闰年年份
    6, 14, 19, 25, 33, 36, 38, 41, 44, 52, 55, 79, 117, 136, 147, 150, 155, 158, 185, 193 
};

//声明
static int isGregorianLeapYear(int year);
static int daysInGregorianMonth_func(int y, int m);
static int daysInChineseMonth(int y, int m);
static int nextChineseMonth(int y, int m);
static int sectionalTerm_func(int y, int m);
static int principleTerm_func(int y, int m);                

/*用到的静态变量*/
static int baseYear = 1901;
static int baseMonth = 1;
static int baseDate = 1;
static int baseIndex = 0;
static int baseChineseYear = 4598 - 1;
static int baseChineseMonth = 11;
static int baseChineseDate = 11;
static int gregorianYear;
static int gregorianMonth;
static int gregorianDate;
static int isGregorianLeap;
static int chineseYear;
static int chineseMonth; // 负数表示闰月
static int chineseDate;
static int sectionalTerm;
static int principleTerm;

#define _ABS(x)  ( ((x) > 0) ? (x): -(x) )

static char *stemNames[] =
{ "甲", "乙", "丙", "丁", "戊", "己", "庚", "辛", "壬", "癸" };
static char *branchNames[] =
{ "子", "丑", "寅", "卯", "辰", "巳", "午", "未", "申", "酉", "戌", "亥" };
static char *monthNames[] =
{ "一", "二", "三", "四", "五", "六", "七", "八", "九", "十", "十一", "十二", "十三", 
  "十四", "十五", "十六", "十七", "十八", "十九", "二十", "二十一", "二十二", "二十三",
  "二十四", "二十五", "二十六", "二十七", "二十八", "二十九", "三十", "三十一" 
};
static char *chineseMonthNames[] =
{ "正", "二", "三", "四", "五", "六", "七", "八", "九", "十", "冬", "腊" };
static char *principleTermNames[] =
{ "大寒", "雨水", "春分", "谷雨", "小满", "夏至", "大暑", "处暑", "秋分", "霜降", "小雪", "冬至" };
static char *sectionalTermNames[] =
{ "小寒", "立春", "惊蛰", "清明", "立夏", "芒种", "小暑", "立秋", "白露", "寒露", "立冬", "大雪" };
static char *animalNames[] =
{ "鼠", "牛", "虎", "兔", "龙", "蛇", "马", "羊", "猴", "鸡", "狗", "猪" };


/********   繁体字符  **********************/
static char *stemNames_F[] =
{"ヒ","","","","コ","","┌","ǒ","ば",""};
static char *branchNames_F[] =
{"","ぁ","盙","","ò","","と","ゼ","ビ","ɑ","Ζ","ヨ"};
static char *monthNames_F[] =
{"","","","","き","せ","","","","","","",
 "","","き", "せ", "", "","", "","",
 "","","","き","せ","","","",
 "",""
};
static char *chineseMonthNames_F[] =
{"タ","","","","き","せ","","","","","","镁"};
static char *principleTermNames_F[] =
{"碒","獴","琄だ","é獴","骸","甃","椿","矪椿","だ","流","撤",""};
static char *sectionalTermNames_F[] =
{"碒","ミ琄","佩頷","睲","ミ甃","▇贺","椿","ミ","フ臩","碒臩","ミ","撤"};
static char *animalNames_F[] =
{"公","","","ㄟ","纒","矰","皑","ο","礥","蔓","","睫"};


static void setGregorian(int y, int m, int d)
{
    gregorianYear = y;
    gregorianMonth = m;
    gregorianDate = d;
    isGregorianLeap = isGregorianLeapYear(y);
    ///     dayOfYear = dayOfYear(y,m,d);
    ///     dayOfWeek = dayOfWeek(y,m,d);
    chineseYear = 0;
    chineseMonth = 0;
    chineseDate = 0;
    sectionalTerm = 0;
    principleTerm = 0;
}

static int isGregorianLeapYear(int year)
{
    int isLeap = 0;
    
    if ((year % 4) == 0)
    {
        isLeap = 1;
    }
    if ((year % 100) == 0)
    {
        isLeap = 0;
    }
    if ((year % 400) == 0)
    {
        isLeap = 1;
    }
    return isLeap;
}

static int daysInGregorianMonth_func(int y, int m)
{
    int d = daysInGregorianMonth[m - 1];
    
    if ((m == 2) && (isGregorianLeapYear(y) == 1))
    {
        d++; // 公历闰年二月多一天
    }
    return d;
}

static int computeChineseFields(void)
{
    int startYear = baseYear;
    int startMonth = baseMonth;
    int startDate = baseDate;
    int i = 0;    
    int daysDiff = 0;    
    int lastDate;
    int nextMonth;    
      
    if ((gregorianYear < 1901) || (gregorianYear > 2100))
    {
        return 1;
    }

    chineseYear = baseChineseYear;
    chineseMonth = baseChineseMonth;
    chineseDate = baseChineseDate;
    // 第二个对应日，用以提高计算效率
    // 公历 2000 年 1 月 1 日，对应农历 4697 年 11 月 25 日

    if (gregorianYear >= 2000)
    {
        startYear = baseYear + 99;
        startMonth = 1;
        startDate = 1;
        chineseYear = baseChineseYear + 99;
        chineseMonth = 11;
        chineseDate = 25;
    }

    for (i = startYear; i < gregorianYear; i++)
    {
        daysDiff += 365;
        if (isGregorianLeapYear(i) == 1)
        {
            daysDiff += 1; // leap year
        }
    }
    for (i = startMonth; i < gregorianMonth; i++)
    {
        daysDiff += daysInGregorianMonth_func(gregorianYear, i);
    }
    daysDiff += gregorianDate - startDate;

    chineseDate += daysDiff;
    lastDate = daysInChineseMonth(chineseYear, chineseMonth);
    nextMonth = nextChineseMonth(chineseYear, chineseMonth);
    while (chineseDate > lastDate)
    {
        if (_ABS(nextMonth) < _ABS(chineseMonth))
        {
            chineseYear++;
        }
        chineseMonth = nextMonth;
        chineseDate -= lastDate;
        lastDate = daysInChineseMonth(chineseYear, chineseMonth);
        nextMonth = nextChineseMonth(chineseYear, chineseMonth);
    }
    return 0;
}

static int daysInChineseMonth(int y, int m)
{
    // 注意：闰月 m < 0
    int idx = y - baseChineseYear + baseIndex;
    unsigned int v = 0;
    int l = 0;
    int d = 30;
    int i = 0;
    
    if ((1 <= m) && (m <= 8))
    {
        v = chineseMonths[2 * idx];
        l = m - 1;
        if (((v >> l) & 0x01) == 1)
        {
            d = 29;
        }
    }
    else if ((9 <= m) && (m <= 12))
    {
        v = chineseMonths[(2 * idx) + 1];
        l = m - 9;
        if (((v >> l) & 0x01) == 1)
        {
            d = 29;
        }
    }
    else
    {
        v = chineseMonths[(2 * idx) + 1];
        v = (v >> 4) & 0x0F;
        if (v != (unsigned int)_ABS(m))
        {
            d = 0;
        }
        else
        {
            d = 29;
            for (i = 0; i < (int)(sizeof(bigLeapMonthYears) / sizeof(int)); i++)
            {
                if (bigLeapMonthYears[i] == idx)
                {
                    d = 30;
                    break;
                }
            }
        }
    }
    return d;
}

static int nextChineseMonth(int y, int m)
{
    int idx;
    unsigned int v;    
    int n = _ABS(m) + 1;    
    
    if (m > 0)
    {
        idx = y - baseChineseYear + baseIndex;
        v = chineseMonths[2 * idx + 1];
        v = (v >> 4) & 0x0F;
        if (v == (unsigned int)m)
        {
            n = -m;
        }
    }
    if (n == 13)
    {
        n = 1;
    }
    
    return n;
}

static int computeSolarTerms(void)
{
    if ((gregorianYear < 1901) || (gregorianYear > 2100))
    {
        return 1;
    }
    sectionalTerm = sectionalTerm_func(gregorianYear, gregorianMonth);
    principleTerm = principleTerm_func(gregorianYear, gregorianMonth);
    return 0;
}

static int sectionalTerm_func(int y, int m)
{
    int term = 0;
    int idx = 0;
    int ry = y - baseYear + 1;    
    
    
    if ((y < 1901) || (y > 2100))
    {
        return 0;
    }
    

    while (ry >= sectionalTermYear[m - 1][idx])
    {
        idx++;
    }
    
    term = (int)sectionalTermMap[m - 1][(4 * idx) + (ry % 4)];
    if ((ry == 121) && (m == 4))
    {
        term = 5;
    }
    if ((ry == 132) && (m == 4))
    {
        term = 5;
    }
    if ((ry == 194) && (m == 6))
    {
        term = 6;
    }
    
    return term;
}

static int principleTerm_func(int y, int m)
{
    int idx = 0;
    int ry = y - baseYear + 1;    
    int term = 0;
      
    if ((y < 1901) || (y > 2100))
    {
        return 0;
    }

    while (ry >= principleTermYear[m - 1][idx])
    {
        idx++;
    }
    
    term = principleTermMap[m - 1][(4 * idx) + (ry % 4)];

    if ((ry == 171) && (m == 3))
    {
        term = 21;
    }
    if ((ry == 181) && (m == 5))
    {
        term = 21;
    }
    
    return term;
}

/*注意str必须已分配空间且str空间足够大*/

int getLunarString(char *str, const DateVarType *scolar, DateVarType *lunar, int languageId)
{
    int year = scolar->year;
    int month = scolar->month;
    int day = scolar->day;    
    
    
    if (str == NULL)
    {
        memset(lunar, 0, sizeof(DateVarType));
        return 0;
    }

    setGregorian(year, month, day);
    computeChineseFields();
    computeSolarTerms();

    lunar->year = chineseYear;
    lunar->month = _ABS(chineseMonth);
    lunar->day = chineseDate;

    switch (languageId)
    {
        case 2: //简体
        if ((chineseYear>0)&&(chineseDate>0)&&(gregorianMonth>0)&&(chineseMonth!=0))
        {
            if (gregorianDate == principleTerm)
            {
                sprintf(str, "%s%s(%s)年%s月%s   %s", stemNames[(chineseYear - 1) % 10], 
                        branchNames[(chineseYear - 1) % 12], animalNames[(chineseYear - 1) % 12], 
                        chineseMonthNames[_ABS(chineseMonth) - 1], monthNames[chineseDate - 1], 
                        principleTermNames[gregorianMonth - 1]);
            }
            else if (gregorianDate == sectionalTerm)
            {
                sprintf(str, "%s%s(%s)年%s月%s  %s", stemNames[(chineseYear - 1) % 10], 
                        branchNames[(chineseYear - 1) % 12], animalNames[(chineseYear - 1) % 12], 
                        chineseMonthNames[_ABS(chineseMonth) - 1], monthNames[chineseDate - 1], 
                        sectionalTermNames[gregorianMonth - 1]);
            }
            else   
            {
                sprintf(str, "%s%s(%s)年%s月%s", stemNames[(chineseYear - 1) % 10], 
                        branchNames[(chineseYear - 1) % 12], animalNames[(chineseYear - 1) % 12], 
                        chineseMonthNames[_ABS(chineseMonth) - 1], monthNames[chineseDate - 1]);
            }
        }
        break;

        case 3: //繁体
        if ((chineseYear>0)&&(chineseDate>0)&&(gregorianMonth>0)&&(chineseMonth!=0))     
        {     
            if (gregorianDate == principleTerm)
            {
                sprintf(str, "%s%s(%s)%sる%s   %s", stemNames_F[(chineseYear-1)%10],
                        branchNames_F[(chineseYear - 1) % 12], animalNames_F[(chineseYear - 1) % 12], 
                        chineseMonthNames_F[_ABS(chineseMonth) - 1], monthNames_F[chineseDate - 1], 
                        principleTermNames_F[gregorianMonth - 1]);
            }
            else if (gregorianDate == sectionalTerm)
            {
                sprintf(str, "%s%s(%s)%sる%s  %s", stemNames_F[(chineseYear-1)%10],
                        branchNames_F[(chineseYear - 1) % 12], animalNames_F[(chineseYear - 1) % 12], 
                        chineseMonthNames_F[_ABS(chineseMonth) - 1], monthNames_F[chineseDate - 1], 
                        sectionalTermNames_F[gregorianMonth - 1]); 
            }
            else
            {
                sprintf(str, "%s%s(%s)%sる%s", stemNames_F[(chineseYear-1)%10],
                        branchNames_F[(chineseYear - 1) % 12], animalNames_F[(chineseYear - 1) % 12], 
                        chineseMonthNames_F[_ABS(chineseMonth) - 1], monthNames_F[chineseDate - 1]);
            } 
        }
        break;

        default:
        sprintf(str, "%d/%d/%d", year, month, day);

    }

    //printf(str);
    //printf("\n");
    return 0;
}

/*
 int main(int argc, char* argv[])
 {
 int indexg;
 int indexz;
 //输入阳历，计算阴历

 char str[48];
 getLunarString(str, 2006, 11, 22);
 printf("strlen (str)=%d \n", strlen(str));
 return 0;
 }

 */

