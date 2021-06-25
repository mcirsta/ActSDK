/*
 ********************************************************************************
 *                         uSDK130
 *
 *
 *
 *                     (c) Copyright 2002-2003, Actions Co,Ld.
 *                                       All Right Reserved
 *
 * File   :  getHoliday.c
 * By     :  ywwang
 * Purpose:   根据公历判断法定节日
 *
 * Version:       first version     2006-11-20
 ********************************************************************************
 ********************************************************************************
 */

/*本文件向外部提供的唯一接口*/
/**
 *  int  getHolidayStr(char *str, const DateVarType *scolar, DateVarType *lunar,  int lanid)
 *  输入参数：year, month, day: 公历的年/月/日
 *            lanid: 语言id号
 *
 */

#include "calendar_play.h"
#include "date.h"



static int getChineseHoliday_S(char *str, int year, int month, int day);
static int getChineseHoliday_T(char *str, int year, int month, int day);
static int getForeignHoliday(char *str, int year, int month, int day);
static int getLunarHoliday_S(char *str, const DateVarType *lunar);
static int getLunarHoliday_T(char *str, const DateVarType *lunar);

// 没有对应节日，返回0，否则返回1
int getHolidayStr(char *str, const DateVarType *scolar, DateVarType *lunar, int lanid)
{
    int y = scolar->year;
    int m = scolar->month;
    int d = scolar->day;
    int retVal = 0;
    
    if (NULL == str)
    {
        return 0;
    }
    
    switch (lanid)
    {
        case 2: //语言环境是简体中文
        retVal = getLunarHoliday_S(str, lunar);
        if (retVal == 0)
        {
            retVal = getChineseHoliday_S(str, y, m, d);
        }
        break;
        case 3: //繁体中文
        retVal = getLunarHoliday_T(str, lunar);
        if (retVal == 0)
        {
            retVal = getChineseHoliday_T(str, y, m, d);
        }
        break;
        default: //默认是英文
        retVal = getForeignHoliday(str, y, m, d);
    }
    //printf("in getHolidaystr :          %s \n", str);

    return retVal;
}

/*阳历5月第二个星期日是母亲节*/
static int isMother_Day(int y, int m, int d)
{
    unsigned int jd;
    
    if ((d < 8) || (d > 14))
    {
        return 0;
    }
    if (setYMD(y, m, d, &jd) == 1)
    {
        if ((dayOfWeek(&jd) % 7) == 0)
        {
            return 1;
        }
    }
    return 0;
}

/*阳历6月第三个星期日是父亲节*/
static int isFather_Day(int yy, int mm, int dd)
{
    unsigned int jd;
    
    if ((dd < 15) || (dd > 21))
    {
        return 0;
    }
    if (setYMD(yy, mm, dd, &jd) == 1)
    {
        if ((dayOfWeek(&jd) % 7) == 0)
        {
            return 1;
        }
    }
    return 0;
}

//返回 中国节日
static int getChineseHoliday_S(char * str, int yy, int mm, int dd)
{

    switch (mm)
    {
        case 1:
        switch (dd)
        {
            case 1:
            memcpy(str, "元旦", strlen("元旦") + 1);
            return 1;
            default:
            str[0] = '\0';
            return 0;
        }
        case 2:
        switch (dd)
        {
            case 14:
            memcpy(str, "情人节", strlen("情人节") + 1);
            return 1;
            default:
            str[0] = '\0';
            return 0;
        }
        case 3:
        switch (dd)
        {
            case 8:
            memcpy(str, "妇女节", strlen("妇女节") + 1);
            return 1;

            case 12:
            memcpy(str, "植树节", strlen("植树节") + 1);
            return 1;
            default:
            str[0] = '\0';
            return 0;
        }
        case 4:
        switch (dd)
        {
            case 1:
            memcpy(str, "愚人节", strlen("愚人节") + 1);
            return 1;
            default:
            str[0] = '\0';
            return 0;
        }
        case 5:
        switch (dd)
        {
            case 4:
            memcpy(str, "青年节", strlen("青年节") + 1);
            return 1;
            case 1:
            memcpy(str, "国际劳动节", strlen("国际劳动节") + 1);
            return 1;
            case 12:
            memcpy(str, "护士节", strlen("护士节") + 1);
            return 1;
            default:
            if (isMother_Day(yy, mm, dd) == 1)
            {
                memcpy(str, "母亲节", strlen("母亲节") + 1);
                return 1;
            }
            else
            {
                str[0] = '\0';
                return 0;
            }
        }
        case 6:
        switch (dd)
        {
            case 1:
            memcpy(str, "儿童节", strlen("儿童节") + 1);
            return 1;
            default:
            if (isFather_Day(yy, mm, dd) == 1)
            {
                memcpy(str, "父亲节", strlen("父亲节") + 1);
                return 1;
            }
            else
            {
                str[0] = '\0';
                return 0;
            }
        }
        case 7:
        switch (dd)
        {
            case 1:
            memcpy(str, "建党节", strlen("建党节") + 1);
            return 1;
            default:
            str[0] = '\0';
            return 0;
        }
        case 8:
        switch (dd)
        {
            case 1:
            memcpy(str, "建军节", strlen("建军节") + 1);
            return 1;
            default:
            str[0] = '\0';
            return 0;
        }
        case 9:
        switch (dd)
        {
            case 10:
            memcpy(str, "教师节", strlen("教师节") + 1);
            return 1;
            case 18:
            memcpy(str, "九一八事变", strlen("九一八事变") + 1);
            return 1;
            default:
            str[0] = '\0';
            return 0;
        }
        case 10:
        switch (dd)
        {
            case 1:
            memcpy(str, "国庆节", strlen("国庆节") + 1);
            return 1;
            default:
            str[0] = '\0';
            return 0;
        }
        case 12:
        switch (dd)
        {
            case 25:
            memcpy(str, "圣诞节", strlen("圣诞节") + 1);
            return 1;
            default:
            str[0] = '\0';
            return 0;
        }

        default:
        str[0] = '\0';
        return 0;
    }

}

//返回 中国节日之繁体版
static int getChineseHoliday_T(char * str, int y, int m, int d)
{

    switch (m)
    {
        case 1:
        switch (d)
        {
            case 1:
            memcpy(str, "じス", strlen("じス")+1);
            return 1;
            default:
            str[0] = '\0';
            return 0;
        }
        case 2:
        switch (d)
        {
            case 14:
            memcpy(str, "薄竊", strlen("薄竊")+1);
            return 1;
            default:
            str[0] = '\0';
            return 0;
        }
        case 3:
        switch (d)
        {
            case 8:
            memcpy(str, "包竊", strlen("包竊")+1);
            return 1;
            default:
            str[0] = '\0';
            return 0;
        }
        case 4:
        switch (d)
        {
            case 1:
            memcpy(str, "稭竊", strlen("稭竊")+1);
            return 1;
            default:
            str[0] = '\0';
            return 0;
        }
        case 5:
        switch (d)
        {
            case 4:
            memcpy(str, "獵竊", strlen("獵竊")+1);
            return 1;
            case 1:
            memcpy(str, "瓣悔骋笆竊", strlen("瓣悔骋笆竊")+1);
            return 1;

            case 12:
            memcpy(str, "臔竊", strlen("臔竊")+1);
            return 1;

            default:
            if (isMother_Day(y, m, d) == 1)
            {
                memcpy(str, "ダ克竊", strlen("ダ克竊")+1);
                return 1;
            }
            else
            {
                str[0] = '\0';
                return 0;
            }
        }
        case 6:
        switch (d)
        {
            case 1:
            memcpy(str, "ㄠ担竊", strlen("ㄠ担竊")+1);
            return 1;
            default:
            if (isFather_Day(y, m, d) == 1)
            {
                memcpy(str, "克竊", strlen("克竊")+1);
                return 1;
            }
            else
            {
                str[0] = '\0';
                return 0;
            }
        }
        case 7:
        switch (d)
        {
            case 1:
            memcpy(str, "囊竊", strlen("囊竊")+1);
            return 1;
            default:
            str[0] = '\0';
            return 0;
        }
        case 8:
        switch (d)
        {
            case 1:
            memcpy(str, "瓁竊", strlen("瓁竊")+1);
            return 1;
            default:
            str[0] = '\0';
            return 0;
        }
        case 9:
        switch (d)
        {
            case 10:
            memcpy(str, "毙畍竊", strlen("毙畍竊")+1);
            return 1;
            case 18:
            memcpy(str, "ㄆ跑", strlen("ㄆ跑")+1);
            return 1;
            default:
            str[0] = '\0';
            return 0;
        }
        case 10:
        switch (d)
        {
            case 1:
            memcpy(str, "瓣紋竊", strlen("瓣紋竊")+1);
            return 1;
            default:
            str[0] = '\0';
            return 0;
        }
        case 12:
        switch (d)
        {
            case 25:
            memcpy(str, "圣诞节", strlen("圣诞节")+1);
            return 1;
            default:
            str[0] = '\0';
            return 0;
        }

        default:
        str[0] = '\0';
        return 0;
    }

}

//返回西方节日
static int getForeignHoliday(char * str, int y, int m, int d)
{
    //printf("year =%d month=%d day=%d \n", year, month, day);
    switch (m)
    {
        case 1:
        switch (d)
        {
            case 1:
            memcpy(str, "New Years's Day", strlen("New Years's Day") + 1);
            return 1;
            default:
            str[0] = '\0';
            return 0;
        }
        case 2:
        switch (d)
        {
            case 14:
            memcpy(str, "Valentine's Day", strlen("valentine's day") + 1);
            return 1;
            default:
            str[0] = '\0';
            return 0;
        }
//        case 3:
//        switch (d)
//        {
//            //case 12:
//            //memcpy(str, "Easter", strlen("Easter") + 1);
//            //return 1;
//            default:
//            str[0] = '\0';
//            return 0;
//        }
        case 4:
        switch (d)
        {
            case 1:
            memcpy(str, "Fool's Day", strlen("Fool's Day") + 1);
            return 1;
            default:
            str[0] = '\0';
            return 0;
        }
        case 10:
        switch (d)
        {
            case 31:
            memcpy(str, "Halloween Eve", strlen("Halloween Eve") + 1);
            return 1;
            default:
            str[0] = '\0';
            return 0;
        }

        case 12:
        switch (d)
        {
            case 25:
            memcpy(str, "Christmas Day", strlen("Christmas Day") + 1);
            return 1;
            default:
            str[0] = '\0';
            return 0;
        }

        default:
        str[0] = '\0';
        return 0;
    }

}

/*
 *     如果存在农历节日,返回1,否则返回0
 *
 */
static int getLunarHoliday_S(char *str, const DateVarType *lunar)
{
    int retVal = 0;
    
    switch (lunar->month)
    {
        case 1:
        switch (lunar->day)
        {
            case 1:
            memcpy(str, "春节", strlen("春节") + 1);
            retVal = 1;
            default:
            break;
        }
        break;
        case 5:
        switch (lunar->day)
        {
            case 5:
            memcpy(str, "端午节", strlen("端午节") + 1);
            retVal = 1;
            default:
            break;
        }
        break;
        case 7:
        switch (lunar->day)
        {
            case 7:
            memcpy(str, "七夕", strlen("七夕") + 1);
            retVal = 1;
            default:
            break;
        }
        break;
        case 8:
        switch (lunar->day)
        {
            case 15:
            memcpy(str, "中秋节", strlen("中秋节") + 1);
            retVal = 1;
            default:
            break;
        }
        break;
        case 9:
        switch (lunar->day)
        {
            case 9:
            memcpy(str, "重阳节", strlen("重阳节") + 1);
            retVal = 1;
            default:
            break;
        }
        break;
        default:
        break;

    }
    if (retVal == 0)
    {
        str[0] = '\0';
    }
    
    return retVal;
}

/*
 *     如果存在农历节日,返回1,否则返回0
 *
 */
static int getLunarHoliday_T(char *str, const DateVarType *lunar)
{
    int retVal = 0;
    switch (lunar->month)
    {

        case 1:
        switch (lunar->day)
        {
            case 1:
            memcpy(str, "琄竊", strlen("琄竊")+1);
            retVal = 1;
            default:
            break;
        }
        break;
        case 5:
        switch (lunar->day)
        {
            case 5:
            memcpy(str, "狠と竊", strlen("狠と竊")+1);
            retVal = 1;
            default:
            break;
        }
        break;
        case 7:
        switch (lunar->day)
        {
            case 7:
            memcpy(str, "", strlen("")+1);
            retVal = 1;
            default:
            break;
        }
        break;
        case 8:
        switch (lunar->day)
        {
            case 15:
            memcpy(str, "い竊", strlen("い竊")+1);
            retVal = 1;
            default:
            break;
        }
        break;
        case 9:
        switch (lunar->day)
        {
            case 9:
            memcpy(str, "锭竊", strlen("锭竊")+1);
            retVal = 1;
            default:
            break;
        }
        break;

        default:
        break;
    }
    if (retVal == 1)
    {
        str[0] = '\0';
    }
    
    return retVal;
}

