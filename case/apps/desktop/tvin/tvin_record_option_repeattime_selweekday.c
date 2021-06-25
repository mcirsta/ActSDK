/********************************************************************************
 *                              USDK(1100)
 *                             Module: tvin
 *                 Copyright(c) 2003-2009 Actions Semiconductor,
 *                            All Rights Reserved.
 * File : tvin_record_option_repeattime_selweekday.c
 * History:
 *      <author>    <time>           <version >             <desc>
 *      Jerry Fu    2009-4-28             v1.0               build this file
 ********************************************************************************/
/*!
* \file     tvin_record_option_repeattime_selweekday.c
* \brief    define data structure of weekday menu and delclare funtions
* \author   Jerry Fu
*
* \version 1.0
* \date  2009/04/28
*******************************************************************************/
#include "tvin.h"

/* menu max items */
#define  TVIN_OPTION_WEEKDAY_MAX_ITEMS   (7)

static tvin_scene_record_t *s_tvin_record;
static menu_t  *pMenuWeekday = NULL;
static menu_t  *upper_level_menu;
static int upper_level_index;
static int repeat_time_index;
static int save_weekday;

static bool  weekday_is_changed = false;


static unsigned  int  tvin_option_weekday_textid[TVIN_OPTION_WEEKDAY_MAX_ITEMS] =
{    
    STR_SUNDAY,
    STR_MONDAY,
    STR_TUESDAY,
    STR_WEDNESDAY,
    STR_THURSDAY,
    STR_FRIDAY,
    STR_SATURDAY,
};


static bool _option_repeatrec_weekday( menuview_t *menuview, menuview_sig_e signal );
static  menu_t* _option_repeatrec_weekday_init(menuview_t *menuview);
static bool _option_repeatrec_weekday_exit( menuview_t *menuview );


static bool _option_repeatrec_weekday( menuview_t *menuview, menuview_sig_e signal )
{
    int cur_index;
    picture_resource_t *pic;
    menu_t *cur_menu = NULL;
    menuitem_t *cur_item = NULL;
   
    cur_menu = menuview->menu;
    if(NULL == cur_menu)
    {
        return false;
    }

    cur_index = menu_get_cur_index(cur_menu);

    cur_item = menu_get_cur_item( cur_menu );    
    if(NULL == cur_item)
    {
        return false;
    }
    
    pic = get_scene_child(s_tvin_record->scene, PIC_TVIN_OPTION_DOT);
    if(NULL == pic)
    {
        print_err("get picture resource error\n");
        return false;
    }

    switch ( signal )
    {
    case WIDGET_SIG_MENUVIEW_OK:       
        if(cur_item->sel_icon_id != RES_NULL)
        {
            cur_item->sel_icon_id = RES_NULL;
            cur_item->unsel_icon_id = RES_NULL;
            s_tvin_record->tvin_config.repeat_record[repeat_time_index].repeat_day_flag &= ~(1<<cur_index);
        }
        else
        {
            cur_item->sel_icon_id = (int)(pic->pic_id);
            cur_item->unsel_icon_id = (int)(pic->pic_id);
            s_tvin_record->tvin_config.repeat_record[repeat_time_index].repeat_day_flag |= (1<<cur_index);
        }
        menuview_set_menu(menuview, cur_menu);
        menuview_paint(menuview);
        break;
        
    case WIDGET_SIG_MENUVIEW_QUIT:
        // return to parent menu
        if(save_weekday != (s_tvin_record->tvin_config.repeat_record[repeat_time_index].repeat_day_flag))
        {
            g_tvin_cfgs[TVIN_CFG_REPEAT1_WEEKDAY + repeat_time_index].value = \
                (int)(s_tvin_record->tvin_config.repeat_record[repeat_time_index].repeat_day_flag);    
            g_tvin_cfgs[TVIN_CFG_REPEAT1_WEEKDAY + repeat_time_index].changed = true;
            tvin_cfg_write(&g_tvin_cfgs[TVIN_CFG_REPEAT1_WEEKDAY]);
            weekday_is_changed = true;
            set_weekday_status(weekday_is_changed);
            weekday_is_changed = false;
        }                
        _option_repeatrec_weekday_exit(menuview);
        break;
        
    default:
        break;
    }
    return true;
}


/* 退出存储路径选择菜单 */
static bool _option_repeatrec_weekday_exit( menuview_t *menuview )
{    
    //退回到上级菜单
    menuview_set_menu(menuview, upper_level_menu);
    menuview_paint(menuview);
    tvin_option_repeatrec_weekday_deinit();
    return true;
}


static  menu_t* _option_repeatrec_weekday_init(menuview_t *menuview)
{
    scene_t* scene;
    menu_t *pMenu;
    unsigned char repeat_weekday_flag;
    tvin_scene_record_t *pRecord;
    string_resource_t *str_res;
    picture_resource_t *pic;
    menuitem_t item[TVIN_OPTION_WEEKDAY_MAX_ITEMS];
    int num;    
    pMenu = pMenuWeekday;

    upper_level_menu = menuview->menu;
    if(NULL == upper_level_menu)
    {
        return NULL;
    }
    
    upper_level_index = menu_get_cur_index(upper_level_menu);
    
    s_tvin_record = fetch_tvin_record();
    if(NULL == s_tvin_record)
    {
        return NULL;
    }
    
    pRecord = s_tvin_record;
    memset(item, 0, TVIN_OPTION_WEEKDAY_MAX_ITEMS*sizeof(menuitem_t));
    save_weekday = (int)(pRecord->tvin_config.repeat_record[repeat_time_index].repeat_day_flag);
    //initial menu
    if( NULL == pMenu)
    {
        //get scene
        scene = pRecord->scene;
        if ( scene == NULL )
        {
            print_err("cann't find app scene!");
            return false;
        }
                
        pic = get_scene_child(s_tvin_record->scene, PIC_TVIN_OPTION_DOT);
        if(NULL == pic)
        {
            print_err("get picture resource error\n");
            return NULL;
        }

        //create menu
        pMenu = menu_create();
        if ( pMenu == NULL )
        {
            print_err("cann't create menu!");
            return false;
        }
        else
        {    
            for(num=0; num<TVIN_OPTION_WEEKDAY_MAX_ITEMS; num++)
            {
                item[num].unsel_icon_id = RES_NULL;

                repeat_weekday_flag = s_tvin_record->tvin_config.repeat_record[repeat_time_index].repeat_day_flag;
                if(0 != (repeat_weekday_flag & (1<<num)))
                {
                    item[num].sel_icon_id = (int)(pic->pic_id);
                    item[num].unsel_icon_id = (int)(pic->pic_id);
                }
                else
                {
                    item[num].sel_icon_id = RES_NULL;
                    item[num].unsel_icon_id = RES_NULL;
                }
                
                str_res = (string_resource_t *)get_resgroup_child(pRecord->group,tvin_option_weekday_textid[num]);
                if(str_res != NULL)
                {
                    item[num].text_id = (int)(str_res->str_id);
                }
                item[num].has_child = false;                
                item[num].callback = (item_cb_t)_option_repeatrec_weekday;
            }                                                
        }        
        menu_append_items(pMenu,item, TVIN_OPTION_WEEKDAY_MAX_ITEMS);
        menu_set_cur_index( pMenu, 0);
    }    
    return pMenu;
}


bool tvin_option_repeatrec_weekday_enter( menuview_t *menuview, int repeat_time_num)
{
    repeat_time_index = repeat_time_num;
    if(pMenuWeekday == NULL)
    {
        pMenuWeekday = _option_repeatrec_weekday_init(menuview);
    }    
    menuview_set_menu(menuview, pMenuWeekday);
    menuview_paint(menuview);
    return true;
}

bool tvin_option_repeatrec_weekday_deinit(void)
{
    if( pMenuWeekday!= NULL )
    {
        menu_delete( pMenuWeekday );
        pMenuWeekday = NULL;
    }    
    return true;
}


