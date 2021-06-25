/********************************************************************************
 *                              USDK(1100)
 *                             Module: app 
 *                 Copyright(c) 2003-2009 Actions Semiconductor,
 *                            All Rights Reserved. 
 * File : headerbar.c
 * History:         
 *      <author>    <time>           <version >             <desc>
 *       fubingrui   2009-4-09          v1.0               build this file 
 ********************************************************************************/
/*!
* \file     adjust_ui.c
* \brief    adjust widget
* \author   fubingrui
*               
* \version 1.0
* \date  2009/04/09
*******************************************************************************/
#include "adjust_private.h"

struct adjust_private_s
{  
    /* record key status  */    
    key_status_t   adj_key_sta; 
    
    string_resource_t *str_adjust_title;
    int str_title_id;
        
    picregion_resource_t  *picregion_adjust_bg;
    picregion_resource_t *picregion_left_arrow;
    picregion_resource_t *picregion_right_arrow;
    picture_resource_t *pic_disp_bg;
    
    adjust_private_textdigit_t  text_digit;
    adjust_private_switch_t   adjust_onoff;
    adjust_pritvate_time_t  adjust_time;
    adjust_private_date_t   adjust_date;

    bool show_bg;
    bool show_title;
    bool show_left_arrow;
    bool show_right_arrow;
    bool show_disp_bg;
};

static  int   draw_pic_by_framenum(int hdc, picregion_resource_t *picregion,int frame_number)
{
    picregion_frame_t  picframe;
    int x;
    int y;
    int  pic_id;
    get_picregion_frame(picregion, (unsigned short)frame_number, &picframe);
    
    x = picframe.x;
    y = picframe.y;
    pic_id = (int)picframe.pic_id;
    sty_draw_picture_id(hdc, pic_id, x, y);
    return 0;
}

static  void  draw_bg(adjust_t *pAdjust)
{
    picregion_resource_t *picregion;
    
    if(pAdjust->pAdjPrivate->show_bg == true)
    {
        return;
    }

    picregion = pAdjust->pAdjPrivate->picregion_adjust_bg;
    draw_pic_by_framenum(pAdjust->hdc, picregion, 0);
    pAdjust->pAdjPrivate->show_bg = true;   
}

static void draw_title(adjust_t *pAdjust)
{
    char *pChar;    
    if(pAdjust->pAdjPrivate->show_title == true)
    {
        return;
    }    
    pChar = (char *)sty_get_string(pAdjust->pAdjPrivate->str_title_id);
    sty_draw_string_res_ext(pAdjust->hdc, pAdjust->pAdjPrivate->str_adjust_title, pChar);
    pAdjust->pAdjPrivate->show_title = true;
}


static void  draw_leftarrow(adjust_t *pAdjust)
{
    picregion_resource_t *picregion;
    int layer;
    
    if(pAdjust->pAdjPrivate->show_left_arrow ==true)
    {
        return;
    }

    if(pAdjust->pAdjPrivate->adj_key_sta.key_down_status == KEY_PRESSED)
    {
        layer = 1;  
    }
    else
    {
        layer = 0;
    }
    
    picregion = pAdjust->pAdjPrivate->picregion_left_arrow;
    draw_pic_by_framenum(pAdjust->hdc, picregion, layer);
    pAdjust->pAdjPrivate->show_left_arrow = true;

}


static void draw_rightarrow(adjust_t *pAdjust)
{
    picregion_resource_t *picregion;
    int layer;
    
    if(pAdjust->pAdjPrivate->show_right_arrow ==true)
    {
        return;
    }

    if(pAdjust->pAdjPrivate->adj_key_sta.key_up_status == KEY_PRESSED)
    {
        layer = 1;  
    }
    else
    {
        layer = 0;
    }

    picregion = pAdjust->pAdjPrivate->picregion_right_arrow;
    draw_pic_by_framenum(pAdjust->hdc, picregion, layer);
    pAdjust->pAdjPrivate->show_right_arrow = true;

}

static  void  draw_dispbg(adjust_t *pAdjust)
{
    picture_resource_t *pic;

    if(pAdjust->pAdjPrivate->show_disp_bg ==true)
    {
        return;
    }
    pic = pAdjust->pAdjPrivate->pic_disp_bg;
    sty_draw_picture_res(pAdjust->hdc, pic);
    pAdjust->pAdjPrivate->show_disp_bg = true;
}

static void  draw_switchon_bg(adjust_t *pAdjust)
{
    if(pAdjust->pAdjPrivate->adjust_onoff.show_switchon_bg ==true)
    {
        return;
    }

    sty_draw_picture_res(pAdjust->hdc, pAdjust->pAdjPrivate->adjust_onoff.pic_switch_on_bg);
    pAdjust->pAdjPrivate->adjust_onoff.show_switchon_bg = true;
}

static void  draw_switchoff_bg(adjust_t *pAdjust)
{
    if(pAdjust->pAdjPrivate->adjust_onoff.show_switchoff_bg ==true)
    {
        return;
    }

    sty_draw_picture_res(pAdjust->hdc, pAdjust->pAdjPrivate->adjust_onoff.pic_switch_off_bg);
    pAdjust->pAdjPrivate->adjust_onoff.show_switchoff_bg = true;
}

static void  draw_switchon_text(adjust_t *pAdjust)
{
    if(pAdjust->pAdjPrivate->adjust_onoff.show_texton == true)
    {
        return;
    }

    sty_draw_string_res(pAdjust->hdc, pAdjust->pAdjPrivate->adjust_onoff.str_text_on);
    pAdjust->pAdjPrivate->adjust_onoff.show_texton = true;
}

static void  draw_switchoff_text(adjust_t *pAdjust)
{
    if(pAdjust->pAdjPrivate->adjust_onoff.show_textoff == true)
    {
        return;
    }

    sty_draw_string_res(pAdjust->hdc, pAdjust->pAdjPrivate->adjust_onoff.str_text_off);
    pAdjust->pAdjPrivate->adjust_onoff.show_textoff = true;
}



static void  draw_textdigit_string(adjust_t *pAdjust)
{
    char buf[32];
    char *pUnit = NULL;
    int str_id;
    
    adjust_textdigit_init_t *textdigit_para;
    textdigit_para = &(pAdjust->adjust_digit);    
    
    if(pAdjust->pAdjPrivate->text_digit.show_text ==true)
    {
        return;
    }

    /* only display digit info */
    if( (textdigit_para->str_disptext_id[0] == 0) && ((textdigit_para->str_disp[0]) == NULL) )
    {
        if((textdigit_para->cur_val) == (textdigit_para->min_val))
        {            
            if(textdigit_para->str_min_id != 0)
            {
                pUnit = (char *)sty_get_string(textdigit_para->str_min_id);
                sty_draw_string_res_ext(pAdjust->hdc, pAdjust->pAdjPrivate->text_digit.str_adjust_text, pUnit);
                return;
            }            
        }

        if((textdigit_para->cur_val) == (textdigit_para->max_val))
        {
            if(textdigit_para->str_max_id != 0)
            {
                pUnit = (char *)sty_get_string(textdigit_para->str_max_id);
                sty_draw_string_res_ext(pAdjust->hdc, pAdjust->pAdjPrivate->text_digit.str_adjust_text, pUnit);
                return;
            }          
        }

        sprintf(buf, "%d", textdigit_para->cur_val);
        if(textdigit_para->str_unit[0] != 0)
        {
            pUnit = textdigit_para->str_unit;
        }
        else
        {
            if(textdigit_para->str_unit_id > 0)
            {
                pUnit = (char *)sty_get_string(textdigit_para->str_unit_id);
            }
        }        

        if(pUnit != NULL)
        {
            strcat(buf,"  ");
            strcat(buf, pUnit);
        }

        sty_draw_string_res_ext(pAdjust->hdc, pAdjust->pAdjPrivate->text_digit.str_adjust_text, buf);
    }
    else
    {
        if(textdigit_para->str_disp[textdigit_para->cur_val] != NULL)
        {
            pUnit = textdigit_para->str_disp[textdigit_para->cur_val];
        }
        else
        {
            str_id = textdigit_para->str_disptext_id[textdigit_para->cur_val];
            pUnit = (char *)sty_get_string(str_id);
        }        

        if(pUnit != NULL)
        {
            sty_draw_string_res_ext(pAdjust->hdc, pAdjust->pAdjPrivate->text_digit.str_adjust_text, pUnit);
        }
    }
    
    pAdjust->pAdjPrivate->text_digit.show_text = true;
}

static void  draw_hour(adjust_t  *pAdjust)
{
    int layer;

    time_para_t *pTime;
    pTime = &(pAdjust->pAdjPrivate->adjust_time.time_para);
    if (pTime->time_index == 0)
    {
        layer = 2 * ((pTime->time_digit_info[0]) / 10);
        draw_pic_by_framenum(pAdjust->hdc, pAdjust->pAdjPrivate->adjust_time.picregion_hour_high, layer);
        layer = 2 * ((pTime->time_digit_info[0]) % 10);
        draw_pic_by_framenum(pAdjust->hdc, pAdjust->pAdjPrivate->adjust_time.picregion_hour_low, layer);
    }
    else
    {
        layer = 2 * ((pTime->time_digit_info[0]) / 10);
        layer++;
        draw_pic_by_framenum(pAdjust->hdc, pAdjust->pAdjPrivate->adjust_time.picregion_hour_high, layer);
        layer = 2 * ((pTime->time_digit_info[0]) % 10);
        layer++;
        draw_pic_by_framenum(pAdjust->hdc, pAdjust->pAdjPrivate->adjust_time.picregion_hour_low, layer);
    }
}

static void  draw_time_sep(adjust_t  *pAdjust)
{    
    sty_draw_picture_res(pAdjust->hdc, pAdjust->pAdjPrivate->adjust_time.pic_time_sep);
}

static void  draw_minute(adjust_t  *pAdjust)
{
    int layer;

    time_para_t *pTime;

    pTime = &(pAdjust->pAdjPrivate->adjust_time.time_para);

    if (pTime->time_index == 1)
    {
        layer = 2 * ((pTime->time_digit_info[1]) / 10);
        draw_pic_by_framenum(pAdjust->hdc, pAdjust->pAdjPrivate->adjust_time.picregion_min_high, layer);
        layer = 2 * ((pTime->time_digit_info[1]) % 10);
        draw_pic_by_framenum(pAdjust->hdc, pAdjust->pAdjPrivate->adjust_time.picregion_min_low, layer);
    }
    else
    {
        layer = 2 * ((pTime->time_digit_info[1]) / 10);
        layer++;
        draw_pic_by_framenum(pAdjust->hdc, pAdjust->pAdjPrivate->adjust_time.picregion_min_high, layer);
        layer = 2 * ((pTime->time_digit_info[1]) % 10);
        layer++;
        draw_pic_by_framenum(pAdjust->hdc, pAdjust->pAdjPrivate->adjust_time.picregion_min_low, layer);
    }
}

static void draw_ampm(adjust_t *pAdjust)
{
    int layer;
    time_para_t *pTime;
    pTime = &(pAdjust->pAdjPrivate->adjust_time.time_para);
   
    if(pTime->time_index == 2)
    {
        layer = 2*(pTime->time_digit_info[2]) + 1;
                
    }
    else
    {
        layer = 2*(pTime->time_digit_info[2]);
    }
    draw_pic_by_framenum(pAdjust->hdc, pAdjust->pAdjPrivate->adjust_time.picregion_ampm, layer);
}


static void  draw_year(adjust_t *pAdjust)
{
    int layer;
    int year;
    int temp;
     
    date_para_t *pDate;

    pDate = &(pAdjust->pAdjPrivate->adjust_date.date_para);
           
    if(pDate->date_index == 0)
    {
        year = pDate->date_digit_info[0];
        temp = year/1000;
        layer = 2*temp;
        draw_pic_by_framenum(pAdjust->hdc, pAdjust->pAdjPrivate->adjust_date.picregion_year4, layer);

        temp = (year % 1000)/100;
        layer = 2*temp;
        draw_pic_by_framenum(pAdjust->hdc, pAdjust->pAdjPrivate->adjust_date.picregion_year3, layer);

        temp = (year % 100)/10;
        layer = 2*temp;
        draw_pic_by_framenum(pAdjust->hdc, pAdjust->pAdjPrivate->adjust_date.picregion_year2, layer);

        temp = year%10;
        layer = 2*temp;
        draw_pic_by_framenum(pAdjust->hdc, pAdjust->pAdjPrivate->adjust_date.picregion_year1, layer);        
    }
    else
    {
        year = pDate->date_digit_info[0];
        temp = year/1000;
        layer = (2*temp) + 1;
        draw_pic_by_framenum(pAdjust->hdc, pAdjust->pAdjPrivate->adjust_date.picregion_year4, layer);

        temp = (year % 1000)/100;
        layer = (2*temp) + 1;
        draw_pic_by_framenum(pAdjust->hdc, pAdjust->pAdjPrivate->adjust_date.picregion_year3, layer);

        temp = (year % 100)/10;
        layer = (2*temp) + 1;
        draw_pic_by_framenum(pAdjust->hdc, pAdjust->pAdjPrivate->adjust_date.picregion_year2, layer);

        temp = year%10;
        layer = (2*temp) + 1;
        draw_pic_by_framenum(pAdjust->hdc, pAdjust->pAdjPrivate->adjust_date.picregion_year1, layer);
    }    
}


static void  draw_month(adjust_t  *pAdjust)
{
    int layer;
    date_para_t *pDate;

    pDate = &(pAdjust->pAdjPrivate->adjust_date.date_para);
    if (pDate->date_index == 1)
    {
        layer = 2 * ((pDate->date_digit_info[1]) / 10);
        draw_pic_by_framenum(pAdjust->hdc, pAdjust->pAdjPrivate->adjust_date.picregion_month_high, layer);
        layer = 2 * ((pDate->date_digit_info[1]) % 10);
        draw_pic_by_framenum(pAdjust->hdc, pAdjust->pAdjPrivate->adjust_date.picregion_month_low, layer);
    }
    else
    {
        layer = 2 * ((pDate->date_digit_info[1]) / 10);
        layer++;
        draw_pic_by_framenum(pAdjust->hdc, pAdjust->pAdjPrivate->adjust_date.picregion_month_high, layer);
        layer = 2 * ((pDate->date_digit_info[1]) % 10);
        layer++;
        draw_pic_by_framenum(pAdjust->hdc, pAdjust->pAdjPrivate->adjust_date.picregion_month_low, layer);
    }
}

static void  draw_day(adjust_t  *pAdjust)
{
    int layer;
    date_para_t *pDate;

    pDate = &(pAdjust->pAdjPrivate->adjust_date.date_para);

    if (pDate->date_index == 2)
    {
        layer = 2 * ((pDate->date_digit_info[2]) / 10);
        draw_pic_by_framenum(pAdjust->hdc, pAdjust->pAdjPrivate->adjust_date.picregion_day_high, layer);
        layer = 2 * ((pDate->date_digit_info[2]) % 10);
        draw_pic_by_framenum(pAdjust->hdc, pAdjust->pAdjPrivate->adjust_date.picregion_day_low, layer);
    }
    else
    {
        layer = 2 * ((pDate->date_digit_info[2]) / 10);
        layer++;
        draw_pic_by_framenum(pAdjust->hdc, pAdjust->pAdjPrivate->adjust_date.picregion_day_high, layer);
        layer = 2 * ((pDate->date_digit_info[2]) % 10);
        layer++;
        draw_pic_by_framenum(pAdjust->hdc, pAdjust->pAdjPrivate->adjust_date.picregion_day_low, layer);
    }
}


static void  draw_date_sep(adjust_t *pAdjust)
{
    sty_draw_picture_res(pAdjust->hdc, pAdjust->pAdjPrivate->adjust_date.pic_date_sep);
}

static void  draw_date_sep1(adjust_t *pAdjust)
{    
    sty_draw_picture_res(pAdjust->hdc, pAdjust->pAdjPrivate->adjust_date.pic_date_sep1);
}


static void   draw_textdigit(adjust_t *pAdjust)
{
    draw_bg(pAdjust);
    draw_title(pAdjust);
    draw_leftarrow(pAdjust);
    draw_rightarrow(pAdjust);
    
    draw_dispbg(pAdjust);
    draw_textdigit_string(pAdjust);
}

static bool draw_switch(adjust_t *pAdjust)
{
    draw_bg(pAdjust);
    draw_title(pAdjust);
    draw_leftarrow(pAdjust);
    draw_rightarrow(pAdjust);
    draw_dispbg(pAdjust);
    draw_switchon_bg(pAdjust);
    draw_switchoff_bg(pAdjust);
    draw_switchon_text(pAdjust);
    draw_switchoff_text(pAdjust);
    return true;
}

static bool draw_time(adjust_t *pAdjust)
{
    pAdjust->pAdjPrivate->show_bg = false;
    pAdjust->pAdjPrivate->show_title = false;
    pAdjust->pAdjPrivate->show_left_arrow = false;
    pAdjust->pAdjPrivate->show_right_arrow = false;

    canvas_erase_all(pAdjust->canvas_id);
    draw_bg(pAdjust);
    draw_title(pAdjust);

    draw_leftarrow(pAdjust);
    draw_rightarrow(pAdjust);

    draw_hour(pAdjust);
    draw_time_sep(pAdjust);
    draw_minute(pAdjust);
    if(pAdjust->time_mode == TIME_MODE_12)
    {
        draw_ampm(pAdjust);
    }    
    return true;
}

static bool draw_date(adjust_t *pAdjust)
{    
    pAdjust->pAdjPrivate->show_bg = false;
    pAdjust->pAdjPrivate->show_title = false;
    pAdjust->pAdjPrivate->show_left_arrow = false;
    pAdjust->pAdjPrivate->show_right_arrow = false;

    canvas_erase_all(pAdjust->canvas_id);
    draw_bg(pAdjust);    
    draw_title(pAdjust);        
    draw_leftarrow(pAdjust);
    draw_rightarrow(pAdjust);

    draw_year(pAdjust);
    draw_date_sep(pAdjust);
    draw_month(pAdjust);
    draw_date_sep1(pAdjust);
    draw_day(pAdjust);
    return true;
}

static bool draw_record_len(adjust_t *pAdjust)
{
    pAdjust->pAdjPrivate->show_bg = false;
    pAdjust->pAdjPrivate->show_title = false;
    pAdjust->pAdjPrivate->show_left_arrow = false;
    pAdjust->pAdjPrivate->show_right_arrow = false;
    canvas_erase_all(pAdjust->canvas_id);
    draw_bg(pAdjust);
    draw_title(pAdjust);

    draw_leftarrow(pAdjust);
    draw_rightarrow(pAdjust);

    draw_hour(pAdjust);
    draw_time_sep(pAdjust);
    draw_minute(pAdjust);
 
    return true;
}

bool  adjust_paint(adjust_t *pAdjust)
{
    if(pAdjust->hdc == 0)
    {
        pAdjust->hdc = gui_dc_get(pAdjust->canvas_id);
        if( pAdjust->hdc == 0 )
        {   
            return false;  
        }
    }
    
    switch(pAdjust->adjust_style)
    {
    case ADJ_STYLE_TEXTDIGIT:
        draw_textdigit(pAdjust);
        break;

    case ADJ_STYLE_SWITCH:
        draw_switch(pAdjust);
        break;

    case ADJ_STYLE_TIME:
        draw_time(pAdjust);
        break;

    case ADJ_STYLE_DATE:
        draw_date(pAdjust);
        break;

    case ADJ_STYLE_RECLEN:
        draw_record_len(pAdjust);
        break;
    default:
        break;
    }
    return true;
}
    
    
    

