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
 * Purpose:   ���ݹ����жϷ�������
 *
 * Version:       first version     2006-11-20
 ********************************************************************************
 ********************************************************************************
 */

/*���ļ����ⲿ�ṩ��Ψһ�ӿ�*/
/**
 *  int  getHolidayStr(char *str, const DateVarType *scolar, DateVarType *lunar,  int lanid)
 *  ���������year, month, day: ��������/��/��
 *            lanid: ����id��
 *
 */

#include "calendar_play.h"
#include "date.h"



static int getChineseHoliday_S(char *str, int year, int month, int day);
static int getChineseHoliday_T(char *str, int year, int month, int day);
static int getForeignHoliday(char *str, int year, int month, int day);
static int getLunarHoliday_S(char *str, const DateVarType *lunar);
static int getLunarHoliday_T(char *str, const DateVarType *lunar);

// û�ж�Ӧ���գ�����0�����򷵻�1
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
        case 2: //���Ի����Ǽ�������
        retVal = getLunarHoliday_S(str, lunar);
        if (retVal == 0)
        {
            retVal = getChineseHoliday_S(str, y, m, d);
        }
        break;
        case 3: //��������
        retVal = getLunarHoliday_T(str, lunar);
        if (retVal == 0)
        {
            retVal = getChineseHoliday_T(str, y, m, d);
        }
        break;
        default: //Ĭ����Ӣ��
        retVal = getForeignHoliday(str, y, m, d);
    }
    //printf("in getHolidaystr :          %s \n", str);

    return retVal;
}

/*����5�µڶ�����������ĸ�׽�*/
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

/*����6�µ������������Ǹ��׽�*/
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

//���� �й�����
static int getChineseHoliday_S(char * str, int yy, int mm, int dd)
{

    switch (mm)
    {
        case 1:
        switch (dd)
        {
            case 1:
            memcpy(str, "Ԫ��", strlen("Ԫ��") + 1);
            return 1;
            default:
            str[0] = '\0';
            return 0;
        }
        case 2:
        switch (dd)
        {
            case 14:
            memcpy(str, "���˽�", strlen("���˽�") + 1);
            return 1;
            default:
            str[0] = '\0';
            return 0;
        }
        case 3:
        switch (dd)
        {
            case 8:
            memcpy(str, "��Ů��", strlen("��Ů��") + 1);
            return 1;

            case 12:
            memcpy(str, "ֲ����", strlen("ֲ����") + 1);
            return 1;
            default:
            str[0] = '\0';
            return 0;
        }
        case 4:
        switch (dd)
        {
            case 1:
            memcpy(str, "���˽�", strlen("���˽�") + 1);
            return 1;
            default:
            str[0] = '\0';
            return 0;
        }
        case 5:
        switch (dd)
        {
            case 4:
            memcpy(str, "�����", strlen("�����") + 1);
            return 1;
            case 1:
            memcpy(str, "�����Ͷ���", strlen("�����Ͷ���") + 1);
            return 1;
            case 12:
            memcpy(str, "��ʿ��", strlen("��ʿ��") + 1);
            return 1;
            default:
            if (isMother_Day(yy, mm, dd) == 1)
            {
                memcpy(str, "ĸ�׽�", strlen("ĸ�׽�") + 1);
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
            memcpy(str, "��ͯ��", strlen("��ͯ��") + 1);
            return 1;
            default:
            if (isFather_Day(yy, mm, dd) == 1)
            {
                memcpy(str, "���׽�", strlen("���׽�") + 1);
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
            memcpy(str, "������", strlen("������") + 1);
            return 1;
            default:
            str[0] = '\0';
            return 0;
        }
        case 8:
        switch (dd)
        {
            case 1:
            memcpy(str, "������", strlen("������") + 1);
            return 1;
            default:
            str[0] = '\0';
            return 0;
        }
        case 9:
        switch (dd)
        {
            case 10:
            memcpy(str, "��ʦ��", strlen("��ʦ��") + 1);
            return 1;
            case 18:
            memcpy(str, "��һ���±�", strlen("��һ���±�") + 1);
            return 1;
            default:
            str[0] = '\0';
            return 0;
        }
        case 10:
        switch (dd)
        {
            case 1:
            memcpy(str, "�����", strlen("�����") + 1);
            return 1;
            default:
            str[0] = '\0';
            return 0;
        }
        case 12:
        switch (dd)
        {
            case 25:
            memcpy(str, "ʥ����", strlen("ʥ����") + 1);
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

//���� �й�����֮�����
static int getChineseHoliday_T(char * str, int y, int m, int d)
{

    switch (m)
    {
        case 1:
        switch (d)
        {
            case 1:
            memcpy(str, "����", strlen("����")+1);
            return 1;
            default:
            str[0] = '\0';
            return 0;
        }
        case 2:
        switch (d)
        {
            case 14:
            memcpy(str, "���H�`", strlen("���H�`")+1);
            return 1;
            default:
            str[0] = '\0';
            return 0;
        }
        case 3:
        switch (d)
        {
            case 8:
            memcpy(str, "���k�`", strlen("���k�`")+1);
            return 1;
            default:
            str[0] = '\0';
            return 0;
        }
        case 4:
        switch (d)
        {
            case 1:
            memcpy(str, "�M�H�`", strlen("�M�H�`")+1);
            return 1;
            default:
            str[0] = '\0';
            return 0;
        }
        case 5:
        switch (d)
        {
            case 4:
            memcpy(str, "�C�~�`", strlen("�C�~�`")+1);
            return 1;
            case 1:
            memcpy(str, "��ڳҰʸ`", strlen("��ڳҰʸ`")+1);
            return 1;

            case 12:
            memcpy(str, "�@�h�`", strlen("�@�h�`")+1);
            return 1;

            default:
            if (isMother_Day(y, m, d) == 1)
            {
                memcpy(str, "���˸`", strlen("���˸`")+1);
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
            memcpy(str, "�ൣ�`", strlen("�ൣ�`")+1);
            return 1;
            default:
            if (isFather_Day(y, m, d) == 1)
            {
                memcpy(str, "���˸`", strlen("���˸`")+1);
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
            memcpy(str, "���Ҹ`", strlen("���Ҹ`")+1);
            return 1;
            default:
            str[0] = '\0';
            return 0;
        }
        case 8:
        switch (d)
        {
            case 1:
            memcpy(str, "�حx�`", strlen("�حx�`")+1);
            return 1;
            default:
            str[0] = '\0';
            return 0;
        }
        case 9:
        switch (d)
        {
            case 10:
            memcpy(str, "�Юv�`", strlen("�Юv�`")+1);
            return 1;
            case 18:
            memcpy(str, "�E�@�K����", strlen("�E�@�K����")+1);
            return 1;
            default:
            str[0] = '\0';
            return 0;
        }
        case 10:
        switch (d)
        {
            case 1:
            memcpy(str, "��y�`", strlen("��y�`")+1);
            return 1;
            default:
            str[0] = '\0';
            return 0;
        }
        case 12:
        switch (d)
        {
            case 25:
            memcpy(str, "ʥ����", strlen("ʥ����")+1);
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

//������������
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
 *     �������ũ������,����1,���򷵻�0
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
            memcpy(str, "����", strlen("����") + 1);
            retVal = 1;
            default:
            break;
        }
        break;
        case 5:
        switch (lunar->day)
        {
            case 5:
            memcpy(str, "�����", strlen("�����") + 1);
            retVal = 1;
            default:
            break;
        }
        break;
        case 7:
        switch (lunar->day)
        {
            case 7:
            memcpy(str, "��Ϧ", strlen("��Ϧ") + 1);
            retVal = 1;
            default:
            break;
        }
        break;
        case 8:
        switch (lunar->day)
        {
            case 15:
            memcpy(str, "�����", strlen("�����") + 1);
            retVal = 1;
            default:
            break;
        }
        break;
        case 9:
        switch (lunar->day)
        {
            case 9:
            memcpy(str, "������", strlen("������") + 1);
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
 *     �������ũ������,����1,���򷵻�0
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
            memcpy(str, "�K�`", strlen("�K�`")+1);
            retVal = 1;
            default:
            break;
        }
        break;
        case 5:
        switch (lunar->day)
        {
            case 5:
            memcpy(str, "�ݤȸ`", strlen("�ݤȸ`")+1);
            retVal = 1;
            default:
            break;
        }
        break;
        case 7:
        switch (lunar->day)
        {
            case 7:
            memcpy(str, "�C�i", strlen("�C�i")+1);
            retVal = 1;
            default:
            break;
        }
        break;
        case 8:
        switch (lunar->day)
        {
            case 15:
            memcpy(str, "����`", strlen("����`")+1);
            retVal = 1;
            default:
            break;
        }
        break;
        case 9:
        switch (lunar->day)
        {
            case 9:
            memcpy(str, "�����`", strlen("�����`")+1);
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

