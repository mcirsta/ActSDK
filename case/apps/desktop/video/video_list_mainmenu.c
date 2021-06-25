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

/* include file of this application, ��Ӧ�õ�ͷ�ļ� */
#include "include_video.h"

/* extern function, �������� */
static menu_t * _mainmenu_init(void);
static bool _mainmenu_deinit(void);
static int _proc_item_nowplaying(menuview_t *menuview, menuview_sig_e signal);
static int _proc_item_dirlist(menuview_t *menuview, menuview_sig_e signal);
static int _proc_item_taglist(menuview_t *menuview, menuview_sig_e signal);
static int _proc_item_dirlist_swf(menuview_t *menuview, menuview_sig_e signal);

/* define variable, �������� */
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
/* mainmenu �ĸ���index */
static int s_mainmenu_current = 0;

/* �Ƿ���np�ļ�, �����Ƿ���ʾnp menu�� */
static int s_saved_has_np_file = -1;

#if 0
#endif
/* �������˵� */
bool video_list_mainmenu_enter(menuview_t *menuview, fusion_enter_e fenter)
{
    fusion_effect_t eff;
    menu_t *cur_menu;

    if (FUSION_FORWARD == fenter)
    {
        //ǰ����ʽ, ΪӦ�ü����л�, ʹ��FUSION_TYPE_APP_CHANGE��Ч
        fusion_get_config(FUSION_TYPE_APP_CHANGE, fenter, &eff);
    }
    else if (FUSION_BACKWARD == fenter)
    {
        //���˷�ʽ, Ϊ�˵������л�, ʹ��FUSION_TYPE_COMMON��Ч
        fusion_get_config(FUSION_TYPE_COMMON, fenter, &eff);
    }
    else
    {
    }
    fusion_set_effect(&eff);
    /* ���ô˽���֮ǰ������, ��Ϊ��Ч��һ֡ */
    fusion_set_first_frame(NULL);

    cur_menu = _mainmenu_init();
    menuview_set_menu(menuview, cur_menu);

    menuview_load_style(menuview, MENUVIEW_MENULIST_TEXT);
    menuview_paint(menuview);

    /* ���ô˽��������, ��Ϊ��Ч���һ֡ */
    fusion_set_last_frame(NULL);

    /* ��ʾ��Ч */
    fusion_display(NULL);

    return true;
}

/* �˳����˵� */
bool video_list_mainmenu_exit(void)
{
    //delete main menu
    _mainmenu_deinit();

    return true;
}

/******************************************************************************/
/*!
 * \par  Description:
 *       mainmenu�˵���ʼ��, ������ʾ
 * \param[in]
 * \retval       true:�ɹ�; false:ʧ��
 *   ע��:
 *       ��Ҫ�ж��Ƿ���np��, �����Ƿ���ʾ����
 *       ��Ҫ��¼��ǰ������, ���ڴ�playing����. ��������np��ʱ�ĸ�����¼
 *******************************************************************************/
static menu_t * _mainmenu_init(void)
{
    menuitem_t menuitem;
    unsigned int i;
    int tmp;
    static unsigned char s_cur_sdram;

#if 0
    //���ݲ����б���ʾnp�˵���
    bool has_np_file = true;
    int np_num;
    np_num = video_plist_get_item_num( video_vars.nplist );
    if ( np_num <= 0 )
    {
        has_np_file = false;
    }
#else
#endif

    //��ȡ�ڴ��С
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
            //��np��, ��np����ӵ�mainmenu�˵�
            i = 0;
            //֮ǰû��np, ������np, �����ѡ������Ҫ����һ��. ����֮ǰû��np, ������np�����
            if (0 == s_saved_has_np_file)
            {
                s_mainmenu_current++;
            }
        }
        else
        {
            //û��np��, �򲻽�np����ӵ�mainmenu�˵�
            i = 1;
            //֮ǰ��np, ����û��np, �����ѡ������Ҫ����һ��.
            if (1 == s_saved_has_np_file)
            {
                s_mainmenu_current--;
            }
        }
        //ֻ���ڴ����8M��֧��swf
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

        //init current index, ��ʼ������index
        tmp = menu_get_total(s_mainmenu);
        //�����ǰ����index >= �˵�����, ��index����һ. ����np����ʧ�����
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

    //�������index, save highlight index
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
 *       ����np��˵��Ľ����˳�����
 * \param[in]
 * \retval
 *   ע��:
 *******************************************************************************/
static int _proc_item_nowplaying(menuview_t *menuview, menuview_sig_e signal)
{
    bool result = true;
    char fullpath[PATH_MAX] =
    { 0 };

    switch (signal)
    {
        case WIDGET_SIG_MENUVIEW_OK:
        //�˳�list sceneǰ, ����˳�ʱmenu�Ķϵ���ں���
        video_list_widget_menu_exit(menuview, NULL);

        strcpy( fullpath, video_vars.plist.np_fullpath );
        video_nplist_init( fullpath );
        video_nplist_get_index_by_fullpath( video_vars.plist.np_fullpath, &video_vars.plist.np_index );       
        print_dbg("\nfull path: %s\nindex: %d\n",fullpath,video_vars.plist.np_index);
        
        //�ҵ��ļ���׺, ����swf�Ƚ�, ѡ�������Ӧ�Ĳ��ų���
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
 *       ����dirlist��˵��Ľ����˳�����
 * \param[in]
 * \retval
 *   ע��:
 *******************************************************************************/
static int _proc_item_dirlist(menuview_t *menuview, menuview_sig_e signal)
{
    bool result = true;

    switch (signal)
    {
        case WIDGET_SIG_MENUVIEW_OK:
        result = video_list_dirlist_create(menuview, FUSION_FORWARD);
        //����ʧ��, �˻ص�mainmenu
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
 *       ����taglist��˵��Ľ����˳�����
 * \param[in]
 * \retval
 *   ע��:
 *******************************************************************************/
static int _proc_item_taglist(menuview_t *menuview, menuview_sig_e signal)
{
    bool result = true;

    switch (signal)
    {
        case WIDGET_SIG_MENUVIEW_OK:
        result = video_list_taglist_enter(menuview, FUSION_FORWARD);
        //����ʧ��, �˻ص�mainmenu
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
 *       ����dirlist_swf��˵��Ľ����˳�����
 * \param[in]
 * \retval
 *   ע��:
 *******************************************************************************/
static int _proc_item_dirlist_swf(menuview_t *menuview, menuview_sig_e signal)
{
    bool result = true;

    switch (signal)
    {
        case WIDGET_SIG_MENUVIEW_OK:
        result = video_list_dirlist_swf_create(menuview, FUSION_FORWARD);
        //����ʧ��, �˻ص�mainmenu
        if (false == result)
        {
            video_list_mainmenu_enter(menuview, FUSION_BACKWARD);
            break;
        }
        //shaofeng shaofeng ����ʱ����dirlistʱ, ����swf�м��
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

