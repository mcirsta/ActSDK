/********************************************************************************
 *                              USDK(1100)
 *                             Module: app
 *                 Copyright(c) 2003-2009 Actions Semiconductor,
 *                            All Rights Reserved.
 * File : tvin_record_option_savepath.c
 * History:
 *      <author>    <time>           <version >             <desc>
 *      Jerry Fu    2009-4-28             v1.0               build this file
 ********************************************************************************/
/*!
* \file     tvin_record_option_savepath.c
* \brief    set video file save path
* \author   Jerry Fu
*
* \version 1.0
* \date  2009/04/28
*******************************************************************************/
#include "tvin.h"
/* menu max items */
#define  TVIN_OPTION_SAVEPATH_MAX_ITEMS   (3)

static tvin_scene_record_t *s_tvin_record;
static menu_t  *pMenuPath = NULL;
static menu_t  *upper_level_menu;
static int last_num;

static unsigned  int  tvin_option_savepath_textid[TVIN_OPTION_SAVEPATH_MAX_ITEMS] =\
{    
    STR_INTER_MEM,
    STR_CARD_MEMORY,
    STR_EX_MEMORY,
};

static int valid_path[4] =\
{
    INTERNAL_MEMORY,
    CARD_MEMORY,
    EXTERNAL_MEMORY,
    INVALID_MEMORY,
};

static bool _option_savepath_setmemory( menuview_t *menuview, menuview_sig_e signal );
static bool _option_savepath_exit( menuview_t *menuview );
static  menu_t* _option_savepath_init(menuview_t *menuview);


static void _set_savepath_cfg(int path)
{
    g_tvin_cfgs[TVIN_CFG_SAVEPATH].value = path;
    g_tvin_cfgs[TVIN_CFG_SAVEPATH].changed = true;
    tvin_set_record_path(path);
}

static bool _option_savepath_setmemory( menuview_t *menuview, menuview_sig_e signal )
{
    int cur_index;
    picture_resource_t *pic;
    menu_t *cur_menu = NULL;
    menuitem_t *temp_item;
    mmm_disk_info_t  disk_info;

    cur_menu = menuview->menu;
    if(NULL == cur_menu)
    {
        return false;
    }
    
    cur_index = menu_get_cur_index(cur_menu);
    
    pic = (picture_resource_t *)get_scene_child(s_tvin_record->scene, PIC_TVIN_OPTION_DOT);
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

            temp_item = menu_get_item(cur_menu, last_num);
            if(NULL == temp_item)
            {
                return false;
            }

            temp_item->sel_icon_id = RES_NULL;
            temp_item->unsel_icon_id = RES_NULL;            
            s_tvin_record->tvin_config.save_path = valid_path[cur_index];  
            last_num = cur_index;
        }
        menuview_set_menu(menuview, cur_menu);
        menuview_paint(menuview);
        break;
                
    case WIDGET_SIG_MENUVIEW_QUIT:
        _set_savepath_cfg(s_tvin_record->tvin_config.save_path);
        if(true == tvin_get_disk_info(&disk_info))
        {
            s_tvin_record->total_time = disk_info.info;
            s_tvin_record->record_remain_time = s_tvin_record->total_time;
            s_tvin_record->cur_record_time = 0;
            s_tvin_record->record_time_is_show = false;
            s_tvin_record->remain_time_is_show = false;
            s_tvin_record->pbar_bg_is_show = false;

            s_tvin_record->saved_x = s_tvin_record->xstart_pos;

            tvin_scene_record_paint(s_tvin_record);
            gui_screen_update();        
        }        
        _option_savepath_exit(menuview);
        break;
        
    default:
        break;
    }
    return true;
}



/* 退出存储路径选择菜单 */
static bool _option_savepath_exit( menuview_t *menuview )
{    
    //退回到上级菜单
    menuview_set_menu(menuview, upper_level_menu);
    menuview_paint(menuview);
    tvin_option_savepath_deinit();
    return true;
}


static tvin_save_path_e  _is_main_valid(void)
{
    hotplugin_dev_type_e main_disk;
    main_disk = sys_get_main_disk();
    if(main_disk == HOTPLUGIN_DEV_TYPE_LOCAL)
    {
        /* 主盘是 本地磁盘*/
        //if(NULL == sys_get_main_disk_dir(DIR_TVIN))
        //{
        //    return INVALID_MEMORY;
        //}
        return INTERNAL_MEMORY;
    }
    else if(main_disk == HOTPLUGIN_DEV_TYPE_CARD)
    {
        //if(NULL == sys_get_main_disk_dir(DIR_TVIN))
        //{
        //    return INVALID_MEMORY;
        //}
        return CARD_MEMORY;
    }
    else if(main_disk == HOTPLUGIN_DEV_TYPE_UHOST)
    {
        //if(NULL == sys_get_main_disk_dir(DIR_TVIN))
        //{
        //    return INVALID_MEMORY;
        //}
        return EXTERNAL_MEMORY;
    } 
    else
    {
         /*没有盘*/
        print_err("has no main disk");
        return INVALID_MEMORY;
    }
}

static bool  _is_local_valid(void)
{
    hotplugin_dev_info_t  dev_info;
    if(false == sys_get_hotplugin_dev_info(HOTPLUGIN_DEV_TYPE_LOCAL, &dev_info))
    {
        return false;
    }
 
    if(HOTPLUGIN_DEV_ACCESS_OK != dev_info.access)
    {
        return false;
    }

    //if(false == sys_check_media_dir(sys_get_media_dir(HOTPLUGIN_DEV_TYPE_LOCAL,DIR_TVIN), UDISK_ROOT))
    //{
    //    return false;
    //}

    return true;
}

static bool  _is_card_valid(void)
{
    hotplugin_dev_info_t  dev_info;
    
    if(false == sys_get_hotplugin_dev_info(HOTPLUGIN_DEV_TYPE_CARD, &dev_info))
    {
        return false;
    }
       
    if(HOTPLUGIN_DEV_ACCESS_OK != dev_info.access)
    {
        return false;
    }

    //if(false == sys_check_media_dir(sys_get_media_dir(HOTPLUGIN_DEV_TYPE_CARD,DIR_TVIN), CARD_ROOT))
    //{
    //    return false;
    //}

    return true;
}

static bool  _is_uhost_valid(void)
{
    hotplugin_dev_info_t  dev_info;
    if(false == sys_get_hotplugin_dev_info(HOTPLUGIN_DEV_TYPE_UHOST, &dev_info))
    {
        return false;
    }

    if(HOTPLUGIN_DEV_ACCESS_OK != dev_info.access)
    {
        return false;
    }

    //if(false == sys_check_media_dir(sys_get_media_dir(HOTPLUGIN_DEV_TYPE_UHOST,DIR_TVIN), EXTERNAL_ROOT))
    //{
    //    return false;
    //}

    return true;
}

static  menu_t* _option_savepath_init(menuview_t *menuview)
{
    scene_t* scene;
    menu_t *pMenu;
    tvin_scene_record_t *pRecord;
    string_resource_t *str_res;
    picture_resource_t *pic;
    
    menuitem_t item[TVIN_OPTION_SAVEPATH_MAX_ITEMS];
    int num;
    pMenu = pMenuPath;

    s_tvin_record = fetch_tvin_record();
    if(NULL == s_tvin_record)
    {
        return NULL;
    }
    
    pRecord = s_tvin_record;
    memset(item, 0, TVIN_OPTION_SAVEPATH_MAX_ITEMS*sizeof(menuitem_t));

    valid_path[0] = INVALID_MEMORY;
    valid_path[1] = INVALID_MEMORY;
    valid_path[2] = INVALID_MEMORY;
    valid_path[3] = INVALID_MEMORY;
    
    /* 若原来设置的磁盘不存在，则将存储路径设置为当前主盘，若主盘不存在，则保存路径进入后为空 */
    switch(s_tvin_record->tvin_config.save_path)
    {
    case INTERNAL_MEMORY:  
        if(false == _is_local_valid())
        {
            s_tvin_record->tvin_config.save_path = _is_main_valid();
        }
        break;

    case CARD_MEMORY:
        if(false == _is_card_valid())
        {
            s_tvin_record->tvin_config.save_path = _is_main_valid();
        }
        break;

    case EXTERNAL_MEMORY:
        if(false == _is_uhost_valid())
        {
            s_tvin_record->tvin_config.save_path = _is_main_valid();
        }
        break;

    default:
        /*默认情况保存路径恢复为当前主盘*/
        s_tvin_record->tvin_config.save_path = _is_main_valid();
        break;
    }
    if(s_tvin_record->tvin_config.save_path != INVALID_MEMORY)
    {
        if(true == _is_local_valid())
        {
            valid_path[0] = INTERNAL_MEMORY;
            if(true == _is_card_valid())
            {
                valid_path[1] = CARD_MEMORY;
                if(true == _is_uhost_valid())
                {
                    valid_path[2] = EXTERNAL_MEMORY;
                }  
            }
            else if(true == _is_uhost_valid())
            {
                valid_path[1] = EXTERNAL_MEMORY;
            }
            else
            {
            }            
        }
        else if(true == _is_card_valid())
        {
            valid_path[0] = CARD_MEMORY;
            if(true == _is_uhost_valid())
            {
                valid_path[1] = EXTERNAL_MEMORY;
            } 
        }
        else if(true == _is_uhost_valid())
        {
            valid_path[0] = EXTERNAL_MEMORY;
        }
        else
        {

        }   
    }            
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
                
        pic = (picture_resource_t *)get_scene_child(s_tvin_record->scene, PIC_TVIN_OPTION_DOT);
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
            num = 0;
            pMenu->callback = (menu_cb_t)_option_savepath_setmemory;
            while(INVALID_MEMORY != valid_path[num])
            {
                str_res = (string_resource_t *)get_resgroup_child(pRecord->group, \
                          tvin_option_savepath_textid[valid_path[num]]);
                if(str_res != NULL)
                {
                    item[num].text_id = (int)(str_res->str_id);
                }
                item[num].has_child = false;
                item[num].callback = (item_cb_t)_option_savepath_setmemory;

                if(s_tvin_record->tvin_config.save_path == valid_path[num])
                {                 
                    item[num].sel_icon_id = (int)(pic->pic_id);
                    item[num].unsel_icon_id = (int)(pic->pic_id);
                    last_num = num;
                }
                menu_append_item(pMenu, &item[num]);
                num++;
            }            
        }                               
        menu_set_cur_index( pMenu, 0 );//
    }    
    return pMenu;
}


bool tvin_option_savepath_update( menuview_t *menuview )
{
    string_resource_t *str_dialog_info;
    tvin_scene_record_t *tmp_pRecord;
    tmp_pRecord = fetch_tvin_record();
    if(pMenuPath == NULL)
    {
        pMenuPath = _option_savepath_init(menuview);
    }
    if ( pMenuPath == NULL )
    {
        print_err("pMenuPath is null!");
        return false;
    }
    menuview_set_menu(menuview, pMenuPath);
    menuview_paint(menuview);
    
    if(pMenuPath->total == 0)
    {
        //提示无存储器
        print_err();
        tmp_pRecord = fetch_tvin_record();
        str_dialog_info = (string_resource_t *)get_scene_child(tmp_pRecord->scene, STR_NODISK);
        if(str_dialog_info != NULL)
        {
            if(true == tvin_create_dialog(str_dialog_info->str_id, DIALOG_HEAD_WARN))
            {
                dialog_paint(tmp_pRecord->dialog);
            }
        }
    }
    return true;
}


bool tvin_option_savepath_enter( menuview_t *menuview )
{
    
    upper_level_menu = menuview->menu;
    if(NULL == upper_level_menu)
    {
        return false;
    }
    
    tvin_option_savepath_update(menuview);
    return true;
}

bool tvin_option_savepath_deinit(void)
{
    if( pMenuPath!= NULL )
    {
        menu_delete( pMenuPath );
        pMenuPath = NULL;
    }    
    return true;
}

menu_t *fetch_menu_savepath(void)
{
    return pMenuPath;
}

/*!
 *  \brief
 *  当磁盘状态有变化时，更新保存路径的内容
 */
bool update_menu_savepath(void)
{
    tvin_scene_record_t *tmp_pRecord;
    tmp_pRecord = fetch_tvin_record();
    tvin_option_savepath_deinit();
    tvin_option_savepath_update(tmp_pRecord->pRecMView);
    return true;
}

