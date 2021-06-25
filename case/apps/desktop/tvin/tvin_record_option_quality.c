/********************************************************************************
 *                              USDK(1100)
 *                             Module: tvin
 *                 Copyright(c) 2003-2009 Actions Semiconductor,
 *                            All Rights Reserved.
 * File : tvin_record_option_quality.c
 * History:
 *      <author>    <time>           <version >             <desc>
 *      Jerry Fu    2009-4-28             v1.0               build this file
 ********************************************************************************/
/*!
* \file     tvin_record_option_quality.c
* \brief    set record quality
* \author   Jerry Fu
*
* \version 1.0
* \date  2009/04/28
*******************************************************************************/
#include "tvin.h"

/* menu max items */
#define  TVIN_OPTION_QUALITY_MAX_ITEMS   (4)

static tvin_scene_record_t *s_ptvin_record;
static menu_t  *s_quality_menu = NULL;
static menu_t  *upper_level_menu;

static unsigned  int  tvin_option_quality_textid[TVIN_OPTION_QUALITY_MAX_ITEMS] =\
{    
    STR_RECORD_QUALITY_LOW,
    STR_RECORD_QUALITY_MEDIUM,
    STR_RECORD_QUALITY_HIGH,
    STR_RECORD_QUALITY_AUTO,
};

static void _set_quality_cfg(int quality);
static bool _option_set_record_quality( menuview_t *menuview, menuview_sig_e signal );
static bool _option_quality_exit( menuview_t *menuview );
static  menu_t* _option_quality_init(menuview_t *menuview);


static void _set_quality_cfg(int quality)
{
    g_tvin_cfgs[TVIN_CFG_RECORD_QUALITY].value = quality;
    g_tvin_cfgs[TVIN_CFG_RECORD_QUALITY].changed = true;
}

static bool _option_set_record_quality( menuview_t *menuview, menuview_sig_e signal )
{
    int cur_index;
    picture_resource_t *pic;
    menu_t *cur_menu = NULL;
    menuitem_t *temp_item;
    mmm_disk_info_t  disk_info;

    cur_menu = menuview->menu;
    if(NULL == cur_menu)
    {
        print_err("can't get current menu of menuview\n");
        return false;
    }
    
    cur_index = menu_get_cur_index(cur_menu);
    pic = get_scene_child(s_ptvin_record->scene, PIC_TVIN_OPTION_DOT);
    if(NULL == pic)
    {
        return false;
    }
 
    switch ( signal )
    {
    case WIDGET_SIG_MENUVIEW_OK:
        temp_item = menu_get_cur_item(cur_menu);
        if(NULL == temp_item)
        {
            return false;
        }

        if(RES_NULL == temp_item->sel_icon_id )
        {
            temp_item->sel_icon_id = (int)(pic->pic_id);
            temp_item->unsel_icon_id = (int)(pic->pic_id);

            temp_item = menu_get_item(cur_menu, s_ptvin_record->tvin_config.record_quality);
            if(NULL == temp_item)
            {
                return false;
            }

            temp_item->sel_icon_id = RES_NULL;
            temp_item->unsel_icon_id = RES_NULL;

            s_ptvin_record->tvin_config.record_quality = cur_index;            
        }
        menuview_set_menu(menuview, cur_menu);
        menuview_paint(menuview);
        break;
        
    case WIDGET_SIG_MENUVIEW_QUIT:
        _set_quality_cfg(s_ptvin_record->tvin_config.record_quality);
        set_record_quality();
        if(true == tvin_get_disk_info(&disk_info))
        {
            s_ptvin_record->total_time = disk_info.info;
            s_ptvin_record->record_remain_time = s_ptvin_record->total_time;
            s_ptvin_record->cur_record_time = 0;
            s_ptvin_record->record_time_is_show = false;
            s_ptvin_record->remain_time_is_show = false;
            s_ptvin_record->pbar_bg_is_show = false;

            s_ptvin_record->saved_x = s_ptvin_record->xstart_pos;

            tvin_scene_record_paint(s_ptvin_record);
            gui_screen_update();        
        }        
        _option_quality_exit(menuview);
        break;
        
    default:
        break;
    }
    return true;
}



/* 退出存储路径选择菜单 */
static bool _option_quality_exit( menuview_t *menuview )
{    
    //退回到上级菜单
    menuview_set_menu(menuview, upper_level_menu);
    menuview_paint(menuview);
    tvin_option_quality_deinit();
    return true;
}



static  menu_t* _option_quality_init(menuview_t *menuview)
{
    scene_t* scene;
    menu_t *pmenu;
    tvin_scene_record_t *precord;
    string_resource_t *str_res;
    picture_resource_t *pic;
    
    menuitem_t item[TVIN_OPTION_QUALITY_MAX_ITEMS];
    int num;
    pmenu = s_quality_menu;

    upper_level_menu = menuview->menu;
    if(NULL == upper_level_menu)
    {
        return false;
    }
        
    precord = fetch_tvin_record();
    if(NULL == precord)
    {
        return NULL;
    }
    
    s_ptvin_record = precord;
    memset(item, 0, TVIN_OPTION_QUALITY_MAX_ITEMS*sizeof(menuitem_t));
    //initial menu
    if( NULL == pmenu)
    {
        //get scene
        scene = precord->scene;
        if ( scene == NULL )
        {
            print_err("cann't find app scene!");
            return false;
        }
                
        pic = (picture_resource_t *)get_scene_child(s_ptvin_record->scene, PIC_TVIN_OPTION_DOT);
        if(NULL == pic)
        {
            print_err("can't get picture resource\n");
            return NULL;
        }

        //create menu
        pmenu = menu_create();
        if ( pmenu == NULL )
        {
            print_err("cann't create menu!");
            return false;
        }
        else
        {    
            for(num=0; num<TVIN_OPTION_QUALITY_MAX_ITEMS; num++)
            {
                item[num].unsel_icon_id = RES_NULL;
                item[num].sel_icon_id = RES_NULL;
                if(s_ptvin_record->tvin_config.record_quality == num)
                {
                    item[num].sel_icon_id = (int)(pic->pic_id);
                    item[num].unsel_icon_id = (int)(pic->pic_id);
                }                                
                str_res = (string_resource_t *)get_resgroup_child(precord->group, tvin_option_quality_textid[num]);
                if(str_res != NULL)
                {
                    item[num].text_id = (int)(str_res->str_id);
                }
                item[num].has_child = false;
                item[num].callback = (item_cb_t)_option_set_record_quality;
            }                                                       
        }        
        menu_append_items(pmenu,item, TVIN_OPTION_QUALITY_MAX_ITEMS);
        menu_set_cur_index( pmenu, s_ptvin_record->tvin_config.record_quality );
    }    
    return pmenu;
}


bool tvin_option_quality_enter( menuview_t *menuview )
{
    if(s_quality_menu == NULL)
    {
        s_quality_menu = _option_quality_init(menuview);
    }
    menuview_set_menu(menuview, s_quality_menu);
    menuview_paint(menuview);
    return true;
}

bool tvin_option_quality_deinit(void)
{
    if( s_quality_menu!= NULL )
    {
        menu_delete( s_quality_menu );
        s_quality_menu = NULL;
    }    
    return true;
}


