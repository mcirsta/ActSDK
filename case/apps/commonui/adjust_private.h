/********************************************************************************
*                              USDK(1100)
*                             Module: head file
*                 Copyright(c) 2003-2009 Actions Semiconductor,
*                            All Rights Reserved.
* File : adjust_control.h
* History:
*      <author>    <time>           <version >                      <desc>
*     fubingrui    2009-4-22          v1.0               define private data used by adjust widget
********************************************************************************/
/*!
* \file     adjust_private.h
* \brief    adjust widget
* \author   fubingrui
*
* \version 1.0
* \date  2009/04/22
*******************************************************************************/
#ifndef      __ADJUST_PRIVATE_H__
#define      __ADJUST_PRIVATE_H__

#include "include_case.h"


#define   TIME_ITEM_MAX                         (3)
#define   DATE_ITEM_MAX                         (3)

typedef enum
{
    KEY_RELEASED,
    KEY_PRESSED,    
}adj_key_sta_e;

typedef  enum
{
    TIME_AM,
    TIME_PM,
}time_ampm_e;


typedef  struct
{
    int  time_index;    
    int  time_digit_info[TIME_ITEM_MAX];  /* –° ±£¨∑÷÷” , 0:AM 1:PM*/        
}time_para_t;


typedef  struct
{
    int  date_index;    
    int  date_digit_info[DATE_ITEM_MAX];      
}date_para_t;


typedef  struct
{
    adj_key_sta_e  key_up_status;
    adj_key_sta_e  key_down_status;
    adj_key_sta_e  key_menu_status;
}key_status_t;


typedef  struct
{
    string_resource_t * str_adjust_text;
    bool show_text;
}adjust_private_textdigit_t;

typedef  struct
{
    picture_resource_t *pic_switch_on_bg;
    picture_resource_t *pic_switch_off_bg;
    string_resource_t *str_text_on;
    string_resource_t *str_text_off;

    bool show_switchon_bg;
    bool show_switchoff_bg;
    bool show_texton;
    bool show_textoff;
}adjust_private_switch_t;


typedef  struct
{    
    picregion_resource_t *picregion_hour_high;
    picregion_resource_t *picregion_hour_low;
    picregion_resource_t *picregion_min_high;
    picregion_resource_t *picregion_min_low;
    picregion_resource_t *picregion_ampm;
    picture_resource_t *pic_time_sep;
   
    bool  show_hour;
    bool  show_min;
    bool  show_ampm;
    bool  show_time_sep;

    time_para_t  time_para;
}adjust_pritvate_time_t;

typedef  struct
{    
    picregion_resource_t *picregion_left_arrow;
    picregion_resource_t *picregion_right_arrow;
    
    picregion_resource_t *picregion_year4;
    picregion_resource_t *picregion_year3;
    picregion_resource_t *picregion_year2;
    picregion_resource_t *picregion_year1;

    picregion_resource_t *picregion_month_high;
    picregion_resource_t *picregion_month_low;

    picregion_resource_t *picregion_day_high;
    picregion_resource_t *picregion_day_low;

    picture_resource_t *pic_date_sep;
    picture_resource_t *pic_date_sep1;

    bool show_left_arrow;
    bool show_right_arrow;
    bool show_year;    
    bool show_month;
    bool show_day;
    bool show_date_sep;
    bool show_date_sep1;

    date_para_t  date_para;
}adjust_private_date_t;


#endif




