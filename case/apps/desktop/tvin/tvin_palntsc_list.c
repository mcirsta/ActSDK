/********************************************************************************
*                              USDK(1100)
*                             Module: tvin
*                 Copyright(c) 2003-2009 Actions Semiconductor,
*                            All Rights Reserved.
* File : tvin_palntsc_list.c
* History:
*      <author>    <time>           <version >             <desc>
*      Jerry Fu    2009-4-28             v1.0               build this file
********************************************************************************/
/*!
* \file     tvin_palntsc_list.c
* \brief    选择PAL或者NTSC制式的list
* \author   Jerry Fu
*
* \version 1.0
* \date  2009/04/28
*******************************************************************************/
#include "tvin.h"
#include "tvin_palntsc_list.h"

/* menu max items */
#define  TVIN_PALNTSC_LIST_MAX_ITEMS   2

static menu_t *palntsc_menu = NULL;

unsigned  int  tvin_list_textid[TVIN_PALNTSC_LIST_MAX_ITEMS] =\
{
    STR_TV_PAL,
    STR_TV_NTSC
};



static int _list_select_tv_signal( menuview_t *menuview, menuview_sig_e signal );

static menu_t* _tvin_palntsc_list_init(void);


static void _set_tv_signal_cfg(tvin_signal_mode_e signal_mode)
{
    switch(signal_mode)
    {
    case TV_SIGNAL_PAL:
        sprintf(g_tvin_cfgs[TVIN_CFG_TVIN_SIGNAL].text, "PAL");
        g_tvin_cfgs[TVIN_CFG_TVIN_SIGNAL].changed = true;
        tvin_cfg_write(&g_tvin_cfgs[TVIN_CFG_TVIN_SIGNAL]);
        //设置TVIN输入信号的制式，启动预览功能
        set_tvin_mode(ZONE_PALD);
        break;

    case TV_SIGNAL_NTSC:
        sprintf(g_tvin_cfgs[TVIN_CFG_TVIN_SIGNAL].text, "NTSC");
        g_tvin_cfgs[TVIN_CFG_TVIN_SIGNAL].changed = true;
        tvin_cfg_write(&g_tvin_cfgs[TVIN_CFG_TVIN_SIGNAL]);
        set_tvin_mode(ZONE_NTSC);
        break;   

    default:
        sprintf(g_tvin_cfgs[TVIN_CFG_TVIN_SIGNAL].text, "PAL");
        g_tvin_cfgs[TVIN_CFG_TVIN_SIGNAL].changed = true;
        tvin_cfg_write(&g_tvin_cfgs[TVIN_CFG_TVIN_SIGNAL]);
        //设置TVIN输入信号的制式，启动预览功能
        set_tvin_mode(ZONE_PALD);
        break;    
    }
}


static int _list_select_tv_signal( menuview_t *menuview, menuview_sig_e signal )
{
    picture_resource_t *pic;

    int menu_index;
    tvin_scene_palntsc_t *palntsc;
    menu_t *cur_menu = NULL;
    menuitem_t *temp_item;

    cur_menu = menuview->menu;
    if(NULL == cur_menu)
    {
        print_err("can't get current menu of menuview\n");
        return -1;
    }
    
    palntsc = fetch_tvin_palntsc();
    if(NULL == palntsc)
    {
        return -1;
    }
    
    pic = (picture_resource_t *)get_scene_child(palntsc->scene, PIC_TVIN_LIST_DOT);
    if(NULL == pic)
    {
        return -1;
    }
    
    switch ( signal )
    {
    case WIDGET_SIG_MENUVIEW_UP:
    case WIDGET_SIG_MENUVIEW_DOWN:
        menu_index = menu_get_cur_index(cur_menu);
        temp_item = menu_get_cur_item(cur_menu);
        if(NULL == temp_item)
        {
            return -1;
        }
        
        switch(menu_index)
        {
        case 0:
            temp_item->sel_icon_id = (int)(pic->pic_id);
            temp_item->unsel_icon_id = (int)(pic->pic_id);
            
            temp_item = menu_get_item(cur_menu,1);
            if(NULL == temp_item)
            {
                break;
            }
            
            temp_item->sel_icon_id = RES_NULL;
            temp_item->unsel_icon_id = RES_NULL;
            _set_tv_signal_cfg(TV_SIGNAL_PAL);
            break;

        case 1:
            temp_item->sel_icon_id = (int)(pic->pic_id);
            temp_item->unsel_icon_id = (int)(pic->pic_id);
            
            temp_item = menu_get_item(cur_menu,0);
            if(NULL == temp_item)
            {
                break;
            }
            
            temp_item->sel_icon_id = RES_NULL;
            temp_item->unsel_icon_id = RES_NULL;
            _set_tv_signal_cfg(TV_SIGNAL_NTSC);
            break;
            
        default:
            break;
        }
        
        palntsc->tvin_signal_palntsc = menu_index;                               
        menuview_set_menu(menuview, cur_menu);
        menuview_paint(menuview);
        break;
                                   
    case WIDGET_SIG_MENUVIEW_QUIT:
        break;
        
    default:
        break;
    }
    return 1;
}



static menu_t* _tvin_palntsc_list_init(void)
{
    scene_t* scene;
    menu_t *pmenu;
    tvin_scene_palntsc_t *palntsc;
    string_resource_t *str_res;
    picture_resource_t *pic_res;
    menuitem_t item[TVIN_PALNTSC_LIST_MAX_ITEMS];
    int num;
    pmenu = palntsc_menu;
    palntsc = fetch_tvin_palntsc();
    if(NULL == palntsc)
    {
        return NULL;
    }

    pic_res = (picture_resource_t *)get_scene_child(palntsc->scene, PIC_TVIN_LIST_DOT);
    if(NULL == pic_res)
    {
        print_err("get picture resource error");
        return NULL;
    }

    memset(item, 0, TVIN_PALNTSC_LIST_MAX_ITEMS*sizeof(menuitem_t));
    //initial menu
    if( NULL == pmenu)
    {
        //get scene
        scene = palntsc->scene;
        if ( scene == NULL )
        {
            print_err("cann't find app scene!");
            return false;
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
            for(num=0; num<TVIN_PALNTSC_LIST_MAX_ITEMS; num++)
            {             
                str_res = (string_resource_t *)get_resgroup_child(palntsc->group, tvin_list_textid[num]);
                if(NULL == str_res)
                {
                    return NULL;
                }

                item[num].text_id = (int)(str_res->str_id);                     
                item[num].callback = (item_cb_t)_list_select_tv_signal;
                if(palntsc->tvin_signal_palntsc == num)
                {
                    item[num].sel_icon_id = (int)(pic_res->pic_id);
                    item[num].unsel_icon_id = (int)(pic_res->pic_id);
                }
            }                          
        }
        menu_append_items(pmenu,item,TVIN_PALNTSC_LIST_MAX_ITEMS);
        menu_set_cur_index( pmenu, palntsc->tvin_signal_palntsc);
    }    
    return pmenu;
}


/* 进入主菜单 */
bool tvin_palntsc_list_enter( menuview_t *menuview )
{	
    if(NULL == palntsc_menu)
    {
        palntsc_menu = _tvin_palntsc_list_init();
        if(NULL == palntsc_menu)
        {
            return false;
        }
    }
    menuview_set_menu(menuview, palntsc_menu);
    menuview_paint( menuview );	
    return true;
}

bool tvin_palntsc_list_exit(menuview_t *menuview)
{
    bool result;
    tvin_widget_palntsc_list_delete(menuview);
    result = tvin_palntsc_list_deinit();
    return result;
}


bool tvin_palntsc_list_deinit(void)
{
    if(NULL != palntsc_menu)
    {
        menu_delete(palntsc_menu); 
        palntsc_menu = NULL;
    }
    return true;
}



