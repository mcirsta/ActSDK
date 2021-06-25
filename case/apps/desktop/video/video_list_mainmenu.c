/*******************************************************************************
 *                              NOYA1100
 *                            Module: video
 *                 Copyright(c) 2003-2008 Actions Semiconductor,
 *                            All Rights Reserved.
 * History:
 *      <author>    <time>           <version >             <desc>
 *      shaofeng    2009-05-16          1.0             build this file
 *******************************************************************************/
/*!
 * \file     video_list_mainmenu.c
 * \brief
 * \author   shaofeng
 * \par      GENERAL DESCRIPTION:
 *
 * \par      EXTERNALIZED FUNCTIONS:
 *
 * \version 1.0
 * \date  2009/05/16
 *******************************************************************************/

#include "include_case.h"

/* include file of this application, 本应用的头文件 */
#include "include_video.h"

/* extern function, 函数声明 */
static menu_t * _mainmenu_init(void);
static bool _mainmenu_deinit(void);
static int _proc_item_nowplaying(menuview_t *menuview, menuview_sig_e signal);
static int _proc_item_dirlist(menuview_t *menuview, menuview_sig_e signal);
static int _proc_item_taglist(menuview_t *menuview, menuview_sig_e signal);
static int _proc_item_dirlist_swf(menuview_t *menuview, menuview_sig_e signal);

/* define variable, 变量定义 */
static menu_t *s_mainmenu = NULL;
static const video_menu_info_t s_mainmenu_info[] =
{
    { STR_NOWPLAYING, 0, 0 },
    { STR_DIRLIST, 0, 0 },
    { STR_TAGLIST, 0, 0 },
    { STR_FLASH_DIRLIST, 0, 0 }, 
};
/* menu items */
static menuitem_t s_mainmenu_items[] =
{
    { 0, 0, 0, false, (item_cb_t) _proc_item_nowplaying, NULL, NULL, NULL, NULL },
    { 0, 0, 0, true, (item_cb_t) _proc_item_dirlist, NULL, NULL, NULL, NULL },
    { 0, 0, 0, true, (item_cb_t) _proc_item_taglist, NULL, NULL, NULL, NULL },
    { 0, 0, 0, true, (item_cb_t) _proc_item_dirlist_swf, NULL, NULL, NULL, NULL }, 
};
/* mainmenu 的高亮index */
static int s_mainmenu_current = 0;

/* 是否有np文件, 用于是否显示np menu项 */
static int s_saved_has_np_file = -1;

#if 0
#endif
/* 进入主菜单 */
bool video_list_mainmenu_enter(menuview_t *menuview, fusion_enter_e fenter)
{
    fusion_effect_t eff;
    menu_t *cur_menu;

    if (FUSION_FORWARD == fenter)
    {
        //前进方式, 为应用级的切换, 使用FUSION_TYPE_APP_CHANGE特效
        fusion_get_config(FUSION_TYPE_APP_CHANGE, fenter, &eff);
    }
    else if (FUSION_BACKWARD == fenter)
    {
        //后退方式, 为菜单级的切换, 使用FUSION_TYPE_COMMON特效
        fusion_get_config(FUSION_TYPE_COMMON, fenter, &eff);
    }
    else
    {
    }
    fusion_set_effect(&eff);
    /* 设置此界面之前的数据, 作为特效第一帧 */
    fusion_set_first_frame(NULL);

    cur_menu = _mainmenu_init();
    menuview_set_menu(menuview, cur_menu);

    menuview_load_style(menuview, MENUVIEW_MENULIST_TEXT);
    menuview_paint(menuview);

    /* 设置此界面的数据, 作为特效最后一帧 */
    fusion_set_last_frame(NULL);

    /* 显示特效 */
    fusion_display(NULL);

    return true;
}

/* 退出主菜单 */
bool video_list_mainmenu_exit(void)
{
    //delete main menu
    _mainmenu_deinit();

    return true;
}

/******************************************************************************/
/*!
 * \par  Description:
 *       mainmenu菜单初始化, 用于显示
 * \param[in]
 * \retval       true:成功; false:失败
 *   注意:
 *       需要判断是否有np项, 用于是否显示此项
 *       需要记录当前高亮项, 用于从playing返回. 尤其有无np项时的高亮记录
 *******************************************************************************/
static menu_t * _mainmenu_init(void)
{
    menuitem_t menuitem;
    unsigned int i;
    int tmp;
    static unsigned char s_cur_sdram;

#if 0
    //根据播放列表显示np菜单项
    bool has_np_file = true;
    int np_num;
    np_num = video_plist_get_item_num( video_vars.nplist );
    if ( np_num <= 0 )
    {
        has_np_file = false;
    }
#else
#endif

    //获取内存大小
    s_cur_sdram = get_sdram_cap();
    print_dbg("SDRAM size:%d\n", s_cur_sdram);

    //initial menu
    if (s_mainmenu == NULL)
    {
        //create menu
        s_mainmenu = menu_create();
        if (NULL == s_mainmenu)
        {
            print_err("menu_create failed");
            return NULL;
        }
        video_load_menuitem(MAINMENU_RESGROUP, s_mainmenu_info, sizeof(s_mainmenu_items) / sizeof(menuitem_t),
                s_mainmenu_items);

        //init items
        memset(&menuitem, 0, sizeof(menuitem_t));
        if (1 == video_vars.flag_has_np_file)
        {
            //有np项, 则将np项添加到mainmenu菜单
            i = 0;
            //之前没有np, 现在有np, 则高亮选中条需要下移一个. 用于之前没有np, 后来有np的情况
            if (0 == s_saved_has_np_file)
            {
                s_mainmenu_current++;
            }
        }
        else
        {
            //没有np项, 则不将np项添加到mainmenu菜单
            i = 1;
            //之前有np, 现在没有np, 则高亮选中条需要上移一个.
            if (1 == s_saved_has_np_file)
            {
                s_mainmenu_current--;
            }
        }
        //只有内存大于8M才支持swf
        if ((1 == video_vars.swf_support) && (s_cur_sdram > 8))
        {
            for (; i < (sizeof(s_mainmenu_items) / sizeof(menuitem_t)); i++)
            {
                menuitem.text_id = s_mainmenu_items[i].text_id;
                menuitem.has_child = s_mainmenu_items[i].has_child;
                menuitem.callback = s_mainmenu_items[i].callback;

                menu_append_item(s_mainmenu, &menuitem);
            }
        }
        else
        {
            for (; i < ((sizeof(s_mainmenu_items) / sizeof(menuitem_t)) - 1); i++)
            {
                menuitem.text_id = s_mainmenu_items[i].text_id;
                menuitem.has_child = s_mainmenu_items[i].has_child;
                menuitem.callback = s_mainmenu_items[i].callback;

                menu_append_item(s_mainmenu, &menuitem);
            }
        }

        //init current index, 初始化高亮index
        tmp = menu_get_total(s_mainmenu);
        //如果当前高亮index >= 菜单总数, 则index减少一. 用于np项消失的情况
        if (s_mainmenu_current >= tmp)
        {
            s_mainmenu_current--;
        }
        if (s_mainmenu_current < 0)
        {
            s_mainmenu_current = 0;
        }
        menu_set_cur_index(s_mainmenu, s_mainmenu_current);
    }

    s_saved_has_np_file = ( int )video_vars.flag_has_np_file;

    return s_mainmenu;
}

static bool _mainmenu_deinit(void)
{
    //delete menu
    if (NULL == s_mainmenu)
    {
        return false;
    }

    //保存高亮index, save highlight index
    s_mainmenu_current = menu_get_global_offset(s_mainmenu) + menu_get_cur_index(s_mainmenu);
    if (s_mainmenu_current < 0)
    {
        s_mainmenu_current = 0;
    }

    menu_delete(s_mainmenu);
    s_mainmenu = NULL;

    return true;
}

/******************************************************************************/
/*!
 * \par  Description:
 *       处理np项菜单的进入退出操作
 * \param[in]
 * \retval
 *   注意:
 *******************************************************************************/
static int _proc_item_nowplaying(menuview_t *menuview, menuview_sig_e signal)
{
    bool result = true;
    char fullpath[PATH_MAX] =
    { 0 };

    switch (signal)
    {
        case WIDGET_SIG_MENUVIEW_OK:
        //退出list scene前, 清除退出时menu的断点入口函数
        video_list_widget_menu_exit(menuview, NULL);

        strcpy( fullpath, video_vars.plist.np_fullpath );
        video_nplist_init( fullpath );
        video_nplist_get_index_by_fullpath( video_vars.plist.np_fullpath, &video_vars.plist.np_index );       
        print_dbg("\nfull path: %s\nindex: %d\n",fullpath,video_vars.plist.np_index);
        
        //找到文件后缀, 并与swf比较, 选择进入相应的播放场景
        result = is_swf_ext(fullpath);
        if (true == result)
        {
            video_vars.scene_list.app_scene.scene_type = SCENE_SWF;
        }
        else
        {
            video_vars.scene_list.app_scene.scene_type = SCENE_PLAYING;
        }

        exit_msg_loop();
        break;

        case WIDGET_SIG_MENUVIEW_QUIT:
        video_list_mainmenu_exit();

        video_vars.scene_list.app_scene.scene_type = SCENE_EXIT;
        exit_msg_loop();
        break;

        default:
        break;
    }

    return 1;
}

/******************************************************************************/
/*!
 * \par  Description:
 *       处理dirlist项菜单的进入退出操作
 * \param[in]
 * \retval
 *   注意:
 *******************************************************************************/
static int _proc_item_dirlist(menuview_t *menuview, menuview_sig_e signal)
{
    bool result = true;

    switch (signal)
    {
        case WIDGET_SIG_MENUVIEW_OK:
        result = video_list_dirlist_create(menuview, FUSION_FORWARD);
        //进入失败, 退回到mainmenu
        if (false == result)
        {
            video_list_mainmenu_enter(menuview, FUSION_BACKWARD);
            break;
        }
        break;

        case WIDGET_SIG_MENUVIEW_QUIT:
        video_list_mainmenu_exit();

        video_vars.scene_list.app_scene.scene_type = SCENE_EXIT;
        exit_msg_loop();
        break;

        default:
        break;
    }

    return 1;
}

/******************************************************************************/
/*!
 * \par  Description:
 *       处理taglist项菜单的进入退出操作
 * \param[in]
 * \retval
 *   注意:
 *******************************************************************************/
static int _proc_item_taglist(menuview_t *menuview, menuview_sig_e signal)
{
    bool result = true;

    switch (signal)
    {
        case WIDGET_SIG_MENUVIEW_OK:
        result = video_list_taglist_enter(menuview, FUSION_FORWARD);
        //进入失败, 退回到mainmenu
        if (false == result)
        {
            video_list_mainmenu_enter(menuview, FUSION_BACKWARD);
        }
        break;
        case WIDGET_SIG_MENUVIEW_QUIT:
        video_list_mainmenu_exit();

        video_vars.scene_list.app_scene.scene_type = SCENE_EXIT;
        exit_msg_loop();
        break;

        default:
        break;
    }

    return 1;
}

/******************************************************************************/
/*!
 * \par  Description:
 *       处理dirlist_swf项菜单的进入退出操作
 * \param[in]
 * \retval
 *   注意:
 *******************************************************************************/
static int _proc_item_dirlist_swf(menuview_t *menuview, menuview_sig_e signal)
{
    bool result = true;

    switch (signal)
    {
        case WIDGET_SIG_MENUVIEW_OK:
        result = video_list_dirlist_swf_create(menuview, FUSION_FORWARD);
        //进入失败, 退回到mainmenu
        if (false == result)
        {
            video_list_mainmenu_enter(menuview, FUSION_BACKWARD);
            break;
        }
        //shaofeng shaofeng 播放时还是dirlist时, 加载swf中间件
        break;

        case WIDGET_SIG_MENUVIEW_QUIT:
        video_list_mainmenu_exit();

        video_vars.scene_list.app_scene.scene_type = SCENE_EXIT;
        exit_msg_loop();
        break;

        default:
        break;
    }

    return 1;
}

