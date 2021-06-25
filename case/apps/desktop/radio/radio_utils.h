/********************************************************************************
 *                              USDK(1100)
 *                             Module: app
 *                 Copyright(c) 2003-2009 Actions Semiconductor,
 *                            All Rights Reserved.
 * File : radio_utils.h
 * History:
 *      <author>    <time>           <version >             <desc>
 *      cheason    2009-04-23          v1.0               build this file
 ********************************************************************************/
/*!
 * \file     radio_utils.h
 * \brief    FM应用通用函数
 * \author   cheason
 *
 * \version 1.0
 * \date  2009/04/23
 *******************************************************************************/

#ifndef __RADIO_UTILS_H__
#define __RADIO_UTILS_H__


void freq_to_str(char *str, unsigned int freq);
int get_radio_band(int app_band);
void get_band_range(int app_band, unsigned int *max, unsigned int *min);
unsigned int get_band_step(int app_band);

int get_next_channel_index(const int *table, int cur_index);
int get_prev_channel_index(const int *table, int cur_index);

#endif /* __RADIO_UTILS_H__ */

