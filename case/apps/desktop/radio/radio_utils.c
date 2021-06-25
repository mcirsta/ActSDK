/********************************************************************************
 *                              USDK(1100)
 *                             Module: app
 *                 Copyright(c) 2003-2009 Actions Semiconductor,
 *                            All Rights Reserved.
 * File : radio_utils.c
 * History:
 *      <author>    <time>           <version >             <desc>
 *      cheason    2009-04-23          v1.0               build this file
 ********************************************************************************/
/*!
 * \file     radio_utils.c
 * \brief    FM应用通用函数
 * \author   cheason
 *
 * \version 1.0
 * \date  2009/04/23
 *******************************************************************************/

#include "radio_app.h"
#include "radio_playing_option_band.h"
#include "radio_utils.h"

/*
 * 查询某个值在表中的index
 * 参数：
 *   value: 查询的值
 *   table: 表的首地址
 *   num: 表中包含值的个数
 */
 /*
int get_index(int value, int *table, int num)
{
    int i;
    for(i=0; i<num; i++)
    {
        if(value == table[i])
        {
            return i;
        }
    }

    return -1;
}
*/

/*
 * 频率值转成字符串
 * 利用频率值的特点，并假定字符串空间足够
 */
void freq_to_str(char *str,unsigned int freq)
{
    unsigned int integ, fract, temp;

    integ = freq/1000;

    temp = freq%1000;
    fract = temp/100;

    temp = temp%100;
    if(0 != temp)
    {
        fract = (fract * 10) + temp;
    }

    temp = temp%10;
    if(0 != temp)
    {
        fract = (fract * 10) + temp;
    }

    sprintf(str, "%d.%d", integ, fract);
}

/*
 * 获取当前应用波段对应的fm驱动波段
 */
int get_radio_band(int app_band)
{
    switch (app_band)
    {


    case BAND_JAPAN:
        return JAPAN;

    case BAND_EUROPE:
        return EURO;

    case BAND_CHINA:
    case BAND_USA:
    default:
        return US;
    }
}

/*
 * 获取应用波段对应的最大最小频率值
 */
void get_band_range(int app_band,unsigned int *max, unsigned int *min)
{
    switch (app_band)
    {

    case BAND_CHINA:
    case BAND_USA:
    case BAND_EUROPE:
        {
            *max = 108000;
            *min = 87500;
        }
        break;

    case BAND_JAPAN:
        {
            *max = 90000;
            *min = 76000;
        }
        break;
    default:
        {
            *max = 108000;
            *min = 87500;
        }
        break;

    }
}

/*
 * 获取应用波段的步长
 */
unsigned int get_band_step(int app_band)
{
    int ret = 100;

    switch (app_band)
    {

    case BAND_CHINA:
    case BAND_USA:
    case BAND_JAPAN:
        ret = 100;
        break;

    case BAND_EUROPE:
        ret = 50;
        break;
    default:
        ret = 100;
        break;
    }

    return ret;
}

#if 0
/*
 * 获取频道表中频道的个数
 */
int get_channel_num(const int *table)
{
    int i, count;

    count = 0;
    for(i = 0; i < MAX_CHANNEL_NUM; i++)
    {
        if(table[i] != 0)
            {count ++;}
    }

    return count;
}
#endif
/*
 * 获取下一个频道索引
 */
int get_next_channel_index(const int *table, int cur_index)
{
    int i, ret = -1;

    for(i = cur_index + 1; i < MAX_CHANNEL_NUM; i++)
    {
        if(table[i] != 0)
            {break;}
    }

    if(i >= MAX_CHANNEL_NUM)
    {
        for(i = 0; i <= cur_index; i++)
        {
            if(table[i] != 0)
                {break;}
        }
        if(i <= cur_index)
            {ret = i;}
    }
    else
    {
        ret = i;
    }

    return ret;
}

/*
 * 获取前一个频道索引
 */
int get_prev_channel_index(const int *table, int cur_index)
{
    int i, ret = -1;

    for(i = cur_index - 1; i >= 0; i--)
    {
        if(table[i] != 0)
            {break;}
    }

    if(i < 0)
    {
        for(i = MAX_CHANNEL_NUM - 1; i >= cur_index ; i--)
        {
            if(table[i] != 0)
                {break;}
        }
        if(i >= cur_index)
            {ret = i;}
    }
    else
    {
        ret = i;
    }

    return ret;
}





